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
 * @author S.Mehran M.Ziabary <ziabary@targoman.com>
 * @author Kambiz Zandi <kambizzandi@gmail.com>
 */

#include "clsAPIObject.h"
#include "APICache.hpp"

namespace Targoman::API::Server {

#define USE_ARG_AT(_i) \
    InvokableMethod.parameterType(_i) < TAPI_BASE_USER_DEFINED_TYPEID ? \
    gOrderedMetaTypeInfo.at(InvokableMethod.parameterType(_i))->makeGenericArgument(_arguments.at(_i), this->ParamNames.at(_i), &ArgStorage[_i]) : \
    gUserDefinedTypesInfo.at(InvokableMethod.parameterType(_i) - TAPI_BASE_USER_DEFINED_TYPEID)->makeGenericArgument(_arguments.at(_i), this->ParamNames.at(_i), &ArgStorage[_i]) \

#define CLEAN_ARG_AT(_i) \
    InvokableMethod.parameterType(_i) < TAPI_BASE_USER_DEFINED_TYPEID ? \
    gOrderedMetaTypeInfo.at(InvokableMethod.parameterType(_i))->cleanup(ArgStorage[_i]) : \
    gUserDefinedTypesInfo.at(InvokableMethod.parameterType(_i) - TAPI_BASE_USER_DEFINED_TYPEID)->cleanup(ArgStorage[_i])

clsAPIObject::clsAPIObject(intfPureModule* _module,
                           QMetaMethodExtended _method,
                           bool _async,
                           qint32 _cache4Internal,
                           qint32 _cache4Central,
                           qint32 _ttl,
                           bool _hasExtraMethodName) :
    QObject(_module),
    BaseMethod(_method),
    IsAsync(_async),
    Cache4Secs(_cache4Internal),
    Cache4SecsCentral(_cache4Central),
    TTL(_ttl ? _ttl : ServerConfigs::APICallTimeout.value()),
    RequiredParamsCount(static_cast<quint8>(_method.parameterCount())),
    HasExtraMethodName(_hasExtraMethodName),
    Parent(_module)
{
    quint8 i = 0;
    foreach(const QByteArray& ParamName, _method.parameterNames()){
        this->ParamNames.append(ParamName.startsWith('_') ? ParamName.mid(1) : ParamName);
        this->ParamTypes.append(QMetaType::typeName(_method.parameterType(i)));
        ++i;
    }
}

clsAPIObject::~clsAPIObject()
{}

intfAPIArgManipulator* clsAPIObject::argSpecs(quint8 _paramIndex) const {
    if (this->BaseMethod.parameterType(_paramIndex) < TAPI_BASE_USER_DEFINED_TYPEID)
        return gOrderedMetaTypeInfo.at(this->BaseMethod.parameterType(_paramIndex));
    else
        return gUserDefinedTypesInfo.at(this->BaseMethod.parameterType(_paramIndex) - TAPI_BASE_USER_DEFINED_TYPEID);
}

QVariant clsAPIObject::invoke(
        bool _isUpdateMethod,
        const QStringList& _args,
        /*OUT*/ QVariantMap &_responseHeaders,
        QList<QPair<QString, QString>> _bodyArgs,
        qhttp::THeaderHash _headers,
        qhttp::THeaderHash _cookies,
        QJsonObject _jwt,
        QString _remoteIP,
        QString _extraAPIPath
    ) const
{
    Q_ASSERT_X(this->parent(), "parent module", "Parent module not found to invoke method");

    int ExtraArgCount = 0;
    if(this->ParamTypes.contains(PARAM_COOKIES))
        ExtraArgCount++;
    if(this->ParamTypes.contains(PARAM_HEADERS))
        ExtraArgCount++;
    if(this->ParamTypes.contains(PARAM_JWT))
        ExtraArgCount++;
    if(this->ParamTypes.contains(PARAM_REMOTE_IP))
        ExtraArgCount++;
    if(this->ParamTypes.contains(PARAM_PKSBYPATH))
        ExtraArgCount++;
    if(this->ParamTypes.contains(PARAM_ORMFIELDS))
        ExtraArgCount++;

    if(_args.size() + _bodyArgs.size() + ExtraArgCount < this->RequiredParamsCount)
        throw exHTTPBadRequest("Not enough arguments");

    QVariantList Arguments;

    qint8 FirstArgumentWithValue = -1;
    qint8 LastArgumentWithValue = -1;

    for(quint8 i=0; i< this->ParamNames.count(); ++i ){
        bool ParamNotFound = true;
        QVariant ArgumentValue;

        static auto parseArgValue = [ArgumentValue](const QString& _paramName, QString _value) -> QVariant {
            _value = _value.trimmed();
            if ((_value.startsWith('[') && _value.endsWith(']')) ||
               (_value.startsWith('{') && _value.endsWith('}'))){
                QJsonParseError Error;
                QJsonDocument JSON = QJsonDocument::fromJson(_value.toUtf8(), &Error);
                if(JSON.isNull())
                    throw exHTTPBadRequest(QString("Invalid value for %1: %2").arg(_paramName).arg(Error.errorString()));
                return JSON.toVariant();
            }else{
                return _value;
            }
        };

        if(this->ParamTypes.at(i) == PARAM_COOKIES){
            ParamNotFound = false;
            ArgumentValue = _cookies.toVariant();
        }

        if(ParamNotFound && this->ParamTypes.at(i) == PARAM_HEADERS){
            ParamNotFound = false;
            ArgumentValue = _headers.toVariant();
        }

        if(ParamNotFound && this->ParamTypes.at(i) == PARAM_JWT){
            ParamNotFound = false;
            ArgumentValue = _jwt;
        }

        if(ParamNotFound && this->ParamTypes.at(i) == PARAM_REMOTE_IP){
            ParamNotFound = false;
            ArgumentValue = _remoteIP;
        }

        if(ParamNotFound && this->ParamTypes.at(i) == PARAM_PKSBYPATH){
            ParamNotFound = false;
            ArgumentValue = _extraAPIPath;
        }

        if(ParamNotFound && this->ParamTypes.at(i) == PARAM_ORMFIELDS){
            ParamNotFound = false;
            TAPI::ORMFields_t ORMFields;
            foreach (const QString& Arg, _args)
                ORMFields.insert(Arg.mid(0,Arg.indexOf('=')), parseArgValue(Arg.mid(0,Arg.indexOf('=')), QUrl::fromPercentEncoding(Arg.mid(Arg.indexOf('=') + 1).toUtf8())));

            foreach (auto BodyArg, _bodyArgs)
                ORMFields.insert(BodyArg.first, parseArgValue(BodyArg.first, BodyArg.second));
            ArgumentValue = ORMFields;
        }

        if(ParamNotFound)
            foreach (const QString& Arg, _args){
                if(Arg.startsWith(this->ParamNames.at(i)+ '=')){
                    ParamNotFound = false;
                    ArgumentValue = parseArgValue(this->ParamNames.at(i), QUrl::fromPercentEncoding(Arg.mid(Arg.indexOf('=') + 1).toUtf8()));
                    break;
                }
            }

        if (ParamNotFound)
            foreach (auto BodyArg, _bodyArgs) {
                bool ConversionResult = true;
                if (BodyArg.first == this->ParamNames.at(i) || (BodyArg.first.toInt(&ConversionResult) == i && ConversionResult)) {
                    ParamNotFound = false;
                    ArgumentValue = parseArgValue(this->ParamNames.at(i), BodyArg.second);
                    break;
                }
            }

        if (ParamNotFound) {
            if(i < this->RequiredParamsCount)
                throw exHTTPBadRequest(QString("Required parameter <%1> not specified").arg(this->ParamNames.at(i).constData()));
            else if (_isUpdateMethod)
                Arguments.append(QVariant());
            else
                Arguments.append(this->defaultValue(i));
            continue;
        }
        else if(ArgumentValue.isValid() == false)
            throw exHTTPBadRequest(QString("Invalid value for %1: no conversion to QVariant defined").arg(this->ParamNames.at(i).constData()));

        if (FirstArgumentWithValue < 0)
            FirstArgumentWithValue = static_cast<qint8>(i);
        LastArgumentWithValue = static_cast<qint8>(i);

        if (this->BaseMethod.parameterType(i) >= TAPI_BASE_USER_DEFINED_TYPEID) {
            Q_ASSERT(this->BaseMethod.parameterType(i) - TAPI_BASE_USER_DEFINED_TYPEID < gUserDefinedTypesInfo.size());
            Q_ASSERT(gUserDefinedTypesInfo.at(this->BaseMethod.parameterType(i) - TAPI_BASE_USER_DEFINED_TYPEID) != nullptr);

            Arguments.push_back(ArgumentValue);
        }
        else {
            Q_ASSERT(this->BaseMethod.parameterType(i) < gOrderedMetaTypeInfo.size());
            Q_ASSERT(gOrderedMetaTypeInfo.at(this->BaseMethod.parameterType(i)) != nullptr);

            Arguments.push_back(ArgumentValue);
        }
    }

    if (FirstArgumentWithValue < 0)
        Arguments.clear();
    else if (LastArgumentWithValue < Arguments.size() - 1)
        Arguments = Arguments.mid(0, LastArgumentWithValue + 1);

    QString CacheKey;
    if (this->Cache4Secs != 0 || this->Cache4SecsCentral != 0)
        CacheKey = this->makeCacheKey(Arguments);

    if (this->Cache4Secs != 0) {
        QVariant CachedValue =  InternalCache::storedValue(CacheKey);
        if (CachedValue.isValid()) {
            gServerStats.APIInternalCacheStats[this->BaseMethod.name()].inc();

            QVariantMap Map = CachedValue.toMap();
            _responseHeaders = Map.value("headers").toMap();
            return Map.value("result");
        }
    }

    if (this->Cache4SecsCentral) {
        QVariant CachedValue =  CentralCache::storedValue(CacheKey);
        if(CachedValue.isValid()){
            gServerStats.APICentralCacheStats[this->BaseMethod.name()].inc();

            QVariantMap Map = CachedValue.toMap();
            _responseHeaders = Map.value("headers").toMap();
            return Map.value("result");
        }
    }

    intfAPIArgManipulator *APIMethod;

    if (this->BaseMethod.returnType() >= TAPI_BASE_USER_DEFINED_TYPEID) {
        Q_ASSERT(this->BaseMethod.returnType() - TAPI_BASE_USER_DEFINED_TYPEID < gUserDefinedTypesInfo.size());
        Q_ASSERT(gUserDefinedTypesInfo.at(this->BaseMethod.returnType() - TAPI_BASE_USER_DEFINED_TYPEID) != nullptr);

        APIMethod = gUserDefinedTypesInfo.at(this->BaseMethod.returnType() - TAPI_BASE_USER_DEFINED_TYPEID);
    }
    else {
        Q_ASSERT(this->BaseMethod.returnType() < gOrderedMetaTypeInfo.size());
        Q_ASSERT(gOrderedMetaTypeInfo.at(this->BaseMethod.returnType()) != nullptr);

        APIMethod = gOrderedMetaTypeInfo.at(this->BaseMethod.returnType());
    }

    QVariant Result = APIMethod->invokeMethod(
                          this,
                          Arguments,
                          _responseHeaders);

    QVariantMap ResultWithHeader = QVariantMap({
                                                   { "result", Result },
                                                   { "headers", _responseHeaders },
                                               });
    if (this->Cache4Secs != 0)
        InternalCache::setValue(CacheKey, ResultWithHeader, this->Cache4Secs);
    else if (this->Cache4SecsCentral != 0)
        CentralCache::setValue(CacheKey, ResultWithHeader, this->Cache4SecsCentral);

    gServerStats.APICallsStats[this->BaseMethod.name()].inc();
    return Result;
}

void clsAPIObject::invokeMethod(
        const QVariantList& _arguments,
        QGenericReturnArgument _returnArg,
        /*OUT*/ QVariantMap &_responseHeaders
    ) const
{
    bool InvocationResult = true;
    QMetaMethod InvokableMethod;

    if (_arguments.size() == this->ParamNames.size())
        InvokableMethod = this->BaseMethod;
    else
        InvokableMethod = this->LessArgumentMethods.at(this->ParamNames.size() - _arguments.size() - 1);

#ifdef QT_DEBUG
    if (InvokableMethod.name().toStdString() == "apiPOSTfixtureCleanup")
    {
        int i = 0;
    }
#endif

    QVector<void*> ArgStorage(_arguments.size(), {});

    QMetaObject::Connection Conn_addResponseHeader;

    try
    {
        if (_arguments.size() > 10)
            throw exHTTPInternalServerError(QString("At most 10 method params are supported"));

        QGenericArgument Arguments[10];

        for (int i=0; i<10; ++i)
        {
            if (i < _arguments.size())
                Arguments[i] = USE_ARG_AT(i);
            else
                Arguments[i] = QGenericArgument();
        }

        QObject *parent = this->parent();
        intfPureModule* pureModule = dynamic_cast<intfPureModule*>(parent);
        Conn_addResponseHeader = QObject::connect(pureModule, &intfPureModule::addResponseHeader,
                         [&_responseHeaders](const QString &_header, const QString &_value) {
                            _responseHeaders.insert(_header, _value);
                         }
                        );

        InvocationResult = InvokableMethod.invoke(
            parent,
            this->IsAsync ? Qt::QueuedConnection : Qt::DirectConnection,
            _returnArg,
            Arguments[0],
            Arguments[1],
            Arguments[2],
            Arguments[3],
            Arguments[4],
            Arguments[5],
            Arguments[6],
            Arguments[7],
            Arguments[8],
            Arguments[9]
        );

        if (InvocationResult == false)
            throw exHTTPInternalServerError(QString("Unable to invoke method"));

        QObject::disconnect(Conn_addResponseHeader);

        for (int i=0; i<_arguments.size(); ++i)
            CLEAN_ARG_AT(i);
    }
    catch(...)
    {
        QObject::disconnect(Conn_addResponseHeader);

        for (int i=0; i<_arguments.size(); ++i)
            CLEAN_ARG_AT(i);
        throw;
    }
}

bool clsAPIObject::isPolymorphic(const QMetaMethodExtended& _method) {
       if(_method.parameterCount() == 0)
           return false;
       for(int i=0; i< qMin(_method.parameterCount(), this->BaseMethod.parameterCount()); ++i)
           if(this->BaseMethod.parameterType(i) != _method.parameterType(i))
               return true;
       return false;
}

void clsAPIObject::updateDefaultValues(const QMetaMethodExtended& _method) {
    if(_method.parameterNames().size() < this->RequiredParamsCount){
        this->RequiredParamsCount = static_cast<quint8>(_method.parameterNames().size());
        this->LessArgumentMethods.append(_method);
    }
}

} //namespace Targoman::API::Server
