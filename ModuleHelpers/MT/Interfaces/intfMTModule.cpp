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

namespace Targoman::API::ModuleHelpers::MT::Interfaces {

//using namespace Helpers;

template <bool _itmplIsTokenBase>
intfMTModule<_itmplIsTokenBase>::intfMTModule(
        const QString                               &_module,
        const QString                               &_schema,
        //Account --
        AssetUsageLimitsCols_t                      _exclusiveAssetUsageLimitsCols,
        intfAccountUnits                            *_units,
        intfAccountProducts                         *_products,
        intfAccountSaleables                        *_saleables,
        intfAccountSaleablesFiles                   *_saleablesFiles,
        intfAccountUserAssets<_itmplIsTokenBase>    *_userAssets,
        intfAccountUserAssetsFiles                  *_userAssetsFiles,
        intfAccountAssetUsage<_itmplIsTokenBase>    *_assetUsages,
        intfAccountCoupons                          *_discounts,
        intfAccountPrizes                           *_prizes,
        //MT --
        intfMTCorrectionRules_Type                  *_correctionRules,
        intfMTDigestedTranslationLogs_Type          *_digestedTranslationLogs,
        intfMTMultiDic_Type                         *_multiDic,
        intfMTTokenStats_Type                       *_tokenStats,
        intfMTTranslatedPhrases_Type                *_translatedPhrases,
        intfMTTranslationLogs_Type                  *_translationLogs
    ) :
    intfAccountingBasedModule<_itmplIsTokenBase>(
        _module,
        _schema,
        mergeAssetUsageLimitsCols({
                { MTAction::TRANSLATE, {
                    /* day   */ {}, //tblAccountAssetUsageMTBase::ExtraFields::usgUsedWordsPerDay,
                    /* week  */ {}, //tblAccountAssetUsageMTBase::ExtraFields::usgUsedWordsPerWeek,
                    /* month */ {}, //tblAccountAssetUsageMTBase::ExtraFields::usgUsedWordsPerMonth,
                    /* total */ tblAccountAssetUsageMTBase::ExtraFields::usgUsedTotalWords
                }},
            }, _exclusiveAssetUsageLimitsCols),
        _units,
        _products,
        _saleables,
        _saleablesFiles,
        _userAssets,
        _userAssetsFiles,
        _assetUsages,
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

/*********************************************************************\
|** accounting *******************************************************|
\*********************************************************************/
template <bool _itmplIsTokenBase>
stuServiceCreditsInfo intfMTModule<_itmplIsTokenBase>::retrieveServiceCreditsInfo(
    INTFAPICALLBOOM_IMPL &APICALLBOOM_PARAM,
    quint64 _actorID,
    const ServiceUsage_t &_requestedUsage,
    const QString &_action
) {
    ORMSelectQuery SelectQuery = this->accountUserAssets()->makeSelectQuery(APICALLBOOM_PARAM);

    SelectQuery
        .leftJoinWith(tblAccountUserAssetsBase::Relation::Usage)
        .leftJoinWith(tblAccountUserAssetsBase::Relation::Saleable)

        .where({ tblAccountUserAssetsBase::Fields::uas_actorID, enuConditionOperator::Equal, _actorID })

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
//    QString CoalescePart = QString("COALESCE(%1,%2,%3)")
//                           .arg(tblAccountUserAssetsMTBase::ExtraFields::uasCreditTotalWords)
//                           .arg(tblAccountSaleablesMTBase::ExtraFields::slbCreditTotalWords)
//                           .arg(tblAccountProductsMTBase::ExtraFields::prdCreditTotalWords)
//                           ;

    SelectQuery.andWhere(clsCondition({ tblAccountAssetUsageMTBase::ExtraFields::usgUsedTotalWords, enuConditionOperator::Null })
                         .orCond({ tblAccountUserAssetsMTBase::ExtraFields::uasCreditTotalWords, enuConditionOperator::Null })
                         .orCond({ DBExpression::VALUE(QString("%1-%2")
                                        .arg(tblAccountUserAssetsMTBase::ExtraFields::uasCreditTotalWords)
                                        .arg(tblAccountAssetUsageMTBase::ExtraFields::usgUsedTotalWords)
                                   ),
                                   enuConditionOperator::Greater,
                                   0 }));
//                         .orCond({ DBExpression::VALUE(CoalescePart), enuConditionOperator::Null })
//                         .orCond({ DBExpression::VALUE(QString("%1-%2")
//                                        .arg(CoalescePart)
//                                        .arg(tblAccountAssetUsageMTBase::ExtraFields::usgUsedTotalWords)
//                                   ),
//                                   enuConditionOperator::Greater,
//                                   0 }));

//    for (auto _usageIter = _requestedUsage.constBegin();
//         _usageIter != _requestedUsage.constEnd();
//         _usageIter++
//    ) {
//        if (_usageIter.key() != RequestedUsage::CREDIT)
//            continue;

//        QVariantMap CreditValues = _usageIter.value().toMap();
//        QString CreditKey = CreditValues.firstKey();
//        qint64 CreditValue = CreditValues.first().toLongLong();

//        SelectQuery.andWhere({  })
//    }

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

        AssetItem.fromJson(JsonUserAssetInfo); // -> for save _lastFromJsonSource
        AssetItem.Product.fromJson(JsonUserAssetInfo);
        AssetItem.Saleable.fromJson(JsonUserAssetInfo);
        AssetItem.UserAsset.fromJson(JsonUserAssetInfo);
        AssetItem.AssetUsage.fromJson(JsonUserAssetInfo);

        if (DBCurrentDateTime.isNull())
            TAPI::setFromVariant(DBCurrentDateTime, UserAssetInfo.value(Targoman::API::CURRENT_TIMESTAMP));

        //-- --------------------------------
        UsageLimits_t SaleableUsageLimits;
        for (auto Iter = this->AssetUsageLimitsCols.begin();
            Iter != this->AssetUsageLimitsCols.end();
            Iter++
        ) {
            if (_action.isEmpty() || Iter.key() == _action) {
                //Key: action
                SaleableUsageLimits.insert(Iter.key(), {
                    NULLABLE_INSTANTIATE_FROM_QVARIANT(quint32, UserAssetInfo.value(Iter->PerDay)),
                    NULLABLE_INSTANTIATE_FROM_QVARIANT(quint32, UserAssetInfo.value(Iter->PerWeek)),
                    NULLABLE_INSTANTIATE_FROM_QVARIANT(quint32, UserAssetInfo.value(Iter->PerMonth)),
                    NULLABLE_INSTANTIATE_FROM_QVARIANT(quint64, UserAssetInfo.value(Iter->Total))
                });
            }
        }
        AssetItem.Digested.Limits = SaleableUsageLimits;

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
}

template <bool _itmplIsTokenBase>
bool intfMTModule<_itmplIsTokenBase>::isEmpty(
    INTFAPICALLBOOM_IMPL &APICALLBOOM_PARAM,
    const UsageLimits_t &_limits
) const {
}

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

    if (NULLABLE_HAS_VALUE(AccountSaleablesMTBaseDTO.slbCreditTotalWords))
        Result.insert(tblAccountUserAssetsMTBase::ExtraFields::uasCreditTotalWords,
                      NULLABLE_VALUE(AccountSaleablesMTBaseDTO.slbCreditTotalWords)
                      );
    else if (NULLABLE_HAS_VALUE(AccountProductsMTBaseDTO.prdCreditTotalWords))
        Result.insert(tblAccountUserAssetsMTBase::ExtraFields::uasCreditTotalWords,
                      NULLABLE_VALUE(AccountProductsMTBaseDTO.prdCreditTotalWords)
                      );
    //else leave uasCreditTotalWords as null

    return Result;
}

// expose
/*********************************************************************/
template class intfMTModule<false>;
template class intfMTModule<true>;

} //namespace Targoman::API::ModuleHelpers::MT::Interfaces

