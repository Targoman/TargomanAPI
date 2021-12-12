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

#ifndef TARGOMAN_API_MODULES_TICKETING_ORM_TICKETS_H
#define TARGOMAN_API_MODULES_TICKETING_ORM_TICKETS_H

#include "Interfaces/API/intfSQLBasedModule.h"

namespace Targoman::API::TicketingModule {

//structures and enumes goes here

TARGOMAN_DEFINE_ENUM(enuTicketStatus,
                     New       = 'N',
                     Removed   = 'R'
                     )

namespace ORM {

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-variable"
namespace tblTickets {
    constexpr char Name[] = "tblTickets";
    TARGOMAN_CREATE_CONSTEXPR(tktID);
    TARGOMAN_CREATE_CONSTEXPR(tktTarget_usrID);
    TARGOMAN_CREATE_CONSTEXPR(tkt_svcID);
    TARGOMAN_CREATE_CONSTEXPR(tktInReply_tktID);
    TARGOMAN_CREATE_CONSTEXPR(tktBase_tktID);
    TARGOMAN_CREATE_CONSTEXPR(tktType);
    TARGOMAN_CREATE_CONSTEXPR(tktTitle);
    TARGOMAN_CREATE_CONSTEXPR(tktBodyMarkdown);
    TARGOMAN_CREATE_CONSTEXPR(tktHasAttachment);
    TARGOMAN_CREATE_CONSTEXPR(tktStatus);
    TARGOMAN_CREATE_CONSTEXPR(tktCreationDateTime);
    TARGOMAN_CREATE_CONSTEXPR(tktCreatedBy_usrID);
    TARGOMAN_CREATE_CONSTEXPR(tktUpdatedBy_usrID);
}

namespace tblTicketRead {
    constexpr char Name[] = "tblTicketRead";
    TARGOMAN_CREATE_CONSTEXPR(tkr_tktID);
    TARGOMAN_CREATE_CONSTEXPR(tkrBy_usrID);
    TARGOMAN_CREATE_CONSTEXPR(tkrDateTime);
}
#pragma GCC diagnostic pop

class Tickets : public intfSQLBasedModule
{
    Q_OBJECT
    TARGOMAN_DEFINE_API_SUBMODULE(Ticketing, Tickets)

private slots:
    QVariant ORMGET("Get Tickets")
};

class TicketRead : public intfSQLBasedModule
{
    Q_OBJECT
    TARGOMAN_DEFINE_API_SUBMODULE(Ticketing, TicketRead)
};

} //namespace ORM
} //namespace Targoman::API::TicketingModule

TAPI_DECLARE_METATYPE_ENUM(Targoman::API::TicketingModule, enuTicketStatus);

#endif // TARGOMAN_API_MODULES_TICKETING_ORM_TICKETS_H
