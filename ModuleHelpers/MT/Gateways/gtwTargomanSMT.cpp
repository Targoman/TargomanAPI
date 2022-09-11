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

#include "gtwTargomanSMT.h"

namespace Targoman::API::ModuleHelpers::MT::Gateways {

TARGOMAN_API_MT_GATEWAY_IMPL(gtwTargomanSMT)

QVariantMap gtwTargomanSMT::doTranslation(
    INTFAPICALLBOOM_IMPL &APICALLBOOM_PARAM,
    const stuEngineSpecs& _engineSpecs,
    const QString& _text,
    bool _detailed,
    bool _detokenize
) {

    return {};
}


/*

    public function __construct($requiredDBs = 'base', $requiredSingleShema = 'MT'){
        clsBaseTranslation::__construct($requiredDBs, $requiredSingleShema);
    }

    protected $en2faServers = NULL;
    protected $fa2enServers = NULL;

    public function translate($dir, $sourceText, $clientIP, $tuid, $detailed = false, $defaultClass= null)
    {
        if($dir=="en2fa")
            $CurrServers=$this->en2faServers;
        else
            $CurrServers=$this->fa2enServers;

        $Retry=0;
        $LastServer = 0;
        $LastServerFile = __DIR__."/../../temp/LastServer.".$dir.".txt";

        $header[] = "TE: deflate,gzip;q=0.3";
        $header[] = "Connection: TE, close";
        $header[] = "Accept: text/xml";
        $header[] = "Accept: multipart/*";
        $header[] = "Accept: application/soa";
        $header[] = "User-Agent: HLTLab/XMLRPC/0.9.2";
        $header[] = "Content-type: text/xml";

        $request = '<?xml version="1.0" encoding="UTF-8"?>
                    <methodCall><methodName>translate</methodName>
                    <params><param><value><struct>
                    <member><name>align</name><value><string>false</string></value></member>
                    <member><name>text</name>
                    <value><string> '.str_replace(array("&", "<", ">", "\"", "'", "\\"),
                        array("&amp;", "&lt;", "&gt;", "&quot;", "&apos;", " "), $sourceText).'</string></value>
                    </member></struct></value></param></params></methodCall>';

        $header[] = "Content-length: ".strlen($request);
        $time_start = microtime(true);
        $PreferdServer = $this->getPreferedServerAndTUID($tuid, $dir, 'SMT', $sourceText, $clientIP);

        while ($Retry < 5) {
            if ($PreferdServer == -1){
                if (file_exists($LastServerFile)) {
                    $LastServer = file_get_contents($LastServerFile);
                    if ($LastServer + 1 >= count($CurrServers))
                        $LastServer = 0;
                    else
                        $LastServer = $LastServer + 1;
                }

                file_put_contents($LastServerFile, $LastServer);
                $Resp["tuid"]=$this->getTUID('SMT', $dir, $clientIP, $sourceText, $LastServer);
            }else{
                $LastServer = $PreferdServer;
                $Resp["tuid"]=$tuid;
            }

            $ch = curl_init();
            curl_setopt($ch, CURLOPT_URL, $CurrServers[$LastServer]);
            curl_setopt($ch, CURLOPT_RETURNTRANSFER, 1);
            curl_setopt($ch, CURLOPT_TIMEOUT, 5);
            curl_setopt($ch, CURLOPT_CONNECTTIMEOUT, 5);
            curl_setopt($ch, CURLOPT_HTTPHEADER, $header);
            curl_setopt($ch, CURLOPT_POSTFIELDS, $request);

            $Data = curl_exec($ch);
            $Resp["TrTime"]=((microtime(true) - $time_start));
            $Resp["serverID"] = preg_replace(
                "/http\:\/\/(.*)\:.*" . "/",
                "\1",
                $CurrServers[$LastServer]) ;

            if (curl_errno($ch) == 7) {
                $Retry = $Retry + 1;
                continue;
            }else if(curl_errno($ch)){
                $Resp["message"]  = curl_error($ch);
                $Resp['errno'] = "-100";
            }else {
                preg_match('#<string>(.+?)</string>#is', $Data, $JSONStr);
                $JSON = (array)json_decode("{".str_replace(array("\t","\r","\n"),array("\\t","\\r","\\n"),$JSONStr[1])."}");
                switch (json_last_error()) {
                    case JSON_ERROR_NONE:
                        if ($detailed)
                            $Resp["data"] = $JSON;
                        else{
                            $Text="";
                            foreach($JSON["t"][0] as $Par)
                                $Text.= $Par[1]."\n";
                            $Resp["data"] = array('t' => $Text);
                        }
                        $Resp = $this->finalizeTranslationResult($Resp);

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
                $Resp["message"]  = "\n\n\nUnable to parse JSON: $errString: {".str_replace("\t","\\t",$JSONStr[1])."}\n";
                $Resp['errno'] = -10;
                return $Resp;
            }
        }
        $Resp['errno'] = -20;
        $Resp["message"] = "Unable to translate with SMT after 5 retries.";

        return $Resp;
    }


*/

} // namespace Targoman::API::ModuleHelpers::MT::Gateways
