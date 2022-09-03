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
 * @author Kambiz Zandi <kambizzandi@gmail.com>
 */

#include "clsEngine.h"
#include "../Gateways/intfTranslatorGateway.hpp"
#include "MTHelper.h"

namespace Targoman::API::ModuleHelpers::MT::Classes {

using namespace Gateways;

QVariantMap clsEngine::doTranslation(
    INTFAPICALLBOOM_IMPL &APICALLBOOM_PARAM,
    const QString& _text,
    bool _detailed,
    bool _detokinize
) {
    intfTranslatorGateway* Gateway = MTHelper::instance().getGateway(this->EngineSpecs.DriverName);

    return Gateway->doTranslation(APICALLBOOM_PARAM,
                                  this->EngineSpecs,
                                  _text,
                                  _detailed,
                                  _detokinize);
}

} //namespace Targoman::API::ModuleHelpers::MT::Classes
