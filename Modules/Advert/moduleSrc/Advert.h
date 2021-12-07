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

#include "Interfaces/ORM/intfActionLogs.h"
#include "Interfaces/ORM/intfUploads.h"
#include "libTargomanCommon/Configuration/tmplConfigurable.h"
#include "Interfaces/API/intfSQLBasedWithActionLogsModule.h"
#include "Interfaces/AAA/AAA.hpp"
#include "ORM/Defs.hpp"
#include "Interfaces/AAA/Accounting_Defs.hpp"
using namespace Targoman::API::AAA;
using namespace Targoman::API::ORM;

//namespace TAPI {
namespace Targoman::API::AdvertModule {

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

TARGOMAN_ACTIONLOG_PREPARENT;
TARGOMAN_UPLOAD_PREPARENT;

class Advert : public intfAccountingBasedModule
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID INTFPUREMODULE_IID)
    Q_INTERFACES(Targoman::API::API::intfPureModule)
    TARGOMAN_API_MODULE_DB_CONFIGS(Advert);
    TARGOMAN_DEFINE_API_MODULE(Advert);
    TARGOMAN_API_DEFINE_ACTIONLOG(Advert, AdvertSchema);
    TARGOMAN_API_DEFINE_UPLOAD(Advert, AdvertSchema);

protected:
    virtual stuServiceCreditsInfo retrieveServiceCreditsInfo(quint64 _usrID);
    virtual void breakCredit(quint64 _slbID);
    virtual bool isUnlimited(const UsageLimits_t& _limits) const;
    virtual bool isEmpty(const UsageLimits_t& _limits) const;
    virtual void applyAssetAdditives(TAPI::JWT_t _JWT,
                                     INOUT stuAssetItem& _assetItem,
                                     const OrderAdditives_t& _orderAdditives);
protected slots:
//    bool REST_POST(
//        processVoucher,
//        (
//            TAPI::JWT_t _JWT,
//            Targoman::API::AAA::stuVoucherItem _voucherItem
//        ),
//        "Process voucher item"
//    )

//    bool REST_POST(
//        cancelVoucher,
//        (
//            TAPI::JWT_t _JWT,
//            Targoman::API::AAA::stuVoucherItem _voucherItem
//        ),
//        "Cancel voucher item"
//    )

    Targoman::API::AdvertModule::stuAdvert REST_GET(
        newBanner,
        (
            TAPI::RemoteIP_t _REMOTE_IP,
            QString _location,
            Targoman::API::AdvertModule::enuAdvertOrder::Type _order
        ),
        "Get new banner based on location and order info"
    )

    Targoman::API::AdvertModule::stuAdvert REST_GET(
        newText,
        (
            TAPI::RemoteIP_t _REMOTE_IP,
            QString _location,
            Targoman::API::AdvertModule::enuAdvertOrder::Type _order,
            QString _keywords
        ),
        "Get new text advertisement"
    )

    QString REST_GET(
        retrieveURL,
        (
            TAPI::RemoteIP_t _REMOTE_IP,
            quint64 _id,
            TAPI::IPv4_t _clientIP,
            QString _agent
        ),
        "Retrieve URL of the specified Advertisement"
    )

#ifdef QT_DEBUG
protected slots:
    QVariant REST_POST(
        fixtureSetup,
        (
                TAPI::RemoteIP_t _REMOTE_IP,
                TAPI::JWT_t _JWT
        ),
        "Create sample data"
    )

    QVariant REST_POST(
        fixtureCleanup,
        (
                TAPI::RemoteIP_t _REMOTE_IP
        ),
        "Cleanup sample data"
    )
//    bool REST_POST(
//        fixtureSetupVoucher,
//        (
//                TAPI::RemoteIP_t _REMOTE_IP,
//                TAPI::JWT_t _JWT
//        ),
//        "Sets up basket and voucher"
//    )

//protected:
//    virtual QVariant fixtureSetup(TAPI::RemoteIP_t _REMOTE_IP);
//    virtual QVariant fixtureCleanup(TAPI::RemoteIP_t _REMOTE_IP);

#endif
};

TARGOMAN_ACTIONLOG_POSTPARENT(Advert, AdvertSchema);
TARGOMAN_UPLOAD_POSTPARENT(Advert, AdvertSchema);

} //namespace Targoman::API::AdvertModule

TAPI_DECLARE_METATYPE(Targoman::API::AdvertModule::stuAdvert);

#endif // TARGOMAN_API_MODULES_ACCOUNT_ADVERT_H
