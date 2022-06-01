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

#ifndef TARGOMAN_API_MODULES_ACCOUNT_ORM_WALLETTRANSACTIONS_H
#define TARGOMAN_API_MODULES_ACCOUNT_ORM_WALLETTRANSACTIONS_H

#include "Interfaces/AAA/AAA.hpp"
#include "Interfaces/API/intfSQLBasedModule.h"
#include "UserWallets.h"
#include "Voucher.h"

namespace Targoman::API::AccountModule {

//structures and enumes goes here

//what is this?
//TARGOMAN_DEFINE_ENUM(enuWalletTransactionStatus,
//                     New = 'A',
//                     Processed = 'P',
//                     Removed = 'R'
//                     )

TARGOMAN_DEFINE_ENUM(enuWalletTransactionStatus,
                     New      = 'N',
//                     Pending  = 'P',
//                     Payed    = 'Y',
                     Succeded = 'A',
//                     Error    = 'E',
                     Removed  = 'R'
                     )

} //namespace Targoman::API::AccountModule

TAPI_DECLARE_METATYPE_ENUM(Targoman::API::AccountModule, enuWalletTransactionStatus);

namespace Targoman::API::AccountModule {
namespace ORM {

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-variable"

namespace tblWalletsBalanceHistory {
    constexpr char Name[] = "tblWalletsBalanceHistory";

    namespace Fields {
        TARGOMAN_CREATE_CONSTEXPR(wbl_wltID);
        TARGOMAN_CREATE_CONSTEXPR(wblBalance);
        TARGOMAN_CREATE_CONSTEXPR(wblSumIncome);
        TARGOMAN_CREATE_CONSTEXPR(wblSumExpense);
        TARGOMAN_CREATE_CONSTEXPR(wblSumCredit);
        TARGOMAN_CREATE_CONSTEXPR(wblSumDebit);
    }
}

namespace tblWalletsTransactions {
    constexpr char Name[] = "tblWalletsTransactions";

    namespace Fields {
        TARGOMAN_CREATE_CONSTEXPR(wltID);
        TARGOMAN_CREATE_CONSTEXPR(wlt_walID);
        TARGOMAN_CREATE_CONSTEXPR(wlt_vchID);
        TARGOMAN_CREATE_CONSTEXPR(wlt_vchType);
        TARGOMAN_CREATE_CONSTEXPR(wltAmount);
        TARGOMAN_CREATE_CONSTEXPR(wltStatus);
        TARGOMAN_CREATE_CONSTEXPR(wltDateTime);
    }

    namespace Relation {
//        constexpr char AAA[] = "aaa";
    }

    namespace Private {
        const QList<clsORMField> ORMFields = {
            ///< ColName                               Type                            Validation                  Default     UpBy    Sort   Filter Self  Virt   PK
                { Fields::wltID,        ORM_PRIMARYKEY_64 },
                { Fields::wlt_walID,    S(quint64),                     QFV.integer().minValue(1),  QRequired,  UPNone, true,  true },
                { Fields::wlt_vchID,    S(quint64),                     QFV.integer().minValue(1),  QRequired,  UPNone, true,  true },
                { Fields::wlt_vchType,  S(Targoman::API::AAA::enuVoucherType::Type), QFV, Targoman::API::AAA::enuVoucherType::Expense, UPNone },
                { Fields::wltAmount,    S(qint64),                      QFV,                        QInvalid,   UPNone, false, false },
                { Fields::wltStatus,    ORM_STATUS_FIELD(Targoman::API::AccountModule::enuWalletTransactionStatus, Targoman::API::AccountModule::enuWalletTransactionStatus::New) },
                { Fields::wltDateTime,  ORM_CREATED_ON },
            };

        const QList<stuRelation> Relations = {
            ///< Col                                   Reference Table                         ForeignCol                      Rename  LeftJoin
                { Fields::wlt_walID,    R(AAASchema, tblUserWallets::Name),     tblUserWallets::Fields::walID },
                { Fields::wlt_vchID,    R(AAASchema, tblVoucher::Name),         tblVoucher::Fields::vchID },
                { Fields::wltID,        R(AAASchema, tblWalletsBalanceHistory::Name),  tblWalletsBalanceHistory::Fields::wbl_wltID },
            };

        const QList<stuDBIndex> Indexes = {
        };

    } //namespace Private

    TAPI_DEFINE_VARIANT_ENABLED_STRUCT(DTO,
        SF_ORM_PRIMARYKEY_64        (wltID),
        SF_quint64                  (wlt_walID),
        SF_quint64                  (wlt_vchID),
        SF_Enum                     (wlt_vchType, Targoman::API::AAA::enuVoucherType, Targoman::API::AAA::enuVoucherType::Expense),
        SF_qint64                   (wltAmount),
        SF_ORM_STATUS_FIELD         (wltStatus, Targoman::API::AccountModule::enuWalletTransactionStatus, Targoman::API::AccountModule::enuWalletTransactionStatus::New),
        SF_ORM_CREATED_ON           (wltDateTime)
    );
}

namespace tblWalletsBalanceHistory {
    namespace Relation {
//        constexpr char AAA[] = "aaa";
    }

    namespace Private {
        const QList<clsORMField> ORMFields = {
            ///< ColName                           Type        Validation              Default     UpBy    Sort    Filter Self  Virt   PK
                { Fields::wbl_wltID,     S(qint64),  QFV,                    QRequired,  UPNone },
                { Fields::wblBalance,    S(qint64),  QFV.allwaysInvalid(),   QInvalid,   UPNone, false,  false },
                { Fields::wblSumDebit,   S(qint64),  QFV.allwaysInvalid(),   QInvalid,   UPNone, false,  false },
                { Fields::wblSumCredit,  S(qint64),  QFV.allwaysInvalid(),   QInvalid,   UPNone, false,  false },
                { Fields::wblSumIncome,  S(qint64),  QFV.allwaysInvalid(),   QInvalid,   UPNone, false,  false },
                { Fields::wblSumExpense, S(qint64),  QFV.allwaysInvalid(),   QInvalid,   UPNone, false,  false },
            };

        const QList<stuRelation> Relations = {
            ///< Col                               Reference Table                             ForeignCol                      Rename   LeftJoin
                { Fields::wbl_wltID,     R(AAASchema, tblWalletsTransactions::Name), tblWalletsTransactions::Fields::wltID },
            };

        const QList<stuDBIndex> Indexes = {
        };

    } //namespace Private

    TAPI_DEFINE_VARIANT_ENABLED_STRUCT(DTO,
        SF_qint64                   (wbl_wltID),
        SF_qint64                   (wblBalance),
        SF_qint64                   (wblSumDebit),
        SF_qint64                   (wblSumCredit),
        SF_qint64                   (wblSumIncome),
        SF_qint64                   (wblSumExpense)
    );
}
#pragma GCC diagnostic pop

class WalletTransactions : public intfSQLBasedModule
{
    Q_OBJECT
    TARGOMAN_DEFINE_API_SUBMODULE(Account, WalletTransactions)

private slots:
    QVariant ORMGET("Get WalletTransactions information.")
};

class WalletsBalanceHistory : public intfSQLBasedModule
{
    Q_OBJECT
    TARGOMAN_DEFINE_API_SUBMODULE(Account, WalletsBalanceHistory)

private:
};

} //namespace ORM
} //namespace Targoman::API::AccountModule

#endif // TARGOMAN_API_MODULES_ACCOUNT_ORM_WALLETTRANSACTIONS_H
