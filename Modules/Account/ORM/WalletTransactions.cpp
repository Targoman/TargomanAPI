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

namespace Targoman {
namespace API {
namespace AAA {

using namespace ORM;

void WalletTransactions::init()
{
    WalletBalances::instance().init();
}

QVariant WalletTransactions::apiGET(GET_METHOD_ARGS_IMPL)
{
    if(Authorization::hasPriv(_JWT, this->privOn(EHTTP_GET,this->moduleName())) == false)
        this->setSelfFilters({{"wal_usrID", clsJWT(_JWT).usrID()}}, _EXTRAPATH, _ORMFILTERS, _filters);

    return this->selectFromTable(AAADACInstance(), {}, {}, GET_METHOD_CALL_ARGS);
}

WalletTransactions::WalletTransactions() :
    clsTable("AAA",
              "tblWalletsTransactions",
              { ///<ColName         Type                        Validation                          Default    RO   Sort  Filter Self  Virt   PK
                {"wltID",           S(quint32),                 QFV.integer().minValue(1),          ORM_PRIMARY_KEY},
                {"wlt_walID",       S(quint64),                 QFV.integer().minValue(1),          QInvalid,   true, true, true},
                {"wlt_invID",       S(quint64),                 QFV.integer().minValue(1),          QInvalid,   true, true, true},
                {"wltDateTime",     S(TAPI::DateTime_t),        QFV,                                QNull,      true},
                {"wltAmount",       S(qint64),                  QFV,                                QInvalid,   true,false,false},
                {"wltStatus",       S(TAPI::enuWalletTransactionStatus::Type), QFV,                 TAPI::enuWalletTransactionStatus::New},
              },
              { ///< Col       Reference Table             ForeignCol     Rename   LeftJoin
                {"wlt_walID", "AAA.tblUserWallets",         "walID"},
                {"wlt_invID", "AAA.tblInvoice",             "invID"},
                {"wltID",     "AAA.tblWalletBalances",      "wbl_wltID"},
              })
{
    TAPI_REGISTER_TARGOMAN_ENUM(TAPI::enuWalletTransactionStatus);
}

void WalletBalances::init()
{;}

WalletBalances::WalletBalances() :
    clsTable("AAA",
              "tblWalletBalances",
              { ///<ColName         Type         Validation                       Default    RO   Sort  Filter Self  Virt   PK
//              {"wbl_wltID",       S(quint64),  QFV.integer().minValue(1),       ORM_PRIMARY_KEY},
                {"wblBalance",      S(qint64),   QFV.allwaysInvalid(),            QInvalid, true,false,false},
              },
              {
              }
              )
{
}

}
}
}
