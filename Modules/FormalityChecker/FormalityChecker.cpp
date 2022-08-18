/******************************************************************************
#   TargomanAPI: REST API for Targoman
#
#   Copyright 2014-2019 by Targoman Intelligent Processing <http://tip.co.ir>
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

#include "FormalityChecker.h"
#include "ModuleHelpers/NLP/FormalityChecker.h"

namespace Targoman {
namespace API {

using namespace Targoman::API::ModuleHelpers;

TARGOMAN_API_MODULE_IMPLEMENT(FormalityChecker)

FormalityChecker::FormalityChecker() :
    intfPureModule("FormalityChecker")
{ ; }

bool FormalityChecker::init() {
    NLP::FormalityChecker::instance();

    return true;
}

QString IMPL_REST_GET_OR_POST(FormalityChecker, check, (
    APICALLBOOM_TYPE_NO_JWT_IMPL &APICALLBOOM_PARAM,
    const QString _text,
    const TAPI::ISO639_2_t& _lang
)) {
    return NLP::FormalityChecker::instance().check(_lang, _text);
}

}
}

