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

#ifndef TEST_ADVERTFIXTURE_HPP
#define TEST_ADVERTFIXTURE_HPP

#include <QRandomGenerator>
#include "Interfaces/Test/testCommon.hpp"
#include "Interfaces/AAA/clsJWT.hpp"
#include "ORM/actionLogs.hpp"
#include <cstdlib>
#include <unistd.h>

using namespace Targoman::API;
using namespace Targoman::API::AAA;

//#include "Interfaces/DBM/QueryBuilders.h"
//using namespace Targoman::API::DBM;

//#include "Interfaces/AAA/Accounting_Defs.hpp"
//using namespace Targoman::API::AAA;

//#include "../moduleSrc/ORM/Accounting.h"
//#include "../moduleSrc/ORM/Locations.h"
//using namespace Targoman::API::AdvertModule;
//using namespace Targoman::API::AdvertModule::ORM;

//#include "Interfaces/Helpers/SecurityHelper.h"
//using namespace Targoman::API::Helpers;

class testAdvertFixture : public clsBaseTest
{
    Q_OBJECT

public:
    testAdvertFixture(const QString &_dbPrefix) : clsBaseTest(_dbPrefix) {}

    QString LastRandomNumber;
    QString CreatedUserEmail;
    QString CreatedAdminEmail;

//    QVariant LocationID;
//    QVariant BannerProductID;
//    QVariant BannerSaleableID;
//    QString BannerProductCode;
//    QString BannerSaleableCode;
//    //    QVariant PaymentGatewayID;
//    QString CouponCode;
//    QVariant CouponID;
//    Targoman::API::AAA::stuPreVoucher LastPreVoucher;
//    Targoman::API::AAA::stuVoucher Voucher;
//    Targoman::API::AAA::stuVoucher ApproveOnlinePaymentVoucher;

    void cleanupUnitTestData()
    {
        clsDAC DAC;
        DAC.execQuery("", QString("UPDATE %1AAA.tblUser SET usrStatus='R' WHERE usrEmail IN(?,?)").arg(this->DBPrefix), { UT_UserEmail, UT_AdminUserEmail });
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
                "Account/fixtureSetup",
                {},
                {
                    { "random", 1 },
                });

            qDebug() << "--------- Account fixtureSetup: " << Result;

            QVERIFY(Result.isValid());

            this->LastRandomNumber = Result.toMap().value("Random").toString();

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

    void setupAdvertFixture()
    {
        QT_TRY {
            QVariant Result = callAdminAPI(
                RESTClientHelper::POST,
                "Advert/fixtureSetup",
                {},
                {
                    { "random", this->LastRandomNumber },
                });

            qDebug() << "--------- Advert fixtureSetup: " << Result;

            QVERIFY(Result.isValid());

        } QT_CATCH (const std::exception &exp) {
            QTest::qFail(exp.what(), __FILE__, __LINE__);
        }
    }

private:
private slots:
    /***************************************************************************************/
    /* cleanup *****************************************************************************/
    /***************************************************************************************/
    void cleanupAdvertFixture()
    {
        QT_TRY {
            QVariant Result = callAdminAPI(
                RESTClientHelper::POST,
                "Advert/fixtureCleanup",
                {},
                {
                    { "random", this->LastRandomNumber },
                });

            qDebug() << "--------- Advert fixtureCleanup: " << Result;

            QVERIFY(Result.isValid());

        } QT_CATCH (const std::exception &exp) {
            QTest::qFail(exp.what(), __FILE__, __LINE__);
        }
    }

    void cleanupAccountFixture()
    {
        QT_TRY {
            QVariant Result = callAdminAPI(
                RESTClientHelper::POST,
                "Account/fixtureCleanup",
                {},
                {
                    { "random", this->LastRandomNumber },
                });

            qDebug() << "--------- Account fixtureCleanup: " << Result;

            QVERIFY(Result.isValid());

        } QT_CATCH (const std::exception &exp) {
            QTest::qFail(exp.what(), __FILE__, __LINE__);
        }
    }

};

#endif // TEST_ADVERTFIXTURE_HPP
