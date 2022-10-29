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
 * @author Kambiz Zandi <kambizzandi@gmail.com>
 */

#ifndef TARGOMAN_API_AAA_ACCOUNTING_DEFS_H
#define TARGOMAN_API_AAA_ACCOUNTING_DEFS_H

//#include "Interfaces/Common/APIArgHelperMacrosPrivate.h"
#include "AAADefs.hpp"
#include "clsJWT.hpp"
#include "Interfaces/Common/GenericTypes.h"
#include "Interfaces/Common/GenericEnums.hpp"
#include "Interfaces/Common/QtTypes.hpp"
#include "libTargomanCommon/Configuration/tmplConfigurable.h"
#include "libTargomanCommon/Configuration/Validators.hpp"
#include <QString>
#include <QJsonArray>
#include "Interfaces/Server/clsSimpleCrypt.h"
#include "Interfaces/ObjectStorage/ORM/ObjectStorage.h"
using namespace Targoman::API::ObjectStorage::ORM;

namespace Targoman::API::AAA {

TARGOMAN_DEFINE_ENUM(enuVoucherType,
                     Withdrawal     = 'W',
                     Invoice        = 'E', //Expense -> Invoice
                     Income         = 'I',
                     Credit         = 'C',
                     Debit          = 'D',
                     Prize          = 'Z',
                     TransferFrom   = 'F',
                     TransferTo     = 'T',
                     Freeze         = 'R', //freeze wallet
                     UnFreeze       = 'U', //unfreeze wallet
                     )

TARGOMAN_DEFINE_ENUM(enuVoucherStatus,
                     New            = 'N',
                     Canceled       = 'C',
                     Error          = 'E',
                     Finished       = 'F',
//                     WaitForProcess = 'W',
                     Removed        = 'R',
                     );

TARGOMAN_DEFINE_ENUM(enuDiscountType,
                     Percent  = '%',
                     Currency = '$',
                     );

TARGOMAN_DEFINE_ENUM(enuVoucherItemProcessStatus,
                     Processed  = 'P',
                     Canceled   = 'C',
                     Error      = 'E',
                     Removed    = 'R',
                     );

} //namespace Targoman::API::AAA

TAPI_DECLARE_METATYPE_ENUM(Targoman::API::AAA, enuVoucherType);             // -> TAPI_REGISTER_TARGOMAN_ENUM() in Accounting_Interfaces.cpp
TAPI_DECLARE_METATYPE_ENUM(Targoman::API::AAA, enuVoucherStatus)            // -> TAPI_REGISTER_TARGOMAN_ENUM() in Accounting_Interfaces.cpp
TAPI_DECLARE_METATYPE_ENUM(Targoman::API::AAA, enuDiscountType)             // -> TAPI_REGISTER_TARGOMAN_ENUM() in Accounting_Interfaces.cpp
TAPI_DECLARE_METATYPE_ENUM(Targoman::API::AAA, enuVoucherItemProcessStatus) // -> TAPI_REGISTER_TARGOMAN_ENUM() in Accounting_Interfaces.cpp

namespace Targoman::API::AAA {

constexpr char VOUCHER_ITEM_NAME_INC_WALLET[]           = "INC_WALLET";
constexpr char VOUCHER_ITEM_NAME_COD_CREDIT[]           = "COD_CREDIT";
constexpr char VOUCHER_ITEM_NAME_COD_DEBIT[]            = "COD_DEBIT";
//constexpr char PENDING_VOUCHER_NAME_COUPON_DISCOUNT[]   = "COUPON_DISCOUNT";
constexpr char PENDING_VOUCHER_NAME_REFERRER_PRIZE[]    = "REFERRER_PRIZE";

/***************************************************************************************************/
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-variable"

namespace tblAccountUnitsBase {
    constexpr char Name[] = "tblAccountUnits";

    namespace Fields {
        TARGOMAN_CREATE_CONSTEXPR(untID);
        TARGOMAN_CREATE_CONSTEXPR(untName);
        TARGOMAN_CREATE_CONSTEXPR(untNameI18N);
    } //namespace Fields
}

namespace tblAccountUnitsI18NBase {
    constexpr char Name[] = "tblAccountUnitsI18N";

    namespace Fields {
        TARGOMAN_CREATE_CONSTEXPR(pid);
        TARGOMAN_CREATE_CONSTEXPR(language);
        TARGOMAN_CREATE_CONSTEXPR(untNameI18N);
    } //namespace Fields
}

namespace tblAccountProductsBase {
    constexpr char Name[] = "tblAccountProducts";

    namespace Fields {
        TARGOMAN_CREATE_CONSTEXPR(prdID);
        TARGOMAN_CREATE_CONSTEXPR(prdCode);
        TARGOMAN_CREATE_CONSTEXPR(prdName);
        TARGOMAN_CREATE_CONSTEXPR(prdNameI18N);
        TARGOMAN_CREATE_CONSTEXPR(prdDesc);
        TARGOMAN_CREATE_CONSTEXPR(prdDescI18N);
        TARGOMAN_CREATE_CONSTEXPR(prdValidFromDate);
        TARGOMAN_CREATE_CONSTEXPR(prdValidToDate);
        TARGOMAN_CREATE_CONSTEXPR(prdValidFromHour);
        TARGOMAN_CREATE_CONSTEXPR(prdValidToHour);
        TARGOMAN_CREATE_CONSTEXPR(prdDurationMinutes);
        TARGOMAN_CREATE_CONSTEXPR(prdPrivs);
        TARGOMAN_CREATE_CONSTEXPR(prdVAT);
        TARGOMAN_CREATE_CONSTEXPR(prd_untID);
        TARGOMAN_CREATE_CONSTEXPR(prdQtyIsDecimal);

        ///@TODO: create trigger for this 3 fields
        TARGOMAN_CREATE_CONSTEXPR(prdInStockQty);
        TARGOMAN_CREATE_CONSTEXPR(prdOrderedQty);
        TARGOMAN_CREATE_CONSTEXPR(prdReturnedQty);
        // prdRemainingCount = prdInStockQty - (prdOrderedQty - prdReturnedQty)

        TARGOMAN_CREATE_CONSTEXPR(prdStatus);
        TARGOMAN_CREATE_CONSTEXPR(prdCreatedBy_usrID);
        TARGOMAN_CREATE_CONSTEXPR(prdCreationDateTime);
        TARGOMAN_CREATE_CONSTEXPR(prdUpdatedBy_usrID);
    } //namespace Fields
}

namespace tblAccountProductsI18NBase {
    constexpr char Name[] = "tblAccountProductsI18N";

    namespace Fields {
        TARGOMAN_CREATE_CONSTEXPR(pid);
        TARGOMAN_CREATE_CONSTEXPR(language);
        TARGOMAN_CREATE_CONSTEXPR(prdNameI18N);
        TARGOMAN_CREATE_CONSTEXPR(prdDescI18N);
    } //namespace Fields
}

namespace tblAccountSaleablesBase {
    constexpr char Name[] = "tblAccountSaleables";

    namespace Fields {
        TARGOMAN_CREATE_CONSTEXPR(slbID);
        TARGOMAN_CREATE_CONSTEXPR(slb_prdID);
        TARGOMAN_CREATE_CONSTEXPR(slbCode);
        TARGOMAN_CREATE_CONSTEXPR(slbName);
        TARGOMAN_CREATE_CONSTEXPR(slbNameI18N);
        TARGOMAN_CREATE_CONSTEXPR(slbDesc);
        TARGOMAN_CREATE_CONSTEXPR(slbDescI18N);
        TARGOMAN_CREATE_CONSTEXPR(slbType);
        TARGOMAN_CREATE_CONSTEXPR(slbAvailableFromDate);
        TARGOMAN_CREATE_CONSTEXPR(slbAvailableToDate);
        TARGOMAN_CREATE_CONSTEXPR(slbPrivs);
        TARGOMAN_CREATE_CONSTEXPR(slbBasePrice);
        TARGOMAN_CREATE_CONSTEXPR(slbAdditives);
    //    TARGOMAN_CREATE_CONSTEXPR(slbProductCount); //what is this?
        TARGOMAN_CREATE_CONSTEXPR(slbMaxSaleCountPerUser);

        ///@TODO: create trigger for this 3 fields and make changes to prd Count fields
        TARGOMAN_CREATE_CONSTEXPR(slbInStockQty);
        TARGOMAN_CREATE_CONSTEXPR(slbOrderedQty);
        TARGOMAN_CREATE_CONSTEXPR(slbReturnedQty);
        // slbRemainingQty = slbInStockQty - (slbOrderedQty - slbReturnedQty)

