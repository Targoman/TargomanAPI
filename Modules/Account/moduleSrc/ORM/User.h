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
 * @author S. Mehran M. Ziabary <ziabary@targoman.com>
 * @author Kambiz Zandi <kambizzandi@gmail.com>
 */

#ifndef TARGOMAN_API_MODULES_ACCOUNT_ORM_USER_H
#define TARGOMAN_API_MODULES_ACCOUNT_ORM_USER_H

#include "Interfaces/AAA/AAA.hpp"
#include "Interfaces/API/intfSQLBasedModule.h"

namespace Targoman::API::AccountModule {

/*****************************************************************\
|* User **********************************************************|
\*****************************************************************/

//structures and enumes goes here

namespace ORM {

class User : public intfSQLBasedModule
{
    Q_OBJECT
    TARGOMAN_DEFINE_API_SUBMODULE(Account, User)

private slots:
    QVariant ORMGET("Get user information")
    quint64 ORMCREATE("Create a new user by priviledged user. Email or Mobile is required")
    bool ORMUPDATE("Update User info by priviledged user")
    bool ORMDELETE("Delete a User by priviledged user")

    TAPI::RawData_t REST_GET(
        photo,
        (
            TAPI::JWT_t _JWT,
            quint64 _usrID
        ),
        "Get user photo as image"
    )

    bool REST_UPDATE(
        photo,
        (
            TAPI::JWT_t _JWT,
            TAPI::Base64Image_t _image
        ),
        "Updates user image based using a base64 encoded image"
    )

    bool REST_UPDATE(
        email,
        (
            TAPI::JWT_t     _JWT,
            TAPI::Email_t   _email,
            TAPI::MD5_t     _psw,
            QString         _salt
        ),
        "Update user email"
    )

    bool REST_UPDATE(
        mobile,
        (
            TAPI::JWT_t     _JWT,
            TAPI::Mobile_t  _mobile,
            TAPI::MD5_t     _pass,
            QString         _salt
        ),
        "Update user mobile"
    )

    bool REST_UPDATE(
        personalInfo,
        (
            TAPI::JWT_t         _JWT,
            QString             _name = {},
            QString             _family = {},
            TAPI::ISO639_2_t    _language = {},
            NULLABLE_TYPE(TAPI::enuGender::Type) _gender = {}
        ),
        "Update User personal info"
    )

    bool REST_UPDATE(
        financialInfo,
        (
            TAPI::JWT_t     _JWT,
            TAPI::Sheba_t   _iban = {},
            TAPI::Ether_t   _ether = {}
        ),
        "Update User financial info"
    )

    bool REST_UPDATE(
        extraInfo,
        (
            TAPI::JWT_t     _JWT,
            NULLABLE_TYPE(TAPI::Date_t)    _birthDate = NULLABLE_NULL_VALUE,
            QString         _job = {},
            QString         _education = {},
            QString         _fieldOfStudy = {},
//            QString         _language = {},
            QString         _theme = {}
        ),
        "Update User extra info"
    )

};

} //namespace ORM

/*****************************************************************\
|* UserExtraInfo *************************************************|
\*****************************************************************/

//structures and enumes goes here

TARGOMAN_DEFINE_ENUM(enuUserExtraInfoJsonKey,
                     Job,
                     Education,
                     FieldOfStudy,
                     Language,
                     Theme
                     );

namespace ORM {

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-variable"
namespace tblUserExtraInfo {
    constexpr char Name[] = "tblUserExtraInfo";
    TARGOMAN_CREATE_CONSTEXPR(uei_usrID);
    //TARGOMAN_CREATE_CONSTEXPR(ueiGender);
    TARGOMAN_CREATE_CONSTEXPR(ueiBirthDate);
    TARGOMAN_CREATE_CONSTEXPR(ueiPhoto);
    TARGOMAN_CREATE_CONSTEXPR(ueiOAuthAccounts);
    TARGOMAN_CREATE_CONSTEXPR(ueiIBAN);
    TARGOMAN_CREATE_CONSTEXPR(ueiEther);
    TARGOMAN_CREATE_CONSTEXPR(ueiExtraInfo);
    TARGOMAN_CREATE_CONSTEXPR(ueiUpdatedBy_usrID);
}
#pragma GCC diagnostic pop

class UserExtraInfo : public intfSQLBasedModule
{
    Q_OBJECT
    TARGOMAN_DEFINE_API_SUBMODULE(Account, UserExtraInfo)

private slots:
//    bool REST_UPDATE(
//        birthDate,
//        (
//            TAPI::JWT_t _JWT,
//            QString _birthDate
//        ),
//        "Updates users birth date"
//    )

//    bool REST_UPDATE(
//        photo,
//        (
//            TAPI::JWT_t _JWT,
//            TAPI::Base64Image_t _image
//        ),
//        "Updates user image based using a base64 encoded image"
//    )

//    bool REST_UPDATE(
//        sheba,
//        (
//            TAPI::JWT_t _JWT,
//            TAPI::Sheba_t _sheba
//        ),
//        "Updates user Sheba address"
//    )

//    bool REST_UPDATE(
//        etherAddress,
//        (
//            TAPI::JWT_t _JWT,
//            TAPI::Ether_t _etherAddress
//        ),
//        "Updates user ethercoin address"
//    )
};

} //namespace ORM
} //namespace Targoman::API::AccountModule

TAPI_DECLARE_METATYPE_ENUM(Targoman::API::AccountModule, enuUserExtraInfoJsonKey);

#endif // TARGOMAN_API_MODULES_ACCOUNT_ORM_USER_H
