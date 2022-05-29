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

#include "ActiveSessions.h"
#include "User.h"

//#include "Interfaces/ORM/APIQueryBuilders.h"

TAPI_REGISTER_TARGOMAN_ENUM(Targoman::API::AccountModule, enuSessionStatus);

namespace Targoman::API::AccountModule::ORM {

ActiveSessions::ActiveSessions() :
    intfSQLBasedModule(
        AAASchema,
        tblActiveSessions::Name,
        tblActiveSessions::Private::ORMFields,
        tblActiveSessions::Private::Relations,
        tblActiveSessions::Private::Indexes
    ) { ; }

QVariant IMPL_ORMGET(ActiveSessions) {
//  QVariantMap ExtraFilters;

    if (Authorization::hasPriv(_APICALLBOOM, this->privOn(EHTTP_GET, this->moduleBaseName())) == false)
        this->setSelfFilters({{tblActiveSessions::Fields::ssn_usrID, _APICALLBOOM.getUserID()}}, _filters);

    return this->Select(GET_METHOD_ARGS_CALL_INTERNAL_BOOM);

//  return query.one();

  //  return this->selectFromTable({}, {}, GET_METHOD_CALL_ARGS_APICALL);
}

bool IMPL_ORMDELETE(ActiveSessions) {
  TAPI::ORMFields_t ExtraFilters;

  if (_pksByPath.trimmed() == clsJWT(_APICALLBOOM.getJWT()).session())
    throw exHTTPForbidden("Deleting current session is not allowed");

  if (Authorization::hasPriv(_APICALLBOOM, this->privOn(EHTTP_DELETE, this->moduleBaseName())) == false)
      ExtraFilters.insert(tblActiveSessions::Fields::ssn_usrID, _APICALLBOOM.getUserID());
//  this->setSelfFilters({{tblActiveSessions::Fields::ssn_usrID, _APICALLBOOM.getUserID()}}, ExtraFilters);

  return this->DeleteByPks(DELETE_METHOD_ARGS_CALL_INTERNAL_BOOM, ExtraFilters, true);
//  return this->deleteByPKs(DELETE_METHOD_CALL_ARGS_APICALL, ExtraFilters, true);
}

} //namespace Targoman::API::AccountModule::ORM
