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

#ifndef TARGOMAN_API_MODULEHELPERS_MT_ENGINES_MARIANTRANSBPE_V1_7_H
#define TARGOMAN_API_MODULEHELPERS_MT_ENGINES_MARIANTRANSBPE_V1_7_H

#include "gtwMarianTransBPE.v1.6.h"
#include "../MTHelper.h"

namespace Targoman::API::ModuleHelpers::MT::Gateways {

/**
 * CAUTION:
 * place #include this header file in ActiveGateways.h for proper driver registration
 */

class gtwMarianTransBPE_v1_7 : public gtwMarianTransBPE_v1_6
{
public:
    constexpr static char Name[] = "MarianTransBPE_v1_7";

    TARGOMAN_API_MT_GATEWAY_DEFINE(gtwMarianTransBPE_v1_7)



/*
    protected function preprocessText($sourceText) {
//      dumpVar(array("ntfMarianTransBPE_v1_7::preprocessText"));
      $sourceText = preg_replace('/(_ _ )+/', '_ _ ', $sourceText);
      return $sourceText;
    }
*/

};

} //namespace Targoman::API::ModuleHelpers::MT::Engines

#endif // TARGOMAN_API_MODULEHELPERS_MT_ENGINES_MARIANTRANSBPE_V1_7_H
