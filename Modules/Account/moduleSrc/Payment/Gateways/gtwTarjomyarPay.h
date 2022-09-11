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

#ifndef TARGOMAN_API_MODULES_ACCOUNT_PGTW_TARJOMYARPAY_H
#define TARGOMAN_API_MODULES_ACCOUNT_PGTW_TARJOMYARPAY_H

#include "../intfPaymentGateway.h"
#include "../PaymentLogic.h"

/**
 * CAUTION:
 * place #include "gtwTarjomyarPay.h" in ActivePayments.h for proper driver registration
 */

namespace Targoman::API::AccountModule::Payment::Gateways {

class gtwTarjomyarPay : public intfPaymentGateway
{
    TARGOMAN_DEFINE_API_PAYMENT_GATEWAY(
        Targoman::API::AccountModule::enuPaymentGatewayType::IranIntermediateGateway,
        gtwTarjomyarPay)

public:
    constexpr static char Name[] = "TarjomyarPay";

private:
    constexpr static char METAINFO_KEY_TOKEN[] = "token";

    //($token, $amount, $cb, $desc)
    constexpr static char URL_GTW_REQUEST[] = "https://api.targoman.ir/svn/Branch/NGT/rest/Affiliates/requestPayment";

    //($token, $ordID)
    constexpr static char URL_GTW_VERIFY[]  = "https://api.targoman.ir/svn/Branch/NGT/rest/Affiliates/approvePayment";
};

} //namespace Targoman::API::AccountModule::Payment::Gateways

#endif // TARGOMAN_API_MODULES_ACCOUNT_PGTW_TARJOMYARPAY_H
