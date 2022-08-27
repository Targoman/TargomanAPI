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

#include "MTHelpers.h"
#include "../MTAPI.h"

//TAPI_REGISTER_TARGOMAN_ENUM(Targoman::API::MTAPIModule, enu...);

//using namespace Targoman::API::AAA;

namespace Targoman::API::MTAPIModule::ORM {

/******************************************************/
TARGOMAN_API_SUBMODULE_IMPLEMENT(MTAPI, CorrectionRules)

CorrectionRules::CorrectionRules() :
    intfCorrectionRules(
        MTAPISchema,
        tblCorrectionRules::Private::ExtraORMFields,
        tblCorrectionRules::Private::ExtraRelations,
        tblCorrectionRules::Private::ExtraIndexes
) { ; }

/******************************************************/
TARGOMAN_API_SUBMODULE_IMPLEMENT(MTAPI, DigestedTranslationLogs)

DigestedTranslationLogs::DigestedTranslationLogs() :
    intfDigestedTranslationLogs(
        MTAPISchema,
        tblDigestedTranslationLogs::Private::ExtraORMFields,
        tblDigestedTranslationLogs::Private::ExtraRelations,
        tblDigestedTranslationLogs::Private::ExtraIndexes
) { ; }

/******************************************************/
TARGOMAN_API_SUBMODULE_IMPLEMENT(MTAPI, MultiDic)

MultiDic::MultiDic() :
    intfMultiDic(
        MTAPISchema,
        tblMultiDic::Private::ExtraORMFields,
        tblMultiDic::Private::ExtraRelations,
        tblMultiDic::Private::ExtraIndexes
) { ; }

/******************************************************/
TARGOMAN_API_SUBMODULE_IMPLEMENT(MTAPI, TokenStats)

TokenStats::TokenStats() :
    intfTokenStats(
        MTAPISchema,
        tblTokenStats::Private::ExtraORMFields,
        tblTokenStats::Private::ExtraRelations,
        tblTokenStats::Private::ExtraIndexes
) { ; }

/******************************************************/
TARGOMAN_API_SUBMODULE_IMPLEMENT(MTAPI, TranslatedPhrases)

TranslatedPhrases::TranslatedPhrases() :
    intfTranslatedPhrases(
        MTAPISchema,
        tblTranslatedPhrases::Private::ExtraORMFields,
        tblTranslatedPhrases::Private::ExtraRelations,
        tblTranslatedPhrases::Private::ExtraIndexes
) { ; }

/******************************************************/
TARGOMAN_API_SUBMODULE_IMPLEMENT(MTAPI, TranslationLogs)

TranslationLogs::TranslationLogs() :
    intfTranslationLogs(
        MTAPISchema,
        tblTranslationLogs::Private::ExtraORMFields,
        tblTranslationLogs::Private::ExtraRelations,
        tblTranslationLogs::Private::ExtraIndexes
) { ; }

/******************************************************/
} // namespace Targoman::API::MTAPIModule::ORM
