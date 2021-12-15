/******************************************************************************
#   TargomanAPI: REST API for Targoman
#
#   Copyright 2014-2020 by Targoman Intelligent Processing <http://tip.co.ir>
#
#   TargomanAPI is free software: you can redistribute it and/or modify
#   it under the terms of the GNU AFFERO GENERAL PUBLIC LICENSE as published by
#   the Free Software Foundation, either version 3 of the License, or
#   (at your option) any later version.
#
#   TargomanAPI is distributed in the hope that it will be useful,
#   but WITHOUT ANY WARRANTY; without even the implied warranty of
#   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#   GNU AFFERO GENERAL PUBLIC LICENSE for more details.
#
#   You should have received a copy of the GNU AFFERO GENERAL PUBLIC LICENSE
#   along with Targoman. If not, see <http://www.gnu.org/licenses/>.
 ******************************************************************************/
/**
 * @author S. Mehran M. Ziabary <ziabary@targoman.com>
 * @author Kambiz Zandi <kambizzandi@gmail.com>
 */

#include <QUuid>
#include <QMimeDatabase>
#include <QtConcurrent/QtConcurrent>
#include "ObjectStorageHelper.h"
#include "ORM/ObjectStorage.h"
using namespace Targoman::API::ORM;

#include "Interfaces/Helpers/SecurityHelper.h"
using namespace Targoman::API::Helpers;

#include <fstream>
//#include <aws/core/utils/memory/stl/AWSStreamFwd.h>
#include <aws/core/Aws.h>
//#include <aws/core/auth/AWSAuthSigner.h>
#include <aws/core/auth/AWSCredentials.h>
#include <aws/s3/S3Client.h>
#include <aws/s3/model/PutObjectRequest.h>
using namespace Aws;
#include "QHttp/qhttpfwd.hpp"
using namespace qhttp;

/** installing AmazonAWS S3 from source ************************************************************
git clone --recurse-submodules https://github.com/aws/aws-sdk-cpp
mkdir aws-build.s3
cd aws-build.s3
cmake ../aws-sdk-cpp -DCMAKE_BUILD_TYPE=Release -DCMAKE_INSTALL_PREFIX=/usr -DCMAKE_PREFIX_PATH=/usr -DBUILD_ONLY="s3"
make
sudo make install
*/

//TAPI_REGISTER_METATYPE(
//    /* complexity      */ COMPLEXITY_Object,
//    /* namespace       */ Targoman::API::Helpers,
//    /* type            */ stuSaveFileResult,
//    /* toVariantLambda */ [](const stuSaveFileResult& _value) -> QVariant { return _value.toJson().toVariantMap(); }
//);

