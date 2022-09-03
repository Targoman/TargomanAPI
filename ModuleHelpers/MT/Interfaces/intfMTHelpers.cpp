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

#include "intfMTHelpers.h"

TAPI_REGISTER_TARGOMAN_ENUM(Targoman::API::ModuleHelpers::MT, enuCorrectionRuleType);
TAPI_REGISTER_TARGOMAN_ENUM(Targoman::API::ModuleHelpers::MT, enuMultiDicStatus);

#define TAPI_HELPERORM_SUBMODULE_IMPLEMENT(_className, _tblName) \
    template <TAPI::enuTokenActorType::Type _tokenActorType> \
    _className<_tokenActorType>::_className( \
        const QString& _schema, \
        const QList<DBM::clsORMField>& _exclusiveCols, \
        const QList<DBM::stuRelation>& _exclusiveRelations, \
        const QList<DBM::stuDBIndex>& _exclusiveIndexes \
    ) : \
        intfSQLBasedModule( \
            _schema, \
            _tblName::Name, \
            _tblName::Private::ORMFields + _exclusiveCols, \
            _tblName::Private::Relations(_schema) + _exclusiveRelations, \
            _tblName::Private::Indexes + _exclusiveIndexes \
        ), \
        intfHelperORMBase<_tokenActorType>()

namespace Targoman::API::ModuleHelpers::MT::Interfaces {

/******************************************************/
TAPI_HELPERORM_SUBMODULE_IMPLEMENT(intfCorrectionRules, tblCorrectionRulesBase)
{ ; }

/******************************************************/
TAPI_HELPERORM_SUBMODULE_IMPLEMENT(intfDigestedTranslationLogs, tblDigestedTranslationLogsBase)
{ ; }

/******************************************************/
TAPI_HELPERORM_SUBMODULE_IMPLEMENT(intfMultiDic, tblMultiDicBase)
{ ; }

/******************************************************/
TAPI_HELPERORM_SUBMODULE_IMPLEMENT(intfTokenStats, tblTokenStatsBase)
{ ; }

/******************************************************/
TAPI_HELPERORM_SUBMODULE_IMPLEMENT(intfTranslatedPhrases, tblTranslatedPhrasesBase)
{ ; }

/******************************************************/
TAPI_HELPERORM_SUBMODULE_IMPLEMENT(intfTranslationLogs, tblTranslationLogsBase)
{ ; }

/******************************************************/

template class intfCorrectionRules<TAPI::enuTokenActorType::USER>;
template class intfDigestedTranslationLogs<TAPI::enuTokenActorType::USER>;
template class intfMultiDic<TAPI::enuTokenActorType::USER>;
template class intfTokenStats<TAPI::enuTokenActorType::USER>;
template class intfTranslatedPhrases<TAPI::enuTokenActorType::USER>;
template class intfTranslationLogs<TAPI::enuTokenActorType::USER>;

template class intfCorrectionRules<TAPI::enuTokenActorType::API>;
template class intfDigestedTranslationLogs<TAPI::enuTokenActorType::API>;
template class intfMultiDic<TAPI::enuTokenActorType::API>;
template class intfTokenStats<TAPI::enuTokenActorType::API>;
template class intfTranslatedPhrases<TAPI::enuTokenActorType::API>;
template class intfTranslationLogs<TAPI::enuTokenActorType::API>;

} //namespace Targoman::API::ModuleHelpers::MT::Interfaces
