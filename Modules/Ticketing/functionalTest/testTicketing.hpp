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

#include "Interfaces/DBM/QueryBuilders.h"
using namespace Targoman::API::DBM;

#include "Interfaces/AAA/Accounting_Defs.hpp"
using namespace Targoman::API::AAA;

#include "Interfaces/Helpers/SecurityHelper.h"
using namespace Targoman::API::Helpers;

class testTicketing : public clsBaseTest
{
    Q_OBJECT

    QString CreatedUserEmail;
    QString CreatedAdminEmail;

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

    /***************************************************************************************/
    /***************************************************************************************/
    /***************************************************************************************/
    void setupAccountFixture()
    {
        QT_TRY {
            QVariant Result = callAdminAPI(
                RESTClientHelper::POST,
                "Account/fixtureSetup"
            );

            qDebug() << "--------- Account fixtureSetup: " << Result;

            QVERIFY(Result.isValid());

            this->CreatedUserEmail = Result.toMap().value("User").toMap().value("email").toString();
            gUserID = Result.toMap().value("User").toMap().value("usrID").toULongLong();

            this->CreatedAdminEmail = Result.toMap().value("Admin").toMap().value("email").toString();
            gAdminUserID = Result.toMap().value("Admin").toMap().value("usrID").toULongLong();

        } QT_CATCH (const std::exception &exp) {
            QTest::qFail(exp.what(), __FILE__, __LINE__);
        }
    }

    void login_user()
    {
        QJsonObject MultiJWT;
        //5d12d36cd5f66fe3e72f7b03cbb75333 = MD5(1234 + df6d2338b2b8fce1ec2f6dda0a630eb0 # 987)
        QVERIFY((MultiJWT = callAPI(RESTClientHelper::POST,
                                "Account/login",{},{
                                    { "emailOrMobile", this->CreatedUserEmail },
                                    { "pass", "5d12d36cd5f66fe3e72f7b03cbb75333" },
                                    { "salt", "1234" },
                                }).toJsonObject()).size());

        gEncodedJWT = MultiJWT.value("ssn").toString();
        gJWT = QJsonDocument::fromJson(QByteArray::fromBase64(gEncodedJWT.split('.').at(1).toLatin1())).object();

        QVERIFY(clsJWT(gJWT).usrID() == gUserID);
        QVERIFY(clsJWT(gJWT).usrStatus() == TAPI::enuUserStatus::Active);
    }

    void login_admin()
    {
        QJsonObject MultiJWT;
        //5d12d36cd5f66fe3e72f7b03cbb75333 = MD5(1234 + df6d2338b2b8fce1ec2f6dda0a630eb0 # 987)
        QVERIFY((MultiJWT = callAPI(RESTClientHelper::POST,
                                "Account/login",{},{
                                    { "emailOrMobile", this->CreatedAdminEmail },
                                    { "pass", "5d12d36cd5f66fe3e72f7b03cbb75333" },
                                    { "salt", "1234" },
                                }).toJsonObject()).size());

        gEncodedAdminJWT = MultiJWT.value("ssn").toString();
        gAdminJWT = QJsonDocument::fromJson(QByteArray::fromBase64(gEncodedAdminJWT.split('.').at(1).toLatin1())).object();

        QVERIFY(clsJWT(gAdminJWT).usrID() == gAdminUserID);
        QVERIFY(clsJWT(gAdminJWT).usrStatus() == TAPI::enuUserStatus::Active);
    }

    /***************************************************************************************/
    void UploadFiles_save()
    {
//        static constexpr char APPLICATION_JSON_HEADER[] = "application/json";
//        static constexpr char APPLICATION_FORM_HEADER[] = "application/x-www-form-urlencoded";
//        static constexpr char MULTIPART_BOUNDARY_HEADER[] = "multipart/form-data; boundary=";

        QT_TRY {
            QVariant Result = callAdminAPI(
                RESTClientHelper::PUT,
                "Ticketing/UploadFiles/save",
                {},
                {},
                {},
                {
                    { "file", "../../README.md" },
                }
            );

        } QT_CATCH (const std::exception &exp) {
            QTest::qFail(exp.what(), __FILE__, __LINE__);
        }
    }

private:
private slots:
    /***************************************************************************************/
    /* cleanup *****************************************************************************/
    /***************************************************************************************/

    void cleanupAccountFixture()
    {
        QT_TRY {
            QVariant Result = callAdminAPI(
                RESTClientHelper::POST,
                "Account/fixtureCleanup",
                {},
                {}
            );

            qDebug() << "--------- Account fixtureCleanup: " << Result;

            QVERIFY(Result.isValid());

        } QT_CATCH (const std::exception &exp) {
            QTest::qFail(exp.what(), __FILE__, __LINE__);
        }
    }
};

#endif // TEST_TICKETING_HPP
