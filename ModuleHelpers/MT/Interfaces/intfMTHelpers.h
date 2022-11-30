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

#ifndef TARGOMAN_API_MODULEHELPERS_MT_INTERFACES_INTFMTHELPERS_H
#define TARGOMAN_API_MODULEHELPERS_MT_INTERFACES_INTFMTHELPERS_H

#include "Interfaces/Common/APIArgHelperMacros.hpp"
#include "Interfaces/Common/GenericTypes.h"
#include "Interfaces/Common/GenericEnums.hpp"
#include "Interfaces/Common/QtTypes.hpp"
#include "Interfaces/API/intfSQLBasedModule.h"
using namespace Targoman::API::API;

namespace Targoman::API::ModuleHelpers::MT {

TARGOMAN_DEFINE_ENUM(enuCorrectionRuleType,
                     Replace        = 'R',
                     Translation    = 'T'
                     );

TARGOMAN_DEFINE_ENUM(enuMultiDicStatus,
                     Active     = 'A',
                     Removed    = 'R'
                     );

} //namespace Targoman::API::ModuleHelpers::MT

TAPI_DECLARE_METATYPE_ENUM(Targoman::API::ModuleHelpers::MT, enuCorrectionRuleType);
TAPI_DECLARE_METATYPE_ENUM(Targoman::API::ModuleHelpers::MT, enuMultiDicStatus);

namespace Targoman::API::ModuleHelpers::MT::Interfaces {

/******************************************************/
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-variable"

namespace tblMTCorrectionRulesBase {
    constexpr char Name[] = "tblMTCorrectionRules";

    namespace Fields {
        TARGOMAN_CREATE_CONSTEXPR(crlID);
        TARGOMAN_CREATE_CONSTEXPR(crlType);
        TARGOMAN_CREATE_CONSTEXPR(crlPattern);
        TARGOMAN_CREATE_CONSTEXPR(crlReplacement);
    } //namespace Fields
}

namespace tblMTDigestedTranslationLogsBase {
    constexpr char Name[] = "tblMTDigestedTranslationLogs";

    namespace Fields {
        TARGOMAN_CREATE_CONSTEXPR(dtlDateTime);
        TARGOMAN_CREATE_CONSTEXPR(dtl_aptID);
        TARGOMAN_CREATE_CONSTEXPR(dtl_actorID);
        TARGOMAN_CREATE_CONSTEXPR(dtlEngine);
        TARGOMAN_CREATE_CONSTEXPR(dtlDir);
        TARGOMAN_CREATE_CONSTEXPR(dtlTotalWordsRequested);
        TARGOMAN_CREATE_CONSTEXPR(dtlTotalTranslatedWords);
        TARGOMAN_CREATE_CONSTEXPR(dtlAverageTranslationTime);
        TARGOMAN_CREATE_CONSTEXPR(dtlMaxSpeed);
        TARGOMAN_CREATE_CONSTEXPR(dtlAverageSourceWordCount);
        TARGOMAN_CREATE_CONSTEXPR(dtlMaxSourceWordCount);
        TARGOMAN_CREATE_CONSTEXPR(dtlMinSourceWordCount);
        TARGOMAN_CREATE_CONSTEXPR(dtlOkCount);
        TARGOMAN_CREATE_CONSTEXPR(dtlErrLongCount);
        TARGOMAN_CREATE_CONSTEXPR(dtlErrNoResourceCount);
        TARGOMAN_CREATE_CONSTEXPR(dtlErrOtherCount);
    } //namespace Fields
}

namespace tblMTMultiDicBase {
    constexpr char Name[] = "tblMTMultiDic";

