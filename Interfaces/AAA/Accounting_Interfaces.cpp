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
 * @author S.Mehran M.Ziabary <ziabary@targoman.com>
 * @author Kambiz Zandi <kambizzandi@gmail.com>
 */

#include "intfAccountingBasedModule.h"
#include "PrivHelpers.h"
#include "Accounting_Interfaces.h"
#include "Server/ServerConfigs.h"
#include "Interfaces/AAA/Authorization.h"
#include "Interfaces/Common/QtTypes.hpp"

using namespace Targoman::API;

//#include "Interfaces/ORM/APIQueryBuilders.h"

using namespace Targoman::API::AAA;

//TAPI_REGISTER_TARGOMAN_ENUM(Targoman::API::AAA, enuPreVoucherType);
TAPI_REGISTER_TARGOMAN_ENUM(Targoman::API::AAA, enuVoucherType);
TAPI_REGISTER_TARGOMAN_ENUM(Targoman::API::AAA, enuVoucherStatus);
TAPI_REGISTER_TARGOMAN_ENUM(Targoman::API::AAA, enuDiscountType);
TAPI_REGISTER_TARGOMAN_ENUM(Targoman::API::AAA, enuVoucherItemProcessStatus);

TAPI_REGISTER_METATYPE_TYPE_STRUCT(
    /* namespace          */ Targoman::API::AAA,
    /* type               */ stuVoucherItem
);

TAPI_REGISTER_METATYPE_TYPE_STRUCT(
    /* namespace          */ Targoman::API::AAA,
    /* type               */ stuPreVoucher
);

TAPI_REGISTER_METATYPE_TYPE_STRUCT(
    /* namespace          */ Targoman::API::AAA,
    /* type               */ stuBasketActionResult
);

TAPI_REGISTER_METATYPE_TYPE_STRUCT(
    /* namespace          */ Targoman::API::AAA,
    /* type               */ stuVoucher
);

TAPI_REGISTER_METATYPE_TYPE_STRUCT(
    /* namespace          */ Targoman::API::AAA,
    /* type               */ stuVoucherItemForTrustedAction
);

TAPI_REGISTER_METATYPE(
    /* complexity         */ COMPLEXITY_Complex,
    /* namespace          */ Targoman::API::AAA,
    /* type               */ OrderAdditives_t,
    /* fnToVariant        */ [](const OrderAdditives_t& _value) -> QVariant { return QVariant::fromValue(_value); },
    /* fnFromVariant      */ [](const QVariant& _value, const QByteArray&) -> OrderAdditives_t {
        auto Map = _value.toMap();
        OrderAdditives_t Additives;
        for (auto Iter = Map.begin(); Iter != Map.end(); ++Iter)
            Additives.insert(Iter.key(), Iter.value().toString());
        return Additives;
    }
);

