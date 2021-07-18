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

#include "Accounting.h"
#include "PrivHelpers.h"
#include "Accounting_Interfaces.h"
#include "Server/ServerConfigs.h"
#include "Interfaces/AAA/Authorization.h"
#include "Interfaces/Common/QtTypes.hpp"

using namespace Targoman::API;

#include "Interfaces/ORM/APIQueryBuilders.h"

TAPI_REGISTER_METATYPE(
    COMPLEXITY_Complex,
    TAPI, stuVoucher,
    [](const TAPI::stuVoucher& _value) -> QVariant{return _value.toJson().toVariantMap();}
);

TAPI_REGISTER_METATYPE(
    /* complexity         */ COMPLEXITY_Object,
    /* namespace          */ TAPI,
    /* type               */ stuPreVoucher,
    /* toVariantLambda    */ [](const TAPI::stuPreVoucher& _value) -> QVariant {
//        qDebug() << "stuPreVoucher(1) ================================= round:" << _value.Round;
        return _value.toJson().toVariantMap();
    },
    /* fromVariantLambda  */ [](const QVariant& _value, const QByteArray& _paramName) -> TAPI::stuPreVoucher {
//        qDebug() << "stuPreVoucher(2) =================================" << _paramName << ":" << _value;
        if (_value.isValid() == false)
        {
//            qDebug() << "stuPreVoucher(2.1) =================================" << _paramName << ":" << _value;
            return TAPI::stuPreVoucher();
        }

        if (_value.canConvert<QVariantMap>()
//                || _value.canConvert<QVariantList>()
//                || _value.canConvert<double>()
            )
        {
            auto ret = QJsonDocument::fromVariant(_value);
//            qDebug() << "stuPreVoucher(2.2) =================================" << _paramName << ":" << _value << "=" << ret.object();
            return TAPI::stuPreVoucher().fromJson(ret.object());
        }

        if (_value.toString().isEmpty())
        {
//            qDebug() << "stuPreVoucher(2.3) =================================" << _paramName << ":" << _value;
            return TAPI::stuPreVoucher();
        }

        QJsonParseError Error;
        QJsonDocument Doc;
        Doc = Doc.fromJson(_value.toString().toUtf8(), &Error);
//        qDebug() << "stuPreVoucher(2.4) =================================" << _paramName << ":" << _value << "=" << Doc;

        if (Error.error != QJsonParseError::NoError)
            throw exHTTPBadRequest(_paramName + " is not a valid Prevoucher: <"+_value.toString()+">" + Error.errorString());
        if (Doc.isObject() == false)
            throw exHTTPBadRequest(_paramName + " is not a valid Prevoucher object: <"+_value.toString()+">");
        return TAPI::stuPreVoucher().fromJson(Doc.object());
    }
);

TAPI_REGISTER_METATYPE(
    COMPLEXITY_Complex,
    TAPI, OrderAdditives_t,
    [](const TAPI::OrderAdditives_t& _value) -> QVariant{return QVariant::fromValue(_value);},
    [](const QVariant& _value, const QByteArray&) -> TAPI::OrderAdditives_t {
        auto Map = _value.toMap();
        TAPI::OrderAdditives_t Additives;
        for(auto Iter = Map.begin(); Iter != Map.end(); ++Iter)
            Additives.insert(Iter.key(), Iter.value().toString());
        return Additives;
    }
);

TAPI_REGISTER_TARGOMAN_ENUM(TAPI, enuVoucherStatus);
TAPI_REGISTER_TARGOMAN_ENUM(TAPI, enuDiscountType);


