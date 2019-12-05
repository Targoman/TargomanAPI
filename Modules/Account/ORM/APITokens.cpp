/******************************************************************************
#   TargomanAPI: REST API for Targoman
#
#   Copyright 2014-2019 by Targoman Intelligent Processing <http://tip.co.ir>
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
#include "Helpers/AAA/AAA.hpp"

namespace Targoman {
namespace API {
namespace AAA {
using namespace QHttp;

void APITokens::init()
{;}

QVariant APITokens::apiGET(GET_METHOD_ARGS_IMPL)
{
    if(!this->isSelf({{"apt_usrID", clsJWT(_JWT).usrID()}}, _EXTRAPATH, _DIRECTFILTERS, _filters))
        Authorization::checkPriv(_JWT, {"Account:APITokens:CRUD~0100"});

    return this->selectFromTable(AAADACInstance(), {}, {}, GET_METHOD_CALL_ARGS);
}

bool APITokens::apiUPDATE(QHttp::JWT_t _JWT,
                         quint64 _tokenID,
                         QString _token,
                         quint32 _userID,
                         quint32 _rolID,
                         QHttp::ISO639_2_t _lang,
                         bool    _validateIP,
                         QHttp::Date_t _expiryDate,
                         QHttp::JSON_t _extraPrivs,
                         Targoman::API::enuAPITokensStatus::Type _status)
{
/*    Authorization::checkPriv(_JWT,{"Account:APITokens:CRUD~0010"});
    return this->update(AAADACInstance(),
                        {{"aptID", _tokenID}},
                        {
                            {"aptToken", _token},
                            {"apt_usrID", _userID == 0 ? QVariant() : _userID},
                            {"apt_rolID", _rolID  == 0 ? QVariant() : _rolID},
                            {"aptLang", _lang},
                            {"aptValidateIP", _validateIP},
                            {"aptExtraPriviledges", _extraPrivs},
                            {"aptExpiryDate", _expiryDate},
                            {"aptUpdatedBy_usrID", clsJWT(_JWT).usrID()},
                            {"aptStatus", _status}
                        }
                        );
                        */
}

bool APITokens::apiDELETE(QHttp::JWT_t _JWT, QHttp::ExtraPath_t _EXTRAPATH)
{
/*    Authorization::checkPriv(_JWT,{"Account:APITokens:CRUD~0001"});
    return this->deleteByPKs(AAADACInstance(), {{this->Cols.first().Name, _EXTRAPATH}});*/
}

quint32 APITokens::apiCREATE(QHttp::JWT_t _JWT,
                            QString _token,
                            quint32 _userID,
                            quint32 _rolID,
                            QHttp::ISO639_2_t _lang,
                            bool    _validateIP,
                            QHttp::Date_t _expiryDate,
                            QHttp::JSON_t _extraPrivs,
                            Targoman::API::enuAPITokensStatus::Type _status)
{
/*    Authorization::checkPriv(_JWT,{"Account:APITokens:CRUD~1000"});
    return this->create(AAADACInstance(),
                        {
                            {"aptToken", _token},
                            {"apt_usrID", _userID},
                            {"apt_rolID", _rolID},
                            {"aptLang", _lang},
                            {"aptValidateIP", _validateIP},
                            {"aptExtraPriviledges", _extraPrivs},
                            {"aptExpiryDate", _expiryDate},
                            {"aptStatus", _status}
                        }
                        ).toUInt();
                        */
}

APITokens::APITokens() :
    clsTable("AAA",
              "tblAPITokens",
              { ///<ColName             Type                    Validation                       RO   Sort  Filter Self  Virt   PK
                {"aptID",               S(quint64),             QFV.integer().minValue(1),      ORM_PRIMARY_KEY},
                {"aptToken",            S(QString),             QFV.asciiAlNum().maxLenght(50), true, true, false},
                {"apt_usrID",           S(quint32),             QFV.integer().minValue(1)},
                {"apt_rolID",           S(quint32),             QFV.integer().minValue(1)},
                {"aptLang",             S(QHttp::ISO639_2_t),   QFV},
                {"aptValidateIP",       S(bool),                QFV},
                {"aptExtraPriviledges", S(QHttp::JSON_t),       QFV,                           false,false, false},
                {"aptExpiryDate",       S(QHttp::DateTime_t),   QFV},
                {"aptLastActivity",     S(QHttp::DateTime_t),   QFV,                            true},
                {"aptAccessCount",      S(quint32),             QFV.integer().minValue(1),      true},
                {"aptCreatedBy_usrID",  S(quint32),             QFV.integer().minValue(1),      true},
                {"aptCreationDateTime", S(QHttp::DateTime_t),   QFV,                            true},
                {"aptUpdatedBy_usrID",  S(quint32),             QFV.integer().minValue(1)},
                {"aptStatus",           S(Targoman::API::enuAPITokensStatus::Type)},
              },
              { ///< Col               Reference Table    ForeignCol   Rename     LeftJoin
                {"apt_rolID",          "AAA.tblRoles",    "rolID",     "",         true},
                {"apt_usrID",          "AAA.tblUser",     "rolID",     "Owner_",   true},
                {"rolCreatedBy_usrID", "AAA.tblUser",     "usrID",     "Creator_", true},
                {"rolUpdatedBy_usrID", "AAA.tblUser",     "usrID",     "Updater_", true}
              })
{
    QHTTP_REGISTER_TARGOMAN_ENUM(Targoman::API::enuAPITokensStatus);
    this->registerMyRESTAPIs();
}

}
}
}
