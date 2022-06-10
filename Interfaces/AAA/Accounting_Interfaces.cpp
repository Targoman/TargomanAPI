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

TAPI_REGISTER_TARGOMAN_ENUM(Targoman::API::AAA, enuVoucherType);
TAPI_REGISTER_TARGOMAN_ENUM(Targoman::API::AAA, enuVoucherStatus);
TAPI_REGISTER_TARGOMAN_ENUM(Targoman::API::AAA, enuDiscountType);
TAPI_REGISTER_TARGOMAN_ENUM(Targoman::API::AAA, enuVoucherItemProcessStatus);

TAPI_REGISTER_METATYPE(
    /* complexity         */ COMPLEXITY_Object,
    /* namespace          */ Targoman::API::AAA,
    /* type               */ stuVoucherItem,
    /* fnToVariant    */ [](const stuVoucherItem& _value) -> QVariant {
//        qDebug() << "stuVoucherItem(1) ================================= round:" << _value.Round;
        return _value.toJson().toVariantMap();
    },
    /* fnFromVariant  */ [](const QVariant& _value, Q_DECL_UNUSED const QString& _paramName = "") -> stuVoucherItem {
//        qDebug() << "stuVoucherItem(2) =================================" << _paramName << ":" << _value;
        if (_value.isValid() == false) {
//            qDebug() << "stuVoucherItem(2.1) =================================" << _paramName << ":" << _value;
            return stuVoucherItem();
        }

        if (_value.canConvert<QVariantMap>()
//                || _value.canConvert<QVariantList>()
//                || _value.canConvert<double>()
            ) {
            auto ret = QJsonDocument::fromVariant(_value);
//            qDebug() << "stuVoucherItem(2.2) =================================" << _paramName << ":" << _value << "=" << ret.object();
            return stuVoucherItem().fromJson(ret.object());
        }

        if (_value.toString().isEmpty()) {
//            qDebug() << "stuVoucherItem(2.3) =================================" << _paramName << ":" << _value;
            return stuVoucherItem();
        }

        QJsonParseError Error;
        QJsonDocument Doc;
        Doc = Doc.fromJson(_value.toString().toUtf8(), &Error);
//        qDebug() << "stuVoucherItem(2.4) =================================" << _paramName << ":" << _value << "=" << Doc;

        if (Error.error != QJsonParseError::NoError)
            throw exHTTPBadRequest(_paramName + " is not a valid Prevoucher: <"+_value.toString()+">" + Error.errorString());
        if (Doc.isObject() == false)
            throw exHTTPBadRequest(_paramName + " is not a valid Prevoucher object: <"+_value.toString()+">");
        return stuVoucherItem().fromJson(Doc.object());
    }
);

TAPI_REGISTER_METATYPE(
    /* complexity         */ COMPLEXITY_Object,
    /* namespace          */ Targoman::API::AAA,
    /* type               */ stuPreVoucher,
    /* fnToVariant    */ [](const stuPreVoucher& _value) -> QVariant { return _value.toJson().toVariantMap(); },
    /* fnFromVariant  */ [](const QVariant& _value, Q_DECL_UNUSED const QString& _paramName = "") -> stuPreVoucher {
        if (_value.isValid() == false) {
            return stuPreVoucher();
        }

        if (_value.canConvert<QVariantMap>()
//                || _value.canConvert<QVariantList>()
//                || _value.canConvert<double>()
            ) {
            auto ret = QJsonDocument::fromVariant(_value);
            return stuPreVoucher().fromJson(ret.object());
        }

        if (_value.toString().isEmpty()) {
            return stuPreVoucher();
        }

        QJsonParseError Error;
        QJsonDocument Doc;
        Doc = Doc.fromJson(_value.toString().toUtf8(), &Error);

        if (Error.error != QJsonParseError::NoError)
            throw exHTTPBadRequest(_paramName + " is not a valid Prevoucher: <"+_value.toString()+">" + Error.errorString());
        if (Doc.isObject() == false)
            throw exHTTPBadRequest(_paramName + " is not a valid Prevoucher object: <"+_value.toString()+">");
        return stuPreVoucher().fromJson(Doc.object());
    }
);

TAPI_REGISTER_METATYPE(
    /* complexity         */ COMPLEXITY_Complex,
    /* namespace          */ Targoman::API::AAA,
    /* type               */ stuVoucher,
    /* fnToVariant    */ [](const stuVoucher& _value) -> QVariant { return _value.toJson().toVariantMap(); }
);

