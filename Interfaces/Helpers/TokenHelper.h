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

#ifndef TARGOMAN_API_TOKENHELPER_H
#define TARGOMAN_API_TOKENHELPER_H

#include <QString>
#include <QDateTime>
#include <QMutex>
#include "libTargomanCommon/Macros.h"

namespace Targoman::API::Helpers {

enum class enuTokenBanType {
    None,
    Block,
    Pause,
};

struct stuTokenBanInfo {
    quint64 ID;
    QString MD5;
    QDateTime DueDate;
    enuTokenBanType BanType;
};

class TokenHelper
{
public:
    static bool UpdateTokenBinList();
    static enuTokenBanType GetTokenBanType(const QString &_token);
//    static void BanToken(
//        quint64 _id,
//        const QString &_token,
//        QDateTime _dueDate,
//        enuTokenBanType _banType
//    );

private:
    static quint64 LastFetchedTokenBinID;
    static qint64 LastFetchedSecsSinceEpoch;
    static QMutex Lock;
    //key: TokenMD5
    static QMap<QString, stuTokenBanInfo> TokenBanList;
};

} //namespace Targoman::API::Helpers

#endif // TARGOMAN_API_TOKENHELPER_H
