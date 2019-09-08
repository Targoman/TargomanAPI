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
#include "intfTranslatorEngine.hpp"
#include "libTargomanDBM/clsDAC.h"
#include "libTargomanCommon/tmplBoundedCache.hpp"

namespace Targoman {
namespace Apps {
class clsFormalityChecker;
namespace Classes {

static QString TARGOMAN_PRIV_PREFIX = "Targoman:can";
static QString TARGOMAN_QUOTA_PREFIX = "Targoman:";

typedef QPair<QString, QString>  TranslationDir_t;

class TranslationDispatcher
{
public:
    static TranslationDispatcher& instance(){static TranslationDispatcher* Instance = nullptr; return *(Q_LIKELY(Instance) ? Instance : (Instance = new TranslationDispatcher));}
    ~TranslationDispatcher();

    QVariantMap doTranslation(const QJsonObject& _privInfo,
                              const QString &_text,
                              const TranslationDir_t& _dir,
                              const QString& _engine,
                              bool _useSpecialClass,
                              bool _detailed);

    QString tokenize(const QString& _text, const QString& _lang);
    QString detokenize(const QString& _text, const QString& _lang);
    QString detectClass(const QString& _engine, const QString& _text, const QString& _lang);
    QString preprocessText(const QString& _text, const QString& _lang);
    QVariantMap retrieveDicResponse(const QString& _text, const QString& _lang);
    void addDicLog(const QString& _lang, quint64 _worcCount, const QString& _text);
    void addErrorLog(quint64 _aptID, const QString& _engine, const QString& _dir, quint64 _worcCount, const QString& _text, qint8 _errorCode);
    void addTranslationLog(quint64 _aptID, const QString& _engine, const QString& _dir, quint64 _worcCount, const QString& _text, int _trTime);

    inline bool isValidEngine(const QString& _engine, const TranslationDir_t& _dir){
        return this->RegisteredEngines.contains(intfTranslatorEngine::makeEngineName(_engine, _dir.first, _dir.second));
    }

    static inline TranslationDir_t dirLangs(const QString& _dir){
        if(_dir.contains("2"))
            return qMakePair(_dir.split("2").first(), _dir.split("2").last());
        else if(_dir.contains("_"))
            return qMakePair(_dir.split("_").first(), _dir.split("_").last());
        return  qMakePair(QString(),QString());
    }

private:
    TranslationDispatcher();
    Q_DISABLE_COPY(TranslationDispatcher)

    QHash<QString, intfTranslatorEngine*> RegisteredEngines;
    QScopedPointer<Targoman::DBManager::clsDAC> DAC;
    QScopedPointer<clsFormalityChecker> FormalityChecker;
    Targoman::Common::tmplBoundedCache<QHash, QString, QVariantMap> TranslationCache;
    QList<QPair<QRegularExpression, QString>> CorrectionRule;
    QTime LastCorrectionRuleUpdateTime;
};


}
}
}
#endif // TARGOMAN_APPS_CLASSES_TRANSLATIONDISPATCHER_H
