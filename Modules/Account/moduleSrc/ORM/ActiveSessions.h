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

#ifndef TARGOMAN_API_MODULES_ACCOUNT_ORM_ACTIVESESSIONS_H
#define TARGOMAN_API_MODULES_ACCOUNT_ORM_ACTIVESESSIONS_H

#include "Account.h"
#include "Interfaces/AAA/AAA.hpp"
#include "Interfaces/API/intfSQLBasedModule.h"

namespace Targoman::API::AccountModule {

//structures and enumes goes here

TARGOMAN_DEFINE_ENUM(enuSessionStatus,
                     Active    = 'A',
                     LoggedOut = 'G',
                     FiredOut  = 'F',
                     Expired   = 'E',
                     Removed   = 'R'
                     )

} //namespace Targoman::API::AccountModule

TAPI_DECLARE_METATYPE_ENUM(Targoman::API::AccountModule, enuSessionStatus);

namespace Targoman::API::AccountModule {
namespace ORM {

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-variable"
namespace tblActiveSessions {
    constexpr char Name[] = "tblActiveSessions";

    namespace Fields {
        TARGOMAN_CREATE_CONSTEXPR(ssnKey);
        TARGOMAN_CREATE_CONSTEXPR(ssn_usrID);
        TARGOMAN_CREATE_CONSTEXPR(ssnIP);
        TARGOMAN_CREATE_CONSTEXPR(ssnIPReadable);
        TARGOMAN_CREATE_CONSTEXPR(ssnCreationDateTime);
        TARGOMAN_CREATE_CONSTEXPR(ssnInfo);
        TARGOMAN_CREATE_CONSTEXPR(ssnFingerPrint);
        TARGOMAN_CREATE_CONSTEXPR(ssnLastActivity);
        TARGOMAN_CREATE_CONSTEXPR(ssnLastRenew);
        TARGOMAN_CREATE_CONSTEXPR(ssnRemember);
        TARGOMAN_CREATE_CONSTEXPR(ssnJWT);
        TARGOMAN_CREATE_CONSTEXPR(ssnStatus);
        TARGOMAN_CREATE_CONSTEXPR(ssnUpdatedBy_usrID);
    }

    inline QStringList ColumnNames(QString _tableAlias = "") {
        if (_tableAlias.isEmpty() == false)
            _tableAlias += ".";

        return {
            _tableAlias + Fields::ssnKey,
            _tableAlias + Fields::ssn_usrID,
            _tableAlias + Fields::ssnIP,
            _tableAlias + Fields::ssnIPReadable,
            _tableAlias + Fields::ssnCreationDateTime,
            _tableAlias + Fields::ssnInfo,
            _tableAlias + Fields::ssnFingerPrint,
            _tableAlias + Fields::ssnLastActivity,
            _tableAlias + Fields::ssnLastRenew,
            _tableAlias + Fields::ssnRemember,
            _tableAlias + Fields::ssnJWT,
            _tableAlias + Fields::ssnStatus,
            _tableAlias + Fields::ssnUpdatedBy_usrID,
        };
    }

    namespace Relation {
        // constexpr char AAA[] = "aaa";
    }

    namespace Private {
        const QList<clsORMField> ORMFields = {
            ///< ColName                                   Type                    Validation                  Default     UpBy    Sort    Filter  Self    Virt    PK      Select
            { Fields::ssnKey,                S(TAPI::MD5_t),         QFV,                        ORM_PRIMARY_KEY },
            { Fields::ssn_usrID,             S(quint64),             QFV.integer().minValue(1),  QRequired,  UPNone },
            { Fields::ssnIP,                 S(quint32),             QFV.integer().minValue(1),  QRequired,  UPNone, false,  false,  false,  false,  false,  false },
            { Fields::ssnIPReadable,         S(QString),             QFV.allwaysInvalid(),       QInvalid,   UPNone, false,  false,  false,  false,  false,  false },
            { Fields::ssnInfo,               S(TAPI::JSON_t),        QFV,                        QNull,      UPNone, false,  false,  false,  false,  false,  false },
            { Fields::ssnFingerPrint,        S(TAPI::MD5_t),         QFV.allwaysInvalid(),       QNull,      UPNone, false,  false,  false,  false,  false,  false },
            { Fields::ssnLastActivity,       S(TAPI::DateTime_t),    QFV,                        QNull,      UPNone },
            { Fields::ssnLastRenew,          S(quint64),             QFV,                        QNull,      UPNone },
            { Fields::ssnRemember,           S(bool),                QFV,                        false,      UPNone },
            { Fields::ssnJWT,                S(QString),             QFV,                        QNull,      UPAdmin,false,  false,  false,  false,  false,  false },
            { Fields::ssnStatus,             ORM_STATUS_FIELD(Targoman::API::AccountModule::enuSessionStatus, Targoman::API::AccountModule::enuSessionStatus::Active) },
            { Fields::ssnCreationDateTime,   ORM_CREATED_ON },
            { Fields::ssnUpdatedBy_usrID,    ORM_UPDATED_BY },
        };

        const QList<stuRelation> Relations = {
            ///< Col                                       Reference Table              ForeignCol      Rename      LeftJoin
            { Fields::ssn_usrID,             R(AAASchema, tblUser::Name), tblUser::Fields::usrID, "Owner_" },
            { Fields::ssnUpdatedBy_usrID,    R(AAASchema, tblUser::Name), tblUser::Fields::usrID, "Updater_", true }
        };

        const QList<stuDBIndex> Indexes = {
        };

    } //namespace Private

    TAPI_DEFINE_VARIANT_ENABLED_STRUCT(DTO,
        SF_MD5_t                    (ssnKey),
        SF_quint64                  (ssn_usrID),
        SF_quint32                  (ssnIP),
        SF_QString                  (ssnIPReadable),
        SF_JSON_t                   (ssnInfo),
        SF_MD5_t                    (ssnFingerPrint),
        SF_DateTime_t               (ssnLastActivity),
        SF_quint64                  (ssnLastRenew),
        SF_bool                     (ssnRemember),
        SF_QString                  (ssnJWT),
        SF_ORM_STATUS_FIELD         (ssnStatus, Targoman::API::AccountModule::enuSessionStatus, Targoman::API::AccountModule::enuSessionStatus::Active),
        SF_ORM_CREATED_ON           (ssnCreationDateTime),
        SF_ORM_UPDATED_BY           (ssnUpdatedBy_usrID)
    );
}
#pragma GCC diagnostic pop

class ActiveSessions : public intfSQLBasedModule
{
    Q_OBJECT
    TARGOMAN_DEFINE_API_SUBMODULE(Account, ActiveSessions)

private slots:
    QVariant ORMGET("Get ActiveSessions information.")
    bool ORMDELETE("Delete an active session. Destroying current session is not allowed.")
};

} //namespace ORM
} //namespace Targoman::API::AccountModule

#endif // TARGOMAN_API_MODULES_ACCOUNT_ORM_ACTIVESESSIONS_H
