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

#ifndef TEST_MT_HPP
#define TEST_MT_HPP

#include <QRandomGenerator>
#include "Interfaces/Test/testCommon.hpp"
#include "Interfaces/AAA/clsJWT.hpp"
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

class testMT : public clsBaseTest
{
    Q_OBJECT

public:
    testMT(const QString &_dbPrefix) : clsBaseTest(_dbPrefix) { ; }

    QString LastRandomNumber; // = "998877998877";
    QString CreatedUserEmail;
    QString CreatedAdminEmail;

    QVariant MTProductID;
    QVariant MTSaleableID;
    QString MTProductCode;
    QString MTSaleableCode;
    Targoman::API::AAA::stuPreVoucher LastPreVoucher;
    Targoman::API::AAA::stuVoucher BasketVoucher;
//    Targoman::API::AAA::stuVoucher Voucher;
    Targoman::API::AAA::stuVoucher ApproveOnlinePaymentVoucher;

    quint64 TokenID;
    QString TokenJWT;

private slots:
    void initTestCase() {
        initUnitTestData(false);
    }

    void cleanupTestCase() {
        gEncodedAdminJWT = "";
        gEncodedJWT = "";
        cleanupUnitTestData();
    }

private:
    /***************************************************************************************/
    /* cleanup *****************************************************************************/
    /***************************************************************************************/
    void cleanupUnitTestData() {
//        clsDAC DAC;
//        DAC.execQuery("", QString("UPDATE %1AAA.tblUser SET usrStatus='R' WHERE usrEmail IN(?,?)").arg(this->DBPrefix), { UT_UserEmail, UT_AdminUserEmail });

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
    void createProduct_mt() {
        this->MTProductCode = QString("p%1").arg(QRandomGenerator::global()->generate());

        QT_TRY {
            this->MTProductID = callAdminAPI(
                RESTClientHelper::PUT,
                "MT/AccountProducts",
                {},
                {
                    { tblAccountProductsBase::Fields::prdCode,          this->MTProductCode },
                    { tblAccountProductsBase::Fields::prdName,          "test mt product 123" },
                    { tblAccountProductsBase::Fields::prdInStockQty,    1'000 },
                    { tblAccountProductsBase::Fields::prd_untID,        1 },
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

            QVERIFY(this->MTProductID > 0);

        } QT_CATCH (const std::exception &exp) {
            QTest::qFail(exp.what(), __FILE__, __LINE__);
        }
    }

    void createSaleable_mt() {
        this->MTSaleableCode = QString("%1-s%2").arg(this->MTProductCode).arg(QRandomGenerator::global()->generate());

        QT_TRY {
            this->MTSaleableID = callAdminAPI(
                RESTClientHelper::PUT,
                "MT/AccountSaleables",
                {},
                {
                    { tblAccountSaleablesBase::Fields::slbCode,             this->MTSaleableCode },
                    { tblAccountSaleablesBase::Fields::slbName,             "test mt Saleable 456 name" },
                    { tblAccountSaleablesBase::Fields::slbDesc,             "test mt Saleable 456 desc" },
                    { tblAccountSaleablesBase::Fields::slb_prdID,           this->MTProductID },
                    { tblAccountSaleablesBase::Fields::slbType,             TAPI::enuSaleableType::toStr(TAPI::enuSaleableType::Special) },
                    { tblAccountSaleablesBase::Fields::slbBasePrice,        12'000 },
                    { tblAccountSaleablesBase::Fields::slbInStockQty,       150 },
                    { tblAccountSaleablesBase::Fields::slbVoucherTemplate,  "test mt Saleable 456 vt" },
                }
            );

            QVERIFY(this->MTSaleableID > 0);

        } QT_CATCH (const std::exception &exp) {
            QTest::qFail(exp.what(), __FILE__, __LINE__);
        }
    }

    void create_api() {
        QT_TRY {
            QVariant Result = this->callUserAPI(
                        RESTClientHelper::enuHTTPMethod::POST,
                        "Account/APITokens/request",
                        {},
                        {
                            { "name", "test mt" },
//                            { "services", QStringList({
//                                "MT",
//                                "blablabla",
//                            }) },
                        });

            QVERIFY(Result.isValid());

            QVariantMap ResultToMap = Result.toMap();

            this->TokenID  = ResultToMap["iD"].toULongLong();
            this->TokenJWT = ResultToMap["token"].toString();

            QVERIFY(this->TokenID > 0);
            QVERIFY(this->TokenJWT.isEmpty() == false);

        } QT_CATCH (const std::exception &exp) {
            QTest::qFail(exp.what(), __FILE__, __LINE__);
        }
    }

    void addToBasket_1() {
        QT_TRY {
            int ItemsCount = this->LastPreVoucher.Items.length();

            QVariant Result = callUserAPI(
                RESTClientHelper::POST,
                "MT/addToBasket",
                {},
                {
                    { "saleableCode",           this->MTSaleableCode },
//                    { "orderAdditives",         QVariantMap({ { "adtv1", "1 1 1" }, { "adtv2", 111 } }) },
                    { "qty",                    1 },
                    { "apiToken",               this->TokenJWT },
                    { "lastPreVoucher",         this->LastPreVoucher.toJson().toVariantMap() },
                }
            );

            stuBasketActionResult BasketActionResult;
            BasketActionResult.fromJson(Result.toJsonObject());
            this->LastPreVoucher = BasketActionResult.PreVoucher;

            auto item = this->LastPreVoucher.Items.last();
            QVERIFY(item.TotalPrice == 12'000);

            QVERIFY(this->LastPreVoucher.Items.length() > ItemsCount);

        } QT_CATCH (const std::exception &exp) {
            QTest::qFail(exp.what(), __FILE__, __LINE__);
        }
    }

    void finalizeBasket_1() {
        QT_TRY {
            QVariant Result = callUserAPI(
                RESTClientHelper::POST,
                "Account/finalizeBasket",
                {},
                {
                    { "preVoucher",             this->LastPreVoucher.toJson().toVariantMap() },
                    { "gatewayType",            "_DeveloperTest" },
                    { "domain",                 "dev.test" },
                    { "paymentVerifyCallback",  "http://127.0.0.1:10000/rest/v1/Account/OnlinePayments/devTestCallbackPage" },
                    { "allowChargeWallet",      true },
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

    void approveOnlinePayment_1() {
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
                QVERIFY(this->ApproveOnlinePaymentVoucher.Payed == 12'000);
                QVERIFY(this->ApproveOnlinePaymentVoucher.Remained == 0);

            } QT_CATCH (const std::exception &exp) {
                QTest::qFail(exp.what(), __FILE__, __LINE__);
            }
        }
    }

    void get_changed_token() {
        QT_TRY {
            QVariant Result = callUserAPI(
                RESTClientHelper::GET,
                "Account/APITokens/" + QString::number(this->TokenID)
            );

            QVERIFY(Result.isValid());

            this->TokenJWT = Result.toMap()["aptToken"].toString();

        } QT_CATCH (const std::exception &exp) {
            QTest::qFail(exp.what(), __FILE__, __LINE__);
        }
    }

    void translate_en2fa_1() {
        QT_TRY {
            QVariant Result = this->callAPI(
                        this->TokenJWT,
                        RESTClientHelper::enuHTTPMethod::POST,
                        "MT/translate",
                        {},
                        {
                            { "text", "This is a sample text for testing." },
                            { "dir", "en2fa" },
                        });

            QVERIFY(Result.isValid());

        } QT_CATCH (const std::exception &exp) {
            QTest::qFail(exp.what(), __FILE__, __LINE__);
        }
    }

    void translate_fa2en_1() {
        QT_TRY {
            QVariant Result = this->callAPI(
                        this->TokenJWT,
                        RESTClientHelper::enuHTTPMethod::POST,
                        "MT/translate",
                        {},
                        {
                            { "text", "این یک متن نمونه برای آزمایش است." },
                            { "dir", "fa2en" },
                        });

            QVERIFY(Result.isValid());

        } QT_CATCH (const std::exception &exp) {
            QTest::qFail(exp.what(), __FILE__, __LINE__);
        }
    }

};

#endif // TEST_MT_HPP
