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

#include "PaymentOrder.h"
#include "Helpers/AAA/AAA.hpp"

namespace Targoman {
namespace API {
namespace AAA {
using namespace QHttp;

void PaymentOrders::init()
{;}

QVariant PaymentOrders::apiGET(GET_METHOD_ARGS_IMPL)
{
//    bool IsSelf = _EXTRAPATH.split(',').size() == 3 && clsJWT(_JWT).usrID() != _EXTRAPATH.split(',').last().toUInt();
//    if(_EXTRAPATH.isEmpty() || IsSelf == false)
//        Authorization::checkPriv(_JWT,{"Account:PaymentOrders:CRUD~0100"});

//    return this->selectFromTable(AAADACInstance(), {},
//                                 IsSelf ? "" : QString("+inv_usrID=%1").arg(clsJWT(_JWT).usrID()),
//                                 GET_METHOD_CALL_ARGS);
}

PaymentOrders::PaymentOrders() :
    clsTable("AAA",
              "tblPaymentOrders",
              { ///<ColName             Type                    Validation                          RO   Sort  Filter Self  Virt   PK
                {"pyoID",               S(quint64),             QFV.integer().minValue(1),          ORM_PRIMARY_KEY},
                {"pyoMD5",              S(QHttp::MD5_t),        QFV,                                ORM_PRIMARY_KEY},
                {"pyoCreationDateTime", S(QHttp::DateTime_t),   QFV,                        true},
                {"pyo_invID",           S(quint64),             QFV.integer().minValue(1),          true},
                {"pyoBankTrnID",        S(QString),             QFV.allwaysValid().maxLenght(50),   true},
                {"pyoAmount",           S(QHttp::DateTime_t),   QFV,                        true},
                {"pyoStatus",           S(Targoman::API::enuPaymentStatus::Type)},
                {"pyoResult",           S(QString),             QFV,                                true,false,false},
              },
              { ///< Col       Reference Table             ForeignCol   Rename     LeftJoin
                {"bpo_invID",  "AAA.tblInvoice",           "invID",     "",        true},
              })
{
    QHTTP_REGISTER_TARGOMAN_ENUM(Targoman::API::enuPaymentStatus);

    this->registerMyRESTAPIs();
}

}
}
}
