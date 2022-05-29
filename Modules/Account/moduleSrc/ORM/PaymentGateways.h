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

} // namespace Targoman::API::AccountModule

//TAPI_DECLARE_METATYPE_ENUM(TAPI::enuPaymentGateway);

TAPI_DECLARE_METATYPE_ENUM(Targoman::API::AccountModule, enuPaymentGatewayType);
//TAPI_DECLARE_METATYPE(Targoman::API::AccountModule::enuPaymentGatewayType::List);

TAPI_DECLARE_METATYPE_ENUM(Targoman::API::AccountModule, enuPaymentGatewayStatus);
TAPI_DECLARE_METATYPE_ENUM(Targoman::API::AccountModule, enuPaymentGatewayTransactionFeeType);

namespace Targoman::API::AccountModule {

namespace ORM {

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-variable"

namespace tblPaymentGateways {
    constexpr char Name[] = "tblPaymentGateways";

    namespace Fields {
        TARGOMAN_CREATE_CONSTEXPR(pgwID);
        TARGOMAN_CREATE_CONSTEXPR(pgwName);
        TARGOMAN_CREATE_CONSTEXPR(pgwType);
        TARGOMAN_CREATE_CONSTEXPR(pgwDriver);
        TARGOMAN_CREATE_CONSTEXPR(pgwMetaInfo);     //meta info keys are defined and using by driver
        TARGOMAN_CREATE_CONSTEXPR(pgw_curID);
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

    inline QStringList ColumnNames(QString _tableAlias = "") {
        if (_tableAlias.isEmpty() == false)
            _tableAlias += ".";

        return {
            _tableAlias + Fields::pgwID,
            _tableAlias + Fields::pgwName,
            _tableAlias + Fields::pgwType,
            _tableAlias + Fields::pgwDriver,
            _tableAlias + Fields::pgwMetaInfo,
            _tableAlias + Fields::pgw_curID,
            _tableAlias + Fields::pgwAllowedDomainName,
            _tableAlias + Fields::pgwTransactionFeeValue,
            _tableAlias + Fields::pgwTransactionFeeType,
            _tableAlias + Fields::pgwMinRequestAmount,
            _tableAlias + Fields::pgwMaxRequestAmount,
            _tableAlias + Fields::pgwMaxPerDayAmount,
            _tableAlias + Fields::pgwLastPaymentDateTime,
            _tableAlias + Fields::pgwSumTodayPaidAmount,
            _tableAlias + Fields::pgwSumRequestCount,
            _tableAlias + Fields::pgwSumRequestAmount,
            _tableAlias + Fields::pgwSumFailedCount,
            _tableAlias + Fields::pgwSumOkCount,
            _tableAlias + Fields::pgwSumPaidAmount,
            _tableAlias + Fields::pgwStatus,
            _tableAlias + Fields::pgwCreatedBy_usrID,
            _tableAlias + Fields::pgwCreationDateTime,
            _tableAlias + Fields::pgwUpdatedBy_usrID,
        };
    }

    namespace Relation {
        // constexpr char AAA[] = "aaa";
    }

