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
#include "Server/ServerConfigs.h"

TAPI_REGISTER_METATYPE(
            COMPLEXITY_Complex,
            TAPI, stuInvoice,
            [](const TAPI::stuInvoice& _value) -> QVariant{return _value.toJson().toVariantMap();}
);
TAPI_REGISTER_TARGOMAN_ENUM(TAPI,enuInvoiceStatus);

namespace Targoman {
namespace API {
namespace AAA {
namespace Accounting{

constexpr char PKG_REMAININGDAYS[] = "RD";
constexpr char PKG_REMAININGHOURS[] = "RH";
constexpr char PKG_STARTDATE[] = "SD";
constexpr char PKG_ENDDATE[] = "ED";
constexpr char PKG_STARTTIME[] = "ST";
constexpr char PKG_ENDTIME[] = "ET";
constexpr char PKG_LIMITS[] = "LM";
constexpr char PKG_PROPS[] = "PP";

constexpr char ASA_PACKAGE[] = "PK";
constexpr char ASA_ISFROMPARENT[] = "IP";
constexpr char ASA_TTL[] = "TT";
constexpr char ASA_LIMITSONPARENT[] = "LP";


static QMap<QString, intfAccounting*> ServiceRegistry;

/******************************************************************/
QJsonObject stuPackage::toJson(bool _full)
{
    QJsonObject Info;
    if(this->RemainingDays > -1) Info[PKG_REMAININGDAYS] = this->RemainingDays;
    if(this->RemainingHours > -1) Info[PKG_REMAININGHOURS] = this->RemainingHours;
    if(this->StartDate.isValid()) Info[PKG_STARTDATE] = this->StartDate.toString();
    if(this->EndDate.isValid()) Info[PKG_ENDDATE] = this->EndDate.toString();
    if(this->StartTime.isValid()) Info[PKG_STARTTIME] = this->StartTime.toString();
    if(this->EndTime.isValid()) Info[PKG_ENDTIME] = this->EndTime.toString();
    if(this->Properties.size()) Info[PKG_PROPS] = this->Properties;
    if(_full){
        QJsonObject Limits;
        for(auto LimitIter = this->Remaining.begin();
            LimitIter != this->Remaining.end();
            LimitIter++)
            Limits.insert(LimitIter.key(), LimitIter->toJson());
        Info[PKG_LIMITS] = Limits;
    }
    return Info;
}

stuPackage&stuPackage::fromJson(const QJsonObject& _obj)
{
    this->RemainingDays = static_cast<qint32>(_obj.contains(PKG_REMAININGDAYS) ? _obj.value(PKG_REMAININGDAYS).toInt() : -1);
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
    return *this;
}

/******************************************************************/
stuActiveServiceAccount::stuActiveServiceAccount(const stuPackage& _package, bool _isFromParent, const PackageRemaining_t& _myLimitsOnParent, qint64 _ttl) :
    ActivePackage(_package),
    IsFromParent(_isFromParent),
    MyLimitsOnParent(_myLimitsOnParent),
    TTL(_ttl)
{;}

QJsonObject stuActiveServiceAccount::toJson(bool _full)
{
    QJsonObject Account = {
        {ASA_PACKAGE, this->ActivePackage.toJson(_full)},
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

stuActiveServiceAccount& stuActiveServiceAccount::fromJson(const QJsonObject _obj)
{
    this->ActivePackage = stuPackage().fromJson(_obj.value(ASA_PACKAGE).toObject());
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
stuServiceAccountInfo::stuServiceAccountInfo(
        ActivePackages_t _activePackages,
        QSharedPointer<stuPackage> _preferedPackage,
        quint32 _parentID,
        PackageRemaining_t _myLimitsOnParent) :
    ActivePackages(_activePackages),
    PreferedPackage(_preferedPackage),
    ParentID(_parentID),
    MyLimitsOnParent(_myLimitsOnParent)
{}

/******************************************************************/
intfAccounting* serviceAccounting(const QString& _serviceName)
{
    Q_UNUSED(serviceAccounting);
    return ServiceRegistry.value(_serviceName);
}

stuActiveServiceAccount intfAccounting::activeAccountObject(quint32 _usrID)
{
    return this->findActiveAccount(_usrID);
}

intfAccounting::intfAccounting(const QString& _name) :
    ServiceName(_name)
{
    ServiceRegistry.insert(_name, this);
}

intfAccounting::~intfAccounting()
{;}

stuActiveServiceAccount intfAccounting::findActiveAccount(quint32 _usrID, const ServiceUsage_t& _requestedUsage)
{
    stuServiceAccountInfo AccountInfo = this->retrieveServiceAccountInfo(_usrID);
    if(AccountInfo.ActivePackages.isEmpty())
        return stuActiveServiceAccount();

    QDateTime Now = QDateTime::currentDateTime();

    auto effectiveStartTime = [Now](const stuPackage& _package) -> QDateTime { return QDateTime(Now.date()).addMSecs(_package.StartTime.msecsSinceStartOfDay());};
    auto effectiveEndTime  = [Now](const stuPackage& _package) -> QDateTime{
        return _package.StartTime < _package.EndTime ?
                    QDateTime(Now.date()).addMSecs(_package.EndTime.msecsSinceStartOfDay())
                  : _package.EndDate == Now.date() ?
                        QDateTime(Now.date().addDays(1)) :
                        QDateTime(Now.date().addDays(1)).addMSecs(_package.EndTime.msecsSinceStartOfDay()) ;
    };
    auto isInvalidPackage = [this, Now, effectiveStartTime, effectiveEndTime, _requestedUsage](const stuPackage& _package) -> bool{
        if((_package.StartDate.isValid() && _package.StartDate > Now.date())
           ||(_package.EndDate.isValid() && _package.EndDate < Now.date())
           ||(_package.StartTime.isValid() && Now < effectiveStartTime(_package))
           ||(_package.EndTime.isValid() && Now > effectiveEndTime(_package))
           ||this->isEmpty(_package.Remaining)
           )
            return false;
        if(_requestedUsage.size()){
            for(auto UsageIter = _requestedUsage.begin();
                UsageIter != _requestedUsage.end();
                UsageIter++){
                if(_package.Remaining.contains(UsageIter.key()) == false)
                    continue;
                if(this->isUnlimited(_package.Remaining) == false){
                    stuUsage Remaining = _package.Remaining.value(UsageIter.key());
                    if((Remaining.PerDay >= 0 && Remaining.PerDay - UsageIter.value() <= 0)
                       ||(Remaining.PerWeek >= 0 && Remaining.PerWeek - UsageIter.value() <= 0)
                       ||(Remaining.PerMonth >= 0 && Remaining.PerMonth - UsageIter.value() <= 0)
                       ||(Remaining.Total >= 0 && Remaining.Total - UsageIter.value() <= 0))
                        return false;
                }
            }
        }
        return true;
    };

    if(AccountInfo.PreferedPackage.isNull() == false && isInvalidPackage(*AccountInfo.PreferedPackage) == false)
        return stuActiveServiceAccount(*AccountInfo.PreferedPackage,
                                       AccountInfo.ParentID ? true : false,
                                       AccountInfo.MyLimitsOnParent,
                                       -1);

    QList<stuPackage> CandidatePackages;
    auto comparePackages = [this, &effectiveEndTime] (const stuPackage& a, stuPackage& b) {
        if(a.EndDate.isValid() && b.EndDate.isValid() == false) return -1;
        if(a.EndDate.isValid() == false && b.EndDate.isValid()) return 1;
        if(this->isUnlimited(a.Remaining) && this->isUnlimited(b.Remaining) == false) return -1;
        if(this->isUnlimited(a.Remaining) == false && this->isUnlimited(b.Remaining)) return 1;
        if(a.EndTime.isValid() && b.EndTime.isValid() == false) return -1;
        if(a.EndTime.isValid() == false && b.EndTime.isValid()) return 1;
        if(a.EndTime.isValid() && b.EndTime.isValid()) {
            if(effectiveEndTime(a) < effectiveEndTime(b)) return -1;
            if(effectiveEndTime(a) > effectiveEndTime(b)) return 1;
        }
        if(a.EndDate.isValid() && b.EndDate.isValid() && a.EndDate != b.EndDate)
            return b.EndDate > a.EndDate ? -1 : 1;
        return 0;
    };

    for(auto AccountIter = AccountInfo.ActivePackages.begin();
        AccountIter != AccountInfo.ActivePackages.end();
        AccountIter++){
        const stuPackage& Package = AccountIter.value();

        if(isInvalidPackage(Package))
            continue;

        if(CandidatePackages.isEmpty()){
            CandidatePackages.append(Package);
            continue;
        }

        bool Inserted = false;
        for(auto CandidateIter = CandidatePackages.begin();
            CandidateIter != CandidatePackages.end();
            CandidateIter++){
            if( comparePackages(Package, *CandidateIter) <0) {
                this->breakPackage(CandidateIter->PackageID);
                CandidatePackages.insert(CandidateIter, Package);
                Inserted = true;
                break;
            }
        }
        if(Inserted == false)
            CandidatePackages.append(Package);
    }

    if(CandidatePackages.isEmpty()){
        if(_requestedUsage.size())
            throw exPaymentRequired("You don't have any active account");

        return stuActiveServiceAccount();
    }

    const stuPackage& ActivePackage = CandidatePackages.first();
    QDateTime NextDigestTime;
    if(ActivePackage.EndDate.isValid()){
        if(ActivePackage.EndTime.isValid())
            NextDigestTime = effectiveEndTime(ActivePackage);
        else
            NextDigestTime = QDateTime(ActivePackage.EndDate.addDays(1));
    }else if (ActivePackage.EndTime.isValid())
        NextDigestTime = effectiveEndTime(ActivePackage);

    return stuActiveServiceAccount(ActivePackage,
                                   AccountInfo.ParentID ? true : false,
                                   AccountInfo.MyLimitsOnParent,
                                   NextDigestTime.isValid() ? (Now.msecsTo(NextDigestTime) / 1000) : -1);
}

void intfAccounting::hasCredit(const clsJWT& _jwt, const ServiceUsage_t& _requestedUsage)
{
    QJsonObject Privs = _jwt.privsObject();
    if(Privs.contains(this->ServiceName) == false)
        throw exHTTPForbidden("[81] You don't have access to: " + this->ServiceName);

    stuActiveServiceAccount ActiveServiceAccount = this->findActiveAccount(clsJWT(_jwt).usrID(), _requestedUsage);
    if(ActiveServiceAccount.TTL == 0)
        throw exHTTPForbidden("[82] You don't have access to: " + this->ServiceName);

    auto checkCredit = [](auto _usageIter, auto _remaining, auto _type) {
        if(_remaining.PerDay >= 0 && _remaining.PerDay - _usageIter.value() <= 0)
            throw exPaymentRequired(QString("You have not enough credit: <%1:Day:%2>").arg(_type).arg(_usageIter.key()));
        if(_remaining.PerWeek >= 0 && _remaining.PerWeek - _usageIter.value() <= 0)
            throw exPaymentRequired(QString("You have not enough credit: <%1:Week:%2>").arg(_type).arg(_usageIter.key()));
        if(_remaining.PerMonth >= 0 && _remaining.PerMonth - _usageIter.value() <= 0)
            throw exPaymentRequired(QString("You have not enough credit: <%1:Month:%2>").arg(_type).arg(_usageIter.key()));
        if(_remaining.Total >= 0 && _remaining.Total - _usageIter.value() <= 0)
            throw exPaymentRequired(QString("You have not enough credit: <%1:Total:%2>").arg(_type).arg(_usageIter.key()));
    };

    const stuPackage& Package = ActiveServiceAccount.ActivePackage;
    stuUsage CurrUsage;
    if(ActiveServiceAccount.IsFromParent){
        for(auto UsageIter = _requestedUsage.begin();
            UsageIter != _requestedUsage.end();
            UsageIter++){
            if(Package.Remaining.contains(UsageIter.key()) == false)
                continue;
            if(this->isUnlimited(ActiveServiceAccount.MyLimitsOnParent) == false)
                checkCredit(UsageIter, ActiveServiceAccount.MyLimitsOnParent.value(UsageIter.key()), "Own");

            checkCredit(UsageIter, Package.Remaining.value(UsageIter.key()), "Parent");
        }
        return;
    }else if(this->isUnlimited(Package.Remaining))
        return;
    else{
        for(auto UsageIter = _requestedUsage.begin();
            UsageIter != _requestedUsage.end();
            UsageIter++){
            if(Package.Remaining.contains(UsageIter.key()) == false)
                continue;
            checkCredit(UsageIter, Package.Remaining.value(UsageIter.key()), "Self");
        }
    }
}

//TODO When a package is selected as JustThis then there is no need to store list in JWT
//TODO reset if per day/perweek/per month are set

}
}
}
}

