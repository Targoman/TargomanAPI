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

#include "Interfaces/ORM/APIQueryBuilders.h"

TAPI_REGISTER_TARGOMAN_ENUM(TAPI, enuSessionStatus);

namespace Targoman {
namespace API {
namespace AAA {

using namespace ORM;

QVariant ActiveSessions::apiGET(GET_METHOD_ARGS_IMPL_APICALL)
{
//  QVariantMap ExtraFilters;

    if (Authorization::hasPriv(_JWT, this->privOn(EHTTP_GET,this->moduleBaseName())) == false)
        this->setSelfFilters({{tblActiveSessions::ssn_usrID, clsJWT(_JWT).usrID()}}, _filters);

    return Targoman::API::Query::Select(*this, GET_METHOD_CALL_ARGS_INTERNAL_CALL);

//  return query.one();

  //  return this->selectFromTable({}, {}, GET_METHOD_CALL_ARGS_APICALL);
}

bool ActiveSessions::apiDELETE(DELETE_METHOD_ARGS_IMPL_APICALL)
{
  TAPI::ORMFields_t ExtraFilters;

  if (_pksByPath.trimmed() == clsJWT(_JWT).session())
    throw exHTTPForbidden("Deleting current session is not allowed");

  if (Authorization::hasPriv(_JWT, this->privOn(EHTTP_DELETE,this->moduleBaseName())) == false)
    this->setSelfFilters({{tblActiveSessions::ssn_usrID, clsJWT(_JWT).usrID()}}, ExtraFilters);

  return Targoman::API::Query::Delete(*this, DELETE_METHOD_CALL_ARGS_INTERNAL_CALL, ExtraFilters, true);
//  return this->deleteByPKs(DELETE_METHOD_CALL_ARGS_APICALL, ExtraFilters, true);
}

ActiveSessions::ActiveSessions() :
  clsTable(AAASchema,
           tblActiveSessions::Name,
            { ///<ColName                              Type                    Validation                   Default    UpBy   Sort  Filter Self  Virt   PK
              {tblActiveSessions::ssnKey,              S(TAPI::MD5_t),        QFV,                         ORM_PRIMARY_KEY},
              {tblActiveSessions::ssn_usrID,           S(quint64),            QFV.integer().minValue(1),   QRequired,   UPNone},
              {tblActiveSessions::ssnIP,               S(quint32),            QFV.integer().minValue(1),   QRequired,   UPNone},
              {tblActiveSessions::ssnIPReadable,       S(QString),            QFV.allwaysInvalid(),        QInvalid,   UPNone,false,false},
              {tblActiveSessions::ssnCreationDateTime, ORM_CREATED_ON},
              {tblActiveSessions::ssnInfo,             S(TAPI::JSON_t),       QFV,                         QNull,      UPNone,false,false},
              {tblActiveSessions::ssnFingerPrint,      S(TAPI::MD5_t),        QFV.allwaysInvalid(),        QNull,      UPNone,false,false},
              {tblActiveSessions::ssnLastActivity,     S(TAPI::DateTime_t),   QFV,                         QNull,      UPNone},
              {tblActiveSessions::ssnRemember,         S(bool),               QFV,                         false,      UPNone},
              {tblActiveSessions::ssnUpdatedBy_usrID,  ORM_UPDATED_BY},
              {tblActiveSessions::ssnStatus,           S(TAPI::enuSessionStatus::Type), QFV,               TAPI::enuSessionStatus::Active,UPStatus},
            },
            { ///< Col                                Reference Table                  ForeignCol          Rename      LeftJoin
              {tblActiveSessions::ssn_usrID,          R(AAASchema,tblUser::Name),      tblUser::usrID,     "Owner_"},
              {tblActiveSessions::ssnUpdatedBy_usrID, R(AAASchema,tblUser::Name),      tblUser::usrID,     "Updater_", true}
            })
{
}

}
}
}

