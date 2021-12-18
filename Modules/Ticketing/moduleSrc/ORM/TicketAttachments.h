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

#ifndef TARGOMAN_API_MODULES_TICKETING_ORM_TICKETATTACHMENTS_H
#define TARGOMAN_API_MODULES_TICKETING_ORM_TICKETATTACHMENTS_H

#include "Interfaces/API/intfSQLBasedModule.h"

namespace Targoman::API::TicketingModule {

//structures and enumes goes here

namespace ORM {

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-variable"
namespace tblTicketAttachments {
    constexpr char Name[] = "tblTicketAttachments";
    TARGOMAN_CREATE_CONSTEXPR(tatID);
    TARGOMAN_CREATE_CONSTEXPR(tat_tktID);
    TARGOMAN_CREATE_CONSTEXPR(tat_uplID);
    TARGOMAN_CREATE_CONSTEXPR(tatCreationDateTime);
    TARGOMAN_CREATE_CONSTEXPR(tatCreatedBy_usrID);
}
#pragma GCC diagnostic pop

class TicketAttachments : public intfSQLBasedModule
{
    Q_OBJECT
    TARGOMAN_DEFINE_API_SUBMODULE(Ticketing, TicketAttachments)

private slots:
    QVariant ORMGET("Get Ticket Attachments")
};

} //namespace ORM
} //namespace Targoman::API::TicketingModule

#endif // TARGOMAN_API_MODULES_TICKETING_ORM_TICKETATTACHMENTS_H
