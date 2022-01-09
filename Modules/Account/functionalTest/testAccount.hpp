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

#ifndef TEST_ACCOUNT_HPP
#define TEST_ACCOUNT_HPP

#include "Interfaces/Test/testCommon.hpp"
#include "Interfaces/AAA/clsJWT.hpp"
#include "ORM/actionLogs.hpp"
#include "ORM/activeSessions.hpp"
#include "ORM/roles.hpp"
#include "ORM/service.hpp"
#include "ORM/tokens.hpp"
#include <cstdlib>
#include <unistd.h>

using namespace Targoman::API;
using namespace Targoman::API::AAA;

class testAccount : public clsBaseTest
{
    Q_OBJECT

private slots:
    void initTestCase() {
        initUnitTestData(false);
    }

    void cleanupTestCase() {
//        cleanupUnitTestData();
    }

    //-------------------------------------------------------
    void NormalizePhoneNumber()
    {
        QVariant Result = callAPI(
                              RESTClientHelper::POST,
                              "Account/normalizePhoneNumber",
                              {},
                              {
                                  { "phone", "0999-888-1010" },
                                  { "country", "IR" },
                               })
                          ;

        QVERIFY(Result.toString() == "+989998881010");
    }

    void SignupByMobileOnly_0999_888_1010()
    {
        QVariant Result = callAPI(
                              RESTClientHelper::POST,
                              "Account/loginByMobileOnly",
                              {},
                              {
                                  { "mobile", "0999-888-1010" },
                                  { "signupIfNotExists", true },
                                  { "signupRole", UT_RoleName },
                               })
                          ;

//        qDebug() << Result;
//        QVERIFY(UserID > 0);

//        clsDAC DAC;
//        DAC.execQuery("", "UPDATE tblUser SET tblUser.usr_rolID=? WHERE tblUser.usrID=?", {UT_AdminRoleID, gAdminUserID});
    }

    void ApproveMobile_And_Login_0999_888_1010()
    {
        clsDAC DAC;
        QJsonObject AprInfo = DAC.execQuery("",
                                            "SELECT aprID"
                                            "     , apr_usrID"
                                            "     , aprApprovalCode"
                                            "  FROM tblApprovalRequest"
                                            " WHERE aprApprovalKey = '+989998881010'"
                                            " ORDER BY aprID DESC"
                                            )
                              .toJson(true)
                              .object()
                              ;

        quint64 AprID = AprInfo
                       .value("aprID")
                       .toDouble();
        QVERIFY(AprID > 0);

        quint64 UserID = AprInfo
                       .value("apr_usrID")
                       .toDouble();
        QVERIFY(AprID > 0);

        QString Code = AprInfo
                       .value("aprApprovalCode")
                       .toString();
        QVERIFY(Code.isEmpty() == false);

        DAC.execQuery("",
                      "UPDATE tblApprovalRequest"
                      "   SET aprStatus = 'S'"
                      "     , aprSentDate = NOW()"
                      " WHERE aprID=?",
                      {
                          AprID
                      });

        QJsonObject MultiJWT;
        QVERIFY((MultiJWT = callAPI(
                    RESTClientHelper::POST,
                    "Account/approveMobile",
                    {},
                    {
                        { "mobile", "0999-888-1010" },
                        { "code", Code },
                        { "autoLogin", true },
//                        { "services",  },
//                        { "rememberMe",  },
//                        { "sessionInfo",  },
//                        { "fingerprint",  },
                    })
                .toJsonObject()).size());

        gEncodedJWT = MultiJWT.value("ssn").toString();
        gJWT = QJsonDocument::fromJson(QByteArray::fromBase64(gEncodedJWT.split('.').at(1).toLatin1())).object();

        QVERIFY(clsJWT(gJWT).usrID() == UserID);
        QVERIFY(clsJWT(gJWT).usrStatus() == TAPI::enuUserStatus::Active);
    }

    void Logout__0999_888_1010()
    {
        QVERIFY(callAPI(RESTClientHelper::POST, "Account/logout").toBool());
        QVERIFY((gEncodedJWT = callAPI(RESTClientHelper::POST, "Account/refreshJWT").toString()).isEmpty());
    }

