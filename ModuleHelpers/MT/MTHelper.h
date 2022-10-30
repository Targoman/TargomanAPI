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

#ifndef TARGOMAN_API_MODULEHELPERS_MT_CLASSES_MTHELPER_H
#define TARGOMAN_API_MODULEHELPERS_MT_CLASSES_MTHELPER_H

#include <QJsonObject>
#include "libTargomanCommon/tmplBoundedCache.hpp"
#include "libTargomanCommon/Configuration/tmplConfigurableMultiMap.hpp"
#include "MTDefs.hpp"
#include "Classes/clsEngine.h"
#include "Gateways/intfTranslatorGateway.hpp"
//#include "Interfaces/AAA/intfAccountingBasedModule.h"
#include "Interfaces/API/intfModuleHelper.h"
#include "Interfaces/Server/APICallBoom.h"
#include "Interfaces/intfMTModule.h"
using namespace Targoman::Common::Configuration;
using namespace Targoman::API::AAA;
using namespace Targoman::API::API;
using namespace Targoman::API::Server;

/***********************************************************************************/
///@TODO: move this to common
#define TARGOMAN_BEGIN_STATIC_CTOR(_name) \
    class _##_name##_static_constructor { \
    public: \
        _##_name##_static_constructor() {

#define TARGOMAN_END_STATIC_CTOR(_name) \
        } \
    }; \
    static _##_name##_static_constructor _##_name##_static_constructor_internal;

///@TODO: move this to common
#define instanceGetterPtr(_class) static _class* instancePtr() { \
    static _class* Instance = nullptr; return (Q_LIKELY(Instance) ? Instance : (Instance = new _class)); \
}

///@TODO: move this to common
#define TAPI_DISABLE_COPY(Class) \
    Q_DISABLE_COPY(Class)

#define TARGOMAN_API_MT_GATEWAY_DEFINE(_name) \
public: \
    instanceGetterPtr(_name); \
protected: \
    _name(); \
    TAPI_DISABLE_COPY(_name); \
    TARGOMAN_BEGIN_STATIC_CTOR(_name) \
        Targoman::API::ModuleHelpers::MT::MTHelper::registerGateway(_name::Name, _name::instancePtr()); \
    TARGOMAN_END_STATIC_CTOR(_name)

#define TARGOMAN_API_MT_GATEWAY_IMPL(_name) \
    _name::_##_name##_static_constructor _name::_##_name##_static_constructor_internal; \
    _name::_name() { ; }

/***********************************************************************************/
namespace Targoman::API::ModuleHelpers::MT {

using namespace Classes;
using namespace Gateways;
using namespace Interfaces;

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
    tmplConfigurable<QString> Name;

    stuCfgTranslationEngineAlias(const QString& _basePath) :
        Name(_basePath + "Name", "", "")
    { ; }
};

struct TranslationConfigs
{
    static inline QString makeConfig(const QString &_name) { return "/Translation/" + _name; }

    static tmplConfigurableMultiMap<stuCfgTranslationEngine> Engines;
    static tmplConfigurableMultiMap<stuCfgTranslationEngineAlias> Aliases;
};

class MTHelper : public intfModuleHelper
{
    TARGOMAN_API_MODULEHELPER_DEFINE_DB_CONFIGS(MTHelper)

public:
    static MTHelper& instance() {
        static MTHelper* Instance = nullptr;
        return *(Q_LIKELY(Instance) ? Instance : (Instance = new MTHelper));
    }
    ~MTHelper();

    void initialize();

private:
    void registerEngines();

public:
    inline bool isValidEngine(const QString& _engine, const TranslationDir_t& _dir) {
        return this->RegisteredEngines.contains(stuEngineSpecs::makeFullName(_engine, _dir.first, _dir.second));
    }

    template <TAPI::enuTokenActorType::Type _itmplTokenActorType>
    QVariantMap doTranslation(
        INTFAPICALLBOOM_DECL &APICALLBOOM_PARAM,
        intfMTModule<_itmplTokenActorType == TAPI::enuTokenActorType::API> *_mtModule,
        QString _text,
        const TranslationDir_t &_dir,
        const QString &_engine,
        bool _useSpecialClass,
        bool _detailed,
        bool _detokenize,
        bool _dic, // = false,
        bool _dicFull, // = false
        int &_preprocessTime,
        int &_translationTime
    );

protected:
//    clsEngine* findEngine(
//        INTFAPICALLBOOM_DECL &APICALLBOOM_PARAM,
//        QString _text,
//        const TranslationDir_t& _dir,
//        const QString& _engine,
//        bool _useSpecialClass,
//        /*OUT*/ QString &_class
//    );

    template <TAPI::enuTokenActorType::Type _itmplTokenActorType>
    QVariantMap internalDoTranslation(
        INTFAPICALLBOOM_DECL &APICALLBOOM_PARAM,
        intfMTModule<_itmplTokenActorType == TAPI::enuTokenActorType::API> *_mtModule,
        QString _text,
        const TranslationDir_t& _dir,
//        const QString& _engine,
//        bool _useSpecialClass,
        const QString &_class,
        clsEngine* _translationEngine,
        bool _detailed,
        bool _detokenize,
        int& _preprocessTime,
        int& _translationTime
    );

public:
//    QString cleanText(QString &_text);
    QString tokenize(const QString& _text, const QString& _lang);
    QString detokenize(const QString& _text, const QString& _lang);
    QString detectClass(const QString& _engine, const QString& _text, const QString& _lang);

    template <TAPI::enuTokenActorType::Type _itmplTokenActorType>
    QString preprocessText(
        INTFAPICALLBOOM_DECL &APICALLBOOM_PARAM,
        intfMTModule<_itmplTokenActorType == TAPI::enuTokenActorType::API> *_mtModule,
        const QString& _text,
        const QString& _lang
    );

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
    MTHelper();
    Q_DISABLE_COPY(MTHelper)

    QHash<QString, clsEngine*> RegisteredEngines;
    Targoman::Common::tmplBoundedCache<QHash, QString, QVariantMap> TranslationCache;
    QList<QPair<QRegularExpression, QString>> CorrectionRule;
    QTime LastCorrectionRuleUpdateTime;

public:
    static void registerGateway(const QString& _gatewayName, intfTranslatorGateway* _gateway);
    static intfTranslatorGateway* getGateway(const QString& _gatewayName);
private:
    static inline QMap<QString, intfTranslatorGateway*> RegisteredGateways;
};

} //namespace Targoman::API::ModuleHelpers::MT

#endif // TARGOMAN_API_MODULEHELPERS_MT_CLASSES_MTHELPER_H
