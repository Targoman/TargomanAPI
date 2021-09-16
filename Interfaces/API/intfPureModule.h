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

#ifndef TARGOMAN_API_INTFPUREMODULE_H
#define TARGOMAN_API_INTFPUREMODULE_H

#include <QObject>
#include <QtPlugin>
#include <QMap>
#include "libTargomanCommon/Configuration/intfConfigurableModule.hpp"
#include "libTargomanCommon/exTargomanBase.h"
#include "QHttp/qhttpfwd.hpp"
#include "Interfaces/Common/base.h"
#include "Interfaces/Common/GenericTypes.h"
#include "Interfaces/Common/HTTPExceptions.hpp"
#include "Interfaces/Common/APIArgHelperMacros.hpp"
#include "Interfaces/DBM/clsORMField.h"
#include "Interfaces/DBM/clsTable.h"

//used by Api call methods
#define GET_METHOD_ARGS_HEADER_APICALL   TAPI::JWT_t _JWT, TAPI::PKsByPath_t _pksByPath = {}, quint64 _offset = 0, quint16 _limit = 10, TAPI::Cols_t _cols = {}, TAPI::Filter_t _filters = {}, TAPI::OrderBy_t _orderBy = {}, TAPI::GroupBy_t _groupBy = {}, bool _reportCount = true
#define GET_METHOD_ARGS_IMPL_APICALL     TAPI::JWT_t _JWT, TAPI::PKsByPath_t _pksByPath, quint64 _offset, quint16 _limit, TAPI::Cols_t _cols, TAPI::Filter_t _filters, TAPI::OrderBy_t _orderBy, TAPI::GroupBy_t _groupBy, bool _reportCount
//#define GET_METHOD_CALL_ARGS_APICALL     _JWT, _pksByPath, _offset, _limit, _cols, _filters, _orderBy, _groupBy, _reportCount
#define ORMGET(_doc)                     apiGET(GET_METHOD_ARGS_HEADER_APICALL); \
    QString signOfGET(){ return TARGOMAN_M2STR((GET_METHOD_ARGS_HEADER_APICALL)); } \
    QString docOfGET() { return _doc; }
#define ORMGETPureVirtual(_doc)          apiGET(GET_METHOD_ARGS_HEADER_APICALL)=0; \
    virtual QString signOfGET(){ return TARGOMAN_M2STR((GET_METHOD_ARGS_HEADER_APICALL)); } \
    virtual QString docOfGET() { return _doc; }
#define ORMGETWithName(_name, _doc)      apiGET##_name(GET_METHOD_ARGS_HEADER_APICALL); \
    QString signOfGET##_name(){ return TARGOMAN_M2STR((GET_METHOD_ARGS_HEADER_APICALL)); } \
    QString docOfGET##_name() { return _doc; }
//used by ApiQuery
#define GET_METHOD_ARGS_HEADER_INTERNAL_CALL   quint64 _userID, TAPI::PKsByPath_t _pksByPath = {}, quint64 _offset = 0, quint16 _limit = 10, TAPI::Cols_t _cols = {}, TAPI::Filter_t _filters = {}, TAPI::OrderBy_t _orderBy = {}, TAPI::GroupBy_t _groupBy = {}, bool _reportCount = true
#define GET_METHOD_ARGS_IMPL_INTERNAL_CALL     quint64 _userID, TAPI::PKsByPath_t _pksByPath, quint64 _offset, quint16 _limit, TAPI::Cols_t _cols, TAPI::Filter_t _filters, TAPI::OrderBy_t _orderBy, TAPI::GroupBy_t _groupBy, bool _reportCount
#define GET_METHOD_CALL_ARGS_INTERNAL_CALL     clsJWT(_JWT).usrID(), _pksByPath, _offset, _limit, _cols, _filters, _orderBy, _groupBy, _reportCount
#define GET_METHOD_CALL_ARGS_INTERNAL_CALL_RAW _userID, _pksByPath, _offset, _limit, _cols, _filters, _orderBy, _groupBy, _reportCount

//used by Api call methods
#define CREATE_METHOD_ARGS_HEADER_APICALL   TAPI::JWT_t _JWT, TAPI::ORMFields_t _createInfo = {}
#define CREATE_METHOD_ARGS_IMPL_APICALL     TAPI::JWT_t _JWT, TAPI::ORMFields_t _createInfo
//#define CREATE_METHOD_CALL_ARGS_APICALL     _JWT, _createInfo
#define ORMCREATE(_doc)                     apiCREATE(CREATE_METHOD_ARGS_HEADER_APICALL); \
    QString signOfCREATE(){ return TARGOMAN_M2STR((CREATE_METHOD_ARGS_HEADER_APICALL)); } \
    QString docOfCREATE() { return _doc; }
