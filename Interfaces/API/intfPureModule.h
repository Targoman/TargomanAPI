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

#include "Interfaces/Server/APICallBoom.h"
using namespace Targoman::API::Server;

//#define RESPONSE_HEADER_X_PAGINATION_TOTAL_COUNT    "x-pagination-total-count"
//#define RESPONSE_HEADER_X_PAGINATION_PAGE_COUNT     "x-pagination-page-count"
//#define RESPONSE_HEADER_X_PAGINATION_CURRENT_PAGE   "x-pagination-current-page"
//#define RESPONSE_HEADER_X_PAGINATION_PER_PAGE       "x-pagination-per-page"

/**********************************************************************/
#define ORM_EX_CONFIG(_method, ...)     QVariantMap confOf##_method() { return __VA_ARGS__; }

/**********************************************************************\
|** GET ***************************************************************|
\**********************************************************************/
//used by Api call methods
#define INTERNAL_GET_METHOD_ARGS_DECL_APICALL(_apiCallBoomType) \
    _apiCallBoomType    &APICALLBOOM_PARAM, \
    TAPI::PKsByPath_t   _pksByPath = {}, \
    quint64             _pageIndex = 0, \
    quint16             _pageSize = 20, \
    TAPI::Cols_t        _cols = {}, \
    TAPI::Filter_t      _filters = {}, \
    TAPI::OrderBy_t     _orderBy = {}, \
    TAPI::GroupBy_t     _groupBy = {}, \
    bool                _reportCount = true

#define INTERNAL_GET_METHOD_ARGS_IMPL_APICALL(_apiCallBoomType) \
    _apiCallBoomType    &APICALLBOOM_PARAM, \
    TAPI::PKsByPath_t   _pksByPath, \
    quint64             _pageIndex, \
    quint16             _pageSize, \
    TAPI::Cols_t        _cols, \
    TAPI::Filter_t      _filters, \
    TAPI::OrderBy_t     _orderBy, \
    TAPI::GroupBy_t     _groupBy, \
    bool                _reportCount

#define            GET_METHOD_ARGS_DECL_APICALL     INTERNAL_GET_METHOD_ARGS_DECL_APICALL(APICALLBOOM_TYPE_JWT_DECL)
#define ANONYMOUSE_GET_METHOD_ARGS_DECL_APICALL     INTERNAL_GET_METHOD_ARGS_DECL_APICALL(APICALLBOOM_TYPE_NO_JWT_DECL)
#define            GET_METHOD_ARGS_IMPL_APICALL     INTERNAL_GET_METHOD_ARGS_IMPL_APICALL(APICALLBOOM_TYPE_JWT_IMPL)
#define ANONYMOUSE_GET_METHOD_ARGS_IMPL_APICALL     INTERNAL_GET_METHOD_ARGS_IMPL_APICALL(APICALLBOOM_TYPE_NO_JWT_IMPL)

#define ANONYMOUSE_ORMGET(_doc, ...)                apiGET(ANONYMOUSE_GET_METHOD_ARGS_DECL_APICALL) __VA_ARGS__; \
                                                    QString signOfGET() { return TARGOMAN_M2STR((GET_METHOD_ARGS_DECL_APICALL)); } \
                                                    QString docOfGET() { return _doc; }

#define ANONYMOUSE_ORMGET_HIDDEN(_doc, ...)         ANONYMOUSE_ORMGET(_doc, __VA_ARGS__); ORM_EX_CONFIG(GET, { EXRESTCONFIG_HIDDEN })

#define ORMGET(_doc, ...)                           apiGET(GET_METHOD_ARGS_DECL_APICALL) __VA_ARGS__; \
                                                    QString signOfGET() { return TARGOMAN_M2STR((GET_METHOD_ARGS_DECL_APICALL)); } \
                                                    QString docOfGET() { return _doc; }

#define IMPL_ORMGET(_module)                        _module::apiGET(GET_METHOD_ARGS_IMPL_APICALL)
#define IMPL_ANONYMOUSE_ORMGET(_module)             _module::apiGET(ANONYMOUSE_GET_METHOD_ARGS_IMPL_APICALL)

