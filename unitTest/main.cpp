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
#include "QueryBuilders.hpp"

int main(int argc, char *argv[])
{
    QCoreApplication App(argc, argv);
    App.setAttribute(Qt::AA_Use96Dpi, true);


    bool BreakOnFirstFail = true;
    int FailedTests = 0;
    try{
        if(BreakOnFirstFail && !FailedTests) FailedTests += QTest::qExec(new QueryBuilders, argc, argv);
    }catch(std::exception &e){
        qDebug()<<e.what();
    }
    if(FailedTests > 0){
        qDebug() << "total number of failed tests: " << FailedTests;
    }else{
        qDebug() << "all tests passed :)";
    }

    return FailedTests;
    /**/
}

