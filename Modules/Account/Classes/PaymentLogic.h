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

#ifndef TARGOMAN_API_MODULES_ACCOUNT_CLASSES_PAYMENTLOGIN_H
#define TARGOMAN_API_MODULES_ACCOUNT_CLASSES_PAYMENTLOGIN_H

#include "ORM/Payments.h"
#include "ORM/Voucher.h"
#include "ORM/UserWallets.h"
#include "ORM/WalletTransactions.h"

#include "Defs.hpp"

namespace Targoman {
namespace API {
namespace AAA {

class PaymentLogic {
public:
    static QString createOnlinePaymentLink(TAPI::enuPaymentGateway::Type _gateway, quint64 _invID, const QString& _invDesc, quint32 _toPay, const QString _callback);
    static quint64 approveOnlinePayment(TAPI::enuPaymentGateway::Type _gateway, const QJsonObject& _pgResponse, const QString& _domain);
    static TAPI::stuVoucher processVoucher(quint64 _VoucherID);
};

}
}
}


#endif // TARGOMAN_API_MODULES_ACCOUNT_CLASSES_PAYMENTLOGIN_H
