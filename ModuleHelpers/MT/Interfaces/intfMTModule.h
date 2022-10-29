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

#ifndef TARGOMAN_API_MODULEHELPERS_MT_INTERFACES_INTFMTMODULE_H
#define TARGOMAN_API_MODULEHELPERS_MT_INTERFACES_INTFMTMODULE_H

#include "Interfaces/AAA/intfAccountingBasedModule.h"
#include "intfMTHelpers.h"
using namespace Targoman::API::AAA;

namespace Targoman::API::ModuleHelpers::MT::Interfaces {

template <bool _itmplIsTokenBase>
class intfMTModule : public intfAccountingBasedModule<_itmplIsTokenBase>
{

typedef intfMTCorrectionRules<_itmplIsTokenBase ? TAPI::enuTokenActorType::API : TAPI::enuTokenActorType::USER>
    intfMTCorrectionRules_Type;

typedef intfMTDigestedTranslationLogs<_itmplIsTokenBase ? TAPI::enuTokenActorType::API : TAPI::enuTokenActorType::USER>
    intfMTDigestedTranslationLogs_Type;

typedef intfMTMultiDic<_itmplIsTokenBase ? TAPI::enuTokenActorType::API : TAPI::enuTokenActorType::USER>
    intfMTMultiDic_Type;

typedef intfMTTokenStats<_itmplIsTokenBase ? TAPI::enuTokenActorType::API : TAPI::enuTokenActorType::USER>
    intfMTTokenStats_Type;

typedef intfMTTranslatedPhrases<_itmplIsTokenBase ? TAPI::enuTokenActorType::API : TAPI::enuTokenActorType::USER>
    intfMTTranslatedPhrases_Type;

typedef intfMTTranslationLogs<_itmplIsTokenBase ? TAPI::enuTokenActorType::API : TAPI::enuTokenActorType::USER>
    intfMTTranslationLogs_Type;

public:
    intfMTModule(
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
        intfAccountAssetUsageHistory<_itmplIsTokenBase> *_assetUsageHistory,
        intfAccountCoupons                              *_discounts,
        intfAccountPrizes                               *_prizes,
        //MT --
        intfMTCorrectionRules_Type                      *_correctionRules,
        intfMTDigestedTranslationLogs_Type              *_digestedTranslationLogs,
        intfMTMultiDic_Type                             *_multiDic,
        intfMTTokenStats_Type                           *_tokenStats,
        intfMTTranslatedPhrases_Type                    *_translatedPhrases,
        intfMTTranslationLogs_Type                      *_translationLogs
    );

protected:
    virtual stuServiceCreditsInfo retrieveServiceCreditsInfo(
        INTFAPICALLBOOM_DECL &APICALLBOOM_PARAM,
        quint64 _actorID,
        const ServiceUsage_t &_requestedUsage = {},
        const QString &_action = {}
    );

    virtual void breakCredit(
        INTFAPICALLBOOM_DECL &APICALLBOOM_PARAM,
        const stuAssetItem &_assetItem,
        const QString &_action = {}
    );

    virtual bool isUnlimited(
        INTFAPICALLBOOM_DECL &APICALLBOOM_PARAM,
        const UsageLimits_t &_limits
    ) const;

    virtual bool isEmpty(
        INTFAPICALLBOOM_DECL &APICALLBOOM_PARAM,
        const UsageLimits_t &_limits
    ) const;

public:
    virtual void saveAccountUsage(
        INTFAPICALLBOOM_DECL &APICALLBOOM_PARAM,
        stuActiveCredit &_activeCredit,
        const ServiceUsage_t &_requestedUsage,
        const QString &_action = {}
    );

protected:
    virtual QVariantMap getCustomUserAssetFieldsForQuery(
        INTFAPICALLBOOM_DECL    &APICALLBOOM_PARAM,
        INOUT stuBasketItem     &_basketItem,
        const stuVoucherItem    *_oldVoucherItem = nullptr
    );

public:
    QScopedPointer<intfMTCorrectionRules_Type>         MTCorrectionRules;
    QScopedPointer<intfMTDigestedTranslationLogs_Type> MTDigestedTranslationLogs;
    QScopedPointer<intfMTMultiDic_Type>                MTMultiDic;
    QScopedPointer<intfMTTokenStats_Type>              MTTokenStats;
    QScopedPointer<intfMTTranslatedPhrases_Type>       MTTranslatedPhrases;
    QScopedPointer<intfMTTranslationLogs_Type>         MTTranslationLogs;
};

} //namespace Targoman::API::ModuleHelpers::MT::Interfaces

#endif // TARGOMAN_API_MODULEHELPERS_MT_INTERFACES_INTFMTMODULE_H
