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

#ifndef TARGOMAN_API_MODULES_ACCOUNT_ORM_SERVICES_H
#define TARGOMAN_API_MODULES_ACCOUNT_ORM_SERVICES_H

#include "Interfaces/ORM/clsTable.h"
#include "Interfaces/AAA/AAA.hpp"

using namespace Targoman::API::ORM;

namespace Targoman::API::AccountModule {

//structures and enumes goes here

namespace ORM {

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-variable"
namespace tblService {
constexpr char Name[] = "tblService";
//constexpr char Invalid_ProcessVoucherItemEndPoint[] = "INVALID";
TARGOMAN_CREATE_CONSTEXPR(svcID);
TARGOMAN_CREATE_CONSTEXPR(svcName);
TARGOMAN_CREATE_CONSTEXPR(svc_rolID);
TARGOMAN_CREATE_CONSTEXPR(svcProcessVoucherItemEndPoint);
TARGOMAN_CREATE_CONSTEXPR(svcCancelVoucherItemEndPoint);
TARGOMAN_CREATE_CONSTEXPR(svcStatus);
TARGOMAN_CREATE_CONSTEXPR(svcCreationDateTime);
TARGOMAN_CREATE_CONSTEXPR(svcCreatedBy_usrID);
TARGOMAN_CREATE_CONSTEXPR(svcUpdatedBy_usrID);
}
#pragma GCC diagnostic pop

class Service : public clsTable
{
    Q_OBJECT

private slots:
    QVariant ORMGET("Get Service information")
    quint64 ORMCREATE("Create a new Service by priviledged user")
    bool ORMUPDATE("Update Service info by priviledged user")
    bool ORMDELETE("Delete a Service")

private:
    TARGOMAN_DEFINE_API_SUBMODULE(Account, Service)
};

} //namespace ORM
} //namespace Targoman::API::AccountModule

#endif // TARGOMAN_API_MODULES_ACCOUNT_ORM_SERVICES_H
