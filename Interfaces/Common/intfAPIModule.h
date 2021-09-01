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

#ifndef TARGOMAN_API_INTFAPIMODULE_HPP
#define TARGOMAN_API_INTFAPIMODULE_HPP

#include <QObject>
#include <QtPlugin>
#include <QMap>

#include "libTargomanCommon/Configuration/intfConfigurableModule.hpp"
#include "libTargomanCommon/exTargomanBase.h"

#include "QHttp/qhttpfwd.hpp"

#include "Interfaces/Common/base.h"
#include "Interfaces/Common/HTTPExceptions.hpp"
#include "Interfaces/Common/GenericTypes.h"
#include "Interfaces/ORM/clsORMField.h"

namespace Targoman::API {

/**********************************************************************/
/**  document QT_NO_CAST_FROM_ASCII */
/**********************************************************************/
/**
  * @brief CACHEABLE macros are predefined macros in order to mark each API cache TTL. You can add more cache time as you wish while
  *        following cache definition pattern "\d+(S|M|H)" where last character means S: Seconds, M: Minutes, H: Hours and digits must be between 0 to 16384
  */
#ifndef Q_MOC_RUN
#  define CACHEABLE_1S
#  define CACHEABLE_3S
#  define CACHEABLE_5S
#  define CACHEABLE_10S
#  define CACHEABLE_30S
#  define CACHEABLE_1M
#  define CACHEABLE_5M
#  define CACHEABLE_10M
#  define CACHEABLE_1H
#  define CACHEABLE_3H
#  define CACHEABLE_6H
#  define CACHEABLE_12H
#  define CACHEABLE_24H
#  define CACHEABLE_INF
#endif

/**
  * @brief CENTRALCACHE macros are predefined macros in order to mark each API central cache TTL. You can add more cache time as you wish while
  *        following cache definition pattern "\d+(S|M|H)" where last character means S: Seconds, M: Minutes, H: Hours and digits must be between 0 to 16384
  */
#ifndef Q_MOC_RUN
#  define CENTRALCACHE_1S
#  define CENTRALCACHE_3S
#  define CENTRALCACHE_5S
#  define CENTRALCACHE_10S
#  define CENTRALCACHE_30S
#  define CENTRALCACHE_1M
#  define CENTRALCACHE_5M
#  define CENTRALCACHE_10M
#  define CENTRALCACHE_1H
#  define CENTRALCACHE_3H
#  define CENTRALCACHE_6H
#  define CENTRALCACHE_12H
#  define CENTRALCACHE_24H
#endif

/**
  * @brief APITIMEOUT macros are predefined macros in order to mark each API TTL. You can add more cache time as you wish while
  *        following definition pattern "\d+(S|M|H)" where last character means S: Seconds and digits must be between 0 to 16384
  */
#ifndef Q_MOC_RUN
#  define APITIMEOUT_1S
#  define APITIMEOUT_3S
#  define APITIMEOUT_5S
#  define APITIMEOUT_10S
#  define APITIMEOUT_30S
#endif

#define REST(_method, _name, _sig, _doc) api##_method##_name _sig; QString signOf##_method##_name(){ return #_sig; } QString docOf##_method##_name(){ return #_doc; }
#define ASYNC_REST(_method, _name, _sig, _doc) asyncApi##_method##_name _sig;QString signOf##_method##_name(){ return #_sig; } QString docOf##_method##_name(){ return #_doc; }

#define INTFAPIMODULE_IID "TARGOMAN.API.INTFAPIMODULE/1.0.0"

class intfAPIModule : public Common::Configuration::intfModule
{
public:
    struct stuDBInfo {
        QString Host;
        quint16 Port;
        QString User;
        QString Pass;
        QString Schema;

        stuDBInfo(QString _schema = "", quint16 _port = 0, QString _host = "", QString _user = "", QString _pass = "");
        QString toConnStr(bool _noSchema = false);
    };

    struct stuModuleMethod {
        intfAPIModule* Module;
        QMetaMethod Method;
        stuModuleMethod(intfAPIModule* _module, const QMetaMethod& _method) :
            Module(_module),
            Method(_method)
        {}
    };

    typedef QList<stuModuleMethod> ModuleMethods_t;

public:
    intfAPIModule(Targoman::Common::Configuration::intfModule *_parent = nullptr);
    virtual ~intfAPIModule();
    virtual QList<ORM::clsORMField> filterItems(qhttp::THttpMethod _method = qhttp::EHTTP_ACL) { Q_UNUSED(_method) return {}; }
    virtual void updateFilterParamType(const QString& _fieldTypeName, QMetaType::Type _typeID) { Q_UNUSED(_fieldTypeName) Q_UNUSED(_typeID) }
    virtual ModuleMethods_t listOfMethods() = 0;
    virtual stuDBInfo requiredDB() const { return {}; }
    virtual QString parentModuleName() const = 0;
    virtual bool init() { return true; }

protected:
    ModuleMethods_t Methods;
};

} //namespace Targoman::API

