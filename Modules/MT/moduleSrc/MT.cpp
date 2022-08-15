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
 * @author S. Mehran M. Ziabary <ziabary@targoman.com>
 * @author Kambiz Zandi <kambizzandi@gmail.com>
 */

#include "MT.h"
#include "Defs.hpp"
#include "ModuleHelpers/MT/Classes/TranslationDispatcher.h"

using namespace Targoman::API::ModuleHelpers::MT::Classes;

namespace Targoman::API::MTModule {

TARGOMAN_IMPL_API_MODULE(MT)
TARGOMAN_API_MODULE_DB_CONFIG_IMPL(MT, MTSchema)

MT::MT() :
    intfSQLBasedWithActionLogsModule(
        MTDomain,
        MTSchema
) {
    TARGOMAN_API_IMPLEMENT_MIGRATIONS(MT, MTSchema);
    TARGOMAN_API_IMPLEMENT_ACTIONLOG(MT, MTSchema);
    TARGOMAN_API_IMPLEMENT_FAQ(MT, MTSchema);

    // Register translation engines
    TranslationDispatcher::instance().registerEngines();
}

/*
QVariantMap IMPL_REST_GET_OR_POST(MT, Translate, (
    const TAPI::RemoteIP_t& _REMOTE_IP,
    const QString& _token,
    QString _text,
    QString _dir,
    const QString& _engine,
    bool _detailed,
    bool _detok,
    bool _dic,
    bool _dicFull
)) {
    QTime Timer, OverallTime;

    int PreprocessTime = 0;
    Timer.start();OverallTime.start();

    _text = _text.trimmed();
    if (_text.isEmpty())
        throw exHTTPBadRequest("Input text must not be empty");
    _dir = _dir.replace('_', '2');

    TranslationDir_t Dir = TranslationDispatcher::dirLangs(_dir);
    if (Dir.first.isNull())
        throw exHTTPBadRequest("Invalid translation direction format");

    if (!TranslationDispatcher::instance().isValidEngine(_engine, Dir) == false)
        throw exHTTPBadRequest("Invalid engine/direction combination");

    QJsonObject TokenInfo = Authorization::retrieveTokenInfo(_token,
                                                             _REMOTE_IP, {
                                                                 TARGOMAN_PRIV_PREFIX + _engine,
                                                                 TARGOMAN_PRIV_PREFIX + _dir,
                                                                 _dic ? (TARGOMAN_PRIV_PREFIX + "Dic") : QString(),
                                                                 _dicFull ? (TARGOMAN_PRIV_PREFIX + "DicFull") : QString()
                                                             });


    QJsonObject Stats = this->execQuery(
            "SELECT * FROM tblTokenStats "
            "WHERE tks_tokID = ? "
            "  AND tksEngine=? "
            "  AND tksDir=? ",
    {
        {TokenInfo[TOKENItems::tokID]},
        {_engine},
        {_dir},
    }
    ).toJson(true).object ();

    if (Stats.isEmpty())
        this->execQuery("INSERT IGNORE INTO tblTokenStats (tks_tokID,tksEngine,tksDir) VALUES(?, ?, ?)", {
        {TokenInfo[TOKENItems::tokID]},
        {_engine},
        {_dir},
    });

    _text = TranslationDispatcher::instance().tokenize(_text, Dir.first);
    quint64 SourceWordCount = static_cast<quint64>(_text.split(' ').size());

    QJsonObject Privs = Authorization::privObjectFromInfo(TokenInfo);

    //@TODO: fix
//    Accounting::checkCredit(Privs, TARGOMAN_QUOTA_PREFIX+_engine+"MaxPerDay", Stats["tksTodayWords"].toDouble()+ SourceWordCount);
//    Accounting::checkCredit(Privs, TARGOMAN_QUOTA_PREFIX+_engine+"MaxPerMonth", Stats["tksThisMonthWords"].toDouble()+ SourceWordCount);
//    Accounting::checkCredit(Privs, TARGOMAN_QUOTA_PREFIX+_engine+"MaxTotal", Stats["tksTotalWords"].toDouble()+ SourceWordCount);
//    Accounting::checkCredit(Privs, TARGOMAN_QUOTA_PREFIX+_dir+"MaxPerDay", Stats["tksTodayWords"].toDouble()+ SourceWordCount);
//    Accounting::checkCredit(Privs, TARGOMAN_QUOTA_PREFIX+_dir+"MaxPerMonth", Stats["tksThisMonthWords"].toDouble()+ SourceWordCount);
//    Accounting::checkCredit(Privs, TARGOMAN_QUOTA_PREFIX+_dir+"MaxTotal", Stats["tksTotalWords"].toDouble()+ SourceWordCount);
//    Accounting::checkCredit(Privs, TARGOMAN_QUOTA_PREFIX+"MaxPerDay", Stats["tksTodayWords"].toDouble()+ SourceWordCount);
//    Accounting::checkCredit(Privs, TARGOMAN_QUOTA_PREFIX+"MaxPerMonth", Stats["tksThisMonthWords"].toDouble()+ SourceWordCount);
//    Accounting::checkCredit(Privs, TARGOMAN_QUOTA_PREFIX+"MaxTotal", Stats["tksTotalWords"].toDouble()+ SourceWordCount);

    if (_dic) {
        if (Authorization::hasPriv(Privs, {TARGOMAN_PRIV_PREFIX + "Dic"})) {
            if (_dicFull && Authorization::hasPriv(Privs, {TARGOMAN_PRIV_PREFIX + "DicFull"}))
                throw exAuthorization("Not enought privileges to retrieve dictionary full response.");

            PreprocessTime = Timer.elapsed();Timer.restart();
            QVariantMap DicResponse =  TranslationDispatcher::instance().retrieveDicResponse(_text, Dir.first);
            if (DicResponse.size()) {
                if (_detailed) {
                    DicResponse[RESULTItems::TIMES]= QVariantMap({
                                                                     {RESULTItems::TIMESItems::PRE, PreprocessTime},
                                                                     {RESULTItems::TIMESItems::TR, Timer.elapsed()},
                                                                     {RESULTItems::TIMESItems::POST, 0},
                                                                     {RESULTItems::TIMESItems::ALL, PreprocessTime+Timer.elapsed()}
                                                                 });
                }
                TranslationDispatcher::instance().addDicLog(Dir.first, SourceWordCount, _text);
                return DicResponse;
            }
        } else
            throw exAuthorization("Not enought privileges to retrieve dictionary response.");
    }

    PreprocessTime += Timer.elapsed();

    try {
        int InternalPreprocessTime = 0, InternalTranslationTime = 0, InternalPostprocessTime = 0;
        QVariantMap Translation = TranslationDispatcher::instance().doTranslation(Privs,
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
        if (_detailed) {
            Translation[RESULTItems::TIMES]= QVariantMap({
                                                             {RESULTItems::TIMESItems::PRE, InternalPreprocessTime + PreprocessTime},
                                                             {RESULTItems::TIMESItems::TR, InternalTranslationTime},
                                                             {RESULTItems::TIMESItems::POST, InternalPostprocessTime + Timer.elapsed()},
                                                             {RESULTItems::TIMESItems::ALL, OverallTime.elapsed()}
                                                         });
        } else
            Translation[RESULTItems::TIME]= OverallTime.elapsed();

        TranslationDispatcher::instance().addTranslationLog(static_cast<quint64>(TokenInfo[TOKENItems::tokID].toInt()), _engine, _dir, SourceWordCount, _text, OverallTime.elapsed());

        if (Authorization::hasPriv(Privs, {TARGOMAN_PRIV_PREFIX + "ReportServer"}) == false)
            Translation.remove(RESULTItems::SERVERID);

        return Translation;
    } catch (Common::exTargomanBase& ex) {
        TranslationDispatcher::instance().addErrorLog(static_cast<quint64>(TokenInfo[TOKENItems::tokID].toInt()), _engine, _dir, SourceWordCount, _text, ex.code());
        throw;
    }
}

QVariantMap IMPL_REST_GET_OR_POST(MT, Test, (
    const TAPI::RemoteIP_t& _REMOTE_IP,
    const QString& _token,
    const QString& _arg
)) {
    return {
        {"inputArg", _arg},
        {"info", Authorization::retrieveTokenInfo(_token, _REMOTE_IP)}
    };
}
*/

} //namespace Targoman::API::MTModule
