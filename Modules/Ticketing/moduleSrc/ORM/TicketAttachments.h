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

#ifndef TARGOMAN_API_MODULES_TICKETING_ORM_TICKETATTACHMENTS_H
#define TARGOMAN_API_MODULES_TICKETING_ORM_TICKETATTACHMENTS_H

#include "Interfaces/API/intfSQLBasedModule.h"
#include "Defs.hpp"
#include "Tickets.h"
#include "Interfaces/ObjectStorage/ObjectStorageManager.h"
using namespace Targoman::API::ObjectStorage;

namespace Targoman::API::TicketingModule {

//structures and enumes goes here

namespace ORM {

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-variable"
namespace tblTicketAttachments {
    constexpr char Name[] = "tblTicketAttachments";

    namespace Fields {
        TARGOMAN_CREATE_CONSTEXPR(tatID);
        TARGOMAN_CREATE_CONSTEXPR(tat_tktID);
        TARGOMAN_CREATE_CONSTEXPR(tat_uplID);
        TARGOMAN_CREATE_CONSTEXPR(tatCreationDateTime);
        TARGOMAN_CREATE_CONSTEXPR(tatCreatedBy_usrID);
    }

    namespace Relation {
        constexpr char Tickets[] = "tickets";
    }

    namespace Private {
        const QList<clsORMField> ORMFields = {
            ///ColName                                       Type                    Validation                 Default      UpBy   Sort  Filter Self  Virt   PK
                { Fields::tatID,                  ORM_PRIMARYKEY_64 },
                { Fields::tat_tktID,              S(quint64),             QFV.integer().minValue(1), QRequired,   UPNone },
                { Fields::tat_uplID,              S(quint64),             QFV.integer().minValue(1), QRequired,   UPNone },
                { Fields::tatCreationDateTime,    ORM_CREATED_ON },
                { Fields::tatCreatedBy_usrID,     ORM_CREATED_BY },
            };

        const QList<stuRelation> Relations = {
            ///< Alias        Col                               Reference Table                            ForeignCol          Rename     LeftJoin
                { Relation::Tickets, { Fields::tat_tktID,  R(TicketingSchema, tblTickets::Name),      tblTickets::Fields::tktID } },
                { Fields::tat_uplID,  R(TicketingSchema, tblUploadFiles::Name),  tblUploadFiles::Fields::uflID },
                //this index is preventing querybuilder invalid column for filter error:
                { Fields::tat_uplID,  R(TicketingSchema, tblUploadQueue::Name),  tblUploadQueue::Fields::uqu_uflID },
                ORM_RELATION_OF_CREATOR(Fields::tatCreatedBy_usrID),
            };

        const QList<stuDBIndex> Indexes = {
        };

    } //namespace Private

    TAPI_DEFINE_STRUCT(DTO,
        SF_ORM_PRIMARYKEY_64        (tatID),
        SF_quint64                  (tat_tktID),
        SF_quint64                  (tat_uplID),
        SF_ORM_CREATED_ON           (tatCreationDateTime),
        SF_ORM_CREATED_BY           (tatCreatedBy_usrID)
    );
}
#pragma GCC diagnostic pop

class TicketAttachments : public intfSQLBasedModule
{
    Q_OBJECT
    TARGOMAN_API_SUBMODULE_DEFINE(Ticketing, TicketAttachments)

private slots:
    QVariant ORMGET_USER("Get Ticket Attachments")
};

} //namespace ORM
} //namespace Targoman::API::TicketingModule

#endif // TARGOMAN_API_MODULES_TICKETING_ORM_TICKETATTACHMENTS_H