    //-------------------------------------------------------
    void Signup()
    {
        //df6d2338b2b8fce1ec2f6dda0a630eb0 # 987
        QVERIFY((gUserID = callAPI(RESTClientHelper::PUT,
                                        "Account/signup", {}, {
                                            {"emailOrMobile", UT_UserEmail},
                                            {"name", "unit"},
                                            {"family", "test"},
                                            {"pass", "df6d2338b2b8fce1ec2f6dda0a630eb0"},
                                            {"role", UT_RoleName}
                                        })
                           .toMap()
                           .value("usrID")
                           .toULongLong()) > 0)
                ;

        //df6d2338b2b8fce1ec2f6dda0a630eb0 # 987
        QVERIFY((gAdminUserID = callAPI(RESTClientHelper::PUT,
                                        "Account/signup", {}, {
                                            {"emailOrMobile", UT_AdminUserEmail},
                                            {"name", "admin unit"},
                                            {"family", "test"},
                                            {"pass", "df6d2338b2b8fce1ec2f6dda0a630eb0"},
                                            {"role", UT_RoleName}
                                        })
                                .toMap()
                                .value("usrID")
                                .toULongLong()) > 0)
                ;

        clsDAC DAC;
        DAC.execQuery("", "UPDATE tblUser SET tblUser.usr_rolID=? WHERE tblUser.usrID=?", {UT_AdminRoleID, gAdminUserID});
    }

    void ResendEmailApproveCode()
    {
        QVariant Result = callAPI(RESTClientHelper::POST,
                                  "Account/resendApprovalCode",
                                  {},
                                  {
                                      { "emailOrMobile", UT_UserEmail },
                                  });
        QVERIFY(Result.toBool());
    }

    void ApprovalRequest_timerInfo_before_send()
    {
        QVariant Result = callAPI(RESTClientHelper::POST,
                                  "Account/ApprovalRequest/timerInfo",
                                  {},
                                  {
                                      { "emailOrMobile", UT_UserEmail },
                                  });
        qDebug() << Result;
        QVERIFY(Result.isValid() == false);
    }

    void ApprovalRequest_timerInfo_after_send()
    {
        clsDAC DAC;
        DAC.execQuery("", "UPDATE tblApprovalRequest SET aprStatus = 'S', aprSentDate = DATE_SUB(NOW(), INTERVAL 10 SECOND) WHERE apr_usrID=?",
        {gUserID});

        QVariant Result = callAPI(RESTClientHelper::POST,
                                  "Account/ApprovalRequest/timerInfo",
                                  {},
                                  {
                                      { "emailOrMobile", UT_UserEmail },
                                  });
        qDebug() << Result;
        QVERIFY(Result.isValid());
    }
//private:
    void ApproveEmail()
    {
        clsDAC DAC;
        QString Code = DAC.execQuery("",
                                     "SELECT aprApprovalCode"
                                     "  FROM tblApprovalRequest"
                                     " WHERE apr_usrID=?",
                                     {
                                         gUserID
                                     })
                       .toJson(true)
                       .object()
                       .value("aprApprovalCode")
                       .toString()
                       ;

        DAC.execQuery("", "UPDATE tblApprovalRequest SET aprStatus = 'S', aprSentDate = NOW() WHERE apr_usrID=?",
        {gUserID});

        QVariant Result = callAPI(RESTClientHelper::POST,
                                  "Account/approveEmail",
                                  {},
                                  {
                                      { "email", UT_UserEmail },
                                      { "uuid", Code },
                                  });
    }

    void ApproveAdminEmail()
    {
        clsDAC DAC;
        QString Code = DAC.execQuery("", "SELECT aprApprovalCode FROM tblApprovalRequest WHERE apr_usrID=?",
        {gAdminUserID}).toJson(true).object().value("aprApprovalCode").toString();

        DAC.execQuery("", "UPDATE tblApprovalRequest SET aprStatus = 'S', aprSentDate = NOW() WHERE apr_usrID=?",
        {gAdminUserID});

        QVariant Result = callAPI(RESTClientHelper::POST,
                                  "Account/approveEmail",
                                  {},
                                  {
                                      { "email", UT_AdminUserEmail },
                                      { "uuid", Code },
                                  });
    }

