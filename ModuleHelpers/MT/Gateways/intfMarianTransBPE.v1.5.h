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

#ifndef TARGOMAN_API_MODULEHELPERS_MT_ENGINES_MARIANTRANSBPE_V1_5_H
#define TARGOMAN_API_MODULEHELPERS_MT_ENGINES_MARIANTRANSBPE_V1_5_H

#include "clsBaseNMT.h"

namespace Targoman::API::ModuleHelpers::MT::Engines {

class intfMarianTransBPE_v1_5  extends intfBaseNMT {

    public function __construct($class, $loadBalancer){
        intfBaseNMT::__construct($class, $loadBalancer);
    }

    protected function preprocessText($sourceText) {
        return preg_replace("/(\S)('(?:s|ll|d|t))/", "$1 $2", $sourceText);
    }
    
    protected function makeSrcSentences($sourceText) {
        $srcSentences=[];
        
        foreach(explode("\n", $sourceText) as $par){
          $srcSentences[]=["src"=>"^^^PAR^^^"];
          foreach(explode(" .", $par) as $snt)
            $srcSentences[]=["src"=>$snt.' .'];
        }
        
        return $srcSentences;
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
            $str='';
            $startOfPar = false;
            if ($detailed) {
                $resp['tr'] = array(
                    'base' => [],
                    'phrases' => [],
                    'alignments' => []
                );
                $SentenceIdx=0;
                $tempObj = array(
                    'base' => [],
                    'phrases' => [],
                    'alignments' => []
                );
                
                echo json_encode($json['rslt']);
                foreach($json['rslt'] as $Item) {
                    if(is_object($Item))
                        $Item = get_object_vars($Item);
                    
                    if(trim($Item['tokens']['0']) === '.' && $startOfPar) 
                      continue;
                      
                    //print_r(json_encode($tempObj['base']));

                    if($Item['tokens']['0'] === '^^^PAR^^^'){
                      $startOfPar = true;
                      if(sizeof($tempObj['base']) > 0) {
                        $resp['tr']['base'][] = $tempObj['base'];
                        $resp['tr']['phrases'][] = $tempObj['base'];
                        $resp['tr']['base'] = $tempObj['base'];
                      }
                      
                      continue;
                    }
                    
                    $tempObj['base'][] = array($concat_base_phrases($Item), implode(' ', $Item['tokens']));
                    $Index = 0;
                    $tempObj['phrases'][$SentenceIdx] = [];
                    foreach($Item['phrases'] as $Phrase)
                        $tempObj['phrases'][$SentenceIdx][] = array($Phrase[0], $Index++);
                    $Index = 0;
                    $tempObj['alignments'][$SentenceIdx] = [];
                    foreach($Item['alignments'] as $Alignment) {
                        $tempObj['alignments'][$SentenceIdx][] = array(
                            implode(' ', array_map(function($i) use($Item) { return $Item['tokens'][$i]; }, $Alignment)),
                            $Alignment[0]+1,
                            $this->buildAlignments($Item['phrases'][$Index])
                        );
                        $Index++;
                    }
                    $SentenceIdx++;
                }
            } else {
                foreach($json['rslt'] as $Item) {
                    if(is_object($Item))
                        $Item = get_object_vars($Item);
                    if(trim($Item['tokens']['0']) === '.' && $startOfPar)    continue;
                    
                    if($Item['tokens']['0'] === '^^^PAR^^^'){
                      if($str !== '') 
                        $str.='\n';
                      $startOfPar = true;
                      continue;
                    }
                    $str .= implode(' ', array_map($get_base_phrase, $Item['phrases'])).' ';
                }
                
                $resp['tr'] = ['simple' => trim($this->detokenize($str,$dir))];
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
    
}

} //namespace Targoman::API::ModuleHelpers::MT::Engines

#endif // TARGOMAN_API_MODULEHELPERS_MT_ENGINES_MARIANTRANSBPE_V1_5_H
