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
    //Upload Queue
    SET_FIELD_FROM_VARIANT_MAP_SAME_NAME(this->, uquID                  , _info, tblUploadQueue);
    SET_FIELD_FROM_VARIANT_MAP_SAME_NAME(this->, uqu_uflID              , _info, tblUploadQueue);
    SET_FIELD_FROM_VARIANT_MAP_SAME_NAME(this->, uqu_ugwID              , _info, tblUploadQueue);
    SET_FIELD_FROM_VARIANT_MAP_SAME_NAME(this->, uquStatus              , _info, tblUploadQueue);
    //Upload File
    SET_FIELD_FROM_VARIANT_MAP_SAME_NAME(this->, uflID                  , _info, tblUploadFiles);
    SET_FIELD_FROM_VARIANT_MAP_SAME_NAME(this->, uflFileName            , _info, tblUploadFiles);
    SET_FIELD_FROM_VARIANT_MAP_SAME_NAME(this->, uflFileUUID            , _info, tblUploadFiles);
    SET_FIELD_FROM_VARIANT_MAP_SAME_NAME(this->, uflSize                , _info, tblUploadFiles);
    SET_FIELD_FROM_VARIANT_MAP_SAME_NAME(this->, uflFileType            , _info, tblUploadFiles);
    SET_FIELD_FROM_VARIANT_MAP_SAME_NAME(this->, uflMimeType            , _info, tblUploadFiles);
    SET_FIELD_FROM_VARIANT_MAP_SAME_NAME(this->, uflLocalFullFileName   , _info, tblUploadFiles);
    ///TODO: bug at this line:
//    SET_FIELD_FROM_VARIANT_MAP_SAME_NAME(this->, uflStatus              , _info, tblUploadFiles);
    //Upload Gateway
    SET_FIELD_FROM_VARIANT_MAP_SAME_NAME(this->, ugwID                  , _info, tblUploadGateways);
    SET_FIELD_FROM_VARIANT_MAP_SAME_NAME(this->, ugwType                , _info, tblUploadGateways);
