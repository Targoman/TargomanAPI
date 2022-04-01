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

#ifndef TARGOMAN_API_MODULES_TICKETING_ORM_UNITS_H
#define TARGOMAN_API_MODULES_TICKETING_ORM_UNITS_H

#include "Interfaces/API/intfSQLBasedModule.h"

namespace Targoman::API::TicketingModule {

//structures and enumes goes here

namespace ORM {

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-variable"
namespace tblUnits {
    constexpr char Name[] = "tblUnits";
    TARGOMAN_CREATE_CONSTEXPR(untID);
    TARGOMAN_CREATE_CONSTEXPR(unt_depID);
    TARGOMAN_CREATE_CONSTEXPR(untName);
    TARGOMAN_CREATE_CONSTEXPR(untCreationDateTime);
    TARGOMAN_CREATE_CONSTEXPR(untCreatedBy_usrID);
}
#pragma GCC diagnostic pop

class Units : public intfSQLBasedModule
{
    Q_OBJECT
    TARGOMAN_DEFINE_API_SUBMODULE(Ticketing, Units)

private slots:
    QVariant ORMGET("Get units")
    quint32 ORMCREATE("Create a new unit")
    bool ORMUPDATE("Update unit")
    bool ORMDELETE("Delete a unit")
};

} //namespace ORM
} //namespace Targoman::API::TicketingModule

#endif // TARGOMAN_API_MODULES_TICKETING_ORM_UNITS_H
