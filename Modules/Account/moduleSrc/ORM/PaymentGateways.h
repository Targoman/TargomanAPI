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

#ifndef TARGOMAN_API_MODULES_ACCOUNT_ORM_PAYMENTGATEWAYS_H
#define TARGOMAN_API_MODULES_ACCOUNT_ORM_PAYMENTGATEWAYS_H

#include "Interfaces/ORM/clsTable.h"
#include "Interfaces/AAA/AAA.hpp"

namespace TAPI {
TARGOMAN_DEFINE_ENUM(enuPaymentGatewayType,
                     IranBank                   = 'I',
                     IranIntermediateGateway    = 'M',
                     InternationalDebitCart     = 'D',
                     InternationalCreditCart    = 'C',
                     );
TARGOMAN_DEFINE_ENUM(enuPaymentGatewayDriver,
                     IranMellatBank     = 'M',
                     IranPasargadBank   = 'P',
                     IranVaseteZibal    = 'Z',
                     TurkeyZiraatBank   = 'T',
                     );
TARGOMAN_DEFINE_ENUM(enuPaymentGatewayStatus,
                     Removed  = 'R',
                     Active   = 'A',
                     );
}

namespace Targoman {
namespace API {
namespace AAA {

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-variable"

namespace tblPaymentGateways {
constexpr char Name[] = "tblPaymentGateways";
TARGOMAN_CREATE_CONSTEXPR(pgwID);
TARGOMAN_CREATE_CONSTEXPR(pgwName);
TARGOMAN_CREATE_CONSTEXPR(pgwUUID); //used for making payment callback url
TARGOMAN_CREATE_CONSTEXPR(pgwType);
TARGOMAN_CREATE_CONSTEXPR(pgwDriver);
TARGOMAN_CREATE_CONSTEXPR(pgwCredentials);
TARGOMAN_CREATE_CONSTEXPR(pgwStatus);
TARGOMAN_CREATE_CONSTEXPR(pgwCreatedBy_usrID);
TARGOMAN_CREATE_CONSTEXPR(pgwCreationDateTime);
TARGOMAN_CREATE_CONSTEXPR(pgwUpdatedBy_usrID);
}

#pragma GCC diagnostic pop

class PaymentGateways : public ORM::clsTable
{
    Q_OBJECT

private slots:
    QVariant ORMGET("Get payment gateway information")
    quint32 ORMCREATE("Create a new payment gateway by priviledged user")
    bool ORMUPDATE("Update payment gateway info by priviledged user")
    bool ORMDELETE("Delete a payment gateway")

private:
    TARGOMAN_DEFINE_API_SUBMODULE(Account, PaymentGateways)
};

}
}
}

TAPI_DECLARE_METATYPE_ENUM(TAPI::enuPaymentGatewayType);
TAPI_DECLARE_METATYPE_ENUM(TAPI::enuPaymentGatewayDriver);
TAPI_DECLARE_METATYPE_ENUM(TAPI::enuPaymentGatewayStatus);

#endif // TARGOMAN_API_MODULES_ACCOUNT_ORM_PAYMENTGATEWAYS_H
