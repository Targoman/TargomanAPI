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

#include "../moduleSrc/ORM/Accounting.h"
#include "../moduleSrc/ORM/Locations.h"
using namespace Targoman::API::AdvertModule;
using namespace Targoman::API::AdvertModule::ORM;

#include "Interfaces/Helpers/SecurityHelper.h"
using namespace Targoman::API::Helpers;

class testAdvert : public clsBaseTest
{
    Q_OBJECT

public:
    testAdvert(const QString &_dbPrefix) : clsBaseTest(_dbPrefix) { ; }

    QString LastRandomNumber = "998877998877";
    QString CreatedUserEmail;
    QString CreatedAdminEmail;

    QVariant LocationID;
    QVariant BannerProductID;
    QVariant BannerSaleableID;
    QString BannerProductCode;
    QString BannerSaleableCode;
    //    QVariant PaymentGatewayID;
    QString CouponCode;
    QVariant CouponID;
    Targoman::API::AAA::stuPreVoucher LastPreVoucher;
    Targoman::API::AAA::stuVoucher BasketVoucher;
    Targoman::API::AAA::stuVoucher Voucher;
    Targoman::API::AAA::stuVoucher ApproveOnlinePaymentVoucher;
    QVariant OfflinePaymentClaimID;
    Targoman::API::AAA::stuVoucher ApproveOfflinePaymentVoucher;

private slots:
    void initTestCase() {
        initUnitTestData(false, true);
    }

    void cleanupTestCase() {
        gEncodedAdminJWT = "";
        gEncodedJWT = "";
//        cleanupUnitTestData();
    }

private:
    /***************************************************************************************/
    /* cleanup *****************************************************************************/
    /***************************************************************************************/
    void cleanupUnitTestData() {
//        clsDAC DAC;
//        DAC.execQuery("", QString("UPDATE %1AAA.tblUser SET usrStatus='R' WHERE usrEmail IN(?,?)").arg(this->DBPrefix), { UT_UserEmail, UT_AdminUserEmail });

        //    Targoman::API::AAA::stuVoucher approveOnlinePaymentVoucher;
        //    void deleteOnlinePayment()
        //    {
        //        QT_TRY {
        //            int ItemsCount = lastPreVoucher.Items.length();

        //            QVariant Result = callAdminAPI(
        //                DELETE,
        //                QString("Account/OnlinePayment"
        //            );

        //            lastPreVoucher.fromJson(Result.toJsonObject());

        //            QVERIFY(lastPreVoucher.Items.length() > ItemsCount);

        //        } QT_CATCH (const std::exception &exp) {
        //            QTest::qFail(exp.what(), __FILE__, __LINE__);
        //        }
        //    }

        //    Targoman::API::AAA::stuVoucher voucher;
        //    Targoman::API::AAA::stuPreVoucher lastPreVoucher;

        if (this->CouponID > 0) {
            try {
                QVariant Result = callAdminAPI(
                    RESTClientHelper::DELETE,
                    QString("Advert/AccountCoupons/%1").arg(this->CouponID.toString())
                );

            } catch (std::exception &_exp) {
                TargomanDebug(5) << _exp.what();
            }
        }

        //    void deletePaymentGateway_devtest()
        //    {
        //        if (paymentGatewayID > 0)
        //        {
        //            QT_TRY {
        //                QVariant Result = callAdminAPI(
        //                    DELETE,
        //                    QString("Advert/PaymentGateways/%1").arg(paymentGatewayID.toString())
        //                );

        //                QVERIFY(Result == true);

        //            } QT_CATCH (const std::exception &exp) {
        //                QTest::qFail(exp.what(), __FILE__, __LINE__);
        //            }
        //        }
        //    }

        if (this->BannerSaleableID > 0) {
            try {
                QVariant Result = callAdminAPI(
                    RESTClientHelper::DELETE,
                    QString("Advert/AccountSaleables/%1").arg(this->BannerSaleableID.toString())
                );

            } catch (std::exception &_exp) {
                TargomanDebug(5) << _exp.what();
            }
        }

        if (this->BannerProductID > 0) {
            try {
                QVariant Result = callAdminAPI(
                    RESTClientHelper::DELETE,
                    QString("Advert/AccountProducts/%1").arg(this->BannerProductID.toString())
                );

            } catch (std::exception &_exp) {
                TargomanDebug(5) << _exp.what();
            }
        }

        if (this->LocationID > 0) {
            try {
                QVariant Result = callAdminAPI(
                    RESTClientHelper::DELETE,
                    QString("Advert/Locations/%1").arg(this->LocationID.toString())
                );

            } catch (std::exception &_exp) {
                TargomanDebug(5) << _exp.what();
            }
        }

        try {
            QVariant Result = callAdminAPI(
                RESTClientHelper::POST,
                "Account/fixtureCleanup",
                {},
                {
                    { "random", this->LastRandomNumber },
                });

        } catch (std::exception &_exp) {
            TargomanDebug(5) << _exp.what();
        }

    }

private slots:
    /***************************************************************************************/
    /* tests *******************************************************************************/
    /***************************************************************************************/
    void setupAccountFixture() {
        QT_TRY {
            QVariant Result = callAdminAPI(
                RESTClientHelper::POST,
                "Account/fixtureSetup",
                {},
                {
                    { "random", this->LastRandomNumber },
                });

            QVERIFY(Result.isValid());

//            this->LastRandomNumber = Result.toMap().value("Random").toString();

            this->CreatedUserEmail = Result.toMap().value("User").toMap().value("email").toString();
            gUserID = Result.toMap().value("User").toMap().value("usrID").toULongLong();

            this->CreatedAdminEmail = Result.toMap().value("Admin").toMap().value("email").toString();
            gAdminUserID = Result.toMap().value("Admin").toMap().value("usrID").toULongLong();

        } QT_CATCH (const std::exception &exp) {
            QTest::qFail(exp.what(), __FILE__, __LINE__);
        }
    }

