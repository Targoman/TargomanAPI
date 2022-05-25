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

#include "Currency.h"
//#include "User.h"

TAPI_REGISTER_TARGOMAN_ENUM(Targoman::API::AccountModule, enuCurrencyStatus);

namespace Targoman::API::AccountModule::ORM {

Currency::Currency() :
    intfSQLBasedModule(
        AAASchema,
        tblCurrency::Name,
        tblCurrency::Private::ORMFields,
        tblCurrency::Private::Relations,
        tblCurrency::Private::Indexes
) { ; }

QVariant IMPL_ANONYMOUSE_ORMGET(Currency) {
    return this->Select(*this, GET_METHOD_ARGS_CALL_INTERNAL_BOOM);
}

quint64 IMPL_ORMCREATE(Currency) {
    Authorization::checkPriv(_APICALLBOOM.getJWT(), this->privOn(EHTTP_DELETE, this->moduleBaseName()));

    return this->Create(*this, CREATE_METHOD_ARGS_CALL_INTERNAL_BOOM);
}

bool IMPL_ORMUPDATE(Currency) {
    Authorization::checkPriv(_APICALLBOOM.getJWT(), this->privOn(EHTTP_PATCH, this->moduleBaseName()));

    return this->Update(*this, UPDATE_METHOD_ARGS_CALL_INTERNAL_BOOM);
}

bool IMPL_ORMDELETE(Currency) {
    Authorization::checkPriv(_APICALLBOOM.getJWT(), this->privOn(EHTTP_DELETE, this->moduleBaseName()));

    return this->DeleteByPks(*this, DELETE_METHOD_ARGS_CALL_INTERNAL_BOOM);
}

/**
 * @callby:
 *     operator
 *     owner
 */
bool IMPL_REST_UPDATE(Currency, setAsDefault, (
    APICALLBOOM_TYPE_JWT_IMPL &APICALLBOOM_PARAM,
    quint32 _curID
)) {
    Authorization::checkPriv(_APICALLBOOM.getJWT(), { this->moduleBaseName() + ":canChangeDefault" });

    this->callSP(APICALLBOOM_PARAM,
                 "spCurrency_SetAsDefault", {
                     { "iUserID", _APICALLBOOM.getUserID() },
                     { "iCurID", _curID },
                 });

    return true;
}

} //namespace Targoman::API::AccountModule::ORM
