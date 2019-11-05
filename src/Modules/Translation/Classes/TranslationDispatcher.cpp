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
#include <QVariantMap>
#include <QTime>
#include <QUrl>

#include "TranslationDispatcher.h"
#include "3rdParty/E4MT/src/clsFormalityChecker.h"
#include "libTargomanTextProcessor/TextProcessor.h"
#include "libTargomanCommon/Configuration/ConfigManager.h"
#include "Configs.h"
#include "../Engines/clsNMT.h"
#include "QHttp/QRESTServer.h"
#include "Helpers/AAA/AAA.hpp"

namespace Targoman {
namespace API {
namespace Modules {
namespace Translation {
namespace Classes {

using namespace Apps;
using namespace NLPLibs;
using namespace DBManager;
using namespace Common;
using namespace Common::Configuration;
using namespace Common::Configuration;
using namespace QHttp;
using namespace Targoman::API::Helpers::AAA;

TranslationDispatcher::~TranslationDispatcher()
{ ; }

QVariantMap TranslationDispatcher::doTranslation(const QJsonObject& _privInfo,
                                                 QString _text,
                                                 const TranslationDir_t& _dir,
                                                 const QString& _engine,
                                                 bool _useSpecialClass,
                                                 bool _detailed,
                                                 bool _detokenize,
                                                 int& _preprocessTime,
                                                 int& _translationTime)
{
    if(_detailed && Authorization::hasPriv(_privInfo, {TARGOMAN_PRIV_PREFIX + "Detailed"}) == false)
        throw exAuthorization("Not enought priviledges to get detailed translation response.");
    QTime CacheLookupTime; CacheLookupTime.start();
    QString CacheKey = QCryptographicHash::hash(QString("%1:%2:%3:%4:%5").arg(_useSpecialClass).arg(_engine, _dir.first, _dir.second, _text).toUtf8(),QCryptographicHash::Md4).toHex();
    QVariantMap CachedTranslation = this->TranslationCache[CacheKey];
    if(CachedTranslation.isEmpty()){
        QTime Timer;Timer.start();
        QString Class;

        intfTranslatorEngine* TranslationEngine  = nullptr;
        if(_useSpecialClass){
            Class = this->detectClass(_engine, _text, _dir.first);
            TranslationEngine = this->RegisteredEngines.value(stuEngineSpecs::makeFullName(_engine, _dir.first, _dir.second, Class));
        }
        if(TranslationEngine == nullptr)
            TranslationEngine = this->RegisteredEngines.value(stuEngineSpecs::makeFullName(_engine, _dir.first, _dir.second));

        if(TranslationEngine == nullptr)
            throw exHTTPInternalServerError("Unable to find generic translator engine");

        _text = this->preprocessText(_text, _dir.first);
        _preprocessTime = Timer.elapsed();Timer.restart();

        if(TranslationEngine->specs().SupportsIXML == false)
            _text = TargomanTextProcessor::instance().ixml2Text(_text, _dir.first, false, false,false);


        CachedTranslation = TranslationEngine->doTranslation(_text, _detailed, _detokenize);

        _translationTime = Timer.elapsed();
        if(Class.size())
            CachedTranslation[RESULTItems::CLASS] = Class;

        if(CachedTranslation.value(RESULTItems::ERRNO, 0).toInt() == 0)
            this->TranslationCache.insert(CacheKey, CachedTranslation);
        else{
            switch(CachedTranslation.value(RESULTItems::ERRNO, 0).toInt()){
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
    }else{
        CachedTranslation[RESULTItems::CACHE] = CacheLookupTime.elapsed();
    }
    return CachedTranslation;
}

QString TranslationDispatcher::detectClass(const QString& _engine, const QString& _text, const QString& _lang)
{
    Q_UNUSED(_engine);
    if(gConfigs::Classifier::SupportsIXML.value()== false)
        return this->FormalityChecker->check(_lang, TargomanTextProcessor::instance().ixml2Text(_text, _lang, false, false,false));
    else
        return this->FormalityChecker->check(_lang, _text);
}

QString TranslationDispatcher::preprocessText(const QString& _text, const QString& _lang)
{
    Q_UNUSED (_lang)
    if(this->CorrectionRule.isEmpty() || this->LastCorrectionRuleUpdateTime.elapsed() > 3600){
        clsDACResult Result ;//= this->DAC->execQueryCacheable(3600,QString(), "SELECT crlPattern, crlReplacement FROM MT.tblCorrectionRules WHERE crlType = 'R'");
        if(Result.isValid()){
            this->CorrectionRule.clear();
            while(Result.next())
                this->CorrectionRule.append(
                        qMakePair(
                            QRegularExpression(Result.value(0).toString(),
                                               QRegularExpression::UseUnicodePropertiesOption |
                                               QRegularExpression::MultilineOption
                                               ), Result.value(1).toString())
                        );
            this->LastCorrectionRuleUpdateTime.start();
        }
    }
    QString Text = _text;
    foreach(auto Rule, this->CorrectionRule)
        Text = Text.replace(Rule.first, Rule.second);

    return Text;
}

QString TranslationDispatcher::tokenize(const QString& _text, const QString& _lang)
{
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

QString TranslationDispatcher::detokenize(const QString& _text, const QString& _lang)
{
    return TargomanTextProcessor::instance().ixml2Text(_text, _lang, true, _lang=="fa" || _lang=="ar", false);
}

QVariantMap TranslationDispatcher::retrieveDicResponse(const QString& _text, const QString& _lang)
{
    Q_UNUSED(_text); Q_UNUSED (_lang);
    return QVariantMap();
}

void TranslationDispatcher::addDicLog(const QString& _lang, quint64 _wordCount, const QString& _text)
{
    Q_UNUSED(_text); Q_UNUSED (_lang); Q_UNUSED (_wordCount)
}

void TranslationDispatcher::addErrorLog(quint64 _aptID, const QString& _engine, const QString& _dir, quint64 _wordCount, const QString& _text, qint32 _errorCode)
{
    Q_UNUSED(_text); Q_UNUSED (_dir); Q_UNUSED (_wordCount);Q_UNUSED (_aptID); Q_UNUSED (_errorCode);Q_UNUSED (_engine)
}

void TranslationDispatcher::addTranslationLog(quint64 _aptID, const QString& _engine, const QString& _dir, quint64 _wordCount, const QString& _text, int _trTime)
{
    Q_UNUSED(_text); Q_UNUSED (_dir); Q_UNUSED (_wordCount);Q_UNUSED (_aptID);Q_UNUSED (_engine); Q_UNUSED (_trTime)
}

void TranslationDispatcher::registerEngines()
{
    foreach (const QString& Key, gConfigs::TranslationServers.keys()){
        const tmplConfigurableArray<gConfigs::Server>& ServersConfig =
                gConfigs::TranslationServers.values(Key);
        for(size_t i=0; i<ServersConfig.size(); ++i){
            gConfigs::Server Server = ServersConfig.at(i);
            if(Server.Active.value() == false)
                continue;

            intfTranslatorEngine* Engine = nullptr;
            switch(enuEngine::toEnum(Key)){
            case enuEngine::NMT:
                Engine = new Engines::clsBaseNMT(stuEngineSpecs(enuEngine::toEnum(Key),
                                                                   Server.SourceLang.value(),
                                                                   Server.DestLang.value(),
                                                                   Server.Class.value(),
                                                                   Server.SupportsIXML.value(),
                                                                   Server.URL.value())
                            );
                break;
            case enuEngine::Unknown:
                throw exTargomanInitialization("Invalid engine type");
            }

            if(Engine)
                this->RegisteredEngines.insert(stuEngineSpecs::makeFullName(Key, Server.SourceLang.value(), Server.DestLang.value(), Server.Class.value()),
                                               Engine
                                               );

        }

    }
}

TranslationDispatcher::TranslationDispatcher() :
    DAC(new DBManager::clsDAC),
    FormalityChecker(new clsFormalityChecker)
{
    TargomanTextProcessor::stuConfigs TPConfigs;
    TPConfigs.AbbreviationsFile = gConfigs::TextProcessor::AbbreviationFile.value();
    TPConfigs.NormalizationFile = gConfigs::TextProcessor::NormalizationFile.value();
    TPConfigs.SpellCorrectorBaseConfigPath = gConfigs::TextProcessor::SpellCorrectorBaseConfigPath.value();

    QSharedPointer<QSettings>  ConfigSettings = ConfigManager::instance().configSettings();

    if (ConfigSettings.isNull() == false){
        ConfigSettings->beginGroup(gConfigs::TextProcessor::SpellCorrectorLanguageBasedConfigs.configPath());
        foreach(const QString& Lang, ConfigSettings->childGroups()){
            foreach (const QString& Key, ConfigSettings->allKeys()){
                ConfigSettings->beginGroup(Lang);
                TPConfigs.SpellCorrectorLanguageBasedConfigs[Lang].insert(Key, ConfigSettings->value(Key));
                ConfigSettings->endGroup();
            }
        }
        ConfigSettings->endGroup();
    }

    TargomanTextProcessor::instance().init(TPConfigs);
}

/********************************************/
intfTranslatorEngine::~intfTranslatorEngine()
{;}

}
}
}
}
}
