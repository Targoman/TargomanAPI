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

#ifndef TARGOMAN_API_MODULES_ACCOUNT_ORM_PAYMENTGATEWAYS_H
#define TARGOMAN_API_MODULES_ACCOUNT_ORM_PAYMENTGATEWAYS_H

#include "Interfaces/ORM/clsTable.h"
#include "Interfaces/AAA/AAA.hpp"

///TODO: move this to TargomanCommon
/*
#define TARGOMAN_DEFINE_ENUM_LIST(_name) \
    namespace _name { \
        typedef QList<_name::Type> List; \
        static inline QString toCSV(const List& _values, const char* _itemSurrounder="") { \
            QStringList out; \
            foreach (Type _value, _values) { \
                out.append(QString("%1%2%1").arg(_itemSurrounder).arg(_name::toStr(_value))); \
            } \
            return out.join(","); \
        } \
    }
*/
//-----------------------------------------------------
namespace TAPI {
///deprecated
//TARGOMAN_DEFINE_ENUM(enuPaymentGateway,
//                     Zibal    = 'Z',
//                     ZarrinPal= 'L',
//                     NextPay  = 'N',
//                     Pardano  = 'O',
//                     Parsian  = 'P',
//                     Mellat   = 'M',
//                     Pasargad = 'G',
//                     Saman    = 'S',
//                     AsanPardakht = 'A',
//                     Gap      = 'W',
//                     VISA     = 'V',
//                     MasterCard= 'C',
//                     )

TARGOMAN_DEFINE_ENUM(enuPaymentGatewayType,
                     COD                        = 'D', //offline payment
                     IranBank                   = 'I',
                     IranIntermediateGateway    = 'M',
                     InternationalDebitCart     = 'D',
                     InternationalCreditCart    = 'C',
                     CryptoCurrency             = 'B',
                     DevelopersTest             = '-',
                     );
//TARGOMAN_DEFINE_ENUM_LIST(enuPaymentGatewayType);

//TARGOMAN_DEFINE_ENUM(enuPaymentGatewayDriver,
//                     IranMellatBank     = 'M',
//                     IranPasargadBank   = 'P',
//                     IranVaseteZibal    = 'Z',
//                     TurkeyZiraatBank   = 'T',
//                     CryptoEther        = 'E',
//                     );
TARGOMAN_DEFINE_ENUM(enuPaymentGatewayStatus,
                     Active   = 'A',
                     Disabled = 'D',
                     Removed  = 'R',
                     );
TARGOMAN_DEFINE_ENUM(enuPaymentGatewayTransactionFeeType,
                     Percent = '%',
                     Currency = '$',
                     );
}
//TAPI_DECLARE_METATYPE_ENUM(TAPI::enuPaymentGateway);

TAPI_DECLARE_METATYPE_ENUM(TAPI::enuPaymentGatewayType);
//TAPI_DECLARE_METATYPE(TAPI::enuPaymentGatewayType::List);

TAPI_DECLARE_METATYPE_ENUM(TAPI::enuPaymentGatewayStatus);
TAPI_DECLARE_METATYPE_ENUM(TAPI::enuPaymentGatewayTransactionFeeType);

//-----------------------------------------------------
namespace Targoman::API::AAA {

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-variable"

namespace tblPaymentGateways {
constexpr char Name[] = "tblPaymentGateways";
TARGOMAN_CREATE_CONSTEXPR(pgwID);
TARGOMAN_CREATE_CONSTEXPR(pgwName);
TARGOMAN_CREATE_CONSTEXPR(pgwType);
TARGOMAN_CREATE_CONSTEXPR(pgwDriver);
TARGOMAN_CREATE_CONSTEXPR(pgwMetaInfo);     //meta info keys are defined and using by driver
//------------------
TARGOMAN_CREATE_CONSTEXPR(pgwTransactionFeeValue);
TARGOMAN_CREATE_CONSTEXPR(pgwTransactionFeeType);
//------------------
TARGOMAN_CREATE_CONSTEXPR(pgwMinRequestAmount);
TARGOMAN_CREATE_CONSTEXPR(pgwMaxRequestAmount);
TARGOMAN_CREATE_CONSTEXPR(pgwMaxPerDayAmount);
//load balance:
TARGOMAN_CREATE_CONSTEXPR(pgwLastPaymentDateTime);
TARGOMAN_CREATE_CONSTEXPR(pgwSumTodayPaidAmount);
//using:
TARGOMAN_CREATE_CONSTEXPR(pgwSumRequestCount);
TARGOMAN_CREATE_CONSTEXPR(pgwSumRequestAmount);
TARGOMAN_CREATE_CONSTEXPR(pgwSumFailedCount);
TARGOMAN_CREATE_CONSTEXPR(pgwSumOkCount);
TARGOMAN_CREATE_CONSTEXPR(pgwSumPaidAmount);
//
TARGOMAN_CREATE_CONSTEXPR(pgwStatus);
TARGOMAN_CREATE_CONSTEXPR(pgwCreatedBy_usrID);
TARGOMAN_CREATE_CONSTEXPR(pgwCreationDateTime);
TARGOMAN_CREATE_CONSTEXPR(pgwUpdatedBy_usrID);
}

#pragma GCC diagnostic pop

//TAPI_DEFINE_VARIANT_ENABLED_STRUCT(stuPaymentGateway,
//    SF_quint32(pgwID),
//    SF_QString(pgwName),
//    SF_Enum(TAPI::enuPaymentGatewayType, pgwType, TAPI::enuPaymentGatewayType::COD),
//    SF_Enum(TAPI::enuPaymentGatewayDriver, pgwDriver, TAPI::enuPaymentGatewayDriver::IranMellatBank),
//    SF_JSON(pgwMetaInfo)
//    SF_NULLABLE_quint32(pgwTransactionFeeValue),
//    SF_Enum(TAPI::enuPaymentGatewayTransactionFeeType::Type(pgwTransactionFeeType),
//    SF_quint32(pgwMinRequestAmount),
//    SF_NULLABLE_quint32)(pgwMaxRequestAmount),
//    SF_NULLABLE_quint32)(pgwMaxPerDayAmount),
//    SF_Generic(DateTime_t)(pgwLastPaymentDateTime),
//    SF_quint64(pgwSumTodayPaidAmount),
//    SF_quint32(pgwSumRequestCount),
//    SF_quint64(pgwSumRequestAmount),
//    SF_quint32(pgwSumFailedCount),
//    SF_quint32(pgwSumOkCount),
//    SF_quint64(pgwSumPaidAmount),
//    SF_Enum(TAPI::enuPaymentGatewayStatus::Type, pgwStatus),
//);

struct stuPaymentGateway
{
    quint32 pgwID;
    QString pgwName;
    TAPI::enuPaymentGatewayType::Type pgwType;
    QString pgwDriver;
    NULLABLE_TYPE(TAPI::JSON_t) pgwMetaInfo;

