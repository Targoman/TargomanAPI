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
#include "Interfaces/AAA/Authorization.h"
#include "Accounting_Interfaces.h"

using namespace Targoman::API;
using namespace Targoman::API::ORM;

TAPI_REGISTER_METATYPE(
        COMPLEXITY_Complex,
        TAPI, stuVoucher,
        [](const TAPI::stuVoucher& _value) -> QVariant{return _value.toJson().toVariantMap();}
);
TAPI_REGISTER_METATYPE(
        COMPLEXITY_Complex,
        TAPI, stuPreVoucher,
        [](const TAPI::stuPreVoucher& _value) -> QVariant{return _value.toJson().toVariantMap();},
        [](const QVariant& _value, const QByteArray& _param) -> TAPI::stuPreVoucher {
            if(_value.isValid() == false)
                return TAPI::stuPreVoucher();

            if(_value.toString().isEmpty())
                return TAPI::stuPreVoucher();

            QJsonParseError Error;
            QJsonDocument Doc;
            Doc = Doc.fromJson(_value.toString().toUtf8(), &Error);

            if(Error.error != QJsonParseError::NoError)
                throw exHTTPBadRequest(_param + " is not a valid Prevoucher: <"+_value.toString()+">" + Error.errorString());
            if(Doc.isObject() == false)
                throw exHTTPBadRequest(_param + " is not a valid Prevoucher object: <"+_value.toString()+">");
            return  TAPI::stuPreVoucher().fromJson(Doc.object());
        }
);
TAPI_REGISTER_TARGOMAN_ENUM(TAPI,enuVoucherStatus);
TAPI_REGISTER_TARGOMAN_ENUM(TAPI,enuDiscountType);


