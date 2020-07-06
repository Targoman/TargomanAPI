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
 @author S. Mehran M. Ziabary <ziabary@targoman.com>
 */

#ifndef TARGOMAN_API_MODULES_ACCOUNT_ORM_USER_H
#define TARGOMAN_API_MODULES_ACCOUNT_ORM_USER_H

#include "Interfaces/ORM/clsTable.h"
#include "Interfaces/AAA/AAA.hpp"

namespace Targoman {
namespace API {
namespace AAA {

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-variable"
namespace tblUserExtraInfo {
constexpr char Name[] = "tblUserExtraInfo";
TARGOMAN_CREATE_CONSTEXPR(uei_usrID);
TARGOMAN_CREATE_CONSTEXPR(ueiGender);
TARGOMAN_CREATE_CONSTEXPR(ueiExtraInfo);
TARGOMAN_CREATE_CONSTEXPR(ueiPhoto);
TARGOMAN_CREATE_CONSTEXPR(ueiSheba);
TARGOMAN_CREATE_CONSTEXPR(ueiUpdatedBy_usrID);
TARGOMAN_CREATE_CONSTEXPR(ueiOAuthAccounts);
}
#pragma GCC diagnostic pop

class User : public ORM::clsTable
{
    Q_OBJECT
private slots:
    QVariant ORMGET("Get user information")
    bool ORMDELETE("Delete a User by priviledged user")
    bool ORMUPDATE("Update User info by priviledged user")
    quint32 ORMCREATE("Create a new user by priviledged user. Email or Mobile is required")

    bool REST(UPDATE,profile,(TAPI::JWT_t _JWT,
                              NULLABLE(TAPI::enuUserGender::Type) _gender = {},
                              NULLABLE(QString) _name = {},
                              NULLABLE(QString) _family = {},
                              NULLABLE(TAPI::ISO639_2_t) _lang = {},
                              NULLABLE(TAPI::Email_t) _email = {},
                              NULLABLE(TAPI::Mobile_t) _mobile = {},
                              NULLABLE(TAPI::MD5_t) _pass = {},
                              NULLABLE(QString) _salt = {}),
              "Update User profile. Take note that this method does not change password "
              "Password and Salt are required to change email or mobile")

    private:
        TARGOMAN_DEFINE_API_SUBMODULE(Account,User)
};

    class UserExtraInfo : public ORM::clsTable
    {
        Q_OBJECT
    private slots:
        bool REST(UPDATE,Photo,(TAPI::JWT_t _JWT, TAPI::Base64Image_t _image),
                  "Updates user image based using a base64 encoded image")
        bool REST(UPDATE,Sheba,(TAPI::JWT_t _JWT, TAPI::Sheba_t _sheba),
                  "Updates user image based using a base64 encoded image")
        private:
            TARGOMAN_DEFINE_API_SUBMODULE(Account,UserExtraInfo)
    };

    }
}
}



#endif // TARGOMAN_API_MODULES_ACCOUNT_ORM_USER_H
