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

#include "gtwLuaOpenNMT.h"

namespace Targoman::API::ModuleHelpers::MT::Gateways {

TARGOMAN_API_MT_GATEWAY_IMPL(gtwLuaOpenNMT)

QVariantMap gtwLuaOpenNMT::doTranslation(
    INTFAPICALLBOOM_IMPL &APICALLBOOM_PARAM,
    const stuEngineSpecs& _engineSpecs,
    const QString& _text,
    bool _detailed,
    bool _detokenize
) {

    return {};
}

/*

    protected $en2faLoadBalancer = NULL;
    protected $fa2enLoadBalancer = NULL;
    protected $ar2faLoadBalancer = NULL;

    protected function finalizeTranslationResult($result){
        if(isset($result['serverID']))
            $Resp['serverID'] = $result['serverID'];
        else
            $Resp['serverID'] = "127.0.0.100";
        if (isset($result['data']) && isset($result['data']['t'])){
            if(isset($result['data']['serverID'])){
                $Resp['serverID'] = $result['data']['serverID'];
            }else

            if(is_array($result['data']['t'])){
                $Resp['tr']['base'] = $result['data']['t'][0];
                $Resp['tr']['phrases'] = $result['data']['t'][1];
                $Resp['tr']['alignments'] = $result['data']['t'][2];
            }else{
                $Resp['tr']['simple'] = $result['data']['t'];
            }
            $Resp['time'] = $result['time'];
            $Resp['tuid'] = $result['tuid'];
        }else{
            $Resp = $result;
            if(isset($result['data']['serverID'])){
                $Resp['serverID'] = $result['data']['serverID'];
                unset($Resp['data']['serverID']);
            }
        }
        return  $Resp;
    }

    public function translate($dir, $sourceText, $clientIP, $tuid, $detailed = false, $defaultClass= null)
    {
        $Retry=0;

        $time_start = microtime(true);

        if($this->getPreferedServerAndTUID($tuid, $dir, 'NMT', $sourceText, $clientIP) == -1)
            $Resp["tuid"] = $this->getTUID('NMT', $dir, $clientIP, $sourceText, 1);
        else
            $Resp["tuid"] = $tuid;

        if($dir=="en2fa")
            $CurrServer=$this->en2faLoadBalancer;
        elseif($dir=="ar2fa")
            $CurrServer=$this->ar2faLoadBalancer;
        else
            $CurrServer=$this->fa2enLoadBalancer;

        while ($Retry < 5){
            $ch = curl_init();


            curl_setopt($ch, CURLOPT_URL, $CurrServer);
            curl_setopt($ch, CURLOPT_HTTPHEADER, array("Content-Type: application/json"));
            curl_setopt($ch, CURLOPT_RETURNTRANSFER, 1);
            curl_setopt($ch, CURLOPT_TIMEOUT, 10);
            curl_setopt($ch, CURLOPT_POST, 1);
            curl_setopt($ch, CURLOPT_POSTFIELDS, json_encode([array("src"=>$sourceText)]));

            $Data = curl_exec($ch);
            $Resp["time"]=((microtime(true) - $time_start));
            $Resp['serverID'] = 0;

            if (curl_errno($ch) == 7) {
                $Retry = $Retry + 1;
                continue;
            }else if(curl_errno($ch)){
                $Resp["message"]  = curl_error($ch);
                $Resp['errno'] = "-100";
            }else {
                $JSONStr = clean($Data);
                $JSON = (array)json_decode(str_replace(array("\t","\r","\n"),array("\\t","\\r","\\n"),$JSONStr));
                switch (json_last_error()) {
                    case JSON_ERROR_NONE:
                        if ($detailed)
                            $Resp["data"] = $JSON;
                        else{
                            $Text="";
                            foreach($JSON["t"][0] as $Par)
                                $Text.= $Par[0]."\n";
                            $Resp["data"] = array('t' => $Text, 'serverID' => isset($JSON["serverID"]) ? $JSON["serverID"] : NULL);
                        }
                        $Resp['errno'] = "0";

                        $Resp = $this->finalizeTranslationResult($Resp);

                        if(isset($Resp['tr']) && isset($Resp['tr']['base'])){
                            for($i=0;$i<count($Resp['tr']['base']); ++$i){
                                $Temp = $Resp['tr']['base'][$i][0];
                                $Resp['tr']['base'][$i][0] = $Resp['tr']['base'][$i][1];
                                $Resp['tr']['base'][$i][1] = $Temp;
                            }
                        }

                        return $Resp;
                    break;
                    case JSON_ERROR_DEPTH:
                        $errString = 'Maximum stack depth exceeded';
                    break;
                    case JSON_ERROR_STATE_MISMATCH:
                        $errString = 'Underflow or the modes mismatch';
                    break;
                    case JSON_ERROR_CTRL_CHAR:
                        $errString = 'Unexpected control character found';
                    break;
                    case JSON_ERROR_SYNTAX:
                        $errString = 'Syntax error, malformed JSON';
                    break;
                    case JSON_ERROR_UTF8:
                        $errString = 'Malformed UTF-8 characters, possibly incorrectly encoded';
                    break;
                    default:
                        $errString = 'Unknown Error';
                }
                $Resp["message"]  = "Unable to parse JSON: $errString: {".str_replace("\t","\\t",$JSONStr)."}";
                $Resp['errno'] = -10;
                $Retry = $Retry + 1;
                continue;
            }
        }
        $Resp['errno'] = -20;
        $Resp["message"] = "Unable to translate with NMT after 5 retries.";

        return $Resp;
    }


*/

} // namespace Targoman::API::ModuleHelpers::MT::Gateways
