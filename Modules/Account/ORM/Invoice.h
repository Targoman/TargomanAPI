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
 @author S. Mehran M. Ziabary <ziabary@targoman.com>
 */

#ifndef TARGOMAN_API_MODULES_ACCOUNT_ORM_INVOICE_H
#define TARGOMAN_API_MODULES_ACCOUNT_ORM_INVOICE_H

#include "Interfaces/ORM/clsTable.h"
#include "Interfaces/AAA/AAA.hpp"

namespace TAPI {
TARGOMAN_DEFINE_ENUM(enuInvoiceStatus,
                     New      = 'N',
                     Canceled = 'C',
                     Finished = 'F',
                     Removed  = 'R'
                                )

TARGOMAN_DEFINE_ENUM(enuInvoiceType,
                     Payment    = 'P',
                     Withdrawal = 'W'
                                  )
}

namespace Targoman {
namespace API {
namespace AAA {

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-variable"
namespace tblInvoice {
constexpr char Name[] = "tblInvoice";
TARGOMAN_CREATE_CONSTEXPR(invID);
TARGOMAN_CREATE_CONSTEXPR(invCreationDateTime);
TARGOMAN_CREATE_CONSTEXPR(inv_svcID);
TARGOMAN_CREATE_CONSTEXPR(inv_ordID);
TARGOMAN_CREATE_CONSTEXPR(inv_usrID);
TARGOMAN_CREATE_CONSTEXPR(invDesc);
TARGOMAN_CREATE_CONSTEXPR(invPaymentType);
TARGOMAN_CREATE_CONSTEXPR(invTotalAmount);
TARGOMAN_CREATE_CONSTEXPR(invStatus);
}
#pragma GCC diagnostic pop

class Invoice : public ORM::clsTable
{
    Q_OBJECT
private slots:
    QVariant ORMGET("Get Invoice information")
    bool ORMDELETE("Delete an Invoice. Take note that User can just delete invoices with Payoff type")
    quint64 REST(CREATE,withdraw,(TAPI::JWT_t _JWT,
                                 quint64 _amount,
                                 quint64 _walletID),
                "Create a new payback request by user.")

    private:
        TARGOMAN_DEFINE_API_SUBMODULE(Account,Invoice)
};

}
}
}

TAPI_DECLARE_METATYPE(TAPI::enuInvoiceStatus::Type);
TAPI_DECLARE_METATYPE(TAPI::enuInvoiceType::Type);

#endif // TARGOMAN_API_MODULES_ACCOUNT_ORM_INVOICE_H
