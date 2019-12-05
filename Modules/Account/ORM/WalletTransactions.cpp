/******************************************************************************
#   TargomanAPI: REST API for Targoman
#
#   Copyright 2014-2019 by Targoman Intelligent Processing <http://tip.co.ir>
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
#include "Helpers/AAA/AAA.hpp"

namespace Targoman {
namespace API {
namespace AAA {
using namespace QHttp;

void WalletTransactions::init()
{
    WalletBalances::instance().init();
}

QVariant WalletTransactions::apiGET(GET_METHOD_ARGS_IMPL)
{
//    if(_EXTRAPATH.isEmpty())
//        Authorization::checkPriv(_JWT,{"Account:WalletTransactions:CRUD~0100"});

//    return this->selectFromTable(AAADACInstance(), {},
//                                 _EXTRAPATH.isEmpty() ? "" : QString("+wal_usrID=%1").arg(clsJWT(_JWT).usrID()),
//                                 GET_METHOD_CALL_ARGS);
}

WalletTransactions::WalletTransactions() :
    clsTable("AAA",
              "tblWalletsTransactions",
              { ///<ColName         Type                        Validation                          RO   Sort  Filter Self  Virt   PK
                {"wltID",           S(quint32),             QFV.integer().minValue(1),  ORM_PRIMARY_KEY},
                {"wlt_walID",       S(quint64),             QFV.integer().minValue(1),  true, true, true},
                {"wltDateTime",     S(QHttp::DateTime_t),   QFV,                        true},
                {"wltAmount",       S(qint64),              QFV,                        true,false,false},
                {"wltStatus",       S(Targoman::API::enuWalletTransactionStatus::Type)},
              },
              { ///< Col       Reference Table             ForeignCol     Rename   LeftJoin
                {"wlt_walID", "AAA.tblUserWallets",         "walID"},
                {"wltID",     "AAA.tblWalletsTransactions", "wbl_wltID"},
              })
{
    QHTTP_REGISTER_TARGOMAN_ENUM(Targoman::API::enuWalletTransactionStatus);

    this->registerMyRESTAPIs();
}

void WalletBalances::init()
{;}

WalletBalances::WalletBalances() :
    clsTable("AAA",
              "tblWalletBalances",
              { ///<ColName         Type         Validation            RO   Sort  Filter Self  Virt   PK
                {"wblBalance",      S(qint64),   QFV.allwaysInvalid(), true,false,false},
              },
              {
              }
              )
{
    this->registerMyRESTAPIs();
}

}
}
}
