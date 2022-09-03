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

#ifndef TARGOMAN_API_MODULES_ADVERT_ORM_CLICKS_H
#define TARGOMAN_API_MODULES_ADVERT_ORM_CLICKS_H

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
namespace tblClicks {
    constexpr char Name[] = "tblClicks";

    namespace Fields {
        TARGOMAN_CREATE_CONSTEXPR(clkID);
        TARGOMAN_CREATE_CONSTEXPR(clk_binID);
        TARGOMAN_CREATE_CONSTEXPR(clk_locID);
        TARGOMAN_CREATE_CONSTEXPR(clkDateTime);
        TARGOMAN_CREATE_CONSTEXPR(clkIP);
        TARGOMAN_CREATE_CONSTEXPR(clkDevice);
        TARGOMAN_CREATE_CONSTEXPR(clkScreenSize);
        TARGOMAN_CREATE_CONSTEXPR(clkOS);
        TARGOMAN_CREATE_CONSTEXPR(clkBrowser);
    }

    namespace Relation {
//        constexpr char AAA[] = "aaa";
    }

    namespace Private {
        const QList<clsORMField> ORMFields = {
            ///<ColName                 Type                    Validation                      UpBy   Sort  Filter Self  Virt   PK
                { Fields::clkID,          ORM_PRIMARYKEY_64},
                { Fields::clk_binID,      S(quint32),             QFV.integer().minValue(1),      QInvalid, UPNone},
                { Fields::clk_locID,      S(quint32),             QFV.integer().minValue(1),      QInvalid, UPNone},
                { Fields::clkDateTime,    S(TAPI::DateTime_t),    QFV,                            QInvalid, UPNone},
                { Fields::clkIP,          S(quint32),             QFV.ipv4(),                     QInvalid, UPNone},
                { Fields::clkDevice,      S(quint32),             QFV.asciiAlNum().maxLenght(50), QNull,    UPNone},
                { Fields::clkScreenSize,  S(quint32),             QFV.asciiAlNum().maxLenght(50), QNull,    UPNone},
                { Fields::clkOS,          S(quint32),             QFV.asciiAlNum().maxLenght(50), QNull,    UPNone},
                { Fields::clkBrowser,     S(quint32),             QFV.asciiAlNum().maxLenght(50), QNull,    UPNone},
            };

        const QList<stuRelation> Relations = {
            ///Col                 Reference Table                     ForeignCol   Rename     LeftJoin
                { Fields::clk_binID,   R(AdvertSchema,tblBin::Name),       tblBin::Fields::binID},
                { Fields::clk_locID,   R(AdvertSchema,tblLocations::Name), tblLocations::Fields::locID },
            };

        const QList<stuDBIndex> Indexes = {
        };

    } //namespace Private

    TAPI_DEFINE_STRUCT(DTO,
        SF_ORM_PRIMARYKEY_64        (clkID),
        SF_quint32                  (clk_binID),
        SF_quint32                  (clk_locID),
        SF_DateTime_t               (clkDateTime),
        SF_quint32                  (clkIP),
        SF_quint32                  (clkDevice),
        SF_quint32                  (clkScreenSize),
        SF_quint32                  (clkOS),
        SF_quint32                  (clkBrowser)
    );
}
#pragma GCC diagnostic pop

class Clicks : public intfSQLBasedModule
{
    Q_OBJECT
    TARGOMAN_API_SUBMODULE_DEFINE(Advert, Clicks)

private slots:
    QVariant ORMGET_USER("Get Clicks information")
};

} //namespace ORM
} //namespace Targoman::API::AdvertModule

#endif // TARGOMAN_API_MODULES_ADVERT_ORM_CLICKS_H
