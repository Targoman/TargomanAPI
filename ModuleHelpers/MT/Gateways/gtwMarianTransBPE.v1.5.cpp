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

#include "gtwMarianTransBPE.v1.5.h"

namespace Targoman::API::ModuleHelpers::MT::Gateways {

TARGOMAN_API_MT_GATEWAY_IMPL(gtwMarianTransBPE_v1_5)

//protected function makeSrcSentences($sourceText) {
//    $srcSentences=[];

//    foreach(explode("\n", $sourceText) as $par){
//      $srcSentences[]=["src"=>"^^^PAR^^^"];
//      foreach(explode(" .", $par) as $snt)
//        $srcSentences[]=["src"=>$snt.' .'];
//    }

//    return $srcSentences;
//}
QVariantList gtwMarianTransBPE_v1_5::makeSrcSentences(const QString &_sourceText) {
    QVariantList Result;

    foreach (auto Par, _sourceText.split("\n", QString::SkipEmptyParts)) {
        Result.append(QVariantMap({ { "src", "^^^PAR^^^" } }));

        foreach (auto Snt, Par.split(" .", QString::SkipEmptyParts)) {
            Result.append(QVariantMap({ { "src", Snt + " ." } }));
        }
    }

    return Result;
}

//protected function buildProperResponse($resp, $json, $detailed, $dir) {
QVariantMap intfBaseNMTGateway::buildProperResponse(
    const stuEngineSpecs& _engineSpecs,
    const QJsonDocument& _doc,
    bool _detailed,
    bool _detok
) {
    auto invalidResponse = [_doc]() -> QVariantMap {
                           return {
                                      { RESULTItems::ERRNO,   enuTranslationError::InvalidServerResponse },
                                      { RESULTItems::MESSAGE, "Invalid response from server" + _doc.toJson(QJsonDocument::Compact) }
                                  };
    };

    if (_doc.isObject() == false)
        return invalidResponse();

    QVariantMap BaseMap = _doc.toVariant().toMap();
    QVariantMap Result;

    if (BaseMap.isEmpty())
        return invalidResponse();

    if (BaseMap.contains(NMTResponse::rslt)) {
        static auto baseTranslation = [_engineSpecs, _detok](const QVariantMap& SentenceResults) {
            QStringList TrTokens;

            foreach (auto Phrase, SentenceResults[NMTResponse::Result::phrases].toList())
                TrTokens.append(Phrase.toList().at(0).toString());

            if (_detok)
                return TranslationDispatcher::instance().detokenize(TrTokens.join(" "), _engineSpecs.DestLang);

            return TrTokens.join(" ");
        };

//        QString Str = "";
        bool StartOfPar = false;

        if (!_detailed) {
            QStringList TrSentences;

            foreach (QVariant SentenceResults, BaseMap[NMTResponse::rslt].toList()) {
                QVariantMap SentenceResultsMap = SentenceResults.toMap();
                QVariantList TokensList = SentenceResultsMap[NMTResponse::Result::tokens].toList();

                if (TokensList.at(0).toString().trimmed() == "." && StartOfPar)
                    continue;

                if (TokensList.at(0).toString().trimmed() == "^^^PAR^^^") {
                    StartOfPar = true;
//                    if (Str != "")
//                        Str += "\n";
                    continue;
                }
//                Str += implode(' ', array_map($get_base_phrase, $Item['phrases'])).' ';
                TrSentences.append(baseTranslation(SentenceResultsMap));
            }

//            $resp['tr'] = ['simple' => trim($this->detokenize($str,$dir))];
            Result[RESULTItems::TRANSLATION] = QVariantMap({
                                                               { RESULTItems::SIMPLE, TrSentences.join('\n') },
                                                           });
        } else {






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

