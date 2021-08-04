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
#include "PaymentGateways/gtwDevTest.h"
#include "PaymentGateways/gtwMellatBank.h"
#include "PaymentGateways/gtwZibal.h"
#include "Interfaces/ORM/APIQueryBuilders.h"
#include "Interfaces/Helpers/URLHelper.h"

namespace Targoman::API::AAA {

using namespace Targoman::DBManager;
using namespace Targoman::API::Helpers;

void PaymentLogic::registerDriver(const QString& _driverName, intfPaymentGateway*  _driver)
{
    if (PaymentLogic::RegisteredDrivers.contains(_driverName))
        throw Common::exTargomanBase(QString("The class for driver name `%1` has been already registered").arg(_driverName));

    qDebug() << "registering payment gateway driver:" << _driverName;
    PaymentLogic::RegisteredDrivers.insert(_driverName,  _driver);
}
intfPaymentGateway* PaymentLogic::getDriver(const QString& _driverName)
{
    if (PaymentLogic::RegisteredDrivers.contains(_driverName) == false)
        throw Common::exTargomanBase(QString("The class with driver name `%1` has not been registered").arg(_driverName));

    return PaymentLogic::RegisteredDrivers[_driverName];
}

//template <class TPaymentGatewayClass>
//void PaymentLogic::registerDriver(const QString& _driverName, TPaymentGatewayClass* (*_instanceFunc)())
//{
//    if (PaymentLogic::RegisteredDrivers.contains(_driverName))
//        throw Common::exTargomanBase(QString("The class for driver name `%1` has been already registered").arg(_driverName));

//    qDebug() << "registering payment gateway driver:" << _driverName;
//    PaymentLogic::RegisteredDrivers.insert(_driverName, (PAYMENTGATEWAY_INSTANCE_FUNC)_instanceFunc);
//}
//intfPaymentGateway* PaymentLogic::getDriver(const QString& _driverName)
//{
//    if (PaymentLogic::RegisteredDrivers.contains(_driverName) == false)
//        throw Common::exTargomanBase(QString("The class with driver name `%1` has not been registered").arg(_driverName));

//    PAYMENTGATEWAY_INSTANCE_FUNC InstanceFunc = PaymentLogic::RegisteredDrivers[_driverName];

//    return InstanceFunc();
//}

//static inline QString enuPaymentGatewayTypeToCSV(const TAPI::enuPaymentGatewayType::Types &_values, const char* _itemSurrounder="") {
//    QStringList out;

//    foreach (auto _value, _values) {
//        out.append(QString("%1%2%1").arg(_itemSurrounder).arg(TAPI::enuPaymentGatewayType::toStr(_value)));
//    }

//    return out.join(",");
//}

const stuPaymentGateway PaymentLogic::findBestPaymentGateway(
        quint32 _amount,
        TAPI::enuPaymentGatewayType::Type _gatewayType,
        const QString& _domain
    )
{
//    QString CSVGatewayTypes = enuPaymentGatewayTypeToCSV(_gatewayTypes, "'");

    SelectQuery qry = SelectQuery(PaymentGateways::instance())
        .addCol(tblPaymentGateways::pgwID)
        .addCol(tblPaymentGateways::pgwName)
        .addCol(tblPaymentGateways::pgwType)
        .addCol(tblPaymentGateways::pgwDriver)
        .addCol(tblPaymentGateways::pgwMetaInfo)
        .addCol("tmptbl_inner.inner_pgwSumTodayPaidAmount")
        .addCol("tmptbl_inner.inner_pgwTransactionFeeAmount")
        .leftJoin(SelectQuery(PaymentGateways::instance())
            .addCol(tblPaymentGateways::pgwID)
            .addCol(enuConditionalAggregation::IF,
                    { tblPaymentGateways::pgwTransactionFeeType, enuConditionOperator::Equal, TAPI::enuPaymentGatewayTransactionFeeType::Percent }
                    , DBExpression::VALUE(QString("%1 * %2 / 100").arg(tblPaymentGateways::pgwTransactionFeeValue).arg(_amount))
                    , DBExpression::VALUE(tblPaymentGateways::pgwTransactionFeeValue)
                    , "inner_pgwTransactionFeeAmount"
                   )
            .addCol(enuConditionalAggregation::IF,
                    clsCondition({ tblPaymentGateways::pgwLastPaymentDateTime, enuConditionOperator::Null })
                    .orCond({ tblPaymentGateways::pgwLastPaymentDateTime, enuConditionOperator::Less, DBExpression::CURDATE() })
                    , 0
                    , DBExpression::VALUE(tblPaymentGateways::pgwSumTodayPaidAmount)
                    , "inner_pgwSumTodayPaidAmount"
                   )
            .where({ tblPaymentGateways::pgwType, enuConditionOperator::Equal, _gatewayType })
            .andWhere({ tblPaymentGateways::pgwAllowedDomainName, enuConditionOperator::Equal, _domain })
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
        .where({ tblPaymentGateways::pgwType, enuConditionOperator::Equal, _gatewayType })
        .andWhere({ tblPaymentGateways::pgwAllowedDomainName, enuConditionOperator::Equal, _domain })
        .orderBy("tmptbl_inner.inner_pgwTransactionFeeAmount")
        .orderBy("tmptbl_inner.inner_pgwSumTodayPaidAmount")
        .orderBy("RAND()")
    ;

//    stuPaymentGateway PaymentGateway = qry.one<stuPaymentGateway>();
    QVariantMap PaymentGatewayInfo = qry.one();
    stuPaymentGateway PaymentGateway;
    PaymentGateway.readFromVariantMap(PaymentGatewayInfo);

    return PaymentGateway;
}

QString PaymentLogic::createOnlinePaymentLink(
        TAPI::enuPaymentGatewayType::Type _gatewayType,
        const QString& _domain,
        quint64 _vchID,
        const QString& _invDesc,
        quint32 _toPay,
        const QString _paymentVerifyCallback,
        /*OUT*/ TAPI::MD5_t& _outPaymentMD5
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
    stuPaymentGateway PaymentGateway = PaymentLogic::findBestPaymentGateway(_toPay, _gatewayType, _domain);

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
                     .callSP("sp_CREATE_onlinePayment", {
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
        _outPaymentMD5 = onpMD5;

        QString Callback = URLHelper::addParameter(_paymentVerifyCallback, "paymentMD5", onpMD5);

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
            Targoman::API::Query::Update(
                        OnlinePayments::instance(),
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

        Targoman::API::Query::Update(
                    OnlinePayments::instance(),
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

        //increase pgwSumRequestCount and pgwSumRequestAmount
        try
        {
            PaymentGateways::instance()
                     .callSP("sp_UPDATE_paymentGateway_RequestCounters", {
                                 { "iPgwID", PaymentGateway.pgwID },
                                 { "iAmount", _toPay },
                             })
            ;
        }
        catch (...)
        {
        }

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
        Targoman::API::Query::Update(
                    OnlinePayments::instance(),
                    SYSTEM_USER_ID,
                    {},
                    TAPI::ORMFields_t({
                        { tblOnlinePayments::onpResult, e.what() },
                        { tblOnlinePayments::onpStatus, TAPI::enuPaymentStatus::Error },
                    }),
                    {
                        { tblOnlinePayments::onpMD5, onpMD5 }
                    });
        throw;
    }

}

///TODO: settle after verify
quint64 PaymentLogic::approveOnlinePayment(
        const QString& _paymentMD5,
        const TAPI::JSON_t& _pgResponse,
        const QString& _domain
    )
{
    if (_paymentMD5.isEmpty())
        throw exPayment("paymentMD5 is empty");

    QVariantMap OnlinePaymentInfo = SelectQuery(OnlinePayments::instance())
            .addCol(tblOnlinePayments::onpID)
            .addCol(tblOnlinePayments::onpMD5)
            .addCol(tblOnlinePayments::onp_vchID)
            .addCol(tblOnlinePayments::onp_pgwID)
            .addCol(tblOnlinePayments::onpPGTrnID)
            .addCol(tblOnlinePayments::onpAmount)
            .addCol(tblOnlinePayments::onpResult)
            .addCol(tblOnlinePayments::onpStatus)
            //----------------
            .addCol(tblPaymentGateways::pgwID)
            .addCol(tblPaymentGateways::pgwName)
            .addCol(tblPaymentGateways::pgwType)
            .addCol(tblPaymentGateways::pgwDriver)
            .addCol(tblPaymentGateways::pgwMetaInfo)
            //----------------
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
        Targoman::API::Query::Update(
                    OnlinePayments::instance(),
                    SYSTEM_USER_ID,
                    {},
                    TAPI::ORMFields_t({
                        { tblOnlinePayments::onpResult, PaymentResponse.Result.isEmpty() ? QString(PaymentResponse.ErrorCode) : PaymentResponse.Result },
                        { tblOnlinePayments::onpStatus, TAPI::enuPaymentStatus::Error },
                    }),
                    {
                        { tblOnlinePayments::onpMD5, _paymentMD5 }
                    });

        //increase pgwSumFailedCount
        PaymentGateways::instance()
                 .callSP("sp_UPDATE_paymentGateway_FailedCounters", {
                             { "iPgwID", OnlinePayment.onp_pgwID },
                             { "iAmount", OnlinePayment.onpAmount },
                         })
        ;

        throw exPayment("Unable to create payment request: " + PaymentResponse.ErrorString);
    }

    Targoman::API::Query::Update(
                OnlinePayments::instance(),
                SYSTEM_USER_ID,
                {},
                TAPI::ORMFields_t({
//                    { tblOnlinePayments::onpPGTrnID, PaymentResponse.TrackID },
                    { tblOnlinePayments::onpResult, PaymentResponse.Result },
                    { tblOnlinePayments::onpStatus, TAPI::enuPaymentStatus::Payed },
                }),
                {
                    { tblOnlinePayments::onpMD5, _paymentMD5 }
                });

    //update pgwLastPaymentDateTime and pgwSumTodayPaidAmount
    //increase pgwSumOkCount and pgwSumPaidAmount
    try
    {
        PaymentGateways::instance()
                 .callSP("sp_UPDATE_paymentGateway_OkCounters", {
                             { "iPgwID", OnlinePayment.onp_pgwID },
                             { "iAmount", OnlinePayment.onpAmount },
                         })
        ;
    }
    catch (...)
    {
    }

    Targoman::API::Query::Update(
                PaymentGateways::instance(),
                SYSTEM_USER_ID,
                {},
                TAPI::ORMFields_t({
                   { tblPaymentGateways::pgwSumOkCount, DBExpression::VALUE(QString("%1 + 1").arg(tblPaymentGateways::pgwSumOkCount))},
                   { tblPaymentGateways::pgwSumPaidAmount, DBExpression::VALUE(QString("%1 + %2").arg(tblPaymentGateways::pgwSumPaidAmount).arg(OnlinePayment.onpAmount))},
                }),
                {
                   { tblPaymentGateways::pgwID, OnlinePayment.onp_pgwID }
                });

    return OnlinePayment.onp_vchID;
}

} //namespace Targoman::API::AAA
