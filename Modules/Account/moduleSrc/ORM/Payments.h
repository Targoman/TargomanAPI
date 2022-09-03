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

#ifndef TARGOMAN_API_MODULES_ACCOUNT_ORM_PAYMENTORDERS_H
#define TARGOMAN_API_MODULES_ACCOUNT_ORM_PAYMENTORDERS_H

#include "PaymentGateways.h"
#include "Interfaces/AAA/AAA.hpp"
#include "Interfaces/API/intfSQLBasedModule.h"
#include "Voucher.h"
#include "Interfaces/ObjectStorage/ObjectStorageManager.h"
using namespace Targoman::API::ObjectStorage;

namespace Targoman::API::AccountModule {

//enumes goes here

TARGOMAN_DEFINE_ENUM(enuPaymentStatus,
                     New      = 'N',
                     Pending  = 'P', //used for Claimed for offline payments
                     Payed    = 'Y',
                     Succeded = 'A',
                     Error    = 'E',
                     Rejected = 'J',
                     Removed  = 'R'
                     )

} //namespace Targoman::API::AccountModule

TAPI_DECLARE_METATYPE_ENUM(Targoman::API::AccountModule, enuPaymentStatus);

namespace Targoman::API::AccountModule {

//structures goes here

namespace ORM {

/*****************************************************************\
|* OnlinePayments ************************************************|
\*****************************************************************/
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-variable"
namespace tblOnlinePayments {
    constexpr char Name[] = "tblOnlinePayments";

    namespace Fields {
        TARGOMAN_CREATE_CONSTEXPR(onpID);
        TARGOMAN_CREATE_CONSTEXPR(onpMD5); //used for making payment callback url, e.g.: https://{tg.com}/callback/payment/verify?paymentKey={onpMD5}
        TARGOMAN_CREATE_CONSTEXPR(onp_vchID);
        TARGOMAN_CREATE_CONSTEXPR(onp_pgwID);
        TARGOMAN_CREATE_CONSTEXPR(onpTrackNumber);
        TARGOMAN_CREATE_CONSTEXPR(onpAmount);
        TARGOMAN_CREATE_CONSTEXPR(onpCallbackUrl);
        TARGOMAN_CREATE_CONSTEXPR(onpTarget_walID);
        TARGOMAN_CREATE_CONSTEXPR(onpResult);
        TARGOMAN_CREATE_CONSTEXPR(onpStatus);
        TARGOMAN_CREATE_CONSTEXPR(onpCreationDateTime);
        TARGOMAN_CREATE_CONSTEXPR(onpLastUpdateDateTime);
    }

    namespace Relation {
        // constexpr char AAA[] = "aaa";
    }

    namespace Private {
        const QList<clsORMField> ORMFields = {
            ///ColName                                   Type                    Validation                          Default     UpBy   Sort  Filter Self  Virt   PK
            { Fields::onpID,                 ORM_PRIMARYKEY_64 },
            { Fields::onpMD5,                S(TAPI::MD5_t),         QFV,                                QRequired,  UPAdmin },
            { Fields::onp_vchID,             S(quint64),             QFV.integer().minValue(1),          QRequired,  UPAdmin },
            { Fields::onp_pgwID,             S(quint32),             QFV.integer().minValue(1),          QRequired,  UPAdmin },
            { Fields::onpTrackNumber,        S(QString),             QFV.allwaysValid().maxLenght(50),   QNull,      UPAdmin },
            { Fields::onpAmount,             S(quint64),             QFV.integer().minValue(1),          QRequired,  UPAdmin },
            { Fields::onpCallbackUrl,        S(QString),             QFV.allwaysValid().maxLenght(2048), QRequired,  UPAdmin },
            { Fields::onpTarget_walID,       S(quint64),             QFV,                                QNull,      UPNone },
            { Fields::onpResult,             S(QString),             QFV,                                QNull,      UPAdmin, false, false },
            { Fields::onpStatus,             ORM_STATUS_FIELD(Targoman::API::AccountModule::enuPaymentStatus, Targoman::API::AccountModule::enuPaymentStatus::New) },
            { Fields::onpCreationDateTime,   ORM_CREATED_ON },
            { Fields::onpLastUpdateDateTime, ORM_UPDATED_ON },
        };

