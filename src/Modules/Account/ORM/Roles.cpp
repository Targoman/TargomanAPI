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

#include "Roles.h"
#include "Helpers/AAA/AAA.hpp"

class Roles;
void Roles::init()
{

}

QVariant Roles::apiGET(GET_METHOD_ARGS_IMPL)
{
    Authorization::hasPriv(_JWT,{"Account:CRUD~0100"});

    return this->selectFromTable(AAADACInstance(), {}, {}, GET_METHOD_CALL_ARGS);
}

Roles::Roles() :  intfTable("AAA",
                          "tblRoles",
                          "rol",
                          {
                            {"rolID", QFieldValidator().integer().minValue(1), true, true, {}, true},
                            {"rolName", QFieldValidator().unicodeAlNum().maxLenght(100)},
                            {"usrFamilyName", QFieldValidator().unicodeAlNum().maxLenght(100)},
                            {"usrEmail", QFieldValidator().emailNotFake()},
                            {"usrMobile", QFieldValidator().mobile()},
                            {"usrApprovalState"},
                            //{"usrPass"},
                            {"rolParent_rolID", QFieldValidator().integer().minValue(1)},
                            {"rolPrivileges", QFieldValidator().json(), false, false},
                            {"rolCreatedBy_usrID", QFieldValidator().integer().minValue(1)},
                            {"rolCreationDateTime", QFieldValidator().dateTime()},
                            {"rolUpdatedBy_usrID", QFieldValidator().integer().minValue(1)},
                            {"rolStatus"},
                          },
                          {
                            {"rolParent_rolID",    "AAA.tblRoles", "rolID", "Parent_", true},
                            {"rolCreatedBy_usrID", "AAA.tblUser",  "usrID", "Creator_", true},
                            {"rolUpdatedBy_usrID", "AAA.tblUser",  "usrID", "Updater_", true}
                          })
{
    this->registerMyRESTAPIs();
}


