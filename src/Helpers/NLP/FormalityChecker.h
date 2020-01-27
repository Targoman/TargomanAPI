/******************************************************************************
 * TargomanAAA: Authentication, Authorization, Accounting framework           *
 *                                                                            *
 * Copyright 2014-2019 by Targoman Intelligent Processing <http://tip.co.ir>  *
 *                                                                            *
 * TargomanAAA is free software: you can redistribute it and/or modify        *
 * it under the terms of the GNU Lesser General Public License as published   *
 * by the Free Software Foundation, either version 3 of the License, or       *
 * (at your option) any later version.                                        *
 *                                                                            *
 * TargomanAAA is distributed in the hope that it will be useful,             *
 * but WITHOUT ANY WARRANTY; without even the implied warranty of             *
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the              *
 * GNU Lesser General Public License for more details.                        *
 * You should have received a copy of the GNU Lesser General Public License   *
 * along with Targoman. If not, see <http://www.gnu.org/licenses/>.           *
 *                                                                            *
 ******************************************************************************/
/**
 * @author S. Mehran M. Ziabary <ziabary@targoman.com>
 */

#ifndef TARGOMAN_API_HELPERS_NLP_FORMALITYCHECKER_H
#define TARGOMAN_API_HELPERS_NLP_FORMALITYCHECKER_H

#include "3rdParty/E4MT/src/clsFormalityChecker.h"

namespace Targoman {
namespace API {
namespace Helpers {
namespace NLP {

class FormalityChecker {
public:
    static FormalityChecker& instance(){static FormalityChecker* Instance = nullptr; return *(Instance ? Instance : (Instance = new FormalityChecker));}
    QString check(const QString _lang, QString _text){
        return this->FCInstance->check(_lang, _text);
    };

private:
    FormalityChecker() :
        FCInstance(new Targoman::Apps::clsFormalityChecker)
    {}

    Q_DISABLE_COPY(FormalityChecker)
private:
    QScopedPointer<Targoman::Apps::clsFormalityChecker> FCInstance;
};

}
}
}
}
#endif // TARGOMAN_API_HELPERS_NLP_FORMALITYCHECKER_H
