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
//#include "Interfaces/ORM/APIQueryBuilders.h"
#include "Units.h"
#include "Departments.h"

TAPI_REGISTER_TARGOMAN_ENUM(Targoman::API::TicketingModule, enuTicketStatus);

using namespace Targoman::API::TicketingModule;

//TAPI_REGISTER_METATYPE(
//    /* complexity         */ COMPLEXITY_Object,
//    /* namespace          */ Targoman::API::TicketingModule,
//    /* type               */ stuTicketScope,
//    /* toVariantLambda    */ [](const stuTicketScope& _value) -> QVariant { return _value.toJson().toVariantMap(); },
//    /* fromVariantLambda  */ [](const QVariant& _value, Q_DECL_UNUSED const QString& _paramName = "") -> stuTicketScope {
//        if (_value.isValid() == false)
//            return stuTicketScope();

//        if (_value.canConvert<QVariantMap>())
//            return stuTicketScope().fromJson(QJsonDocument::fromVariant(_value).object());

//        if (_value.toString().isEmpty())
//            return stuTicketScope();

//        QJsonParseError Error;
//        QJsonDocument Doc;
//        Doc = Doc.fromJson(_value.toString().toUtf8(), &Error);

//        if (Error.error != QJsonParseError::NoError)
//            throw exHTTPBadRequest(_paramName + " is not a valid stuTicketScope: <" + _value.toString() + ">" + Error.errorString());

//        if (Doc.isObject() == false)
//            throw exHTTPBadRequest(_paramName + " is not a valid stuTicketScope object: <" + _value.toString() + ">");

//        return stuTicketScope().fromJson(Doc.object());
//    }
//);

