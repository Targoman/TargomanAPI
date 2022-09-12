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

#ifndef TARGOMAN_API_MODULES_ACCOUNT_ORM_TOKENBIN_H
#define TARGOMAN_API_MODULES_ACCOUNT_ORM_TOKENBIN_H

#include "Interfaces/AAA/AAA.hpp"
#include "Interfaces/API/intfSQLBasedModule.h"

namespace Targoman::API::AccountModule {
//structures and enumes goes here

} //namespace Targoman::API::AccountModule

//TAPI_DECLARE_METATYPE_ENUM(Targoman::API::AccountModule, enu...);

namespace Targoman::API::AccountModule {
namespace ORM {

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-variable"
namespace tblTokenBin {
    constexpr char Name[] = "tblTokenBin";

    namespace Fields {
        TARGOMAN_CREATE_CONSTEXPR(tkbID);
        TARGOMAN_CREATE_CONSTEXPR(tkbTokenMD5);
        TARGOMAN_CREATE_CONSTEXPR(tkbTokenExpiredAt);
        TARGOMAN_CREATE_CONSTEXPR(tkbBlockedAt);
        TARGOMAN_CREATE_CONSTEXPR(tkbBlockedBy_usrID);
    }

    namespace Relation {
        // constexpr char AAA[] = "aaa";
    }

    namespace Private {
        const QList<clsORMField> ORMFields = {
            //ColName                       Type                    Validation  Default     UpBy   Sort  Filter Self  Virt   PK
            { Fields::tkbID,                ORM_PRIMARYKEY_64 },
            { Fields::tkbTokenMD5,          S(TAPI::MD5_t),         QFV,        QRequired,  UPNone },
            { Fields::tkbTokenExpiredAt,    S(TAPI::DateTime_t),    QFV,        QRequired,  UPNone },
            { Fields::tkbBlockedAt,         ORM_CREATED_ON },
            { Fields::tkbBlockedBy_usrID,   ORM_CREATED_BY },
        };

        const QList<stuRelation> Relations = {
            //Col                           Reference Table                 ForeignCol              Rename      LeftJoin
            { Fields::tkbBlockedBy_usrID,   R(AAASchema, tblUser::Name),    tblUser::Fields::usrID, "Blocker_", true },
        };

        const QList<stuDBIndex> Indexes = {
        };

    } //namespace Private

    TAPI_DEFINE_STRUCT(DTO,
        SF_ORM_PRIMARYKEY_64        (tkbID),
        SF_MD5_t                    (tkbTokenMD5),
        SF_DateTime_t               (tkbTokenExpiredAt),
        SF_ORM_CREATED_ON           (tkbBlockedAt),
        SF_ORM_CREATED_BY           (tkbBlockedBy_usrID)
    );
}
#pragma GCC diagnostic pop

class TokenBin : public intfSQLBasedModule
{
    Q_OBJECT
    TARGOMAN_API_SUBMODULE_DEFINE(Account, TokenBin)

private slots:
    QVariant ORMGET_USER("Get TokenBin information")
};

} //namespace ORM
} //namespace Targoman::API::AccountModule

#endif // TARGOMAN_API_MODULES_ACCOUNT_ORM_TOKENBIN_H
