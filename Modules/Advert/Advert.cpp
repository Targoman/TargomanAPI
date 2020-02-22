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
 @author S. Mehran M. Ziabary <ziabary@targoman.com>
 */

#include "Advert.h"
#include "QFieldValidator.h"
#include "Interfaces/AAA/PrivHelpers.h"
#include "Interfaces/Common/GenericEnums.hpp"
#include "Interfaces/Common/HTTPExceptions.hpp"

#include "ORM/Defs.hpp"
#include "ORM/ActiveAds.h"
#include "ORM/Bin.h"
#include "ORM/Clicks.h"
#include "ORM/Props.h"
#include "ORM/Locations.h"

TAPI_REGISTER_TARGOMAN_ENUM(TAPI,enuAdvertType);
TAPI_REGISTER_TARGOMAN_ENUM(TAPI,enuAdvertOrder);
TAPI_REGISTER_TARGOMAN_ENUM(TAPI,enuBannerSizes);
TAPI_REGISTER_TARGOMAN_ENUM(TAPI,enuAccountOrdersStatus);

TAPI_REGISTER_METATYPE(
        COMPLEXITY_Complex,
        TAPI, stuAdvert,
        [](const TAPI::stuAdvert& _value) -> QVariant{return _value.toJson().toVariantMap();}
);

