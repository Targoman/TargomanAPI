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

#ifndef TARGOMAN_API_MODULES_ACCOUNT_CLASSES_PAYMENTLOGIN_H
#define TARGOMAN_API_MODULES_ACCOUNT_CLASSES_PAYMENTLOGIN_H

#include "ORM/Payments.h"
#include "ORM/Voucher.h"
#include "ORM/UserWallets.h"
#include "ORM/WalletTransactions.h"

#include "Defs.hpp"

#include "PaymentGateways/intfPaymentGateway.h"
#include "ORM/PaymentGateways.h"

typedef intfPaymentGateway* (*PAYMENTGATEWAY_INSTANCE_FUNC)();
#define PAYMENTGATEWAY_REGISTERED_DRIVERS_PAIR_TYPES QString,PAYMENTGATEWAY_INSTANCE_FUNC

namespace Targoman::API::AAA {

/**
 * @brief The PaymentLogic class is central point of managing payments. Such as start payment, handling payment callback, ...
 */
class PaymentLogic
{
public:
    template <class TPaymentGatewayClass>
    static void registerDriver(const QString& _driverName, TPaymentGatewayClass* (*_instanceFunc)());
    static intfPaymentGateway* getDriver(const QString& _driverName);

    static const stuPaymentGateway findBestPaymentGateway(
        quint32 _amount,
        TAPI::enuPaymentGatewayType::Type _gatewayType
    );

    static QString createOnlinePaymentLink(
        TAPI::enuPaymentGatewayType::Type _gatewayType,
        quint64 _vchID,
        const QString& _invDesc,
        quint32 _toPay,
        const QString _paymentVerifyCallback,
        /*OUT*/ TAPI::MD5_t& _outPaymentMD5
    );

    static quint64 approveOnlinePayment(
        const QString& _paymentMD5,
        const TAPI::JSON_t& _pgResponse,
        const QString& _domain
    );

public:
    static Targoman::Common::Configuration::tmplConfigurable<FilePath_t> TransactionLogFile;
    static void log(const QString _gw, const QString _function, quint16 _line, const QVariantList& _info) {
        if (PaymentLogic::TransactionLogFile.value().isEmpty())
            return;

        QFile File(PaymentLogic::TransactionLogFile.value()); // TODO read from config file

        if (File.open(QFile::WriteOnly | QFile::Append)) {
            QTextStream Out(&File);
            Out << QDateTime::currentDateTime().toString(Qt::ISODateWithMs)
                << QString("[%1:%2:%3] ").arg(_gw, _function).arg(_line)
                << QJsonDocument::fromVariant(_info).toJson();
        }
    }

protected:
    /**
     * @brief RegisteredPaymentGateways stores one instance of payment gateways classes
     * key: QString Driver Name
     */
    static inline QMap<PAYMENTGATEWAY_REGISTERED_DRIVERS_PAIR_TYPES> RegisteredDrivers;
};

} //namespace Targoman::API::AAA

#endif // TARGOMAN_API_MODULES_ACCOUNT_CLASSES_PAYMENTLOGIN_H
