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

#include "clsActionLogs.h"
#include "Helpers/AAA/AAA.hpp"

namespace Targoman {
namespace API {
namespace Helpers {
namespace ORM {

QVariant clsRESTAPIWithActionLogs::apiGETActionLogs(GET_METHOD_ARGS_IMPL)
{
    Authorization::checkPriv(_JWT,{this->Module + ":ActiveAds:CRUD~0100"});
    return this->selectFromTable(this->DAC, {}, {}, GET_METHOD_CALL_ARGS);
}

clsRESTAPIWithActionLogs::clsRESTAPIWithActionLogs(DBManager::clsDAC& _dac, const QString& _schema, const QString& _module) :
    clsTable(_schema,
            "tblActionLogs",
            { ///<ColName             Type            Validation                      RO   Sort  Filter  PK
              {"atlID",               T(quint64),     QFV.integer().minValue(1),      true, true, true, true},
              {"atlBy_usrID",         T(quint32),     QFV.integer().minValue(1),      true},
              {"atlInsertionDateTime",T(QHttp::DateTime_t),   QFV,                            true},
              {"atlType",             T(QString),     QFV.asciiAlNum().maxLenght(50), true},
              {"atlDescription",      T(QString),     QFV.allwaysInvalid(),           true, false,false},
            },
            {
                {"atlBy_usrID",        "AAA.tblUser",      "usrID",     "By"},
            }),
    DAC(_dac),
    Module(_module)
{
}

}
}
}
}
