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
#include <QVariantMap>
#include <QTime>

#include "TranslationDispatcher.h"
#include "libTargomanAAA/AAA.h"

namespace Targoman {
namespace Apps {
namespace Classes {

QVariantMap TranslationDispatcher::doTranslation(const QString &_remoteIP,
                                                 const QJsonObject& _privInfo,
                                                 QString _text,
                                                 const TranslationDir_t &_dir,
                                                 const QString &_engine,
                                                 bool _detailed,
                                                 bool _dic,
                                                 bool _dicFull)
{
    QVariantMap Result;

    if(_detailed && AAA::hasPriv(_privInfo, {TARGOMAN_PRIV_PREFIX + "Detailed"}) == false)
        throw exAuthorization("Not enought priviledges to get detailed translation response.");


    return Result;
}

QString TranslationDispatcher::detectClass(const QString &_text, const QString &_lang)
{
        return _text;
}

QString TranslationDispatcher::tokenize(const QString &_text, const QString &_lang)
{
    return _text;
}

QString TranslationDispatcher::detokenize(const QString &_text, const QString &_lang)
{
    return _text;
}

QString TranslationDispatcher::retrieveEngineByTextClass(QString &_engine, const QString &_text, const QString &_lang)
{
    return _engine;
}

QVariantMap TranslationDispatcher::retrieveDicResponse(const QString &_text, const QString &_lang)
{

}

void TranslationDispatcher::addDicLog(const QString &_lang, quint64 _worcCount, const QString &_text)
{

}

void TranslationDispatcher::addErrorLog(quint64 _aptID, const QString &_engine, const QString &_dir, quint64 _worcCount, const QString &_text, qint8 _errorCode)
{

}

void TranslationDispatcher::addTranslationLog(quint64 _aptID, const QString &_engine, const QString &_dir, quint64 _worcCount, const QString &_text, qint8 _trTime)
{

}

TranslationDispatcher::TranslationDispatcher() :
    DAC(new DBManager::clsDAC)
{ ; }

}
}
}
