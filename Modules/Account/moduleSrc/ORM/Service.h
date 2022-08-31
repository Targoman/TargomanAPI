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

#ifndef TARGOMAN_API_MODULES_ACCOUNT_ORM_SERVICES_H
#define TARGOMAN_API_MODULES_ACCOUNT_ORM_SERVICES_H

#include "Interfaces/AAA/AAA.hpp"
#include "Interfaces/API/intfSQLBasedModule.h"
#include "Roles.h"

namespace Targoman::API::AccountModule {

//structures and enumes goes here

namespace ORM {

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-variable"
namespace tblService {
    constexpr char Name[] = "tblService";
    //constexpr char Invalid_ProcessVoucherItemEndPoint[] = "INVALID";

    namespace Fields {
        TARGOMAN_CREATE_CONSTEXPR(svcID);
        TARGOMAN_CREATE_CONSTEXPR(svcName);
//        TARGOMAN_CREATE_CONSTEXPR(svcOppositeTokenTypeServiceName);
        TARGOMAN_CREATE_CONSTEXPR(svc_rolID);
        TARGOMAN_CREATE_CONSTEXPR(svcProcessVoucherItemEndPoint);
        TARGOMAN_CREATE_CONSTEXPR(svcCancelVoucherItemEndPoint);
//        TARGOMAN_CREATE_CONSTEXPR(svcAcceptableTokenType);
        TARGOMAN_CREATE_CONSTEXPR(svcStatus);
        TARGOMAN_CREATE_CONSTEXPR(svcCreationDateTime);
        TARGOMAN_CREATE_CONSTEXPR(svcCreatedBy_usrID);
        TARGOMAN_CREATE_CONSTEXPR(svcUpdatedBy_usrID);
    }

    namespace Relation {
        // constexpr char AAA[] = "aaa";
    }

    namespace Private {
        const QList<clsORMField> ORMFields = {
            //ColName                                   Type                            Validation      Default     UpBy   Sort  Filter Self  Virt   PK
            { Fields::svcID,                            ORM_PRIMARYKEY_32 },
            { Fields::svcName,                          S(QString),                     QFV,            QRequired,  UPAdmin },
//            { Fields::svcOppositeTokenTypeServiceName,  S(QString),                     QFV,            QNull,      UPAdmin },
            { Fields::svc_rolID,                        S(quint32),                     QFV,            QRequired,  UPAdmin },
            { Fields::svcProcessVoucherItemEndPoint,    S(NULLABLE_TYPE(QString)),      QFV,            QNull,      UPAdmin },
            { Fields::svcCancelVoucherItemEndPoint,     S(NULLABLE_TYPE(QString)),      QFV,            QNull,      UPAdmin },
//            { Fields::svcAcceptableTokenType,           S(TAPI::enuTokenActorType::Type), QFV, TAPI::enuTokenActorType::User, UPAdmin },
            { Fields::svcStatus,                        ORM_STATUS_FIELD(TAPI::enuGenericStatus, TAPI::enuGenericStatus::Active) },
            { ORM_INVALIDATED_AT_FIELD },
            { Fields::svcCreationDateTime,              ORM_CREATED_ON },
            { Fields::svcCreatedBy_usrID,               ORM_CREATED_BY },
            { Fields::svcUpdatedBy_usrID,               ORM_UPDATED_BY },
        };

        const QList<stuRelation> Relations = {
            ///Col                    Reference Table              ForeignCol      Rename     LeftJoin
                { Fields::svc_rolID, R(AAASchema, tblRoles::Name), tblRoles::Fields::rolID },
                ORM_RELATION_OF_CREATOR(Fields::svcCreatedBy_usrID),
                ORM_RELATION_OF_UPDATER(Fields::svcUpdatedBy_usrID),
            };

        const QList<stuDBIndex> Indexes = {
            { {
                  Fields::svcName,
                  ORM_INVALIDATED_AT_FIELD_NAME,
              }, enuDBIndex::Unique },
        };

    } //namespace Private

    TAPI_DEFINE_STRUCT(DTO,
        SF_ORM_PRIMARYKEY_32        (svcID),
        SF_QString                  (svcName),
//        SF_QString                  (svcOppositeTokenTypeServiceName),
        SF_quint32                  (svc_rolID),
        SF_QString                  (svcProcessVoucherItemEndPoint),
        SF_QString                  (svcCancelVoucherItemEndPoint),
//        SF_Enum                     (svcAcceptableTokenType, TAPI::enuTokenActorType, TAPI::enuTokenActorType::User),
        SF_ORM_STATUS_FIELD         (svcStatus, TAPI::enuGenericStatus, TAPI::enuGenericStatus::Active),
        SF_ORM_CREATED_ON           (svcCreationDateTime),
        SF_ORM_CREATED_BY           (svcCreatedBy_usrID),
        SF_ORM_UPDATED_BY           (svcUpdatedBy_usrID)
    );
}
#pragma GCC diagnostic pop

class Service : public intfSQLBasedModule
{
    Q_OBJECT
    TARGOMAN_API_SUBMODULE_DEFINE(Account, Service)

private slots:
    QVariant ORMGET("Get Service information")
    quint64 ORMCREATE("Create a new Service by an authorized user")
    bool ORMUPDATE("Update Service info by an authorized user")
    bool ORMDELETE("Delete a Service")
};

} //namespace ORM
} //namespace Targoman::API::AccountModule

#endif // TARGOMAN_API_MODULES_ACCOUNT_ORM_SERVICES_H
