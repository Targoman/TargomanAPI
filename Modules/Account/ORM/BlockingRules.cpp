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

#include "BlockingRules.h"

namespace Targoman {
namespace API {
namespace AAA {

using namespace ORM;

QVariant BlockingRules::apiGET(GET_METHOD_ARGS_IMPL)
{
    Authorization::checkPriv(_JWT, this->privOn(EHTTP_GET,this->moduleBaseName()));
    return this->selectFromTable({}, {}, GET_METHOD_CALL_ARGS);
}

bool BlockingRules::apiDELETE(DELETE_METHOD_ARGS_IMPL)
{
    Authorization::checkPriv(_JWT, this->privOn(EHTTP_DELETE,this->moduleBaseName()));
    return this->deleteByPKs(DELETE_METHOD_CALL_ARGS);
}


bool BlockingRules::apiUPDATE(UPDATE_METHOD_ARGS_IMPL)
{
    Authorization::checkPriv(_JWT, this->privOn(EHTTP_PATCH,this->moduleBaseName()));
    return this->update(UPDATE_METHOD_CALL_ARGS);
}

quint64 BlockingRules::apiCREATE(CREATE_METHOD_ARGS_IMPL)
{
    Authorization::checkPriv(_JWT, this->privOn(EHTTP_PUT,this->moduleBaseName()));
    return this->create(CREATE_METHOD_CALL_ARGS).toULongLong();
}

BlockingRules::BlockingRules() :
    clsTable(AAASchema,
              "tblBlockingRuless",
              { ///<ColName             Type                Validation                       Default    RO   Sort  Filter Self  Virt   PK
                {"blrID",               S(quint64),         QFV.integer().minValue(1),       ORM_PRIMARY_KEY},
                {"blr_ipbIP",           S(quint32),         QFV.integer().minValue(1),       QNull},
                {"blr_ipIPReadable",    S(TAPI::IPv4_t),    QFV,                             QInvalid, true,false,false},
                {"blrStartingTime",     S(TAPI::DateTime_t),QFV,                             QNull,    true, true},
                {"blrEndingTime",       S(TAPI::DateTime_t),QFV,                             QNull},
                {"blrCause",            S(QString),         QFV,                             QNull,   false,false,false},
                {"blrCreatedBy_usrID",  S(quint32),         QFV.integer().minValue(1),       QInvalid, true},
                {"blrCreationDateTime", S(TAPI::DateTime_t),QFV,                             QNull,    true},
                {"blrUpdatedBy_usrID",  S(quint32),         QFV.integer().minValue(1),       QNull,    true},
                {"blrStatus",           S(TAPI::enuGenericStatus::Type), QFV,                TAPI::enuGenericStatus::Active},
              },
              { ///< Col       Reference Table             ForeignCol   Rename     LeftJoin
                {"blrCreatedBy_usrID", "AAA.tblUser",      "usrID",     "Creator_", true},
                {"blrUpdatedBy_usrID", "AAA.tblUser",      "usrID",     "Updater_", true}
              })
{
}

}
}
}