    void readFromVariantMap(const QVariantMap& _info)
    {
        SET_FIELD_FROM_VARIANT_MAP(this->pgwID,                  _info, tblPaymentGateways, pgwID);
        SET_FIELD_FROM_VARIANT_MAP(this->pgwName,                _info, tblPaymentGateways, pgwName);
        SET_FIELD_FROM_VARIANT_MAP(this->pgwType,                _info, tblPaymentGateways, pgwType);
        SET_FIELD_FROM_VARIANT_MAP(this->pgwDriver,              _info, tblPaymentGateways, pgwDriver);
        SET_FIELD_FROM_VARIANT_MAP(this->pgwMetaInfo,            _info, tblPaymentGateways, pgwMetaInfo);
//        SET_FIELD_FROM_VARIANT_MAP(this->pgwTransactionFeeValue, _info, tblPaymentGateways, pgwTransactionFeeValue);
//        SET_FIELD_FROM_VARIANT_MAP(this->pgwTransactionFeeType,  _info, tblPaymentGateways, pgwTransactionFeeType);
//        SET_FIELD_FROM_VARIANT_MAP(this->pgwMinRequestAmount,    _info, tblPaymentGateways, pgwMinRequestAmount);
//        SET_FIELD_FROM_VARIANT_MAP(this->pgwMaxRequestAmount,    _info, tblPaymentGateways, pgwMaxRequestAmount);
//        SET_FIELD_FROM_VARIANT_MAP(this->pgwMaxPerDayAmount,     _info, tblPaymentGateways, pgwMaxPerDayAmount);
//        SET_FIELD_FROM_VARIANT_MAP(this->pgwLastPaymentDateTime, _info, tblPaymentGateways, pgwLastPaymentDateTime);
//        SET_FIELD_FROM_VARIANT_MAP(this->pgwSumTodayPaidAmount,  _info, tblPaymentGateways, pgwSumTodayPaidAmount);
//        SET_FIELD_FROM_VARIANT_MAP(this->pgwSumRequestCount,     _info, tblPaymentGateways, pgwSumRequestCount);
//        SET_FIELD_FROM_VARIANT_MAP(this->pgwSumRequestAmount,    _info, tblPaymentGateways, pgwSumRequestAmount);
//        SET_FIELD_FROM_VARIANT_MAP(this->pgwSumFailedCount,      _info, tblPaymentGateways, pgwSumFailedCount);
//        SET_FIELD_FROM_VARIANT_MAP(this->pgwSumOkCount,          _info, tblPaymentGateways, pgwSumOkCount);
//        SET_FIELD_FROM_VARIANT_MAP(this->pgwSumPaidAmount,       _info, tblPaymentGateways, pgwSumPaidAmount);
//        SET_FIELD_FROM_VARIANT_MAP(this->pgwStatus,              _info, tblPaymentGateways, pgwStatus);
    }
};

class PaymentGateways : public ORM::clsTable
{
    Q_OBJECT

private slots:
    QVariant ORMGET("Get payment gateway information")
    quint32 ORMCREATE("Create a new payment gateway by priviledged user")
    bool ORMUPDATE("Update payment gateway info by priviledged user")
    bool ORMDELETE("Delete a payment gateway")

private:
    TARGOMAN_DEFINE_API_SUBMODULE(Account, PaymentGateways)
};

} //namespace Targoman::API::AAA

#endif // TARGOMAN_API_MODULES_ACCOUNT_ORM_PAYMENTGATEWAYS_H
