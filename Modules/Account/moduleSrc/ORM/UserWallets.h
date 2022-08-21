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

#ifndef TARGOMAN_API_MODULES_ACCOUNT_ORM_USERWALLETS_H
#define TARGOMAN_API_MODULES_ACCOUNT_ORM_USERWALLETS_H

#include "PaymentGateways.h"
#include "Interfaces/AAA/AAA.hpp"
#include "Interfaces/API/intfSQLBasedModule.h"

namespace Targoman::API::AccountModule {

//structures and enumes goes here

TARGOMAN_DEFINE_ENUM(enuUserWalletStatus,
                     Active   = 'A',
                     Deactive = 'P',
                     Removed  = 'R'
                     )
} //namespace Targoman::API::AccountModule

TAPI_DECLARE_METATYPE_ENUM(Targoman::API::AccountModule, enuUserWalletStatus);

namespace Targoman::API::AccountModule {
namespace ORM {

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-variable"
namespace tblUserWallets {
    constexpr char Name[] = "tblUserWallets";

    namespace Fields {
        TARGOMAN_CREATE_CONSTEXPR(walID);
        TARGOMAN_CREATE_CONSTEXPR(wal_usrID);
        TARGOMAN_CREATE_CONSTEXPR(walName);
        TARGOMAN_CREATE_CONSTEXPR(walDefault);
        TARGOMAN_CREATE_CONSTEXPR(walMinBalance);
        TARGOMAN_CREATE_CONSTEXPR(walMaxTransferPerDay);
        TARGOMAN_CREATE_CONSTEXPR(walNotTransferableAmount);
        TARGOMAN_CREATE_CONSTEXPR(walFreezedAmount);
        TARGOMAN_CREATE_CONSTEXPR(walBalance);
        TARGOMAN_CREATE_CONSTEXPR(walSumIncome);
        TARGOMAN_CREATE_CONSTEXPR(walSumExpense);
        TARGOMAN_CREATE_CONSTEXPR(walSumCredit);
        TARGOMAN_CREATE_CONSTEXPR(walSumDebit);
        TARGOMAN_CREATE_CONSTEXPR(walCreatedBy_usrID);
        TARGOMAN_CREATE_CONSTEXPR(walCreationDateTime);
        TARGOMAN_CREATE_CONSTEXPR(walUpdatedBy_usrID);
        TARGOMAN_CREATE_CONSTEXPR(walStatus);
    } //namespace Fields

    namespace Relation {
//        constexpr char AAA[] = "aaa";
    }

    namespace Private {
        const QList<clsORMField> ORMFields = {
            //ColName                           Type                        Validation                          Default     UpBy     Sort   Filter Self  Virt   PK
            { Fields::walID,                    ORM_PRIMARYKEY_64 },
            { Fields::wal_usrID,                S(quint64),                 QFV.integer().minValue(1),          QRequired,  UPNone },
            { Fields::walName,                  S(QString),                 QFV.unicodeAlNum().maxLenght(100),  "default",  UPOwner },
            { Fields::walDefault,               S(bool),                    QFV,                                false,      UPOwner },
            { Fields::walMinBalance,            S(quint64),                 QFV,                                0,          UPAdmin, false, false },
            { Fields::walMaxTransferPerDay,     S(quint64),                 QFV,                                10000000,   UPAdmin, false, false },
            { Fields::walNotTransferableAmount, S(quint64),                 QFV,                                0,          UPAdmin, false, false },
            { Fields::walFreezedAmount,         S(quint64),                 QFV,                                0,          UPAdmin, false, false },
            { Fields::walBalance,               S(qint64),                  QFV,                                QInvalid,   UPNone,  false, false },
            { Fields::walSumIncome,             S(quint64),                 QFV,                                QInvalid,   UPNone,  false, false },
            { Fields::walSumExpense,            S(quint64),                 QFV,                                QInvalid,   UPNone,  false, false },
            { Fields::walSumCredit,             S(quint64),                 QFV,                                QInvalid,   UPNone,  false, false },
            { Fields::walSumDebit,              S(quint64),                 QFV,                                QInvalid,   UPNone,  false, false },
            { Fields::walStatus,                ORM_STATUS_FIELD(Targoman::API::AccountModule::enuUserWalletStatus, Targoman::API::AccountModule::enuUserWalletStatus::Active) },
            { ORM_INVALIDATED_AT_FIELD },
            { Fields::walCreationDateTime,      ORM_CREATED_ON },
            { Fields::walCreatedBy_usrID,       ORM_CREATED_BY },
            { Fields::walUpdatedBy_usrID,       ORM_UPDATED_BY },
        };

