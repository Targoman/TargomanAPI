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

#include "Interfaces/AAA/Accounting_Defs.hpp"
using namespace Targoman::API::AAA::Accounting;

#include "../moduleSrc/ORM/Accounting.h"
#include "../moduleSrc/ORM/Locations.h"
using namespace Targoman::API::Advertisement;


namespace TAPI {
TARGOMAN_DEFINE_ENUM(enuPaymentGatewayType,
                     COD                        = 'D', //offline payment
                     IranBank                   = 'I',
                     IranIntermediateGateway    = 'M',
                     InternationalDebitCart     = 'D',
                     InternationalCreditCart    = 'C',
                     CryptoCurrency             = 'B',
                     DevelopersTest             = '-',
                     );
}

class testAdvert : public clsBaseTest
{
    Q_OBJECT

    QVariant locationID;
    QVariant bannerProductID;
    QVariant bannerSaleableID;
    QVariant couponID;
    QVariant lastPreVoucher;

    void cleanupUnitTestData() {
        clsDAC DAC;
        DAC.execQuery("", "UPDATE AAA.tblUser SET usrStatus='R' WHERE usrEmail IN(?,?)", { UT_UserEmail, UT_AdminUserEmail });
    }

private slots:
    void initTestCase() {
        initUnitTestData(false);
    }

    void cleanupTestCase() {
        cleanupUnitTestData();
    }

    void Signup_user() {
        QVERIFY((gUserID = callAPI(PUT,
                                   "Account/signup", {}, {
                                       {"emailOrMobile", UT_UserEmail},
                                       {"name", "unit"},
                                       {"family", "test"},
                                       {"pass", "df6d2338b2b8fce1ec2f6dda0a630eb0"},
                                       {"role", UT_RoleName}
                                   }).toMap().value("usrID").toULongLong()) > 0);
    }

//    void Signup_user_again() {
//        QVERIFY((gUserID = callAPI(PUT,
//                                   "Account/signup", {}, {
//                                       {"emailOrMobile", UT_UserEmail},
//                                       {"name", "unit"},
//                                       {"family", "test"},
//                                       {"pass", "df6d2338b2b8fce1ec2f6dda0a630eb0"},
//                                       {"role", UT_RoleName}
//                                   }).toMap().value("usrID").toULongLong()) > 0);
//    }

