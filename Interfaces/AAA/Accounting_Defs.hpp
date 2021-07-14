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

#include "AAADefs.hpp"
#include "clsJWT.hpp"
#include "Interfaces/Common/GenericTypes.h"
#include "Interfaces/Common/QtTypes.hpp"
#include "libTargomanCommon/Configuration/tmplConfigurable.h"
#include "libTargomanCommon/Configuration/Validators.hpp"
#include <QString>

namespace TAPI {
TARGOMAN_DEFINE_ENUM(enuVoucherStatus,
                     New      = 'N',
                     Canceled = 'C',
                     Error    = 'E',
                     Finished = 'F',
                     Removed  = 'R'
                                )
TARGOMAN_DEFINE_ENUM(enuDiscountType,
                     Percent = '%',
                     Currency  = '$')
}

namespace Targoman {
namespace API {
namespace AAA {
namespace Accounting {

inline QString makeConfig(const QString& _name){return "/zModule_Account/" + _name;}
extern Common::Configuration::tmplConfigurable<QString> Secret;

TAPI_DEFINE_VARIANT_ENABLED_STRUCT(stuPrize,
    QString,      Desc      , QString()    , v.size(), v, v.toString(),
    QJsonObject , PrizeInfo , QJsonObject(), v.size(), v, v.toObject()
);

TAPI_DEFINE_VARIANT_ENABLED_STRUCT(stuDiscount,
    SF_quint64(ID),
    SF_QString(Name),
    SF_qreal(Amount),
    SF_quint32(MaxAmount),
    SF_Enum(TAPI::enuDiscountType, AmountType, TAPI::enuDiscountType::Percent)
);

TAPI_DEFINE_VARIANT_ENABLED_STRUCT(stuVoucherItem,
    QString     , Service   , QString()    , v.size(), v         , v.toString(),
    quint64     , OrderID   , 0            , v       , C2DBL(v)  , static_cast<quint64>(v.toDouble()),
    QString     , Desc      , QString()    , v.size(), v         , v.toString(),
    quint32     , UnitPrice , 0            , v       , C2DBL(v)  , static_cast<quint32>(v.toDouble()),
    qint16      , Qty       , 0            , v       , v         , static_cast<qint16>(v.toDouble()),
    quint32     , SubTotal  , 0            , v       , C2DBL(v)  , static_cast<quint32>(v.toDouble()),
    stuDiscount , Discount  , stuDiscount(), v.ID>0  , v.toJson(), stuDiscount().fromJson(v.toObject()),
    quint32     , DisAmount , 0            , v       , C2DBL(v)  , static_cast<quint32>(v.toDouble()),
    quint8      , VATPercent, 0            , v       , C2DBL(v)  , static_cast<quint8>(v.toInt()),
    quint32     , VATAmount , 0            , v       , C2DBL(v)  , static_cast<quint32>(v.toDouble()),
    QString     , Sign      , QString()    , v.size(), v         , v.toString()
);

/*****************************************************************************/
TAPI_DEFINE_VARIANT_ENABLED_STRUCT(stuUsage,
    SF_NULLABLE_quint32(PerDay),
    SF_NULLABLE_quint32(PerWeek),
    SF_NULLABLE_quint32(PerMonth),
    SF_NULLABLE_quint64(Total)
);

typedef QMap<QString, stuUsage> UsageLimits_t;

struct stuUsageColDefinition{
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
    TAPI::ProductCode_t          prdCode;
    QString                      prdName;
    TAPI::Date_t                 prdValidFromDate;
    TAPI::Date_t                 prdValidToDate;
    NULLABLE_TYPE(quint8)        prdValidFromHour;
    NULLABLE_TYPE(quint8)        prdValidToHour;
    TAPI::PrivObject_t           prdPrivs;
    NULLABLE_TYPE(double)        prdVAT;
    quint32                      prdInStockCount;
    NULLABLE_TYPE(quint32)       prdOrderedCount;
    NULLABLE_TYPE(quint32)       prdReturnedCount;
    TAPI::enuGenericStatus::Type prdStatus;

    //saleable
    quint32                      slbID;
    TAPI::SaleableCode_t         slbCode;
    QString                      slbName;
    TAPI::JSON_t                 slbPrivs;
    qreal                        slbBasePrice;
    TAPI::SaleableAdditive_t     slbAdditives;
    quint32                      slbProductCount;
    NULLABLE_TYPE(quint32)       slbMaxSaleCountPerUser;
    quint32                      slbInStockCount;
    NULLABLE_TYPE(quint32)       slbOrderedCount;
    NULLABLE_TYPE(quint32)       slbReturnedCount;
    QString                      slbVoucherTemplate;
    TAPI::enuGenericStatus::Type slbStatus;

    struct {
        QJsonObject   Additives;
        UsageLimits_t Limits;
        QJsonObject   Privs;
    } Digested;

    QJsonObject toJson(bool _full);
    stuAssetItem& fromJson(const QJsonObject& _obj);
};

typedef QMap<QString, stuAssetItem> ActiveCredits_t;

struct stuServiceCreditsInfo {
    ActiveCredits_t             ActiveCredits;
    NULLABLE_TYPE(stuAssetItem) PreferedCredit;
    NULLABLE_TYPE(quint32)      ParentID;
    UsageLimits_t               MyLimitsOnParent;
    QDateTime                   DBCurrentDateTime;

