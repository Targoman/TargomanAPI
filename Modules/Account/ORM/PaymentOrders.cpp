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

#include "PaymentOrders.h"

TAPI_REGISTER_TARGOMAN_ENUM(TAPI, enuPaymentStatus);

namespace Targoman {
namespace API {
namespace AAA {

using namespace ORM;

QVariant PaymentOrders::apiGET(GET_METHOD_ARGS_IMPL)
{
    if(Authorization::hasPriv(_JWT, this->privOn(EHTTP_GET,this->moduleBaseName())) == false)
        this->setSelfFilters({{"inv_usrID", clsJWT(_JWT).usrID()}}, _EXTRAPATH, _ORMFILTERS, _filters);

    return this->selectFromTable({}, {}, GET_METHOD_CALL_ARGS);
}

PaymentOrders::PaymentOrders() :
    clsTable(AAASchema,
              "tblPaymentOrders",
              { ///<ColName             Type                    Validation                          Default    RO   Sort  Filter Self  Virt   PK
                {"pyoID",               S(quint64),             QFV.integer().minValue(1),          true},
                {"pyoMD5",              S(TAPI::MD5_t),         QFV,                                ORM_PRIMARY_KEY},
                {"pyoCreationDateTime", S(TAPI::DateTime_t),    QFV,                                QNull,      true},
                {"pyo_invID",           S(quint64),             QFV.integer().minValue(1),          QNull,      true},
                {"pyoBankTrnID",        S(QString),             QFV.allwaysValid().maxLenght(50),   QInvalid,   true},
                {"pyoAmount",           S(TAPI::DateTime_t),    QFV,                                QInvalid,   true},
                {"pyoStatus",           S(TAPI::enuPaymentStatus::Type),QFV,                        TAPI::enuPaymentStatus::Pending},
                {"pyoResult",           S(QString),             QFV,                                QNull,      true,false,false},
              },
              { ///< Col       Reference Table             ForeignCol   Rename     LeftJoin
                {"bpo_invID",  "AAA.tblInvoice",           "invID",     "",        true},
              })
{
}

}
}
}
