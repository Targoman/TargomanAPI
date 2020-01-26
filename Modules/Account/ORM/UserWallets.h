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

#ifndef TARGOMAN_API_MODULES_ACCOUNT_ORM_USERWALLETS_H
#define TARGOMAN_API_MODULES_ACCOUNT_ORM_USERWALLETS_H

#include "QHttp/intfRESTAPIHolder.h"
#include "libTargomanDBM/clsDAC.h"
#include "Helpers/ORM/clsTable.h"

namespace Targoman {
namespace API {
TARGOMAN_DEFINE_ENUM(enuUserWalletStatus,
                     Active = 'A',
                     Deactive = 'P',
                     Removed = 'R'
                     )
namespace AAA {

class UserWallets : public clsTable
{
    Q_OBJECT
public:
    void init();

private slots:
    QVariant ORMGET("Get UserWallets information")
    bool ORMDELETE("Delete a Wallet. Take note that default wallet can not be deleted")
    bool ORMUPDATE("Update Wallet info by priviledged user")
    quint64 ORMCREATE("Create a new Wallet")

    bool API(UPDATE,defaultWallet,(QHttp::JWT_t _JWT, quint64 _walID),
             "change default wallet")
    bool API(CREATE,transfer,(QHttp::JWT_t _JWT,
                              const QString& _destLogin,
                              quint32 _amount,
                              const QHttp::MD5_t& _pass,
                              const QString& _salt,
                              quint64 _walID = 0),
             "Transfer money to other user wallet. Default will be used if not defined")

    bool API(CREATE,deposit,(QHttp::JWT_t _JWT, quint32 _amount, quint64 _walID = 0),
             "Deposit money in the specified or default wallet")

private:
    UserWallets();
    TARGOMAN_DEFINE_SINGLETON_SUBMODULE(Account,UserWallets);
};

}
}
}

Q_DECLARE_METATYPE(Targoman::API::enuUserWalletStatus::Type);

#endif // TARGOMAN_API_MODULES_ACCOUNT_ORM_USERWALLETS_H