    void Login()
    {
        //5d12d36cd5f66fe3e72f7b03cbb75333 = MD5(1234 + df6d2338b2b8fce1ec2f6dda0a630eb0 # 987)
        QJsonObject MultiJWT;
        QVERIFY((MultiJWT = callAPI(RESTClientHelper::POST,
                                "Account/login",{},{
                                    {"emailOrMobile", UT_UserEmail},
                                    {"pass", "5d12d36cd5f66fe3e72f7b03cbb75333"},
                                    {"salt", 1234},
                                }).toJsonObject()).size());

        gEncodedJWT = MultiJWT.value("ssn").toString();
        gJWT = QJsonDocument::fromJson(QByteArray::fromBase64(gEncodedJWT.split('.').at(1).toLatin1())).object();

        QVERIFY(clsJWT(gJWT).usrID() == gUserID);
        QVERIFY(clsJWT(gJWT).usrStatus() == TAPI::enuUserStatus::Active);
    }

    void Logout(){
        QVERIFY(callAPI(RESTClientHelper::POST, "Account/logout").toBool());
        QVERIFY((gEncodedJWT = callAPI(RESTClientHelper::POST, "Account/refreshJWT").toString()).isEmpty());
    }

//    void loginAsGuest(){
//        QVERIFY((callAPI(RESTClientHelper::POST,
//                         "Account/loginAsGuest",{},{
//                             {"sessionInfo", "{\"a\":1}"},
//                        }).toString()).size());
//    }

    void LoginAgain(){
        QJsonObject MultiJWT;
        //5d12d36cd5f66fe3e72f7b03cbb75333 = MD5(1234 + df6d2338b2b8fce1ec2f6dda0a630eb0 # 987)
        QVERIFY((MultiJWT = callAPI(RESTClientHelper::POST,
                                "Account/login",{},{
                                    {"emailOrMobile", UT_UserEmail},
                                    {"pass", "5d12d36cd5f66fe3e72f7b03cbb75333"},
                                    {"salt", 1234},
                                }).toJsonObject()).size());
        gEncodedJWT = MultiJWT.value("ssn").toString();
        gLoginJWT = MultiJWT.value("lgn").toString();
        gJWT = QJsonDocument::fromJson(QByteArray::fromBase64(gEncodedJWT.split('.').at(1).toLatin1())).object();

        //5d12d36cd5f66fe3e72f7b03cbb75333 = MD5(1234 + df6d2338b2b8fce1ec2f6dda0a630eb0 # 987)
        QVERIFY((MultiJWT = callAPI(RESTClientHelper::POST,
                                "Account/login",{},{
                                    {"emailOrMobile", UT_AdminUserEmail},
                                    {"pass", "5d12d36cd5f66fe3e72f7b03cbb75333"},
                                    {"salt", 1234},
                                }).toJsonObject()).size());
        gEncodedAdminJWT = MultiJWT.value("ssn").toString();
        gAdminJWT = QJsonDocument::fromJson(QByteArray::fromBase64(gEncodedAdminJWT.split('.').at(1).toLatin1())).object();
    }

    void RefreshJWT(){
        QJsonObject MultiJWT;

        QVERIFY((MultiJWT = callRefreshAPI().toJsonObject()).size());
        gEncodedJWT = MultiJWT.value("ssn").toString();
    }

    void CreateForgotPasswordLink(){
        QVERIFY(callAPI(RESTClientHelper::POST,
                        "Account/createForgotPasswordLink",{},{
                            {"login", UT_UserEmail},
                            {"via", "Web"},
                        }).toBool());
    }

