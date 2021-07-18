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

#include "PaymentLogic.h"
#include "PaymentGateways/Zibal.hpp"

#include "Interfaces/ORM/APIQueryBuilders.h"

//these 3 lines moved to Account.cpp for preventing not registered error:
//TAPI_REGISTER_TARGOMAN_ENUM(TAPI, enuVoucherType);
//TAPI_REGISTER_TARGOMAN_ENUM(TAPI, enuPaymentStatus);
//TAPI_REGISTER_TARGOMAN_ENUM(TAPI, enuPaymentGateway);

namespace Targoman {
namespace API {
namespace AAA {

using namespace DBManager;

///TODO attention for Minimum Bank Transaction Amount
QString PaymentLogic::createOnlinePaymentLink(TAPI::enuPaymentGateway::Type _gateway, quint64 _vchID, const QString& _invDesc, quint32 _toPay, const QString _callback)
{
    TAPI::MD5_t opyMD5;
    quint8 Retries = 0;
    while(true){
        try{
            clsDACResult Result = OnlinePayments::instance().callSP("sp_CREATE_newOnlinePayment", {
                                                                        {"iVoucherID", _vchID},
                                                                        {"iGateway", QString(_gateway)}
                                                                    });
            opyMD5 = Result.spDirectOutputs().value("oMD5").toString();
            break;
        }catch(...){
            if (++Retries > 3)
                throw;
        }
    }

    try{
        stuPaymentResponse PaymentResponse;
        switch(_gateway){
        case TAPI::enuPaymentGateway::Zibal:
            PaymentResponse = Zibal::request(opyMD5, _toPay, _callback, _invDesc);
            break;
        case TAPI::enuPaymentGateway::Saman:
        case TAPI::enuPaymentGateway::ZarrinPal:
        case TAPI::enuPaymentGateway::Mellat:
        case TAPI::enuPaymentGateway::NextPay:
        case TAPI::enuPaymentGateway::Pardano:
        case TAPI::enuPaymentGateway::Parsian:
        case TAPI::enuPaymentGateway::Pasargad:
        case TAPI::enuPaymentGateway::AsanPardakht:
        case TAPI::enuPaymentGateway::VISA:
        case TAPI::enuPaymentGateway::MasterCard:
        case TAPI::enuPaymentGateway::Gap:
            throw exHTTPBadRequest("Gateway not suppored yet");
        }

        if (PaymentResponse.ErrorCode) {
            Targoman::API::Query::Update(OnlinePayments::instance(),
                                         SYSTEM_USER_ID,
                                         {},
                                         TAPI::ORMFields_t({
                                            { tblOnlinePayments::onpStatus, TAPI::enuPaymentStatus::Error },
                                            { tblOnlinePayments::onpResult, PaymentResponse.Result.isEmpty() ? QString(PaymentResponse.ErrorCode) : PaymentResponse.Result },
                                         }),
                                         {
                                            { tblOnlinePayments::onpMD5, opyMD5 }
                                         });
            throw exPayment("Unable to create payment request: " + PaymentResponse.ErrorString);
        }

        Targoman::API::Query::Update(OnlinePayments::instance(),
                                     SYSTEM_USER_ID,
                                     {},
                                     TAPI::ORMFields_t({
                                        { tblOnlinePayments::onpPGTrnID, PaymentResponse.TrackID },
                                        { tblOnlinePayments::onpStatus, TAPI::enuPaymentStatus::Pending },
                                        { tblOnlinePayments::onpResult, PaymentResponse.Result },
                                     }),
                                     {
                                        { tblOnlinePayments::onpMD5, opyMD5 }
                                     });

        return PaymentResponse.PaymentLink;
    }catch(exPayment&){
        throw;
    }catch(exHTTPBadRequest&){
        throw;
    }catch(std::exception &e){
        Targoman::API::Query::Update(OnlinePayments::instance(),
                                     SYSTEM_USER_ID,
                                     {},
                                     TAPI::ORMFields_t({
                                        { tblOnlinePayments::onpStatus, TAPI::enuPaymentStatus::Error },
                                        { tblOnlinePayments::onpResult, e.what() },
                                     }),
                                     {
                                        { tblOnlinePayments::onpMD5, opyMD5 }
                                     });
        throw;
    }

}

///TODO settle after verify
quint64 PaymentLogic::approveOnlinePayment(TAPI::enuPaymentGateway::Type _gateway, const TAPI::JSON_t& _pgResponse, const QString& _domain)
{
    stuPaymentResponse PaymentResponse;
    switch(_gateway){
    case TAPI::enuPaymentGateway::Zibal:
        PaymentResponse = Zibal::verify(_pgResponse, _domain);
        break;
    case TAPI::enuPaymentGateway::ZarrinPal:
    case TAPI::enuPaymentGateway::Saman:
    case TAPI::enuPaymentGateway::Mellat:
    case TAPI::enuPaymentGateway::NextPay:
    case TAPI::enuPaymentGateway::Pardano:
    case TAPI::enuPaymentGateway::Parsian:
    case TAPI::enuPaymentGateway::Pasargad:
    case TAPI::enuPaymentGateway::AsanPardakht:
    case TAPI::enuPaymentGateway::VISA:
    case TAPI::enuPaymentGateway::MasterCard:
    case TAPI::enuPaymentGateway::Gap:
        throw exHTTPBadRequest("Gateway not suppored yet");
    }

//    QVariant VoucherID = OnlinePayments::instance().selectFromTable({},
//                                                                    QString("%1=%2").arg(tblOnlinePayments::onpMD5, PaymentResponse.OrderMD5), {},
//                                                                    0, 1,
//                                                                    tblOnlinePayments::onp_vchID).toMap().first();
    QVariant VoucherID = SelectQuery(OnlinePayments::instance())
        .addCol(tblOnlinePayments::onp_vchID)
        .where({ tblOnlinePayments::onpMD5, enuConditionOperator::Equal, PaymentResponse.OrderMD5 })
        .one();

    if (VoucherID.isValid() == false)
        throw exHTTPBadRequest("Voucher not found");

    if (PaymentResponse.ErrorCode) {
        Targoman::API::Query::Update(OnlinePayments::instance(),
                                     SYSTEM_USER_ID,
                                     {},
                                     TAPI::ORMFields_t({
                                        { tblOnlinePayments::onpStatus, TAPI::enuPaymentStatus::Error },
                                        { tblOnlinePayments::onpResult, PaymentResponse.Result.isEmpty() ? QString(PaymentResponse.ErrorCode) : PaymentResponse.Result },
                                     }),
                                     {
                                        { tblOnlinePayments::onpMD5, PaymentResponse.OrderMD5 }
                                     });
        throw exPayment("Unable to create payment request: " + PaymentResponse.ErrorString);
    }

    Targoman::API::Query::Update(OnlinePayments::instance(),
                                 SYSTEM_USER_ID,
                                 {},
                                 TAPI::ORMFields_t({
                                    { tblOnlinePayments::onpPGTrnID, PaymentResponse.TrackID },
                                    { tblOnlinePayments::onpStatus, TAPI::enuPaymentStatus::Payed },
                                    { tblOnlinePayments::onpResult, PaymentResponse.Result },
                                 }),
                                 {
                                    { tblOnlinePayments::onpMD5, PaymentResponse.OrderMD5 }
                                 });
    return VoucherID.toULongLong();
}

TAPI::stuVoucher PaymentLogic::processVoucher(quint64 _voucherID)
{
    QVariant VoucherDesc = SelectQuery(Voucher::instance())
                           .addCol(tblVoucher::vchDesc)
                           .where({ tblVoucher::vchID, enuConditionOperator::Equal, _voucherID })
                           .one()
                           .value(tblVoucher::vchDesc);

    //QVariant VoucherDesc = Voucher::instance().selectFromTableByID(_voucherID, tblVoucher::vchDesc).toMap().value(tblVoucher::vchDesc);

    if (!VoucherDesc.canConvert<QJsonObject>())
        throw exHTTPInternalServerError(QString("Voucher with ID: %1 not found or invalid json").arg(_voucherID));

    TAPI::stuPreVoucher PreVoucher;
    PreVoucher.fromJson(VoucherDesc.toJsonObject());

    ///TODO process voucher and apply it

    foreach(auto VoucherItem, PreVoucher.Items){
        ///TODO call svcProcessVoucherEndPoint

    }

    return TAPI::stuVoucher(
                _voucherID,
                PreVoucher,
                QString(),
                TAPI::enuVoucherStatus::Finished
                );

}

}
}
}
