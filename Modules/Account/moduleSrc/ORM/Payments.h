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

namespace Targoman::API::AccountModule {

//enumes goes here

TARGOMAN_DEFINE_ENUM(enuPaymentStatus,
                     New      = 'N',
                     Pending  = 'P',
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
        TARGOMAN_CREATE_CONSTEXPR(onpMD5); //used for making payment callback url, e.g.: https://{tg.com}/callback/payment/verify?paymentMD5={onpMD5}
        TARGOMAN_CREATE_CONSTEXPR(onp_vchID);
        TARGOMAN_CREATE_CONSTEXPR(onp_pgwID);
        TARGOMAN_CREATE_CONSTEXPR(onpTrackNumber);
        TARGOMAN_CREATE_CONSTEXPR(onpAmount);
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
    TARGOMAN_DEFINE_API_SUBMODULE(Account, OnlinePayments)

private slots:
    QVariant ORMGET("Get OnlinePayment information.")

#ifdef QT_DEBUG
    QVariant REST_GET_OR_POST(
        devTestPayPage,
        (
            APICALLBOOM_TYPE_NO_JWT_DECL &APICALLBOOM_PARAM,
            QString _trackID,
            QString _callback
        ),
        ""
    )
#endif
};

/*****************************************************************\
|* OfflinePaymentClaims ******************************************|
\*****************************************************************/
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
            ///ColName                                           Type                        Validation                          Default     UpBy   Sort  Filter Self  Virt   PK
            { Fields::ofpcID,                  ORM_PRIMARYKEY_64},
            { Fields::ofpc_vchID,              S(NULLABLE_TYPE(quint64)),  QFV.integer().minValue(1),          QNull,      UPOwner},
            { Fields::ofpcBank,                S(QString),                 QFV.allwaysValid().maxLenght(50),   QRequired,  UPOwner},
            { Fields::ofpcReceiptCode,         S(QString),                 QFV.allwaysValid().maxLenght(50),   QRequired,  UPOwner},
            { Fields::ofpcReceiptDate,         S(TAPI::DateTime_t),        QFV,                                QRequired,  UPOwner},
            { Fields::ofpcAmount,              S(quint32),                 QFV,                                QRequired,  UPOwner},
            { Fields::ofpcTarget_walID,        S(quint64),                 QFV,                                QNull,      UPNone },
            { Fields::ofpcNotes,               S(QString),                 QFV.allwaysValid().maxLenght(500),  QNull,      UPOwner},
            { Fields::ofpcStatus,              ORM_STATUS_FIELD(Targoman::API::AccountModule::enuPaymentStatus, Targoman::API::AccountModule::enuPaymentStatus::New) },
            { Fields::ofpcCreationDateTime,    ORM_CREATED_ON},
            { Fields::ofpcCreatedBy_usrID,     ORM_CREATED_BY},
            { Fields::ofpcUpdatedBy_usrID,     ORM_UPDATED_BY},
        };

        const QList<stuRelation> Relations = {
            ///Col                                               Reference Table                 ForeignCol          Rename     LeftJoin
            { Fields::ofpc_vchID,              R(AAASchema, tblVoucher::Name), tblVoucher::Fields::vchID },
            ORM_RELATION_OF_CREATOR(Fields::ofpcCreatedBy_usrID),
            ORM_RELATION_OF_UPDATER(Fields::ofpcUpdatedBy_usrID),
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
    TARGOMAN_DEFINE_API_SUBMODULE(Account, OfflinePaymentClaims)

private slots:
    QVariant ORMGET("Get OfflinePaymentClaim(s) information.")
//    quint32 ORMCREATE("Create a new OfflinePaymentClaim")
//    bool ORMUPDATE("Update OfflinePaymentClaim info")
//    bool ORMDELETE("Delete an OfflinePaymentClaim")
};

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
            ///ColName                                   Type                    Validation                          Default     UpBy   Sort  Filter Self  Virt   PK
                { Fields::ofpID,                ORM_PRIMARYKEY_64},
                { Fields::ofp_vchID,            S(quint64),             QFV.integer().minValue(1),          QRequired,  UPOwner},
                { Fields::ofpBank,              S(QString),             QFV.allwaysValid().maxLenght(50),   QRequired,  UPOwner},
                { Fields::ofpReceiptCode,       S(QString),             QFV.allwaysValid().maxLenght(50),   QRequired,  UPOwner},
                { Fields::ofpReceiptDate,       S(TAPI::DateTime_t),    QFV,                                QRequired,  UPOwner},
                { Fields::ofpAmount,            S(quint32),             QFV,                                QRequired,  UPOwner},
                { Fields::ofpTarget_walID,      S(quint64),             QFV,                                QNull,      UPNone },
                { Fields::ofpNotes,             S(QString),             QFV.allwaysValid().maxLenght(500),  QNull,      UPOwner},
                { Fields::ofpStatus,            ORM_STATUS_FIELD(Targoman::API::AccountModule::enuPaymentStatus, Targoman::API::AccountModule::enuPaymentStatus::Pending) },
                { Fields::ofpCreationDateTime,  ORM_CREATED_ON},
                { Fields::ofpCreatedBy_usrID,   ORM_CREATED_BY},
                { Fields::ofpUpdatedBy_usrID,   ORM_UPDATED_BY},
            };

        const QList<stuRelation> Relations = {
            ///Col                        Reference Table                  ForeignCol         Rename     LeftJoin
                { Fields::ofp_vchID,         R(AAASchema,tblVoucher::Name),   tblVoucher::Fields::vchID},
                ORM_RELATION_OF_CREATOR(Fields::ofpCreatedBy_usrID),
                ORM_RELATION_OF_UPDATER(Fields::ofpUpdatedBy_usrID),
            };

        const QList<stuDBIndex> Indexes = {
        };

    } //namespace Private

    TAPI_DEFINE_STRUCT(DTO,
        SF_ORM_PRIMARYKEY_64        (ofpID),
        SF_quint64                  (ofp_vchID),
        SF_QString                  (ofpBank),
        SF_QString                  (ofpReceiptCode),
//        SF_QString                  (ofpReceiptDate),
        SF_DateTime_t               (ofpReceiptDate),
        SF_quint32                  (ofpAmount),
        SF_NULLABLE_quint64         (ofpTarget_walID),
        SF_QString                  (ofpNotes),
        SF_ORM_STATUS_FIELD         (ofpStatus, Targoman::API::AccountModule::enuPaymentStatus, Targoman::API::AccountModule::enuPaymentStatus::Pending),
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
    TARGOMAN_DEFINE_API_SUBMODULE(Account, OfflinePayments)

private slots:
    QVariant ORMGET("Get OfflinePayment information.")
    bool ORMUPDATE("Update OfflinePayment info by priviledged user")
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
