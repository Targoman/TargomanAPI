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

#ifndef TARGOMAN_API_MODULEHELPERS_MT_CLASSES_INTFTRANSLATORENGINE_HPP
#define TARGOMAN_API_MODULEHELPERS_MT_CLASSES_INTFTRANSLATORENGINE_HPP

#include <QString>
#include <QVariantMap>
#include <QUrl>
#include "libTargomanCommon/Macros.h"
#include "../MTDefs.hpp"

namespace Targoman::API::ModuleHelpers::MT::Classes {

class intfTranslatorEngine
{
public:
    explicit intfTranslatorEngine() { ; }
    virtual ~intfTranslatorEngine() { ; }

public:
    virtual QVariantMap doTranslation(const QString& _text, bool _detailed, bool _detokinize) = 0;

//    QString fullName() { return this->EngineSpecs.fullName(); }

//    inline const stuEngineSpecs& specs() { return this->EngineSpecs; }

//protected:
//    stuEngineSpecs EngineSpecs;
};

} //namespace Targoman::API::ModuleHelpers::MT::Classes

#endif // TARGOMAN_API_MODULEHELPERS_MT_CLASSES_INTFTRANSLATORENGINE_HPP
