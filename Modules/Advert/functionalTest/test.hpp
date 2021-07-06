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

#ifndef TEST_ADVERT_HPP
#define TEST_ADVERT_HPP

#include "Interfaces/Test/testCommon.hpp"
#include "Interfaces/AAA/clsJWT.hpp"
#include "ORM/actionLogs.hpp"
#include <cstdlib>
#include <unistd.h>

using namespace Targoman::API;
using namespace Targoman::API::AAA;

#include "Interfaces/ORM/QueryBuilders.h"
using namespace Targoman::API::ORM;

#include "../moduleSrc/ORM/Locations.h"
using namespace Targoman::API::Advertisement;

class testAdvert: public clsBaseTest
{
    Q_OBJECT

private slots:
    void initTestCase() {
        initUnitTestData(false);
    }

    void cleanupTestCase() {
//        cleanupUnitTestData();
    }

    void Signup(){
        QVERIFY((gUserID = callAPI(PUT,
                                        "Account/signup", {}, {
                                            {"emailOrMobile", UT_UserEmail},
                                            {"name", "unit"},
                                            {"family", "test"},
                                            {"pass", "df6d2338b2b8fce1ec2f6dda0a630eb0"},
                                            {"role", UT_RoleName}
                                        }).toMap().value("usrID").toULongLong()) > 0);

        QVERIFY((gAdminUserID = callAPI(PUT,
                                        "Account/signup", {}, {
                                            {"emailOrMobile", UT_AdminUserEmail},
                                            {"name", "admin unit"},
                                            {"family", "test"},
                                            {"pass", "df6d2338b2b8fce1ec2f6dda0a630eb0"},
                                            {"role", UT_RoleName}
                                        }).toMap().value("usrID").toULongLong()) > 0);

        clsDAC DAC;
        DAC.execQuery("", "UPDATE tblUser SET tblUser.usr_rolID=? WHERE tblUser.usrID=?", {UT_AdminRoleID, gAdminUserID});
    }

    void ApproveEmail(){
        clsDAC DAC;
        QString Code = DAC.execQuery("", "SELECT aprApprovalCode FROM tblApprovalRequest WHERE apr_usrID=?",
        {gUserID}).toJson(true).object().value("aprApprovalCode").toString();

        DAC.execQuery("", "UPDATE tblApprovalRequest SET aprStatus = 'S' WHERE apr_usrID=?",
        {gUserID});

        QVERIFY(callAPI(POST,
                        "Account/approveEmail", {},{
                            {"uuid", Code}
                        }).toBool());
    }

    void ApproveAdminEmail() {
        clsDAC DAC;
        QString Code = DAC.execQuery("", "SELECT aprApprovalCode FROM tblApprovalRequest WHERE apr_usrID=?",
        {gAdminUserID}).toJson(true).object().value("aprApprovalCode").toString();

        DAC.execQuery("", "UPDATE tblApprovalRequest SET aprStatus = 'S' WHERE apr_usrID=?",
        {gAdminUserID});

        QVERIFY(callAPI(POST,
                        "Account/approveEmail", {},{
                            {"uuid", Code}
                        }).toBool());
    }

    void Login() {
        QJsonObject MultiJWT;
        QVERIFY((MultiJWT = callAPI(POST,
                                "Account/login",{},{
                                    {"login", UT_AdminUserEmail},
                                    {"pass", "5d12d36cd5f66fe3e72f7b03cbb75333"},
                                    {"salt", 1234},
                                }).toJsonObject()).size());

        gEncodedAdminJWT = MultiJWT.value("ssn").toString();
        gAdminJWT = QJsonDocument::fromJson(QByteArray::fromBase64(gEncodedAdminJWT.split('.').at(1).toLatin1())).object();

        QVERIFY(clsJWT(gAdminJWT).usrID() == gAdminUserID);
        QVERIFY(clsJWT(gAdminJWT).usrStatus() == TAPI::enuUserStatus::Active);
    }

    //test non privileged user for creating location: fail

    QVariant locationID;
    void getOrCreateLocation() {
        QVariantMap locationInfo = callAdminAPI(
            GET,
            "Advert/Locations",
            {
                { "filters", "locURL=http://www.abbasgholi.com + locPlaceCode=ABC"}
            }
        ).toMap();

        qDebug() << "--------- locationInfo: " << locationInfo;

        if (locationInfo.isEmpty() == false)
            locationID = locationInfo[tblLocations::locID];
        else
        {
            locationID = callAdminAPI(
                PUT,
                "Advert/Locations",
                {},
                {
                    { "locURL",        "http://www.abbasgholi.com" },
                    { "locPlaceCode",  "ABC" },
                }
            );
        }

        qDebug() << "--------- locationID: " << locationID;
    }

    QVariant bannerProductID;
    void getOrCreateProduct_banner() {
        QVariantMap productInfo = callAdminAPI(
            GET,
            "Advert/Products",
            {
                { "filters", "prdCode=p123" },
            }
        ).toMap();

        qDebug() << "--------- productInfo: " << productInfo;

        if (productInfo.isEmpty() == false)
            bannerProductID = productInfo[tblLocations::locID];
        else
        {
            bannerProductID = callAdminAPI(
                PUT,
                "Advert/Products",
                {},
                {
                    { "productCode",    "p123" },
                    { "productName",    "test product 123" },
                    { "locationID",     locationID }
                }
            );
        }

        qDebug() << "--------- bannerProductID: " << bannerProductID;
    }

    QVariant bannerSaleableID;
    void getOrCreateSaleable_banner() {
    }
private:
    void addToBasket() {
//        TAPI::stuPreVoucher intfRESTAPIWithAccounting::apiPOSTaddToBasket(
//        TAPI::stuPreVoucher voucher
        QVariant result = callAPI(
            POST,
            "Advert/addToBasket",
            {},
            {
                { "JWT", gAdminJWT },
                { "saleableCode", "slb code for test" },
                { "orderAdditives", {} },
                { "qty", 12 },
                { "discountCode", "abcd11" },
                { "referrer", "" },
                { "extraRefererParams", {} },
                { "lastPreVoucher", {} },
            }
        );
    }

    void cleanupSaleableData() {
    }

};

#endif // TEST_ADVERT_HPP
