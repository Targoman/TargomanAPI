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
 * @author S.Mehran M.Ziabary <ziabary@targoman.com>
 * @author Kambiz Zandi <kambizzandi@gmail.com>
 */

#include "intfMTModule.h"
#include "MTDefs.hpp"
#include "intfMTAccounting.h"
//#include "Interfaces/Helpers/JSonHelper.h"
#include "libTargomanDBM/Definitions.h"

namespace Targoman::API::ModuleHelpers::MT::Interfaces {

//using namespace Helpers;
using namespace DBManager;

template <bool _itmplIsTokenBase>
intfMTModule<_itmplIsTokenBase>::intfMTModule(
        const QString                                   &_module,
        const QString                                   &_schema,
        //Account --
        AssetUsageLimitsCols_t                          _exclusiveAssetUsageLimitsCols,
        intfAccountUnits                                *_units,
        intfAccountProducts                             *_products,
        intfAccountSaleables                            *_saleables,
        intfAccountSaleablesFiles                       *_saleablesFiles,
        intfAccountUserAssets<_itmplIsTokenBase>        *_userAssets,
        intfAccountUserAssetsFiles                      *_userAssetsFiles,
        intfAccountAssetUsage<_itmplIsTokenBase>        *_assetUsage,
        intfAccountCoupons                              *_discounts,
        intfAccountPrizes                               *_prizes,
        //MT --
        intfMTCorrectionRules_Type                      *_correctionRules,
        intfMTDigestedTranslationLogs_Type              *_digestedTranslationLogs,
        intfMTMultiDic_Type                             *_multiDic,
        intfMTTokenStats_Type                           *_tokenStats,
        intfMTTranslatedPhrases_Type                    *_translatedPhrases,
        intfMTTranslationLogs_Type                      *_translationLogs
    ) :
    intfAccountingBasedModule<_itmplIsTokenBase>(
        _module,
        _schema,
        mergeAssetUsageLimitsCols({
                { MTAction::TRANSLATE, {
                    /* asset-day   */ {}, //tblAccountUserAssetsMTBase::ExtraFields::uasCreditWordsPerDay,
//                    /* usage-day   */ {}, //tblAccountAssetUsageMTBase::ExtraFields::usgUsedWordsPerDay,
                    /* asset-week  */ {}, //tblAccountUserAssetsMTBase::ExtraFields::uasCreditWordsPerWeek,
//                    /* usage-week  */ {}, //tblAccountAssetUsageMTBase::ExtraFields::usgUsedWordsPerWeek,
                    /* asset-month */ {}, //tblAccountUserAssetsMTBase::ExtraFields::uasCreditWordsPerMonth,
//                    /* usage-month */ {}, //tblAccountAssetUsageMTBase::ExtraFields::usgUsedWordsPerMonth,
                    /* asset-total */ {} //tblAccountUserAssetsMTBase::ExtraFields::uasCreditTotalWords,
//                    /* usage-total */ {} //tblAccountAssetUsageMTBase::ExtraFields::usgUsedTotalWords
                }},
            }, _exclusiveAssetUsageLimitsCols),
        _units,
        _products,
        _saleables,
        _saleablesFiles,
        _userAssets,
        _userAssetsFiles,
        _assetUsage,
        _discounts,
        _prizes
    ),
    MTCorrectionRules(_correctionRules),
    MTDigestedTranslationLogs(_digestedTranslationLogs),
    MTMultiDic(_multiDic),
    MTTokenStats(_tokenStats),
    MTTranslatedPhrases(_translatedPhrases),
    MTTranslationLogs(_translationLogs)
{ ; }

// basket
/*********************************************************************/
template <bool _itmplIsTokenBase>
QVariantMap intfMTModule<_itmplIsTokenBase>::getCustomUserAssetFieldsForQuery(
    INTFAPICALLBOOM_IMPL    &APICALLBOOM_PARAM,
    INOUT stuBasketItem     &_basketItem,
    Q_DECL_UNUSED const stuVoucherItem    *_oldVoucherItem /*= nullptr*/
) {
    QVariantMap Result;

    tblAccountProductsMTBase::DTO AccountProductsMTBaseDTO;
    AccountProductsMTBaseDTO.fromJson(_basketItem._lastFromJsonSource);

    tblAccountSaleablesMTBase::DTO AccountSaleablesMTBaseDTO;
    AccountSaleablesMTBaseDTO.fromJson(_basketItem._lastFromJsonSource);

    if (AccountProductsMTBaseDTO.prdCreditSpecs.isEmpty() == false)
        Result.insert(tblAccountUserAssetsMTBase::ExtraFields::uasCreditSpecs,
                      AccountProductsMTBaseDTO.prdCreditSpecs
                      );

    return Result;
}

// accounting
/*********************************************************************/

NULLABLE_TYPE(stuAssetCredit) extractCreditValue(
    QVariantMap _creditSpecs,
    QString _creditKey
) {
    if (_creditKey.isEmpty())
        return NULLABLE_NULL_VALUE;

    QStringList Keys = _creditKey.split("::");

    auto FnExtractCreditValues = [](QVariant &_value) -> NULLABLE_TYPE(stuAssetCredit) {

        if (_value.canConvert<QVariantMap>() == false) { //.userType() != QMetaType::QVariantMap)
            stuAssetCredit Credit;

            //total
            if (_value.toLongLong() >= 0)
                Credit.Total = _value.toLongLong();

            return Credit;
        }

        QVariantMap ValueAsMap = _value.toMap();

        if (ValueAsMap.contains("day")
            || ValueAsMap.contains("week")
            || ValueAsMap.contains("month")
            || ValueAsMap.contains("total")
        ) {
            stuAssetCredit Credit;

            //day
            if (ValueAsMap.contains("day") && (ValueAsMap["day"].toLongLong() >= 0))
                Credit.PerDay = ValueAsMap["day"].toLongLong();

            //week
            if (ValueAsMap.contains("week") && (ValueAsMap["week"].toLongLong() >= 0))
                Credit.PerWeek = ValueAsMap["week"].toLongLong();

            //month
            if (ValueAsMap.contains("month") && (ValueAsMap["month"].toLongLong() >= 0))
                Credit.PerMonth = ValueAsMap["month"].toLongLong();

            //total
            if (ValueAsMap.contains("total") && (ValueAsMap["total"].toLongLong() >= 0))
                Credit.Total = ValueAsMap["total"].toLongLong();

            return Credit;
        }

        return NULLABLE_NULL_VALUE;
    };

//    while (Keys.length()) {
        QString Key = Keys.first().toLower();
        Keys.removeFirst();

        if (_creditSpecs.contains(Key)) {
            QVariant Value = _creditSpecs[Key];

            NULLABLE_TYPE(stuAssetCredit) Credit = FnExtractCreditValues(Value);
            if (Credit != NULLABLE_NULL_VALUE)
                return Credit;

            QVariantMap ValueAsMap = Value.toMap();
            auto Ret = extractCreditValue(
                           ValueAsMap,
                           Keys.join("::")
                           );

            if (Ret != NULLABLE_NULL_VALUE)
                return Ret;
        }

        //Key not found? look at neighbour `ALL`
        if (_creditSpecs.contains("ALL")) {
            QVariant Value = _creditSpecs["ALL"];

            NULLABLE_TYPE(stuAssetCredit) Credit = FnExtractCreditValues(Value);
            if (Credit != NULLABLE_NULL_VALUE)
                return Credit;

            throw exHTTPInternalServerError("Incorrect value for credit");
        }
//    }

    return NULLABLE_NULL_VALUE;
}


template <bool _itmplIsTokenBase>
stuServiceCreditsInfo intfMTModule<_itmplIsTokenBase>::retrieveServiceCreditsInfo(
    INTFAPICALLBOOM_IMPL &APICALLBOOM_PARAM,
    quint64 _actorID,
    const ServiceUsage_t &_requestedUsage,
    const QString &_action
) {
    QString CreditKey;
    for (auto UsageIter = _requestedUsage.begin();
         UsageIter != _requestedUsage.end();
         UsageIter++
    ) {
        if (UsageIter.key() != RequestedUsage::CREDIT)
            continue;

        QVariantMap CreditValues = UsageIter.value().toMap();

        CreditKey = CreditValues.firstKey();
        if (CreditKey.startsWith(QString("%1::").arg(MTAction::TRANSLATE)))
            CreditKey = CreditKey.mid(QString("%1::").arg(MTAction::TRANSLATE).length());

//        qint64 UsedWordCount = CreditValues.first().toLongLong();
        break;
    }

    if (CreditKey.isEmpty())
        throw exHTTPInternalServerError("Credit key is empty");

    ORMSelectQuery SelectQuery = this->accountUserAssets()->makeSelectQuery(APICALLBOOM_PARAM)
                                 .where({ tblAccountUserAssetsBase::Fields::uasStatus,
                                          enuConditionOperator::Equal,
                                          enuAuditableStatus::Active })
                                 .addCols(this->accountUserAssets()->selectableColumnNames())
                                 ;

    SelectQuery
        //total:
        .nestedLeftJoin(this->AccountAssetUsage->makeSelectQuery(APICALLBOOM_PARAM, "", true, false)
                        .where({ tblAccountAssetUsageBase::Fields::usgResolution,
                                 enuConditionOperator::Equal,
                                 Targoman::API::AAA::enuAssetUsageResolution::Total
                               })
                        .andWhere({
                                      { enuAggregation::LOWER, tblAccountAssetUsageBase::Fields::usgKey },
                                      enuConditionOperator::Equal,
                                      CreditKey.toLower()
                                  })
//                        , tblAccountAssetUsageBase::Name,
                        , "tblAccountAssetUsage_Total",
                        { "tblAccountAssetUsage_Total", tblAccountAssetUsageBase::Fields::usg_uasID,
                          enuConditionOperator::Equal,
                          tblAccountUserAssetsBase::Name, tblAccountUserAssetsBase::Fields::uasID }
                        )
//        .addCols(this->accountAssetUsage()->selectableColumnNames())
        .addCol(R("tblAccountAssetUsage_Total", tblAccountAssetUsageMTBase::ExtraFields::usgUsedTotalWords),
//                tblAccountAssetUsageMTBase::ExtraFields::usgUsedTotalWords
                "UsedTotal"
                )

        //day:
        .nestedLeftJoin(this->AccountAssetUsage->makeSelectQuery(APICALLBOOM_PARAM, "", true, false)
                        .addCol(tblAccountAssetUsageBase::Fields::usg_uasID)
                        .addCol(enuAggregation::SUM,
                                tblAccountAssetUsageMTBase::ExtraFields::usgUsedTotalWords,
                                "SumUsed")
                        .where({ tblAccountAssetUsageBase::Fields::usgResolution,
                                 enuConditionOperator::Equal,
                                 Targoman::API::AAA::enuAssetUsageResolution::Day
                               })
                        .andWhere({
                                      { enuAggregation::LOWER, tblAccountAssetUsageBase::Fields::usgKey },
                                      enuConditionOperator::Equal,
                                      CreditKey.toLower()
                                  })
                        .groupBy(tblAccountAssetUsageBase::Fields::usg_uasID)
                        , "tblAccountAssetUsage_perDay",
                        { "tblAccountAssetUsage_perDay", tblAccountAssetUsageBase::Fields::usg_uasID,
                          enuConditionOperator::Equal,
                          tblAccountUserAssetsBase::Name, tblAccountUserAssetsBase::Fields::uasID }
                        )
        .addCol(R("tblAccountAssetUsage_perDay", "SumUsed"), "UsedPerDay")
    ;

    SelectQuery
        .leftJoinWith(tblAccountUserAssetsBase::Relation::Saleable)
        .addCols(this->accountSaleables()->selectableColumnNames())
    ;

    SelectQuery
        .inlineLeftJoin(tblAccountProductsBase::Name/*, tblAccountProductsBase::Name*/, clsCondition(
                        tblAccountProductsBase::Name,
                        tblAccountProductsBase::Fields::prdID,
                        enuConditionOperator::Equal,
                        tblAccountSaleablesBase::Name,
                        tblAccountSaleablesBase::Fields::slb_prdID
                        ))
        .addCols(this->accountProducts()->selectableColumnNames())
    ;

    SelectQuery
        .andWhere({ tblAccountUserAssetsBase::Fields::uas_actorID, enuConditionOperator::Equal, _actorID })

        .andWhere(clsCondition({ tblAccountUserAssetsBase::Fields::uasValidFromDate, enuConditionOperator::Null })
            .orCond({ tblAccountUserAssetsBase::Fields::uasValidFromDate, enuConditionOperator::LessEqual,
                DBExpression::NOW() })
        )
        .andWhere(clsCondition({ tblAccountUserAssetsBase::Fields::uasValidToDate, enuConditionOperator::Null })
            .orCond({ tblAccountUserAssetsBase::Fields::uasValidToDate, enuConditionOperator::GreaterEqual,
                DBExpression::NOW() })
        )

        .andWhere(clsCondition({ tblAccountUserAssetsBase::Fields::uasValidFromHour, enuConditionOperator::Null })
            .orCond({ tblAccountUserAssetsBase::Fields::uasValidFromHour, enuConditionOperator::LessEqual,
                DBExpression::VALUE("HOUR(NOW())") })
        )
        .andWhere(clsCondition({ tblAccountUserAssetsBase::Fields::uasValidToHour, enuConditionOperator::Null })
            .orCond({ tblAccountUserAssetsBase::Fields::uasValidToHour, enuConditionOperator::GreaterEqual,
                DBExpression::VALUE("HOUR(NOW())") })
        )
    ;

    ///check remaining credit
//    SelectQuery.andWhere(clsCondition({ tblAccountAssetUsageMTBase::ExtraFields::usgUsedTotalWords, enuConditionOperator::Null })
//                         .orCond({ tblAccountUserAssetsMTBase::ExtraFields::uasCreditSpecs, enuConditionOperator::Null })
//                         .orCond({ DBExpression::VALUE(QString("%1-%2")
//                                        .arg(tblAccountUserAssetsMTBase::ExtraFields::uasCreditSpecs)
//                                        .arg(tblAccountAssetUsageMTBase::ExtraFields::usgUsedTotalWords)
//                                   ),
//                                   enuConditionOperator::Greater,
//                                   0 }));

    //-------------------------------
    SelectQuery.orderBy(tblAccountUserAssetsBase::Fields::uasDurationMinutes);

    //-------------------------------
    TAPI::stuTable Table = SelectQuery.pageSize(0).all();

    QDateTime DBCurrentDateTime;
    ActiveCredits_t ActiveCredits;
    NULLABLE_TYPE(stuAssetItem) PreferedCredit = NULLABLE_NULL_VALUE;

    foreach (QVariant Row, Table.Rows) {
        QVariantMap UserAssetInfo = Row.toMap();
        QJsonObject JsonUserAssetInfo = QJsonObject::fromVariantMap(UserAssetInfo);

        stuAssetItem AssetItem;

        //-- --------------------------------
        AssetItem.fromJson(JsonUserAssetInfo); // -> for save _lastFromJsonSource
        AssetItem.Product.fromJson(JsonUserAssetInfo);
        AssetItem.Saleable.fromJson(JsonUserAssetInfo);
        AssetItem.UserAsset.fromJson(JsonUserAssetInfo);
//        AssetItem.AssetUsage.fromJson(JsonUserAssetInfo);

        if (DBCurrentDateTime.isNull())
            TAPI::setFromVariant(DBCurrentDateTime, UserAssetInfo.value(Targoman::API::CURRENT_TIMESTAMP));

        //-- --------------------------------
        QVariantMap CreditSpecs;
        if (UserAssetInfo.contains(tblAccountUserAssetsMTBase::ExtraFields::uasCreditSpecs))
            CreditSpecs = UserAssetInfo[tblAccountUserAssetsMTBase::ExtraFields::uasCreditSpecs].toMap();
        if (CreditSpecs.isEmpty() && UserAssetInfo.contains(tblAccountProductsMTBase::ExtraFields::prdCreditSpecs))
            CreditSpecs = UserAssetInfo[tblAccountProductsMTBase::ExtraFields::prdCreditSpecs].toMap();

        if (CreditSpecs.isEmpty()) {
            //set to zero if not defined?
//                AssetItem.Credit = stuAssetCredit(0, 0, 0, 0);
        } else {
            auto Value = extractCreditValue(CreditSpecs, CreditKey);
            if (NULLABLE_HAS_VALUE(Value))
                AssetItem.Credit = NULLABLE_VALUE(Value);
        }

        //zero credit is not allowed
        if ((AssetItem.Credit.PerDay == 0)
                && (AssetItem.Credit.PerWeek == 0)
                && (AssetItem.Credit.PerMonth == 0)
                && (AssetItem.Credit.Total == 0)
            )
            continue;

//        quint32 usgUsedTotalWords = 0;
//        if (UserAssetInfo.contains(tblAccountAssetUsageMTBase::ExtraFields::usgUsedTotalWords))
//            usgUsedTotalWords = UserAssetInfo[tblAccountAssetUsageMTBase::ExtraFields::usgUsedTotalWords].toULongLong();

//        //check remaining credit
//        if ((AssetItem.Credit.Total > 0)
//                && (AssetItem.Credit.Total <= usgUsedTotalWords))
//            continue;

        //-- remaining credit --------------------------------
        UsageLimits_t SaleableUsageLimits;

        stuUsage Usage(-1, -1, -1, -1);

        //day
        if (UserAssetInfo.contains("UsedPerDay") && (AssetItem.Credit.PerDay > 0)) {
            qint32 UsedPerDay = UserAssetInfo["UsedPerDay"].toLongLong();
            if (AssetItem.Credit.PerDay <= UsedPerDay)
                continue;
            Usage.PerDay = NULLABLE_VALUE(AssetItem.Credit.PerDay) - UsedPerDay;
        }

        //week

        //month

        //total
        if (UserAssetInfo.contains("UsedTotal") && (AssetItem.Credit.Total > 0)) {
            qint32 UsedTotal = UserAssetInfo["UsedTotal"].toLongLong();
            if (AssetItem.Credit.Total <= UsedTotal)
                continue;
            Usage.Total = NULLABLE_VALUE(AssetItem.Credit.Total) - UsedTotal;
        }

        SaleableUsageLimits.insert(_action, Usage);

        /*
        for (auto Iter = this->AssetUsageLimitsCols.begin();
            Iter != this->AssetUsageLimitsCols.end();
            Iter++
        ) {
            if (_action.isEmpty() || Iter.key() == _action) {

                stuUsage Usage;

                if (Iter->AssetPerDay.isEmpty() == false)
                    Usage.PerDay = UserAssetInfo.value(Iter->AssetPerDay).toUInt()
                                   - (UserAssetInfo.value(Iter->UsagePerDay).isValid()
                                      ? UserAssetInfo.value(Iter->UsagePerDay).toUInt()
                                      : 0);

                if (Iter->AssetPerWeek.isEmpty() == false)
                    Usage.PerWeek = UserAssetInfo.value(Iter->AssetPerWeek).toUInt()
                                   - (UserAssetInfo.value(Iter->UsagePerWeek).isValid()
                                      ? UserAssetInfo.value(Iter->UsagePerWeek).toUInt()
                                      : 0);

                if (Iter->AssetPerMonth.isEmpty() == false)
                    Usage.PerMonth = UserAssetInfo.value(Iter->AssetPerMonth).toUInt()
                                   - (UserAssetInfo.value(Iter->UsagePerMonth).isValid()
                                      ? UserAssetInfo.value(Iter->UsagePerMonth).toUInt()
                                      : 0);

                if (Iter->AssetTotal.isEmpty() == false)
                    Usage.Total = UserAssetInfo.value(Iter->AssetTotal).toUInt()
                                   - (UserAssetInfo.value(Iter->UsageTotal).isValid()
                                      ? UserAssetInfo.value(Iter->UsageTotal).toUInt()
                                      : 0);

                //Key: action
                SaleableUsageLimits.insert(Iter.key(), Usage);
//                {
//                    NULLABLE_INSTANTIATE_FROM_QVARIANT(quint32, UserAssetInfo.value(Iter->PerDay)),
//                    NULLABLE_INSTANTIATE_FROM_QVARIANT(quint32, UserAssetInfo.value(Iter->PerWeek)),
//                    NULLABLE_INSTANTIATE_FROM_QVARIANT(quint32, UserAssetInfo.value(Iter->PerMonth)),
//                    NULLABLE_INSTANTIATE_FROM_QVARIANT(quint64, UserAssetInfo.value(Iter->Total))
//                });
            }
        }
        */
        AssetItem.Digested.Limits = SaleableUsageLimits;

        //-- --------------------------------
        this->digestPrivs(APICALLBOOM_PARAM, AssetItem);

        //-- --------------------------------
        ActiveCredits.append(AssetItem);

        if (AssetItem.UserAsset.uasPrefered)
            PreferedCredit = AssetItem;
    }

    return stuServiceCreditsInfo(
        /* activeCredits     */ ActiveCredits,
        /* preferedCredit    */ PreferedCredit,
        /* parentID          */ NULLABLE_NULL_VALUE,
        /* myLimitsOnParent  */ {},
        /* dbCurrentDateTime */ DBCurrentDateTime
    );
}

template <bool _itmplIsTokenBase>
void intfMTModule<_itmplIsTokenBase>::breakCredit(
    INTFAPICALLBOOM_IMPL &APICALLBOOM_PARAM,
    const stuAssetItem &_assetItem,
    Q_DECL_UNUSED const QString &_action
) {
    /*
                        1         2         3
     days   : 0         0         0         0
     before : |-----------------------------|  total: 100 words / used: 30 words / Duration: 30 Days
     now    :           |
     after  :           ?-------------------?  total: 100 words / used: 30 words / Duration: 20 Days / Start & End date: NULL
    */

//    tblAccountProductsMTBase::DTO AccountProductsMTBaseDTO;
//    AccountProductsMTBaseDTO.fromJson(_assetItem._lastFromJsonSource);

//    tblAccountSaleablesMTBase::DTO AccountSaleablesMTBaseDTO;
//    AccountSaleablesMTBaseDTO.fromJson(_assetItem._lastFromJsonSource);

    tblAccountUserAssetsMTBase::DTO AccountUserAssetsMTBaseDTO;
    AccountUserAssetsMTBaseDTO.fromJson(_assetItem._lastFromJsonSource);

//    tblAccountAssetUsageMTBase::DTO AccountAssetUsageMTBaseDTO;
//    AccountAssetUsageMTBaseDTO.fromJson(_assetItem._lastFromJsonSource);

    if (AccountUserAssetsMTBaseDTO.uasValidFromDate.isValid() == false)
        return;

    if (AccountUserAssetsMTBaseDTO.uasValidToDate.isValid() == false)
        return;

    if (NULLABLE_IS_NULL(AccountUserAssetsMTBaseDTO.uasDurationMinutes))
        return;

    QDateTime DBCurrentDateTime;
    TAPI::setFromVariant(DBCurrentDateTime, _assetItem._lastFromJsonSource.value(Targoman::API::CURRENT_TIMESTAMP));

    quint32 MinutesElapsed = abs(AccountUserAssetsMTBaseDTO.uasValidFromDate.secsTo(DBCurrentDateTime)) / 60;
    quint32 MinutesRemained = NULLABLE_VALUE(AccountUserAssetsMTBaseDTO.uasDurationMinutes) - MinutesElapsed;

//    if (NULLABLE_IS_NULL(AccountUserAssetsMTBaseDTO.uasCreditTotalWords))
//        return;

    //-------------
    ORMUpdateQuery UpdateQuery = this->accountUserAssets()->makeUpdateQuery(APICALLBOOM_PARAM);

    UpdateQuery
            .set(tblAccountUserAssetsBase::Fields::uasValidFromDate, DBExpression::NIL())
            .set(tblAccountUserAssetsBase::Fields::uasValidToDate, DBExpression::NIL())
            .set(tblAccountUserAssetsBase::Fields::uasDurationMinutes, MinutesRemained)
            .set(tblAccountUserAssetsBase::Fields::uasBreakedAt, DBExpression::NOW())
            .where({ tblAccountUserAssetsBase::Fields::uasID, enuConditionOperator::Equal, AccountUserAssetsMTBaseDTO.uasID })
            ;

    UpdateQuery.execute(APICALLBOOM_PARAM.getActorID());
}

template <bool _itmplIsTokenBase>
bool intfMTModule<_itmplIsTokenBase>::isUnlimited(
    INTFAPICALLBOOM_IMPL &APICALLBOOM_PARAM,
    const UsageLimits_t &_limits
) const {

    for (auto it = _limits.constBegin();
         it != _limits.constEnd();
         it++
    ) {
        if (NULLABLE_HAS_VALUE(it->PerDay))
            return false;

        if (NULLABLE_HAS_VALUE(it->PerWeek))
            return false;

        if (NULLABLE_HAS_VALUE(it->PerMonth))
            return false;

        if (NULLABLE_HAS_VALUE(it->Total))
            return false;
    }

    return true;
}

template <bool _itmplIsTokenBase>
bool intfMTModule<_itmplIsTokenBase>::isEmpty(
    INTFAPICALLBOOM_IMPL &APICALLBOOM_PARAM,
    const UsageLimits_t &_limits
) const {

    for (auto it = _limits.constBegin();
         it != _limits.constEnd();
         it++
    ) {
        if (NULLABLE_HAS_VALUE(it->PerDay) && NULLABLE_VALUE(it->PerDay) > 0)
            return false;

        if (NULLABLE_HAS_VALUE(it->PerWeek) && NULLABLE_VALUE(it->PerWeek) > 0)
            return false;

        if (NULLABLE_HAS_VALUE(it->PerMonth) && NULLABLE_VALUE(it->PerMonth) > 0)
            return false;

        if (NULLABLE_HAS_VALUE(it->Total) && NULLABLE_VALUE(it->Total) > 0)
            return false;
    }

    return true;
}

template <bool _itmplIsTokenBase>
void intfMTModule<_itmplIsTokenBase>::saveAccountUsage(
    INTFAPICALLBOOM_IMPL &APICALLBOOM_PARAM,
    stuActiveCredit &_activeCredit,
    const ServiceUsage_t &_requestedUsage,
    const QString &_action
) {
    if (_activeCredit.Credit.UserAsset.uasID == 0)
        return;

    QString QueryString;

    // start and end dates
    //---------------------------------------------
    if (_activeCredit.Credit.Product.prdStartAtFirstUse
        && (_activeCredit.Credit.UserAsset.uasValidFromDate.isValid() == false)
        && (_activeCredit.Credit.UserAsset.uasValidToDate.isValid() == false)
        && NULLABLE_HAS_VALUE(_activeCredit.Credit.UserAsset.uasDurationMinutes)
        && NULLABLE_VALUE(_activeCredit.Credit.UserAsset.uasDurationMinutes) > 0
    ) {
        QueryString = QString(R"(
            UPDATE %1
               SET uasValidFromDate = NOW()
                 , uasValidToDate = DATE_ADD(NOW(), INTERVAL uasDurationMinutes MINUTE)
                 , uasUpdatedBy_usrID = ?
             WHERE uasID = ?
)")
            .arg(tblAccountUserAssetsBase::Name)
        ;

        clsDACResult Result = this->accountUserAssets()->execQuery(APICALLBOOM_PARAM,
                                                                   QueryString,
                                                                   {
                                                                       APICALLBOOM_PARAM.getActorID(SYSTEM_USER_ID),
                                                                       _activeCredit.Credit.UserAsset.uasID
                                                                   });
    }

    // usage
    //---------------------------------------------
    for (auto UsageIter = _requestedUsage.begin();
         UsageIter != _requestedUsage.end();
         UsageIter++
    ) {
        if (UsageIter.key() != RequestedUsage::CREDIT)
            return;

        QVariantMap CreditValues = UsageIter.value().toMap();

        QString CreditKey = CreditValues.firstKey();
        if (CreditKey.startsWith(QString("%1::").arg(MTAction::TRANSLATE)))
            CreditKey = CreditKey.mid(QString("%1::").arg(MTAction::TRANSLATE).length());

        qint64 UsedWordCount = CreditValues.first().toLongLong();

        QueryString = QString(R"(
            INSERT INTO %1
               SET usg_uasID = ?
                 , usgResolution = ?
                 , usgLastDateTime = NOW()
                 , usgKey = '%2'
                 , usgUsedTotalWords = ?
                ON DUPLICATE KEY UPDATE
                   usgLastDateTime = NOW()
                 , usgUsedTotalWords = IFNULL(usgUsedTotalWords, 0) + ?
)")
            .arg(tblAccountAssetUsageBase::Name)
            .arg(CreditKey)
        ;

        QList<QChar> Resolutions = {
            QChar(enuAssetUsageResolution::Total),
//            QChar(enuAssetUsageResolution::Year),
//            QChar(enuAssetUsageResolution::Month),
//            QChar(enuAssetUsageResolution::Week),
            QChar(enuAssetUsageResolution::Day),
            QChar(enuAssetUsageResolution::Hour),
//            QChar(enuAssetUsageResolution::Minute),
        };

        foreach (QChar Resolution, Resolutions) {
            clsDACResult Result = this->accountAssetUsage()->execQuery(APICALLBOOM_PARAM,
                                                                       QueryString,
                                                                       {
                                                                           _activeCredit.Credit.UserAsset.uasID,
                                                                           Resolution,
                                                                           UsedWordCount,
                                                                           UsedWordCount,
                                                                       });
        }
    }
}

// expose
/*********************************************************************/
template class intfMTModule<false>;
template class intfMTModule<true>;

} //namespace Targoman::API::ModuleHelpers::MT::Interfaces