    namespace Fields {
        TARGOMAN_CREATE_CONSTEXPR(dicID);
        TARGOMAN_CREATE_CONSTEXPR(dicSourceLang);
        TARGOMAN_CREATE_CONSTEXPR(dicWord);
        TARGOMAN_CREATE_CONSTEXPR(dicTranslation);
        TARGOMAN_CREATE_CONSTEXPR(dicStatus);
        TARGOMAN_CREATE_CONSTEXPR(_dicVersion);
        TARGOMAN_CREATE_CONSTEXPR(dicCreationDateTime);
        TARGOMAN_CREATE_CONSTEXPR(dicCreatedBy_actorID);
        TARGOMAN_CREATE_CONSTEXPR(dicUpdatedBy_actorID);
    }
}

namespace tblMTTokenStatsBase {
    constexpr char Name[] = "tblMTTokenStats";

    namespace Fields {
        TARGOMAN_CREATE_CONSTEXPR(tks_tokID);
        TARGOMAN_CREATE_CONSTEXPR(tksEngine);
        TARGOMAN_CREATE_CONSTEXPR(tksDir);
        TARGOMAN_CREATE_CONSTEXPR(tksTodayWords);
        TARGOMAN_CREATE_CONSTEXPR(tksThisMonthWords);
        TARGOMAN_CREATE_CONSTEXPR(tksTotalWords);
    }
}

namespace tblMTTranslatedPhrasesBase {
    constexpr char Name[] = "tblMTTranslatedPhrases";

    namespace Fields {
        TARGOMAN_CREATE_CONSTEXPR(tps_tlsID);
        TARGOMAN_CREATE_CONSTEXPR(tpsSourceString);
        TARGOMAN_CREATE_CONSTEXPR(tpsTargetString);
    }
}

namespace tblMTTranslationLogsBase {
    constexpr char Name[] = "tblMTTranslationLogs";

    namespace Fields {
        TARGOMAN_CREATE_CONSTEXPR(tlsID);
        TARGOMAN_CREATE_CONSTEXPR(tls_actorID);
        TARGOMAN_CREATE_CONSTEXPR(tls_aptID);
        TARGOMAN_CREATE_CONSTEXPR(tlsRequestTime);
        TARGOMAN_CREATE_CONSTEXPR(tlsDir);
        TARGOMAN_CREATE_CONSTEXPR(tlsEngine);
        TARGOMAN_CREATE_CONSTEXPR(tlsClass);
        TARGOMAN_CREATE_CONSTEXPR(tlsServer);
        TARGOMAN_CREATE_CONSTEXPR(tlsSourceWordsCount);
        TARGOMAN_CREATE_CONSTEXPR(tlsTranslationTime);
        TARGOMAN_CREATE_CONSTEXPR(tlsErrNo);
    }
}

//-- tbl inside
namespace tblMTCorrectionRulesBase {
    namespace Relation {
    }

    namespace Private {
        const QList<clsORMField> ORMFields = {
            //ColName                   Type                Validation          Default     UpBy    Sort    Filter  Self    Virt    PK
            { Fields::crlID,            ORM_PRIMARYKEY_32 },
            { Fields::crlType,          S(Targoman::API::ModuleHelpers::MT::enuCorrectionRuleType::Type), QFV, Targoman::API::ModuleHelpers::MT::enuCorrectionRuleType::Replace, UPAdmin },
            { Fields::crlPattern,       S(QString),         QFV.maxLenght(250), QRequired,  UPAdmin },
            { Fields::crlReplacement,   S(QString),         QFV.maxLenght(500), QRequired,  UPAdmin },
        };

        inline const QList<stuRelation> Relations(Q_DECL_UNUSED const QString& _schema) {
            return {};
        };

        const QList<stuDBIndex> Indexes = {
        };

    } //namespace Private

#define SF_tblMTCorrectionRulesBase_DTO \
    SF_ORM_PRIMARYKEY_32        (crlID), \
    SF_Enum                     (crlType, Targoman::API::ModuleHelpers::MT::enuCorrectionRuleType, Targoman::API::ModuleHelpers::MT::enuCorrectionRuleType::Replace), \
    SF_QString                  (crlPattern), \
    SF_QString                  (crlReplacement)

    TAPI_DEFINE_STRUCT(DTO,
        SF_tblMTCorrectionRulesBase_DTO
    );
}

namespace tblMTDigestedTranslationLogsBase {
    namespace Relation {
    }

