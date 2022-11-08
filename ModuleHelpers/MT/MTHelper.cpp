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

#include <QVariantMap>
#include <QTime>
#include <QUrl>
#include <QMutex>
#include <QMutexLocker>
#include <QString>
#include <QRegularExpression>

#include "MTHelper.h"
#include "libTargomanTextProcessor/TextProcessor.h"
#include "Interfaces/AAA/AAA.hpp"
#include "ModuleHelpers/NLP/FormalityChecker.h"
#include "libTargomanCommon/Configuration/tmplConfigurableArray.hpp"

/***********************************************************************************/
/***********************************************************************************/
/***********************************************************************************/
#include "Gateways/ActiveGateways.h"
/***********************************************************************************/
/***********************************************************************************/
/***********************************************************************************/

using namespace Targoman::API::ModuleHelpers::NLP;

namespace Targoman::API::ModuleHelpers::MT {

using namespace Apps;
using namespace NLPLibs;
using namespace DBManager;
using namespace Common;
//using namespace Common::Configuration;
using namespace AAA;
//using namespace NLP;
using namespace Gateways;

TARGOMAN_API_MODULEHELPER_IMPLEMENT_DB_CONFIG(MTHelper, "MTCommon")

tmplConfigurableMultiMap<stuCfgTranslationEngine> TranslationConfigs::Engines(
    TranslationConfigs::makeConfig("Engines"),
    "List of valid translation servers to connect to them separated by their translation engine"
);

tmplConfigurableMultiMap<stuCfgTranslationEngineAlias> TranslationConfigs::Aliases(
    TranslationConfigs::makeConfig("Aliases"),
    ""
);

MTHelper::MTHelper() {
    FormalityChecker::instance();
}

MTHelper::~MTHelper() { ; }

void MTHelper::registerGateway(const QString& _gatewayName, intfTranslatorGateway* _gateway) {
    if (MTHelper::RegisteredGateways.contains(_gatewayName))
        throw Targoman::Common::exTargomanBase(QString("The class for gateway name `%1` has been already registered").arg(_gatewayName));

    TargomanDebug(5) << "registering MT gateway:" << _gatewayName;

    MTHelper::RegisteredGateways.insert(_gatewayName, _gateway);
}
intfTranslatorGateway* MTHelper::getGateway(const QString& _gatewayName) {
    if (MTHelper::RegisteredGateways.contains(_gatewayName) == false)
        throw Targoman::Common::exTargomanBase(QString("The class with gateway name `%1` has not been registered").arg(_gatewayName));

    return MTHelper::RegisteredGateways[_gatewayName];
}

//called by MT, Targoman, Tarjomyar, ...
static QMutex InitializeLock;
void MTHelper::initialize() {
    QMutexLocker Locker(&InitializeLock);

    ///@TODO: register MTCommon database

    this->registerEngines();
}

void MTHelper::registerEngines() {
    QStringList ValidEngines = enuEngine::options();

    auto Keys = TranslationConfigs::Engines.keys();
    foreach (const QString& Key, Keys) {
        const tmplConfigurableArray<stuCfgTranslationEngine>& ServersConfig = TranslationConfigs::Engines.values(Key);

        for (size_t i=0; i<ServersConfig.size(); ++i) {
            stuCfgTranslationEngine Server = ServersConfig.at(i);

            QString EngineRegKey = stuEngineSpecs::makeFullName(Key, Server.SourceLang.value(), Server.DestLang.value(), Server.Class.value());
            if (this->RegisteredEngines.contains(EngineRegKey))
                continue;
//                throw exTargomanInitialization(QString("Engine %1 already registered").arg(Engine->fullName()));

            TargomanDebug(5) << "registering MT engine:"
                             << Key
                             << Server.SourceLang.value()
                             << Server.DestLang.value()
                             << Server.Class.value()
                             << Server.URLs.value()
                             << Server.SupportsIXML.value()
                             << Server.Active.value()
                             << Server.Driver.value()
                             ;

            if (Server.Active.value() == false)
                continue;

            if (ValidEngines.contains(Key) == false)
                throw exTargomanInitialization(QString("Invalid engine type `%1`").arg(Key));

            //Throws if not found
            Q_DECL_UNUSED intfTranslatorGateway* Gateway = this->getGateway(Server.Driver.value());

            clsEngine* Engine = new clsEngine(stuEngineSpecs(
                                               enuEngine::toEnum(Key),
                                               Server.SourceLang.value(),
                                               Server.DestLang.value(),
                                               Server.Class.value(),
                                               Server.URLs.value(),
                                               Server.SupportsIXML.value(),
                                               Server.Driver.value()
//                                               this->getGateway(Server.Driver.value())
                                          ));

            this->RegisteredEngines.insert(
                    EngineRegKey,
//                    Engine->fullName(),
                    Engine
                    );
        }
    }
}

//https://stackoverflow.com/questions/1401317/remove-non-utf8-characters-from-string
//QString MTHelper::cleanText(QString &_text) {
//    QString Regex = R"(
///
//  (
//    (?: [\x00-\x7F]                 # single-byte sequences   0xxxxxxx
//    |   [\xC0-\xDF][\x80-\xBF]      # double-byte sequences   110xxxxx 10xxxxxx
//    |   [\xE0-\xEF][\x80-\xBF]{2}   # triple-byte sequences   1110xxxx 10xxxxxx * 2
//    |   [\xF0-\xF7][\x80-\xBF]{3}   # quadruple-byte sequence 11110xxx 10xxxxxx * 3
//    ){1,100}                        # ...one or more times
//  )
//| .                                 # anything else
///x
//)";

//    return _text.replace(QRegularExpression(Regex), "$1");
//}

template <TAPI::enuTokenActorType::Type _itmplTokenActorType>
QVariantMap MTHelper::doTranslation(
    INTFAPICALLBOOM_IMPL &APICALLBOOM_PARAM,
    intfMTModule<_itmplTokenActorType == TAPI::enuTokenActorType::API> *_mtModule,
    QString _text,
    const TranslationDir_t &_dir,
    const QString &_engine,
    bool _useSpecialClass,
    bool _detailed,
    bool _detokenize,
    bool _dic,
    bool _dicFull,
    int &_preprocessTime,
    int &_translationTime
) {
    APICALLBOOM_PARAM.createScopeTiming("translate");

    QTime Timer;
    Timer.start();

    if (_detailed && Authorization::hasPriv(APICALLBOOM_PARAM, { TARGOMAN_PRIV_PREFIX + "Detailed" }) == false)
        throw exAuthorization("Not enought privileges to get detailed translation response.");

//    _text = this->cleanText(unicode_trim(_text));
//    auto SourceWordCount = _text.split(QRegularExpression("~[^\\p{L}\\p{N}\\']+~u")).length();

    _text = this->tokenize(_text, _dir.first);
    quint64 SourceWordCount = static_cast<quint64>(_text.split(' ').size());
    QString SourceClass = this->detectClass(_text, _dir.first);

    //dic
    //-----------------------------------------
    if (_dic) {
        if (Authorization::hasPriv(APICALLBOOM_PARAM, { TARGOMAN_PRIV_PREFIX + "Dic" })) {
            if (_dicFull && Authorization::hasPriv(APICALLBOOM_PARAM, { TARGOMAN_PRIV_PREFIX + "DicFull" }))
                throw exAuthorization("Not enought privileges to retrieve dictionary full response.");

            _preprocessTime = Timer.elapsed();
            Timer.restart();

            QVariantMap DicResponse = MTHelper::instance().retrieveDicResponse(_text, _dir.first);

            _translationTime = Timer.elapsed();

            if (DicResponse.size()) {
                if (_detailed) {
                    DicResponse[RESULTItems::TIMES] = QVariantMap({
                                                                      { RESULTItems::TIMESItems::PRE,  _preprocessTime },
                                                                      { RESULTItems::TIMESItems::TR,   _translationTime },
                                                                      { RESULTItems::TIMESItems::POST, 0 },
                                                                      { RESULTItems::TIMESItems::ALL,  _preprocessTime + _translationTime },
                                                                  });
                }

                MTHelper::instance().addDicLog(_dir.first, SourceWordCount, _text);
                return DicResponse;
            }
        } else
            throw exAuthorization("Not enought privileges to retrieve dictionary response.");
    }

    //cache
    //-----------------------------------------
    QTime CacheLookupTime;
    CacheLookupTime.start();

    QString FullDir = _dir.first + "2" + _dir.second;
    QString TranslateKey = QString("%1::%2::%3").arg(_engine).arg(FullDir).arg(SourceClass);

    QString CacheKey = QCryptographicHash::hash(QString("%1::%2")
//                                                .arg(_useSpecialClass)
                                                .arg(TranslateKey)
                                                .arg(_text)
                                                .toUtf8(),
                                                QCryptographicHash::Md4).toHex();

    QVariantMap CachedTranslation = this->TranslationCache[CacheKey];

    if (CachedTranslation.isEmpty() == false) {

//        $this->addTranslationLog($ClientIP,$dir,NULL,$EngineSQLName,"127.0.0.254",$SourceWordCount,0,$sourcePhrase,"Cache");

        CachedTranslation[RESULTItems::CACHE] = CacheLookupTime.elapsed();
        return CachedTranslation;
    }

    //check credit
    //-----------------------------------------

    ServiceUsage_t RequestedUsage = {
//            { QString("%1=%2::%3-%4").arg(RequestedUsage::CREDIT).arg(_engine).arg(_dir.first).arg(_dir.second), SourceWordCount },
        { RequestedUsage::CREDIT, QVariantMap({
              { QString("%1::%2").arg(MTAction::TRANSLATE).arg(TranslateKey), SourceWordCount },
        }) },
//            { MTRequestedUsage::ENGINE,     _engine },
//            { MTRequestedUsage::DIR,        QString("%1-%2").arg(_dir.first).arg(_dir.second) },
//            { RequestedUsage::CREDIT,       SourceWordCount },
        { MTRequestedUsage::DIC,        _dic ? 1 : 0 },
//            { MTRequestedUsage::SAMPLE,      },
    };

    stuActiveCredit ActiveCredit;

    if (APICALLBOOM_PARAM.isAnonymouse() == false) {
//        QVariant Result = _mtModule->accountUserAssets()->Select(APICALLBOOM_PARAM,
//                                                                 {}, 0, 0, {}, {}, {}, {}, false, false,
//                                                                 clsCondition(tblAccountUserAssetsBase::Fields::uas_actorID,
//                                                                              enuConditionOperator::Equal,
//                                                                              APICALLBOOM_PARAM.getActorID())/*,
//                                                                 3600*/);


//        $SPResult = $this->DB['MT']->call("sp_READ_usedCredit", array($this->Privs["tokID"], $EngineSQLName));

//        if(count($SPResult['rows']) == 2){
//            $this->checkCredit($EngineSQLName.'MaxPerDay', $SPResult['rows'][0]['tstTodayWordCount'] + $SourceWordCount);
//            $this->checkCredit($EngineSQLName.'MaxPerMonth', $SPResult['rows'][0]['tstMonthWordCount'] + $SourceWordCount);
//            $this->checkCredit($EngineSQLName.'MaxTotal', $SPResult['rows'][0]['tstTotalWordCount'] + $SourceWordCount);
//            $this->checkCredit('MaxPerDay', $SPResult['rows'][1]['tstTodayWordCount'] + $SourceWordCount);
//            $this->checkCredit('MaxPerMonth', $SPResult['rows'][1]['tstMonthWordCount'] + $SourceWordCount);
//            $this->checkCredit('MaxTotal', $SPResult['rows'][1]['tstTotalWordCount'] + $SourceWordCount);
//        }

        //check credit
        //-----------------------------------------
        ActiveCredit = _mtModule->checkUsageIsAllowed(
            APICALLBOOM_PARAM,
            RequestedUsage,
            MTAction::TRANSLATE
            );

        TargomanDebug(5).noquote() << "Action:" << MTAction::TRANSLATE << endl
                                   << "RequestedUsage:" << QJsonDocument(QJsonObject::fromVariantMap(RequestedUsage)).toJson() << endl
                                   << "ActiveCredit:" << QJsonDocument(ActiveCredit.toJson()).toJson();
    }

#ifdef QT_DEBUG
    if (_engine == "TEST_AAA") {
        _mtModule->saveAccountUsage(
            APICALLBOOM_PARAM,
            ActiveCredit,
            RequestedUsage,
            MTAction::TRANSLATE
            );

        QVariantMap TranslationResult;
        return TranslationResult;
    }
#endif

    //-----------------------------------------
    auto FnFindClassAndEngine = [this](
        QString _text,
        const TranslationDir_t &_dir,
        const QString &_engine,
        bool _useSpecialClass,
        QString &_class
    ) -> clsEngine* {
        clsEngine* TranslationEngine = nullptr;

        if (_useSpecialClass) {
            if (_class.isEmpty())
                _class = this->detectClass(/*_engine, */_text, _dir.first);

            TranslationEngine = this->RegisteredEngines.value(stuEngineSpecs::makeFullName(_engine, _dir.first, _dir.second, _class));

            if (TranslationEngine != nullptr)
                return TranslationEngine;
        }

        TranslationEngine = this->RegisteredEngines.value(stuEngineSpecs::makeFullName(_engine, _dir.first, _dir.second));
        if (TranslationEngine != nullptr)
            return TranslationEngine;

        if (_dir.first == "en" || _dir.second == "en")
            throw exHTTPInternalServerError(QString("Unable to find translator engine for `%1 -> %2`").arg(_dir.first).arg(_dir.second));

        return nullptr;
    };

    auto FnGetTranslationResultText = [](const QVariantMap &_TranslationResult) -> QString {
        QVariantMap ResultAsMap = _TranslationResult[RESULTItems::TRANSLATION].toMap();

        if (ResultAsMap.contains(RESULTItems::SIMPLE))
            return ResultAsMap[RESULTItems::SIMPLE].toString();

        if (ResultAsMap.contains(RESULTItems::TRANSLATIONItems::BASE)) {
            QStringList ResultList;
            foreach (auto OneLine, ResultAsMap[RESULTItems::TRANSLATIONItems::BASE].toList())
                ResultList.append(OneLine.toString());

            return ResultList.join('\n');
        }

        throw exHTTPInternalServerError("Unable to find translate result");
    };

    //1: find _dir.first class end engine
    clsEngine* TranslationEngine = FnFindClassAndEngine(
                                       _text,
                                       _dir,
                                       _engine,
                                       _useSpecialClass,
                                       SourceClass
                                       );

    if (TranslationEngine != nullptr) {
        QVariantMap TranslationResult = this->internalDoTranslation<_itmplTokenActorType>(
                                          APICALLBOOM_PARAM,
                                          _mtModule,
                                          _text,
                                          _dir,
//                                          _engine,
//                                          _useSpecialClass,
                                          SourceClass,
                                          TranslationEngine,
                                          _detailed,
                                          _detokenize,
                                          _preprocessTime,
                                          _translationTime
                                          );


        if (TranslationResult.value(RESULTItems::ERRNO, 0).toInt() == 0)
            this->TranslationCache.insert(CacheKey, TranslationResult);

        _mtModule->saveAccountUsage(
            APICALLBOOM_PARAM,
            ActiveCredit,
            RequestedUsage,
            MTAction::TRANSLATE
            );

        return TranslationResult;
    }

    //need more steps
    //-----------------------------------------
    QString TextToTranslate = _text;
    TranslationDir_t Dir;
    QString Class = "";
    _detailed = false;

    //phase 1
    Dir = { _dir.first, "en" };
    clsEngine* PreTranslationEngine = FnFindClassAndEngine(
                                          TextToTranslate,
                                          Dir,
                                          _engine,
                                          _useSpecialClass,
                                          Class
                                          );

    QVariantMap PreTranslationResult = this->internalDoTranslation<_itmplTokenActorType>(
                                         APICALLBOOM_PARAM,
                                         _mtModule,
                                         TextToTranslate,
                                         Dir,
//                                         _engine,
//                                         _useSpecialClass,
                                         Class,
                                         PreTranslationEngine,
                                         _detailed,
                                         _detokenize,
                                         _preprocessTime,
                                         _translationTime
                                         );

    TextToTranslate = FnGetTranslationResultText(PreTranslationResult);

    //phase 2
    Class = "";
    Dir = { "en", _dir.second };
    clsEngine* PostTranslationEngine = FnFindClassAndEngine(
                                           TextToTranslate,
                                           Dir,
                                           _engine,
                                           _useSpecialClass,
                                           Class
                                           );

    QVariantMap PostTranslationResult = this->internalDoTranslation<_itmplTokenActorType>(
                                          APICALLBOOM_PARAM,
                                          _mtModule,
                                          TextToTranslate,
                                          Dir,
//                                          _engine,
//                                          _useSpecialClass,
                                          Class,
                                          PostTranslationEngine,
                                          _detailed,
                                          _detokenize,
                                          _preprocessTime,
                                          _translationTime
                                          );

    TextToTranslate = FnGetTranslationResultText(PostTranslationResult);

    //result
    if (PostTranslationResult.value(RESULTItems::ERRNO, 0).toInt() == 0)
        this->TranslationCache.insert(CacheKey, PostTranslationResult);

    _mtModule->saveAccountUsage(
        APICALLBOOM_PARAM,
        ActiveCredit,
        RequestedUsage,
        MTAction::TRANSLATE
        );

    return PostTranslationResult;
}

//clsEngine* MTHelper::findEngine(
//    INTFAPICALLBOOM_IMPL &APICALLBOOM_PARAM,
//    QString _text,
//    const TranslationDir_t& _dir,
//    const QString& _engine,
//    bool _useSpecialClass,
//    /*OUT*/ QString &_class
//) {
//    clsEngine* TranslationEngine = nullptr;

//    if (_useSpecialClass) {
//        _class = this->detectClass(_engine, _text, _dir.first);

//        TranslationEngine = this->RegisteredEngines.value(stuEngineSpecs::makeFullName(_engine, _dir.first, _dir.second, _class));

//        if (TranslationEngine != nullptr)
//            return TranslationEngine;
//    }

//    TranslationEngine = this->RegisteredEngines.value(stuEngineSpecs::makeFullName(_engine, _dir.first, _dir.second));
//    if (TranslationEngine != nullptr)
//        return TranslationEngine;

//    if (_dir.first == "en" || _dir.second == "en")
//        throw exHTTPInternalServerError(QString("Unable to find translator engine for %1_%2").arg(_dir.first).arg(_dir.second));

//    return nullptr;
//}

template <TAPI::enuTokenActorType::Type _itmplTokenActorType>
QVariantMap MTHelper::internalDoTranslation(
    INTFAPICALLBOOM_IMPL &APICALLBOOM_PARAM,
    intfMTModule<_itmplTokenActorType == TAPI::enuTokenActorType::API> *_mtModule,
    QString _text,
    const TranslationDir_t& _dir,
//    const QString& _engine,
//    bool _useSpecialClass,
    const QString &_class,
    clsEngine* _translationEngine,
    bool _detailed,
    bool _detokenize,
    int& _preprocessTime,
    int& _translationTime
) {
    QVariantMap TranslationResult;

    QTime Timer;
    Timer.start();

    _text = this->preprocessText<_itmplTokenActorType>(
                APICALLBOOM_PARAM,
                _mtModule,
                _text,
                _dir.first
                );

    _preprocessTime = Timer.elapsed();
    Timer.restart();

    if (_translationEngine->specs().SupportsIXML == false)
        _text = TargomanTextProcessor::instance().ixml2Text(_text, false, false,false);

    /********************************/
    TranslationResult = _translationEngine->doTranslation(
                            APICALLBOOM_PARAM,
                            _text,
                            _detailed,
                            _detokenize);
    /********************************/

    _translationTime = Timer.elapsed();

    if (_class.size() && Authorization::hasPriv(APICALLBOOM_PARAM, { TARGOMAN_PRIV_PREFIX + "ReportClass" }))
        TranslationResult[RESULTItems::CLASS] = _class;

    switch (TranslationResult.value(RESULTItems::ERRNO, 0).toInt()) {
        case enuTranslationError::Ok:
            break;

        case enuTranslationError::OperationTimedOut:
        case enuTranslationError::ConnectionRefused:
            throw exHTTPRequestTimeout(QString("%1 (%2 -> %3)")
                                       .arg(TranslationResult.value(RESULTItems::MESSAGE).toString())
                                       .arg(_dir.first)
                                       .arg(_dir.second)
                                       );

        case enuTranslationError::InvalidServerResponse:
        case enuTranslationError::JsonParseError:
            throw exHTTPUnprocessableEntity(QString("%1 (%2 -> %3)")
                                            .arg(TranslationResult.value(RESULTItems::MESSAGE).toString())
                                            .arg(_dir.first)
                                            .arg(_dir.second)
                                            );

        case enuTranslationError::CURLError:
        default:
            throw exHTTPExpectationFailed(QString("%1 (%2 -> %3)")
                                          .arg(TranslationResult.value(RESULTItems::MESSAGE).toString())
                                          .arg(_dir.first)
                                          .arg(_dir.second)
                                          );
    }

    return TranslationResult;
}

QString MTHelper::detectClass(
//    const QString& _engine,
    const QString& _text,
    const QString& _lang
) {
    if (gConfigs::Classifier::SupportsIXML.value() == false)
        return FormalityChecker::instance().check(_lang, TargomanTextProcessor::instance().ixml2Text(_text, false, false, false));

    return FormalityChecker::instance().check(_lang, _text);
}

template <TAPI::enuTokenActorType::Type _itmplTokenActorType>
QString MTHelper::preprocessText(
    INTFAPICALLBOOM_IMPL &APICALLBOOM_PARAM,
    intfMTModule<_itmplTokenActorType == TAPI::enuTokenActorType::API> *_mtModule,
    const QString& _text,
    const QString& _lang
) {
    Q_UNUSED (_lang)

    if (this->CorrectionRule.isEmpty() || this->LastCorrectionRuleUpdateTime.elapsed() > 3600) {
        QVariant Result = _mtModule->MTCorrectionRules->Select(APICALLBOOM_PARAM,
                                                               {}, 0, 0, {}, {}, {}, {}, false, false,
                                                               clsCondition(tblMTCorrectionRulesBase::Fields::crlType,
                                                                            enuConditionOperator::Equal,
                                                                            enuCorrectionRuleType::Replace),
                                                               3600);

        if (Result.isValid()) {
            TAPI::stuTable ResultTable = Result.value<TAPI::stuTable>();

            this->CorrectionRule.clear();
            foreach (auto Row, ResultTable.Rows) {
                this->CorrectionRule.append(
                        qMakePair(
                            QRegularExpression(Row.toMap()[tblMTCorrectionRulesBase::Fields::crlPattern].toString(),
                                               QRegularExpression::UseUnicodePropertiesOption | QRegularExpression::MultilineOption),
                            Row.toMap()[tblMTCorrectionRulesBase::Fields::crlReplacement].toString())
                        );
            }
            this->LastCorrectionRuleUpdateTime.start();
        }
    }

    QString Text = _text;

    foreach (auto Rule, this->CorrectionRule)
        Text = Text.replace(Rule.first, Rule.second);

    return Text;
}

QString MTHelper::tokenize(const QString& _text, const QString& _lang) {
    bool SpellCorrected;
    QList<stuIXMLReplacement> SentenceBreakReplacements;
    SentenceBreakReplacements.append(
                stuIXMLReplacement(
                    QRegularExpression("(\\s)([\\.\\?\\!])(\\s)"),
                    "\\1\\2\\3")); //"\\1\\2\\n\\3"

    return TargomanTextProcessor::instance().text2IXML(
                _text,
                SpellCorrected,
                _lang,
                0,
                false,
                _lang == "fa",
                QList<enuTextTags::Type>(),
                SentenceBreakReplacements
                );
}

QString MTHelper::detokenize(const QString& _text, const QString& _lang) {
    return TargomanTextProcessor::instance().ixml2Text(_text, true, _lang=="fa" || _lang=="ar", false);
}

QVariantMap MTHelper::retrieveDicResponse(const QString& _text, const QString& _lang) {
    Q_UNUSED(_text); Q_UNUSED (_lang);
    return QVariantMap();
}

void MTHelper::addDicLog(const QString& _lang, quint64 _wordCount, const QString& _text) {
    Q_UNUSED(_text); Q_UNUSED (_lang); Q_UNUSED (_wordCount)
}

void MTHelper::addErrorLog(quint64 _aptID, const QString& _engine, const QString& _dir, quint64 _wordCount, const QString& _text, qint32 _errorCode) {
    Q_UNUSED(_text); Q_UNUSED (_dir); Q_UNUSED (_wordCount);Q_UNUSED (_aptID); Q_UNUSED (_errorCode);Q_UNUSED (_engine)
}

void MTHelper::addTranslationLog(quint64 _aptID, const QString& _engine, const QString& _dir, quint64 _wordCount, const QString& _text, int _trTime) {
    Q_UNUSED(_text); Q_UNUSED (_dir); Q_UNUSED (_wordCount);Q_UNUSED (_aptID);Q_UNUSED (_engine); Q_UNUSED (_trTime)
}

/***********************************************************************************/
template QVariantMap MTHelper::doTranslation<TAPI::enuTokenActorType::USER>(
    INTFAPICALLBOOM_DECL &APICALLBOOM_PARAM,
    intfMTModule<false> *_mtModule,
    QString _text,
    const TranslationDir_t& _dir,
    const QString& _engine,
    bool _useSpecialClass,
    bool _detailed,
    bool _detokenize,
    bool _dic, // = false,
    bool _dicFull, // = false
    int& _preprocessTime,
    int& _translationTime
);

template QString MTHelper::preprocessText<TAPI::enuTokenActorType::USER>(
    INTFAPICALLBOOM_IMPL &APICALLBOOM_PARAM,
    intfMTModule<false> *_mtModule,
    const QString& _text,
    const QString& _lang
);

template QVariantMap MTHelper::doTranslation<TAPI::enuTokenActorType::API>(
    INTFAPICALLBOOM_DECL &APICALLBOOM_PARAM,
    intfMTModule<true> *_mtModule,
    QString _text,
    const TranslationDir_t& _dir,
    const QString& _engine,
    bool _useSpecialClass,
    bool _detailed,
    bool _detokenize,
    bool _dic, // = false,
    bool _dicFull, // = false
    int& _preprocessTime,
    int& _translationTime
);

template QString MTHelper::preprocessText<TAPI::enuTokenActorType::API>(
    INTFAPICALLBOOM_IMPL &APICALLBOOM_PARAM,
    intfMTModule<true> *_mtModule,
    const QString& _text,
    const QString& _lang
);

} // namespace Targoman::API::ModuleHelpers::MT
