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

#include "PaymentLogic.h"
#include "PaymentGateways/Zibal.hpp"

TAPI_REGISTER_TARGOMAN_ENUM(TAPI, enuVoucherType);
TAPI_REGISTER_TARGOMAN_ENUM(TAPI, enuPaymentStatus);
TAPI_REGISTER_TARGOMAN_ENUM(TAPI, enuPaymentGateways);

namespace Targoman {
namespace API {
namespace AAA {

using namespace DBManager;

QString PaymentLogic::createOnlinePaymentLink(TAPI::enuPaymentGateways::Type _gateway, quint64 _invID, const QString& _invDesc, quint32 _toPay, const QString _callback)
{
    TAPI::MD5_t opyMD5;
    quint8 Retries = 0;
    while(true){
        try{
            clsDACResult Result = OnlinePayments::instance().callSP("sp_CREATE_newOnlinePayment", {
                                                                        {"iInvID", _invID},
                                                                        {"iGateway", QString(_gateway)}
                                                                    });
            opyMD5 = Result.spDirectOutputs().value("oMD5").toString();
            break;
        }catch(...){
            if(++Retries > 3)
                throw;
        }
    }

    try{
        stuPaymentResponse PaymentResponse;
        switch(_gateway){
        case TAPI::enuPaymentGateways::Zibal:
            PaymentResponse = Zibal::request(opyMD5, _toPay, _callback, _invDesc);
            break;
        case TAPI::enuPaymentGateways::Saman:
        case TAPI::enuPaymentGateways::Mellat:
        case TAPI::enuPaymentGateways::NextPay:
        case TAPI::enuPaymentGateways::Pardano:
        case TAPI::enuPaymentGateways::Parsian:
        case TAPI::enuPaymentGateways::Pasargad:
        case TAPI::enuPaymentGateways::AsanPardakht:
            throw exHTTPBadRequest("Gateway not suppored yet");
        }

        if(PaymentResponse.ErrorCode){
            OnlinePayments::instance().update(SYSTEM_USER_ID, {{tblOnlinePayments::onpMD5, opyMD5}}, {
                                                  {tblOnlinePayments::onpStatus, TAPI::enuPaymentStatus::Error},
                                                  {tblOnlinePayments::onpResult, PaymentResponse.Result.isEmpty() ? QString(PaymentResponse.ErrorCode) : PaymentResponse.Result},
                                              });
            throw exPayment("Unable to create payment request: " + PaymentResponse.ErrorString);
        }

        OnlinePayments::instance().update(SYSTEM_USER_ID, {{tblOnlinePayments::onpMD5, opyMD5}}, {
                                              {tblOnlinePayments::onpPGTrnID, PaymentResponse.TrackID},
                                              {tblOnlinePayments::onpStatus, TAPI::enuPaymentStatus::Pending},
                                              {tblOnlinePayments::onpResult, PaymentResponse.Result},
                                          });

        return PaymentResponse.PaymentLink;
    }catch(exPayment&){
        throw;
    }catch(exHTTPBadRequest&){
        throw;
    }catch(std::exception &e){
        OnlinePayments::instance().update(SYSTEM_USER_ID, {{tblOnlinePayments::onpMD5, opyMD5}}, {
                                              {tblOnlinePayments::onpStatus, TAPI::enuPaymentStatus::Error},
                                              {tblOnlinePayments::onpResult, e.what()},
                                          });
        throw;
    }

}

quint64 PaymentLogic::approveOnlinePayment(TAPI::enuPaymentGateways::Type _gateway, const QJsonObject& _pgResponse, const QString& _domain)
{
    stuPaymentResponse PaymentResponse;
    switch(_gateway){
    case TAPI::enuPaymentGateways::Zibal:
        PaymentResponse = Zibal::verify(_pgResponse, _domain);
        break;
    case TAPI::enuPaymentGateways::Saman:
    case TAPI::enuPaymentGateways::Mellat:
    case TAPI::enuPaymentGateways::NextPay:
    case TAPI::enuPaymentGateways::Pardano:
    case TAPI::enuPaymentGateways::Parsian:
    case TAPI::enuPaymentGateways::Pasargad:
    case TAPI::enuPaymentGateways::AsanPardakht:
        throw exHTTPBadRequest("Gateway not suppored yet");
    }

    QVariant VoucherID = OnlinePayments::instance().selectFromTable({},
                                               QString("%1=%2").arg(tblOnlinePayments::onpMD5, PaymentResponse.OrderMD5), {}, {},
                                               0, 1,
                                               tblOnlinePayments::onp_vchID).toMap().first();
    if(VoucherID.isValid() == false)
        throw exHTTPBadRequest("Voucher not found");

    if(PaymentResponse.ErrorCode){
        OnlinePayments::instance().update(SYSTEM_USER_ID, {{tblOnlinePayments::onpMD5, PaymentResponse.OrderMD5}}, {
                                              {tblOnlinePayments::onpStatus, TAPI::enuPaymentStatus::Error},
                                              {tblOnlinePayments::onpResult, PaymentResponse.Result.isEmpty() ? QString(PaymentResponse.ErrorCode) : PaymentResponse.Result},
                                          });
        throw exPayment("Unable to create payment request: " + PaymentResponse.ErrorString);
    }

    OnlinePayments::instance().update(SYSTEM_USER_ID, {{tblOnlinePayments::onpMD5, PaymentResponse.OrderMD5}}, {
                                          {tblOnlinePayments::onpPGTrnID, PaymentResponse.TrackID},
                                          {tblOnlinePayments::onpStatus, TAPI::enuPaymentStatus::Payed},
                                          {tblOnlinePayments::onpResult, PaymentResponse.Result},
                                      });
    return VoucherID.toULongLong();
}

TAPI::stuVoucher PaymentLogic::processVoucher(quint64 _VoucherID)
{
    //TODO
}

}
}
}
