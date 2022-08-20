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

#ifndef TARGOMAN_API_MODULEHELPERS_MT_ENGINES_MARIANTRANSBPE_V1_6_H
#define TARGOMAN_API_MODULEHELPERS_MT_ENGINES_MARIANTRANSBPE_V1_6_H

#include "intfBaseNMTGateway.h"
#include "../Classes/TranslationDispatcher.h"

namespace Targoman::API::ModuleHelpers::MT::Gateways {

/**
 * CAUTION:
 * place #include this header file in ActiveGateways.h for proper driver registration
 */

class gtwMarianTransBPE_v1_6 : public intfBaseNMTGateway
{
public:
    constexpr static char Name[] = "MarianTransBPE_v1_6";

    TARGOMAN_API_MT_GATEWAY_DEFINE(gtwMarianTransBPE_v1_6)



/*
    protected function preprocessText($sourceText) {
//      dumpVar(array("ntfMarianTransBPE_v1_6::preprocessText"));
      $sourceText = preg_replace('/(_ _ )+/', '_ _ ', $sourceText);
      $sourceText = preg_replace('/ \. /', " .\n", $sourceText);
      return $sourceText;
    }

    protected function maxChars() {return 700;}
    protected function maxCharPerWord() {return 30;}

    protected function makeSrcSentences($sourceText) {
        dumpVar(array(
            "sourceText" => $sourceText
        ));
        $makeSrc = function($phrase) {
            return ["src"=>$phrase];
        };
        
        return array_map($makeSrc, explode("\n", $sourceText));
    }
    
    protected function buildProperResponse($resp, $json, $detailed, $dir) {
        if(is_object($json))
            $json = get_object_vars($json);
        if (isset($json['rslt'])) {
            $get_base_phrase = function($phrases) {
                return $phrases[0];
            };
            $concat_base_phrases = function($item) use($get_base_phrase, $dir) {
                if(is_object($item))
                    $item = get_object_vars($item);
                return $this->detokenize(implode(' ', array_map($get_base_phrase, $item['phrases'])),$dir);
            };
            if ($detailed) {
                $resp['tr'] = array(
                    'base' => [],
                    'phrases' => [],
                    'alignments' => []
                );
                $SentenceIdx=0;
                foreach($json['rslt'] as $Item) {
                    if(is_object($Item))
                        $Item = get_object_vars($Item);
                    $resp['tr']['base'][] = array($concat_base_phrases($Item), implode(' ', $Item['tokens']));
                    $Index = 0;
                    $resp['tr']['phrases'][$SentenceIdx] = [];
                    foreach($Item['phrases'] as $Phrase)
                        $resp['tr']['phrases'][$SentenceIdx][] = array($Phrase[0], $Index++);
                    $Index = 0;
                    $resp['tr']['alignments'][$SentenceIdx] = [];
                    foreach($Item['alignments'] as $Alignment) {
                        $resp['tr']['alignments'][$SentenceIdx][] = array(
                            implode(' ', array_map(function($i) use($Item) { return $Item['tokens'][$i]; }, $Alignment)),
                            $Alignment[0]+1,
                            $this->buildAlignments($Item['phrases'][$Index])
                        );
                        $Index++;
                    }
                    $SentenceIdx++;
                }
            } else {
                $resp['tr'] = array(
                    'simple' => implode("\n", array_map($concat_base_phrases, $json['rslt']))
                );
            }
            $resp['serverID'] = isset($json["serverName"]) ? $json["serverName"] : NULL;
        } else {
            if ($detailed)
                $resp["data"] = $json;
            else{
                $Text="";
                foreach($json["t"][0] as $Par)
                    $Text.= $Par[0]."\n";
                $resp["data"] = array('t' => $Text, 'serverID' => isset($json["serverName"]) ? $json["serverName"] : NULL);
            }
            $resp = $this->finalizeTranslationResult($resp);
        }
        $resp['errno'] = "0";
        return $resp;
    }
*/

};

} //namespace Targoman::API::ModuleHelpers::MT::Engines

#endif // TARGOMAN_API_MODULEHELPERS_MT_ENGINES_MARIANTRANSBPE_V1_6_H
