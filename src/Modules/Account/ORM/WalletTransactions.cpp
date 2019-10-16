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

using namespace Targoman;
using namespace Targoman::API;
using namespace QHttp;

void WalletTransactions::init()
{
    WalletBalances::instance().init();
}

QVariant WalletTransactions::apiGET(GET_METHOD_ARGS_IMPL)
{
    Authorization::hasPriv(_JWT,{"Account:CRUD~0100"});

    return this->selectFromTable(AAADACInstance(), {}, {}, GET_METHOD_CALL_ARGS);
}

WalletTransactions::WalletTransactions() :
    intfTable("AAA",
              "tblWalletsTransactions",
              "wlt",
              { ///<ColName    Validation                   Sort  Filter AS  RO   PK
                {"wlt_walID",   QFV.integer().minValue(1),  true, true, "", true, true},
                {"wltID",       QFV.integer().minValue(1),  true, true, "", true, true},
                {"wltDateTime", QFV.dateTime(),             true, true, "", true},
                {"wltAmount",   QFV.allwaysInvalid(),      false,false, "", true},
                {"wltStatus",   QFV.matches(QRegularExpression(QString("^[%1]$").arg(enuWalletTransactionStatus::options().join("|"))))},
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
    intfTable("AAA",
              "tblWalletBalances",
              "wbl",
              {
                {"wblBalance", QFV.allwaysInvalid(), false, false, "", true},
              },
              {
              }
              )
{

}
