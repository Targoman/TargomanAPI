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

    QString LastRandomNumber; // = "998877998877";
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

        if (this->LastRandomNumber.isEmpty() == false) {
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
                    { "random", 1 },
                });

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

        QVERIFY(clsJWT(gJWT).actorID() == gUserID);
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

        QVERIFY(clsJWT(gAdminJWT).actorID() == gAdminUserID);
        QVERIFY(clsJWT(gAdminJWT).usrStatus() == TAPI::enuUserStatus::Active);
    }

    /***************************************************/
    void getAccountUnits_with_I18N() {
        QVariant Result = callGuestAPI(
            RESTClientHelper::GET,
            "Advert/AccountUnits"
        );
    }

    void getAccountUnits_with_I18N_notranslate() {
        QVariant Result = callGuestAPI(
            RESTClientHelper::GET,
            "Advert/AccountUnits",
            {
                { "translate", false },
            }
        );
    }

    void getAccountProducts_with_I18N() {
        QVariant Result = callGuestAPI(
            RESTClientHelper::GET,
            "Advert/AccountProducts"
        );
    }

    void getAccountProducts_with_I18N_notranslate() {
        QVariant Result = callGuestAPI(
            RESTClientHelper::GET,
            "Advert/AccountProducts",
            {
                { "translate", false },
            }
        );
    }

    void getAccountSaleable_join_product_with_I18N() {
        QVariant Result = callGuestAPI(
            RESTClientHelper::GET,
            "Advert/AccountSaleables"
        );
    }

    void getAccountSaleable_join_product_with_I18N_notranslate() {
        QVariant Result = callGuestAPI(
            RESTClientHelper::GET,
            "Advert/AccountSaleables",
            {
                { "translate", false },
            }
        );
    }

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
                    { tblAccountProductsBase::Fields::prd_untID,        1 },
                    { tblAccountProducts::ExtraFields::prdType,       Targoman::API::AdvertModule::enuProductType::toStr(Targoman::API::AdvertModule::enuProductType::Advertise) },
                    { tblAccountProducts::ExtraFields::prd_locID,     this->LocationID },
                    { tblAccountProductsBase::Fields::prdNameI18N,      QVariantMap({
                          { "fa", "عنوان فارسی ۱۲۳" },
                          { "ar", "عنوان عربی ۱۲۳" },
                          { "fr", "عنوان فرانسوی ۱۲۳" },
                    }) },
                    { tblAccountProductsBase::Fields::prdDescI18N,      QVariantMap({
                          { "fa", "شرح فارسی ۱۲۳" },
                          { "ar", "شرح عربی ۱۲۳" },
                    }) },
                }
            );

            QVERIFY(this->BannerProductID > 0);

        } QT_CATCH (const std::exception &exp) {
            QTest::qFail(exp.what(), __FILE__, __LINE__);
        }
    }

    void updateProduct_banner() {
        QT_TRY {
            auto Result = callAdminAPI(
                RESTClientHelper::PATCH,
                QString("Advert/AccountProducts/") + this->BannerProductID.toString(),
                {},
                {
                    { tblAccountProductsBase::Fields::prdName,          "test product 123456" },
                    { tblAccountProductsBase::Fields::prdNameI18N,      QVariantMap({
                          { "fa", "عنوان فارسی 123456" },
                    }) },
                    { tblAccountProductsBase::Fields::prdDescI18N,      QVariantMap({
                          { "fa", "شرح فارسی 123456" },
                          { "ar", "شرح عربی 123456" },
                    }) },
                }
            );

            QVERIFY(Result > 0);

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
                    { tblAccountSaleablesI18NBase::Fields::slbNameI18N,     QVariantMap({
                          { "fa", "عنوان فارسی 123456" },
                          { "ar", "عنوان عربی 123456" },
                    }) },
                    { tblAccountSaleablesI18NBase::Fields::slbDescI18N,     QVariantMap({
                          { "fa", "شرح فارسی 123456" },
                    }) },
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
//                    { "referrerParams",      {} },
                    { "lastPreVoucher", this->LastPreVoucher.toJson().toVariantMap() },
                },
                {},
                { "accept-language: fa" }
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
                    { "referrerParams",      {} },
                    { "lastPreVoucher", this->LastPreVoucher.toJson().toVariantMap() },
                },
                {},
                { "accept-language: fa" }
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
                    { "referrerParams",      {} },
                    { "lastPreVoucher", this->LastPreVoucher.toJson().toVariantMap() },
                },
                {},
                { "accept-language: fa" }
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
                    { tblAccountCouponsBase::Fields::cpnAmountType,                 enuDiscountType::toStr(enuDiscountType::Percent) },
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
                    { "orderAdditives",         QVariantMap({ { "adtv1", "1 1 1" }, { "adtv2", 111 } }) },
                    { "qty",                    5 },
                    { "discountCode",           this->CouponCode },
                    { "referrer",               "freepaper.com" },
                    { "referrerParams",         {} },
                    { "lastPreVoucher",         this->LastPreVoucher.toJson().toVariantMap() },
                },
                {},
                { "accept-language: fa" }
            );

            stuBasketActionResult BasketActionResult;
            BasketActionResult.fromJson(Result.toJsonObject());
            this->LastPreVoucher = BasketActionResult.PreVoucher;

            QVERIFY(this->LastPreVoucher.Items.length() > ItemsCount);
            QVERIFY(this->LastPreVoucher.Round == 200);
            QVERIFY(this->LastPreVoucher.ToPay == 46'000);

            auto item = this->LastPreVoucher.Items.last();
            QVERIFY(item.DisAmount == 13'800);
            QVERIFY(item.TotalPrice == 46'200);

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
                    { "itemUUID",       this->LastPreVoucher.Items.last().UUID },
                    { "lastPreVoucher", this->LastPreVoucher.toJson().toVariantMap() },
                }
            );

            stuBasketActionResult BasketActionResult;
            BasketActionResult.fromJson(Result.toJsonObject());
            this->LastPreVoucher = BasketActionResult.PreVoucher;

            QVERIFY(this->LastPreVoucher.Items.length() < ItemsCount);
            QVERIFY(this->LastPreVoucher.Round == 0);
            QVERIFY(this->LastPreVoucher.ToPay == 0);

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
                    { "orderAdditives",         QVariantMap({ { "adtv1", "1 1 1" }, { "adtv2", 111 } }) },
                    { "qty",                    8 },
                    { "discountCode",           this->CouponCode },
                    { "referrer",               "freepaper.com" },
                    { "referrerParams",         {} },
                    { "lastPreVoucher",         this->LastPreVoucher.toJson().toVariantMap() },
                },
                {},
                { "accept-language: fa" }
            );

            stuBasketActionResult BasketActionResult;
            BasketActionResult.fromJson(Result.toJsonObject());
            this->LastPreVoucher = BasketActionResult.PreVoucher;

            QVERIFY(this->LastPreVoucher.Items.length() > ItemsCount);
            QVERIFY(this->LastPreVoucher.Round == 920);
            QVERIFY(this->LastPreVoucher.ToPay == 73'000);

            auto item = this->LastPreVoucher.Items.last();
            QVERIFY(item.DisAmount == 22'080);
            QVERIFY(item.TotalPrice == 73'920);

        } QT_CATCH (const std::exception &exp) {
            QTest::qFail(exp.what(), __FILE__, __LINE__);
        }
    }

    void updateBasketItem() {
        QT_TRY {
            int ItemsCount = this->LastPreVoucher.Items.length();

            QVariant Result = callAdminAPI(
                RESTClientHelper::POST,
                "Advert/updateBasketItem",
                {},
                {
                    { "itemUUID",               this->LastPreVoucher.Items.last().UUID },
                    { "newQty",                 10 },
//                    { "newDiscountCode",        ??? },
                    { "lastPreVoucher",         this->LastPreVoucher.toJson().toVariantMap() },
                }
            );

            stuBasketActionResult BasketActionResult;
            BasketActionResult.fromJson(Result.toJsonObject());
            this->LastPreVoucher = BasketActionResult.PreVoucher;

            QVERIFY(this->LastPreVoucher.Items.length() == ItemsCount);
            QVERIFY(this->LastPreVoucher.Round == 400);
            QVERIFY(this->LastPreVoucher.ToPay == 92'000);

            auto item = this->LastPreVoucher.Items.last();
            QVERIFY(item.DisAmount == 27'600);
            QVERIFY(item.TotalPrice == 92'400);

        } QT_CATCH (const std::exception &exp) {
            QTest::qFail(exp.what(), __FILE__, __LINE__);
        }
    }

    void updateBasketItem_remove_coupon() {
        QT_TRY {
            int ItemsCount = this->LastPreVoucher.Items.length();

            QVariant Result = callAdminAPI(
                RESTClientHelper::POST,
                "Advert/updateBasketItem",
                {},
                {
                    { "itemUUID",               this->LastPreVoucher.Items.last().UUID },
                    { "newQty",                 10 },
                    { "newDiscountCode",        "" },
                    { "lastPreVoucher",         this->LastPreVoucher.toJson().toVariantMap() },
                }
            );

            stuBasketActionResult BasketActionResult;
            BasketActionResult.fromJson(Result.toJsonObject());
            this->LastPreVoucher = BasketActionResult.PreVoucher;

            QVERIFY(this->LastPreVoucher.Items.length() == ItemsCount);
            QVERIFY(this->LastPreVoucher.Round == 0);
            QVERIFY(this->LastPreVoucher.ToPay == 114'000);

            auto item = this->LastPreVoucher.Items.last();
            QVERIFY(item.DisAmount == 6'000);
            QVERIFY(item.TotalPrice == 114'000);

        } QT_CATCH (const std::exception &exp) {
            QTest::qFail(exp.what(), __FILE__, __LINE__);
        }
    }

    void addToBasket_valid_coupon_new_additives_1() {
        QT_TRY {
            int ItemsCount = this->LastPreVoucher.Items.length();

            QVariant Result = callAdminAPI(
                RESTClientHelper::POST,
                "Advert/addToBasket",
                {},
                {
                    { "saleableCode",           this->BannerSaleableCode },
                    { "orderAdditives",         QVariantMap({ { "adtv1", "2 2 2" }, { "adtv2", 222 } }) },
                    { "qty",                    14 },
                    { "discountCode",           this->CouponCode },
                    { "referrer",               "freepaper.com" },
                    { "referrerParams",         {} },
                    { "lastPreVoucher",         this->LastPreVoucher.toJson().toVariantMap() },
                },
                {},
                { "accept-language: fa" }
            );

            stuBasketActionResult BasketActionResult;
            BasketActionResult.fromJson(Result.toJsonObject());
            this->LastPreVoucher = BasketActionResult.PreVoucher;

            QVERIFY(this->LastPreVoucher.Items.length() > ItemsCount);
            QVERIFY(this->LastPreVoucher.Round == 360);
            QVERIFY(this->LastPreVoucher.ToPay == 243'000);

            auto item = this->LastPreVoucher.Items.last();
            QVERIFY(item.DisAmount == 38'640);
            QVERIFY(item.TotalPrice == 129'360);

        } QT_CATCH (const std::exception &exp) {
            QTest::qFail(exp.what(), __FILE__, __LINE__);
        }
    }

    void addToBasket_valid_coupon_new_additives_1_same() {
        QT_TRY {
            int ItemsCount = this->LastPreVoucher.Items.length();

            QVariant Result = callAdminAPI(
                RESTClientHelper::POST,
                "Advert/addToBasket",
                {},
                {
                    { "saleableCode",           this->BannerSaleableCode },
                    { "orderAdditives",         QVariantMap({ { "adtv1", "2 2 2" }, { "adtv2", 222 } }) },
                    { "qty",                    1 },
                    { "discountCode",           this->CouponCode },
                    { "referrer",               "freepaper.com" },
                    { "referrerParams",         {} },
                    { "lastPreVoucher",         this->LastPreVoucher.toJson().toVariantMap() },
                },
                {},
                { "accept-language: fa" }
            );

            stuBasketActionResult BasketActionResult;
            BasketActionResult.fromJson(Result.toJsonObject());
            this->LastPreVoucher = BasketActionResult.PreVoucher;

            QVERIFY(this->LastPreVoucher.Items.length() == ItemsCount);
            QVERIFY(this->LastPreVoucher.Round == 600);
            QVERIFY(this->LastPreVoucher.ToPay == 252'000);

            auto item = this->LastPreVoucher.Items.last();
            QVERIFY(item.DisAmount == 41'400);
            QVERIFY(item.TotalPrice == 138'600);

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
                    { "paymentVerifyCallback",  "http://127.0.0.1:10000/rest/v1/Account/OnlinePayments/devTestCallbackPage" },
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
                },
                {},
                {
                    { "accept-language: fa" },
                }
            );

            QVERIFY(Result.isValid());

        } QT_CATCH (const std::exception &exp) {
            QTest::qFail(exp.what(), __FILE__, __LINE__);
        }
    }

    void finalizeBasket_1_of_3_nopermit() {
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
                    { "paymentVerifyCallback",  "http://127.0.0.1:10000/rest/v1/Account/OnlinePayments/devTestCallbackPage" },
                }
            );

//            this->BasketVoucher.fromJson(Result.toJsonObject());

//            QVERIFY(this->BasketVoucher.ID > 0);

        } QT_CATCH (const std::exception &exp) {
            if (strcmp(exp.what(), "Permission to charge the wallet is not given") != 0)
                QTest::qFail(exp.what(), __FILE__, __LINE__);
        }
    }

    ///@TODO: create test for change basket after calling not finished finalize

    //3 step for pay basket
    void finalizeBasket_1_of_3() {
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
                    { "paymentVerifyCallback",  "http://127.0.0.1:10000/rest/v1/Account/OnlinePayments/devTestCallbackPage" },
                    { "allowChargeWallet", true },
                }
            );

            this->BasketVoucher.fromJson(Result.toJsonObject());
            this->LastPreVoucher = this->BasketVoucher.Info;

            QVERIFY(this->BasketVoucher.ID > 0);
            QVERIFY(this->LastPreVoucher.VoucherID > 0);

        } QT_CATCH (const std::exception &exp) {
            QTest::qFail(exp.what(), __FILE__, __LINE__);
        }
    }

    void approveOnlinePayment_1_of_3() {
        if (this->BasketVoucher.PaymentKey.isEmpty() == false) {
            QT_TRY {
                QVariant Result = callGuestAPI(
                    RESTClientHelper::POST,
                    "Account/approveOnlinePayment",
                    {},
                    {
                        { "paymentKey",     this->BasketVoucher.PaymentKey },
//                        { "domain",         "dev.test" },
                        { "pgResponse",     QVariantMap({
                            { "result",     "ok" },
                        }) },
                    }
                );

                this->ApproveOnlinePaymentVoucher.fromJson(Result.toJsonObject());

                QVERIFY(this->ApproveOnlinePaymentVoucher.ID > 0);
                QVERIFY(this->ApproveOnlinePaymentVoucher.Payed == 100'000);
                QVERIFY(this->ApproveOnlinePaymentVoucher.Remained == 152'000);

            } QT_CATCH (const std::exception &exp) {
                QTest::qFail(exp.what(), __FILE__, __LINE__);
            }
        }
    }

    void finalizeBasket_2_of_3() {
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
                    { "paymentVerifyCallback",  "http://127.0.0.1:10000/rest/v1/Account/OnlinePayments/devTestCallbackPage" },
                    { "allowChargeWallet", true },
                }
            );

            this->BasketVoucher.fromJson(Result.toJsonObject());
            this->LastPreVoucher = this->BasketVoucher.Info;

            QVERIFY(this->BasketVoucher.ID > 0);
            QVERIFY(this->LastPreVoucher.VoucherID > 0);

        } QT_CATCH (const std::exception &exp) {
            QTest::qFail(exp.what(), __FILE__, __LINE__);
        }
    }

    void approveOnlinePayment_2_of_3() {
        if (this->BasketVoucher.PaymentKey.isEmpty() == false) {
            QT_TRY {
                QVariant Result = callGuestAPI(
                    RESTClientHelper::POST,
                    "Account/approveOnlinePayment",
                    {},
                    {
                        { "paymentKey",     this->BasketVoucher.PaymentKey },
//                        { "domain",         "dev.test" },
                        { "pgResponse",     QVariantMap({
                            { "result",     "ok" },
                        }) },
                    }
                );

                this->ApproveOnlinePaymentVoucher.fromJson(Result.toJsonObject());

                QVERIFY(this->ApproveOnlinePaymentVoucher.ID > 0);
                QVERIFY(this->ApproveOnlinePaymentVoucher.Payed == 200'000);
                QVERIFY(this->ApproveOnlinePaymentVoucher.Remained == 52'000);

            } QT_CATCH (const std::exception &exp) {
                QTest::qFail(exp.what(), __FILE__, __LINE__);
            }
        }
    }

    void getvoucherinfo_1() {
        QT_TRY {
            QVariant Result = callAdminAPI(
                RESTClientHelper::GET,
                QString("Account/Voucher/%1").arg(this->LastPreVoucher.VoucherID),
                {},
                {}
            );

        } QT_CATCH (const std::exception &exp) {
            QTest::qFail(exp.what(), __FILE__, __LINE__);
        }
    }

    void finalizeBasket_3_of_3() {
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
                    { "paymentVerifyCallback",  "http://127.0.0.1:10000/rest/v1/Account/OnlinePayments/devTestCallbackPage" },
                    { "allowChargeWallet", true },
                }
            );

            this->BasketVoucher.fromJson(Result.toJsonObject());
            this->LastPreVoucher = this->BasketVoucher.Info;

            QVERIFY(this->BasketVoucher.ID > 0);
            QVERIFY(this->LastPreVoucher.VoucherID > 0);

        } QT_CATCH (const std::exception &exp) {
            QTest::qFail(exp.what(), __FILE__, __LINE__);
        }
    }

    void approveOnlinePayment_3_of_3() {
        if (this->BasketVoucher.PaymentKey.isEmpty() == false) {
            QT_TRY {
                QVariant Result = callGuestAPI(
                    RESTClientHelper::POST,
                    "Account/approveOnlinePayment",
                    {},
                    {
                        { "paymentKey",     this->BasketVoucher.PaymentKey },
//                        { "domain",         "dev.test" },
                        { "pgResponse",     QVariantMap({
                            { "result",     "ok" },
                        }) },
                    }
                );

                this->ApproveOnlinePaymentVoucher.fromJson(Result.toJsonObject());

                QVERIFY(this->ApproveOnlinePaymentVoucher.ID > 0);
                QVERIFY(this->ApproveOnlinePaymentVoucher.Payed == 252'000);
                QVERIFY(this->ApproveOnlinePaymentVoucher.Remained == 0);

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
                    { "paymentVerifyCallback", "http://127.0.0.1:10000/rest/v1/Account/OnlinePayments/devTestCallbackPage" }
                }
            );

            this->Voucher.fromJson(Result.toJsonObject());

            QVERIFY(this->Voucher.PaymentKey.isEmpty() == false);

        } catch (exTargomanBase &e) {
            QFAIL (QString("error(%1):%2").arg(e.code()).arg(e.what()).toStdString().c_str());
        } catch (std::exception &e) {
            QFAIL (e.what());
        }
    }
    void approveOnlinePayment_for_requestIncrease_DEVTEST_with_domain() {
        if (this->Voucher.PaymentKey.isEmpty() == false) {
            QT_TRY {
                QVariant Result = callGuestAPI(
                    RESTClientHelper::POST,
                    "Account/approveOnlinePayment",
                    {},
                    {
                        { "paymentKey",     this->Voucher.PaymentKey },
//                        { "domain",         "dev.test" },
                        { "pgResponse",     QVariantMap({
                            { "result",     "ok" },
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

    void addToBasket2_no_coupon_1() {
        this->LastPreVoucher = {};

        QT_TRY {
            int ItemsCount = this->LastPreVoucher.Items.length();

            QVariant Result = callUserAPI(
                RESTClientHelper::POST,
                "Advert/addToBasket",
                {},
                {
                    { "saleableCode",           this->BannerSaleableCode },
                    { "orderAdditives",         QVariantMap({ { "adtv1", "1 1 1" }, { "adtv2", 111 } }) },
                    { "qty",                    20 },
//                    { "discountCode",           this->CouponCode },
                    { "referrer",               "freepaper.com" },
                    { "referrerParams",         {} },
                    { "lastPreVoucher",         this->LastPreVoucher.toJson().toVariantMap() },
                },
                {},
                { "accept-language: fa" }
            );

            stuBasketActionResult BasketActionResult;
            BasketActionResult.fromJson(Result.toJsonObject());
            this->LastPreVoucher = BasketActionResult.PreVoucher;

            QVERIFY(this->LastPreVoucher.Items.length() > ItemsCount);
            QVERIFY(this->LastPreVoucher.Round == 000);
            QVERIFY(this->LastPreVoucher.ToPay == 230'000);

            auto item = this->LastPreVoucher.Items.last();
            QVERIFY(item.DisAmount == 10'000);
            QVERIFY(item.TotalPrice == 230'000);

        } QT_CATCH (const std::exception &exp) {
            QTest::qFail(exp.what(), __FILE__, __LINE__);
        }
    }
    void addToBasket2_no_coupon_2() {
        QT_TRY {
            int ItemsCount = this->LastPreVoucher.Items.length();

            QVariant Result = callUserAPI(
                RESTClientHelper::POST,
                "Advert/addToBasket",
                {},
                {
                    { "saleableCode",           this->BannerSaleableCode },
                    { "orderAdditives",         QVariantMap({ { "adtv1", "1 1 1" }, { "adtv2", 111 } }) },
                    { "qty",                    25 },
//                    { "discountCode",           this->CouponCode },
                    { "referrer",               "freepaper.com" },
                    { "referrerParams",         {} },
                    { "lastPreVoucher",         this->LastPreVoucher.toJson().toVariantMap() },
                },
                {},
                { "accept-language: fa" }
            );

            stuBasketActionResult BasketActionResult;
            BasketActionResult.fromJson(Result.toJsonObject());
            this->LastPreVoucher = BasketActionResult.PreVoucher;

            QVERIFY(this->LastPreVoucher.Items.length() == ItemsCount);
            QVERIFY(this->LastPreVoucher.Round == 000);
            QVERIFY(this->LastPreVoucher.ToPay == 530'000);

            auto item = this->LastPreVoucher.Items.last();
            QVERIFY(item.DisAmount == 10'000);
            QVERIFY(item.TotalPrice == 530'000);

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
                    { "paymentVerifyCallback",  "http://127.0.0.1:10000/rest/v1/Account/OnlinePayments/devTestCallbackPage" },
                }
            );

            this->BasketVoucher.fromJson(Result.toJsonObject());

            QVERIFY(this->BasketVoucher.ID > 0);

        } QT_CATCH (const std::exception &exp) {
            QTest::qFail(exp.what(), __FILE__, __LINE__);
        }
    }

//    void payForBasket2() {
//        this->Voucher = {};

//        QT_TRY {
//            QVariant Result = callUserAPI(
//                RESTClientHelper::POST,
//                "Account/payForBasket",
//                {},
//                {
//                    { "domain",                 "dev.test" },
//                    { "voucherID",              this->BasketVoucher.ID },
//                    { "gatewayType",            "_DeveloperTest" },
//                    { "amount",                 100'000 },
////                    { "walID",                  0 },
//                    { "paymentVerifyCallback",  "http://127.0.0.1:10000/rest/v1/Account/OnlinePayments/devTestCallbackPage" },
//                }
//            );

//            this->Voucher.fromJson(Result.toJsonObject());

//            QVERIFY(this->Voucher.ID > 0);

//        } QT_CATCH (const std::exception &exp) {
//            QTest::qFail(exp.what(), __FILE__, __LINE__);
//        }
//    }

//    void payForBasket2_approveOnlinePayment() {
//        if (this->Voucher.PaymentKey.isEmpty() == false) {
//            QT_TRY {
//                QVariant Result = callGuestAPI(
//                    RESTClientHelper::POST,
//                    "Account/approveOnlinePayment",
//                    {},
//                    {
//                        { "paymentKey",     this->Voucher.PaymentKey },
//                        //{ "domain",         "dev.test" },
//                        { "pgResponse",     QVariantMap({
//                            { "result",     "ok" },
//                        }) },
//                    }
//                );

//                this->ApproveOnlinePaymentVoucher.fromJson(Result.toJsonObject());

////                QVERIFY(this->ApproveOnlinePaymentVoucher.ID > 0);

//            } QT_CATCH (const std::exception &e) {
//                QFAIL (QString("error(%1)").arg(e.what()).toStdString().c_str());
//            }

//            this->Voucher = {};
//        }
//    }

    void claimOfflinePayment2() {
        QT_TRY {
            this->OfflinePaymentClaimID = callUserAPI(
                RESTClientHelper::POST,
                "Account/claimOfflinePayment",
                {},
                {
//                    { "voucherID",      this->BasketVoucher.ID },
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
                QVariant Result = callAdminAPI(
                    RESTClientHelper::POST,
                    "Account/approveOfflinePayment",
                    {},
                    {
                        { "offlinePaymentID", this->OfflinePaymentClaimID },
                    }
                );

                this->ApproveOfflinePaymentVoucher.fromJson(Result.toJsonObject());

                QVERIFY(this->ApproveOfflinePaymentVoucher.ID > 0);

            } QT_CATCH (const std::exception &exp) {
                QTest::qFail(exp.what(), __FILE__, __LINE__);
            }
        }
    }

    void get_userassets() {
        QT_TRY {
            QVariant Result = callAdminAPI(
                RESTClientHelper::GET,
                "Advert/AccountUserAssets",
                {},
                {
                }
            );

//            QVERIFY(this->ApproveOfflinePaymentVoucher.ID > 0);

        } QT_CATCH (const std::exception &exp) {
            QTest::qFail(exp.what(), __FILE__, __LINE__);
        }
    }

};

#endif // TEST_ADVERT_HPP
