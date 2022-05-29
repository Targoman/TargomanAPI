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
 * @author S.Mehran M.Ziabary <ziabary@targoman.com>
 * @author Kambiz Zandi <kambizzandi@gmail.com>
 */

#ifndef TARGOMAN_API_OBJECTSTORAGE_ObjectStorageManager_H
#define TARGOMAN_API_OBJECTSTORAGE_ObjectStorageManager_H

#include <QString>
#include <QVariantMap>
#include "libTargomanCommon/CmdIO.h"
#include "Interfaces/Common/APIArgHelperMacros.hpp"
#include "Interfaces/Common/GenericTypes.h"
#include <QJsonObject>
#include "Interfaces/ObjectStorage/ORM/ObjectStorage.h"
using namespace Targoman::API::ObjectStorage::ORM;

namespace Targoman::API::ObjectStorage::ORM {
class intfUploadFiles;
class intfUploadGateways;
class intfUploadQueue;
}

namespace Targoman::API::ObjectStorage {

//TAPI_DEFINE_VARIANT_ENABLED_STRUCT(stuSaveFileResult,
//    SF_QString                  (FullFileUrl),
//    SF_QString                  (MimeType),
//    SF_quint8                   (IsUploaded),
//    SF_quint64                  (uplID)
//);

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-variable"
TARGOMAN_CREATE_CONSTEXPR(uflFullFileUrl);
#pragma GCC diagnostic pop

class ObjectStorageManager
{
public:
    static QVariantMap saveFiles(
        INTFAPICALLBOOM_DECL &APICALLBOOM_PARAM,
//        const quint64 _currentUserID,
        Targoman::API::ObjectStorage::ORM::intfUploadFiles &_uploadFiles,
        Targoman::API::ObjectStorage::ORM::intfUploadQueue &_uploadQueue,
        Targoman::API::ObjectStorage::ORM::intfUploadGateways &_uploadGateways,
        const TAPI::Files_t &_files,
        const QString &_path = {}
    );

    static quint64 saveFile(
        INTFAPICALLBOOM_DECL &APICALLBOOM_PARAM,
//        const quint64 _currentUserID,
        Targoman::API::ObjectStorage::ORM::intfUploadFiles &_uploadFiles,
        Targoman::API::ObjectStorage::ORM::intfUploadQueue &_uploadQueue,
        Targoman::API::ObjectStorage::ORM::intfUploadGateways &_uploadGateways,
        const TAPI::stuFileInfo &_file,
        QString _path = {}
    );

//    static Targoman::API::Helpers::stuSaveFileResult saveFile(
//            const quint64 _currentUserID,
//            Targoman::API::ObjectStorage::ORM::intfUploadFiles *_uploadFiles,
//            QString &_fileName,
//            const QString &_base64Content
//            );

    static void applyGetFileUrlInQuery(
        SelectQuery &_query,
        Targoman::API::ObjectStorage::ORM::intfUploadFiles &_uploadFiles,
        Targoman::API::ObjectStorage::ORM::intfUploadQueue &_uploadQueue,
        Targoman::API::ObjectStorage::ORM::intfUploadGateways &_uploadGateways,
        const QString &_foreignTableName,
        const QString &_foreignTableUploadedFileIDFieldName
        );

//    static QString getFileUrl(
//        const quint64 _currentUserID,
//        Targoman::API::ObjectStorage::ORM::intfUploadFiles &_uploadFiles,
//        Targoman::API::ObjectStorage::ORM::intfUploadQueue &_uploadQueue,
//        Targoman::API::ObjectStorage::ORM::intfUploadGateways &_uploadGateways,
//        const quint64 _uploadedFileID
//    );

private:
    struct stuProcessQueueParams {
        quint64 CurrentUserID;
        Targoman::API::ObjectStorage::ORM::intfUploadFiles &UploadFiles;
        Targoman::API::ObjectStorage::ORM::intfUploadQueue &UploadQueue;
        Targoman::API::ObjectStorage::ORM::intfUploadGateways &UploadGateways;
        quint64 UploadedFileID = 0;
        quint16 MaxItemsCount = 100;

        stuProcessQueueParams(
            quint64 _currentUserID,
            Targoman::API::ObjectStorage::ORM::intfUploadFiles &_uploadFiles,
            Targoman::API::ObjectStorage::ORM::intfUploadQueue &_uploadQueue,
            Targoman::API::ObjectStorage::ORM::intfUploadGateways &_uploadGateways,
            quint64 _uploadedFileID = 0,
            quint16 _maxItemsCount = 100
        ) :
            CurrentUserID(_currentUserID),
            UploadFiles(_uploadFiles),
            UploadQueue(_uploadQueue),
            UploadGateways(_uploadGateways),
            UploadedFileID(_uploadedFileID),
            MaxItemsCount(_maxItemsCount)
        { ; }
    };

    static bool processQueue(const stuProcessQueueParams &_processQueueParams);

    static bool storeFileToGateway(const ORM::Private::stuProcessUploadQueueInfo &_queueInfo);
};

} //namespace Targoman::API::ObjectStorage

//TAPI_DECLARE_METATYPE(Targoman::API::Helpers::stuSaveFileResult) // -> TAPI_REGISTER_METATYPE() in ObjectStorageManager.cpp

#endif // TARGOMAN_API_OBJECTSTORAGE_ObjectStorageManager_H
