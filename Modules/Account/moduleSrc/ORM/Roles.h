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

#ifndef TARGOMAN_API_MODULES_ACCOUNT_ORM_ROLES_H
#define TARGOMAN_API_MODULES_ACCOUNT_ORM_ROLES_H

#include "Interfaces/AAA/AAA.hpp"
#include "Interfaces/API/intfSQLBasedModule.h"

namespace Targoman::API::AccountModule {

//structures and enumes goes here

TARGOMAN_DEFINE_ENUM(enuRoleStatus,
                     Active   = 'A',
                     Blocked  = 'B',
                     Removed  = 'R'
                     )

} //namespace Targoman::API::AccountModule

TAPI_DECLARE_METATYPE_ENUM(Targoman::API::AccountModule, enuRoleStatus);

namespace Targoman::API::AccountModule {
namespace ORM {

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-variable"
namespace tblRoles {
    constexpr char Name[] = "tblRoles";

    namespace Fields {
        TARGOMAN_CREATE_CONSTEXPR(rolID);
        TARGOMAN_CREATE_CONSTEXPR(rolName);
        TARGOMAN_CREATE_CONSTEXPR(rolParent_rolID);
        TARGOMAN_CREATE_CONSTEXPR(rolPrivileges);
        TARGOMAN_CREATE_CONSTEXPR(rolSignupAllowedIPs);
        TARGOMAN_CREATE_CONSTEXPR(rolStatus);
        TARGOMAN_CREATE_CONSTEXPR(rolCreatedBy_usrID);
        TARGOMAN_CREATE_CONSTEXPR(rolCreationDateTime);
        TARGOMAN_CREATE_CONSTEXPR(rolUpdatedBy_usrID);
    }

    namespace Relation {
        // constexpr char AAA[] = "aaa";
    }

    namespace Private {
        const QList<clsORMField> ORMFields = {
            ///<ColName                          Type                   Validation                        Default    UpBy   Sort  Filter Self  Virt   PK
                { Fields::rolID,                ORM_PRIMARYKEY_32 },
                { Fields::rolName,              S(QString),            QFV/*.unicodeAlNum()*/.maxLenght(50), QRequired, UPAdmin },
                { Fields::rolParent_rolID,      S(quint32),            QFV.integer().minValue(1),        QNull,     UPAdmin },
                { Fields::rolPrivileges,        S(TAPI::PrivObject_t), QFV,                              QRequired, UPAdmin, false, false },
                { Fields::rolSignupAllowedIPs,  S(QString),            QFV,                              QNull,     UPAdmin, false, false }, //OJO This must be validated after splitting by comma
                { Fields::rolStatus,            ORM_STATUS_FIELD(Targoman::API::AccountModule::enuRoleStatus, Targoman::API::AccountModule::enuRoleStatus::Active) },
                { ORM_INVALIDATED_AT_FIELD },
                { Fields::rolCreationDateTime,  ORM_CREATED_ON },
                { Fields::rolCreatedBy_usrID,   ORM_CREATED_BY_NULLABLE },
                { Fields::rolUpdatedBy_usrID,   ORM_UPDATED_BY },
            };

        const QList<stuRelation> Relations = {
            ///Col                        Reference Table              ForeignCol       Rename     LeftJoin
                { Fields::rolParent_rolID, R(AAASchema,tblRoles::Name), Fields::rolID, "Parent_", true },
                ORM_RELATION_OF_CREATOR(Fields::rolCreatedBy_usrID),
                ORM_RELATION_OF_UPDATER(Fields::rolUpdatedBy_usrID),
            };

        const QList<stuDBIndex> Indexes = {
            { {
                Fields::rolName,
                ORM_INVALIDATED_AT_FIELD_NAME,
              }, enuDBIndex::Unique },
        };

    } //namespace Private

    TAPI_DEFINE_STRUCT(DTO,
        SF_ORM_PRIMARYKEY_32        (rolID),
        SF_QString                  (rolName),
        SF_quint32                  (rolParent_rolID),
        SF_JSON_t                   (rolPrivileges),
        SF_QString                  (rolSignupAllowedIPs),
        SF_ORM_STATUS_FIELD         (rolStatus, Targoman::API::AccountModule::enuRoleStatus, Targoman::API::AccountModule::enuRoleStatus::Active),
        SF_ORM_CREATED_ON           (rolCreationDateTime),
        SF_ORM_CREATED_BY_NULLABLE  (rolCreatedBy_usrID),
        SF_ORM_UPDATED_BY           (rolUpdatedBy_usrID)
    );
}
#pragma GCC diagnostic pop

class Roles : public intfSQLBasedModule
{
    Q_OBJECT
    TARGOMAN_DEFINE_API_SUBMODULE(Account, Roles)

private slots:
    QVariant ORMGET("Get roles information")
    quint32 ORMCREATE("Create a new Role by an authorized user")
    bool ORMUPDATE("Update role info by an authorized user")
    bool ORMDELETE("Delete a Role")
};

} //namespace ORM
} //namespace Targoman::API::AccountModule

#endif // TARGOMAN_API_MODULES_ACCOUNT_ORM_ROLES_H
