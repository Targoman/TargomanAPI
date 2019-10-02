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
#ifndef TARGOMAN_API_CLASSES_INTFTRANSLATORENGINE_HPP
#define TARGOMAN_API_CLASSES_INTFTRANSLATORENGINE_HPP

#include <QString>
#include <QVariantMap>
#include <QUrl>
#include "libTargomanCommon/Macros.h"
#include "Modules/Translation/TranslationDefs.hpp"

namespace Targoman {
namespace API {
namespace Modules {
namespace Translation {
namespace Classes {


TARGOMAN_DEFINE_ENHANCED_ENUM(enuEngine,
                               NMT,
                               )
struct stuEngineSpecs{
    enuEngine::Type Engine;
    QString SourceLang;
    QString DestLang;
    QString Class;
    bool    SupportsIXML;
    QUrl    URL;

    stuEngineSpecs(enuEngine::Type _engine = enuEngine::Unknown,
                   const QString& _sourceLang = QString(),
                   const QString& _destLang = QString(),
                   const QString& _class = QString(),
                   bool _supportsIXML = true,
                   const QUrl& _url = QUrl()
                   ) :
        Engine(_engine),
        SourceLang(_sourceLang),
        DestLang(_destLang),
        Class(_class),
        SupportsIXML(_supportsIXML),
        URL(_url)
    {;}

    inline QString fullName(){
        return this->makeFullName(enuEngine::toStr(this->Engine), this->SourceLang, this->DestLang, this->Class);
    }

    static inline QString makeFullName(const QString& _engine,
                                const QString& _sourceLang = QString(),
                                const QString& _destLang = QString(),
                                const QString& _class = QString()){

        return _class.isNull() ?
                    QString("%1;%2_%3").arg(_engine).arg(_sourceLang).arg(_destLang) :
                    QString("%1;%2;%3_%4").arg(_engine).arg(_class).arg(_sourceLang).arg(_destLang);
    }
};

class intfTranslatorEngine{
public:
    explicit intfTranslatorEngine(const stuEngineSpecs& _specs) :
       EngineSpecs(_specs)
    {}

    virtual ~intfTranslatorEngine();
    virtual QVariantMap doTranslation(const QString& _text, bool _detailed, bool _detokinize) = 0;

    QString fullName(){return this->EngineSpecs.fullName();}

    inline const stuEngineSpecs& specs(){return this->EngineSpecs;}

protected:
    stuEngineSpecs EngineSpecs;
};

}
}
}
}
}
#endif // TARGOMAN_API_CLASSES_INTFTRANSLATORENGINE_HPP