        TARGOMAN_CREATE_CONSTEXPR(slbVoucherTemplate);
        TARGOMAN_CREATE_CONSTEXPR(slbValidFromDate);
        TARGOMAN_CREATE_CONSTEXPR(slbValidToDate);
        TARGOMAN_CREATE_CONSTEXPR(slbValidFromHour);
        TARGOMAN_CREATE_CONSTEXPR(slbValidToHour);
        TARGOMAN_CREATE_CONSTEXPR(slbDurationMinutes);
        TARGOMAN_CREATE_CONSTEXPR(slbStartAtFirstUse);

        TARGOMAN_CREATE_CONSTEXPR(slbStatus);
        TARGOMAN_CREATE_CONSTEXPR(slbCreatedBy_usrID);
        TARGOMAN_CREATE_CONSTEXPR(slbCreationDateTime);
        TARGOMAN_CREATE_CONSTEXPR(slbUpdatedBy_usrID);
    }
}

namespace tblAccountSaleablesI18NBase {
    constexpr char Name[] = "tblAccountSaleablesI18N";

    namespace Fields {
        TARGOMAN_CREATE_CONSTEXPR(pid);
        TARGOMAN_CREATE_CONSTEXPR(language);
        TARGOMAN_CREATE_CONSTEXPR(slbNameI18N);
        TARGOMAN_CREATE_CONSTEXPR(slbDescI18N);
    } //namespace Fields
}

namespace tblAccountSaleablesFilesBase {
    constexpr char Name[] = "tblAccountSaleablesFiles";

    namespace Fields {
        TARGOMAN_CREATE_CONSTEXPR(slfID);
        TARGOMAN_CREATE_CONSTEXPR(slf_slbID);
        TARGOMAN_CREATE_CONSTEXPR(slfName);
        TARGOMAN_CREATE_CONSTEXPR(slfDesc);
        TARGOMAN_CREATE_CONSTEXPR(slfMimeTypes);
        TARGOMAN_CREATE_CONSTEXPR(slfExtensions);
        TARGOMAN_CREATE_CONSTEXPR(slfMinSize);
        TARGOMAN_CREATE_CONSTEXPR(slfMaxSize);
        TARGOMAN_CREATE_CONSTEXPR(slfMinCount);
        TARGOMAN_CREATE_CONSTEXPR(slfMaxCount);
    } //namespace Fields
}

namespace tblAccountUserAssetsBase {
    constexpr char Name[] = "tblAccountUserAssets";

    namespace Fields {
        TARGOMAN_CREATE_CONSTEXPR(uasID);
        TARGOMAN_CREATE_CONSTEXPR(uas_actorID);
        TARGOMAN_CREATE_CONSTEXPR(uas_slbID);
        TARGOMAN_CREATE_CONSTEXPR(uasQty);
        TARGOMAN_CREATE_CONSTEXPR(uas_vchID);
        TARGOMAN_CREATE_CONSTEXPR(uasVoucherItemUUID);
        TARGOMAN_CREATE_CONSTEXPR(uasVoucherItemInfo);
        TARGOMAN_CREATE_CONSTEXPR(uas_cpnID);
        TARGOMAN_CREATE_CONSTEXPR(uasDiscountAmount);
        TARGOMAN_CREATE_CONSTEXPR(uasPrefered);
        TARGOMAN_CREATE_CONSTEXPR(uasValidFromDate);
        TARGOMAN_CREATE_CONSTEXPR(uasValidToDate);
        TARGOMAN_CREATE_CONSTEXPR(uasValidFromHour);
        TARGOMAN_CREATE_CONSTEXPR(uasValidToHour);
        TARGOMAN_CREATE_CONSTEXPR(uasDurationMinutes);
        TARGOMAN_CREATE_CONSTEXPR(uasBreakedAt);
        TARGOMAN_CREATE_CONSTEXPR(uasStatus);
        TARGOMAN_CREATE_CONSTEXPR(uasCreationDateTime);
        TARGOMAN_CREATE_CONSTEXPR(uasUpdatedBy_usrID);
    }
}

namespace tblAccountUserAssetsFilesBase {
    constexpr char Name[] = "tblAccountUserAssets_files";

    namespace Fields {
        TARGOMAN_CREATE_CONSTEXPR(uasuflID);
        TARGOMAN_CREATE_CONSTEXPR(uasufl_uasID);
        TARGOMAN_CREATE_CONSTEXPR(uasufl_slfID);
        TARGOMAN_CREATE_CONSTEXPR(uasufl_uflID);
        TARGOMAN_CREATE_CONSTEXPR(uasuflCreationDateTime);
        TARGOMAN_CREATE_CONSTEXPR(uasuflCreatedBy_usrID);
    }
}

namespace tblAccountAssetUsageBase {
    constexpr char Name[] = "tblAccountAssetUsage";

    namespace Fields {
        TARGOMAN_CREATE_CONSTEXPR(usg_uasID);
    }
}

namespace tblAccountAssetUsageHistoryBase {
    constexpr char Name[] = "tblAccountAssetUsageHistory";

    namespace Fields {
        TARGOMAN_CREATE_CONSTEXPR(ush_uasID);
        TARGOMAN_CREATE_CONSTEXPR(ushLastDateTime);
    }
}

///@TODO: max usage count (user, system)
///@TODO: CodeCommentMark
namespace tblAccountCouponsBase {
    constexpr char Name[] = "tblAccountCoupons";

    namespace Fields {
        TARGOMAN_CREATE_CONSTEXPR(cpnID);
        TARGOMAN_CREATE_CONSTEXPR(cpnCode);

        //rules
        TARGOMAN_CREATE_CONSTEXPR(cpnPrimaryCount);
        TARGOMAN_CREATE_CONSTEXPR(cpnTotalMaxAmount);   //nullable
        TARGOMAN_CREATE_CONSTEXPR(cpnPerUserMaxCount);  //nullable
        TARGOMAN_CREATE_CONSTEXPR(cpnPerUserMaxAmount); //nullable
        TARGOMAN_CREATE_CONSTEXPR(cpnValidFrom);
        TARGOMAN_CREATE_CONSTEXPR(cpnExpiryTime);

        //actions
        TARGOMAN_CREATE_CONSTEXPR(cpnAmount);
        TARGOMAN_CREATE_CONSTEXPR(cpnAmountType);
        TARGOMAN_CREATE_CONSTEXPR(cpnMaxAmount);
        TARGOMAN_CREATE_CONSTEXPR(cpnSaleableBasedMultiplier);

        //stats
        TARGOMAN_CREATE_CONSTEXPR(cpnTotalUsedCount);
        TARGOMAN_CREATE_CONSTEXPR(cpnTotalUsedAmount);

        TARGOMAN_CREATE_CONSTEXPR(cpnStatus);
        TARGOMAN_CREATE_CONSTEXPR(cpnCreationDateTime);
        TARGOMAN_CREATE_CONSTEXPR(cpnCreatedBy_usrID);
        TARGOMAN_CREATE_CONSTEXPR(cpnUpdatedBy_usrID);
    }
}

namespace tblAccountPrizesBase {
    constexpr char Name[] = "tblAccountPrizes";

    namespace Fields {
        TARGOMAN_CREATE_CONSTEXPR(przID);
        TARGOMAN_CREATE_CONSTEXPR(przStatus);
        TARGOMAN_CREATE_CONSTEXPR(przCreationDateTime);
        TARGOMAN_CREATE_CONSTEXPR(przCreatedBy_usrID);
        TARGOMAN_CREATE_CONSTEXPR(przUpdatedBy_usrID);
    }
}

//-- tbl inside
namespace tblAccountUnitsBase {
    namespace Relation {
    }

    namespace Private {
        const QList<clsORMField> ORMFields = {
            //ColName               Type                Validation  Default     UpBy   Sort  Filter Self  Virt   PK
            { Fields::untID,        ORM_PRIMARYKEY_16 },
            ORM_MULTILANGUAGE(Fields::untName, QRequired,  UPOwner),
        };

        inline const QList<stuRelation> Relations(Q_DECL_UNUSED const QString& _schema) {
            return {
                { Fields::untID, R(_schema, tblAccountUnitsI18NBase::Name), tblAccountUnitsI18NBase::Fields::pid },
            };
        };

        const QList<stuDBIndex> Indexes = {
        };

    } //namespace Private

#define SF_tblAccountUnitsBase_DTO \
    SF_ORM_PRIMARYKEY_16        (untID), \
    SF_QString                  (untName)

    TAPI_DEFINE_STRUCT(DTO,
        SF_tblAccountUnitsBase_DTO
    );
}

namespace tblAccountUnitsI18NBase {
    namespace Relation {
    }

    namespace Private {
        const QList<clsORMField> ORMFields = {
            //ColName               Type                Validation  Default     UpBy    Sort  Filter Self   Virt   PK
            { Fields::pid,          S(quint16),         QFV,        QRequired,  UPNone, true, true,  false, false, true },
            { Fields::language,     S(QString),         QFV,        QRequired,  UPNone, true, true,  false, false, true },
            { Fields::untNameI18N,  S(QString),         QFV,        QRequired,  UPOwner },
        };

