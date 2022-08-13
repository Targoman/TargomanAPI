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

#ifndef TARGOMAN_API_MODULES_ACCOUNT_ORM_BLOCKINGRULES_H
#define TARGOMAN_API_MODULES_ACCOUNT_ORM_BLOCKINGRULES_H

#include "Interfaces/AAA/AAA.hpp"
#include "Interfaces/API/intfSQLBasedModule.h"

namespace Targoman::API::AccountModule {

//structures and enumes goes here

namespace ORM {

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-variable"
namespace tblBlockingRules {
    constexpr char Name[] = "tblBlockingRules";

    namespace Fields {
        TARGOMAN_CREATE_CONSTEXPR(blrID);
        TARGOMAN_CREATE_CONSTEXPR(blr_ipbIP);
        TARGOMAN_CREATE_CONSTEXPR(blr_ipIPReadable);
        TARGOMAN_CREATE_CONSTEXPR(blrStartingTime);
        TARGOMAN_CREATE_CONSTEXPR(blrEndingTime);
        TARGOMAN_CREATE_CONSTEXPR(blrCause);
        TARGOMAN_CREATE_CONSTEXPR(blrCreatedBy_usrID);
        TARGOMAN_CREATE_CONSTEXPR(blrCreationDateTime);
        TARGOMAN_CREATE_CONSTEXPR(blrUpdatedBy_usrID);
        TARGOMAN_CREATE_CONSTEXPR(blrStatus);
    }

    namespace Relation {
        // constexpr char AAA[] = "aaa";
    }

    namespace Private {
        const QList<clsORMField> ORMFields = {
            ///ColName                                Type                 Validation                       Default   UpBy     Sort   Filter Self  Virt   PK
            { Fields::blrID,               ORM_PRIMARYKEY_64} ,
            { Fields::blr_ipbIP,           S(quint32),          QFV.integer().minValue(1),       QNull,    UPAdmin },
            { Fields::blr_ipIPReadable,    S(TAPI::IPv4_t),     QFV,                             QInvalid, UPNone,  false, false },
            { Fields::blrStartingTime,     S(TAPI::DateTime_t), QFV,                             QNull,    UPNone,  true },
            { Fields::blrEndingTime,       S(TAPI::DateTime_t), QFV,                             QNull,    UPAdmin },
            { Fields::blrCause,            S(QString),          QFV,                             QNull,    UPAdmin, false, false },
            { Fields::blrStatus,           ORM_STATUS_FIELD(TAPI::enuGenericStatus, TAPI::enuGenericStatus::Active) },
            { Fields::blrCreatedBy_usrID,  ORM_CREATED_BY },
            { Fields::blrCreationDateTime, ORM_CREATED_ON },
            { Fields::blrUpdatedBy_usrID,  ORM_UPDATED_BY },
        };

        const QList<stuRelation> Relations = {
            ///Col                                    Reference Table              ForeignCol          Rename     LeftJoin
            ORM_RELATION_OF_CREATOR(Fields::blrCreatedBy_usrID),
            ORM_RELATION_OF_UPDATER(Fields::blrUpdatedBy_usrID),
        };

        const QList<stuDBIndex> Indexes = {
        };

    } //namespace Private

    TAPI_DEFINE_STRUCT(DTO,
        SF_ORM_PRIMARYKEY_64        (blrID),
        SF_quint32                  (blr_ipbIP),
        SF_QString                  (blr_ipIPReadable),
        SF_DateTime_t               (blrStartingTime),
        SF_DateTime_t               (blrEndingTime),
        SF_QString                  (blrCause),
        SF_ORM_STATUS_FIELD         (blrStatus, TAPI::enuGenericStatus, TAPI::enuGenericStatus::Active),
        SF_ORM_CREATED_BY           (blrCreatedBy_usrID),
        SF_ORM_CREATED_ON           (blrCreationDateTime),
        SF_ORM_UPDATED_BY           (blrUpdatedBy_usrID)
    );
}
#pragma GCC diagnostic pop

class BlockingRules : public intfSQLBasedModule
{
    Q_OBJECT
    TARGOMAN_DEFINE_API_SUBMODULE(Account, BlockingRules)

private slots:
    QVariant ORMGET("Get BlockingRules information")
    quint64 ORMCREATE("Create a new BlockingRule by an authorized user")
    bool ORMUPDATE("Update blocking rule info by an authorized user")
    bool ORMDELETE("Delete a BlockingRules")
};

} //namespace ORM
} //namespace Targoman::API::AccountModule

#endif // TARGOMAN_API_MODULES_ACCOUNT_ORM_BLOCKINGRULES_H