namespace Targoman::API::Helpers {

QVariantMap ObjectStorageHelper::saveFiles(
        intfUploadFiles &_uploadFiles,
        intfUploadGateways &_uploadGateways,
        intfUploadQueue &_uploadQueue,
        const quint64 _currentUserID,
        const TAPI::Files_t &_files
    )
{
    QVariantMap Result;
    foreach(auto _file, _files)
    {
        try
        {
            quint64 ID = saveFile(
                             _uploadFiles,
                             _uploadGateways,
                             _uploadQueue,
                             _currentUserID,
                             _file
                             );
            Result.insert(_file.Name, ID);
        }
        catch (std::exception &exp)
        {
            Result.insert(_file.Name, exp.what());
        }
    }
    return Result;
}

quint64 ObjectStorageHelper::saveFile(
        intfUploadFiles &_uploadFiles,
        intfUploadGateways &_uploadGateways,
        intfUploadQueue &_uploadQueue,
        const quint64 _currentUserID,
        const TAPI::stuFileInfo &_file
    )
{
    Targoman::API::ORM::intfUploadFiles::stuObjectStorageConfigs _objectStorageConfigs = _uploadFiles.getObjectStorageConfigs();

    //save file to the _objectStorageConfigs.LocalStoragePath (LOCAL/NFS)
    if (_file.Size == 0)
        throw exTargomanBase("The file is empty.", ESTATUS_BAD_REQUEST);

    if ((_file.Name.indexOf('/') >= 0) || (_file.Name.indexOf('\\') >= 0))
        throw exTargomanBase("Invalid character in file name.", ESTATUS_BAD_REQUEST);

    //check existance
    QVariant OldData = _uploadFiles.Select(_uploadFiles,
                                      /* currentUserID */ _currentUserID,
                                      /* pksByPath     */ {},
                                      /* offset        */ 0,
                                      /* limit         */ 10,
                                      /* cols          */ {},
                                      /* filters       */ QString("%1=%2").arg(tblUploadFiles::uflFileName).arg(QString(_file.Name).replace(" ", "$SPACE$")),
                                      /* orderBy       */ {},
                                      /* groupBy       */ {},
                                      /* reportCount   */ false
                                      );
    if (OldData.isValid() && (OldData.toList().length() > 0))
        throw exTargomanBase("A file already exists with the same name.", ESTATUS_CONFLICT);

    //move from temp to persistance location
    QString FullPath = _objectStorageConfigs.LocalStoragePath;
    QDir FullPathDir(FullPath);
    if (FullPathDir.exists() == false)
    {
        if (FullPathDir.mkpath(".") == false)
            throw exTargomanBase("Could not create storage folder.", ESTATUS_INTERNAL_SERVER_ERROR);
    }

//    QString FileUUID = SecurityHelper::UUIDtoMD5();
    QString FileUUID = QUuid::createUuid().toString(QUuid::Id128);

    QString FullFileName = QString("%1/%2").arg(FullPath).arg(FileUUID);

    QFile::rename(_file.TempName, FullFileName);

    //get mime type
    QString MimeType = _file.Mime;
    if (MimeType.isEmpty())
    {
        QMimeDatabase MimeDB;
        QString MimeType = MimeDB.mimeTypeForFile(FullFileName).name().toLower();
    }

    //get file extension
    QString FileType;
    int Idx = _file.Name.lastIndexOf('.');
    if (Idx >= 0)
        FileType = _file.Name.mid(Idx + 1).toLower();

    //save to tblUploadFiles
    quint64 UploadedFileID;
    quint16 QueueRowsCount;
    try
    {
        QVariantMap SpOutVars = _uploadFiles.callSP("sp_CREATE_uploadedFile", {
                { "iFileName", _file.Name },
                { "iFileSize", _file.Size },
                { "iFileType", FileType },
                { "iMimeType", MimeType },
                { "iFullFileName", FullFileName },
                { "iCreatorUserID", _currentUserID },
                { "iQueueStatus", QChar(enuUploadQueueStatus::Uploading) }, //this means that cronized s3 process not act to this queue items
            })
            .spDirectOutputs();

        UploadedFileID = SpOutVars.value("oUploadedFileID").toULongLong();
        QueueRowsCount = SpOutVars.value("oQueueRowsCount").toUInt();
    }
    catch (std::exception &exp)
    {
        TargomanDebug(5, "ERROR: sp_CREATE_uploadedFile:" << exp.what());
        throw;
    }

    //trigger async upload to s3(s)
    try
    {
        if (QueueRowsCount > 0)
        {
            QFuture<bool> ret = QtConcurrent::run(
                                    ObjectStorageHelper::processQueue,
                                    stuProcessQueueParams(
                                        _currentUserID,
                                        _uploadFiles,
                                        _uploadGateways,
                                        _uploadQueue,
                                        UploadedFileID,
                                        QueueRowsCount
                                    ));

#ifdef QT_DEBUG
//            bool rrr = ret.result();
#endif
        }
    }
    catch (std::exception &exp)
    {
        TargomanDebug(5, "ERROR: concurrent run of upload file queue:" << exp.what());

        //convert iQueueStatus to New for post processing by cron
        UpdateQuery(_uploadQueue)
            .set(tblUploadQueue::uquStatus, enuUploadQueueStatus::New)
            .where({ tblUploadQueue::uqu_uflID, enuConditionOperator::Equal, UploadedFileID })
            .execute(_currentUserID)
        ;
    }

    return UploadedFileID;
}

bool ObjectStorageHelper::processQueue(const stuProcessQueueParams &_processQueueParams)
{
    TargomanDebug(5, "ObjectStorageHelper::processQueue:" << _processQueueParams.UploadedFileID);

    _processQueueParams.UploadQueue.prepareFiltersList();

    SelectQuery Query = SelectQuery(_processQueueParams.UploadQueue)
        .addCol(tblUploadQueue::uquID)
        .addCol(tblUploadQueue::uqu_uflID)
        .addCol(tblUploadQueue::uqu_ugwID)
        .addCol(tblUploadQueue::uquStatus)
        .addCol(tblUploadFiles::uflID)
        .addCol(tblUploadFiles::uflFileName)
        .addCol(tblUploadFiles::uflSize)
        .addCol(tblUploadFiles::uflFileType)
        .addCol(tblUploadFiles::uflMimeType)
        .addCol(tblUploadFiles::uflLocalFullFileName)
        .addCol(tblUploadFiles::uflStatus)
        .addCol(tblUploadGateways::ugwID)
        .addCol(tblUploadGateways::ugwBucket)
        .addCol(tblUploadGateways::ugwEndpointUrl)
        .addCol(tblUploadGateways::ugwSecretKey)
        .addCol(tblUploadGateways::ugwAccessKey)
        .addCol(tblUploadGateways::ugwMetaInfo)
        .addCol(tblUploadGateways::ugwCreatedFilesCount)
        .addCol(tblUploadGateways::ugwCreatedFilesSize)
        .addCol(tblUploadGateways::ugwLastActionTime)
        .addCol(tblUploadGateways::ugwStatus)

        .innerJoinWith(tblUploadQueue::Relation::File)
        .innerJoinWith(tblUploadQueue::Relation::Gateway)
    ;

    if (_processQueueParams.UploadedFileID > 0)
        Query.andWhere({ tblUploadQueue::uqu_uflID, enuConditionOperator::Equal, _processQueueParams.UploadedFileID })
             .andWhere({ tblUploadQueue::uquStatus, enuConditionOperator::In,
                         "'" + QStringList({ QChar(enuUploadQueueStatus::New), QChar(enuUploadQueueStatus::Uploading) }).join("','") + "'" })
        ;
    else
        Query.andWhere({ tblUploadQueue::uquStatus, enuConditionOperator::Equal, enuUploadQueueStatus::New });

    QVariantList QueueItems;

    try
    {
        QueueItems = Query.all({}, _processQueueParams.MaxItemsCount);
    }
    catch (std::exception &exp)
    {
        TargomanDebug(5, "ERROR: fetching upload queue items:" << exp.what());
    }

    if (QueueItems.isEmpty())
    {
        if (_processQueueParams.UploadedFileID > 0)
            UpdateQuery(_processQueueParams.UploadFiles)
                    .setPksByPath(_processQueueParams.UploadedFileID)
                    .set(tblUploadFiles::uflStatus, enuUploadFileStatus::New)
                    .execute(_processQueueParams.CurrentUserID);

        return false;
    }

    //update Queue Status to Uploading
    QStringList UploadingQueueIDs;
    foreach(QVariant Var, QueueItems)
    {
        Targoman::API::ORM::Private::stuProcessUploadQueueInfo Info;
        Info.fromVariantMap(Var.toMap());

        if (Info.uquStatus == enuUploadQueueStatus::New)
            UploadingQueueIDs.append(QString::number(Info.uquID));
    }
    if (UploadingQueueIDs.length())
    {
        UpdateQuery(_processQueueParams.UploadQueue)
                .set(tblUploadQueue::uquStatus, enuUploadQueueStatus::Uploading)
                .where({ tblUploadQueue::uquID, enuConditionOperator::In, UploadingQueueIDs.join(",") })
                .execute(_processQueueParams.CurrentUserID);
    }

    //------------------------------
    QHash<quint32, quint32> GatewayUploadedFileCount;
    QHash<quint32, quint64> GatewayUploadedFileSize;
//    QStringList UploadedFileIDs;
    QStringList UploadedQueueIDs;
//    QStringList FailedFileIDs;
    QStringList FailedQueueIDs;

    foreach(QVariant Var, QueueItems)
    {
        Targoman::API::ORM::Private::stuProcessUploadQueueInfo Info;
        Info.fromVariantMap(Var.toMap());

        bool S3Stored = false;
        try
        {
            S3Stored = ObjectStorageHelper::uploadFileToS3(
                           Info.uflFileName,
                           Info.uflLocalFullFileName,
                           Info.ugwBucket,
                           Info.ugwEndpointUrl,
                           Info.ugwSecretKey,
                           Info.ugwAccessKey
                           );
        }
        catch (std::exception &exp)
        {
            TargomanDebug(5, "ERROR: storing file to s3:" << exp.what());
        }

        if (S3Stored)
        {
            if (GatewayUploadedFileCount.contains(Info.ugwID))
            {
                GatewayUploadedFileCount[Info.ugwID] += 1;
                GatewayUploadedFileSize[Info.ugwID] += Info.uflSize;
            }
            else
            {
                GatewayUploadedFileCount.insert(Info.ugwID, 1);
                GatewayUploadedFileSize.insert(Info.ugwID, Info.uflSize);
            }

//            QList<quint64> UploadedFileIDs;
            UploadedQueueIDs.append(QString::number(Info.uquID));
        }
        else
        {
//            QList<quint64> FailedFileIDs;
            FailedQueueIDs.append(QString::number(Info.uquID));
        }
    }

    //update gateway ststictics
    if (GatewayUploadedFileCount.count())
    {
        foreach(quint64 GatewayID, GatewayUploadedFileCount.keys())
        {
            UpdateQuery(_processQueueParams.UploadGateways)
                    .increament(tblUploadGateways::ugwCreatedFilesCount, GatewayUploadedFileCount[GatewayID])
                    .increament(tblUploadGateways::ugwCreatedFilesSize, GatewayUploadedFileSize[GatewayID])
                    .setPksByPath(GatewayID)
                    .execute(_processQueueParams.CurrentUserID)
                    ;
        }
    }

    //update queue
    if (UploadedQueueIDs.length())
    {
        UpdateQuery(_processQueueParams.UploadQueue)
                .set(tblUploadQueue::uquStatus, enuUploadQueueStatus::Stored)
                .where({ tblUploadQueue::uquID, enuConditionOperator::In, UploadedQueueIDs.join(',') })
                .execute(_processQueueParams.CurrentUserID);
                ;
    }
    if (FailedQueueIDs.length())
    {
        UpdateQuery(_processQueueParams.UploadQueue)
                .set(tblUploadQueue::uquStatus, enuUploadQueueStatus::New)
                .where({ tblUploadQueue::uquID, enuConditionOperator::In, FailedQueueIDs.join(',') })
                .execute(_processQueueParams.CurrentUserID);
                ;
    }

    ///TODO: re compute upload file status by queue items status

    //------------------------------------------------
    return true;
}

bool ObjectStorageHelper::uploadFileToS3(
        const QString &_fileName,
        const QString &_fullFileName,
        const QString &_bucket,
        const QString &_endpointUrl,
        const QString &_secretKey,
        const QString &_accessKey
        )
{
    ///TODO: use MultipartUploader for files larger than 400 MB


    //upload to s3
    S3::Model::PutObjectRequest Request;

    Request.SetBucket(_bucket.toStdString());
    Request.SetKey(_fileName.toStdString());

    //----------------------------------------
    std::shared_ptr<Aws::IOStream> InputData =
            Aws::MakeShared<Aws::FStream>("SampleAllocationTag",
                                          _fullFileName.toStdString().c_str(),
                                          std::ios_base::in | std::ios_base::binary);

    Request.SetBody(InputData);

    //----------------------------------------
    Aws::Auth::AWSCredentials AWSCredentials(_accessKey.toStdString(), _secretKey.toStdString());

    Aws::Client::ClientConfiguration S3ClientConfig;

//    if (_region.isEmpty() == false)
//        S3ClientConfig.region = _region.toStdString();

    S3ClientConfig.endpointOverride = _endpointUrl.toStdString();

    S3::S3Client S3Client(AWSCredentials, S3ClientConfig);

    Aws::S3::Model::PutObjectOutcome Outcome = S3Client.PutObject(Request);

    if (Outcome.IsSuccess() == false)
        throw exTargomanBase(QString("Could not save file to the s3 server: %1").arg(Outcome.GetError().GetMessage().c_str()), ESTATUS_REQUEST_TIMEOUT);

    return true;
}

} //namespace Targoman::API::Helpers
