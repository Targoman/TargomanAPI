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

#include "gtwDevTest.h"

namespace Targoman::API::AccountModule::Payment::Gateways {

TARGOMAN_IMPL_API_PAYMENT_GATEWAY(gtwDevTest)

// [Response, TrackID, PaymentLink]
std::tuple<QString, QString, QString> gtwDevTest::prepareAndRequest(
    const ORM::tblPaymentGateways::DTO &_paymentGateway,
    TAPI::MD5_t _orderMD5,
    qint64 _amount,
    const QString &_callback,
    const QString &_desc
) {
    Q_UNUSED(_orderMD5);
    Q_UNUSED(_amount);
    Q_UNUSED(_callback);
    Q_UNUSED(_desc);

//    TAPI::JSON_t MetaInfo = NULLABLE_GET_OR_DEFAULT(_paymentGateway.pgwMetaInfo, TAPI::JSON_t());

#ifdef QT_DEBUG
    bool raiseError = _paymentGateway.pgwMetaInfo["raiseError"].toBool();

//    raiseError = true;

    if (raiseError)
        throw exPayment("raiseError is TRUE");

    QString TrackID = "devtest_track_id";

    return {
        "",
        TrackID,
        QString(gtwDevTest::URL_GTW_PAY).replace("{{track_id}}", TrackID)
    };

#else
    throw exPayment("THIS PAYMENT GATEWAY ONLY WORKS ON DEVELEOPMENT PHASE.");
#endif // QT_DEBUG
}

// [Response, refNumber]
std::tuple<QString, QString> gtwDevTest::verifyAndSettle(
    const ORM::tblPaymentGateways::DTO &_paymentGateway,
    const TAPI::JSON_t &_pgResponse,
    const QString &_domain
) {
    Q_UNUSED(_pgResponse);
    Q_UNUSED(_domain);

//    TAPI::JSON_t MetaInfo = NULLABLE_GET_OR_DEFAULT(_paymentGateway.pgwMetaInfo, TAPI::JSON_t());

#ifdef QT_DEBUG
    bool raiseError = _paymentGateway.pgwMetaInfo["raiseError"].toBool();

//    raiseError = true;

//    QString OrderMD5 = _pgResponse.object().value("orderId").toString();
//    QString TrackID = _pgResponse.object().value("trackId").toString();

    if (raiseError)
        throw exPayment("raiseError is TRUE");

    return {
        "",
        "devtest_ref_number"
    };

#else
    throw exPayment("THIS PAYMENT GATEWAY ONLY WORKS ON DEVELEOPMENT PHASE.");
#endif // QT_DEBUG
}

} //namespace Targoman::API::AccountModule::Payment::Gateways
