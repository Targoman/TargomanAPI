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

#ifndef TEST_TICKETING_HPP
#define TEST_TICKETING_HPP

#include <QRandomGenerator>
#include "Interfaces/Test/testCommon.hpp"
#include "Interfaces/AAA/clsJWT.hpp"
#include "ORM/actionLogs.hpp"
#include <cstdlib>
#include <unistd.h>

using namespace Targoman::API;
using namespace Targoman::API::AAA;

#include "Interfaces/ORM/QueryBuilders.h"
using namespace Targoman::API::ORM;

#include "Interfaces/AAA/Accounting_Defs.hpp"
using namespace Targoman::API::AAA::Accounting;

#include "Interfaces/Helpers/SecurityHelper.h"
using namespace Targoman::API::Helpers;

class testTicketing : public clsBaseTest
{
    Q_OBJECT

    void cleanupUnitTestData()
    {
        clsDAC DAC;
        DAC.execQuery("", "UPDATE AAA.tblUser SET usrStatus='R' WHERE usrEmail IN(?,?)", { UT_UserEmail, UT_AdminUserEmail });
    }

private slots:
    void initTestCase()
    {
        initUnitTestData(false);
    }

    void cleanupTestCase()
    {
        cleanupUnitTestData();
    }

    void Signup_user()
    {
        QVERIFY((gUserID = callAPI(RESTClientHelper::PUT,
                                   "Account/signup", {}, {
                                       {"emailOrMobile", UT_UserEmail},
                                       {"name", "unit"},
                                       {"family", "test"},
                                       {"pass", "df6d2338b2b8fce1ec2f6dda0a630eb0"},
                                       {"role", UT_RoleName}
                                   }).toMap().value("usrID").toULongLong()) > 0);
    }

//    void Signup_user_again()
//      {
//        QVERIFY((gUserID = callAPI(PUT,
//                                   "Account/signup", {}, {
//                                       {"emailOrMobile", UT_UserEmail},
//                                       {"name", "unit"},
//                                       {"family", "test"},
//                                       {"pass", "df6d2338b2b8fce1ec2f6dda0a630eb0"},
//                                       {"role", UT_RoleName}
//                                   }).toMap().value("usrID").toULongLong()) > 0);
//    }

    void Signup_admin()
    {
        QVERIFY((gAdminUserID = callAPI(RESTClientHelper::PUT,
                                        "Account/signup", {}, {
                                            {"emailOrMobile", UT_AdminUserEmail},
                                            {"name", "admin unit"},
                                            {"family", "test"},
                                            {"pass", "df6d2338b2b8fce1ec2f6dda0a630eb0"},
                                            {"role", UT_RoleName}
                                        }).toMap().value("usrID").toULongLong()) > 0);

        clsDAC DAC;
        DAC.execQuery("", "UPDATE tblUser SET tblUser.usr_rolID=? WHERE tblUser.usrID=?", { UT_AdminRoleID, gAdminUserID });
    }

    void ApproveEmail_user()
    {
        clsDAC DAC;
        QString Code = DAC.execQuery("", "SELECT aprApprovalCode FROM tblApprovalRequest WHERE apr_usrID=?",
        {gUserID}).toJson(true).object().value("aprApprovalCode").toString();

        DAC.execQuery("", "UPDATE tblApprovalRequest SET aprStatus = 'S' WHERE apr_usrID=?", { gUserID });

        QVERIFY(callAPI(RESTClientHelper::POST,
                        "Account/approveEmail", {}, {
                            { "uuid", Code }
                        }).toBool());
    }

    void ApproveEmail_admin()
    {
        clsDAC DAC;
        QString Code = DAC.execQuery("", "SELECT aprApprovalCode FROM tblApprovalRequest WHERE apr_usrID=?",
        {gAdminUserID}).toJson(true).object().value("aprApprovalCode").toString();

        DAC.execQuery("", "UPDATE tblApprovalRequest SET aprStatus = 'S' WHERE apr_usrID=?",
        {gAdminUserID});

        QVERIFY(callAPI(RESTClientHelper::POST,
                        "Account/approveEmail", {},{
                            {"uuid", Code}
                        }).toBool());
    }

    void Login_admin()
    {
        QJsonObject MultiJWT;
        QVERIFY((MultiJWT = callAPI(RESTClientHelper::POST,
                                "Account/login",{},{
                                    { "login", UT_AdminUserEmail },
                                    { "pass", "5d12d36cd5f66fe3e72f7b03cbb75333" },
                                    { "salt", "1234" },
                                }).toJsonObject()).size());

        gEncodedAdminJWT = MultiJWT.value("ssn").toString();
        gAdminJWT = QJsonDocument::fromJson(QByteArray::fromBase64(gEncodedAdminJWT.split('.').at(1).toLatin1())).object();

        QVERIFY(clsJWT(gAdminJWT).usrID() == gAdminUserID);
        QVERIFY(clsJWT(gAdminJWT).usrStatus() == TAPI::enuUserStatus::Active);
    }

    /***************************************************************************************/
    /***************************************************************************************/
    /***************************************************************************************/

};

#endif // TEST_TICKETING_HPP
