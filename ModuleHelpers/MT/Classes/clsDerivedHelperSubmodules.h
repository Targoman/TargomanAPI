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

template <TAPI::enuTokenActorType::Type _tokenActorType>
class clsDerivedHelperSubmodules
{
public:
    clsDerivedHelperSubmodules(
        intfCorrectionRules<_tokenActorType>         *_correctionRules,
        intfDigestedTranslationLogs<_tokenActorType> *_digestedTranslationLogs,
        intfMultiDic<_tokenActorType>                *_multiDic,
        intfTokenStats<_tokenActorType>              *_tokenStats,
        intfTranslatedPhrases<_tokenActorType>       *_translatedPhrases,
        intfTranslationLogs<_tokenActorType>         *_translationLogs
    );

public:
    QScopedPointer<intfCorrectionRules<_tokenActorType>>         CorrectionRules;
    QScopedPointer<intfDigestedTranslationLogs<_tokenActorType>> DigestedTranslationLogs;
    QScopedPointer<intfMultiDic<_tokenActorType>>                MultiDic;
    QScopedPointer<intfTokenStats<_tokenActorType>>              TokenStats;
    QScopedPointer<intfTranslatedPhrases<_tokenActorType>>       TranslatedPhrases;
    QScopedPointer<intfTranslationLogs<_tokenActorType>>         TranslationLogs;
};

} //namespace Targoman::API::ModuleHelpers::MT::Classes

#endif // TARGOMAN_API_MODULEHELPERS_MT_CLASSES_CLSDERIVEDHELPERSUBMODULES_H
