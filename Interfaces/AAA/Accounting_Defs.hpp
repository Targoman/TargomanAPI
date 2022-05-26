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

#ifndef TARGOMAN_API_AAA_ACCOUNTING_DEFS_H
#define TARGOMAN_API_AAA_ACCOUNTING_DEFS_H

//#include "Interfaces/Common/APIArgHelperMacrosPrivate.h"
#include "AAADefs.hpp"
#include "clsJWT.hpp"
#include "Interfaces/Common/GenericTypes.h"
#include "Interfaces/Common/QtTypes.hpp"
#include "libTargomanCommon/Configuration/tmplConfigurable.h"
#include "libTargomanCommon/Configuration/Validators.hpp"
#include <QString>
#include <QJsonArray>

namespace Targoman::API::AAA {

TARGOMAN_DEFINE_ENUM(enuVoucherStatus,
                     New            = 'N',
                     Canceled       = 'C',
                     Error          = 'E',
                     Finished       = 'F',
//                     WaitForProcess = 'W',
                     Removed        = 'R',
                     );

TARGOMAN_DEFINE_ENUM(enuDiscountType,
                     Percent = '%',
                     Currency  = '$',
                     );

TARGOMAN_DEFINE_ENUM(enuVoucherItemProcessStatus,
                     Canceled   = 'C',
                     Error      = 'E',
                     Finished   = 'F',
                     Removed    = 'R',
                     );

} //namespace Targoman::API::AAA

TAPI_DECLARE_METATYPE_ENUM(Targoman::API::AAA, enuVoucherStatus)  // -> TAPI_REGISTER_TARGOMAN_ENUM() in Accounting_Interfaces.cpp
TAPI_DECLARE_METATYPE_ENUM(Targoman::API::AAA, enuDiscountType)   // -> TAPI_REGISTER_TARGOMAN_ENUM() in Accounting_Interfaces.cpp
TAPI_DECLARE_METATYPE_ENUM(Targoman::API::AAA, enuVoucherItemProcessStatus)   // -> TAPI_REGISTER_TARGOMAN_ENUM() in Accounting_Interfaces.cpp

namespace Targoman::API::AAA {

//inline QString makeConfig(const QString& _name) { return "/zModule_Account/" + _name; }
inline QString makeConfig(const QString& _name) { return "/Module_Account/" + _name; }
extern Targoman::Common::Configuration::tmplConfigurable<QString> Secret;
extern QByteArray voucherSign(const QByteArray& _data);

TAPI_DEFINE_VARIANT_ENABLED_STRUCT(stuPrize,
    QString,      Desc      , QString()    , v.size(), v, v.toString(),
    QJsonObject,  PrizeInfo , QJsonObject(), v.size(), v, v.toObject()
);

TAPI_DEFINE_VARIANT_ENABLED_STRUCT(stuDiscountSaleableBasedMultiplier,
    SF_QString(SaleableCode),
    SF_qreal(Multiplier),
    SF_NULLABLE_qreal(MinQty)
);

struct stuFullDiscount
{
    quint32                         cpnID;
    TAPI::CouponCode_t              cpnCode;
    quint32                         cpnPrimaryCount;
    quint32                         cpnTotalMaxAmount;
    NULLABLE_TYPE(quint32)          cpnPerUserMaxCount;
    NULLABLE_TYPE(quint32)          cpnPerUserMaxAmount;
    TAPI::DateTime_t                cpnValidFrom;
    NULLABLE_TYPE(TAPI::DateTime_t) cpnExpiryTime;
    quint32                         cpnAmount;
    enuDiscountType::Type           cpnAmountType;
    NULLABLE_TYPE(quint32)          cpnMaxAmount;
    TAPI::JSON_t                    cpnSaleableBasedMultiplier;
    quint32                         cpnTotalUsedCount;
    quint32                         cpnTotalUsedAmount;
    TAPI::enuGenericStatus::Type    cpnStatus;

