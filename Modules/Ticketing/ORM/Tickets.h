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

#ifndef TARGOMAN_API_MODULES_TICKETING_ORM_TICKETS_H
#define TARGOMAN_API_MODULES_TICKETING_ORM_TICKETS_H

#include "Interfaces/ORM/clsTable.h"

namespace TAPI{
TARGOMAN_DEFINE_ENUM(enuTicketStatus,
                     New       = 'N',
                     Removed   = 'R'
                     )
}

namespace Targoman {
namespace API {
namespace ORM {

namespace tblTickets {
constexpr char Name[] = "tblTickets";
TARGOMAN_CREATE_CONSTEXPR(tktID);
TARGOMAN_CREATE_CONSTEXPR(tktCreationDateTime);
TARGOMAN_CREATE_CONSTEXPR(tktTarget_usrID);
TARGOMAN_CREATE_CONSTEXPR(tkt_svcID);
TARGOMAN_CREATE_CONSTEXPR(tktInReply_tktID);
TARGOMAN_CREATE_CONSTEXPR(tktBase_tktID);
TARGOMAN_CREATE_CONSTEXPR(tktType);
TARGOMAN_CREATE_CONSTEXPR(tktTitle);
TARGOMAN_CREATE_CONSTEXPR(tktBodyMarkdown);
TARGOMAN_CREATE_CONSTEXPR(tktHasAttachment);
TARGOMAN_CREATE_CONSTEXPR(tktCreatedBy_usrID);
TARGOMAN_CREATE_CONSTEXPR(tktUpdatedBy_usrID);
TARGOMAN_CREATE_CONSTEXPR(tktStatus);
}

namespace tblTicketRead {
constexpr char Name[] = "tblTicketRead";
TARGOMAN_CREATE_CONSTEXPR(tkr_tktID);
TARGOMAN_CREATE_CONSTEXPR(tkrBy_usrID);
TARGOMAN_CREATE_CONSTEXPR(tkrDateTime);
}

class Tickets : public ORM::clsTable
{
    Q_OBJECT
private slots:
    QVariant ORMGET("Get Tickets")


private:
    TARGOMAN_DEFINE_API_SUBMODULE(Ticketing, Tickets)
};

class TicketRead : public ORM::clsTable
{
    Q_OBJECT
private:
    TARGOMAN_DEFINE_API_SUBMODULE(Ticketing, TicketRead)
};

}
}
}

TAPI_DECLARE_METATYPE(TAPI::enuTicketStatus::Type);

#endif // TARGOMAN_API_MODULES_TICKETING_ORM_TICKETS_H
