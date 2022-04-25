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

#include "Interfaces/AAA/AAA.hpp"
#include "Interfaces/API/intfSQLBasedModule.h"

namespace Targoman::API::AccountModule {

//structures and enumes goes here

TARGOMAN_DEFINE_ENUM(enuPaymentGatewayType,
                     _DeveloperTest             = '-',
                     COD                        = 'D', //offline payment
                     IranBank                   = 'I',
                     IranIntermediateGateway    = 'M',
                     InternationalDebitCart     = 'D',
                     InternationalCreditCart    = 'C',
                     CryptoCurrency             = 'B',
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

//TAPI_DEFINE_VARIANT_ENABLED_STRUCT(stuPaymentGateway,
//    SF_quint32(pgwID),
//    SF_QString(pgwName),
//    SF_Enum(Targoman::API::AccountModule::enuPaymentGatewayType, pgwType, Targoman::API::AccountModule::enuPaymentGatewayType::COD),
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
    Targoman::API::AccountModule::enuPaymentGatewayType::Type pgwType;
    QString pgwDriver;
    NULLABLE_TYPE(TAPI::JSON_t) pgwMetaInfo;

    void fromVariantMap(const QVariantMap& _info);
};

namespace ORM {

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-variable"

namespace tblPaymentGateways {
constexpr char Name[] = "tblPaymentGateways";
TARGOMAN_CREATE_CONSTEXPR(pgwID);
TARGOMAN_CREATE_CONSTEXPR(pgwName);
TARGOMAN_CREATE_CONSTEXPR(pgwType);
TARGOMAN_CREATE_CONSTEXPR(pgwDriver);
TARGOMAN_CREATE_CONSTEXPR(pgwMetaInfo);     //meta info keys are defined and using by driver
TARGOMAN_CREATE_CONSTEXPR(pgwAllowedDomainName);
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

class PaymentGateways : public intfSQLBasedModule
{
    Q_OBJECT
    TARGOMAN_DEFINE_API_SUBMODULE(Account, PaymentGateways)

private slots:
    QVariant ORMGET("Get payment gateway information")
    quint32 ORMCREATE("Create a new payment gateway by priviledged user")
    bool ORMUPDATE("Update payment gateway info by priviledged user")
    bool ORMDELETE("Delete a payment gateway")

    QVariantList REST_GET_OR_POST(
        availableGatewayTypes,
        (
            APICALLBOOM_TYPE_JWT_DECL &APICALLBOOM_PARAM,
            quint32 _amount,
            QString _domain
        ),
        "get list of payment gateway types valid for amount and domain"
    )
};

} //namespace ORM
} //namespace Targoman::API::AccountModule

//TAPI_DECLARE_METATYPE_ENUM(TAPI::enuPaymentGateway);

TAPI_DECLARE_METATYPE_ENUM(Targoman::API::AccountModule, enuPaymentGatewayType);
//TAPI_DECLARE_METATYPE(Targoman::API::AccountModule::enuPaymentGatewayType::List);

TAPI_DECLARE_METATYPE_ENUM(Targoman::API::AccountModule, enuPaymentGatewayStatus);
TAPI_DECLARE_METATYPE_ENUM(Targoman::API::AccountModule, enuPaymentGatewayTransactionFeeType);

#endif // TARGOMAN_API_MODULES_ACCOUNT_ORM_PAYMENTGATEWAYS_H