    void ChangePassByUUID(){
        clsDAC DAC;
        QString Code = DAC.execQuery("", "SELECT fprUUID FROM tblForgotPassRequest WHERE fpr_usrID=?",
        {gUserID}).toJson(true).object().value("fprUUID").toString();

        DAC.execQuery("", "UPDATE tblForgotPassRequest SET fprStatus = 'S' WHERE fprUUID=?",
        {Code});

        //827ccb0eea8a706c4c34a16891f84e7b # 12345
        QVERIFY(callAPI(RESTClientHelper::POST,
                        "Account/changePassByUUID", {},{
                            { "uuid", Code },
                            { "newPass", "827ccb0eea8a706c4c34a16891f84e7b" }
                        }).toBool());
    }

    //d769dd673f86addfe039dc2d2dab4f73 = MD5(1234 + 827ccb0eea8a706c4c34a16891f84e7b # 12345)
    //df6d2338b2b8fce1ec2f6dda0a630eb0 # 987
    void ChangePass(){
        QVERIFY(callAPI(RESTClientHelper::POST,
                        "Account/changePass", {},{
                            { "oldPass", "d769dd673f86addfe039dc2d2dab4f73" },
                            { "oldPassSalt", 1234 },
                            { "newPass", "df6d2338b2b8fce1ec2f6dda0a630eb0" }
                        }).toBool());
    }

    void ResendMobileApproveCode()
    {
        clsDAC DAC;

        try {
            //5d12d36cd5f66fe3e72f7b03cbb75333 = MD5(1234 + df6d2338b2b8fce1ec2f6dda0a630eb0 # 987)
            DAC.callSP("", "sp_CREATE_approvalRequest", {
                           { "iBy", "M" },
                           { "iUserID", gUserID },
                           { "iKey", "+989998882020" },
                           { "iPass", "5d12d36cd5f66fe3e72f7b03cbb75333" },
                           { "iSalt", 1234 }
                       });

            QJsonObject Obj = DAC.execQuery("", "SELECT aprApprovalCode FROM tblApprovalRequest WHERE apr_usrID=? AND aprRequestedFor = 'M'",
            { gUserID }).toJson(true).object();

            DAC.execQuery("", "UPDATE tblApprovalRequest SET aprStatus = 'S', aprSentDate = NOW() WHERE apr_usrID=?",
            { gUserID });

            //----------
            QVariant Result = callAPI(RESTClientHelper::POST,
                                      "Account/resendApprovalCode",
                                      {},
                                      {
                                          { "emailOrMobile", "+989998882020" },
                                      });
            QVERIFY(Result.toBool());
        }
        catch(std::exception &e) {
            QFAIL (e.what());
        }
    }

    void ApproveMobile(){
        clsDAC DAC;

        try {
            //5d12d36cd5f66fe3e72f7b03cbb75333 = MD5(1234 + df6d2338b2b8fce1ec2f6dda0a630eb0 # 987)
            DAC.callSP("", "sp_CREATE_approvalRequest", {
                           { "iBy", "M" },
                           { "iUserID", gUserID },
                           { "iKey", "+989998882020" },
                           { "iPass", "5d12d36cd5f66fe3e72f7b03cbb75333" },
                           { "iSalt", 1234 }
                       });

            QJsonObject Obj = DAC.execQuery("", "SELECT aprApprovalCode FROM tblApprovalRequest WHERE apr_usrID=? AND aprRequestedFor = 'M'",
            {gUserID}).toJson(true).object();

            QString Code = Obj.value("aprApprovalCode").toString();

            DAC.execQuery("", "UPDATE tblApprovalRequest SET aprStatus = 'S', aprSentDate = NOW() WHERE apr_usrID=?",
            {gUserID});

            QVariant Result = callAPI(RESTClientHelper::POST,
                            "Account/approveMobile", {},{
                                { "mobile", "09998882020" },
                                { "code", Code }
                            });

            QString Mobile = DAC.execQuery("", "SELECT usrMobile FROM tblUser WHERE usrID=?",
            {gUserID}).toJson(true).object().value("usrMobile").toString();

            QVERIFY(Mobile == "+989998882020");
        }catch(std::exception &e){
            QFAIL (e.what());
        }
    }

