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

typedef QMap<QString, stuUsage> AccountLimits_t;

struct stuAccountPackage {

    qint32 RemainingDays; // can be -1 for unlimited
    qint32 RemainingHours; // can be -1 for unlimited
    QDateTime LastBreakDate;
    AccountLimits_t  Limits;

    QDate StartDate; //can be invalid for normal accounts
    QDate EndDate; //can be invalid for normal accounts
    QTime StartTime; //can be invalid for normal accounts
    QTime EndTime; //can be invalid for normal accounts

    stuAccountPackage(qint32    _remainingDays = -1,
                      qint32    _remainingHours = -1,
                      QDateTime _lastBreakDate = {},
                      AccountLimits_t  _limits = {},
                      QDate     _startDate = {},
                      QDate     _endDate = {},
                      QTime     _startTime = {},
                      QTime     _endTime = {}
                                           ) :
        RemainingDays(_remainingDays),
        RemainingHours(_remainingHours),
        LastBreakDate(_lastBreakDate),
        Limits(_limits),
        StartDate(_startDate),
        EndDate(_endDate),
        StartTime(_startTime),
        EndTime(_endTime)
    {}

    QJsonObject toJson();
    stuAccountPackage& fromJson(const QJsonObject& _obj);
};

typedef QMap<QString, stuAccountPackage> ActivePackages_t;

struct stuUsageMap{
    stuUsage Mine;
    stuUsage Parent;

    stuUsageMap(const stuUsage& _mine, const stuUsage& _parent) :
        Mine(_mine),
        Parent(_parent)
    {}
};

struct stuAccountInfo{
    ActivePackages_t ActivePackages;
    quint32          ParentID; // 0 for no parent
    stuUsage         MyLimitsOnParent;

    QJsonObject toJson();
    stuAccountInfo& fromJson(const QJsonObject& _obj);
};

typedef QMap<QString, quint32> AccountUsage_t;
class intfAccounting{
public:
    QJsonObject activeAccounts(quint32 _usrID);

protected:
    intfAccounting(const QString& _name);
    virtual ~intfAccounting();
    virtual stuAccountInfo retrieveAccountsInfo(quint32 _usrID) = 0;
    virtual void breakActiveAccount(quint64 _accID) = 0;
    virtual stuUsageMap updateUsage(quint64 _accID) = 0;
    virtual bool isUnlimited(const AccountLimits_t& _limits) const = 0;
    virtual bool isEmpty(const AccountLimits_t& _limits) const = 0;

    void hasCredit(const clsJWT& _jwt, const AccountUsage_t& _requestedUsage);
    //TODO prune accounts based on least JWT
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