        inline const QList<stuRelation> Relations(Q_DECL_UNUSED const QString& _schema) {
            return {};
        };

        const QList<stuDBIndex> Indexes = {
        };

    } //namespace Private

#define SF_tblAccountUnitsI18NBase_DTO \
    SF_quint16                  (pid), \
    SF_QString                  (language), \
    SF_QString                  (untNameI18N)

    TAPI_DEFINE_STRUCT(DTO,
        SF_tblAccountUnitsI18NBase_DTO
    );
}

namespace tblAccountProductsBase {
    namespace Relation {
        constexpr char Saleable[] = "saleable";
        constexpr char Unit[] = "unit";
    }

    namespace Private {
        const QList<clsORMField> ORMFields = {
            //ColName                           Type                                Validation                              Default     UpBy   Sort  Filter Self  Virt   PK
            { Fields::prdID,                    ORM_PRIMARYKEY_32 },
            { Fields::prdCode,                  S(TAPI::ProductCode_t),             QFV,                                    QRequired,  UPOwner },
            ORM_MULTILANGUAGE(Fields::prdName, QRequired,  UPOwner),
            ORM_MULTILANGUAGE(Fields::prdDesc, QNull,      UPOwner),
            { Fields::prdValidFromDate,         S(NULLABLE_TYPE(TAPI::DateTime_t)), QFV,                                    QNull,      UPOwner },
            { Fields::prdValidToDate,           S(NULLABLE_TYPE(TAPI::DateTime_t)), QFV,                                    QNull,      UPOwner },
            { Fields::prdValidFromHour,         S(NULLABLE_TYPE(quint8)),           QFV.integer().minValue(0).maxValue(23), QNull,      UPOwner },
            { Fields::prdValidToHour,           S(NULLABLE_TYPE(quint8)),           QFV.integer().minValue(0).maxValue(23), QNull,      UPOwner },
            { Fields::prdDurationMinutes,       S(NULLABLE_TYPE(quint32)),          QFV,                                    QNull,      UPOwner },
            { Fields::prdPrivs,                 S(TAPI::PrivObject_t),              QFV,                                    QNull,      UPOwner },
            { Fields::prdVAT,                   S(NULLABLE_TYPE(double)),           QFV.real().minValue(0).maxValue(100),   QNull,      UPOwner },
            { Fields::prd_untID,                S(NULLABLE_TYPE(quint16)),          QFV,                                    QNull,      UPOwner },
            { Fields::prdQtyIsDecimal,          S(bool),                            QFV,                                    false,      UPAdmin },
            { Fields::prdInStockQty,            S(double),                          QFV.integer().minValue(0),              QRequired,  UPAdmin },
            { Fields::prdOrderedQty,            S(NULLABLE_TYPE(double)),           QFV,                                    QNull,      UPAdmin },
            { Fields::prdReturnedQty,           S(NULLABLE_TYPE(double)),           QFV,                                    QNull,      UPAdmin },
            { Fields::prdStatus,                ORM_STATUS_FIELD(TAPI::enuGenericStatus, TAPI::enuGenericStatus::Active) },
            { ORM_INVALIDATED_AT_FIELD },
            { Fields::prdCreationDateTime,      ORM_CREATED_ON },
            { Fields::prdCreatedBy_usrID,       ORM_CREATED_BY },
            { Fields::prdUpdatedBy_usrID,       ORM_UPDATED_BY },
        };

        inline const QList<stuRelation> Relations(Q_DECL_UNUSED const QString& _schema) {
            return {
                ///<  Col                                           Reference Table    ForeignCol          Rename     LeftJoin
                { Relation::Saleable,
                    { Fields::prdID, R(_schema, tblAccountSaleablesBase::Name), tblAccountSaleablesBase::Fields::slb_prdID } },
                ORM_RELATION_OF_CREATOR(Fields::prdCreatedBy_usrID),
                ORM_RELATION_OF_UPDATER(Fields::prdUpdatedBy_usrID),
                { Fields::prdID, R(_schema, tblAccountProductsI18NBase::Name), tblAccountProductsI18NBase::Fields::pid },
                { Relation::Unit,
                    { Fields::prd_untID, R(_schema, tblAccountUnitsBase::Name), tblAccountUnitsBase::Fields::untID } },
            };
        };

        const QList<stuDBIndex> Indexes = {
            { {
                Fields::prdCode,
                ORM_INVALIDATED_AT_FIELD_NAME,
              }, enuDBIndex::Unique },
//            { Fields::prdCanUseFromHour },
//            { Fields::prdCanUseToHour },
            { Fields::prdStatus },
            { Fields::prdCreatedBy_usrID },
            { Fields::prdCreationDateTime },
            { Fields::prdUpdatedBy_usrID },
        };

    } //namespace Private

//    SF_NULLABLE_quint8          (prdCanUseFromHour),
//    SF_NULLABLE_quint8          (prdCanUseToHour),

#define SF_tblAccountProductsBase_DTO \
    SF_ORM_PRIMARYKEY_32        (prdID), \
    SF_QString                  (prdCode), \
    SF_QString                  (prdName), \
    SF_QString                  (prdDesc), \
    SF_DateTime_t               (prdValidFromDate), \
    SF_DateTime_t               (prdValidToDate), \
    SF_NULLABLE_quint8          (prdValidFromHour), \
    SF_NULLABLE_quint8          (prdValidToHour), \
    SF_NULLABLE_quint32         (prdDurationMinutes), \
    SF_JSON_t                   (prdPrivs), \
    SF_NULLABLE_qreal           (prdVAT), \
    SF_NULLABLE_quint16         (prd_untID), \
    SF_bool                     (prdQtyIsDecimal), \
    SF_qreal                    (prdInStockQty), \
    SF_NULLABLE_qreal           (prdOrderedQty), \
    SF_NULLABLE_qreal           (prdReturnedQty), \
    SF_ORM_STATUS_FIELD         (prdStatus, TAPI::enuGenericStatus, TAPI::enuGenericStatus::Active), \
    SF_ORM_CREATED_ON           (prdCreationDateTime), \
    SF_ORM_CREATED_BY           (prdCreatedBy_usrID), \
    SF_ORM_UPDATED_BY           (prdUpdatedBy_usrID)

    TAPI_DEFINE_STRUCT(DTO,
        SF_tblAccountProductsBase_DTO
    );
}

namespace tblAccountProductsI18NBase {
    namespace Relation {
    }

    namespace Private {
        const QList<clsORMField> ORMFields = {
            //ColName               Type                Validation  Default     UpBy    Sort  Filter Self   Virt   PK
            { Fields::pid,          S(quint32),         QFV,        QRequired,  UPNone, true, true,  false, false, true },
            { Fields::language,     S(QString),         QFV,        QRequired,  UPNone, true, true,  false, false, true },
            { Fields::prdNameI18N,  S(QString),         QFV,        QRequired,  UPOwner },
            { Fields::prdDescI18N,  S(QString),         QFV,        QNull,      UPOwner },
        };

        inline const QList<stuRelation> Relations(Q_DECL_UNUSED const QString& _schema) {
            return {};
        };

        const QList<stuDBIndex> Indexes = {
        };

    } //namespace Private

#define SF_tblAccountProductsI18NBase_DTO \
    SF_quint32                  (pid), \
    SF_QString                  (language), \
    SF_QString                  (prdNameI18N), \
    SF_QString                  (prdDescI18N)

    TAPI_DEFINE_STRUCT(DTO,
        SF_tblAccountProductsI18NBase_DTO
    );
}

namespace tblAccountSaleablesBase {
    namespace Relation {
        constexpr char Product[] = "product";
        constexpr char UserAsset[] = "userAsset";
    }

