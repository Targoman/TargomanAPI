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

#ifndef TARGOMAN_API_MODULES_AAA_ORM_USER_H
#define TARGOMAN_API_MODULES_AAA_ORM_USER_H

#include "QHttp/intfRESTAPIHolder.h"
#include "libTargomanDBM/clsDAC.h"
#include "Helpers/ORM/intfTable.h"
#include "Helpers/AAA/GenericEnums.hpp"

namespace Targoman {
namespace API {

#ifndef API
#define API(_method, _name, _sig, _doc) api##_method##_name _sig; QString signOf##_method##_name(){ return #_sig; } QString docOf##_method##_name(){ return _doc; }
#endif

class User : public QHttp::intfRESTAPIHolder, private intfTable
{
    Q_OBJECT
public:
    void init();

private slots:
    QVariant ORMGET("Get user information")

    bool API(UPDATE,profile,(QHttp::JWT_t _JWT,
                             QString _name = {},
                             QString _family = {},
                             QHttp::Email_t _email = {},
                             QHttp::Mobile_t _mobile = {}),
             "Update User profile")

    bool API(UPDATE,,(QHttp::JWT_t _JWT,
                      quint64 _userID,
                      QString _name = {},
                      QString _family = {},
                      QHttp::Email_t _email = {},
                      QHttp::Mobile_t _mobile = {},
                      enuUserApproval::Type _approvalState = {},
                      quint64 _roleID = {},
                      QHttp::JSON_t _specialPrivs = {},
                      qint8 _maxSessions = {},
                      enuUserStatus::Type _status = {}),
             "Update User info by priviledged user")

private:
    User();
    TARGOMAN_DEFINE_SINGLETON_SUBMODULE(Account,User);
};

class UserExtraInfo : public QHttp::intfRESTAPIHolder, private intfTable
{
    Q_OBJECT
public:
    void init();

private:
    UserExtraInfo();
    TARGOMAN_DEFINE_SINGLETON_SUBMODULE(Account,UserExtraInfo);
};

}
}
#endif // TARGOMAN_API_MODULES_AAA_ORM_USER_H
