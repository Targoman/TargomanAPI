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

#ifndef TARGOMAN_API_MODULES_ACCOUNT_ORM_IPBIN_H
#define TARGOMAN_API_MODULES_ACCOUNT_ORM_IPBIN_H

#include "Interfaces/AAA/AAA.hpp"
#include "Interfaces/API/intfSQLBasedModule.h"

namespace Targoman::API::AccountModule {

//structures and enumes goes here

TARGOMAN_DEFINE_ENUM(enuIPBinStatus,
                     Active  = 'A',
                     Blocked = 'B',
                     Removed = 'R'
                               )

namespace ORM {

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-variable"
namespace tblIPBin {
constexpr char Name[] = "tblIPBin";
TARGOMAN_CREATE_CONSTEXPR(ipbIP);
TARGOMAN_CREATE_CONSTEXPR(ipbReadable);
TARGOMAN_CREATE_CONSTEXPR(ipbFirstAccess);
TARGOMAN_CREATE_CONSTEXPR(ipbAccessCount);
TARGOMAN_CREATE_CONSTEXPR(ipbLastAccess);
TARGOMAN_CREATE_CONSTEXPR(ipbBlockedBy_usrID);
TARGOMAN_CREATE_CONSTEXPR(ipbBlockingTime);
TARGOMAN_CREATE_CONSTEXPR(ipbStatus);
}
#pragma GCC diagnostic pop


class IPBin : public intfSQLBasedModule
{
    Q_OBJECT
    TARGOMAN_DEFINE_API_SUBMODULE(Account, IPBin)

private slots:
    QVariant ORMGET("Get IPBin information")
};

} //namespace ORM
} //namespace Targoman::API::AccountModule

TARGOMAN_DECLARE_METATYPE_ENUM(Targoman::API::AccountModule, enuIPBinStatus);

#endif // TARGOMAN_API_MODULES_ACCOUNT_ORM_IPBIN_H