    namespace Private {
        const QList<clsORMField> ORMFields = {
            //ColName                           Type                                Validation                      Default     UpBy      Sort  Filter Self  Virt   PK
            { Fields::slbID,                    ORM_PRIMARYKEY_32 },
            { Fields::slb_prdID,                S(quint32),                         QFV.integer().minValue(1),      QRequired,  UPOwner },
            { Fields::slbCode,                  S(TAPI::SaleableCode_t),            QFV,                            QRequired,  UPOwner },
            ORM_MULTILANGUAGE(Fields::slbName, QRequired,  UPOwner),
            ORM_MULTILANGUAGE(Fields::slbDesc, QNull,      UPOwner),
            { Fields::slbType,                  S(TAPI::enuSaleableType::Type),     QFV,                            TAPI::enuSaleableType::Normal, UPOwner },
            { Fields::slbAvailableFromDate,          S(TAPI::DateTime_t),                QFV,                            QNull,      UPOwner },
            { Fields::slbAvailableToDate,            S(TAPI::DateTime_t),                QFV,                            QNull,      UPOwner },
            { Fields::slbPrivs,                 S(TAPI::JSON_t),                    QFV,                            QNull,      UPOwner },
            { Fields::slbBasePrice,             S(qreal),                           QFV.real().minValue(0),         QRequired,  UPOwner },
            { Fields::slbAdditives,             S(TAPI::SaleableAdditive_t),        QFV,                            QNull,      UPOwner },
            { Fields::slbMaxSaleCountPerUser,   S(NULLABLE_TYPE(quint32)),          QFV,                            QNull,      UPOwner },
            { Fields::slbInStockQty,            S(double),                          QFV.integer().minValue(0),      QRequired,  UPAdmin },
            { Fields::slbOrderedQty,            S(NULLABLE_TYPE(double)),           QFV,                            QNull,      UPAdmin },
            { Fields::slbReturnedQty,           S(NULLABLE_TYPE(double)),           QFV,                            QNull,      UPAdmin },
            { Fields::slbVoucherTemplate,       S(QString),                         QFV,                            QNull,      UPOwner },
            { Fields::slbValidFromDate,         S(NULLABLE_TYPE(TAPI::DateTime_t)), QFV,                            QNull,      UPOwner },
            { Fields::slbValidToDate,           S(NULLABLE_TYPE(TAPI::DateTime_t)), QFV,                            QNull,      UPOwner },
            { Fields::slbValidFromHour,         S(NULLABLE_TYPE(quint8)),           QFV.integer().minValue(0).maxValue(23), QNull, UPOwner },
            { Fields::slbValidToHour,           S(NULLABLE_TYPE(quint8)),           QFV.integer().minValue(0).maxValue(23), QNull, UPOwner },
            { Fields::slbDurationMinutes,       S(NULLABLE_TYPE(quint32)),          QFV,                            QNull,      UPOwner },
            { Fields::slbStartAtFirstUse,       S(bool),                            QFV,                            false,      UPOwner },
            { Fields::slbStatus,                ORM_STATUS_FIELD(TAPI::enuGenericStatus, TAPI::enuGenericStatus::Active) },
            { ORM_INVALIDATED_AT_FIELD },
            { Fields::slbCreationDateTime,      ORM_CREATED_ON },
            { Fields::slbCreatedBy_usrID,       ORM_CREATED_BY },
            { Fields::slbUpdatedBy_usrID,       ORM_UPDATED_BY },
        };

        inline const QList<stuRelation> Relations(Q_DECL_UNUSED const QString& _schema) {
            return {
                ///<  Relation Name Col                                 Reference Table                           ForeignCol                    Rename LeftJoin
                { Relation::Product,
                    { Fields::slb_prdID, R(_schema, tblAccountProductsBase::Name), tblAccountProductsBase::Fields::prdID } },
                { Relation::UserAsset,
                    { Fields::slbID, R(_schema, tblAccountUserAssetsBase::Name), tblAccountUserAssetsBase::Fields::uas_slbID } },
                ORM_RELATION_OF_CREATOR(Fields::slbCreatedBy_usrID),
                ORM_RELATION_OF_UPDATER(Fields::slbUpdatedBy_usrID),
                { Fields::slbID, R(_schema, tblAccountSaleablesI18NBase::Name), tblAccountSaleablesI18NBase::Fields::pid },
            };
        };

        const QList<stuDBIndex> Indexes = {
            { {
                Fields::slbCode,
                ORM_INVALIDATED_AT_FIELD_NAME,
              }, enuDBIndex::Unique },
            { Fields::slbType },
            { Fields::slbAvailableFromDate },
            { Fields::slbAvailableToDate },
            { Fields::slbStatus },
            { Fields::slbCreatedBy_usrID },
            { Fields::slbCreationDateTime },
            { Fields::slbUpdatedBy_usrID },
        };

    } //namespace Private

#define SF_tblAccountSaleablesBase_DTO \
    SF_ORM_PRIMARYKEY_32        (slbID), \
    SF_quint32                  (slb_prdID), \
    SF_QString                  (slbCode), \
    SF_QString                  (slbName), \
    SF_QString                  (slbDesc), \
    SF_Enum                     (slbType, TAPI::enuSaleableType, TAPI::enuSaleableType::Normal), \
    SF_DateTime_t               (slbAvailableFromDate), \
    SF_DateTime_t               (slbAvailableToDate), \
    SF_JSON_t                   (slbPrivs), \
    SF_qreal                    (slbBasePrice), \
    SF_JSON_t                   (slbAdditives), \
    SF_NULLABLE_quint32         (slbMaxSaleCountPerUser), \
    SF_qreal                    (slbInStockQty), \
    SF_NULLABLE_qreal           (slbOrderedQty), \
    SF_NULLABLE_qreal           (slbReturnedQty), \
    SF_QString                  (slbVoucherTemplate), \
    SF_DateTime_t               (slbValidFromDate), \
    SF_DateTime_t               (slbValidToDate), \
    SF_NULLABLE_quint8          (slbValidFromHour), \
    SF_NULLABLE_quint8          (slbValidToHour), \
    SF_NULLABLE_quint32         (slbDurationMinutes), \
    SF_bool                     (slbStartAtFirstUse), \
    SF_ORM_STATUS_FIELD         (slbStatus, TAPI::enuGenericStatus, TAPI::enuGenericStatus::Active), \
    SF_ORM_CREATED_ON           (slbCreationDateTime), \
    SF_ORM_CREATED_BY           (slbCreatedBy_usrID), \
    SF_ORM_UPDATED_BY           (slbUpdatedBy_usrID)

    TAPI_DEFINE_STRUCT(DTO,
        SF_tblAccountSaleablesBase_DTO
    );
}

namespace tblAccountSaleablesI18NBase {
    namespace Relation {
    }

    namespace Private {
        const QList<clsORMField> ORMFields = {
            //ColName               Type                Validation  Default     UpBy    Sort  Filter Self   Virt   PK
            { Fields::pid,          S(quint32),         QFV,        QRequired,  UPNone, true, true,  false, false, true },
            { Fields::language,     S(QString),         QFV,        QRequired,  UPNone, true, true,  false, false, true },
            { Fields::slbNameI18N,  S(QString),         QFV,        QRequired,  UPOwner },
            { Fields::slbDescI18N,  S(QString),         QFV,        QNull,      UPOwner },
        };

        inline const QList<stuRelation> Relations(Q_DECL_UNUSED const QString& _schema) {
            return {
                //<Col                      Reference Table                             ForeignCol                              Rename     LeftJoin
            };
        };

        const QList<stuDBIndex> Indexes = {
        };

    } //namespace Private

#define SF_tblAccountSaleablesI18NBase_DTO \
    SF_quint32                  (pid), \
    SF_QString                  (language), \
    SF_QString                  (slbNameI18N), \
    SF_QString                  (slbDescI18N)

    TAPI_DEFINE_STRUCT(DTO,
        SF_tblAccountSaleablesI18NBase_DTO
    );
}

namespace tblAccountSaleablesFilesBase {
    namespace Relation {
    }

    namespace Private {
        const QList<clsORMField> ORMFields = {
            //ColName                   Type                    Validation  Default     UpBy    Sort  Filter Self   Virt   PK
            { Fields::slfID,            ORM_PRIMARYKEY_64 },
            { Fields::slf_slbID,        S(quint32),             QFV,        QRequired,  UPAdmin },
            { Fields::slfName,          S(QString),             QFV,        QRequired,  UPAdmin },
            { Fields::slfDesc,          S(QString),             QFV,        QNull,      UPAdmin },
            { Fields::slfMimeTypes,     S(QString),             QFV,        QNull,      UPAdmin },
            { Fields::slfExtensions,    S(QString),             QFV,        QNull,      UPAdmin },
            { Fields::slfMinSize,       S(quint64),             QFV,        QNull,      UPAdmin },
            { Fields::slfMaxSize,       S(quint64),             QFV,        QNull,      UPAdmin },
            { Fields::slfMinCount,      S(quint16),             QFV,        QNull,      UPAdmin },
            { Fields::slfMaxCount,      S(quint16),             QFV,        QNull,      UPAdmin },
        };

        inline const QList<stuRelation> Relations(Q_DECL_UNUSED const QString& _schema) {
            return {
                //<Col                      Reference Table                             ForeignCol                              Rename      LeftJoin
                { Fields::slf_slbID,        R(_schema, tblAccountSaleablesBase::Name),  tblAccountSaleablesBase::Fields::slbID, "",         true },
            };
        };

