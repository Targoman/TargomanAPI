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

clsDerivedHelperSubmodules::clsDerivedHelperSubmodules(
    intfCorrectionRules         *_correctionRules,
    intfDigestedTranslationLogs *_digestedTranslationLogs,
    intfMultiDic                *_multiDic,
    intfTokenStats              *_tokenStats,
    intfTranslatedPhrases       *_translatedPhrases,
    intfTranslationLogs         *_translationLogs
) :
    CorrectionRules         (_correctionRules),
    DigestedTranslationLogs (_digestedTranslationLogs),
    MultiDic                (_multiDic),
    TokenStats              (_tokenStats),
    TranslatedPhrases       (_translatedPhrases),
    TranslationLogs         (_translationLogs)
{ ; }

} //namespace Targoman::API::ModuleHelpers::MT::Classes
