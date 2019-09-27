/******************************************************************************
#   TargomanAPI: REST API for Targoman
#
#   Copyright 2014-2019 by Targoman Intelligent Processing <http://tip.co.ir>
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
#ifndef TARGOMAN_APPS_APPTARGOMANAPI_H
#define TARGOMAN_APPS_APPTARGOMANAPI_H

#include <QObject>

namespace Targoman{
namespace Apps {

class appTargomanAPI : public QObject
{
    Q_OBJECT
public:
    explicit appTargomanAPI(QObject *parent = nullptr);

public slots:
    void slotExecute();

};

}
}
#endif // TARGOMAN_APPS_APPTARGOMANAPI_H
