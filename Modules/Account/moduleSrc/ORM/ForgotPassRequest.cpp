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

#include "ForgotPassRequest.h"
#include "User.h"

//#include "Interfaces/ORM/APIQueryBuilders.h"

TAPI_REGISTER_TARGOMAN_ENUM(Targoman::API::AccountModule, enuFPRStatus);
TAPI_REGISTER_TARGOMAN_ENUM(Targoman::API::AccountModule, enuForgotPassLinkVia);

namespace Targoman::API::AccountModule::ORM {

ForgotPassRequest::ForgotPassRequest() :
    intfSQLBasedModule(
        AAASchema,
        tblForgotPassRequest::Name,
        {///< ColName                                   Type                    Validation                  Default     UpBy   Sort  Filter Self  Virt   PK
            { tblForgotPassRequest::fprID,              ORM_PRIMARYKEY_64 },
            { tblForgotPassRequest::fpr_usrID,          S(quint64),             QFV.integer().minValue(1),  QRequired,  UPNone },
            { tblForgotPassRequest::fprRequestedVia,    S(Targoman::API::AccountModule::enuForgotPassLinkVia::Type), QFV, Targoman::API::AccountModule::enuForgotPassLinkVia::Email, UPNone },
            { tblForgotPassRequest::fprCode,            S(QString),             QFV,                        QRequired,  UPNone },
            { tblForgotPassRequest::fprRequestDate,     ORM_CREATED_ON },
            { tblForgotPassRequest::fprApplyDate,       S(TAPI::DateTime_t),    QFV,                        QNull,      UPAdmin },
            { tblForgotPassRequest::fprStatus,          ORM_STATUS_FIELD(Targoman::API::AccountModule::enuFPRStatus, Targoman::API::AccountModule::enuFPRStatus::New) },
        },
        {///< Col                                Reference Table                 ForeignCol
            { tblForgotPassRequest::Relation::User, { tblForgotPassRequest::fpr_usrID,   R(AAASchema,tblUser::Name),     tblUser::usrID } },
        }
    ) { ; }

QVariant IMPL_ORMGET(ForgotPassRequest) {
    Authorization::checkPriv(_APICALLBOOM.getJWT(), this->privOn(EHTTP_GET, this->moduleBaseName()));

    return this->Select(*this, GET_METHOD_CALL_ARGS_INTERNAL_CALL_BOOM);

//    return query.one();

    //    return this->selectFromTable({}, {}, GET_METHOD_CALL_ARGS_APICALL);
}

bool IMPL_ORMDELETE(ForgotPassRequest) {
    Authorization::checkPriv(_APICALLBOOM.getJWT(), this->privOn(EHTTP_DELETE, this->moduleBaseName()));

    return this->DeleteByPks(*this, DELETE_METHOD_CALL_ARGS_INTERNAL_CALL_BOOM2USER, {}, true);
}

} //namespace Targoman::API::AccountModule::ORM
