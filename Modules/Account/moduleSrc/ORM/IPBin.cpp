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

#include "IPBin.h"
#include "User.h"

TAPI_REGISTER_TARGOMAN_ENUM(Targoman::API::AccountModule, enuIPBinStatus);

namespace Targoman::API::AccountModule::ORM {

IPBin::IPBin() :
    intfSQLBasedModule(
        AAASchema,
        tblIPBin::Name,
        tblIPBin::Private::ORMFields,
        tblIPBin::Private::Relations,
        tblIPBin::Private::Indexes
) { ; }

QVariant IMPL_ORMGET(IPBin) {
    Authorization::checkPriv(APICALLBOOM_PARAM, this->privOn(EHTTP_GET, this->moduleBaseName()));

    return this->Select(GET_METHOD_ARGS_CALL_VALUES);

//    return query.one();

    //    return this->selectFromTable({}, {}, GET_METHOD_CALL_ARGS_APICALL);
}

} //namespace Targoman::API::AccountModule::ORM
