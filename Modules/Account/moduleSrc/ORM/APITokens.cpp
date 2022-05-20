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

#include "APITokens.h"
#include "User.h"
#include "Service.h"

//#include "Interfaces/ORM/APIQueryBuilders.h"

TAPI_REGISTER_TARGOMAN_ENUM(Targoman::API::AccountModule, enuAPITokensStatus);

namespace Targoman::API::AccountModule::ORM {

APITokens::APITokens() :
    intfSQLBasedModule(
        AAASchema,
        "tblAPITokens",
        {///< ColName                            Type                   Validation                      Default    UpBy   Sort  Filter Self  Virt   PK
            { tblAPITokens::aptID,               ORM_PRIMARYKEY_64 },
            { tblAPITokens::aptToken,            S(QString),            QFV.asciiAlNum().maxLenght(50), QRequired, UPNone, true, false },
            { tblAPITokens::apt_usrID,           S(quint64),            QFV.integer().minValue(1),      QRequired, UPNone },
            { tblAPITokens::apt_svcID,           S(quint32),            QFV.integer().minValue(1),      QRequired, UPAdmin },
            { tblAPITokens::aptLang,             S(TAPI::ISO639_2_t),   QFV,                            "en",      UPAdmin },
            { tblAPITokens::aptValidateIP,       S(bool),               QFV,                            false,     UPAdmin },
            { tblAPITokens::aptExtraPriviledges, S(TAPI::PrivObject_t), QFV,                            QNull,     UPAdmin, false, false },
            { tblAPITokens::aptExpiryDate,       S(TAPI::DateTime_t),   QFV,                            QNull,     UPAdmin },
            { tblAPITokens::aptLastActivity,     S(TAPI::DateTime_t),   QFV,                            QInvalid,  UPNone },
            { tblAPITokens::aptAccessCount,      S(quint32),            QFV.integer().minValue(1),      QInvalid,  UPNone },
            { tblAPITokens::aptStatus,           ORM_STATUS_FIELD(Targoman::API::AccountModule::enuAPITokensStatus, Targoman::API::AccountModule::enuAPITokensStatus::Active) },
            { ORM_INVALIDATED_AT_FIELD },
            { tblAPITokens::aptCreatedBy_usrID,  ORM_CREATED_BY },
            { tblAPITokens::aptCreationDateTime, ORM_CREATED_ON },
            { tblAPITokens::aptUpdatedBy_usrID,  ORM_UPDATED_BY },
        },
        {///< Col                                Reference Table                 ForeignCol         Rename    LeftJoin
            { tblAPITokens::apt_svcID,           R(AAASchema, tblService::Name), tblService::svcID, {},       true},
            { tblAPITokens::apt_usrID,           R(AAASchema, tblUser::Name),    tblUser::usrID,    "Owner_", true},
            ORM_RELATION_OF_CREATOR(tblAPITokens::aptCreatedBy_usrID),
            ORM_RELATION_OF_UPDATER(tblAPITokens::aptUpdatedBy_usrID),
        },
        {
            { {
                tblAPITokens::aptToken,
                ORM_INVALIDATED_AT_FIELD_NAME,
              }, enuDBIndex::Unique },
        }
    ) { ; }

QVariant IMPL_ORMGET(APITokens) {
    if (Authorization::hasPriv(_APICALLBOOM.getJWT(), this->privOn(EHTTP_GET, this->moduleBaseName())) == false)
        this->setSelfFilters({{tblAPITokens::apt_usrID, _APICALLBOOM.getUserID()}}, _filters);

    return this->Select(*this, GET_METHOD_ARGS_CALL_INTERNAL_BOOM);
}

quint64 IMPL_ORMCREATE(APITokens) {
    Authorization::checkPriv(_APICALLBOOM.getJWT(), this->privOn(EHTTP_PUT, this->moduleBaseName()));

    return this->Create(*this, CREATE_METHOD_ARGS_CALL_INTERNAL_BOOM2USER);
}

bool IMPL_ORMUPDATE(APITokens) {
    Authorization::checkPriv(_APICALLBOOM.getJWT(), this->privOn(EHTTP_PATCH, this->moduleBaseName()));

    return this->Update(*this, UPDATE_METHOD_ARGS_CALL_INTERNAL_BOOM2USER);
}

bool IMPL_ORMDELETE(APITokens) {
    TAPI::ORMFields_t ExtraFilters;
    if (Authorization::hasPriv(_APICALLBOOM.getJWT(), this->privOn(EHTTP_DELETE, this->moduleBaseName())) == false)
        ExtraFilters.insert(tblAPITokens::apt_usrID, _APICALLBOOM.getUserID());
//    this->setSelfFilters({{tblAPITokens::apt_usrID, _APICALLBOOM.getUserID()}}, ExtraFilters);

    return this->DeleteByPks(*this, DELETE_METHOD_ARGS_CALL_INTERNAL_BOOM2USER, ExtraFilters);
}

} //namespace Targoman::API::AccountModule::ORM
