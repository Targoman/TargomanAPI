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

#ifndef TARGOMAN_API_MODULES_ACCOUNT_ORM_IPSTATS_H
#define TARGOMAN_API_MODULES_ACCOUNT_ORM_IPSTATS_H

#include "Interfaces/AAA/AAA.hpp"
#include "Interfaces/API/intfSQLBasedModule.h"
#include "IPBin.h"

namespace Targoman::API::AccountModule {

//structures and enumes goes here

namespace ORM {

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-variable"
namespace tblIPStats {
    constexpr char Name[] = "tblIPStats";

    namespace Fields {
        TARGOMAN_CREATE_CONSTEXPR(ips_ipbIP);
        TARGOMAN_CREATE_CONSTEXPR(ipsTimeStamp);
        TARGOMAN_CREATE_CONSTEXPR(ipsInsertionDate);
    }

    namespace Relation {
        // constexpr char AAA[] = "aaa";
    }

    namespace Private {
        const QList<clsORMField> ORMFields = {
            ///<ColName                       Type                 Validation                     Default    UpBy   Sort  Filter Self  Virt   PK
            { Fields::ips_ipbIP,           ORM_PRIMARYKEY_32},
            { Fields::ipsTimeStamp,        S(double),           QFV.allwaysValid(),            QRequired, UPNone},
            { Fields::ipsInsertionDate,    ORM_CREATED_ON},
          };

        const QList<stuRelation> Relations = {
            ///Col                  Reference Table                  ForeignCol
            { Fields::ips_ipbIP,   R(AAASchema, tblIPBin::Name),     tblIPBin::Fields::ipbIP },
          };

        const QList<stuDBIndex> Indexes = {
        };

    } //namespace Private

    TAPI_DEFINE_STRUCT(DTO,
        SF_ORM_PRIMARYKEY_32        (ips_ipbIP),
        SF_qreal                    (ipsTimeStamp),
        SF_ORM_CREATED_ON           (ipsInsertionDate)
    );
}
#pragma GCC diagnostic pop

class IPStats : public intfSQLBasedModule
{
    Q_OBJECT
    TARGOMAN_DEFINE_API_SUBMODULE(Account, IPStats)

private slots:
    QVariant ORMGET("Get IPStats information")
};

} //namespace ORM
} //namespace Targoman::API::AccountModule

#endif // TARGOMAN_API_MODULES_ACCOUNT_ORM_IPSTATS_H