namespace Targoman {
namespace API {

using namespace Targoman;
using namespace TAPI;
using namespace AAA;
using namespace Advertisement;

TARGOMAN_API_MODULE_DB_CONFIG_IMPL(Advert);

stuAdvert Advert::apiGETNewBanner(TAPI::RemoteIP_t _REMOTE_IP, QString _location, TAPI::enuAdvertOrder::Type _order)
{
    /*clsDACResult Result = AdvertDACInstance().execQuery(
                                "",
                                "SELECT "
                                )*/
}

stuAdvert Advert::apiGETNewText(TAPI::RemoteIP_t _REMOTE_IP, QString _location, TAPI::enuAdvertOrder::Type _order, const QString _keywords)
{

}

QString Advert::apiGETRetrieveURL(TAPI::RemoteIP_t _REMOTE_IP, quint64 _id, TAPI::IPv4_t _clientIP, QString _agent)
{

}

Advert::Advert() :
    ORM::clsRESTAPIWithActionLogs(AdvertSchema, AdvertDomain),
    Accounting::intfAccounting(this->moduleBaseName()),
    AccountPackages(new Advertisement::clsAccountPackages),
    AccountUserPackages(new Advertisement::clsAccountUserPackages),
    AccountUsage(new Advertisement::clsAccountUsage),
    //    AccountPrizes(new Advertisement::clsAccountPrizes),
    AccountDiscountss(new Advertisement::clsAccountDiscounts)
{
    this->addSubModule(this->AccountPackages);
    this->addSubModule(this->AccountUserPackages);
    this->addSubModule(this->AccountUsage);
    //this->addSubModule(this->AccountPrizes); // There is no prize in advertisement module
    this->addSubModule(this->AccountDiscountss);
    this->addSubModule(new Advertisement::clsAccountUsage);
    this->addSubModule(new Advertisement::clsAccountUserPackages);
    this->addSubModule(new Advertisement::clsAccountDiscounts);

    this->addSubModule(new Advertisement::ActiveAds);
    this->addSubModule(new Advertisement::Bin);
    this->addSubModule(new Advertisement::Locations);
    this->addSubModule(new Advertisement::Banners);
    this->addSubModule(new Advertisement::Clicks);
    this->addSubModule(new Advertisement::Props);
}

/*******************************************************************************************************************/
Accounting::stuServiceAccountInfo Advert::retrieveServiceAccountInfo(quint32 _usrID)
{

}

void Advert::breakPackage(quint64 _pkgID)
{

}

bool Advert::isUnlimited(const Accounting::PackageRemaining_t& _limits) const
{

}

bool Advert::isEmpty(const Accounting::PackageRemaining_t& _limits) const
{

}
/*******************************************************************************************************************/
TAPI::stuPreInvoice Advert::apiPOSTpreInvoice(TAPI::PackageCode_t _pkg, TAPI::DiscountCode_t _discountCode, QString _referer, QJsonObject _extraRefererParams){
    QString ExtraFilters = QString ("( %1>=NOW() + %2<DATE_ADD(NOW(),INTERVAL$SPACE$15$SPACE$Min)")
            .arg(tblAccountPackages::pkgCanBePurchasedSince)
            .arg(tblAccountPackages::pkgNotAvailableSince);

    QVariantMap PackageInfo = this->AccountPackages->selectFromTable({}, ExtraFilters, _pkg, {}, 0, 1,
                                                                     QStringList({
                                                                                     tblAccountPackages::pkgID,
                                                                                     tblAccountPackages::pkgCode,
                                                                                     tblAccountPackages::pkgRemainingDays,
                                                                                     tblAccountPackages::pkgShowPerDay,
                                                                                     tblAccountPackages::pkgShowTotal,
                                                                                     tblAccountPackages::pkgClicksPerDay,
                                                                                     tblAccountPackages::pkgClicksPerMonth,
                                                                                     tblAccountPackages::pkgClicksTotal,
                                                                                     tblAccountPackages::pkgValidFromDate,
                                                                                     tblAccountPackages::pkgValidToDate,
                                                                                     tblAccountPackages::pkgValidFromTime,
                                                                                     tblAccountPackages::pkgValidToTime,
                                                                                     tblAccountPackages::pkgPrivs,
                                                                                 }).join(',')
                                                                     ).toMap();

    stuPackage Package(
                PackageInfo.value(tblAccountPackages::pkgID).toUInt(),
                PackageInfo.value(tblAccountPackages::pkgCode).toString(),
                PackageInfo.value(tblAccountPackages::pkgRemainingDays).toInt(),
                -1,
    {
                    {"show", {
                         PackageInfo.value(tblAccountPackages::pkgShowPerDay).toInt(),
                         -1,
                         -1,
                         PackageInfo.value(tblAccountPackages::pkgShowTotal).toInt(),
                     }},
                    {"clicks", {
                         PackageInfo.value(tblAccountPackages::pkgClicksPerDay).toInt(),
                         -1,
                         PackageInfo.value(tblAccountPackages::pkgClicksPerMonth).toInt(),
                         PackageInfo.value(tblAccountPackages::pkgClicksTotal).toInt(),
                     }},
                },
                PackageInfo.value(tblAccountPackages::pkgValidFromDate).toDate(),
                PackageInfo.value(tblAccountPackages::pkgValidToDate).toDate(),
                PackageInfo.value(tblAccountPackages::pkgValidFromTime).toTime(),
                PackageInfo.value(tblAccountPackages::pkgValidToTime).toTime(),
                QJsonObject::fromVariantMap(PackageInfo.value(tblAccountPackages::pkgPrivs).toMap())
                );

    stuDiscount Discount;
    if(_discountCode.size()){
        ExtraFilters = QString("%1 >= NOW()").arg(tblAccountDiscounts::disValidFrom);
        QVariantMap DiscountInfo = this->AccountDiscountss->selectFromTable({}, ExtraFilters, _discountCode, {}, 0, 1,
                                                                            QStringList({
                                                                                            tblAccountDiscounts::disID,
                                                                                            tblAccountDiscounts::disCode,
                                                                                            tblAccountDiscounts::disType,
                                                                                            tblAccountDiscounts::disStatus,
                                                                                            tblAccountDiscounts::disPackageBasedAmount,
                                                                                            tblAccountDiscounts::disPrimaryCount,
                                                                                            tblAccountDiscounts::disUsedCount,
                                                                                            tblAccountDiscounts::disValidFrom,
                                                                                        }).join(',')
                                                                            ).toMap();
        if(DiscountInfo.value(tblAccountDiscounts::disExpiryTime).toDateTime() < )
        if(DiscountInfo.value(tblAccountDiscounts::disPrimaryCount).toInt() <= DiscountInfo.value(tblAccountDiscounts::disUsedCount).toInt())
            throw exHTTPBadRequest("Discount code has been finished");
    }
}

TAPI::stuInvoice Advert::apiPOSTcreateInvoice(TAPI::PackageCode_t _pkg, TAPI::URL_t _callBack, TAPI::DiscountCode_t _discountCode, QString _referer, QJsonObject _extraRefererParams){
}

TAPI::stuInvoice Advert::apiPOSTapproveInvoice(TAPI::MD5_t _invCode, TAPI::JSON_t _bankInfo){
}


}
}
