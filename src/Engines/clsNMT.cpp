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
 @author S. Mehran M. Ziabary <ziabary@targoman.com>
 */
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>
#include "clsNMT.h"
#include "3rdParty/QtCUrl/src/QtCUrl.h"
#include "Classes/TranslationDispatcher.h"

#include <QtDebug>

namespace Targoman {
namespace Apps {
namespace Engines {

using namespace Classes;

constexpr char RESPONSE_RESULT[]= "rslt";
constexpr char RESPONSE_SERVERID[]= "serverName";
constexpr char RESPONSE_RESULT_PHRASES[]= "phrases";
constexpr char RESPONSE_RESULT_ALIGNMENTS[]= "alignments";
constexpr char RESPONSE_RESULT_TOKENS[]= "tokens";

clsBaseNMT::clsBaseNMT(const Classes::stuEngineSpecs& _specs) :
    Classes::intfTranslatorEngine(_specs)
{ ; }

QVariantMap clsBaseNMT::doTranslation(const QString& _text, bool _detailed, bool _detokenize)
{
    int Retries = 0;
    QVariantMap Result;

    auto makeSourceArray = [](const QString& _text){
        QVariantList  List;
        foreach(auto Line, _text.split("\n", QString::SkipEmptyParts))
            List.append(QVariantMap({{"src", Line}}));
        return List;
    };

    while(Retries < 5){
        QtCUrl CUrl;
        CUrl.setTextCodec("UTF-8");

        QtCUrl::Options Opt;
        Opt[CURLOPT_URL] = this->EngineSpecs.URL;
        Opt[CURLOPT_POST] = true;
        Opt[CURLOPT_TIMEOUT] = 1;
        Opt[CURLOPT_FAILONERROR] = true;
        Opt[CURLOPT_HTTPHEADER] = QStringList({
                                                  {"Content-Type: application/json"},
                                              });
        Opt[CURLOPT_POSTFIELDS] = QJsonDocument::fromVariant(makeSourceArray(_text)).toJson(QJsonDocument::Compact);
        QString CUrlResult = CUrl.exec(Opt);

        if (CUrl.lastError().code() == CURLE_OPERATION_TIMEDOUT){
            Result[RESULT_ERRNO]   = enuTranslationError::OperationTimedOut;
            Result[RESULT_MESSAGE] = "Unable to translate with NMT after 5 retries.";
            ++Retries;
        }else if(CUrl.lastError().code() == CURLE_COULDNT_CONNECT){
            Result[RESULT_ERRNO]   = enuTranslationError::ConnectionRefused;
            Result[RESULT_MESSAGE] = "Unable to translate with NMT after 5 retries.";
            ++Retries;
        }else if(CUrl.lastError().isOk() == false){
            Result[RESULT_ERRNO] = enuTranslationError::CURLError + CUrl.lastError().code();
            Result[RESULT_MESSAGE] = CUrl.lastError().text();
            return Result;
        }else{
            QJsonParseError JsonError;
            QJsonDocument Doc = QJsonDocument::fromJson(CUrlResult.toUtf8(),& JsonError);
            if(JsonError.error != QJsonParseError::NoError){
                Result[RESULT_ERRNO] = enuTranslationError::JsonParseError;
                Result[RESULT_MESSAGE] = "Unable to parse JSON: " + JsonError.errorString() + '"' + CUrlResult + '"';
                ++Retries;
                continue;
            }
            Result = this->buildProperResponse(Doc, _detailed, _detokenize);

            return Result;
        }
    }
    return  Result;
}

QVariantMap clsBaseNMT::buildProperResponse(const QJsonDocument& _doc, bool _detailed, bool _detok)
{
    Q_UNUSED(_detailed)

    auto invalidResponse = [_doc]() -> QVariantMap{
            return  {
                {RESULT_ERRNO, enuTranslationError::InvalidServerResponse},
                {RESULT_MESSAGE, "Invalid response from server" + _doc.toJson(QJsonDocument::Compact)}
            };
    };

    if(_doc.isObject() == false)
        return invalidResponse();

    QVariantMap BaseMap = _doc.toVariant().toMap();
    QVariantMap Result;

    if(BaseMap.isEmpty()) return invalidResponse();

    if(BaseMap.contains(RESPONSE_SERVERID))
        Result[RESULT_SERVERID] = BaseMap[RESPONSE_SERVERID].toString();

    static auto baseTranslation = [_detok, this](const QVariantMap& SentenceResults){
        QStringList TrTokens;
        foreach(auto Phrase, SentenceResults[RESPONSE_RESULT_PHRASES].toList()){
            TrTokens.append(Phrase.toList().at(0).toString());
        }
        if(_detok)
            return TranslationDispatcher::instance().detokenize(TrTokens.join(" "), this->EngineSpecs.DestLang);
        else
            return  TrTokens.join(" ");
    };

    if(!_detailed){
        QStringList TrSentences;
        foreach(QVariant SentenceResults, BaseMap[RESPONSE_RESULT].toList())
            TrSentences.append (baseTranslation(SentenceResults.toMap()));
        Result[RESULT_SIMPLE] = TrSentences.join('\n');
    }else{
        QVariantList ResultBaseList, ResultPhrasesList, ResultAlignmentsList;

        foreach(auto SentenceResults, BaseMap[RESPONSE_RESULT].toList()){
            QStringList TempStringList;
            QVariantList TempList;
            QVariantMap SentenceResultMap = SentenceResults.toMap();
            QVariantList TokensList = SentenceResultMap[RESPONSE_RESULT_TOKENS].toList();

            foreach(auto Token, TokensList)
                TempStringList.append(Token.toString());
            ResultBaseList.push_back(QVariantList({{TempStringList.join(" ")},{baseTranslation(SentenceResultMap)}}));

            quint16 Index = 0;
            foreach(auto Phrases, SentenceResultMap[RESPONSE_RESULT_PHRASES].toList())
                TempList.push_back(QVariantList({
                                                    {_detok ?
                                                     TranslationDispatcher::instance().detokenize(Phrases.toList().at(0).toString(), this->EngineSpecs.DestLang) :
                                                     Phrases.toList().at(0)},
                                                    {++Index}
                                                }));
            ResultPhrasesList.push_back(TempList);

            Index = 0; TempList.clear();

            static auto buildAlignments = [this, _detok](const QVariantList& _phrases){
                QVariantList Result;
                bool IsFirst = true;
                foreach(auto Phrase, _phrases){
                    if(Phrase.toString().size())
                        Result.push_back(QVariantList({
                                                          {_detok ?
                                                           TranslationDispatcher::instance().detokenize(Phrase.toString(), this->EngineSpecs.DestLang) :
                                                           Phrase},
                                                          {IsFirst}
                                                      }));
                    IsFirst = false;
                }
                return Result;
            };

            foreach(auto Alignment, SentenceResultMap[RESPONSE_RESULT_ALIGNMENTS].toList()){
                TempStringList.clear();

                foreach(auto AlignmentItem, Alignment.toList())
                    TempStringList.append(TokensList.at(AlignmentItem.toInt()).toString());
                TempList.push_back(QVariantList({
                                                    {TempStringList.join(' ')},
                                                    {Alignment.toList().at(0).toInt() + 1},
                                                    {buildAlignments(SentenceResultMap[RESPONSE_RESULT_PHRASES].toList().at(Index).toList())}
                                                }));
                Index++;
            }
            ResultAlignmentsList.push_back(TempList);

        }

        Result[RESULT_TRANSLATION] = QVariantMap({
                                                     {RESULT_TRANSLATION_BASE, ResultBaseList},
                                                     {RESULT_TRANSLATION_PHRASES, ResultPhrasesList},
                                                     {RESULT_TRANSLATION_ALIGNMENTS, ResultAlignmentsList},
                                                 });
    }

    return Result;
}

}
}
}