    void login_user() {
        //5d12d36cd5f66fe3e72f7b03cbb75333 = MD5(1234 + df6d2338b2b8fce1ec2f6dda0a630eb0 # 987)
        QVariant Result = callUserAPI(RESTClientHelper::POST,
                                "Account/login",{},{
                                    { "emailOrMobile", this->CreatedUserEmail },
                                    { "pass", "5d12d36cd5f66fe3e72f7b03cbb75333" },
                                    { "salt", "1234" },
                                });
        QVERIFY(Result.isValid());

        gEncodedJWT = Result.toString();
        gJWT = QJsonDocument::fromJson(QByteArray::fromBase64(gEncodedJWT.split('.').at(1).toLatin1())).object();

        QVERIFY(clsJWT(gJWT).usrID() == gUserID);
        QVERIFY(clsJWT(gJWT).usrStatus() == TAPI::enuUserStatus::Active);
    }

    void login_admin() {
        //5d12d36cd5f66fe3e72f7b03cbb75333 = MD5(1234 + df6d2338b2b8fce1ec2f6dda0a630eb0 # 987)
        QVariant Result = callUserAPI(RESTClientHelper::POST,
                                "Account/login",{},{
                                    { "emailOrMobile", this->CreatedAdminEmail },
                                    { "pass", "5d12d36cd5f66fe3e72f7b03cbb75333" },
                                    { "salt", "1234" },
                                });
        QVERIFY(Result.isValid());

        gEncodedAdminJWT = Result.toString();
        gAdminJWT = QJsonDocument::fromJson(QByteArray::fromBase64(gEncodedAdminJWT.split('.').at(1).toLatin1())).object();

        QVERIFY(clsJWT(gAdminJWT).usrID() == gAdminUserID);
        QVERIFY(clsJWT(gAdminJWT).usrStatus() == TAPI::enuUserStatus::Active);
    }

