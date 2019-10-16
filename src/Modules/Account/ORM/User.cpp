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

#include "User.h"
#include "Helpers/AAA/AAA.hpp"

using namespace Targoman;
using namespace Targoman::API;
using namespace QHttp;

void User::init()
{
    UserExtraInfo::instance().init();
}

QVariant User::apiGET(GET_METHOD_ARGS_IMPL)
{
    if(clsJWT(_JWT).usrID() != _EXTRAPATH.toUInt())
        Authorization::hasPriv(_JWT,{"Account:CRUD~0100"});

    return this->selectFromTable(
                AAADACInstance(),
                {},
                {},
                GET_METHOD_CALL_ARGS);
}

User::User() :  intfTable("AAA",
                          "tblUser",
                          "usr",
                          { ///<ColName                  Validation                            Sort  Filter AS  RO    PK
                            {"usrID",                    QFV.integer().minValue(1),            true, true,  "", true, true},
                            {"usrName",                  QFV.unicodeAlNum().maxLenght(100)},
                            {"usrFamily",                QFV.unicodeAlNum().maxLenght(100)},
                            {"usrEmail",                 QFV.emailNotFake()},
                            {"usrMobile",                QFV.mobile()},
                            {"usrApprovalState"},
                            //{"usrPass"},
                            {"usr_rolID",                QFV.integer().minValue(1)},
                            {"usrSpecialPrivs",          QFV.json(),                            false, false},
                            {"usrMaxConcurrentSessions", QFV.integer().betweenValues(-1, 100)},
                            {"usrActiveSessions",        QFV.integer().betweenValues(-1, 1000), true, true, "", true},
                            {"usrLastLogin",             QFV.dateTime(),                        true, true, "", true},
                            {"usrCreatedBy_usrID",       QFV.integer().minValue(1),             true, true, "", true},
                            {"usrCreationDateTime",      QFV.dateTime(),                        true, true, "", true},
                            {"usrUpdatedBy_usrID",       QFV.integer().minValue(1)},
                            {"usrStatus"},
                          },
                          { ///< Col               Reference Table          ForeignCol    Rename     LeftJoin
                            {"usr_rolID",          "AAA.tblRoles",          "rolID"},
                            {"usrID",              "AAA.tblUserExtraInfo",  "uei_usrID",  "",          true},
                            {"usrCreatedBy_usrID", "AAA.tblUser",           "usrID",      "Creator_",  true},
                            {"usrUpdatedBy_usrID", "AAA.tblUser",           "usrID",      "Updater_",  true}
                          })
{
    this->registerMyRESTAPIs();
}



void UserExtraInfo::init()
{;}

UserExtraInfo::UserExtraInfo() :
    intfTable ("AAA",
               "tblUserExtraInfo",
               "uei",
               {///<ColName    Validation                     Sort  Filter    AS  RO   PK
                   {"ueiExtraInfo",       QFV.allwaysValid(), false, false},
                   {"ueiPhoto",           QFV.allwaysValid(), false, false},
                   {"ueiUpdatedBy_usrID", QFV.integer().minValue(1)},
                   {"ueiOAuthAccounts",   QFV.allwaysInvalid()}
               },
               { ///< Col                 Reference Table       ForeignCol     Rename     LeftJoin
                   {"ueiUpdatedBy_usrID", "AAA.tblUser",        "usrID",      "InfoUpdater_", true}
               })
{

}