    void User_Photo_Set() {
        QVariant Result = callAPI(RESTClientHelper::PATCH,
                                  "Account/User/photo",
                                  {},
                                  {
                                      { "image", "data:image/png;base64,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" },
                                  });

        qDebug() << Result;
    }
    void User_Photo_Get() {
        QVariant Result = callAPI(RESTClientHelper::GET,
                                  "Account/User/photo",
                                  {
                                      { "usrID", gUserID },
                                  },
                                  {}
                                  );

        qDebug() << Result;
    }
    void User_Photo_Delete() {
        QVariant Result = callAPI(RESTClientHelper::DELETE,
                                  "Account/User/photo",
                                  {},
                                  {}
                                  );

        qDebug() << Result;
    }

    ///TODO: test [DELETE]  Account/User/photo
    ///TODO: test [PATCH]   Account/User/email
    ///TODO: test [PATCH]   Account/User/mobile

    void UpdateUserPersonalInfo() {
        QVERIFY(callAPI(RESTClientHelper::PATCH,
                        "Account/User/personalInfo",
                        {},
                        {
                            { "name", "hassan" },
                            { "family", "taghi" },
                            { "language", "fa" },
                            { "gender", "Male" },
                        })
                .toBool());
    }

    void UpdateUserFinancialInfo() {
        QVERIFY(callAPI(RESTClientHelper::PATCH,
                        "Account/User/financialInfo",
                        {},
                        {
                            { "iban", "IR112222222222" },
                            { "ether", "zzzzzzzzzzzz" },
                        })
                .toBool());
    }

    void UpdateUserExtraInfo_json() {
        QVERIFY(callAPI(RESTClientHelper::PATCH,
                        "Account/User/extraInfo",
                        {},
                        {
//                            { "job", "1234" },
                            { "education", "456" },
                        })
                .toBool());
    }

    void UpdateUserExtraInfo_birthdate() {
        QVERIFY(callAPI(RESTClientHelper::PATCH,
                        "Account/User/extraInfo",
                        {},
                        {
                            { "birthDate", "1973/09/11" },
                        })
                .toBool());
    }

    void UpdateUserExtraInfo_clearBirthDate() {
        QVERIFY(callAPI(RESTClientHelper::PATCH,
                        "Account/User/extraInfo",
                        {},
                        {
                            { "birthDate", "" },
                        })
                .toBool());
    }

    void UpdateUserExtraInfo_all() {
        QVERIFY(callAPI(RESTClientHelper::PATCH,
                        "Account/User/extraInfo",
                        {},
                        {
                            { "birthDate", "1973/09/11" },
                            { "job", "1234" },
                            { "education", "" },
                        })
                .toBool());
    }

    void User_Get_With_ExtraInfo() {
        QVariant Result = callAPI(RESTClientHelper::GET,
                                  QString("Account/User/%1").arg(gUserID)
                                  );

        qDebug() << Result;
    }

private slots:
//private:
    /***************************************************************************************/
    /* cleanup *****************************************************************************/
    /***************************************************************************************/
    void cleanupAll()
    {
        QVariantMap Result;

        clsDAC DAC;

        try
        {
            QString QueryString = R"(
                DELETE wb
                  FROM tblWalletBalances wb
            INNER JOIN tblWalletsTransactions wt
                    ON wt.wltID = wb.wbl_wltID
            INNER JOIN tblUserWallets uw
                    ON uw.walID = wt.wlt_walID
            INNER JOIN tblUser u
                    ON u.usrID = uw.wal_usrID
                 WHERE LOWER(u.usrEmail) LIKE 'unit_test%'
                    OR u.usrMobile LIKE '+98999888%'
            ;)";
            clsDACResult DACResult = DAC.execQuery("", QueryString);
            Result.insert("tblWalletBalances", QVariantMap({{ "numRowsAffected", DACResult.numRowsAffected() }}));
        }
        catch(...)
        {
        }

