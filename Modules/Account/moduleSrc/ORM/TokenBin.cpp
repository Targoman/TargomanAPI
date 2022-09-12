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

#include "TokenBin.h"
//#include "User.h"
#include "../Account.h"

//TAPI_REGISTER_TARGOMAN_ENUM(Targoman::API::AccountModule, enuTokenBinStatus);

namespace Targoman::API::AccountModule::ORM {

TARGOMAN_API_SUBMODULE_IMPLEMENT(Account, TokenBin)

TokenBin::TokenBin() :
    intfSQLBasedModule(
        AAASchema,
        tblTokenBin::Name,
        tblTokenBin::Private::ORMFields,
        tblTokenBin::Private::Relations,
        tblTokenBin::Private::Indexes
) { ; }

QVariant IMPL_ORMGET_USER(TokenBin) {
    Authorization::checkPriv(APICALLBOOM_PARAM, this->privOn(EHTTP_GET, this->moduleBaseName()));

    return this->Select(GET_METHOD_ARGS_CALL_VALUES);
}

} //namespace Targoman::API::AccountModule::ORM
