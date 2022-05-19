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

#ifndef TARGOMAN_API_MODULES_ACCOUNT_PGTW_ZIBAL_H
#define TARGOMAN_API_MODULES_ACCOUNT_PGTW_ZIBAL_H

#include "../intfPaymentGateway.h"
#include "../PaymentLogic.h"

/**
 * CAUTION:
 * place #include "gtwZibal.h" in ActivePayments.h for proper driver registration
 */

namespace Targoman::API::AccountModule::Payment::Gateways {

class gtwZibal : public intfPaymentGateway
{
    TARGOMAN_DEFINE_API_PAYMENT_GATEWAY(
        Targoman::API::AccountModule::enuPaymentGatewayType::IranIntermediateGateway,
        gtwZibal)

public:
    constexpr static char Name[] = "Zibal";

private:
    constexpr static char URL_GTW_REQUEST[] = "https://gateway.zibal.ir/v1/request";
    constexpr static char URL_GTW_VERIFY[]  = "https://gateway.zibal.ir/v1/verify";
    constexpr static char URL_GTW_PAY[]     = "https://gateway.zibal.ir/start/{{track_id}}/direct";
    constexpr static char METAINFO_KEY_MERCHANT_ID[] = "merchantid";

    virtual QString errorString(int _errCode);
    QString statusString(int _statusCode);
};

} //namespace Targoman::API::AccountModule::Payment::Gateways

#endif // TARGOMAN_API_MODULES_ACCOUNT_PGTW_ZIBAL_H
