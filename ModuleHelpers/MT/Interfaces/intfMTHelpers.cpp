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

namespace Targoman::API::ModuleHelpers::MT::Interfaces {

/******************************************************/
intfCorrectionRules::intfCorrectionRules(
    const QString& _schema,
    const QList<DBM::clsORMField>& _exclusiveCols,
    const QList<DBM::stuRelation>& _exclusiveRelations,
    const QList<DBM::stuDBIndex>& _exclusiveIndexes
) :
    intfSQLBasedModule(
        _schema,
        tblCorrectionRulesBase::Name,
        tblCorrectionRulesBase::Private::ORMFields + _exclusiveCols,
        tblCorrectionRulesBase::Private::Relations(_schema) + _exclusiveRelations,
        tblCorrectionRulesBase::Private::Indexes + _exclusiveIndexes
) { ; }

/******************************************************/
intfDigestedTranslationLogs::intfDigestedTranslationLogs(
    const QString& _schema,
    const QList<DBM::clsORMField>& _exclusiveCols,
    const QList<DBM::stuRelation>& _exclusiveRelations,
    const QList<DBM::stuDBIndex>& _exclusiveIndexes
) :
    intfSQLBasedModule(
        _schema,
        tblDigestedTranslationLogsBase::Name,
        tblDigestedTranslationLogsBase::Private::ORMFields + _exclusiveCols,
        tblDigestedTranslationLogsBase::Private::Relations(_schema) + _exclusiveRelations,
        tblDigestedTranslationLogsBase::Private::Indexes + _exclusiveIndexes
) { ; }

/******************************************************/
intfMultiDic::intfMultiDic(
    const QString& _schema,
    const QList<DBM::clsORMField>& _exclusiveCols,
    const QList<DBM::stuRelation>& _exclusiveRelations,
    const QList<DBM::stuDBIndex>& _exclusiveIndexes
) :
    intfSQLBasedModule(
        _schema,
        tblMultiDicBase::Name,
        tblMultiDicBase::Private::ORMFields + _exclusiveCols,
        tblMultiDicBase::Private::Relations(_schema) + _exclusiveRelations,
        tblMultiDicBase::Private::Indexes + _exclusiveIndexes
) { ; }

/******************************************************/
intfTokenStats::intfTokenStats(
    const QString& _schema,
    const QList<DBM::clsORMField>& _exclusiveCols,
    const QList<DBM::stuRelation>& _exclusiveRelations,
    const QList<DBM::stuDBIndex>& _exclusiveIndexes
) :
    intfSQLBasedModule(
        _schema,
        tblTokenStatsBase::Name,
        tblTokenStatsBase::Private::ORMFields + _exclusiveCols,
        tblTokenStatsBase::Private::Relations(_schema) + _exclusiveRelations,
        tblTokenStatsBase::Private::Indexes + _exclusiveIndexes
) { ; }

/******************************************************/
intfTranslatedPhrases::intfTranslatedPhrases(
    const QString& _schema,
    const QList<DBM::clsORMField>& _exclusiveCols,
    const QList<DBM::stuRelation>& _exclusiveRelations,
    const QList<DBM::stuDBIndex>& _exclusiveIndexes
) :
    intfSQLBasedModule(
        _schema,
        tblTranslatedPhrasesBase::Name,
        tblTranslatedPhrasesBase::Private::ORMFields + _exclusiveCols,
        tblTranslatedPhrasesBase::Private::Relations(_schema) + _exclusiveRelations,
        tblTranslatedPhrasesBase::Private::Indexes + _exclusiveIndexes
) { ; }

/******************************************************/
intfTranslationLogs::intfTranslationLogs(
    const QString& _schema,
    const QList<DBM::clsORMField>& _exclusiveCols,
    const QList<DBM::stuRelation>& _exclusiveRelations,
    const QList<DBM::stuDBIndex>& _exclusiveIndexes
) :
    intfSQLBasedModule(
        _schema,
        tblTranslationLogsBase::Name,
        tblTranslationLogsBase::Private::ORMFields + _exclusiveCols,
        tblTranslationLogsBase::Private::Relations(_schema) + _exclusiveRelations,
        tblTranslationLogsBase::Private::Indexes + _exclusiveIndexes
) { ; }

/******************************************************/

} //namespace Targoman::API::ModuleHelpers::MT::Interfaces
