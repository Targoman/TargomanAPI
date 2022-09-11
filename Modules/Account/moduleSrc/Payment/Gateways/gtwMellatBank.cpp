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

namespace Targoman::API::AccountModule::Payment::Gateways {

TARGOMAN_IMPL_API_PAYMENT_GATEWAY(gtwMellatBank)

// [Response, TrackID, PaymentLink]
std::tuple<QString, QString, QString> gtwMellatBank::prepareAndRequest(
    INTFAPICALLBOOM_IMPL &APICALLBOOM_PARAM,
    const ORM::tblPaymentGateways::DTO &_paymentGateway,
    TAPI::MD5_t _paymentKey,
    qint64 _amount,
    const QString& _callback,
    const QString& _desc
) {
    Q_UNUSED(_paymentGateway);
    Q_UNUSED(_paymentKey);
    Q_UNUSED(_amount);
    Q_UNUSED(_callback);
    Q_UNUSED(_desc);

    return {
        "",
        "devtest_track_id",
        "http://devtest.gateway/pay"
    };
}

// [Response, refNumber]
std::tuple<QString, QString> gtwMellatBank::verifyAndSettle(
    INTFAPICALLBOOM_IMPL &APICALLBOOM_PARAM,
    const ORM::tblPaymentGateways::DTO &_paymentGateway,
    Q_DECL_UNUSED const ORM::tblOnlinePayments::DTO &_onlinePaymentDTO,
    const TAPI::JSON_t& _pgResponse
//    const QString& _domain
) {
    Q_UNUSED(_paymentGateway);
    Q_UNUSED(_pgResponse);
//    Q_UNUSED(_domain);

    return {
        "",
        "devtest_ref_number"
    };
}

QString gtwMellatBank::errorString(int _errCode) {
    Q_UNUSED(_errCode);

    return "Uknown";
}

} //namespace Targoman::API::AccountModule::Payment::Gateways
