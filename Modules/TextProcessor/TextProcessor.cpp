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

#include "TextProcessor.h"
#include "3rdParty/E4MT/src/clsFormalityChecker.h"
#include "libTargomanTextProcessor/TextProcessor.h"
#include "libTargomanCommon/Configuration/ConfigManager.h"
#include "Configs.h"

using namespace Targoman::Apps;
using namespace Targoman::NLPLibs;
using namespace Targoman::Common;
using namespace Targoman::Common::Configuration;
using namespace QHttp;

void TextProcessor::init()
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

QString TextProcessor::apiNormalize(const QString _text, const QHttp::ISO639_2_t& _lang)
{
    return TargomanTextProcessor::instance().normalizeText(_text, false, _lang);
}

QString TextProcessor::apiText2IXML (const QString& _text,
                                         const QHttp::ISO639_2_t& _lang,
                                         bool _useSpellCorrector)
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
                _useSpellCorrector && _lang == "fa",
                QList<enuTextTags::Type>(),
                SentenceBreakReplacements
                );
}

QString TextProcessor::apiIxml2Text(const QString& _ixml,
                                        const QHttp::ISO639_2_t& _lang,
                                        bool _detokenize,
                                        bool _hinidiDigits,
                                        bool _breakSentences)
{
    return TargomanTextProcessor::instance().ixml2Text(_ixml, _lang, _detokenize, _hinidiDigits, _breakSentences);
}

QString TextProcessor::apiTokenize (const QString& _text,
                                        const QHttp::ISO639_2_t& _lang,
                                        bool _useSpellCorrector)
{
    bool SpellCorrected;
    QList<stuIXMLReplacement> SentenceBreakReplacements;
    SentenceBreakReplacements.append(
                stuIXMLReplacement(
                    QRegularExpression("(\\s)([\\.\\?\\!])(\\s)"),
                    "\\1\\2\\3")); //"\\1\\2\\n\\3"

    QString Tokenized = TargomanTextProcessor::instance().text2IXML(
                _text,
                SpellCorrected,
                _lang,
                0,
                false,
                _useSpellCorrector && (_lang == "fa"),
                QList<enuTextTags::Type>(),
                SentenceBreakReplacements
                );

    return TargomanTextProcessor::instance().ixml2Text(_text, _lang, false, false, false);
}

TextProcessor::TextProcessor()
{
    this->registerMyRESTAPIs();
}



