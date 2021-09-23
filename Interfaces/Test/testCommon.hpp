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

#ifndef TESTCOMMON_HPP
#define TESTCOMMON_HPP

#include <QtTest/QtTest>

#include "libTargomanDBM/clsDAC.h"
using namespace Targoman::DBManager;

#include "Interfaces/Helpers/RESTClientHelper.h"
using namespace Targoman::API::Helpers;

constexpr char UT_UserEmail[] = "unit_test@unittest.test";
constexpr char UT_AdminUserEmail[] = "unit_test_admin@unittest.test";
constexpr quint32 UT_AdminRoleID = 3;
constexpr quint64 UT_SystemUserID = 1;

constexpr char UT_RoleName[] = "UnitTest_Role";
constexpr char UT_ServiceRoleName[] = "UnitTest_Service_Role";
constexpr char UT_ServiceName[] = "UnitTest_Service";
constexpr char UT_AdminToken[] = "Sample_UnitTest_Admin_Token";
constexpr char UT_NormalToken[] = "Sample_UnitTest_Token";

extern unsigned int gServiceRoleID;
extern unsigned int gServiceID;
extern unsigned int gAPITokenID;
extern unsigned int gAPIAdminTokenID;

#define TAPI_MARSHAL_TEST_VARIABLES \
    QString APIURL = "http://127.0.0.1:10000/rest/v1"; \
    QString gEncodedJWT; \
    QString gLoginJWT; \
    QString gEncodedAdminJWT; \
    QJsonObject gJWT; \
    QJsonObject gAdminJWT; \
    quint64 gUserID; \
    quint64 gAdminUserID; \
    QVariant gInvalid; \
    unsigned int gServiceRoleID; \
    unsigned int gServiceID; \
    unsigned int gAPITokenID; \
    unsigned int gAPIAdminTokenID;

extern QString APIURL;
extern QString gEncodedJWT;
extern QString gLoginJWT;
extern QJsonObject gJWT;
extern QJsonObject gAdminJWT;
extern QString gEncodedAdminJWT;
extern quint64 gUserID;
extern quint64 gAdminUserID;
extern QVariant gInvalid;

class clsBaseTest : public QObject
{
protected:
    void initUnitTestData(bool _createUsers = true, bool _deleteOldTestData = true)
    {
        if (_deleteOldTestData)
            cleanupUnitTestData();
//            deleteOldTestData();

        clsDAC DAC;

        clsDACResult Result = DAC.execQuery("", "INSERT IGNORE INTO tblRoles SET rolName=?, rolCreatedBy_usrID=?", { UT_RoleName, UT_SystemUserID });
        auto rolID = Result.lastInsertId().toUInt();

        if (rolID == 0)
            clsDACResult Result2 = DAC.execQuery("", "SELECT rolID FROM tblRoles WHERE rolName=? AND rolStatus <> 'R'", { UT_RoleName });

        if (_createUsers) {
            DAC.execQuery("", R"(
          INSERT IGNORE
            INTO tblUser
             SET usrEmail = ?,
                 usrName = 'unit',
                 usrFamily = 'test',
                 usrPass = 'df6d2338b2b8fce1ec2f6dda0a630eb0',
                 usr_rolID = ?,
                 usrCreatedBy_usrID = ?
                )",
                {
                    UT_UserEmail,
                    rolID,
                    UT_SystemUserID
                }
            );

            clsDACResult res2 = DAC.execQuery("", R"(
          INSERT IGNORE
            INTO tblUser
             SET usrEmail = ?,
                 usrName = 'admin unit',
                 usrFamily = 'test',
                 usrPass = 'df6d2338b2b8fce1ec2f6dda0a630eb0',
                 usr_rolID = ?,
                 usrCreatedBy_usrID = ?
                )",
                {
                    UT_AdminUserEmail,
                    UT_AdminRoleID,
                    UT_SystemUserID
                }
            );

            gAdminUserID = res2.lastInsertId().toULongLong();

            DAC.execQuery("", "UPDATE tblUser SET tblUser.usr_rolID=3 WHERE tblUser.usrID=?", { gAdminUserID });
        }
    }

    virtual void cleanupUnitTestData() {
        deleteOldTestData();
    }

    QVariant callRefreshAPI() {
        return RESTClientHelper::callAPI(
            gLoginJWT, RESTClientHelper::GET, "Account/refreshJWT", {}, {}, APIURL
        );
    }

    QVariant callAPI(
            RESTClientHelper::enuHTTPMethod _method,
            const QString& _api,
            const QVariantMap& _urlArgs = {},
            const QVariantMap& _postFields = {}
        )
    {
        return RESTClientHelper::callAPI(
            gEncodedJWT, _method, _api, _urlArgs, _postFields, APIURL
        );
    }

    QVariant callAdminAPI(
            RESTClientHelper::enuHTTPMethod _method,
            const QString& _api,
            const QVariantMap& _urlArgs = {},
            const QVariantMap& _postFields = {}
        )
    {
        return RESTClientHelper::callAPI(
            gEncodedAdminJWT, _method, _api, _urlArgs, _postFields, APIURL
        );
    }

private:
    void deleteOldTestData() //bool _createUsers=false)
    {
        clsDAC DAC;

        DAC.execQuery("", "DELETE FROM AAA.tblAPITokens WHERE aptToken IN(?,?)", { UT_NormalToken, UT_AdminToken });
        DAC.execQuery("", "DELETE FROM AAA.tblService WHERE svcName=?", { UT_ServiceName });

        DAC.execQuery("", "UPDATE AAA.tblUser SET usrUpdatedBy_usrID=NULL WHERE usrEmail IN(?,?)", { UT_UserEmail, UT_AdminUserEmail });
//        DAC.execQuery("", "DELETE FROM AAA.tblUser WHERE usrEmail IN (?,?)", {UT_UserEmail, UT_AdminUserEmail});
//        DAC.execQuery("", "DELETE FROM AAA.tblRoles WHERE rolName IN(?,?)", {UT_ServiceRoleName, UT_RoleName});

        /*
DELETE FROM tblWalletsTransactions;
DELETE FROM tblWalletBalances;
DELETE FROM tblUserWallets;
DELETE FROM tblOnlinePayments;
DELETE FROM tblOfflinePayments;
DELETE FROM tblVoucher;
DELETE FROM tblApprovalRequest;
DELETE FROM tblActiveSessions;
DELETE FROM tblUser WHERE usrID > 100;
DELETE FROM tblRoles WHERE LOWER(rolName) LIKE '%test%'
        */
    }
};

#endif // TESTCOMMON_HPP