Q_DECLARE_INTERFACE(Targoman::API::intfAPIModule, INTFAPIMODULE_IID)

#define TARGOMAN_DEFINE_API_MODULE(_name) \
public: \
    QString moduleFullName() { return Targoman::Common::demangle(typeid(_name).name()); } \
    QString moduleBaseName() { return QStringLiteral(TARGOMAN_M2STR(_name)); }  \
    QString parentModuleName() const final { return QString(); } \
    ModuleMethods_t listOfMethods() final { \
        if (this->Methods.size()) return this->Methods; \
        for (int i=0; i<this->metaObject()->methodCount(); ++i) \
            this->Methods.append({ this, this->metaObject()->method(i) }); \
        return this->Methods; \
    } \
    void addSubModule(intfAPIModule* _submodule) { \
        if (this->Methods.isEmpty()) listOfMethods(); \
        if (_submodule->parentModuleName() != this->moduleBaseName()) \
            throw Common::exTargomanNotImplemented(QString("Not from same parent (%1 <> %2)").arg(_submodule->parentModuleName()).arg(this->moduleBaseName())); \
        for (int i=0; i<_submodule->metaObject()->methodCount(); ++i) \
            this->Methods.append({ _submodule, _submodule->metaObject()->method(i) }); \
        _submodule->init(); \
    } \
private: \
    TAPI_DISABLE_COPY(_name); \
public: _name();

#define TARGOMAN_DEFINE_API_SUBMODULE(_module, _name) \
public: \
    QString moduleFullName() { return Targoman::Common::demangle(typeid(_name).name()); }\
    QString moduleBaseName() { return QStringLiteral(TARGOMAN_M2STR(TARGOMAN_CAT_BY_COLON(_module,_name))); } \
    ModuleMethods_t listOfMethods() final { \
        throw Common::exTargomanNotImplemented("listOfMethods must not be called on submodules"); \
    } \
    QString parentModuleName() const final { return TARGOMAN_M2STR(_module); } \
    static _name& instance() { static _name* Instance = nullptr; return *(Q_LIKELY(Instance) ? Instance : (Instance = new _name)); } \
private: \
    _name(); \
    TAPI_DISABLE_COPY(_name)

//static inline QString makeConfig(const QString& _name) { return QString("zModule_%1/DB/%2").arg(TARGOMAN_M2STR(_module), _name); }
#define TARGOMAN_API_MODULE_DB_CONFIGS(_module) \
    struct DB { \
        static inline QString makeConfig(const QString& _name) { return QString("/Module_%1/DB/%2").arg(TARGOMAN_M2STR(_module), _name); } \
        static Common::Configuration::tmplConfigurable<QString>       Host;   \
        static Common::Configuration::tmplRangedConfigurable<quint16> Port;   \
        static Common::Configuration::tmplConfigurable<QString>       User;   \
        static Common::Configuration::tmplConfigurable<QString>       Pass;   \
        static Common::Configuration::tmplConfigurable<QString>       Schema; \
    }; \
public: \
    stuDBInfo requiredDB() const { \
        return stuDBInfo( \
            DB::Schema.value(), \
            DB::Port.value(), \
            DB::Host.value(), \
            DB::User.value(), \
            DB::Pass.value() \
        ); \
    }

/**
  * Host must be no default, because requiredDB() uses this
  */
#define TARGOMAN_API_MODULE_DB_CONFIG_IMPL(_module, _schema)\
    using namespace Targoman::Common::Configuration;        \
    tmplConfigurable<QString> _module::DB::Host(            \
        _module::DB::makeConfig("Host"),                    \
        "Database Host address",                            \
        ""                                                  \
    );                                                      \
    tmplRangedConfigurable<quint16> _module::DB::Port(      \
        _module::DB::makeConfig("Port"),                    \
        "Database port",                                    \
        10, 65000,                                          \
        3306                                                \
    );                                                      \
    tmplConfigurable<QString> _module::DB::User(            \
        _module::DB::makeConfig("User"),                    \
        "Database username for connection",                 \
        "root"                                              \
    );                                                      \
    tmplConfigurable<QString> _module::DB::Pass(            \
        _module::DB::makeConfig("Pass"),                    \
        "Database password",                                \
        ""                                                  \
    );                                                      \
    tmplConfigurable<QString> _module::DB::Schema(          \
        _module::DB::makeConfig("Schema"),                  \
        "Database schema",                                  \
        _schema                                             \
    );                                                      \

#endif // TARGOMAN_API_INTFAPIMODULE_HPP
