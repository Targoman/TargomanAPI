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
#include "Helpers/AAA/UserEnums.hpp"
//#include "Roles.h"

class Roles;
void User::init()
{

}

QVariant User::apiGET(GET_METHOD_ARGS_IMPL)
{
    if(clsJWT(_JWT).usrID() != _EXTRAPATH.toUInt())
        Authorization::hasPriv(_JWT,{"Account:CRUD~0100"});

    return this->selectFromTable(AAADACInstance(), {}, {}, GET_METHOD_CALL_ARGS);
}

User::User() :  intfTable("AAA",
                          "tblUser",
                          "usr",
                          {
                            {"usrID", QFieldValidator().integer().minValue(1), true, true, {}, true},
                            {"usrGivenName", QFieldValidator().unicodeAlNum().maxLenght(100)},
                            {"usrFamilyName", QFieldValidator().unicodeAlNum().maxLenght(100)},
                            {"usrEmail", QFieldValidator().emailNotFake()},
                            {"usrMobile", QFieldValidator().mobile()},
                            {"usrApprovalState"},
                            //{"usrPass"},
                            {"usr_rolID", QFieldValidator().integer().minValue(1)},
                            {"usrSpecialPrivs", QFieldValidator().json(), false, false},
                            {"usrMaxConcurrentSessions", QFieldValidator().integer().betweenValues(-1, 100)},
                            {"usrActiveSessions", QFieldValidator().integer().betweenValues(-1, 1000)},
                            {"usrLastLogin", QFieldValidator().dateTime()},
                            {"usrCreatedBy_usrID", QFieldValidator().integer().minValue(1)},
                            {"usrCreationDateTime", QFieldValidator().dateTime()},
                            {"usrUpdatedBy_usrID", QFieldValidator().integer().minValue(1)},
                            {"usrStatus"},
                          },
                          {
                            {"usr_rolID", "AAA.Roles", "rolID"},
                            {"usrCreatedBy_usrID", "AAA.User", "usrID", "Creator_", true},
                            {"usrCreatedBy_usrID", "AAA.User", "usrID", "Updater_", true}
                          })
{
    this->registerMyRESTAPIs();
}


