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

#include "Voucher.h"
#include "../Account.h"

namespace Targoman::API::AccountModule::ORM {

TARGOMAN_API_SUBMODULE_IMPLEMENT(Account, Voucher)

Voucher::Voucher() :
    intfSQLBasedModule(
        AAASchema,
        tblVoucher::Name,
        tblVoucher::Private::ORMFields,
        tblVoucher::Private::Relations,
        tblVoucher::Private::Indexes
) { ; }

ORMSelectQuery Voucher::makeSelectQuery(INTFAPICALLCONTEXT_IMPL &_apiCallContext, const QString &_alias, Q_DECL_UNUSED bool _translate, Q_DECL_UNUSED bool _isRoot) {

    ORMSelectQuery Query = intfSQLBasedModule::makeSelectQuery(_apiCallContext, _alias, _translate);

    if (_isRoot) {
        Query.addCols(this->selectableColumnNames())
             .nestedLeftJoin(Voucher::instance().makeSelectQuery(_apiCallContext, "", _translate, false)
                             .addCol(tblVoucher::Fields::vch_rootVchID)
                             .addCol(DBExpression::VALUE("SUM(tblVoucher.vchTotalAmount * CASE tblVoucher.vchType WHEN 'R' THEN 1 ELSE -1 END)"), "vchTotalFreezed")
                             .where({ tblVoucher::Fields::vchType, enuConditionOperator::In, QString("'%1','%2'")
                                      .arg(QChar(enuVoucherType::Freeze)).arg(QChar(enuVoucherType::UnFreeze)) })
                             .andWhere({ tblVoucher::Fields::vch_rootVchID, enuConditionOperator::NotNull })
                             .groupBy(tblVoucher::Fields::vch_rootVchID)
                             , "tmpFreeze"
                             , { "tmpFreeze", tblVoucher::Fields::vch_rootVchID,
                                 enuConditionOperator::Equal,
                                 tblVoucher::Name, tblVoucher::Fields::vchID }
                             )
             .addCol("tmpFreeze.vchTotalFreezed")
        ;
    }

    return Query;
}

QVariant IMPL_ORMGET_USER(Voucher) {
    if (Authorization::hasPriv(_apiCallContext, this->privOn(EHTTP_GET, this->moduleBaseName())) == false)
        this->setSelfFilters({{tblVoucher::Fields::vch_usrID, _apiCallContext.getActorID()}}, _filters);

    return this->Select(GET_METHOD_ARGS_CALL_VALUES);
}

bool IMPL_ORMDELETE_USER(Voucher) {
    TAPI::ORMFields_t ExtraFilters;

    if (Authorization::hasPriv(_apiCallContext, this->privOn(EHTTP_DELETE, this->moduleBaseName())) == false) {
        ExtraFilters.insert(tblVoucher::Fields::vchType, Targoman::API::AAA::enuVoucherType::toStr(Targoman::API::AAA::enuVoucherType::Withdrawal));

        ExtraFilters.insert(tblVoucher::Fields::vch_usrID, _apiCallContext.getActorID());
//        this->setSelfFilters({{tblVoucher::Fields::vch_usrID, _apiCallContext.getActorID()}}, ExtraFilters);
    }

    return this->DeleteByPks(DELETE_METHOD_ARGS_CALL_VALUES, ExtraFilters);
}

} //namespace Targoman::API::AccountModule::ORM
