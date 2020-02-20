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

#ifndef TARGOMAN_API_MODULES_ADVERT_ADVERT_H
#define TARGOMAN_API_MODULES_ADVERT_ADVERT_H

#include "libTargomanCommon/Configuration/tmplConfigurable.h"

#include "Interfaces/ORM/clsRESTAPIWithActionLogs.h"
#include "Interfaces/AAA/AAA.hpp"
#include "ORM/Defs.hpp"

namespace TAPI{
struct stuAdvert{
    quint64 ID;
    QString Title;
    QString Description;
    QString PrettyURL;
    stuAdvert(quint64 _id = 0, QString _title = {}, QString _description = {}, QString _prettyURL = {}):
        ID(_id),
        Title(_title),
        Description(_description),
        PrettyURL(_prettyURL)
    {}

    QVariant toVariant() const{
      return QVariantMap({
                           {"id", this->ID},
                             {"title", this->Title},
                             {"description", this->Description},
                             {"prettyURL", this->PrettyURL}
                         });
    }
};

struct stuAdvertBill{

};
}

namespace Targoman {
namespace API {

class Advert : public ORM::clsRESTAPIWithActionLogs, public Accounting::intfAccounting
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID INTFAPIMODULE_IID)
    Q_INTERFACES(Targoman::API::intfAPIModule)
    TARGOMAN_API_MODULE_DB_CONFIGS(Advert);
    TARGOMAN_DEFINE_API_MODULE(Advert);

public:
    stuDBInfo requiredDB() const {return stuDBInfo(AdvertSchema);}

private:
    Accounting::stuServiceAccountInfo retrieveServiceAccountInfo(quint32 _usrID);
    void breakPackage(quint64 _pkgID);
    bool isUnlimited(const Accounting::PackageRemaining_t& _limits) const;
    bool isEmpty(const Accounting::PackageRemaining_t& _limits) const;

private slots:
    TAPI::stuAdvert REST(GET,NewBanner,(TAPI::RemoteIP_t _REMOTE_IP, QString _location, TAPI::enuAdvertOrder::Type _order),
                         "Get new banner based on location and order info")
    TAPI::stuAdvert REST(GET,NewText,(TAPI::RemoteIP_t _REMOTE_IP, QString _location, TAPI::enuAdvertOrder::Type _order, QString _keywords),
                         "Get new text advertisement")
    QString         REST(GET,RetrieveURL, (TAPI::RemoteIP_t _REMOTE_IP, quint64 _id, TAPI::IPv4_t _clientIP, QString _agent),
                        "Retrieve URL of the specified Advertisement")

/***************************************************************************************************************************/
    TAPI::stuPreInvoice REST(POST, preInvoice, (TAPI::PackageCode_t _pkg, TAPI::DiscountCode_t _discountCode = {}, QString _referer = {}, QJsonObject _extraRefererParams = {}),
                          "create a pre-invoice based on input params")

    TAPI::stuInvoice REST(POST, createInvoice, (TAPI::PackageCode_t _pkg, TAPI::URL_t _callBack, TAPI::DiscountCode_t _discountCode = {}, QString _referer = {}, QJsonObject _extraRefererParams = {}),
                          "create a pre-invoice based on input params")

    TAPI::stuInvoice REST(POST, approveInvoice, (TAPI::MD5_t _invCode, TAPI::JSON_t _bankInfo),
                          "approve invoce back from bank")
};

}
}

TAPI_DECLARE_METATYPE(TAPI::stuAdvert);

#endif // TARGOMAN_API_MODULES_ACCOUNT_ADVERT_H
