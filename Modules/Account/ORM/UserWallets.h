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

private:
    UserWallets();
    TARGOMAN_DEFINE_SINGLETON_SUBMODULE(Account,UserWallets);
};

}
}
}

Q_DECLARE_METATYPE(Targoman::API::enuUserWalletStatus::Type);

#endif // TARGOMAN_API_MODULES_ACCOUNT_ORM_USERWALLETS_H