#define ORMGET_HIDDEN(_doc, ...)                    ORMGET(_doc, __VA_ARGS__); ORM_EX_CONFIG(GET, { EXRESTCONFIG_HIDDEN })

#define ORMGETByName(_name, _doc, ...)              apiGET##_name(GET_METHOD_ARGS_DECL_APICALL) __VA_ARGS__; \
                                                    QString signOfGET##_name() { return TARGOMAN_M2STR((GET_METHOD_ARGS_DECL_APICALL)); } \
                                                    QString docOfGET##_name() { return _doc; }

//used by intfSQLBasedModule
#define GET_METHOD_ARGS_DECL_INTERNAL_BOOM          INTERNAL_GET_METHOD_ARGS_DECL_APICALL(INTFAPICALLBOOM_DECL)
#define GET_METHOD_ARGS_IMPL_INTERNAL_BOOM          INTERNAL_GET_METHOD_ARGS_IMPL_APICALL(INTFAPICALLBOOM_IMPL)

#define GET_METHOD_ARGS_CALL_INTERNAL_BOOM \
    APICALLBOOM_PARAM, \
    _pksByPath, \
    _pageIndex, \
    _pageSize, \
    _cols, \
    _filters, \
    _orderBy, \
    _groupBy, \
    _reportCount

/**********************************************************************\
|** CREATE ************************************************************|
\**********************************************************************/
//used by Api call methods
#define CREATE_METHOD_ARGS_DECL_APICALL             APICALLBOOM_TYPE_JWT_DECL &APICALLBOOM_PARAM, TAPI::ORMFields_t _createInfo = {}
#define CREATE_METHOD_ARGS_IMPL_APICALL             APICALLBOOM_TYPE_JWT_IMPL &APICALLBOOM_PARAM, TAPI::ORMFields_t _createInfo

#define ORMCREATE(_doc, ...)                        apiCREATE(CREATE_METHOD_ARGS_DECL_APICALL) __VA_ARGS__; \
    QString signOfCREATE() { return TARGOMAN_M2STR((CREATE_METHOD_ARGS_DECL_APICALL)); } \
    QString docOfCREATE() { return _doc; }

#define IMPL_ORMCREATE(_module)                     _module::apiCREATE(CREATE_METHOD_ARGS_IMPL_APICALL)

//used by intfSQLBasedModule
//#define CREATE_METHOD_ARGS_DECL_INTERNAL_USER       quint64 _userID, TAPI::ORMFields_t _createInfo = {}
//#define CREATE_METHOD_ARGS_IMPL_INTERNAL_USER       quint64 _userID, TAPI::ORMFields_t _createInfo
//#define CREATE_METHOD_ARGS_CALL_INTERNAL_USER       _userID, _createInfo

#define CREATE_METHOD_ARGS_DECL_INTERNAL_BOOM       INTFAPICALLBOOM_DECL &APICALLBOOM_PARAM, TAPI::ORMFields_t _createInfo = {}
#define CREATE_METHOD_ARGS_IMPL_INTERNAL_BOOM       INTFAPICALLBOOM_IMPL &APICALLBOOM_PARAM, TAPI::ORMFields_t _createInfo
#define CREATE_METHOD_ARGS_CALL_INTERNAL_BOOM       APICALLBOOM_PARAM, _createInfo

//#define CREATE_METHOD_ARGS_CALL_INTERNAL_BOOM2USER  APICALLBOOM_PARAM.getUserID(), _createInfo

/**********************************************************************\
|** UPDATE ************************************************************|
\**********************************************************************/
//used by Api call methods
#define UPDATE_METHOD_ARGS_DECL_APICALL             APICALLBOOM_TYPE_JWT_DECL &APICALLBOOM_PARAM, TAPI::PKsByPath_t _pksByPath = {}, const TAPI::ORMFields_t& _updateInfo = {}
#define UPDATE_METHOD_ARGS_IMPL_APICALL             APICALLBOOM_TYPE_JWT_IMPL &APICALLBOOM_PARAM, TAPI::PKsByPath_t _pksByPath, const TAPI::ORMFields_t& _updateInfo

