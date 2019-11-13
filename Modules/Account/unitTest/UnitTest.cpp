/******************************************************************************
 * Targoman: A robust Machine Translation framework               *
 *                                                                            *
 * Copyright 2014-2018 by ITRC <http://itrc.ac.ir>                            *
 *                                                                            *
 * This file is part of Targoman.                                             *
 *                                                                            *
 * Targoman is free software: you can redistribute it and/or modify           *
 * it under the terms of the GNU Lesser General Public License as published   *
 * by the Free Software Foundation, either version 3 of the License, or       *
 * (at your option) any later version.                                        *
 *                                                                            *
 * Targoman is distributed in the hope that it will be useful,                *
 * but WITHOUT ANY WARRANTY; without even the implied warranty of             *
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the              *
 * GNU Lesser General Public License for more details.                        *
 * You should have received a copy of the GNU Lesser General Public License   *
 * along with Targoman. If not, see <http://www.gnu.org/licenses/>.           *
 *                                                                            *
 ******************************************************************************/
/**
 * @author S. Mohammad M. Ziabary <ziabary@targoman.com>
 * @author Saeed Torabzadeh <saeed.torabzadeh@targoman.com>
 */

#include "testBase.hpp"

#include "libTargomanCommon/Logger.h"
#include "libTargomanCommon/CmdIO.h"
#include "libTargomanDBM/clsDAC.h"
#include "libTargomanCommon/Macros.h"
#include "libTargomanCommon/Configuration/ConfigManager.h"
#include "libTargomanCommon/Logger.h"
#include "Configs.h"

#include "testBase.hpp"

using namespace Targoman;
using namespace Targoman::Common;
using namespace Targoman::Apps;
using namespace Targoman::DBManager;

int main(int argc, char *argv[])
{
    QCoreApplication App(argc, argv);
    App.setAttribute(Qt::AA_Use96Dpi, true);

    Targoman::Common::TARGOMAN_IO_SETTINGS.Debug.setLevel(1);
    Targoman::Common::TARGOMAN_IO_SETTINGS.Debug.setDetails(true);
    Targoman::Common::Logger::instance().setActive(true);
    Targoman::Common::Logger::instance().setVisible();

    clsDAC::addDBEngine(enuDBEngines::MySQL);
    clsDAC::setConnectionString("HOST=192.168.0.240;"
                                "PORT=3306;"
                                "USER=root;"
                                "PASSWORD=targoman123;"
                                "SCHEMA=AAA");

    clsDAC DAC;
    DAC.execQuery("", "DELETE FROM tblUser WHERE usrEmail='unit_test@unittest.test'");
    DAC.execQuery("", "DELETE FROM tblRoles WHERE rolName=?", {"unit_test"});

    int FailedTests = 0;
    FailedTests += QTest::qExec(new testBase, argc, argv);

    if(FailedTests > 0){
        qDebug() << "total number of failed tests: " << FailedTests;
    }else{
        qDebug() << "all tests passed :)";
    }
    return FailedTests;
}