    namespace Private {
        const QList<clsORMField> ORMFields = {
            //ColName                               Type                Validation          Default     UpBy    Sort    Filter  Self    Virt    PK
            { Fields::dtlDateTime,                  S(TAPI::Date_t),    QFV,                QRequired,  UPNone },
            { Fields::dtl_aptID,                    S(quint64),         QFV,                QRequired,  UPNone },
            { Fields::dtl_actorID,                  S(quint64),         QFV,                QRequired,  UPNone },
            { Fields::dtlEngine,                    S(QString),         QFV.maxLenght(3),   QRequired,  UPNone },
            { Fields::dtlDir,                       S(QString),         QFV.maxLenght(5),   QRequired,  UPNone },
            { Fields::dtlTotalWordsRequested,       S(quint64),         QFV,                QRequired,  UPOwner },
            { Fields::dtlTotalTranslatedWords,      S(quint64),         QFV,                QRequired,  UPOwner },
            { Fields::dtlAverageTranslationTime,    S(double),          QFV,                QRequired,  UPOwner },
            { Fields::dtlMaxSpeed,                  S(double),          QFV,                QRequired,  UPOwner },
            { Fields::dtlAverageSourceWordCount,    S(double),          QFV,                QRequired,  UPOwner },
            { Fields::dtlMaxSourceWordCount,        S(quint32),         QFV,                QRequired,  UPOwner },
            { Fields::dtlMinSourceWordCount,        S(quint32),         QFV,                QRequired,  UPOwner },
            { Fields::dtlOkCount,                   S(quint64),         QFV,                QRequired,  UPOwner },
            { Fields::dtlErrLongCount,              S(quint64),         QFV,                QRequired,  UPOwner },
            { Fields::dtlErrNoResourceCount,        S(quint64),         QFV,                QRequired,  UPOwner },
            { Fields::dtlErrOtherCount,             S(quint64),         QFV,                QRequired,  UPOwner },
        };

        inline const QList<stuRelation> Relations(Q_DECL_UNUSED const QString& _schema) {
            return {};
        };

        const QList<stuDBIndex> Indexes = {
            { {
                Fields::dtlDateTime,
                Fields::dtl_aptID,
                Fields::dtl_actorID,
                Fields::dtlEngine,
                Fields::dtlDir,
              }, enuDBIndex::Primary },
        };

    } //namespace Private

#define SF_tblMTDigestedTranslationLogsBase_DTO \
    SF_Date_t                   (dtlDateTime), \
    SF_quint64                  (dtl_aptID), \
    SF_quint64                  (dtl_actorID), \
    SF_QString                  (dtlEngine), \
    SF_QString                  (dtlDir), \
    SF_quint64                  (dtlTotalWordsRequested), \
    SF_quint64                  (dtlTotalTranslatedWords), \
    SF_double                   (dtlAverageTranslationTime), \
    SF_double                   (dtlMaxSpeed), \
    SF_double                   (dtlAverageSourceWordCount), \
    SF_quint32                  (dtlMaxSourceWordCount), \
    SF_quint32                  (dtlMinSourceWordCount), \
    SF_quint64                  (dtlOkCount), \
    SF_quint64                  (dtlErrLongCount), \
    SF_quint64                  (dtlErrNoResourceCount), \
    SF_quint64                  (dtlErrOtherCount)

    TAPI_DEFINE_STRUCT(DTO,
        SF_tblMTDigestedTranslationLogsBase_DTO
    );
}

namespace tblMTMultiDicBase {
    namespace Relation {
    }