#define ORMUPDATE(_doc, ...)                        apiUPDATE(UPDATE_METHOD_ARGS_DECL_APICALL) __VA_ARGS__; \
    QString signOfUPDATE() { return TARGOMAN_M2STR((UPDATE_METHOD_ARGS_DECL_APICALL)); } \
    QString docOfUPDATE() { return _doc; }

#define IMPL_ORMUPDATE(_module)                     _module::apiUPDATE(UPDATE_METHOD_ARGS_IMPL_APICALL)

//used by intfSQLBasedModule
//#define UPDATE_METHOD_ARGS_DECL_INTERNAL_USER       quint64 _userID, TAPI::PKsByPath_t _pksByPath = {}, const TAPI::ORMFields_t& _updateInfo = {}
//#define UPDATE_METHOD_ARGS_IMPL_INTERNAL_USER       quint64 _userID, TAPI::PKsByPath_t _pksByPath, const TAPI::ORMFields_t& _updateInfo
//#define UPDATE_METHOD_ARGS_CALL_INTERNAL_USER       _userID, _pksByPath, _updateInfo

#define UPDATE_METHOD_ARGS_DECL_INTERNAL_BOOM       INTFAPICALLBOOM_DECL &APICALLBOOM_PARAM, TAPI::PKsByPath_t _pksByPath = {}, const TAPI::ORMFields_t& _updateInfo = {}
#define UPDATE_METHOD_ARGS_IMPL_INTERNAL_BOOM       INTFAPICALLBOOM_IMPL &APICALLBOOM_PARAM, TAPI::PKsByPath_t _pksByPath, const TAPI::ORMFields_t& _updateInfo
#define UPDATE_METHOD_ARGS_CALL_INTERNAL_BOOM       APICALLBOOM_PARAM, _pksByPath, _updateInfo

//#define UPDATE_METHOD_ARGS_CALL_INTERNAL_BOOM2USER  APICALLBOOM_PARAM.getUserID(), _pksByPath, _updateInfo

/**********************************************************************\
|** DELETE ************************************************************|
\**********************************************************************/
//used by Api call methods
#define DELETE_METHOD_ARGS_DECL_APICALL             APICALLBOOM_TYPE_JWT_DECL &APICALLBOOM_PARAM, TAPI::PKsByPath_t _pksByPath = {}
#define DELETE_METHOD_ARGS_IMPL_APICALL             APICALLBOOM_TYPE_JWT_IMPL &APICALLBOOM_PARAM, TAPI::PKsByPath_t _pksByPath

#define ORMDELETE(_doc, ...)                        apiDELETE(DELETE_METHOD_ARGS_DECL_APICALL) __VA_ARGS__; \
    QString signOfDELETE() { return TARGOMAN_M2STR((DELETE_METHOD_ARGS_DECL_APICALL)); } \
    QString docOfDELETE() { return _doc; }

#define IMPL_ORMDELETE(_module)                     _module::apiDELETE(DELETE_METHOD_ARGS_IMPL_APICALL)

//used by intfSQLBasedModule
//#define DELETE_METHOD_ARGS_DECL_INTERNAL_USER       quint64 _userID, TAPI::PKsByPath_t _pksByPath = {}
//#define DELETE_METHOD_ARGS_IMPL_INTERNAL_USER       quint64 _userID, TAPI::PKsByPath_t _pksByPath
//#define DELETE_METHOD_ARGS_CALL_INTERNAL_USER       _userID, _pksByPath

#define DELETE_METHOD_ARGS_DECL_INTERNAL_BOOM       INTFAPICALLBOOM_DECL &APICALLBOOM_PARAM, TAPI::PKsByPath_t _pksByPath = {}
#define DELETE_METHOD_ARGS_IMPL_INTERNAL_BOOM       INTFAPICALLBOOM_IMPL &APICALLBOOM_PARAM, TAPI::PKsByPath_t _pksByPath
#define DELETE_METHOD_ARGS_CALL_INTERNAL_BOOM       APICALLBOOM_PARAM, _pksByPath

//#define DELETE_METHOD_ARGS_CALL_INTERNAL_BOOM2USER  APICALLBOOM_PARAM.getUserID(), _pksByPath

/**********************************************************************/
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

