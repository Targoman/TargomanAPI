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
#ifndef TEST_ACCOUNT_ORM_ROLES_HPP
#define TEST_ACCOUNT_ORM_ROLES_HPP

#include "Interfaces/Test/testCommon.hpp"

#include "Interfaces/AAA/clsJWT.hpp"
using namespace Targoman::API::AAA;

class testRoles: public clsBaseTest
{
    Q_OBJECT

private slots:
    void Roles_CREATE_Unprivileged() {
        QVERIFY(callUserAPI(RESTClientHelper::PUT, QString("Account/Roles/"),{},{
                                {"rolName", UT_ServiceRoleName},
                             }) == gInvalid);
    }

    void Roles_CREATE_Admin() {
        QVERIFY(callAdminAPI(RESTClientHelper::PUT, QString("Account/Roles/"),{},{
                                {"rolName", UT_ServiceRoleName},
                             }) == gInvalid);
        QVERIFY(callAdminAPI(RESTClientHelper::PUT, QString("Account/Roles/"),{},{
                                {"rolName", UT_ServiceRoleName},
                                 {"rolPrivileges", QJsonArray({"12"})}
                             }) == gInvalid);
        QVERIFY(callAdminAPI(RESTClientHelper::PUT, QString("Account/Roles/"),{},{
                                {"rolName", UT_ServiceRoleName},
                                 {"rolPrivileges", QJsonObject()}
                             }) == gInvalid);
        QVERIFY(callAdminAPI(RESTClientHelper::PUT, QString("Account/Roles/"),{},{
                                {"rolName", UT_ServiceRoleName},
                                 {"rolPrivileges", QJsonObject({{"ALL",1}})}
                             }) == gInvalid);
        QVERIFY((gServiceRoleID = callAdminAPI(RESTClientHelper::PUT, QString("Account/Roles/"),{},{
                                {"rolName", UT_ServiceRoleName},
                                 {"rolPrivileges", QJsonObject({{UT_ServiceName,QJsonObject({{"All", 1}})}})}
                             }).toUInt()) > 0);
    }

    void Roles_UPDATE_Unprivileged() {
        QVERIFY(callUserAPI(RESTClientHelper::PATCH, QString("Account/Roles/%1").arg(gServiceRoleID),{},{
                                 {"rolPrivileges", QJsonObject({{UT_ServiceName,QJsonObject({{"All", 1}})}})}
                             }) == gInvalid);
    }

    void Roles_UPDATE_Admin() {
        QVERIFY(callAdminAPI(RESTClientHelper::PATCH, QString("Account/Roles/%1").arg(gServiceRoleID),{},{
                             }) == gInvalid);
        QVERIFY(callAdminAPI(RESTClientHelper::PATCH, QString("Account/Roles/%1").arg(gServiceRoleID),{},{
                                 {"rolPrivileges", QJsonArray({"12"})}
                             }) == gInvalid);
        QVERIFY(callAdminAPI(RESTClientHelper::PATCH, QString("Account/Roles/%1").arg(gServiceRoleID),{},{
                                 {"rolPrivileges", QJsonObject()}
                             }) .toBool());
        QVERIFY(callAdminAPI(RESTClientHelper::PATCH, QString("Account/Roles/%1").arg(gServiceRoleID),{},{
                                 {"rolPrivileges", QJsonObject({{"ALL",1}})}
                             }) == gInvalid);
        QVERIFY(callAdminAPI(RESTClientHelper::PATCH, QString("Account/Roles/%1").arg(gServiceRoleID),{},{
                                 {"rolPrivileges", QJsonObject({{UT_ServiceName,QJsonObject({{"All", 1}})}})}
                             }).toBool());
    }

    void Roles_DELETE_Unprivileged() {
        QVERIFY(callUserAPI(RESTClientHelper::DELETE, QString("Account/Roles/%1").arg(gServiceRoleID)) == gInvalid);
    }

    void Roles_DELETE_Admin() {
        QVERIFY(callAdminAPI(RESTClientHelper::DELETE, QString("Account/Roles/")) == gInvalid);
        QVERIFY(callAdminAPI(RESTClientHelper::DELETE, QString("Account/Roles/%1").arg(gServiceRoleID)).toBool());
    }

    void Roles_GET_Unprivileged() {
        QVERIFY(callUserAPI(RESTClientHelper::GET, QString("Account/Roles/")) == gInvalid);
        QVERIFY(callUserAPI(RESTClientHelper::GET,
                        QString("Account/Roles/%1").arg(gServiceRoleID), {
                            {"rolName",UT_ServiceRoleName}, {"cols", "rolName"}
                        }) == gInvalid);
    }

    void Roles_GET_Admin() {
        QVERIFY(callAdminAPI(RESTClientHelper::GET, QString("Account/Roles/"),{{"cols", "rolName"}}).toMap().size());
        QVERIFY(callAdminAPI(RESTClientHelper::GET,
                        QString("Account/Roles/%1").arg(gServiceRoleID), {
                            {"rolName",UT_ServiceRoleName}, {"cols", "rolName"}
                        }).toMap().value("rolName").toString() == UT_ServiceRoleName);
    }
};

#endif // TEST_ACCOUNT_ORM_ROLES_HPP
