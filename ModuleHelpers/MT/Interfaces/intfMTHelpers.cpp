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
    template <TAPI::enuTokenActorType::Type _itmplTokenActorType> \
    _className<_itmplTokenActorType>::_className( \
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
        intfMTHelperORMBase<_itmplTokenActorType>()

namespace Targoman::API::ModuleHelpers::MT::Interfaces {

/******************************************************/
TAPI_HELPERORM_SUBMODULE_IMPLEMENT(intfMTCorrectionRules, tblMTCorrectionRulesBase)
{ ; }

/******************************************************/
TAPI_HELPERORM_SUBMODULE_IMPLEMENT(intfMTDigestedTranslationLogs, tblMTDigestedTranslationLogsBase)
{ ; }

/******************************************************/
TAPI_HELPERORM_SUBMODULE_IMPLEMENT(intfMTMultiDic, tblMTMultiDicBase)
{ ; }

/******************************************************/
TAPI_HELPERORM_SUBMODULE_IMPLEMENT(intfMTTokenStats, tblMTTokenStatsBase)
{ ; }

/******************************************************/
TAPI_HELPERORM_SUBMODULE_IMPLEMENT(intfMTTranslatedPhrases, tblMTTranslatedPhrasesBase)
{ ; }

/******************************************************/
TAPI_HELPERORM_SUBMODULE_IMPLEMENT(intfMTTranslationLogs, tblMTTranslationLogsBase)
{ ; }

/******************************************************/

template class intfMTCorrectionRules<TAPI::enuTokenActorType::USER>;
template class intfMTDigestedTranslationLogs<TAPI::enuTokenActorType::USER>;
template class intfMTMultiDic<TAPI::enuTokenActorType::USER>;
template class intfMTTokenStats<TAPI::enuTokenActorType::USER>;
template class intfMTTranslatedPhrases<TAPI::enuTokenActorType::USER>;
template class intfMTTranslationLogs<TAPI::enuTokenActorType::USER>;

template class intfMTCorrectionRules<TAPI::enuTokenActorType::API>;
template class intfMTDigestedTranslationLogs<TAPI::enuTokenActorType::API>;
template class intfMTMultiDic<TAPI::enuTokenActorType::API>;
template class intfMTTokenStats<TAPI::enuTokenActorType::API>;
template class intfMTTranslatedPhrases<TAPI::enuTokenActorType::API>;
template class intfMTTranslationLogs<TAPI::enuTokenActorType::API>;

} //namespace Targoman::API::ModuleHelpers::MT::Interfaces