//-- REST BASE MACROS
#define EXRESTCONFIGKEY_HIDDEN "hidden"
#define EXRESTCONFIG_HIDDEN { EXRESTCONFIGKEY_HIDDEN, true }
#define EXRESTCONFIGKEY_ALIAS "alias"
#define EXRESTCONFIG_ALIAS(_alias) { EXRESTCONFIGKEY_ALIAS, _alias }

#define EXREST(_method, _name, _sig, _doc, ...) \
    api##_method##_name _sig; \
    QString signOf##_method##_name() { return #_sig; } \
    QString docOf##_method##_name() { return #_doc; } \
    QVariantMap confOf##_method##_name() { return __VA_ARGS__; }
#define REST(_method, _name, _sig, _doc) \
    EXREST(_method, _name, _sig, _doc, {})
#define IMPL_REST(_method, _module, _name, _params) \
    _module::api##_method##_name _params

#define ASYNC_EXREST(_method, _name, _sig, _doc, ...) \
    asyncApi##_method##_name _sig; \
    QString signOf##_method##_name() { return #_sig; } \
    QString docOf##_method##_name() { return #_doc; } \
    QVariantMap confOf##_method##_name() { return __VA_ARGS__; }
#define ASYNC_REST(_method, _name, _sig, _doc) \
    ASYNC_EXREST(_method, _name, _sig, _doc, {})
#define IMPL_ASYNC_REST(_method, _module, _name, _params) \
    _module::asyncApi##_method##_name _params

//-- REST MACROS
#define           EXREST_GET_OR_POST(_name, _sig, _doc, ...)              EXREST(, _name, _sig, _doc, __VA_ARGS__)
#define     ASYNC_EXREST_GET_OR_POST(_name, _sig, _doc, ...)        ASYNC_EXREST(, _name, _sig, _doc, __VA_ARGS__)
#define             REST_GET_OR_POST(_name, _sig, _doc)                     REST(, _name, _sig, _doc)
#define       ASYNC_REST_GET_OR_POST(_name, _sig, _doc)               ASYNC_REST(, _name, _sig, _doc)
#define        IMPL_REST_GET_OR_POST(_module, _name, _params)          IMPL_REST(, _module, _name, _params)
#define  IMPL_ASYNC_REST_GET_OR_POST(_module, _name, _params)    IMPL_ASYNC_REST(, _module, _name, _params)

///@TODO: create IMPL_REST_... for other cases
#define           EXREST_GET(_name, _sig, _doc, ...)                      EXREST(GET, _name, _sig, _doc, __VA_ARGS__)
#define     ASYNC_EXREST_GET(_name, _sig, _doc, ...)                ASYNC_EXREST(GET, _name, _sig, _doc, __VA_ARGS__)
#define             REST_GET(_name, _sig, _doc)                             REST(GET, _name, _sig, _doc)
#define       ASYNC_REST_GET(_name, _sig, _doc)                       ASYNC_REST(GET, _name, _sig, _doc)
#define        IMPL_REST_GET(_module, _name, _params)                  IMPL_REST(GET, _module, _name, _params)
#define  IMPL_ASYNC_REST_GET(_module, _name, _params)            IMPL_ASYNC_REST(GET, _module, _name, _params)

#define           EXREST_PUT(_name, _sig, _doc, ...)                      EXREST(PUT, _name, _sig, _doc, __VA_ARGS__)
#define     ASYNC_EXREST_PUT(_name, _sig, _doc, ...)                ASYNC_EXREST(PUT, _name, _sig, _doc, __VA_ARGS__)
#define             REST_PUT(_name, _sig, _doc)                             REST(PUT, _name, _sig, _doc)
#define       ASYNC_REST_PUT(_name, _sig, _doc)                       ASYNC_REST(PUT, _name, _sig, _doc)
#define        IMPL_REST_PUT(_module, _name, _params)                  IMPL_REST(PUT, _module, _name, _params)
#define  IMPL_ASYNC_REST_PUT(_module, _name, _params)            IMPL_ASYNC_REST(PUT, _module, _name, _params)

