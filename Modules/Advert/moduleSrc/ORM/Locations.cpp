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

#include "Interfaces/ORM/APIQueryBuilders.h"
//using namespace Targoman::API::ORM;

namespace Targoman {
namespace API {
namespace Advertisement {

using namespace ORM;

Locations::Locations() :
    clsTable(
        AdvertSchema,
        tblLocations::Name,
        {///< ColName                            Type                 Validation                      Default    UpBy   Sort  Filter Self  Virt   PK
            { tblLocations::locID,               ORM_PRIMARY_KEY32 },
            { tblLocations::locURL,              S(TAPI::URL_t),      QFV/*.integer().minValue(1)*/,  QRequired, UPAdmin },
            { tblLocations::locPlaceCode,        S(TAPI::String_t),   QFV.maxLenght(3),               QRequired, UPAdmin },
            { tblLocations::locStatus,           S(TAPI::enuGenericStatus::Type), QFV,                TAPI::enuGenericStatus::Active, UPStatus },
            { tblLocations::locCreatedBy_usrID,  ORM_CREATED_BY },
            { tblLocations::locCreationDateTime, ORM_CREATED_ON },
            { tblLocations::locUpdatedBy_usrID,  ORM_UPDATED_BY },
        },
        {///< Col                            Reference Table                ForeignCol        Rename      LeftJoin
            ORM_RELATION_OF_CREATOR(tblLocations::locCreatedBy_usrID),
            ORM_RELATION_OF_UPDATER(tblLocations::locUpdatedBy_usrID),
        },
        {
            { { tblLocations::locURL, tblLocations::locPlaceCode, tblLocations::locStatus }, enuDBIndex::Unique },
            { tblLocations::locURL },
            { tblLocations::locPlaceCode },
            { tblLocations::locStatus },
            { tblLocations::locCreatedBy_usrID },
            { tblLocations::locCreationDateTime },
            { tblLocations::locUpdatedBy_usrID },
        }
    )
{}

QVariant Locations::apiGET(GET_METHOD_ARGS_IMPL)
{
    Authorization::checkPriv(_JWT, this->privOn(EHTTP_GET,this->moduleBaseName()));

    return Targoman::API::Query::SelectOne(*this, GET_METHOD_CALL_ARGS); //, ExtraFilters, CACHE_TIME);

//    QT_TRY {
//        return query.one();
//    }
//    QT_CATCH(const std::exception& _exp) {
//        qDebug() << _exp.what();
//        return {};
//    }

    //    return this->selectFromTable({}, {}, GET_METHOD_CALL_ARGS);
}

quint32 Locations::apiCREATE(CREATE_METHOD_ARGS_IMPL)
{
    Authorization::checkPriv(_JWT, this->privOn(EHTTP_PUT, this->moduleBaseName()));
//    //return this->create(CREATE_METHOD_CALL_ARGS).toUInt();

    return Targoman::API::Query::Create(*this, CREATE_METHOD_CALL_ARGS);

//    ApiCreateQuery query = ApiCreateQuery(*this, CREATE_METHOD_CALL_ARGS);

//    clsJWT JWT(_JWT);
//    quint64 insertedID = query.execute(JWT.usrID());
//    return insertedID;
}

bool Locations::apiUPDATE(UPDATE_METHOD_ARGS_IMPL)
{
    Authorization::checkPriv(_JWT, this->privOn(EHTTP_PATCH, this->moduleBaseName()));
    return this->update(UPDATE_METHOD_CALL_ARGS);
}

bool Locations::apiDELETE(DELETE_METHOD_ARGS_IMPL)
{
    Authorization::checkPriv(_JWT, this->privOn(EHTTP_DELETE, this->moduleBaseName()));
    return this->deleteByPKs(DELETE_METHOD_CALL_ARGS);
}

}
}
}
