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

#ifndef TARGOMAN_API_AAA_ACCOUNTING_H
#define TARGOMAN_API_AAA_ACCOUNTING_H

#include "AAADefs.hpp"
#include "clsJWT.hpp"
#include "Interfaces/Common/GenericTypes.h"

namespace TAPI {
TARGOMAN_DEFINE_ENUM(enuInvoiceStatus,
                     New      = 'N',
                     Canceled = 'C',
                     Finished = 'F',
                     Removed  = 'R'
                                )
}

namespace Targoman {
namespace API {
namespace AAA {
namespace Accounting{

TARGOMAN_DEFINE_ENUM(enuDiscountType,
                     Percent = '%',
                     Amount  = '$',
                     Free    = 'Z')

TAPI_DEFINE_VARIANT_ENABLED_STRUCT(
        stuPrize,
        QString,      Desc      , QString()    , v.size(), v, v.toString(),
        QJsonObject , PrizeInfo , QJsonObject(), v.size(), v, v.toObject()
        );

TAPI_DEFINE_VARIANT_ENABLED_STRUCT(
        stuDiscount,
        qint64 , ID       , 0        , v > -1  , v, static_cast<qint64>(v.toDouble()),
        QString, Name     , QString(), v.size(), v, v.toString(),
        qint64 , Amount   , 0        , v       , v, static_cast<qint64>(v.toDouble()),
        qint32 , MaxAmount, 0        , v       , v, static_cast<qint32>(v.toDouble()),
        enuDiscountType::Type, Type, enuDiscountType::Percent, v, v,static_cast<enuDiscountType::Type>(v.toString().toLatin1().constData()[0])
        );

TAPI_DEFINE_VARIANT_ENABLED_STRUCT(
        stuInvoiceItem,
        QString     , Desc      , QString()    , v.size(), v         , v.toString(),
        qint64      , Price     , 0            , v       , v         , static_cast<qint64>(v.toDouble()),
        qint16      , Count     , 0            , v       , v         , static_cast<qint16>(v.toDouble()),
        stuDiscount , Discount  , stuDiscount(), v.ID>0  , v.toJson(), stuDiscount().fromJson(v.toObject()),

        qint8       , TaxPercent, 0            , v       , v         , static_cast<qint8>(v.toInt()),
        qint64      , TaxAmount , 0            , v       , v         , static_cast<qint64>(v.toDouble())
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
    ActivePackages_t ActivePackages;
    QSharedPointer<stuPackage> PreferedPackage;
    quint32          ParentID; // 0 for no parent
    PackageRemaining_t  MyLimitsOnParent;

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

typedef QMap<QString, qint32> ServiceUsage_t;

class intfAccounting{
public:
    stuActiveServiceAccount activeAccountObject(quint32 _usrID);

protected:
    intfAccounting(const QString& _name);
    virtual ~intfAccounting();
    virtual stuServiceAccountInfo retrieveServiceAccountInfo(quint32 _usrID) = 0;
    virtual void breakPackage(quint64 _pkgID) = 0;
    virtual bool isUnlimited(const PackageRemaining_t& _limits) const = 0;
    virtual bool isEmpty(const PackageRemaining_t& _limits) const = 0;

    void hasCredit(const clsJWT& _jwt, const ServiceUsage_t& _requestedUsage);

private:
    stuActiveServiceAccount findActiveAccount(quint32 _usrID, const ServiceUsage_t& _requestedUsage = {});

private:
    QString ServiceName;
};
//Break on first start time and first start date
extern intfAccounting* serviceAccounting(const QString& _serviceName);


}
}
}
}

#include <QJsonArray>
namespace TAPI {
using namespace Targoman::API::AAA::Accounting;
typedef QList<stuInvoiceItem> InvItems_t;

TAPI_DEFINE_VARIANT_ENABLED_STRUCT(
        stuPreInvoice,
        InvItems_t    , Items   , InvItems_t() , v.size(),
            [](auto v){QJsonArray A; foreach(auto a, v) A.append(a.toJson()); return A;}(v),
            [](auto v){InvItems_t L; foreach(auto I, v.toArray()) L.append(stuInvoiceItem().fromJson(I.toObject())); return L;}(v),
        stuPrize      , Prize   , stuPrize()   , v.Desc.size(), v.toJson(), stuPrize().fromJson(v.toObject()),
        QString       , Summary , QString()    , v.size()     , v          , v.toString(),
        qint64        , ToPay   , 0            , v>-1         , v          , static_cast<qint64>(v.toDouble())
)

TAPI_DEFINE_VARIANT_ENABLED_STRUCT(
        stuInvoice,
        MD5_t                 , ID          , MD5_t()              , v.size(), v         , v.toString(),
        stuPreInvoice         , Info        , stuPreInvoice()      , v.ToPay , v.toJson(), stuPreInvoice().fromJson(v.toObject()),
        QString               , PaymentLink , QString()            , v.size(), v         , v.toString(),
        enuInvoiceStatus::Type, Status      , enuInvoiceStatus::New, v       , v         ,static_cast<enuInvoiceStatus::Type>(v.toString().toLatin1().constData()[0])
)

}

TAPI_DECLARE_METATYPE(TAPI::stuInvoice)
TAPI_DECLARE_METATYPE(TAPI::enuInvoiceStatus::Type)

#endif // TARGOMAN_API_AAA_ACCOUNTING_H
