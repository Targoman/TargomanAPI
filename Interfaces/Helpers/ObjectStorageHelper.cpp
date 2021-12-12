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
#include "ObjectStorageHelper.h"
#include "ORM/ObjectStorage.h"
using namespace Targoman::API::ORM;

#include "Interfaces/Helpers/SecurityHelper.h"
using namespace Targoman::API::Helpers;

#include <fstream>
//#include <aws/core/utils/memory/stl/AWSStreamFwd.h>
#include <aws/core/Aws.h>
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

TAPI_REGISTER_METATYPE(
    /* complexity      */ COMPLEXITY_Object,
    /* namespace       */ Targoman::API::Helpers,
    /* type            */ stuSaveFileResult,
    /* toVariantLambda */ [](const stuSaveFileResult& _value) -> QVariant { return _value.toJson().toVariantMap(); }
);

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
    QDir(FullPath).mkpath(".");

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
        FileType = _file.Name.mid(Idx + 2).toLower();

    //save to tblUploadFiles
    TAPI::ORMFields_t CreateInfo({
        { tblUploadFiles::uflFileName, _file.Name },
        { tblUploadFiles::uflSize, _file.Size },
        { tblUploadFiles::uflMimeType, MimeType },
        { tblUploadFiles::uflLocalFullFileName, FullFileName },
    });
    quint64 UploadedFileID = _uploadFiles.Create(_uploadFiles, _currentUserID, CreateInfo);

    //find matched uploadGateways
    SelectQuery QueryMatched = SelectQuery(_uploadGateways)
            .addCol(tblUploadGateways::ugwID)
            .andWhere(clsCondition({ tblUploadGateways::ugwAllowedFileTypes, enuConditionOperator::Null })
                .orCond({ { enuAggregation::LOWER, tblUploadGateways::ugwAllowedFileTypes }, enuConditionOperator::Like, FileType }))
            .andWhere(clsCondition({ tblUploadGateways::ugwAllowedMimeTypes, enuConditionOperator::Null })
                .orCond({ { enuAggregation::LOWER, tblUploadGateways::ugwAllowedMimeTypes }, enuConditionOperator::Like, MimeType }))
            .andWhere(clsCondition({ tblUploadGateways::ugwAllowedMinFileSize, enuConditionOperator::Null })
                .orCond({ tblUploadGateways::ugwAllowedMinFileSize, enuConditionOperator::LessEqual, _file.Size }))
            .andWhere(clsCondition({ tblUploadGateways::ugwAllowedMaxFileSize, enuConditionOperator::Null })
                .orCond({ tblUploadGateways::ugwAllowedMaxFileSize, enuConditionOperator::GreaterEqual, _file.Size }))

                               AND ugwMaxFilesCount IS NULL
                               OR ugwMaxFilesCount < (ugwCreatedFilesCount - ugwDeletedFilesCount)

                               AND ugwMaxFilesSize IS NULL
                               OR ugwMaxFilesSize <= (ugwCreatedFilesSize - ugwDeletedFilesSize + _file.Size)


tblUploadGateways::ugwMaxFilesCount
tblUploadGateways::ugwMaxFilesSize

