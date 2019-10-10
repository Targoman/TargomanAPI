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
{

}

QVariant APIToken::apiGET(GET_METHOD_ARGS_IMPL)
{
    Authorization::hasPriv(_JWT,{"Account:CRUD~0100"});

    return this->selectFromTable(AAADACInstance(), {}, {}, GET_METHOD_CALL_ARGS);
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
