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

#ifndef TARGOMAN_API_MODULES_TICKETING_ORM_UNITS_H
#define TARGOMAN_API_MODULES_TICKETING_ORM_UNITS_H

#include "Interfaces/API/intfSQLBasedModule.h"
#include "Interfaces/AAA/AAA.hpp"
#include "Defs.hpp"
#include "Departments.h"

namespace Targoman::API::TicketingModule {

//structures and enumes goes here

namespace ORM {

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-variable"
namespace tblUnits {
    constexpr char Name[] = "tblUnits";

    namespace Fields {
        TARGOMAN_CREATE_CONSTEXPR(untID);
        TARGOMAN_CREATE_CONSTEXPR(unt_depID);
        TARGOMAN_CREATE_CONSTEXPR(untName);
        TARGOMAN_CREATE_CONSTEXPR(untCreationDateTime);
        TARGOMAN_CREATE_CONSTEXPR(untCreatedBy_usrID);
    }

    namespace Relation {
//        constexpr char AAA[] = "aaa";
    }

    namespace Private {
        const QList<clsORMField> ORMFields = {
            ///ColName                           Type                    Validation                  Default     UpBy   Sort  Filter Self  Virt   PK
                { Fields::untID,                  ORM_PRIMARYKEY_32 },
                { Fields::unt_depID,              S(quint32),             QFV.integer().minValue(1),  QRequired,  UPNone },
                { Fields::untName,                S(QString),             QFV,                        QRequired,  UPNone },
                { Fields::untCreationDateTime,    ORM_CREATED_ON },
                { Fields::untCreatedBy_usrID,     ORM_CREATED_BY },
            };

        const QList<stuRelation> Relations = {
            ///Col                   Reference Table                             ForeignCol              Rename      LeftJoin
                { Fields::unt_depID,  R(TicketingSchema, tblDepartments::Name),   tblDepartments::Fields::depID },
                ORM_RELATION_OF_CREATOR(Fields::untCreatedBy_usrID),
            };

        const QList<stuDBIndex> Indexes = {
        };

    } //namespace Private

    TAPI_DEFINE_STRUCT(DTO,
        SF_ORM_PRIMARYKEY_32        (untID),
        SF_quint32                  (unt_depID),
        SF_QString                  (untName),
        SF_ORM_CREATED_ON           (untCreationDateTime),
        SF_ORM_CREATED_BY           (untCreatedBy_usrID)
    );
}
#pragma GCC diagnostic pop

class Units : public intfSQLBasedModule
{
    Q_OBJECT
    TARGOMAN_API_SUBMODULE_DEFINE(Ticketing, Units)

private slots:
    QVariant ORMGET_ANONYMOUSE("Get units")
    quint32 ORMCREATE_USER("Create a new unit")
    bool ORMUPDATE_USER("Update unit")
    bool ORMDELETE_USER("Delete a unit")
};

} //namespace ORM
} //namespace Targoman::API::TicketingModule

#endif // TARGOMAN_API_MODULES_TICKETING_ORM_UNITS_H