    void fromVariantMap(const QVariantMap& _info);
};

TAPI_DEFINE_VARIANT_ENABLED_STRUCT(stuDiscount3,
    SF_quint64(ID),
    SF_QString(Code),
    SF_qreal(Amount)
);

//Caution: Do not rename fields. Field names are used in vchDesc (as json)
TAPI_DEFINE_VARIANT_ENABLED_STRUCT(stuVoucherItem,
    SF_QString   (Service),
    SF_quint64   (OrderID),
    SF_MD5_t     (UUID),
    SF_QString   (Desc),
    SF_quint32   (UnitPrice),
    SF_qreal     (Qty),
    SF_quint32   (SubTotal),
    SF_Var_Struct(stuDiscount3, Discount, v.ID > 0),
    SF_quint32   (DisAmount),
    SF_quint8    (VATPercent),
    SF_quint32   (VATAmount),
    SF_QString   (Sign)
);

TAPI_DEFINE_VARIANT_ENABLED_STRUCT(stuVoucherItemForTrustedAction,
    SF_quint64(UserID),
    SF_quint64(VoucherID),
    SF_Struct(stuVoucherItem, VoucherItem, v.Qty>0),
    SF_QString(Sign)
);

/*****************************************************************************/
TAPI_DEFINE_VARIANT_ENABLED_STRUCT(stuUsage,
    SF_NULLABLE_quint32(PerDay),
    SF_NULLABLE_quint32(PerWeek),
    SF_NULLABLE_quint32(PerMonth),
    SF_NULLABLE_quint64(Total)
);

typedef QMap<QString, stuUsage> UsageLimits_t;

#define SF_UsageLimits_t(_name) SF_Generic( \
    /* type              */ UsageLimits_t, \
    /* name              */ Digested_Limits, \
    /* def               */ UsageLimits_t(), \
    /* validator         */ v.size(), \
    /* type 2 QJsonValue */ [](UsageLimits_t v) -> QJsonValue { \
                                QJsonObject A; \
                                for (auto it = v.begin(); \
                                    it != v.end(); \
                                    it++ \
                                ) { \
                                    A.insert(it.key(), it->toJson()); \
                                } \
                                return A; \
                            }(v), \
    /* QJsonValue 2 type */ [](QJsonValue v) -> UsageLimits_t { \
                                UsageLimits_t L; \
                                QJsonObject O = v.toObject(); \
                                foreach (const QString &Key, O.keys()) \
                                    L.insert(Key, stuUsage().fromJson(O.value(Key).toObject())); \
                                return L; \
                            }(v) \
    )

struct stuUsageColDefinition {
    QString PerDay;
    QString PerWeek;
    QString PerMonth;
    QString Total;

    stuUsageColDefinition(const QString& _perDay, const QString& _perWeek, const QString& _perMonth, const QString& _total) :
        PerDay(_perDay), PerWeek(_perWeek), PerMonth(_perMonth), Total(_total)
    { ; }
};

typedef QMap<QString, stuUsageColDefinition> AssetUsageLimitsCols_t;

typedef QMap<QString, QString> OrderAdditives_t;

