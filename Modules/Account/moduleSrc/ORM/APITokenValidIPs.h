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

#ifndef TARGOMAN_API_MODULES_ACCOUNT_ORM_APITOKENVALIDIPS_H
#define TARGOMAN_API_MODULES_ACCOUNT_ORM_APITOKENVALIDIPS_H

#include "Interfaces/AAA/AAA.hpp"
#include "Interfaces/API/intfSQLBasedModule.h"
#include "APITokens.h"

namespace Targoman::API::AccountModule {

//structures and enumes goes here

namespace ORM {

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-variable"
namespace tblAPITokenValidIPs {
    constexpr char Name[] = "tblAPITokenValidIPs";

    namespace Fields {
        TARGOMAN_CREATE_CONSTEXPR(tviID);
        TARGOMAN_CREATE_CONSTEXPR(tvi_aptID);
        TARGOMAN_CREATE_CONSTEXPR(tviIP);
        TARGOMAN_CREATE_CONSTEXPR(tviIPReadable);
        TARGOMAN_CREATE_CONSTEXPR(tviCreatedBy_usrID);
        TARGOMAN_CREATE_CONSTEXPR(tviCreationDateTime);
        TARGOMAN_CREATE_CONSTEXPR(tviUpdatedBy_usrID);
        TARGOMAN_CREATE_CONSTEXPR(tviStatus);
    }

    inline QStringList ColumnNames(QString _tableAlias = "") {
        if (_tableAlias.isEmpty() == false)
            _tableAlias += ".";

        return {
            _tableAlias + Fields::tviID,
            _tableAlias + Fields::tvi_aptID,
            _tableAlias + Fields::tviIP,
            _tableAlias + Fields::tviIPReadable,
            _tableAlias + Fields::tviCreatedBy_usrID,
            _tableAlias + Fields::tviCreationDateTime,
            _tableAlias + Fields::tviUpdatedBy_usrID,
            _tableAlias + Fields::tviStatus,
        };
    }

    namespace Relation {
        // constexpr char AAA[] = "aaa";
    }

    namespace Private {
        const QList<clsORMField> ORMFields = {
            ///< ColName                                   Type                 Validation                      Default     UpBy   Sort  Filter Self  Virt   PK
            { Fields::tviID,               ORM_PRIMARYKEY_64 },
            { Fields::tvi_aptID,           S(quint64),          QFV.integer().minValue(1),      QRequired,  UPNone },
            { Fields::tviIP,               S(quint64),          QFV.integer().minValue(1),      QRequired,  UPOwner },
            { Fields::tviIPReadable,       S(QString),          QFV.allwaysInvalid(),           QInvalid,   UPNone, false, false },
            { Fields::tviStatus,           ORM_STATUS_FIELD(TAPI::enuGenericStatus, TAPI::enuGenericStatus::Active) },
            { ORM_INVALIDATED_AT_FIELD },
            { Fields::tviCreationDateTime, ORM_CREATED_ON },
            { Fields::tviCreatedBy_usrID,  ORM_CREATED_BY },
            { Fields::tviUpdatedBy_usrID,  ORM_UPDATED_BY },
        };

        const QList<stuRelation> Relations = {
            ///< Col                                     Reference Table                   ForeignCol              Rename     LeftJoin
            { Fields::tvi_aptID,          R(AAASchema, tblAPITokens::Name),  tblAPITokens::Fields::aptID },
            ORM_RELATION_OF_CREATOR(Fields::tviCreatedBy_usrID),
            ORM_RELATION_OF_UPDATER(Fields::tviUpdatedBy_usrID),
        };

        const QList<stuDBIndex> Indexes = {
            { {
                Fields::tvi_aptID,
                Fields::tviIP,
                ORM_INVALIDATED_AT_FIELD_NAME,
              }, enuDBIndex::Unique },
        };

    } //namespace Private

    TAPI_DEFINE_VARIANT_ENABLED_STRUCT(DTO,
        SF_ORM_PRIMARYKEY_64        (tviID),
        SF_quint64                  (tvi_aptID),
        SF_quint64                  (tviIP),
        SF_QString                  (tviIPReadable),
        SF_ORM_STATUS_FIELD         (tviStatus, TAPI::enuGenericStatus, TAPI::enuGenericStatus::Active),
        SF_ORM_CREATED_ON           (tviCreationDateTime),
        SF_ORM_CREATED_BY           (tviCreatedBy_usrID),
        SF_ORM_UPDATED_BY           (tviUpdatedBy_usrID)
    );
}
#pragma GCC diagnostic pop

class APITokenValidIPs : public intfSQLBasedModule
{
    Q_OBJECT
    TARGOMAN_DEFINE_API_SUBMODULE(Account, APITokenValidIPs)

private slots:
    QVariant ORMGET("Get APITokenValidIPs information")
    bool ORMDELETE("Delete an APITokenValidIP")
    bool ORMUPDATE("Update token valid IP info")
    quint64 ORMCREATE("Create a new APITokenValidIP")
};

} //namespace ORM
} //namespace Targoman::API::AccountModule

#endif // TARGOMAN_API_MODULES_ACCOUNT_ORM_APITOKENVALIDIPS_H
