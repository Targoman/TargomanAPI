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

/******************************************************/
/* tbl name & fields **********************************/
/******************************************************/
namespace tblAPITokens {
    constexpr char Name[] = "tblAPITokens";

    namespace Fields {
        TARGOMAN_CREATE_CONSTEXPR(aptID);
        TARGOMAN_CREATE_CONSTEXPR(aptToken);
        TARGOMAN_CREATE_CONSTEXPR(aptName);
        TARGOMAN_CREATE_CONSTEXPR(apt_usrID);
        TARGOMAN_CREATE_CONSTEXPR(aptLang);
        TARGOMAN_CREATE_CONSTEXPR(aptValidateIP);
        TARGOMAN_CREATE_CONSTEXPR(aptExtraPrivileges);
        TARGOMAN_CREATE_CONSTEXPR(aptExpiryDate);
        TARGOMAN_CREATE_CONSTEXPR(aptLastActivity);
        TARGOMAN_CREATE_CONSTEXPR(aptAccessCount);
        TARGOMAN_CREATE_CONSTEXPR(aptStatus);
        TARGOMAN_CREATE_CONSTEXPR(aptCreationDateTime);
        TARGOMAN_CREATE_CONSTEXPR(aptCreatedBy_usrID);
        TARGOMAN_CREATE_CONSTEXPR(aptUpdatedBy_usrID);
    }
}

namespace tblAPITokenServices {
    constexpr char Name[] = "tblAPITokenServices";

    namespace Fields {
        TARGOMAN_CREATE_CONSTEXPR(aptsvc_aptID);
        TARGOMAN_CREATE_CONSTEXPR(aptsvc_svcID);
        TARGOMAN_CREATE_CONSTEXPR(aptsvcCreationDateTime);
        TARGOMAN_CREATE_CONSTEXPR(aptsvcCreatedBy_usrID);
    }
}

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
}

/******************************************************/
/* tbl inside *****************************************/
/******************************************************/
namespace tblAPITokens {
    namespace Relation {
        // constexpr char AAA[] = "aaa";
    }

    namespace Private {
        const QList<clsORMField> ORMFields = {
            //ColName                       Type                    validation                          Default     UpBy        Sort  Filter Self  Virt   PK
            { Fields::aptID,                ORM_PRIMARYKEY_64 },
            { Fields::aptToken,             S(QString),             QFV.asciiAlNum().maxLenght(750),    QRequired,  UPAdmin,     true, true },
            { Fields::aptName,              S(QString),             QFV.asciiAlNum().maxLenght(250),    QRequired,  UPOwner,    true, true },
            { Fields::apt_usrID,            S(quint64),             QFV.integer().minValue(1),          QRequired,  UPNone },
            { Fields::aptLang,              S(TAPI::ISO639_2_t),    QFV,                                "en",       UPOwner },
            { Fields::aptValidateIP,        S(bool),                QFV,                                false,      UPOwner },
            { Fields::aptExtraPrivileges,   S(TAPI::PrivObject_t),  QFV,                                QNull,      UPAdmin,    false, false },
            { Fields::aptExpiryDate,        S(TAPI::DateTime_t),    QFV,                                QNull,      UPAdmin },
            { Fields::aptLastActivity,      S(TAPI::DateTime_t),    QFV,                                QInvalid,   UPNone },
            { Fields::aptAccessCount,       S(quint32),             QFV.integer().minValue(1),          QInvalid,   UPNone },
            { Fields::aptStatus,            ORM_STATUS_FIELD(Targoman::API::AccountModule::enuAPITokensStatus, Targoman::API::AccountModule::enuAPITokensStatus::Active) },
            { ORM_INVALIDATED_AT_FIELD },
            { Fields::aptCreationDateTime,  ORM_CREATED_ON },
            { Fields::aptCreatedBy_usrID,   ORM_CREATED_BY },
            { Fields::aptUpdatedBy_usrID,   ORM_UPDATED_BY },
        };

        const QList<stuRelation> Relations = {
            //Col                           Reference Table                 ForeignCol                  Rename      LeftJoin
            { Fields::apt_usrID,            R(AAASchema, tblUser::Name),    tblUser::Fields::usrID,     "Owner_",   true },
            ORM_RELATION_OF_CREATOR(Fields::aptCreatedBy_usrID),
            ORM_RELATION_OF_UPDATER(Fields::aptUpdatedBy_usrID),
        };

        const QList<stuDBIndex> Indexes = {
            { {
                Fields::aptToken,
                ORM_INVALIDATED_AT_FIELD_NAME,
              }, enuDBIndex::Unique },
//            { {
//                Fields::aptName,
//                Fields::apt_usrID,
//                ORM_INVALIDATED_AT_FIELD_NAME,
//              }, enuDBIndex::Unique },
        };

    } //namespace Private