#define           EXREST_POST(_name, _sig, _doc, ...)                     EXREST(POST, _name, _sig, _doc, __VA_ARGS__)
#define     ASYNC_EXREST_POST(_name, _sig, _doc, ...)               ASYNC_EXREST(POST, _name, _sig, _doc, __VA_ARGS__)
#define             REST_POST(_name, _sig, _doc)                            REST(POST, _name, _sig, _doc)
#define       ASYNC_REST_POST(_name, _sig, _doc)                      ASYNC_REST(POST, _name, _sig, _doc)
#define        IMPL_REST_POST(_module, _name, _params)                 IMPL_REST(POST, _module, _name, _params)
#define  IMPL_ASYNC_REST_POST(_module, _name, _params)           IMPL_ASYNC_REST(POST, _module, _name, _params)

#define           EXREST_CREATE(_name, _sig, _doc, ...)                   EXREST(CREATE, _name, _sig, _doc, __VA_ARGS__)
#define     ASYNC_EXREST_CREATE(_name, _sig, _doc, ...)             ASYNC_EXREST(CREATE, _name, _sig, _doc, __VA_ARGS__)
#define             REST_CREATE(_name, _sig, _doc)                          REST(CREATE, _name, _sig, _doc)
#define       ASYNC_REST_CREATE(_name, _sig, _doc)                    ASYNC_REST(CREATE, _name, _sig, _doc)
#define        IMPL_REST_CREATE(_module, _name, _params)               IMPL_REST(CREATE, _module, _name, _params)
#define  IMPL_ASYNC_REST_CREATE(_module, _name, _params)         IMPL_ASYNC_REST(CREATE, _module, _name, _params)

#define           EXREST_UPDATE(_name, _sig, _doc, ...)                   EXREST(UPDATE, _name, _sig, _doc, __VA_ARGS__)
#define     ASYNC_EXREST_UPDATE(_name, _sig, _doc, ...)             ASYNC_EXREST(UPDATE, _name, _sig, _doc, __VA_ARGS__)
#define             REST_UPDATE(_name, _sig, _doc)                          REST(UPDATE, _name, _sig, _doc)
#define       ASYNC_REST_UPDATE(_name, _sig, _doc)                    ASYNC_REST(UPDATE, _name, _sig, _doc)
#define        IMPL_REST_UPDATE(_module, _name, _params)               IMPL_REST(UPDATE, _module, _name, _params)
#define  IMPL_ASYNC_REST_UPDATE(_module, _name, _params)         IMPL_ASYNC_REST(UPDATE, _module, _name, _params)
//--
//#define           EXREST_PATCH(_name, _sig, _doc, ...)             EXREST_UPDATE(_name, _sig, _doc, __VA_ARGS__)
//#define     ASYNC_EXREST_PATCH(_name, _sig, _doc, ...)       ASYNC_EXREST_UPDATE(_name, _sig, _doc, __VA_ARGS__)
//#define             REST_PATCH(_name, _sig, _doc)                    REST_UPDATE(_name, _sig, _doc)
//#define       ASYNC_REST_PATCH(_name, _sig, _doc)              ASYNC_REST_UPDATE(_name, _sig, _doc)
//#define        IMPL_REST_PATCH(_module, _name, _params)                IMPL_REST(PATCH, _module, _name, _params)
//#define  IMPL_ASYNC_REST_PATCH(_module, _name, _params)          IMPL_ASYNC_REST(PATCH, _module, _name, _params)

#define           EXREST_DELETE(_name, _sig, _doc, ...)                   EXREST(DELETE, _name, _sig, _doc, __VA_ARGS__)
#define     ASYNC_EXREST_DELETE(_name, _sig, _doc, ...)             ASYNC_EXREST(DELETE, _name, _sig, _doc, __VA_ARGS__)
#define             REST_DELETE(_name, _sig, _doc)                          REST(DELETE, _name, _sig, _doc)
#define       ASYNC_REST_DELETE(_name, _sig, _doc)                    ASYNC_REST(DELETE, _name, _sig, _doc)
#define        IMPL_REST_DELETE(_module, _name, _params)               IMPL_REST(DELETE, _module, _name, _params)
#define  IMPL_ASYNC_REST_DELETE(_module, _name, _params)         IMPL_ASYNC_REST(DELETE, _module, _name, _params)

