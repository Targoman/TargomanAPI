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
#include "Interfaces/Server/APICallBoom.h"

namespace Targoman::API::Server {

//#define USE_ARG_AT(_i) \
//    InvokableMethod.parameterType(_i) < TAPI_BASE_USER_DEFINED_TYPEID ? \
//    gOrderedMetaTypeInfo.at(InvokableMethod.parameterType(_i))->makeGenericArgument(_arguments.at(_i), this->ParamNames.at(_i), &ArgStorage[_i]) : \
//    gUserDefinedTypesInfo.at(InvokableMethod.parameterType(_i) - TAPI_BASE_USER_DEFINED_TYPEID)->makeGenericArgument(_arguments.at(_i), this->ParamNames.at(_i), &ArgStorage[_i]) \

//#define CLEAN_ARG_AT(_i) \
//    InvokableMethod.parameterType(_i) < TAPI_BASE_USER_DEFINED_TYPEID ? \
//    gOrderedMetaTypeInfo.at(InvokableMethod.parameterType(_i))->cleanup(ArgStorage[_i]) : \
//    gUserDefinedTypesInfo.at(InvokableMethod.parameterType(_i) - TAPI_BASE_USER_DEFINED_TYPEID)->cleanup(ArgStorage[_i])

clsAPIObject::clsAPIObject(
    intfPureModule* _module,
    QMetaMethodExtended _method,
    bool _async,
    qint32 _cache4Internal,
    qint32 _cache4Central,
    qint32 _ttl,
    bool _hasExtraMethodName
) :
    QObject(_module),
    BaseMethod(_method),
    IsAsync(_async),
    Cache4Secs(_cache4Internal),
    Cache4SecsCentral(_cache4Central),
    TTL(_ttl ? _ttl : ServerConfigs::APICallTimeout.value()),
    RequiredParamsCount(static_cast<quint8>(_method.parameterCount())),
    HasExtraMethodName(_hasExtraMethodName),
    Parent(_module),
    RequiresJWT(false)
{
    QList<QByteArray> parameterTypes = _method.parameterTypes();
    quint8 i = 0;

    foreach (const QByteArray &ParamName, _method.parameterNames()) {
        QString ParameterTypeName = parameterTypes.at(i);

        /*if (ParameterTypeName.startsWith(APICALLBOOM_TYPE_JWT_DECL)) {
            this->ParamNames.append("JWT");
            this->ParamTypesName.append(PARAM_JWT);
            this->ParamTypesID.append(QMetaType::type(PARAM_JWT));
            this->BaseMethod.DefaultValues[0] = {};
        } else */if (ParameterTypeName.startsWith(APICALLBOOM_TYPE_BASE_STR)) { //APICALLBOOM_TYPE_NO_JWT_DECL)) {
            --this->RequiredParamsCount;
            this->BaseMethod.DefaultValues.removeAt(0);
            if (ParameterTypeName.startsWith(APICALLBOOM_TYPE_JWT_DECL_STR))
                this->RequiresJWT = true;
        } else {
            QByteArray ParamNameNoUnderScore = (ParamName.startsWith('_') ? ParamName.mid(1) : ParamName);
            this->ParamNames.append(ParamNameNoUnderScore);
            this->ParamTypesName.append(QMetaType::typeName(_method.parameterType(i)));
            this->ParamTypesID.append(_method.parameterType(i));

//            if (/*this->BaseMethod*/_method.DefaultValues.at(i).isValid())
//                --this->RequiredParamsCount;
        }

        ++i;
    }
}

clsAPIObject::~clsAPIObject() { ; }

intfAPIArgManipulator* clsAPIObject::argSpecs(quint8 _paramIndex) const {
    if (/*this->BaseMethod.parameterType*/this->ParamTypesID.at(_paramIndex) < TAPI_BASE_USER_DEFINED_TYPEID)
        return gOrderedMetaTypeInfo.at(/*this->BaseMethod.parameterType*/this->ParamTypesID.at(_paramIndex));
    else
        return gUserDefinedTypesInfo.at(/*this->BaseMethod.parameterType*/this->ParamTypesID.at(_paramIndex) - TAPI_BASE_USER_DEFINED_TYPEID);
}

QVariant clsAPIObject::invoke(
    INTFAPICALLBOOM_IMPL *_APICALLBOOM,
    bool _isUpdateMethod,
    const QStringList& _args,
//    /*OUT*/ QVariantMap &_responseHeaders,
    QList<QPair<QString, QString>> _bodyArgs,
//    qhttp::THeaderHash _headers,
//    qhttp::THeaderHash _cookies,
//    QJsonObject _jwt,
//    QString _remoteIP,
    QString _extraAPIPath
) const {
    Q_ASSERT_X(this->parent(), "parent module", "Parent module not found to invoke method");

    int ExtraArgCount = 0;
//    if (this->ParamTypesName.contains(PARAM_COOKIES))
//        ExtraArgCount++;
//    if (this->ParamTypesName.contains(PARAM_HEADERS))
//        ExtraArgCount++;
//    if (this->ParamTypesName.contains(PARAM_JWT))
//        ExtraArgCount++;
//    if (this->ParamTypesName.contains(PARAM_REMOTE_IP))
//        ExtraArgCount++;
    if (this->ParamTypesName.contains(PARAM_PKSBYPATH))
        ExtraArgCount++;
    if (this->ParamTypesName.contains(PARAM_ORMFIELDS))
        ExtraArgCount++;

    if (_args.size() + _bodyArgs.size() + ExtraArgCount < this->RequiredParamsCount)
        throw exHTTPBadRequest("Not enough arguments");

    QVariantList Arguments;

    qint8 FirstArgumentWithValue = -1;
    qint8 LastArgumentWithValue = -1;

    for (quint8 i=0; i< this->ParamNames.count(); ++i) {
        bool ParamNotFound = true;
        QVariant ArgumentValue;

        static auto parseArgValue = [ArgumentValue](const QString& _paramName, QString _value) -> QVariant {
            _value = _value.trimmed();
            if ((_value.startsWith('[') && _value.endsWith(']'))
                    || (_value.startsWith('{') && _value.endsWith('}'))) {
                QJsonParseError Error;
                QJsonDocument JSON = QJsonDocument::fromJson(_value.toUtf8(), &Error);
                if (JSON.isNull())
                    throw exHTTPBadRequest(QString("Invalid value for %1: %2").arg(_paramName).arg(Error.errorString()));
                return JSON.toVariant();
            } else {
                return _value;
            }
        };

//        if (this->ParamTypesName.at(i) == PARAM_COOKIES) {
//            ParamNotFound = false;
//            ArgumentValue = _cookies.toVariant();
//        }

//        if (ParamNotFound && this->ParamTypesName.at(i) == PARAM_HEADERS) {
//            ParamNotFound = false;
//            ArgumentValue = _headers.toVariant();
//        }

//        if (ParamNotFound && this->ParamTypesName.at(i) == PARAM_JWT) {
//            ParamNotFound = false;
//            ArgumentValue = _jwt;
//        }

//        if (ParamNotFound && this->ParamTypesName.at(i) == PARAM_REMOTE_IP) {
//            ParamNotFound = false;
//            ArgumentValue = _remoteIP;
//        }

        if (ParamNotFound && this->ParamTypesName.at(i) == PARAM_PKSBYPATH) {
            ParamNotFound = false;
            ArgumentValue = _extraAPIPath;
        }

        if (ParamNotFound && this->ParamTypesName.at(i) == PARAM_ORMFIELDS) {
            ParamNotFound = false;
            TAPI::ORMFields_t ORMFields;

            foreach (const QString& Arg, _args)
                ORMFields.insert(Arg.mid(0,Arg.indexOf('=')), parseArgValue(Arg.mid(0,Arg.indexOf('=')), QUrl::fromPercentEncoding(Arg.mid(Arg.indexOf('=') + 1).toUtf8())));

            foreach (auto BodyArg, _bodyArgs)
                ORMFields.insert(BodyArg.first, parseArgValue(BodyArg.first, BodyArg.second));

            ArgumentValue = ORMFields;
        }

        if (ParamNotFound)
            foreach (const QString& Arg, _args) {
                if (Arg.startsWith(this->ParamNames.at(i) + '=')) {
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
            if (i < this->RequiredParamsCount)
                throw exHTTPBadRequest(QString("Required parameter <%1> not specified").arg(this->ParamNames.at(i).constData()));
            else if (_isUpdateMethod)
                Arguments.append(QVariant());
            else
                Arguments.append(this->defaultValue(i));
            continue;
        } else if (ArgumentValue.isValid() == false)
            throw exHTTPBadRequest(QString("Invalid value for %1: no conversion to QVariant defined").arg(this->ParamNames.at(i).constData()));

        if (FirstArgumentWithValue < 0)
            FirstArgumentWithValue = static_cast<qint8>(i);
        LastArgumentWithValue = static_cast<qint8>(i);

        if (this->/*BaseMethod.parameterType*/ParamTypesID.at(i) >= TAPI_BASE_USER_DEFINED_TYPEID) {
            Q_ASSERT(this->/*BaseMethod.parameterType*/ParamTypesID.at(i) - TAPI_BASE_USER_DEFINED_TYPEID < gUserDefinedTypesInfo.size());
            Q_ASSERT(gUserDefinedTypesInfo.at(this->/*BaseMethod.parameterType*/ParamTypesID.at(i) - TAPI_BASE_USER_DEFINED_TYPEID) != nullptr);

            Arguments.push_back(ArgumentValue);
        } else {
            Q_ASSERT(this->/*BaseMethod.parameterType*/ParamTypesID.at(i) < gOrderedMetaTypeInfo.size());
            Q_ASSERT(gOrderedMetaTypeInfo.at(this->/*BaseMethod.parameterType*/ParamTypesID.at(i)) != nullptr);

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
            _APICALLBOOM->setResponseHeaders(Map.value("headers").toMap());
            return Map.value("result");
        }
    }

    if (this->Cache4SecsCentral) {
        QVariant CachedValue =  CentralCache::storedValue(CacheKey);
        if (CachedValue.isValid()) {
            gServerStats.APICentralCacheStats[this->BaseMethod.name()].inc();

            QVariantMap Map = CachedValue.toMap();
            _APICALLBOOM->setResponseHeaders(Map.value("headers").toMap());
            return Map.value("result");
        }
    }

    intfAPIArgManipulator *APIMethod;

    if (this->BaseMethod.returnType() >= TAPI_BASE_USER_DEFINED_TYPEID) {
        Q_ASSERT(this->BaseMethod.returnType() - TAPI_BASE_USER_DEFINED_TYPEID < gUserDefinedTypesInfo.size());
        Q_ASSERT(gUserDefinedTypesInfo.at(this->BaseMethod.returnType() - TAPI_BASE_USER_DEFINED_TYPEID) != nullptr);

        APIMethod = gUserDefinedTypesInfo.at(this->BaseMethod.returnType() - TAPI_BASE_USER_DEFINED_TYPEID);
    } else {
        Q_ASSERT(this->BaseMethod.returnType() < gOrderedMetaTypeInfo.size());
        Q_ASSERT(gOrderedMetaTypeInfo.at(this->BaseMethod.returnType()) != nullptr);

        APIMethod = gOrderedMetaTypeInfo.at(this->BaseMethod.returnType());
    }

    QVariant Result = APIMethod->invokeMethod(
                          this,
                          _APICALLBOOM,
                          Arguments
//                          _responseHeaders
                          );

    QVariantMap ResultWithHeader = QVariantMap({
                                                   { "result", Result },
                                                   { "headers", _APICALLBOOM->getResponseHeaders() },
                                               });
    if (this->Cache4Secs != 0)
        InternalCache::setValue(CacheKey, ResultWithHeader, this->Cache4Secs);
    else if (this->Cache4SecsCentral != 0)
        CentralCache::setValue(CacheKey, ResultWithHeader, this->Cache4SecsCentral);

    gServerStats.APICallsStats[this->BaseMethod.name()].inc();

    return Result;
}

void clsAPIObject::invokeMethod(
    INTFAPICALLBOOM_IMPL *_APICALLBOOM,
    const QVariantList& _arguments,
    QGenericReturnArgument _returnArg
//    /*OUT*/ QVariantMap &_responseHeaders
) const {
    if (_arguments.size() > 9)
        throw exHTTPInternalServerError(QString("At most 9 method params are supported"));

    bool InvocationResult = true;
    QMetaMethod InvokableMethod;

    if (_arguments.size() == this->ParamNames.size())
        InvokableMethod = this->BaseMethod;
    else
        InvokableMethod = this->LessArgumentMethods.at(this->ParamNames.size() - _arguments.size() - 1);

    QVector<void*> ArgStorage(_arguments.size(), {});

#ifdef QT_DEBUG
    if (this->BaseMethod.name() == "apiPOSTclaimOfflinePayment") {
        int ii; ii = 0;
    }
#endif

    auto useArgAt = [=, &_arguments, &ArgStorage](int _i) -> QGenericArgument {
        Targoman::API::Common::intfAPIArgManipulator* ArgMan = (this->ParamTypesID.at(_i) < TAPI_BASE_USER_DEFINED_TYPEID
            ? gOrderedMetaTypeInfo.at(this->ParamTypesID.at(_i))
            : gUserDefinedTypesInfo.at(this->ParamTypesID.at(_i) - TAPI_BASE_USER_DEFINED_TYPEID)
        );
        return ArgMan->makeGenericArgument(_arguments.at(_i), this->ParamNames.at(_i), &ArgStorage[_i]);
    };

    auto cleanArgAt = [=, &ArgStorage](int _i) {
        Targoman::API::Common::intfAPIArgManipulator* ArgMan = (this->ParamTypesID.at(_i) < TAPI_BASE_USER_DEFINED_TYPEID
            ? gOrderedMetaTypeInfo.at(this->ParamTypesID.at(_i))
            : gUserDefinedTypesInfo.at(this->ParamTypesID.at(_i) - TAPI_BASE_USER_DEFINED_TYPEID)
        );
        ArgMan->cleanup(ArgStorage[_i]);
    };

    try {
        QGenericArgument _APICALLBOOM_ARG("_APICALLBOOM", _APICALLBOOM);

        QGenericArgument Arguments[9];

        for (int i=0; i<9; ++i) {
            if (i < _arguments.size())
                Arguments[i] = useArgAt(i); //USE_ARG_AT(i);
            else
                Arguments[i] = QGenericArgument();
        }

        QObject *parent = this->parent();
//        intfPureModule* pureModule = dynamic_cast<intfPureModule*>(parent);

        InvocationResult = InvokableMethod.invoke(
            parent,
            this->IsAsync ? Qt::QueuedConnection : Qt::DirectConnection,
            _returnArg,
            _APICALLBOOM_ARG, //Q_ARG(intfAPICallBoom, *_APICALLBOOM),
            Arguments[0],
            Arguments[1],
            Arguments[2],
            Arguments[3],
            Arguments[4],
            Arguments[5],
            Arguments[6],
            Arguments[7],
            Arguments[8]
        );

        if (InvocationResult == false)
            throw exHTTPInternalServerError(QString("Unable to invoke method"));

        for (int i=0; i<_arguments.size(); ++i)
            cleanArgAt(i); //CLEAN_ARG_AT(i);
    } catch (...) {

        for (int i=0; i<_arguments.size(); ++i)
            cleanArgAt(i); //CLEAN_ARG_AT(i);

        throw;
    }
}

bool clsAPIObject::isPolymorphic(const QMetaMethodExtended& _method) {
    if (_method.parameterCount() == 0)
        return false;

    for (int i=0; i< qMin(_method.parameterCount(), this->BaseMethod.parameterCount()); ++i)
        if (this->BaseMethod.parameterType(i) != _method.parameterType(i))
            return true;

    return false;
}

void clsAPIObject::updateDefaultValues(const QMetaMethodExtended& _method) {
    ///-1: for _APICALLBOOM
    if ((_method.parameterNames().size() - 1) < this->RequiredParamsCount) {
        this->RequiredParamsCount = static_cast<quint8>(_method.parameterNames().size() - 1);

        QMetaMethodExtended Method = _method;
        Method.DefaultValues.removeAt(0); //for _APICALLBOOM
        this->LessArgumentMethods.append(Method);
    }
}

} //namespace Targoman::API::Server
