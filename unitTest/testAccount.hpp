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
#ifndef TESTACCOUNT_HPP
#define TESTACCOUNT_HPP

#include "testCommon.hpp"
#include "3rdParty/QRestServer/libsrc/Private/QJWT.h"
#include "Helpers/AAA/clsJWT.hpp"

using namespace Targoman::API;
using namespace Targoman::API::Helpers::AAA;

class testAccount: public clsBaseTest
{
    Q_OBJECT

private slots:
    void initTestCase(){
        clsDAC DAC;
        DAC.execQuery("", "DELETE FROM AAA.tblUser WHERE usrEmail=?", {"unit_test@unittest.test"});
        DAC.execQuery("", "INSERT IGNORE INTO tblRoles SET rolName='unitTest', rolPrivileges='{}', rolSignupAllowedIPs='127.0.0.1'");
    }

    void cleanupTestCase(){
        // clsDAC DAC;
        // DAC.execQuery("", "DELETE FROM AAA.tblUser WHERE usrEmail=?", {"unit_test@unittest.test"});
    }

    void Signup(){
        QVERIFY((gUserID = callAPI(PUT,
                                        "Account/signup", {}, {
                                            {"emailOrMobile", "unit_test@unittest.test"},
                                            {"name", "unit"},
                                            {"family", "test"},
                                            {"pass", "81dc9bdb52d04dc20036dbd8313ed055"},
                                            {"role", "unitTest"}
                                        }).toUInt()) > 0);
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

    void Login(){
        QVERIFY((gEncodedJWT = callAPI(POST,
                                "Account/login",{},{
                                    {"login", "unit_test@unittest.test"},
                                    {"pass", "1113183d7dc42beec92b6a393230db4b"},
                                    {"salt", 1234},
                                }).toString()).size());
        gJWT = QJsonDocument::fromJson(QByteArray::fromBase64(gEncodedJWT.split('.').at(1).toLatin1())).object();
        qDebug()<<gJWT;

        QVERIFY(clsJWT(gJWT).usrID() == gUserID);
        QVERIFY(clsJWT(gJWT).usrStatus() == enuUserStatus::Active);
    }

    void Logout(){
        QVERIFY(callAPI(POST, "Account/logout").toBool());
        QVERIFY((gEncodedJWT = callAPI(POST, "Account/refreshJWT").toString()).isEmpty());
    }

//    void loginAsGuest(){
//        QVERIFY((callAPI(POST,
//                         "Account/loginAsGuest",{},{
//                             {"sessionInfo", "{\"a\":1}"},
//                        }).toString()).size());
//    }

    void LoginAgain(){
        QVERIFY((gEncodedJWT = callAPI(POST,
                                "Account/login",{},{
                                    {"login", "unit_test@unittest.test"},
                                    {"pass", "1113183d7dc42beec92b6a393230db4b"},
                                    {"salt", 1234},
                                }).toString()).size());
    }

    void RefreshJWT(){
        QVERIFY((gEncodedJWT = callAPI(GET, "Account/refreshJWT").toString()).size());
    }

    void CreateForgotPasswordLink(){
        QVERIFY(callAPI(POST,
                        "Account/createForgotPasswordLink",{},{
                            {"login", "unit_test@unittest.test"},
                            {"via", "Web"},
                        }).toBool());
    }

    void ChangePassByUUID(){
        clsDAC DAC;
        QString Code = DAC.execQuery("", "SELECT fprUUID FROM tblForgotPassRequest WHERE fpr_usrID=?",
        {gUserID}).toJson(true).object().value("fprUUID").toString();

        DAC.execQuery("", "UPDATE tblForgotPassRequest SET fprStatus = 'S' WHERE fprUUID=?",
        {Code});

        QVERIFY(callAPI(POST,
                        "Account/changePassByUUID", {},{
                            {"uuid", Code},
                            {"newPass", "827ccb0eea8a706c4c34a16891f84e7b"}
                        }).toBool());
    }

    void ChangePass(){
        QVERIFY(callAPI(POST,
                        "Account/changePass", {},{
                            {"oldPass", "d769dd673f86addfe039dc2d2dab4f73"},
                            {"oldPassSalt", 1234},
                            {"newPass", "81dc9bdb52d04dc20036dbd8313ed055"}
                        }).toBool());
    }

    void ApproveMobile(){
        clsDAC DAC;

        DAC.callSP("", "sp_CREATE_approvalRequest", {
                       {"iWhat2Approve", "M"},
                       {"iUserID", gUserID},
                       {"iValue", "09121234567"},
                       {"iPass","1113183d7dc42beec92b6a393230db4b"},
                       {"iSalt", 1234}
                   });

        QString Code = DAC.execQuery("", "SELECT aprApprovalCode FROM tblApprovalRequest WHERE apr_usrID=? AND aprRequestedFor = 'M'",
        {gUserID}).toJson(true).object().value("aprApprovalCode").toString();

        DAC.execQuery("", "UPDATE tblApprovalRequest SET aprStatus = 'S' WHERE apr_usrID=?",
        {gUserID});

        QVERIFY(callAPI(POST,
                        "Account/approveMobile", {},{
                            {"mobile", "09121234567"},
                            {"code", Code}
                        }).toBool());
        QString Mobile = DAC.execQuery("", "SELECT usrMobile FROM tblUser WHERE usrID=?",
        {gUserID}).toJson(true).object().value("usrMobile").toString();

        QVERIFY(Mobile == "09121234567");
    }
/**/
    //@TODO on update return updated info and update JWT
    //@TODO If no old mobile it must be set on mobile update
};


#endif // TESTACCOUNT_HPP
