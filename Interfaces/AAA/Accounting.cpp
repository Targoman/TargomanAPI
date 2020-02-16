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

#include "Accounting.h"
#include "PrivHelpers.h"

namespace Targoman {
namespace API {
namespace AAA {
namespace Accounting{

constexpr char ACC_REMAININGDAYS[] = "RD";
constexpr char ACC_REMAININGHOURS[] = "RH";
constexpr char ACC_LASTBREAKDATE[] = "BD";
constexpr char ACC_STARTDATE[] = "SD";
constexpr char ACC_ENDDATE[] = "ED";
constexpr char ACC_STARTTIME[] = "ST";
constexpr char ACC_ENDTIME[] = "ET";
constexpr char ACC_LIMITS[] = "LM";
constexpr char LIMIT_PERDAY[] = "PD";
constexpr char LIMIT_PERWEEK[] = "PW";
constexpr char LIMIT_PERMONTH[] = "PM";
constexpr char LIMIT_TOTAL[] = "TT";
constexpr char INFO_PACKAGES[] = "PK";
constexpr char INFO_PARENT[] = "PR";
constexpr char INFO_LIMITS[] = "LM";

static QMap<QString, intfAccounting*> ServiceRegistry;

/******************************************************************/
QJsonObject stuAccountPackage::toJson()
{
    QJsonObject Info;
    if(this->RemainingDays > -1) Info[ACC_REMAININGDAYS] = this->RemainingDays;
    if(this->RemainingHours > -1) Info[ACC_REMAININGHOURS] = this->RemainingHours;
    if(this->LastBreakDate.isValid()) Info[ACC_LASTBREAKDATE] = this->LastBreakDate.toString();
    if(this->StartDate.isValid()) Info[ACC_STARTDATE] = this->StartDate.toString();
    if(this->EndDate.isValid()) Info[ACC_ENDDATE] = this->EndDate.toString();
    if(this->StartTime.isValid()) Info[ACC_STARTTIME] = this->StartTime.toString();
    if(this->EndTime.isValid()) Info[ACC_ENDTIME] = this->EndTime.toString();
    QJsonObject Limits;
    for(auto LimitIter = this->Limits.begin();
        LimitIter != this->Limits.end();
        LimitIter++)
        Limits.insert(LimitIter.key(), LimitIter->toJson());
    Info[ACC_LIMITS] = Limits;
    return Info;
}

stuAccountPackage&stuAccountPackage::fromJson(const QJsonObject& _obj)
{
    this->RemainingDays = static_cast<qint32>(_obj.contains(ACC_REMAININGDAYS) ? _obj.value(ACC_REMAININGDAYS).toInt() : -1);
    this->LastBreakDate = _obj.contains(ACC_LASTBREAKDATE) ? QDateTime::fromString(_obj.value(ACC_LASTBREAKDATE).toString()) : QDateTime();
    this->StartDate = _obj.contains(ACC_STARTDATE) ? QDate::fromString(_obj.value(ACC_STARTDATE).toString()) : QDate();
    this->EndDate = _obj.contains(ACC_ENDDATE) ? QDate::fromString(_obj.value(ACC_ENDDATE).toString()) : QDate();
    this->StartTime = _obj.contains(ACC_STARTTIME) ? QTime::fromString(_obj.value(ACC_STARTTIME).toString()) : QTime();
    this->EndTime = _obj.contains(ACC_ENDTIME) ? QTime::fromString(_obj.value(ACC_ENDTIME).toString()) : QTime();
    QJsonObject Limits = _obj.value(ACC_LIMITS).toObject();
    for(auto LimitIter = Limits.begin();
        LimitIter != Limits.end();
        LimitIter++)
        this->Limits.insert(LimitIter.key(), stuUsage().fromJson(LimitIter->toObject()));

    return *this;
}

/******************************************************************/
QJsonObject stuUsage::toJson() const
{
    QJsonObject Limit;
    if(this->PerDay > -1) Limit[LIMIT_PERDAY] = this->PerDay;
    if(this->PerWeek > -1) Limit[LIMIT_PERWEEK] = this->PerWeek;
    if(this->PerMonth > -1) Limit[LIMIT_PERMONTH] = this->PerMonth;
    if(this->Total > -1) Limit[LIMIT_TOTAL] = this->Total;
    return Limit;
}

stuUsage& stuUsage::fromJson(const QJsonObject& _obj)
{
    this->PerDay   = static_cast<qint16>(_obj.contains(LIMIT_PERDAY) ? _obj.value(LIMIT_PERDAY).toInt() : -1);
    this->PerWeek  = static_cast<qint32>(_obj.contains(LIMIT_PERWEEK) ? _obj.value(LIMIT_PERWEEK).toInt() : -1);
    this->PerMonth = static_cast<qint32>(_obj.contains(LIMIT_PERMONTH) ? _obj.value(LIMIT_PERMONTH).toInt() : -1);
    this->Total    = static_cast<qint64>(_obj.contains(LIMIT_TOTAL) ? _obj.value(LIMIT_TOTAL).toInt() : -1);
    return *this;
}

/******************************************************************/
QJsonObject stuAccountInfo::toJson()
{
    QJsonObject Info;
    QJsonObject ActivePackages;
    for(auto Package = this->ActivePackages.begin();
        Package != this->ActivePackages.end();
        Package++)
        ActivePackages[Package.key()] = Package->toJson();
    Info[INFO_PACKAGES] = ActivePackages;
    if(this->ParentID) {
        Info[INFO_PARENT] = static_cast<qint32>(this->ParentID);
        Info[INFO_LIMITS] = this->MyLimitsOnParent.toJson();
    }
    return Info;
}

stuAccountInfo& stuAccountInfo::fromJson(const QJsonObject& _obj)
{
    QJsonObject ActivePackages = _obj.value(INFO_PACKAGES).toObject();
    for(auto PackageIter = ActivePackages.begin();
        PackageIter != ActivePackages.end();
        PackageIter++)
        this->ActivePackages.insert(PackageIter.key(), stuAccountPackage().fromJson(PackageIter.value().toObject()));
    if(_obj.contains(INFO_PARENT)){
        this->ParentID = static_cast<quint32>(_obj.value(INFO_PARENT).toInt());
        this->MyLimitsOnParent = stuUsage().fromJson(_obj.value(INFO_LIMITS).toObject());
    }
    return *this;
}

/******************************************************************/
intfAccounting* serviceAccounting(const QString& _serviceName)
{
    Q_UNUSED(serviceAccounting);
    return ServiceRegistry.value(_serviceName);
}

QJsonObject intfAccounting::activeAccounts(quint32 _usrID)
{
    return { {this->Name, this->retrieveAccountsInfo(_usrID).toJson() } };
}

intfAccounting::intfAccounting(const QString& _name) :
    Name(_name)
{;}

intfAccounting::~intfAccounting()
{;}

void intfAccounting::hasCredit(const clsJWT& _jwt, const AccountUsage_t& _requestedUsage)
{
    QJsonObject Privs = _jwt.privsObject();
    if(Privs.contains(this->Name) == false)
        throw exHTTPForbidden("[81] You don't have access to: " + this->Name);

    stuAccountInfo AccountInfo = stuAccountInfo().fromJson(Privs.value(this->Name).toObject());
    if(AccountInfo.ActivePackages.isEmpty())
        throw exHTTPForbidden("[82] You don't have access to: " + this->Name);

    QList<stuAccountPackage> CandidatePackages;
    QDateTime Now = QDateTime::currentDateTime();

    for(auto AccountIter = AccountInfo.ActivePackages.begin();
        AccountIter != AccountInfo.ActivePackages.end();
        AccountIter++){
        const stuAccountPackage& Package = AccountIter.value();
        static auto effectiveStartTime = [Now, Package](){ return QDateTime(Now.date()).addMSecs(Package.StartTime.msecsSinceStartOfDay());};
        static auto effectiveEndTime  = [Now, Package](){
            return Package.StartTime < Package.EndTime ?
                        QDateTime(Now.date()).addMSecs(Package.EndTime.msecsSinceStartOfDay())
                      : QDateTime(Now.date().addDays(1)).addMSecs(Package.EndTime.msecsSinceStartOfDay()) ;
        };

        //Validate
        if((Package.StartDate.isValid() && Package.StartDate > Now.date())
           ||(Package.EndDate.isValid() && Package.EndDate < Now.date())
           ||(Package.StartTime.isValid() && Now < effectiveStartTime())
           ||(Package.EndTime.isValid() && Now > effectiveEndTime())
           ||this->isEmpty(Package.Limits)
           )
            continue;
        if(CandidatePackages.isEmpty())
            CandidatePackages.append(Package);

        //for(auto CandidateIter = this->)
    }
}
//TODO seems that patch function is creating wrong example with value instead of null
//TODO reset everyday for time period
/*void checkCredit(const QJsonObject& _privs, const QString& _selector, double _value)
{
    QVariant Credit =
    if(Credit.isValid() &&
       Credit.canConvert<double>() &&
       Credit.toDouble() >= 0 &&
       Credit.toDouble() < _value)
        throw exAccounting("You have exceeded your "+_selector+" quota.");
}*/

}
}
}
}

