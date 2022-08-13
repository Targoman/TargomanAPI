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
#include "Interfaces/AAA/AAA.hpp"
#include "Defs.hpp"
#include "Units.h"
#include "Departments.h"

namespace Targoman::API::TicketingModule {

//structures and enumes goes here

TARGOMAN_DEFINE_ENUM(enuTicketStatus,
                     New       = 'N',
                     Removed   = 'R'
                     )

//TAPI_DEFINE_STRUCT(stuTicketScope,
//    QString, baseTicketID,    QString(), v.size(), v, v.toString(),
//    QString, inReplyTicketID, QString(), v.size(), v, v.toString()
//);

} //namespace Targoman::API::TicketingModule

TAPI_DECLARE_METATYPE_ENUM(Targoman::API::TicketingModule, enuTicketStatus);
//TAPI_DECLARE_METATYPE(Targoman::API::TicketingModule::stuTicketScope)

namespace Targoman::API::TicketingModule {
namespace ORM {

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-variable"

namespace tblTicketRead {
    constexpr char Name[] = "tblTicketRead";

    namespace Fields {
        TARGOMAN_CREATE_CONSTEXPR(tkr_tktID);
        TARGOMAN_CREATE_CONSTEXPR(tkrBy_usrID);
        TARGOMAN_CREATE_CONSTEXPR(tkrDateTime);
    }
}

namespace tblTickets {
    constexpr char Name[] = "tblTickets";

    namespace Fields {
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

    namespace Relation {
        constexpr char Base[] = "Base";
        constexpr char InReply[] = "InReply";
        constexpr char TicketRead[] = "TicketRead";
    }

    namespace Private {
        const QList<clsORMField> ORMFields = {
            //ColName                       Type                        Validation                  Default     UpBy   Sort  Filter Self  Virt   PK
            { Fields::tktID,                ORM_PRIMARYKEY_64 },
            { Fields::tktTarget_usrID,      S(NULLABLE_TYPE(quint64)),  QFV.integer().minValue(1),  QNull,      UPNone },
            { Fields::tkt_svcID,            S(NULLABLE_TYPE(quint32)),  QFV.integer().minValue(1),  QNull,      UPNone },
            { Fields::tkt_untID,            S(NULLABLE_TYPE(quint32)),  QFV.integer().minValue(1),  QNull,      UPNone },
            { Fields::tktBase_tktID,        S(NULLABLE_TYPE(quint64)),  QFV.integer().minValue(1),  QNull,      UPNone },
            { Fields::tktInReply_tktID,     S(NULLABLE_TYPE(quint64)),  QFV.integer().minValue(1),  QNull,      UPNone },
            { Fields::tktType,              S(Targoman::API::TicketingModule::enuTicketType::Type), QFV, Targoman::API::TicketingModule::enuTicketType::Message, UPNone },
            { Fields::tktTitle,             S(QString),                 QFV.allwaysValid(),         QRequired,  UPNone },
            { Fields::tktBody,              S(QString),                 QFV.allwaysValid(),         QRequired,  UPNone },
            { Fields::tktStatus,            ORM_STATUS_FIELD(Targoman::API::TicketingModule::enuTicketStatus, Targoman::API::TicketingModule::enuTicketStatus::New) },
            { Fields::tktCreationDateTime,  ORM_CREATED_ON },
            { Fields::tktCreatedBy_usrID,   ORM_CREATED_BY },
            { Fields::tktUpdatedBy_usrID,   ORM_UPDATED_BY },
        };

        const QList<stuRelation> Relations = {
            //Col                           Reference Table                             ForeignCol                          Rename          LeftJoin
            { Relation::Base, {
              Fields::tktBase_tktID,        R(TicketingSchema, tblTickets::Name),       Fields::tktID,                      "Base_",        true }
            },
            { Relation::InReply, {
              Fields::tktInReply_tktID,     R(TicketingSchema, tblTickets::Name),       Fields::tktID,                      "InReply_",     true }
            },
            { Fields::tktTarget_usrID,      R(AAASchema, tblUser::Name),                tblUser::Fields::usrID,             "Target_",      true },
            { Fields::tkt_untID,            R(TicketingSchema, tblUnits::Name),         tblUnits::Fields::untID },
            { Relation::TicketRead, {
              Fields::tktID,                R(TicketingSchema, tblTicketRead::Name),    tblTicketRead::Fields::tkr_tktID } //   "ReadInfo_",    true }
            },
            ORM_RELATION_OF_CREATOR(Fields::tktCreatedBy_usrID),
            ORM_RELATION_OF_UPDATER(Fields::tktUpdatedBy_usrID),
        };

