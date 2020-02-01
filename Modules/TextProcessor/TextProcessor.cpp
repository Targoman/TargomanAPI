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
#include "libTargomanCommon/Configuration/ConfigManager.h"
#include "Interfaces/NLP/TextProcessor.hpp"

namespace Targoman {
namespace API {

using namespace Targoman::NLPLibs;
using namespace Targoman::Common;
using namespace Targoman::Common::Configuration;

QString TextProcessor::apiNormalize(const QString _text, const TAPI::ISO639_2_t& _lang)
{
    return NLP::TextProcessor::instance().normalizeText(_text, _lang);
}

QString TextProcessor::apiText2IXML (const QString& _text,
                                         const TAPI::ISO639_2_t& _lang,
                                         bool _useSpellCorrector)
{
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

QString TextProcessor::apiIxml2Text(const QString& _ixml,
                                        const TAPI::ISO639_2_t& _lang,
                                        bool _detokenize,
                                        bool _hinidiDigits,
                                        bool _breakSentences)
{
    return NLP::TextProcessor::instance().ixml2Text(_ixml, _lang, _detokenize, _hinidiDigits, _breakSentences);
}

QString TextProcessor::apiTokenize (const QString& _text,
                                        const TAPI::ISO639_2_t& _lang,
                                        bool _useSpellCorrector)
{
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
                _useSpellCorrector && (_lang == "fa"),
                QList<enuTextTags::Type>(),
                SentenceBreakReplacements
                );

    return NLP::TextProcessor::instance().ixml2Text(_text, _lang, false, false, false);
}

}
}

