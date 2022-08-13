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

#ifndef TARGOMAN_API_MODULES_ADVERT_ORM_PROPS_H
#define TARGOMAN_API_MODULES_ADVERT_ORM_PROPS_H

#include "Interfaces/AAA/AAA.hpp"
#include "Interfaces/API/intfSQLBasedModule.h"
#include "Defs.hpp"
#include "Locations.h"
#include "Bin.h"

namespace Targoman::API::AdvertModule {

//structures and enumes goes here

namespace ORM {

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-variable"
namespace tblProps {
    constexpr char Name[] = "tblProps";

    namespace Fields {
        TARGOMAN_CREATE_CONSTEXPR(prp_binID);
        TARGOMAN_CREATE_CONSTEXPR(prp_locID);
        TARGOMAN_CREATE_CONSTEXPR(prpOrder);
        TARGOMAN_CREATE_CONSTEXPR(prpKeyword);
        TARGOMAN_CREATE_CONSTEXPR(prpStartDate);
        TARGOMAN_CREATE_CONSTEXPR(prpEndDate);
        TARGOMAN_CREATE_CONSTEXPR(prpCreatedBy_usrID);
        TARGOMAN_CREATE_CONSTEXPR(prpCreationDateTime);
        TARGOMAN_CREATE_CONSTEXPR(prpUpdatedBy_usrID);
    }

    namespace Relation {
//        constexpr char AAA[] = "aaa";
    }

    namespace Private {
        const QList<clsORMField> ORMFields = {
            ///<ColName                     Type                    Validation                        UpBy   Sort  Filter Self  Virt   PK
                { Fields::prp_binID,           ORM_PRIMARYKEY_32},
                { Fields::prp_locID,           ORM_PRIMARYKEY_32},
                { Fields::prpOrder,            S(Targoman::API::AdvertModule::enuAdvertOrder::Type), QFV,                       Targoman::API::AdvertModule::enuAdvertOrder::Normal,  UPOwner},
                { Fields::prpKeyword,          S(QString),             QFV.unicodeAlNum().maxLenght(50), QRequired,  UPOwner},
                { Fields::prpStartDate,        S(TAPI::DateTime_t),    QFV,                              QRequired,  UPOwner},
                { Fields::prpEndDate,          S(TAPI::DateTime_t),    QFV,                              QRequired,  UPOwner},
                { Fields::prpCreatedBy_usrID,  ORM_CREATED_BY},
                { Fields::prpCreationDateTime, ORM_CREATED_ON},
                { Fields::prpUpdatedBy_usrID,  ORM_UPDATED_BY},
            };

        const QList<stuRelation> Relations = {
            ///Col                        Reference Table                     ForeignCol   Rename     LeftJoin
                { Fields::prp_binID,           R(AdvertSchema, tblBin::Name),       tblBin::Fields::binID },
                { Fields::prp_locID,           R(AdvertSchema, tblLocations::Name), tblLocations::Fields::locID },
                ORM_RELATION_OF_CREATOR(Fields::prpCreatedBy_usrID),
                ORM_RELATION_OF_UPDATER(Fields::prpUpdatedBy_usrID),
            };

        const QList<stuDBIndex> Indexes = {
        };

    } //namespace Private

    TAPI_DEFINE_STRUCT(DTO,
        SF_ORM_PRIMARYKEY_32        (prp_binID),
        SF_ORM_PRIMARYKEY_32        (prp_locID),
        SF_Enum                     (prpOrder, Targoman::API::AdvertModule::enuAdvertOrder, Targoman::API::AdvertModule::enuAdvertOrder::Normal),
        SF_QString                  (prpKeyword),
        SF_DateTime_t               (prpStartDate),
        SF_DateTime_t               (prpEndDate),
        SF_ORM_CREATED_BY           (prpCreatedBy_usrID),
        SF_ORM_CREATED_ON           (prpCreationDateTime),
        SF_ORM_UPDATED_BY           (prpUpdatedBy_usrID)
    );
}
#pragma GCC diagnostic pop

class Props : public intfSQLBasedModule
{
    Q_OBJECT
    TARGOMAN_DEFINE_API_SUBMODULE(Advert,Props)

private slots:
    QVariant ORMGET("Get Props information")
    bool ORMDELETE("Delete a Prop")
    bool ORMUPDATE("Update Prop info by an authorized user")
    quint64 ORMCREATE("Create a new Prop by an authorized user")
};

} //namespace ORM
} //namespace Targoman::API::AdvertModule

#endif // TARGOMAN_API_MODULES_ADVERT_ORM_PROPS_H
