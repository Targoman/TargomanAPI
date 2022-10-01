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

template <TAPI::enuTokenActorType::Type _itmplTokenActorType>
QVariantMap MTHelper::doTranslation(
    INTFAPICALLBOOM_IMPL &APICALLBOOM_PARAM,
//    const QJsonObject& _privInfo,
    clsDerivedHelperSubmodules<_itmplTokenActorType> &DerivedHelperSubmodules,
    QString _text,
    const TranslationDir_t& _dir,
    const QString& _engine,
    bool _useSpecialClass,
    bool _detailed,
    bool _detokenize,
    int& _preprocessTime,
    int& _translationTime
) {
    APICALLBOOM_PARAM.createScopeTiming("translate");

    auto FnFindClassAndEngine = [this](
                                QString _text,
                                const TranslationDir_t &_dir,
                                const QString &_engine,
                                bool _useSpecialClass,
                                QString &_class
    ) -> clsEngine* {
        clsEngine* TranslationEngine = nullptr;
        _class = "";

        if (_useSpecialClass) {
            _class = this->detectClass(_engine, _text, _dir.first);

            TranslationEngine = this->RegisteredEngines.value(stuEngineSpecs::makeFullName(_engine, _dir.first, _dir.second, _class));

            if (TranslationEngine != nullptr)
                return TranslationEngine;
        }

        TranslationEngine = this->RegisteredEngines.value(stuEngineSpecs::makeFullName(_engine, _dir.first, _dir.second));
        if (TranslationEngine != nullptr)
            return TranslationEngine;

        if (_dir.first == "en" || _dir.second == "en")
            throw exHTTPInternalServerError(QString("Unable to find translator engine for %1_%2").arg(_dir.first).arg(_dir.second));

        return nullptr;
    };

    auto FnGetTranslateResultText = [](const QVariantMap &_translateResult) -> QString {
        QVariantMap ResultAsMap = _translateResult[RESULTItems::TRANSLATION].toMap();

        if (ResultAsMap.contains(RESULTItems::SIMPLE))
            return ResultAsMap[RESULTItems::SIMPLE].toString();

        if (ResultAsMap.contains(RESULTItems::TRANSLATIONItems::BASE)) {
            QStringList ResultList;
            foreach (auto OneLine, ResultAsMap[RESULTItems::TRANSLATIONItems::BASE].toList())
                ResultList.append(OneLine.toString());

            return ResultList.join('\n');
        }

        throw exHTTPInternalServerError("Unable to find pre translate result");
    };

    QString Class;

    //1: find _dir.first class end engine
    clsEngine* TranslationEngine = FnFindClassAndEngine(
                                       _text,
                                       _dir,
                                       _engine,
                                       _useSpecialClass,
                                       Class
                                       );

    if (TranslationEngine != nullptr) {
        QVariantMap TranslateResult = this->internalDoTranslation(
                                          APICALLBOOM_PARAM,
                                          DerivedHelperSubmodules,
                                          _text,
                                          _dir,
                                          _engine,
                                          _useSpecialClass,
                                          Class,
                                          TranslationEngine,
                                          _detailed,
                                          _detokenize,
                                          _preprocessTime,
                                          _translationTime
                                          );

        return TranslateResult;
    }

    //need more steps
    QString TextToTranslate = _text;
    TranslationDir_t Dir;

    //phase 1
    Dir = { _dir.first, "en" };
    clsEngine* PreTranslationEngine = FnFindClassAndEngine(
                                          TextToTranslate,
                                          Dir,
                                          _engine,
                                          _useSpecialClass,
                                          Class
                                          );

    QVariantMap PreTranslateResult = this->internalDoTranslation(
                                         APICALLBOOM_PARAM,
                                         DerivedHelperSubmodules,
                                         TextToTranslate,
                                         Dir,
                                         _engine,
                                         _useSpecialClass,
                                         Class,
                                         PreTranslationEngine,
                                         _detailed,
                                         _detokenize,
                                         _preprocessTime,
                                         _translationTime
                                         );

    TextToTranslate = FnGetTranslateResultText(PreTranslateResult);

    //phase 2
    Dir = { "en", _dir.second };
    clsEngine* PostTranslationEngine = FnFindClassAndEngine(
                                           TextToTranslate,
                                           Dir,
                                           _engine,
                                           _useSpecialClass,
                                           Class
                                           );

    QVariantMap PostTranslateResult = this->internalDoTranslation(
                                          APICALLBOOM_PARAM,
                                          DerivedHelperSubmodules,
                                          TextToTranslate,
                                          Dir,
                                          _engine,
                                          _useSpecialClass,
                                          Class,
                                          PostTranslationEngine,
                                          _detailed,
                                          _detokenize,
                                          _preprocessTime,
                                          _translationTime
                                          );

    TextToTranslate = FnGetTranslateResultText(PostTranslateResult);

    //result


    return PostTranslateResult;
}

