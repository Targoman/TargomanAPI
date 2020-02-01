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

#include "ActiveSessions.h"
#include "Account.h"

namespace Targoman {
namespace API {
namespace AAA {

using namespace ORM;

void ActiveSessions::init()
{;}

QVariant ActiveSessions::apiGET(GET_METHOD_ARGS_IMPL)
{
    if(Authorization::hasPriv(_JWT, this->privOn(EHTTP_GET,this->moduleName())) == false)
        this->setSelfFilters({{"ssn_usrID", clsJWT(_JWT).usrID()}}, _EXTRAPATH, _ORMFILTERS, _filters);

    return this->selectFromTable(AAADACInstance(), {}, {}, GET_METHOD_CALL_ARGS);
}

bool ActiveSessions::apiDELETE(DELETE_METHOD_ARGS_IMPL)
{
    if(_EXTRAPATH.trimmed() == clsJWT(_JWT).session())
        throw exHTTPForbidden("Deleting current session is not allowed");

    if(Authorization::hasPriv(_JWT, this->privOn(EHTTP_DELETE,this->moduleName())) == false)
        this->setSelfFilters({{"ssn_usrID", clsJWT(_JWT).usrID()}}, _EXTRAPATH, _ORMFILTERS);

    return this->deleteByPKs(AAADACInstance(), DELETE_METHOD_CALL_ARGS, true);
}

ActiveSessions::ActiveSessions(intfAPIModule* _parent) :
    intfAPIModule(_parent),
    clsTable("AAA",
              "tblActiveSessions",
              { ///<ColName             Type                    Validation                   Default    RO   Sort  Filter Self  Virt   PK
                {"ssnKey",              S(TAPI::MD5_t),        QFV,                         ORM_PRIMARY_KEY},
                {"ssn_usrID",           S(quint32),            QFV.integer().minValue(1),   QInvalid,  true},
                {"ssnIP",               S(quint32),            QFV.integer().minValue(1),   QInvalid,  true},
                {"ssnIPReadable",       S(QString),            QFV.allwaysInvalid(),        QInvalid,  true,false,false},
                {"ssnCreationDateTime", S(TAPI::DateTime_t),   QFV,                         QNull,     true},
                {"ssnInfo",             S(TAPI::JSON_t),       QFV,                         QNull,     true,false,false},
                {"ssnFingerPrint",      S(TAPI::MD5_t),        QFV.allwaysInvalid(),        QNull,     true,false,false},
                {"ssnLastActivity",     S(TAPI::DateTime_t),   QFV,                         QNull,     true},
                {"ssnRemember",         S(bool),               QFV,                         false,     true},
                {"ssnUpdatedBy_usrID",  S(quint32),            QFV.integer().minValue(1),   QNull,     true},
                {"ssnStatus",           S(TAPI::enuSessionStatus::Type), QFV,               TAPI::enuSessionStatus::Active},
              },
              { ///< Col                Reference Table    ForeignCol   Rename     LeftJoin
                {"ssn_usrID",          "AAA.tblUser",      "usrID",     "Owner_"},
                {"ssnUpdatedBy_usrID", "AAA.tblUser",      "usrID",     "Updater_", true}
              })
{
    TAPI_REGISTER_TARGOMAN_ENUM(TAPI::enuSessionStatus);
    this->parent()->addSubmoduleMethods()
}

}
}
}