        const QList<stuDBIndex> Indexes = {
        };

    } //namespace Private

#define SF_tblAccountSaleablesFilesBase_DTO \
    SF_quint64                  (slfID), \
    SF_quint32                  (slf_slbID), \
    SF_QString                  (slfName), \
    SF_QString                  (slfDesc), \
    SF_QString                  (slfMimeTypes), \
    SF_QString                  (slfExtensions), \
    SF_quint64                  (slfMinSize), \
    SF_quint64                  (slfMaxSize), \
    SF_quint16                  (slfMinCount), \
    SF_quint16                  (slfMaxCount)

    TAPI_DEFINE_STRUCT(DTO,
        SF_tblAccountSaleablesFilesBase_DTO
    );
}

namespace tblAccountUserAssetsBase {
    namespace Relation {
        constexpr char Saleable[] = "Saleable";
        constexpr char Usage[] = "Usage";
    }

    namespace Private {
        const QList<clsORMField> ORMFields = {
            //Col                               Type                                Validation                  Default     UpBy   Sort  Filter Self  Virt   PK
            { Fields::uasID,                    ORM_PRIMARYKEY_64 },
            { Fields::uas_actorID,              S(quint64),                         QFV.integer().minValue(1),  QRequired,  UPNone },
            { Fields::uas_slbID,                S(quint64),                         QFV.integer().minValue(1),  QRequired,  UPNone },
            { Fields::uasQty,                   S(double),                          QFV,                        QRequired,  UPAdmin },
            { Fields::uas_vchID,                S(NULLABLE_TYPE(quint64)),          QFV.integer().minValue(1),  QNull,      UPAdmin },
            { Fields::uasVoucherItemUUID,       S(TAPI::MD5_t),                     QFV,                        QRequired,  UPNone },
            { Fields::uasVoucherItemInfo,       S(TAPI::JSON_t),                    QFV,                        QRequired,  UPAdmin },
            { Fields::uas_cpnID,                S(NULLABLE_TYPE(quint32)),          QFV,                        QNull,      UPAdmin },
            { Fields::uasDiscountAmount,        S(NULLABLE_TYPE(quint32)),          QFV,                        QNull,      UPAdmin },
            { Fields::uasPrefered,              S(bool),                            QFV,                        false,      UPOwner },
            { Fields::uasValidFromDate,         S(NULLABLE_TYPE(TAPI::DateTime_t)), QFV,                        QNull,      UPOwner },
            { Fields::uasValidToDate,           S(NULLABLE_TYPE(TAPI::DateTime_t)), QFV,                        QNull,      UPOwner },
            { Fields::uasValidFromHour,         S(NULLABLE_TYPE(quint8)),           QFV.integer().minValue(0).maxValue(23), QNull, UPOwner },
            { Fields::uasValidToHour,           S(NULLABLE_TYPE(quint8)),           QFV.integer().minValue(0).maxValue(23), QNull, UPOwner },
            { Fields::uasDurationMinutes,       S(NULLABLE_TYPE(quint32)),          QFV,                        QNull,      UPOwner },
            { Fields::uasBreakedAt,             S(NULLABLE_TYPE(TAPI::DateTime_t)), QFV,                        QNull,      UPOwner },
            { Fields::uasStatus,                ORM_STATUS_FIELD(TAPI::enuAuditableStatus, TAPI::enuAuditableStatus::Pending) },
            { ORM_INVALIDATED_AT_FIELD },
            { Fields::uasCreationDateTime,      ORM_CREATED_ON }, //S(TAPI::DateTime_t),        QFV,                        QNow,       UPNone },
            { Fields::uasUpdatedBy_usrID,       ORM_UPDATED_BY },
        };

        inline const QList<stuRelation> Relations(Q_DECL_UNUSED const QString& _schema) {
            return {
                //<Col                      Reference Table                             ForeignCol                              Rename     LeftJoin
//                { Fields::uas_usrID,        R(AAASchema, tblUser::Name),                tblUser::Fields::usrID,                 "Owner_" },
                { Relation::Saleable,
                    { Fields::uas_slbID,    R(_schema, tblAccountSaleablesBase::Name),  tblAccountSaleablesBase::Fields::slbID, "",         true } },
                { Fields::uas_cpnID,        R(_schema, tblAccountCouponsBase::Name),    tblAccountCouponsBase::Fields::cpnID,   "",         true },
//                { Fields::uas_vchID,        R(AAASchema, tblVoucher::Name),             tblVoucher::Fields::vchID,              "",         true},
                { Relation::Usage,
                    { Fields::uasID,        R(_schema, tblAccountAssetUsageBase::Name),  tblAccountAssetUsageBase::Fields::usg_uasID } },
            };
        };

        const QList<stuDBIndex> Indexes = {
            { {
                  Fields::uas_actorID,
                  Fields::uasVoucherItemUUID,
                  ORM_INVALIDATED_AT_FIELD_NAME,
              }, enuDBIndex::Unique },
            { Fields::uas_actorID },
            { Fields::uas_slbID },
            { Fields::uas_vchID },
            { Fields::uasVoucherItemUUID },
            { Fields::uas_cpnID },
            { Fields::uasStatus },
            { Fields::uasCreationDateTime },
            { Fields::uasUpdatedBy_usrID },
        };

    } //namespace Private

#define SF_tblAccountUserAssetsBase_DTO \
    SF_ORM_PRIMARYKEY_64        (uasID), \
    SF_quint64                  (uas_actorID), \
    SF_quint64                  (uas_slbID), \
    SF_qreal                    (uasQty), \
    SF_NULLABLE_quint64         (uas_vchID), \
    SF_MD5_t                    (uasVoucherItemUUID), \
    SF_JSON_t                   (uasVoucherItemInfo), \
    SF_NULLABLE_quint32         (uas_cpnID), \
    SF_NULLABLE_quint32         (uasDiscountAmount), \
    SF_bool                     (uasPrefered), \
    SF_DateTime_t               (uasValidFromDate), \
    SF_DateTime_t               (uasValidToDate), \
    SF_NULLABLE_quint8          (uasValidFromHour), \
    SF_NULLABLE_quint8          (uasValidToHour), \
    SF_NULLABLE_quint32         (uasDurationMinutes), \
    SF_DateTime_t               (uasBreakedAt), \
    SF_ORM_STATUS_FIELD         (uasStatus, TAPI::enuAuditableStatus, TAPI::enuAuditableStatus::Pending), \
    SF_DateTime_t               (uasCreationDateTime), \
    SF_ORM_UPDATED_BY           (uasUpdatedBy_usrID)

    TAPI_DEFINE_STRUCT(DTO,
        SF_tblAccountUserAssetsBase_DTO
    );
}

namespace tblAccountUserAssetsFilesBase {
    namespace Relation {
        constexpr char UserAssets[]   = "UserAssets";
        constexpr char SaleableFile[] = "SaleableFile";
        constexpr char UploadedFile[] = "UploadedFile";
    }

    namespace Private {
        const QList<clsORMField> ORMFields = {
            //Col                               Type                    Validation      Default     UpBy   Sort  Filter Self  Virt   PK
            { Fields::uasuflID,                 ORM_PRIMARYKEY_64 },
            { Fields::uasufl_uasID,             S(quint64),             QFV,            QRequired,  UPNone },
            { Fields::uasufl_slfID,             S(quint64),             QFV,            QRequired,  UPNone },
            { Fields::uasufl_uflID,             S(quint64),             QFV,            QRequired,  UPNone },
            { Fields::uasuflCreationDateTime,   S(TAPI::DateTime_t),    QFV,            QNow,       UPNone },
            { Fields::uasuflCreatedBy_usrID,    ORM_CREATED_BY },
        };

        inline const QList<stuRelation> Relations(Q_DECL_UNUSED const QString& _schema) {
            return {
                //Col                           Reference Table                                 ForeignCol                      Rename     LeftJoin
                { Relation::UserAssets,
                    { Fields::uasufl_uasID,     R(_schema, tblAccountUserAssetsBase::Name),     tblAccountUserAssetsBase::Fields::uasID } },
                { Relation::SaleableFile,
                    { Fields::uasufl_slfID,     R(_schema, tblAccountSaleablesFilesBase::Name),  tblAccountSaleablesFilesBase::Fields::slfID } },
                { Relation::UploadedFile,
                    { Fields::uasufl_uflID,     R(_schema, tblUploadFiles::Name),               tblUploadFiles::Fields::uflID } },
            };
        };

        const QList<stuDBIndex> Indexes = {
        };

    } //namespace Private

#define SF_tblAccountUserAssetsFilesBase_DTO \
    SF_ORM_PRIMARYKEY_64        (uasuflID), \
    SF_quint64                  (uasufl_uasID), \
    SF_quint64                  (uasufl_slfID), \
    SF_quint64                  (uasufl_uflID), \
    SF_DateTime_t               (uasuflCreationDateTime), \
    SF_ORM_UPDATED_BY           (uasuflCreatedBy_usrID)

