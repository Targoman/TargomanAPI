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
 * @author S. Mehran M. Ziabary <ziabary@targoman.com>
 * @author Kambiz Zandi <kambizzandi@gmail.com>
 */

#ifndef TARGOMAN_API_MODULES_ADVERT_ADVERT_H
#define TARGOMAN_API_MODULES_ADVERT_ADVERT_H

#include "libTargomanCommon/Configuration/tmplConfigurable.h"

#include "Interfaces/ORM/clsRESTAPIWithActionLogs.h"
#include "Interfaces/AAA/AAA.hpp"
#include "ORM/Defs.hpp"

#include "Interfaces/AAA/Accounting_Defs.hpp"
using namespace Targoman::API::AAA::Accounting;

namespace TAPI {

TAPI_DEFINE_VARIANT_ENABLED_STRUCT(stuAdvert,
    SF_quint64(ID),
    SF_QString(Title),
    SF_QString(Description),
    SF_QString(PrettyURL)

//    stuAdvert(quint64 _id = 0, QString _title = {}, QString _description = {}, QString _prettyURL = {}) :
//        ID(_id),
//        Title(_title),
//        Description(_description),
//        PrettyURL(_prettyURL)
//    {}

//    QVariant toVariant() const{
//        return QVariantMap({
//            {"id", this->ID},
//            {"title", this->Title},
//            {"description", this->Description},
//            {"prettyURL", this->PrettyURL}
//        });
//    }
);

struct stuAdvertBill {
};

}

namespace Targoman::API {

class Advert : public Accounting::intfRESTAPIWithAccounting
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID INTFAPIMODULE_IID)
    Q_INTERFACES(Targoman::API::intfAPIModule)
    TARGOMAN_API_MODULE_DB_CONFIGS(Advert);
    TARGOMAN_DEFINE_API_MODULE(Advert);

public:
    stuDBInfo requiredDB() const { return stuDBInfo(AdvertSchema); }

private:
    Accounting::stuServiceCreditsInfo retrieveServiceCreditsInfo(quint64 _usrID);
    void breakCredit(quint64 _slbID);
    bool isUnlimited(const Accounting::UsageLimits_t& _limits) const;
    bool isEmpty(const Accounting::UsageLimits_t& _limits) const;

protected:
    virtual void applyAssetAdditives(TAPI::JWT_t _JWT,
                                     INOUT stuAssetItem& _assetItem,
                                     const OrderAdditives_t& _orderAdditives);
private slots:
    TAPI::stuAdvert REST(
            GET,
            NewBanner,
            (
                TAPI::RemoteIP_t _REMOTE_IP,
                QString _location,
                TAPI::enuAdvertOrder::Type _order
            ),
            "Get new banner based on location and order info")

    TAPI::stuAdvert REST(
            GET,
            NewText,
            (
                TAPI::RemoteIP_t _REMOTE_IP,
                QString _location,
                TAPI::enuAdvertOrder::Type _order,
                QString _keywords
            ),
            "Get new text advertisement")

    QString REST(
            GET,
            RetrieveURL,
            (
                TAPI::RemoteIP_t _REMOTE_IP,
                quint64 _id,
                TAPI::IPv4_t _clientIP,
                QString _agent
            ),
            "Retrieve URL of the specified Advertisement")

};

} //namespace Targoman::API

TAPI_DECLARE_METATYPE(TAPI::stuAdvert);

#endif // TARGOMAN_API_MODULES_ACCOUNT_ADVERT_H
