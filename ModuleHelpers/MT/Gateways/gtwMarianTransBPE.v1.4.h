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

#ifndef TARGOMAN_API_MODULEHELPERS_MT_GATEWAYS_MARIANTRANSBPE_V1_4_H
#define TARGOMAN_API_MODULEHELPERS_MT_GATEWAYS_MARIANTRANSBPE_V1_4_H

#include "intfBaseNMTGateway.h"
#include "../Classes/TranslationDispatcher.h"

namespace Targoman::API::ModuleHelpers::MT::Gateways {

/**
 * CAUTION:
 * place #include this header file in ActiveGateways.h for proper driver registration
 */

class gtwMarianTransBPE_v1_4 : public intfBaseNMTGateway
{
public:
    constexpr static char Name[] = "MarianTransBPE_v1_4";

    TARGOMAN_API_MT_GATEWAY_DEFINE(gtwMarianTransBPE_v1_4)



/*
    gtwMarianTransBPE_v1_4(const Classes::stuEngineSpecs& _specs) :
        intfBaseNMTGateway(_specs)
    { ; }

    protected function preprocessText($sourceText) {
      $sourceText = preg_replace('/(_ _ )+/', '_ _ ', $sourceText);
      $sourceText = preg_replace('/ \. /', " .\n", $sourceText);
      return $sourceText;
    }

    protected function maxChars() {return 700;}
    protected function maxCharPerWord() {return 30;}

    protected function makeSrcSentences($sourceText) {
        $makeSrc = function($phrase) {
            return ["src"=>$phrase];
        };
        
        return array_slice(array_map($makeSrc, explode("\n", $sourceText)), 0, -1);
    }
*/

};

} //namespace Targoman::API::ModuleHelpers::MT::Gateways

#endif // TARGOMAN_API_MODULEHELPERS_MT_GATEWAYS_MARIANTRANSBPE_V1_4_H