    TAPI_DEFINE_STRUCT(DTO,
        SF_tblAccountUserAssetsFilesBase_DTO
    );
}

namespace tblAccountAssetUsageBase {
    namespace Relation {
//        constexpr char AAA[] = "aaa";
    }

    namespace Private {
        const QList<clsORMField> ORMFields = {
            ///<  ColName                              Type                Validation                              Default    UpBy   Sort  Filter Self  Virt   PK
            { Fields::usg_uasID, ORM_PRIMARYKEY_64},
        };

        inline const QList<stuRelation> Relations(Q_DECL_UNUSED const QString& _schema) {
            return {
                ///<  Col                                  Reference Table                                  ForeignCol                        Rename     LeftJoin
                { Fields::usg_uasID, R(_schema, tblAccountUserAssetsBase::Name), tblAccountUserAssetsBase::Fields::uasID},
            };
        };

        const QList<stuDBIndex> Indexes = {
        };

    } //namespace Private

#define SF_tblAccountAssetUsageBase_DTO \
    SF_ORM_PRIMARYKEY_64        (usg_uasID)

    TAPI_DEFINE_STRUCT(DTO,
        SF_tblAccountAssetUsageBase_DTO
    );
}

namespace tblAccountAssetUsageHistoryBase {
    namespace Relation {
//        constexpr char AAA[] = "aaa";
    }

    namespace Private {
        const QList<clsORMField> ORMFields = {
            //ColName                   Type                    Validation  Default    UpBy   Sort  Filter Self  Virt   PK
            { Fields::ush_uasID,        ORM_PRIMARYKEY_64 },
            { Fields::ushLastDateTime,  S(TAPI::DateTime_t),    QFV,        QRequired, UPAdmin },
        };

        inline const QList<stuRelation> Relations(Q_DECL_UNUSED const QString& _schema) {
            return {
                //Col                   Reference Table                             ForeignCol                                  Rename     LeftJoin
                { Fields::ush_uasID,    R(_schema, tblAccountUserAssetsBase::Name), tblAccountUserAssetsBase::Fields::uasID },
            };
        };

        const QList<stuDBIndex> Indexes = {
        };

    } //namespace Private

#define SF_tblAccountAssetUsageHistoryBase_DTO \
    SF_ORM_PRIMARYKEY_64        (ush_uasID), \
    SF_DateTime_t               (ushLastDateTime)

    TAPI_DEFINE_STRUCT(DTO,
        SF_tblAccountAssetUsageHistoryBase_DTO
    );
}

namespace tblAccountCouponsBase {
    namespace Relation {
//        constexpr char AAA[] = "aaa";
    }

    namespace Private {
        const QList<clsORMField> ORMFields = {
            ///ColName                                            Type                               Validation                               Default    UpBy   Sort  Filter Self  Virt   PK
            { Fields::cpnID,                      ORM_PRIMARYKEY_32 },
            { Fields::cpnCode,                    S(TAPI::CouponCode_t),             QFV,                                     QRequired, UPAdmin },
            { Fields::cpnPrimaryCount,            S(quint32),                        QFV.integer().minValue(1),               1,         UPAdmin },
            { Fields::cpnTotalMaxAmount,          S(quint32),                        QFV.integer().minValue(1),               1,         UPAdmin },
            { Fields::cpnPerUserMaxCount,         S(NULLABLE_TYPE(quint32)),         QFV.optional(QFV.integer().minValue(1)), QNull,     UPAdmin },
            { Fields::cpnPerUserMaxAmount,        S(NULLABLE_TYPE(quint32)),         QFV.integer().minValue(1),               QNull,     UPAdmin },
            { Fields::cpnValidFrom,               S(TAPI::DateTime_t),               QFV,                                     QRequired, UPAdmin },
            { Fields::cpnExpiryTime,              S(NULLABLE_TYPE(TAPI::DateTime_t)),QFV,                                     QNull,     UPAdmin },
            { Fields::cpnAmount,                  S(quint32),                        QFV,                                     QRequired, UPAdmin }, //, false, false },
            { Fields::cpnAmountType,              S(Targoman::API::AAA::enuDiscountType::Type), QFV,              Targoman::API::AAA::enuDiscountType::Percent, UPAdmin },
            { Fields::cpnMaxAmount,               S(NULLABLE_TYPE(quint32)),         QFV,                                     QNull,     UPAdmin }, //, false, false },
            { Fields::cpnSaleableBasedMultiplier, S(TAPI::JSON_t),                   QFV,                                     QRequired, UPAdmin }, //, false, false },
//            { Fields::cpnSaleableBasedMultiplier, S(QList<Targoman::API::AAA::stuDiscountSaleableBasedMultiplier>), QFV,                    QRequired, UPAdmin, false, false },
            { Fields::cpnTotalUsedCount,          S(quint32),                        QFV.integer().minValue(0),               0,         UPNone },
            { Fields::cpnTotalUsedAmount,         S(quint32),                        QFV.integer().minValue(0),               0,         UPNone },
            { Fields::cpnStatus,                  ORM_STATUS_FIELD(TAPI::enuGenericStatus, TAPI::enuGenericStatus::Active) },
            { ORM_INVALIDATED_AT_FIELD },
            { Fields::cpnCreationDateTime,        ORM_CREATED_ON },
            { Fields::cpnCreatedBy_usrID,         ORM_CREATED_BY },
            { Fields::cpnUpdatedBy_usrID,         ORM_UPDATED_BY },
        };

        inline const QList<stuRelation> Relations(Q_DECL_UNUSED const QString& _schema) {
            return {
                ///Col                           Reference Table                             ForeignCol                              Rename    LeftJoin
                { Fields::cpnID, R(_schema, tblAccountUserAssetsBase::Name), tblAccountUserAssetsBase::Fields::uas_cpnID,    "",       true },
                ORM_RELATION_OF_CREATOR(Fields::cpnCreatedBy_usrID),
                ORM_RELATION_OF_UPDATER(Fields::cpnUpdatedBy_usrID),
            };
        };

        const QList<stuDBIndex> Indexes = {
            { {
               Fields::cpnCode,
               ORM_INVALIDATED_AT_FIELD_NAME,
              }, enuDBIndex::Unique },
            { Fields::cpnAmountType },
            { Fields::cpnValidFrom },
            { Fields::cpnExpiryTime },
            { Fields::cpnStatus },
            { Fields::cpnCreatedBy_usrID },
            { Fields::cpnCreationDateTime },
            { Fields::cpnUpdatedBy_usrID },
        };

    } //namespace Private

#define SF_tblAccountCouponsBase_DTO \
    SF_ORM_PRIMARYKEY_32        (cpnID), \
    SF_QString                  (cpnCode), \
    SF_quint32                  (cpnPrimaryCount), \
    SF_quint32                  (cpnTotalMaxAmount), \
    SF_NULLABLE_quint32         (cpnPerUserMaxCount), \
    SF_NULLABLE_quint32         (cpnPerUserMaxAmount), \
    SF_DateTime_t               (cpnValidFrom), \
    SF_DateTime_t               (cpnExpiryTime), \
    SF_quint32                  (cpnAmount), \
    SF_Enum                     (cpnAmountType, Targoman::API::AAA::enuDiscountType, Targoman::API::AAA::enuDiscountType::Percent), \
    SF_NULLABLE_quint32         (cpnMaxAmount), \
    SF_JSON_t                   (cpnSaleableBasedMultiplier), \
    SF_quint32                  (cpnTotalUsedCount), \
    SF_quint32                  (cpnTotalUsedAmount), \
    SF_ORM_STATUS_FIELD         (cpnStatus, TAPI::enuGenericStatus, TAPI::enuGenericStatus::Active), \
    SF_ORM_CREATED_ON           (cpnCreationDateTime), \
    SF_ORM_CREATED_BY           (cpnCreatedBy_usrID), \
    SF_ORM_UPDATED_BY           (cpnUpdatedBy_usrID)

    TAPI_DEFINE_STRUCT(DTO,
        SF_tblAccountCouponsBase_DTO
    );
}

namespace tblAccountPrizesBase {
    namespace Relation {
//        constexpr char AAA[] = "aaa";
    }

    namespace Private {
        const QList<clsORMField> ORMFields = {
            ///ColName                                            Type                               Validation                               Default    UpBy   Sort  Filter Self  Virt   PK
            { Fields::przID,                      ORM_PRIMARYKEY_32 },
            { Fields::przStatus,                  ORM_STATUS_FIELD(TAPI::enuGenericStatus, TAPI::enuGenericStatus::Active) },
            { ORM_INVALIDATED_AT_FIELD },
            { Fields::przCreationDateTime,        ORM_CREATED_ON },
            { Fields::przCreatedBy_usrID,         ORM_CREATED_BY },
            { Fields::przUpdatedBy_usrID,         ORM_UPDATED_BY },
        };

