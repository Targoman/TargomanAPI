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

constexpr quint64 UT_SystemUserID       = 1;
constexpr quint32 UT_AdminRoleID        = 3;
//all this constants must be begin with unit_test:
constexpr char UT_UserEmail[]           = "unit_test@unittest.test";
constexpr char UT_AdminUserEmail[]      = "unit_test_admin@unittest.test";
constexpr char UT_RoleName[]            = "unit_test_Role";
constexpr char UT_ServiceRoleName[]     = "unit_test_Service_Role";
constexpr char UT_ServiceName[]         = "unit_test_Service";
constexpr char UT_AdminToken[]          = "unit_test_Admin_Token";
constexpr char UT_NormalToken[]         = "unit_test_Token";

extern unsigned int gServiceRoleID;
extern unsigned int gServiceID;
extern unsigned int gAPITokenID;
extern unsigned int gAPIAdminTokenID;

//QString gLoginJWT;
#define TAPI_MARSHAL_TEST_VARIABLES \
    QString APIURL = "http://127.0.0.1:10000/rest/v1"; \
    QString gEncodedJWT; \
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
//extern QString gLoginJWT;
extern QJsonObject gJWT;
extern QJsonObject gAdminJWT;
extern QString gEncodedAdminJWT;
extern quint64 gUserID;
extern quint64 gAdminUserID;
extern QVariant gInvalid;

inline char **findDBPrefixFromArguments(int argc, char *argv[], QString &_dbPrefix, int &_outArgsCount) {
    std::list<std::string> commandList;
    commandList.push_back(argv[0]);
    if (argc > 1) {
        for (int idx=1; idx<argc; idx++) {
            if ((qstricmp(argv[idx], "dbprefix") == 0)
                    || (qstricmp(argv[idx], "-dbprefix") == 0)
                    || (qstricmp(argv[idx], "--dbprefix") == 0)
                ) {
                if (argc-1 > idx) {
                    _dbPrefix = argv[idx+1];
                    ++idx;
                }
            } else
                commandList.push_back(argv[idx]);
        }
    }

    char **OutArgs = (char**)malloc(sizeof(char*) * commandList.size());
    _outArgsCount = 0;

    for (std::list<std::string>::iterator it=commandList.begin(); it!=commandList.end(); ++it) {
        char *item = new char[strlen((*it).c_str()) + 1];
        strcpy(item, (*it).c_str());
        OutArgs[_outArgsCount] = item;
        ++_outArgsCount;
    }

    return OutArgs;
}

class clsBaseTest : public QObject
{
public:
    clsBaseTest(const QString &_dbPrefix) : DBPrefix(_dbPrefix) { ; }
    QString DBPrefix;

protected:
    void initUnitTestData(bool _createUsers = true, bool _deleteOldTestData = true) {
        if (_deleteOldTestData)
            cleanupUnitTestData();
//            deleteOldTestData();

        clsDAC DAC("AAA");

        clsDACResult Result = DAC.execQuery("", QString("INSERT IGNORE INTO %1AAA.tblRoles SET rolName=?, rolCreatedBy_usrID=?").arg(this->DBPrefix), { UT_RoleName, UT_SystemUserID });
        auto rolID = Result.lastInsertId().toUInt();

        if (rolID == 0)
            clsDACResult Result2 = DAC.execQuery("", QString("SELECT rolID FROM %1AAA.tblRoles WHERE rolName=? AND rolStatus <> 'R'").arg(this->DBPrefix), { UT_RoleName });

        //df6d2338b2b8fce1ec2f6dda0a630eb0 # 987
        if (_createUsers) {
            DAC.execQuery("", QString(R"(
          INSERT IGNORE
            INTO %1AAA.tblUser
             SET usrEmail = ?,
                 usrName = 'unit',
                 usrFamily = 'test',
                 usrPass = 'df6d2338b2b8fce1ec2f6dda0a630eb0',
                 usr_rolID = ?,
                 usrCreatedBy_usrID = ?
)").arg(this->DBPrefix),
                {
                    UT_UserEmail,
                    rolID,
                    UT_SystemUserID
                }
            );

            //df6d2338b2b8fce1ec2f6dda0a630eb0 # 987
            clsDACResult res2 = DAC.execQuery("", QString(R"(
          INSERT IGNORE
            INTO %1AAA.tblUser
             SET usrEmail = ?,
                 usrName = 'admin unit',
                 usrFamily = 'test',
                 usrPass = 'df6d2338b2b8fce1ec2f6dda0a630eb0',
                 usr_rolID = ?,
                 usrCreatedBy_usrID = ?
)").arg(this->DBPrefix),
                {
                    UT_AdminUserEmail,
                    UT_AdminRoleID,
                    UT_SystemUserID
                }
            );

            gAdminUserID = res2.lastInsertId().toULongLong();

            DAC.execQuery("", QString("UPDATE %1AAA.tblUser SET tblUser.usr_rolID=3 WHERE tblUser.usrID=?").arg(this->DBPrefix), { gAdminUserID });
        }
    }