//used by ApiQuery
#define CREATE_METHOD_ARGS_HEADER_INTERNAL_CALL quint64 _userID, TAPI::ORMFields_t _createInfo = {}
#define CREATE_METHOD_ARGS_IMPL_INTERNAL_CALL   quint64 _userID, TAPI::ORMFields_t _createInfo
#define CREATE_METHOD_CALL_ARGS_INTERNAL_CALL   clsJWT(_JWT).usrID(), _createInfo

//used by Api call methods
#define UPDATE_METHOD_ARGS_HEADER_APICALL TAPI::JWT_t _JWT, TAPI::PKsByPath_t _pksByPath = {}, const TAPI::ORMFields_t& _updateInfo = {}
#define UPDATE_METHOD_ARGS_IMPL_APICALL   TAPI::JWT_t _JWT, TAPI::PKsByPath_t _pksByPath, const TAPI::ORMFields_t& _updateInfo
//#define UPDATE_METHOD_CALL_ARGS_APICALL   clsJWT(_JWT).usrID(), _pksByPath, _updateInfo
#define ORMUPDATE(_doc)                   apiUPDATE(UPDATE_METHOD_ARGS_HEADER_APICALL); \
    QString signOfUPDATE(){ return TARGOMAN_M2STR((UPDATE_METHOD_ARGS_HEADER_APICALL)); } \
    QString docOfUPDATE() { return _doc; }
//used by ApiQuery
#define UPDATE_METHOD_ARGS_HEADER_INTERNAL_CALL quint64 _userID, TAPI::PKsByPath_t _pksByPath = {}, const TAPI::ORMFields_t& _updateInfo = {}
#define UPDATE_METHOD_ARGS_IMPL_INTERNAL_CALL   quint64 _userID, TAPI::PKsByPath_t _pksByPath, const TAPI::ORMFields_t& _updateInfo
#define UPDATE_METHOD_CALL_ARGS_INTERNAL_CALL   clsJWT(_JWT).usrID(), _pksByPath, _updateInfo

//used by Api call methods
#define DELETE_METHOD_ARGS_HEADER_APICALL TAPI::JWT_t _JWT, TAPI::PKsByPath_t _pksByPath = {}
#define DELETE_METHOD_ARGS_IMPL_APICALL   TAPI::JWT_t _JWT, TAPI::PKsByPath_t _pksByPath
//#define DELETE_METHOD_CALL_ARGS_APICALL   clsJWT(_JWT).usrID(), _pksByPath
#define ORMDELETE(_doc)                   apiDELETE(DELETE_METHOD_ARGS_HEADER_APICALL); \
    QString signOfDELETE(){ return TARGOMAN_M2STR((DELETE_METHOD_ARGS_HEADER_APICALL)); } \
    QString docOfDELETE() { return _doc; }
//used by ApiQuery
#define DELETE_METHOD_ARGS_HEADER_INTERNAL_CALL quint64 _userID, TAPI::PKsByPath_t _pksByPath = {}
#define DELETE_METHOD_ARGS_IMPL_INTERNAL_CALL   quint64 _userID, TAPI::PKsByPath_t _pksByPath
#define DELETE_METHOD_CALL_ARGS_INTERNAL_CALL   clsJWT(_JWT).usrID(), _pksByPath

namespace TAPI {
TAPI_ADD_TYPE_STRING(Cols_t);
TAPI_ADD_TYPE_STRING(Filter_t);
TAPI_ADD_TYPE_STRING(OrderBy_t);
TAPI_ADD_TYPE_STRING(GroupBy_t);
}

TAPI_DECLARE_METATYPE(TAPI::Cols_t);
TAPI_DECLARE_METATYPE(TAPI::Filter_t);
TAPI_DECLARE_METATYPE(TAPI::OrderBy_t);
TAPI_DECLARE_METATYPE(TAPI::GroupBy_t);

namespace Targoman::API::API {

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

#define INTFPUREMODULE_IID "TARGOMAN.API.API.INTFPUREMODULE/1.0.0"

//class intfBaseModule
//{
//public:
//    intfBaseModule(intfBaseModule* _parent = nullptr) { Q_UNUSED(_parent) }

//    virtual QString moduleFullName() {
//        throw exTargomanMustBeImplemented("Seems that you forgot to use TARGOMAN_DEFINE_MODULE_SCOPE macro");
//    }

//    virtual QString moduleBaseName(){
//        throw exTargomanMustBeImplemented("Seems that you forgot to use TARGOMAN_DEFINE_MODULE_SCOPE macro");
//    }
//};

class intfPureModule : public Targoman::Common::Configuration::intfModule
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
        intfPureModule* Module;
        QMetaMethod Method;