        try
        {
            QString QueryString = R"(
                DELETE wt
                  FROM tblWalletsTransactions wt
            INNER JOIN tblUserWallets uw
                    ON uw.walID = wt.wlt_walID
            INNER JOIN tblUser u
                    ON u.usrID = uw.wal_usrID
                 WHERE LOWER(u.usrEmail) LIKE 'unit_test%'
                    OR u.usrMobile LIKE '+98999888%'
            ;)";
            clsDACResult DACResult = DAC.execQuery("", QueryString);
            Result.insert("tblWalletsTransactions", QVariantMap({{ "numRowsAffected", DACResult.numRowsAffected() }}));
        }
        catch(...)
        {
        }

        try
        {
            QString QueryString = R"(
                DELETE uw
                  FROM tblUserWallets uw
            INNER JOIN tblUser u
                    ON u.usrID = uw.wal_usrID
                 WHERE LOWER(u.usrEmail) LIKE 'unit_test%'
                    OR u.usrMobile LIKE '+98999888%'
            ;)";
            clsDACResult DACResult = DAC.execQuery("", QueryString);
            Result.insert("tblUserWallets", QVariantMap({{ "numRowsAffected", DACResult.numRowsAffected() }}));
        }
        catch(...)
        {
        }

        try
        {
            QString QueryString = R"(
                DELETE op
                  FROM tblOnlinePayments op
            INNER JOIN tblVoucher vch
                    ON vch.vchID = op.onp_vchID
            INNER JOIN tblUser u
                    ON u.usrID = vch.vch_usrID
                 WHERE LOWER(u.usrEmail) LIKE 'unit_test%'
                    OR u.usrMobile LIKE '+98999888%'
            ;)";
            clsDACResult DACResult = DAC.execQuery("", QueryString);
            Result.insert("tblOnlinePayments", QVariantMap({{ "numRowsAffected", DACResult.numRowsAffected() }}));
        }
        catch(...)
        {
        }

        try
        {
            QString QueryString = R"(
                DELETE fp
                  FROM tblOfflinePayments fp
            INNER JOIN tblVoucher vch
                    ON vch.vchID = fp.ofp_vchID
            INNER JOIN tblUser u
                    ON u.usrID = vch.vch_usrID
                 WHERE LOWER(u.usrEmail) LIKE 'unit_test%'
                    OR u.usrMobile LIKE '+98999888%'
            ;)";
            clsDACResult DACResult = DAC.execQuery("", QueryString);
            Result.insert("tblOfflinePayments", QVariantMap({{ "numRowsAffected", DACResult.numRowsAffected() }}));
        }
        catch(...)
        {
        }

        try
        {
            QString QueryString = R"(
                DELETE vch
                  FROM tblVoucher vch
            INNER JOIN tblUser u
                    ON u.usrID = vch.vch_usrID
                 WHERE LOWER(u.usrEmail) LIKE 'unit_test%'
                    OR u.usrMobile LIKE '+98999888%'
            ;)";
            clsDACResult DACResult = DAC.execQuery("", QueryString);
            Result.insert("tblVoucher", QVariantMap({{ "numRowsAffected", DACResult.numRowsAffected() }}));
        }
        catch(...)
        {
        }

        try
        {
            QString QueryString = R"(
                DELETE apr
                  FROM tblApprovalRequest apr
            INNER JOIN tblUser u
                    ON u.usrID = apr.apr_usrID
                 WHERE LOWER(u.usrEmail) LIKE 'unit_test%'
                    OR u.usrMobile LIKE '+98999888%'
            ;)";
            clsDACResult DACResult = DAC.execQuery("", QueryString);
            Result.insert("tblApprovalRequest", QVariantMap({{ "numRowsAffected", DACResult.numRowsAffected() }}));
        }
        catch(...)
        {
        }

        try
        {
            QString QueryString = R"(
                DELETE sn
                  FROM tblActiveSessions sn
            INNER JOIN tblUser u
                    ON u.usrID = sn.ssn_usrID
                 WHERE LOWER(u.usrEmail) LIKE 'unit_test%'
                    OR u.usrMobile LIKE '+98999888%'
            ;)";
            clsDACResult DACResult = DAC.execQuery("", QueryString);
            Result.insert("tblActiveSessions", QVariantMap({{ "numRowsAffected", DACResult.numRowsAffected() }}));
        }
        catch(...)
        {
        }

        try
        {
            QString QueryString = R"(
                DELETE u
                  FROM tblUser u
                 WHERE LOWER(u.usrEmail) LIKE 'unit_test%'
                    OR u.usrMobile LIKE '+98999888%'
            ;)";
            clsDACResult DACResult = DAC.execQuery("", QueryString);
            Result.insert("tblUser", QVariantMap({{ "numRowsAffected", DACResult.numRowsAffected() }}));
        }
        catch(...)
        {
        }

        try
        {
            QString QueryString = R"(
                DELETE r
                  FROM tblRoles r
                 WHERE LOWER(r.rolName) LIKE 'unit_test%'
            ;)";
            clsDACResult DACResult = DAC.execQuery("", QueryString);
            Result.insert("tblRoles", QVariantMap({{ "numRowsAffected", DACResult.numRowsAffected() }}));
        }
        catch(...)
        {
        }

        qDebug() << Result;
    }
