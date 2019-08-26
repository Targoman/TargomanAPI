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
 @author S. Mohammad M. Ziabary <ziabary@targoman.com>
 */
#ifndef TARGOMAN_APPS_CLASSES_TRANSLATIONDISPATCHER_H
#define TARGOMAN_APPS_CLASSES_TRANSLATIONDISPATCHER_H

#include <QJsonObject>

namespace Targoman {
namespace Apps {
namespace Classes {

class TranslationDispatcher
{
public:
    TranslationDispatcher();

    QJsonObject doTranslation(const QString& _remoteIP,
                  const QJsonObject& _info,
                  QString _text,
                  const QString& _dir,
                  const QString& _clientIP = "127.0.0.10",
                  const QString& _engine = "NMT",
                  bool _detailed = false,
                  bool _dic=false,
                  bool _dicFull = false);
};

}
}
}
#endif // TARGOMAN_APPS_CLASSES_TRANSLATIONDISPATCHER_H
