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
 @author S. Mehran M. Ziabary <ziabary@targoman.com>
 */

#include "APITokens.h"

namespace Targoman {
namespace API {
namespace AAA {
using namespace ORM;

QVariant APITokens::apiGET(GET_METHOD_ARGS_IMPL)
{
    if(Authorization::hasPriv(_JWT, this->privOn(EHTTP_GET,this->moduleBaseName())) == false)
        this->setSelfFilters({{"apt_usrID", clsJWT(_JWT).usrID()}}, _EXTRAPATH, _ORMFILTERS, _filters);
    return this->selectFromTable({}, {}, GET_METHOD_CALL_ARGS);
}

bool APITokens::apiDELETE(DELETE_METHOD_ARGS_IMPL)
{
    if(Authorization::hasPriv(_JWT, this->privOn(EHTTP_DELETE,this->moduleBaseName())) == false)
        this->setSelfFilters({{"apt_usrID", clsJWT(_JWT).usrID()}}, _EXTRAPATH, _ORMFILTERS);

    return this->deleteByPKs(DELETE_METHOD_CALL_ARGS);
}

bool APITokens::apiUPDATE(UPDATE_METHOD_ARGS_IMPL)
{
    Authorization::checkPriv(_JWT, this->privOn(EHTTP_PATCH,this->moduleBaseName()));
    return this->update(UPDATE_METHOD_CALL_ARGS);
}

quint32 APITokens::apiCREATE(CREATE_METHOD_ARGS_IMPL)
{
    Authorization::checkPriv(_JWT, this->privOn(EHTTP_PUT,this->moduleBaseName()));
    return this->create(CREATE_METHOD_CALL_ARGS).toUInt();
}

APITokens::APITokens() :
    clsTable(AAASchema,
              "tblAPITokens",
              { ///<ColName             Type                   Validation                      Default    RO   Sort  Filter Self  Virt   PK
                {"aptID",               S(quint64),            QFV.integer().minValue(1),      ORM_PRIMARY_KEY},
                {"aptToken",            S(QString),            QFV.asciiAlNum().maxLenght(50), QInvalid,  true, true, false},
                {"apt_usrID",           S(quint32),            QFV.integer().minValue(1),      0},
                {"apt_svcID",           S(quint32),            QFV.integer().minValue(1),      0},
                {"aptLang",             S(TAPI::ISO639_2_t),   QFV,                            "en"},
                {"aptValidateIP",       S(bool),               QFV,                            false},
                {"aptExtraPriviledges", S(TAPI::JSON_t),       QFV,                            QNull,    false,false, false},
                {"aptExpiryDate",       S(TAPI::DateTime_t),   QFV,                            QNull},
                {"aptLastActivity",     S(TAPI::DateTime_t),   QFV,                            QNull ,    true},
                {"aptAccessCount",      S(quint32),            QFV.integer().minValue(1),      0,         true},
                {"aptCreatedBy_usrID",  S(quint32),            QFV.integer().minValue(1),      QInvalid,  true},
                {"aptCreationDateTime", S(TAPI::DateTime_t),   QFV,                            QNull,     true},
                {"aptUpdatedBy_usrID",  S(quint32),            QFV.integer().minValue(1),      QNull,     true},
                {"aptStatus",           S(TAPI::enuAPITokensStatus::Type),QFV,                 TAPI::enuAPITokensStatus::Active},
              },
              { ///< Col               Reference Table    ForeignCol   Rename     LeftJoin
                {"apt_svcID",          "AAA.tblServices", "svcID",     "",         true},
                {"apt_usrID",          "AAA.tblUser",     "usrID",     "Owner_",   true},
                {"aptCreatedBy_usrID", "AAA.tblUser",     "usrID",     "Creator_", true},
                {"aptUpdatedBy_usrID", "AAA.tblUser",     "usrID",     "Updater_", true}
              })
{
    TAPI_REGISTER_TARGOMAN_ENUM(TAPI::enuAPITokensStatus);
}

}
}
}
