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

TAPI_REGISTER_TARGOMAN_ENUM(Targoman::API::TicketingModule, enuTicketStatus);

using namespace Targoman::API::TicketingModule;

//TAPI_REGISTER_METATYPE_TYPE_STRUCT(
//    /* namespace          */ Targoman::API::TicketingModule,
//    /* type               */ stuTicketScope
//);

namespace Targoman::API::TicketingModule::ORM {

/******************************************************************************\
|* Tickets ********************************************************************|
\******************************************************************************/
Tickets::Tickets() :
    intfSQLBasedModule(
        TicketingSchema,
        tblTickets::Name,
        tblTickets::Private::ORMFields,
        tblTickets::Private::Relations,
        tblTickets::Private::Indexes
) { ; }

QVariant Tickets::apiGET(
//    GET_METHOD_ARGS_IMPL_APICALL,
    APICALLBOOM_TYPE_JWT_IMPL &APICALLBOOM_PARAM,
    TAPI::PKsByPath_t _pksByPath,
    quint64 _pageIndex,
    quint16 _pageSize,
//        TAPI::Cols_t _cols,
//        TAPI::Filter_t _filters,
    TAPI::OrderBy_t _orderBy,
//        TAPI::GroupBy_t _groupBy,
    bool _reportCount,
    //-------------------------------------
//    const stuTicketScope &_ticketScope
    quint64 _baseTicketID,
    quint64 _inReplyTicketID
) {
    TAPI::Cols_t _cols;
    TAPI::Filter_t _filters;
    TAPI::GroupBy_t _groupBy;

    quint64 CurrentUserID = _APICALLBOOM.getUserID();
    clsCondition ExtraFilters = {};

    if (Authorization::hasPriv(_APICALLBOOM, this->privOn(EHTTP_GET, this->moduleBaseName())) == false)
        ExtraFilters
            .setCond({ tblTickets::Fields::tktTarget_usrID, enuConditionOperator::Equal, CurrentUserID })
            .orCond({ tblTickets::Fields::tktCreatedBy_usrID, enuConditionOperator::Equal, CurrentUserID })
            .orCond(
                clsCondition({ tblTickets::Fields::tktTarget_usrID, enuConditionOperator::Null })
                .andCond({ tblTickets::Fields::tktType, enuConditionOperator::Equal, enuTicketType::Broadcast })
            );

    auto fnTouchQuery = [&_baseTicketID, _inReplyTicketID](SelectQuery &_query) {
        _query
            .addCols({
                         tblTickets::Fields::tktID,
                         tblTickets::Fields::tktTarget_usrID,
                         tblTickets::Fields::tkt_svcID,
                         tblTickets::Fields::tkt_untID,
                         tblTickets::Fields::tktBase_tktID,
                         tblTickets::Fields::tktInReply_tktID,
                         tblTickets::Fields::tktType,
                         tblTickets::Fields::tktTitle,
                         tblTickets::Fields::tktBody,
                         tblTickets::Fields::tktStatus,
                         tblTickets::Fields::tktCreationDateTime,
                         tblTickets::Fields::tktCreatedBy_usrID,
                         tblTickets::Fields::tktUpdatedBy_usrID,
                         CURRENT_TIMESTAMP,
                     })
            .leftJoin(tblUnits::Name)
            .leftJoin(tblDepartments::Name, clsCondition(tblDepartments::Name, tblDepartments::Fields::depID, enuConditionOperator::Equal, tblUnits::Name, tblUnits::Fields::unt_depID))
            .addCols({
                         tblUnits::Fields::untName,
                         tblDepartments::Fields::depID,
                         tblDepartments::Fields::depName,
                     })
        ;

        if (_inReplyTicketID > 0)
            _query.andWhere({ tblTickets::Fields::tktInReply_tktID, enuConditionOperator::Equal, _inReplyTicketID });
        else {
            if (_baseTicketID == 0)
                _query.andWhere({ tblTickets::Fields::tktBase_tktID, enuConditionOperator::Null });
            else
                _query.andWhere({ tblTickets::Fields::tktBase_tktID, enuConditionOperator::Equal, _baseTicketID });
        }

        _query
            .leftJoin(SelectQuery(Tickets::instance())
                      .addCol(tblTickets::Fields::tktBase_tktID)
                      .addCol(enuAggregation::COUNT, tblTickets::Fields::tktID, "_cnt")
                      .addCol(enuAggregation::MAX, tblTickets::Fields::tktCreationDateTime, "_maxdate")
                      .where(clsCondition(tblTickets::Fields::tktBase_tktID, enuConditionOperator::NotNull))
                      .groupBy(tblTickets::Fields::tktBase_tktID),
                      "tmpReply",
                      clsCondition("tmpReply", tblTickets::Fields::tktBase_tktID, enuConditionOperator::Equal, tblTickets::Name, tblTickets::Fields::tktID)
            )
            .addCol("tmpReply._cnt", "TotalReplyCount")
            .addCol("tmpReply._maxdate", "LastReplyDateTime")
        ;
    };

    return this->Select(GET_METHOD_ARGS_CALL_INTERNAL_BOOM, ExtraFilters, 0, fnTouchQuery);
}

/******************************************************************************\
|* TicketRead *****************************************************************|
\******************************************************************************/
TicketRead::TicketRead() :
    intfSQLBasedModule(
        TicketingSchema,
        tblTicketRead::Name,
        tblTicketRead::Private::ORMFields,
        tblTicketRead::Private::Relations,
        tblTicketRead::Private::Indexes
) { ; }

} //namespace Targoman::API::TicketingModule::ORM