TAPI_REGISTER_METATYPE(
    /* complexity         */ COMPLEXITY_Complex,
    /* namespace          */ Targoman::API::AAA,
    /* type               */ stuVoucherItemForTrustedAction,
    /* fnToVariant    */ [](const stuVoucherItemForTrustedAction& _value) -> QVariant { return _value.toJson().toVariantMap(); },
    /* fnFromVariant  */ [](const QVariant& _value, Q_DECL_UNUSED const QString& _paramName = "") -> stuVoucherItemForTrustedAction {
    //        qDebug() << "stuVoucherItemForTrustedAction(2) =================================" << _paramName << ":" << _value;
        if (_value.isValid() == false) {
    //            qDebug() << "stuVoucherItemForTrustedAction(2.1) =================================" << _paramName << ":" << _value;
            return stuVoucherItemForTrustedAction();
        }

        if (_value.canConvert<QVariantMap>()
    //                || _value.canConvert<QVariantList>()
    //                || _value.canConvert<double>()
            ) {
            auto ret = QJsonDocument::fromVariant(_value);
    //            qDebug() << "stuVoucherItemForTrustedAction(2.2) =================================" << _paramName << ":" << _value << "=" << ret.object();
            return stuVoucherItemForTrustedAction().fromJson(ret.object());
        }

        if (_value.toString().isEmpty()) {
    //            qDebug() << "stuVoucherItemForTrustedAction(2.3) =================================" << _paramName << ":" << _value;
            return stuVoucherItemForTrustedAction();
        }

        QJsonParseError Error;
        QJsonDocument Doc;
        Doc = Doc.fromJson(_value.toString().toUtf8(), &Error);
    //        qDebug() << "stuVoucherItemForTrustedAction(2.4) =================================" << _paramName << ":" << _value << "=" << Doc;

        if (Error.error != QJsonParseError::NoError)
            throw exHTTPBadRequest(_paramName + " is not a valid VoucherItemForTrustedAction: <"+_value.toString()+">" + Error.errorString());
        if (Doc.isObject() == false)
            throw exHTTPBadRequest(_paramName + " is not a valid VoucherItemForTrustedAction object: <"+_value.toString()+">");
        return stuVoucherItemForTrustedAction().fromJson(Doc.object());
    }
);