        inline const QList<stuRelation> Relations(Q_DECL_UNUSED const QString& _schema) {
            return {};
        };

        const QList<stuDBIndex> Indexes = {
        };

    } //namespace Private

#define SF_tblAccountPrizesBase_DTO \
    SF_ORM_PRIMARYKEY_32        (przID), \
    SF_ORM_STATUS_FIELD         (przStatus, TAPI::enuGenericStatus, TAPI::enuGenericStatus::Active), \
    SF_ORM_CREATED_ON           (przCreationDateTime), \
    SF_ORM_CREATED_BY           (przCreatedBy_usrID), \
    SF_ORM_UPDATED_BY           (przUpdatedBy_usrID)

    TAPI_DEFINE_STRUCT(DTO,
        SF_tblAccountPrizesBase_DTO
    );
}

/*
not used??????
namespace tblAccountReferralsBase {
    constexpr char Name[] = "tblAccountReferrals";

    namespace Fields {
        TARGOMAN_CREATE_CONSTEXPR(ref_usrID);
        TARGOMAN_CREATE_CONSTEXPR(refCanUseFromDateTime);
        TARGOMAN_CREATE_CONSTEXPR(refCanUseToDateTime);
        TARGOMAN_CREATE_CONSTEXPR(refPrizeInfo);
        TARGOMAN_CREATE_CONSTEXPR(refStatus);
        TARGOMAN_CREATE_CONSTEXPR(refCreationDateTime);
        TARGOMAN_CREATE_CONSTEXPR(refCreatedBy_usrID);
        TARGOMAN_CREATE_CONSTEXPR(refUpdatedBy_usrID);
    }

    namespace Relation {
//        constexpr char AAA[] = "aaa";
    }

    namespace Private {
        const QList<clsORMField> ORMFields = {
        };

        inline const QList<stuRelation> Relations(Q_DECL_UNUSED const QString& _schema) {
            return {};
        };

        const QList<stuDBIndex> Indexes = {
        };

    } //namespace Private

    TAPI_DEFINE_STRUCT(DTO,
    );
}
*/

#pragma GCC diagnostic pop




TAPI_DEFINE_STRUCT(stuPrize,
    SF_QString          (Desc),
    SF_QJsonObject      (PrizeInfo) //Interpreted by the module
);
//,  QJsonObject, QJsonObject(), v.size(), v, v.toObject()

TAPI_DEFINE_STRUCT(stuDiscountSaleableBasedMultiplier,
    SF_QString          (SaleableCode),
    SF_qreal            (Multiplier),
    SF_NULLABLE_qreal   (MinQty)
);

TAPI_DEFINE_STRUCT(stuPendingSystemDiscount,
    SF_QString          (Key),
    SF_QString          (Desc),
    SF_qreal            (Amount),
    SF_Enum             (AmountType, enuDiscountType, enuDiscountType::Percent),
    SF_qreal            (Max) //MaxType is opposite of AmountType
);

TAPI_DEFINE_STRUCT(stuSystemDiscount,
//    SF_QString          (Key),
    SF_qreal            (Amount),
    SF_QJsonObject      (Info)
);
typedef QMap<QString, stuSystemDiscount> SystemDiscounts_t;

TAPI_DEFINE_STRUCT(stuCouponDiscount,
    SF_quint64          (ID),
    SF_QString          (Code),
    SF_qreal            (Amount)
);

/*****************************************************************************/
TAPI_DEFINE_STRUCT(stuPendingVoucher,
    SF_QString          (Name),
    SF_Enum             (Type, enuVoucherType, enuVoucherType::Credit),
    SF_quint64          (Amount),
    SF_QJsonObject      (Info)
);

TAPI_DEFINE_STRUCT(stuVoucherItemPrivate,
    SF_QListOfVarStruct (PendingVouchers, stuPendingVoucher)
);

TAPI_DEFINE_STRUCT(stuBasketItem,
    SF_Struct           (Unit, tblAccountUnitsBase::DTO, v.untID),
    SF_Struct           (Product, tblAccountProductsBase::DTO, v.prdID),
    SF_Struct           (Saleable, tblAccountSaleablesBase::DTO, v.slbID),

    SF_qreal            (prdQtyInHand),
    SF_qreal            (slbQtyInHand),

    //-- input
    SF_QMapOfQString    (OrderAdditives),
    SF_QString          (DiscountCode),
    SF_QString          (Referrer),
    SF_JSON_t           (ReferrerParams),
    SF_qreal            (Qty),

    SF_QJsonObject      (APITokenPayload),
    SF_quint64          (AssetActorID), //CurrentUserID or APIToken.Payload[uid]

    //-- compute
    SF_qreal            (UnitPrice),
    SF_qreal            (SubTotal),

    SF_QMapOfVarStruct  (SystemDiscounts, stuSystemDiscount, SystemDiscounts_t),
    SF_Struct           (CouponDiscount, stuCouponDiscount, v.ID),
    SF_qreal            (Discount),
    SF_qreal            (AfterDiscount),
    SF_quint8           (VATPercent),
    SF_qreal            (VAT),
    SF_qreal            (TotalPrice),

//    SF_Struct           (Digested, stuDigested, [](Q_DECL_UNUSED auto v) { return true; }(v)),

    SF_QJsonObject      (AdditionalInfo), //per service

    SF_Struct           (Private, stuVoucherItemPrivate, [](Q_DECL_UNUSED auto v) { return true; } (v))
);

/*****************************************************************************/
TAPI_DEFINE_STRUCT(stuUsage,
    SF_NULLABLE_quint32 (PerDay),
    SF_NULLABLE_quint32 (PerWeek),
    SF_NULLABLE_quint32 (PerMonth),
    SF_NULLABLE_quint64 (Total)
);

///key : action
typedef QMap<QString, stuUsage> UsageLimits_t;

struct stuUsageColDefinition {
    QString AssetPerDay;
    QString UsagePerDay;
    QString AssetPerWeek;
    QString UsagePerWeek;
    QString AssetPerMonth;
    QString UsagePerMonth;
    QString AssetTotal;
    QString UsageTotal;

    stuUsageColDefinition()
    { ; }

    stuUsageColDefinition(const stuUsageColDefinition &_other) :
        AssetPerDay(_other.AssetPerDay),     UsagePerDay(_other.UsagePerDay),
        AssetPerWeek(_other.AssetPerWeek),   UsagePerWeek(_other.UsagePerWeek),
        AssetPerMonth(_other.AssetPerMonth), UsagePerMonth(_other.UsagePerMonth),
        AssetTotal(_other.AssetTotal),       UsageTotal(_other.UsageTotal)
    { ; }

    stuUsageColDefinition(
        const QString &_assetPerDay,    const QString &_usagePerDay,
        const QString &_assetPerWeek,   const QString &_usagePerWeek,
        const QString &_assetPerMonth,  const QString &_usagePerMonth,
        const QString &_assetTotal,     const QString &_usageTotal
    ) :
        AssetPerDay(_assetPerDay),      UsagePerDay(_usagePerDay),
        AssetPerWeek(_assetPerWeek),    UsagePerWeek(_usagePerWeek),
        AssetPerMonth(_assetPerMonth),  UsagePerMonth(_usagePerMonth),
        AssetTotal(_assetTotal),        UsageTotal(_usageTotal)
    { ; }
};

///key : action
typedef QMap<QString, stuUsageColDefinition> AssetUsageLimitsCols_t;

inline AssetUsageLimitsCols_t mergeAssetUsageLimitsCols(const AssetUsageLimitsCols_t &_a, const AssetUsageLimitsCols_t &_b) {

    AssetUsageLimitsCols_t Result = _a;

    for (auto it = _b.constBegin();
         it != _b.constEnd();
         it++
    ) {
        if (Result.contains(it.key()) == false) {
            Result.insert(it.key(), it.value());
        } else {
            if (it->AssetPerDay.isEmpty() == false)      Result[it.key()].AssetPerDay   = it->AssetPerDay;
            if (it->AssetPerWeek.isEmpty() == false)     Result[it.key()].AssetPerWeek  = it->AssetPerWeek;
            if (it->AssetPerMonth.isEmpty() == false)    Result[it.key()].AssetPerMonth = it->AssetPerMonth;
            if (it->AssetTotal.isEmpty() == false)       Result[it.key()].AssetTotal    = it->AssetTotal;

            if (it->UsagePerDay.isEmpty() == false)      Result[it.key()].UsagePerDay   = it->UsagePerDay;
            if (it->UsagePerWeek.isEmpty() == false)     Result[it.key()].UsagePerWeek  = it->UsagePerWeek;
            if (it->UsagePerMonth.isEmpty() == false)    Result[it.key()].UsagePerMonth = it->UsagePerMonth;
            if (it->UsageTotal.isEmpty() == false)       Result[it.key()].UsageTotal    = it->UsageTotal;
        }
    }

    return Result;
}

typedef QMap<QString, QString> OrderAdditives_t;

TAPI_DEFINE_STRUCT(stuDigested,
//    QJsonObject   Additives;

//    UsageLimits_t Limits;
    SF_QMapOfVarStruct  (Limits, stuUsage, UsageLimits_t)

//    QJsonObject   Privs;
);

TAPI_DEFINE_STRUCT(stuAssetItem,
    SF_Struct           (Product, tblAccountProductsBase::DTO, v.prdID),
    SF_Struct           (Saleable, tblAccountSaleablesBase::DTO, v.slbID),
    SF_Struct           (UserAsset, tblAccountUserAssetsBase::DTO, v.uasID),
    SF_Struct           (AssetUsage, tblAccountAssetUsageBase::DTO, v.usg_uasID),

    SF_Struct           (Digested, stuDigested, [](Q_DECL_UNUSED auto v) { return true; }(v))
);

///@TODO: key : ???
//typedef QMap<QString, stuAssetItem> ActiveCredits_t;
typedef QList<stuAssetItem> ActiveCredits_t;

struct stuServiceCreditsInfo {
    ActiveCredits_t             ActiveCredits;
    NULLABLE_TYPE(stuAssetItem) PreferedCredit;
    NULLABLE_TYPE(quint32)      ParentID;
    UsageLimits_t               MyLimitsOnParent;
    QDateTime                   DBCurrentDateTime;

