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
                     COD                        = 'O',
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
TARGOMAN_DEFINE_ENUM(enuPaymentGatewayTypeStatus,
                     Active   = 'A',
                     Disabled = 'D',
                     Removed  = 'R',
                     );

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

TAPI_DECLARE_METATYPE_ENUM(Targoman::API::AccountModule, enuPaymentGatewayTypeStatus);
TAPI_DECLARE_METATYPE_ENUM(Targoman::API::AccountModule, enuPaymentGatewayStatus);
TAPI_DECLARE_METATYPE_ENUM(Targoman::API::AccountModule, enuPaymentGatewayTransactionFeeType);

namespace Targoman::API::AccountModule {

namespace ORM {

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-variable"

namespace tblPaymentGatewayTypes {
    constexpr char Name[] = "tblPaymentGatewayTypes";

    namespace Fields {
        TARGOMAN_CREATE_CONSTEXPR(pgtID);
        TARGOMAN_CREATE_CONSTEXPR(pgtType);
        TARGOMAN_CREATE_CONSTEXPR(pgtName);
        TARGOMAN_CREATE_CONSTEXPR(pgtNameI18N);
        TARGOMAN_CREATE_CONSTEXPR(pgtMinRequestAmount);
        TARGOMAN_CREATE_CONSTEXPR(pgtMaxRequestAmount);
        TARGOMAN_CREATE_CONSTEXPR(pgtStatus);
    }
}

namespace tblPaymentGatewayTypesI18N {
    constexpr char Name[] = "tblPaymentGatewayTypesI18N";

    namespace Fields {
        TARGOMAN_CREATE_CONSTEXPR(pid);
        TARGOMAN_CREATE_CONSTEXPR(language);
        TARGOMAN_CREATE_CONSTEXPR(pgtNameI18N);
    }
}

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
//        TARGOMAN_CREATE_CONSTEXPR(pgwMinRequestAmount);
//        TARGOMAN_CREATE_CONSTEXPR(pgwMaxRequestAmount);
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
}

//-- tbl inside
namespace tblPaymentGatewayTypes {

    namespace Relation {
        // constexpr char AAA[] = "aaa";
    }

    namespace Private {
        const QList<clsORMField> ORMFields = {
            //ColName                           Type                        Validation                              Default     UpBy     Sort   Filter Self  Virt   PK
            { Fields::pgtID,                    ORM_PRIMARYKEY_16 },
            { Fields::pgtType,                  S(Targoman::API::AccountModule::enuPaymentGatewayType::Type), QFV,  QRequired,  UPAdmin },
            ORM_MULTILANGUAGE(Fields::pgtName, QRequired, UPAdmin),
            { Fields::pgtMinRequestAmount,      S(qreal),                   QFV.minValue(1),                        1,          UPAdmin },
            { Fields::pgtMaxRequestAmount,      S(NULLABLE_TYPE(double)),   QFV,                                    QNull,      UPAdmin },
            { Fields::pgtStatus,                ORM_STATUS_FIELD(Targoman::API::AccountModule::enuPaymentGatewayTypeStatus, Targoman::API::AccountModule::enuPaymentGatewayTypeStatus::Active) },
        };

        const QList<stuRelation> Relations = {
            //Col                           Reference Table              ForeignCol       Rename     LeftJoin
            { Fields::pgtType,              R(AAASchema, tblPaymentGateways::Name), tblPaymentGateways::Fields::pgwType },
            { Fields::pgtID,                R(AAASchema, tblPaymentGatewayTypesI18N::Name), tblPaymentGatewayTypesI18N::Fields::pid },
        };

        const QList<stuDBIndex> Indexes = {
        };

    } //namespace Private

    TAPI_DEFINE_STRUCT(DTO,
        SF_ORM_PRIMARYKEY_16        (pgtID),
        SF_Enum                     (pgtType, Targoman::API::AccountModule::enuPaymentGatewayType, Targoman::API::AccountModule::enuPaymentGatewayType::COD),
        SF_QString                  (pgtName),
        SF_qreal                    (pgtMinRequestAmount),
        SF_NULLABLE_qreal           (pgtMaxRequestAmount),
        SF_ORM_STATUS_FIELD         (pgtStatus, Targoman::API::AccountModule::enuPaymentGatewayTypeStatus, Targoman::API::AccountModule::enuPaymentGatewayTypeStatus::Active)
    );
}

namespace tblPaymentGatewayTypesI18N {
    namespace Relation {
    }

    namespace Private {
        const QList<clsORMField> ORMFields = {
            //ColName               Type                Validation  Default     UpBy    Sort  Filter Self   Virt   PK
            { Fields::pid,          S(quint16),         QFV,        QRequired,  UPNone, true, true,  false, false, true },
            { Fields::language,     S(QString),         QFV,        QRequired,  UPNone, true, true,  false, false, true },
            { Fields::pgtNameI18N,  S(QString),         QFV,        QRequired,  UPOwner },
        };

        const QList<stuRelation> Relations = {
        };

        const QList<stuDBIndex> Indexes = {
        };

    } //namespace Private

    TAPI_DEFINE_STRUCT(DTO,
        SF_quint16                  (pid),
        SF_QString                  (language),
        SF_QString                  (pgtNameI18N)
    );
}

namespace tblPaymentGateways {
    namespace Relation {
         constexpr char Type[] = "type";
    }

