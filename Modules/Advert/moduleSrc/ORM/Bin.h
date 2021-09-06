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

#ifndef TARGOMAN_API_MODULES_ADVERT_ORM_BIN_H
#define TARGOMAN_API_MODULES_ADVERT_ORM_BIN_H

#include "Interfaces/ORM/clsTable.h"
#include "Interfaces/AAA/AAA.hpp"

namespace Targoman {
namespace API {
namespace AdvertModule {

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-variable"
namespace tblBin {
constexpr char Name[] = "tblBin";
TARGOMAN_CREATE_CONSTEXPR(binID);
TARGOMAN_CREATE_CONSTEXPR(binType);
TARGOMAN_CREATE_CONSTEXPR(binTitle);
TARGOMAN_CREATE_CONSTEXPR(binDesc);
TARGOMAN_CREATE_CONSTEXPR(binPrettyURL);
TARGOMAN_CREATE_CONSTEXPR(binURL);
TARGOMAN_CREATE_CONSTEXPR(binShown);
TARGOMAN_CREATE_CONSTEXPR(binClicks);
TARGOMAN_CREATE_CONSTEXPR(binCreatedBy_usrID);
TARGOMAN_CREATE_CONSTEXPR(binCreationDateTime);
TARGOMAN_CREATE_CONSTEXPR(binUpdatedBy_usrID);
TARGOMAN_CREATE_CONSTEXPR(binStatus);
}

namespace tblBanners {
constexpr char Name[] = "tblBanners";
TARGOMAN_CREATE_CONSTEXPR(bnrID);
TARGOMAN_CREATE_CONSTEXPR(bnrImage);
TARGOMAN_CREATE_CONSTEXPR(bnrSize);
TARGOMAN_CREATE_CONSTEXPR(bnrUpdatedBy_usrID);
}
#pragma GCC diagnostic pop

class Bin : public ORM::clsTable
{
    Q_OBJECT
private slots:
    QVariant ORMGET("Get Bin information")

private:
    TARGOMAN_DEFINE_API_SUBMODULE(Advert,Bin)
};

class Banners : public ORM::clsTable
{
    Q_OBJECT
private:
    TARGOMAN_DEFINE_API_SUBMODULE(Advert,Banners)
};
}
}
}
#endif // TARGOMAN_API_MODULES_ADVERT_ORM_BIN_H
