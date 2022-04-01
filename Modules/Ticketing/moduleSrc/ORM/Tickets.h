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

#include "Interfaces/Common/APIArgHelperMacros.hpp"
#include "Interfaces/API/intfSQLBasedModule.h"

namespace Targoman::API::TicketingModule {

//structures and enumes goes here

TARGOMAN_DEFINE_ENUM(enuTicketStatus,
                     New       = 'N',
                     Removed   = 'R'
                     )

//TAPI_DEFINE_VARIANT_ENABLED_STRUCT(stuTicketScope,
//    QString, baseTicketID,    QString(), v.size(), v, v.toString(),
//    QString, inReplyTicketID, QString(), v.size(), v, v.toString()
//);

namespace ORM {

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-variable"
namespace tblTickets {
    constexpr char Name[] = "tblTickets";
    TARGOMAN_CREATE_CONSTEXPR(tktID);
    TARGOMAN_CREATE_CONSTEXPR(tktTarget_usrID);
    TARGOMAN_CREATE_CONSTEXPR(tkt_svcID);
    TARGOMAN_CREATE_CONSTEXPR(tkt_untID);
    TARGOMAN_CREATE_CONSTEXPR(tktBase_tktID);
    TARGOMAN_CREATE_CONSTEXPR(tktInReply_tktID);
    TARGOMAN_CREATE_CONSTEXPR(tktType);
    TARGOMAN_CREATE_CONSTEXPR(tktTitle);
    TARGOMAN_CREATE_CONSTEXPR(tktBody);
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
//    QVariant ORMGET("Get Tickets")
    QVariant REST_GET(
        ,
        (
//            GET_METHOD_ARGS_HEADER_APICALL,
            TAPI::JWT_t _JWT,
            TAPI::PKsByPath_t _pksByPath = {},
            quint64 _pageIndex = 0,
            quint16 _pageSize = 20,
//            TAPI::Cols_t _cols = {},
//            TAPI::Filter_t _filters = {},
            TAPI::OrderBy_t _orderBy = {},
//            TAPI::GroupBy_t _groupBy = {},
            bool _reportCount = true,
            //-------------------------------------
//            const Targoman::API::TicketingModule::stuTicketScope &_ticketScope = {}
            quint64 _baseTicketID = 0,
            quint64 _inReplyTicketID = 0
        ),
        "Get Tickets"
    )

};

class TicketRead : public intfSQLBasedModule
{
    Q_OBJECT
    TARGOMAN_DEFINE_API_SUBMODULE(Ticketing, TicketRead)
};

} //namespace ORM
} //namespace Targoman::API::TicketingModule

TAPI_DECLARE_METATYPE_ENUM(Targoman::API::TicketingModule, enuTicketStatus);
//TAPI_DECLARE_METATYPE(Targoman::API::TicketingModule::stuTicketScope)

#endif // TARGOMAN_API_MODULES_TICKETING_ORM_TICKETS_H
