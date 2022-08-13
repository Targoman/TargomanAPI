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
#ifndef TEST_ACCOUNT_ORM_SERVICE_HPP
#define TEST_ACCOUNT_ORM_SERVICE_HPP

#include "Interfaces/Test/testCommon.hpp"
#include "Interfaces/AAA/clsJWT.hpp"
using namespace Targoman::API::AAA;

class testService: public clsBaseTest
{
    Q_OBJECT

private slots:
    void Service_CREATE_Unprivileged() {
        QVERIFY(callUserAPI(RESTClientHelper::PUT, QString("Account/Service/"),{},{
                                {"svcName", UT_ServiceName},
                             }) == gInvalid);
    }

    void Service_CREATE_Admin() {
        QVERIFY(callAdminAPI(RESTClientHelper::PUT, QString("Account/Service/"),{},{
                                               {"svcName", UT_ServiceName},
                             }) == gInvalid);
        QVERIFY(callAdminAPI(RESTClientHelper::PUT, QString("Account/Service/"),{},{
                                 {"svcName", UT_ServiceName},
                                 {"svc_rolID", 0},
                             }) == gInvalid);
        QVERIFY((gServiceID =  callAdminAPI(RESTClientHelper::PUT, QString("Account/Service/"),{},{
                                               {"svcName", UT_ServiceName},
                                               {"svc_rolID", gServiceRoleID},
                             }).toUInt()) > 0);
    }

    void Service_UPDATE_Unprivileged() {
        QVERIFY(callUserAPI(RESTClientHelper::PATCH, QString("Account/Service/%1").arg(gServiceID),{},{
                                 {"svcStatus", "Removed"}
                             }) == gInvalid);
    }

    void Service_UPDATE_Admin() {
        QVERIFY(callAdminAPI(RESTClientHelper::PATCH, QString("Account/Service/%1").arg(gServiceID),{},{
                                 {"svcStatus", "Removed"}
                             }).toBool());
    }

    void Service_DELETE_Unprivileged() {
        QVERIFY(callUserAPI(RESTClientHelper::DELETE, QString("Account/Service/%1").arg(gServiceID)) == gInvalid);
    }

    void Service_DELETE_Admin() {
        QVERIFY(callAdminAPI(RESTClientHelper::DELETE, QString("Account/Service/")) == gInvalid);
        QVERIFY(callAdminAPI(RESTClientHelper::DELETE, QString("Account/Service/%1").arg(gServiceID)).toBool() == false);
        QVERIFY(callAdminAPI(RESTClientHelper::PATCH, QString("Account/Service/%1").arg(gServiceID),{},{
                                 {"svcStatus", "Active"}
                             }).toBool());
        QVERIFY(callAdminAPI(RESTClientHelper::DELETE, QString("Account/Service/%1").arg(gServiceID)).toBool());
        QVERIFY(callAdminAPI(RESTClientHelper::PATCH, QString("Account/Service/%1").arg(gServiceID),{},{
                                 {"svcStatus", "Active"}
                             }).toBool());
    }

    void Service_CREATE_recreate() {
        QVERIFY(callAdminAPI(RESTClientHelper::PUT, QString("Account/Service/"),{},{
                                               {"svcName", UT_ServiceName},
                                               {"svc_rolID", gServiceRoleID},
                             }) == gInvalid);
    }

    void Service_GET_Unprivileged() {
        QVERIFY(callUserAPI(RESTClientHelper::GET, QString("Account/Service/")).toMap().isEmpty());
        QVERIFY(callUserAPI(RESTClientHelper::GET,
                        QString("Account/Service/%1").arg(gServiceID), {
                            {"rolName",UT_ServiceName}, {"cols", "rolName"}
                        }) == gInvalid);
    }

    void Service_GET_Admin() {
        QVERIFY(callAdminAPI(RESTClientHelper::GET, QString("Account/Service/"),{{"cols", "svcName"}}).toMap().size());
        QVERIFY(callAdminAPI(RESTClientHelper::GET,
                        QString("Account/Service/%1").arg(gServiceID), {
                            {"cols", "svcName"}
                        }).toMap().value("svcName").toString() == UT_ServiceName);
    }
};


#endif // TEST_ACCOUNT_ORM_SERVICE_HPP
