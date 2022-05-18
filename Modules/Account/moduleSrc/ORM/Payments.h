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

    inline QStringList ColumnNames() {
        return {
            onpID,
            onpMD5,
            onp_vchID,
            onp_pgwID,
            onpTrackNumber,
            onpAmount,
            onpTarget_walID,
            onpResult,
            onpStatus,
            onpCreationDateTime,
            onpLastUpdateDateTime,
        };
    }

    TAPI_DEFINE_VARIANT_ENABLED_STRUCT(DTO,
        SF_quint64(onpID),
        SF_MD5_t(onpMD5),
        SF_quint64(onp_vchID),
        SF_quint32(onp_pgwID),
        SF_QString(onpTrackNumber),
        SF_quint32(onpAmount),
        SF_NULLABLE_quint64(onpTarget_walID),
        SF_QString(onpResult),
        SF_Enum(Targoman::API::AccountModule::enuPaymentStatus, onpStatus, Targoman::API::AccountModule::enuPaymentStatus::Pending),
        SF_QString(onpCreationDateTime),
        SF_QString(onpLastUpdateDateTime)
    );
}
#pragma GCC diagnostic pop

class OnlinePayments : public intfSQLBasedModule
{
    Q_OBJECT
    TARGOMAN_DEFINE_API_SUBMODULE(Account, OnlinePayments)

private slots:
    QVariant ORMGET("Get OnlinePayment information.")
};

/*****************************************************************\
|* OfflinePaymentClaims ******************************************|
\*****************************************************************/
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-variable"
namespace tblOfflinePaymentClaims {
    constexpr char Name[] = "tblOfflinePaymentClaims";

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

    inline QStringList ColumnNames() {
        return {
            ofpcID,
            ofpc_vchID,
            ofpcBank,
            ofpcReceiptCode,
            ofpcReceiptDate,
            ofpcAmount,
            ofpcTarget_walID,
            ofpcNotes,
            ofpcStatus,
            ofpcCreationDateTime,
            ofpcCreatedBy_usrID,
            ofpcUpdatedBy_usrID,
        };
    }

    TAPI_DEFINE_VARIANT_ENABLED_STRUCT(DTO,
        SF_quint64(ofpcID),
        SF_NULLABLE_quint64(ofpc_vchID),
        SF_QString(ofpcBank),
        SF_QString(ofpcReceiptCode),
        SF_QString(ofpcReceiptDate),
        SF_quint32(ofpcAmount),
        SF_NULLABLE_quint64(ofpcTarget_walID),
        SF_QString(ofpcNotes),
        SF_Enum(Targoman::API::AccountModule::enuPaymentStatus, ofpcStatus, Targoman::API::AccountModule::enuPaymentStatus::New),
        SF_QString(ofpcCreationDateTime),
        SF_quint64(ofpcCreatedBy_usrID),
        SF_quint64(ofpcUpdatedBy_usrID)
    );
//        SF_DateTime_t(ofpcReceiptDate),
//        SF_DateTime_t(ofpcCreationDateTime),
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

    inline QStringList ColumnNames() {
        return {
            ofpID,
            ofp_vchID,
            ofpBank,
            ofpReceiptCode,
            ofpReceiptDate,
            ofpAmount,
            ofpTarget_walID,
            ofpNotes,
            ofpStatus,
            ofpCreationDateTime,
            ofpCreatedBy_usrID,
            ofpUpdatedBy_usrID,
        };
    }

    TAPI_DEFINE_VARIANT_ENABLED_STRUCT(DTO,
        SF_quint64(ofpID),
        SF_quint64(ofp_vchID),
        SF_QString(ofpBank),
        SF_QString(ofpReceiptCode),
        SF_QString(ofpReceiptDate),
        SF_quint32(ofpAmount),
        SF_NULLABLE_quint64(ofpTarget_walID),
        SF_QString(ofpNotes),
        SF_Enum(Targoman::API::AccountModule::enuPaymentStatus, ofpStatus, Targoman::API::AccountModule::enuPaymentStatus::Pending),
        SF_QString(ofpCreationDateTime),
        SF_quint64(ofpCreatedBy_usrID),
        SF_quint64(ofpUpdatedBy_usrID)
    );
//        SF_DateTime_t(ofpReceiptDate),
//        SF_DateTime_t(ofpCreationDateTime),
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