TAPI_DEFINE_VARIANT_ENABLED_STRUCT(stuAssetItem,
    //product
    SF_quint32          (prdID),
    SF_QString          (prdCode), //TAPI::ProductCode_t
    SF_QString          (prdName),
    SF_Date_t           (prdValidFromDate),
    SF_Date_t           (prdValidToDate),
    SF_NULLABLE_quint8  (prdValidFromHour),
    SF_NULLABLE_quint8  (prdValidToHour),
    SF_JSON_t           (prdPrivs), //TAPI::PrivObject_t
    SF_NULLABLE_qreal   (prdVAT),
    SF_quint32          (prdInStockQty),
    SF_NULLABLE_quint32 (prdOrderedQty),
    SF_NULLABLE_quint32 (prdReturnedQty),
    SF_Enum             (TAPI::enuGenericStatus, prdStatus, TAPI::enuGenericStatus::Active),

    //saleable
    SF_quint32          (slbID),
    SF_QString          (slbCode), //TAPI::SaleableCode_t
    SF_QString          (slbName),
    SF_JSON_t           (slbPrivs),
    SF_qreal            (slbBasePrice),
    SF_JSON_t           (slbAdditives), //TAPI::SaleableAdditive_t
//    SF_quint32          (slbProductCount),
    SF_NULLABLE_quint32 (slbMaxSaleCountPerUser),
    SF_quint32          (slbInStockQty),
    SF_NULLABLE_quint32 (slbOrderedQty),
    SF_NULLABLE_quint32 (slbReturnedQty),
    SF_QString          (slbVoucherTemplate),
    SF_Enum             (TAPI::enuGenericStatus, slbStatus, TAPI::enuGenericStatus::Active),

    SF_qreal            (SubTotal),
    SF_Var_Struct       (stuDiscount3, Discount, [](Q_DECL_UNUSED auto v) { return true; }(v)), //NULLABLE_TYPE
    SF_qreal            (TotalPrice),

    /*
    struct {
//        QJsonObject   Additives;
        UsageLimits_t Limits;
//        QJsonObject   Privs;
    } Digested;*/

//        SF_UsageLimits_t(Digested_Limits)
    SF_Generic(
        /* type              */ UsageLimits_t,
        /* name              */ Digested_Limits,
        /* def               */ UsageLimits_t(),
        /* validator         */ v.size(),
        /* type 2 QJsonValue */ [](UsageLimits_t v) -> QJsonValue {
                                    QJsonObject A;
                                    for (auto it = v.begin();
                                        it != v.end();
                                        it++
                                    ) {
                                        A.insert(it.key(), it->toJson());
                                    }
                                    return A;
                                }(v),
        /* QJsonValue 2 type */ [](QJsonValue v) -> UsageLimits_t {
                                    UsageLimits_t L;
                                    QJsonObject O = v.toObject();
                                    foreach (const QString &Key, O.keys())
                                        L.insert(Key, stuUsage().fromJson(O.value(Key).toObject()));
                                    return L;
                                }(v)
    )

//    QJsonObject toJson(bool _full);
//    stuAssetItem& fromJson(const QJsonObject& _obj);
//    void fromVariantMap(const QVariantMap& _info);
);

typedef QMap<QString, stuAssetItem> ActiveCredits_t;

struct stuServiceCreditsInfo {
    ActiveCredits_t             ActiveCredits;
    NULLABLE_TYPE(stuAssetItem) PreferedCredit;
    NULLABLE_TYPE(quint32)      ParentID;
    UsageLimits_t               MyLimitsOnParent;
    QDateTime                   DBCurrentDateTime;

    stuServiceCreditsInfo(ActiveCredits_t             _activeCredits,
                          NULLABLE_TYPE(stuAssetItem) _preferedCredit,
                          NULLABLE_TYPE(quint32)      _parentID,
                          UsageLimits_t               _myLimitsOnParent,
                          QDateTime                   _dbCurrentDateTime);
};

/*
struct stuActiveCredit {
    stuAssetItem  Credit;
    bool          IsFromParent;
    UsageLimits_t MyLimitsOnParent;
    qint64        TTL;

    stuActiveCredit(const stuAssetItem& _credit = {},
                    bool _isFromParent = false,
                    const UsageLimits_t& _myLimitsOnParent = {},
                    qint64 _ttl = 0);

//    QJsonObject toJson(bool _full);
//    stuActiveCredit& fromJson(const QJsonObject _obj);
};
*/

TAPI_DEFINE_VARIANT_ENABLED_STRUCT(stuActiveCredit,
    SF_Var_Struct(stuAssetItem, Credit, v.prdID),
    SF_bool(IsFromParent),
//    SF_UsageLimits_t(MyLimitsOnParent),
    SF_Generic(
        /* type              */ UsageLimits_t,
        /* name              */ MyLimitsOnParent,
        /* def               */ UsageLimits_t(),
        /* validator         */ v.size(),
        /* type 2 QJsonValue */ [](UsageLimits_t v) -> QJsonValue {
                                    QJsonObject A;
                                    for (auto it = v.begin();
                                        it != v.end();
                                        it++
                                    ) {
                                        A.insert(it.key(), it->toJson());
                                    }
                                    return A;
                                }(v),
        /* QJsonValue 2 type */ [](QJsonValue v) -> UsageLimits_t {
                                    UsageLimits_t L;
                                    QJsonObject O = v.toObject();
                                    foreach (const QString &Key, O.keys())
                                        L.insert(Key, stuUsage().fromJson(O.value(Key).toObject()));
                                    return L;
                                }(v)
    ),
    SF_qint64(TTL)
);

typedef QList<stuVoucherItem> InvItems_t;

