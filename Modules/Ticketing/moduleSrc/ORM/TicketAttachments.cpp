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

#include "TicketAttachments.h"
#include "Interfaces/AAA/AAA.hpp"
#include "Defs.hpp"
#include "Tickets.h"
//#include "Interfaces/ObjectStorage/ORM/ObjectStorage.h"
#include "Interfaces/ObjectStorage/ObjectStorageManager.h"
using namespace Targoman::API::ObjectStorage;
#include "Ticketing.h"

using namespace Targoman::API::ORM;

namespace Targoman::API::TicketingModule::ORM {

/******************************************************************************/
/******************************************************************************/
/******************************************************************************/
TicketAttachments::TicketAttachments() :
    intfSQLBasedModule(
        TicketingSchema,
        tblTicketAttachments::Name,
        {///< ColName                                       Type                    Validation                 Default      UpBy   Sort  Filter Self  Virt   PK
            { tblTicketAttachments::tatID,                  ORM_PRIMARYKEY_64 },
            { tblTicketAttachments::tat_tktID,              S(quint64),             QFV.integer().minValue(1), QRequired,   UPNone },
            { tblTicketAttachments::tat_uplID,              S(quint64),             QFV.integer().minValue(1), QRequired,   UPNone },
            { tblTicketAttachments::tatCreationDateTime,    ORM_CREATED_ON },
            { tblTicketAttachments::tatCreatedBy_usrID,     ORM_CREATED_BY },
        },
        {///< Alias        Col                               Reference Table                            ForeignCol          Rename     LeftJoin
            { "tickets", { tblTicketAttachments::tat_tktID,  R(TicketingSchema, tblTickets::Name),      tblTickets::tktID } },
                         { tblTicketAttachments::tat_uplID,  R(TicketingSchema, tblUploadFiles::Name),  tblUploadFiles::uflID },
                         //this index is preventing querybuilder invalid column for filter error:
                         { tblTicketAttachments::tat_uplID,  R(TicketingSchema, tblUploadQueue::Name),  tblUploadQueue::uqu_uflID },
                         ORM_RELATION_OF_CREATOR(tblTicketAttachments::tatCreatedBy_usrID),
        }
) { ; }

QVariant IMPL_ORMGET(TicketAttachments) {
//    QString ExtraFilters;
//    if (Authorization::hasPriv(_APICALLBOOM.getJWT(), this->privOn(EHTTP_GET, this->moduleBaseName())) == false)
//        ExtraFilters = QString ("( %1=%2 | %3=%4 | ( %5=NULL + %7=%8 )")
//                       .arg(tblTicketAttachments::tktTarget_usrID).arg(_APICALLBOOM.getUserID())
//                       .arg(tblTicketAttachments::tktCreatedBy_usrID).arg(_APICALLBOOM.getUserID())
//                       .arg(tblTicketAttachments::tktTarget_usrID)
//                       .arg(tblTicketAttachments::tktType).arg((Targoman::API::TicketingModule::enuTicketType::toStr(Targoman::API::TicketingModule::enuTicketType::Broadcast)));

    UploadQueue::instance().prepareFiltersList();

    clsCondition ExtraFilters = {};
    if (Authorization::hasPriv(_APICALLBOOM.getJWT(), this->privOn(EHTTP_GET, this->moduleBaseName())) == false)
        ExtraFilters
            .setCond({ tblTickets::tktTarget_usrID, enuConditionOperator::Equal, _APICALLBOOM.getUserID() })
            .orCond({ tblTickets::tktCreatedBy_usrID, enuConditionOperator::Equal, _APICALLBOOM.getUserID() })
            .orCond(
                clsCondition({ tblTickets::tktTarget_usrID, enuConditionOperator::Null })
                .andCond({ tblTickets::tktType,
                           enuConditionOperator::Equal,
                           Targoman::API::TicketingModule::enuTicketType::toStr(Targoman::API::TicketingModule::enuTicketType::Broadcast) })
            );

    auto QueryLambda = [](SelectQuery &_query) {
        _query
            .addCols({
                         tblTicketAttachments::tatID,
                         tblTicketAttachments::tat_tktID,
                         tblTicketAttachments::tat_uplID,
                         tblTicketAttachments::tatCreationDateTime,
                         tblTicketAttachments::tatCreatedBy_usrID,
                     })
            .innerJoinWith("tickets")
            .addCols({
                         tblTickets::tktID,
                         tblTickets::tktTarget_usrID,
                         tblTickets::tkt_svcID,
                         tblTickets::tktBase_tktID,
                         tblTickets::tktInReply_tktID,
                         tblTickets::tktType,
                         tblTickets::tktTitle,
                         tblTickets::tktBody,
                         tblTickets::tktStatus,
                         tblTickets::tktCreationDateTime,
                         tblTickets::tktCreatedBy_usrID,
                         tblTickets::tktUpdatedBy_usrID,
                     })
        ;

        ObjectStorageManager::applyGetFileUrlInQuery(
                    _query,
                    UploadFiles::instance(),
                    UploadQueue::instance(),
                    UploadGateways::instance(),
                    tblTicketAttachments::Name,
                    tblTicketAttachments::tat_uplID
                    );

    };

    return this->Select(*this, GET_METHOD_CALL_ARGS_INTERNAL_CALL, ExtraFilters, 0, QueryLambda);
}

} //namespace Targoman::API::TicketingModule::ORM
