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

#include "Tickets.h"
#include "Interfaces/AAA/AAA.hpp"
#include "Defs.hpp"

#include "Interfaces/ORM/APIQueryBuilders.h"

TAPI_REGISTER_TARGOMAN_ENUM(TAPI,enuTicketStatus);

namespace Targoman {
namespace API {

using namespace ORM;

QVariant Tickets::apiGET(GET_METHOD_ARGS_IMPL_APICALL)
{
//    QString ExtraFilters;
//    if (Authorization::hasPriv(_JWT, this->privOn(EHTTP_GET, this->moduleBaseName())) == false)
//        ExtraFilters = QString ("( %1=%2 | %3=%4 | ( %5=NULL + %7=%8 )")
//                       .arg(tblTickets::tktTarget_usrID).arg(clsJWT(_JWT).usrID())
//                       .arg(tblTickets::tktCreatedBy_usrID).arg(clsJWT(_JWT).usrID())
//                       .arg(tblTickets::tktTarget_usrID)
//                       .arg(tblTickets::tktType).arg((TAPI::enuTicketType::toStr(TAPI::enuTicketType::Broadcast)));

    clsCondition ExtraFilters = {};
    if (Authorization::hasPriv(_JWT, this->privOn(EHTTP_GET, this->moduleBaseName())) == false)
        ExtraFilters
            .setCond({ tblTickets::tktTarget_usrID, enuConditionOperator::Equal, clsJWT(_JWT).usrID() })
            .orCond({ tblTickets::tktCreatedBy_usrID, enuConditionOperator::Equal, clsJWT(_JWT).usrID() })
            .orCond(clsCondition({ tblTickets::tktTarget_usrID, enuConditionOperator::Null })
                .andCond({tblTickets::tktType, enuConditionOperator::Equal, TAPI::enuTicketType::toStr(TAPI::enuTicketType::Broadcast) })
            );

    return Targoman::API::Query::Select(*this, GET_METHOD_CALL_ARGS_INTERNAL_CALL, ExtraFilters);

//    if (Authorization::hasPriv(_JWT, this->privOn(EHTTP_GET, this->moduleBaseName())) == false)
//        query
//            .where({ tblTickets::tktTarget_usrID, enuConditionOperator::Equal, clsJWT(_JWT).usrID() })
//            .orWhere({ tblTickets::tktCreatedBy_usrID, enuConditionOperator::Equal, clsJWT(_JWT).usrID() })
//            .orWhere(//clsCondition::scope(
//                clsCondition(tblTickets::tktTarget_usrID, enuConditionOperator::Null)
//                .orCond({ tblTickets::tktType, enuConditionOperator::Equal, TAPI::enuTicketType::toStr(TAPI::enuTicketType::Broadcast) })
//            )//)
//        ;

//    return query.one();

    //    return this->selectFromTable({}, ExtraFilters, GET_METHOD_CALL_ARGS_APICALL);
}

Tickets::Tickets() :
    clsTable(
        TicketingSchema,
        tblTickets::Name,
        {///< ColName                          Type                    Validation                 Default    UpBy   Sort  Filter Self  Virt   PK
            { tblTickets::tktID,               ORM_PRIMARYKEY_64 },
            { tblTickets::tktTarget_usrID,     S(quint64),             QFV.integer().minValue(1), QNull,     UPNone },
            { tblTickets::tkt_svcID,           S(quint32),             QFV.integer().minValue(1), QNull,     UPNone },
            { tblTickets::tktInReply_tktID,    S(quint64),             QFV.integer().minValue(1), QNull,     UPNone },
            { tblTickets::tktBase_tktID,       S(quint64),             QFV.integer().minValue(1), QNull,     UPNone },
            { tblTickets::tktType,             S(TAPI::enuTicketType::Type), QFV,                 TAPI::enuTicketType::Message, UPNone },
            { tblTickets::tktTitle,            S(TAPI::JSON_t),        QFV,                       QRequired, UPNone, false, false },
            { tblTickets::tktBodyMarkdown,     S(QString),             QFV.allwaysValid(),        QRequired, UPNone, false, false },
            { tblTickets::tktHasAttachment,    S(TAPI::DateTime_t),    QFV,                       false,     UPNone },
            { tblTickets::tktStatus,           ORM_STATUS_FIELD(TAPI::enuTicketStatus, TAPI::enuTicketStatus::New) },
            { tblTickets::tktCreationDateTime, ORM_CREATED_ON },
            { tblTickets::tktCreatedBy_usrID,  ORM_CREATED_BY },
            { tblTickets::tktUpdatedBy_usrID,  ORM_UPDATED_BY },
        },
        {///< Col                           Reference Table                           ForeignCol                 Rename     LeftJoin
            { tblTickets::tktInReply_tktID, R(TicketingSchema,tblTickets::Name),      tblTickets::tktID,          "InReply_" , true },
            { tblTickets::tktTarget_usrID,  R(AAASchema,tblUser::Name),               tblUser::usrID,             "Target_"  , true },
            { tblTickets::tktID,            R(TicketingSchema,tblTicketRead::Name),   tblTicketRead::tkr_tktID,   "ReadInfo_", true },
            ORM_RELATION_OF_CREATOR(tblTickets::tktCreatedBy_usrID),
            ORM_RELATION_OF_UPDATER(tblTickets::tktUpdatedBy_usrID),
        }
    )
{
}

/******************************************************************************/
TicketRead::TicketRead() :
    clsTable(TicketingSchema,
              tblTicketRead::Name,
              { ///<ColName                     Type                   Validation                       Default    UpBy   Sort  Filter Self  Virt   PK
//              {tblTicketRead::tkr_tktID",     ORM_PRIMARYKEY_64},
                {tblTicketRead::tkrBy_usrID,    S(quint64),            QFV.integer().minValue(1),       QInvalid, UPNone,false,false},
                {tblTicketRead::tkrDateTime,    S(TAPI::DateTime_t),   QFV.allwaysInvalid(),            QInvalid, UPNone,false,false},
              },
              {
              }
              )
{
}

}
}