    stuServiceCreditsInfo(ActiveCredits_t         _activeCredits,
                          NULLABLE_TYPE(stuAssetItem) _preferedCredit,
                          NULLABLE_TYPE(quint32)      _parentID,
                          UsageLimits_t          _myLimitsOnParent,
                          QDateTime              _dbCurrentDateTime);
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

}}}}

#include <QJsonArray>
namespace TAPI {

//using namespace Targoman::API;
using namespace Targoman::API::AAA::Accounting;

typedef QList<stuVoucherItem> InvItems_t;

TAPI_DEFINE_VARIANT_ENABLED_STRUCT(stuPreVoucher,
    SF_Generic(
        /* type        */ InvItems_t,
        /* name        */ Items,
        /* def         */ InvItems_t(),
        /* validator   */ v.size(),
        /* fromVariant */ [](auto v){QJsonArray A; foreach(auto a, v) A.append(a.toJson()); return A;}(v),
        /* toVariant   */ [](auto v){InvItems_t L; foreach(auto I, v.toArray()) L.append(stuVoucherItem().fromJson(I.toObject())); return L;}(v)
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
    SF_Enum(enuVoucherStatus, Status, enuVoucherStatus::New)
);

//bool KKKKKKKKKKKKKKKKKK() {return true;}

TAPI_DEFINE_VARIANT_ENABLED_STRUCT(stuDiscountSaleableBasedMultiplier,
    SF_QString(SaleableCode),
    SF_qreal(Multiplier),
    SF_NULLABLE_quint32(MinCount)
);

} //namespace TAPI

//TAPI_DECLARE_METATYPE(TAPI::stuDiscountSaleableBasedMultiplier)

namespace Targoman {
namespace API {
namespace AAA {
namespace Accounting {

typedef QMap<QString, qint32> ServiceUsage_t;

extern void checkPreVoucherSanity(TAPI::stuPreVoucher _preVoucher);

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
TARGOMAN_CREATE_CONSTEXPR(prdInStockCount);
TARGOMAN_CREATE_CONSTEXPR(prdOrderedCount);
TARGOMAN_CREATE_CONSTEXPR(prdReturnedCount);
// prdRemainingCount = prdInStockCount - (prdOrderedCount - prdReturnedCount)

TARGOMAN_CREATE_CONSTEXPR(prdStatus);
TARGOMAN_CREATE_CONSTEXPR(prdCreatedBy_usrID);
TARGOMAN_CREATE_CONSTEXPR(prdCreationDateTime);
TARGOMAN_CREATE_CONSTEXPR(prdUpdatedBy_usrID);
}

namespace tblAccountSaleablesBase {
constexpr char Name[] = "tblAccountSaleables";
TARGOMAN_CREATE_CONSTEXPR(slbID);
TARGOMAN_CREATE_CONSTEXPR(slbCode);
TARGOMAN_CREATE_CONSTEXPR(slb_prdID);
TARGOMAN_CREATE_CONSTEXPR(slbName);
TARGOMAN_CREATE_CONSTEXPR(slbDesc);
TARGOMAN_CREATE_CONSTEXPR(slbType);
TARGOMAN_CREATE_CONSTEXPR(slbAvailableFromDate);
TARGOMAN_CREATE_CONSTEXPR(slbAvailableToDate);
TARGOMAN_CREATE_CONSTEXPR(slbPrivs);
TARGOMAN_CREATE_CONSTEXPR(slbBasePrice);
TARGOMAN_CREATE_CONSTEXPR(slbAdditives);
TARGOMAN_CREATE_CONSTEXPR(slbProductCount);
TARGOMAN_CREATE_CONSTEXPR(slbMaxSaleCountPerUser);

///TODO: create trigger for this 3 fields and make changes to prd Count fields
TARGOMAN_CREATE_CONSTEXPR(slbInStockCount);
TARGOMAN_CREATE_CONSTEXPR(slbOrderedCount);
TARGOMAN_CREATE_CONSTEXPR(slbReturnedCount);
// slbRemainingCount = slbInStockCount - (slbOrderedCount - slbReturnedCount)

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
TARGOMAN_CREATE_CONSTEXPR(uas_vchID);
TARGOMAN_CREATE_CONSTEXPR(uasVoucherItemUUID);
TARGOMAN_CREATE_CONSTEXPR(uas_cpnID);
TARGOMAN_CREATE_CONSTEXPR(uasPrefered);
TARGOMAN_CREATE_CONSTEXPR(uasOrderDateTime);
TARGOMAN_CREATE_CONSTEXPR(uasStatus);
TARGOMAN_CREATE_CONSTEXPR(uasUpdatedBy_usrID);
}

namespace tblAccountAssetUsageBase {
constexpr char Name[] = "tblAccountAssetUsage";
TARGOMAN_CREATE_CONSTEXPR(usg_uasID);
}

//TODO: max usage count (user, system)
//TODO: CodeCommentMark
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
}
}
}
}

TAPI_DECLARE_METATYPE(TAPI::stuVoucher)
TAPI_DECLARE_METATYPE(TAPI::OrderAdditives_t)
TAPI_DECLARE_METATYPE(TAPI::stuPreVoucher)
TAPI_DECLARE_METATYPE_ENUM(TAPI::enuVoucherStatus)
TAPI_DECLARE_METATYPE_ENUM(TAPI::enuDiscountType)

#endif // TARGOMAN_API_AAA_ACCOUNTING_DEFS_H
