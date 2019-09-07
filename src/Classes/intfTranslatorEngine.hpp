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
#ifndef TARGOMAN_APPS_CLASSES_INTFTRANSLATORENGINE_HPP
#define TARGOMAN_APPS_CLASSES_INTFTRANSLATORENGINE_HPP

#include <QString>
#include <QVariantMap>

namespace Targoman {
namespace Apps {
namespace Classes {

class intfTranslatorEngine{
public:
    virtual ~intfTranslatorEngine();
    virtual QVariantMap doTranslation(const QString& _text, bool _detailed) = 0;

    static inline QString makeEngineName(const QString& _engine, const QString& _sourceLang, const QString& _destLang){
        return QString("%1:%2_%3").arg(_engine).arg(_sourceLang).arg(_destLang);
    }
};

}
}
}
#endif // TARGOMAN_APPS_CLASSES_INTFTRANSLATORENGINE_HPP
