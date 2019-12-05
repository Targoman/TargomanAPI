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

#include "BlockingRules.h"

namespace Targoman {
namespace API {
namespace AAA {
using namespace QHttp;

void BlockingRules::init()
{;}

QVariant BlockingRules::apiGET(GET_METHOD_ARGS_IMPL)
{
//    Authorization::checkPriv(_JWT,{"Account:BlockingRules:CRUD~0100"});

//    return this->selectFromTable(AAADACInstance(), {}, {}, GET_METHOD_CALL_ARGS);
}

bool BlockingRules::apiDELETE(QHttp::JWT_t _JWT, QHttp::ExtraPath_t _EXTRAPATH)
{
//    Authorization::checkPriv(_JWT,{"Account:BlockingRules:CRUD~0001"});
//    return this->deleteByPKs(AAADACInstance(), {{this->Cols.first().Name, _EXTRAPATH}});
}

bool BlockingRules::apiUPDATE(QHttp::JWT_t _JWT,
                             quint64 _blrID,
                             QHttp::IPv4_t _ip,
                             QHttp::DateTime_t _startTime,
                             QHttp::DateTime_t _endTime,
                             QString _cause,
                             Targoman::API::enuGenericStatus::Type _status)
{
//    Authorization::checkPriv(_JWT,{"Account:BlockingRules:CRUD~0010"});
//    return this->update(AAADACInstance(),
//                        {{"blrID", _blrID}},
//                        {
//                            {"blr_ipbIP", _ip.isEmpty() ? QVariant() : QHostAddress(_ip).toIPv4Address()},
//                            {"blrStartingTime", _startTime},
//                            {"blrEndingTime", _endTime},
//                            {"blrCause", _cause},
//                            {"blrUpdatedBy_usrID", clsJWT(_JWT).usrID()},
//                            {"blrStatus", _status},
//                        }
//                        );
}

quint64 BlockingRules::apiCREATE(QHttp::JWT_t _JWT,
                                QHttp::IPv4_t _ip,
                                QHttp::DateTime_t _startTime,
                                QHttp::DateTime_t _endTime,
                                QString _cause)
{
//    Authorization::checkPriv(_JWT,{"Account:BlockingRules:CRUD~1000"});
//    return this->create(AAADACInstance(),
//                        {
//                            {"blr_ipbIP", QHostAddress(_ip).toIPv4Address()},
//                            {"blrStartingTime", _startTime},
//                            {"blrEndingTime", _endTime},
//                            {"blrCause", _cause},
//                        }
//                        ).toUInt();
}

BlockingRules::BlockingRules() :
    clsTable("AAA",
              "tblBlockingRuless",
              { ///<ColName             Type                 Validation                       RO   Sort  Filter Self  Virt   PK
                {"blrID",               S(quint64),          QFV.integer().minValue(1),       ORM_PRIMARY_KEY},
                {"blr_ipbIP",           S(quint32),          QFV.integer().minValue(1)},
                {"blr_ipIPReadable",    S(QHttp::IPv4_t),    QFV,                             true,false,false},
                {"blrStartingTime",     S(QHttp::DateTime_t),QFV,                             true, true},
                {"blrEndingTime",       S(QHttp::DateTime_t),QFV},
                {"blrCause",            S(QString),          QFV,                            false,false,false},
                {"blrCreatedBy_usrID",  S(quint32),          QFV.integer().minValue(1),       true},
                {"blrCreationDateTime", S(QHttp::DateTime_t),QFV,                             true},
                {"blrUpdatedBy_usrID",  S(quint32),          QFV.integer().minValue(1)},
                {"blrStatus",           S(Targoman::API::enuGenericStatus::Type)},
              },
              { ///< Col       Reference Table             ForeignCol   Rename     LeftJoin
                {"blrCreatedBy_usrID", "AAA.tblUser",      "usrID",     "Creator_", true},
                {"blrUpdatedBy_usrID", "AAA.tblUser",      "usrID",     "Updater_", true}
              })
{
    this->registerMyRESTAPIs();
}

}
}
}
