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

#include "IPBin.h"
#include "Helpers/AAA/AAA.hpp"

namespace Targoman {
namespace API {
namespace AAA {
using namespace QHttp;

void IPBin::init()
{;}

QVariant IPBin::apiGET(GET_METHOD_ARGS_IMPL)
{
    Authorization::checkPriv(_JWT, this->privOn(EHTTP_GET,this->moduleName()));
    return this->selectFromTable(AAADACInstance(), {}, {}, GET_METHOD_CALL_ARGS);
}

IPBin::IPBin() :
    clsTable("AAA",
              "tblIPBin",
              { ///<ColName             Type                 Validation                          Default    RO   Sort  Filter Self  Virt   PK
                {"ipbIP",               S(quint32),          QFV.integer().minValue(1),          ORM_PRIMARY_KEY},
                {"ipbReadable",         S(QHttp::IPv4_t),    QFV,                                QInvalid,  true},
                {"ipbFirstAccess",      S(QHttp::DateTime_t),QFV,                                QNull,     true},
                {"ipbAccessCount",      S(quint64),          QFV.integer().minValue(0),          0,         true},
                {"ipbLastAccess",       S(QHttp::DateTime_t),QFV,                                QNull,     true},
                {"ipbBlockedBy_usrID",  S(quint32),          QFV.integer().minValue(1),          QNull,     true},
                {"ipbBlockingTime",     S(QHttp::DateTime_t),QFV,                                QNull,     true},
                {"ipbStatus",           S(Targoman::API::enuIPBinStatus::Type), QFV,             Targoman::API::enuIPBinStatus::Active},
              },
              { ///< Col               Reference Table     ForeignCol Rename      LeftJoin
                {"ipbBlockedBy_usrID", "AAA.tblUser",      "usrID",   "Blocker_", true},
              })
{
    QHTTP_REGISTER_TARGOMAN_ENUM(Targoman::API::enuIPBinStatus);
    this->registerMyRESTAPIs();
}

}
}
}

