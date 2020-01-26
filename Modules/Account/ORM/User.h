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

#ifndef TARGOMAN_API_MODULES_ACCOUNT_ORM_USER_H
#define TARGOMAN_API_MODULES_ACCOUNT_ORM_USER_H

#include "QHttp/intfRESTAPIHolder.h"
#include "libTargomanDBM/clsDAC.h"
#include "Helpers/ORM/clsTable.h"
#include "Helpers/AAA/GenericEnums.hpp"

namespace Targoman {
namespace API {
namespace AAA {

class User : public clsTable
{
    Q_OBJECT
public:
    void init();

private slots:
    QVariant ORMGET("Get user information")
    bool ORMDELETE("Delete a User by priviledged user")
    bool ORMUPDATE("Update User info by priviledged user")
    quint32 ORMCREATE("Create a new user by priviledged user. Email or Mobile is required")

    bool API(UPDATE,profile,(QHttp::JWT_t _JWT,
                             Targoman::API::enuUserSex::Type _sex = {},
                             QString _name = {},
                             QString _family = {},
                             QHttp::ISO639_2_t _lang = {},
                             QHttp::Email_t _email = {},
                             QHttp::Mobile_t _mobile = {},
                             QHttp::MD5_t _pass = {},
                             QString _salt = {}),
             "Update User profile. Take note that this method does not change password "
             "Password and Salt are required to change email or mobile")

private:
    User();
    TARGOMAN_DEFINE_SINGLETON_SUBMODULE(Account,User);
};

class UserExtraInfo : public clsTable
{
    Q_OBJECT
public:
    void init();

private slots:
    bool API(UPDATE,Photo,(QHttp::JWT_t _JWT, QHttp::Base64Image_t _image),
             "Updates user image based using a base64 encoded image")
    bool API(UPDATE,Sheba,(QHttp::JWT_t _JWT, QHttp::Sheba_t _sheba),
             "Updates user image based using a base64 encoded image")
private:
    UserExtraInfo();
    TARGOMAN_DEFINE_SINGLETON_SUBMODULE(Account,UserExtraInfo);
};

}
}
}

#endif // TARGOMAN_API_MODULES_ACCOUNT_ORM_USER_H