        const QList<stuRelation> Relations = {
            ///<                     Col                           Reference Table                            ForeignCol         Rename     LeftJoin
            { "voucher",        { Fields::onp_vchID, R(AAASchema, tblVoucher::Name),            tblVoucher::Fields::vchID } },
            { "paymentGateway", { Fields::onp_pgwID, R(AAASchema, tblPaymentGateways::Name),    tblPaymentGateways::Fields::pgwID } },
        };

        const QList<stuDBIndex> Indexes = {
        };

    } //namespace Private

    TAPI_DEFINE_STRUCT(DTO,
        SF_ORM_PRIMARYKEY_64        (onpID),
        SF_MD5_t                    (onpMD5),
        SF_quint64                  (onp_vchID),
        SF_quint32                  (onp_pgwID),
        SF_QString                  (onpTrackNumber),
        SF_quint32                  (onpAmount),
        SF_QString                  (onpCallbackUrl),
        SF_NULLABLE_quint64         (onpTarget_walID),
        SF_QString                  (onpResult),
        SF_ORM_STATUS_FIELD         (onpStatus, Targoman::API::AccountModule::enuPaymentStatus, Targoman::API::AccountModule::enuPaymentStatus::Pending),
        SF_DateTime_t               (onpCreationDateTime),
        SF_QString                  (onpLastUpdateDateTime)
    );
}
#pragma GCC diagnostic pop

class OnlinePayments : public intfSQLBasedModule
{
    Q_OBJECT
    TARGOMAN_API_SUBMODULE_DEFINE(Account, OnlinePayments)

private slots:
    QVariant ORMGET_USER("Get OnlinePayment information.")

    QVariant EXREST_GET_OR_POST(
        paymentCallback,
        (
            APICALLBOOM_TYPE_NO_JWT_DECL &APICALLBOOM_PARAM,
            QString _paymentKey
        ),
        "Payment callback proxy",
        {
            EXRESTCONFIG_HIDDEN,
        }
    )

#ifdef QT_DEBUG
    QVariant REST_GET_OR_POST(
        devTestPayPage,
        (
            APICALLBOOM_TYPE_NO_JWT_DECL &APICALLBOOM_PARAM,
            QString _paymentKey,
            QString _trackID,
            QString _callback = {}
        ),
        ""
    )

