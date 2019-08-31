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

#include "Translation.h"
#include "libTargomanAAA/AAA.h"
#include "Classes/TranslationDispatcher.h"

using namespace QHttp;
using namespace Targoman;
using namespace Targoman::DBManager;

static QString PRIV_PREFIX = "Targoman:can";
static QString QUOTA_PREFIX = "Targoman:";

void Translation::init()
{

}

QVariantMap Translation::apiTranslate(const QHttp::RemoteIP_t& _REMOTE_IP,
                                      const QString& _token,
                                      QString _text,
                                      const QString& _dir,
                                      const QString& _engine,
                                      bool _detailed,
                                      bool _dic,
                                      bool _dicFull)
{
    _text = _text.trimmed();
    if(_text.isEmpty())
        throw exHTTPBadRequest("Input text must not be empty");

    //validate Client IP
    //check engine availability + Dir

    QJsonObject TokenInfo = AAA::retrieveTokenInfo(_token,
                                                   _REMOTE_IP, {
                                                       PRIV_PREFIX + _engine,
                                                       PRIV_PREFIX + _dir,
                                                       _dic ? (PRIV_PREFIX + "Dic") : QString(),
                                                       _dicFull ? (PRIV_PREFIX + "DicFull") : QString()
                                                   });
    QJsonObject Stats = this->DAC->execQuery(
            TokenInfo["usrID"].toString(),
            "SELECT * FROM tblTokenStats WHERE tks_tokID = ?",
            {{TokenInfo["tokID"]}}
            ).toJson(true).object ();
    if(Stats.size())
        this->DAC->execQuery(TokenInfo["usrID"].toString(), "INSERT INTO tblTokenStats (tks_tokID) VALUES(?)", {{TokenInfo["tokID"]}});

    quint64 SourceWordCount = 0;

    QJsonObject Privs = TokenInfo["privs"].toObject();
    AAA::checkCredit(Privs, QUOTA_PREFIX+_engine+"MaxPerDay", Stats["tksTodayWords"].toDouble()+ SourceWordCount);
    AAA::checkCredit(Privs, QUOTA_PREFIX+_engine+"MaxPerMonth", Stats["tksThisMonthWords"].toDouble()+ SourceWordCount);
    AAA::checkCredit(Privs, QUOTA_PREFIX+_engine+"MaxTotal", Stats["tksTotalWords"].toDouble()+ SourceWordCount);
    AAA::checkCredit(Privs, QUOTA_PREFIX+_dir+"MaxPerDay", Stats["tksTodayWords"].toDouble()+ SourceWordCount);
    AAA::checkCredit(Privs, QUOTA_PREFIX+_dir+"MaxPerMonth", Stats["tksThisMonthWords"].toDouble()+ SourceWordCount);
    AAA::checkCredit(Privs, QUOTA_PREFIX+_dir+"MaxTotal", Stats["tksTotalWords"].toDouble()+ SourceWordCount);
    AAA::checkCredit(Privs, QUOTA_PREFIX+"MaxPerDay", Stats["tksTodayWords"].toDouble()+ SourceWordCount);
    AAA::checkCredit(Privs, QUOTA_PREFIX+"MaxPerMonth", Stats["tksThisMonthWords"].toDouble()+ SourceWordCount);
    AAA::checkCredit(Privs, QUOTA_PREFIX+"MaxTotal", Stats["tksTotalWords"].toDouble()+ SourceWordCount);

    QVariantMap Translation = Targoman::Apps::Classes::TranslationDispatcher::instance().doTranslation(_REMOTE_IP,
                                                                                       TokenInfo,
                                                                                       _text,
                                                                                       _dir,
                                                                                       _engine,
                                                                                       _detailed,
                                                                                       _dic,
                                                                                       _dicFull
                                                                                       );

    return Translation;
}

QVariantMap Translation::apiTest(const QHttp::RemoteIP_t &_REMOTE_IP, const QString &_token, const QString &_arg)
{
    return {
        {"inputArg", _arg},
        {"info", AAA::retrieveTokenInfo(_token, _REMOTE_IP)}
    };
}

Translation::Translation() :
    DAC(new DBManager::clsDAC)
{
    this->registerMyRESTAPIs();
}

