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

#include "TokenBin.h"
//#include "User.h"
#include "../Account.h"

TAPI_REGISTER_TARGOMAN_ENUM(Targoman::API::AccountModule, enuTokenBinType);

namespace Targoman::API::AccountModule::ORM {

TARGOMAN_API_SUBMODULE_IMPLEMENT(Account, TokenBin)

TokenBin::TokenBin() :
    intfSQLBasedModule(
        AAASchema,
        tblTokenBin::Name,
        tblTokenBin::Private::ORMFields,
        tblTokenBin::Private::Relations,
        tblTokenBin::Private::Indexes
) { ; }

QVariant IMPL_ORMGET_USER(TokenBin) {
    Authorization::checkPriv(_apiCallContext, this->privOn(EHTTP_GET, this->moduleBaseName()));

    return this->Select(GET_METHOD_ARGS_CALL_VALUES);
}

QVariantList IMPL_REST_POST(TokenBin, removeExpiredAndFetchNew, (
    APICALLCONTEXT_TYPE_JWT_ANONYMOUSE_IMPL &_apiCallContext,
    const quint64 _lastFetchedID
)) {
    //1: delete expired
    this->makeDeleteQuery(_apiCallContext)
            .where({ tblTokenBin::Fields::tkbDueDateTime, enuConditionOperator::NotNull })
            .andWhere({ tblTokenBin::Fields::tkbDueDateTime, enuConditionOperator::Less, DBExpression::NOW() })
            .execute(1, {}, true)
            ;

    //2: fetch
    ORMSelectQuery Query = this->makeSelectQuery(_apiCallContext)
                           .addCols({
                                        tblTokenBin::Fields::tkbID,
                                        tblTokenBin::Fields::tkbTokenMD5,
                                        tblTokenBin::Fields::tkbDueDateTime,
                                        tblTokenBin::Fields::tkbType,
                                        tblTokenBin::Fields::tkbCreationDateTime,
//                                        tblTokenBin::Fields::tkbCreatedBy_usrID,
                                    })
                           .pageSize(0);

    if (_lastFetchedID > 0)
        Query.where({ tblTokenBin::Fields::tkbID, enuConditionOperator::Greater, _lastFetchedID });

    auto Ret = Query.all();

    bool CompactList = _apiCallContext.requestHeader("compact-list", false).toBool();
    return Ret.toVariant(CompactList).toList();
}

} //namespace Targoman::API::AccountModule::ORM