clsEngine* MTHelper::findEngine(
    INTFAPICALLBOOM_IMPL &APICALLBOOM_PARAM,
    QString _text,
    const TranslationDir_t& _dir,
    const QString& _engine,
    bool _useSpecialClass,
    /*OUT*/ QString &_class
) {
    clsEngine* TranslationEngine = nullptr;

    if (_useSpecialClass) {
        _class = this->detectClass(_engine, _text, _dir.first);

        TranslationEngine = this->RegisteredEngines.value(stuEngineSpecs::makeFullName(_engine, _dir.first, _dir.second, _class));

        if (TranslationEngine != nullptr)
            return TranslationEngine;
    }

    TranslationEngine = this->RegisteredEngines.value(stuEngineSpecs::makeFullName(_engine, _dir.first, _dir.second));
    if (TranslationEngine != nullptr)
        return TranslationEngine;

    if (_dir.first == "en" || _dir.second == "en")
        throw exHTTPInternalServerError(QString("Unable to find translator engine for %1_%2").arg(_dir.first).arg(_dir.second));

    return nullptr;
}

template <TAPI::enuTokenActorType::Type _itmplTokenActorType>
QVariantMap MTHelper::internalDoTranslation(
    INTFAPICALLBOOM_IMPL &APICALLBOOM_PARAM,
    clsDerivedHelperSubmodules<_itmplTokenActorType> &DerivedHelperSubmodules,
    QString _text,
    const TranslationDir_t& _dir,
    const QString& _engine,
    bool _useSpecialClass,
    const QString &_class,
    clsEngine* _translationEngine,
    bool _detailed,
    bool _detokenize,
    int& _preprocessTime,
    int& _translationTime
) {
    if (_detailed && Authorization::hasPriv(APICALLBOOM_PARAM/*, _privInfo*/, { TARGOMAN_PRIV_PREFIX + "Detailed" }) == false)
        throw exAuthorization("Not enought privileges to get detailed translation response.");

    QTime CacheLookupTime;
    CacheLookupTime.start();

    QString CacheKey = QCryptographicHash::hash(QString("%1:%2:%3:%4:%5").arg(_useSpecialClass).arg(_engine, _dir.first, _dir.second, _text).toUtf8(), QCryptographicHash::Md4).toHex();
    QVariantMap CachedTranslation = this->TranslationCache[CacheKey];

    if (CachedTranslation.isEmpty()) {
        QTime Timer;
        Timer.start();

//        clsEngine* TranslationEngine = nullptr;
//        QString Class;

//        if (_useSpecialClass) {
//            Class = this->detectClass(_engine, _text, _dir.first);
//            TranslationEngine = this->RegisteredEngines.value(stuEngineSpecs::makeFullName(_engine, _dir.first, _dir.second, Class));
//        }

//        if (TranslationEngine == nullptr) {
//            TranslationEngine = this->RegisteredEngines.value(stuEngineSpecs::makeFullName(_engine, _dir.first, _dir.second));

//            if (TranslationEngine == nullptr)
//                throw exHTTPInternalServerError("Unable to find generic translator engine");
//        }

//        if (TranslationEngine == nullptr)
//            throw exHTTPInternalServerError(QString("Unable to find %1 translator engine").arg(Class));

        _text = this->preprocessText(
                    APICALLBOOM_PARAM,
                    DerivedHelperSubmodules,
                    _text,
                    _dir.first
                    );

        _preprocessTime = Timer.elapsed();
        Timer.restart();

        if (_translationEngine->specs().SupportsIXML == false)
            _text = TargomanTextProcessor::instance().ixml2Text(_text, false, false,false);

        /********************************/
        CachedTranslation = _translationEngine->doTranslation(
                                APICALLBOOM_PARAM,
                                _text,
                                _detailed,
                                _detokenize);
        /********************************/

        _translationTime = Timer.elapsed();
        if (_class.size() && Authorization::hasPriv(APICALLBOOM_PARAM/*, _privInfo*/, { TARGOMAN_PRIV_PREFIX + "ReportClass" }))
            CachedTranslation[RESULTItems::CLASS] = _class;

        if (CachedTranslation.value(RESULTItems::ERRNO, 0).toInt() == 0)
            this->TranslationCache.insert(CacheKey, CachedTranslation);
        else {
            switch (CachedTranslation.value(RESULTItems::ERRNO, 0).toInt()) {
                case enuTranslationError::Ok:
                    break;

                case enuTranslationError::OperationTimedOut:
                case enuTranslationError::ConnectionRefused:
                    throw exHTTPRequestTimeout(CachedTranslation.value(RESULTItems::MESSAGE).toString());

                case enuTranslationError::InvalidServerResponse:
                case enuTranslationError::JsonParseError:
                    throw exHTTPUnprocessableEntity(CachedTranslation.value(RESULTItems::MESSAGE).toString());

                case enuTranslationError::CURLError:
                default:
                    throw exHTTPExpectationFailed(CachedTranslation.value(RESULTItems::MESSAGE).toString());
            }
        }
    } else
        CachedTranslation[RESULTItems::CACHE] = CacheLookupTime.elapsed();

    return CachedTranslation;
}