TAPI_DEFINE_VARIANT_ENABLED_STRUCT(stuPreVoucher,
    SF_quint64(UserID),
    SF_Generic(
        /* type              */ InvItems_t,
        /* name              */ Items,
        /* def               */ InvItems_t(),
        /* validator         */ v.size(),
        /* type 2 QJsonValue */ [](auto v) { QJsonArray A; foreach (auto a, v) A.append(a.toJson()); return A; }(v),
        /* QJsonValue 2 type */ [](auto v) { InvItems_t L; foreach (auto I, v.toArray()) L.append(Targoman::API::AAA::stuVoucherItem().fromJson(I.toObject())); return L; }(v)
    ),
    SF_Struct(stuPrize, Prize, v.Desc.size()),
    SF_QString(Summary),
    SF_quint16(Round),
    SF_quint32(ToPay, 0, v>0),
    SF_QString(Sign)
);

TAPI_DEFINE_VARIANT_ENABLED_STRUCT(stuVoucher,
    SF_quint64(ID, 0, v>0),
    SF_Struct(stuPreVoucher, Info, v.ToPay),
    SF_QString(PaymentLink),
    SF_QString(PaymentMD5),
    SF_Enum(enuVoucherStatus, Status, enuVoucherStatus::New)
);

//TAPI_DEFINE_VARIANT_ENABLED_STRUCT(stuVoucherItemProcess,
//    SF_MD5_t(UUID),
//    SF_NULLABLE_Enum(enuVoucherItemProcessStatus, Status)
//);

//bool KKKKKKKKKKKKKKKKKK() {return true;}

typedef QMap<QString, qint32> ServiceUsage_t;

extern void checkPreVoucherSanity(stuPreVoucher _preVoucher);

/***************************************************************************************************/
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-variable"

namespace tblAccountProductsBase {
    constexpr char Name[] = "tblAccountProducts";

    TARGOMAN_CREATE_CONSTEXPR(prdID);
    TARGOMAN_CREATE_CONSTEXPR(prdCode);
    TARGOMAN_CREATE_CONSTEXPR(prdName);
    TARGOMAN_CREATE_CONSTEXPR(prdDesc);
    TARGOMAN_CREATE_CONSTEXPR(prdValidFromDate);
    TARGOMAN_CREATE_CONSTEXPR(prdValidToDate);
    TARGOMAN_CREATE_CONSTEXPR(prdValidFromHour);
    TARGOMAN_CREATE_CONSTEXPR(prdValidToHour);
    TARGOMAN_CREATE_CONSTEXPR(prdPrivs);
    TARGOMAN_CREATE_CONSTEXPR(prdVAT);

    ///TODO: create trigger for this 3 fields
    TARGOMAN_CREATE_CONSTEXPR(prdInStockQty);
    TARGOMAN_CREATE_CONSTEXPR(prdOrderedQty);
    TARGOMAN_CREATE_CONSTEXPR(prdReturnedQty);
    // prdRemainingCount = prdInStockQty - (prdOrderedQty - prdReturnedQty)

    TARGOMAN_CREATE_CONSTEXPR(prdStatus);
    TARGOMAN_CREATE_CONSTEXPR(prdCreatedBy_usrID);
    TARGOMAN_CREATE_CONSTEXPR(prdCreationDateTime);
    TARGOMAN_CREATE_CONSTEXPR(prdUpdatedBy_usrID);

    inline QStringList ColumnNames(QString _tableAlias = "") {
        if (_tableAlias.isEmpty() == false)
            _tableAlias += ".";
        return {
            _tableAlias + prdID,
            _tableAlias + prdCode,
            _tableAlias + prdName,
            _tableAlias + prdDesc,
            _tableAlias + prdValidFromDate,
            _tableAlias + prdValidToDate,
            _tableAlias + prdValidFromHour,
            _tableAlias + prdValidToHour,
            _tableAlias + prdPrivs,
            _tableAlias + prdVAT,
            _tableAlias + prdInStockQty,
            _tableAlias + prdOrderedQty,
            _tableAlias + prdReturnedQty,
            // prdRemainingCount = prdInStockQty - (prdOrderedQty - prdReturnedQty)
            _tableAlias + prdStatus,
            _tableAlias + prdCreatedBy_usrID,
            _tableAlias + prdCreationDateTime,
            _tableAlias + prdUpdatedBy_usrID,
        };
    }

}

