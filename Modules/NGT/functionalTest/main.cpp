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
#include "testNGT.hpp"

TAPI_MARSHAL_TEST_VARIABLES

int main(int argc, char *argv[])
{
    qDebug() << "--------------------------------------------------";
    qDebug() << "-- test module: NGT ------------------------------";
    qDebug() << "--------------------------------------------------";

    QCoreApplication App(argc, argv);
    App.setAttribute(Qt::AA_Use96Dpi, true);

    clsDAC::addDBEngine(enuDBEngines::MySQL);
    clsDAC::setConnectionString("HOST=" TARGOMAN_M2STR(UNITTEST_DB_HOST) ";"
                                "PORT=" TARGOMAN_M2STR(UNITTEST_DB_PORT) ";"
                                "USER=" TARGOMAN_M2STR(UNITTEST_DB_USER) ";"
                                "PASSWORD=" TARGOMAN_M2STR(UNITTEST_DB_PASSWORD) ";"
                                "SCHEMA=" TARGOMAN_M2STR(UNITTEST_DB_SCHEMA) ";");

    bool BreakOnFirstFail = true;
    int FailedTests = 0;
    try {
        FailedTests += QTest::qExec(new testBase, argc, argv);
        if (BreakOnFirstFail && !FailedTests) FailedTests += QTest::qExec(new testNGT, argc, argv);
    } catch(std::exception &e) {
        qDebug()<<e.what();
    }
    if (FailedTests > 0) {
        qDebug() << "total number of failed tests: " << FailedTests;
    } else {
        qDebug() << "all tests passed :)";
    }

    clsDAC::shutdown();

    return FailedTests;
}
