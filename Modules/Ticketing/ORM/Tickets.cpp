/******************************************************************************
#   TargomanAPI: REST API for Targoman
#
#   Copyright 2014-2019 by Targoman Intelligent Processing <http://tip.co.ir>
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

#include "Tickets.h"
#include "Interfaces/AAA/AAA.hpp"
#include "Defs.hpp"

namespace Targoman {
namespace API {
using namespace ORM;
using namespace TAPI;

QVariant Tickets::apiGET(GET_METHOD_ARGS_IMPL)
{
    QString ExtraFilters;
    if(Authorization::hasPriv(_JWT, this->privOn(EHTTP_GET,this->moduleBaseName())) == false)
        ExtraFilters = QString ("( tktTarget_usrID=%1 | tktCreatedBy_usrID=%1 | ( tktTarget_usrID=NULL + tktType=%2 ) )").arg(clsJWT(_JWT).usrID()).arg(enuTicketType::toStr(enuTicketType::Broadcast));

    ExtraFilters = QString ("( tktTarget_usrID=%1 | tktCreatedBy_usrID=%1 | ( tktTarget_usrID=NULL + tktType=%2 ) )").arg(clsJWT(_JWT).usrID()).arg(enuTicketType::toStr(enuTicketType::Broadcast));
    return this->selectFromTable(AAADAC, {}, ExtraFilters, GET_METHOD_CALL_ARGS);
}

Tickets::Tickets() :
    clsTable("Ticketing",
              "tblTickets",
              { ///<ColName             Type                    Validation                   Default    RO   Sort  Filter Self  Virt   PK
                {"tktID",               S(quint64),             QFV.integer().minValue(1),   ORM_PRIMARY_KEY},
                {"tktCreationDateTime", S(QHttp::DateTime_t),   QFV,                         QNull,     true},
                {"tktTarget_usrID",     S(quint32),             QFV.integer().minValue(1),   QNull},
                {"tkt_svcID",           S(quint32),             QFV.integer().minValue(1),   QNull},
                {"tktInReply_tktID",    S(quint64),             QFV.integer().minValue(1),   QNull},
                {"tktType",             S(Targoman::API::enuTicketType::Type),   QFV,        Targoman::API::enuTicketType::Message},
                {"tktTitle",            S(QHttp::JSON_t),       QFV,                         QInvalid, false,false,false},
                {"tktBodyMarkdown",     S(QHttp::MD5_t),        QFV.allwaysInvalid(),        QInvalid, false,false,false},
                {"tktHasAttachment",    S(QHttp::DateTime_t),   QFV,                         false,    false},
                {"tktCreatedBy_usrID",  S(quint32),             QFV.integer().minValue(1),   QNull,     true},
                {"tktUpdatedBy_usrID",  S(quint32),             QFV.integer().minValue(1),   QNull},
                {"tktStatus",           S(Targoman::API::enuTicketStatus::Type), QFV,        Targoman::API::enuTicketStatus::New},
              },
              { ///< Col                Reference Table    ForeignCol   Rename     LeftJoin
                {"tktInReply_tktID",    "Ticketing.tblTickets", "tktID","InReply_" , true},
                {"tktTarget_usrID",     "AAA.tblUser",     "usrID",     "Target_"  , true},
                {"tktID",               "Ticketing.tblTicketRead",  "tkr_tktID", "ReadInfo_",  true},
                {"tktCreatedBy_usrID",  "AAA.tblUser",     "usrID",     "Creator_"},
                {"tktUpdatedBy_usrID",  "AAA.tblUser",     "usrID",     "Updater_",  true},
              })
{
    TAPI_REGISTER_TARGOMAN_ENUM(Targoman::API::enuTicketStatus);
}


/******************************************************************************/
TicketRead::TicketRead() :
    clsTable("Ticketing",
              "tblTicketRead",
              { ///<ColName      Type                   Validation                       Default    RO   Sort  Filter Self  Virt   PK
//              {"tkr_tktID",    S(quint64),            QFV.integer().minValue(1),       ORM_PRIMARY_KEY},
                {"tkrBy_usrID",  S(quint32),            QFV.integer().minValue(1),       QInvalid, true,false,false},
                {"tkrDateTime",  S(QHttp::DateTime_t),  QFV.allwaysInvalid(),            QInvalid, true,false,false},
              },
              {
              }
              )
{
}

}
}
