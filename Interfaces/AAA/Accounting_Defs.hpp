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
 */

#ifndef TARGOMAN_API_AAA_ACCOUNTING_DEFS_H
#define TARGOMAN_API_AAA_ACCOUNTING_DEFS_H

#include "AAADefs.hpp"
#include "clsJWT.hpp"
#include "Interfaces/Common/GenericTypes.h"
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
                     Amount  = '$',
                     Free    = 'Z')
}

namespace Targoman {
namespace API {
namespace AAA {
namespace Accounting{

inline QString makeConfig(const QString& _name){return "/AAA/Accounting/" + _name;}
extern Common::Configuration::tmplConfigurable<QString> Secret;

TAPI_DEFINE_VARIANT_ENABLED_STRUCT(
        stuPrize,
        QString,      Desc      , QString()    , v.size(), v, v.toString(),
        QJsonObject , PrizeInfo , QJsonObject(), v.size(), v, v.toObject()
        );

TAPI_DEFINE_VARIANT_ENABLED_STRUCT(
        stuDiscount,
        quint64, ID       , 0        , v       , C2DBL(v), static_cast<quint64>(v.toDouble()),
        QString, Name     , QString(), v.size(), v       , v.toString(),
        quint32, Amount   , 0        , v       , C2DBL(v), static_cast<quint32>(v.toDouble()),
        quint32, MaxAmount, 0        , v       , C2DBL(v), static_cast<quint32>(v.toDouble()),
        TAPI::enuDiscountType::Type, Type, TAPI::enuDiscountType::Percent, v, v,static_cast<TAPI::enuDiscountType::Type>(v.toString().toLatin1().constData()[0])
);

TAPI_DEFINE_VARIANT_ENABLED_STRUCT(
        stuVoucherItem,
        QString     , Service   , QString()    , v.size(), v         , v.toString(),
        quint64     , OrderID   , 0            , v       , C2DBL(v)  , static_cast<quint64>(v.toDouble()),
        QString     , Desc      , QString()    , v.size(), v         , v.toString(),
        quint32     , Price     , 0            , v       , C2DBL(v)  , static_cast<quint32>(v.toDouble()),
        qint16      , Count     , 0            , v       , v         , static_cast<qint16>(v.toDouble()),
        stuDiscount , Discount  , stuDiscount(), v.ID>0  , v.toJson(), stuDiscount().fromJson(v.toObject()),
        quint32     , DisAmount , 0            , v       , C2DBL(v)  , static_cast<quint32>(v.toDouble()),
        quint8      , TaxPercent, 0            , v       , C2DBL(v)  , static_cast<quint8>(v.toInt()),
        quint32     , TaxAmount , 0            , v       , C2DBL(v)  , static_cast<quint32>(v.toDouble()),
        QString     , Sign      , QString()    , v.size(), v         , v.toString()
        )

/*****************************************************************************/

TAPI_DEFINE_VARIANT_ENABLED_STRUCT(
        stuUsage,
        qint32, PerDay,   -1, v>-1, v, static_cast<qint32>(v.toInt()),
        qint32, PerWeek,  -1, v>-1, v, static_cast<qint32>(v.toInt()),
        qint32, PerMonth, -1, v>-1, v, static_cast<qint32>(v.toInt()),
        qint64, Total,    -1, v>-1, v, static_cast<qint64>(v.toInt())
        );
typedef QMap<QString, stuUsage> PackageRemaining_t;

struct stuUsageColDefinition{
    QString PerDay;
    QString PerWeek;
    QString PerMonth;
    QString Total;
    stuUsageColDefinition(const QString& _perDay, const QString& _perWeek, const QString& _perMonth, const QString& _total) :
        PerDay(_perDay), PerWeek(_perWeek), PerMonth(_perMonth), Total(_total)
    {}
};
typedef QMap<QString, stuUsageColDefinition> PackageRemainingCols_t;

struct stuPackage {
    quint64 PackageID;
    QString PackageCode;
    qint32 RemainingDays; // can be -1 for unlimited
    qint8 RemainingHours; // can be -1 for unlimited
    PackageRemaining_t  Remaining;
    QJsonObject Properties;

    QDate StartDate; //can be invalid for normal accounts
    QDate EndDate; //can be invalid for normal accounts
    QTime StartTime; //can be invalid for normal accounts
    QTime EndTime; //can be invalid for normal accounts

