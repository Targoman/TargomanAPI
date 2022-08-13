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
 * @author S. Mehran M. Ziabary <ziabary@targoman.com>
 * @author Kambiz Zandi <kambizzandi@gmail.com>
 */

#include <QtTest>
#include "testBase.hpp"
#include "testMT.hpp"

TAPI_MARSHAL_TEST_VARIABLES

int main(int _argc, char *_argv[]) {
    qDebug() << "--------------------------------------------------";
    qDebug() << "-- test module: MT -------------------------------";
    qDebug() << "--------------------------------------------------";

    //---------------------
    QString DBPrefix;
    int progArgsCount = 0;
    char **progArgs = findDBPrefixFromArguments(_argc, _argv, DBPrefix, progArgsCount);

    //---------------------
    QCoreApplication App(progArgsCount, progArgs);
    App.setAttribute(Qt::AA_Use96Dpi, true);

    clsDAC::addDBEngine(enuDBEngines::MySQL);
    clsDAC::setConnectionString(QString("HOST=%1;PORT=%2;USER=%3;PASSWORD=%4;SCHEMA=%5%6")
                                .arg(TARGOMAN_M2STR(UNITTEST_DB_HOST))
                                .arg(TARGOMAN_M2STR(UNITTEST_DB_PORT))
                                .arg(TARGOMAN_M2STR(UNITTEST_DB_USER))
                                .arg(TARGOMAN_M2STR(UNITTEST_DB_PASSWORD))
                                .arg(DBPrefix)
                                .arg(TARGOMAN_M2STR(UNITTEST_DB_SCHEMA))
                                );

    bool BreakOnFirstFail = true;
    int FailedTests = 0;

    try {
        FailedTests += QTest::qExec(new testBase(DBPrefix), progArgsCount, progArgs);
        if (BreakOnFirstFail && !FailedTests) FailedTests += QTest::qExec(new testMT(DBPrefix), progArgsCount, progArgs);
    } catch (exTargomanBase &e) {
        ++FailedTests;
        qDebug() << "*** EXCEPTION ***" << QString("error(%1):%2").arg(e.code()).arg(e.what());
    } catch (std::exception &e) {
        ++FailedTests;
        qDebug() << "*** EXCEPTION ***" << e.what();
    }

    if (FailedTests > 0) {
        qDebug() << "total number of failed tests: " << FailedTests;
    } else {
        qDebug() << "all tests passed :)";
    }

    clsDAC::shutdown();

    return FailedTests;
}
