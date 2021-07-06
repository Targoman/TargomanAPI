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
 */

#ifndef TARGOMAN_API_MODULES_ACCOUNT_ORM_IPSTATS_H
#define TARGOMAN_API_MODULES_ACCOUNT_ORM_IPSTATS_H

#include "Interfaces/ORM/clsTable.h"
#include "Interfaces/AAA/AAA.hpp"

namespace Targoman {
namespace API {
namespace AAA {

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-variable"
namespace tblIPStats {
constexpr char Name[] = "tblIPStats";
TARGOMAN_CREATE_CONSTEXPR(ips_ipbIP);
TARGOMAN_CREATE_CONSTEXPR(ipsTimeStamp);
TARGOMAN_CREATE_CONSTEXPR(ipsInsertionDate);
}
#pragma GCC diagnostic pop

class IPStats : public ORM::clsTable
{
    Q_OBJECT
private slots:
    QVariant ORMGET("Get IPStats information")

    private:
        TARGOMAN_DEFINE_API_SUBMODULE(Account,IPStats)
};

}
}
}

#endif // TARGOMAN_API_MODULES_ACCOUNT_ORM_IPSTATS_H
