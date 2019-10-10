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

#include "BankPaymentOrder.h"
#include "Helpers/AAA/AAA.hpp"

using namespace Targoman;
using namespace Targoman::API;
using namespace QHttp;

void BankPaymentOrder::init()
{

}

QVariant BankPaymentOrder::apiGET(GET_METHOD_ARGS_IMPL)
{
    Authorization::hasPriv(_JWT,{"Account:CRUD~0100"});

    return this->selectFromTable(AAADACInstance(), {}, {}, GET_METHOD_CALL_ARGS);
}

BankPaymentOrder::BankPaymentOrder() :
    intfTable("AAA",
              "tblBankPaymentOrder",
              "bpo",
              { ///<ColName            Validation                           Sort   Filter AS  RO   PK
                {"bpoID",               QFV.integer().minValue(1),          true,  true, "", true, true},
                {"bpoMD5",              QFV.md5(),                          true,  true, "", true, true},
                {"bpo_invID",           QFV.integer().minValue(1),          true,  true, "", true},
                {"bpoCreationDateTime", QFV.dateTime(),                     true,  true, "", true},
                {"bpoBankTrnID",        QFV.allwaysValid().maxLenght(50),   true,  true, "", true},
                {"bpoAmount",           QFV.dateTime(),                     true,  true, "", true},
                {"bpoStatus",           QFV.matches(QRegularExpression(QString("^[%1]$").arg(enuBankPaymentStatus::options().join("|"))))},
                {"bpoResult",           QFV.allwaysValid(),                 false,false},
              },
              { ///< Col       Reference Table             ForeignCol   Rename     LeftJoin
                {"bpo_invID",  "AAA.tblInvoice",           "invID",     "",        true},
              })
{
    QHTTP_REGISTER_TARGOMAN_ENUM(Targoman::API::enuBankPaymentStatus);

    this->registerMyRESTAPIs();
}

