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

#include "PhoneHelper.h"
#include <string>
#include "libTargomanCommon/exTargomanBase.h"

namespace Targoman::API::Helpers {

QString PhoneHelper::GetErrorTypeAsString(const i18n::phonenumbers::PhoneNumberUtil::ErrorType& _errorType) {
    switch (_errorType) {
        case i18n::phonenumbers::PhoneNumberUtil::ErrorType::INVALID_COUNTRY_CODE_ERROR:
            return "Invalid Country Code";
        case i18n::phonenumbers::PhoneNumberUtil::ErrorType::NOT_A_NUMBER:
            return "Not A Number";
        case i18n::phonenumbers::PhoneNumberUtil::ErrorType::TOO_SHORT_AFTER_IDD:
            return "Too Short After IDD";
        case i18n::phonenumbers::PhoneNumberUtil::ErrorType::TOO_SHORT_NSN:
            return "Too Short NSN";
        case i18n::phonenumbers::PhoneNumberUtil::ErrorType::TOO_LONG_NSN:
            return "Too Long NSN";
        default:
            return "Unknown Error";
    }
}

QString PhoneHelper::NormalizePhoneNumber(QString _number, const QString& _country) {
    i18n::phonenumbers::PhoneNumberUtil* phoneNumberUtil = i18n::phonenumbers::PhoneNumberUtil::GetInstance();

    i18n::phonenumbers::PhoneNumber number_proto;
    i18n::phonenumbers::PhoneNumberUtil::ErrorType ret = phoneNumberUtil->Parse(_number.toStdString(), _country.toStdString(), &number_proto);
    if (ret != i18n::phonenumbers::PhoneNumberUtil::ErrorType::NO_PARSING_ERROR)
        throw Targoman::Common::exTargomanBase(QString("Error in parsing phone number: %1").arg(GetErrorTypeAsString(ret)));

    std::string formatted_number;
    phoneNumberUtil->Format(number_proto, i18n::phonenumbers::PhoneNumberUtil::PhoneNumberFormat::E164, &formatted_number);
    return formatted_number.c_str();
}

} //namespace Targoman::API::Helpers