namespace Targoman {
namespace API {
namespace AAA {
namespace Accounting{

constexpr char PKG_ID[] = "ID";
constexpr char PKG_CODE[] = "CD";
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


/******************************************************************/
QVariant intfAccountPackages::apiGET(GET_METHOD_ARGS_IMPL)
{
    constexpr quint16 CACHE_TIME  = 15 * 60;
    QString ExtraFilters;
    if(Authorization::hasPriv(_JWT, this->privOn(EHTTP_GET,this->moduleBaseName())) == false)
        ExtraFilters = QString ("( %1>=NOW() | %2<DATE_ADD(NOW(),INTERVAL$SPACE$15$SPACE$Min)")
                       .arg(tblAccountPackagesBase::pkgCanBePurchasedSince)
                       .arg(tblAccountPackagesBase::pkgNotAvailableSince);

    return this->selectFromTable({}, ExtraFilters, GET_METHOD_CALL_ARGS, CACHE_TIME);
}

bool intfAccountPackages::apiDELETE(DELETE_METHOD_ARGS_IMPL)
{
    Authorization::checkPriv(_JWT, this->privOn(EHTTP_DELETE,this->moduleBaseName()));
    return this->deleteByPKs(DELETE_METHOD_CALL_ARGS);
}

bool intfAccountPackages::apiUPDATE(UPDATE_METHOD_ARGS_IMPL)
{
    Authorization::checkPriv(_JWT, this->privOn(EHTTP_PATCH,this->moduleBaseName()));
    return this->update(UPDATE_METHOD_CALL_ARGS);
}

quint32 intfAccountPackages::apiCREATE(CREATE_METHOD_ARGS_IMPL)
{
    Authorization::checkPriv(_JWT, this->privOn(EHTTP_PUT,this->moduleBaseName()));
    return this->create(CREATE_METHOD_CALL_ARGS).toUInt();
}

intfAccountPackages::intfAccountPackages(const QString& _schema,
                                         const QString& _name,
                                         const QList<ORM::clsORMField>& _cols,
                                         const QList<ORM::stuRelation>& _foreignKeys) :
    clsTable(_schema, _name, _cols, _foreignKeys)
{;}

/******************************************************************/
QVariant intfAccountUsage::apiGET(GET_METHOD_ARGS_IMPL)
{
    if(Authorization::hasPriv(_JWT, this->privOn(EHTTP_GET,this->moduleBaseName())) == false)
        this->setSelfFilters({{tblAccountUserPackages::aup_usrID, clsJWT(_JWT).usrID()}}, _EXTRAPATH, _ORMFILTERS, _filters);

    return this->selectFromTable({}, {}, GET_METHOD_CALL_ARGS);
}

intfAccountUsage::intfAccountUsage(const QString& _schema,
                                   const QString& _name,
                                   const QList<ORM::clsORMField>& _cols,
                                   const QList<ORM::stuRelation>& _foreignKeys) :
    clsTable(_schema, _name, _cols, _foreignKeys)
{;}


/******************************************************************/
QVariant intfAccountUserPackages::apiGET(GET_METHOD_ARGS_IMPL)
{
    if(Authorization::hasPriv(_JWT, this->privOn(EHTTP_GET,this->moduleBaseName())) == false)
        this->setSelfFilters({{tblAccountUserPackages::aup_usrID, clsJWT(_JWT).usrID()}}, _EXTRAPATH, _ORMFILTERS, _filters);

    return this->selectFromTable({}, {}, GET_METHOD_CALL_ARGS);
}

bool intfAccountUserPackages::apiUPDATEsetAsPrefered(TAPI::JWT_t _JWT, TAPI::ExtraPath_t _EXTRAPATH){
    bool Ok;
    quint64 UserPackageID = _EXTRAPATH.toUInt(&Ok);
    if(!Ok || !UserPackageID )
        throw exHTTPBadRequest("Invalid UserPackageID provided");

    this->callSP("sp_UPDATE_setAsPrefered", {
                     {"iUserID", clsJWT(_JWT).usrID()},
                     {"iAUPID",  UserPackageID},
                 });
    return false;
}

bool intfAccountUserPackages::apiUPDATEdisablePackage(TAPI::JWT_t _JWT, TAPI::ExtraPath_t _EXTRAPATH){
    bool Ok;
    quint64 UserPackageID = _EXTRAPATH.toUInt(&Ok);
    if(!Ok || !UserPackageID )
        throw exHTTPBadRequest("Invalid UserPackageID provided");
    Authorization::checkPriv(_JWT, this->privOn(EHTTP_PATCH,this->moduleBaseName()));
    return this->update(clsJWT(_JWT).usrID(), {
                            {tblAccountUserPackages::aupID, UserPackageID}
                        }, {
                            {tblAccountUserPackages::aupStatus, TAPI::enuAuditableStatus::Banned},
                        });
}
intfAccountUserPackages::intfAccountUserPackages(const QString& _schema,
                                                 const QString& _name,
                                                 const QList<ORM::clsORMField>& _cols,
                                                 const QList<ORM::stuRelation>& _foreignKeys) :
    clsTable(_schema, _name, _cols, _foreignKeys)
{;}

/******************************************************************/
QVariant intfAccountDiscounts::apiGET(GET_METHOD_ARGS_IMPL)
{
    Authorization::checkPriv(_JWT, this->privOn(EHTTP_GET,this->moduleBaseName()));
    return this->selectFromTable({}, {}, GET_METHOD_CALL_ARGS);
}

bool intfAccountDiscounts::apiDELETE(DELETE_METHOD_ARGS_IMPL)
{
    Authorization::checkPriv(_JWT, this->privOn(EHTTP_DELETE,this->moduleBaseName()));
    return this->deleteByPKs(DELETE_METHOD_CALL_ARGS);
}

bool intfAccountDiscounts::apiUPDATE(UPDATE_METHOD_ARGS_IMPL)
{
    Authorization::checkPriv(_JWT, this->privOn(EHTTP_PATCH,this->moduleBaseName()));
    return this->update(UPDATE_METHOD_CALL_ARGS);
}

quint32 intfAccountDiscounts::apiCREATE(CREATE_METHOD_ARGS_IMPL)
{
    Authorization::checkPriv(_JWT, this->privOn(EHTTP_PUT,this->moduleBaseName()));
    return this->create(CREATE_METHOD_CALL_ARGS).toUInt();
}

intfAccountDiscounts::intfAccountDiscounts(const QString& _schema,
                                           const QString& _name,
                                           const QList<ORM::clsORMField>& _cols,
                                           const QList<ORM::stuRelation>& _foreignKeys) :
    clsTable(_schema, _name, _cols, _foreignKeys)
{;}
/******************************************************************/
QVariant intfAccountPrizes::apiGET(GET_METHOD_ARGS_IMPL)
{
    Authorization::checkPriv(_JWT, this->privOn(EHTTP_GET,this->moduleBaseName()));
    return this->selectFromTable({}, {}, GET_METHOD_CALL_ARGS);
}

bool intfAccountPrizes::apiDELETE(DELETE_METHOD_ARGS_IMPL)
{
    Authorization::checkPriv(_JWT, this->privOn(EHTTP_DELETE,this->moduleBaseName()));
    return this->deleteByPKs(DELETE_METHOD_CALL_ARGS);
}

bool intfAccountPrizes::apiUPDATE(UPDATE_METHOD_ARGS_IMPL)
{
    Authorization::checkPriv(_JWT, this->privOn(EHTTP_PATCH,this->moduleBaseName()));
    return this->update(UPDATE_METHOD_CALL_ARGS);
}

quint32 intfAccountPrizes::apiCREATE(CREATE_METHOD_ARGS_IMPL)
{
    Authorization::checkPriv(_JWT, this->privOn(EHTTP_PUT,this->moduleBaseName()));
    return this->create(CREATE_METHOD_CALL_ARGS).toUInt();
}

intfAccountPrizes::intfAccountPrizes(const QString& _schema,
                                     const QString& _name,
                                     const QList<ORM::clsORMField>& _cols,
                                     const QList<ORM::stuRelation>& _foreignKeys) :
    clsTable(_schema, _name, _cols, _foreignKeys)
{;}

/******************************************************************/
QJsonObject stuPackage::toJson(bool _full)
{
    QJsonObject Info;
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
    }
    return Info;
}

stuPackage&stuPackage::fromJson(const QJsonObject& _obj)
{
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

}
}
}
}

namespace TAPI {
/******************************************************************/
/*QVariant stuPackageReq_t::toVariant() const{
    QVariantMap Value;
    for(auto Iter = this->begin(); Iter != this->end(); ++Iter)
        Value.insert(Iter.key(), Iter.value());

    return Value;
}

stuPackageReq_t& stuPackageReq_t::fromVariant(const QVariant& _value, const QByteArray& _paramName){
    if(_value.canConvert<QMap<QString, QVariant>>() == false)
        throw exHTTPBadRequest("Invalid value specified for parameter:" + _paramName);
    this->clear();
    const QMap<QString, QVariant>& Map = _value.toMap();
    for(auto Iter = Map.begin (); Iter != Map.end (); ++Iter)
        this->insert(Iter.key(), static_cast<qint16>(Iter.value().toInt()));
    return  *this;
}*/
}
