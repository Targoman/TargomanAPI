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
#include "../Ticketing.h"
#include "Interfaces/AAA/AAA.hpp"
//#include "Interfaces/ObjectStorage/ORM/ObjectStorage.h"

//using namespace Targoman::API::ORM;

namespace Targoman::API::TicketingModule::ORM {

TARGOMAN_API_SUBMODULE_IMPLEMENT(Ticketing, TicketAttachments)

TicketAttachments::TicketAttachments() :
    intfSQLBasedModule(
        TicketingSchema,
        tblTicketAttachments::Name,
        tblTicketAttachments::Private::ORMFields,
        tblTicketAttachments::Private::Relations,
        tblTicketAttachments::Private::Indexes
) { ; }

QVariant IMPL_ORMGET_USER(TicketAttachments) {
//    QString ExtraFilters;
//    if (Authorization::hasPriv(APICALLBOOM_PARAM, this->privOn(EHTTP_GET, this->moduleBaseName())) == false)
//        ExtraFilters = QString ("( %1=%2 | %3=%4 | ( %5=NULL + %7=%8 )")
//                       .arg(tblTicketAttachments::Fields::tktTarget_usrID).arg(APICALLBOOM_PARAM.getActorID())
//                       .arg(tblTicketAttachments::Fields::tktCreatedBy_usrID).arg(APICALLBOOM_PARAM.getActorID())
//                       .arg(tblTicketAttachments::Fields::tktTarget_usrID)
//                       .arg(tblTicketAttachments::Fields::tktType).arg((Targoman::API::TicketingModule::enuTicketType::toStr(Targoman::API::TicketingModule::enuTicketType::Broadcast)));

    UploadQueue::instance().prepareFiltersList();

    clsCondition ExtraFilters = {};
    if (Authorization::hasPriv(APICALLBOOM_PARAM, this->privOn(EHTTP_GET, this->moduleBaseName())) == false)
        ExtraFilters
            .setCond({ tblTickets::Fields::tktTarget_usrID, enuConditionOperator::Equal, APICALLBOOM_PARAM.getActorID() })
            .orCond({ tblTickets::Fields::tktCreatedBy_usrID, enuConditionOperator::Equal, APICALLBOOM_PARAM.getActorID() })
            .orCond(
                clsCondition({ tblTickets::Fields::tktTarget_usrID, enuConditionOperator::Null })
                .andCond({ tblTickets::Fields::tktType,
                           enuConditionOperator::Equal,
                           Targoman::API::TicketingModule::enuTicketType::toStr(Targoman::API::TicketingModule::enuTicketType::Broadcast) })
            );

    auto fnTouchQuery = [&APICALLBOOM_PARAM](ORMSelectQuery &_query) {
        _query
            .addCols({
                         tblTicketAttachments::Fields::tatID,
                         tblTicketAttachments::Fields::tat_tktID,
                         tblTicketAttachments::Fields::tat_uplID,
                         tblTicketAttachments::Fields::tatCreationDateTime,
                         tblTicketAttachments::Fields::tatCreatedBy_usrID,
                     })
            .innerJoinWith("tickets")
            .addCols({
                         tblTickets::Fields::tktID,
                         tblTickets::Fields::tktTarget_usrID,
                         tblTickets::Fields::tkt_svcID,
                         tblTickets::Fields::tktBase_tktID,
                         tblTickets::Fields::tktInReply_tktID,
                         tblTickets::Fields::tktType,
                         tblTickets::Fields::tktTitle,
                         tblTickets::Fields::tktBody,
                         tblTickets::Fields::tktStatus,
                         tblTickets::Fields::tktCreationDateTime,
                         tblTickets::Fields::tktCreatedBy_usrID,
                         tblTickets::Fields::tktUpdatedBy_usrID,
                     })
        ;

        ObjectStorageManager::applyGetFileUrlInQuery(
                    APICALLBOOM_PARAM,
                    _query,
                    UploadFiles::instance(),
                    UploadGateways::instance(),
                    UploadQueue::instance(),
                    tblTicketAttachments::Name,
                    tblTicketAttachments::Fields::tat_uplID
                    );

    };

    return this->Select(GET_METHOD_ARGS_CALL_VALUES, ExtraFilters, 0, fnTouchQuery);
}

} //namespace Targoman::API::TicketingModule::ORM
