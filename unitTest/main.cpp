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
#include "testAccount.hpp"
#include "testAccountORM.hpp"

QString APIURL = "http://127.0.0.1:10000/rest/v1";

QString gEncodedJWT;
QJsonObject gJWT;
quint32 gUserID;

int main(int argc, char *argv[])
{
    QCoreApplication App(argc, argv);
    App.setAttribute(Qt::AA_Use96Dpi, true);

    clsDAC::addDBEngine(enuDBEngines::MySQL);
    clsDAC::setConnectionString("HOST=192.168.0.240;"
                                "PORT=3306;"
                                "USER=root;"
                                "PASSWORD=targoman1234;"
                                "SCHEMA=AAA");

    int FailedTests = 0;
    try{
        FailedTests += QTest::qExec(new testBase, argc, argv);
        FailedTests += QTest::qExec(new testAccount, argc, argv);
        FailedTests += QTest::qExec(new testAccountORM, argc, argv);
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