namespace Targoman {
namespace API {
namespace AAA {
namespace Accounting {

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

constexpr char ASA_PACKAGE[] = "PK";
constexpr char ASA_ISFROMPARENT[] = "IP";
constexpr char ASA_TTL[] = "TT";
constexpr char ASA_LIMITSONPARENT[] = "LP";

/******************************************************************/
intfAccountProducts::intfAccountProducts(
        const QString& _schema,
        const QList<ORM::clsORMField>& _exclusiveCols,
        const QList<ORM::stuRelation>& _exclusiveRelations,
        const QList<ORM::stuDBIndex>& _exclusiveIndexes
    )
    : clsTable(_schema,
        tblAccountProductsBase::Name,
        QList<ORM::clsORMField>({
        ///<  ColName                                      Type                      Validation                                Default     UpBy   Sort  Filter Self  Virt   PK
            { tblAccountProductsBase::prdID,               ORM_PRIMARY_KEY32 },
            { tblAccountProductsBase::prdCode,             S(TAPI::ProductCode_t),    QFV,                                     QRequired,  UPOwner },
            { tblAccountProductsBase::prdName,             S(QString),                QFV,                                     QRequired,  UPOwner },
            { tblAccountProductsBase::prdDesc,             S(QString),                QFV,                                     QNull,      UPOwner },
            { tblAccountProductsBase::prdValidFromDate,    S(TAPI::Date_t),           QFV,                                     QNull,      UPOwner },
            { tblAccountProductsBase::prdValidToDate,      S(TAPI::Date_t),           QFV,                                     QNull,      UPOwner },
            { tblAccountProductsBase::prdValidFromHour,    S(NULLABLE_TYPE(quint8)),  QFV.integer().minValue(0).maxValue(23),  QNull,      UPOwner },
            { tblAccountProductsBase::prdValidToHour,      S(NULLABLE_TYPE(quint8)),  QFV.integer().minValue(0).maxValue(23),  QNull,      UPOwner },
            { tblAccountProductsBase::prdPrivs,            S(TAPI::PrivObject_t),     QFV,                                     QNull,      UPOwner },
            { tblAccountProductsBase::prdVAT,              S(NULLABLE_TYPE(double)),  QFV.real().minValue(0).maxValue(100),    QNull,      UPOwner },
            { tblAccountProductsBase::prdInStockCount,     S(quint32),                QFV.integer().minValue(0),               QRequired,  UPAdmin },
            { tblAccountProductsBase::prdOrderedCount,     S(NULLABLE_TYPE(quint32)), QFV,                                     QNull,      UPNone },
            { tblAccountProductsBase::prdReturnedCount,    S(NULLABLE_TYPE(quint32)), QFV,                                     QNull,      UPNone },
            { tblAccountProductsBase::prdStatus,           S(TAPI::enuGenericStatus::Type), QFV,                               TAPI::enuGenericStatus::Active, UPStatus },
            { tblAccountProductsBase::prdCreatedBy_usrID,  ORM_CREATED_BY },
            { tblAccountProductsBase::prdCreationDateTime, ORM_CREATED_ON },
            { tblAccountProductsBase::prdUpdatedBy_usrID,  ORM_UPDATED_BY },
        }) + _exclusiveCols,
        QList<ORM::stuRelation>({
        ///<  Col                                         Reference Table    ForeignCol          Rename     LeftJoin
            ORM_RELATION_OF_CREATOR(tblAccountProductsBase::prdCreatedBy_usrID),
            ORM_RELATION_OF_UPDATER(tblAccountProductsBase::prdUpdatedBy_usrID),
        }) + _exclusiveRelations,
        QList<ORM::stuDBIndex>({
            { { tblAccountProductsBase::prdCode, tblAccountProductsBase::prdStatus }, enuDBIndex::Unique },
            { tblAccountProductsBase::prdValidFromDate },
            { tblAccountProductsBase::prdValidToDate },
            { tblAccountProductsBase::prdValidFromHour },
            { tblAccountProductsBase::prdValidToHour },
            { tblAccountProductsBase::prdStatus },
            { tblAccountProductsBase::prdCreatedBy_usrID },
            { tblAccountProductsBase::prdCreationDateTime },
            { tblAccountProductsBase::prdUpdatedBy_usrID },
        }) + _exclusiveIndexes
    )
{}

QVariant intfAccountProducts::apiGET(GET_METHOD_ARGS_IMPL_APICALL)
{
    ///TODO: get just by priv. users

    Authorization::checkPriv(_JWT, this->privOn(EHTTP_GET, this->moduleBaseName()));

    constexpr quint16 CACHE_TIME = 15 * 60;
    return Targoman::API::Query::Select(*this, GET_METHOD_CALL_ARGS_INTERNAL_CALL, {}, CACHE_TIME);
}

quint32 intfAccountProducts::apiCREATE(CREATE_METHOD_ARGS_IMPL_APICALL)
{
    Authorization::checkPriv(_JWT, this->privOn(EHTTP_PUT, this->moduleBaseName()));

    return Targoman::API::Query::Create(*this, CREATE_METHOD_CALL_ARGS_INTERNAL_CALL);
}

bool intfAccountProducts::apiUPDATE(UPDATE_METHOD_ARGS_IMPL_APICALL)
{
    Authorization::checkPriv(_JWT, this->privOn(EHTTP_PATCH, this->moduleBaseName()));

    return Targoman::API::Query::Update(*this, UPDATE_METHOD_CALL_ARGS_INTERNAL_CALL);
}

bool intfAccountProducts::apiDELETE(DELETE_METHOD_ARGS_IMPL_APICALL)
{
  Authorization::checkPriv(_JWT, this->privOn(EHTTP_DELETE, this->moduleBaseName()));

  return Targoman::API::Query::Delete(*this, DELETE_METHOD_CALL_ARGS_INTERNAL_CALL);
}

/******************************************************************/
intfAccountSaleables::intfAccountSaleables(
        const QString& _schema,
        const QList<ORM::clsORMField>& _exclusiveCols,
        const QList<ORM::stuRelation>& _exclusiveRelations,
        const QList<ORM::stuDBIndex>& _exclusiveIndexes)
    : clsTable(_schema,
        tblAccountSaleablesBase::Name,
        QList<ORM::clsORMField>({
        ///<  ColName                                          Type                             Validation                       Default     UpBy      Sort  Filter Self  Virt   PK
            { tblAccountSaleablesBase::slbID,                  ORM_PRIMARY_KEY32 },
            { tblAccountSaleablesBase::slb_prdID,              S(quint32),                      QFV.integer().minValue(1),       QRequired,  UPOwner },
            { tblAccountSaleablesBase::slbCode,                S(TAPI::SaleableCode_t),         QFV,                             QRequired,  UPOwner },
            { tblAccountSaleablesBase::slbName,                S(QString),                      QFV,                             QRequired,  UPOwner },
            { tblAccountSaleablesBase::slbDesc,                S(QString),                      QFV,                             QNull,      UPOwner },
            { tblAccountSaleablesBase::slbType,                S(TAPI::enuSaleableType::Type),  QFV,                             TAPI::enuSaleableType::Normal, UPOwner },
            { tblAccountSaleablesBase::slbAvailableFromDate,   S(TAPI::DateTime_t),             QFV,                             QNow,       UPOwner },
            { tblAccountSaleablesBase::slbAvailableToDate,     S(TAPI::DateTime_t),             QFV,                             QNull,      UPOwner },
            { tblAccountSaleablesBase::slbPrivs,               S(TAPI::JSON_t),                 QFV,                             QNull,      UPOwner },
            { tblAccountSaleablesBase::slbBasePrice,           S(qreal),                        QFV.real().minValue(0),          QRequired,  UPOwner },
            { tblAccountSaleablesBase::slbAdditives,           S(TAPI::SaleableAdditive_t),     QFV,                             QNull,      UPOwner },
            { tblAccountSaleablesBase::slbProductCount,        S(quint32),                      QFV.integer().minValue(1),       QRequired,  UPOwner},
            { tblAccountSaleablesBase::slbMaxSaleCountPerUser, S(NULLABLE_TYPE(quint32)),       QFV,                             QNull,      UPOwner},
            { tblAccountSaleablesBase::slbInStockCount,        S(quint32),                      QFV.integer().minValue(0),       QRequired,  UPAdmin },
            { tblAccountSaleablesBase::slbOrderedCount,        S(NULLABLE_TYPE(quint32)),       QFV,                             QNull,      UPNone },
            { tblAccountSaleablesBase::slbReturnedCount,       S(NULLABLE_TYPE(quint32)),       QFV,                             QNull,      UPNone },
            { tblAccountSaleablesBase::slbVoucherTemplate,     S(QString),                      QFV,                             QNull,      UPOwner },
            { tblAccountSaleablesBase::slbStatus,              S(TAPI::enuGenericStatus::Type), QFV,                             TAPI::enuGenericStatus::Active, UPStatus },
            { tblAccountSaleablesBase::slbCreatedBy_usrID,     ORM_CREATED_BY },
            { tblAccountSaleablesBase::slbCreationDateTime,    ORM_CREATED_ON },
            { tblAccountSaleablesBase::slbUpdatedBy_usrID,     ORM_UPDATED_BY },
        }) + _exclusiveCols,
        QList<ORM::stuRelation>({
        ///<  Relation Name Col                                 Reference Table                           ForeignCol                    Rename LeftJoin
            { "product",  { tblAccountSaleablesBase::slb_prdID, R(_schema, tblAccountProductsBase::Name), tblAccountProductsBase::prdID } },
            ORM_RELATION_OF_CREATOR(tblAccountSaleablesBase::slbCreatedBy_usrID),
            ORM_RELATION_OF_UPDATER(tblAccountSaleablesBase::slbUpdatedBy_usrID),
        }) + _exclusiveRelations,
        QList<ORM::stuDBIndex>({
            { { tblAccountSaleablesBase::slbCode, tblAccountSaleablesBase::slbStatus }, enuDBIndex::Unique },
            { tblAccountSaleablesBase::slbType },
            { tblAccountSaleablesBase::slbAvailableFromDate },
            { tblAccountSaleablesBase::slbAvailableToDate },
            { tblAccountSaleablesBase::slbStatus },
            { tblAccountSaleablesBase::slbCreatedBy_usrID },
            { tblAccountSaleablesBase::slbCreationDateTime },
            { tblAccountSaleablesBase::slbUpdatedBy_usrID },
        }) + _exclusiveIndexes
    )
{}

QVariant intfAccountSaleables::apiGET(GET_METHOD_ARGS_IMPL_APICALL)
{
//    QString ExtraFilters;
//    if (Authorization::hasPriv(_JWT, this->privOn(EHTTP_GET, this->moduleBaseName())) == false)
//        ExtraFilters = QString ("%1<=NOW() + ( %2=NULL | %2>=DATE_ADD(NOW(),INTERVAL$SPACE$15$SPACEMIN) )")
//                       .arg(tblAccountSaleablesBase::slbAvailableFromDate)
//                       .arg(tblAccountSaleablesBase::slbAvailableToDate);

    clsCondition ExtraFilters = {};
    if (Authorization::hasPriv(_JWT, this->privOn(EHTTP_GET, this->moduleBaseName())) == false)
        ExtraFilters
            .setCond({ tblAccountSaleablesBase::slbAvailableFromDate, enuConditionOperator::LessEqual, DBExpression::NOW() })
            .andCond(clsCondition({ tblAccountSaleablesBase::slbAvailableToDate, enuConditionOperator::Null })
                .orCond({ tblAccountSaleablesBase::slbAvailableToDate, enuConditionOperator::GreaterEqual,
                          DBExpression::DATE_ADD(DBExpression::NOW(), 15, enuDBExpressionIntervalUnit::MINUTE) })
            );

    constexpr quint16 CACHE_TIME = 15 * 60;
    return Targoman::API::Query::Select(*this, GET_METHOD_CALL_ARGS_INTERNAL_CALL, ExtraFilters, CACHE_TIME);
}

quint32 intfAccountSaleables::apiCREATE(CREATE_METHOD_ARGS_IMPL_APICALL)
{
    Authorization::checkPriv(_JWT, this->privOn(EHTTP_PUT, this->moduleBaseName()));

    return Targoman::API::Query::Create(*this, CREATE_METHOD_CALL_ARGS_INTERNAL_CALL);
}

bool intfAccountSaleables::apiUPDATE(UPDATE_METHOD_ARGS_IMPL_APICALL)
{
    Authorization::checkPriv(_JWT, this->privOn(EHTTP_PATCH, this->moduleBaseName()));

    return Targoman::API::Query::Update(*this, UPDATE_METHOD_CALL_ARGS_INTERNAL_CALL);
}

bool intfAccountSaleables::apiDELETE(DELETE_METHOD_ARGS_IMPL_APICALL)
{
  Authorization::checkPriv(_JWT, this->privOn(EHTTP_DELETE, this->moduleBaseName()));

  return Targoman::API::Query::Delete(*this, DELETE_METHOD_CALL_ARGS_INTERNAL_CALL);
}

/******************************************************************/
intfAccountUserAssets::intfAccountUserAssets(
        const QString& _schema,
        const QList<ORM::clsORMField>& _exclusiveCols,
        const QList<ORM::stuRelation>& _exclusiveRelations,
        const QList<stuDBIndex>& _exclusiveIndexes)
    : clsTable(
        _schema,
        tblAccountUserAssetsBase::Name,
        QList<ORM::clsORMField>({
        ///<  ColName                                       Type                       Validation                 Default     UpBy   Sort  Filter Self  Virt   PK
            { tblAccountUserAssetsBase::uasID,              ORM_PRIMARY_KEY64 },
            { tblAccountUserAssetsBase::uas_usrID,          S(quint64),                QFV.integer().minValue(1), QRequired,  UPAdmin },
            { tblAccountUserAssetsBase::uas_slbID,          S(quint64),                QFV.integer().minValue(1), QRequired,  UPNone },
            { tblAccountUserAssetsBase::uas_vchID,          S(NULLABLE_TYPE(quint64)), QFV.integer().minValue(1), QNull,      UPNone },
            { tblAccountUserAssetsBase::uasVoucherItemUUID, S(TAPI::MD5_t),            QFV,                       QRequired,  UPNone },
            { tblAccountUserAssetsBase::uas_cpnID,          S(NULLABLE_TYPE(quint32)), QFV,                       QNull,      UPNone },
            { tblAccountUserAssetsBase::uasDiscountAmount,  S(NULLABLE_TYPE(quint32)), QFV,                       QNull,      UPNone },
            { tblAccountUserAssetsBase::uasPrefered,        S(bool),                   QFV,                       false,      UPOwner },
            { tblAccountUserAssetsBase::uasOrderDateTime,   S(TAPI::DateTime_t),       QFV,                       QNow,       UPNone },
            { tblAccountUserAssetsBase::uasStatus,          S(TAPI::enuAuditableStatus::Type), QFV,               TAPI::enuAuditableStatus::Pending, UPStatus },
            { tblAccountUserAssetsBase::uasUpdatedBy_usrID, ORM_UPDATED_BY },
        }) + _exclusiveCols,
        QList<ORM::stuRelation>({
        ///<  Col                                  Reference Table                            ForeignCol                      Rename     LeftJoin
            { tblAccountUserAssetsBase::uas_usrID, R(AAASchema, tblUser::Name),               tblUser::usrID,                 "Owner_" },
            { tblAccountUserAssetsBase::uas_slbID, R(_schema, tblAccountSaleablesBase::Name), tblAccountSaleablesBase::slbID, "",       true },
            { tblAccountUserAssetsBase::uas_cpnID, R(_schema, tblAccountCouponsBase::Name),   tblAccountCouponsBase::cpnID,   "",       true },
            //Voucher is not accessible as it is in another schema
            //{tblAccountUserAssets::uas_vchID,    R(AAASchema,tblVoucher::Name),  tblVoucher::vchID,    "", true},
        }) + _exclusiveRelations,
        QList<ORM::stuDBIndex>({
            { {
                  tblAccountUserAssetsBase::uas_usrID,
//                  tblAccountUserAssetsBase::uas_slbID,
                  tblAccountUserAssetsBase::uasVoucherItemUUID,
                  tblAccountUserAssetsBase::uasStatus
              }, enuDBIndex::Unique },
            { tblAccountUserAssetsBase::uas_usrID },
            { tblAccountUserAssetsBase::uas_slbID },
            { tblAccountUserAssetsBase::uas_vchID },
            { tblAccountUserAssetsBase::uasVoucherItemUUID },
            { tblAccountUserAssetsBase::uas_cpnID },
            { tblAccountUserAssetsBase::uasOrderDateTime },
            { tblAccountUserAssetsBase::uasStatus },
            { tblAccountUserAssetsBase::uasUpdatedBy_usrID },
        }) + _exclusiveIndexes
    )
{}

QVariant intfAccountUserAssets::apiGET(GET_METHOD_ARGS_IMPL_APICALL)
{
  if (Authorization::hasPriv(_JWT, this->privOn(EHTTP_GET, this->moduleBaseName())) == false)
    this->setSelfFilters({{tblAccountUserAssetsBase::uas_usrID, clsJWT(_JWT).usrID()}}, _filters);

  return Targoman::API::Query::Select(*this, GET_METHOD_CALL_ARGS_INTERNAL_CALL);
}

bool intfAccountUserAssets::apiUPDATEsetAsPrefered(TAPI::JWT_t _JWT, TAPI::PKsByPath_t _pksByPath){
  bool Ok;
  quint64 UserPackageID = _pksByPath.toUInt(&Ok);
  if(!Ok || !UserPackageID )
    throw exHTTPBadRequest("Invalid UserPackageID provided");

  this->callSP("sp_UPDATE_setAsPrefered", {
                 {"iUserID", clsJWT(_JWT).usrID()},
                 {"iUASID",  UserPackageID},
               });
  return false;
}

bool intfAccountUserAssets::apiUPDATEdisablePackage(TAPI::JWT_t _JWT, TAPI::PKsByPath_t _pksByPath){
  bool Ok;
  quint64 UserPackageID = _pksByPath.toUInt(&Ok);
  if (!Ok || !UserPackageID )
    throw exHTTPBadRequest("Invalid UserPackageID provided");

  Authorization::checkPriv(_JWT, this->privOn(EHTTP_PATCH, this->moduleBaseName()));
  /*return this->update(clsJWT(_JWT).usrID(), {
                            {tblAccountUserAssets::uasID, UserPackageID}
                        }, {
                            {tblAccountUserAssets::uasStatus, TAPI::enuAuditableStatus::Banned},
                        });*/
  throw Common::exTargomanMustBeImplemented(__FUNCTION__);
  return Ok;
}

/******************************************************************/
intfAccountAssetUsage::intfAccountAssetUsage(
        const QString& _schema,
        const QList<ORM::clsORMField>& _exclusiveCols,
        const QList<ORM::stuRelation>& _exclusiveRelations,
        const QList<stuDBIndex>& _exclusiveIndexes)
    : clsTable(_schema,
        tblAccountAssetUsageBase::Name,
        QList<ORM::clsORMField>({
        ///<  ColName                              Type                Validation                              Default    UpBy   Sort  Filter Self  Virt   PK
            { tblAccountAssetUsageBase::usg_uasID, ORM_PRIMARY_KEY64},
        }) + _exclusiveCols,
        QList<ORM::stuRelation>({
        ///<  Col                                  Reference Table                                  ForeignCol                        Rename     LeftJoin
            { tblAccountAssetUsageBase::usg_uasID, R(_schema, tblAccountUserAssetsBase::Name), tblAccountUserAssetsBase::uasID},
        }) + _exclusiveRelations,
        _exclusiveIndexes
    )
{}

QVariant intfAccountAssetUsage::apiGET(GET_METHOD_ARGS_IMPL_APICALL)
{
    if (Authorization::hasPriv(_JWT, this->privOn(EHTTP_GET, this->moduleBaseName())) == false)
      this->setSelfFilters({{tblAccountUserAssetsBase::uas_usrID, clsJWT(_JWT).usrID()}}, _filters);

    return Targoman::API::Query::Select(*this, GET_METHOD_CALL_ARGS_INTERNAL_CALL);
}

/******************************************************************/
intfAccountCoupons::intfAccountCoupons(const QString& _schema)
    : clsTable(_schema,
        tblAccountCouponsBase::Name,
        {///< ColName                                           Type                               Validation                               Default    UpBy   Sort  Filter Self  Virt   PK
            { tblAccountCouponsBase::cpnID,                     ORM_PRIMARY_KEY32},
            { tblAccountCouponsBase::cpnCode,                   S(TAPI::CouponCode_t),             QFV,                                     QRequired, UPAdmin},
            { tblAccountCouponsBase::cpnPrimaryCount,           S(quint32),                        QFV.integer().minValue(1),               1,         UPAdmin},
            { tblAccountCouponsBase::cpnTotalMaxAmount,         S(quint32),                        QFV.integer().minValue(1),               1,         UPAdmin},
            { tblAccountCouponsBase::cpnPerUserMaxCount,        S(NULLABLE_TYPE(quint32)),         QFV.optional(QFV.integer().minValue(1)), QNull,     UPAdmin},
            { tblAccountCouponsBase::cpnPerUserMaxAmount,       S(NULLABLE_TYPE(quint32)),         QFV.integer().minValue(1),               QNull,     UPAdmin},
            { tblAccountCouponsBase::cpnValidFrom,              S(TAPI::DateTime_t),               QFV,                                     QRequired, UPAdmin},
            { tblAccountCouponsBase::cpnExpiryTime,             S(NULLABLE_TYPE(TAPI::DateTime_t)),QFV,                                     QNull,     UPAdmin},
            { tblAccountCouponsBase::cpnAmount,                 S(quint32),                        QFV,                                     QRequired, UPAdmin}, //, false, false},
            { tblAccountCouponsBase::cpnAmountType,             S(TAPI::enuDiscountType::Type),    QFV,                                     TAPI::enuDiscountType::Percent, UPAdmin},
            { tblAccountCouponsBase::cpnMaxAmount,              S(NULLABLE_TYPE(quint32)),         QFV,                                     QNull,     UPAdmin}, //, false, false},
            { tblAccountCouponsBase::cpnSaleableBasedMultiplier,S(TAPI::JSON_t),                   QFV,                                     QRequired, UPAdmin}, //, false, false},
//            { tblAccountCouponsBase::cpnSaleableBasedMultiplier,S(QList<TAPI::stuDiscountSaleableBasedMultiplier>), QFV,                    QRequired, UPAdmin, false, false},
            { tblAccountCouponsBase::cpnTotalUsedCount,         S(quint32),                        QFV.integer().minValue(0),               0,         UPNone},
            { tblAccountCouponsBase::cpnTotalUsedAmount,        S(quint32),                        QFV.integer().minValue(0),               0,         UPNone},
            { tblAccountCouponsBase::cpnStatus,                 S(TAPI::enuGenericStatus::Type),   QFV,                                     TAPI::enuGenericStatus::Active, UPStatus},
            { tblAccountCouponsBase::cpnCreatedBy_usrID,        ORM_CREATED_BY},
            { tblAccountCouponsBase::cpnCreationDateTime,       ORM_CREATED_ON},
            { tblAccountCouponsBase::cpnUpdatedBy_usrID,        ORM_UPDATED_BY},
        },
        {///< Col                                       Reference Table                         ForeignCol          Rename     LeftJoin
            ORM_RELATION_OF_CREATOR(tblAccountCouponsBase::cpnCreatedBy_usrID),
            ORM_RELATION_OF_UPDATER(tblAccountCouponsBase::cpnUpdatedBy_usrID),
        },
        {
            { {
               tblAccountCouponsBase::cpnCode,
               tblAccountCouponsBase::cpnStatus
              }, enuDBIndex::Unique },
            { tblAccountCouponsBase::cpnAmountType },
            { tblAccountCouponsBase::cpnValidFrom },
            { tblAccountCouponsBase::cpnExpiryTime },
            { tblAccountCouponsBase::cpnStatus },
            { tblAccountCouponsBase::cpnCreatedBy_usrID },
            { tblAccountCouponsBase::cpnCreationDateTime },
            { tblAccountCouponsBase::cpnUpdatedBy_usrID },
        }
    )
{}

QVariant intfAccountCoupons::apiGET(GET_METHOD_ARGS_IMPL_APICALL)
{
  Authorization::checkPriv(_JWT, this->privOn(EHTTP_GET, this->moduleBaseName()));

  return Targoman::API::Query::Select(*this, GET_METHOD_CALL_ARGS_INTERNAL_CALL);

//  return query.one();

//    return this->selectFromTable({}, {}, GET_METHOD_CALL_ARGS_APICALL);
}

quint32 intfAccountCoupons::apiCREATE(CREATE_METHOD_ARGS_IMPL_APICALL)
{
    Authorization::checkPriv(_JWT, this->privOn(EHTTP_PUT, this->moduleBaseName()));

    return Targoman::API::Query::Create(*this, CREATE_METHOD_CALL_ARGS_INTERNAL_CALL);
}

bool intfAccountCoupons::apiUPDATE(UPDATE_METHOD_ARGS_IMPL_APICALL)
{
    Authorization::checkPriv(_JWT, this->privOn(EHTTP_PATCH, this->moduleBaseName()));

    return Targoman::API::Query::Update(*this, UPDATE_METHOD_CALL_ARGS_INTERNAL_CALL);
}

bool intfAccountCoupons::apiDELETE(DELETE_METHOD_ARGS_IMPL_APICALL)
{
  Authorization::checkPriv(_JWT, this->privOn(EHTTP_DELETE, this->moduleBaseName()));

  return Targoman::API::Query::Delete(*this, DELETE_METHOD_CALL_ARGS_INTERNAL_CALL);
}

/******************************************************************/
intfAccountPrizes::intfAccountPrizes(const QString& _schema,
                                     const QString& _name,
                                     const QList<ORM::clsORMField>& _cols,
                                     const QList<ORM::stuRelation>& _relations) :
  clsTable(_schema, _name, _cols, _relations)
{}

QVariant intfAccountPrizes::apiGET(GET_METHOD_ARGS_IMPL_APICALL)
{
  Authorization::checkPriv(_JWT, this->privOn(EHTTP_GET, this->moduleBaseName()));

  return Targoman::API::Query::Select(*this, GET_METHOD_CALL_ARGS_INTERNAL_CALL);

//  return query.one();

  //  return this->selectFromTable({}, {}, GET_METHOD_CALL_ARGS_APICALL);
}

quint32 intfAccountPrizes::apiCREATE(CREATE_METHOD_ARGS_IMPL_APICALL)
{
    Authorization::checkPriv(_JWT, this->privOn(EHTTP_PUT, this->moduleBaseName()));

    return Targoman::API::Query::Create(*this, CREATE_METHOD_CALL_ARGS_INTERNAL_CALL);
}

bool intfAccountPrizes::apiUPDATE(UPDATE_METHOD_ARGS_IMPL_APICALL)
{
    Authorization::checkPriv(_JWT, this->privOn(EHTTP_PATCH, this->moduleBaseName()));

    return Targoman::API::Query::Update(*this, UPDATE_METHOD_CALL_ARGS_INTERNAL_CALL);
}

bool intfAccountPrizes::apiDELETE(DELETE_METHOD_ARGS_IMPL_APICALL)
{
  Authorization::checkPriv(_JWT, this->privOn(EHTTP_DELETE, this->moduleBaseName()));

  return Targoman::API::Query::Delete(*this, DELETE_METHOD_CALL_ARGS_INTERNAL_CALL);
}

/******************************************************************/
QJsonObject stuAssetItem::toJson(bool _full)
{
    ///TODO Complete me
  QJsonObject Info;/*
  if(this->PackageID > 0)       Info[PKG_ID] = static_cast<double>(this->PackageID);
  if(this->PackageCode > -1)    Info[PKG_CODE] = this->PackageCode;
  if(this->RemainingDays > -1)  Info[PKG_REMAININGDAYS] = this->RemainingDays;
  if(this->RemainingHours > -1) Info[PKG_REMAININGHOURS] = this->RemainingHours;
  if(this->StartDate.isValid()) Info[PKG_STARTDATE] = this->StartDate.toString();
  if(this->EndDate.isValid())   Info[PKG_ENDDATE] = this->EndDate.toString();
  if(this->StartTime.isValid()) Info[PKG_STARTTIME] = this->StartTime.toString();
  if(this->EndTime.isValid())   Info[PKG_ENDTIME] = this->EndTime.toString();
  if(this->Properties.size())   Info[PKG_PROPS] = this->Properties;
  if(_full){
    QJsonObject Limits;
    for(auto LimitIter = this->Remaining.begin();
        LimitIter != this->Remaining.end();
        LimitIter++)
      Limits.insert(LimitIter.key(), LimitIter->toJson());
    Info[PKG_LIMITS] = Limits;
  }*/
  return Info;
}

stuAssetItem&stuAssetItem::fromJson(const QJsonObject& _obj)
{
    /* TODO COMPLETE ME
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
  for(auto LimitIter = Limits.begin();
      LimitIter != Limits.end();
      LimitIter++)
    this->Remaining.insert(LimitIter.key(), stuUsage().fromJson(LimitIter->toObject()));
    */
  return *this;
}

/******************************************************************/
stuActiveCredit::stuActiveCredit(const stuAssetItem& _credit, bool _isFromParent, const UsageLimits_t& _myLimitsOnParent, qint64 _ttl) :
  Credit(_credit),
  IsFromParent(_isFromParent),
  MyLimitsOnParent(_myLimitsOnParent),
  TTL(_ttl)
{}

QJsonObject stuActiveCredit::toJson(bool _full)
{
  QJsonObject Account = {
    {ASA_PACKAGE, this->Credit.toJson(_full)},
    {ASA_TTL, static_cast<double>(this->TTL)},
  };
  if(this->IsFromParent)
    Account.insert(ASA_ISFROMPARENT, true);
  for(auto LimitIter = this->MyLimitsOnParent.begin();
      LimitIter != this->MyLimitsOnParent.end();
      LimitIter++)
    Account.insert(LimitIter.key(), LimitIter->toJson());
  return Account;
}

stuActiveCredit& stuActiveCredit::fromJson(const QJsonObject _obj)
{
  this->Credit = stuAssetItem().fromJson(_obj.value(ASA_PACKAGE).toObject());
  this->IsFromParent  = _obj.value(ASA_ISFROMPARENT).toBool();
  this->TTL = static_cast<qint64>(_obj.value(ASA_TTL).toDouble());
  QJsonObject Limits = _obj.value(ASA_LIMITSONPARENT).toObject();
  for(auto LimitIter = Limits.begin();
      LimitIter != Limits.end();
      LimitIter++)
    this->MyLimitsOnParent.insert(LimitIter.key(), stuUsage().fromJson(LimitIter->toObject()));

  return *this;
}

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
    DBCurrentDateTime(_dbCurrentDateTime)
{}

} //namespace Accounting
} //namespace AAA
} //namespace API
} //namespace Targoman

namespace TAPI {
/******************************************************************/
/*QVariant stuAssetItemReq_t::toVariant() const{
    QVariantMap Value;
    for(auto Iter = this->begin(); Iter != this->end(); ++Iter)
        Value.insert(Iter.key(), Iter.value());

    return Value;
}

stuAssetItemReq_t& stuAssetItemReq_t::fromVariant(const QVariant& _value, const QByteArray& _paramName){
    if(_value.canConvert<QMap<QString, QVariant>>() == false)
        throw exHTTPBadRequest("Invalid value specified for parameter:" + _paramName);
    this->clear();
    const QMap<QString, QVariant>& Map = _value.toMap();
    for(auto Iter = Map.begin (); Iter != Map.end (); ++Iter)
        this->insert(Iter.key(), static_cast<qint16>(Iter.value().toInt()));
    return  *this;
}*/
}
