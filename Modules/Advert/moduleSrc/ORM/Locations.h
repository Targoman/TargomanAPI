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

#ifndef TARGOMAN_API_MODULES_ADVERT_ORM_LOCATIONS_H
#define TARGOMAN_API_MODULES_ADVERT_ORM_LOCATIONS_H

#include "Interfaces/ORM/clsTable.h"
#include "Interfaces/AAA/AAA.hpp"

namespace Targoman {
namespace API {
namespace Advertisement {

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-variable"

namespace tblLocations {
constexpr char Name[] = "tblLocations";
TARGOMAN_CREATE_CONSTEXPR(locID);
TARGOMAN_CREATE_CONSTEXPR(locURL);
TARGOMAN_CREATE_CONSTEXPR(locPlaceCode);
TARGOMAN_CREATE_CONSTEXPR(locCreatedBy_usrID);
TARGOMAN_CREATE_CONSTEXPR(locCreationDateTime);
TARGOMAN_CREATE_CONSTEXPR(locUpdatedBy_usrID);
TARGOMAN_CREATE_CONSTEXPR(locStatus);
}

#pragma GCC diagnostic pop

class Locations : public ORM::clsTable
{
    Q_OBJECT
private slots:
    QVariant ORMGET("Get Locations information")
    quint32 ORMCREATE("Create a new Location by priviledged user")
    bool ORMUPDATE("Update Location info by priviledged user")
    bool ORMDELETE("Delete a Locations")

private:
    TARGOMAN_DEFINE_API_SUBMODULE(Advert,Locations)
};

}
}
}

#endif // TARGOMAN_API_MODULES_ADVERT_ORM_LOCATIONS_H
