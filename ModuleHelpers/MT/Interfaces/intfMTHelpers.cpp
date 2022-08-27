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

#define TAPI_ORM_SUBMODULE_IMPLEMENT(_className, _baseName, _tblName) \
    _className::_className( \
        const QString& _schema, \
        const QList<DBM::clsORMField>& _exclusiveCols, \
        const QList<DBM::stuRelation>& _exclusiveRelations, \
        const QList<DBM::stuDBIndex>& _exclusiveIndexes \
    ) : \
        _baseName( \
            _schema, \
            _tblName::Name, \
            _tblName::Private::ORMFields + _exclusiveCols, \
            _tblName::Private::Relations(_schema) + _exclusiveRelations, \
            _tblName::Private::Indexes + _exclusiveIndexes \
    )

namespace Targoman::API::ModuleHelpers::MT::Interfaces {

/******************************************************/
TAPI_ORM_SUBMODULE_IMPLEMENT(intfCorrectionRules, intfSQLBasedModule, tblCorrectionRulesBase)
{ ; }

/******************************************************/
TAPI_ORM_SUBMODULE_IMPLEMENT(intfDigestedTranslationLogs, intfSQLBasedModule, tblDigestedTranslationLogsBase)
{ ; }

/******************************************************/
TAPI_ORM_SUBMODULE_IMPLEMENT(intfMultiDic, intfSQLBasedModule, tblMultiDicBase)
{ ; }

/******************************************************/
TAPI_ORM_SUBMODULE_IMPLEMENT(intfTokenStats, intfSQLBasedModule, tblTokenStatsBase)
{ ; }

/******************************************************/
TAPI_ORM_SUBMODULE_IMPLEMENT(intfTranslatedPhrases, intfSQLBasedModule, tblTranslatedPhrasesBase)
{ ; }

/******************************************************/
TAPI_ORM_SUBMODULE_IMPLEMENT(intfTranslationLogs, intfSQLBasedModule, tblTranslationLogsBase)
{ ; }

/******************************************************/

} //namespace Targoman::API::ModuleHelpers::MT::Interfaces
