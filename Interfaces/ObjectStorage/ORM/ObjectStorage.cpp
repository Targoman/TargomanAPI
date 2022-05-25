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

#include "ObjectStorage.h"
#include "Interfaces/AAA/Authorization.h"
#include "Interfaces/AAA/AAADefs.hpp"
#include "Interfaces/AAA/clsJWT.hpp"
using namespace Targoman::API::AAA;

#include "Interfaces/Common/QtTypes.hpp"

TAPI_REGISTER_TARGOMAN_ENUM(Targoman::API::ObjectStorage::ORM, enuUploadFileStatus);
TAPI_REGISTER_TARGOMAN_ENUM(Targoman::API::ObjectStorage::ORM, enuUploadQueueStatus);
TAPI_REGISTER_TARGOMAN_ENUM(Targoman::API::ObjectStorage::ORM, enuUploadGatewayType);
TAPI_REGISTER_TARGOMAN_ENUM(Targoman::API::ObjectStorage::ORM, enuUploadGatewayStatus);

namespace Targoman::API::ObjectStorage::ORM {

namespace Private {

void stuProcessUploadQueueInfo::fromVariantMap(const QVariantMap& _info) {
    QJsonObject JsonInfo = QJsonObject::fromVariantMap(_info);

    this->UploadFiles.fromJson(JsonInfo);
    this->UploadQueue.fromJson(JsonInfo);
    this->UploadGateways.fromJson(JsonInfo);
}

} //namespace Private

/******************************************************************/
/******************************************************************/
/******************************************************************/
intfUploadFiles::intfUploadFiles(
    const QString &_schema,
    const QString &_name
) :
    intfSQLBasedModule(
        _schema,
        _name,
        tblUploadFiles::Name,
        {///< ColName                               Type                    Validation                  Default     UpBy    Sort  Filter Self  Virt   PK
            { tblUploadFiles::uflID,                ORM_PRIMARYKEY_64 },
            { tblUploadFiles::uflPath,              S(QString),             QFV,                        QRequired,  UPNone },
            { tblUploadFiles::uflOriginalFileName,  S(QString),             QFV,                        QRequired,  UPNone },
            { tblUploadFiles::uflCounter,           S(quint32),             QFV,                        QNull,      UPNone },
            { tblUploadFiles::uflStoredFileName,    S(QString),             QFV,                        QRequired,  UPNone },
            { tblUploadFiles::uflSize,              S(quint64),             QFV.integer().minValue(1),  QRequired,  UPNone },
            { tblUploadFiles::uflFileType,          S(QString),             QFV,                        QNull,      UPNone },
            { tblUploadFiles::uflMimeType,          S(QString),             QFV,                        QNull,      UPNone },
            { tblUploadFiles::uflLocalFullFileName, S(QString),             QFV,                        QRequired,  UPNone },
            { tblUploadFiles::uflStatus,            ORM_STATUS_FIELD(Targoman::API::ObjectStorage::ORM::enuUploadFileStatus, Targoman::API::ObjectStorage::ORM::enuUploadFileStatus::New) },
            { tblUploadFiles::uflCreationDateTime,  ORM_CREATED_ON },
            { tblUploadFiles::uflCreatedBy_usrID,   ORM_CREATED_BY },
            { tblUploadFiles::uflUpdatedBy_usrID,   ORM_UPDATED_BY },
        },
        {///< Col                        Reference Table              ForeignCol       Rename     LeftJoin
            ORM_RELATION_OF_CREATOR(tblUploadFiles::uflCreatedBy_usrID),
            ORM_RELATION_OF_UPDATER(tblUploadFiles::uflUpdatedBy_usrID),
        }
) { ; }

QVariant IMPL_ORMGET(intfUploadFiles) {
    Authorization::checkPriv(_APICALLBOOM.getJWT(), this->privOn(EHTTP_GET, this->moduleBaseName()));
    return this->Select(*this, GET_METHOD_ARGS_CALL_INTERNAL_BOOM);
}

/******************************************************************/
/******************************************************************/
/******************************************************************/
intfUploadQueue::intfUploadQueue(
    const QString &_schema,
    const QString &_name
) :
    intfSQLBasedModule(
        _schema,
        _name,
        tblUploadQueue::Name,
        {///< ColName                               Type                    Validation                  Default     UpBy    Sort  Filter Self  Virt   PK
            { tblUploadQueue::uquID,                ORM_PRIMARYKEY_64 },
            { tblUploadQueue::uqu_uflID,            S(quint64),             QFV.integer().minValue(1),  QRequired,  UPNone },
            { tblUploadQueue::uqu_ugwID,            S(quint32),             QFV.integer().minValue(1),  QRequired,  UPNone },
            { tblUploadQueue::uquLockedAt,          S(TAPI::DateTime_t),    QFV,                        QNull,      UPAdmin },
            { tblUploadQueue::uquLockedBy,          S(QString),             QFV,                        QNull,      UPAdmin },
            { tblUploadQueue::uquLastTryAt,         S(TAPI::DateTime_t),    QFV,                        QNull,      UPAdmin },
            { tblUploadQueue::uquStoredAt,          S(TAPI::DateTime_t),    QFV,                        QNull,      UPAdmin },
            { tblUploadQueue::uquResult,            S(QString),             QFV,                        QNull,      UPAdmin },
            { tblUploadQueue::uquStatus,            ORM_STATUS_FIELD(Targoman::API::ObjectStorage::ORM::enuUploadQueueStatus, Targoman::API::ObjectStorage::ORM::enuUploadQueueStatus::New) },
            { tblUploadQueue::uquCreationDateTime,  ORM_CREATED_ON },
            { tblUploadQueue::uquCreatedBy_usrID,   ORM_CREATED_BY },
            { tblUploadQueue::uquUpdatedBy_usrID,   ORM_UPDATED_BY },
        },
        {///<                                       Col                             Reference Table                         ForeignCol              Rename      LeftJoin
            { tblUploadQueue::Relation::File,       { tblUploadQueue::uqu_uflID,    R(_schema, tblUploadFiles::Name),       tblUploadFiles::uflID } },
            { tblUploadQueue::Relation::Gateway,    { tblUploadQueue::uqu_ugwID,    R(_schema, tblUploadGateways::Name),    tblUploadGateways::ugwID } },
            ORM_RELATION_OF_CREATOR(tblUploadQueue::uquCreatedBy_usrID),
            ORM_RELATION_OF_UPDATER(tblUploadQueue::uquUpdatedBy_usrID),
        }
) { ; }

QVariant IMPL_ORMGET(intfUploadQueue) {
    Authorization::checkPriv(_APICALLBOOM.getJWT(), this->privOn(EHTTP_GET, this->moduleBaseName()));
    return this->Select(*this, GET_METHOD_ARGS_CALL_INTERNAL_BOOM);
}

/******************************************************************/
/******************************************************************/
/******************************************************************/
intfUploadGateways::intfUploadGateways(
    const QString &_schema,
    const QString &_name
) :
    intfSQLBasedModule(
        _schema,
        _name,
        tblUploadGateways::Name,
        {///< ColName                                       Type                        Validation                          Default     UpBy    Sort  Filter Self  Virt   PK
            { tblUploadGateways::ugwID,                     ORM_PRIMARYKEY_32 },
            { tblUploadGateways::ugwName,                   S(QString),                 QFV.maxLenght(50),                  QRequired,  UPAdmin },
            { tblUploadGateways::ugwType,                   S(Targoman::API::ObjectStorage::ORM::enuUploadGatewayType::Type), QFV, QRequired,  UPAdmin },
            { tblUploadGateways::ugwBucket,                 S(QString),                 QFV.maxLenght(256),                 QRequired,  UPAdmin },
            { tblUploadGateways::ugwEndpointUrl,            S(QString),                 QFV.maxLenght(512),                 QRequired,  UPAdmin },
            { tblUploadGateways::ugwEndpointIsVirtualHosted,S(bool),                    QFV,                                false,      UPOwner },
            { tblUploadGateways::ugwMetaInfo,               S(TAPI::JSON_t),            QFV,                                QNull,      UPAdmin },
            // conditions
            { tblUploadGateways::ugwAllowedFileTypes,       S(QString),                 QFV.unicodeAlNum().maxLenght(512),  QNull,      UPAdmin },
            { tblUploadGateways::ugwAllowedMimeTypes,       S(QString),                 QFV.unicodeAlNum().maxLenght(1024), QNull,      UPAdmin },
            { tblUploadGateways::ugwAllowedMinFileSize,     S(NULLABLE_TYPE(quint64)),  QFV,                                QNull,      UPAdmin },
            { tblUploadGateways::ugwAllowedMaxFileSize,     S(NULLABLE_TYPE(quint64)),  QFV,                                QNull,      UPAdmin },
            { tblUploadGateways::ugwMaxFilesCount,          S(NULLABLE_TYPE(quint64)),  QFV,                                QNull,      UPAdmin },
            { tblUploadGateways::ugwMaxFilesSize,           S(NULLABLE_TYPE(quint64)),  QFV,                                QNull,      UPAdmin },
            // statistics
            { tblUploadGateways::ugwCreatedFilesCount,      S(quint64),                 QFV,                                0,          UPAdmin },
            { tblUploadGateways::ugwCreatedFilesSize,       S(quint64),                 QFV,                                0,          UPAdmin },
            { tblUploadGateways::ugwDeletedFilesCount,      S(quint64),                 QFV,                                0,          UPAdmin },
            { tblUploadGateways::ugwDeletedFilesSize,       S(quint64),                 QFV,                                0,          UPAdmin },
            { tblUploadGateways::ugwLastActionTime,         S(NULLABLE_TYPE(TAPI::DateTime_t)), QFV,                        QNull,      UPAdmin },
            //------------------
            { tblUploadGateways::ugwStatus,                 ORM_STATUS_FIELD(Targoman::API::ObjectStorage::ORM::enuUploadGatewayStatus, Targoman::API::ObjectStorage::ORM::enuUploadGatewayStatus::Active) },
            { tblUploadGateways::ugwCreationDateTime,       ORM_CREATED_ON },
            { tblUploadGateways::ugwCreatedBy_usrID,        ORM_CREATED_BY },
            { tblUploadGateways::ugwUpdatedBy_usrID,        ORM_UPDATED_BY },
        },
        {///< Col                        Reference Table              ForeignCol       Rename     LeftJoin
            ORM_RELATION_OF_CREATOR(tblUploadGateways::ugwCreatedBy_usrID),
            ORM_RELATION_OF_UPDATER(tblUploadGateways::ugwUpdatedBy_usrID),
        }
) { ; }

QVariant IMPL_ORMGET(intfUploadGateways) {
    Authorization::checkPriv(_APICALLBOOM.getJWT(), this->privOn(EHTTP_GET, this->moduleBaseName()));
    return this->Select(*this, GET_METHOD_ARGS_CALL_INTERNAL_BOOM);
}

quint32 IMPL_ORMCREATE(intfUploadGateways) {
    Authorization::checkPriv(_APICALLBOOM.getJWT(), this->privOn(EHTTP_PUT, this->moduleBaseName()));

//    if (_createInfo.contains(tblintfUploadGateways::ugwAllowedDomainName))
//        _createInfo[tblintfUploadGateways::ugwAllowedDomainName] = URLHelper::domain(_createInfo[tblUploadGateways::ugwAllowedDomainName].toString());

    return this->Create(*this, CREATE_METHOD_ARGS_CALL_INTERNAL_BOOM);
}

bool IMPL_ORMUPDATE(intfUploadGateways) {
    Authorization::checkPriv(_APICALLBOOM.getJWT(), this->privOn(EHTTP_PATCH, this->moduleBaseName()));
    return this->Update(*this, UPDATE_METHOD_ARGS_CALL_INTERNAL_BOOM);
}

bool IMPL_ORMDELETE(intfUploadGateways) {
    Authorization::checkPriv(_APICALLBOOM.getJWT(), this->privOn(EHTTP_DELETE, this->moduleBaseName()));
    return this->DeleteByPks(*this, DELETE_METHOD_ARGS_CALL_INTERNAL_BOOM);
}

} //namespace Targoman::API::ObjectStorage::ORM