    QVariant REST_GET_OR_POST(
        devTestCallbackPage,
        (
            APICALLBOOM_TYPE_NO_JWT_DECL &APICALLBOOM_PARAM,
            QString _paymentKey,
            QString _result
        ),
        ""
    )
#endif
};

/*****************************************************************\
|* OfflinePaymentClaims ******************************************|
\*****************************************************************/
/*

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-variable"
namespace tblOfflinePaymentClaims {
    constexpr char Name[] = "tblOfflinePaymentClaims";

    namespace Fields {
        TARGOMAN_CREATE_CONSTEXPR(ofpcID);
        TARGOMAN_CREATE_CONSTEXPR(ofpc_vchID);
        TARGOMAN_CREATE_CONSTEXPR(ofpcBank);
        TARGOMAN_CREATE_CONSTEXPR(ofpcReceiptCode);
        TARGOMAN_CREATE_CONSTEXPR(ofpcReceiptDate);
        TARGOMAN_CREATE_CONSTEXPR(ofpcAmount);
        TARGOMAN_CREATE_CONSTEXPR(ofpcTarget_walID);
        TARGOMAN_CREATE_CONSTEXPR(ofpcNotes);
        TARGOMAN_CREATE_CONSTEXPR(ofpcReceiptImage_uflID);
        TARGOMAN_CREATE_CONSTEXPR(ofpcStatus);
        TARGOMAN_CREATE_CONSTEXPR(ofpcCreationDateTime);
        TARGOMAN_CREATE_CONSTEXPR(ofpcCreatedBy_usrID);
        TARGOMAN_CREATE_CONSTEXPR(ofpcUpdatedBy_usrID);
    }

    namespace Relation {
        // constexpr char AAA[] = "aaa";
    }

    namespace Private {
        const QList<clsORMField> ORMFields = {
            //ColName                           Type                        Validation                          Default     UpBy   Sort  Filter Self  Virt   PK
            { Fields::ofpcID,                   ORM_PRIMARYKEY_64 },
            { Fields::ofpc_vchID,               S(NULLABLE_TYPE(quint64)),  QFV.integer().minValue(1),          QNull,      UPOwner },
            { Fields::ofpcBank,                 S(QString),                 QFV.allwaysValid().maxLenght(50),   QRequired,  UPOwner },
            { Fields::ofpcReceiptCode,          S(QString),                 QFV.allwaysValid().maxLenght(50),   QRequired,  UPOwner },
            { Fields::ofpcReceiptDate,          S(TAPI::DateTime_t),        QFV,                                QRequired,  UPOwner },
            { Fields::ofpcAmount,               S(quint32),                 QFV,                                QRequired,  UPOwner },
            { Fields::ofpcTarget_walID,         S(NULLABLE_TYPE(quint64)),  QFV,                                QNull,      UPNone  },
            { Fields::ofpcNotes,                S(QString),                 QFV.allwaysValid().maxLenght(500),  QNull,      UPOwner },
            { Fields::ofpcReceiptImage_uflID,   S(NULLABLE_TYPE(quint64)),  QFV,                                QNull,      UPOwner },
            { Fields::ofpcStatus,               ORM_STATUS_FIELD(Targoman::API::AccountModule::enuPaymentStatus, Targoman::API::AccountModule::enuPaymentStatus::New) },
            { Fields::ofpcCreationDateTime,     ORM_CREATED_ON },
            { Fields::ofpcCreatedBy_usrID,      ORM_CREATED_BY },
            { Fields::ofpcUpdatedBy_usrID,      ORM_UPDATED_BY },
        };

        const QList<stuRelation> Relations = {
            //Col                               Reference Table                 ForeignCol          Rename     LeftJoin
            { Fields::ofpc_vchID,               R(AAASchema, tblVoucher::Name), tblVoucher::Fields::vchID },
            ORM_RELATION_OF_CREATOR(Fields::ofpcCreatedBy_usrID),
            ORM_RELATION_OF_UPDATER(Fields::ofpcUpdatedBy_usrID),
            { Fields::ofpcReceiptImage_uflID,   R(AAASchema, tblUploadFiles::Name), tblUploadFiles::Fields::uflID },
            { Fields::ofpcReceiptImage_uflID,   R(AAASchema, tblUploadQueue::Name), tblUploadQueue::Fields::uqu_uflID},
        };

        const QList<stuDBIndex> Indexes = {
        };

    } //namespace Private

    TAPI_DEFINE_STRUCT(DTO,
        SF_ORM_PRIMARYKEY_64        (ofpcID),
        SF_NULLABLE_quint64         (ofpc_vchID),
        SF_QString                  (ofpcBank),
        SF_QString                  (ofpcReceiptCode),
//        SF_QString                  (ofpcReceiptDate),
        SF_DateTime_t               (ofpcReceiptDate),
        SF_quint32                  (ofpcAmount),
        SF_NULLABLE_quint64         (ofpcTarget_walID),
        SF_QString                  (ofpcNotes),
        SF_NULLABLE_quint64         (ofpcReceiptImage_uflID),
        SF_ORM_STATUS_FIELD         (ofpcStatus, Targoman::API::AccountModule::enuPaymentStatus, Targoman::API::AccountModule::enuPaymentStatus::New),
//        SF_QString                  (ofpcCreationDateTime),
        SF_DateTime_t               (ofpcCreationDateTime),
        SF_quint64                  (ofpcCreatedBy_usrID),
        SF_quint64                  (ofpcUpdatedBy_usrID)
    );
}
#pragma GCC diagnostic pop

class OfflinePaymentClaims : public intfSQLBasedModule
{
    Q_OBJECT
    TARGOMAN_API_SUBMODULE_DEFINE(Account, OfflinePaymentClaims)

private slots:
    QVariant ORMGET_USER("Get OfflinePaymentClaim(s) information.")
//    quint32 ORMCREATE_USER("Create a new OfflinePaymentClaim")
//    bool ORMUPDATE_USER("Update OfflinePaymentClaim info")
//    bool ORMDELETE_USER("Delete an OfflinePaymentClaim")
};

*/

/*
CREATE TABLE `tblOfflinePaymentClaims_attachments` (
    `ofpcatID` BIGINT(20) UNSIGNED NOT NULL AUTO_INCREMENT,
    `ofpcat_ofpcID` BIGINT(20) UNSIGNED NULL DEFAULT NULL,
    `ofpcat_uplID` BIGINT(20) UNSIGNED NULL DEFAULT NULL,
    `ofpcatCreationDateTime` DATETIME NOT NULL DEFAULT CURRENT_TIMESTAMP,
    `ofpcatCreatedBy_usrID` BIGINT(20) UNSIGNED NULL DEFAULT NULL,
    PRIMARY KEY (`ofpcatID`) USING BTREE,
    INDEX `FK_tblOfflinePaymentClaims_attachments_tblOfflinePaymentClaims` (`ofpcat_ofpcID`) USING BTREE,
    INDEX `FK_tblOfflinePaymentClaims_attachments_tblUploads` (`ofpcat_uplID`) USING BTREE,
    CONSTRAINT `FK_tblOfflinePaymentClaims_attachments_tblOfflinePaymentClaims` FOREIGN KEY (`ofpcat_ofpcID`) REFERENCES `tblOfflinePaymentClaims` (`ofpcID`) ON UPDATE NO ACTION ON DELETE CASCADE,
    CONSTRAINT `FK_tblOfflinePaymentClaims_attachments_tblUploads` FOREIGN KEY (`ofpcat_uplID`) REFERENCES `tblUploadFiles` (`uflID`) ON UPDATE NO ACTION ON DELETE CASCADE
)
COLLATE='utf8mb4_general_ci'
ENGINE=InnoDB
;

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-variable"
namespace tblOfflinePaymentClaimsAttachments {
    constexpr char Name[] = "tblOfflinePaymentClaims_attachments";

    namespace Fields {
        TARGOMAN_CREATE_CONSTEXPR(ofpcatID);
        TARGOMAN_CREATE_CONSTEXPR(ofpcat_ofpcID);
        TARGOMAN_CREATE_CONSTEXPR(ofpcat_uplID);
        TARGOMAN_CREATE_CONSTEXPR(ofpcatCreationDateTime);
        TARGOMAN_CREATE_CONSTEXPR(ofpcatCreatedBy_usrID);
    }

    namespace Relation {
        constexpr char OfflinePaymentClaims[] = "OfflinePaymentClaims";
    }

    namespace Private {
        const QList<clsORMField> ORMFields = {
            //ColName                           Type                    Validation                  Default     UpBy   Sort  Filter Self  Virt   PK
            { Fields::ofpcatID,                 ORM_PRIMARYKEY_64 },
            { Fields::ofpcat_ofpcID,            S(quint64),             QFV.integer().minValue(1),  QRequired,  UPNone },
            { Fields::ofpcat_uplID,             S(quint64),             QFV.integer().minValue(1),  QRequired,  UPNone },
            { Fields::ofpcatCreationDateTime,   ORM_CREATED_ON },
            { Fields::ofpcatCreatedBy_usrID,    ORM_CREATED_BY },
        };

        const QList<stuRelation> Relations = {
            //Alias        Col                               Reference Table                            ForeignCol          Rename     LeftJoin
            { Relation::OfflinePaymentClaims,
              { Fields::ofpcat_ofpcID,  R(AAASchema, tblOfflinePaymentClaims::Name), tblOfflinePaymentClaims::Fields::ofpcID } },
            { Fields::ofpcat_uplID,  R(AAASchema, tblUploadFiles::Name),  tblUploadFiles::Fields::uflID },
            //this index is preventing querybuilder invalid column for filter error:
            { Fields::ofpcat_uplID,  R(AAASchema, tblUploadQueue::Name),  tblUploadQueue::Fields::uqu_uflID },
            ORM_RELATION_OF_CREATOR(Fields::ofpcatCreatedBy_usrID),
        };

        const QList<stuDBIndex> Indexes = {
        };

    } //namespace Private

    TAPI_DEFINE_STRUCT(DTO,
        SF_ORM_PRIMARYKEY_64        (ofpcatID),
        SF_quint64                  (ofpcat_ofpcID),
        SF_quint64                  (ofpcat_uplID),
        SF_ORM_CREATED_ON           (ofpcatCreationDateTime),
        SF_ORM_CREATED_BY           (ofpcatCreatedBy_usrID)
    );
}
#pragma GCC diagnostic pop

class OfflinePaymentClaimsAttachments : public intfSQLBasedModule
{
    Q_OBJECT
    TARGOMAN_API_SUBMODULE_DEFINE(Account, OfflinePaymentClaimsAttachments)

private slots:
    QVariant ORMGET_USER("Get OfflinePaymentClaims Attachments")
};
*/

/*****************************************************************\
|* OfflinePayments ***********************************************|
\*****************************************************************/
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-variable"
namespace tblOfflinePayments {
    constexpr char Name[] = "tblOfflinePayments";

