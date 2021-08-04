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

#ifndef TARGOMAN_API_MODULES_ACCOUNT_ORM_VOUCHER_H
#define TARGOMAN_API_MODULES_ACCOUNT_ORM_VOUCHER_H

#include "Interfaces/ORM/clsTable.h"
#include "Interfaces/AAA/AAA.hpp"
#include "Classes/Defs.hpp"
#include "ORM/PaymentGateways.h"

//-----------------------------------------------------
namespace TAPI {
TARGOMAN_DEFINE_ENUM(enuVoucherType,
                     Withdrawal     = 'W',
                     Expense        = 'E',
                     Income         = 'I',
                     Credit         = 'C',
                     Prize          = 'Z',
                     TransferTo     = 'T',
                     TransferFrom   = 'F',
                     )
}
TAPI_DECLARE_METATYPE_ENUM(TAPI::enuVoucherType);

//-----------------------------------------------------
namespace Targoman::API::AAA {

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-variable"
namespace tblVoucher {
    constexpr char Name[] = "tblVoucher";
    TARGOMAN_CREATE_CONSTEXPR(vchID);
    TARGOMAN_CREATE_CONSTEXPR(vchCreationDateTime);
    TARGOMAN_CREATE_CONSTEXPR(vch_usrID);
    TARGOMAN_CREATE_CONSTEXPR(vchDesc);
    TARGOMAN_CREATE_CONSTEXPR(vchType);
    TARGOMAN_CREATE_CONSTEXPR(vchTotalAmount);
    TARGOMAN_CREATE_CONSTEXPR(vchStatus);
}
#pragma GCC diagnostic pop

class Voucher : public ORM::clsTable
{
    Q_OBJECT

private slots:
    QVariant ORMGET("Get Voucher information")
    bool ORMDELETE("Delete an Voucher. Take note that User can just delete Vouchers with Payoff type")

    TAPI::stuVoucher REST(
            CREATE,
            requestIncrease,
            (
                TAPI::JWT_t _JWT,
                quint32 _amount,
                TAPI::enuPaymentGatewayType::Type _gatewayType,
                QString _domain,
                quint64 _walletID = 0,
                QString _paymentVerifyCallback = {}
            ),
            "Increase wallet balance by online payment"
            "Set callbackURL = OFFLINE for offline payment, url for online payment"
            "Also set walletID >0 to use specified wallet or 0 for using default wallet"
            "When callback is set to URL you must specify payment gateway")

    quint64 REST(
            CREATE,
            requestWithdraw,
            (
                TAPI::JWT_t _JWT,
                quint64 _amount,
                quint64 _walID,
                const QString& _desc = {}
            ),
            "Create a new withdraw request by user.")

    quint64 REST(
            CREATE,
            requestWithdrawFor,
            (
                TAPI::JWT_t _JWT,
                quint64 _targetUsrID,
                quint64 _amount,
                TAPI::JSON_t _desc
            ),
            "Create a new withdraw request for another user by priviledged user. "
            "Description object must contain at least an string field named 'desc'")

private:
    TARGOMAN_DEFINE_API_SUBMODULE(Account, Voucher)
};

} //namespace Targoman::API::AAA

#endif // TARGOMAN_API_MODULES_ACCOUNT_ORM_VOUCHER_H
