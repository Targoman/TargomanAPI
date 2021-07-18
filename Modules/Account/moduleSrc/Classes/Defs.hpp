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
 */

#ifndef TARGOMAN_API_MODULES_ACCOUNT_CLASSES_DEFS_HPP
#define TARGOMAN_API_MODULES_ACCOUNT_CLASSES_DEFS_HPP

#include "Interfaces/Common/APIArgHelperMacros.hpp"
#include "libTargomanCommon/Macros.h"

namespace TAPI {

TARGOMAN_DEFINE_ENUM(enuVoucherType,
                     Withdrawal     = 'W',
                     Expense        = 'E',
                     Income         = 'I',
                     Credit         = 'C',
                     Prize          = 'Z',
                     TransferTo     = 'T',
                     TransferFrom   = 'F',
                     )

TARGOMAN_DEFINE_ENUM(enuPaymentStatus,
                     New      = 'N',
                     Pending  = 'P',
                     Payed    = 'Y',
                     Succeded = 'A',
                     Error    = 'E',
                     Removed  = 'R'
                                )

TARGOMAN_DEFINE_ENUM(enuPaymentGateway,
                     Zibal    = 'Z',
                     ZarrinPal= 'L',
                     NextPay  = 'N',
                     Pardano  = 'O',
                     Parsian  = 'P',
                     Mellat   = 'M',
                     Pasargad = 'G',
                     Saman    = 'S',
                     AsanPardakht = 'A',
                     Gap      = 'W',
                     VISA     = 'V',
                     MasterCard= 'C',
                                )

}

TAPI_DECLARE_METATYPE_ENUM(TAPI::enuVoucherType);
TAPI_DECLARE_METATYPE_ENUM(TAPI::enuPaymentStatus);
TAPI_DECLARE_METATYPE_ENUM(TAPI::enuPaymentGateway);

#endif //TARGOMAN_API_MODULES_ACCOUNT_CLASSES_DEFS_HPP
