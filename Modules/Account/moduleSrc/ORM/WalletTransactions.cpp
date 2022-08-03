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
#include "Payment/PaymentLogic.h"

TAPI_REGISTER_TARGOMAN_ENUM(Targoman::API::AccountModule, enuWalletTransactionStatus);

namespace Targoman::API::AccountModule::ORM {

/*
SELECT *
FROM tblWalletsTransactions
LEFT JOIN tblWalletsBalanceHistory
ON tblWalletsBalanceHistory.wbl_wltID = tblWalletsTransactions.wltID
LEFT JOIN tblVoucher
ON tblVoucher.vchID = tblWalletsTransactions.wlt_vchID
LEFT JOIN tblUserWallets
ON tblUserWallets.walID = tblWalletsTransactions.wlt_walID
ORDER BY wlt_walID, wltID -- , wltDateTime
-- wlt_walID, wltID
*/

/*****************************************************************\
|* WalletTransactions ********************************************|
\*****************************************************************/
WalletTransactions::WalletTransactions() :
    intfSQLBasedModule(
        AAASchema,
        tblWalletsTransactions::Name,
        tblWalletsTransactions::Private::ORMFields,
        tblWalletsTransactions::Private::Relations,
        tblWalletsTransactions::Private::Indexes
) { ; }

QVariant IMPL_ORMGET(WalletTransactions) {
    if (Authorization::hasPriv(APICALLBOOM_PARAM, this->privOn(EHTTP_GET, this->moduleBaseName())) == false)
        this->setSelfFilters({{tblUserWallets::Fields::wal_usrID, APICALLBOOM_PARAM.getUserID()}}, _filters);

    auto fnTouchQuery = [](ORMSelectQuery &_query) {
        _query.innerJoin(tblUserWallets::Name);
    };

    return this->Select(GET_METHOD_ARGS_CALL_VALUES, {}, 0, fnTouchQuery);
}

/*****************************************************************\
|* WalletsBalanceHistory *****************************************|
\*****************************************************************/
WalletsBalanceHistory::WalletsBalanceHistory() :
    intfSQLBasedModule(
        AAASchema,
        tblWalletsBalanceHistory::Name,
        tblWalletsBalanceHistory::Private::ORMFields,
        tblWalletsBalanceHistory::Private::Relations,
        tblWalletsBalanceHistory::Private::Indexes
) { ; }

} //namespace Targoman::API::AccountModule::ORM
