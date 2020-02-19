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

namespace TAPI {
TARGOMAN_DEFINE_ENUM(enuUserPrefOnPackage,
                     JustThis,
                     UntillFinished,
                     Auto
                     )
}
namespace Targoman {
namespace API {
namespace AAA {
namespace Accounting{

struct stuUsage{
    qint16 PerDay;  // can be -1 for unlimited
    qint32 PerWeek; // can be -1 for unlimited
    qint32 PerMonth;// can be -1 for unlimited
    qint64 Total;   // can be -1 for unlimited

    stuUsage(qint16 _perDay = -1, qint32 _perWeek = -1, qint32 _perMonth = -1, qint64 _total = -1) :
        PerDay(_perDay),
        PerWeek(_perWeek),
        PerMonth(_perMonth),
        Total(_total)
    {;}

    QJsonObject toJson() const ;
    stuUsage& fromJson(const QJsonObject& _obj);
};

typedef QMap<QString, stuUsage> PackageRemaining_t;

struct stuPackage {
    quint64 PackageID;
    qint32 RemainingDays; // can be -1 for unlimited
    qint32 RemainingHours; // can be -1 for unlimited
    QDateTime LastBreakDate;
    PackageRemaining_t  Remaining;
    QJsonObject Properties;
    TAPI::enuUserPrefOnPackage::Type UserPreference;

    QDate StartDate; //can be invalid for normal accounts
    QDate EndDate; //can be invalid for normal accounts
    QTime StartTime; //can be invalid for normal accounts
    QTime EndTime; //can be invalid for normal accounts

    stuPackage(quint64 _pkgID = 0,
               qint32    _remainingDays = -1,
               qint32    _remainingHours = -1,
               QDateTime _lastBreakDate = {},
               PackageRemaining_t  _remaining = {},
               QDate     _startDate = {},
               QDate     _endDate = {},
               QTime     _startTime = {},
               QTime     _endTime = {},
               QJsonObject _properties = {},
               TAPI::enuUserPrefOnPackage::Type      _userPreference = TAPI::enuUserPrefOnPackage::Auto
                                                                       ) :
        PackageID(_pkgID),
        RemainingDays(_remainingDays),
        RemainingHours(_remainingHours),
        LastBreakDate(_lastBreakDate),
        Remaining(_remaining),
        Properties(_properties),
        UserPreference(_userPreference),
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
    QString Name;
};
//Break on first start time and first start date
extern intfAccounting* serviceAccounting(const QString& _serviceName);


}
}
}
}

#endif // TARGOMAN_API_AAA_ACCOUNTING_H
