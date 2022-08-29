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

#ifndef TARGOMAN_API_MODULES_TEXTPROCESSOR_TEXTPROCESSOR_H
#define TARGOMAN_API_MODULES_TEXTPROCESSOR_TEXTPROCESSOR_H

#include "Interfaces/API/intfPureModule.h"

namespace Targoman::API::TextProcessorModule {

class TextProcessor : public intfPureModule
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID INTFPUREMODULE_IID)
    Q_INTERFACES(Targoman::API::API::intfPureModule)
    //---------------------------------------------------------
    TARGOMAN_API_MODULE_DEFINE(TextProcessor, enuTokenActorType::User);
    //---------------------------------------------------------

public:
    virtual void initializeModule();

private slots:
    QString REST_GET_OR_POST(
        normalize,
        (
            APICALLBOOM_TYPE_NO_JWT_DECL &APICALLBOOM_PARAM,
            const QString _text,
            const TAPI::ISO639_2_t& _lang,
            bool _useSpellCorrector = true
        ),
        "Normalizes input text"
    )

    QString REST_GET_OR_POST(
        text2IXML,
        (
            APICALLBOOM_TYPE_NO_JWT_DECL &APICALLBOOM_PARAM,
            const QString& _text,
            const TAPI::ISO639_2_t& _lang,
            bool _useSpellCorrector = true
        ),
        "Normalizes and tokenizes text and tags special fields in an inline-XML format"
    )

    QString REST_GET_OR_POST(
        ixml2Text,
        (
            APICALLBOOM_TYPE_NO_JWT_DECL &APICALLBOOM_PARAM,
            const QString& _ixml,
            bool _detokenize = true,
            bool _hinidiDigits = false,
            bool _arabicPunctuations = false,
            bool _breakSentences = false
        ),
        "Removes IXML tags injected by text2IXML API"
    )

    QString REST_GET_OR_POST(
        tokenize,
        (
            APICALLBOOM_TYPE_NO_JWT_DECL &APICALLBOOM_PARAM,
            const QString& _text,
            const TAPI::ISO639_2_t& _lang,
            bool _useSpellCorrector = true,
            bool _hindiNumerals = false,
            bool _arabicPunctuations = false
        ),
        "Tokenizes string without tagging special fields"
    )
};

} //namespace Targoman::API::TextProcessorModule

#endif // TARGOMAN_API_MODULES_TEXTPROCESSOR_TEXTPROCESSOR_H