        const QList<stuDBIndex> Indexes = {
        };

    } //namespace Private

    TAPI_DEFINE_STRUCT(DTO,
        SF_ORM_PRIMARYKEY_64        (tktID),
        SF_NULLABLE_quint64         (tktTarget_usrID),
        SF_NULLABLE_quint32         (tkt_svcID),
        SF_NULLABLE_quint32         (tkt_untID),
        SF_NULLABLE_quint64         (tktBase_tktID),
        SF_NULLABLE_quint64         (tktInReply_tktID),
        SF_Enum                     (tktType, Targoman::API::TicketingModule::enuTicketType, Targoman::API::TicketingModule::enuTicketType::Message),
        SF_QString                  (tktTitle),
        SF_QString                  (tktBody),
        SF_ORM_STATUS_FIELD         (tktStatus, Targoman::API::TicketingModule::enuTicketStatus, Targoman::API::TicketingModule::enuTicketStatus::New),
        SF_ORM_CREATED_ON           (tktCreationDateTime),
        SF_ORM_CREATED_BY           (tktCreatedBy_usrID),
        SF_ORM_UPDATED_BY           (tktUpdatedBy_usrID)
    );
}

namespace tblTicketRead {
    namespace Relation {
//        constexpr char AAA[] = "aaa";
    }

    namespace Private {
        const QList<clsORMField> ORMFields = {
            //<ColName              Type                    Validation                  Default     UpBy   Sort  Filter Self  Virt   PK
            { Fields::tkr_tktID,    S(quint64),             QFV.integer().minValue(1),  QRequired,  UPNone},
            { Fields::tkrBy_usrID,  S(quint64),             QFV.integer().minValue(1),  QRequired,  UPNone },
            { Fields::tkrDateTime,  S(TAPI::DateTime_t),    QFV,                        QNow,       UPNone },
        };

        const QList<stuRelation> Relations = {
        };

        const QList<stuDBIndex> Indexes = {
        };

    } //namespace Private

    TAPI_DEFINE_STRUCT(DTO,
        SF_ORM_PRIMARYKEY_64        (tkr_tktID),
        SF_quint64                  (tkrBy_usrID),
        SF_DateTime_t               (tkrDateTime)
    );
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
//            GET_METHOD_ARGS_DECL_APICALL,
            APICALLBOOM_TYPE_JWT_DECL &APICALLBOOM_PARAM,
            TAPI::PKsByPath_t _pksByPath = {},
            quint64 _pageIndex = 0,
            quint16 _pageSize = 20,
//            TAPI::Cols_t _cols = {},
            TAPI::Filter_t _filters = {},
            TAPI::OrderBy_t _orderBy = {},
//            TAPI::GroupBy_t _groupBy = {},
            bool _reportCount = true,
            //-------------------------------------
//            const Targoman::API::TicketingModule::stuTicketScope &_ticketScope = {}
            NULLABLE_TYPE(quint64) _baseTicketID = NULLABLE_NULL_VALUE,
            NULLABLE_TYPE(quint64) _inReplyTicketID = NULLABLE_NULL_VALUE
        ),
        "Get Tickets"
    )

    QVariant REST_UPDATE(
        setAsRead,
        (
            APICALLBOOM_TYPE_JWT_DECL &APICALLBOOM_PARAM,
            TAPI::PKsByPath_t _pksByPath
        ),
        "Mark a ticket as readed by current user"
    )

};

class TicketRead : public intfSQLBasedModule
{
    Q_OBJECT
    TARGOMAN_DEFINE_API_SUBMODULE(Ticketing, TicketRead)
};

} //namespace ORM
} //namespace Targoman::API::TicketingModule

#endif // TARGOMAN_API_MODULES_TICKETING_ORM_TICKETS_H
