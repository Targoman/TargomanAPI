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

namespace Targoman {
namespace API {
namespace AAA {
using namespace QHttp;

void User::init()
{
    UserExtraInfo::instance().init();
}

QVariant User::apiGET(GET_METHOD_ARGS_IMPL)
{
    if(clsJWT(_JWT).usrID() != _EXTRAPATH.toUInt())
        Authorization::checkPriv(_JWT, {"Account:User:CRUD~0100"});

    return this->selectFromTable(
                AAADACInstance(),
                {},
                {},
                GET_METHOD_CALL_ARGS);
}

bool User::apiUPDATEprofile(QHttp::JWT_t _JWT,
                            Targoman::API::enuUserSex::Type,
                            QString _name,
                            QString _family,
                            QHttp::ISO639_2_t _lang,
                            QHttp::Email_t _email,
                            QHttp::Mobile_t _mobile,
                            QHttp::MD5_t _pass,
                            QString _salt){
    if(_email.size() || _mobile.size ()){
        QFV.asciiAlNum().maxLenght(20).validate(_salt, "salt");
        if(_pass.isEmpty())
            throw exHTTPBadRequest("Password and salt are required to change email");
    }

    if(_email.size())
        AAADACInstance().callSP("","AAA.sp_CREATE_approvalRequest",{
                                    {"iWhat2Approve", "E"},
                                    {"iUserID", clsJWT(_JWT).usrID()},
                                    {"iValue", _email},
                                    {"iPass", _pass},
                                    {"iSalt", _salt},
                                });
    if(_mobile.size())
        AAADACInstance().callSP("","AAA.sp_CREATE_approvalRequest",{
                                    {"iWhat2Approve", "E"},
                                    {"iUserID", clsJWT(_JWT).usrID()},
                                    {"iValue", _email},
                                    {"iPass", _pass},
                                    {"iSalt", _salt},
                                });

    if(_name.size() || _family.size())
        return this->update(AAADACInstance(),
                            {{"usrID", clsJWT(_JWT).usrID()}},
                            {
                                {"usrName",   _name},
                                {"usrFamily", _family},
                                {"usrLanguage", _lang}
                            }
                            );
    return true;
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
//    Authorization::checkPriv(_JWT,{"Account:User:CRUD~0010"});
//    return this->update(AAADACInstance(),
//                        {{"usrID", _userID}},
//                        {
//                            {"usrName", _name},
//                            {"usrFamily", _family},
//                            {"usrEmail", _email},
//                            {"usrMobile", _mobile},
//                            {"usrApprovalState", _approvalState},
//                            {"usr_rolID", _roleID == 0 ? QVariant() : _roleID},
//                            {"usrSpecialPrivs", _specialPrivs},
//                            {"usrUpdatedBy_usrID", clsJWT(_JWT).usrID()},
//                            {"usrStatus", _status},
//                        }
//                        );
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
//    Authorization::checkPriv(_JWT,{"Account:User:CRUD~1000"});
//    if(_email.isEmpty() && _mobile.isEmpty())
//        throw exHTTPBadRequest("Either email or mobile must be provided to create user");

//    return this->create(AAADACInstance(),
//                        {
//                            {"usrName", _name},
//                            {"usrFamily", _family},
//                            {"usrEmail", _email},
//                            {"usrMobile", _mobile},
//                            {"usrApprovalState", _approvalState},
//                            {"usr_rolID", _roleID == 0 ? QVariant() : _roleID},
//                            {"usrSpecialPrivs", _specialPrivs},
//                            {"usrMaxSessions", _maxSessions},
//                            {"usrUpdatedBy_usrID", clsJWT(_JWT).usrID()},
//                            {"usrStatus", _status},
//                        }
//                        ).toUInt();
}

User::User() : clsTable("AAA",
                         "tblUser",
                         { ///<ColName             Type                      Validation                       RO   Sort  Filter Self  Virt   PK
                           {"usrID",               S(quint32),          QFV.integer().minValue(1),            ORM_PRIMARY_KEY},
                           {"usrSex",              S(Targoman::API::enuUserSex::Type)},
                           {"usrName",             S(QString),          QFV.unicodeAlNum().maxLenght(100)},
                           {"usrFamily",           S(QString),          QFV.unicodeAlNum().maxLenght(100)},
                           {"usrEmail",            S(QHttp::Email_t),   QFV.emailNotFake()},
                           {"usrMobile",           S(QHttp::Mobile_t),  QFV},
                           {"usrApprovalState",    S(Targoman::API::enuUserApproval::Type)},
                         //{"usrPass"},
                           {"usr_rolID",           S(quint32),          QFV.integer().minValue(1)},
                           {"usrSpecialPrivs",     S(QHttp::JSON_t),    QFV,                                  false,false,false},
                           {"usrLanguage",         S(QString),          QFV.languageCode()},
                           {"usrMaxSessions",      S(quint32),          QFV.integer().betweenValues(-1, 100)},
                           {"usrActiveSessions",   S(quint32),          QFV.integer().betweenValues(-1, 1000), true},
                           {"usrLastLogin",        S(QHttp::DateTime_t),QFV,                                   true},
                           {"usrCreatedBy_usrID",  S(quint32),          QFV.integer().minValue(1),             true},
                           {"usrCreationDateTime", S(QHttp::DateTime_t),QFV,                                   true},
                           {"usrUpdatedBy_usrID",  S(quint32),          QFV.integer().minValue(1)},
                           {"usrStatus",           S(Targoman::API::enuUserStatus::Type)},
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
    clsTable ("AAA",
               "tblUserExtraInfo",
               {  ///<ColName             Type                      Validation                        RO   Sort  Filter Self  Virt   PK
                   {"uei_usrID",          S(quint32),               QFV.integer().minValue(1),       ORM_PRIMARY_KEY},
                   {"ueiExtraInfo",       S(QString),               QFV,                            false,false,false},
                   {"ueiPhoto",           S(QHttp::Base64Image_t),  QFV,                            false,false,false},
                   {"ueiUpdatedBy_usrID", S(quint32),               QFV.integer().minValue(1)},
                   {"ueiOAuthAccounts",   S(QHttp::JSON_t)}
               },
               { ///< Col                 Reference Table       ForeignCol     Rename     LeftJoin
                   {"ueiUpdatedBy_usrID", "AAA.tblUser",        "usrID",      "InfoUpdater_", true}
               })
{
    this->registerMyRESTAPIs();
}

}
}
}