    namespace Private {
        const QList<clsORMField> ORMFields = {
            //ColName                       Type                Validation                  Default     UpBy    Sort    Filter  Self    Virt    PK
            { Fields::dicID,                ORM_PRIMARYKEY_32 },
            { Fields::dicSourceLang,        S(QString),         QFV.maxLenght(2),           QRequired,  UPOwner },
            { Fields::dicWord,              S(QString),         QFV.maxLenght(100),         QRequired,  UPOwner },
            { Fields::dicTranslation,       S(TAPI::JSON_t),    QFV,                        QNull,      UPOwner },
            { Fields::dicStatus,            ORM_STATUS_FIELD(Targoman::API::ModuleHelpers::MT::enuMultiDicStatus, Targoman::API::ModuleHelpers::MT::enuMultiDicStatus::Active) },
            { Fields::_dicVersion,          S(quint32),         QFV.integer().minValue(1),  0,          UPOwner },
            { Fields::dicCreationDateTime,  ORM_CREATED_ON },
            { Fields::dicCreatedBy_actorID, ORM_CREATED_BY },
            { Fields::dicUpdatedBy_actorID, ORM_UPDATED_BY },
        };

        inline const QList<stuRelation> Relations(Q_DECL_UNUSED const QString& _schema) {
            return {};
        };

        const QList<stuDBIndex> Indexes = {
        };

    } //namespace Private

#define SF_tblMTMultiDicBase_DTO \
    SF_ORM_PRIMARYKEY_32        (dicID), \
    SF_QString                  (dicSourceLang), \
    SF_QString                  (dicWord), \
    SF_JSON_t                   (dicTranslation), \
    SF_ORM_STATUS_FIELD         (dicStatus, Targoman::API::ModuleHelpers::MT::enuMultiDicStatus, Targoman::API::ModuleHelpers::MT::enuMultiDicStatus::Active), \
    SF_quint32                  (_dicVersion), \
    SF_ORM_CREATED_ON           (dicCreationDateTime), \
    SF_ORM_CREATED_BY           (dicCreatedBy_actorID), \
    SF_ORM_UPDATED_BY           (dicUpdatedBy_actorID)

    TAPI_DEFINE_STRUCT(DTO,
        SF_tblMTMultiDicBase_DTO
    );
}

namespace tblMTTokenStatsBase {
    namespace Relation {
    }

    namespace Private {
        const QList<clsORMField> ORMFields = {
            //ColName                       Type        Validation          Default     UpBy    Sort    Filter  Self    Virt    PK
            { Fields::tks_tokID,            S(quint64), QFV,                QRequired,  UPNone },
            { Fields::tksEngine,            S(QString), QFV.maxLenght(50),  QRequired,  UPNone },
            { Fields::tksDir,               S(QString), QFV.maxLenght(50),  QRequired,  UPNone },
            { Fields::tksTodayWords,        S(quint32), QFV,                0,          UPOwner },
            { Fields::tksThisMonthWords,    S(quint32), QFV,                0,          UPOwner },
            { Fields::tksTotalWords,        S(quint64), QFV,                0,          UPOwner },
        };

        inline const QList<stuRelation> Relations(Q_DECL_UNUSED const QString& _schema) {
            return {};
        };

        const QList<stuDBIndex> Indexes = {
            { {
                  Fields::tks_tokID,
                  Fields::tksEngine,
                  Fields::tksDir,
              }, enuDBIndex::Primary },
        };

    } //namespace Private

#define SF_tblMTTokenStatsBase_DTO \
    SF_quint64                  (tks_tokID), \
    SF_QString                  (tksEngine), \
    SF_QString                  (tksDir), \
    SF_quint32                  (tksTodayWords), \
    SF_quint32                  (tksThisMonthWords), \
    SF_quint64                  (tksTotalWords)

    TAPI_DEFINE_STRUCT(DTO,
        SF_tblMTTokenStatsBase_DTO
    );
}

namespace tblMTTranslatedPhrasesBase {
    namespace Relation {
    }

    namespace Private {
        const QList<clsORMField> ORMFields = {
            //ColName                   Type                    Validation  Default     UpBy    Sort    Filter  Self    Virt    PK
            { Fields::tps_tlsID,        S(quint64),             QFV,        QRequired,  UPNone },
            { Fields::tpsSourceString,  S(TAPI::DBLongText_t),  QFV,        QRequired,  UPOwner },
            { Fields::tpsTargetString,  S(TAPI::DBLongText_t),  QFV,        QNull,      UPOwner },
        };

