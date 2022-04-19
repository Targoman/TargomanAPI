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

#ifndef TEST_JWT_HPP
#define TEST_JWT_HPP

#include <QRandomGenerator>
#include "Interfaces/Test/testCommon.hpp"
#include "Interfaces/AAA/clsJWT.hpp"
#include "ORM/actionLogs.hpp"
#include <cstdlib>
#include <unistd.h>

using namespace Targoman::API;
using namespace Targoman::API::AAA;

#include "Interfaces/Helpers/SecurityHelper.h"
using namespace Targoman::API::Helpers;

class testJWT : public clsBaseTest
{
    Q_OBJECT

public:
    testJWT(const QString &_dbPrefix) : clsBaseTest(_dbPrefix) { ; }

    void cleanupUnitTestData() {
        clsDAC DAC;
        DAC.execQuery("", QString("UPDATE %1AAA.tblUser SET usrStatus='R' WHERE usrEmail IN(?,?)").arg(this->DBPrefix), { UT_UserEmail, UT_AdminUserEmail });
    }

    QString LastRandomNumber;

//    QString CreatedUserEmail;
//    QString CreatedAdminEmail;

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

private slots:
    void initTestCase() {
        initUnitTestData(false);
    }

    void cleanupTestCase() {
        gEncodedAdminJWT = "";
        gEncodedJWT = "";
        cleanupUnitTestData();
    }

    /***************************************************************************************/
    void setupAccountFixture() {
        QT_TRY {
            QVariant Result = callAdminAPI(
                RESTClientHelper::POST,
                "Account/fixtureSetup",
                {},
                {
                    { "random", 1 },
                });

            QVERIFY(Result.isValid());

            this->LastRandomNumber = Result.toMap().value("Random").toString();

        } QT_CATCH (const std::exception &exp) {
            QTest::qFail(exp.what(), __FILE__, __LINE__);
        }
    }

    /***************************************************************************************/


    /***************************************************************************************/
    void cleanupAccountFixture() {
        QT_TRY {
            QVariant Result = callAdminAPI(
                RESTClientHelper::POST,
                "Account/fixtureCleanup",
                {},
                {
                    { "random", this->LastRandomNumber },
                });

            QVERIFY(Result.isValid());

        } QT_CATCH (const std::exception &exp) {
            QTest::qFail(exp.what(), __FILE__, __LINE__);
        }
    }

};

#endif // TEST_JWT_HPP
