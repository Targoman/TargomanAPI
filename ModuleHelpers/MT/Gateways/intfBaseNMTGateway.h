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

#ifndef TARGOMAN_API_MODULEHELPERS_MT_GATEWAYS_INTFBASENMTGATEWAY_H
#define TARGOMAN_API_MODULEHELPERS_MT_GATEWAYS_INTFBASENMTGATEWAY_H

#include <QUrl>
#include "intfTranslatorGateway.hpp"
#include "Classes/clsEngine.h"

namespace Targoman::API::ModuleHelpers::MT::Gateways {

using namespace Classes;

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-variable"
namespace NMTResponse {
    TARGOMAN_CREATE_CONSTEXPR(rslt);
    TARGOMAN_CREATE_CONSTEXPR(serverName);

    namespace Result {
        TARGOMAN_CREATE_CONSTEXPR(phrases);
        TARGOMAN_CREATE_CONSTEXPR(alignments);
        TARGOMAN_CREATE_CONSTEXPR(tokens);
    }
}
#pragma GCC diagnostic pop

class intfBaseNMTGateway : public intfTranslatorGateway
{
public:
    intfBaseNMTGateway();

public:
    QVariantMap doTranslation(
            INTFAPICALLCONTEXT_DECL &_apiCallContext,
            const stuEngineSpecs& _engineSpecs,
            const QString& _text,
            bool _detailed,
            bool _detokenize
            );

protected:
//    virtual QString preprocessText(QString _sourceText);
    virtual QVariantList makeSrcSentences(const QString &_sourceText);
    virtual QVariantMap buildProperResponse(const stuEngineSpecs& _engineSpecs, const QJsonDocument& _doc, bool _detailed, bool _detok);
};

} //namespace Targoman::API::ModuleHelpers::MT::Gateways

#endif // TARGOMAN_API_MODULEHELPERS_MT_GATEWAYS_INTFBASENMTGATEWAY_H
