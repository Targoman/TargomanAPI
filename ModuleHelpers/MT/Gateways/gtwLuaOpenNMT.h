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

#ifndef TARGOMAN_API_MODULEHELPERS_MT_ENGINES_LUAOPENNMT_H
#define TARGOMAN_API_MODULEHELPERS_MT_ENGINES_LUAOPENNMT_H

#include "intfTranslatorGateway.hpp"
#include "../Classes/TranslationDispatcher.h"

namespace Targoman::API::ModuleHelpers::MT::Gateways {

/**
 * CAUTION:
 * place #include this header file in ActiveGateways.h for proper driver registration
 */

class gtwLuaOpenNMT : public intfTranslatorGateway
{
public:
    constexpr static char Name[] = "LuaOpenNMT";

    TARGOMAN_API_MT_GATEWAY_DEFINE(gtwLuaOpenNMT)

public:
    QVariantMap doTranslation(const stuEngineSpecs& _engineSpecs, const QString& _text, bool _detailed, bool _detokenize);
};

} //namespace Targoman::API::ModuleHelpers::MT::Engines

#endif // TARGOMAN_API_MODULEHELPERS_MT_ENGINES_LUAOPENNMT_H