namespace tblAccountSaleablesBase {
    constexpr char Name[] = "tblAccountSaleables";

    TARGOMAN_CREATE_CONSTEXPR(slbID);
    TARGOMAN_CREATE_CONSTEXPR(slb_prdID);
    TARGOMAN_CREATE_CONSTEXPR(slbCode);
    TARGOMAN_CREATE_CONSTEXPR(slbName);
    TARGOMAN_CREATE_CONSTEXPR(slbDesc);
    TARGOMAN_CREATE_CONSTEXPR(slbType);
    TARGOMAN_CREATE_CONSTEXPR(slbAvailableFromDate);
    TARGOMAN_CREATE_CONSTEXPR(slbAvailableToDate);
    TARGOMAN_CREATE_CONSTEXPR(slbPrivs);
    TARGOMAN_CREATE_CONSTEXPR(slbBasePrice);
    TARGOMAN_CREATE_CONSTEXPR(slbAdditives);
//    TARGOMAN_CREATE_CONSTEXPR(slbProductCount); //what is this?
    TARGOMAN_CREATE_CONSTEXPR(slbMaxSaleCountPerUser);

    ///TODO: create trigger for this 3 fields and make changes to prd Count fields
    TARGOMAN_CREATE_CONSTEXPR(slbInStockQty);
    TARGOMAN_CREATE_CONSTEXPR(slbOrderedQty);
    TARGOMAN_CREATE_CONSTEXPR(slbReturnedQty);
    // slbRemainingCount = slbInStockQty - (slbOrderedQty - slbReturnedQty)

    TARGOMAN_CREATE_CONSTEXPR(slbVoucherTemplate);
    TARGOMAN_CREATE_CONSTEXPR(slbStatus);
    TARGOMAN_CREATE_CONSTEXPR(slbCreatedBy_usrID);
    TARGOMAN_CREATE_CONSTEXPR(slbCreationDateTime);
    TARGOMAN_CREATE_CONSTEXPR(slbUpdatedBy_usrID);

    inline QStringList ColumnNames(QString _tableAlias = "") {
        if (_tableAlias.isEmpty() == false)
            _tableAlias += ".";
        return {
            _tableAlias + slbID,
            _tableAlias + slb_prdID,
            _tableAlias + slbCode,
            _tableAlias + slbName,
            _tableAlias + slbDesc,
            _tableAlias + slbType,
            _tableAlias + slbAvailableFromDate,
            _tableAlias + slbAvailableToDate,
            _tableAlias + slbPrivs,
            _tableAlias + slbBasePrice,
            _tableAlias + slbAdditives,
//            _tableAlias + slbProductCount, //what is this?
            _tableAlias + slbMaxSaleCountPerUser,
            _tableAlias + slbInStockQty,
            _tableAlias + slbOrderedQty,
            _tableAlias + slbReturnedQty,
//            _tableAlias + slbRemainingCount = slbInStockQty - (slbOrderedQty - slbReturnedQty)
            _tableAlias + slbVoucherTemplate,
            _tableAlias + slbStatus,
            _tableAlias + slbCreatedBy_usrID,
            _tableAlias + slbCreationDateTime,
            _tableAlias + slbUpdatedBy_usrID,
        };
    }

}

namespace tblAccountUserAssetsBase {
    constexpr char Name[] = "tblAccountUserAssets";

    TARGOMAN_CREATE_CONSTEXPR(uasID);
    TARGOMAN_CREATE_CONSTEXPR(uas_usrID);
    TARGOMAN_CREATE_CONSTEXPR(uas_slbID);
    TARGOMAN_CREATE_CONSTEXPR(uasQty);
    TARGOMAN_CREATE_CONSTEXPR(uas_vchID);
    TARGOMAN_CREATE_CONSTEXPR(uasVoucherItemUUID);
    TARGOMAN_CREATE_CONSTEXPR(uas_cpnID);
    TARGOMAN_CREATE_CONSTEXPR(uasDiscountAmount);
    TARGOMAN_CREATE_CONSTEXPR(uasPrefered);
    TARGOMAN_CREATE_CONSTEXPR(uasOrderDateTime);
    TARGOMAN_CREATE_CONSTEXPR(uasStatus);
    TARGOMAN_CREATE_CONSTEXPR(uasUpdatedBy_usrID);
}

