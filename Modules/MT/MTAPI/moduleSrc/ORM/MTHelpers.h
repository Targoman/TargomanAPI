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

#ifndef TARGOMAN_API_MODULES_MTAPI_ORM_MTHELPERS_H
#define TARGOMAN_API_MODULES_MTAPI_ORM_MTHELPERS_H

#include "ModuleHelpers/MT/Interfaces/intfMTHelpers.h"
#include "../MTAPIDefs.hpp"

using namespace Targoman::API::ModuleHelpers::MT::Interfaces;

namespace Targoman::API::MTAPIModule {

//structures and enumes goes here

//TARGOMAN_DEFINE_ENUM (enu...,

} //namespace Targoman::API::MTAPIModule

//TAPI_DECLARE_METATYPE_ENUM(Targoman::API::MTAPIModule, enu...);

namespace Targoman::API::MTAPIModule::ORM {

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-variable"

namespace tblCorrectionRules {

    namespace ExtraFields {
    }

    namespace ExtraRelation {
    }

    namespace Private {
        const QList<clsORMField> ExtraORMFields = {
            ///ColName                                    Type        Validation                         Default    UpBy       Sort  Filter Self  Virt   PK
        };

        const QList<stuRelation> ExtraRelations = {
            ///Col                              Reference Table                      ForeignCol             Rename     LeftJoin
        };

        const QList<stuDBIndex> ExtraIndexes = {
        };

    } //namespace Private

    TAPI_DEFINE_STRUCT(DTO,
        SF_tblCorrectionRulesBase_DTO
    );
}

namespace tblDigestedTranslationLogs {

    namespace ExtraFields {
    }

    namespace ExtraRelation {
    }

    namespace Private {
        const QList<clsORMField> ExtraORMFields = {
            ///ColName                                    Type        Validation                         Default    UpBy       Sort  Filter Self  Virt   PK
        };

        const QList<stuRelation> ExtraRelations = {
            ///Col                              Reference Table                      ForeignCol             Rename     LeftJoin
        };

        const QList<stuDBIndex> ExtraIndexes = {
        };

    } //namespace Private

    TAPI_DEFINE_STRUCT(DTO,
        SF_tblDigestedTranslationLogsBase_DTO
    );
}

namespace tblMultiDic {

    namespace ExtraFields {
    }

    namespace ExtraRelation {
    }

    namespace Private {
        const QList<clsORMField> ExtraORMFields = {
            ///ColName                                     Type        Validation                  Default    UpBy       Sort  Filter Self  Virt   PK
        };

        const QList<stuRelation> ExtraRelations = {
        };

        const QList<stuDBIndex> ExtraIndexes = {
        };

    } //namespace Private

    TAPI_DEFINE_STRUCT(DTO,
        SF_tblMultiDicBase_DTO
    );
}

namespace tblTokenStats {

    namespace ExtraFields {
    }

    namespace ExtraRelation {
    }

    namespace Private {
        const QList<clsORMField> ExtraORMFields = {
            //ColName                           Type        Validation                  Default     UpBy    Sort    Filter Self  Virt   PK
        };

        const QList<stuRelation> ExtraRelations = {
        };

        const QList<stuDBIndex> ExtraIndexes = {
        };

    } //namespace Private

    TAPI_DEFINE_STRUCT(DTO,
        SF_tblTokenStatsBase_DTO
    );
}

namespace tblTranslatedPhrases {

    namespace ExtraFields {
        //usgRemainingDays
    }

    namespace ExtraRelation {
    }

    namespace Private {
        const QList<clsORMField> ExtraORMFields = {
            //ColName                           Type        Validation                  Default     UpBy    Sort    Filter Self  Virt   PK
        };

        const QList<stuRelation> ExtraRelations = {
        };

        const QList<stuDBIndex> ExtraIndexes = {
        };

    } //namespace Private

    TAPI_DEFINE_STRUCT(DTO,
        SF_tblTranslatedPhrasesBase_DTO
    );
}

namespace tblTranslationLogs {

    namespace ExtraFields {
    }

    namespace ExtraRelation {
    }

    namespace Private {
        const QList<clsORMField> ExtraORMFields = {
            ///ColName                  Type        Validation                  Default    UpBy       Sort  Filter Self  Virt   PK
        };

        const QList<stuRelation> ExtraRelations = {
        };

        const QList<stuDBIndex> ExtraIndexes = {
        };

    } //namespace Private

    TAPI_DEFINE_STRUCT(DTO,
        SF_tblTranslationLogsBase_DTO
    );
}

#pragma GCC diagnostic pop

/******************************************************/
class CorrectionRules : public intfCorrectionRules
{
    Q_OBJECT
    TARGOMAN_API_SUBMODULE_DEFINE(MTAPI, CorrectionRules)
};

/******************************************************/
class DigestedTranslationLogs : public intfDigestedTranslationLogs
{
    Q_OBJECT
    TARGOMAN_API_SUBMODULE_DEFINE(MTAPI, DigestedTranslationLogs)
};

/******************************************************/
class MultiDic : public intfMultiDic
{
    Q_OBJECT
    TARGOMAN_API_SUBMODULE_DEFINE(MTAPI, MultiDic)
};

/******************************************************/
class TokenStats : public intfTokenStats
{
    Q_OBJECT
    TARGOMAN_API_SUBMODULE_DEFINE(MTAPI, TokenStats)
};

/******************************************************/
class TranslatedPhrases : public intfTranslatedPhrases
{
    Q_OBJECT
    TARGOMAN_API_SUBMODULE_DEFINE(MTAPI, TranslatedPhrases)
};

/******************************************************/
class TranslationLogs : public intfTranslationLogs
{
    Q_OBJECT
    TARGOMAN_API_SUBMODULE_DEFINE(MTAPI, TranslationLogs)
};

/******************************************************/
} //namespace Targoman::API::MTAPIModule::ORM

#endif // TARGOMAN_API_MODULES_MTAPI_ORM_MTHELPERS_H