    namespace Private {
        const QList<clsORMField> ORMFields = {
            //ColName                           Type                                                Validation                          Default     UpBy     Sort   Filter Self  Virt   PK
            { Fields::pgwID,                    ORM_PRIMARYKEY_32 },
            { Fields::pgwName,                  S(QString),                                         QFV.unicodeAlNum().maxLenght(64),   QRequired,  UPAdmin },
            { Fields::pgwType,                  S(Targoman::API::AccountModule::enuPaymentGatewayType::Type), QFV,                      QRequired,  UPAdmin },
            { Fields::pgwDriver,                S(QString),                                         QFV,                                QRequired,  UPAdmin },
            { Fields::pgwMetaInfo,              S(NULLABLE_TYPE(TAPI::JSON_t)),                     QFV,                                QNull,      UPAdmin },
            { Fields::pgw_curID,                S(quint32),                                         QFV,                                QRequired,  UPAdmin },
            { Fields::pgwAllowedDomainName,     S(QString),                                         QFV.unicodeAlNum().maxLenght(64),   QRequired,  UPAdmin },
            //------------------
            { Fields::pgwTransactionFeeValue,   S(NULLABLE_TYPE(quint32)),                          QFV,                                QNull,      UPAdmin },
            { Fields::pgwTransactionFeeType,    S(Targoman::API::AccountModule::enuPaymentGatewayTransactionFeeType::Type), QFV, Targoman::API::AccountModule::enuPaymentGatewayTransactionFeeType::Currency, UPAdmin },
            //------------------
//            { Fields::pgwMinRequestAmount,      S(quint32),                                         QFV.minValue(1),                    1,          UPAdmin },
//            { Fields::pgwMaxRequestAmount,      S(NULLABLE_TYPE(quint32)),                          QFV,                                QNull,      UPAdmin },
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
            //Col                        Reference Table              ForeignCol       Rename     LeftJoin
            { Relation::Type,
              { Fields::pgwType, R(AAASchema, tblPaymentGatewayTypes::Name), tblPaymentGatewayTypes::Fields::pgtType } },
            ORM_RELATION_OF_CREATOR(Fields::pgwCreatedBy_usrID),
            ORM_RELATION_OF_UPDATER(Fields::pgwUpdatedBy_usrID),
        };

        const QList<stuDBIndex> Indexes = {
        };

    } //namespace Private

    TAPI_DEFINE_STRUCT(DTO,
        SF_ORM_PRIMARYKEY_32        (pgwID),
        SF_QString                  (pgwName),
        SF_Enum                     (pgwType, Targoman::API::AccountModule::enuPaymentGatewayType, Targoman::API::AccountModule::enuPaymentGatewayType::COD),
        SF_QString                  (pgwDriver),
        SF_JSON_t                   (pgwMetaInfo),
        SF_quint32                  (pgw_curID),
        SF_QString                  (pgwAllowedDomainName),
        SF_NULLABLE_quint32         (pgwTransactionFeeValue),
        SF_Enum                     (pgwTransactionFeeType, Targoman::API::AccountModule::enuPaymentGatewayTransactionFeeType, Targoman::API::AccountModule::enuPaymentGatewayTransactionFeeType::Currency),
//        SF_quint32                  (pgwMinRequestAmount),
//        SF_NULLABLE_quint32         (pgwMaxRequestAmount),
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

/******************************************************/
class PaymentGatewayTypesI18N : public intfSQLBasedModule
{
//    Q _OBJECT
    TARGOMAN_API_SUBMODULE_DEFINE(Account, PaymentGatewayTypesI18N)
};

/******************************************************/
class PaymentGatewayTypes : public intfSQLBasedModule
{
    Q_OBJECT
    TARGOMAN_API_SUBMODULE_DEFINE(Account, PaymentGatewayTypes)

public:
    virtual ORMSelectQuery makeSelectQuery(INTFAPICALLBOOM_DECL &APICALLBOOM_PARAM, const QString &_alias = {}, bool _translate = true, bool _isRoot = true);

private slots:
    QVariant ORMGET_USER("Get payment gateway type information")
    quint32 ORMCREATE_USER("Create a new payment gateway type by an authorized user")
    bool ORMUPDATE_USER("Update payment gateway type info by an authorized user")
    bool ORMDELETE_USER("Delete a payment gateway type")
};

/******************************************************/
class PaymentGateways : public intfSQLBasedModule
{
    Q_OBJECT
    TARGOMAN_API_SUBMODULE_DEFINE(Account, PaymentGateways)

private slots:
    QVariant ORMGET_USER("Get payment gateway information")
    quint32 ORMCREATE_USER("Create a new payment gateway by an authorized user")
    bool ORMUPDATE_USER("Update payment gateway info by an authorized user")
    bool ORMDELETE_USER("Delete a payment gateway")

    QVariantList REST_GET_OR_POST(
        availableGatewayTypes,
        (
            APICALLBOOM_TYPE_JWT_USER_DECL &APICALLBOOM_PARAM,
            quint32 _amount,
            QString _domain
        ),
        "get list of payment gateway types valid for amount and domain"
    )
};

/******************************************************/
} //namespace ORM
} //namespace Targoman::API::AccountModule

#endif // TARGOMAN_API_MODULES_ACCOUNT_ORM_PAYMENTGATEWAYS_H
