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

#include "APITokenValidIPs.h"

namespace Targoman {
namespace API {
namespace AAA {
using namespace QHttp;

void APITokenValidIPs::init()
{;}

QVariant APITokenValidIPs::apiGET(GET_METHOD_ARGS_IMPL)
{
    if(!this->isSelf({{"apt_usrID", clsJWT(_JWT).usrID()}}, _EXTRAPATH, _DIRECTFILTERS, _filters))
        Authorization::checkPriv(_JWT, {"Account:APITokenValidIPs:CRUD~0100"});

    return this->selectFromTable(AAADACInstance(), {}, {}, GET_METHOD_CALL_ARGS);
}

bool APITokenValidIPs::apiDELETE(QHttp::JWT_t _JWT, QHttp::ExtraPath_t _EXTRAPATH)
{
//    Authorization::checkPriv(_JWT,{"Account:APITokenValidIPs:CRUD~0001"});
//    return this->deleteByPKs(AAADACInstance(), {{this->Cols.first().Name, _EXTRAPATH}});
}

bool APITokenValidIPs::apiUPDATE(QHttp::JWT_t _JWT,
                                 quint64 _tviID,
                                 QHttp::IPv4_t _ip,
                                 Targoman::API::enuGenericStatus::Type _status)
{
//    Authorization::checkPriv(_JWT,{"Account:APITokenValidIPs:CRUD~0010"});
//    return this->update(AAADACInstance(),
//                        {{"tviID", _tviID}},
//                        {
//                            {"tviIP", _ip.isEmpty() ? QVariant() : QHostAddress(_ip).toIPv4Address()},
//                            {"tviUpdatedBy_usrID", clsJWT(_JWT).usrID()},
//                            {"tviStatus", _status},
//                        }
//                        );
}

quint64 APITokenValidIPs::apiCREATE(QHttp::JWT_t _JWT,
                                    quint64 _tokenID,
                                    QHttp::IPv4_t _ip)
{
//    Authorization::checkPriv(_JWT,{"Account:APITokenValidIPs:CRUD~1000"});
//    return this->create(AAADACInstance(),
//                        {
//                            {"tvi_aptID", _tokenID},
//                            {"tviIP", QHostAddress(_ip).toIPv4Address()},
//                        }
//                        ).toUInt();
}

APITokenValidIPs::APITokenValidIPs() :
    clsTable("AAA",
              "tblAPITokenValidIPs",
              { ///<ColName             Type                 Validation                       RO   Sort  Filter Self  Virt   PK
                {"tviID",               S(quint64),          QFV.integer().minValue(1),      ORM_PRIMARY_KEY},
                {"tvi_aptID",           S(quint64),          QFV.integer().minValue(1),      true},
                {"tviIP",               S(quint64),          QFV.integer().minValue(1),      true},
                {"tviIPReadable",       S(QString),          QFV.allwaysInvalid(),           true,false, false},
                {"tviCreatedBy_usrID",  S(quint32),          QFV.integer().minValue(1),      true},
                {"tviCreationDateTime", S(QHttp::DateTime_t),QFV,                            true},
                {"tviUpdatedBy_usrID",  S(quint32),          QFV.integer().minValue(1)},
                {"tviStatus",           S(Targoman::API::enuGenericStatus::Type)},
              },
              { ///< Col               Reference Table     ForeignCol   Rename     LeftJoin
                {"tvi_aptID",          "AAA.tblAPITokens",  "aptID"},
                {"tviCreatedBy_usrID", "AAA.tblUser",      "usrID",     "Creator_", true},
                {"tviUpdatedBy_usrID", "AAA.tblUser",      "usrID",     "Updater_", true}
              })
{
    this->registerMyRESTAPIs();
}

}
}
}
