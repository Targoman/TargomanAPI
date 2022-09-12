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

#include "TokenHelper.h"
#include "RESTClientHelper.h"

namespace Targoman::API::Helpers {

quint64 TokenHelper::LastFetchedTokenBinID = 0;
qint64 TokenHelper::LastFetchedSecsSinceEpoch = 0;
QMutex TokenHelper::Lock;
//key: TokenMD5
QMap<QString, stuTokenBanInfo> TokenHelper::TokenBanList;

bool TokenHelper::UpdateTokenBinList()
{
    //every 1 minutes
    if ((QDateTime::currentSecsSinceEpoch() - TokenHelper::LastFetchedSecsSinceEpoch < 60)
            || TokenHelper::TokenBanList.isEmpty())
        return true;

    QMutexLocker Locker(&TokenHelper::Lock);

    //remove expired from cache
    if (TokenHelper::TokenBanList.isEmpty() == false) {
        for (QMap<QString, stuTokenBanInfo>::iterator it = TokenHelper::TokenBanList.begin();
             it != TokenHelper::TokenBanList.end();
             it++
        ) {
            if (it->DueDate.isValid() && (it->DueDate <= QDateTime::currentDateTime()))
                it = TokenHelper::TokenBanList.erase(it);
        }
    }

    //delete expired from database and fetch newly items
    QVariant Result = RESTClientHelper::callAPI(
        RESTClientHelper::POST,
        "/Account/removeExpiredAndFetchNew",
        {},
        {
            { "lastFetchedID", TokenHelper::LastFetchedTokenBinID },
        }
    );

    TokenHelper::LastFetchedSecsSinceEpoch = QDateTime::currentSecsSinceEpoch();

    if (Result.isValid() == false)
        return false;

    TAPI::stuTable Table = Result.value<TAPI::stuTable>();

    foreach (QVariant Row, Table.Rows) {
        QVariantMap Map = Row.toMap();

        stuTokenBanInfo TokenBanInfo;
        TokenBanInfo.ID = Map["tkbID"].toULongLong();
        TokenBanInfo.MD5 = Map["tkbTokenMD5"].toString();
        TokenBanInfo.DueDate = Map["tkbDueDateTime"].toDateTime();

        QString Type = Map["tkbType"].toString();
        if (Type == "Block")
            TokenBanInfo.BanType = enuTokenBanType::Block;
        else if (Type == "Pause")
            TokenBanInfo.BanType = enuTokenBanType::Pause;

        TokenHelper::TokenBanList.insert(TokenBanInfo.MD5, TokenBanInfo);
        if (TokenHelper::LastFetchedTokenBinID < TokenBanInfo.ID)
            TokenHelper::LastFetchedTokenBinID = TokenBanInfo.ID;
    }

    return true;
}

enuTokenBanType TokenHelper::GetTokenBanType(const QString &_token)
{
    try {
        TokenHelper::UpdateTokenBinList();

        QString TokenMD5 = QCryptographicHash::hash(_token.toLatin1(), QCryptographicHash::Md5).toHex().constData();

        if (TokenHelper::TokenBanList.contains(TokenMD5) == false)
            return enuTokenBanType::None;

        return TokenHelper::TokenBanList[TokenMD5].BanType;

    }  catch (std::exception &_exp) {
        TargomanDebug(5) << _exp.what();

        return enuTokenBanType::None;
    }
}

//void TokenHelper::BanToken(
//    quint64 _id,
//    const QString &_token,
//    QDateTime _dueDate,
//    enuTokenBanType _banType
//) {
//    QString TokenMD5 = QCryptographicHash::hash(_token.toLatin1(), QCryptographicHash::Md5).toHex().constData();

//    stuTokenBanInfo TokenBanInfo;
//    TokenBanInfo.ID = _id;
//    TokenBanInfo.MD5 = TokenMD5;
//    TokenBanInfo.DueDate = _dueDate;
//    TokenBanInfo.BanType = _banType;

//    TokenHelper::TokenBanList[TokenMD5] = TokenBanInfo;
//}

} //namespace Targoman::API::Helpers