namespace tblAccountAssetUsageBase {
    constexpr char Name[] = "tblAccountAssetUsage";
    TARGOMAN_CREATE_CONSTEXPR(usg_uasID);
}

///TODO: max usage count (user, system)
///TODO: CodeCommentMark
namespace tblAccountCouponsBase {
    constexpr char Name[] = "tblAccountCoupons";
    TARGOMAN_CREATE_CONSTEXPR(cpnID);
    TARGOMAN_CREATE_CONSTEXPR(cpnCode);

    //rules
    TARGOMAN_CREATE_CONSTEXPR(cpnPrimaryCount);
    TARGOMAN_CREATE_CONSTEXPR(cpnTotalMaxAmount);   //nullable
    TARGOMAN_CREATE_CONSTEXPR(cpnPerUserMaxCount);  //nullable
    TARGOMAN_CREATE_CONSTEXPR(cpnPerUserMaxAmount); //nullable
    TARGOMAN_CREATE_CONSTEXPR(cpnValidFrom);
    TARGOMAN_CREATE_CONSTEXPR(cpnExpiryTime);

    //actions
    TARGOMAN_CREATE_CONSTEXPR(cpnAmount);
    TARGOMAN_CREATE_CONSTEXPR(cpnAmountType);
    TARGOMAN_CREATE_CONSTEXPR(cpnMaxAmount);
    TARGOMAN_CREATE_CONSTEXPR(cpnSaleableBasedMultiplier);

    //stats
    TARGOMAN_CREATE_CONSTEXPR(cpnTotalUsedCount);
    TARGOMAN_CREATE_CONSTEXPR(cpnTotalUsedAmount);

    TARGOMAN_CREATE_CONSTEXPR(cpnStatus);
    TARGOMAN_CREATE_CONSTEXPR(cpnCreatedBy_usrID);
    TARGOMAN_CREATE_CONSTEXPR(cpnCreationDateTime);
    TARGOMAN_CREATE_CONSTEXPR(cpnUpdatedBy_usrID);
}

namespace tblAccountReferalsBase {
    constexpr char Name[] = "tblAccountReferals";

    TARGOMAN_CREATE_CONSTEXPR(ref_usrID);
    TARGOMAN_CREATE_CONSTEXPR(refValidFromDateTime);
    TARGOMAN_CREATE_CONSTEXPR(refValidToDateTime);
    TARGOMAN_CREATE_CONSTEXPR(refPrizeInfo);
    TARGOMAN_CREATE_CONSTEXPR(refStatus);
    TARGOMAN_CREATE_CONSTEXPR(refCreatedBy_usrID);
    TARGOMAN_CREATE_CONSTEXPR(refCreationDateTime);
    TARGOMAN_CREATE_CONSTEXPR(refUpdatedBy_usrID);
}

#pragma GCC diagnostic pop

} //namespace Targoman::API::AAA

TAPI_DECLARE_METATYPE(Targoman::API::AAA::stuPrize)
TAPI_DECLARE_METATYPE(Targoman::API::AAA::stuDiscount3)
TAPI_DECLARE_METATYPE(Targoman::API::AAA::stuVoucherItem)         // -> TAPI_REGISTER_METATYPE() in Accounting_Interfaces.cpp
TAPI_DECLARE_METATYPE(Targoman::API::AAA::stuUsage)

TAPI_DECLARE_METATYPE(Targoman::API::AAA::stuPreVoucher)          // -> TAPI_REGISTER_METATYPE() in Accounting_Interfaces.cpp
TAPI_DECLARE_METATYPE(Targoman::API::AAA::stuVoucher)             // -> TAPI_REGISTER_METATYPE() in Accounting_Interfaces.cpp
TAPI_DECLARE_METATYPE(Targoman::API::AAA::OrderAdditives_t)       // -> TAPI_REGISTER_METATYPE() in Accounting_Interfaces.cpp

TAPI_DECLARE_METATYPE(Targoman::API::AAA::stuVoucherItemForTrustedAction)   // -> TAPI_REGISTER_METATYPE() in Accounting_Interfaces.cpp

#endif // TARGOMAN_API_AAA_ACCOUNTING_DEFS_H