    stuPackage(quint64   _pkgID = 0,
               QString   _pkgCode = {},
               qint32    _remainingDays = -1,
               qint8     _remainingHours = -1,
               PackageRemaining_t  _remaining = {},
               QDate     _startDate = {},
               QDate     _endDate = {},
               QTime     _startTime = {},
               QTime     _endTime = {},
               QJsonObject _properties = {}) :
        PackageID(_pkgID),
        PackageCode(_pkgCode),
        RemainingDays(_remainingDays),
        RemainingHours(_remainingHours),
        Remaining(_remaining),
        Properties(_properties),
        StartDate(_startDate),
        EndDate(_endDate),
        StartTime(_startTime),
        EndTime(_endTime)
    {}

    QJsonObject toJson(bool _full);
    stuPackage& fromJson(const QJsonObject& _obj);
};

typedef QMap<QString, stuPackage> ActivePackages_t;

struct stuServiceAccountInfo{
    ActivePackages_t            ActivePackages;
    QSharedPointer<stuPackage>  PreferedPackage;
    quint32                     ParentID; // 0 for no parent
    PackageRemaining_t          MyLimitsOnParent;

    stuServiceAccountInfo(ActivePackages_t _activePackages,
                          QSharedPointer<stuPackage> _preferedPackage,
                          quint32          _parentID,
                          PackageRemaining_t  _myLimitsOnParent
                          );
};

struct stuActiveServiceAccount{
    stuPackage ActivePackage;
    bool       IsFromParent;
    PackageRemaining_t MyLimitsOnParent;
    qint64     TTL;

    stuActiveServiceAccount(const stuPackage& _package = {},
                            bool _isFromParent = false,
                            const PackageRemaining_t& _myLimitsOnParent = {},
                            qint64 _ttl = 0);
    QJsonObject toJson(bool _full);
    stuActiveServiceAccount& fromJson(const QJsonObject _obj);
};

}}}}

#include <QJsonArray>
namespace TAPI {
using namespace Targoman::API::AAA::Accounting;
typedef QList<stuVoucherItem> InvItems_t;

TAPI_DEFINE_VARIANT_ENABLED_STRUCT(
        stuPreVoucher,
        InvItems_t    , Items   , InvItems_t() , v.size(),
            [](auto v){QJsonArray A; foreach(auto a, v) A.append(a.toJson()); return A;}(v),
            [](auto v){InvItems_t L; foreach(auto I, v.toArray()) L.append(stuVoucherItem().fromJson(I.toObject())); return L;}(v),
        stuPrize      , Prize     , stuPrize()   , v.Desc.size(), v.toJson(), stuPrize().fromJson(v.toObject()),
        QString       , Summary   , QString()    , v.size()     , v          , v.toString(),
        quint16       , Round     , 0            , v>0          , v          , static_cast<quint16>(v.toInt()),
        quint32       , ToPay     , 0            , v>0          , C2DBL(v)   , static_cast<quint32>(v.toDouble()),
        QString       , Sign      , QString()    , v.size()     , v         , v.toString()
);

TAPI_DEFINE_VARIANT_ENABLED_STRUCT(
        stuVoucher,
        quint64               , ID          , 0                    , v>0     , C2DBL(v)   , static_cast<quint64>(v.toDouble()),
        stuPreVoucher         , Info        , stuPreVoucher()      , v.ToPay , v.toJson(), stuPreVoucher().fromJson(v.toObject()),
        QString               , PaymentLink , QString()            , v.size(), v         , v.toString(),
        enuVoucherStatus::Type, Status      , enuVoucherStatus::New, v       , v         , static_cast<enuVoucherStatus::Type>(v.toString().toLatin1().constData()[0])
);

}

