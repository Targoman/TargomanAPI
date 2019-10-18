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
        Authorization::checkPriv(_JWT,{"Account:User:CRUD~0100"});

    return this->selectFromTable(
                AAADACInstance(),
                {},
                {},
                GET_METHOD_CALL_ARGS);
}

bool User::apiUPDATEprofile(QHttp::JWT_t _JWT,
                            QString _name,
                            QString _family,
                            QHttp::Email_t _email,
                            QHttp::Mobile_t _mobile){
    return this->update(AAADACInstance(),
                        {{"usrID", clsJWT(_JWT).usrID()}},
                        {
                            {"usrName",   _name},
                            {"usrFamily", _family},
                            {"usrEmail",  _email},
                            {"usrMobile", _mobile},
                        }
                        );
}

bool User::apiUPDATE(QHttp::JWT_t _JWT,
                     quint64 _userID,
                     QString _name,
                     QString _family,
                     QHttp::Email_t _email,
                     QHttp::Mobile_t _mobile,
                     enuUserApproval::Type _approvalState,
                     quint64 _roleID,
                     QHttp::JSON_t _specialPrivs,
                     enuUserStatus::Type _status)
{
    Authorization::checkPriv(_JWT,{"Account:User:CRUD~0010"});
    return this->update(AAADACInstance(),
                        {{"usrID", _userID}},
                        {
                            {"usrName", _name},
                            {"usrFamily", _family},
                            {"usrEmail", _email},
                            {"usrMobile", _mobile},
                            {"usrApprovalState", _approvalState},
                            {"usr_rolID", _roleID == 0 ? QVariant() : _roleID},
                            {"usrSpecialPrivs", _specialPrivs},
                            {"usrUpdatedBy_usrID", clsJWT(_JWT).usrID()},
                            {"usrStatus", _status},
                        }
                        );
}

quint32 User::apiCREATE(QHttp::JWT_t _JWT,
                        QString _name,
                        QString _family,
                        QHttp::Email_t _email,
                        QHttp::Mobile_t _mobile,
                        Targoman::API::enuUserApproval::Type _approvalState,
                        quint64 _roleID,
                        qint8 _maxSessions,
                        QHttp::JSON_t _specialPrivs,
                        Targoman::API::enuUserStatus::Type _status)
{
    Authorization::checkPriv(_JWT,{"Account:User:CRUD~1000"});
    if(_email.isEmpty() && _mobile.isEmpty())
        throw exHTTPBadRequest("Either email or mobile must be provided to create user");

    return this->create(AAADACInstance(),
                        {
                            {"usrName", _name},
                            {"usrFamily", _family},
                            {"usrEmail", _email},
                            {"usrMobile", _mobile},
                            {"usrApprovalState", _approvalState},
                            {"usr_rolID", _roleID == 0 ? QVariant() : _roleID},
                            {"usrSpecialPrivs", _specialPrivs},
                            {"usrMaxSessions", _maxSessions},
                            {"usrUpdatedBy_usrID", clsJWT(_JWT).usrID()},
                            {"usrStatus", _status},
                        }
                        ).toUInt();
}

User::User() :  intfTable("AAA",
                          "tblUser",
                          "usr",
                          { ///<ColName                  Validation                            Sort  Filter AS  RO    PK
                            {"usrID",               QFV.integer().minValue(1),            true, true,  "", true, true},
                            {"usrName",             QFV.unicodeAlNum().maxLenght(100)},
                            {"usrFamily",           QFV.unicodeAlNum().maxLenght(100)},
                            {"usrEmail",            QFV.emailNotFake()},
                            {"usrMobile",           QFV.mobile()},
                            {"usrApprovalState"},
                          //{"usrPass"},
                            {"usr_rolID",           QFV.integer().minValue(1)},
                            {"usrSpecialPrivs",     QFV.json(),                            false, false},
                            {"usrMaxSessions",      QFV.integer().betweenValues(-1, 100)},
                            {"usrActiveSessions",   QFV.integer().betweenValues(-1, 1000), true, true, "", true},
                            {"usrLastLogin",        QFV.dateTime(),                        true, true, "", true},
                            {"usrCreatedBy_usrID",  QFV.integer().minValue(1),             true, true, "", true},
                            {"usrCreationDateTime", QFV.dateTime(),                        true, true, "", true},
                            {"usrUpdatedBy_usrID",  QFV.integer().minValue(1)},
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

bool UserExtraInfo::apiUPDATEPhoto(QHttp::JWT_t _JWT, QHttp::Base64Image_t _image){
    clsDACResult Result = AAADACInstance().execQuery(
                              "",
                              "UPDATE " + this->Name
                              + QUERY_SEPARATOR
                              + "SET ueiPhoto = ?, ueiUpdatedBy_usrID = ?"
                              + QUERY_SEPARATOR
                              + "WHERE uei_usrID = ?",
                              { _image, clsJWT(_JWT).usrID(), clsJWT(_JWT).usrID() }
        );

    return Result.numRowsAffected() > 0;
}

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
    this->registerMyRESTAPIs();
}
