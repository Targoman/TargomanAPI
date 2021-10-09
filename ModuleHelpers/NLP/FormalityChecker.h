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

#ifndef TARGOMAN_API_NLP_FORMALITYCHECKER_H
#define TARGOMAN_API_NLP_FORMALITYCHECKER_H

#include "libTargomanCommon/Configuration/tmplConfigurable.h"
#include "3rdParty/E4MT/src/clsFormalityChecker.h"

#include "Interfaces/Common/base.h"

namespace Targoman {
namespace Apps {
struct gConfigs{
    struct Classifier{
        static inline QString makeConfig(const QString& _name);
        static Targoman::Common::Configuration::tmplConfigurable<double>       Threshold;
        static Targoman::Common::Configuration::tmplConfigurable<FilePath_t>   ModelPath;
        static Targoman::Common::Configuration::tmplConfigurable<QString>      ModelPattern;
        static Targoman::Common::Configuration::tmplConfigurable<bool>         SupportsIXML;
    };
};
}

namespace API {
namespace ModuleHelpers {
namespace NLP {

class Q_DECL_EXPORT FormalityChecker
{
public:
    instanceGetter(FormalityChecker)
    inline QString check(const QString _lang, QString _text){return this->FCInstance->check(_lang, _text);}

private:
    FormalityChecker() : FCInstance(new Targoman::Apps::clsFormalityChecker) {}

    TARGOMAN_DISABLE_COPY(FormalityChecker)
private:
    QScopedPointer<Targoman::Apps::clsFormalityChecker> FCInstance;
};

}
}
}
}

#endif // TARGOMAN_API_NLP_FORMALITYCHECKER_H
