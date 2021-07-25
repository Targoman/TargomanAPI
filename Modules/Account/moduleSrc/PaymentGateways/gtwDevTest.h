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

#include <qglobal.h>
#include "intfPaymentGateway.h"
#include "Classes/PaymentLogic.h"

namespace Targoman::API::AAA {

/**
 * @brief *********** THE gtwDevTest CLASS IS JUST FOR DEVELOPERS IN DEBUG MODE. NOT FOR PRODUCTION ***********
 */
class gtwDevTest : public intfPaymentGateway
{
public:
    constexpr static char Name[] = "DevTest";

TARGOMAN_DEFINE_API_PAYMENT_GATEWAY(
    TAPI::enuPaymentGatewayType::DevelopersTest,
    gtwDevTest)
};

} //namespace Targoman::API::AAA

#endif // TARGOMAN_API_MODULES_ACCOUNT_PGTW_DEVNULL_H
