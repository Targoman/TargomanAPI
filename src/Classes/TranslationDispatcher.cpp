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
 @author S. Mohammad M. Ziabary <ziabary@targoman.com>
 */
#include <QVariantMap>
#include <QTime>

#include "TranslationDispatcher.h"
#include "libTargomanAAA/AAA.h"
#include "3rdParty/E4MT/src/clsFormalityChecker.h"
#include "libTargomanTextProcessor/TextProcessor.h"
#include "libTargomanCommon/Configuration/ConfigManager.h"

using namespace Targoman::NLPLibs;
using namespace Targoman::DBManager;
using namespace Targoman::Common;
using namespace Targoman::Common::Configuration;

namespace Targoman {
namespace Apps {
namespace Classes {

TranslationDispatcher::~TranslationDispatcher()
{ ; }

QVariantMap TranslationDispatcher::doTranslation(const QJsonObject& _privInfo,
                                                 const QString& _text,
                                                 const TranslationDir_t &_dir,
                                                 const QString &_engine,
                                                 bool _useSpecialClass,
                                                 bool _detailed)
{
    if(_detailed && AAA::hasPriv(_privInfo, {TARGOMAN_PRIV_PREFIX + "Detailed"}) == false)
        throw exAuthorization("Not enought priviledges to get detailed translation response.");

    QString CacheKey = QString("%1:%2:%3:%4:%5").arg(_useSpecialClass).arg(_engine, _dir.first, _dir.second, _text);
    QVariantMap CachedTranslation = this->TranslationCache[CacheKey];
    QString Class;
    if(CachedTranslation.isEmpty()){

        intfTranslatorEngine* TranslationEngine  = nullptr;
        if(_useSpecialClass){
            Class = this->detectClass(_engine, _text, _dir.first);
            TranslationEngine = this->RegisteredEngines.value(
                        intfTranslatorEngine::makeEngineName(_engine, _dir.first, _dir.second, Class)
                        );
        }
        if(TranslationEngine == nullptr)
            TranslationEngine = this->RegisteredEngines.value(intfTranslatorEngine::makeEngineName(_engine, _dir.first, _dir.second));

        CachedTranslation = TranslationEngine->doTranslation(this->preprocessText(_text, _dir.first), _detailed);
        if(Class.size())
            CachedTranslation["class"] = Class;
    }else{
        CachedTranslation["cache"] = true;
    }

    return CachedTranslation;
}

QString TranslationDispatcher::detectClass(const QString& _engine, const QString &_text, const QString &_lang)
{
    Q_UNUSED(_engine);
    return this->FormalityChecker->check(_lang, _text);
}

QString TranslationDispatcher::preprocessText(const QString &_text, const QString &_lang)
{
    Q_UNUSED (_lang)
    if(this->CorrectionRule.isEmpty() || this->LastCorrectionRuleUpdateTime.elapsed() > 3600){
        clsDACResult Result = this->DAC->execQueryCacheable(3600,QString(), "SELECT crlPattern, crlReplacement FROM MT.tblCorrectionRules WHERE crlType = 'R'");
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

QString TranslationDispatcher::tokenize(const QString &_text, const QString &_lang)
{
    bool SpellCorrected;
    QList<stuIXMLReplacement> SentenceBreakReplacements;
    SentenceBreakReplacements.append(
                stuIXMLReplacement(
                    QRegExp("(\\s)([\\.\\?\\!])(\\s)"),
                    "\\1\\2\n\\3"));

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

QString TranslationDispatcher::detokenize(const QString &_text, const QString &_lang)
{
    return TargomanTextProcessor::instance().ixml2Text(_text, _lang, true, _lang=="fa" || _lang=="ar", false);
}

QVariantMap TranslationDispatcher::retrieveDicResponse(const QString &_text, const QString &_lang)
{
    Q_UNUSED(_text); Q_UNUSED (_lang);
    return QVariantMap();
}

void TranslationDispatcher::addDicLog(const QString &_lang, quint64 _wordCount, const QString &_text)
{
    Q_UNUSED(_text); Q_UNUSED (_lang); Q_UNUSED (_wordCount)
}

void TranslationDispatcher::addErrorLog(quint64 _aptID, const QString &_engine, const QString &_dir, quint64 _wordCount, const QString &_text, qint8 _errorCode)
{
    Q_UNUSED(_text); Q_UNUSED (_dir); Q_UNUSED (_wordCount);Q_UNUSED (_aptID); Q_UNUSED (_errorCode);Q_UNUSED (_engine)
}

void TranslationDispatcher::addTranslationLog(quint64 _aptID, const QString &_engine, const QString &_dir, quint64 _wordCount, const QString &_text, int _trTime)
{
    Q_UNUSED(_text); Q_UNUSED (_dir); Q_UNUSED (_wordCount);Q_UNUSED (_aptID);Q_UNUSED (_engine); Q_UNUSED (_trTime)
}

TranslationDispatcher::TranslationDispatcher() :
    DAC(new DBManager::clsDAC),
    FormalityChecker(new clsFormalityChecker)
{
    TargomanTextProcessor::instance().init(ConfigManager::instance().configSettings());
}

/********************************************/
intfTranslatorEngine::~intfTranslatorEngine()
{;}

}
}
}
