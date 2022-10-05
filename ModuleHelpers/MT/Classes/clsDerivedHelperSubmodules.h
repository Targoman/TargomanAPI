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

#ifndef TARGOMAN_API_MODULEHELPERS_MT_CLASSES_CLSDERIVEDHELPERSUBMODULES_H
#define TARGOMAN_API_MODULEHELPERS_MT_CLASSES_CLSDERIVEDHELPERSUBMODULES_H

#include "../Interfaces/intfMTHelpers.h"

namespace Targoman::API::ModuleHelpers::MT::Classes {

using namespace Interfaces;

template <TAPI::enuTokenActorType::Type _itmplTokenActorType>
class clsDerivedHelperSubmodules
{
public:
    clsDerivedHelperSubmodules(
        intfMTCorrectionRules<_itmplTokenActorType>         *_correctionRules,
        intfMTDigestedTranslationLogs<_itmplTokenActorType> *_digestedTranslationLogs,
        intfMTMultiDic<_itmplTokenActorType>                *_multiDic,
        intfMTTokenStats<_itmplTokenActorType>              *_tokenStats,
        intfMTTranslatedPhrases<_itmplTokenActorType>       *_translatedPhrases,
        intfMTTranslationLogs<_itmplTokenActorType>         *_translationLogs
    );

public:
    QScopedPointer<intfMTCorrectionRules<_itmplTokenActorType>>         MTCorrectionRules;
    QScopedPointer<intfMTDigestedTranslationLogs<_itmplTokenActorType>> MTDigestedTranslationLogs;
    QScopedPointer<intfMTMultiDic<_itmplTokenActorType>>                MTMultiDic;
    QScopedPointer<intfMTTokenStats<_itmplTokenActorType>>              MTTokenStats;
    QScopedPointer<intfMTTranslatedPhrases<_itmplTokenActorType>>       MTTranslatedPhrases;
    QScopedPointer<intfMTTranslationLogs<_itmplTokenActorType>>         MTTranslationLogs;
};

} //namespace Targoman::API::ModuleHelpers::MT::Classes

#endif // TARGOMAN_API_MODULEHELPERS_MT_CLASSES_CLSDERIVEDHELPERSUBMODULES_H
