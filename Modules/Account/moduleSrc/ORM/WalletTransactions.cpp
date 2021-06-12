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
 @author S. Mehran M. Ziabary <ziabary@targoman.com>
 */

#include "WalletTransactions.h"
#include "UserWallets.h"
#include "Voucher.h"
#include "Classes/PaymentLogic.h"

#include "Interfaces/ORM/QueryBuilders.h"
using namespace Targoman::API::ORM;

TAPI_REGISTER_TARGOMAN_ENUM(TAPI, enuWalletTransactionStatus);

namespace Targoman {
namespace API {
namespace AAA {

using namespace ORM;

QVariant WalletTransactions::apiGET(GET_METHOD_ARGS_IMPL)
{
    if(Authorization::hasPriv(_JWT, this->privOn(EHTTP_GET,this->moduleBaseName())) == false)
        this->setSelfFilters({{tblUserWallets::wal_usrID, clsJWT(_JWT).usrID()}}, _filters);

//    return this->selectFromTable({}, {}, GET_METHOD_CALL_ARGS);

    ApiSelectQuery query = ApiSelectQuery(*this, GET_METHOD_CALL_ARGS);

    return query.one();
}

WalletTransactions::WalletTransactions() :
    clsTable(AAASchema,
              tblWalletsTransactions::Name,
              { ///<ColName                             Type                        Validation                          Default    UpBy   Sort  Filter Self  Virt   PK
                {tblWalletsTransactions::wltID,         ORM_PRIMARY_KEY64},
                {tblWalletsTransactions::wlt_walID,     S(quint64),                 QFV.integer().minValue(1),          QRequired,   UPNone, true, true},
                {tblWalletsTransactions::wlt_vchID,     S(quint64),                 QFV.integer().minValue(1),          QRequired,   UPNone, true, true},
                {tblWalletsTransactions::wltDateTime,   ORM_CREATED_ON},
                {tblWalletsTransactions::wlt_vchType,   S(TAPI::enuVoucherType::Type), QFV,                             TAPI::enuVoucherType::Expense,UPNone},
                {tblWalletsTransactions::wltAmount,     S(qint64),                  QFV,                                QInvalid,   UPNone,false,false},
                {tblWalletsTransactions::wltStatus,     S(TAPI::enuWalletTransactionStatus::Type), QFV,                 TAPI::enuWalletTransactionStatus::New, UPStatus},
              },
              { ///< Col                            Reference Table                         ForeignCol     Rename   LeftJoin
                {tblWalletsTransactions::wlt_walID, R(AAASchema,tblUserWallets::Name),      tblUserWallets::walID},
                {tblWalletsTransactions::wlt_vchID, R(AAASchema,tblVoucher::Name),          tblVoucher::vchID},
                {tblWalletsTransactions::wltID,     R(AAASchema,tblWalletBalances::Name),   tblWalletBalances::wbl_wltID},
              })
{
}

WalletBalances::WalletBalances() :
    clsTable(AAASchema,
              tblWalletBalances::Name,
              { ///<ColName                         Type         Validation                       Default    UpBy   Sort  Filter Self  Virt   PK
//              {tblWalletBalances::wbl_wltID,      ORM_PRIMARY_KEY64},
                {tblWalletBalances::wblBalance,     S(qint64),   QFV.allwaysInvalid(),            QInvalid, UPNone,false,false},
                {tblWalletBalances::wblSumDebit,    S(qint64),   QFV.allwaysInvalid(),            QInvalid, UPNone,false,false},
                {tblWalletBalances::wblSumCredit,   S(qint64),   QFV.allwaysInvalid(),            QInvalid, UPNone,false,false},
                {tblWalletBalances::wblSumIncome,   S(qint64),   QFV.allwaysInvalid(),            QInvalid, UPNone,false,false},
                {tblWalletBalances::wblSumExpense,  S(qint64),   QFV.allwaysInvalid(),            QInvalid, UPNone,false,false},
              },
              {
              }
              )
{
}

}
}
}
