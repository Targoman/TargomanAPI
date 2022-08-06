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
    TAPI::Filter_t _filters,
    TAPI::OrderBy_t _orderBy,
//        TAPI::GroupBy_t _groupBy,
    bool _reportCount,
    //-------------------------------------
//    const stuTicketScope &_ticketScope
    NULLABLE_TYPE(quint64) _baseTicketID,
    NULLABLE_TYPE(quint64) _inReplyTicketID
) {
    TAPI::Cols_t _cols;
    TAPI::GroupBy_t _groupBy;

    quint64 CurrentUserID = APICALLBOOM_PARAM.getUserID();
    clsCondition ExtraFilters = {};

    if (Authorization::hasPriv(APICALLBOOM_PARAM, this->privOn(EHTTP_GET, this->moduleBaseName())) == false)
        ExtraFilters
            .setCond({ tblTickets::Fields::tktTarget_usrID, enuConditionOperator::Equal, CurrentUserID })
            .orCond({ tblTickets::Fields::tktCreatedBy_usrID, enuConditionOperator::Equal, CurrentUserID })
            .orCond(
                clsCondition({ tblTickets::Fields::tktTarget_usrID, enuConditionOperator::Null })
                .andCond({ tblTickets::Fields::tktType, enuConditionOperator::Equal, enuTicketType::Broadcast })
            );

    auto fnTouchQuery = [this, &APICALLBOOM_PARAM, &_baseTicketID, &_inReplyTicketID, &CurrentUserID](ORMSelectQuery &_query) {
        _query
            .addCols(this->selectableColumnNames())
            .leftJoin(tblUnits::Name)
            .leftJoin(tblDepartments::Name, clsCondition(tblDepartments::Name, tblDepartments::Fields::depID, enuConditionOperator::Equal, tblUnits::Name, tblUnits::Fields::unt_depID))
            .addCols({
                         tblUnits::Fields::untName,
                         tblDepartments::Fields::depID,
                         tblDepartments::Fields::depName,
                     })
        ;

        if (NULLABLE_HAS_VALUE(_inReplyTicketID))
            _query.andWhere({ tblTickets::Fields::tktInReply_tktID, enuConditionOperator::Equal, NULLABLE_VALUE(_inReplyTicketID) });
        else if (NULLABLE_HAS_VALUE(_baseTicketID)) {
            if (NULLABLE_VALUE(_baseTicketID) == 0)
                _query.andWhere({ tblTickets::Fields::tktBase_tktID, enuConditionOperator::Null });
            else
                _query.andWhere({ tblTickets::Fields::tktBase_tktID, enuConditionOperator::Equal, NULLABLE_VALUE(_baseTicketID) });
        }

        _query
            .nestedLeftJoin(Tickets::instance().makeSelectQuery(APICALLBOOM_PARAM)
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

        _query
            .leftJoin(tblTicketRead::Name,
                      clsCondition(tblTicketRead::Name, tblTicketRead::Fields::tkr_tktID, enuConditionOperator::Equal, tblTickets::Name, tblTickets::Fields::tktID)
                      .andCond({ tblTicketRead::Fields::tkrBy_usrID, enuConditionOperator::Equal, CurrentUserID })
                      )
            .addCols(TicketRead::instance().selectableColumnNames())
        ;

    };

    return this->Select(GET_METHOD_ARGS_CALL_VALUES, ExtraFilters, 0, fnTouchQuery);
}

QVariant IMPL_REST_UPDATE(Tickets, setAsRead, (
    APICALLBOOM_TYPE_JWT_IMPL &APICALLBOOM_PARAM,
    TAPI::PKsByPath_t _pksByPath
)) {
    quint64 CurrentUserID = APICALLBOOM_PARAM.getUserID();

    TicketRead::instance().prepareFiltersList();

    return TicketRead::instance().makeCreateQuery(APICALLBOOM_PARAM)
            .options_ignore()
            .addCols({
                        tblTicketRead::Fields::tkr_tktID,
                        tblTicketRead::Fields::tkrBy_usrID,
                    })
            .values({
                        { tblTicketRead::Fields::tkr_tktID, _pksByPath },
                        { tblTicketRead::Fields::tkrBy_usrID, CurrentUserID },
                    })
            .execute(CurrentUserID);
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
