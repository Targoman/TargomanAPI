/******************************************************************************
#   TargomanAPI: REST API for Targoman
#
#   Copyright 2014-2020 by Targoman Intelligent Processing <http://tip.co.ir>
#
#   TargomanAPI is free software: you can redistribute it and/or modify
#   it under the terms of the GNU AFFERO GENERAL PUBLIC LICENSE as published by
#   the Free Software Foundation, either version 3 of the License, or
#   (at your option) any later version.
#
#   TargomanAPI is distributed in the hope that it will be useful,
#   but WITHOUT ANY WARRANTY; without even the implied warranty of
#   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#   GNU AFFERO GENERAL PUBLIC LICENSE for more details.
#
#   You should have received a copy of the GNU AFFERO GENERAL PUBLIC LICENSE
#   along with Targoman. If not, see <http://www.gnu.org/licenses/>.
 ******************************************************************************/
/**
 @author S. Mehran M. Ziabary <ziabary@targoman.com>
 */

#include <QtTest>
#include "testBase.hpp"
#include "Account/test.h"

QString APIURL = "http://127.0.0.1:10000/rest/v1";

QString gEncodedJWT;
QString gEncodedAdminJWT;
QJsonObject gJWT;
QJsonObject gAdminJWT;
quint32 gUserID;
quint32 gAdminUserID;
QVariant gInvalid;

int main(int argc, char *argv[])
{
    QCoreApplication App(argc, argv);
    App.setAttribute(Qt::AA_Use96Dpi, true);

    clsDAC::addDBEngine(enuDBEngines::MySQL);
    clsDAC::setConnectionString("HOST=192.168.0.240;"
                                "PORT=3306;"
                                "USER=root;"
                                "PASSWORD=targoman123;"
                                "SCHEMA=AAA");

    bool BreakOnFirstFail = true;
    int FailedTests = 0;
    try{
        FailedTests += QTest::qExec(new testBase, argc, argv);
        if(BreakOnFirstFail && !FailedTests) FailedTests += QTest::qExec(new testAccount, argc, argv);
        if(BreakOnFirstFail && !FailedTests) FailedTests += QTest::qExec(new testActionLogs, argc, argv);
        if(BreakOnFirstFail && !FailedTests) FailedTests += QTest::qExec(new testActiveSessions, argc, argv);
        if(BreakOnFirstFail && !FailedTests) FailedTests += QTest::qExec(new testRoles, argc, argv);
        if(BreakOnFirstFail && !FailedTests) FailedTests += QTest::qExec(new testService, argc, argv);
        if(BreakOnFirstFail && !FailedTests) FailedTests += QTest::qExec(new testAPITokens, argc, argv);
    }catch(std::exception &e){
        qDebug()<<e.what();
    }
    if(FailedTests > 0){
        qDebug() << "total number of failed tests: " << FailedTests;
    }else{
        qDebug() << "all tests passed :)";
    }

    clsDAC::shutdown();

    return FailedTests;
    /**/
}

