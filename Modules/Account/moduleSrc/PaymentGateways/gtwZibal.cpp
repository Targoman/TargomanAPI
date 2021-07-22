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

//#include "ORM/PaymentGateways.h"
//#include "Interfaces/ORM/APIQueryBuilders.h"

#include "gtwZibal.h"
#include "Classes/PaymentLogic.h"

namespace Targoman::API::AAA {

TARGOMAN_IMPL_API_PAYMENT_GATEWAY(gtwZibal)

stuPaymentResponse gtwZibal::request(TAPI::MD5_t _orderMD5, qint64 _amount, const QString& _callback, const QString& _desc)
{
}

stuPaymentResponse gtwZibal::verify(const TAPI::JSON_t& _pgResponse, const QString& _domain)
{

}

} //namespace Targoman::API::AAA