/*
DELETE wb
    FROM tblWalletBalances wb
    INNER JOIN tblWalletsTransactions wt
    ON wt.wltID = wb.wbl_wltID
    INNER JOIN tblUserWallets uw
    ON uw.walID = wt.wlt_walID
    INNER JOIN tblUser u
    ON u.usrID = uw.wal_usrID
    WHERE LOWER(u.usrEmail) LIKE 'unit_test%'
    OR u.usrMobile LIKE '+98999888%'
;

DELETE wt
    FROM tblWalletsTransactions wt
    INNER JOIN tblUserWallets uw
    ON uw.walID = wt.wlt_walID
    INNER JOIN tblUser u
    ON u.usrID = uw.wal_usrID
    WHERE LOWER(u.usrEmail) LIKE 'unit_test%'
    OR u.usrMobile LIKE '+98999888%'
;

DELETE uw
    FROM tblUserWallets uw
    INNER JOIN tblUser u
    ON u.usrID = uw.wal_usrID
    WHERE LOWER(u.usrEmail) LIKE 'unit_test%'
    OR u.usrMobile LIKE '+98999888%'
;

DELETE op
    FROM tblOnlinePayments op
    INNER JOIN tblVoucher vch
    ON vch.vchID = op.onp_vchID
    INNER JOIN tblUser u
    ON u.usrID = vch.vch_usrID
    WHERE LOWER(u.usrEmail) LIKE 'unit_test%'
    OR u.usrMobile LIKE '+98999888%'
;

DELETE fp
    FROM tblOfflinePayments fp
    INNER JOIN tblVoucher vch
    ON vch.vchID = fp.ofp_vchID
    INNER JOIN tblUser u
    ON u.usrID = vch.vch_usrID
    WHERE LOWER(u.usrEmail) LIKE 'unit_test%'
    OR u.usrMobile LIKE '+98999888%'
;

DELETE vch
    FROM tblVoucher vch
    INNER JOIN tblUser u
    ON u.usrID = vch.vch_usrID
    WHERE LOWER(u.usrEmail) LIKE 'unit_test%'
    OR u.usrMobile LIKE '+98999888%'
;

DELETE apr
    FROM tblApprovalRequest apr
    INNER JOIN tblUser u
    ON u.usrID = apr.apr_usrID
    WHERE LOWER(u.usrEmail) LIKE 'unit_test%'
    OR u.usrMobile LIKE '+98999888%'
;

DELETE sn
    FROM tblActiveSessions sn
    INNER JOIN tblUser u
    ON u.usrID = sn.ssn_usrID
    WHERE LOWER(u.usrEmail) LIKE 'unit_test%'
    OR u.usrMobile LIKE '+98999888%'
;

DELETE u
    FROM tblUser u
    WHERE LOWER(u.usrEmail) LIKE 'unit_test%'
    OR u.usrMobile LIKE '+98999888%'
;

DELETE r
    FROM tblRoles r
    WHERE LOWER(r.rolName) LIKE 'unit_test%'
;
*/

};

#endif // TEST_ACCOUNT_HPP
