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

#ifndef TARGOMAN_API_HELPERS_ORM_CLSRESTAPIWithActionLogs_HPP
#define TARGOMAN_API_HELPERS_ORM_CLSRESTAPIWithActionLogs_HPP

#include "QHttp/intfRESTAPIHolder.h"
#include "libTargomanDBM/clsDAC.h"
#include "Helpers/ORM/clsTable.h"

namespace Targoman {
namespace API {
namespace Helpers {
namespace ORM {

class clsRESTAPIWithActionLogs : protected clsTable {
    Q_OBJECT
public:
    clsRESTAPIWithActionLogs(DBManager::clsDAC& _dac, const QString& _schema, const QString& _module);

private slots:
    QVariant apiGETActionLogs(GET_METHOD_ARGS_HEADER);
    QString signOfGETActionLogs(){ return TARGOMAN_M2STR((GET_METHOD_ARGS_HEADER)); }
    QString docOfGETActionLogs(){ return "Get ActionLogs information"; }

private:
    DBManager::clsDAC& DAC;
    QString Module;
};
}
}
}
}

#endif // TARGOMAN_API_HELPERS_ORM_CLSRESTAPIWithActionLogs_HPP
