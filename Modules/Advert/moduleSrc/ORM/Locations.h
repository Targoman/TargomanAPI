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

#ifndef TARGOMAN_API_MODULES_ADVERT_ORM_LOCATIONS_H
#define TARGOMAN_API_MODULES_ADVERT_ORM_LOCATIONS_H

#include "Interfaces/AAA/AAA.hpp"
#include "Interfaces/API/intfSQLBasedModule.h"

namespace Targoman::API::AdvertModule {

//structures and enumes goes here

namespace ORM {

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-variable"

namespace tblLocations {
    constexpr char Name[] = "tblLocations";

    namespace Fields {
        TARGOMAN_CREATE_CONSTEXPR(locID);
        TARGOMAN_CREATE_CONSTEXPR(locURL);
        TARGOMAN_CREATE_CONSTEXPR(locPlaceCode);
        TARGOMAN_CREATE_CONSTEXPR(locCreatedBy_usrID);
        TARGOMAN_CREATE_CONSTEXPR(locCreationDateTime);
        TARGOMAN_CREATE_CONSTEXPR(locUpdatedBy_usrID);
        TARGOMAN_CREATE_CONSTEXPR(locStatus);
    }

    namespace Relation {
//        constexpr char AAA[] = "aaa";
    }

    namespace Private {
        const QList<clsORMField> ORMFields = {
            ///ColName                            Type                 Validation                      Default    UpBy   Sort  Filter Self  Virt   PK
                { Fields::locID,               ORM_PRIMARYKEY_32 },
                { Fields::locURL,              S(TAPI::URL_t),      QFV/*.integer().minValue(1)*/,  QRequired, UPAdmin },
                { Fields::locPlaceCode,        S(TAPI::String_t),   QFV.maxLenght(3),               QRequired, UPAdmin },
                { Fields::locStatus,           ORM_STATUS_FIELD(TAPI::enuGenericStatus, TAPI::enuGenericStatus::Active) },
                { ORM_INVALIDATED_AT_FIELD },
                { Fields::locCreationDateTime, ORM_CREATED_ON },
                { Fields::locCreatedBy_usrID,  ORM_CREATED_BY },
                { Fields::locUpdatedBy_usrID,  ORM_UPDATED_BY },
            };

        const QList<stuRelation> Relations = {
            ///Col                            Reference Table                ForeignCol        Rename      LeftJoin
                ORM_RELATION_OF_CREATOR(Fields::locCreatedBy_usrID),
                ORM_RELATION_OF_UPDATER(Fields::locUpdatedBy_usrID),
            };

        const QList<stuDBIndex> Indexes = {
            { {
                Fields::locURL,
                Fields::locPlaceCode,
                ORM_INVALIDATED_AT_FIELD_NAME,
              }, enuDBIndex::Unique },
            { Fields::locURL },
            { Fields::locPlaceCode },
            { Fields::locStatus },
            { Fields::locCreatedBy_usrID },
            { Fields::locCreationDateTime },
            { Fields::locUpdatedBy_usrID },
        };

    } //namespace Private

    TAPI_DEFINE_STRUCT(DTO,
        SF_ORM_PRIMARYKEY_32        (locID),
        SF_QString                  (locURL),
        SF_QString                  (locPlaceCode),
        SF_ORM_STATUS_FIELD         (locStatus, TAPI::enuGenericStatus, TAPI::enuGenericStatus::Active),
        SF_ORM_CREATED_ON           (locCreationDateTime),
        SF_ORM_CREATED_BY           (locCreatedBy_usrID),
        SF_ORM_UPDATED_BY           (locUpdatedBy_usrID)
    );
}

#pragma GCC diagnostic pop

class Locations : public intfSQLBasedModule
{
    Q_OBJECT
    TARGOMAN_DEFINE_API_SUBMODULE(Advert, Locations)

private slots:
    QVariant ORMGET("Get Locations information")
    quint32 ORMCREATE("Create a new Location by an authorized user")
    bool ORMUPDATE("Update Location info by an authorized user")
    bool ORMDELETE("Delete a Locations")
};

} //namespace ORM
} //namespace Targoman::API::AdvertModule

#endif // TARGOMAN_API_MODULES_ADVERT_ORM_LOCATIONS_H
