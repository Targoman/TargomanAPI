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

#ifndef TARGOMAN_API_MODULEHELPERS_MT_CLASSES_TRANSLATIONDISPATCHER_H
#define TARGOMAN_API_MODULEHELPERS_MT_CLASSES_TRANSLATIONDISPATCHER_H

#include <QJsonObject>
#include "libTargomanCommon/tmplBoundedCache.hpp"
#include "libTargomanCommon/Configuration/tmplConfigurableMultiMap.hpp"
#include "../MTDefs.hpp"
#include "clsEngine.h"
#include "../Gateways/intfTranslatorGateway.hpp"
#include "Interfaces/Server/APICallBoom.h"
using namespace Targoman::Common::Configuration;
using namespace Targoman::API::Server;

/***********************************************************************************/
///@TODO: move this to common
#define TARGOMAN_BEGIN_STATIC_CTOR(_className) \
    struct _##_className##_static_constructor { \
        _##_className##_static_constructor() {

#define TARGOMAN_END_STATIC_CTOR(_className) \
        } \
    }; \
    static inline _##_className##_static_constructor _##_className##_static_constructor_internal;

///@TODO: move this to common
#define instanceGetterPtr(_class) static _class* instancePtr() { \
    static _class* Instance = nullptr; return (Q_LIKELY(Instance) ? Instance : (Instance = new _class)); \
}

///@TODO: move this to common
#define TAPI_DISABLE_COPY(Class) \
    Q_DISABLE_COPY(Class)

#define TARGOMAN_API_MT_GATEWAY_DEFINE(_gatewayClassName) \
public: \
    instanceGetterPtr(_gatewayClassName); \
private: \
    _gatewayClassName(); \
    TAPI_DISABLE_COPY(_gatewayClassName); \
    TARGOMAN_BEGIN_STATIC_CTOR(_gatewayClassName) \
        Targoman::API::ModuleHelpers::MT::Classes::TranslationDispatcher::registerGateway(_gatewayClassName::Name, _gatewayClassName::instancePtr()); \
    TARGOMAN_END_STATIC_CTOR(_gatewayClassName)

#define TARGOMAN_API_MT_GATEWAY_IMPL(_gatewayClassName) \
    _gatewayClassName::_gatewayClassName() { ; }

/***********************************************************************************/
namespace Targoman::API::ModuleHelpers::MT::Classes {

using namespace Gateways;

static QString TARGOMAN_PRIV_PREFIX = "Targoman:can";
static QString TARGOMAN_QUOTA_PREFIX = "Targoman:";

TARGOMAN_DEFINE_ENUM(enuTranslationError,
                     Ok = 0,
                     OperationTimedOut = -10,
                     ConnectionRefused = -20,
                     JsonParseError = -30,
                     InvalidServerResponse = -40,
                     CURLError = -100
                     )

typedef QPair<QString, QString> TranslationDir_t;

struct stuCfgTranslationEngine {
    tmplConfigurable<QString>       SourceLang;
    tmplConfigurable<QString>       DestLang;
    tmplConfigurable<QString>       Class; //formal, informal, all
    tmplConfigurable<QStringList>   URLs;
    tmplConfigurable<bool>          SupportsIXML;
    tmplConfigurable<bool>          Active;
    tmplConfigurable<QString>       Driver;

//        struct stuStatistics {
//            tmplConfigurable<quint64> OkResponses;
//            tmplConfigurable<quint64> FailedResponses;

//            stuStatistics(const QString& _basePath);
//        } Statistics;

    stuCfgTranslationEngine(const QString& _basePath) :
        SourceLang  (_basePath + "SourceLang", "", ""),
        DestLang    (_basePath + "DestLang", "", ""),
        Class       (_basePath + "Class", "", ""),
        URLs        (_basePath + "URLs", ""),
        SupportsIXML(_basePath + "SupportsIXML", "", true),
        Active      (_basePath + "Active", "", true),
        Driver      (_basePath + "Driver", "", "")
    { ; }
};

struct stuCfgTranslationEngineAlias {
    tmplConfigurable<QString>       Name;

    stuCfgTranslationEngineAlias(const QString& _basePath) :
        Name  (_basePath + "Name", "", "")
    { ; }
};

struct TranslationConfigs
{
    static inline QString makeConfig(const QString &_name) { return "/Translation/" + _name; }

    static tmplConfigurableMultiMap<stuCfgTranslationEngine> Engines;
    static tmplConfigurableMultiMap<stuCfgTranslationEngineAlias> Aliases;
};

class TranslationDispatcher
{
public:
    static TranslationDispatcher& instance() {
        static TranslationDispatcher* Instance = nullptr;
        return *(Q_LIKELY(Instance) ? Instance : (Instance = new TranslationDispatcher));
    }
    ~TranslationDispatcher();

    void registerEngines();

    inline bool isValidEngine(const QString& _engine, const TranslationDir_t& _dir) {
        return this->RegisteredEngines.contains(stuEngineSpecs::makeFullName(_engine, _dir.first, _dir.second));
    }

    QVariantMap doTranslation(
        INTFAPICALLBOOM_DECL &APICALLBOOM_PARAM,
//        const QJsonObject& _privInfo,
        QString _text,
        const TranslationDir_t& _dir,
        const QString& _engine,
        bool _useSpecialClass,
        bool _detailed, bool _detokenize,
        int& _preprocessTime,
        int& _translationTime
    );

    QString tokenize(const QString& _text, const QString& _lang);
    QString detokenize(const QString& _text, const QString& _lang);
    QString detectClass(const QString& _engine, const QString& _text, const QString& _lang);
    QString preprocessText(const QString& _text, const QString& _lang);
    QVariantMap retrieveDicResponse(const QString& _text, const QString& _lang);
    void addDicLog(const QString& _lang, quint64 _worcCount, const QString& _text);
    void addErrorLog(quint64 _aptID, const QString& _engine, const QString& _dir, quint64 _worcCount, const QString& _text, qint32 _errorCode);
    void addTranslationLog(quint64 _aptID, const QString& _engine, const QString& _dir, quint64 _worcCount, const QString& _text, int _trTime);

    static inline TranslationDir_t dirLangs(const QString& _dir) {
        if (_dir.contains("2"))
            return qMakePair(_dir.split("2").first(), _dir.split("2").last());

        if (_dir.contains("_"))
            return qMakePair(_dir.split("_").first(), _dir.split("_").last());

        return qMakePair(QString(), QString());
    }

private:
    QVariantMap finalizeTranslation(QVariantMap& _translationResult, bool _detailed, const QString& _destLang);

private:
    TranslationDispatcher();
    Q_DISABLE_COPY(TranslationDispatcher)

    QHash<QString, /*intfTranslatorGateway*/clsEngine*> RegisteredEngines;
    Targoman::Common::tmplBoundedCache<QHash, QString, QVariantMap> TranslationCache;
    QList<QPair<QRegularExpression, QString>> CorrectionRule;
    QTime LastCorrectionRuleUpdateTime;

public:
    static void registerGateway(const QString& _gatewayName, intfTranslatorGateway* _gateway);
    static intfTranslatorGateway* getGateway(const QString& _gatewayName);
private:
    static inline QMap<QString, intfTranslatorGateway*> RegisteredGateways;
};

} //namespace Targoman::API::ModuleHelpers::MT::Classes

#endif // TARGOMAN_API_MODULEHELPERS_MT_CLASSES_TRANSLATIONDISPATCHER_H
