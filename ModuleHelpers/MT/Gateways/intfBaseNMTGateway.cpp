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

#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>
#include "intfBaseNMTGateway.h"
#include "QtCUrl.h"
#include "../Classes/TranslationDispatcher.h"

#include <QtDebug>

namespace Targoman::API::ModuleHelpers::MT::Gateways {

using namespace Classes;

namespace NMTResponse {
    TARGOMAN_CREATE_CONSTEXPR(rslt);
    TARGOMAN_CREATE_CONSTEXPR(serverName);

    namespace Result {
        TARGOMAN_CREATE_CONSTEXPR(phrases);
        TARGOMAN_CREATE_CONSTEXPR(alignments);
        TARGOMAN_CREATE_CONSTEXPR(tokens);
    }
}

intfBaseNMTGateway::intfBaseNMTGateway()
{ ; }

QVariantMap intfBaseNMTGateway::doTranslation(
    const stuEngineSpecs& _engineSpecs,
    const QString& _text,
    bool _detailed,
    bool _detokenize
) {
    int Retries = 0;
    QVariantMap Result;

//    auto makeSourceArray = [](const QString& _text) {
//        QVariantList  List;
//        foreach (auto Line, _text.split("\n", QString::SkipEmptyParts))
//            List.append(QVariantMap({ { "src", Line } }));
//        return List;
//    };

    QVariantList SourceSentences = this->makeSrcSentences(_text);

    while (Retries < 5) {
        QtCUrl CUrl;
        CUrl.setTextCodec("UTF-8");

        QtCUrl::Options Opt;
        Opt[CURLOPT_URL] = _engineSpecs.URL;
        Opt[CURLOPT_POST] = true;
        Opt[CURLOPT_TIMEOUT] = 1;
        Opt[CURLOPT_FAILONERROR] = true;
        Opt[CURLOPT_HTTPHEADER] = QStringList({
                                                  { "Content-Type: application/json" },
                                              });
//        Opt[CURLOPT_POSTFIELDS] = QJsonDocument::fromVariant(makeSourceArray(_text)).toJson(QJsonDocument::Compact);
        Opt[CURLOPT_POSTFIELDS] = QJsonDocument::fromVariant(SourceSentences).toJson(QJsonDocument::Compact);
        QString CUrlResult = CUrl.exec(Opt);

        if (CUrl.lastError().code() == CURLE_OPERATION_TIMEDOUT) {
            Result[RESULTItems::ERRNO]   = enuTranslationError::OperationTimedOut;
            Result[RESULTItems::MESSAGE] = "Unable to translate with NMT after 5 retries.";
            ++Retries;
        } else if (CUrl.lastError().code() == CURLE_COULDNT_CONNECT) {
            Result[RESULTItems::ERRNO]   = enuTranslationError::ConnectionRefused;
            Result[RESULTItems::MESSAGE] = "Unable to translate with NMT after 5 retries.";
            ++Retries;
        } else if (CUrl.lastError().isOk() == false) {
            Result[RESULTItems::ERRNO] = enuTranslationError::CURLError + CUrl.lastError().code();
            Result[RESULTItems::MESSAGE] = CUrl.lastError().text();
            return Result;
        } else {
            QJsonParseError JsonError;
            QJsonDocument Doc = QJsonDocument::fromJson(CUrlResult.toUtf8(),& JsonError);
            if (JsonError.error != QJsonParseError::NoError) {
                Result[RESULTItems::ERRNO] = enuTranslationError::JsonParseError;
                Result[RESULTItems::MESSAGE] = "Unable to parse JSON: " + JsonError.errorString() + '"' + CUrlResult + '"';
                ++Retries;
                continue;
            }
            Result = this->buildProperResponse(_engineSpecs, Doc, _detailed, _detokenize);

            return Result;
        }
    }
    return  Result;
}

QVariantList intfBaseNMTGateway::makeSrcSentences(const QString &_sourceText) {
    QVariantList Result;
    foreach (auto Part, _sourceText.split("\n", QString::SkipEmptyParts)) {
        Result.append(QVariantMap({ { "src", Part } }));
    }
    return Result;
}

QVariantMap intfBaseNMTGateway::buildProperResponse(
    const stuEngineSpecs& _engineSpecs,
    const QJsonDocument& _doc,
    bool _detailed,
    bool _detok
) {
    Q_UNUSED(_detailed)

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

    if (BaseMap.isEmpty()) return invalidResponse();

    if (BaseMap.contains(NMTResponse::serverName))
        Result[RESULTItems::SERVERID] = BaseMap[NMTResponse::serverName].toString();

    static auto baseTranslation = [_engineSpecs, _detok](const QVariantMap& SentenceResults) {
        QStringList TrTokens;

        foreach (auto Phrase, SentenceResults[NMTResponse::Result::phrases].toList())
            TrTokens.append(Phrase.toList().at(0).toString());

        if (_detok)
            return TranslationDispatcher::instance().detokenize(TrTokens.join(" "), _engineSpecs.DestLang);

        return TrTokens.join(" ");
    };

    if (!_detailed) {
        QStringList TrSentences;

        foreach (QVariant SentenceResults, BaseMap[NMTResponse::rslt].toList())
            TrSentences.append(baseTranslation(SentenceResults.toMap()));

        Result[RESULTItems::SIMPLE] = TrSentences.join('\n');
    } else {
        QVariantList ResultBaseList, ResultPhrasesList, ResultAlignmentsList;

        foreach (auto SentenceResults, BaseMap[NMTResponse::rslt].toList()) {
            QStringList TempStringList;
            QVariantList TempList;
            QVariantMap SentenceResultMap = SentenceResults.toMap();
            QVariantList TokensList = SentenceResultMap[NMTResponse::Result::tokens].toList();

            foreach (auto Token, TokensList)
                TempStringList.append(Token.toString());

            ResultBaseList.push_back(QVariantList({
                                                      { TempStringList.join(" ") },
                                                      { baseTranslation(SentenceResultMap) }
                                                  }));

            quint16 Index = 0;
            foreach (auto Phrases, SentenceResultMap[NMTResponse::Result::phrases].toList())
                TempList.push_back(QVariantList({
                                                    { _detok
                                                      ? TranslationDispatcher::instance().detokenize(Phrases.toList().at(0).toString(), _engineSpecs.DestLang)
                                                      : Phrases.toList().at(0) },
                                                    { Index++ }
                                                }));
            ResultPhrasesList.push_back(TempList);

            Index = 0;
            TempList.clear();

            static auto buildAlignments = [_engineSpecs, _detok](const QVariantList& _phrases) {
                QVariantList Result;
                bool IsFirst = true;

                foreach (auto Phrase, _phrases) {
                    if (Phrase.toString().size())
                        Result.push_back(QVariantList({
                                                          { _detok
                                                            ? TranslationDispatcher::instance().detokenize(Phrase.toString(), _engineSpecs.DestLang)
                                                            : Phrase },
                                                          { IsFirst }
                                                      }));
                    IsFirst = false;
                }
                return Result;
            };

            foreach (auto Alignment, SentenceResultMap[NMTResponse::Result::alignments].toList()) {
                TempStringList.clear();

                foreach (auto AlignmentItem, Alignment.toList())
                    TempStringList.append(TokensList.at(AlignmentItem.toInt()).toString());

                TempList.push_back(QVariantList({
                                                    { TempStringList.join(' ') },
                                                    { Alignment.toList().at(0).toInt() + 1 },
                                                    { buildAlignments(SentenceResultMap[NMTResponse::Result::phrases].toList().at(Index).toList()) }
                                                }));
                Index++;
            }
            ResultAlignmentsList.push_back(TempList);
        }

        Result[RESULTItems::TRANSLATION] = QVariantMap({
                                                           { RESULTItems::TRANSLATIONItems::BASE, ResultBaseList },
                                                           { RESULTItems::TRANSLATIONItems::PHRASES, ResultPhrasesList },
                                                           { RESULTItems::TRANSLATIONItems::ALIGNMENTS, ResultAlignmentsList },
                                                       });
    }

    return Result;
}

} // namespace Targoman::API::ModuleHelpers::MT::Gateways
