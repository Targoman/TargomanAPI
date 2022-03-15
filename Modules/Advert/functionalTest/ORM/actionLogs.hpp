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
 * @author Kambiz Zandi <kambizzandi@gmail.com>
 */

#ifndef TEST_ADVERT_ORM_ACTIVELOGS_HPP
#define TEST_ADVERT_ORM_ACTIVELOGS_HPP

#include "Interfaces/Test/testCommon.hpp"
#include "Interfaces/AAA/clsJWT.hpp"
using namespace Targoman::API::AAA;

class testActionLogs: public clsBaseTest
{
    Q_OBJECT

public:
    testActionLogs(const QString &_dbPrefix) : clsBaseTest(_dbPrefix) {}

private slots:
    void unprivActionLogs(){
        QVERIFY(callAPI(RESTClientHelper::GET, "Advert/ActionLogs").toString().isEmpty());
    }
    void privActionLogs(){
        QVERIFY(callAdminAPI(RESTClientHelper::GET, "Advert/ActionLogs").toString().isEmpty());
    }
};

#endif // TEST_ADVERT_ORM_ACTIVELOGS_HPP
