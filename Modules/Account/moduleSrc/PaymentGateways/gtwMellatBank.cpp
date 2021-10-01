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

#include "gtwMellatBank.h"

namespace Targoman::API::AccountModule::PaymentGateways {

TARGOMAN_IMPL_API_PAYMENT_GATEWAY(gtwMellatBank)

stuPaymentResponse gtwMellatBank::request(
        const stuPaymentGateway& _paymentGateway,
        TAPI::MD5_t _orderMD5,
        qint64 _amount,
        const QString& _callback,
        const QString& _desc
    )
{
    Q_UNUSED(_paymentGateway);
    Q_UNUSED(_orderMD5);
    Q_UNUSED(_amount);
    Q_UNUSED(_callback);
    Q_UNUSED(_desc);

    return stuPaymentResponse(
        "devtest_track_id",
        "http://devtest.gateway/pay"
    );
}

stuPaymentResponse gtwMellatBank::verify(
        const stuPaymentGateway& _paymentGateway,
        const TAPI::JSON_t& _pgResponse,
        const QString& _domain
    )
{
    Q_UNUSED(_paymentGateway);
    Q_UNUSED(_pgResponse);
    Q_UNUSED(_domain);

    return {};
}

QString gtwMellatBank::errorString(int _errCode)
{
    return "UNKNOWN";
}

} //namespace Targoman::API::AccountModule::PaymentGateways