        stuModuleMethod(intfPureModule* _module, const QMetaMethod& _method) :
            Module(_module),
            Method(_method)
        {}
    };

    typedef QList<stuModuleMethod> ModuleMethods_t;

public:
    intfPureModule(
        const QString& _moduleName,
        Targoman::Common::Configuration::intfModule *_parent = nullptr
    );
    virtual ~intfPureModule() = default;

    virtual QString parentModuleName() const /*{ return QString(); }; //*/= 0;
    virtual stuDBInfo requiredDB() const { return {}; }

    virtual bool init() { return true; }

    virtual QList<DBM::clsORMField> filterItems(qhttp::THttpMethod _method = qhttp::EHTTP_ACL)
    {
//        Q_UNUSED(_method)

        Targoman::API::DBM::clsTable* PTHIS = dynamic_cast<Targoman::API::DBM::clsTable*>(this);
        if (PTHIS != nullptr)
            return PTHIS->filterItems(_method);

        return {};
    }
    virtual void updateFilterParamType(const QString& _fieldTypeName, QMetaType::Type _typeID)
    {
//        Q_UNUSED(_fieldTypeName)
//        Q_UNUSED(_typeID)

        Targoman::API::DBM::clsTable* PTHIS = dynamic_cast<Targoman::API::DBM::clsTable*>(this);
        if (PTHIS != nullptr)
            PTHIS->updateFilterParamType(_fieldTypeName, _typeID);
    }

    virtual ModuleMethods_t listOfMethods() = 0;

protected:
    ModuleMethods_t Methods;
    QString ModuleName;
};

} //namespace Targoman::API::API

Q_DECLARE_INTERFACE(Targoman::API::API::intfPureModule, INTFPUREMODULE_IID)

#define TARGOMAN_DEFINE_API_MODULE(_name) \
public: \
    QString parentModuleName() const final { return QString(); } \
    QString moduleBaseName() { return QStringLiteral(TARGOMAN_M2STR(_name)); }  \
    QString moduleFullName() { return Targoman::Common::demangle(typeid(_name).name()); } \
    ModuleMethods_t listOfMethods() final { \
        if (this->Methods.size()) return this->Methods; \
        for (int i=0; i<this->metaObject()->methodCount(); ++i) \
            this->Methods.append({ this, this->metaObject()->method(i) }); \
        return this->Methods; \
    } \
    void addSubModule(intfPureModule* _submodule) { \
        if (this->Methods.isEmpty()) listOfMethods(); \
        if (_submodule->parentModuleName() != this->moduleBaseName()) \
            throw Targoman::Common::exTargomanNotImplemented(QString("Not from same parent (%1 <> %2)").arg(_submodule->parentModuleName()).arg(this->moduleBaseName())); \
        for (int i=0; i<_submodule->metaObject()->methodCount(); ++i) \
            this->Methods.append({ _submodule, _submodule->metaObject()->method(i) }); \
        _submodule->init(); \
    } \
private: \
    TAPI_DISABLE_COPY(_name); \
public: \
    _name();

#define TARGOMAN_DEFINE_API_SUBMODULE(_module, _name) \
public: \
    QString parentModuleName() const final { return TARGOMAN_M2STR(_module); } \
    QString moduleBaseName() { return QStringLiteral(TARGOMAN_M2STR(TARGOMAN_CAT_BY_COLON(_module, _name))); } \
    QString moduleFullName() { return Targoman::Common::demangle(typeid(_name).name()); } \
    ModuleMethods_t listOfMethods() final { \
        throw Targoman::Common::exTargomanNotImplemented("listOfMethods must not be called on submodules"); \
    } \
    static _name& instance() { static _name* Instance = nullptr; return *(Q_LIKELY(Instance) ? Instance : (Instance = new _name)); } \
private: \
    _name(); \
    TAPI_DISABLE_COPY(_name)

//static inline QString makeConfig(const QString& _name) { return QString("zModule_%1/DB/%2").arg(TARGOMAN_M2STR(_module), _name); }
#define TARGOMAN_API_MODULE_DB_CONFIGS(_module) \
    struct DB { \
        static inline QString makeConfig(const QString& _name) { return QString("/Module_%1/DB/%2").arg(TARGOMAN_M2STR(_module), _name); } \
        static Targoman::Common::Configuration::tmplConfigurable<QString>       Host;   \
        static Targoman::Common::Configuration::tmplRangedConfigurable<quint16> Port;   \
        static Targoman::Common::Configuration::tmplConfigurable<QString>       User;   \
        static Targoman::Common::Configuration::tmplConfigurable<QString>       Pass;   \
        static Targoman::Common::Configuration::tmplConfigurable<QString>       Schema; \
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

#endif // TARGOMAN_API_INTFPUREMODULE_H
