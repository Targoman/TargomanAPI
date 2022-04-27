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

#ifndef TARGOMAN_API_MODULES_ACCOUNT_ORM_WALLETTRANSACTIONS_H
#define TARGOMAN_API_MODULES_ACCOUNT_ORM_WALLETTRANSACTIONS_H

#include "Interfaces/AAA/AAA.hpp"
#include "Interfaces/API/intfSQLBasedModule.h"

namespace Targoman::API::AccountModule {

//structures and enumes goes here

//what is this?
//TARGOMAN_DEFINE_ENUM(enuWalletTransactionStatus,
//                     New = 'A',
//                     Processed = 'P',
//                     Removed = 'R'
//                     )

TARGOMAN_DEFINE_ENUM(enuWalletTransactionStatus,
                     New      = 'N',
                     Pending  = 'P',
                     Payed    = 'Y',
                     Succeded = 'A',
                     Error    = 'E',
                     Removed  = 'R'
                     )

namespace ORM {

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-variable"
namespace tblWalletsTransactions {
    constexpr char Name[] = "tblWalletsTransactions";
    TARGOMAN_CREATE_CONSTEXPR(wltID);
    TARGOMAN_CREATE_CONSTEXPR(wlt_walID);
    TARGOMAN_CREATE_CONSTEXPR(wlt_vchID);
    TARGOMAN_CREATE_CONSTEXPR(wlt_vchType);
    TARGOMAN_CREATE_CONSTEXPR(wltAmount);
    TARGOMAN_CREATE_CONSTEXPR(wltStatus);
    TARGOMAN_CREATE_CONSTEXPR(wltDateTime);
}

namespace tblWalletBalances {
    constexpr char Name[] = "tblWalletBalances";
    TARGOMAN_CREATE_CONSTEXPR(wbl_wltID);
    TARGOMAN_CREATE_CONSTEXPR(wblBalance);
    TARGOMAN_CREATE_CONSTEXPR(wblSumIncome);
    TARGOMAN_CREATE_CONSTEXPR(wblSumExpense);
    TARGOMAN_CREATE_CONSTEXPR(wblSumCredit);
    TARGOMAN_CREATE_CONSTEXPR(wblSumDebit);
}
#pragma GCC diagnostic pop


class WalletTransactions : public intfSQLBasedModule
{
    Q_OBJECT
    TARGOMAN_DEFINE_API_SUBMODULE(Account, WalletTransactions)

private slots:
    QVariant ORMGET("Get WalletTransactions information.")
};

class WalletBalances : public intfSQLBasedModule
{
    Q_OBJECT
    TARGOMAN_DEFINE_API_SUBMODULE(Account, WalletBalances)

private:
};

} //namespace ORM
} //namespace Targoman::API::AccountModule

TAPI_DECLARE_METATYPE_ENUM(Targoman::API::AccountModule, enuWalletTransactionStatus);

#endif // TARGOMAN_API_MODULES_ACCOUNT_ORM_WALLETTRANSACTIONS_H
