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

#ifndef TARGOMAN_API_MODULES_TICKETING_ORM_DEPARTMENTS_H
#define TARGOMAN_API_MODULES_TICKETING_ORM_DEPARTMENTS_H

#include "Interfaces/API/intfSQLBasedModule.h"
#include "Interfaces/AAA/AAA.hpp"
#include "Defs.hpp"

namespace Targoman::API::TicketingModule {

//structures and enumes goes here

namespace ORM {

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-variable"
namespace tblDepartments {
    constexpr char Name[] = "tblDepartments";

    namespace Fields {
        TARGOMAN_CREATE_CONSTEXPR(depID);
        TARGOMAN_CREATE_CONSTEXPR(depName);
        TARGOMAN_CREATE_CONSTEXPR(depCreationDateTime);
        TARGOMAN_CREATE_CONSTEXPR(depCreatedBy_usrID);
    }

    namespace Relation {
//        constexpr char AAA[] = "aaa";
    }

    namespace Private {
        const QList<clsORMField> ORMFields = {
            ///ColName                               Type                    Validation      Default     UpBy   Sort  Filter Self  Virt   PK
                { Fields::depID,                ORM_PRIMARYKEY_32 },
                { Fields::depName,              S(QString),             QFV,            QRequired,  UPNone },
                { Fields::depCreationDateTime,  ORM_CREATED_ON },
                { Fields::depCreatedBy_usrID,   ORM_CREATED_BY },
            };

        const QList<stuRelation> Relations = {
            ///Col                                   Reference Table         ForeignCol      Rename      LeftJoin
                ORM_RELATION_OF_CREATOR(Fields::depCreatedBy_usrID),
            };

        const QList<stuDBIndex> Indexes = {
        };

    } //namespace Private

    TAPI_DEFINE_STRUCT(DTO,
        SF_ORM_PRIMARYKEY_32        (depID),
        SF_QString                  (depName),
        SF_ORM_CREATED_ON           (depCreationDateTime),
        SF_ORM_CREATED_BY           (depCreatedBy_usrID)
    );
}
#pragma GCC diagnostic pop

class Departments : public intfSQLBasedModule
{
    Q_OBJECT
    TARGOMAN_API_SUBMODULE_DEFINE(Ticketing, Departments)

private slots:
    QVariant ANONYMOUSE_ORMGET("Get departments")
    quint32 ORMCREATE("Create a new department")
    bool ORMUPDATE("Update department")
    bool ORMDELETE("Delete a department")
};

} //namespace ORM
} //namespace Targoman::API::TicketingModule

#endif // TARGOMAN_API_MODULES_TICKETING_ORM_DEPARTMENTS_H
