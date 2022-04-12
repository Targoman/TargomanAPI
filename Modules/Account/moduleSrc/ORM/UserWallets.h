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
            APISession<true> &_SESSION,
            quint64 _walID
        ),
        "change default wallet"
    )

    bool REST_CREATE(
        transfer,
        (
            APISession<true> &_SESSION,
            const QString& _destLogin,
            quint32 _amount,
            const TAPI::MD5_t& _pass,
            const QString& _salt,
            quint64 _fromWalID = 0
        ),
        "Transfer money to other user's default wallet. Default wallet will be used if not specified"
    )
};

} //namespace ORM
} //namespace Targoman::API::AccountModule

TAPI_DECLARE_METATYPE_ENUM(Targoman::API::AccountModule, enuUserWalletStatus);

#endif // TARGOMAN_API_MODULES_ACCOUNT_ORM_USERWALLETS_H