QString MTHelper::detectClass(const QString& _engine, const QString& _text, const QString& _lang) {
    Q_UNUSED(_engine);

    if (gConfigs::Classifier::SupportsIXML.value() == false)
        return FormalityChecker::instance().check(_lang, TargomanTextProcessor::instance().ixml2Text(_text, false, false, false));

    return FormalityChecker::instance().check(_lang, _text);
}

template <TAPI::enuTokenActorType::Type _itmplTokenActorType>
QString MTHelper::preprocessText(
    INTFAPICALLBOOM_IMPL &APICALLBOOM_PARAM,
    clsDerivedHelperSubmodules<_itmplTokenActorType> &DerivedHelperSubmodules,
    const QString& _text,
    const QString& _lang
) {
    Q_UNUSED (_lang)

    if (this->CorrectionRule.isEmpty() || this->LastCorrectionRuleUpdateTime.elapsed() > 3600) {
        QVariant Result = DerivedHelperSubmodules.CorrectionRules->Select(APICALLBOOM_PARAM,
                                                                          {}, 0, 0, {}, {}, {}, {}, false, false,
                                                                          clsCondition(tblCorrectionRulesBase::Fields::crlType,
                                                                                       enuConditionOperator::Equal,
                                                                                       enuCorrectionRuleType::Replace),
                                                                          3600);

        if (Result.isValid()) {
            TAPI::stuTable ResultTable = Result.value<TAPI::stuTable>();

            this->CorrectionRule.clear();
            foreach (auto Row, ResultTable.Rows) {
                this->CorrectionRule.append(
                        qMakePair(
                            QRegularExpression(Row.toMap()[tblCorrectionRulesBase::Fields::crlPattern].toString(),
                                               QRegularExpression::UseUnicodePropertiesOption | QRegularExpression::MultilineOption),
                            Row.toMap()[tblCorrectionRulesBase::Fields::crlReplacement].toString())
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
    clsDerivedHelperSubmodules<TAPI::enuTokenActorType::USER> &DerivedHelperSubmodules,
    QString _text,
    const TranslationDir_t& _dir,
    const QString& _engine,
    bool _useSpecialClass,
    bool _detailed, bool _detokenize,
    int& _preprocessTime,
    int& _translationTime
);

template QString MTHelper::preprocessText<TAPI::enuTokenActorType::USER>(
    INTFAPICALLBOOM_IMPL &APICALLBOOM_PARAM,
    clsDerivedHelperSubmodules<TAPI::enuTokenActorType::USER> &DerivedHelperSubmodules,
    const QString& _text,
    const QString& _lang
);

template QVariantMap MTHelper::doTranslation<TAPI::enuTokenActorType::API>(
    INTFAPICALLBOOM_DECL &APICALLBOOM_PARAM,
    clsDerivedHelperSubmodules<TAPI::enuTokenActorType::API> &DerivedHelperSubmodules,
    QString _text,
    const TranslationDir_t& _dir,
    const QString& _engine,
    bool _useSpecialClass,
    bool _detailed, bool _detokenize,
    int& _preprocessTime,
    int& _translationTime
);

template QString MTHelper::preprocessText<TAPI::enuTokenActorType::API>(
    INTFAPICALLBOOM_IMPL &APICALLBOOM_PARAM,
    clsDerivedHelperSubmodules<TAPI::enuTokenActorType::API> &DerivedHelperSubmodules,
    const QString& _text,
    const QString& _lang
);

} // namespace Targoman::API::ModuleHelpers::MT