namespace Targoman::API::AAA {

//constexpr char PKG_ID[] = "ID";
//constexpr char PKG_CODE[] = "CD";
//constexpr char PKG_REMAININGDAYS[] = "RD";
//constexpr char PKG_REMAININGHOURS[] = "RH";
//constexpr char PKG_STARTDATE[] = "SD";
//constexpr char PKG_ENDDATE[] = "ED";
//constexpr char PKG_STARTTIME[] = "ST";
//constexpr char PKG_ENDTIME[] = "ET";
//constexpr char PKG_LIMITS[] = "LM";
//constexpr char PKG_PROPS[] = "PP";

//constexpr char ASA_PACKAGE[] = "PK";
//constexpr char ASA_ISFROMPARENT[] = "IP";
//constexpr char ASA_TTL[] = "TT";
//constexpr char ASA_LIMITSONPARENT[] = "LP";

/******************************************************************/
/******************************************************************/
/******************************************************************/
intfAccountUnitsTranslate* intfAccountUnitsTranslate::myInstance;

intfAccountUnitsTranslate::intfAccountUnitsTranslate(
    const QString& _schema,
    const QList<DBM::clsORMField>& _exclusiveCols,
    const QList<DBM::stuRelation>& _exclusiveRelations,
    const QList<DBM::stuDBIndex>& _exclusiveIndexes
) :
    intfSQLBasedModule(
        _schema,
        tblAccountUnitsTranslateBase::Name,
        tblAccountUnitsTranslateBase::Private::ORMFields + _exclusiveCols,
        tblAccountUnitsTranslateBase::Private::Relations(_schema) + _exclusiveRelations,
        tblAccountUnitsTranslateBase::Private::Indexes + _exclusiveIndexes
) {
    intfAccountUnitsTranslate::myInstance = this;
}

/******************************************************************/
intfAccountUnits::intfAccountUnits(
    const QString& _schema,
    const QList<DBM::clsORMField>& _exclusiveCols,
    const QList<DBM::stuRelation>& _exclusiveRelations,
    const QList<DBM::stuDBIndex>& _exclusiveIndexes
) :
    intfSQLBasedModule(
        _schema,
        tblAccountUnitsBase::Name,
        tblAccountUnitsBase::Private::ORMFields + _exclusiveCols,
        tblAccountUnitsBase::Private::Relations(_schema) + _exclusiveRelations,
        tblAccountUnitsBase::Private::Indexes + _exclusiveIndexes
) { ; }

ORMSelectQuery intfAccountUnits::GetSelectQuery(INTFAPICALLBOOM_IMPL &APICALLBOOM_PARAM, const QString& _alias) {
    intfAccountUnitsTranslate::myInstance->prepareFiltersList();

    return intfSQLBasedModule::GetSelectQuery(APICALLBOOM_PARAM, _alias)
            .addCols(this->SelectableColumnNames())
            .removeCols({
                            tblAccountUnitsBase::Fields::untName
                        })
            .nestedLeftJoin(intfAccountUnitsTranslate::myInstance->GetSelectQuery(APICALLBOOM_PARAM)
                      .where({ tblAccountUnitsTranslateBase::Fields::language, enuConditionOperator::Equal, APICALLBOOM_PARAM.language() })
                      , "lng_tblAccountUnits"
                      , { "lng_tblAccountUnits", tblAccountUnitsTranslateBase::Fields::pid,
                          enuConditionOperator::Equal,
                          tblAccountUnitsBase::Name, tblAccountUnitsBase::Fields::untID
                      }
                     )
            .addCol(enuConditionalAggregation::IF,
                    { "lng_tblAccountUnits", tblAccountUnitsTranslateBase::Fields::untName, enuConditionOperator::Null },
                    DBExpression::VALUE(R(_alias.isEmpty() ? tblAccountUnitsBase::Name : _alias, tblAccountUnitsBase::Fields::untName)),
                    DBExpression::VALUE(R("lng_tblAccountUnits", tblAccountUnitsTranslateBase::Fields::untName)),
                    tblAccountUnitsBase::Fields::untName
                   )
    ;
}

QVariant IMPL_ANONYMOUSE_ORMGET(intfAccountUnits) {
    constexpr quint16 CACHE_TIME = 15 * 60;
    return this->Select(GET_METHOD_ARGS_CALL_INTERNAL_BOOM, {}, CACHE_TIME);
}

quint32 IMPL_ORMCREATE(intfAccountUnits) {
    Authorization::checkPriv(APICALLBOOM_PARAM, this->privOn(EHTTP_PUT, this->moduleBaseName()));

    return this->Create(CREATE_METHOD_ARGS_CALL_INTERNAL_BOOM);
}

bool IMPL_ORMUPDATE(intfAccountUnits) {
    Authorization::checkPriv(APICALLBOOM_PARAM, this->privOn(EHTTP_PATCH, this->moduleBaseName()));

    return this->Update(UPDATE_METHOD_ARGS_CALL_INTERNAL_BOOM);
}

bool IMPL_ORMDELETE(intfAccountUnits) {
  Authorization::checkPriv(APICALLBOOM_PARAM, this->privOn(EHTTP_DELETE, this->moduleBaseName()));

  return this->DeleteByPks(DELETE_METHOD_ARGS_CALL_INTERNAL_BOOM);
}

/******************************************************************/
/******************************************************************/
/******************************************************************/
intfAccountProductsTranslate* intfAccountProductsTranslate::myInstance;

intfAccountProductsTranslate::intfAccountProductsTranslate(
    const QString& _schema,
    const QList<DBM::clsORMField>& _exclusiveCols,
    const QList<DBM::stuRelation>& _exclusiveRelations,
    const QList<DBM::stuDBIndex>& _exclusiveIndexes
) :
    intfSQLBasedModule(
        _schema,
        tblAccountProductsTranslateBase::Name,
        tblAccountProductsTranslateBase::Private::ORMFields + _exclusiveCols,
        tblAccountProductsTranslateBase::Private::Relations(_schema) + _exclusiveRelations,
        tblAccountProductsTranslateBase::Private::Indexes + _exclusiveIndexes
) {
    intfAccountProductsTranslate::myInstance = this;
}

/******************************************************************/
intfAccountProducts::intfAccountProducts(
    const QString& _schema,
    const QList<DBM::clsORMField>& _exclusiveCols,
    const QList<DBM::stuRelation>& _exclusiveRelations,
    const QList<DBM::stuDBIndex>& _exclusiveIndexes
) :
    intfSQLBasedModule(
        _schema,
        tblAccountProductsBase::Name,
        tblAccountProductsBase::Private::ORMFields + _exclusiveCols,
        tblAccountProductsBase::Private::Relations(_schema) + _exclusiveRelations,
        tblAccountProductsBase::Private::Indexes + _exclusiveIndexes
) { ; }

ORMSelectQuery intfAccountProducts::GetSelectQuery(INTFAPICALLBOOM_IMPL &APICALLBOOM_PARAM, const QString& _alias) {
    intfAccountProductsTranslate::myInstance->prepareFiltersList();

    return intfSQLBasedModule::GetSelectQuery(APICALLBOOM_PARAM, _alias)
            .addCols(this->SelectableColumnNames())
            .removeCols({
                            tblAccountProductsBase::Fields::prdName,
                            tblAccountProductsBase::Fields::prdDesc,
                        })
            .nestedLeftJoin(intfAccountProductsTranslate::myInstance->GetSelectQuery(APICALLBOOM_PARAM)
                      .where({ tblAccountProductsTranslateBase::Fields::language, enuConditionOperator::Equal, APICALLBOOM_PARAM.language() })
                      , "lng_tblAccountProducts"
                      , { "lng_tblAccountProducts", tblAccountProductsTranslateBase::Fields::pid,
                          enuConditionOperator::Equal,
                          tblAccountProductsBase::Name, tblAccountProductsBase::Fields::prdID
                      }
                     )
            .addCol(enuConditionalAggregation::IF,
                    { "lng_tblAccountProducts", tblAccountProductsTranslateBase::Fields::prdName, enuConditionOperator::Null },
                    DBExpression::VALUE(R(_alias.isEmpty() ? tblAccountProductsBase::Name : _alias, tblAccountProductsBase::Fields::prdName)),
                    DBExpression::VALUE(R("lng_tblAccountProducts", tblAccountProductsTranslateBase::Fields::prdName)),
                    tblAccountProductsBase::Fields::prdName
                   )
            .addCol(enuConditionalAggregation::IF,
                    { "lng_tblAccountProducts", tblAccountProductsTranslateBase::Fields::prdDesc, enuConditionOperator::Null },
                    DBExpression::VALUE(R(_alias.isEmpty() ? tblAccountProductsBase::Name : _alias, tblAccountProductsBase::Fields::prdDesc)),
                    DBExpression::VALUE(R("lng_tblAccountProducts", tblAccountProductsTranslateBase::Fields::prdDesc)),
                    tblAccountProductsBase::Fields::prdDesc
                   )
            .inlineInnerJoin(tblAccountUnitsBase::Name/*, tblAccountUnitsBase::Name*/, clsCondition(
                             tblAccountUnitsBase::Name,
                             tblAccountUnitsBase::Fields::untID,
                             enuConditionOperator::Equal,
                             tblAccountProductsBase::Name,
                             tblAccountProductsBase::Fields::prd_untID
                             ))
//            .innerJoinWith(tblAccountProductsBase::Relation::Unit)
//            .addCol(tblAccountUnitsBase::Fields::untName)
    ;
}

#ifdef QT_DEBUG
QVariant IMPL_ANONYMOUSE_ORMGET(intfAccountProducts) {
#else
QVariant IMPL_ORMGET(intfAccountProducts) {
    Authorization::checkPriv(APICALLBOOM_PARAM, this->privOn(EHTTP_GET, this->moduleBaseName()));
#endif

    constexpr quint16 CACHE_TIME = 15 * 60;

    return this->Select(GET_METHOD_ARGS_CALL_INTERNAL_BOOM, {}, CACHE_TIME);
}

quint32 IMPL_ORMCREATE(intfAccountProducts) {
    Authorization::checkPriv(APICALLBOOM_PARAM, this->privOn(EHTTP_PUT, this->moduleBaseName()));

    return this->Create(CREATE_METHOD_ARGS_CALL_INTERNAL_BOOM);
}

bool IMPL_ORMUPDATE(intfAccountProducts) {
    Authorization::checkPriv(APICALLBOOM_PARAM, this->privOn(EHTTP_PATCH, this->moduleBaseName()));

    return this->Update(UPDATE_METHOD_ARGS_CALL_INTERNAL_BOOM);
}

bool IMPL_ORMDELETE(intfAccountProducts) {
  Authorization::checkPriv(APICALLBOOM_PARAM, this->privOn(EHTTP_DELETE, this->moduleBaseName()));

  return this->DeleteByPks(DELETE_METHOD_ARGS_CALL_INTERNAL_BOOM);
}

/******************************************************************/
/******************************************************************/
/******************************************************************/
intfAccountSaleablesTranslate* intfAccountSaleablesTranslate::myInstance;

intfAccountSaleablesTranslate::intfAccountSaleablesTranslate(
    const QString& _schema,
    const QList<DBM::clsORMField>& _exclusiveCols,
    const QList<DBM::stuRelation>& _exclusiveRelations,
    const QList<DBM::stuDBIndex>& _exclusiveIndexes
) :
    intfSQLBasedModule(
        _schema,
        tblAccountSaleablesTranslateBase::Name,
        tblAccountSaleablesTranslateBase::Private::ORMFields + _exclusiveCols,
        tblAccountSaleablesTranslateBase::Private::Relations(_schema) + _exclusiveRelations,
        tblAccountSaleablesTranslateBase::Private::Indexes + _exclusiveIndexes
) {
    intfAccountSaleablesTranslate::myInstance = this;
}

/******************************************************************/
intfAccountSaleables::intfAccountSaleables(
    const QString& _schema,
    const QList<DBM::clsORMField>& _exclusiveCols,
    const QList<DBM::stuRelation>& _exclusiveRelations,
    const QList<DBM::stuDBIndex>& _exclusiveIndexes
) :
    intfSQLBasedModule(
        _schema,
        tblAccountSaleablesBase::Name,
        tblAccountSaleablesBase::Private::ORMFields + _exclusiveCols,
        tblAccountSaleablesBase::Private::Relations(_schema) + _exclusiveRelations,
        tblAccountSaleablesBase::Private::Indexes + _exclusiveIndexes
) { ; }

ORMSelectQuery intfAccountSaleables::GetSelectQuery(INTFAPICALLBOOM_IMPL &APICALLBOOM_PARAM, const QString& _alias) {
    intfAccountSaleablesTranslate::myInstance->prepareFiltersList();

    return intfSQLBasedModule::GetSelectQuery(APICALLBOOM_PARAM, _alias)
            .addCols(this->SelectableColumnNames())
            .removeCols({
                            tblAccountSaleablesBase::Fields::slbName,
                            tblAccountSaleablesBase::Fields::slbDesc,
                        })
            .nestedLeftJoin(intfAccountSaleablesTranslate::myInstance->GetSelectQuery(APICALLBOOM_PARAM)
                      .where({ tblAccountSaleablesTranslateBase::Fields::language, enuConditionOperator::Equal, APICALLBOOM_PARAM.language() })
                      , "lng_tblAccountSaleables"
                      , { "lng_tblAccountSaleables", tblAccountSaleablesTranslateBase::Fields::pid,
                          enuConditionOperator::Equal,
                          tblAccountSaleablesBase::Name, tblAccountSaleablesBase::Fields::slbID
                      }
                     )
            .addCol(enuConditionalAggregation::IF,
                    { "lng_tblAccountSaleables", tblAccountSaleablesTranslateBase::Fields::slbName, enuConditionOperator::Null },
                    DBExpression::VALUE(R(_alias.isEmpty() ? tblAccountSaleablesBase::Name : _alias, tblAccountSaleablesBase::Fields::slbName)),
                    DBExpression::VALUE(R("lng_tblAccountSaleables", tblAccountSaleablesTranslateBase::Fields::slbName)),
                    tblAccountSaleablesBase::Fields::slbName
                   )
            .addCol(enuConditionalAggregation::IF,
                    { "lng_tblAccountSaleables", tblAccountSaleablesTranslateBase::Fields::slbDesc, enuConditionOperator::Null },
                    DBExpression::VALUE(R(_alias.isEmpty() ? tblAccountSaleablesBase::Name : _alias, tblAccountSaleablesBase::Fields::slbDesc)),
                    DBExpression::VALUE(R("lng_tblAccountSaleables", tblAccountSaleablesTranslateBase::Fields::slbDesc)),
                    tblAccountSaleablesBase::Fields::slbDesc
                   )
    ;
}

#ifdef QT_DEBUG
QVariant IMPL_ANONYMOUSE_ORMGET(intfAccountSaleables) {
    clsCondition ExtraFilters = {};
#else
QVariant IMPL_ORMGET(intfAccountSaleables) {
    clsCondition ExtraFilters = {};
    if (Authorization::hasPriv(APICALLBOOM_PARAM, this->privOn(EHTTP_GET, this->moduleBaseName())) == false)
        ExtraFilters
            .setCond({ tblAccountSaleablesBase::Fields::slbAvailableFromDate, enuConditionOperator::LessEqual, DBExpression::NOW() })
            .andCond(clsCondition({ tblAccountSaleablesBase::Fields::slbAvailableToDate, enuConditionOperator::Null })
                .orCond({ tblAccountSaleablesBase::Fields::slbAvailableToDate, enuConditionOperator::GreaterEqual,
                          DBExpression::DATE_ADD(DBExpression::NOW(), 15, enuDBExpressionIntervalUnit::MINUTE) })
            );
#endif

    constexpr quint16 CACHE_TIME = 15 * 60;

    auto fnTouchQuery = [/*this, */&_cols](ORMSelectQuery &_query) {
        if (_cols.isEmpty()) {
//            _query
                //will be adding in GetSelectQuery
//                .addCols(this->SelectableColumnNames())
//            ;
        } else {
            _query.addCSVCols(_cols);
            _cols = {};
        }

        _query
            .addCol(DBExpression::VALUE("slbInStockQty - IFNULL(slbOrderedQty,0) + IFNULL(slbReturnedQty,0)"), "slbQtyInHand")
//            .addCol(DBExpression::VALUE("prdInStockQty - IFNULL(prdOrderedQty,0) + IFNULL(prdReturnedQty,0)"), "prdQtyInHand")
//            .innerJoinWith(tblAccountSaleablesBase::Relation::Product)
        ;

        _query.inlineInnerJoin(tblAccountProductsBase::Name/*, tblAccountProductsBase::Name*/, clsCondition(
                                   tblAccountProductsBase::Name,
                                   tblAccountProductsBase::Fields::prdID,
                                   enuConditionOperator::Equal,
                                   tblAccountSaleablesBase::Name,
                                   tblAccountSaleablesBase::Fields::slb_prdID
                                   ));
    };

    return this->Select(GET_METHOD_ARGS_CALL_INTERNAL_BOOM, ExtraFilters, CACHE_TIME, fnTouchQuery);
}

quint32 IMPL_ORMCREATE(intfAccountSaleables) {
    Authorization::checkPriv(APICALLBOOM_PARAM, this->privOn(EHTTP_PUT, this->moduleBaseName()));

    return this->Create(CREATE_METHOD_ARGS_CALL_INTERNAL_BOOM);
}

bool IMPL_ORMUPDATE(intfAccountSaleables) {
    Authorization::checkPriv(APICALLBOOM_PARAM, this->privOn(EHTTP_PATCH, this->moduleBaseName()));

    return this->Update(UPDATE_METHOD_ARGS_CALL_INTERNAL_BOOM);
}

bool IMPL_ORMDELETE(intfAccountSaleables) {
  Authorization::checkPriv(APICALLBOOM_PARAM, this->privOn(EHTTP_DELETE, this->moduleBaseName()));

  return this->DeleteByPks(DELETE_METHOD_ARGS_CALL_INTERNAL_BOOM);
}

/******************************************************************/
intfAccountUserAssets::intfAccountUserAssets(
    const QString& _schema,
    const QList<DBM::clsORMField>& _exclusiveCols,
    const QList<DBM::stuRelation>& _exclusiveRelations,
    const QList<stuDBIndex>& _exclusiveIndexes
) :
    intfSQLBasedModule(
        _schema,
        tblAccountUserAssetsBase::Name,
        tblAccountUserAssetsBase::Private::ORMFields + _exclusiveCols,
        tblAccountUserAssetsBase::Private::Relations(_schema) + _exclusiveRelations,
        tblAccountUserAssetsBase::Private::Indexes + _exclusiveIndexes
) { ; }

QVariant IMPL_ORMGET(intfAccountUserAssets) {
  if (Authorization::hasPriv(APICALLBOOM_PARAM, this->privOn(EHTTP_GET, this->moduleBaseName())) == false)
    this->setSelfFilters({{tblAccountUserAssetsBase::Fields::uas_usrID, APICALLBOOM_PARAM.getUserID() }}, _filters);

  return this->Select(GET_METHOD_ARGS_CALL_INTERNAL_BOOM);
}

bool IMPL_REST_UPDATE(intfAccountUserAssets, setAsPrefered, (
    APICALLBOOM_TYPE_JWT_IMPL &APICALLBOOM_PARAM,
    TAPI::PKsByPath_t _pksByPath
)) {
  bool Ok;
  quint64 UserPackageID = _pksByPath.toUInt(&Ok);
  if (!Ok || !UserPackageID )
    throw exHTTPBadRequest("Invalid UserPackageID provided");

  this->callSP(APICALLBOOM_PARAM,
               "spUserAsset_SetAsPrefered", {
                   { "iUserID", APICALLBOOM_PARAM.getUserID() },
                   { "iUASID",  UserPackageID },
               });
  return false;
}

bool IMPL_REST_UPDATE(intfAccountUserAssets, disablePackage, (
    APICALLBOOM_TYPE_JWT_IMPL &APICALLBOOM_PARAM,
    TAPI::PKsByPath_t _pksByPath
)) {
  bool Ok;
  quint64 UserPackageID = _pksByPath.toUInt(&Ok);
  if (!Ok || !UserPackageID )
    throw exHTTPBadRequest("Invalid UserPackageID provided");

  Authorization::checkPriv(APICALLBOOM_PARAM, this->privOn(EHTTP_PATCH, this->moduleBaseName()));
  /*return this->update(APICALLBOOM_PARAM.getUserID(), {
                            {tblAccountUserAssets::Fields::uasID, UserPackageID}
                        }, {
                            {tblAccountUserAssets::Fields::uasStatus, TAPI::enuAuditableStatus::Banned},
                        });*/
  throw Targoman::Common::exTargomanMustBeImplemented(__FUNCTION__);
  return Ok;
}

/******************************************************************/
intfAccountAssetUsage::intfAccountAssetUsage(
    const QString& _schema,
    const QList<DBM::clsORMField>& _exclusiveCols,
    const QList<DBM::stuRelation>& _exclusiveRelations,
    const QList<stuDBIndex>& _exclusiveIndexes
) :
    intfSQLBasedModule(
        _schema,
        tblAccountAssetUsageBase::Name,
        tblAccountAssetUsageBase::Private::ORMFields + _exclusiveCols,
        tblAccountAssetUsageBase::Private::Relations(_schema) + _exclusiveRelations,
        tblAccountAssetUsageBase::Private::Indexes + _exclusiveIndexes
) { ; }

QVariant IMPL_ORMGET(intfAccountAssetUsage) {
    if (Authorization::hasPriv(APICALLBOOM_PARAM, this->privOn(EHTTP_GET, this->moduleBaseName())) == false)
      this->setSelfFilters({{tblAccountUserAssetsBase::Fields::uas_usrID, APICALLBOOM_PARAM.getUserID()}}, _filters);

    return this->Select(GET_METHOD_ARGS_CALL_INTERNAL_BOOM);
}

/******************************************************************/
intfAccountCoupons::intfAccountCoupons(
    const QString& _schema,
    const QList<DBM::clsORMField>& _exclusiveCols,
    const QList<DBM::stuRelation>& _exclusiveRelations,
    const QList<stuDBIndex>& _exclusiveIndexes
) :
    intfSQLBasedModule(
        _schema,
        tblAccountCouponsBase::Name,
        tblAccountCouponsBase::Private::ORMFields + _exclusiveCols,
        tblAccountCouponsBase::Private::Relations(_schema) + _exclusiveRelations,
        tblAccountCouponsBase::Private::Indexes + _exclusiveIndexes
) { ; }

QVariant IMPL_ORMGET(intfAccountCoupons) {
  Authorization::checkPriv(APICALLBOOM_PARAM, this->privOn(EHTTP_GET, this->moduleBaseName()));

  return this->Select(GET_METHOD_ARGS_CALL_INTERNAL_BOOM);

//  return query.one();

//    return this->selectFromTable({}, {}, GET_METHOD_CALL_ARGS_APICALL);
}

quint32 IMPL_ORMCREATE(intfAccountCoupons) {
    Authorization::checkPriv(APICALLBOOM_PARAM, this->privOn(EHTTP_PUT, this->moduleBaseName()));

    return this->Create(CREATE_METHOD_ARGS_CALL_INTERNAL_BOOM);
}

bool IMPL_ORMUPDATE(intfAccountCoupons) {
    Authorization::checkPriv(APICALLBOOM_PARAM, this->privOn(EHTTP_PATCH, this->moduleBaseName()));

    return this->Update(UPDATE_METHOD_ARGS_CALL_INTERNAL_BOOM);
}

bool IMPL_ORMDELETE(intfAccountCoupons) {
  Authorization::checkPriv(APICALLBOOM_PARAM, this->privOn(EHTTP_DELETE, this->moduleBaseName()));

  return this->DeleteByPks(DELETE_METHOD_ARGS_CALL_INTERNAL_BOOM);
}

/******************************************************************/
intfAccountPrizes::intfAccountPrizes(
    const QString& _schema,
    const QList<DBM::clsORMField>& _exclusiveCols,
    const QList<DBM::stuRelation>& _exclusiveRelations,
    const QList<stuDBIndex>& _exclusiveIndexes
) :
    intfSQLBasedModule(
        _schema,
        tblAccountPrizesBase::Name,
        tblAccountPrizesBase::Private::ORMFields + _exclusiveCols,
        tblAccountPrizesBase::Private::Relations(_schema) + _exclusiveRelations,
        tblAccountPrizesBase::Private::Indexes + _exclusiveIndexes
) { ; }

QVariant IMPL_ORMGET(intfAccountPrizes) {
  Authorization::checkPriv(APICALLBOOM_PARAM, this->privOn(EHTTP_GET, this->moduleBaseName()));

  return this->Select(GET_METHOD_ARGS_CALL_INTERNAL_BOOM);

//  return query.one();

  //  return this->selectFromTable({}, {}, GET_METHOD_CALL_ARGS_APICALL);
}

quint32 IMPL_ORMCREATE(intfAccountPrizes) {
    Authorization::checkPriv(APICALLBOOM_PARAM, this->privOn(EHTTP_PUT, this->moduleBaseName()));

    return this->Create(CREATE_METHOD_ARGS_CALL_INTERNAL_BOOM);
}

bool IMPL_ORMUPDATE(intfAccountPrizes) {
    Authorization::checkPriv(APICALLBOOM_PARAM, this->privOn(EHTTP_PATCH, this->moduleBaseName()));

    return this->Update(UPDATE_METHOD_ARGS_CALL_INTERNAL_BOOM);
}

bool IMPL_ORMDELETE(intfAccountPrizes) {
  Authorization::checkPriv(APICALLBOOM_PARAM, this->privOn(EHTTP_DELETE, this->moduleBaseName()));

  return this->DeleteByPks(DELETE_METHOD_ARGS_CALL_INTERNAL_BOOM);
}

//} //namespace Targoman::API::AAA {

/******************************************************************/
//namespace TAPI {

//using namespace Targoman::API::AAA;

//stuActiveCredit::stuActiveCredit(const stuAssetItem& _credit, bool _isFromParent, const UsageLimits_t& _myLimitsOnParent, qint64 _ttl) :
//  Credit(_credit),
//  IsFromParent(_isFromParent),
//  MyLimitsOnParent(_myLimitsOnParent),
//  TTL(_ttl) { ; }

//QJsonObject stuActiveCredit::toJson(bool _full) {
//  QJsonObject Account = {
//    { ASA_PACKAGE, this->Credit.toJson(_full) },
//    { ASA_TTL, static_cast<double>(this->TTL) },
//  };
//  if (this->IsFromParent)
//    Account.insert(ASA_ISFROMPARENT, true);
//  for (auto LimitIter = this->MyLimitsOnParent.begin();
//      LimitIter != this->MyLimitsOnParent.end();
//      LimitIter++)
//    Account.insert(LimitIter.key(), LimitIter->toJson());
//  return Account;
//}

//stuActiveCredit& stuActiveCredit::fromJson(const QJsonObject _obj) {
//  this->Credit = stuAssetItem().fromJson(_obj.value(ASA_PACKAGE).toObject());
//  this->IsFromParent  = _obj.value(ASA_ISFROMPARENT).toBool();
//  this->TTL = static_cast<qint64>(_obj.value(ASA_TTL).toDouble());
//  QJsonObject Limits = _obj.value(ASA_LIMITSONPARENT).toObject();
//  for (auto LimitIter = Limits.begin();
//      LimitIter != Limits.end();
//      LimitIter++)
//    this->MyLimitsOnParent.insert(LimitIter.key(), stuUsage().fromJson(LimitIter->toObject()));

//  return *this;
//}

/******************************************************************/
stuServiceCreditsInfo::stuServiceCreditsInfo(
        ActiveCredits_t       _activeCredits,
        NULLABLE_TYPE(stuAssetItem) _preferedCredit,
        NULLABLE_TYPE(quint32)      _parentID,
        UsageLimits_t          _myLimitsOnParent,
        QDateTime              _dbCurrentDateTime) :
    ActiveCredits(_activeCredits),
    PreferedCredit(_preferedCredit),
    ParentID(_parentID),
    MyLimitsOnParent(_myLimitsOnParent),
    DBCurrentDateTime(_dbCurrentDateTime) { ; }

//QJsonObject stuAssetItem::toJson(bool _full) {
//    ///@TODO: very important: Complete this
//  QJsonObject Info;
/*
  if (this->PackageID > 0)       Info[PKG_ID] = static_cast<double>(this->PackageID);
  if (this->PackageCode > -1)    Info[PKG_CODE] = this->PackageCode;
  if (this->RemainingDays > -1)  Info[PKG_REMAININGDAYS] = this->RemainingDays;
  if (this->RemainingHours > -1) Info[PKG_REMAININGHOURS] = this->RemainingHours;
  if (this->StartDate.isValid()) Info[PKG_STARTDATE] = this->StartDate.toString();
  if (this->EndDate.isValid())   Info[PKG_ENDDATE] = this->EndDate.toString();
  if (this->StartTime.isValid()) Info[PKG_STARTTIME] = this->StartTime.toString();
  if (this->EndTime.isValid())   Info[PKG_ENDTIME] = this->EndTime.toString();
  if (this->Properties.size())   Info[PKG_PROPS] = this->Properties;
  if (_full) {
    QJsonObject Limits;
    for (auto LimitIter = this->Remaining.begin();
        LimitIter != this->Remaining.end();
        LimitIter++)
      Limits.insert(LimitIter.key(), LimitIter->toJson());
    Info[PKG_LIMITS] = Limits;
  }*/
//  return Info;
//}

//stuAssetItem &stuAssetItem::fromJson(const QJsonObject& _obj) {
//    ///@TODO: very important: Complete this
//  return *this;
/*
this->PackageID = static_cast<quint64>(_obj.contains(PKG_ID) ? _obj.value(PKG_ID).toDouble() : 0);
this->PackageCode = _obj.contains(PKG_CODE) ? _obj.value(PKG_CODE).toString() : QString();
this->RemainingDays = static_cast<qint32>(_obj.contains(PKG_REMAININGDAYS) ? _obj.value(PKG_REMAININGDAYS).toInt() : -1);
this->RemainingHours = static_cast<qint8>(_obj.contains(PKG_REMAININGHOURS) ? _obj.value(PKG_REMAININGHOURS).toInt() : -1);
this->StartDate = _obj.contains(PKG_STARTDATE) ? QDate::fromString(_obj.value(PKG_STARTDATE).toString()) : QDate();
this->EndDate = _obj.contains(PKG_ENDDATE) ? QDate::fromString(_obj.value(PKG_ENDDATE).toString()) : QDate();
this->StartTime = _obj.contains(PKG_STARTTIME) ? QTime::fromString(_obj.value(PKG_STARTTIME).toString()) : QTime();
this->EndTime = _obj.contains(PKG_ENDTIME) ? QTime::fromString(_obj.value(PKG_ENDTIME).toString()) : QTime();
this->Properties = _obj.value(PKG_PROPS).toObject();
QJsonObject Limits = _obj.value(PKG_LIMITS).toObject();
for (auto LimitIter = Limits.begin();
   LimitIter != Limits.end();
   LimitIter++)
 this->Remaining.insert(LimitIter.key(), stuUsage().fromJson(LimitIter->toObject()));
 */
 //}

/******************************************************************/
/*QVariant stuAssetItemReq_t::toVariant() const{
    QVariantMap Value;
    for (auto Iter = this->begin(); Iter != this->end(); ++Iter)
        Value.insert(Iter.key(), Iter.value());

    return Value;
}

stuAssetItemReq_t& stuAssetItemReq_t::fromVariant(const QVariant& _value, const QByteArray& _paramName) {
    if (_value.canConvert<QMap<QString, QVariant>>() == false)
        throw exHTTPBadRequest("Invalid value specified for parameter:" + _paramName);
    this->clear();
    const QMap<QString, QVariant>& Map = _value.toMap();
    for (auto Iter = Map.begin (); Iter != Map.end (); ++Iter)
        this->insert(Iter.key(), static_cast<qint16>(Iter.value().toInt()));
    return  *this;
}*/

}
