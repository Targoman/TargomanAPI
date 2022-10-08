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

#ifndef TARGOMAN_API_MODULES_TARGOMANMT_ORM_MTHELPERS_H
#define TARGOMAN_API_MODULES_TARGOMANMT_ORM_MTHELPERS_H

#include "ModuleHelpers/MT/Interfaces/intfMTHelpers.h"
#include "../TargomanMTDefs.hpp"

using namespace Targoman::API::ModuleHelpers::MT::Interfaces;

namespace Targoman::API::TargomanMTModule {

//structures and enumes goes here

//TARGOMAN_DEFINE_ENUM (enu...,

} //namespace Targoman::API::TargomanMTModule

//TAPI_DECLARE_METATYPE_ENUM(Targoman::API::TargomanMTModule, enu...);

namespace Targoman::API::TargomanMTModule::ORM {

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-variable"

namespace tblMTCorrectionRules {

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
        SF_tblMTCorrectionRulesBase_DTO
    );
}

namespace tblMTDigestedTranslationLogs {

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
        SF_tblMTDigestedTranslationLogsBase_DTO
    );
}

namespace tblMTMultiDic {

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
        SF_tblMTMultiDicBase_DTO
    );
}

namespace tblMTTokenStats {

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
        SF_tblMTTokenStatsBase_DTO
    );
}

namespace tblMTTranslatedPhrases {

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
        SF_tblMTTranslatedPhrasesBase_DTO
    );
}

namespace tblMTTranslationLogs {

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
        SF_tblMTTranslationLogsBase_DTO
    );
}

#pragma GCC diagnostic pop

/******************************************************/
class MTCorrectionRules : public intfMTCorrectionRules<TAPI::enuTokenActorType::USER>
{
    Q_OBJECT
    TARGOMAN_API_SUBMODULE_DEFINE(TargomanMT, MTCorrectionRules)
};

/******************************************************/
class MTDigestedTranslationLogs : public intfMTDigestedTranslationLogs<TAPI::enuTokenActorType::USER>
{
    Q_OBJECT
    TARGOMAN_API_SUBMODULE_DEFINE(TargomanMT, MTDigestedTranslationLogs)
};

/******************************************************/
class MTMultiDic : public intfMTMultiDic<TAPI::enuTokenActorType::USER>
{
    Q_OBJECT
    TARGOMAN_API_SUBMODULE_DEFINE(TargomanMT, MTMultiDic)
};

/******************************************************/
class MTTokenStats : public intfMTTokenStats<TAPI::enuTokenActorType::USER>
{
    Q_OBJECT
    TARGOMAN_API_SUBMODULE_DEFINE(TargomanMT, MTTokenStats)
};

/******************************************************/
class MTTranslatedPhrases : public intfMTTranslatedPhrases<TAPI::enuTokenActorType::USER>
{
    Q_OBJECT
    TARGOMAN_API_SUBMODULE_DEFINE(TargomanMT, MTTranslatedPhrases)
};

/******************************************************/
class MTTranslationLogs : public intfMTTranslationLogs<TAPI::enuTokenActorType::USER>
{
    Q_OBJECT
    TARGOMAN_API_SUBMODULE_DEFINE(TargomanMT, MTTranslationLogs)
};

/******************************************************/
} //namespace Targoman::API::TargomanMTModule::ORM

#endif // TARGOMAN_API_MODULES_TARGOMANMT_ORM_MTHELPERS_H
