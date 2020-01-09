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

bool User::apiDELETE(DELETE_METHOD_ARGS_IMPL)
{
    Authorization::checkPriv(_JWT, this->privOn(EHTTP_DELETE,this->moduleName()));
    return this->deleteByPKs(AAADACInstance(), DELETE_METHOD_CALL_ARGS);
}

bool User::apiUPDATE(UPDATE_METHOD_ARGS_IMPL)
{
    Authorization::checkPriv(_JWT, this->privOn(EHTTP_PATCH,this->moduleName()));
    return this->update(AAADACInstance(), UPDATE_METHOD_CALL_ARGS);
}

bool User::apiUPDATEprofile(QHttp::JWT_t _JWT,
                            Targoman::API::enuUserSex::Type _sex,
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

    if(_name.size() || _family.size() || _sex != enuUserSex::NotExpressed)
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

quint32 User::apiCREATE(CREATE_METHOD_ARGS_IMPL)
{
    Authorization::checkPriv(_JWT, this->privOn(EHTTP_PUT,this->moduleName()));
    if(_ORMFILTERS.value("usrEmail").toString().isEmpty() && _ORMFILTERS.value("usrMobile").toString().isEmpty())
        throw exHTTPBadRequest("Either email or mobile must be provided to create user");

    return this->create(AAADACInstance(), CREATE_METHOD_CALL_ARGS).toUInt();
}

User::User() : clsTable("AAA",
                         "tblUser",
                         { ///<ColName             Type                      Validation                       Default    RO   Sort  Filter Self  Virt   PK
                           {"usrID",               S(quint32),          QFV.integer().minValue(1),            QInvalid, true, true, true, true, false, true},
                           {"usrSex",              S(Targoman::API::enuUserSex::Type), QFV,                   Targoman::API::enuUserSex::NotExpressed},
                           {"usrName",             S(QString),          QFV.unicodeAlNum().maxLenght(100),    QNull},
                           {"usrFamily",           S(QString),          QFV.unicodeAlNum().maxLenght(100),    QNull},
                           {"usrEmail",            S(QHttp::Email_t),   QFV.emailNotFake(),                   QNull},
                           {"usrMobile",           S(QHttp::Mobile_t),  QFV,                                  QNull},
                           {"usrApprovalState",    S(Targoman::API::enuUserApproval::Type),QFV,               Targoman::API::enuUserApproval::None},
                         //{"usrPass"},
                           {"usr_rolID",           S(quint32),          QFV.integer().minValue(1),            QInvalid},
                           {"usrSpecialPrivs",     S(QHttp::JSON_t),    QFV,                                  QNull,   false,false,false},
                           {"usrLanguage",         S(QString),          QFV.languageCode(),                   "fa"},
                           {"usrMaxSessions",      S(quint32),          QFV.integer().betweenValues(-1, 100), -1},
                           {"usrActiveSessions",   S(quint32),          QFV.integer().betweenValues(-1, 1000),QInvalid, true},
                           {"usrLastLogin",        S(QHttp::DateTime_t),QFV,                                  QInvalid, true},
                           {"usrCreatedBy_usrID",  S(quint32),          QFV.integer().minValue(1),            QInvalid, true},
                           {"usrCreationDateTime", S(QHttp::DateTime_t),QFV,                                  QNull,    true},
                           {"usrUpdatedBy_usrID",  S(quint32),          QFV.integer().minValue(1),            QNull,    true},
                           {"usrStatus",           S(Targoman::API::enuUserStatus::Type), QFV,                Targoman::API::enuUserStatus::MustValidate},
                         },
                         { ///< Col               Reference Table          ForeignCol    Rename     LeftJoin
                           {"usr_rolID",          "AAA.tblRoles",          "rolID"},
                           {"usrID",              "AAA.tblUserExtraInfo",  "uei_usrID",  "",          true},
                           {"usrCreatedBy_usrID", "AAA.tblUser",           "usrID",      "Creator_",  true},
                           {"usrUpdatedBy_usrID", "AAA.tblUser",           "usrID",      "Updater_",  true},
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
               {  ///<ColName             Type                      Validation                      Default    RO   Sort  Filter Self  Virt   PK
//                   {"uei_usrID",          S(quint32),               QFV.integer().minValue(1),       ORM_PRIMARY_KEY},
                   {"ueiExtraInfo",       S(QString),               QFV,                            QNull,  false,false,false},
                   {"ueiPhoto",           S(QHttp::Base64Image_t),  QFV,                            QNull,  false,false,false},
                   {"ueiUpdatedBy_usrID", S(quint32),               QFV.integer().minValue(1),      QNull},
                   {"ueiOAuthAccounts",   S(QHttp::JSON_t),         QFV,                            QNull}
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