    namespace Private {
        const QList<clsORMField> ORMFields = {
            ///< ColName                                       Type                                                Validation                          Default     UpBy     Sort   Filter Self  Virt   PK
            { Fields::pgwID,                    ORM_PRIMARYKEY_32 },
            { Fields::pgwName,                  S(QString),                                         QFV.unicodeAlNum().maxLenght(64),   QRequired,  UPAdmin },
            { Fields::pgwType,                  S(Targoman::API::AccountModule::enuPaymentGatewayType::Type),               QFV,                                QRequired,  UPAdmin },
            { Fields::pgwDriver,                S(QString),                                         QFV,                                QRequired,  UPAdmin },
            { Fields::pgwMetaInfo,              S(NULLABLE_TYPE(TAPI::JSON_t)),                     QFV,                                QNull,      UPAdmin },
            { Fields::pgw_curID,                S(quint32),                                         QFV,                                QRequired,  UPAdmin },
            { Fields::pgwAllowedDomainName,     S(QString),                                         QFV.unicodeAlNum().maxLenght(64),   QRequired,  UPAdmin },
            //------------------
            { Fields::pgwTransactionFeeValue,   S(NULLABLE_TYPE(quint32)),                          QFV,                                QNull,      UPAdmin },
            { Fields::pgwTransactionFeeType,    S(Targoman::API::AccountModule::enuPaymentGatewayTransactionFeeType::Type), QFV, Targoman::API::AccountModule::enuPaymentGatewayTransactionFeeType::Currency, UPAdmin },
            //------------------
            { Fields::pgwMinRequestAmount,      S(quint32),                                         QFV.minValue(1),                    1,          UPAdmin },
            { Fields::pgwMaxRequestAmount,      S(NULLABLE_TYPE(quint32)),                          QFV,                                QNull,      UPAdmin },
            { Fields::pgwMaxPerDayAmount,       S(NULLABLE_TYPE(quint32)),                          QFV,                                QNull,      UPAdmin },
            //------------------
            { Fields::pgwLastPaymentDateTime,   S(NULLABLE_TYPE(TAPI::DateTime_t)),                 QFV,                                QNull,      UPAdmin },
            { Fields::pgwSumTodayPaidAmount,    S(quint64),                                         QFV,                                0,          UPAdmin },
            //------------------
            { Fields::pgwSumRequestCount,       S(quint32),                                         QFV,                                0,          UPAdmin },
            { Fields::pgwSumRequestAmount,      S(quint64),                                         QFV,                                0,          UPAdmin },
            { Fields::pgwSumFailedCount,        S(quint32),                                         QFV,                                0,          UPAdmin },
            { Fields::pgwSumOkCount,            S(quint32),                                         QFV,                                0,          UPAdmin },
            { Fields::pgwSumPaidAmount,         S(quint64),                                         QFV,                                0,          UPAdmin },
            //------------------
            { Fields::pgwStatus,                ORM_STATUS_FIELD(Targoman::API::AccountModule::enuPaymentGatewayStatus, Targoman::API::AccountModule::enuPaymentGatewayStatus::Active) },
            { Fields::pgwCreationDateTime,      ORM_CREATED_ON },
            { Fields::pgwCreatedBy_usrID,       ORM_CREATED_BY },
            { Fields::pgwUpdatedBy_usrID,       ORM_UPDATED_BY },
        };

        const QList<stuRelation> Relations = {
            ///< Col                        Reference Table              ForeignCol       Rename     LeftJoin
            ORM_RELATION_OF_CREATOR(Fields::pgwCreatedBy_usrID),
            ORM_RELATION_OF_UPDATER(Fields::pgwUpdatedBy_usrID),
        };

        const QList<stuDBIndex> Indexes = {
        };

    } //namespace Private

    TAPI_DEFINE_VARIANT_ENABLED_STRUCT(DTO,
        SF_ORM_PRIMARYKEY_32        (pgwID),
        SF_QString                  (pgwName),
        SF_Enum                     (pgwType, Targoman::API::AccountModule::enuPaymentGatewayType, Targoman::API::AccountModule::enuPaymentGatewayType::COD),
        SF_QString                  (pgwDriver),
        SF_JSON_t                   (pgwMetaInfo),
        SF_quint32                  (pgw_curID),
        SF_QString                  (pgwAllowedDomainName),
        SF_NULLABLE_quint32         (pgwTransactionFeeValue),
        SF_Enum                     (pgwTransactionFeeType, Targoman::API::AccountModule::enuPaymentGatewayTransactionFeeType, Targoman::API::AccountModule::enuPaymentGatewayTransactionFeeType::Currency),
        SF_quint32                  (pgwMinRequestAmount),
        SF_NULLABLE_quint32         (pgwMaxRequestAmount),
        SF_NULLABLE_quint32         (pgwMaxPerDayAmount),
        SF_DateTime_t               (pgwLastPaymentDateTime),
        SF_quint64                  (pgwSumTodayPaidAmount),
        SF_quint32                  (pgwSumRequestCount),
        SF_quint64                  (pgwSumRequestAmount),
        SF_quint32                  (pgwSumFailedCount),
        SF_quint32                  (pgwSumOkCount),
        SF_quint64                  (pgwSumPaidAmount),
        SF_ORM_STATUS_FIELD         (pgwStatus, Targoman::API::AccountModule::enuPaymentGatewayStatus, Targoman::API::AccountModule::enuPaymentGatewayStatus::Active),
        SF_QString                  (pgwCreationDateTime),
        SF_quint64                  (pgwCreatedBy_usrID),
        SF_quint64                  (pgwUpdatedBy_usrID)
    );
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

#endif // TARGOMAN_API_MODULES_ACCOUNT_ORM_PAYMENTGATEWAYS_H
