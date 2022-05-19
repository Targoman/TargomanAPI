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

#include "WalletTransactions.h"
#include "UserWallets.h"
#include "Voucher.h"
#include "Payment/PaymentLogic.h"

//#include "Interfaces/ORM/APIQueryBuilders.h"

TAPI_REGISTER_TARGOMAN_ENUM(Targoman::API::AccountModule, enuWalletTransactionStatus);

namespace Targoman::API::AccountModule::ORM {

WalletTransactions::WalletTransactions() :
    intfSQLBasedModule(
        AAASchema,
        tblWalletsTransactions::Name,
        {///< ColName                               Type                            Validation                  Default     UpBy    Sort   Filter Self  Virt   PK
            { tblWalletsTransactions::wltID,        ORM_PRIMARYKEY_64 },
            { tblWalletsTransactions::wlt_walID,    S(quint64),                     QFV.integer().minValue(1),  QRequired,  UPNone, true,  true },
            { tblWalletsTransactions::wlt_vchID,    S(quint64),                     QFV.integer().minValue(1),  QRequired,  UPNone, true,  true },
            { tblWalletsTransactions::wlt_vchType,  S(Targoman::API::AccountModule::enuVoucherType::Type), QFV, Targoman::API::AccountModule::enuVoucherType::Expense, UPNone },
            { tblWalletsTransactions::wltAmount,    S(qint64),                      QFV,                        QInvalid,   UPNone, false, false },
            { tblWalletsTransactions::wltStatus,    ORM_STATUS_FIELD(Targoman::API::AccountModule::enuWalletTransactionStatus, Targoman::API::AccountModule::enuWalletTransactionStatus::New) },
            { tblWalletsTransactions::wltDateTime,  ORM_CREATED_ON },
        },
        {///< Col                                   Reference Table                         ForeignCol                      Rename  LeftJoin
            { tblWalletsTransactions::wlt_walID,    R(AAASchema, tblUserWallets::Name),     tblUserWallets::walID },
            { tblWalletsTransactions::wlt_vchID,    R(AAASchema, tblVoucher::Name),         tblVoucher::vchID },
            { tblWalletsTransactions::wltID,        R(AAASchema, tblWalletsBalanceHistory::Name),  tblWalletsBalanceHistory::wbl_wltID },
        }
    ) { ; }

QVariant IMPL_ORMGET(WalletTransactions) {
    if (Authorization::hasPriv(_APICALLBOOM.getJWT(), this->privOn(EHTTP_GET, this->moduleBaseName())) == false)
        this->setSelfFilters({{tblUserWallets::wal_usrID, _APICALLBOOM.getUserID()}}, _filters);

    auto QueryLambda = [](SelectQuery &_query) {
        _query.innerJoin(tblUserWallets::Name);
    };

    return this->Select(*this, GET_METHOD_CALL_ARGS_INTERNAL_CALL_BOOM, {}, 0, QueryLambda);
}

WalletsBalanceHistory::WalletsBalanceHistory() :
    intfSQLBasedModule(
        AAASchema,
        tblWalletsBalanceHistory::Name,
        {///< ColName                           Type        Validation              Default     UpBy    Sort    Filter Self  Virt   PK
            { tblWalletsBalanceHistory::wbl_wltID,     S(qint64),  QFV,                    QRequired,  UPNone },
            { tblWalletsBalanceHistory::wblBalance,    S(qint64),  QFV.allwaysInvalid(),   QInvalid,   UPNone, false,  false },
            { tblWalletsBalanceHistory::wblSumDebit,   S(qint64),  QFV.allwaysInvalid(),   QInvalid,   UPNone, false,  false },
            { tblWalletsBalanceHistory::wblSumCredit,  S(qint64),  QFV.allwaysInvalid(),   QInvalid,   UPNone, false,  false },
            { tblWalletsBalanceHistory::wblSumIncome,  S(qint64),  QFV.allwaysInvalid(),   QInvalid,   UPNone, false,  false },
            { tblWalletsBalanceHistory::wblSumExpense, S(qint64),  QFV.allwaysInvalid(),   QInvalid,   UPNone, false,  false },
        },
        {///< Col                               Reference Table                             ForeignCol                      Rename   LeftJoin
            { tblWalletsBalanceHistory::wbl_wltID,     R(AAASchema, tblWalletsTransactions::Name), tblWalletsTransactions::wltID },
        }
    ) { ; }

} //namespace Targoman::API::AccountModule::ORM