        inline const QList<stuRelation> Relations(Q_DECL_UNUSED const QString& _schema) {
            return {
                //Col                   Reference Table                             ForeignCol                              Rename    LeftJoin
                { Fields::tps_tlsID,    R(_schema, tblMTTranslationLogsBase::Name),   tblMTTranslationLogsBase::Fields::tlsID },
            };
        };

        const QList<stuDBIndex> Indexes = {
            { Fields::tps_tlsID, enuDBIndex::Primary },
        };

    } //namespace Private

#define SF_tblMTTranslatedPhrasesBase_DTO \
    SF_quint64                  (tps_tlsID), \
    SF_QString                  (tpsSourceString), \
    SF_QString                  (tpsTargetString)

    TAPI_DEFINE_STRUCT(DTO,
        SF_tblMTTranslatedPhrasesBase_DTO
    );
}

namespace tblMTTranslationLogsBase {
    namespace Relation {
    }

    namespace Private {
        const QList<clsORMField> ORMFields = {
            //ColName                       Type                        Validation  Default     UpBy    Sort    Filter  Self    Virt    PK
            { Fields::tlsID,                ORM_PRIMARYKEY_64 },
            { Fields::tls_actorID,          S(quint64),                 QFV,        QRequired,  UPOwner },
            { Fields::tls_aptID,            S(NULLABLE_TYPE(quint32)),  QFV,        QNull,      UPOwner },
            { Fields::tlsRequestTime,       S(TAPI::DateTime_t),        QFV,        QNow,       UPOwner },
            { Fields::tlsDir,               S(QString),                 QFV,        QRequired,  UPOwner },
            { Fields::tlsEngine,            S(QString),                 QFV,        QRequired,  UPOwner },
            { Fields::tlsClass,             S(QString),                 QFV,        QRequired,  UPOwner },
            { Fields::tlsServer,            S(NULLABLE_TYPE(qint64)),   QFV,        QNull,      UPOwner },
            { Fields::tlsSourceWordsCount,  S(qint32),                  QFV,        QRequired,  UPOwner },
            { Fields::tlsTranslationTime,   S(double),                  QFV,        QRequired,  UPOwner },
            { Fields::tlsErrNo,             S(qint8),                   QFV,        0,          UPOwner },
        };

        inline const QList<stuRelation> Relations(Q_DECL_UNUSED const QString& _schema) {
            return {};
        };

        const QList<stuDBIndex> Indexes = {
        };

    } //namespace Private

#define SF_tblMTTranslationLogsBase_DTO \
    SF_ORM_PRIMARYKEY_64        (tlsID), \
    SF_quint64                  (tls_actorID), \
    SF_NULLABLE_quint32         (tls_aptID), \
    SF_DateTime_t               (tlsRequestTime), \
    SF_QString                  (tlsDir), \
    SF_QString                  (tlsEngine), \
    SF_QString                  (tlsClass), \
    SF_NULLABLE_qint64          (tlsServer), \
    SF_qint32                   (tlsSourceWordsCount), \
    SF_double                   (tlsTranslationTime), \
    SF_qint8                    (tlsErrNo)

    TAPI_DEFINE_STRUCT(DTO,
        SF_tblMTTranslationLogsBase_DTO
    );
}

#pragma GCC diagnostic pop

/******************************************************/
template <TAPI::enuTokenActorType::Type _itmplTokenActorType>
class intfMTHelperORMBase
{
public:
    typedef typename std::conditional<_itmplTokenActorType == TAPI::enuTokenActorType::USER,
                             APICALLCONTEXT_TYPE_JWT_USER_DECL,
                             APICALLCONTEXT_TYPE_JWT_API_DECL>::type
    APICALLCONTEXT_TYPE_JWT_TOKENBASE_DECL;
//    MT_JWT_TYPE_DECL;

