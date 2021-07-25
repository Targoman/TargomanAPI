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

#ifndef TARGOMAN_API_MODULES_ACCOUNT_ORM_PAYMENTORDERS_H
#define TARGOMAN_API_MODULES_ACCOUNT_ORM_PAYMENTORDERS_H

#include "Interfaces/ORM/clsTable.h"
#include "Interfaces/AAA/AAA.hpp"
#include "PaymentGateways.h"

//-----------------------------------------------------
namespace TAPI {
TARGOMAN_DEFINE_ENUM(enuPaymentStatus,
                     New      = 'N',
                     Pending  = 'P',
                     Payed    = 'Y',
                     Succeded = 'A',
                     Error    = 'E',
                     Removed  = 'R'
                     )
}
TAPI_DECLARE_METATYPE_ENUM(TAPI::enuPaymentStatus);

//-----------------------------------------------------
namespace Targoman::API::AAA {

/*****************************************************************\
|* OnlinePayments ************************************************|
\*****************************************************************/
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-variable"
namespace tblOnlinePayments {
constexpr char Name[] = "tblOnlinePayments";
TARGOMAN_CREATE_CONSTEXPR(onpID);
TARGOMAN_CREATE_CONSTEXPR(onpMD5); //used for making payment callback url, e.g.: https://{tg.com}/callback/payment/verify?paymentMD5={onpMD5}
TARGOMAN_CREATE_CONSTEXPR(onp_vchID);
TARGOMAN_CREATE_CONSTEXPR(onp_pgwID);
TARGOMAN_CREATE_CONSTEXPR(onpPGTrnID);
TARGOMAN_CREATE_CONSTEXPR(onpAmount);
TARGOMAN_CREATE_CONSTEXPR(onpResult);
TARGOMAN_CREATE_CONSTEXPR(onpStatus);
TARGOMAN_CREATE_CONSTEXPR(onpCreationDateTime);
TARGOMAN_CREATE_CONSTEXPR(onpLastUpdateDateTime);
}
#pragma GCC diagnostic pop

struct stuOnlinePayment
{
    quint64 onpID;
    TAPI::MD5_t onpMD5;
    quint64 onp_vchID;
    quint32 onp_pgwID;
    QString onpPGTrnID;
    quint64 onpAmount;
    QString onpResult;
    TAPI::enuPaymentStatus::Type onpStatus;

    stuPaymentGateway PaymentGateway;

    void readFromVariantMap(const QVariantMap& _info)
    {
        SET_FIELD_FROM_VARIANT_MAP(this->onpID,      _info, tblOnlinePayments, onpID);
        SET_FIELD_FROM_VARIANT_MAP(this->onpMD5,     _info, tblOnlinePayments, onpMD5);
        SET_FIELD_FROM_VARIANT_MAP(this->onp_vchID,  _info, tblOnlinePayments, onp_vchID);
        SET_FIELD_FROM_VARIANT_MAP(this->onp_pgwID,  _info, tblOnlinePayments, onp_pgwID);
        SET_FIELD_FROM_VARIANT_MAP(this->onpPGTrnID, _info, tblOnlinePayments, onpPGTrnID);
        SET_FIELD_FROM_VARIANT_MAP(this->onpAmount,  _info, tblOnlinePayments, onpAmount);
        SET_FIELD_FROM_VARIANT_MAP(this->onpResult,  _info, tblOnlinePayments, onpResult);
        SET_FIELD_FROM_VARIANT_MAP(this->onpStatus,  _info, tblOnlinePayments, onpStatus);

        this->PaymentGateway.readFromVariantMap(_info);
    }
};

class OnlinePayments : public ORM::clsTable
{
    Q_OBJECT
private slots:
    QVariant ORMGET("Get OnlinePayment information.")

private:
    TARGOMAN_DEFINE_API_SUBMODULE(Account,OnlinePayments)
};

/*****************************************************************\
|* OfflinePayments ***********************************************|
\*****************************************************************/
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-variable"
namespace tblOfflinePayments {
constexpr char Name[] = "tblOfflinePayments";
TARGOMAN_CREATE_CONSTEXPR(ofpID);
TARGOMAN_CREATE_CONSTEXPR(ofp_vchID);
TARGOMAN_CREATE_CONSTEXPR(ofpBank);
TARGOMAN_CREATE_CONSTEXPR(ofpReceiptCode);
TARGOMAN_CREATE_CONSTEXPR(ofpReceiptDate);
TARGOMAN_CREATE_CONSTEXPR(ofpAmount);
TARGOMAN_CREATE_CONSTEXPR(ofpNotes);
TARGOMAN_CREATE_CONSTEXPR(ofpStatus);
TARGOMAN_CREATE_CONSTEXPR(ofpCreationDateTime);
TARGOMAN_CREATE_CONSTEXPR(ofpCreatedBy_usrID);
TARGOMAN_CREATE_CONSTEXPR(ofpUpdatedBy_usrID);
}
#pragma GCC diagnostic pop

class OfflinePayments : public ORM::clsTable
{
    Q_OBJECT
private slots:
    QVariant ORMGET("Get OfflinePayment information.")
    bool ORMUPDATE("Update OfflinePayment info by priviledged user")

private:
    TARGOMAN_DEFINE_API_SUBMODULE(Account,OfflinePayments)
};

} //namespace Targoman::API::AAA

#endif // TARGOMAN_API_MODULES_ACCOUNT_ORM_PAYMENTORDERS_H