//    SET_FIELD_FROM_VARIANT_MAP_SAME_NAME(this->, ugwBucket              , _info, tblUploadGateways);
//    SET_FIELD_FROM_VARIANT_MAP_SAME_NAME(this->, ugwEndpointUrl         , _info, tblUploadGateways);
//    SET_FIELD_FROM_VARIANT_MAP_SAME_NAME(this->, ugwSecretKey           , _info, tblUploadGateways);
//    SET_FIELD_FROM_VARIANT_MAP_SAME_NAME(this->, ugwAccessKey           , _info, tblUploadGateways);


    QVariant v = _info.value(tblUploadGateways::ugwMetaInfo);
    TAPI::setFromVariant(this->ugwMetaInfo, _info.value(tblUploadGateways::ugwMetaInfo));

    SET_FIELD_FROM_VARIANT_MAP_SAME_NAME(this->, ugwMetaInfo            , _info, tblUploadGateways);

    SET_FIELD_FROM_VARIANT_MAP_SAME_NAME(this->, ugwCreatedFilesCount   , _info, tblUploadGateways);
    SET_FIELD_FROM_VARIANT_MAP_SAME_NAME(this->, ugwCreatedFilesSize    , _info, tblUploadGateways);
    SET_FIELD_FROM_VARIANT_MAP_SAME_NAME(this->, ugwLastActionTime      , _info, tblUploadGateways);
    SET_FIELD_FROM_VARIANT_MAP_SAME_NAME(this->, ugwStatus              , _info, tblUploadGateways);
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
        {///< ColName                                       Type                    Validation                  Default     UpBy    Sort  Filter Self  Virt   PK
            { tblUploadFiles::uflID,                        ORM_PRIMARYKEY_64 },
            { tblUploadFiles::uflFileName,                  S(QString),             QFV,                        QRequired,  UPNone },
            { tblUploadFiles::uflFileUUID,                  S(QString),             QFV,                        QRequired,  UPNone },
            { tblUploadFiles::uflSize,                      S(quint64),             QFV.integer().minValue(1),  QRequired,  UPNone },
            { tblUploadFiles::uflFileType,                  S(QString),             QFV,                        QNull,      UPNone },
            { tblUploadFiles::uflMimeType,                  S(QString),             QFV,                        QNull,      UPNone },
            { tblUploadFiles::uflLocalFullFileName,         S(QString),             QFV,                        QRequired,  UPNone },
            { tblUploadFiles::uflStatus,                    ORM_STATUS_FIELD(Targoman::API::ObjectStorage::ORM::enuUploadFileStatus, Targoman::API::ObjectStorage::ORM::enuUploadFileStatus::New) },
            { tblUploadFiles::uflCreationDateTime,          ORM_CREATED_ON },
            { tblUploadFiles::uflCreatedBy_usrID,           ORM_CREATED_BY },
            { tblUploadFiles::uflUpdatedBy_usrID,           ORM_UPDATED_BY },
        },
        {///< Col                        Reference Table              ForeignCol       Rename     LeftJoin
            ORM_RELATION_OF_CREATOR(tblUploadFiles::uflCreatedBy_usrID),
            ORM_RELATION_OF_UPDATER(tblUploadFiles::uflUpdatedBy_usrID),
        }
    ) { ; }

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
        {///< ColName                                   Type                        Validation                          Default     UpBy    Sort  Filter Self  Virt   PK
            { tblUploadGateways::ugwID,                 ORM_PRIMARYKEY_32 },
            { tblUploadGateways::ugwType,               S(Targoman::API::ObjectStorage::ORM::enuUploadGatewayType::Type), QFV,         QRequired,  UPAdmin },
//            { tblUploadGateways::ugwBucket,             S(QString),                 QFV.unicodeAlNum().maxLenght(128),  QRequired,  UPAdmin },
//            { tblUploadGateways::ugwEndpointUrl,        S(QString),                 QFV.unicodeAlNum().maxLenght(512),  QRequired,  UPAdmin },
//            { tblUploadGateways::ugwSecretKey,          S(QString),                 QFV.unicodeAlNum().maxLenght(128),  QRequired,  UPAdmin },
//            { tblUploadGateways::ugwAccessKey,          S(QString),                 QFV.unicodeAlNum().maxLenght(128),  QRequired,  UPAdmin },
            { tblUploadGateways::ugwMetaInfo,           S(TAPI::JSON_t),            QFV,                                QNull,      UPAdmin },
            // conditions
            { tblUploadGateways::ugwAllowedFileTypes,   S(QString),                 QFV.unicodeAlNum().maxLenght(512),  QNull,      UPAdmin },
            { tblUploadGateways::ugwAllowedMimeTypes,   S(QString),                 QFV.unicodeAlNum().maxLenght(1024), QNull,      UPAdmin },
            { tblUploadGateways::ugwAllowedMinFileSize, S(NULLABLE_TYPE(quint64)),  QFV,                                QNull,      UPAdmin },
            { tblUploadGateways::ugwAllowedMaxFileSize, S(NULLABLE_TYPE(quint64)),  QFV,                                QNull,      UPAdmin },
            { tblUploadGateways::ugwMaxFilesCount,      S(NULLABLE_TYPE(quint64)),  QFV,                                QNull,      UPAdmin },
            { tblUploadGateways::ugwMaxFilesSize,       S(NULLABLE_TYPE(quint64)),  QFV,                                QNull,      UPAdmin },
            // statistics
            { tblUploadGateways::ugwCreatedFilesCount,  S(quint64),                 QFV,                                0,          UPAdmin },
            { tblUploadGateways::ugwCreatedFilesSize,   S(quint64),                 QFV,                                0,          UPAdmin },
            { tblUploadGateways::ugwDeletedFilesCount,  S(quint64),                 QFV,                                0,          UPAdmin },
            { tblUploadGateways::ugwDeletedFilesSize,   S(quint64),                 QFV,                                0,          UPAdmin },
            { tblUploadGateways::ugwLastActionTime,     S(NULLABLE_TYPE(TAPI::DateTime_t)), QFV,                        QNull,      UPAdmin },
            //------------------
            { tblUploadGateways::ugwStatus,             ORM_STATUS_FIELD(Targoman::API::ObjectStorage::ORM::enuUploadGatewayStatus, Targoman::API::ObjectStorage::ORM::enuUploadGatewayStatus::Active) },
            { tblUploadGateways::ugwCreationDateTime,   ORM_CREATED_ON },
            { tblUploadGateways::ugwCreatedBy_usrID,    ORM_CREATED_BY },
            { tblUploadGateways::ugwUpdatedBy_usrID,    ORM_UPDATED_BY },
        },
        {///< Col                        Reference Table              ForeignCol       Rename     LeftJoin
            ORM_RELATION_OF_CREATOR(tblUploadGateways::ugwCreatedBy_usrID),
            ORM_RELATION_OF_UPDATER(tblUploadGateways::ugwUpdatedBy_usrID),
        }
    ) { ; }

QVariant intfUploadGateways::apiGET(GET_METHOD_ARGS_IMPL_APICALL) {
    Authorization::checkPriv(_JWT, this->privOn(EHTTP_GET, this->moduleBaseName()));
    return /*Targoman::API::Query::*/this->Select(*this, GET_METHOD_CALL_ARGS_INTERNAL_CALL);
}

quint32 intfUploadGateways::apiCREATE(CREATE_METHOD_ARGS_IMPL_APICALL) {
    Authorization::checkPriv(_JWT, this->privOn(EHTTP_PUT, this->moduleBaseName()));

//    if (_createInfo.contains(tblintfUploadGateways::ugwAllowedDomainName))
//        _createInfo[tblintfUploadGateways::ugwAllowedDomainName] = URLHelper::domain(_createInfo[tblUploadGateways::ugwAllowedDomainName].toString());

    return /*Targoman::API::Query::*/this->Create(*this, CREATE_METHOD_CALL_ARGS_INTERNAL_CALL);
}

bool intfUploadGateways::apiUPDATE(UPDATE_METHOD_ARGS_IMPL_APICALL) {
    Authorization::checkPriv(_JWT, this->privOn(EHTTP_PATCH, this->moduleBaseName()));
    return /*Targoman::API::Query::*/this->Update(*this, UPDATE_METHOD_CALL_ARGS_INTERNAL_CALL);
}

bool intfUploadGateways::apiDELETE(DELETE_METHOD_ARGS_IMPL_APICALL) {
    Authorization::checkPriv(_JWT, this->privOn(EHTTP_DELETE, this->moduleBaseName()));
    return /*Targoman::API::Query::*/this->DeleteByPks(*this, DELETE_METHOD_CALL_ARGS_INTERNAL_CALL);
}

} //namespace Targoman::API::ObjectStorage::ORM
