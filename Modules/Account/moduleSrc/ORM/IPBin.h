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

#ifndef TARGOMAN_API_MODULES_ACCOUNT_ORM_IPBIN_H
#define TARGOMAN_API_MODULES_ACCOUNT_ORM_IPBIN_H

#include "Interfaces/AAA/AAA.hpp"
#include "Interfaces/API/intfSQLBasedModule.h"

namespace Targoman::API::AccountModule {

//structures and enumes goes here

TARGOMAN_DEFINE_ENUM(enuIPBinStatus,
                     Active  = 'A',
                     Blocked = 'B',
                     Removed = 'R'
                     )

} //namespace Targoman::API::AccountModule

TAPI_DECLARE_METATYPE_ENUM(Targoman::API::AccountModule, enuIPBinStatus);

namespace Targoman::API::AccountModule {
namespace ORM {

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-variable"
namespace tblIPBin {
    constexpr char Name[] = "tblIPBin";

    namespace Fields {
        TARGOMAN_CREATE_CONSTEXPR(ipbIP);
        TARGOMAN_CREATE_CONSTEXPR(ipbReadable);
        TARGOMAN_CREATE_CONSTEXPR(ipbFirstAccess);
        TARGOMAN_CREATE_CONSTEXPR(ipbAccessCount);
        TARGOMAN_CREATE_CONSTEXPR(ipbLastAccess);
        TARGOMAN_CREATE_CONSTEXPR(ipbBlockedBy_usrID);
        TARGOMAN_CREATE_CONSTEXPR(ipbBlockingTime);
        TARGOMAN_CREATE_CONSTEXPR(ipbStatus);
    }

    namespace Relation {
        // constexpr char AAA[] = "aaa";
    }

    namespace Private {
        const QList<clsORMField> ORMFields = {
            ///ColName                        Type                 Validation                 Default    UpBy   Sort  Filter Self  Virt   PK
            { Fields::ipbIP,               ORM_PRIMARYKEY_32 },
            { Fields::ipbReadable,         S(TAPI::IPv4_t),     QFV,                       QInvalid,  UPNone },
            { Fields::ipbFirstAccess,      S(TAPI::DateTime_t), QFV,                       QAuto,     UPNone },
            { Fields::ipbAccessCount,      S(quint64),          QFV.integer().minValue(0), 0,         UPNone },
            { Fields::ipbLastAccess,       S(TAPI::DateTime_t), QFV,                       QAuto,     UPNone },
            { Fields::ipbBlockedBy_usrID,  S(quint64),          QFV.integer().minValue(1), QNull,     UPNone },
            { Fields::ipbBlockingTime,     S(TAPI::DateTime_t), QFV,                       QNull,     UPNone },
            { Fields::ipbStatus,           ORM_STATUS_FIELD(Targoman::API::AccountModule::enuIPBinStatus, Targoman::API::AccountModule::enuIPBinStatus::Active) },
            { ORM_INVALIDATED_AT_FIELD },
        };

        const QList<stuRelation> Relations = {
            ///Col                            Reference Table             ForeignCol      Rename      LeftJoin
            { Fields::ipbBlockedBy_usrID,  R(AAASchema, tblUser::Name), tblUser::Fields::usrID, "Blocker_", true },
        };

        const QList<stuDBIndex> Indexes = {
            { {
                Fields::ipbIP,
                ORM_INVALIDATED_AT_FIELD_NAME,
              }, enuDBIndex::Unique },
        };

    } //namespace Private

    TAPI_DEFINE_STRUCT(DTO,
        SF_ORM_PRIMARYKEY_32        (ipbIP),
        SF_QString                  (ipbReadable),
        SF_DateTime_t               (ipbFirstAccess),
        SF_quint64                  (ipbAccessCount),
        SF_DateTime_t               (ipbLastAccess),
        SF_quint64                  (ipbBlockedBy_usrID),
        SF_DateTime_t               (ipbBlockingTime),
        SF_ORM_STATUS_FIELD         (ipbStatus, Targoman::API::AccountModule::enuIPBinStatus, Targoman::API::AccountModule::enuIPBinStatus::Active)
    );
}
#pragma GCC diagnostic pop

class IPBin : public intfSQLBasedModule
{
    Q_OBJECT
    TARGOMAN_API_SUBMODULE_DEFINE(Account, IPBin)

private slots:
    QVariant ORMGET_USER("Get IPBin information")
};

} //namespace ORM
} //namespace Targoman::API::AccountModule

#endif // TARGOMAN_API_MODULES_ACCOUNT_ORM_IPBIN_H
