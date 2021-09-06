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

#ifndef TARGOMAN_API_MODULES_ADVERT_ORM_PROPS_H
#define TARGOMAN_API_MODULES_ADVERT_ORM_PROPS_H

#include "Interfaces/ORM/clsTable.h"
#include "Interfaces/AAA/AAA.hpp"

namespace Targoman {
namespace API {
namespace AdvertModule {

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-variable"
namespace tblProps {
constexpr char Name[] = "tblProps";
TARGOMAN_CREATE_CONSTEXPR(prp_binID);
TARGOMAN_CREATE_CONSTEXPR(prp_locID);
TARGOMAN_CREATE_CONSTEXPR(prpOrder);
TARGOMAN_CREATE_CONSTEXPR(prpKeyword);
TARGOMAN_CREATE_CONSTEXPR(prpStartDate);
TARGOMAN_CREATE_CONSTEXPR(prpEndDate);
TARGOMAN_CREATE_CONSTEXPR(prpCreatedBy_usrID);
TARGOMAN_CREATE_CONSTEXPR(prpCreationDateTime);
TARGOMAN_CREATE_CONSTEXPR(prpUpdatedBy_usrID);
}
#pragma GCC diagnostic pop

class Props : public ORM::clsTable
{
    Q_OBJECT
private slots:
    QVariant ORMGET("Get Props information")
    bool ORMDELETE("Delete a Prop")
    bool ORMUPDATE("Update Prop info by priviledged user")
    quint64 ORMCREATE("Create a new Prop by priviledged user")

private:
    TARGOMAN_DEFINE_API_SUBMODULE(Advert,Props)
};

}
}
}
#endif // TARGOMAN_API_MODULES_ADVERT_ORM_PROPS_H
