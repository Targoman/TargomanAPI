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
                     New      = 'N',
                     Canceled = 'C',
                     Error    = 'E',
                     Finished = 'F',
                     Removed  = 'R',
                     );
TARGOMAN_DEFINE_ENUM(enuDiscountType,
                     Percent = '%',
                     Currency  = '$',
                     );

//inline QString makeConfig(const QString& _name) { return "/zModule_Account/" + _name; }
inline QString makeConfig(const QString& _name) { return "/Module_Account/" + _name; }
extern Targoman::Common::Configuration::tmplConfigurable<QString> Secret;

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
    SF_QString(Service),    //QString      , Service    , QString()      , v.size() , v          , v.toString()                          ,
    SF_quint64(OrderID),    //quint64      , OrderID    , 0              , v        , C2DBL(v)   , static_cast<quint64>(v.toDouble())    ,
    SF_MD5_t(UUID),         //TAPI::MD5_t  , UUID       , QString()      , v.size() , v          , v.toString()                          ,
    SF_QString(Desc),       //QString      , Desc       , QString()      , v.size() , v          , v.toString()                          ,
    SF_quint32(UnitPrice),  //quint32      , UnitPrice  , 0              , v        , C2DBL(v)   , static_cast<quint32>(v.toDouble())    ,
    SF_qreal(Qty),          //qreal        , Qty        , 0              , v        , C2DBL(v)   , v.toDouble()                          ,
    SF_quint32(SubTotal),   //quint32      , SubTotal   , 0              , v        , C2DBL(v)   , static_cast<quint32>(v.toDouble())    ,
    SF_Generic(             //stuDiscount3 , Discount   , stuDiscount3() , v.ID>0   , v.toJson() , stuDiscount3().fromJson(v.toObject()) ,
        /* type        */ stuDiscount3,
        /* name        */ Discount,
        /* def         */ stuDiscount3(),
        /* validator   */ v.ID > 0,
        /* fromVariant */ v.toJson(),
        /* toVariant   */ stuDiscount3().fromJson(v.toObject())
    ),
    SF_quint32(DisAmount),  //quint32      , DisAmount  , 0              , v        , C2DBL(v)   , static_cast<quint32>(v.toDouble())    ,
    SF_quint8(VATPercent),  //quint8       , VATPercent , 0              , v        , C2DBL(v)   , static_cast<quint8>(v.toInt())        ,
    SF_quint32(VATAmount),  //quint32      , VATAmount  , 0              , v        , C2DBL(v)   , static_cast<quint32>(v.toDouble())    ,
    SF_QString(Sign)        //QString      , Sign       , QString()      , v.size() , v          , v.toString()
);

/*****************************************************************************/
TAPI_DEFINE_VARIANT_ENABLED_STRUCT(stuUsage,
    SF_NULLABLE_quint32(PerDay),
    SF_NULLABLE_quint32(PerWeek),
    SF_NULLABLE_quint32(PerMonth),
    SF_NULLABLE_quint64(Total)
);

typedef QMap<QString, stuUsage> UsageLimits_t;

struct stuUsageColDefinition {
    QString PerDay;
    QString PerWeek;
    QString PerMonth;
    QString Total;

    stuUsageColDefinition(const QString& _perDay, const QString& _perWeek, const QString& _perMonth, const QString& _total) :
        PerDay(_perDay), PerWeek(_perWeek), PerMonth(_perMonth), Total(_total)
    {}
};

typedef QMap<QString, stuUsageColDefinition> AssetUsageLimitsCols_t;

typedef QMap<QString, QString> OrderAdditives_t;

struct stuAssetItem {
    //product
    quint32                         prdID;
    TAPI::ProductCode_t             prdCode;
    QString                         prdName;
    TAPI::Date_t                    prdValidFromDate;
    TAPI::Date_t                    prdValidToDate;
    NULLABLE_TYPE(quint8)           prdValidFromHour;
    NULLABLE_TYPE(quint8)           prdValidToHour;
    TAPI::PrivObject_t              prdPrivs;
    NULLABLE_TYPE(double)           prdVAT;
    quint32                         prdInStockQty;
    NULLABLE_TYPE(quint32)          prdOrderedQty;
    NULLABLE_TYPE(quint32)          prdReturnedQty;
    TAPI::enuGenericStatus::Type    prdStatus;

    //saleable
    quint32                         slbID;
    TAPI::SaleableCode_t            slbCode;
    QString                         slbName;
    TAPI::JSON_t                    slbPrivs;
    qreal                           slbBasePrice;
    TAPI::SaleableAdditive_t        slbAdditives;
//    quint32                         slbProductCount;
    NULLABLE_TYPE(quint32)          slbMaxSaleCountPerUser;
    quint32                         slbInStockQty;
    NULLABLE_TYPE(quint32)          slbOrderedQty;
    NULLABLE_TYPE(quint32)          slbReturnedQty;
    QString                         slbVoucherTemplate;
    TAPI::enuGenericStatus::Type    slbStatus;

    qreal                           SubTotal;
    NULLABLE_TYPE(stuDiscount3)     Discount;
    qreal                           TotalPrice;

    struct {
//        QJsonObject   Additives;
        UsageLimits_t Limits;
//        QJsonObject   Privs;
    } Digested;

    QJsonObject toJson(bool _full);
    stuAssetItem& fromJson(const QJsonObject& _obj);
    void fromVariantMap(const QVariantMap& _info);
};

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

struct stuActiveCredit {
    stuAssetItem  Credit;
    bool          IsFromParent;
    UsageLimits_t MyLimitsOnParent;
    qint64        TTL;

    stuActiveCredit(const stuAssetItem& _credit = {},
                    bool _isFromParent = false,
                    const UsageLimits_t& _myLimitsOnParent = {},
                    qint64 _ttl = 0);
    QJsonObject toJson(bool _full);
    stuActiveCredit& fromJson(const QJsonObject _obj);
};

typedef QList<stuVoucherItem> InvItems_t;

TAPI_DEFINE_VARIANT_ENABLED_STRUCT(stuPreVoucher,
    SF_Generic(
        /* type        */ InvItems_t,
        /* name        */ Items,
        /* def         */ InvItems_t(),
        /* validator   */ v.size(),
        /* fromVariant */ [](auto v){QJsonArray A; foreach(auto a, v) A.append(a.toJson()); return A;}(v),
        /* toVariant   */ [](auto v){InvItems_t L; foreach(auto I, v.toArray()) L.append(Targoman::API::AAA::stuVoucherItem().fromJson(I.toObject())); return L;}(v)
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
}

namespace tblAccountUserAssetsBase {
    constexpr char Name[] = "tblAccountUserAssets";
    TARGOMAN_CREATE_CONSTEXPR(uasID);
    TARGOMAN_CREATE_CONSTEXPR(uas_usrID);
    TARGOMAN_CREATE_CONSTEXPR(uas_slbID);
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

TAPI_DECLARE_METATYPE_ENUM(Targoman::API::AAA, enuVoucherStatus)  // -> TAPI_REGISTER_TARGOMAN_ENUM() in Accounting_Interfaces.cpp
TAPI_DECLARE_METATYPE_ENUM(Targoman::API::AAA, enuDiscountType)   // -> TAPI_REGISTER_TARGOMAN_ENUM() in Accounting_Interfaces.cpp

#endif // TARGOMAN_API_AAA_ACCOUNTING_DEFS_H