    /***************************************************/
//    void removeBasketItem_v1() {
//        callUserAPI(
//            RESTClientHelper::POST,
//            "Advert/removeBasketItem_v1",
//            {},
//            {
//                { "itemUUID", "4fc1849756f6c4f6b57031a7c6172ccd" },
//                { "lastPreVoucher", "{\"items\": [{\"desc\": \"fixture saleable 3381473638 name\",\"disAmount\": 0,\"orderID\": 195,\"qty\": 1,\"service\": \"Advert\",\"sign\":\"bA8fP3XLLuzmd5NieJ5JoVu0bRWy6R8duRFGzUrUquY=\",\"subTotal\": 12000,\"uUID\": \"a7875e24c51449e516233d8b001eff04\",\"unitPrice\": 12000,\"vATAmount\": 0,\"vATPercent\":0},{\"desc\": \"fixture saleable 3381473638 name\",\"disAmount\": 0,\"orderID\": 197,\"qty\": 1,\"service\": \"Advert\",\"sign\":\"KPi9KMXoH2Aakbass8WnJiOGWqi+jpR7q9UMJMSCmdU=\",\"subTotal\": 12000,\"uUID\": \"4fc1849756f6c4f6b57031a7c6172ccd\",\"unitPrice\": 12000,\"vATAmount\": 0,\"vATPercent\": 0}],\"round\": 360,\"sign\": \"RJQjl1FiueP+puJzFFXb/+MWt5J71WRq4cr006w/4+g=\",\"summary\": \"3 items\",\"toPay\": 35640}" }
//            }
//        );
//    }

    void createLocation() {
        QString url = QString("http://www.%1.com").arg(SecurityHelper::UUIDtoMD5());

        QT_TRY {
            this->LocationID = callAdminAPI(
                RESTClientHelper::PUT,
                "Advert/Locations",
                {},
                {
                    { tblLocations::Fields::locURL,        url },
                    { tblLocations::Fields::locPlaceCode,  "ABC" },
                }
            );

            QVERIFY(this->LocationID > 0);

        } QT_CATCH (const std::exception &exp) {
            QTest::qFail(exp.what(), __FILE__, __LINE__);
        }
    }

