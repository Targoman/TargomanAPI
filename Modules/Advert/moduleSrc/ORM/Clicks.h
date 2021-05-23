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

#ifndef TARGOMAN_API_MODULES_ADVERT_ORM_CLICKS_H
#define TARGOMAN_API_MODULES_ADVERT_ORM_CLICKS_H

#include "Interfaces/ORM/clsTable.h"
#include "Interfaces/AAA/AAA.hpp"

namespace Targoman {
namespace API {
namespace Advertisement {

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-variable"
namespace tblClicks {
constexpr char Name[] = "tblClicks";
TARGOMAN_CREATE_CONSTEXPR(clkID);
TARGOMAN_CREATE_CONSTEXPR(clk_binID);
TARGOMAN_CREATE_CONSTEXPR(clk_locID);
TARGOMAN_CREATE_CONSTEXPR(clkDateTime);
TARGOMAN_CREATE_CONSTEXPR(clkIP);
TARGOMAN_CREATE_CONSTEXPR(clkDevice);
TARGOMAN_CREATE_CONSTEXPR(clkScreenSize);
TARGOMAN_CREATE_CONSTEXPR(clkOS);
TARGOMAN_CREATE_CONSTEXPR(clkBrowser);
}
#pragma GCC diagnostic pop

class Clicks : public ORM::clsTable
{
    Q_OBJECT
private slots:
    QVariant ORMGET("Get Clicks information")

private:
    TARGOMAN_DEFINE_API_SUBMODULE(Advert,Clicks)
};

}
}
}
#endif // TARGOMAN_API_MODULES_ADVERT_ORM_CLICKS_H
