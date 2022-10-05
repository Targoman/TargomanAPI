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
TARGOMAN_API_SUBMODULE_IMPLEMENT(MT, MTCorrectionRules)

MTCorrectionRules::MTCorrectionRules() :
    intfMTCorrectionRules(
        MTSchema,
        tblMTCorrectionRules::Private::ExtraORMFields,
        tblMTCorrectionRules::Private::ExtraRelations,
        tblMTCorrectionRules::Private::ExtraIndexes
) { ; }

/******************************************************/
TARGOMAN_API_SUBMODULE_IMPLEMENT(MT, MTDigestedTranslationLogs)

MTDigestedTranslationLogs::MTDigestedTranslationLogs() :
    intfMTDigestedTranslationLogs(
        MTSchema,
        tblMTDigestedTranslationLogs::Private::ExtraORMFields,
        tblMTDigestedTranslationLogs::Private::ExtraRelations,
        tblMTDigestedTranslationLogs::Private::ExtraIndexes
) { ; }

/******************************************************/
TARGOMAN_API_SUBMODULE_IMPLEMENT(MT, MTMultiDic)

MTMultiDic::MTMultiDic() :
    intfMTMultiDic(
        MTSchema,
        tblMTMultiDic::Private::ExtraORMFields,
        tblMTMultiDic::Private::ExtraRelations,
        tblMTMultiDic::Private::ExtraIndexes
) { ; }

/******************************************************/
TARGOMAN_API_SUBMODULE_IMPLEMENT(MT, MTTokenStats)

MTTokenStats::MTTokenStats() :
    intfMTTokenStats(
        MTSchema,
        tblMTTokenStats::Private::ExtraORMFields,
        tblMTTokenStats::Private::ExtraRelations,
        tblMTTokenStats::Private::ExtraIndexes
) { ; }

/******************************************************/
TARGOMAN_API_SUBMODULE_IMPLEMENT(MT, MTTranslatedPhrases)

MTTranslatedPhrases::MTTranslatedPhrases() :
    intfMTTranslatedPhrases(
        MTSchema,
        tblMTTranslatedPhrases::Private::ExtraORMFields,
        tblMTTranslatedPhrases::Private::ExtraRelations,
        tblMTTranslatedPhrases::Private::ExtraIndexes
) { ; }

/******************************************************/
TARGOMAN_API_SUBMODULE_IMPLEMENT(MT, MTTranslationLogs)

MTTranslationLogs::MTTranslationLogs() :
    intfMTTranslationLogs(
        MTSchema,
        tblMTTranslationLogs::Private::ExtraORMFields,
        tblMTTranslationLogs::Private::ExtraRelations,
        tblMTTranslationLogs::Private::ExtraIndexes
) { ; }

/******************************************************/
} // namespace Targoman::API::MTModule::ORM
