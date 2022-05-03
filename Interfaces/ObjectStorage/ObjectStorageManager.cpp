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
#include "ObjectStorageManager.h"

#include "Interfaces/Helpers/SecurityHelper.h"
using namespace Targoman::API::Helpers;

//#include <fstream>

#include "Gateways/gtwNFS.h"
#ifdef TARGOMAN_API_ENABLE_AWS_S3
#include "Gateways/gtwAWSS3.h"
#endif

#include "QHttp/qhttpfwd.hpp"
using namespace qhttp;

//TAPI_REGISTER_METATYPE(
//    /* complexity      */ COMPLEXITY_Object,
//    /* namespace       */ Targoman::API::ObjectStorage,
//    /* type            */ stuSaveFileResult,
//    /* toVariantLambda */ [](const stuSaveFileResult& _value) -> QVariant { return _value.toJson().toVariantMap(); }
//);

namespace Targoman::API::ObjectStorage {

QString ObjectStorageManager::getFileUrl(
    const quint64 _currentUserID,
    Targoman::API::ObjectStorage::ORM::intfUploadFiles &_uploadFiles,
    Targoman::API::ObjectStorage::ORM::intfUploadQueue &_uploadQueue,
    Targoman::API::ObjectStorage::ORM::intfUploadGateways &_uploadGateways,
    const quint64 _uploadedFileID
) {
    //if not stored yet, return local http server url that serves uflLocalFullFileName
}

void ObjectStorageManager::applyGetFileUrlInQuery(
    SelectQuery &_query,
    Targoman::API::ObjectStorage::ORM::intfUploadFiles &_uploadFiles,
    Targoman::API::ObjectStorage::ORM::intfUploadQueue &_uploadQueue
//        Targoman::API::ObjectStorage::ORM::intfUploadGateways &_uploadGateways,
//        const QString &_foreignTableName,
//        const QString &_foreignTableUploadedFileIDFieldName
) {
    Targoman::API::DBM::clsTable* TBL_uploadFiles = dynamic_cast<Targoman::API::DBM::clsTable*>(&_uploadFiles);
//    Targoman::API::DBM::clsTable* TBL_uploadQueue = dynamic_cast<Targoman::API::DBM::clsTable*>(&_uploadQueue);

    /*
     * https://dev.mysql.com/doc/refman/5.7/en/group-by-handling.html#:~:text=If%20the%20ONLY_FULL_GROUP_BY%20SQL%20mode,are%20functionally%20dependent%20on%20them.:
     *  -> You can achieve the same effect without disabling ONLY_FULL_GROUP_BY by using ANY_VALUE() to refer to the nonaggregated column.
     */
    _query
        .innerJoin(TBL_uploadFiles->name())
        .addCols({
                    tblUploadFiles::uflID,
                    tblUploadFiles::uflPath,
                    tblUploadFiles::uflOriginalFileName,
                    tblUploadFiles::uflSize,
                    tblUploadFiles::uflFileType,
                    tblUploadFiles::uflMimeType,
                    tblUploadFiles::uflLocalFullFileName,
                    tblUploadFiles::uflStatus,
                    tblUploadFiles::uflCreationDateTime,
                    tblUploadFiles::uflCreatedBy_usrID,
                    tblUploadFiles::uflUpdatedBy_usrID,
                 })

        .innerJoin(SelectQuery(_uploadQueue)
                   .addCol(tblUploadQueue::uqu_uflID) //, "_q_uflID")
                   .addCol(DBExpression::VALUE(QString("ANY_VALUE(%1)").arg(tblUploadGateways::ugwType)), uflFullFileUrl)
                   .innerJoin(tblUploadGateways::Name)
                   .where({ tblUploadQueue::uquStatus, enuConditionOperator::Equal, enuUploadQueueStatus::Stored })
                   .groupBy(tblUploadQueue::uqu_uflID)
                   ,
                   "tmpQueue",
                   clsCondition("tmpQueue", tblUploadQueue::uqu_uflID,
                                enuConditionOperator::Equal,
                                TBL_uploadFiles->name(), tblUploadFiles::uflID)
                   )
        .addCol(QString("tmpQueue.%1").arg(uflFullFileUrl), uflFullFileUrl)
    ;
}

QVariantMap ObjectStorageManager::saveFiles(
    const quint64 _currentUserID,
    intfUploadFiles &_uploadFiles,
    intfUploadQueue &_uploadQueue,
    intfUploadGateways &_uploadGateways,
    const TAPI::Files_t &_files,
    const QString &_path
) {
    QVariantMap Result;
    foreach (auto _file, _files) {
        try {
            quint64 ID = saveFile(
                             _currentUserID,
                             _uploadFiles,
                             _uploadQueue,
                             _uploadGateways,
                             _file,
                             _path
                             );
            Result.insert(_file.Name, ID);
        } catch (std::exception &exp) {
            Result.insert(_file.Name, exp.what());
        }
    }
    return Result;
}

quint64 ObjectStorageManager::saveFile(
    const quint64 _currentUserID,
    intfUploadFiles &_uploadFiles,
    intfUploadQueue &_uploadQueue,
    intfUploadGateways &_uploadGateways,
    const TAPI::stuFileInfo &_file,
    QString _path
) {
    ORM::intfUploadFiles::stuObjectStorageConfigs _objectStorageConfigs = _uploadFiles.getObjectStorageConfigs();

    //save file to the _objectStorageConfigs.TempLocalStoragePath (LOCAL/NFS)
    if (_file.Size == 0)
        throw exTargomanBase("The file is empty.", ESTATUS_BAD_REQUEST);

    if ((_file.Name.indexOf('/') >= 0) || (_file.Name.indexOf('\\') >= 0))
        throw exTargomanBase("Invalid character in file name.", ESTATUS_BAD_REQUEST);

    //------------------------------
    QString FullTempPath = _objectStorageConfigs.TempLocalStoragePath;
    QDir FullPathDir(FullTempPath);
    if (FullPathDir.exists() == false) {
        if (FullPathDir.mkpath(".") == false)
            throw exTargomanBase("Could not create storage folder.", ESTATUS_INTERNAL_SERVER_ERROR);
    }
    //--
    QString StrOwnerID = QString("user_%1").arg(_currentUserID);
    if (FullPathDir.exists(StrOwnerID) == false) {
        if (FullPathDir.mkpath(StrOwnerID) == false)
            throw exTargomanBase("Could not create path under storage folder.", ESTATUS_INTERNAL_SERVER_ERROR);
    }
    FullPathDir.cd(StrOwnerID);
    //--
    if (_path.isEmpty() == false) {
        if (FullPathDir.exists(_path) == false) {
            if (FullPathDir.mkpath(_path) == false)
                throw exTargomanBase("Could not create path under storage folder.", ESTATUS_INTERNAL_SERVER_ERROR);
        }
        FullPathDir.cd(_path);

        _path = StrOwnerID + "/" + _path;
    }
    FullTempPath = FullPathDir.absolutePath();

    //get mime type
    QString MimeType = _file.Mime;
    if (MimeType.isEmpty()) {
        QMimeDatabase MimeDB;
        QString MimeType = MimeDB.mimeTypeForFile(_file.TempName).name().toLower();
    }

    //get file extension
    QString FileType;
    int Idx = _file.Name.lastIndexOf('.');
    if (Idx >= 0)
        FileType = _file.Name.mid(Idx + 1).toLower();

    //save to tblUploadFiles
    quint64 UploadedFileID = 0;
    quint16 QueueRowsCount = 0;
    QString FullFileName;

    try {
        QVariantMap SpOutVars = _uploadFiles.callSP("spUploadedFile_Create", {
            { "iPath",              _path },
            { "iOriginalFileName",  _file.Name },
            { "iFullTempPath",      FullTempPath },
            { "iFileSize",          _file.Size },
            { "iFileType",          FileType },
            { "iMimeType",          MimeType },
            { "iCreatorUserID",     _currentUserID },
            { "iLocked",            1 },
        })
        .spDirectOutputs();

        UploadedFileID = SpOutVars.value("oUploadedFileID").toULongLong();
        QueueRowsCount = SpOutVars.value("oQueueRowsCount").toUInt();

        if (UploadedFileID <= 0)
            return 0;

        //move from temp to persistance location
//        QString FileUUID = QUuid::createUuid().toString(QUuid::Id128);
        QString oStoredFileName = SpOutVars.value("oStoredFileName").toString();
        FullFileName = QString("%1/%2").arg(FullTempPath).arg(oStoredFileName);
        QFile::rename(_file.TempName, FullFileName);
    } catch (std::exception &exp) {
        TargomanDebug(5, "ERROR: spUploadedFile_Create:" << exp.what());

        if (UploadedFileID > 0) {
            try {
                _uploadFiles.DeleteByPks(_uploadFiles, _currentUserID, QString::number(UploadedFileID));

            }  catch (...) { ; }
        }

        throw;
    }

    //trigger async upload to storage(s)
    try {
        if (QueueRowsCount > 0) {
            TargomanDebug(5, "before queue ObjectStorageManager::processQueue(" << UploadedFileID << ")");
            QFuture<bool> ret = QtConcurrent::run(
                                    ObjectStorageManager::processQueue,
                                    stuProcessQueueParams(
                                        _currentUserID,
                                        _uploadFiles,
                                        _uploadQueue,
                                        _uploadGateways,
                                        UploadedFileID,
                                        QueueRowsCount
                                    ));
            TargomanDebug(5, "after queue ObjectStorageManager::processQueue(" << UploadedFileID << ")");

#ifdef QT_DEBUG
//            bool rrr = ret.result();
#endif
        }
    } catch (std::exception &exp) {
        TargomanDebug(5, "ERROR: concurrent run of upload file queue(" << UploadedFileID << "):" << exp.what());

        UpdateQuery(_uploadQueue)
            .set(tblUploadQueue::uquLockedAt, DBExpression::NIL())
            .where({ tblUploadQueue::uqu_uflID, enuConditionOperator::Equal, UploadedFileID })
            .andWhere({ tblUploadQueue::uquLockedAt, enuConditionOperator::NotNull })
            .execute(_currentUserID)
        ;
    }

    return UploadedFileID;
}

bool ObjectStorageManager::processQueue(
    const stuProcessQueueParams &_processQueueParams
) {
    TargomanDebug(5, "ObjectStorageManager::processQueue(" << _processQueueParams.UploadedFileID << ")");

    _processQueueParams.UploadQueue.prepareFiltersList();

    SelectQuery Query = SelectQuery(_processQueueParams.UploadQueue)
        .addCols({
                     tblUploadQueue::uquID,
                     tblUploadQueue::uqu_uflID,
                     tblUploadQueue::uqu_ugwID,
                     tblUploadQueue::uquStatus,
                     tblUploadFiles::uflID,
                     tblUploadFiles::uflOriginalFileName,
                     tblUploadFiles::uflStoredFileName,
                     tblUploadFiles::uflPath,
                     tblUploadFiles::uflSize,
                     tblUploadFiles::uflFileType,
                     tblUploadFiles::uflMimeType,
                     tblUploadFiles::uflLocalFullFileName,
                     tblUploadFiles::uflStatus,
                     tblUploadFiles::uflCreatedBy_usrID,
                     tblUploadGateways::ugwID,
                     tblUploadGateways::ugwType,
                     tblUploadGateways::ugwMetaInfo,
                     tblUploadGateways::ugwCreatedFilesCount,
                     tblUploadGateways::ugwCreatedFilesSize,
                     tblUploadGateways::ugwLastActionTime,
                     tblUploadGateways::ugwStatus,
                 })
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

    try {
        QueueItems = Query.all({}, _processQueueParams.MaxItemsCount);
    } catch (std::exception &exp) {
        TargomanDebug(5, "ERROR: fetching upload queue items:" << exp.what());
    }

    if (QueueItems.isEmpty()) {
        if (_processQueueParams.UploadedFileID > 0)
            UpdateQuery(_processQueueParams.UploadFiles)
                    .setPksByPath(_processQueueParams.UploadedFileID)
                    .set(tblUploadFiles::uflStatus, enuUploadFileStatus::New)
                    .execute(_processQueueParams.CurrentUserID);

        return false;
    }

    QList<ORM::Private::stuProcessUploadQueueInfo> QueueInfos;

    //update Queue Status to Uploading
    QStringList UploadingQueueIDs;
    foreach (QVariant Var, QueueItems) {
        ORM::Private::stuProcessUploadQueueInfo QueueInfo;
        QueueInfo.fromVariantMap(Var.toMap());
        QueueInfos.append(QueueInfo);

        if (QueueInfo.UploadQueue.uquStatus == enuUploadQueueStatus::New)
            UploadingQueueIDs.append(QString::number(QueueInfo.UploadQueue.uquID));
    }

    if (UploadingQueueIDs.length()) {
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

    foreach (ORM::Private::stuProcessUploadQueueInfo QueueInfo, QueueInfos) {
        bool Stored = false;
        try {
            Stored = ObjectStorageManager::storeFile(
                         QueueInfo.UploadGateways.ugwType,
                         QueueInfo.UploadGateways.ugwMetaInfo,
//                         QueueInfo.UploadFiles.uflCreatedBy_usrID,
                         QueueInfo.UploadFiles.uflLocalFullFileName,
                         QueueInfo.UploadFiles.uflPath,
                         QueueInfo.UploadFiles.uflStoredFileName
                         );
        } catch (std::exception &exp) {
            TargomanDebug(5, "ERROR: storing file to remote storage: " << exp.what());
        }

        if (Stored) {
            if (GatewayUploadedFileCount.contains(QueueInfo.UploadGateways.ugwID)) {
                GatewayUploadedFileCount[QueueInfo.UploadGateways.ugwID] += 1;
                GatewayUploadedFileSize[QueueInfo.UploadGateways.ugwID] += QueueInfo.UploadFiles.uflSize;
            } else {
                GatewayUploadedFileCount.insert(QueueInfo.UploadGateways.ugwID, 1);
                GatewayUploadedFileSize.insert(QueueInfo.UploadGateways.ugwID, QueueInfo.UploadFiles.uflSize);
            }

//            QList<quint64> UploadedFileIDs;
            UploadedQueueIDs.append(QString::number(QueueInfo.UploadQueue.uquID));
        } else {
//            QList<quint64> FailedFileIDs;
            FailedQueueIDs.append(QString::number(QueueInfo.UploadQueue.uquID));
        }
    }

    //update gateway ststictics
    if (GatewayUploadedFileCount.count()) {
        foreach (quint64 GatewayID, GatewayUploadedFileCount.keys()) {
            UpdateQuery(_processQueueParams.UploadGateways)
                    .increament(tblUploadGateways::ugwCreatedFilesCount, GatewayUploadedFileCount[GatewayID])
                    .increament(tblUploadGateways::ugwCreatedFilesSize, GatewayUploadedFileSize[GatewayID])
                    .setPksByPath(GatewayID)
                    .execute(_processQueueParams.CurrentUserID)
                    ;
        }
    }

    //update queue
    if (UploadedQueueIDs.length()) {
        UpdateQuery(_processQueueParams.UploadQueue)
                .set(tblUploadQueue::uquStatus, enuUploadQueueStatus::Stored)
                .where({ tblUploadQueue::uquID, enuConditionOperator::In, UploadedQueueIDs.join(',') })
                .execute(_processQueueParams.CurrentUserID);
                ;
    }
    if (FailedQueueIDs.length()) {
        UpdateQuery(_processQueueParams.UploadQueue)
                .set(tblUploadQueue::uquStatus, enuUploadQueueStatus::New)
                .where({ tblUploadQueue::uquID, enuConditionOperator::In, FailedQueueIDs.join(',') })
                .execute(_processQueueParams.CurrentUserID);
                ;
    }

    ///TODO: re compute upload file status by queue items status in the next version
//    QString QueueIDs = QueueInfos.keys().join(',');
//    update

    //------------------------------------------------
    return true;
}

bool ObjectStorageManager::storeFile(
    const NULLABLE_TYPE(enuUploadGatewayType::Type) &_storageType,
    const TAPI::JSON_t &_metaInfo,
//    quint64 _ownerUserID,
    const QString &_fullFileName,
    const QString &_path,
    const QString &_fileName
) {
    if (NULLABLE_IS_NULL(_storageType))
        throw exTargomanBase("storageType IS NULL");

//    QStringList FullPathParts;
//    FullPathParts << QString("user_%1").arg(_ownerUserID);
//    if (_path.isEmpty() == false)
//        FullPathParts << _path;

    switch (NULLABLE_VALUE(_storageType)) {
        case enuUploadGatewayType::NFS:
            return Gateways::gtwNFS::storeFile(
                        _metaInfo,
                        _fullFileName,
                        _path, //FullPathParts.join("/"),
                        _fileName
                        );

        case enuUploadGatewayType::AWSS3:
            return Gateways::gtwAWSS3::storeFile(
                        _metaInfo,
                        _fullFileName,
                        _path, //FullPathParts.join("/"),
                        _fileName
                        );

    }

    return false;
}

} //namespace Targoman::API::ObjectStorage
