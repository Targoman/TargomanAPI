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

template class intfMTModule<false>;
template class intfMTModule<true>;

} //namespace Targoman::API::ModuleHelpers::MT::Interfaces

