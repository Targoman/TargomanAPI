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

#include "Interfaces/ORM/clsTable.h"
#include "Interfaces/AAA/AAA.hpp"

using namespace Targoman::API::ORM;

namespace Targoman::API::AccountModule {

//structures and enumes goes here

namespace ORM {

/*****************************************************************\
|* User **********************************************************|
\*****************************************************************/
class User : public clsTable
{
    Q_OBJECT

private slots:
    QVariant ORMGET("Get user information")
    quint64 ORMCREATE("Create a new user by priviledged user. Email or Mobile is required")
    bool ORMUPDATE("Update User info by priviledged user")
    bool ORMDELETE("Delete a User by priviledged user")

    TAPI::RawData_t REST(
        GET,
        photo,
        (
            TAPI::JWT_t _JWT,
            quint64 _usrID
        ),
        "Get user photo as image"
    )

    bool REST(
        UPDATE,
        profile,
        (
            TAPI::JWT_t _JWT,
            NULLABLE_TYPE(TAPI::enuUserGender::Type) _gender = {},
            NULLABLE_TYPE(QString) _name = {},
            NULLABLE_TYPE(QString) _family = {},
            NULLABLE_TYPE(TAPI::ISO639_2_t) _lang = {},
            NULLABLE_TYPE(TAPI::Email_t) _email = {},
            NULLABLE_TYPE(TAPI::Mobile_t) _mobile = {},
            NULLABLE_TYPE(TAPI::MD5_t) _pass = {},
            NULLABLE_TYPE(QString) _salt = {}
        ),
        "Update User profile. Take note that this method does not change password "
        "Password and Salt are required to change email or mobile"
    )

private:
    TARGOMAN_DEFINE_API_SUBMODULE(Account, User)
};

/*****************************************************************\
|* UserExtraInfo *************************************************|
\*****************************************************************/
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-variable"
namespace tblUserExtraInfo {
constexpr char Name[] = "tblUserExtraInfo";
TARGOMAN_CREATE_CONSTEXPR(uei_usrID);
TARGOMAN_CREATE_CONSTEXPR(ueiGender);
TARGOMAN_CREATE_CONSTEXPR(ueiExtraInfo);
TARGOMAN_CREATE_CONSTEXPR(ueiPhoto);
TARGOMAN_CREATE_CONSTEXPR(ueiIBAN);
TARGOMAN_CREATE_CONSTEXPR(ueiEther);
TARGOMAN_CREATE_CONSTEXPR(ueiOAuthAccounts);
TARGOMAN_CREATE_CONSTEXPR(ueiUpdatedBy_usrID);
}
#pragma GCC diagnostic pop

class UserExtraInfo : public clsTable
{
    Q_OBJECT

private slots:
    bool REST(
        UPDATE,
        photo,
        (
            TAPI::JWT_t _JWT,
            TAPI::Base64Image_t _image
        ),
        "Updates user image based using a base64 encoded image"
    )

    bool REST(
        UPDATE,
        sheba,
        (
            TAPI::JWT_t _JWT,
            TAPI::Sheba_t _sheba
        ),
        "Updates user Sheba address"
    )

    bool REST(
        UPDATE,
        etherAdress,
        (
            TAPI::JWT_t _JWT,
            TAPI::Ether_t _etherAddress
        ),
        "Updates user ethercoin address"
    )

private:
    TARGOMAN_DEFINE_API_SUBMODULE(Account, UserExtraInfo)
};

} //namespace ORM
} //namespace Targoman::API::AccountModule

#endif // TARGOMAN_API_MODULES_ACCOUNT_ORM_USER_H
