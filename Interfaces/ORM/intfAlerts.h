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

#ifndef TARGOMAN_API_ALERTS_H
#define TARGOMAN_API_ALERTS_H

#include "Interfaces/DBM/clsTable.h"
#include "Interfaces/AAA/AAADefs.hpp"

using namespace Targoman::API::DBM;

namespace Targoman::API::ORM {

TARGOMAN_DEFINE_ENUM(enuAlertType,
                     Informational = 'I',
                     Critical = 'C',
                     Warning = 'W'
                     )

TARGOMAN_DEFINE_ENUM(enuAlertStatus,
                     New = 'N',
                     Sent = 'S',
                     Error = 'E'
                     )

} //namespace Targoman::API::ORM

TAPI_DECLARE_METATYPE_ENUM(Targoman::API::ORM, enuAlertType);
TAPI_DECLARE_METATYPE_ENUM(Targoman::API::ORM, enuAlertStatus);

namespace Targoman::API::ORM {

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-variable"
namespace tblAlerts {
    constexpr char Name[] = "tblAlerts";

    namespace Fields {
        TARGOMAN_CREATE_CONSTEXPR(alrID);
        TARGOMAN_CREATE_CONSTEXPR(alrType);
        TARGOMAN_CREATE_CONSTEXPR(alr_usrID);
        TARGOMAN_CREATE_CONSTEXPR(alrLanguage);
        TARGOMAN_CREATE_CONSTEXPR(alrReplacedContactInfo);
        TARGOMAN_CREATE_CONSTEXPR(alr_altCode);
        TARGOMAN_CREATE_CONSTEXPR(alrReplacements);
        TARGOMAN_CREATE_CONSTEXPR(alrCreateDate);
        TARGOMAN_CREATE_CONSTEXPR(alrLockedAt);
        TARGOMAN_CREATE_CONSTEXPR(alrLockedBy);
        TARGOMAN_CREATE_CONSTEXPR(alrLastTryAt);
        TARGOMAN_CREATE_CONSTEXPR(alrSentDate);
        TARGOMAN_CREATE_CONSTEXPR(alrStatus);
        TARGOMAN_CREATE_CONSTEXPR(alrResult);
    }

    inline QStringList ColumnNames(QString _tableAlias = "") {
        if (_tableAlias.isEmpty() == false)
            _tableAlias += ".";

        return {
            _tableAlias + Fields::alrID,
            _tableAlias + Fields::alrType,
            _tableAlias + Fields::alr_usrID,
            _tableAlias + Fields::alrLanguage,
            _tableAlias + Fields::alrReplacedContactInfo,
            _tableAlias + Fields::alr_altCode,
            _tableAlias + Fields::alrReplacements,
            _tableAlias + Fields::alrCreateDate,
            _tableAlias + Fields::alrLockedAt,
            _tableAlias + Fields::alrLockedBy,
            _tableAlias + Fields::alrLastTryAt,
            _tableAlias + Fields::alrSentDate,
            _tableAlias + Fields::alrStatus,
            _tableAlias + Fields::alrResult,
        };
    }

    namespace Relation {
        // constexpr char AAA[] = "aaa";
    }

    namespace Private {
        const QList<clsORMField> ORMFields = {
            ///< ColName                               Type                                        Validation                  Default     UpBy       Sort    Filter  Self    Virt    PK      Select  RenameAs
            { Fields::alrID,                     ORM_PRIMARYKEY_64 },
            { Fields::alrType,                   S(Targoman::API::ORM::enuAlertType::Type),  QFV,                        QRequired,  UPOwner },
            { Fields::alr_usrID,                 S(NULLABLE_TYPE(quint64)),                  QFV.integer().minValue(1),  QNull,      UPOwner },
            { Fields::alrLanguage,               S(TAPI::String_t),                          QFV.maxLenght(2),           QRequired,  UPAdmin },
            { Fields::alrReplacedContactInfo,    S(TAPI::String_t),                          QFV.maxLenght(50),          QRequired,  UPAdmin },
            { Fields::alr_altCode,               S(TAPI::String_t),                          QFV.maxLenght(50),          QRequired,  UPAdmin },
            { Fields::alrReplacements,           S(TAPI::DBText_t),                          QFV,                        QRequired,  UPAdmin },
            { Fields::alrCreateDate,             ORM_CREATED_ON },
            { Fields::alrLockedAt,               S(TAPI::DateTime_t),                        QFV,                        QNull,      UPAdmin },
            { Fields::alrLockedBy,               S(QString),                                 QFV,                        QNull,      UPAdmin },
            { Fields::alrLastTryAt,              S(TAPI::DateTime_t),                        QFV,                        QNull,      UPAdmin },
            { Fields::alrSentDate,               S(TAPI::DateTime_t),                        QFV,                        QNull,      UPAdmin },
            { Fields::alrStatus,                 ORM_STATUS_FIELD(Targoman::API::ORM::enuAlertStatus,                    Targoman::API::ORM::enuAlertStatus::New) },
            { Fields::alrResult,                 S(TAPI::JSON_t),                            QFV,                        QNull,      UPNone, false, false },
        };

        const QList<stuRelation> Relations = {
        };

        const QList<stuDBIndex> Indexes = {
        };

    } //namespace Private

    TAPI_DEFINE_VARIANT_ENABLED_STRUCT(DTO,
        SF_ORM_PRIMARYKEY_64        (alrID),
        SF_Enum                     (alrType, Targoman::API::ORM::enuAlertType, Targoman::API::ORM::enuAlertType::Informational),
        SF_NULLABLE_quint64         (alr_usrID),
        SF_QString                  (alrLanguage),
        SF_QString                  (alrReplacedContactInfo),
        SF_QString                  (alr_altCode),
        SF_QString                  (alrReplacements),
        SF_DateTime_t               (alrLockedAt),
        SF_QString                  (alrLockedBy),
        SF_DateTime_t               (alrLastTryAt),
        SF_DateTime_t               (alrSentDate),
        SF_JSON_t                   (alrResult),
        SF_ORM_STATUS_FIELD         (alrStatus, Targoman::API::ORM::enuAlertStatus, Targoman::API::ORM::enuAlertStatus::New),
        SF_ORM_CREATED_ON           (alrCreateDate)
    );
}
#pragma GCC diagnostic pop

class intfAlerts : public clsTable
{
public:
    intfAlerts();
//    instanceGetter(intfAlerts)

public:
    static bool createNewAlert(
        quint64 _currentUserID,
        enuAlertType::Type _type,
        quint64 _usrID,
        QString _replacedContactInfo,
        QString _altCode,
        QString _replacements
    );
};

} //namespace Targoman::API::ORM

#endif // TARGOMAN_API_ALERTS_H
