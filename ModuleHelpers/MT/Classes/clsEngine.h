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

#ifndef TARGOMAN_API_MODULEHELPERS_MT_CLASSES_CLSENGINE_H
#define TARGOMAN_API_MODULEHELPERS_MT_CLASSES_CLSENGINE_H

#include <QString>
#include <QVariantMap>
#include <QUrl>
#include "libTargomanCommon/Macros.h"
#include "../MTDefs.hpp"
//#include "../Gateways/intfTranslatorGateway.hpp"
#include "Interfaces/Server/APICallBoom.h"
using namespace Targoman::API::Server;

namespace Targoman::API::ModuleHelpers::MT::Classes {

//using namespace Gateways;

//TARGOMAN_DEFINE_ENHANCED_ENUM(enuEngine,
TARGOMAN_DEFINE_ENUM(enuEngine,
                     NMT,
                     ENT,
                     ITT,
                     TST,
                     SMT,
                     )

struct stuEngineSpecs
{
    enuEngine::Type Engine;
    QString         SourceLang;
    QString         DestLang;
    QString         Class; //formal, informal, all
    QStringList     URLs;
    bool            SupportsIXML;
    QString         DriverName;
//    intfTranslatorGateway* Driver;

    stuEngineSpecs(const stuEngineSpecs& _other) :
        Engine(_other.Engine),
        SourceLang(_other.SourceLang),
        DestLang(_other.DestLang),
        Class(_other.Class),
        URLs(_other.URLs),
        SupportsIXML(_other.SupportsIXML),
        DriverName(_other.DriverName)
//        Driver(_other.Driver)
    { ; }

    stuEngineSpecs(
        enuEngine::Type _engine, // = enuEngine::Unknown,
        const QString& _sourceLang, // = {},
        const QString& _destLang, // = {},
        const QString& _class, // = {},
        const QStringList& _urls, // = QUrl(),
        bool _supportsIXML, // = true,
        const QString& _driverName
//        intfTranslatorGateway* _driver
    ) :
        Engine(_engine),
        SourceLang(_sourceLang),
        DestLang(_destLang),
        Class(_class),
        URLs(_urls),
        SupportsIXML(_supportsIXML),
        DriverName(_driverName)
//        Driver(_driver)
    { ; }

    inline QString fullName() {
        return this->makeFullName(enuEngine::toStr(this->Engine), this->SourceLang, this->DestLang, this->Class);
    }

    static inline QString makeFullName(
        const QString& _engine,
        const QString& _sourceLang = {},
        const QString& _destLang = {},
        const QString& _class = {}
    ) {
        return _class.isNull()
                ? QString("%1;%2_%3").arg(_engine).arg(_sourceLang).arg(_destLang)
                : QString("%1;%2;%3_%4").arg(_engine).arg(_class).arg(_sourceLang).arg(_destLang);
    }
};

class clsEngine
{
public:
    clsEngine(const stuEngineSpecs& _specs) :
        EngineSpecs(_specs)
     { ; }

public:
    QString fullName() { return this->EngineSpecs.fullName(); }
    const stuEngineSpecs& specs() { return this->EngineSpecs; }
    QVariantMap doTranslation(
            INTFAPICALLBOOM_DECL &APICALLBOOM_PARAM,
            const QString& _text,
            bool _detailed,
            bool _detokinize
            );

protected:
    stuEngineSpecs EngineSpecs;
};

} //namespace Targoman::API::ModuleHelpers::MT::Classes

#endif // TARGOMAN_API_MODULEHELPERS_MT_CLASSES_CLSENGINE_H