//
#define INTFPUREMODULE_IID "TARGOMAN.API.API.INTFPUREMODULE/1.0.0"

class intfPureModule : public Targoman::Common::Configuration::intfModule
{
    Q_OBJECT

public:
    struct stuDBInfo {
        QString Host;
        quint16 Port;
        QString User;
        QString Pass;
        QString Schema;

        stuDBInfo(QString _schema = "", quint16 _port = 0, QString _host = "", QString _user = "", QString _pass = "");
        QString toConnStr(const QString &_dbPrefix="", bool _noSchema=false);
    };

    struct stuModuleMethod {
        intfPureModule* Module;
        QMetaMethod Method;

        stuModuleMethod(intfPureModule* _module, const QMetaMethod& _method) :
            Module(_module),
            Method(_method)
        { ; }
    };

    typedef QList<stuModuleMethod> ModuleMethods_t;

public:
    intfPureModule(
        const QString& _moduleName,
        Targoman::Common::Configuration::intfModule *_parent = nullptr
    );
//    virtual ~intfPureModule(); //= default;

    virtual QString parentModuleName() const /*{ return QString(); }; //*/= 0;
    virtual stuDBInfo requiredDB() const { return {}; }

    virtual bool init() { return true; }
    virtual void setInstancePointer() { ; };

    virtual QList<DBM::clsORMField> filterItems(qhttp::THttpMethod _method = qhttp::EHTTP_ACL) {
        Targoman::API::DBM::clsTable* PTHIS = dynamic_cast<Targoman::API::DBM::clsTable*>(this);
        if (PTHIS == nullptr)
            return {};

        PTHIS->prepareFiltersList();
        return PTHIS->filterItems(_method);
    }
    virtual void updateFilterParamType(const QString& _fieldTypeName, QMetaType::Type _typeID) {
        Targoman::API::DBM::clsTable* PTHIS = dynamic_cast<Targoman::API::DBM::clsTable*>(this);
        if (PTHIS == nullptr)
            return;

        PTHIS->prepareFiltersList();
        PTHIS->updateFilterParamType(_fieldTypeName, _typeID);
    }

    virtual ModuleMethods_t listOfMethods() = 0;

//    void addResponseHeaderNameToExpose(const QString &_header);
//signals:
//    void addResponseHeader(const QString &_header, const QString &_value, bool _multiValue = false);

protected:
    ModuleMethods_t Methods;
    QString ModuleName;
};

} //namespace Targoman::API::API

Q_DECLARE_INTERFACE(Targoman::API::API::intfPureModule, INTFPUREMODULE_IID)

//QString moduleBaseName() { return QStringLiteral(TARGOMAN_M2STR(_name)); }

#define TARGOMAN_DEFINE_API_MODULE(_name) \
public: \
    QString parentModuleName() const final { return QString(); } \
    QString moduleBaseName() { return this->ModuleName; }  \
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
    _name(); \
    virtual void setInstancePointer() { _name::InstancePointer = this; } \
    static _name* instance() { return _name::InstancePointer; } \
protected: \
    static _name* InstancePointer;

#define TARGOMAN_IMPL_API_MODULE(_name) \
    _name* _name::InstancePointer;

#define TARGOMAN_DEFINE_API_SUBMODULE_WO_CTOR(_module, _name) \
public: \
    QString parentModuleName() const final { return TARGOMAN_M2STR(_module); } \
    QString moduleBaseName() { return QStringLiteral(TARGOMAN_M2STR(TARGOMAN_CAT_BY_COLON(_module, _name))); } \
    QString moduleFullName() { return Targoman::Common::demangle(typeid(_name).name()); } \
    ModuleMethods_t listOfMethods() final { \
        throw Targoman::Common::exTargomanNotImplemented("listOfMethods must not be called on submodules"); \
    } \
    static _name& instance() { static _name* Instance = nullptr; return *(Q_LIKELY(Instance) ? Instance : (Instance = new _name)); } \
private: \
    TAPI_DISABLE_COPY(_name)

#define TARGOMAN_DEFINE_API_SUBMODULE(_module, _name) \
    TARGOMAN_DEFINE_API_SUBMODULE_WO_CTOR(_module, _name) \
private: \
    _name();

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
