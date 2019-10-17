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

#include "APIToken.h"
#include "Helpers/AAA/AAA.hpp"

using namespace Targoman;
using namespace Targoman::API;
using namespace QHttp;

void APIToken::init()
{;}

QVariant APIToken::apiGET(GET_METHOD_ARGS_IMPL)
{
    Authorization::hasPriv(_JWT,{"Account:APIToken:CRUD~0100"});
    return this->selectFromTable(AAADACInstance(), {}, {}, GET_METHOD_CALL_ARGS);
}

bool APIToken::apiUPDATE(QHttp::JWT_t _JWT,
                         quint64 _tokenID,
                         QString _token,
                         quint32 _userID,
                         quint32 _rolID,
                         QHttp::ISO639_2_t _lang,
                         bool    _validateIP,
                         QHttp::Date_t _expiryDate,
                         QHttp::JSON_t _extraPrivs,
                         Targoman::API::enuAPITokenStatus::Type _status)
{
    Authorization::hasPriv(_JWT,{"Account:APIToken:CRUD~0010"});
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
}

bool APIToken::apiDELETE(QHttp::JWT_t _JWT, QHttp::ExtraPath_t _EXTRAPATH)
{
    Authorization::hasPriv(_JWT,{"Account:APIToken:CRUD~0001"});
    return this->deleteByPKs(AAADACInstance(), {{this->Cols.first().Name, _EXTRAPATH}});
}

quint32 APIToken::apiCREATE(QHttp::JWT_t _JWT,
                            QString _token,
                            quint32 _userID,
                            quint32 _rolID,
                            QHttp::ISO639_2_t _lang,
                            bool    _validateIP,
                            QHttp::Date_t _expiryDate,
                            QHttp::JSON_t _extraPrivs,
                            Targoman::API::enuAPITokenStatus::Type _status)
{
    Authorization::hasPriv(_JWT,{"Account:APIToken:CRUD~1000"});
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
}

APIToken::APIToken() :
    intfTable("AAA",
              "tblAPIToken",
              "apt",
              { ///<ColName            Validation                           Sort  Filter AS  RO   PK
                {"aptID",               QFV.integer().minValue(1),          true, true, "",  true, true},
                {"aptToken",            QFV.asciiAlNum().maxLenght(50),     true, true, "", false, true},
                {"apt_usrID",           QFV.integer().minValue(1)},
                {"apt_rolID",           QFV.integer().minValue(1)},
                {"aptLang",             QFV.languageCode()},
                {"aptValidateIP",       QFV.boolean()},
                {"aptExtraPriviledges", QFV.json(),                         false, false},
                {"aptExpiryDate",       QFV.dateTime()},
                {"aptLastActivity",     QFV.dateTime(),                     true,  true, "", true},
                {"aptAccessCount",      QFV.integer().minValue(1),          true,  true, "", true},
                {"aptCreatedBy_usrID",  QFV.integer().minValue(1),          true,  true, "", true},
                {"aptCreationDateTime", QFV.dateTime(),                     true,  true, "", true},
                {"aptUpdatedBy_usrID",  QFV.integer().minValue(1)},
                {"aptStatus",           QFV.matches(QRegularExpression(QString("^[%1]$").arg(enuAPITokenStatus::options().join("|"))))},
              },
              { ///< Col               Reference Table    ForeignCol   Rename     LeftJoin
                {"apt_rolID",          "AAA.tblRoles",    "rolID",     "",         true},
                {"apt_usrID",          "AAA.tblUser",     "rolID",     "Owner_",   true},
                {"rolCreatedBy_usrID", "AAA.tblUser",     "usrID",     "Creator_", true},
                {"rolUpdatedBy_usrID", "AAA.tblUser",     "usrID",     "Updater_", true}
              })
{
    QHTTP_REGISTER_TARGOMAN_ENUM(Targoman::API::enuAPITokenStatus);
    this->registerMyRESTAPIs();
}
