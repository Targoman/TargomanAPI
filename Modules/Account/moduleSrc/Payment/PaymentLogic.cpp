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
#include "Payment/Gateways/ActiveGateways.h"
//#include "Interfaces/ORM/APIQueryBuilders.h"

#include "Interfaces/Helpers/URLHelper.h"
using namespace Targoman::API::Helpers;

#include "Interfaces/Server/ServerCommon.h"
using namespace Targoman::API::Server;

#include "libTargomanCommon/Configuration/Validators.hpp"
//using namespace Targoman::Common;
using namespace Targoman::Common::Configuration;

using namespace Targoman::DBManager;

using namespace Targoman::API::AccountModule::ORM;

namespace Targoman::API::AccountModule::Payment {

tmplConfigurable<FilePath_t> PaymentLogic::TransactionLogFile(
    AAA::makeConfig("TransactionLogFile"),
    "File to store transaction logs",
    "",
    Validators::tmplPathAccessValidator<
        TARGOMAN_PATH_ACCESS(enuPathAccess::File | enuPathAccess::Writeatble),
    false>,
    "",
    "FILEPATH",
    "transacton-log-file",
    enuConfigSource::Arg | enuConfigSource::File
);

void PaymentLogic::registerDriver(const QString& _driverName, intfPaymentGateway*  _driver) {
    if (PaymentLogic::RegisteredDrivers.contains(_driverName))
        throw Targoman::Common::exTargomanBase(QString("The class for driver name `%1` has been already registered").arg(_driverName));

    qDebug() << "registering payment gateway driver:" << _driverName;
    PaymentLogic::RegisteredDrivers.insert(_driverName,  _driver);
}
intfPaymentGateway* PaymentLogic::getDriver(const QString& _driverName) {
    if (PaymentLogic::RegisteredDrivers.contains(_driverName) == false)
        throw Targoman::Common::exTargomanBase(QString("The class with driver name `%1` has not been registered").arg(_driverName));

    return PaymentLogic::RegisteredDrivers[_driverName];
}

//template <class TPaymentGatewayClass>
//void PaymentLogic::registerDriver(const QString& _driverName, TPaymentGatewayClass* (*_instanceFunc)())
//{
//    if (PaymentLogic::RegisteredDrivers.contains(_driverName))
//        throw Targoman::Common::exTargomanBase(QString("The class for driver name `%1` has been already registered").arg(_driverName));

//    qDebug() << "registering payment gateway driver:" << _driverName;
//    PaymentLogic::RegisteredDrivers.insert(_driverName, (PAYMENTGATEWAY_INSTANCE_FUNC)_instanceFunc);
//}
//intfPaymentGateway* PaymentLogic::getDriver(const QString& _driverName)
//{
//    if (PaymentLogic::RegisteredDrivers.contains(_driverName) == false)
//        throw Targoman::Common::exTargomanBase(QString("The class with driver name `%1` has not been registered").arg(_driverName));

//    PAYMENTGATEWAY_INSTANCE_FUNC InstanceFunc = PaymentLogic::RegisteredDrivers[_driverName];

//    return InstanceFunc();
//}

//static inline QString enuPaymentGatewayTypeToCSV(const enuPaymentGatewayType::Types &_values, const char* _itemSurrounder="") {
//    QStringList out;

//    foreach (auto _value, _values) {
//        out.append(QString("%1%2%1").arg(_itemSurrounder).arg(enuPaymentGatewayType::toStr(_value)));
//    }

//    return out.join(",");
//}

QVariantList PaymentLogic::findAvailableGatewayTypes(
    INTFAPICALLBOOM_IMPL &APICALLBOOM_PARAM,
    quint32 _amount,
    QString _domain
) {
    QString Domain = URLHelper::domain(_domain, true);

    ORMSelectQuery qry = PaymentGatewayTypes::instance().GetSelectQuery(APICALLBOOM_PARAM)
                      .addCol(tblPaymentGatewayTypes::Fields::pgtType)
                      .addCol(tblPaymentGatewayTypes::Fields::pgtName)
                      .addCol(DBExpression::VALUE("IFNULL(pgtMinRequestAmount, 0)"), tblPaymentGatewayTypes::Fields::pgtMinRequestAmount)
                      .addCol(DBExpression::VALUE("IFNULL(pgtMaxRequestAmount, 0)"), tblPaymentGatewayTypes::Fields::pgtMaxRequestAmount)
                      .nestedInnerJoin(PaymentGatewayTypes::instance().GetSelectQuery(APICALLBOOM_PARAM)
                                 .addCol(tblPaymentGatewayTypes::Fields::pgtType)
                                 .addCol(enuAggregation::COUNT, tblPaymentGatewayTypes::Fields::pgtType, "_cnt")

                                 .nestedInnerJoin(PaymentGateways::instance().GetSelectQuery(APICALLBOOM_PARAM)
                                            .addCols(PaymentGateways::instance().SelectableColumnNames())
                                            .addCol(enuConditionalAggregation::IF,
                                                { tblPaymentGateways::Fields::pgwTransactionFeeType, enuConditionOperator::Equal, Targoman::API::AccountModule::enuPaymentGatewayTransactionFeeType::Percent }
                                                , DBExpression::VALUE(QString("%1 * %2 / 100").arg(tblPaymentGateways::Fields::pgwTransactionFeeValue).arg(_amount))
                                                , DBExpression::VALUE(tblPaymentGateways::Fields::pgwTransactionFeeValue)
                                                , "inner_pgwTransactionFeeAmount"
                                            )
                                            .addCol(enuConditionalAggregation::IF,
                                                clsCondition({ tblPaymentGateways::Fields::pgwLastPaymentDateTime, enuConditionOperator::Null })
                                                    .orCond({ tblPaymentGateways::Fields::pgwLastPaymentDateTime, enuConditionOperator::Less, DBExpression::CURDATE() })
                                                , 0
                                                , DBExpression::VALUE(tblPaymentGateways::Fields::pgwSumTodayPaidAmount)
                                                , "inner_pgwSumTodayPaidAmount"
                                            )
//                                            .innerJoinWith(tblPaymentGateways::Relation::Type)
//                                            .andWhere({ tblPaymentGatewayTypes::Fields::pgtMinRequestAmount, enuConditionOperator::LessEqual, _amount })
//                                            .andWhere(
//                                                clsCondition({ tblPaymentGatewayTypes::Fields::pgtMaxRequestAmount, enuConditionOperator::Null })
//                                                .orCond({ tblPaymentGatewayTypes::Fields::pgtMaxRequestAmount, enuConditionOperator::GreaterEqual, _amount })
//                                            )
                                            .andWhere({ { enuAggregation::LOWER, tblPaymentGateways::Fields::pgwAllowedDomainName }, enuConditionOperator::Equal, Domain })
                                            .andWhere(
                                                clsCondition({ tblPaymentGateways::Fields::pgwMaxPerDayAmount, enuConditionOperator::Null })
                                                .orCond(
                                                    clsCondition({ tblPaymentGateways::Fields::pgwLastPaymentDateTime, enuConditionOperator::Null })
                                                    .orCond({ tblPaymentGateways::Fields::pgwLastPaymentDateTime, enuConditionOperator::Less, DBExpression::CURDATE() })
                                                    .orCond({
                                                        tblPaymentGateways::Fields::pgwSumTodayPaidAmount,
                                                        enuConditionOperator::LessEqual,
                                                        DBExpression::VALUE(QString("%1 - %2").arg(tblPaymentGateways::Fields::pgwMaxPerDayAmount).arg(_amount))
                                                    })
                                                )
                                            )
                                            , "tmptbl_inner"
                                            , { "tmptbl_inner", tblPaymentGateways::Fields::pgwType,
                                                enuConditionOperator::Equal,
                                                tblPaymentGatewayTypes::Name, tblPaymentGatewayTypes::Fields::pgtType }
                                 )
                                 .groupBy(tblPaymentGatewayTypes::Fields::pgtType)
                                 , "tmptbl_outer"
                                 , { "tmptbl_outer", tblPaymentGatewayTypes::Fields::pgtType,
                                     enuConditionOperator::Equal,
                                     tblPaymentGatewayTypes::Name, tblPaymentGatewayTypes::Fields::pgtType }
                      )
                      .andWhere({ tblPaymentGatewayTypes::Fields::pgtMinRequestAmount, enuConditionOperator::LessEqual, _amount })
#ifndef QT_DEBUG
                      .andWhere({ tblPaymentGatewayTypes::Fields::pgtType, enuConditionOperator::NotEqual, enuPaymentGatewayType::_DeveloperTest })
#endif
//        .andWhere({ { enuAggregation::LOWER, tblPaymentGateways::Fields::pgwAllowedDomainName }, enuConditionOperator::Equal, Domain })
//        .orderBy("tmptbl_inner.inner_pgwTransactionFeeAmount")
//        .orderBy("tmptbl_inner.inner_pgwSumTodayPaidAmount")
//        .orderBy("RAND()")
//        .groupBy(tblPaymentGateways::Fields::pgwType)
    ;

//    QList<ORM::tblPaymentGateways::DTO> Rows = qry.all<ORM::tblPaymentGateways::DTO>();

    QVariantList Rows = qry.all();

    QVariantList Result;

    foreach (auto Row, Rows) {
        QVariantMap Map = Row.toMap();

        Result.append(QVariantMap({
                                { "Type", Map[tblPaymentGatewayTypes::Fields::pgtType] },
                                { "Name", Map[tblPaymentGatewayTypes::Fields::pgtName] },
                                { "MinRequestAmount", Map[tblPaymentGatewayTypes::Fields::pgtMinRequestAmount] },
                                { "MaxRequestAmount", Map[tblPaymentGatewayTypes::Fields::pgtMaxRequestAmount] },
                     }));
    }

    return Result;
}

const ORM::tblPaymentGateways::DTO PaymentLogic::findBestPaymentGateway(
    INTFAPICALLBOOM_IMPL &APICALLBOOM_PARAM,
    quint32 _amount,
    enuPaymentGatewayType::Type _gatewayType,
    QString _domain
) {
//    QString CSVGatewayTypes = enuPaymentGatewayTypeToCSV(_gatewayTypes, "'");

#ifndef QT_DEBUG
    if (_gatewayType == enuPaymentGatewayType::_DeveloperTest)
        throw exPayment("DeveloperTest is not allowed in non debug mode");
#endif

    QString Domain = URLHelper::domain(_domain, true);

    ORMSelectQuery qry = PaymentGateways::instance().GetSelectQuery(APICALLBOOM_PARAM)
        .addCols(PaymentGateways::instance().SelectableColumnNames())
        .addCols({
                     "tmptbl_inner.inner_pgwSumTodayPaidAmount",
                     "tmptbl_inner.inner_pgwTransactionFeeAmount",
                 })
        .nestedLeftJoin(PaymentGateways::instance().GetSelectQuery(APICALLBOOM_PARAM)
            .addCol(tblPaymentGateways::Fields::pgwID)
            .addCol(enuConditionalAggregation::IF,
                    { tblPaymentGateways::Fields::pgwTransactionFeeType, enuConditionOperator::Equal, Targoman::API::AccountModule::enuPaymentGatewayTransactionFeeType::Percent }
                    , DBExpression::VALUE(QString("%1 * %2 / 100").arg(tblPaymentGateways::Fields::pgwTransactionFeeValue).arg(_amount))
                    , DBExpression::VALUE(tblPaymentGateways::Fields::pgwTransactionFeeValue)
                    , "inner_pgwTransactionFeeAmount"
                   )
            .addCol(enuConditionalAggregation::IF,
                    clsCondition({ tblPaymentGateways::Fields::pgwLastPaymentDateTime, enuConditionOperator::Null })
                    .orCond({ tblPaymentGateways::Fields::pgwLastPaymentDateTime, enuConditionOperator::Less, DBExpression::CURDATE() })
                    , 0
                    , DBExpression::VALUE(tblPaymentGateways::Fields::pgwSumTodayPaidAmount)
                    , "inner_pgwSumTodayPaidAmount"
                   )
            .leftJoinWith(tblPaymentGateways::Relation::Type)
            .where({ tblPaymentGateways::Fields::pgwType, enuConditionOperator::Equal, _gatewayType })
            .andWhere({ { enuAggregation::LOWER, tblPaymentGateways::Fields::pgwAllowedDomainName }, enuConditionOperator::Equal, Domain })
            .andWhere({ tblPaymentGatewayTypes::Fields::pgtMinRequestAmount, enuConditionOperator::LessEqual, _amount })
            .andWhere(
                clsCondition({ tblPaymentGateways::Fields::pgwMaxPerDayAmount, enuConditionOperator::Null })
                .orCond(
                    clsCondition({ tblPaymentGateways::Fields::pgwLastPaymentDateTime, enuConditionOperator::Null })
                    .orCond({ tblPaymentGateways::Fields::pgwLastPaymentDateTime, enuConditionOperator::Less, DBExpression::CURDATE() })
                    .orCond({
                        tblPaymentGateways::Fields::pgwSumTodayPaidAmount,
                        enuConditionOperator::LessEqual,
                        DBExpression::VALUE(QString("%1 - %2").arg(tblPaymentGateways::Fields::pgwMaxPerDayAmount).arg(_amount))
                    })
                )
            )
            .andWhere(
                clsCondition({ tblPaymentGatewayTypes::Fields::pgtMaxRequestAmount, enuConditionOperator::Null })
                .orCond({ tblPaymentGatewayTypes::Fields::pgtMaxRequestAmount, enuConditionOperator::GreaterEqual, _amount })
            )
//            .groupBy(tblPaymentGateways::Fields::pgwID)
            , "tmptbl_inner"
            , { "tmptbl_inner", tblPaymentGateways::Fields::pgwID,
                enuConditionOperator::Equal,
                tblPaymentGateways::Name, tblPaymentGateways::Fields::pgwID }
        )
        .where({ tblPaymentGateways::Fields::pgwType, enuConditionOperator::Equal, _gatewayType })
        .andWhere({ { enuAggregation::LOWER, tblPaymentGateways::Fields::pgwAllowedDomainName }, enuConditionOperator::Equal, Domain })
        .orderBy("tmptbl_inner.inner_pgwTransactionFeeAmount")
        .orderBy("tmptbl_inner.inner_pgwSumTodayPaidAmount")
        .orderBy("RAND()")
    ;

    return qry.one<ORM::tblPaymentGateways::DTO>();
}

QString PaymentLogic::createOnlinePaymentLink(
    INTFAPICALLBOOM_IMPL &APICALLBOOM_PARAM,
    enuPaymentGatewayType::Type _gatewayType,
    QString _domain,
    quint64 _vchID,
    const QString& _invDesc,
    quint32 _toPay,
    const QString _paymentVerifyCallback,
    /*OUT*/ TAPI::MD5_t& _outPaymentKey,
    quint64 _walID
) {
    _domain = URLHelper::domain(_domain, true);

    ///scenario:
    ///1: find best payment gateway
    ///2: get payment gateway driver
    ///3: create payment
    ///4: call driver::request
    ///5: return result for client redirecting

    if (_gatewayType == enuPaymentGatewayType::COD)
        throw exPayment("COD not allowed for online payment");

    QFV.url().validate(_paymentVerifyCallback, "callBack");

    //1: find best payment gateway
    ORM::tblPaymentGateways::DTO PaymentGatewayDTO = PaymentLogic::findBestPaymentGateway(
                                                         APICALLBOOM_PARAM,
                                                         _toPay,
                                                         _gatewayType,
                                                         _domain);

    //2: get payment gateway driver
    intfPaymentGateway* PaymentGatewayDriver = PaymentLogic::getDriver(PaymentGatewayDTO.pgwDriver);

    //3: create payment
    TAPI::MD5_t onpMD5 = OnlinePayments::instance().callSP(APICALLBOOM_PARAM,
                                                           "spOnlinePayment_Create", {
                                                               { "iVoucherID", _vchID },
                                                               { "iGatewayID", PaymentGatewayDTO.pgwID },
                                                               { "iAmount", _toPay },
                                                               { "iCallbackUrl", _paymentVerifyCallback },
                                                               { "iTargetWalID", _walID },
                                                           })
                         .spDirectOutputs()
                         .value("oMD5")
                         .toString()
                         ;

    try {
        _outPaymentKey = onpMD5;

        QString Callback = QString("https://%1%2Account/OnlinePayments/paymentCallback?paymentKey=%3")
                           .arg(APICALLBOOM_PARAM.hostAndPort())
                           .arg(ServerCommonConfigs::BasePathWithVersion)
                           .arg(onpMD5);

//        QString Callback = URLHelper::addParameter(_paymentVerifyCallback, "paymentKey", onpMD5);

        //4: call driver::request
        auto [Response, TrackID, PaymentLink] = PaymentGatewayDriver->prepareAndRequest(
                APICALLBOOM_PARAM,
                PaymentGatewayDTO,
                onpMD5,
                _toPay,
                Callback,
                _invDesc
                );

        OnlinePayments::instance().Update(APICALLBOOM_PARAM, //SYSTEM_USER_ID,
                {},
                TAPI::ORMFields_t({
                   { tblOnlinePayments::Fields::onpTrackNumber, TrackID },
                   { tblOnlinePayments::Fields::onpResult, Response },
                   { tblOnlinePayments::Fields::onpStatus, Targoman::API::AccountModule::enuPaymentStatus::Pending },
                }),
                {
                   { tblOnlinePayments::Fields::onpMD5, onpMD5 }
                });

        //increase pgwSumRequestCount and pgwSumRequestAmount
        try {
            PaymentGateways::instance().callSP(APICALLBOOM_PARAM,
                                               "spPaymentGateway_UpdateRequestCounters", {
                                                   { "iPgwID", PaymentGatewayDTO.pgwID },
                                                   { "iAmount", _toPay },
                                               })
            ;
        } catch (...) { ; }

        //5: return result for client redirecting
        return PaymentLink;

    } catch (std::exception &_exp) {
        OnlinePayments::instance().Update(APICALLBOOM_PARAM, //SYSTEM_USER_ID,
                    {},
                    TAPI::ORMFields_t({
                       { tblOnlinePayments::Fields::onpResult, _exp.what() },
                       { tblOnlinePayments::Fields::onpStatus, Targoman::API::AccountModule::enuPaymentStatus::Error },
                    }),
                    {
                       { tblOnlinePayments::Fields::onpMD5, onpMD5 }
                    });

        throw exPayment(QString("Unable to create payment request: ") + _exp.what());
    }
}

// [PaymentID, VoucherID, TargetWalletID, PaymentAmount]
std::tuple<quint64, quint64, quint64, quint64> PaymentLogic::approveOnlinePayment(
    INTFAPICALLBOOM_IMPL &APICALLBOOM_PARAM,
    const QString& _paymentKey,
    const TAPI::JSON_t& _pgResponse
//    QString _domain
) {
//    _domain = URLHelper::domain(_domain, true);

    if (_paymentKey.isEmpty())
        throw exPayment("paymentKey is empty");

    QVariantMap OnlinePaymentInfo = OnlinePayments::instance().GetSelectQuery(APICALLBOOM_PARAM)
            .addCols(OnlinePayments::instance().SelectableColumnNames())
            .addCols(PaymentGateways::instance().SelectableColumnNames())
            .innerJoinWith("paymentGateway")
            .where({ tblOnlinePayments::Fields::onpMD5, enuConditionOperator::Equal, _paymentKey })
            .one();

    stuOnlinePayment OnlinePayment;
    OnlinePayment.fromVariantMap(OnlinePaymentInfo);

    if (OnlinePayment.OnlinePayment.onpStatus != Targoman::API::AccountModule::enuPaymentStatus::Pending)
        throw exPayment("Only Pending online payments allowed");

    intfPaymentGateway* PaymentGatewayDriver = PaymentLogic::getDriver(OnlinePayment.PaymentGateway.pgwDriver);

    try {
        auto [Response, refNumber] = PaymentGatewayDriver->verifyAndSettle(
                APICALLBOOM_PARAM,
                OnlinePayment.PaymentGateway,
                OnlinePayment.OnlinePayment,
                _pgResponse
//                _domain
                );

        //PaymentResponse.PaymentKey =?= _paymentKey

        OnlinePayments::instance().Update(APICALLBOOM_PARAM, //SYSTEM_USER_ID,
                    {},
                    TAPI::ORMFields_t({
    //                    { tblOnlinePayments::Fields::onpTrackNumber, PaymentResponse.TrackID },
                        { tblOnlinePayments::Fields::onpResult, Response },
                        { tblOnlinePayments::Fields::onpStatus, Targoman::API::AccountModule::enuPaymentStatus::Payed },
                    }),
                    {
                        { tblOnlinePayments::Fields::onpMD5, _paymentKey }
                    });

        try {
            PaymentGateways::instance().callSP(APICALLBOOM_PARAM,
                                               "spPaymentGateway_UpdateOkCounters", {
                                                   { "iPgwID", OnlinePayment.OnlinePayment.onp_pgwID },
                                                   { "iAmount", OnlinePayment.OnlinePayment.onpAmount },
                                               })
            ;
        } catch (...) { ; }

        return {
            OnlinePayment.OnlinePayment.onpID,
            OnlinePayment.OnlinePayment.onp_vchID,
            NULLABLE_GET_OR_DEFAULT(OnlinePayment.OnlinePayment.onpTarget_walID, 0),
            OnlinePayment.OnlinePayment.onpAmount
        };

    } catch (std::exception &_exp) {
        OnlinePayments::instance().Update(APICALLBOOM_PARAM, //SYSTEM_USER_ID,
                    {},
                    TAPI::ORMFields_t({
                        { tblOnlinePayments::Fields::onpResult, _exp.what() },
                        { tblOnlinePayments::Fields::onpStatus, Targoman::API::AccountModule::enuPaymentStatus::Error },
                    }),
                    {
                        { tblOnlinePayments::Fields::onpMD5, _paymentKey }
                    });

        //increase pgwSumFailedCount
        PaymentGateways::instance().callSP(APICALLBOOM_PARAM,
                                           "spPaymentGateway_UpdateFailedCounters", {
                                               { "iPgwID", OnlinePayment.OnlinePayment.onp_pgwID },
                                               { "iAmount", OnlinePayment.OnlinePayment.onpAmount },
                                           })
        ;

        throw exPayment(QString("Unable to verify payment: ") + _exp.what());
    }
}

} //namespace Targoman::API::AccountModule::Payment
