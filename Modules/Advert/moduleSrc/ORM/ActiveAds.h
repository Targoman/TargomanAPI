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

#ifndef TARGOMAN_API_MODULES_ADVERT_ORM_ACTIVEADS_H
#define TARGOMAN_API_MODULES_ADVERT_ORM_ACTIVEADS_H

#include "Interfaces/AAA/AAA.hpp"
#include "Interfaces/API/intfSQLBasedModule.h"
#include "Defs.hpp"
#include "Bin.h"
#include "Locations.h"

namespace Targoman::API::AdvertModule {

//structures and enumes goes here

namespace ORM {

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-variable"
namespace tblActiveAds {
    constexpr char Name[] = "tblActiveAds";

    namespace Fields {
        TARGOMAN_CREATE_CONSTEXPR(act_binID);
        TARGOMAN_CREATE_CONSTEXPR(act_locID);
        TARGOMAN_CREATE_CONSTEXPR(actOrder);
        TARGOMAN_CREATE_CONSTEXPR(actOnKeyword);
    }

    namespace Relation {
//        constexpr char AAA[] = "aaa";
    }

    namespace Private {
        const QList<clsORMField> ORMFields = {
            ///<ColName                  Type           Validation                        Default     UpBy   Sort  Filter Self  Virt   PK
               { Fields::act_binID,    ORM_PRIMARYKEY_32},
               { Fields::act_locID,    ORM_PRIMARYKEY_32},
               { Fields::actOrder,     S(Targoman::API::AdvertModule::enuAdvertOrder::Type),QFV,               Targoman::API::AdvertModule::enuAdvertOrder::Normal, UPNone},
               { Fields::actOnKeyword, S(QString),    QFV.unicodeAlNum().maxLenght(50), QInvalid, UPNone},
             };

        const QList<stuRelation> Relations = {
            ///< Col                     Reference Table                 ForeignCol   Rename     LeftJoin
               { Fields::act_binID,    R(AdvertSchema, tblBin::Name),   tblBin::Fields::binID},
               { Fields::act_locID,    R(AdvertSchema, tblLocations::Name), tblLocations::Fields::locID },
             };

        const QList<stuDBIndex> Indexes = {
        };

    } //namespace Private

    TAPI_DEFINE_VARIANT_ENABLED_STRUCT(DTO,
        SF_ORM_PRIMARYKEY_32        (act_binID),
        SF_ORM_PRIMARYKEY_32        (act_locID),
        SF_Enum                     (actOrder, Targoman::API::AdvertModule::enuAdvertOrder, Targoman::API::AdvertModule::enuAdvertOrder::Normal),
        SF_QString                  (actOnKeyword)
    );
}
#pragma GCC diagnostic pop

class ActiveAds : public intfSQLBasedModule
{
    Q_OBJECT
    TARGOMAN_DEFINE_API_SUBMODULE(Advert, ActiveAds)

private slots:
    QVariant ORMGET("Get Active Adveritsements")
    bool ORMDELETE("Delete an Active Advertisement")
};

} //namespace ORM
} //namespace Targoman::API::AdvertModule

#endif // TARGOMAN_API_MODULES_ADVERT_ORM_ACTIVEADS_H
