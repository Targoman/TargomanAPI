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
 * @author S.Mehran M.Ziabary <ziabary@targoman.com>
 * @author Kambiz Zandi <kambizzandi@gmail.com>
 */

#include <QCoreApplication>
#include <QTimer>

#include "libTargomanCommon/Macros.h"
#include "libTargomanCommon/Configuration/ConfigManager.h"

#include "appTargomanAPI.h"

static const char* LicenseStr =
"%1 Ver: %2 Build %3\n"
"Published under the terms of GNU AFFERO GENERAL PUBLIC LICENSE version 3\n\n"
"%1 [Arguments]\n"
"Arguments: \n";

using namespace Targoman::Common;
using namespace Targoman::API::Server;

int main(int _argc, char *_argv[])
{
/*
    static const QVariant       QNull     = QVariant(QVariant::Line);
    static const QVariant       QInvalid  = QVariant(QVariant::Invalid);
    static const QVariant       QRequired = QVariant(QVariant::RectF);
    static const QVariant       QAuto     = QVariant(QVariant::PointF);

#define compare(_type) \
    qDebug()<<#_type<<" == QNull :" <<(_type == QNull); \
    qDebug()<<#_type<<" == QInvalid :" <<(_type == QInvalid); \
    qDebug()<<#_type<<" == QRequired :" <<(_type == QRequired); \
    qDebug()<<#_type<<" == QAuto :" <<(_type == QAuto); \
    qDebug()<<#_type<<".isValid():"<<_type.isValid(); \
    qDebug()<<"===========================";

    compare(QNull);
    compare(QInvalid);
    compare(QRequired);
    compare(QAuto);

    exit(0);*/

    try
    {
        QCoreApplication App(_argc, _argv);

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wdate-time"
        Configuration::ConfigManager::instance().init(
                    QString(LicenseStr).arg(_argv[0]).arg(TARGOMAN_M2STR(PROJ_VERSION)).arg(__DATE__),
                    App.arguments().mid(1)
                    );
#pragma GCC diagnostic pop

        QTimer::singleShot(10, new appTargomanAPI, SLOT(slotExecute()));

        return App.exec();
    }
    catch(exTargomanBase& ex)
    {
        TargomanError(ex.what());
    }
    catch (std::exception& ex)
    {
        TargomanError(ex.what());
    }
    catch(...)
    {
        TargomanError("FATAL Unrecognized exception");
    }
    return -1;
}
