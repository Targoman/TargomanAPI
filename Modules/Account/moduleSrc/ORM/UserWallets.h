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

#include "ORM/PaymentGateways.h"
#include "Interfaces/AAA/AAA.hpp"
#include "Interfaces/API/intfSQLBasedModule.h"

namespace Targoman::API::AccountModule {

//structures and enumes goes here

TARGOMAN_DEFINE_ENUM(enuUserWalletStatus,
                     Active   = 'A',
                     Deactive = 'P',
                     Removed  = 'R'
                     )

namespace ORM {

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-variable"
namespace tblUserWallets{
constexpr char Name[] = "tblUserWallets";
TARGOMAN_CREATE_CONSTEXPR(walID);
TARGOMAN_CREATE_CONSTEXPR(wal_usrID);
TARGOMAN_CREATE_CONSTEXPR(walName);
TARGOMAN_CREATE_CONSTEXPR(walDefault);
TARGOMAN_CREATE_CONSTEXPR(walMinBalance);
TARGOMAN_CREATE_CONSTEXPR(walNotTransferableAmount);
TARGOMAN_CREATE_CONSTEXPR(walMaxTransferPerDay);
TARGOMAN_CREATE_CONSTEXPR(walLastBalance);
TARGOMAN_CREATE_CONSTEXPR(walSumIncome);
TARGOMAN_CREATE_CONSTEXPR(walSumExpenses);
TARGOMAN_CREATE_CONSTEXPR(walSumCredit);
TARGOMAN_CREATE_CONSTEXPR(walSumDebit);
TARGOMAN_CREATE_CONSTEXPR(walCreatedBy_usrID);
TARGOMAN_CREATE_CONSTEXPR(walCreationDateTime);
TARGOMAN_CREATE_CONSTEXPR(walUpdatedBy_usrID);
TARGOMAN_CREATE_CONSTEXPR(walStatus);
}
#pragma GCC diagnostic pop

class UserWallets : public intfSQLBasedModule
{
    Q_OBJECT
    TARGOMAN_DEFINE_API_SUBMODULE(Account, UserWallets)

private slots:
    QVariant ORMGET("Get UserWallets information")
    quint64 ORMCREATE("Create a new Wallet")
    bool ORMUPDATE("Update Wallet info by priviledged user")
    bool ORMDELETE("Delete a Wallet. Take note that default wallet can not be deleted")

    bool REST_UPDATE(
        setAsDefault, //defaultWallet,
        (
            APICALLBOOM_TYPE_JWT_DECL &APICALLBOOM_PARAM,
            quint64 _walID
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

//    QVariantList REST_GET_OR_POST(
//        availableGatewayTypesForRequestIncrease,
//        (
//            APICALLBOOM_TYPE_JWT_DECL &APICALLBOOM_PARAM,
//            quint32 _amount,
//            QString _domain
//        ),
//        "get list of payment gateway types valid for request increase base on amount and domain"
//    )

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
            const QString _desc = {}
        ),
        "Create a new withdrawal request by user."
    )

    quint64 REST_POST(
        requestWithdrawalFor,
        (
            APICALLBOOM_TYPE_JWT_DECL &APICALLBOOM_PARAM,
            quint64 _amount,
            quint64 _targetUsrID,
            const QString _desc = {}
        ),
        "Create a new withdrawal request for another user by priviledged user. "
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

TAPI_DECLARE_METATYPE_ENUM(Targoman::API::AccountModule, enuUserWalletStatus);

#endif // TARGOMAN_API_MODULES_ACCOUNT_ORM_USERWALLETS_H
