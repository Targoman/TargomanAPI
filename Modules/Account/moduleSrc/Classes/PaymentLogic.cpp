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

QMap<QString, PAYMENTGATEWAY_INSTANCE_FUNC> PaymentLogic::RegisteredDrivers;

template <class T> void PaymentLogic::registerDriver(const QString& _driverName, T& (*_instanceFunc)())
{
    if (PaymentLogic::RegisteredDrivers.contains(_driverName))
        throw Common::exTargomanBase(QString("The class for driver name `%1` has been already registered").arg(_driverName));

    qDebug() << "registring payment gateway driver:" << _driverName;
    PaymentLogic::RegisteredDrivers.insert(_driverName, (PAYMENTGATEWAY_INSTANCE_FUNC)_instanceFunc);
}

intfPaymentGateway* PaymentLogic::getDriver(const QString& _driverName)
{
    if (PaymentLogic::RegisteredDrivers.contains(_driverName) == false)
        throw Common::exTargomanBase(QString("The class with driver name `%1` has not been registered").arg(_driverName));

    PAYMENTGATEWAY_INSTANCE_FUNC InstanceFunc = PaymentLogic::RegisteredDrivers[_driverName];

    return &InstanceFunc();
}

const stuPaymentGateway PaymentLogic::findBestPaymentGateway(
        quint32 _amount,
        TAPI::enuPaymentGatewayType::List _gatewayTypes
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
    PaymentGateway.readFromVariantMap(PaymentGatewayInfo);

    return PaymentGateway;
}

QString PaymentLogic::createOnlinePaymentLink(
        TAPI::enuPaymentGatewayType::Type _gatewayType,
        quint64 _vchID,
        const QString& _invDesc,
        quint32 _toPay,
        const QString _paymentVerifyCallback
    )
{
    ///scenario:
    ///1: find best payment gateway
    ///2: get payment gateway driver
    ///3: create payment
    ///4: call driver::request
    ///5: return result for client redirecting

    if (_gatewayType == TAPI::enuPaymentGatewayType::COD)
        throw exPayment("COD not allowed for online payment");

    QFV.url().validate(_paymentVerifyCallback, "callBack");

    //1: find best payment gateway
    stuPaymentGateway PaymentGateway = PaymentLogic::findBestPaymentGateway(_toPay, TAPI::enuPaymentGatewayType::List({ _gatewayType }));

    //2: get payment gateway driver
    intfPaymentGateway* PaymentGatewayDriver = PaymentLogic::getDriver(PaymentGateway.pgwDriver);

    //3: create payment
    TAPI::MD5_t onpMD5;
    quint8 Retries = 0;
    while (true)
    {
        try
        {
            onpMD5 = OnlinePayments::instance()
                     .callSP("sp_CREATE_newOnlinePayment", {
                                 { "iVoucherID", _vchID },
                                 { "iGatewayID", PaymentGateway.pgwID },
                                 { "iAmount", _toPay },
                             })
                     .spDirectOutputs()
                     .value("oMD5")
                     .toString()
            ;
            break;
        }
        catch (...)
        {
            if (++Retries > 3)
                throw;
        }
    }

    try
    {
        QString Callback = _paymentVerifyCallback;
        Callback += (Callback.indexOf('?') ? "&" : "?");
        Callback += "paymentMD5=" + onpMD5;

        //4: call driver::request
        stuPaymentResponse PaymentResponse = PaymentGatewayDriver->request(
                                                 PaymentGateway,
                                                 onpMD5,
                                                 _toPay,
                                                 Callback,
                                                 _invDesc
                                                 );

        if (PaymentResponse.ErrorCode)
        {
            Targoman::API::Query::Update(OnlinePayments::instance(),
                                         SYSTEM_USER_ID,
                                         {},
                                         TAPI::ORMFields_t({
                                            { tblOnlinePayments::onpResult, PaymentResponse.Result.isEmpty() ? QString(PaymentResponse.ErrorCode) : PaymentResponse.Result },
                                            { tblOnlinePayments::onpStatus, TAPI::enuPaymentStatus::Error },
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
                                        { tblOnlinePayments::onpResult, PaymentResponse.Result },
                                        { tblOnlinePayments::onpStatus, TAPI::enuPaymentStatus::Pending },
                                     }),
                                     {
                                        { tblOnlinePayments::onpMD5, onpMD5 }
                                     });

        //5: return result for client redirecting
        return PaymentResponse.PaymentLink;
    }
    catch(exPayment&)
    {
        throw;
    }
    catch(exHTTPBadRequest&)
    {
        throw;
    }
    catch(std::exception &e)
    {
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
        const QString& _paymentMD5,
        const TAPI::JSON_t& _pgResponse,
        const QString& _domain
    )
{
    if (_paymentMD5.isEmpty())
        throw exPayment("paymentMD5 is empty");

    QVariantMap OnlinePaymentInfo = SelectQuery(OnlinePayments::instance())
            .innerJoinWith("paymentGateway")
            .where({ tblOnlinePayments::onpMD5, enuConditionOperator::Equal, _paymentMD5 })
            .one();
    stuOnlinePayment OnlinePayment;
    OnlinePayment.readFromVariantMap(OnlinePaymentInfo);

    intfPaymentGateway* PaymentGatewayDriver = PaymentLogic::getDriver(OnlinePayment.PaymentGateway.pgwDriver);

    stuPaymentResponse PaymentResponse = PaymentGatewayDriver->verify(
                                             OnlinePayment.PaymentGateway,
                                             _pgResponse,
                                             _domain
                                             );

    //PaymentResponse.OrderMD5 =?= _paymentMD5

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

    return OnlinePayment.onp_vchID;
}

} //namespace Targoman::API::AAA