    stuServiceCreditsInfo(ActiveCredits_t             _activeCredits,
                          NULLABLE_TYPE(stuAssetItem) _preferedCredit,
                          NULLABLE_TYPE(quint32)      _parentID,
                          UsageLimits_t               _myLimitsOnParent,
                          QDateTime                   _dbCurrentDateTime);
};

/*
struct stuActiveCredit {
    stuAssetItem  Credit;
    bool          IsFromParent;
    UsageLimits_t MyLimitsOnParent;
    qint64        TTL;

    stuActiveCredit(const stuAssetItem& _credit = {},
                    bool _isFromParent = false,
                    const UsageLimits_t& _myLimitsOnParent = {},
                    qint64 _ttl = 0);

//    QJsonObject toJson(bool _full);
//    stuActiveCredit& fromJson(const QJsonObject _obj);
};
*/

TAPI_DEFINE_STRUCT(stuActiveCredit,
    SF_Struct           (Credit, stuAssetItem, v.UserAsset.uasID),
    SF_bool             (IsFromParent),
    SF_QMapOfVarStruct  (MyLimitsOnParent, stuUsage, UsageLimits_t)
//    SF_qint64           (TTL)
);

//constexpr char DISCOUNT_TYPE_SYSTEM[]   = "SYSTEM";
//constexpr char DISCOUNT_TYPE_COUPON[]   = "COUPON";

//TAPI_DEFINE_STRUCT(stuVoucherItemReferrer,
//    SF_QString          (Referrer),
//    SF_qreal            (Qty)
//);

//Caution: Do not rename fields. Field names are used in vchDesc (as json)
TAPI_DEFINE_STRUCT(stuVoucherItem,
    SF_QString              (Service),
    SF_quint64              (OrderID), //AssetID per Service
    SF_MD5_t                (UUID),
    SF_QString              (Desc),
    SF_qreal                (Qty),
    SF_QString              (Unit),
    SF_qreal                (UnitPrice),
    SF_qreal                (SubTotal),
    SF_QMapOfVarStruct      (SystemDiscounts, stuSystemDiscount, SystemDiscounts_t),
    SF_Struct               (CouponDiscount, stuCouponDiscount, v.ID),
    SF_qreal                (DisAmount),
    SF_qreal                (AfterDiscount),
    SF_quint8               (VATPercent),
    SF_qreal                (VATAmount),
    SF_qreal                (TotalPrice),

//    SF_QListOfVarStruct (Referrers, stuVoucherItemReferrer),
    SF_QMapOfQString        (Additives),
    SF_QString              (Referrer),
    SF_JSON_t               (ReferrerParams),
    SF_NULLABLE_quint64     (APITokenID),

    SF_QString              (Private), //encrypted + base64
    SF_QListOfVarStruct     (SubItems, stuVoucherItem),

    SF_QString              (Sign)
);

//typedef QList<stuVoucherItem> InvItems_t;

/**
 * @brief TAPI_DEFINE_STRUCT
 * in case of changing fields, must be increase version and complete migratePreVoucher
 */
TAPI_DEFINE_STRUCT(stuPreVoucher,
    SF_quint8           (Version, 1),
//    SF_NULLABLE_Enum    (Type, enuPreVoucherType),
    SF_quint64          (UserID),
    SF_QListOfVarStruct (Items, stuVoucherItem),
//    SF _ Struct           (Prize, stuPrize, v.Desc.size()),
    SF_QString          (Summary),
    SF_quint16          (Round),
    SF_quint32          (ToPay, 0, v>0),
    SF_quint64          (VoucherID),
    SF_QString          (Sign)
);

//v.Items.isEmpty() == false
TAPI_DEFINE_STRUCT(stuBasketActionResult,
    SF_Struct           (PreVoucher, stuPreVoucher, [](Q_DECL_UNUSED auto v) { return true; }(v)),
    SF_MD5_t            (TargetItemKey)
);

TAPI_DEFINE_STRUCT(stuVoucher,
    SF_Struct           (Info, stuPreVoucher, v.ToPay),
//    SF_Struct           (PreVoucher, stuPreVoucher, v.ToPay),
    SF_quint64          (ID, 0, v>0),
    SF_QString          (PaymentLink),
    SF_QString          (PaymentKey),
    SF_quint32          (Payed),
    SF_quint32          (Remained),
    SF_Enum             (Status, enuVoucherStatus, enuVoucherStatus::New)
);

TAPI_DEFINE_STRUCT(stuVoucherItemForTrustedAction,
    SF_quint64          (UserID),
    SF_quint64          (VoucherID),
    SF_Struct           (VoucherItem, stuVoucherItem, v.Qty > 0),
    SF_QString          (Sign)
);

//TAPI_DEFINE_STRUCT(stuVoucherItemProcess,
//    SF_MD5_t                    (UUID),
//    SF_NULLABLE_Enum            (Status, enuVoucherItemProcessStatus)
//);

//bool KKKKKKKKKKKKKKKKKK() {return true;}


namespace RequestedUsage {
    constexpr char CREDIT[] = "credit";
}

//key: request param
typedef QVariantMap ServiceUsage_t;
//typedef QMap<QString, qint32> ServiceUsage_t;

extern void checkPreVoucherSanity(stuPreVoucher _preVoucher);

inline QString makeConfig(const QString& _name) { return "/Module_Account/" + _name; }
extern Targoman::Common::Configuration::tmplConfigurable<QString> Secret;
extern QByteArray sign(stuPreVoucher _preVoucher);
extern QByteArray sign(stuVoucherItem _voucherItem);
extern QByteArray sign(stuVoucherItemForTrustedAction _voucherItemForTrustedAction);
extern Targoman::Common::Configuration::tmplConfigurable<quint64> SimpleCryptKey;
extern clsSimpleCrypt* simpleCryptInstance();

} //namespace Targoman::API::AAA

TAPI_DECLARE_METATYPE(Targoman::API::AAA::stuPrize)
TAPI_DECLARE_METATYPE(Targoman::API::AAA::stuSystemDiscount)
TAPI_DECLARE_METATYPE(Targoman::API::AAA::stuCouponDiscount)
TAPI_DECLARE_METATYPE(Targoman::API::AAA::stuVoucherItem)         // -> TAPI_REGISTER_METATYPE() in Accounting_Interfaces.cpp
TAPI_DECLARE_METATYPE(Targoman::API::AAA::stuUsage)

TAPI_DECLARE_METATYPE(Targoman::API::AAA::stuPreVoucher)          // -> TAPI_REGISTER_METATYPE() in Accounting_Interfaces.cpp
TAPI_DECLARE_METATYPE(Targoman::API::AAA::stuBasketActionResult)  // -> TAPI_REGISTER_METATYPE() in Accounting_Interfaces.cpp
TAPI_DECLARE_METATYPE(Targoman::API::AAA::stuVoucher)             // -> TAPI_REGISTER_METATYPE() in Accounting_Interfaces.cpp
TAPI_DECLARE_METATYPE(Targoman::API::AAA::OrderAdditives_t)       // -> TAPI_REGISTER_METATYPE() in Accounting_Interfaces.cpp

TAPI_DECLARE_METATYPE(Targoman::API::AAA::stuVoucherItemForTrustedAction)   // -> TAPI_REGISTER_METATYPE() in Accounting_Interfaces.cpp

#endif // TARGOMAN_API_AAA_ACCOUNTING_DEFS_H