    TAPI_DEFINE_STRUCT(DTO,
        SF_ORM_PRIMARYKEY_64        (aptID),
        SF_QString                  (aptToken),
        SF_QString                  (aptName),
        SF_quint64                  (apt_usrID),
        SF_QString                  (aptLang),
        SF_bool                     (aptValidateIP),
        SF_JSON_t                   (aptExtraPrivileges),
        SF_DateTime_t               (aptExpiryDate),
        SF_DateTime_t               (aptLastActivity),
        SF_quint32                  (aptAccessCount),
        SF_ORM_STATUS_FIELD         (aptStatus, Targoman::API::AccountModule::enuAPITokensStatus, Targoman::API::AccountModule::enuAPITokensStatus::Active),
        SF_ORM_CREATED_BY           (aptCreatedBy_usrID),
        SF_ORM_CREATED_ON           (aptCreationDateTime),
        SF_ORM_UPDATED_BY           (aptUpdatedBy_usrID)
    );
}

namespace tblAPITokenServices {
    namespace Relation {
        // constexpr char AAA[] = "aaa";
    }

    namespace Private {
        const QList<clsORMField> ORMFields = {
            //ColName                           Type                    validation                  Default     UpBy        Sort  Filter Self  Virt   PK
            { Fields::aptsvc_aptID,             S(quint64),             QFV.integer().minValue(1),  QRequired,  UPNone },
            { Fields::aptsvc_svcID,             S(quint32),             QFV.integer().minValue(1),  QRequired,  UPNone },
            { Fields::aptsvcCreationDateTime,   ORM_CREATED_ON },
            { Fields::aptsvcCreatedBy_usrID,    ORM_CREATED_BY },
        };

        const QList<stuRelation> Relations = {
            //Col                               Reference Table                     ForeignCol                  Rename      LeftJoin
            { Fields::aptsvc_aptID,             R(AAASchema, tblAPITokens::Name),   tblAPITokens::Fields::aptID },
            { Fields::aptsvc_svcID,             R(AAASchema, tblService::Name),     tblService::Fields::svcID },
            ORM_RELATION_OF_CREATOR(Fields::aptsvcCreatedBy_usrID),
        };

        const QList<stuDBIndex> Indexes = {
            { {
                Fields::aptsvc_aptID,
                Fields::aptsvc_svcID,
              }, enuDBIndex::Primary },
        };

    } //namespace Private

    TAPI_DEFINE_STRUCT(DTO,
        SF_quint64                  (aptsvc_aptID),
        SF_quint32                  (aptsvc_svcID),
        SF_ORM_CREATED_BY           (aptsvcCreationDateTime),
        SF_ORM_CREATED_ON           (aptsvcCreatedBy_usrID)
    );
}

namespace tblAPITokenValidIPs {
    namespace Relation {
        // constexpr char AAA[] = "aaa";
    }

    namespace Private {
        const QList<clsORMField> ORMFields = {
            ///ColName                                   Type                 Validation                      Default     UpBy   Sort  Filter Self  Virt   PK
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
            ///Col                                     Reference Table                   ForeignCol              Rename     LeftJoin
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

    TAPI_DEFINE_STRUCT(DTO,
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

/******************************************************/
/* classes ********************************************/
/******************************************************/
class APITokens : public intfSQLBasedModule
{
    Q_OBJECT
    TARGOMAN_API_SUBMODULE_DEFINE(Account, APITokens);

public:
    TAPI::EncodedJWT_t create(
        INTFAPICALLBOOM_DECL &APICALLBOOM_PARAM,
        quint64 _userID,
        const QString &_name,
        const QStringList &_services = {}
    );

private slots:
    QVariant ORMGET("Get APITokens information")
    quint64 ORMCREATE("Create a new APITokens by an authorized user")
    bool ORMUPDATE("Update token info by an authorized user")
    bool ORMDELETE("Delete an APIToken")
};

/******************************************************/
class APITokenServices : public intfSQLBasedModule
{
    Q_OBJECT
    TARGOMAN_API_SUBMODULE_DEFINE(Account, APITokenServices)

private slots:
//    QVariant ORMGET("Get APITokenServices information")
//    quint64 ORMCREATE("Create a new APITokenServices by an authorized user")
//    bool ORMDELETE("Delete an APIToken")
};

/******************************************************/
class APITokenValidIPs : public intfSQLBasedModule
{
    Q_OBJECT
    TARGOMAN_API_SUBMODULE_DEFINE(Account, APITokenValidIPs)

private slots:
    QVariant ORMGET("Get APITokenValidIPs information")
    bool ORMDELETE("Delete an APITokenValidIP")
    bool ORMUPDATE("Update token valid IP info")
    quint64 ORMCREATE("Create a new APITokenValidIP")
};

/******************************************************/
} //namespace ORM
} //namespace Targoman::API::AccountModule

#endif // TARGOMAN_API_MODULES_ACCOUNT_ORM_APITOKENS_H
