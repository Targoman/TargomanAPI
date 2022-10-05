/******************************************************************************
#   TargomanAPI: REST API for Targoman
#
#   Copyright 2014-2019 by Targoman Intelligent Processing <http://tip.co.ir>
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

#include "clsDerivedHelperSubmodules.h"

namespace Targoman::API::ModuleHelpers::MT::Classes {

template <TAPI::enuTokenActorType::Type _itmplTokenActorType>
clsDerivedHelperSubmodules<_itmplTokenActorType>::clsDerivedHelperSubmodules(
    intfMTCorrectionRules<_itmplTokenActorType>         *_correctionRules,
    intfMTDigestedTranslationLogs<_itmplTokenActorType> *_digestedTranslationLogs,
    intfMTMultiDic<_itmplTokenActorType>                *_multiDic,
    intfMTTokenStats<_itmplTokenActorType>              *_tokenStats,
    intfMTTranslatedPhrases<_itmplTokenActorType>       *_translatedPhrases,
    intfMTTranslationLogs<_itmplTokenActorType>         *_translationLogs
) :
    MTCorrectionRules         (_correctionRules),
    MTDigestedTranslationLogs (_digestedTranslationLogs),
    MTMultiDic                (_multiDic),
    MTTokenStats              (_tokenStats),
    MTTranslatedPhrases       (_translatedPhrases),
    MTTranslationLogs         (_translationLogs)
{ ; }

template class clsDerivedHelperSubmodules<TAPI::enuTokenActorType::USER>;
template class clsDerivedHelperSubmodules<TAPI::enuTokenActorType::API>;

} //namespace Targoman::API::ModuleHelpers::MT::Classes