    typedef Q_DECL_UNUSED typename std::conditional<_itmplTokenActorType == TAPI::enuTokenActorType::USER,
                             APICALLCONTEXT_TYPE_JWT_USER_DECL,
                             APICALLCONTEXT_TYPE_JWT_API_DECL>::type
    APICALLCONTEXT_TYPE_JWT_TOKENBASE_IMPL;
//    MT_JWT_TYPE_IMPL;

public:
    TAPI::enuTokenActorType::Type tokenActorType() { return _itmplTokenActorType; }
    bool IsTokenBase() { return _itmplTokenActorType == TAPI::enuTokenActorType::API; }
};

/******************************************************/
/* intf classes ***************************************/
/******************************************************/
template <TAPI::enuTokenActorType::Type _itmplTokenActorType>
class intfMTCorrectionRules : public intfSQLBasedModule, public intfMTHelperORMBase<_itmplTokenActorType>
{
//    Q _OBJECT

public:
    intfMTCorrectionRules(const QString& _schema,
                        const QList<DBM::clsORMField>& _exclusiveCols = {},
                        const QList<DBM::stuRelation>& _exclusiveRelations = {},
                        const QList<DBM::stuDBIndex>& _exclusiveIndexes = {});
};

/******************************************************/
template <TAPI::enuTokenActorType::Type _itmplTokenActorType>
class intfMTDigestedTranslationLogs : public intfSQLBasedModule, public intfMTHelperORMBase<_itmplTokenActorType>
{
//    Q _OBJECT

public:
    intfMTDigestedTranslationLogs(const QString& _schema,
                                const QList<DBM::clsORMField>& _exclusiveCols = {},
                                const QList<DBM::stuRelation>& _exclusiveRelations = {},
                                const QList<DBM::stuDBIndex>& _exclusiveIndexes = {});
};

/******************************************************/
template <TAPI::enuTokenActorType::Type _itmplTokenActorType>
class intfMTMultiDic : public intfSQLBasedModule, public intfMTHelperORMBase<_itmplTokenActorType>
{
//    Q _OBJECT

public:
    intfMTMultiDic(const QString& _schema,
                 const QList<DBM::clsORMField>& _exclusiveCols = {},
                 const QList<DBM::stuRelation>& _exclusiveRelations = {},
                 const QList<DBM::stuDBIndex>& _exclusiveIndexes = {});
};

/******************************************************/
template <TAPI::enuTokenActorType::Type _itmplTokenActorType>
class intfMTTokenStats : public intfSQLBasedModule, public intfMTHelperORMBase<_itmplTokenActorType>
{
//    Q _OBJECT

public:
    intfMTTokenStats(const QString& _schema,
                   const QList<DBM::clsORMField>& _exclusiveCols = {},
                   const QList<DBM::stuRelation>& _exclusiveRelations = {},
                   const QList<DBM::stuDBIndex>& _exclusiveIndexes = {});
};

/******************************************************/
template <TAPI::enuTokenActorType::Type _itmplTokenActorType>
class intfMTTranslatedPhrases : public intfSQLBasedModule, public intfMTHelperORMBase<_itmplTokenActorType>
{
//    Q _OBJECT

public:
    intfMTTranslatedPhrases(const QString& _schema,
                          const QList<DBM::clsORMField>& _exclusiveCols = {},
                          const QList<DBM::stuRelation>& _exclusiveRelations = {},
                          const QList<DBM::stuDBIndex>& _exclusiveIndexes = {});
};

/******************************************************/
template <TAPI::enuTokenActorType::Type _itmplTokenActorType>
class intfMTTranslationLogs : public intfSQLBasedModule, public intfMTHelperORMBase<_itmplTokenActorType>
{
//    Q _OBJECT

public:
    intfMTTranslationLogs(const QString& _schema,
                        const QList<DBM::clsORMField>& _exclusiveCols = {},
                        const QList<DBM::stuRelation>& _exclusiveRelations = {},
                        const QList<DBM::stuDBIndex>& _exclusiveIndexes = {});
};

/******************************************************/
} //namespace Targoman::API::ModuleHelpers::MT::Interfaces

#endif // #define TARGOMAN_API_AAA_ACCOUNTING_INTERFACES_H
