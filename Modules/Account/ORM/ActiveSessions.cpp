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

#include "ActiveSessions.h"
#include "Helpers/AAA/AAA.hpp"

namespace Targoman {
namespace API {
namespace AAA {

void ActiveSessions::init()
{;}

QVariant ActiveSessions::apiGET(GET_METHOD_ARGS_IMPL)
{
    if(!this->isSelf({{"ssn_usrID", clsJWT(_JWT).usrID()}}, _EXTRAPATH, _DIRECTFILTERS, _filters))
        Authorization::checkPriv(_JWT, this->privOn(EHTTP_GET,this->moduleName()));

    QString A = this->moduleName ();
    return this->selectFromTable(AAADACInstance(), {}, {}, GET_METHOD_CALL_ARGS);
}

bool ActiveSessions::apiDELETE(DELETE_METHOD_ARGS_IMPL)
{
    if(_EXTRAPATH.trimmed() == clsJWT(_JWT).session())
        throw exHTTPForbidden("Deleting current session is not allowed");

    if(!this->isSelf({{"ssn_usrID", clsJWT(_JWT).usrID()}}, _EXTRAPATH, _DIRECTFILTERS))
        Authorization::checkPriv(_JWT, this->privOn(EHTTP_DELETE,this->moduleName()));


 /*   QVariantMap PKs = {{this->Cols.first().Name, Key}};
    if(UserID > 0)
        PKs.insert("ssn_usrID", UserID);

    return this->deleteByPKs(AAADACInstance(), PKs);*/
    return false;
}

ActiveSessions::ActiveSessions() :
    clsTable("AAA",
              "tblActiveSessions",
              { ///<ColName             Type            Validation                           RO   Sort  Filter Self  Virt   PK
                {"ssnKey",              S(QHttp::MD5_t),        QFV,                         ORM_PRIMARY_KEY},
                {"ssn_usrID",           S(quint32),             QFV.integer().minValue(1),   ORM_SELF_REAL},
                {"ssnCreationDateTime", S(QHttp::DateTime_t),   QFV,                         true},
                {"ssnIP",               S(quint32),             QFV.integer().minValue(1),   true},
                {"ssnIPReadable",       S(QString),             QFV.allwaysInvalid(),        true,false,false},
                {"ssnInfo",             S(QHttp::JSON_t),       QFV,                         true,false,false},
                {"ssnLastActivity",     S(QHttp::DateTime_t),   QFV,                         true},
                {"ssnRemember",         S(bool),                QFV,                         true},
                {"ssnUpdatedBy_usrID",  S(quint32),             QFV.integer().minValue(1)},
                {"ssnStatus",           S(Targoman::API::enuSessionStatus::Type)},
              },
              { ///< Col                Reference Table    ForeignCol   Rename     LeftJoin
                {"ssn_usrID",          "AAA.tblUser",      "usrID",     "Owner_"},
                {"ssnUpdatedBy_usrID", "AAA.tblUser",      "usrID",     "Updater_", true}
              })
{
    QHTTP_REGISTER_TARGOMAN_ENUM(Targoman::API::enuSessionStatus);
    this->registerMyRESTAPIs();
}

}
}
}

