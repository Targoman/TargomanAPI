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
 */

#ifndef TARGOMAN_API_MODULES_TEXTPROCESSOR_TEXTPROCESSOR_H
#define TARGOMAN_API_MODULES_TEXTPROCESSOR_TEXTPROCESSOR_H

#include "Interfaces/API/intfPureModule.h"

namespace Targoman {
namespace API {

class TextProcessor : public intfPureModule
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID INTFPUREMODULE_IID)
    Q_INTERFACES(Targoman::API::intfPureModule)
    TARGOMAN_DEFINE_API_MODULE(TextProcessor);

public:
    bool init();

private slots:
    QString REST(
        ,
        normalize,
        (
            const QString _text,
            const TAPI::ISO639_2_t& _lang,
            bool _useSpellCorrector = true
        ),
        "Normalizes input text"
    )

    QString REST(
        ,
        text2IXML,
        (
            const QString& _text,
            const TAPI::ISO639_2_t& _lang,
            bool _useSpellCorrector = true
        ),
        "Normalizes and tokenizes text and tags special fields in an inline-XML format"
    )

    QString REST(
        ,
        ixml2Text,
        (
            const QString& _ixml,
            bool _detokenize = true,
            bool _hinidiDigits = false,
            bool _arabicPunctuations = false,
            bool _breakSentences = false
        ),
        "Removes IXML tags injected by text2IXML API"
    )

    QString REST(
        ,
        tokenize,
        (
            const QString& _text,
            const TAPI::ISO639_2_t& _lang,
            bool _useSpellCorrector = true,
            bool _hindiNumerals = false,
            bool _arabicPunctuations = false
        ),
        "Tokenizes string without tagging special fields"
    )
};

}
}

#endif // TARGOMAN_API_MODULES_TEXTPROCESSOR_TEXTPROCESSOR_H
