/*








abstract class intfBaseNMT  extends clsBaseTranslation {

    public function __construct($class, $loadBalancer){
        clsBaseTranslation::__construct();
        $this->loadBalancer = $loadBalancer;
        $this->className = $class;
    }
    
    abstract protected function buildProperResponse($resp, $json, $detailed, $dir);
    abstract protected function makeSrcSentences($sourceText);
    
    protected function maxChars() {return 5000;}
    protected function maxCharPerWord() {return 50;}
    protected function preprocessText($sourceText){ return $sourceText;}
    
    protected function buildAlignments($phrases) {
        $Result = [];
        $Index = 0;
        foreach($phrases as $P) {
            if(!empty($P))
                $Result[] = array($P, $Index == 0);
            ++$Index;
        }
        return $Result;
    }
    //--------------------------------------------------
    function getTUID($engine, $dir, $clientIP, $sourceText, $lastServer = 1) { return NULL; }
   
    //--------------------------------------------------
    public function translate($dir, $sourceText, $clientIP, $tuid, $detailed = false)
    {
//        dumpVar(array(
//            "Original sourceText" => $sourceText
//        ));
        $time_start = microtime(true);

        $Retry=0;
        $sourceText = $this->replaceTokens($sourceText);
//        dumpVar(array(
//            "replaceTokens sourceText" => $sourceText
//        ));
        $sourceText = $this->preprocessText($sourceText);
//        dumpVar(array(
//            "preprocessText sourceText" => $sourceText
//        ));
        
        foreach(explode("\n", $sourceText) as $line) {
          if(strlen($line) > $this->maxChars()) {
              $Resp["message"]  = "Invalid long String: ".$line;
              $Resp['errno'] = -40;
              $Resp['cacheTime'] = 600;
              return $Resp;
          }

          foreach(explode(' ', $line) as $Token)
            if(strlen($Token) > $this->maxCharPerWord() && strpos($Token, '.')==false){
              $Resp["message"]  = "Invalid long Token: ".$Token;
              $Resp['errno'] = -30;
              $Resp['cacheTime'] = 600;
              return $Resp;
            }
        }
        
        $srcSentences = $this->makeSrcSentences($sourceText);
        
        while ($Retry < 3){
            $ch = curl_init();

            curl_setopt($ch, CURLOPT_URL, $this->loadBalancer);
            curl_setopt($ch, CURLOPT_HTTPHEADER, array("Content-Type: application/json"));
            curl_setopt($ch, CURLOPT_RETURNTRANSFER, 1);
            curl_setopt($ch, CURLOPT_TIMEOUT, 30);
            curl_setopt($ch, CURLOPT_POST, 1);
            curl_setopt($ch, CURLOPT_POSTFIELDS, json_encode($srcSentences));

            $Data = curl_exec($ch);

            $Resp["TrTime"]=((microtime(true) - $time_start));
            $Resp['serverID'] = -1;

            if(curl_errno($ch) == CURLE_OPERATION_TIMEDOUT){
                $Retry = $Retry + 1;
                curl_close($ch);
                continue;
            }else if(curl_errno($ch)){
                $Retry = $Retry + 1;
                curl_close($ch);
                continue;
            }else {
                $JSONStr = cleanText($Data);
//                dumpVar(array($this->loadBalancer, $srcSentences, $Data, $JSONStr));
                $JSON = (array)json_decode(str_replace(array("\t","\r","\n"),array("\\t","\\r","\\n"),$JSONStr));
                switch (json_last_error()) {
                    case JSON_ERROR_NONE:
                        $Resp['errno'] = "0";
                        $Resp = $this->buildProperResponse($Resp, $JSON, $detailed, $dir);
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
                curl_close($ch);
                continue;
            }
        }
        $Resp['errno'] = -20;
        $Resp["message"] = "Unable to translate with NMT after 5 retries.";

        return $Resp;
    }
   
}







*/
