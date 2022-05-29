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

#ifndef TARGOMAN_API_MODULES_ACCOUNT_ORM_APITOKENS_H
#define TARGOMAN_API_MODULES_ACCOUNT_ORM_APITOKENS_H

#include "Interfaces/AAA/AAA.hpp"
#include "Interfaces/API/intfSQLBasedModule.h"
#include "Service.h"

namespace Targoman::API::AccountModule {

//structures and enumes goes here

TARGOMAN_DEFINE_ENUM(enuAPITokensStatus,
                     Active         = 'A',
                     Pending        = 'P',
                     CreditFinished = 'C',
                     Removed        = 'R'
                     )

} //namespace Targoman::API::AccountModule

TAPI_DECLARE_METATYPE_ENUM(Targoman::API::AccountModule, enuAPITokensStatus);

namespace Targoman::API::AccountModule {
namespace ORM {

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-variable"
namespace tblAPITokens {
    constexpr char Name[] = "tblAPITokens";

    namespace Fields {
        TARGOMAN_CREATE_CONSTEXPR(aptID);
        TARGOMAN_CREATE_CONSTEXPR(aptToken);
        TARGOMAN_CREATE_CONSTEXPR(apt_usrID);
        TARGOMAN_CREATE_CONSTEXPR(apt_svcID);
        TARGOMAN_CREATE_CONSTEXPR(aptLang);
        TARGOMAN_CREATE_CONSTEXPR(aptValidateIP);
        TARGOMAN_CREATE_CONSTEXPR(aptExtraPriviledges);
        TARGOMAN_CREATE_CONSTEXPR(aptExpiryDate);
        TARGOMAN_CREATE_CONSTEXPR(aptLastActivity);
        TARGOMAN_CREATE_CONSTEXPR(aptAccessCount);
        TARGOMAN_CREATE_CONSTEXPR(aptCreatedBy_usrID);
        TARGOMAN_CREATE_CONSTEXPR(aptCreationDateTime);
        TARGOMAN_CREATE_CONSTEXPR(aptUpdatedBy_usrID);
        TARGOMAN_CREATE_CONSTEXPR(aptStatus);
    }

    inline QStringList ColumnNames(QString _tableAlias = "") {
        if (_tableAlias.isEmpty() == false)
            _tableAlias += ".";

        return {
            _tableAlias + Fields::aptID,
            _tableAlias + Fields::aptToken,
            _tableAlias + Fields::apt_usrID,
            _tableAlias + Fields::apt_svcID,
            _tableAlias + Fields::aptLang,
            _tableAlias + Fields::aptValidateIP,
            _tableAlias + Fields::aptExtraPriviledges,
            _tableAlias + Fields::aptExpiryDate,
            _tableAlias + Fields::aptLastActivity,
            _tableAlias + Fields::aptAccessCount,
            _tableAlias + Fields::aptCreatedBy_usrID,
            _tableAlias + Fields::aptCreationDateTime,
            _tableAlias + Fields::aptUpdatedBy_usrID,
            _tableAlias + Fields::aptStatus,
        };
    }

    namespace Relation {
        // constexpr char AAA[] = "aaa";
    }

    namespace Private {
        const QList<clsORMField> ORMFields = {
            ///< ColName                            Type                   Validation                      Default    UpBy   Sort  Filter Self  Virt   PK
            { Fields::aptID,               ORM_PRIMARYKEY_64 },
            { Fields::aptToken,            S(QString),            QFV.asciiAlNum().maxLenght(50), QRequired, UPNone, true, false },
            { Fields::apt_usrID,           S(quint64),            QFV.integer().minValue(1),      QRequired, UPNone },
            { Fields::apt_svcID,           S(quint32),            QFV.integer().minValue(1),      QRequired, UPAdmin },
            { Fields::aptLang,             S(TAPI::ISO639_2_t),   QFV,                            "en",      UPAdmin },
            { Fields::aptValidateIP,       S(bool),               QFV,                            false,     UPAdmin },
            { Fields::aptExtraPriviledges, S(TAPI::PrivObject_t), QFV,                            QNull,     UPAdmin, false, false },
            { Fields::aptExpiryDate,       S(TAPI::DateTime_t),   QFV,                            QNull,     UPAdmin },
            { Fields::aptLastActivity,     S(TAPI::DateTime_t),   QFV,                            QInvalid,  UPNone },
            { Fields::aptAccessCount,      S(quint32),            QFV.integer().minValue(1),      QInvalid,  UPNone },
            { Fields::aptStatus,           ORM_STATUS_FIELD(Targoman::API::AccountModule::enuAPITokensStatus, Targoman::API::AccountModule::enuAPITokensStatus::Active) },
            { ORM_INVALIDATED_AT_FIELD },
            { Fields::aptCreatedBy_usrID,  ORM_CREATED_BY },
            { Fields::aptCreationDateTime, ORM_CREATED_ON },
            { Fields::aptUpdatedBy_usrID,  ORM_UPDATED_BY },
        };

        const QList<stuRelation> Relations = {
            ///< Col                                Reference Table                 ForeignCol         Rename    LeftJoin
            { Fields::apt_svcID,           R(AAASchema, tblService::Name), tblService::Fields::svcID, {},       true},
            { Fields::apt_usrID,           R(AAASchema, tblUser::Name),    tblUser::Fields::usrID,    "Owner_", true},
            ORM_RELATION_OF_CREATOR(Fields::aptCreatedBy_usrID),
            ORM_RELATION_OF_UPDATER(Fields::aptUpdatedBy_usrID),
        };

        const QList<stuDBIndex> Indexes = {
            { {
                Fields::aptToken,
                ORM_INVALIDATED_AT_FIELD_NAME,
              }, enuDBIndex::Unique },
        };

    } //namespace Private

    TAPI_DEFINE_VARIANT_ENABLED_STRUCT(DTO,
        SF_ORM_PRIMARYKEY_64        (aptID),
        SF_QString                  (aptToken),
        SF_quint64                  (apt_usrID),
        SF_quint32                  (apt_svcID),
        SF_QString                  (aptLang),
        SF_bool                     (aptValidateIP),
        SF_JSON_t                   (aptExtraPriviledges),
        SF_DateTime_t               (aptExpiryDate),
        SF_DateTime_t               (aptLastActivity),
        SF_quint32                  (aptAccessCount),
        SF_ORM_STATUS_FIELD         (aptStatus, Targoman::API::AccountModule::enuAPITokensStatus, Targoman::API::AccountModule::enuAPITokensStatus::Active),
        SF_ORM_CREATED_BY           (aptCreatedBy_usrID),
        SF_ORM_CREATED_ON           (aptCreationDateTime),
        SF_ORM_UPDATED_BY           (aptUpdatedBy_usrID)
    );
}
#pragma GCC diagnostic pop

class APITokens : public intfSQLBasedModule
{
    Q_OBJECT
    TARGOMAN_DEFINE_API_SUBMODULE(Account, APITokens)

private slots:
    QVariant ORMGET("Get APITokens information")
    quint64 ORMCREATE("Create a new APITokens by priviledged user")
    bool ORMUPDATE("Update token info by priviledged user")
    bool ORMDELETE("Delete an APIToken")
};

} //namespace ORM
} //namespace Targoman::API::AccountModule

#endif // TARGOMAN_API_MODULES_ACCOUNT_ORM_APITOKENS_H
