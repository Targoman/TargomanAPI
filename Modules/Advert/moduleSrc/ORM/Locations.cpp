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

#include "Locations.h"
#include "Defs.hpp"

namespace Targoman::API::AdvertModule::ORM {

//using namespace ORM;

Locations::Locations() :
    intfSQLBasedModule(
        AdvertSchema,
        tblLocations::Name,
        {///< ColName                            Type                 Validation                      Default    UpBy   Sort  Filter Self  Virt   PK
            { tblLocations::locID,               ORM_PRIMARYKEY_32 },
            { tblLocations::locURL,              S(TAPI::URL_t),      QFV/*.integer().minValue(1)*/,  QRequired, UPAdmin },
            { tblLocations::locPlaceCode,        S(TAPI::String_t),   QFV.maxLenght(3),               QRequired, UPAdmin },
            { tblLocations::locStatus,           ORM_STATUS_FIELD(TAPI::enuGenericStatus, TAPI::enuGenericStatus::Active) },
            { ORM_INVALIDATED_AT_FIELD },
            { tblLocations::locCreationDateTime, ORM_CREATED_ON },
            { tblLocations::locCreatedBy_usrID,  ORM_CREATED_BY },
            { tblLocations::locUpdatedBy_usrID,  ORM_UPDATED_BY },
        },
        {///< Col                            Reference Table                ForeignCol        Rename      LeftJoin
            ORM_RELATION_OF_CREATOR(tblLocations::locCreatedBy_usrID),
            ORM_RELATION_OF_UPDATER(tblLocations::locUpdatedBy_usrID),
        },
        {
            { {
                tblLocations::locURL,
                tblLocations::locPlaceCode,
                ORM_INVALIDATED_AT_FIELD_NAME,
              }, enuDBIndex::Unique },
            { tblLocations::locURL },
            { tblLocations::locPlaceCode },
            { tblLocations::locStatus },
            { tblLocations::locCreatedBy_usrID },
            { tblLocations::locCreationDateTime },
            { tblLocations::locUpdatedBy_usrID },
        }
    ) { ; }

QVariant IMPL_ORMGET(Locations) {
    Authorization::checkPriv(_APICALLBOOM.getJWT(), this->privOn(EHTTP_GET, this->moduleBaseName()));

    return this->Select(GET_METHOD_ARGS_CALL_INTERNAL_BOOM);
}

quint32 IMPL_ORMCREATE(Locations) {
    Authorization::checkPriv(_APICALLBOOM.getJWT(), this->privOn(EHTTP_PUT, this->moduleBaseName()));

    return this->Create(CREATE_METHOD_ARGS_CALL_INTERNAL_BOOM);
}

bool IMPL_ORMUPDATE(Locations) {
    Authorization::checkPriv(_APICALLBOOM.getJWT(), this->privOn(EHTTP_PATCH, this->moduleBaseName()));

    return this->Update(UPDATE_METHOD_ARGS_CALL_INTERNAL_BOOM);
}

bool IMPL_ORMDELETE(Locations) {
    Authorization::checkPriv(_APICALLBOOM.getJWT(), this->privOn(EHTTP_DELETE, this->moduleBaseName()));

    return this->DeleteByPks(DELETE_METHOD_ARGS_CALL_INTERNAL_BOOM);
}

} //namespace Targoman::API::AdvertModule::ORM