namespace Targoman::API::TicketingModule::ORM {

/******************************************************************************/
/******************************************************************************/
/******************************************************************************/
Tickets::Tickets() :
    intfSQLBasedModule(
        TicketingSchema,
        tblTickets::Name,
        {///< ColName                           Type                        Validation                  Default     UpBy   Sort  Filter Self  Virt   PK
            { tblTickets::tktID,                ORM_PRIMARYKEY_64 },
            { tblTickets::tktTarget_usrID,      S(NULLABLE_TYPE(quint64)),  QFV.integer().minValue(1),  QNull,      UPNone },
            { tblTickets::tkt_svcID,            S(NULLABLE_TYPE(quint32)),  QFV.integer().minValue(1),  QNull,      UPNone },
            { tblTickets::tkt_untID,            S(NULLABLE_TYPE(quint32)),  QFV.integer().minValue(1),  QNull,      UPNone },
            { tblTickets::tktBase_tktID,        S(NULLABLE_TYPE(quint64)),  QFV.integer().minValue(1),  QNull,      UPNone },
            { tblTickets::tktInReply_tktID,     S(NULLABLE_TYPE(quint64)),  QFV.integer().minValue(1),  QNull,      UPNone },
            { tblTickets::tktType,              S(Targoman::API::TicketingModule::enuTicketType::Type), QFV, Targoman::API::TicketingModule::enuTicketType::Message, UPNone },
            { tblTickets::tktTitle,             S(QString),                 QFV.allwaysValid(),         QRequired,  UPNone, false, false },
            { tblTickets::tktBody,              S(QString),                 QFV.allwaysValid(),         QRequired,  UPNone, false, false },
            { tblTickets::tktStatus,            ORM_STATUS_FIELD(Targoman::API::TicketingModule::enuTicketStatus, Targoman::API::TicketingModule::enuTicketStatus::New) },
            { tblTickets::tktCreationDateTime,  ORM_CREATED_ON },
            { tblTickets::tktCreatedBy_usrID,   ORM_CREATED_BY },
            { tblTickets::tktUpdatedBy_usrID,   ORM_UPDATED_BY },
        },
        {///< Col                           Reference Table                           ForeignCol                 Rename     LeftJoin
            { tblTickets::tktInReply_tktID, R(TicketingSchema,tblTickets::Name),      tblTickets::tktID,          "InReply_" , true },
            { tblTickets::tktTarget_usrID,  R(AAASchema,tblUser::Name),               tblUser::usrID,             "Target_"  , true },
            { tblTickets::tktID,            R(TicketingSchema,tblTicketRead::Name),   tblTicketRead::tkr_tktID,   "ReadInfo_", true },
            { tblTickets::tkt_untID,        R(TicketingSchema, tblUnits::Name),       tblUnits::untID },
            ORM_RELATION_OF_CREATOR(tblTickets::tktCreatedBy_usrID),
            ORM_RELATION_OF_UPDATER(tblTickets::tktUpdatedBy_usrID),
        }
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

    if (Authorization::hasPriv(_APICALLBOOM.getJWT(), this->privOn(EHTTP_GET, this->moduleBaseName())) == false)
        ExtraFilters
            .setCond({ tblTickets::tktTarget_usrID, enuConditionOperator::Equal, CurrentUserID })
            .orCond({ tblTickets::tktCreatedBy_usrID, enuConditionOperator::Equal, CurrentUserID })
            .orCond(
                clsCondition({ tblTickets::tktTarget_usrID, enuConditionOperator::Null })
                .andCond({ tblTickets::tktType, enuConditionOperator::Equal, enuTicketType::Broadcast })
            );

    auto QueryLambda = [&_baseTicketID, _inReplyTicketID](SelectQuery &_query) {
        _query
            .addCols({
                         tblTickets::tktID,
                         tblTickets::tktTarget_usrID,
                         tblTickets::tkt_svcID,
                         tblTickets::tkt_untID,
                         tblTickets::tktBase_tktID,
                         tblTickets::tktInReply_tktID,
                         tblTickets::tktType,
                         tblTickets::tktTitle,
                         tblTickets::tktBody,
                         tblTickets::tktStatus,
                         tblTickets::tktCreationDateTime,
                         tblTickets::tktCreatedBy_usrID,
                         tblTickets::tktUpdatedBy_usrID,
                         CURRENT_TIMESTAMP,
                     })
            .leftJoin(tblUnits::Name)
            .leftJoin(tblDepartments::Name, clsCondition(tblDepartments::Name, tblDepartments::depID, enuConditionOperator::Equal, tblUnits::Name, tblUnits::unt_depID))
            .addCols({
                         tblUnits::untName,
                         tblDepartments::depID,
                         tblDepartments::depName,
                     })
        ;

        if (_inReplyTicketID > 0)
            _query.andWhere({ tblTickets::tktInReply_tktID, enuConditionOperator::Equal, _inReplyTicketID });
        else {
            if (_baseTicketID == 0)
                _query.andWhere({ tblTickets::tktBase_tktID, enuConditionOperator::Null });
            else
                _query.andWhere({ tblTickets::tktBase_tktID, enuConditionOperator::Equal, _baseTicketID });
        }

        _query
            .leftJoin(SelectQuery(Tickets::instance())
                      .addCol(tblTickets::tktBase_tktID)
                      .addCol(enuAggregation::COUNT, tblTickets::tktID, "_cnt")
                      .addCol(enuAggregation::MAX, tblTickets::tktCreationDateTime, "_maxdate")
                      .where(clsCondition(tblTickets::tktBase_tktID, enuConditionOperator::NotNull))
                      .groupBy(tblTickets::tktBase_tktID),
                      "tmpReply",
                      clsCondition("tmpReply", tblTickets::tktBase_tktID, enuConditionOperator::Equal, tblTickets::Name, tblTickets::tktID)
            )
            .addCol("tmpReply._cnt", "TotalReplyCount")
            .addCol("tmpReply._maxdate", "LastReplyDateTime")
        ;
    };

    return this->Select(*this, GET_METHOD_CALL_ARGS_INTERNAL_CALL_BOOM, ExtraFilters, 0, QueryLambda);
}

/******************************************************************************/
/******************************************************************************/
/******************************************************************************/
TicketRead::TicketRead() :
    intfSQLBasedModule(
        TicketingSchema,
        tblTicketRead::Name,
        { ///<ColName                       Type                   Validation                       Default    UpBy   Sort  Filter Self  Virt   PK
//            {tblTicketRead::tkr_tktID,      ORM_PRIMARYKEY_64},
            {tblTicketRead::tkrBy_usrID,    S(quint64),            QFV.integer().minValue(1),       QInvalid, UPNone,false,false},
            {tblTicketRead::tkrDateTime,    S(TAPI::DateTime_t),   QFV.allwaysInvalid(),            QInvalid, UPNone,false,false},
        }
    ) { ; }

} //namespace Targoman::API::TicketingModule::ORM