        const QList<stuRelation> Relations = {
            //Col                   Reference Table                 ForeignCol     Rename   LeftJoin
            { Fields::wal_usrID,    R(AAASchema, tblUser::Name),    tblUser::Fields::usrID },
            ORM_RELATION_OF_CREATOR(Fields::walCreatedBy_usrID),
            ORM_RELATION_OF_UPDATER(Fields::walUpdatedBy_usrID),
        };

        const QList<stuDBIndex> Indexes = {
            { {
                Fields::wal_usrID,
                Fields::walName,
                ORM_INVALIDATED_AT_FIELD_NAME,
              }, enuDBIndex::Unique },
        };

    } //namespace Private

    TAPI_DEFINE_STRUCT(DTO,
        SF_ORM_PRIMARYKEY_64        (walID),
        SF_quint64                  (wal_usrID),
        SF_QString                  (walName),
        SF_bool                     (walDefault),
        SF_quint64                  (walMinBalance),
        SF_quint64                  (walMaxTransferPerDay),
        SF_quint64                  (walNotTransferableAmount),
        SF_quint64                  (walFreezedAmount),
        SF_qint64                   (walBalance),
        SF_quint64                  (walSumIncome),
        SF_quint64                  (walSumExpense),
        SF_quint64                  (walSumCredit),
        SF_quint64                  (walSumDebit),
        SF_ORM_STATUS_FIELD         (walStatus, Targoman::API::AccountModule::enuUserWalletStatus, Targoman::API::AccountModule::enuUserWalletStatus::Active),
        SF_ORM_CREATED_ON           (walCreationDateTime),
        SF_ORM_CREATED_BY           (walCreatedBy_usrID),
        SF_ORM_UPDATED_BY           (walUpdatedBy_usrID)
    );
}
#pragma GCC diagnostic pop

class UserWallets : public intfSQLBasedModule
{
    Q_OBJECT
    TARGOMAN_API_SUBMODULE_DEFINE(Account, UserWallets)

private slots:
    QVariant ORMGET("Get UserWallets information")
    quint64 ORMCREATE("Create a new Wallet")
    bool ORMUPDATE("Update Wallet info by an authorized user")
    bool ORMDELETE("Delete a Wallet. Take note that default wallet can not be deleted")

    bool REST_UPDATE(
        setAsDefault, //defaultWallet,
        (
            APICALLBOOM_TYPE_JWT_DECL &APICALLBOOM_PARAM,
            TAPI::PKsByPath_t _pksByPath
        ),
        "change default wallet"
    )

    bool REST_CREATE(
        transfer,
        (
            APICALLBOOM_TYPE_JWT_DECL &APICALLBOOM_PARAM,
            QString _destEmailOrMobile,
            quint32 _amount,
            TAPI::MD5_t _pass,
            QString _salt,
            quint64 _fromWalID = 0
        ),
        "Transfer money to other user's default wallet. Default wallet will be used if not specified"
    )

    Targoman::API::AAA::stuVoucher REST_CREATE(
        requestIncrease,
        (
            APICALLBOOM_TYPE_JWT_DECL &APICALLBOOM_PARAM,
            quint32 _amount,
            Targoman::API::AccountModule::enuPaymentGatewayType::Type _gatewayType,
            QString _domain,
            QString _paymentVerifyCallback,
            quint64 _walID = 0
        ),
        "Increase wallet balance by online payment"
        "Set callbackURL = OFFLINE for offline payment, url for online payment"
        "Also set walletID >0 to use specified wallet or 0 for using default wallet"
        "When callback is set to URL you must specify payment gateway"
    )

    quint64 REST_POST(
        requestWithdrawal,
        (
            APICALLBOOM_TYPE_JWT_DECL &APICALLBOOM_PARAM,
            quint64 _amount,
            quint64 _walID = 0,
            const QString &_desc = {}
        ),
        "Create a new withdrawal request by user."
    )

    quint64 REST_POST(
        requestWithdrawalFor,
        (
            APICALLBOOM_TYPE_JWT_DECL &APICALLBOOM_PARAM,
            quint64 _amount,
            quint64 _targetUsrID,
            const QString &_desc = {}
        ),
        "Create a new withdrawal request for another user by an authorized user. "
        "Description object must contain at least an string field named 'desc'"
    )

    bool REST_POST(
        acceptWithdrawal,
        (
            APICALLBOOM_TYPE_JWT_DECL &APICALLBOOM_PARAM,
            quint64 _voucherID
        ),
        "Make a withdrawal as accepted and finished"
    )

};

} //namespace ORM
} //namespace Targoman::API::AccountModule

#endif // TARGOMAN_API_MODULES_ACCOUNT_ORM_USERWALLETS_H
