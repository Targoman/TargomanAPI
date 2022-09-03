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

#ifndef TARGOMAN_API_MODULEHELPERS_MT_ENGINES_PROMT_H
#define TARGOMAN_API_MODULEHELPERS_MT_ENGINES_PROMT_H

#include "intfTranslatorGateway.hpp"
#include "../Classes/TranslationDispatcher.h"

namespace Targoman::API::ModuleHelpers::MT::Gateways {

/**
 * CAUTION:
 * place #include this header file in ActiveGateways.h for proper driver registration
 */

class gtwPROMT : public intfTranslatorGateway
{
public:
    constexpr static char Name[] = "PROMT";

    TARGOMAN_API_MT_GATEWAY_DEFINE(gtwPROMT)






    const BASE_URL = "http://pts.promt.com/pts/Services";
    const COOKIE_JAR = __DIR__."../../../temp/cookies.txt";

    private function execute($url, $data=null, $post=false) {
        $cooked_data = null;
        if($data !== null) {
            if($post){
                $cooked_data = json_encode($data);
            }else{
                $cooked_data = array();
                foreach($data as $key => $value) {
                    $cooked_data[] = urlencode($key) . "=" . (is_bool($value) ? ($value ? "true" : "false") : urlencode($value));
                }   
                $cooked_data = implode("&", $cooked_data);
            }   
        }   
        $ch = curl_init();
        curl_setopt($ch, CURLOPT_COOKIEJAR, self::COOKIE_JAR);
        curl_setopt($ch, CURLOPT_COOKIEFILE, self::COOKIE_JAR);
        curl_setopt($ch, CURLOPT_RETURNTRANSFER, true);
        curl_setopt($ch, CURLOPT_HTTPHEADER, array('Accept: application/json'));
        if($post) {
            curl_setopt($ch, CURLOPT_POST, 1);
            curl_setopt($ch, CURLOPT_HTTPHEADER, array('Content-Type:application/json', 'Accept: application/json'));
            curl_setopt($ch, CURLOPT_POSTFIELDS, $cooked_data === null ? "" : $cooked_data);
            curl_setopt($ch, CURLOPT_URL, self::BASE_URL . $url); 
        } else {
            $get_url = $url;
            if($cooked_data !== null)
                $get_url .= "?" . $cooked_data;
            curl_setopt($ch, CURLOPT_URL, self::BASE_URL . $get_url);
        }   
        try {
            $result = curl_exec($ch);
            if($result[0] !== '<')
                $result = json_decode($result);
        } catch(Exception $e) {
            print_r($e->getMessage());
            $result = false;
        }   
        curl_close($ch);
        return $result;
    }   

    public function isLoggedIn() {
        return $this->execute("/auth/rest.svc/IsLoggedIn");
    }

    public function login($username, $password, $isPersistent) {
        return $this->execute("/auth/rest.svc/Login", array(
            "username" => $username,
            "password" => $password,
            "isPersistent" => $isPersistent
        ), true);
    }

    public function detectTextLanguage($text) {
        // return $this->execute("/v1/rest.svc/DetectTextLanguage", array("text" => $text));
        $non_cyrillic_count = mb_strlen(preg_replace('/[А-Яа-яЁё]/u', '', $text));
        $cyrillic_count = mb_strlen($text) - $non_cyrillic_count;
        return $cyrillic_count > $non_cyrillic_count ? "ru" : "en";
    }

    public function translateText($text, $from, $to) {
        return $this->execute("/v1/rest.svc/TranslateText", array(
            "text" => $text,
            "from" => $from,
            "to" => $to,
        ),true);
    }
}
/*
 $sourcePhrase = "Биоинженерия – одно из самых современных направлений науки, возникшее на стыке физико-химической биологии, биофизики, генной инженерии и компьютерных технологий.";
 $prompt = new PROMPT();
 $isLoggedIn = $prompt->isLoggedIn();
 echo "IsLoggedIn: " . ($isLoggedIn ? "true" : "false") . "\n";
 if(!$isLoggedIn) {
     $loginSuccess = $prompt->login("targoman", "Targ0man", false);
     echo "LoginSuccess: " . ($loginSuccess ? "true" : "false") . "\n";
     $isLoggedIn = $loginSuccess;
 }
 if(!$isLoggedIn) {
     echo "Error logging into PROMT system.";
 } else {
     $from = $prompt->detectTextLanguage($sourcePhrase);
     echo "SourceLanguage: " . $from . "\n";
     echo "Translation:\n" . $prompt->translateText($sourcePhrase, "ru", "en") . "\n";
 }
 echo "\n\n";
*/

} //namespace Targoman::API::ModuleHelpers::MT::Engines

#endif // TARGOMAN_API_MODULEHELPERS_MT_ENGINES_PROMT_H