    namespace Fields {
        TARGOMAN_CREATE_CONSTEXPR(ofpID);
        TARGOMAN_CREATE_CONSTEXPR(ofp_vchID);
        TARGOMAN_CREATE_CONSTEXPR(ofpBank);
        TARGOMAN_CREATE_CONSTEXPR(ofpReceiptCode);
        TARGOMAN_CREATE_CONSTEXPR(ofpReceiptDate);
        TARGOMAN_CREATE_CONSTEXPR(ofpAmount);
        TARGOMAN_CREATE_CONSTEXPR(ofpTarget_walID);
        TARGOMAN_CREATE_CONSTEXPR(ofpNotes);
        TARGOMAN_CREATE_CONSTEXPR(ofpReceiptImage_uflID);
        TARGOMAN_CREATE_CONSTEXPR(ofpStatus);
        TARGOMAN_CREATE_CONSTEXPR(ofpCreationDateTime);
        TARGOMAN_CREATE_CONSTEXPR(ofpCreatedBy_usrID);
        TARGOMAN_CREATE_CONSTEXPR(ofpUpdatedBy_usrID);
    }

    namespace Relation {
        // constexpr char AAA[] = "aaa";
    }

    namespace Private {
        const QList<clsORMField> ORMFields = {
            //ColName                           Type                        Validation                          Default     UpBy   Sort  Filter Self  Virt   PK
            { Fields::ofpID,                    ORM_PRIMARYKEY_64 },
            { Fields::ofp_vchID,                S(NULLABLE_TYPE(quint64)),  QFV.integer().minValue(1),          QNull,      UPOwner },
            { Fields::ofpBank,                  S(QString),                 QFV.allwaysValid().maxLenght(50),   QRequired,  UPOwner },
            { Fields::ofpReceiptCode,           S(QString),                 QFV.allwaysValid().maxLenght(50),   QRequired,  UPOwner },
            { Fields::ofpReceiptDate,           S(TAPI::DateTime_t),        QFV,                                QRequired,  UPOwner },
            { Fields::ofpAmount,                S(quint32),                 QFV,                                QRequired,  UPOwner },
            { Fields::ofpTarget_walID,          S(NULLABLE_TYPE(quint64)),  QFV,                                QNull,      UPNone  },
            { Fields::ofpNotes,                 S(QString),                 QFV.allwaysValid().maxLenght(500),  QNull,      UPOwner },
            { Fields::ofpReceiptImage_uflID,    S(NULLABLE_TYPE(quint64)),  QFV,                                QNull,      UPOwner },
            { Fields::ofpStatus,                ORM_STATUS_FIELD(Targoman::API::AccountModule::enuPaymentStatus, Targoman::API::AccountModule::enuPaymentStatus::New) },
            { Fields::ofpCreationDateTime,      ORM_CREATED_ON },
            { Fields::ofpCreatedBy_usrID,       ORM_CREATED_BY },
            { Fields::ofpUpdatedBy_usrID,       ORM_UPDATED_BY },
        };