tblUploadGateways::ugwCreatedFilesCount
tblUploadGateways::ugwCreatedFilesSize
tblUploadGateways::ugwDeletedFilesCount
tblUploadGateways::ugwDeletedFilesSize

            ;


    //create tblUploadQueue (queue for upload to gateway)

    //trigger async upload to s3(s)



    return UploadedFileID;









    QString FullFileUrl = _objectStorageConfigs.Bucket;
    FullFileUrl += ".s3";
    if (_objectStorageConfigs.Region.isEmpty() == false)
        FullFileUrl += QString(".%1").arg(_objectStorageConfigs.Region);
    switch (_objectStorageConfigs.Provider)
    {
        case Targoman::API::ORM::enuS3Provider::Amazon:
            FullFileUrl += ".amazonaws.com";
            break;

        ///todo: domain for arvan s3
        case Targoman::API::ORM::enuS3Provider::Arvan:
            FullFileUrl += ".arvan_____XYZ.com";
            break;

        default:
            break;
    }
    FullFileUrl = FullFileUrl.toLower();
    FullFileUrl += QString("/%1").arg(_fileName);

    //----------------------------------------
    QVariant OldData = _uploadFiles.Select(_uploadFiles,
                                      /* currentUserID */ _currentUserID,
                                      /* pksByPath     */ {},
                                      /* offset        */ 0,
                                      /* limit         */ 10,
                                      /* cols          */ {},
                                      /* filters       */ QString("%1=%2").arg(tblUploadFiles::uplURL).arg(FullFileUrl.replace(" ", "$SPACE$")),
                                      /* orderBy       */ {},
                                      /* groupBy       */ {},
                                      /* reportCount   */ false
                                      );
    if (OldData.isValid() && (OldData.toList().length() > 0))
//    if (QFile::exists(FullFileName))
        throw exTargomanBase("A file already exists with the same name.", ESTATUS_CONFLICT);

    QString FullPath = QString("/tmp/TAPI/%1").arg(_uploadFiles->schema());
    QDir(FullPath).mkpath(".");

    QFile File(FullFileName);
    if (File.open(QFile::WriteOnly) == false)
        throw exTargomanBase("Could not create temp file.", ESTATUS_CONFLICT);
    File.write(Content);
    File.close();

    stuSaveFileResult SaveFileResult;
    SaveFileResult.FullFileUrl = FullFileUrl;
    SaveFileResult.MimeType = MimeType;
    QString SaveErrorMessage = "";

    try
    {
        SaveFileResult.IsUploaded = ObjectStorageHelper::uploadFileToS3(
            _fileName,
            FullFileName,
            _objectStorageConfigs.Bucket,
            _objectStorageConfigs.Region
        );
    }
    catch (const std::exception &exp)
    {
        SaveErrorMessage = exp.what();
        SaveFileResult.IsUploaded = false;
    }

    if (SaveFileResult.IsUploaded)
        QFile::remove(FullFileName);

    //------------------------------------------
    TAPI::ORMFields_t CreateInfo({
        { tblUploadFiles::uplURL, FullFileUrl },
        { tblUploadFiles::uplFileName, _fileName },
        { tblUploadFiles::uplSize, Content.length() },
        { tblUploadFiles::uplMimeType, MimeType },
        { tblUploadFiles::uplTempFullFileName, FullFileName },
        { tblUploadFiles::uplStatus, SaveFileResult.IsUploaded
          ? enuUploadFileStatus::Uploaded
          : enuUploadFileStatus::Queued },
    });

    if (SaveErrorMessage.isEmpty() == false)
        CreateInfo.insert(tblUploadFiles::uplUploadLastErrorMessage, SaveErrorMessage);

    SaveFileResult.uplID = _uploadFiles->Create(*_uploadFiles, _currentUserID, CreateInfo);

    return SaveFileResult;
}

bool ObjectStorageHelper::uploadFileToS3(
        const QString &_fileName,
        const QString &_fullFileName,
        const QString &_bucket,
        const QString &_region
        )
{
#ifdef AAAAAAAAAAAAAA
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
    Aws::Client::ClientConfiguration S3ClientConfig;
    if (_region.isEmpty() == false)
        S3ClientConfig.region = _region.toStdString();

    S3ClientConfig.endpointOverride

    S3::S3Client S3Client(S3ClientConfig);

    Aws::S3::Model::PutObjectOutcome Outcome = S3Client.PutObject(Request);

    if (Outcome.IsSuccess() == false)
        throw exTargomanBase(QString("Could not save file to the s3 server: %1").arg(Outcome.GetError().GetMessage().c_str()), ESTATUS_REQUEST_TIMEOUT);

    return true;
#endif
}

} //namespace Targoman::API::Helpers