namespace Targoman {
namespace API {
namespace AAA {
namespace Accounting{

typedef QMap<QString, qint32> ServiceUsage_t;
extern void checkPreVoucherSanity(TAPI::stuPreVoucher _preVoucher);

/***************************************************************************************************/
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-variable"
namespace tblAccountPackagesBase {
constexpr char Name[] = "tblAccountPackages";
TARGOMAN_CREATE_CONSTEXPR(pkgID);
TARGOMAN_CREATE_CONSTEXPR(pkgCode);
TARGOMAN_CREATE_CONSTEXPR(pkgName);
TARGOMAN_CREATE_CONSTEXPR(pkgDesc);
TARGOMAN_CREATE_CONSTEXPR(pkgType);
TARGOMAN_CREATE_CONSTEXPR(pkgRemainingDays);
TARGOMAN_CREATE_CONSTEXPR(pkgValidFromDate);
TARGOMAN_CREATE_CONSTEXPR(pkgValidToDate);
TARGOMAN_CREATE_CONSTEXPR(pkgValidFromTime);
TARGOMAN_CREATE_CONSTEXPR(pkgValidToTime);
TARGOMAN_CREATE_CONSTEXPR(pkg_locID);
TARGOMAN_CREATE_CONSTEXPR(pkgPrivs);
TARGOMAN_CREATE_CONSTEXPR(pkgPrice);
TARGOMAN_CREATE_CONSTEXPR(pkgCanBePurchasedSince);
TARGOMAN_CREATE_CONSTEXPR(pkgNotAvailableSince);
TARGOMAN_CREATE_CONSTEXPR(pkgVoucherTemplate);
TARGOMAN_CREATE_CONSTEXPR(pkgCreatedBy_usrID);
TARGOMAN_CREATE_CONSTEXPR(pkgCreationDateTime);
TARGOMAN_CREATE_CONSTEXPR(pkgUpdatedBy_usrID);
TARGOMAN_CREATE_CONSTEXPR(pkgStatus);
}

namespace tblAccountUserPackages {
constexpr char Name[] = "tblAccountUserPackages";
TARGOMAN_CREATE_CONSTEXPR(aupID);
TARGOMAN_CREATE_CONSTEXPR(aup_usrID);
TARGOMAN_CREATE_CONSTEXPR(aup_pkgID);
TARGOMAN_CREATE_CONSTEXPR(aupPrefered);
TARGOMAN_CREATE_CONSTEXPR(aupPurchaseRequestDateTime);
TARGOMAN_CREATE_CONSTEXPR(aupPaymentDateTime);
TARGOMAN_CREATE_CONSTEXPR(aup_vchID);
TARGOMAN_CREATE_CONSTEXPR(aupUpdatedBy_usrID);
TARGOMAN_CREATE_CONSTEXPR(aupStatus);
}

namespace tblAccountDiscounts {
constexpr char Name[] = "tblAccountDiscounts";
TARGOMAN_CREATE_CONSTEXPR(disID);
TARGOMAN_CREATE_CONSTEXPR(disCode);
TARGOMAN_CREATE_CONSTEXPR(disType);
TARGOMAN_CREATE_CONSTEXPR(disPackageBasedAmount);
TARGOMAN_CREATE_CONSTEXPR(disMaxAmount);
TARGOMAN_CREATE_CONSTEXPR(disValidFrom);
TARGOMAN_CREATE_CONSTEXPR(disExpiryTime);
TARGOMAN_CREATE_CONSTEXPR(disPrimaryCount);
TARGOMAN_CREATE_CONSTEXPR(disUsedCount);
TARGOMAN_CREATE_CONSTEXPR(disCreatedBy_usrID);
TARGOMAN_CREATE_CONSTEXPR(disCreationDateTime);
TARGOMAN_CREATE_CONSTEXPR(disUpdatedBy_usrID);
TARGOMAN_CREATE_CONSTEXPR(disStatus);
}

namespace tblAccountReferals {
constexpr char Name[] = "tblAccountReferals";
TARGOMAN_CREATE_CONSTEXPR(ref_usrID);
TARGOMAN_CREATE_CONSTEXPR(refValidFromDateTime);
TARGOMAN_CREATE_CONSTEXPR(refValidToDateTime);
TARGOMAN_CREATE_CONSTEXPR(refPrizeInfo);
TARGOMAN_CREATE_CONSTEXPR(refCreatedBy_usrID);
TARGOMAN_CREATE_CONSTEXPR(refCreationDateTime);
TARGOMAN_CREATE_CONSTEXPR(refUpdatedBy_usrID);
TARGOMAN_CREATE_CONSTEXPR(refStatus);
}

#pragma GCC diagnostic pop
}
}
}
}


TAPI_DECLARE_METATYPE(TAPI::stuVoucher)
TAPI_DECLARE_METATYPE(TAPI::stuPreVoucher)
TAPI_DECLARE_METATYPE(TAPI::enuVoucherStatus::Type)
TAPI_DECLARE_METATYPE(TAPI::enuDiscountType::Type)

#endif // TARGOMAN_API_AAA_ACCOUNTING_DEFS_H
