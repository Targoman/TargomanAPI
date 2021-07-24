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
#include "PaymentGateways/gtwZibal.h"

#include "Interfaces/ORM/APIQueryBuilders.h"

namespace Targoman::API::AAA {
using namespace DBManager;

QMap<TAPI::enuPaymentGatewayDriver::Type, PAYMENTGATEWAY_INSTANCE_FUNC> PaymentLogic::RegisteredDrivers;

template <class T> void PaymentLogic::registerDriver(const TAPI::enuPaymentGatewayDriver::Type _driver, T& (*_instanceFunc)())
{
    if (PaymentLogic::RegisteredDrivers.contains(_driver))
        throw Common::exTargomanBase("The class for this driver has been already registered");

    PaymentLogic::RegisteredDrivers.insert(_driver, (PAYMENTGATEWAY_INSTANCE_FUNC)_instanceFunc);
}

intfPaymentGateway* PaymentLogic::getDriver(const TAPI::enuPaymentGatewayDriver::Type _driver)
{
    if (PaymentLogic::RegisteredDrivers.contains(_driver) == false)
        throw Common::exTargomanBase("this driver is not registered");

    PAYMENTGATEWAY_INSTANCE_FUNC InstanceFunc = PaymentLogic::RegisteredDrivers[_driver];

    return &InstanceFunc();
}

const stuPaymentGateway findBestPaymentGateway(
        quint32 _amount,
        TAPI::enuPaymentGatewayType::List _gatewayTypes = {}
    )
{
    QString CSVGatewayTypes = TAPI::enuPaymentGatewayType::toCSV(_gatewayTypes, "'");

    SelectQuery qry = SelectQuery(PaymentGateways::instance())
        .addCol(tblPaymentGateways::pgwID)
        .addCol(tblPaymentGateways::pgwName)
        .addCol(tblPaymentGateways::pgwType)
        .addCol(tblPaymentGateways::pgwDriver)
        .addCol(tblPaymentGateways::pgwMetaInfo)
        .addCol("tmptbl_inner.inner_pgwSumTodayPaidAmount")
        .addCol("tmptbl_inner.inner_pgwTransactionFeeAmount")
        .innerJoin(SelectQuery(PaymentGateways::instance())
            .addCol(tblPaymentGateways::pgwID)
            .addCol(enuConditionalAggregation::IF,
                    { tblPaymentGateways::pgwTransactionFeeType, enuConditionOperator::Equal, TAPI::enuPaymentGatewayTransactionFeeType::Percent }
                    , DBExpression::VALUE(QString("%1 * %2 / 100").arg(tblPaymentGateways::pgwTransactionFeeValue).arg(_amount))
                    , tblPaymentGateways::pgwTransactionFeeValue
                    , "inner_pgwTransactionFeeAmount")
            .addCol(enuConditionalAggregation::IF,
                    clsCondition({ tblPaymentGateways::pgwLastPaymentDateTime, enuConditionOperator::Null })
                    .orCond({ tblPaymentGateways::pgwLastPaymentDateTime, enuConditionOperator::Less, DBExpression::CURDATE() })
                    , 0
                    , tblPaymentGateways::pgwSumTodayPaidAmount
                    , "inner_pgwSumTodayPaidAmount")
            .where({ tblPaymentGateways::pgwType, enuConditionOperator::In, CSVGatewayTypes })
            .andWhere({ tblPaymentGateways::pgwMinRequestAmount, enuConditionOperator::LessEqual, _amount })
            .andWhere(
                clsCondition({ tblPaymentGateways::pgwMaxPerDayAmount, enuConditionOperator::Null })
                .orCond(
                    clsCondition({ tblPaymentGateways::pgwLastPaymentDateTime, enuConditionOperator::Null })
                    .orCond({ tblPaymentGateways::pgwLastPaymentDateTime, enuConditionOperator::Less, DBExpression::CURDATE() })
                    .orCond({
                        tblPaymentGateways::pgwSumTodayPaidAmount,
                        enuConditionOperator::LessEqual,
                        DBExpression::VALUE(QString("%1 - %2").arg(tblPaymentGateways::pgwMaxPerDayAmount).arg(_amount))
                    })
                )
            )
            .andWhere(
                clsCondition({ tblPaymentGateways::pgwMaxRequestAmount, enuConditionOperator::Null })
                .orCond({ tblPaymentGateways::pgwMaxRequestAmount, enuConditionOperator::GreaterEqual, _amount })
            )
//            .groupBy(tblPaymentGateways::pgwID)
            , "tmptbl_inner"
            , { "tmptbl_inner", tblPaymentGateways::pgwID,
                enuConditionOperator::Equal,
                tblPaymentGateways::Name, tblPaymentGateways::pgwID }
        )
        .orderBy("tmptbl_inner.inner_pgwTransactionFeeAmount")
        .orderBy("tmptbl_inner.inner_pgwSumTodayPaidAmount")
    ;

//    stuPaymentGateway PaymentGateway = qry.one<stuPaymentGateway>();
    QVariantMap PaymentGatewayInfo = qry.one();

    stuPaymentGateway PaymentGateway;
    SET_FIELD_FROM_VARIANT_MAP(PaymentGateway.pgwID,                  PaymentGatewayInfo, tblPaymentGateways, pgwID);
    SET_FIELD_FROM_VARIANT_MAP(PaymentGateway.pgwName,                PaymentGatewayInfo, tblPaymentGateways, pgwName);
    SET_FIELD_FROM_VARIANT_MAP(PaymentGateway.pgwType,                PaymentGatewayInfo, tblPaymentGateways, pgwType);
    SET_FIELD_FROM_VARIANT_MAP(PaymentGateway.pgwDriver,              PaymentGatewayInfo, tblPaymentGateways, pgwDriver);
    SET_FIELD_FROM_VARIANT_MAP(PaymentGateway.pgwMetaInfo,            PaymentGatewayInfo, tblPaymentGateways, pgwMetaInfo);
//    SET_FIELD_FROM_VARIANT_MAP(PaymentGateway.pgwTransactionFeeValue, PaymentGatewayInfo, tblPaymentGateways, pgwTransactionFeeValue);
//    SET_FIELD_FROM_VARIANT_MAP(PaymentGateway.pgwTransactionFeeType,  PaymentGatewayInfo, tblPaymentGateways, pgwTransactionFeeType);
//    SET_FIELD_FROM_VARIANT_MAP(PaymentGateway.pgwMinRequestAmount,    PaymentGatewayInfo, tblPaymentGateways, pgwMinRequestAmount);
//    SET_FIELD_FROM_VARIANT_MAP(PaymentGateway.pgwMaxRequestAmount,    PaymentGatewayInfo, tblPaymentGateways, pgwMaxRequestAmount);
//    SET_FIELD_FROM_VARIANT_MAP(PaymentGateway.pgwMaxPerDayAmount,     PaymentGatewayInfo, tblPaymentGateways, pgwMaxPerDayAmount);
//    SET_FIELD_FROM_VARIANT_MAP(PaymentGateway.pgwLastPaymentDateTime, PaymentGatewayInfo, tblPaymentGateways, pgwLastPaymentDateTime);
//    SET_FIELD_FROM_VARIANT_MAP(PaymentGateway.pgwSumTodayPaidAmount,  PaymentGatewayInfo, tblPaymentGateways, pgwSumTodayPaidAmount);
//    SET_FIELD_FROM_VARIANT_MAP(PaymentGateway.pgwSumRequestCount,     PaymentGatewayInfo, tblPaymentGateways, pgwSumRequestCount);
//    SET_FIELD_FROM_VARIANT_MAP(PaymentGateway.pgwSumRequestAmount,    PaymentGatewayInfo, tblPaymentGateways, pgwSumRequestAmount);
//    SET_FIELD_FROM_VARIANT_MAP(PaymentGateway.pgwSumFailedCount,      PaymentGatewayInfo, tblPaymentGateways, pgwSumFailedCount);
//    SET_FIELD_FROM_VARIANT_MAP(PaymentGateway.pgwSumOkCount,          PaymentGatewayInfo, tblPaymentGateways, pgwSumOkCount);
//    SET_FIELD_FROM_VARIANT_MAP(PaymentGateway.pgwSumPaidAmount,       PaymentGatewayInfo, tblPaymentGateways, pgwSumPaidAmount);
//    SET_FIELD_FROM_VARIANT_MAP(PaymentGateway.pgwStatus,              PaymentGatewayInfo, tblPaymentGateways, pgwStatus);

    return PaymentGateway;
}

QString PaymentLogic::createOnlinePaymentLink(
        TAPI::enuPaymentGatewayType::Type _gatewayType,
        quint64 _vchID,
        const QString& _invDesc,
        quint32 _toPay,
        const QString _callback
    )
{
    ///scenario:
    ///1: find best payment gateway ORM
    ///2: get payment gateway driver
    ///3: create payment
    ///4: call driver::request
    ///5: return result for client redirecting

    //1: find best payment gateway ORM
    stuPaymentGateway PaymentGateway = findBestPaymentGateway(_toPay, TAPI::enuPaymentGatewayType::List({ _gatewayType }));

    //2: get payment gateway driver
    intfPaymentGateway* PaymentGatewayDriver = PaymentLogic::getDriver(PaymentGateway.pgwDriver);

    //3: create payment
    TAPI::MD5_t onpMD5;
    quint8 Retries = 0;
    while(true) {
        try {
            clsDACResult Result = OnlinePayments::instance().callSP("sp_CREATE_newOnlinePayment", {
                                                                        { "iVoucherID", _vchID },
                                                                        { "iGatewayID", PaymentGateway.pgwID },
                                                                        { "iAmount", _toPay },
                                                                    });
            onpMD5 = Result.spDirectOutputs().value("oMD5").toString();
            break;
        }
        catch(...) {
            if (++Retries > 3)
                throw;
        }
    }

    try{
        //4: call driver::request
        stuPaymentResponse PaymentResponse = PaymentGatewayDriver->request(
                                                 PaymentGateway,
                                                 onpMD5,
                                                 _toPay,
                                                 _callback,
                                                 _invDesc
                                                 );
        if (PaymentResponse.ErrorCode) {
            Targoman::API::Query::Update(OnlinePayments::instance(),
                                         SYSTEM_USER_ID,
                                         {},
                                         TAPI::ORMFields_t({
                                            { tblOnlinePayments::onpStatus, TAPI::enuPaymentStatus::Error },
                                            { tblOnlinePayments::onpResult, PaymentResponse.Result.isEmpty() ? QString(PaymentResponse.ErrorCode) : PaymentResponse.Result },
                                         }),
                                         {
                                            { tblOnlinePayments::onpMD5, onpMD5 }
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
                                        { tblOnlinePayments::onpMD5, onpMD5 }
                                     });

        //5: return result for client redirecting
        return PaymentResponse.PaymentLink;
    }
    catch(exPayment&) {
        throw;
    }
    catch(exHTTPBadRequest&) {
        throw;
    }
    catch(std::exception &e) {
        Targoman::API::Query::Update(OnlinePayments::instance(),
                                     SYSTEM_USER_ID,
                                     {},
                                     TAPI::ORMFields_t({
                                        { tblOnlinePayments::onpStatus, TAPI::enuPaymentStatus::Error },
                                        { tblOnlinePayments::onpResult, e.what() },
                                     }),
                                     {
                                        { tblOnlinePayments::onpMD5, onpMD5 }
                                     });
        throw;
    }

}

///TODO settle after verify
quint64 PaymentLogic::approveOnlinePayment(
        TAPI::enuPaymentGatewayType::Type _gatewayType,
        const TAPI::JSON_t& _pgResponse,
        const QString& _domain
    )
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

} //namespace Targoman::API::AAA
