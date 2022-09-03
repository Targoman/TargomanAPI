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
#include "../MT.h"

//TAPI_REGISTER_TARGOMAN_ENUM(Targoman::API::MTModule, enu...);

//using namespace Targoman::API::AAA;

namespace Targoman::API::MTModule::ORM {

/******************************************************/
TARGOMAN_API_SUBMODULE_IMPLEMENT(MT, CorrectionRules)

CorrectionRules::CorrectionRules() :
    intfCorrectionRules(
        MTSchema,
        tblCorrectionRules::Private::ExtraORMFields,
        tblCorrectionRules::Private::ExtraRelations,
        tblCorrectionRules::Private::ExtraIndexes
) { ; }

/******************************************************/
TARGOMAN_API_SUBMODULE_IMPLEMENT(MT, DigestedTranslationLogs)

DigestedTranslationLogs::DigestedTranslationLogs() :
    intfDigestedTranslationLogs(
        MTSchema,
        tblDigestedTranslationLogs::Private::ExtraORMFields,
        tblDigestedTranslationLogs::Private::ExtraRelations,
        tblDigestedTranslationLogs::Private::ExtraIndexes
) { ; }

/******************************************************/
TARGOMAN_API_SUBMODULE_IMPLEMENT(MT, MultiDic)

MultiDic::MultiDic() :
    intfMultiDic(
        MTSchema,
        tblMultiDic::Private::ExtraORMFields,
        tblMultiDic::Private::ExtraRelations,
        tblMultiDic::Private::ExtraIndexes
) { ; }

/******************************************************/
TARGOMAN_API_SUBMODULE_IMPLEMENT(MT, TokenStats)

TokenStats::TokenStats() :
    intfTokenStats(
        MTSchema,
        tblTokenStats::Private::ExtraORMFields,
        tblTokenStats::Private::ExtraRelations,
        tblTokenStats::Private::ExtraIndexes
) { ; }

/******************************************************/
TARGOMAN_API_SUBMODULE_IMPLEMENT(MT, TranslatedPhrases)

TranslatedPhrases::TranslatedPhrases() :
    intfTranslatedPhrases(
        MTSchema,
        tblTranslatedPhrases::Private::ExtraORMFields,
        tblTranslatedPhrases::Private::ExtraRelations,
        tblTranslatedPhrases::Private::ExtraIndexes
) { ; }

/******************************************************/
TARGOMAN_API_SUBMODULE_IMPLEMENT(MT, TranslationLogs)

TranslationLogs::TranslationLogs() :
    intfTranslationLogs(
        MTSchema,
        tblTranslationLogs::Private::ExtraORMFields,
        tblTranslationLogs::Private::ExtraRelations,
        tblTranslationLogs::Private::ExtraIndexes
) { ; }

/******************************************************/
} // namespace Targoman::API::MTModule::ORM