TAPI_REGISTER_METATYPE(
    /* complexity         */ COMPLEXITY_Complex,
    /* namespace          */ Targoman::API::AAA,
    /* type               */ OrderAdditives_t,
    /* fnToVariant    */ [](const OrderAdditives_t& _value) -> QVariant { return QVariant::fromValue(_value); },
    /* fnFromVariant  */ [](const QVariant& _value, const QByteArray&) -> OrderAdditives_t {
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

QVariant IMPL_ORMGET(intfAccountProducts) {
    Authorization::checkPriv(_APICALLBOOM, this->privOn(EHTTP_GET, this->moduleBaseName()));

    constexpr quint16 CACHE_TIME = 15 * 60;
    return this->Select(GET_METHOD_ARGS_CALL_INTERNAL_BOOM, {}, CACHE_TIME);
}

quint32 IMPL_ORMCREATE(intfAccountProducts) {
    Authorization::checkPriv(_APICALLBOOM, this->privOn(EHTTP_PUT, this->moduleBaseName()));

    return this->Create(CREATE_METHOD_ARGS_CALL_INTERNAL_BOOM);
}

bool IMPL_ORMUPDATE(intfAccountProducts) {
    Authorization::checkPriv(_APICALLBOOM, this->privOn(EHTTP_PATCH, this->moduleBaseName()));

    return this->Update(UPDATE_METHOD_ARGS_CALL_INTERNAL_BOOM);
}

bool IMPL_ORMDELETE(intfAccountProducts) {
  Authorization::checkPriv(_APICALLBOOM, this->privOn(EHTTP_DELETE, this->moduleBaseName()));

  return this->DeleteByPks(DELETE_METHOD_ARGS_CALL_INTERNAL_BOOM);
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

QVariant IMPL_ORMGET(intfAccountSaleables) {
//    QString ExtraFilters;
//    if (Authorization::hasPriv(_APICALLBOOM, this->privOn(EHTTP_GET, this->moduleBaseName())) == false)
//        ExtraFilters = QString ("%1<=NOW() + ( %2=NULL | %2>=DATE_ADD(NOW(),INTERVAL$SPACE$15$SPACEMIN) )")
//                       .arg(tblAccountSaleablesBase::Fields::slbAvailableFromDate)
//                       .arg(tblAccountSaleablesBase::Fields::slbAvailableToDate);

    clsCondition ExtraFilters = {};
    if (Authorization::hasPriv(_APICALLBOOM, this->privOn(EHTTP_GET, this->moduleBaseName())) == false)
        ExtraFilters
            .setCond({ tblAccountSaleablesBase::Fields::slbAvailableFromDate, enuConditionOperator::LessEqual, DBExpression::NOW() })
            .andCond(clsCondition({ tblAccountSaleablesBase::Fields::slbAvailableToDate, enuConditionOperator::Null })
                .orCond({ tblAccountSaleablesBase::Fields::slbAvailableToDate, enuConditionOperator::GreaterEqual,
                          DBExpression::DATE_ADD(DBExpression::NOW(), 15, enuDBExpressionIntervalUnit::MINUTE) })
            );

    constexpr quint16 CACHE_TIME = 15 * 60;
    return this->Select(GET_METHOD_ARGS_CALL_INTERNAL_BOOM, ExtraFilters, CACHE_TIME);
}

quint32 IMPL_ORMCREATE(intfAccountSaleables) {
    Authorization::checkPriv(_APICALLBOOM, this->privOn(EHTTP_PUT, this->moduleBaseName()));

    return this->Create(CREATE_METHOD_ARGS_CALL_INTERNAL_BOOM);
}

bool IMPL_ORMUPDATE(intfAccountSaleables) {
    Authorization::checkPriv(_APICALLBOOM, this->privOn(EHTTP_PATCH, this->moduleBaseName()));

    return this->Update(UPDATE_METHOD_ARGS_CALL_INTERNAL_BOOM);
}

bool IMPL_ORMDELETE(intfAccountSaleables) {
  Authorization::checkPriv(_APICALLBOOM, this->privOn(EHTTP_DELETE, this->moduleBaseName()));

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
  if (Authorization::hasPriv(_APICALLBOOM, this->privOn(EHTTP_GET, this->moduleBaseName())) == false)
    this->setSelfFilters({{tblAccountUserAssetsBase::Fields::uas_usrID, _APICALLBOOM.getUserID() }}, _filters);

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
                   { "iUserID", _APICALLBOOM.getUserID() },
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

  Authorization::checkPriv(_APICALLBOOM, this->privOn(EHTTP_PATCH, this->moduleBaseName()));
  /*return this->update(_APICALLBOOM.getUserID(), {
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
    if (Authorization::hasPriv(_APICALLBOOM, this->privOn(EHTTP_GET, this->moduleBaseName())) == false)
      this->setSelfFilters({{tblAccountUserAssetsBase::Fields::uas_usrID, _APICALLBOOM.getUserID()}}, _filters);

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
  Authorization::checkPriv(_APICALLBOOM, this->privOn(EHTTP_GET, this->moduleBaseName()));

  return this->Select(GET_METHOD_ARGS_CALL_INTERNAL_BOOM);

//  return query.one();

//    return this->selectFromTable({}, {}, GET_METHOD_CALL_ARGS_APICALL);
}

quint32 IMPL_ORMCREATE(intfAccountCoupons) {
    Authorization::checkPriv(_APICALLBOOM, this->privOn(EHTTP_PUT, this->moduleBaseName()));

    return this->Create(CREATE_METHOD_ARGS_CALL_INTERNAL_BOOM);
}

bool IMPL_ORMUPDATE(intfAccountCoupons) {
    Authorization::checkPriv(_APICALLBOOM, this->privOn(EHTTP_PATCH, this->moduleBaseName()));

    return this->Update(UPDATE_METHOD_ARGS_CALL_INTERNAL_BOOM);
}

bool IMPL_ORMDELETE(intfAccountCoupons) {
  Authorization::checkPriv(_APICALLBOOM, this->privOn(EHTTP_DELETE, this->moduleBaseName()));

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
  Authorization::checkPriv(_APICALLBOOM, this->privOn(EHTTP_GET, this->moduleBaseName()));

  return this->Select(GET_METHOD_ARGS_CALL_INTERNAL_BOOM);

//  return query.one();

  //  return this->selectFromTable({}, {}, GET_METHOD_CALL_ARGS_APICALL);
}

quint32 IMPL_ORMCREATE(intfAccountPrizes) {
    Authorization::checkPriv(_APICALLBOOM, this->privOn(EHTTP_PUT, this->moduleBaseName()));

    return this->Create(CREATE_METHOD_ARGS_CALL_INTERNAL_BOOM);
}

bool IMPL_ORMUPDATE(intfAccountPrizes) {
    Authorization::checkPriv(_APICALLBOOM, this->privOn(EHTTP_PATCH, this->moduleBaseName()));

    return this->Update(UPDATE_METHOD_ARGS_CALL_INTERNAL_BOOM);
}

bool IMPL_ORMDELETE(intfAccountPrizes) {
  Authorization::checkPriv(_APICALLBOOM, this->privOn(EHTTP_DELETE, this->moduleBaseName()));

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
//    ///TODO: very important: Complete this
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
//    ///TODO: very important: Complete this
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
