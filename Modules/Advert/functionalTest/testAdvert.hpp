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
//    QVariant paymentGatewayID;
    QVariant couponID;
    TAPI::stuPreVoucher lastPreVoucher;
    TAPI::stuVoucher voucher;
    TAPI::stuVoucher approveOnlinePaymentVoucher;

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
        QVERIFY((gUserID = callAPI(PUT,
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

    void ApproveEmail_user()
    {
        clsDAC DAC;
        QString Code = DAC.execQuery("", "SELECT aprApprovalCode FROM tblApprovalRequest WHERE apr_usrID=?",
        {gUserID}).toJson(true).object().value("aprApprovalCode").toString();

        DAC.execQuery("", "UPDATE tblApprovalRequest SET aprStatus = 'S' WHERE apr_usrID=?", { gUserID });

        QVERIFY(callAPI(POST,
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

        QVERIFY(callAPI(POST,
                        "Account/approveEmail", {},{
                            {"uuid", Code}
                        }).toBool());
    }

    void Login_admin()
    {
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

    /***************************************************************************************/
    /***************************************************************************************/
    /***************************************************************************************/
    void createLocation()
    {
        QT_TRY {
            locationID = callAdminAPI(
                PUT,
                "Advert/Locations",
                {},
                {
                    { tblLocations::locURL,        "http://www.abbasgholi.com" },
                    { tblLocations::locPlaceCode,  "ABC" },
                }
            );

        qDebug() << "--------- locationID: " << locationID;

            QVERIFY(locationID > 0);

        } QT_CATCH (const std::exception &exp) {
            QTest::qFail(exp.what(), __FILE__, __LINE__);
        }
    }

    void createProduct_banner()
    {
        QT_TRY {
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

            qDebug() << "--------- bannerProductID: " << bannerProductID;

            QVERIFY(bannerProductID > 0);

        } QT_CATCH (const std::exception &exp) {
            QTest::qFail(exp.what(), __FILE__, __LINE__);
        }
    }

    void createSaleable_banner()
    {
        QT_TRY {
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

            qDebug() << "--------- bannerSaleableID: " << bannerSaleableID;

            QVERIFY(bannerSaleableID > 0);

        } QT_CATCH (const std::exception &exp) {
            QTest::qFail(exp.what(), __FILE__, __LINE__);
        }
    }

    void createPaymentGateway_devtest()
    {
        auto ret = callAdminAPI(
            GET,
            "Account/PaymentGateways",
            {
                {
                    "filters", QString("%1=%2")
                        .arg("pgwType").arg(TAPI::enuPaymentGatewayType::toStr(TAPI::enuPaymentGatewayType::DevelopersTest)),
                },
                { "reportCount", true },
                { "cols", "pgwID" },
            }
        ).toMap();
//        qDebug() << "--------- PaymentGateways: " << ret;

        auto totalRows = ret["totalRows"].toUInt();
        if (totalRows < 3)
        {
            for (int i=totalRows; i<3; ++i)
            {
                QT_TRY {
                    auto paymentGatewayID = callAdminAPI(
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

                    qDebug() << "--------- paymentGatewayID: " << paymentGatewayID;

                    QVERIFY(paymentGatewayID > 0);

                } QT_CATCH (const std::exception &exp) {
                    QTest::qFail(exp.what(), __FILE__, __LINE__);
                }
            }
        }
    }

    void addToBasket_invalid_saleable_code()
    {
        QT_TRY {
            QVariant result = callAdminAPI(
                POST,
                "Advert/addToBasket",
                {},
                {
                    { "saleableCode", "p123-s456 zzzzzzzzz" },
//                    { "orderAdditives", {} },
//                    { "qty", 12 },
//                    { "discountCode", "abcd11" },
//                    { "referrer", "" },
//                    { "extraRefererParams", {} },
                    { "lastPreVoucher", lastPreVoucher.toJson().toVariantMap() },
                }
            );
            //exHTTPNotFound("No item could be found");

            //QEXCEPTION

        } QT_CATCH (const std::exception &exp) {
            QTest::qFail(exp.what(), __FILE__, __LINE__);
        }
    }

    void addToBasket_invalid_coupon_qty_not_available()
    {
        QT_TRY {
            QVariant result = callAdminAPI(
                POST,
                "Advert/addToBasket",
                {},
                {
                    { "saleableCode", "p123-s456" },
                    { "orderAdditives", {} },
                    { "qty", 999 },
//                    { "discountCode", "zzzzzzzzzzzzzzzzzz" },
                    { "referrer", "" },
                    { "extraRefererParams", {} },
                    { "lastPreVoucher", lastPreVoucher.toJson().toVariantMap() },
                }
            );

            //QEXCEPTION

        } QT_CATCH (const std::exception &exp) {
            QTest::qFail(exp.what(), __FILE__, __LINE__);
        }
    }

    void addToBasket_invalid_coupon_code()
    {
        QT_TRY {
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
                    { "lastPreVoucher", lastPreVoucher.toJson().toVariantMap() },
                }
            );

            //QEXCEPTION

        } QT_CATCH (const std::exception &exp) {
            QTest::qFail(exp.what(), __FILE__, __LINE__);
        }
    }

    void createDiscount()
    {
        QT_TRY {
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

            qDebug() << "--------- couponID: " << couponID;

            QVERIFY(couponID > 0);

        } QT_CATCH (const std::exception &exp) {
            QTest::qFail(exp.what(), __FILE__, __LINE__);
        }
    }

    void addToBasket_valid_coupon_code_1()
    {
        QT_TRY {
            int ItemsCount = lastPreVoucher.Items.length();

            QVariant Result = callAdminAPI(
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
                    { "lastPreVoucher",     lastPreVoucher.toJson().toVariantMap() },
                }
            );

            qDebug() << "--------- addToBasket" << Result;

            lastPreVoucher.fromJson(Result.toJsonObject());

            QVERIFY(lastPreVoucher.Items.length() > ItemsCount);

        } QT_CATCH (const std::exception &exp) {
            QTest::qFail(exp.what(), __FILE__, __LINE__);
        }
    }

    void addToBasket_valid_coupon_code_2()
    {
        QT_TRY {
            int ItemsCount = lastPreVoucher.Items.length();

            QVariant Result = callAdminAPI(
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
                    { "lastPreVoucher",     lastPreVoucher.toJson().toVariantMap() },
                }
            );

            qDebug() << "--------- addToBasket" << Result;

            lastPreVoucher.fromJson(Result.toJsonObject());

            QVERIFY(lastPreVoucher.Items.length() > ItemsCount);

        } QT_CATCH (const std::exception &exp) {
            QTest::qFail(exp.what(), __FILE__, __LINE__);
        }
    }

    void finalizeBasket_empty_items()
    {
        QT_TRY {
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

            //QEXCEPTION

        } QT_CATCH (const std::exception &exp) {
            QTest::qFail(exp.what(), __FILE__, __LINE__);
        }
    }

    void finalizeBasket()
    {
        QT_TRY {
            QVariant Result = callAdminAPI(
                POST,
                "Account/finalizeBasket",
                {},
                {
                    { "preVoucher",             lastPreVoucher.toJson().toVariantMap() },
                    { "gatewayType",            TAPI::enuPaymentGatewayType::toStr(TAPI::enuPaymentGatewayType::DevelopersTest) },
                    { "domain",                 "devtest.com" },
                    { "walletID",               9988 },
                    { "paymentVerifyCallback",  "http://www.a.com" },
                }
            );

            qDebug() << "--------- voucherInfo" << Result;

            voucher.fromJson(Result.toJsonObject());

            QVERIFY(voucher.ID > 0);

        } QT_CATCH (const std::exception &exp) {
            QTest::qFail(exp.what(), __FILE__, __LINE__);
        }
    }

    void approveOnlinePayment()
    {
        if (voucher.PaymentMD5.isEmpty() == false)
        {
            QT_TRY {
                QVariant Result = callAdminAPI(
                    POST,
                    "Account/approveOnlinePayment",
                    {},
                    {
                        { "paymentMD5",     voucher.PaymentMD5 },
                        { "domain",         "this.is.domain" },
                        { "pgResponse",     QVariantMap({
                              { "resp_1", 1 },
                              { "resp_2", 2 },
                              { "resp_3", 3 },
                          }) },
                    }
                );

                qDebug() << "--------- approveOnlinePayment Result" << Result;

                approveOnlinePaymentVoucher.fromJson(Result.toJsonObject());

                QVERIFY(approveOnlinePaymentVoucher.ID > 0);

            } QT_CATCH (const std::exception &exp) {
                QTest::qFail(exp.what(), __FILE__, __LINE__);
            }
        }
    }

    /***************************************************************************************/
    /* cleanup *****************************************************************************/
    /***************************************************************************************/
//    TAPI::stuVoucher approveOnlinePaymentVoucher;
//    void deleteOnlinePayment()
//    {
//        QT_TRY {
//            int ItemsCount = lastPreVoucher.Items.length();

//            QVariant Result = callAdminAPI(
//                DELETE,
//                QString("Account/onlinePayment"
//            );

//            qDebug() << "--------- addToBasket" << Result;

//            lastPreVoucher.fromJson(Result.toJsonObject());

//            QVERIFY(lastPreVoucher.Items.length() > ItemsCount);

//        } QT_CATCH (const std::exception &exp) {
//            QTest::qFail(exp.what(), __FILE__, __LINE__);
//        }
//    }

//    TAPI::stuVoucher voucher;
//    TAPI::stuPreVoucher lastPreVoucher;

    void deleteDiscount()
    {
        QT_TRY {
            QVariant Result = callAdminAPI(
                DELETE,
                QString("Advert/AccountCoupons/%1").arg(couponID.toString())
            );

//            qDebug() << "--------- DELETE result: " << Result;

            QVERIFY(Result == true);

        } QT_CATCH (const std::exception &exp) {
            QTest::qFail(exp.what(), __FILE__, __LINE__);
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

////                qDebug() << "--------- DELETE result: " << Result;

//                QVERIFY(Result == true);

//            } QT_CATCH (const std::exception &exp) {
//                QTest::qFail(exp.what(), __FILE__, __LINE__);
//            }
//        }
//    }

    void deleteSaleable_banner()
    {
        if (bannerSaleableID > 0)
        {
            QT_TRY {
                QVariant Result = callAdminAPI(
                    DELETE,
                    QString("Advert/AccountSaleables/%1").arg(bannerSaleableID.toString())
                );

//                qDebug() << "--------- DELETE result: " << Result;

                QVERIFY(Result == true);

            } QT_CATCH (const std::exception &exp) {
                QTest::qFail(exp.what(), __FILE__, __LINE__);
            }
        }
    }

    void deleteProduct_banner()
    {
        if (bannerProductID > 0)
        {
            QT_TRY {
                QVariant Result = callAdminAPI(
                    DELETE,
                    QString("Advert/AccountProducts/%1").arg(bannerProductID.toString())
                );

//                qDebug() << "--------- DELETE result: " << Result;

                QVERIFY(Result == true);

            } QT_CATCH (const std::exception &exp) {
                QTest::qFail(exp.what(), __FILE__, __LINE__);
            }
        }
    }

    void deleteLocation()
    {
        if (locationID > 0)
        {
            QT_TRY {
                QVariant Result = callAdminAPI(
                    DELETE,
                    QString("Advert/Locations/%1").arg(locationID.toString())
                );

//                qDebug() << "--------- DELETE result: " << Result;

                QVERIFY(Result == true);

            } QT_CATCH (const std::exception &exp) {
                QTest::qFail(exp.what(), __FILE__, __LINE__);
            }
        }
    }

    /***************************************************************************************/
    void deleteAdminUser()
    {
        if (gAdminUserID > 0)
        {
            QT_TRY {
                QVariant Result = callAdminAPI(
                    DELETE,
                    QString("Account/User/%1").arg(gAdminUserID)
                );

//                qDebug() << "--------- DELETE result: " << Result;

                QVERIFY(Result == true);

            } QT_CATCH (const std::exception &exp) {
                QTest::qFail(exp.what(), __FILE__, __LINE__);
            }
        }
    }

    void deleteUser()
    {
        if (gUserID > 0)
        {
            QT_TRY {
                QVariant Result = callAdminAPI(
                    DELETE,
                    QString("Account/User/%1").arg(gUserID)
                );

//                qDebug() << "--------- DELETE result: " << Result;

                QVERIFY(Result == true);

            } QT_CATCH (const std::exception &exp) {
                QTest::qFail(exp.what(), __FILE__, __LINE__);
            }
        }
    }

};

#endif // TEST_ADVERT_HPP