    void Signup_admin() {
        QVERIFY((gAdminUserID = callAPI(PUT,
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

    void ApproveEmail_user() {
        clsDAC DAC;
        QString Code = DAC.execQuery("", "SELECT aprApprovalCode FROM tblApprovalRequest WHERE apr_usrID=?",
        {gUserID}).toJson(true).object().value("aprApprovalCode").toString();

        DAC.execQuery("", "UPDATE tblApprovalRequest SET aprStatus = 'S' WHERE apr_usrID=?", { gUserID });

        QVERIFY(callAPI(POST,
                        "Account/approveEmail", {}, {
                            { "uuid", Code }
                        }).toBool());
    }

    void ApproveEmail_admin() {
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

    void Login_admin() {
        QJsonObject MultiJWT;
        QVERIFY((MultiJWT = callAPI(POST,
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

    /*
    void createDiscount() {
        callAdminAPI(
            PUT,
            "Advert/AccountCoupons",
            {},
            {
                { tblAccountCouponsBase::cpnCode, "cpn-code-aaa" },
//                    { tblAccountCouponsBase::cpnPrimaryCount,           S(quint32),                        QFV.integer().minValue(1),               1,         UPAdmin},
//                    { tblAccountCouponsBase::cpnTotalMaxAmount,         S(quint32),                        QFV.integer().minValue(1),               1,         UPAdmin},
//                    { tblAccountCouponsBase::cpnPerUserMaxCount,        S(NULLABLE_TYPE(quint32)),         QFV.optional(QFV.integer().minValue(1)), QNull,     UPAdmin},
//                    { tblAccountCouponsBase::cpnPerUserMaxAmount,       S(NULLABLE_TYPE(quint32)),         QFV.integer().minValue(1),               QNull,     UPAdmin},
                { tblAccountCouponsBase::cpnValidFrom,                "2020/1/1 1:2:3" },
//                    { tblAccountCouponsBase::cpnExpiryTime,             S(NULLABLE_TYPE(TAPI::DateTime_t)),QFV,                                     QNull,     UPAdmin},
                { tblAccountCouponsBase::cpnAmount,                   15 },
                { tblAccountCouponsBase::cpnAmountType,               TAPI::enuDiscountType::toStr(TAPI::enuDiscountType::Percent) },
//                    { tblAccountCouponsBase::cpnMaxAmount,              S(quint32),                        QFV,                                     1,         UPAdmin, false, false},
                { tblAccountCouponsBase::cpnSaleableBasedMultiplier,
//                        QList<TAPI::stuDiscountSaleableBasedMultiplier>({
//                            { "p123-s456", 1.5, 0 },
//                            { "p123-s456", 1.8, 5 },
//                            { "other",     2.0 },
//                        })
                    QVariantList({
                        QVariantMap({ { "saleableCode", "p123-s456" }, { "multiplier", 1.5 }, { "minCount", 0 } }),
                        QVariantMap({ { "saleableCode", "p123-s456" }, { "multiplier", 1.8 }, { "minCount", 5 } }),
                        QVariantMap({ { "saleableCode", "other" },     { "multiplier", 2.0 }                    }),
                    })
                },
//                    { tblAccountCouponsBase::cpnSaleableBasedMultiplier,  QJsonDocument(QJsonObject({ { "p123-s456", "10" }, { "other", "20" } })) },
//                    { tblAccountCouponsBase::cpnTotalUsedCount,         S(quint32),                        QFV.integer().minValue(0),               0,         UPNone},
//                    { tblAccountCouponsBase::cpnTotalUsedAmount,        S(quint32),                        QFV.integer().minValue(0),               0,         UPNone},
//                    { tblAccountCouponsBase::cpnStatus,                 S(TAPI::enuGenericStatus::Type),   QFV,                                     TAPI::enuGenericStatus::Active, UPStatus},
//                    { tblAccountCouponsBase::cpnCreatedBy_usrID,        ORM_CREATED_BY},
//                    { tblAccountCouponsBase::cpnCreationDateTime,       ORM_CREATED_ON},
//                    { tblAccountCouponsBase::cpnUpdatedBy_usrID,        ORM_UPDATED_BY},
            }
        );
    }
*/

    void getOrCreateLocation() {
        QVariantList locationInfo = callAdminAPI(
            GET,
            "Advert/Locations",
            {
                {
                    "filters", QString("%1=%2 + %3=%4")
                        .arg(tblLocations::locURL).arg("http://www.abbasgholi.com")
                        .arg(tblLocations::locPlaceCode).arg("ABC"),
                },
                { "reportCount", false },
                { "cols", tblLocations::locID },
            }
        ).toList();

        qDebug() << "--------- locationInfo: " << locationInfo;

        if (locationInfo.isEmpty() == false) {
            locationID = locationInfo.at(0).toMap()[tblLocations::locID];
        }
        else
        {
            locationID = callAdminAPI(
                PUT,
                "Advert/Locations",
                {},
                {
                    { tblLocations::locURL,        "http://www.abbasgholi.com" },
                    { tblLocations::locPlaceCode,  "ABC" },
                }
            );
        }

        qDebug() << "--------- locationID: " << locationID;
    }

    void deleteLocation() {
        QVariant result = callAdminAPI(
            DELETE,
            "Advert/Locations/" + locationID.toString()
        );

        qDebug() << "--------- DELETE location: " << result;
    }

    void getOrCreateLocation_2() {
        QVariantList locationInfo = callAdminAPI(
            GET,
            "Advert/Locations",
            {
                {
                    "filters", QString("%1=%2 + %3=%4")
                        .arg(tblLocations::locURL).arg("http://www.abbasgholi.com")
                        .arg(tblLocations::locPlaceCode).arg("ABC"),
                },
                { "reportCount", false },
                { "cols", tblLocations::locID },
            }
        ).toList();

        qDebug() << "--------- locationInfo: " << locationInfo;

        if (locationInfo.isEmpty() == false) {
            locationID = locationInfo.at(0).toMap()[tblLocations::locID];
        }
        else
        {
            locationID = callAdminAPI(
                PUT,
                "Advert/Locations",
                {},
                {
                    { tblLocations::locURL,        "http://www.abbasgholi.com" },
                    { tblLocations::locPlaceCode,  "ABC" },
                }
            );
        }

        qDebug() << "--------- locationID: " << locationID;
    }

    void getOrCreateProduct_banner() {
        QVariantList productInfo = callAdminAPI(
            GET,
            "Advert/AccountProducts",
            {
                {
                    "filters", QString("%1=%2")
                        .arg(tblAccountProductsBase::prdCode)
                        .arg("p123")
                },
                { "reportCount", false },
                { "cols", tblAccountProductsBase::prdID },
            }
        ).toList();

        qDebug() << "--------- productInfo: " << productInfo;

        if (productInfo.isEmpty() == false)
            bannerProductID = productInfo.at(0).toMap()[tblAccountProductsBase::prdID];
        else
        {
            bannerProductID = callAdminAPI(
                PUT,
                "Advert/AccountProducts",
                {},
                {
                    { tblAccountProductsBase::prdCode,          "p123" },
                    { tblAccountProductsBase::prdName,          "test product 123" },
                    { tblAccountProductsBase::prdInStockCount,  1 },
                    { tblAccountProducts::prdType,              TAdvertisement::enuProductType::toStr(TAdvertisement::enuProductType::Advertise) },
                    { tblAccountProducts::prd_locID,            locationID },
                }
            );
        }

        qDebug() << "--------- bannerProductID: " << bannerProductID;
    }

    void getOrCreateSaleable_banner() {
        QVariantList saleableInfo = callAdminAPI(
            GET,
            "Advert/AccountSaleables",
            {
                {
                    "filters", QString("%1=%2")
                        .arg(tblAccountSaleablesBase::slbCode)
                        .arg("p123-s456")
                },
                { "reportCount", false },
                { "cols", tblAccountSaleablesBase::slbID },
            }
        ).toList();

        qDebug() << "--------- saleableInfo: " << saleableInfo;

        if (saleableInfo.isEmpty() == false)
            bannerSaleableID = saleableInfo.at(0).toMap()[tblAccountSaleablesBase::slbID];
        else
        {
            bannerSaleableID = callAdminAPI(
                PUT,
                "Advert/AccountSaleables",
                {},
                {
                    { tblAccountSaleablesBase::slbCode,             "p123-s456" },
                    { tblAccountSaleablesBase::slbName,             "test Saleable 456 name" },
                    { tblAccountSaleablesBase::slbDesc,             "test Saleable 456 desc" },
                    { tblAccountSaleablesBase::slb_prdID,           bannerProductID },
                    { tblAccountSaleablesBase::slbType,             TAPI::enuSaleableType::toStr(TAPI::enuSaleableType::Special) },
                    { tblAccountSaleablesBase::slbBasePrice,        12000 },
                    { tblAccountSaleablesBase::slbProductCount,     900 },
                    { tblAccountSaleablesBase::slbInStockCount,     1 },
                    { tblAccountSaleablesBase::slbVoucherTemplate,  "test Saleable 456 vt" },
                }
            );
        }

        qDebug() << "--------- bannerSaleableID: " << bannerSaleableID;
    }

    void getOrCreatePaymentGateway_devtest() {
        auto info = callAdminAPI(
            PUT,
            "Account/PaymentGateways",
            {},
            {
                { "pgwName",                  QString("devtest %1").arg(time(nullptr)) },
                { "pgwType",                  TAPI::enuPaymentGatewayType::toStr(TAPI::enuPaymentGatewayType::DevelopersTest) },
                { "pgwDriver",                "DevTest" },
                { "pgwMetaInfo",              QVariantMap({
                                                { "username", "hello" },
                                                { "password", "123" },
                                              })
                },
                { "pgwAllowedDomainName",     "devtest.com" },
                //"
//                { "pgwTransactionFeeValue",   S(NULLABLE_TYPE(quint32)),                          QFV,                                QNull,      UPAdmin },
//                { "pgwTransactionFeeType",    S(TAPI::enuPaymentGatewayTransactionFeeType::Type), QFV,                                TAPI::enuPaymentGatewayTransactionFeeType::Currency, UPAdmin },
                //"
//                { "pgwMinRequestAmount",      1 },
//                { "pgwMaxRequestAmount",      S(NULLABLE_TYPE(quint32)),                          QFV,                                QNull,      UPAdmin },
//                { "pgwMaxPerDayAmount",       S(NULLABLE_TYPE(quint32)),                          QFV,                                QNull,      UPAdmin },
                //"
//                { "pgwLastPaymentDateTime",   S(NULLABLE_TYPE(TAPI::DateTime_t)),                 QFV,                                QNull,      UPAdmin },
//                { "pgwSumTodayPaidAmount",    S(quint64),                                         QFV,                                0,          UPAdmin },
                //"
//                { "pgwSumRequestCount",       S(quint32),                                         QFV,                                0,          UPAdmin },
//                { "pgwSumRequestAmountv,      S(quint64),                                         QFV,                                0,          UPAdmin },
//                { "pgwSumFailedCount",        S(quint32),                                         QFV,                                0,          UPAdmin },
//                { "pgwSumOkCount",            S(quint32),                                         QFV,                                0,          UPAdmin },
//                { "pgwSumPaidAmount",         S(quint64),                                         QFV,                                0,          UPAdmin },
            }
        );

        qDebug() << "--------- devtest: " << info;
    }

    void initializeLastPreVoucher() {
        lastPreVoucher = QVariantMap({
//              { "items", {} },
//              { "prize", {} },
//              { "summary", "" },
//              { "round", 0 },
//              { "toPay", 0 },
//              { "sign", 0 }
        });
    }

    void addToBasket_invalid_saleable_code() {
        QVariant result = callAdminAPI(
            POST,
            "Advert/addToBasket",
            {},
            {
                { "saleableCode", "p123-s456 zzzzzzzzz" },
//                { "orderAdditives", {} },
//                { "qty", 12 },
//                { "discountCode", "abcd11" },
//                { "referrer", "" },
//                { "extraRefererParams", {} },
//                { "lastPreVoucher", {} },
            }
        );
        //exHTTPNotFound("No item could be found");
    }

    void addToBasket_invalid_coupon_qty_not_available() {
        QVariant result = callAdminAPI(
            POST,
            "Advert/addToBasket",
            {},
            {
                { "saleableCode", "p123-s456" },
                { "orderAdditives", {} },
                { "qty", 999 },
//                { "discountCode", "zzzzzzzzzzzzzzzzzz" },
                { "referrer", "" },
                { "extraRefererParams", {} },
                { "lastPreVoucher", lastPreVoucher },
            }
        );
    }

    void addToBasket_invalid_coupon_code() {
//        TAPI::stuPreVoucher intfRESTAPIWithAccounting::apiPOSTaddToBasket(
//        TAPI::stuPreVoucher voucher
        QVariant result = callAdminAPI(
            POST,
            "Advert/addToBasket",
            {},
            {
                { "saleableCode", "p123-s456" },
                { "orderAdditives", {} },
                { "qty", 1 },
                { "discountCode", "zzzzzzzzzzzzzzzzzz" },
                { "referrer", "" },
                { "extraRefererParams", {} },
                { "lastPreVoucher", lastPreVoucher },
            }
        );
    }

    void getOrCreateDiscount() {
        QVariantList couponInfo = callAdminAPI(
            GET,
            "Advert/AccountCoupons",
            {
                {
                    "filters", QString("%1=%2")
                        .arg(tblAccountCouponsBase::cpnCode).arg("cpn-code-aaa")
                },
                { "reportCount", false },
                { "cols", tblAccountCouponsBase::cpnID },
            }
        ).toList();

        qDebug() << "--------- couponInfo: " << couponInfo;

//        qDebug() << endl
//                 << "------------------------------" << endl
//                 << QJsonDocument(QJsonObject({ { "a", "b" } })) << endl
//                 << QJsonDocument(QJsonObject({ { "a", "b" } })).toJson() << endl
//                 << QJsonDocument(QJsonObject({ { "a", "b" } })).toVariant() << endl
//                 << endl;

        if (couponInfo.isEmpty() == false) {
            couponID = couponInfo.at(0).toMap()[tblAccountCouponsBase::cpnID];
        }
        else
        {
            couponID = callAdminAPI(
                PUT,
                "Advert/AccountCoupons",
                {},
                {
                    { tblAccountCouponsBase::cpnCode, "cpn-code-aaa" },
                    { tblAccountCouponsBase::cpnPrimaryCount,             500 },
                    { tblAccountCouponsBase::cpnTotalMaxAmount,           15000000 },
                    { tblAccountCouponsBase::cpnPerUserMaxCount,          2 },
                    { tblAccountCouponsBase::cpnPerUserMaxAmount,         1000000 },
                    { tblAccountCouponsBase::cpnValidFrom,                "2020/1/1 1:2:3" },
//                    { tblAccountCouponsBase::cpnExpiryTime,             S(NULLABLE_TYPE(TAPI::DateTime_t)),QFV,                                     QNull,     UPAdmin},
                    { tblAccountCouponsBase::cpnAmount,                   10 },
                    { tblAccountCouponsBase::cpnAmountType,               TAPI::enuDiscountType::toStr(TAPI::enuDiscountType::Percent) },
                    { tblAccountCouponsBase::cpnMaxAmount,                250000 },
                    { tblAccountCouponsBase::cpnSaleableBasedMultiplier,
//                        QList<TAPI::stuDiscountSaleableBasedMultiplier>({
//                            { "p123-s456", 1.5, 0 },
//                            { "p123-s456", 1.8, 5 },
//                            { "other",     2.0 },
//                        })
                        QVariantList({
                            QVariantMap({ { "saleableCode", "p123-s456" }, { "multiplier", 1.5 }, { "minCount", 0 } }),
                            QVariantMap({ { "saleableCode", "p123-s456" }, { "multiplier", 1.8 }, { "minCount", 5 } }),
                            QVariantMap({ { "saleableCode", "other" },     { "multiplier", 2.0 }                    }),
                        })
                    },
//                    { tblAccountCouponsBase::cpnSaleableBasedMultiplier,  QJsonDocument(QJsonObject({ { "p123-s456", "10" }, { "other", "20" } })) },
//                    { tblAccountCouponsBase::cpnTotalUsedCount,         S(quint32),                        QFV.integer().minValue(0),               0,         UPNone},
//                    { tblAccountCouponsBase::cpnTotalUsedAmount,        S(quint32),                        QFV.integer().minValue(0),               0,         UPNone},
//                    { tblAccountCouponsBase::cpnStatus,                 S(TAPI::enuGenericStatus::Type),   QFV,                                     TAPI::enuGenericStatus::Active, UPStatus},
//                    { tblAccountCouponsBase::cpnCreatedBy_usrID,        ORM_CREATED_BY},
//                    { tblAccountCouponsBase::cpnCreationDateTime,       ORM_CREATED_ON},
//                    { tblAccountCouponsBase::cpnUpdatedBy_usrID,        ORM_UPDATED_BY},
                }
            );
        }

        qDebug() << "--------- couponID: " << couponID;
    }

    void addToBasket_valid_coupon_code_1() {
        lastPreVoucher = callAdminAPI(
            POST,
            "Advert/addToBasket",
            {},
            {
                { "saleableCode",       "p123-s456" },
                { "orderAdditives",     QVariantMap({ { "adtv1", "1 1 1" }, { "adtv2", 222 } }) },
                { "qty",                1 },
                { "discountCode",       "cpn-code-aaa" },
                { "referrer",           "" },
                { "extraRefererParams", {} },
                { "lastPreVoucher",     lastPreVoucher },
            }
        );

//        qDebug() << "--------- lastPreVoucher" << lastPreVoucher;
    }

    void addToBasket_valid_coupon_code_2() {
        lastPreVoucher = callAdminAPI(
            POST,
            "Advert/addToBasket",
            {},
            {
                { "saleableCode",       "p123-s456" },
                { "orderAdditives",     QVariantMap({ { "adtv1", "1 1 1" }, { "adtv2", 222 } }) },
                { "qty",                1 },
                { "discountCode",       "cpn-code-aaa" },
                { "referrer",           "" },
                { "extraRefererParams", {} },
                { "lastPreVoucher",     lastPreVoucher },
            }
        );

//        qDebug() << "--------- lastPreVoucher" << lastPreVoucher;
    }

    void finalizeBasket_empty_items() {
        QVariant result = callAdminAPI(
            POST,
            "Account/finalizeBasket",
            {},
            {
                { "preVoucher",             {} },
                { "gatewayType",            TAPI::enuPaymentGatewayType::toStr(TAPI::enuPaymentGatewayType::DevelopersTest) },
                { "domain",                 "devtest.com" },
                { "walletID",               9988 },
                { "paymentVerifyCallback",  "http://www.a.com" },
            }
        );
    }

    void finalizeBasket_online() {
        QVariantMap voucherInfo = callAdminAPI(
            POST,
            "Account/finalizeBasket",
            {},
            {
                { "preVoucher",             lastPreVoucher },
                { "gatewayType",            TAPI::enuPaymentGatewayType::toStr(TAPI::enuPaymentGatewayType::DevelopersTest) },
                { "domain",                 "devtest.com" },
                { "walletID",               9988 },
                { "paymentVerifyCallback",  "http://www.a.com" },
            }
        ).toMap();
        qDebug() << "--------- voucherInfo" << voucherInfo;
        /*
        TAPI::stuVoucher:
        QVariantMap, QMap(
            ("iD", QVariant(double, 83))
            ("info", QVariant(QVariantMap, QMap(
                ("items", QVariant(QVariantList, (
                    QVariant(QVariantMap, QMap(
                        ("desc", QVariant(QString, "test Saleable 456 name"))
                        ("disAmount", QVariant(double, 1800))
                        ("discount", QVariant(QVariantMap, QMap(
                            ("amount", QVariant(double, 1800))
                            ("code", QVariant(QString, "cpn-code-aaa"))
                            ("iD", QVariant(double, 1))
                        )))
                        ("orderID", QVariant(double, 202))
                        ("qty", QVariant(double, 1))
                        ("sign", QVariant(QString, "NJja6zFf7NOAtQSji+FXTAiDhKFx23Qsrn4adUnb9xg="))
                        ("subTotal", QVariant(double, 12000))
                        ("uUID", QVariant(QString, "6f987a18cf4501b5579ba0da8cbd81c9"))
                        ("unitPrice", QVariant(double, 12000))
                    ))
                    , QVariant(QVariantMap, QMap(
                        ("desc", QVariant(QString, "test Saleable 456 name"))
                        ("disAmount", QVariant(double, 1800))
                        ("discount", QVariant(QVariantMap, QMap(
                            ("amount", QVariant(double, 1800))
                            ("code", QVariant(QString, "cpn-code-aaa"))
                            ("iD", QVariant(double, 1))
                        )))
                        ("orderID", QVariant(double, 203))
                        ("qty", QVariant(double, 1))
                        ("sign", QVariant(QString, "G38evDwq/stAJ+Zk7uDNvEhWbeVPiE6jwhJkCgK5L1M="))
                        ("subTotal", QVariant(double, 12000))
                        ("uUID", QVariant(QString, "c876d40dcff62b6330d323c987c0600c"))
                        ("unitPrice", QVariant(double, 12000))
                    ))
                )))
                ("round", QVariant(double, 204))
                ("sign", QVariant(QString, "HEJIoCoisRNYftbKYwGfBMwl1FKrDr2TqTpX3si2PaA="))
                ("summary", QVariant(QString, "2 items"))
                ("toPay", QVariant(double, 20196))
            )))
            ("paymentLink", QVariant(QString, "https://devtest.dom/pay/devtest_track_id"))
            ("paymentMD5", QVariant(QString, "89ca6efe9a2abf7e12e5d05506f29bb6"))
            ("status", QVariant(double, 78))
        )
        */

        QString PaymentMD5 = voucherInfo.value("paymentMD5").toString();
        if (PaymentMD5.isEmpty() == false)
        {
            QVariant Result = callAdminAPI(
                POST,
                "Account/approveOnlinePayment",
                {},
                {
                    { "paymentMD5",     PaymentMD5 },
                    { "domain",         "this.is.domain" },
                    { "pgResponse",     QVariantMap({
                          { "resp_1", 1 },
                          { "resp_2", 2 },
                          { "resp_3", 3 },
                      }) },
                }
            );
            qDebug() << "--------- approveOnlinePayment Result" << Result;
            /*
            TAPI::stuVoucher:
            QVariantMap, QMap(
                ("iD", QVariant(double, 83))
                ("info", QVariant(QVariantMap, QMap(
                    ("items", QVariant(QVariantList, (
                        QVariant(QVariantMap, QMap(
                            ("desc", QVariant(QString, "test Saleable 456 name"))
                            ("disAmount", QVariant(double, 1800))
                            ("discount", QVariant(QVariantMap, QMap(
                                ("amount", QVariant(double, 1800))
                                ("code", QVariant(QString, "cpn-code-aaa"))
                                ("iD", QVariant(double, 1))
                            )))
                            ("orderID", QVariant(double, 202))
                            ("qty", QVariant(double, 1))
                            ("sign", QVariant(QString, "NJja6zFf7NOAtQSji+FXTAiDhKFx23Qsrn4adUnb9xg="))
                            ("subTotal", QVariant(double, 12000))
                            ("uUID", QVariant(QString, "6f987a18cf4501b5579ba0da8cbd81c9"))
                            ("unitPrice", QVariant(double, 12000))
                        ))
                        , QVariant(QVariantMap, QMap(
                            ("desc", QVariant(QString, "test Saleable 456 name"))
                            ("disAmount", QVariant(double, 1800))
                            ("discount", QVariant(QVariantMap, QMap(
                                ("amount", QVariant(double, 1800))
                                ("code", QVariant(QString, "cpn-code-aaa"))
                                ("iD", QVariant(double, 1))
                            )))
                            ("orderID", QVariant(double, 203))
                            ("qty", QVariant(double, 1))
                            ("sign", QVariant(QString, "G38evDwq/stAJ+Zk7uDNvEhWbeVPiE6jwhJkCgK5L1M="))
                            ("subTotal", QVariant(double, 12000))
                            ("uUID", QVariant(QString, "c876d40dcff62b6330d323c987c0600c"))
                            ("unitPrice", QVariant(double, 12000))
                        ))
                    )))
                    ("round", QVariant(double, 204))
                    ("sign", QVariant(QString, "HEJIoCoisRNYftbKYwGfBMwl1FKrDr2TqTpX3si2PaA="))
                    ("summary", QVariant(QString, "2 items"))
                    ("toPay", QVariant(double, 20196))
                )))
                ("status", QVariant(double, 70))
            )
            */
        }
    }

    void cleanupSaleableData() {
    }

};

#endif // TEST_ADVERT_HPP
