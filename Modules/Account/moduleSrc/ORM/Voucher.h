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

#ifndef TARGOMAN_API_MODULES_ACCOUNT_ORM_VOUCHER_H
#define TARGOMAN_API_MODULES_ACCOUNT_ORM_VOUCHER_H

#include "Classes/Defs.hpp"
#include "Interfaces/AAA/AAA.hpp"
#include "Interfaces/API/intfSQLBasedModule.h"

namespace Targoman::API::AccountModule {

//enumes goes here

TARGOMAN_DEFINE_ENUM(enuVoucherType,
                     Withdrawal     = 'W',
                     Expense        = 'E',
                     Income         = 'I',
                     Credit         = 'C',
                     Prize          = 'Z',
                     TransferFrom   = 'F',
                     TransferTo     = 'T',
                     )

} //namespace Targoman::API::AccountModule

TAPI_DECLARE_METATYPE_ENUM(Targoman::API::AccountModule, enuVoucherType);

namespace Targoman::API::AccountModule {

//structures goes here

namespace ORM {

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-variable"

namespace tblVoucher {
    constexpr char Name[] = "tblVoucher";

    TARGOMAN_CREATE_CONSTEXPR(vchID);
    TARGOMAN_CREATE_CONSTEXPR(vch_usrID);
    TARGOMAN_CREATE_CONSTEXPR(vchDesc);
    TARGOMAN_CREATE_CONSTEXPR(vchType);
    TARGOMAN_CREATE_CONSTEXPR(vchTotalAmount);
    TARGOMAN_CREATE_CONSTEXPR(vchProcessResult);
    TARGOMAN_CREATE_CONSTEXPR(vchStatus);
    TARGOMAN_CREATE_CONSTEXPR(vchCreationDateTime);

    inline QStringList ColumnNames(QString _tableAlias = "") {
        if (_tableAlias.isEmpty() == false)
            _tableAlias += ".";
        return {
            _tableAlias + vchID,
            _tableAlias + vch_usrID,
            _tableAlias + vchDesc,
            _tableAlias + vchType,
            _tableAlias + vchTotalAmount,
            _tableAlias + vchProcessResult,
            _tableAlias + vchStatus,
            _tableAlias + vchCreationDateTime,
        };
    }

    TAPI_DEFINE_VARIANT_ENABLED_STRUCT(DTO,
        SF_quint64(vchID),
        SF_quint64(vch_usrID),
        SF_JSON_t(vchDesc),
        SF_NULLABLE_Enum(Targoman::API::AccountModule::enuVoucherType, vchType),
        SF_quint64(vchTotalAmount),
        SF_JSON_t(vchProcessResult),
        SF_Enum(Targoman::API::AAA::enuVoucherStatus, vchStatus, Targoman::API::AAA::enuVoucherStatus::New),
        SF_DateTime_t(vchCreationDateTime)
    );
}
#pragma GCC diagnostic pop

class Voucher : public intfSQLBasedModule
{
    Q_OBJECT
    TARGOMAN_DEFINE_API_SUBMODULE(Account, Voucher)

private slots:
    QVariant ORMGET("Get Voucher information")
    bool ORMDELETE("Delete a Voucher. Take note that User can just delete Vouchers with Payoff type")
};

} //namespace ORM
} //namespace Targoman::API::AccountModule

#endif // TARGOMAN_API_MODULES_ACCOUNT_ORM_VOUCHER_H
