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
        QVERIFY((MultiJWT = callAPI(RESTClientHelper::POST,
                                "Account/login",{},{
                                    {"emailOrMobile", UT_UserEmail},
                                    {"pass", "5d12d36cd5f66fe3e72f7b03cbb75333"},
                                    {"salt", 1234},
                                }).toJsonObject()).size());
        gEncodedJWT = MultiJWT.value("ssn").toString();
        gLoginJWT = MultiJWT.value("lgn").toString();
        gJWT = QJsonDocument::fromJson(QByteArray::fromBase64(gEncodedJWT.split('.').at(1).toLatin1())).object();

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

        QVERIFY(callAPI(RESTClientHelper::POST,
                        "Account/changePassByUUID", {},{
                            {"uuid", Code},
                            {"newPass", "827ccb0eea8a706c4c34a16891f84e7b"}
                        }).toBool());
    }

    void ChangePass(){
        QVERIFY(callAPI(RESTClientHelper::POST,
                        "Account/changePass", {},{
                            { "oldPass", "d769dd673f86addfe039dc2d2dab4f73" },
                            { "oldPassSalt", 1234 },
                            { "newPass", "df6d2338b2b8fce1ec2f6dda0a630eb0" }
                        }).toBool());
    }

    void ApproveMobile(){
        clsDAC DAC;

        try{
            DAC.callSP("", "sp_CREATE_approvalRequest", {
                           { "iWhat2Approve", "M" },
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