    void createProduct_banner() {
        this->BannerProductCode = QString("p%1").arg(QRandomGenerator::global()->generate());

        QT_TRY {
            this->BannerProductID = callAdminAPI(
                RESTClientHelper::PUT,
                "Advert/AccountProducts",
                {},
                {
                    { tblAccountProductsBase::Fields::prdCode,          this->BannerProductCode },
                    { tblAccountProductsBase::Fields::prdName,          "test product 123" },
                    { tblAccountProductsBase::Fields::prdInStockQty,    1'000 },
                    { tblAccountProducts::Fields::prdType,              Targoman::API::AdvertModule::enuProductType::toStr(Targoman::API::AdvertModule::enuProductType::Advertise) },
                    { tblAccountProducts::Fields::prd_locID,            this->LocationID },
                }
            );

            QVERIFY(this->BannerProductID > 0);

        } QT_CATCH (const std::exception &exp) {
            QTest::qFail(exp.what(), __FILE__, __LINE__);
        }
    }

    void createSaleable_banner() {
        this->BannerSaleableCode = QString("%1-s%2").arg(this->BannerProductCode).arg(QRandomGenerator::global()->generate());

        QT_TRY {
            this->BannerSaleableID = callAdminAPI(
                RESTClientHelper::PUT,
                "Advert/AccountSaleables",
                {},
                {
                    { tblAccountSaleablesBase::Fields::slbCode,             this->BannerSaleableCode },
                    { tblAccountSaleablesBase::Fields::slbName,             "test Saleable 456 name" },
                    { tblAccountSaleablesBase::Fields::slbDesc,             "test Saleable 456 desc" },
                    { tblAccountSaleablesBase::Fields::slb_prdID,           this->BannerProductID },
                    { tblAccountSaleablesBase::Fields::slbType,             TAPI::enuSaleableType::toStr(TAPI::enuSaleableType::Special) },
                    { tblAccountSaleablesBase::Fields::slbBasePrice,        12'000 },
//                    { tblAccountSaleablesBase::Fields::slbProductCount,     900 },
                    { tblAccountSaleablesBase::Fields::slbInStockQty,       150 },
                    { tblAccountSaleablesBase::Fields::slbVoucherTemplate,  "test Saleable 456 vt" },
                }
            );

            QVERIFY(this->BannerSaleableID > 0);

        } QT_CATCH (const std::exception &exp) {
            QTest::qFail(exp.what(), __FILE__, __LINE__);
        }
    }
/*
    void createPaymentGateway_devtest() {
        auto ret = callAdminAPI(
            RESTClientHelper::GET,
            "Account/PaymentGateways",
            {
                {
                    "filters", QString("%1=%2")
                        .arg("pgwType").arg("_DeveloperTest"),
                },
                { "reportCount", true },
                { "cols", "pgwID" },
            }
        ).toMap();
        auto totalRows = ret["totalRows"].toUInt();
        if (totalRows < 3) {
            for (int i=totalRows; i<3; ++i) {
                QT_TRY {
                    auto paymentGatewayID = callAdminAPI(
                        RESTClientHelper::PUT,
                        "Account/PaymentGateways",
                        {},
                        {
                            { "pgwName",                  QString("devtest %1").arg(time(nullptr)) },
                            { "pgwType",                  "_DeveloperTest" },
                            { "pgwDriver",                "DevTest" },
                            { "pgwMetaInfo",              QVariantMap({
                                                            { "username", "hello" },
                                                            { "password", "123" },
                                                          })
                            },
                            { "pgwAllowedDomainName",     "dev.test" },
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

                    QVERIFY(paymentGatewayID > 0);

                } QT_CATCH (const std::exception &exp) {
                    QTest::qFail(exp.what(), __FILE__, __LINE__);
                }
            }
        }
    }
*/

    // basket 1:
    void addToBasket_invalid_saleable_code() {
        QT_TRY {
//            QVERIFY_EXCEPTION_THROWN
            QVariant Result = callAdminAPI(
                RESTClientHelper::POST,
                "Advert/addToBasket",
                {},
                {
                    { "saleableCode", "p123-s456 zzzzzzzzz" },
//                    { "orderAdditives", {} },
//                    { "qty", 12 },
//                    { "discountCode", "abcd11" },
                    { "referrer", "freepaper.com" },
//                    { "extraReferrerParams", {} },
                    { "lastPreVoucher", this->LastPreVoucher.toJson().toVariantMap() },
                }
            );

            QVERIFY(Result.isValid() == false);

        } QT_CATCH (const std::exception &exp) {
//            QTest::qFail(exp.what(), __FILE__, __LINE__);
            qDebug() << exp.what();
            QVERIFY(true);
        }
    }

    void addToBasket_invalid_coupon_qty_not_available() {
        QT_TRY {
            QVariant Result = callAdminAPI(
                RESTClientHelper::POST,
                "Advert/addToBasket",
                {},
                {
                    { "saleableCode", this->BannerSaleableCode },
                    { "orderAdditives", {} },
                    { "qty", 999 },
//                    { "discountCode", "zzzzzzzzzzzzzzzzzz" },
                    { "referrer", "freepaper.com" },
                    { "extraReferrerParams", {} },
                    { "lastPreVoucher", this->LastPreVoucher.toJson().toVariantMap() },
                }
            );

            QVERIFY(Result.isValid() == false);

        } QT_CATCH (const std::exception &exp) {
//            QTest::qFail(exp.what(), __FILE__, __LINE__);
            qDebug() << exp.what();
            QVERIFY(true);
        }
    }

    void addToBasket_invalid_coupon_code() {
        QT_TRY {
            QVariant Result = callAdminAPI(
                RESTClientHelper::POST,
                "Advert/addToBasket",
                {},
                {
                    { "saleableCode", this->BannerSaleableCode },
                    { "orderAdditives", {} },
                    { "qty", 1 },
                    { "discountCode", "zzzzzzzzzzzzzzzzzz" },
                    { "referrer", "freepaper.com" },
                    { "extraReferrerParams", {} },
                    { "lastPreVoucher", this->LastPreVoucher.toJson().toVariantMap() },
                }
            );

            QVERIFY(Result.isValid() == false);

        } QT_CATCH (const std::exception &exp) {
//            QTest::qFail(exp.what(), __FILE__, __LINE__);
            qDebug() << exp.what();
            QVERIFY(true);
        }
    }

    void createDiscount() {
        this->CouponCode = QString("cpn-code-%1").arg(QRandomGenerator::global()->generate());

        QT_TRY {
            this->CouponID = callAdminAPI(
                RESTClientHelper::PUT,
                "Advert/AccountCoupons",
                {},
                {
                    { tblAccountCouponsBase::Fields::cpnCode,                       this->CouponCode },
                    { tblAccountCouponsBase::Fields::cpnPrimaryCount,               500 },
                    { tblAccountCouponsBase::Fields::cpnTotalMaxAmount,             15'000'000 },
                    { tblAccountCouponsBase::Fields::cpnPerUserMaxCount,            2 },
                    { tblAccountCouponsBase::Fields::cpnPerUserMaxAmount,           1'000'000 },
                    { tblAccountCouponsBase::Fields::cpnValidFrom,                  "2020/1/1 1:2:3" },
//                    { tblAccountCouponsBase::Fields::cpnExpiryTime,               S(NULLABLE_TYPE(TAPI::DateTime_t)),QFV,                                     QNull,     UPAdmin},
                    { tblAccountCouponsBase::Fields::cpnAmount,                     10 },
                    { tblAccountCouponsBase::Fields::cpnAmountType,                 Targoman::API::AAA::enuDiscountType::toStr(Targoman::API::AAA::enuDiscountType::Percent) },
                    { tblAccountCouponsBase::Fields::cpnMaxAmount,                  250'000 },
                    { tblAccountCouponsBase::Fields::cpnSaleableBasedMultiplier,
//                        QList<Targoman::API::AAA::stuDiscountSaleableBasedMultiplier>({
//                            { this->BannerSaleableCode, 1.5, 0 },
//                            { this->BannerSaleableCode, 1.8, 5 },
//                            { "other",     2.0 },
//                        })
                        QVariantList({
                            QVariantMap({ { "saleableCode", this->BannerSaleableCode }, { "multiplier", 1.5 }, { "minQty", 0 } }),
                            QVariantMap({ { "saleableCode", this->BannerSaleableCode }, { "multiplier", 1.8 }, { "minQty", 5 } }),
                            QVariantMap({ { "saleableCode", "other" },                  { "multiplier", 2.0 }                  }),
                        })
                    },
                }
            );

            QVERIFY(this->CouponID > 0);

        } QT_CATCH (const std::exception &exp) {
            QTest::qFail(exp.what(), __FILE__, __LINE__);
        }
    }

    void addToBasket_valid_coupon_code_1() {
        QT_TRY {
            int ItemsCount = this->LastPreVoucher.Items.length();

            QVariant Result = callAdminAPI(
                RESTClientHelper::POST,
                "Advert/addToBasket",
                {},
                {
                    { "saleableCode",           this->BannerSaleableCode },
                    { "orderAdditives",         QVariantMap({ { "adtv1", "1 1 1" }, { "adtv2", 222 } }) },
                    { "qty",                    10 },
                    { "discountCode",           this->CouponCode },
                    { "referrer",               "freepaper.com" },
                    { "extraReferrerParams",    {} },
                    { "lastPreVoucher",         this->LastPreVoucher.toJson().toVariantMap() },
                }
            );

            this->LastPreVoucher.fromJson(Result.toJsonObject());

            QVERIFY(this->LastPreVoucher.Items.length() > ItemsCount);

        } QT_CATCH (const std::exception &exp) {
            QTest::qFail(exp.what(), __FILE__, __LINE__);
        }
    }

    void removeBasketItem() {
        QT_TRY {
            int ItemsCount = this->LastPreVoucher.Items.length();

            QVariant Result = callAdminAPI(
                RESTClientHelper::POST,
                "Advert/removeBasketItem",
                {},
                {
                    { "itemUUID",       this->LastPreVoucher.Items.at(0).UUID },
                    { "lastPreVoucher", this->LastPreVoucher.toJson().toVariantMap() },
                }
            );

            this->LastPreVoucher.fromJson(Result.toJsonObject());

            QVERIFY(this->LastPreVoucher.Items.length() < ItemsCount);

        } QT_CATCH (const std::exception &exp) {
            QTest::qFail(exp.what(), __FILE__, __LINE__);
        }
    }

    void addToBasket_valid_coupon_code_2() {
        QT_TRY {
            int ItemsCount = this->LastPreVoucher.Items.length();

            QVariant Result = callAdminAPI(
                RESTClientHelper::POST,
                "Advert/addToBasket",
                {},
                {
                    { "saleableCode",           this->BannerSaleableCode },
                    { "orderAdditives",         QVariantMap({ { "adtv1", "1 1 1" }, { "adtv2", 222 } }) },
                    { "qty",                    20 },
                    { "discountCode",           this->CouponCode },
                    { "referrer",               "freepaper.com" },
                    { "extraReferrerParams",    {} },
                    { "lastPreVoucher",         this->LastPreVoucher.toJson().toVariantMap() },
                }
            );

            this->LastPreVoucher.fromJson(Result.toJsonObject());

            QVERIFY(this->LastPreVoucher.Items.length() > ItemsCount);

        } QT_CATCH (const std::exception &exp) {
            QTest::qFail(exp.what(), __FILE__, __LINE__);
        }
    }

    ///TODO: updateBasketItem

    void addToBasket_valid_coupon_code_3() {
        QT_TRY {
            int ItemsCount = this->LastPreVoucher.Items.length();

            QVariant Result = callAdminAPI(
                RESTClientHelper::POST,
                "Advert/addToBasket",
                {},
                {
                    { "saleableCode",           this->BannerSaleableCode },
                    { "orderAdditives",         QVariantMap({ { "adtv1", "1 1 1" }, { "adtv2", 222 } }) },
                    { "qty",                    30 },
                    { "discountCode",           this->CouponCode },
                    { "referrer",               "freepaper.com" },
                    { "extraReferrerParams",    {} },
                    { "lastPreVoucher",         this->LastPreVoucher.toJson().toVariantMap() },
                }
            );

            this->LastPreVoucher.fromJson(Result.toJsonObject());

            QVERIFY(this->LastPreVoucher.Items.length() > ItemsCount);

        } QT_CATCH (const std::exception &exp) {
            QTest::qFail(exp.what(), __FILE__, __LINE__);
        }
    }

    void finalizeBasket_empty_items() {
        QT_TRY {
            QVariant Result = callAdminAPI(
                RESTClientHelper::POST,
                "Account/finalizeBasket",
                {},
                {
                    { "preVoucher",             {} },
                    { "gatewayType",            "_DeveloperTest" },
                    { "domain",                 "dev.test" },
//                    { "walID",               9988 },
                    { "paymentVerifyCallback",  "http://www.a.com" },
                }
            );

            QVERIFY(Result.isValid() == false);

        } QT_CATCH (const std::exception &exp) {
//            QTest::qFail(exp.what(), __FILE__, __LINE__);
            qDebug() << exp.what();
            QVERIFY(true);
        }
    }

    void availableGatewayTypes_for_FinalizeBasket() {
        QT_TRY {
            QVariant Result = callAdminAPI(
                RESTClientHelper::POST,
                "Account/PaymentGateways/availableGatewayTypes",
                {},
                {
                    { "amount", this->LastPreVoucher.ToPay },
                    { "domain", "dev.test" },
                }
            );

            QVERIFY(Result.isValid());

        } QT_CATCH (const std::exception &exp) {
            QTest::qFail(exp.what(), __FILE__, __LINE__);
        }
    }

    void finalizeBasket() {
        QT_TRY {
            QVariant Result = callAdminAPI(
                RESTClientHelper::POST,
                "Account/finalizeBasket",
                {},
                {
                    { "preVoucher",             this->LastPreVoucher.toJson().toVariantMap() },
                    { "gatewayType",            "_DeveloperTest" },
                    { "domain",                 "dev.test" },
//                    { "walID",               9988 },
                    { "paymentVerifyCallback",  "http://www.a.com" },
                }
            );

            this->BasketVoucher.fromJson(Result.toJsonObject());

            QVERIFY(this->BasketVoucher.ID > 0);

        } QT_CATCH (const std::exception &exp) {
            QTest::qFail(exp.what(), __FILE__, __LINE__);
        }
    }

    void approveOnlinePayment() {
        if (this->BasketVoucher.PaymentMD5.isEmpty() == false) {
            QT_TRY {
                QVariant Result = callAdminAPI(
                    RESTClientHelper::POST,
                    "Account/approveOnlinePayment",
                    {},
                    {
                        { "paymentMD5",     this->BasketVoucher.PaymentMD5 },
                        { "domain",         "this.is.domain" },
                        { "pgResponse",     QVariantMap({
                              { "resp_1", 1 },
                              { "resp_2", 2 },
                              { "resp_3", 3 },
                          }) },
                    }
                );

                this->ApproveOnlinePaymentVoucher.fromJson(Result.toJsonObject());

                QVERIFY(this->ApproveOnlinePaymentVoucher.ID > 0);

            } QT_CATCH (const std::exception &exp) {
                QTest::qFail(exp.what(), __FILE__, __LINE__);
            }
        }
    }

    // basket 2:
    void requestIncrease_DEVTEST_with_domain() {
        try {
            this->Voucher = {};

            QVariant Result = callUserAPI(
                RESTClientHelper::PUT,
                "Account/UserWallets/requestIncrease",
                {},
                {
                    { "amount", 10'000 },
                    { "gatewayType", "_DeveloperTest" },
                    { "domain", "dev.Test" },
//                    { "walID", 0 },
                    { "paymentVerifyCallback", "http://www.a.com" }
                }
            );

            this->Voucher.fromJson(Result.toJsonObject());

            QVERIFY(this->Voucher.PaymentMD5.isEmpty() == false);

        } catch (exTargomanBase &e) {
            QFAIL (QString("error(%1):%2").arg(e.code()).arg(e.what()).toStdString().c_str());
        } catch (std::exception &e) {
            QFAIL (e.what());
        }
    }
    void approveOnlinePayment_for_requestIncrease_DEVTEST_with_domain() {
        if (this->Voucher.PaymentMD5.isEmpty() == false) {
            QT_TRY {
                QVariant Result = callUserAPI(
                    RESTClientHelper::POST,
                    "Account/approveOnlinePayment",
                    {},
                    {
                        { "paymentMD5",     this->Voucher.PaymentMD5 },
                        { "domain",         "this.is.domain" },
                        { "pgResponse",     QVariantMap({
                              { "resp_1", 1 },
                              { "resp_2", 2 },
                              { "resp_3", 3 },
                          }) },
                    }
                );

                this->ApproveOnlinePaymentVoucher.fromJson(Result.toJsonObject());

                QVERIFY(this->ApproveOnlinePaymentVoucher.ID > 0);

            } QT_CATCH (const std::exception &e) {
                QFAIL (QString("error(%1)").arg(e.what()).toStdString().c_str());
            }

            this->Voucher = {};
        }
    }

    void addToBasket2_valid_coupon_code_1() {
        this->LastPreVoucher = {};

        QT_TRY {
            int ItemsCount = this->LastPreVoucher.Items.length();

            QVariant Result = callUserAPI(
                RESTClientHelper::POST,
                "Advert/addToBasket",
                {},
                {
                    { "saleableCode",           this->BannerSaleableCode },
                    { "orderAdditives",         QVariantMap({ { "adtv1", "1 1 1" }, { "adtv2", 222 } }) },
                    { "qty",                    10 },
//                    { "discountCode",           this->CouponCode },
                    { "referrer",               "freepaper.com" },
                    { "extraReferrerParams",    {} },
                    { "lastPreVoucher",         this->LastPreVoucher.toJson().toVariantMap() },
                }
            );

            this->LastPreVoucher.fromJson(Result.toJsonObject());

            QVERIFY(this->LastPreVoucher.Items.length() > ItemsCount);

        } QT_CATCH (const std::exception &exp) {
            QTest::qFail(exp.what(), __FILE__, __LINE__);
        }
    }
    void addToBasket2_valid_coupon_code_2() {
        QT_TRY {
            int ItemsCount = this->LastPreVoucher.Items.length();

            QVariant Result = callUserAPI(
                RESTClientHelper::POST,
                "Advert/addToBasket",
                {},
                {
                    { "saleableCode",           this->BannerSaleableCode },
                    { "orderAdditives",         QVariantMap({ { "adtv1", "1 1 1" }, { "adtv2", 222 } }) },
                    { "qty",                    5 },
//                    { "discountCode",           this->CouponCode },
                    { "referrer",               "freepaper.com" },
                    { "extraReferrerParams",    {} },
                    { "lastPreVoucher",         this->LastPreVoucher.toJson().toVariantMap() },
                }
            );

            this->LastPreVoucher.fromJson(Result.toJsonObject());

            QVERIFY(this->LastPreVoucher.Items.length() > ItemsCount);

        } QT_CATCH (const std::exception &exp) {
            QTest::qFail(exp.what(), __FILE__, __LINE__);
        }
    }

    void finalizeBasket2() {
        this->BasketVoucher = {};

        QT_TRY {
            QVariant Result = callUserAPI(
                RESTClientHelper::POST,
                "Account/finalizeBasket",
                {},
                {
                    { "preVoucher",             this->LastPreVoucher.toJson().toVariantMap() },
                    { "gatewayType",            "COD" },
                    { "domain",                 "dev.test" },
                    { "walID",                  0 },
                    { "paymentVerifyCallback",  "http://www.a.com" },
                }
            );

            this->BasketVoucher.fromJson(Result.toJsonObject());

            QVERIFY(this->BasketVoucher.ID > 0);

        } QT_CATCH (const std::exception &exp) {
            QTest::qFail(exp.what(), __FILE__, __LINE__);
        }
    }

    void payForBasket2() {
        this->Voucher = {};

        QT_TRY {
            QVariant Result = callUserAPI(
                RESTClientHelper::POST,
                "Account/payForBasket",
                {},
                {
                    { "domain",                 "dev.test" },
                    { "voucherID",              this->BasketVoucher.ID },
                    { "gatewayType",            "_DeveloperTest" },
                    { "amount",                 100'000 },
//                    { "walID",                  0 },
                    { "paymentVerifyCallback",  "http://www.a.com" },
                }
            );

            this->Voucher.fromJson(Result.toJsonObject());

            QVERIFY(this->Voucher.ID > 0);

        } QT_CATCH (const std::exception &exp) {
            QTest::qFail(exp.what(), __FILE__, __LINE__);
        }
    }

    void payForBasket2_approveOnlinePayment() {
        if (this->Voucher.PaymentMD5.isEmpty() == false) {
            QT_TRY {
                QVariant Result = callUserAPI(
                    RESTClientHelper::POST,
                    "Account/approveOnlinePayment",
                    {},
                    {
                        { "paymentMD5",     this->Voucher.PaymentMD5 },
                        { "domain",         "this.is.domain" },
                        { "pgResponse",     QVariantMap({
                              { "resp_1", 1 },
                              { "resp_2", 2 },
                              { "resp_3", 3 },
                          }) },
                    }
                );

                this->ApproveOnlinePaymentVoucher.fromJson(Result.toJsonObject());

//                QVERIFY(this->ApproveOnlinePaymentVoucher.ID > 0);

            } QT_CATCH (const std::exception &e) {
                QFAIL (QString("error(%1)").arg(e.what()).toStdString().c_str());
            }

            this->Voucher = {};
        }
    }

    void claimOfflinePayment2() {
        QT_TRY {
            this->OfflinePaymentClaimID = callUserAPI(
                RESTClientHelper::POST,
                "Account/claimOfflinePayment",
                {},
                {
                    { "voucherID",      this->BasketVoucher.ID },
                    { "bank",           "bank mellat 2" },
                    { "receiptCode",    "8090" },
                    { "receiptDate",    "2022/03/04" },
                    { "amount",         200'000 },
                    { "note",           "this is note for offline payment 2" },
                }
            );

            QVERIFY(this->OfflinePaymentClaimID > 0);

        } QT_CATCH (const std::exception &exp) {
            QTest::qFail(exp.what(), __FILE__, __LINE__);
        }
    }

    void claimOfflinePayment2_approveOfflinePayment() {
        if (this->OfflinePaymentClaimID > 0) {
            QT_TRY {
                QVariant Result = callUserAPI(
                    RESTClientHelper::POST,
                    "Account/approveOfflinePayment",
                    {},
                    {
                        { "offlinePaymentClaimID", this->OfflinePaymentClaimID },
                    }
                );

                this->ApproveOfflinePaymentVoucher.fromJson(Result.toJsonObject());

                QVERIFY(this->ApproveOfflinePaymentVoucher.ID > 0);

            } QT_CATCH (const std::exception &exp) {
                QTest::qFail(exp.what(), __FILE__, __LINE__);
            }
        }
    }

};

#endif // TEST_ADVERT_HPP
