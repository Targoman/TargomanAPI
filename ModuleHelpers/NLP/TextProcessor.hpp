/******************************************************************************
#   TargomanAPI: REST API for Targoman
#
#   Copyright 2014-2020 by Targoman Intelligent Processing <http://tip.co.ir>
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
 * @author S.Mehran M.Ziabary <ziabary@targoman.com>
 */

#ifndef TARGOMAN_API_NLP_TEXTPROCESSOR_H
#define TARGOMAN_API_NLP_TEXTPROCESSOR_H

#include "libTargomanCommon/Configuration/ConfigManager.h"
#include "libTargomanCommon/Macros.h"
#include "libTargomanTextProcessor/TextProcessor.h"

namespace Targoman {
namespace API {
namespace ModuleHelpers {
namespace NLP {

class TextProcessor
{
public:
    instanceGetter (TextProcessor)

    inline void init() { //QSharedPointer<QSettings> _configSettings = QSharedPointer<QSettings>()){
        NLPLibs::TargomanTextProcessor::instance().init(
                    Targoman::Common::Configuration::ConfigManager::instance().configSettings());
    }

    inline QString text2IXML(const QString& _inStr,
                             INOUT bool &_spellCorrected,
                             const QString& _lang,
                             bool _useSpellCorrector = true,
                             QList<NLPLibs::enuTextTags::Type> _removingTags = QList<NLPLibs::enuTextTags::Type>(),
                             QList<NLPLibs::stuIXMLReplacement> _replacements = QList<NLPLibs::stuIXMLReplacement>()) const{
        return NLPLibs::TargomanTextProcessor::instance().text2IXML(_inStr, _spellCorrected, _lang, 0, false, _useSpellCorrector, _removingTags, _replacements);
    }

    inline QString ixml2Text(const QString& _ixml,
                             bool _detokenize = true,
                             bool _hinidiDigits = false,
                             bool _arabicPunctuations = false,
                             bool _breakSentences = true) const{
        return NLPLibs::TargomanTextProcessor::instance().ixml2Text(_ixml, _detokenize, _hinidiDigits, _arabicPunctuations, _breakSentences);
    }

    inline QString normalizeText(const QString _input,
                                 const QString &_lang) const{
        bool SpellCorrected;
        return this->normalizeText(_input, SpellCorrected, _lang);
    }

    inline QString normalizeText(const QString _input,
                                 INOUT bool &_spellCorrected,
                                 const QString& _lang) const{
        return NLPLibs::TargomanTextProcessor::instance().normalizeText(_input, _spellCorrected, false, _lang);
    }

private:
    TextProcessor(){}
    Q_DISABLE_COPY(TextProcessor)
};

}
}
}
}

#endif // TARGOMAN_API_NLP_TEXTPROCESSOR_H
