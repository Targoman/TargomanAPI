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
#include "libTargomanAAA/Defs.hpp"

using namespace QHttp;
using namespace Targoman;
using namespace Targoman::DBManager;
using namespace Targoman::Apps;
using namespace Targoman::Apps::Classes;

void Translation::init()
{

}

QVariantMap Translation::apiTranslate(const QHttp::RemoteIP_t& _REMOTE_IP,
                                      const QString& _token,
                                      QString _text,
                                      QString _dir,
                                      const QString& _engine,
                                      bool _detailed,
                                      bool _detok,
                                      bool _dic,
                                      bool _dicFull)
{
    QTime Timer, OverallTime;
    int PreprocessTime = 0;
    Timer.start();OverallTime.start();

    _text = _text.trimmed();
    if(_text.isEmpty())
        throw exHTTPBadRequest("Input text must not be empty");
    _dir = _dir.replace('_', '2');

    TranslationDir_t Dir = TranslationDispatcher::dirLangs(_dir);
    if(Dir.first.isNull())
        throw exHTTPBadRequest("Invalid translation direction format");

    if(!TranslationDispatcher::instance().isValidEngine(_engine, Dir) == false)
        throw exHTTPBadRequest("Invalid engine/direction combination");

    QJsonObject TokenInfo = AAA::retrieveTokenInfo(_token,
                                                   _REMOTE_IP, {
                                                       TARGOMAN_PRIV_PREFIX + _engine,
                                                       TARGOMAN_PRIV_PREFIX + _dir,
                                                       _dic ? (TARGOMAN_PRIV_PREFIX + "Dic") : QString(),
                                                       _dicFull ? (TARGOMAN_PRIV_PREFIX + "DicFull") : QString()
                                                   });

    QJsonObject Stats = this->DAC->execQuery(
                TokenInfo[AAA_USERID].toString(),
                "SELECT * FROM tblTokenStats "
                "WHERE tks_tokID = ? "
                "  AND tksEngine=? "
                "  AND tksDir=? ",
                {
                    {TokenInfo[AAA_TOKENID]},
                    {_engine},
                    {_dir},
                }
            ).toJson(true).object ();

    if(Stats.isEmpty())
        this->DAC->execQuery(TokenInfo[AAA_USERID].toString(), "INSERT IGNORE INTO tblTokenStats (tks_tokID,tksEngine,tksDir) VALUES(?, ?, ?)", {
            {TokenInfo[AAA_TOKENID]},
            {_engine},
            {_dir},
        });

    _text = TranslationDispatcher::instance().tokenize(_text, Dir.first);
    quint64 SourceWordCount = static_cast<quint64>(_text.split(' ').size());

    QJsonObject Privs = AAA::privObjectFromInfo(TokenInfo);
    AAA::checkCredit(Privs, TARGOMAN_QUOTA_PREFIX+_engine+"MaxPerDay", Stats["tksTodayWords"].toDouble()+ SourceWordCount);
    AAA::checkCredit(Privs, TARGOMAN_QUOTA_PREFIX+_engine+"MaxPerMonth", Stats["tksThisMonthWords"].toDouble()+ SourceWordCount);
    AAA::checkCredit(Privs, TARGOMAN_QUOTA_PREFIX+_engine+"MaxTotal", Stats["tksTotalWords"].toDouble()+ SourceWordCount);
    AAA::checkCredit(Privs, TARGOMAN_QUOTA_PREFIX+_dir+"MaxPerDay", Stats["tksTodayWords"].toDouble()+ SourceWordCount);
    AAA::checkCredit(Privs, TARGOMAN_QUOTA_PREFIX+_dir+"MaxPerMonth", Stats["tksThisMonthWords"].toDouble()+ SourceWordCount);
    AAA::checkCredit(Privs, TARGOMAN_QUOTA_PREFIX+_dir+"MaxTotal", Stats["tksTotalWords"].toDouble()+ SourceWordCount);
    AAA::checkCredit(Privs, TARGOMAN_QUOTA_PREFIX+"MaxPerDay", Stats["tksTodayWords"].toDouble()+ SourceWordCount);
    AAA::checkCredit(Privs, TARGOMAN_QUOTA_PREFIX+"MaxPerMonth", Stats["tksThisMonthWords"].toDouble()+ SourceWordCount);
    AAA::checkCredit(Privs, TARGOMAN_QUOTA_PREFIX+"MaxTotal", Stats["tksTotalWords"].toDouble()+ SourceWordCount);

    if(_dic){
        if(AAA::hasPriv(Privs, {TARGOMAN_PRIV_PREFIX + "Dic"})){
            if(_dicFull && AAA::hasPriv(Privs, {TARGOMAN_PRIV_PREFIX + "DicFull"}))
                throw exAuthorization("Not enought priviledges to retrieve dictionary full response.");

            PreprocessTime = Timer.elapsed();Timer.restart();
            QVariantMap DicResponse =  TranslationDispatcher::instance().retrieveDicResponse(_text, Dir.first);
            if(DicResponse.size()){
                if(_detailed){
                    DicResponse[RESULT_TIMES]= QVariantMap({
                         {RESULT_TIMES_PRE, PreprocessTime},
                         {RESULT_TIMES_TR, Timer.elapsed()},
                         {RESULT_TIMES_POST, 0},
                         {RESULT_TIMES_ALL, PreprocessTime+Timer.elapsed()}
                    });
                }
                TranslationDispatcher::instance().addDicLog(Dir.first, SourceWordCount, _text);
                return DicResponse;
            }
        }else
            throw exAuthorization("Not enought priviledges to retrieve dictionary response.");
    }

    PreprocessTime += Timer.elapsed();

    try{
        int InternalPreprocessTime = 0, InternalTranslationTime = 0, InternalPostprocessTime = 0;
        QVariantMap Translation = TranslationDispatcher::instance().doTranslation(TokenInfo,
                                                                                  _text,
                                                                                  Dir,
                                                                                  _engine,
                                                                                  true,
                                                                                  _detailed,
                                                                                  _detok,
                                                                                  InternalPreprocessTime,
                                                                                  InternalTranslationTime
                                                                                  );
        Timer.restart();
        if(_detailed){
            Translation[RESULT_TIMES]= QVariantMap({
                 {RESULT_TIMES_PRE, InternalPreprocessTime + PreprocessTime},
                 {RESULT_TIMES_TR, InternalTranslationTime},
                 {RESULT_TIMES_POST, InternalPostprocessTime + Timer.elapsed()},
                 {RESULT_TIMES_ALL, OverallTime.elapsed()}
            });
        }

        TranslationDispatcher::instance().addTranslationLog(static_cast<quint64>(TokenInfo[AAA_TOKENID].toInt()), _engine, _dir, SourceWordCount, _text, OverallTime.elapsed());


        return Translation;
    }catch(Common::exTargomanBase &ex){
        TranslationDispatcher::instance().addErrorLog(static_cast<quint64>(TokenInfo[AAA_TOKENID].toInt()), _engine, _dir, SourceWordCount, _text, ex.code());
        throw;
    }
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

