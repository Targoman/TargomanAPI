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

#ifndef TARGOMAN_API_MODULES_ACCOUNT_ORM_AUTH_H
#define TARGOMAN_API_MODULES_ACCOUNT_ORM_AUTH_H

#include "Interfaces/AAA/AAA.hpp"
#include "Interfaces/API/intfPureModule.h"

namespace Targoman::API::AccountModule {

//structures and enumes goes here

namespace ORM {

class Auth : public intfPureModule
{
    Q_OBJECT
    TARGOMAN_API_SUBMODULE_DEFINE(Account, Auth)

private slots:
    bool REST_GET_OR_POST(
        validateJWT,
        (
            APICALLBOOM_TYPE_JWT_USER_DECL &APICALLBOOM_PARAM
        ),
        "Check JWT"
    )

};

} //namespace ORM
} //namespace Targoman::API::AccountModule

#endif // TARGOMAN_API_MODULES_ACCOUNT_ORM_AUTH_H
