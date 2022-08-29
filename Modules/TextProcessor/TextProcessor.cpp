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

#include "TextProcessor.h"
#include "ModuleHelpers/NLP/TextProcessor.hpp"
//#include "libTargomanCommon/Configuration/ConfigManager.h"

namespace Targoman::API::TextProcessorModule {

using namespace Targoman::NLPLibs;
//using namespace Targoman::Common;
//using namespace Targoman::Common::Configuration;
using namespace Targoman::API::ModuleHelpers;

TARGOMAN_API_MODULE_IMPLEMENT(TextProcessor)

TextProcessor::TextProcessor() :
    intfPureModule("TextProcessor")
{ ; }

void TextProcessor::initializeModule() {
    NLP::TextProcessor::instance().init();
    //Targoman::Common::Configuration::ConfigManager::instance().configSettings());
}

QString IMPL_REST_GET_OR_POST(TextProcessor, normalize, (
    APICALLBOOM_TYPE_NO_JWT_IMPL &APICALLBOOM_PARAM,
    const QString _text,
    const TAPI::ISO639_2_t& _lang,
    bool _useSpellCorrector
)) {
    return NLP::TextProcessor::instance().normalizeText(_text, _useSpellCorrector ? _lang : TAPI::ISO639_2_t());
}

QString IMPL_REST_GET_OR_POST(TextProcessor, text2IXML, (
    APICALLBOOM_TYPE_NO_JWT_IMPL &APICALLBOOM_PARAM,
    const QString& _text,
    const TAPI::ISO639_2_t& _lang,
    bool _useSpellCorrector
)) {
    bool SpellCorrected;
    QList<stuIXMLReplacement> SentenceBreakReplacements;
    SentenceBreakReplacements.append(
                stuIXMLReplacement(
                    QRegularExpression("(\\s)([\\.\\?\\!])(\\s)"),
                    "\\1\\2\\3")); //"\\1\\2\\n\\3"

    return NLP::TextProcessor::instance().text2IXML(
                _text,
                SpellCorrected,
                _lang,
                _useSpellCorrector && _lang == "fa",
                QList<enuTextTags::Type>(),
                SentenceBreakReplacements
                );
}

QString IMPL_REST_GET_OR_POST(TextProcessor, ixml2Text, (
    APICALLBOOM_TYPE_NO_JWT_IMPL &APICALLBOOM_PARAM,
    const QString& _ixml,
    bool _detokenize,
    bool _hinidiDigits,
    bool _arabicPunctuations,
    bool _breakSentences
)) {
    return NLP::TextProcessor::instance().ixml2Text(_ixml, _detokenize, _hinidiDigits, _arabicPunctuations, _breakSentences);
}

QString IMPL_REST_GET_OR_POST(TextProcessor, tokenize, (
    APICALLBOOM_TYPE_NO_JWT_IMPL &APICALLBOOM_PARAM,
    const QString& _text,
    const TAPI::ISO639_2_t& _lang,
    bool _useSpellCorrector,
    bool _hindiNumerals,
    bool _arabicPunctuations
)) {
    bool SpellCorrected;
    QList<stuIXMLReplacement> SentenceBreakReplacements;
    SentenceBreakReplacements.append(
                stuIXMLReplacement(
                    QRegularExpression("(\\s)([\\.\\?\\!])(\\s)"),
                    "\\1\\2\\3")); //"\\1\\2\\n\\3"

    QString Tokenized = NLP::TextProcessor::instance().text2IXML(
                            _text,
                            SpellCorrected,
                            _lang,
                            _useSpellCorrector,
                            QList<enuTextTags::Type>(),
                            SentenceBreakReplacements
                            );

    return NLP::TextProcessor::instance().ixml2Text(Tokenized, false, _hindiNumerals, _arabicPunctuations, false);
}

} //namespace Targoman::API::TextProcessorModule