        const QList<stuRelation> Relations = {
            //Col                               Reference Table                  ForeignCol         Rename     LeftJoin
            { Fields::ofp_vchID,                R(AAASchema,tblVoucher::Name),   tblVoucher::Fields::vchID},
            ORM_RELATION_OF_CREATOR(Fields::ofpCreatedBy_usrID),
            ORM_RELATION_OF_UPDATER(Fields::ofpUpdatedBy_usrID),
            { Fields::ofpReceiptImage_uflID,    R(AAASchema, tblUploadFiles::Name), tblUploadFiles::Fields::uflID },
            { Fields::ofpReceiptImage_uflID,    R(AAASchema, tblUploadQueue::Name), tblUploadQueue::Fields::uqu_uflID},
        };

        const QList<stuDBIndex> Indexes = {
        };

    } //namespace Private

    TAPI_DEFINE_STRUCT(DTO,
        SF_ORM_PRIMARYKEY_64        (ofpID),
        SF_NULLABLE_quint64         (ofp_vchID),
        SF_QString                  (ofpBank),
        SF_QString                  (ofpReceiptCode),
//        SF_QString                  (ofpReceiptDate),
        SF_DateTime_t               (ofpReceiptDate),
        SF_quint32                  (ofpAmount),
        SF_NULLABLE_quint64         (ofpTarget_walID),
        SF_QString                  (ofpNotes),
        SF_NULLABLE_quint64         (ofpReceiptImage_uflID),
        SF_ORM_STATUS_FIELD         (ofpStatus, Targoman::API::AccountModule::enuPaymentStatus, Targoman::API::AccountModule::enuPaymentStatus::New),
//        SF_QString                  (ofpCreationDateTime),
        SF_DateTime_t               (ofpCreationDateTime),
        SF_quint64                  (ofpCreatedBy_usrID),
        SF_quint64                  (ofpUpdatedBy_usrID)
    );
}
#pragma GCC diagnostic pop

class OfflinePayments : public intfSQLBasedModule
{
    Q_OBJECT
    TARGOMAN_API_SUBMODULE_DEFINE(Account, OfflinePayments)

private slots:
    QVariant ORMGET_USER("Get OfflinePayment information.")
    bool ORMUPDATE_USER("Update OfflinePayment info by an authorized user")
};

/*****************************************************************/
} //namespace ORM

struct stuOnlinePayment
{
    ORM::tblOnlinePayments::DTO OnlinePayment;
    ORM::tblPaymentGateways::DTO PaymentGateway;

    void fromVariantMap(const QVariantMap& _info);
};

} //namespace Targoman::API::AccountModule

#endif // TARGOMAN_API_MODULES_ACCOUNT_ORM_PAYMENTORDERS_H
