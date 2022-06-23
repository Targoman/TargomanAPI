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
#include "Interfaces/Helpers/RESTClientHelper.h"
using namespace Targoman::API::Helpers;

namespace Targoman::API::AccountModule::Payment::Gateways {

TARGOMAN_IMPL_API_PAYMENT_GATEWAY(gtwDevTest)

// [Response, TrackID, PaymentLink]
std::tuple<QString, QString, QString> gtwDevTest::prepareAndRequest(
    const ORM::tblPaymentGateways::DTO &_paymentGateway,
    TAPI::MD5_t _paymentKey,
    qint64 _amount,
    const QString &_callback,
    const QString &_desc
) {
    Q_UNUSED(_paymentKey);
    Q_UNUSED(_amount);
    Q_UNUSED(_callback);
    Q_UNUSED(_desc);

//    TAPI::JSON_t MetaInfo = NULLABLE_GET_OR_DEFAULT(_paymentGateway.pgwMetaInfo, TAPI::JSON_t());

#ifdef QT_DEBUG
//    bool raiseError = _paymentGateway.pgwMetaInfo["raiseError"].toBool();

//    raiseError = true;

//    if (raiseError)
//        throw exPayment("raiseError is TRUE");

    QString TrackID = "track_" + _paymentKey;

    QString PaymentLink = QString("%1/Account/OnlinePayments/devTestPayPage?paymentKey=%2&trackID=%3&callback=%4")
                          .arg(ClientConfigs::RESTServerAddress.value())
                          .arg(_paymentKey)
                          .arg(TrackID)
                          .arg(_callback)
                          ;

    return {
        "",
        TrackID,
        PaymentLink
    };

#else
    throw exPayment("THIS PAYMENT GATEWAY ONLY WORKS ON DEVELEOPMENT PHASE.");
#endif // QT_DEBUG
}

// [Response, refNumber]
std::tuple<QString, QString> gtwDevTest::verifyAndSettle(
    const ORM::tblPaymentGateways::DTO &_paymentGateway,
    const ORM::tblOnlinePayments::DTO &_onlinePayment,
    const TAPI::JSON_t &_pgResponse,
    const QString &_domain
) {
    Q_UNUSED(_pgResponse);
    Q_UNUSED(_domain);

//    TAPI::JSON_t MetaInfo = NULLABLE_GET_OR_DEFAULT(_paymentGateway.pgwMetaInfo, TAPI::JSON_t());

#ifdef QT_DEBUG
//    bool raiseError = _paymentGateway.pgwMetaInfo["raiseError"].toBool();

//    raiseError = true;

//    QString PaymentKey = _pgResponse.object().value("orderId").toString();
//    QString TrackID = _pgResponse.object().value("trackId").toString();

    if (_pgResponse.isObject() == false)
        throw exPayment("Invalid response from gateway.");
    QJsonObject PGResponse = _pgResponse.object();
    QString Result = PGResponse.value("result").toString();

    if (Result == "error")
        throw exPayment("Payment failed");

    return {
        "",
        "ref_" + _onlinePayment.onpMD5
    };

#else
    throw exPayment("THIS PAYMENT GATEWAY ONLY WORKS ON DEVELEOPMENT PHASE.");
#endif // QT_DEBUG
}

} //namespace Targoman::API::AccountModule::Payment::Gateways
