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

#ifndef TARGOMAN_API_MODULES_ACCOUNT_PGTW_DEVNULL_H
#define TARGOMAN_API_MODULES_ACCOUNT_PGTW_DEVNULL_H

#include "intfPaymentGateway.h"
#include "Classes/PaymentLogic.h"

/**
 * CAUTION:
 * place #include "PaymentGateways/gtwDevTest.h" in PaymentLogic.cpp for proper driver registration
 */

namespace Targoman::API::AAA {

/**
 * @brief *********** THE gtwDevTest CLASS IS JUST FOR DEVELOPERS IN DEBUG MODE. NOT FOR PRODUCTION ***********
 */
class gtwDevTest : public intfPaymentGateway
{
    TARGOMAN_DEFINE_API_PAYMENT_GATEWAY(
        TAPI::enuPaymentGatewayType::_DeveloperTest,
        gtwDevTest)

public:
    constexpr static char Name[] = "DevTest";

private:
    constexpr static char URL_GTW_PAY[] = "https://devtest.dom/pay/{{track_id}}";
};

} //namespace Targoman::API::AAA

#endif // TARGOMAN_API_MODULES_ACCOUNT_PGTW_DEVNULL_H