    virtual void cleanupUnitTestData() {
        deleteOldTestData();
    }

//    QVariant callRefreshAPI() {
//        return RESTClientHelper::callAPI(
//            gLoginJWT, RESTClientHelper::GET, "Account/refreshJWT", {}, {}, {}, APIURL
//        );
//    }

    QVariant callAPI(
            bool _useAdminJWT,
            RESTClientHelper::enuHTTPMethod _method,
            const QString& _api,
            const QVariantMap& _urlArgs = {},
            const QVariantMap& _postOrFormFields = {},
            const QVariantMap& _formFiles = {},
            QVariantMap *_outResponseHeaders = nullptr
        ) {
        QVariantMap ResponseHeaders;

        QVariant Result = RESTClientHelper::callAPI(
                              (_useAdminJWT ? gEncodedAdminJWT : gEncodedJWT),
                              _method,
                              _api,
                              _urlArgs,
                              _postOrFormFields,
                              _formFiles,
                              APIURL,
                              &ResponseHeaders
                              );

        QJsonDocument Doc = QJsonDocument::fromVariant(QVariantMap({{"result", Result}}));

        qDebug().noquote() << endl
                 << "  Response Headers:" << ResponseHeaders << endl
                 << "  Result:" << Doc.toJson();

        if (ResponseHeaders.contains("x-auth-new-token")) {
//            QString NewJWT = ResponseHeaders.value("x-auth-new-token").toString();

//            qDebug() << ">>>>>>>>>>>>>>>> JWT CHANGED TO" << endl << NewJWT;

//            if (_useAdminJWT)
//                gEncodedAdminJWT = NewJWT;
//            else
//                gEncodedJWT = NewJWT;
        }

        if (_outResponseHeaders != nullptr)
            *_outResponseHeaders = ResponseHeaders;

        return Result;
    }

    QVariant callUserAPI(
            RESTClientHelper::enuHTTPMethod _method,
            const QString& _api,
            const QVariantMap& _urlArgs = {},
            const QVariantMap& _postOrFormFields = {},
            const QVariantMap& _formFiles = {},
            QVariantMap *_outResponseHeaders = nullptr
        ) {
        return callAPI(
                    false,
                    _method,
                    _api,
                    _urlArgs,
                    _postOrFormFields,
                    _formFiles,
                    _outResponseHeaders
                    );
    }

    QVariant callAdminAPI(
            RESTClientHelper::enuHTTPMethod _method,
            const QString& _api,
            const QVariantMap& _urlArgs = {},
            const QVariantMap& _postOrFormFields = {},
            const QVariantMap& _formFiles = {},
            QVariantMap *_outResponseHeaders = nullptr
        ) {
        return callAPI(
                    true,
                    _method,
                    _api,
                    _urlArgs,
                    _postOrFormFields,
                    _formFiles,
                    _outResponseHeaders
                    );
    }

private:
    void deleteOldTestData() { //bool _createUsers=false)
        clsDAC DAC("AAA");

        DAC.execQuery("", QString("DELETE FROM %1AAA.tblAPITokens WHERE aptToken IN(?,?)").arg(this->DBPrefix), { UT_NormalToken, UT_AdminToken });
        DAC.execQuery("", QString("DELETE FROM %1AAA.tblService WHERE svcName=?").arg(this->DBPrefix), { UT_ServiceName });

        DAC.execQuery("", QString("UPDATE %1AAA.tblUser SET usrUpdatedBy_usrID=NULL WHERE usrEmail IN(?,?)").arg(this->DBPrefix), { UT_UserEmail, UT_AdminUserEmail });
//        DAC.execQuery("", QString("DELETE FROM %1AAA.tblUser WHERE usrEmail IN (?,?)").arg(this->DBPrefix), {UT_UserEmail, UT_AdminUserEmail});
//        DAC.execQuery("", QString("DELETE FROM %1AAA.tblRoles WHERE rolName IN(?,?)").arg(this->DBPrefix), {UT_ServiceRoleName, UT_RoleName});

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
