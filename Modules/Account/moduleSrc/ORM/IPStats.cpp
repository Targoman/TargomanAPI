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

#include "IPStats.h"
#include "IPBin.h"

//#include "Interfaces/ORM/APIQueryBuilders.h"

namespace Targoman::API::AccountModule::ORM {

QVariant IPStats::apiGET(APISession<true> &_SESSION, GET_METHOD_ARGS_IMPL_APICALL) {
    Authorization::checkPriv(_SESSION.getJWT(), this->privOn(EHTTP_GET, this->moduleBaseName()));

    return /*Targoman::API::Query::*/this->Select(*this, GET_METHOD_CALL_ARGS_INTERNAL_CALL);

//    return query.one();

    //    return this->selectFromTable({}, {}, GET_METHOD_CALL_ARGS_APICALL);
}

IPStats::IPStats() :
    intfSQLBasedModule(AAASchema,
              tblIPStats::Name,
              { ///<ColName                       Type                 Validation                     Default    UpBy   Sort  Filter Self  Virt   PK
                {tblIPStats::ips_ipbIP,           ORM_PRIMARYKEY_32},
                {tblIPStats::ipsTimeStamp,        S(double),           QFV.allwaysValid(),            QRequired, UPNone},
                {tblIPStats::ipsInsertionDate,    ORM_CREATED_ON},
              },
              { ///< Col                  Reference Table                  ForeignCol
                {tblIPStats::ips_ipbIP,   R(AAASchema,tblIPBin::Name),     tblIPBin::ipbIP },
              }) { ; }

} //namespace Targoman::API::AccountModule::ORM
