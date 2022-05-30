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

#ifndef TARGOMAN_API_MODULES_ACCOUNT_ORM_FORGOTPASSREQUEST_H
#define TARGOMAN_API_MODULES_ACCOUNT_ORM_FORGOTPASSREQUEST_H

#include "Interfaces/AAA/AAA.hpp"
#include "Interfaces/API/intfSQLBasedModule.h"

namespace Targoman::API::AccountModule {

//structures and enumes goes here

TARGOMAN_DEFINE_ENUM(enuFPRStatus,
                     New      = 'N',
                     Sent     = 'S',
                     Applied  = 'A',
                     Removed  = 'R'
                     )
TARGOMAN_DEFINE_ENUM(enuForgotPassLinkVia,
                     Email    = 'E',
                     Mobile   = 'M',
                     )

} //namespace Targoman::API::AccountModule

TAPI_DECLARE_METATYPE_ENUM(Targoman::API::AccountModule, enuFPRStatus);
TAPI_DECLARE_METATYPE_ENUM(Targoman::API::AccountModule, enuForgotPassLinkVia);

namespace Targoman::API::AccountModule {
namespace ORM {

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-variable"
namespace tblForgotPassRequest {
    constexpr char Name[] = "tblForgotPassRequest";

    namespace Fields {
        TARGOMAN_CREATE_CONSTEXPR(fprID);
        TARGOMAN_CREATE_CONSTEXPR(fpr_usrID);
        TARGOMAN_CREATE_CONSTEXPR(fprRequestedVia);
        TARGOMAN_CREATE_CONSTEXPR(fprCode);
        TARGOMAN_CREATE_CONSTEXPR(fprRequestDate);
        TARGOMAN_CREATE_CONSTEXPR(fprApplyDate);
        TARGOMAN_CREATE_CONSTEXPR(fprStatus);
    }

    namespace Relation {
        constexpr char User[] = "user";
    }

    namespace Private {
        const QList<clsORMField> ORMFields = {
            ///< ColName                                   Type                    Validation                  Default     UpBy   Sort  Filter Self  Virt   PK
            { Fields::fprID,              ORM_PRIMARYKEY_64 },
            { Fields::fpr_usrID,          S(quint64),             QFV.integer().minValue(1),  QRequired,  UPNone },
            { Fields::fprRequestedVia,    S(Targoman::API::AccountModule::enuForgotPassLinkVia::Type), QFV, Targoman::API::AccountModule::enuForgotPassLinkVia::Email, UPNone },
            { Fields::fprCode,            S(QString),             QFV,                        QRequired,  UPNone },
            { Fields::fprRequestDate,     ORM_CREATED_ON },
            { Fields::fprApplyDate,       S(TAPI::DateTime_t),    QFV,                        QNull,      UPAdmin },
            { Fields::fprStatus,          ORM_STATUS_FIELD(Targoman::API::AccountModule::enuFPRStatus, Targoman::API::AccountModule::enuFPRStatus::New) },
        };

        const QList<stuRelation> Relations = {
            ///< Col                                Reference Table                 ForeignCol
            { Relation::User, { Fields::fpr_usrID,   R(AAASchema,tblUser::Name),     tblUser::Fields::usrID } },
        };

        const QList<stuDBIndex> Indexes = {
        };

    } //namespace Private

    TAPI_DEFINE_VARIANT_ENABLED_STRUCT(DTO,
        SF_ORM_PRIMARYKEY_64        (fprID),
        SF_quint64                  (fpr_usrID),
        SF_Enum                     (fprRequestedVia, Targoman::API::AccountModule::enuForgotPassLinkVia, Targoman::API::AccountModule::enuForgotPassLinkVia::Email),
        SF_QString                  (fprCode),
        SF_DateTime_t               (fprApplyDate),
        SF_ORM_STATUS_FIELD         (fprStatus, Targoman::API::AccountModule::enuFPRStatus, Targoman::API::AccountModule::enuFPRStatus::New),
        SF_ORM_CREATED_ON           (fprRequestDate)
    );
}
#pragma GCC diagnostic pop

class ForgotPassRequest : public intfSQLBasedModule
{
    Q_OBJECT
    TARGOMAN_DEFINE_API_SUBMODULE(Account, ForgotPassRequest)

private slots:
    QVariant ORMGET("Get ForgotPassRequest information")
    bool ORMDELETE("Delete a ForgotPassRequest")
};

} //namespace ORM
} //namespace Targoman::API::AccountModule

#endif // TARGOMAN_API_MODULES_ACCOUNT_ORM_FORGOTPASSREQUEST_H
