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

namespace Targoman::API::ModuleHelpers::MT::Interfaces {

template <bool _itmplIsTokenBase>
intfMTModule<_itmplIsTokenBase>::intfMTModule(
        const QString                               &_module,
        const QString                               &_schema,
        //Account --
        AssetUsageLimitsCols_t                      _AssetUsageLimitsCols,
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
        _AssetUsageLimitsCols,
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

        .where({ tblAccountUserAssetsBase::Fields::uas_actorID, enuConditionOperator::Equal, _actorID })

        .andWhere(clsCondition({ tblAccountUserAssetsBase::Fields::uasCanUseFromDate, enuConditionOperator::Null })
            .orCond({ tblAccountUserAssetsBase::Fields::uasCanUseFromDate, enuConditionOperator::LessEqual,
                DBExpression::CURDATE() })
        )
        .andWhere(clsCondition({ tblAccountUserAssetsBase::Fields::uasCanUseToDate, enuConditionOperator::Null })
            .orCond({ tblAccountUserAssetsBase::Fields::uasCanUseToDate, enuConditionOperator::GreaterEqual,
                DBExpression::CURDATE() })
        )

        .andWhere(clsCondition({ tblAccountUserAssetsBase::Fields::uasCanUseFromHour, enuConditionOperator::Null })
            .orCond({ tblAccountUserAssetsBase::Fields::uasCanUseFromHour, enuConditionOperator::LessEqual,
                DBExpression::VALUE("HOUR(NOW())") })
        )
        .andWhere(clsCondition({ tblAccountUserAssetsBase::Fields::uasCanUseToHour, enuConditionOperator::Null })
            .orCond({ tblAccountUserAssetsBase::Fields::uasCanUseToHour, enuConditionOperator::GreaterEqual,
                DBExpression::VALUE("HOUR(NOW())") })
        )
    ;

    ///check remaining credit
    SelectQuery.andWhere(clsCondition({ tblAccountAssetUsageMTBase::ExtraFields::usgRemainingTotalWords, enuConditionOperator::Null })
                         .orCond({ tblAccountAssetUsageMTBase::ExtraFields::usgRemainingTotalWords,
                                 enuConditionOperator::Greater,
                                 0 }));

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
    TAPI::stuTable Table = SelectQuery.pageSize(0).all();

    QDateTime DBCurrentDateTime;
    ActiveCredits_t ActiveCredits;
    NULLABLE_TYPE(stuAssetItem) PreferedCredit = NULLABLE_NULL_VALUE;

    foreach (QVariant Row, Table.Rows) {
        QVariantMap UserAssetInfo = Row.toMap();
        QJsonObject JsonUserAssetInfo = QJsonObject::fromVariantMap(UserAssetInfo);

        stuAssetItem AssetItem;

        AssetItem.fromJson(JsonUserAssetInfo); // -> for save _lastFromJsonSource
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
    const QString &_action
) {
    /*
                         1         2         3
        days : 0         0         0         0
        now  :           |
     -- before break:
        a    : |-----------------------------|  total: 100 words / remaining: 30 words / Duration: 30 Days
     -- after break:
        a    : |---------|                      total: 70 words  / remaining: 0 words  / Duration: 10 Days / End date: NOW
        b    :           ?-------------------?  total: 30 words  / remaining: 30 words / Duration: 20 Days / Start & End date: NULL
    */

    tblAccountUserAssetsMTBase::DTO AccountUserAssetsMTBaseDTO;
    AccountUserAssetsMTBaseDTO.fromJson(_assetItem._lastFromJsonSource);

    //is unlimited?
    if (NULLABLE_IS_NULL(AccountUserAssetsMTBaseDTO.uasTotalWords))
        return;

    if (NULLABLE_IS_NULL(AccountUserAssetsMTBaseDTO.uasDurationDays))
        return;

    tblAccountAssetUsageMTBase::DTO AccountAssetUsageMTBaseDTO;
    AccountAssetUsageMTBaseDTO.fromJson(_assetItem._lastFromJsonSource);




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

template <bool _itmplIsTokenBase>
QVariantMap intfMTModule<_itmplIsTokenBase>::getCustomUserAssetFieldsForQuery(
    INTFAPICALLBOOM_IMPL    &APICALLBOOM_PARAM,
    INOUT stuBasketItem     &_basketItem,
    Q_DECL_UNUSED const stuVoucherItem    *_oldVoucherItem /*= nullptr*/
) {
    QVariantMap Result;

    tblAccountSaleablesMTBase::DTO AccountSaleablesMTBaseDTO;
    AccountSaleablesMTBaseDTO.fromJson(_basketItem._lastFromJsonSource);

    if (NULLABLE_HAS_VALUE(AccountSaleablesMTBaseDTO.slbTotalWords))
        Result.insert(tblAccountUserAssetsMTBase::ExtraFields::uasTotalWords,
                      NULLABLE_VALUE(AccountSaleablesMTBaseDTO.slbTotalWords)
                      );

    return Result;
}
/*********************************************************************/

template class intfMTModule<false>;
template class intfMTModule<true>;

} //namespace Targoman::API::ModuleHelpers::MT::Interfaces

