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

#include <algorithm>
#include <QMap>
#include <QMutex>
#include <QTextStream>

#include "RESTAPIRegistry.h"
#include "APICache.hpp"

#include "Interfaces/DBM/clsORMField.h"
#include "Interfaces/Helpers/RESTClientHelper.h"

using namespace std;

namespace Targoman::API {

using namespace TAPI;

/***********************************************************************************************/
#define DO_ON_TYPE_VALID(_complexity, _baseType)  tmplAPIArg<_baseType, _complexity, false, true>::instance(TARGOMAN_M2STR(_baseType))
#define DO_ON_TYPE_IGNORED(_complexity, _baseType) nullptr
#define DO_ON_TYPE_PROXY(_isIntegral, _baseType, ...) DO_ON_TYPE_SELECTOR(__VA_ARGS__, DO_ON_TYPE_IGNORED, DO_ON_TYPE_VALID)(_isIntegral, _baseType)
#define DO_ON_TYPE(_isIntegral, _typeName, _baseType) DO_ON_TYPE_PROXY(_isIntegral, _baseType, IGNORE_TYPE_##_typeName)

#define MAKE_INFO_FOR_VALID_INTEGRAL_METATYPE(_typeName, _id, _baseType) { _id, { DO_ON_TYPE(COMPLEXITY_Integral,  _typeName, _baseType) } },
#define MAKE_INFO_FOR_VALID_COMPLEX_METATYPE(_typeName, _id, _baseType)  { _id, { DO_ON_TYPE(COMPLEXITY_Complex, _typeName, _baseType) } },
#define MAKE_INVALID_METATYPE(_typeName, _id, _baseType) { _id, { nullptr } },
#define DIRECT_INFO_FOR_QT_PRIMITIVES(_type) {qMetaTypeId<_type>(), DO_ON_TYPE_VALID(COMPLEXITY_Integral, _type)},

const QMap<int, intfAPIArgManipulator*> MetaTypeInfoMap = {
    //QT_FOR_EACH_STATIC_PRIMITIVE_TYPE(MAKE_INFO_FOR_VALID_INTEGRAL_METATYPE)
    DIRECT_INFO_FOR_QT_PRIMITIVES(qint8)
    DIRECT_INFO_FOR_QT_PRIMITIVES(qint16)
    DIRECT_INFO_FOR_QT_PRIMITIVES(qint32)
    DIRECT_INFO_FOR_QT_PRIMITIVES(qint64)
    DIRECT_INFO_FOR_QT_PRIMITIVES(quint8)
    DIRECT_INFO_FOR_QT_PRIMITIVES(quint16)
    DIRECT_INFO_FOR_QT_PRIMITIVES(quint32)
    DIRECT_INFO_FOR_QT_PRIMITIVES(quint64)
    DIRECT_INFO_FOR_QT_PRIMITIVES(double)
    DIRECT_INFO_FOR_QT_PRIMITIVES(float)
    DIRECT_INFO_FOR_QT_PRIMITIVES(bool)
    QT_FOR_EACH_STATIC_PRIMITIVE_POINTER(MAKE_INVALID_METATYPE)
    QT_FOR_EACH_STATIC_CORE_CLASS(MAKE_INFO_FOR_VALID_COMPLEX_METATYPE)
    QT_FOR_EACH_STATIC_CORE_POINTER(MAKE_INVALID_METATYPE)
    QT_FOR_EACH_STATIC_CORE_TEMPLATE(MAKE_INFO_FOR_VALID_COMPLEX_METATYPE)
    QT_FOR_EACH_STATIC_GUI_CLASS(MAKE_INVALID_METATYPE)
    QT_FOR_EACH_STATIC_WIDGETS_CLASS(MAKE_INVALID_METATYPE)
};

#define DO_ON_TYPE_NULLABLE_VALID(_complexity, _baseType, _fromVariantLambda, _toORMValueLambda, _descriptionLambda) \
    template <> std::function<QVariant(_baseType _value)> tmplAPIArg<_baseType, _complexity, false, true>::toVariantLambda = nullptr; \
    template <> std::function<_baseType(QVariant _value, const QByteArray& _paramName)> tmplAPIArg<_baseType, _complexity, false, true>::fromVariantLambda = _fromVariantLambda; \
    template <> std::function<QString(const QList<DBM::clsORMField>& _allFields)> tmplAPIArg<_baseType, _complexity, false, true>::descriptionLambda = _descriptionLambda; \
    template <> std::function<QVariant(const QVariant& _val)> tmplAPIArg<_baseType, _complexity, false, true>::toORMValueLambda = _toORMValueLambda; \
    template <> std::function<QVariant(const QVariant& _val)> tmplAPIArg<_baseType, _complexity, false, true>::fromORMValueLambda = nullptr; \
    template <> std::function<QStringList()> tmplAPIArg<_baseType, _complexity, false, true>::optionsLambda = nullptr; \
    \
    template <> std::function<QVariant(NULLABLE_TYPE(_baseType) _value)> tmplAPIArg<NULLABLE_TYPE(_baseType), _complexity, true>::toVariantLambda = \
        [](NULLABLE_TYPE(_baseType) _value) {return NULLABLE_IS_NULL(_value) ? QVariant() : tmplAPIArg<_baseType, _complexity, false, true>::toVariant(*_value);}; \
    template <> std::function<NULLABLE_TYPE(_baseType)(QVariant _value, const QByteArray& _paramName)> tmplAPIArg<NULLABLE_TYPE(_baseType), _complexity, true>::fromVariantLambda = \
        [](const QVariant& _value, const QByteArray& _paramName) -> NULLABLE_TYPE(_baseType) { \
            if (!_value.isValid() || _value.isNull()) return NULLABLE_TYPE(_baseType)(); \
            NULLABLE_VAR(_baseType, Value); \
            Value = tmplAPIArg<_baseType, _complexity, false, true>::fromVariant(_value, _paramName); \
            return Value; \
        }; \
    template <> std::function<QVariant(const QVariant& _val)> tmplAPIArg<NULLABLE_TYPE(_baseType), _complexity, true>::toORMValueLambda = nullptr; \
    template <> std::function<QVariant(const QVariant& _val)> tmplAPIArg<NULLABLE_TYPE(_baseType), _complexity, true>::fromORMValueLambda = nullptr; \
    template <> std::function<QStringList()> tmplAPIArg<NULLABLE_TYPE(_baseType), _complexity, true>::optionsLambda = nullptr; \
    template <> std::function<QString(const QList<DBM::clsORMField>& _allFields)> tmplAPIArg<NULLABLE_TYPE(_baseType), _complexity, true>::descriptionLambda = nullptr; \
    static tmplAPIArg<NULLABLE_TYPE(_baseType), _complexity, true>* Dangling_QSP_##_baseType = tmplAPIArg<NULLABLE_TYPE(_baseType), _complexity, true>::instance(NULLABLE_M2STR(_baseType)); \
//NULLABLE_TYPE(_baseType) Value(new _baseType); -> NULLABLE_VAR(_baseType, Value);


#define DO_ON_TYPE_NULLABLE_IGNORED(...)
#define DO_ON_TYPE_NULLABLE_PROXY(_complexity, _baseType, ...) DO_ON_TYPE_SELECTOR(__VA_ARGS__, DO_ON_TYPE_NULLABLE_IGNORED, DO_ON_TYPE_NULLABLE_VALID)(_complexity, _baseType, nullptr, nullptr, nullptr)
#define DO_ON_NULLABLE_TYPE(_complexity, _typeName, _baseType) DO_ON_TYPE_NULLABLE_PROXY(_complexity, _baseType, IGNORE_TYPE_##_typeName)

#define MAKE_INFO_FOR_VALID_NULLABLE_INTEGRAL_METATYPE(_typeName, _id, _baseType) DO_ON_NULLABLE_TYPE(COMPLEXITY_Integral, _typeName, _baseType)
#define MAKE_INFO_FOR_VALID_NULLABLE_COMPLEX_METATYPE(_typeName, _id, _baseType) DO_ON_NULLABLE_TYPE(COMPLEXITY_Complex, _typeName, _baseType)
#define MAKE_INVALID_NULLABLE_METATYPE(_typeName, _id, _baseType)

QT_FOR_EACH_STATIC_PRIMITIVE_POINTER(MAKE_INVALID_NULLABLE_METATYPE)
QT_FOR_EACH_STATIC_CORE_CLASS(MAKE_INFO_FOR_VALID_NULLABLE_COMPLEX_METATYPE)
QT_FOR_EACH_STATIC_CORE_POINTER(MAKE_INVALID_NULLABLE_METATYPE)
QT_FOR_EACH_STATIC_CORE_TEMPLATE(MAKE_INFO_FOR_VALID_NULLABLE_COMPLEX_METATYPE)
QT_FOR_EACH_STATIC_GUI_CLASS(MAKE_INVALID_NULLABLE_METATYPE)
QT_FOR_EACH_STATIC_WIDGETS_CLASS(MAKE_INVALID_NULLABLE_METATYPE)
DO_ON_TYPE_NULLABLE_VALID(COMPLEXITY_Integral, qint8,   nullptr, nullptr, nullptr)
DO_ON_TYPE_NULLABLE_VALID(COMPLEXITY_Integral, qint16,  nullptr, nullptr, nullptr)
DO_ON_TYPE_NULLABLE_VALID(COMPLEXITY_Integral, qint32,  nullptr, nullptr, nullptr)
DO_ON_TYPE_NULLABLE_VALID(COMPLEXITY_Integral, qint64,  nullptr, nullptr, nullptr)
DO_ON_TYPE_NULLABLE_VALID(COMPLEXITY_Integral, quint8,  nullptr, nullptr, nullptr)
DO_ON_TYPE_NULLABLE_VALID(COMPLEXITY_Integral, quint16, nullptr, nullptr, nullptr)
DO_ON_TYPE_NULLABLE_VALID(COMPLEXITY_Integral, quint32, nullptr, nullptr, nullptr)
DO_ON_TYPE_NULLABLE_VALID(COMPLEXITY_Integral, quint64, nullptr, nullptr, nullptr)
DO_ON_TYPE_NULLABLE_VALID(COMPLEXITY_Integral, float,   nullptr, nullptr, nullptr)
DO_ON_TYPE_NULLABLE_VALID(COMPLEXITY_Integral, double,  nullptr, nullptr, nullptr)
DO_ON_TYPE_NULLABLE_VALID(COMPLEXITY_Integral, bool,
                          [](const QVariant& _value, const QByteArray& _paramName) -> bool {
                              if (_value.toString() == "false" || _value.toString() == "0") return false;
                              if (_value.toString() == "true" || _value.toString() == "1") return true;
                                  throw exHTTPBadRequest(_paramName + " is not a valid bool");
                          },
                          [](const QVariant& _val) -> QVariant {return !(_val == "false" || _val == "0");},
                          [](const QList<DBM::clsORMField>&) -> QString {return "valid bool as 1|true|0|false";}
)

namespace Server {

//using namespace DBM;

struct stuNullableQtType {
    std::function<void()>  registerInMetaTypes;
    intfAPIArgManipulator* ArgManipulator;

    stuNullableQtType(const std::function<void()>& _registrar, intfAPIArgManipulator* _manipulator) :
        registerInMetaTypes(_registrar),
        ArgManipulator(_manipulator)
    { ; }
};

/***********************************************************************************************/
void RESTAPIRegistry::registerMetaTypeInfoMap() {
    if (Q_UNLIKELY(gOrderedMetaTypeInfo.isEmpty())) {
        gOrderedMetaTypeInfo.reserve(MetaTypeInfoMap.lastKey());

        for (auto MetaTypeInfoMapIter = MetaTypeInfoMap.begin();
                MetaTypeInfoMapIter != MetaTypeInfoMap.end();
                ++MetaTypeInfoMapIter) {
            int Gap = MetaTypeInfoMapIter.key() - gOrderedMetaTypeInfo.size();
            for (int i = 0; i< Gap; ++i)
                gOrderedMetaTypeInfo.append(nullptr);
            gOrderedMetaTypeInfo.append(MetaTypeInfoMapIter.value());
        }
    }
}

void RESTAPIRegistry::registerRESTAPI(
    intfPureModule* _module,
    const QMetaMethod& _method
) {
    if ((_method.name().startsWith("api") == false
            && _method.name().startsWith("asyncApi") == false)
            || _method.typeName() == nullptr)
        return;

    foreach (auto Item, _module->filterItems())
        Item.registerTypeIfNotRegisterd(_module);

    RESTAPIRegistry::registerMetaTypeInfoMap();

    try {
        RESTAPIRegistry::validateMethodInputAndOutput(_method);

        QString MethodName = _method.name().constData();
        MethodName = MethodName.mid(MethodName.indexOf("api", 0, Qt::CaseInsensitive) + 3);

        QVariantMap MethodConf;
        QString MethodAlias = MethodName, MethodSignature, MethodDoc;
        bool FoundSignature = false, FoundDoc = false;

        for (int i=0; i<_module->metaObject()->methodCount(); ++i) {
            if (_module->metaObject()->method(i).name() == "signOf" + MethodName) {
                _module->metaObject()->method(i).invoke(_module,
                                                        Qt::DirectConnection,
                                                        Q_RETURN_ARG(QString, MethodSignature)
                                                        );
                FoundSignature = true;
//                break;
            }

            if (_module->metaObject()->method(i).name() == "docOf" + MethodName) {
                _module->metaObject()->method(i).invoke(_module,
                                                        Qt::DirectConnection,
                                                        Q_RETURN_ARG(QString, MethodDoc)
                                                        );
                MethodDoc = MethodDoc.trimmed();
                FoundDoc = true;
//                break;
            }

            if (_module->metaObject()->method(i).name() == "confOf" + MethodName) {
                _module->metaObject()->method(i).invoke(_module,
                                                        Qt::DirectConnection,
                                                        Q_RETURN_ARG(QVariantMap, MethodConf)
                                                        );
            }
        }

        if (MethodConf.contains(EXRESTCONFIGKEY_ALIAS))
            MethodAlias = MethodConf[EXRESTCONFIGKEY_ALIAS].toString();

        bool HiddenMethod = MethodConf.contains(EXRESTCONFIGKEY_HIDDEN);

        if (!FoundSignature)
            throw exRESTRegistry("Seems that you have not use API macro to define your API: " + MethodName);

        if (!FoundDoc)
            throw exRESTRegistry("Seems that you have not use API macro to define your API: " + MethodName);

        constexpr char COMMA_SIGN[] = "$_COMMA_$";

        MethodSignature = MethodSignature.trimmed().replace("','", QString("'%1'").arg(COMMA_SIGN));
        bool DQuoteStarted = false, BackslashFound = false;
        QString CommaReplacedMethodSignature;

        for (int i=0; i< MethodSignature.size(); ++i) {
            if (DQuoteStarted) {
                if (MethodSignature.at(i).toLatin1() == ',')
                    CommaReplacedMethodSignature += COMMA_SIGN;
                else if (MethodSignature.at(i).toLatin1() == '\\') {
                    BackslashFound = true;
                } else if (MethodSignature.at(i).toLatin1() == '"') {
                    CommaReplacedMethodSignature += '"';
                    if (BackslashFound)
                        BackslashFound = false;
                    else
                        DQuoteStarted = false;
                } else
                    CommaReplacedMethodSignature += MethodSignature.at(i);
            } else if (MethodSignature.at(i).toLatin1() == '"') {
                CommaReplacedMethodSignature += '"';
                DQuoteStarted = true;
            } else
                CommaReplacedMethodSignature += MethodSignature.at(i);
        }

        QVariantList DefaultValues;
        bool ContainsFileInput = false;
        QList<QByteArray> parameterTypes = _method.parameterTypes();
        auto ParamsParts = CommaReplacedMethodSignature.split(',');

#ifdef QT_DEBUG
        QString MethodKey = "/" + _module->moduleBaseName().replace(":", "/") + '/' + _method.name();
        MethodKey = MethodKey.replace(QRegularExpression("//+"), "/");
        if (MethodKey == "/Account/OfflinePayments/apiGET") {
            int ii; ii = 0;
        }
#endif

        for (quint8 ParamIndex=0; ParamIndex<ParamsParts.length(); ParamIndex++) {
            QString Param = ParamsParts[ParamIndex];

            if (ParamIndex >= _method.parameterCount())
                break;

            QString ParameterTypeName = parameterTypes.at(ParamIndex);

            //check APICallBoom
            if (ParameterTypeName.startsWith(APICALLBOOM_TYPE_BASE_STR)) {
                DefaultValues.append(QVariant());
                continue;
            }

            if (_method.parameterType(ParamIndex) == qMetaTypeId<TAPI::stuFileInfo>()
                    || _method.parameterType(ParamIndex) == qMetaTypeId<TAPI::Files_t>())
                ContainsFileInput = true;

            intfAPIArgManipulator* ArgSpecs;
            if (_method.parameterType(ParamIndex) < TAPI_BASE_USER_DEFINED_TYPEID)
                ArgSpecs = gOrderedMetaTypeInfo.at(_method.parameterType(ParamIndex));
            else
                ArgSpecs = gUserDefinedTypesInfo.at(_method.parameterType(ParamIndex) - TAPI_BASE_USER_DEFINED_TYPEID);

            Q_ASSERT(ArgSpecs);

            if (Param.contains('=')) {
                QString Value = Param.split('=').last().replace(COMMA_SIGN, ",").trimmed();
                if (Value.startsWith("(")) Value = Value.mid(1).trimmed();
                if (Value.endsWith(")")) Value = Value.mid(0, Value.size() - 1).trimmed();
                if (Value.startsWith("'")) Value = Value.mid(1).trimmed();
                if (Value.endsWith("'")) Value = Value.mid(0, Value.size() - 1).trimmed();
                if (Value.startsWith("\"")) Value = Value.mid(1).trimmed();
                if (Value.endsWith("\"")) Value = Value.mid(0, Value.size() - 1).trimmed();
                Value.replace("\\\"", "\"");
                Value.replace("\\'", "'");

                if (Value == "{}" || Value.contains("()"))
                    DefaultValues.append(ArgSpecs->defaultVariant());
                else if (ArgSpecs->complexity() == COMPLEXITY_Enum)
                    DefaultValues.append(Value.mid(Value.indexOf("::") + 2));
                else
                    DefaultValues.append(Value);
            } else if (ArgSpecs->isNullable())
                throw exRESTRegistry("Nullable parameter: <"+_method.parameterNames().value(ParamIndex)+"> on method: <" + MethodName + "> must have default value" );
            else
                DefaultValues.append(QVariant());
        }

        QMetaMethodExtended Method(_method, DefaultValues, MethodDoc, HiddenMethod);

        auto makeMethodName = [MethodAlias](int _start = 1) -> QString {
            if (MethodAlias.size() >= _start)
                return MethodAlias.at(_start-1).toLower() + MethodAlias.mid(_start);
            return "";
        };

        if (MethodName.startsWith("GET")) {
            if (ContainsFileInput)
                throw exRESTRegistry("file input can not be used with GET method: " + MethodName);
            RESTAPIRegistry::addRegistryEntry(RESTAPIRegistry::Registry, _module, Method, "GET", makeMethodName(sizeof("GET")));
        } else if (MethodName.startsWith("POST"))
            RESTAPIRegistry::addRegistryEntry(RESTAPIRegistry::Registry, _module, Method, "POST", makeMethodName(sizeof("POST")));
        else if (MethodName.startsWith("PUT"))
            RESTAPIRegistry::addRegistryEntry(RESTAPIRegistry::Registry, _module, Method, "PUT", makeMethodName(sizeof("PUT")));
        else if (MethodName.startsWith("CREATE"))
            RESTAPIRegistry::addRegistryEntry(RESTAPIRegistry::Registry, _module, Method, "PUT", makeMethodName(sizeof("CREATE")));
        else if (MethodName.startsWith("PATCH"))
            RESTAPIRegistry::addRegistryEntry(RESTAPIRegistry::Registry, _module, Method, "PATCH", makeMethodName(sizeof("PATCH")));
        else if (MethodName.startsWith("DELETE")) {
            if (ContainsFileInput)
                throw exRESTRegistry("file input can not be used with DELETE method: "+ MethodName);
            RESTAPIRegistry::addRegistryEntry(RESTAPIRegistry::Registry, _module, Method, "DELETE", makeMethodName(sizeof("DELETE")));
        } else if (MethodName.startsWith("UPDATE"))
            RESTAPIRegistry::addRegistryEntry(RESTAPIRegistry::Registry, _module, Method, "PATCH", makeMethodName(sizeof("UPDATE")));
        else if (MethodName.startsWith("WS")) {
#ifdef TARGOMAN_API_ENABLE_WEBSOCKET
            if (ContainsFileInput)
                throw exRESTRegistry("file input can not be used with WebSockets: "+ MethodName);
            RESTAPIRegistry::addRegistryEntry(RESTAPIRegistry::WSRegistry, _module, Method, "WS", makeMethodName(sizeof("WS")));
#else
            throw exRESTRegistry("Websockets are not enabled in this QRestServer please compile with websockets support");
#endif
        } else if (ContainsFileInput)
            throw exRESTRegistry("methods with file input must specifically contain HTTP method: " + MethodName);
        else {
            RESTAPIRegistry::addRegistryEntry(RESTAPIRegistry::Registry, _module, Method, "GET", makeMethodName());
            RESTAPIRegistry::addRegistryEntry(RESTAPIRegistry::Registry, _module, Method, "POST", makeMethodName());
        }
    } catch (Targoman::Common::exTargomanBase& ex) {
        TargomanError("Fatal Error: "<<ex.what());
        throw;
    }
}

QMap<QString, QString> RESTAPIRegistry::extractMethods(
    QHash<QString, clsAPIObject*>& _registry,
    const QString& _module,
    bool _showTypes,
    bool _prettifyTypes
) {
    static auto type2Str = [_prettifyTypes](int _typeID) {
        if (_prettifyTypes == false || _typeID > QMetaType::User)
            return QString(QMetaType::typeName(_typeID));

        return gOrderedMetaTypeInfo.at(_typeID)->PrettyTypeName;
    };

    QMap<QString, QString> Methods;
    foreach (const QString& Key, _registry.keys()) {
        QString Path = Key.split(" ").last();

        if (Path.startsWith(_module) == false)
            continue;

        clsAPIObject* APIObject = _registry.value(Key);
        QStringList Parameters;

        if (APIObject->requiresJWT())
            Parameters.append(QString(_showTypes ? "TAPI::JWT_t " : "") + "JWT");

        for (quint8 i=0; i<APIObject->/*BaseMethod.parameterCount()*/ParamTypesName.count(); ++i) {

            static auto defaultValue = [](clsAPIObject* _apiObject, quint8 i) -> QString {
                if (_apiObject->RequiredParamsCount > i)
                    return "";

                intfAPIArgManipulator* ArgManipulator = _apiObject->argSpecs(i);

                if (ArgManipulator) {
                    switch (ArgManipulator->complexity()) {
                        case COMPLEXITY_Number:
                        case COMPLEXITY_Boolean:
                        case COMPLEXITY_Integral:
                            return QString(" = %1").arg(_apiObject->defaultValue(i).toString());

                        case COMPLEXITY_String:
                        case COMPLEXITY_Complex:
                        case COMPLEXITY_Enum:
                            return QString(" = \"%1\"").arg(_apiObject->defaultValue(i).toString().replace("\"", "\\\""));

                        case COMPLEXITY_Object:
                            return "null"; ///TODO: update this

                        case COMPLEXITY_File:
                            return "";
                    }

                    return " = " + (ArgManipulator->isPrimitiveType()
                                    ? QString("%1")
                                    : (QString("\"%1\"")))
                                .arg(_apiObject->defaultValue(i).toString().replace("\"", "\\\""));
                }

                return "";
            };

            Parameters.append((_showTypes
                               ? type2Str(APIObject->/*BaseMethod.parameterType*/ParamTypesID.at(i)) + " "
                               : "")
                              + /*(APIObject->BaseMethod.parameterNames().at(i).startsWith('_')
                                 ? APIObject->BaseMethod.parameterNames().at(i).mid(1)
                                 : APIObject->BaseMethod.parameterNames().at(i))*/
                                APIObject->ParamNames.at(i)
                              + defaultValue(APIObject, i)
                              );
        }

        Methods.insert(Path + Key.split(" ").first(),
                       QString("%1(%2) %3 %4")
                       .arg(Key)
                       .arg(Parameters.join(", "))
                       .arg(_showTypes
                            ? QString("-> %1").arg(type2Str(APIObject->BaseMethod.returnType()))
                            : "")
                       .arg(APIObject->IsAsync
                            ? "[async]"
                            : "")
                       );
    }

    return Methods;
}

QString RESTAPIRegistry::isValidType(int _typeID, bool _validate4Input) {
    if (_typeID == 0 || _typeID == QMetaType::User) // || _typeID == QMetaType::User)
        return  "is not registered with Qt MetaTypes";

    if (_typeID < TAPI_BASE_USER_DEFINED_TYPEID && (_typeID >= gOrderedMetaTypeInfo.size() || gOrderedMetaTypeInfo.at(_typeID) == nullptr))
        return "is complex type and not supported";

    if (_typeID >= TAPI_BASE_USER_DEFINED_TYPEID) {
        if ((_typeID - TAPI_BASE_USER_DEFINED_TYPEID >= gUserDefinedTypesInfo.size() ||
            gUserDefinedTypesInfo.at(_typeID - TAPI_BASE_USER_DEFINED_TYPEID) == nullptr))
            return "is user defined but not registered";

        if (strcmp(gUserDefinedTypesInfo.at(_typeID - TAPI_BASE_USER_DEFINED_TYPEID)->RealTypeName, QMetaType::typeName(_typeID)))
            return QString("Seems that registration table is corrupted: %1:%2 <-> %3:%4").arg(
                        _typeID).arg(
                        gUserDefinedTypesInfo.at(_typeID - TAPI_BASE_USER_DEFINED_TYPEID)->RealTypeName).arg(
                        _typeID).arg(
                        QMetaType::typeName(_typeID));

        if (_validate4Input) {
            if (!gUserDefinedTypesInfo.at(_typeID - TAPI_BASE_USER_DEFINED_TYPEID)->hasFromVariantMethod())
                return "has no fromVariant lambda so can not be used as input";
        } else {
            if (!gUserDefinedTypesInfo.at(_typeID - TAPI_BASE_USER_DEFINED_TYPEID)->hasToVariantMethod())
                return "has no toVariant lambda so can not be used as output";
        }
    }
    return "";
}

void RESTAPIRegistry::validateMethodInputAndOutput(const QMetaMethod& _method) {
    if (_method.parameterCount() > 10)
        throw exRESTRegistry("Unable to register methods with more than 10 input args <"+_method.name()+">");

    QString ErrMessage;

    if ((ErrMessage = RESTAPIRegistry::isValidType(_method.returnType(), false)).size())
        throw exRESTRegistry(QString("Invalid return type(%1): %2").arg(_method.typeName()).arg(ErrMessage));

    ErrMessage.clear();

    QString FullMethodName = QString("%1::%2")
                             .arg(_method.enclosingMetaObject()->className())
                             .arg(_method.name().toStdString().c_str())
                             ;

    QList<QByteArray> parameterTypes = _method.parameterTypes();
    bool HasApiCallBoomParam = false;

    for (int i=0; i<_method.parameterCount(); ++i) {
        QString ParameterTypeName = parameterTypes.at(i);

        //check APICallBoom
        if (ParameterTypeName.startsWith(APICALLBOOM_TYPE_BASE_STR)) {
            if (HasApiCallBoomParam)
                throw exRESTRegistry(QString("Invalid duplicate APICALLBOOM parameter (%1) at %2 of %3")
                                     .arg(_method.parameterNames().at(i).constData())
                                     .arg(i)
                                     .arg(FullMethodName));

            if (i > 0)
                throw exRESTRegistry(QString("APICALLBOOM parameter must be first argument of %1")
                                     .arg(FullMethodName));

            HasApiCallBoomParam = true;

            continue;
        }

        if (ParameterTypeName == PARAM_JWT)
            throw exRESTRegistry(QString("%1: JWT in api method parameters list is prohibited. Use APICallBoom instead.").arg(FullMethodName));

        if (ParameterTypeName == PARAM_REMOTE_IP)
            throw exRESTRegistry(QString("%1: IP in api method parameters list is prohibited. Use APICallBoom instead.").arg(FullMethodName));

        if (ParameterTypeName == PARAM_COOKIES)
            throw exRESTRegistry(QString("%1: COOKIES in api method parameters list is prohibited. Use APICallBoom instead.").arg(FullMethodName));

        if (ParameterTypeName == PARAM_HEADERS)
            throw exRESTRegistry(QString("%1: HEADERS in api method parameters list is prohibited. Use APICallBoom instead.").arg(FullMethodName));

        //-- others
        if ((ErrMessage = RESTAPIRegistry::isValidType(_method.parameterType(i), true)).size())
            throw exRESTRegistry(QString("Invalid parameter (%1 %2) at %3: %4")
                                 .arg(ParameterTypeName.constData())
                                 .arg(_method.parameterNames().at(i).constData())
                                 .arg(FullMethodName)
                                 .arg(ErrMessage));

        for (int j=i+1; j<_method.parameterCount(); ++j) {
            if (_method.parameterNames().at(j) == _method.parameterNames().at(i))
                throw exRESTRegistry(QString("Invalid duplicate parameter (%1) at %2 and %3 of %4")
                                     .arg(_method.parameterNames()
                                     .at(i).constData())
                                     .arg(i)
                                     .arg(j)
                                     .arg(FullMethodName)
                                     );
        }
    }

    if (HasApiCallBoomParam == false)
        throw exRESTRegistry(QString("APICALLBOOM parameter not defined in %1").arg(FullMethodName));
}

constexpr char CACHE_INTERNAL[] = "CACHEABLE_";
constexpr char CACHE_CENTRAL[]  = "CENTRALCACHE_";
constexpr char API_TIMEOUT[]  = "CENTRALCACHE_";

void RESTAPIRegistry::addRegistryEntry(
    QHash<QString, clsAPIObject *>& _registry,
    intfPureModule* _module,
    const QMetaMethodExtended& _method,
    const QString& _httpMethod,
    const QString& _methodName
) {
    QString MethodKey = RESTAPIRegistry::makeRESTAPIKey(_httpMethod, "/" + _module->moduleBaseName().replace(":", "/") + '/' + _methodName);
    MethodKey = MethodKey.replace(QRegularExpression("//+"), "/");

    if (_registry.contains(MethodKey)) {
        if (RESTAPIRegistry::Registry.value(MethodKey)->isPolymorphic(_method))
            throw exRESTRegistry(QString("Polymorphism is not supported: %1").arg(_method.methodSignature().constData()));

        _registry.value(MethodKey)->updateDefaultValues(_method);
    } else {
        if (RESTAPIRegistry::getTagSeconds(_method, CACHE_INTERNAL) > 0
               && RESTAPIRegistry::getTagSeconds(_method, CACHE_CENTRAL) > 0)
            throw exRESTRegistry("Both internal and central cache can not be defined on an API");

        _registry.insert(MethodKey,
                         new clsAPIObject(_module,
                                          _method,
                                          QString(_method.name()).startsWith("async"),
                                          RESTAPIRegistry::getTagSeconds(_method, CACHE_INTERNAL),
                                          RESTAPIRegistry::getTagSeconds(_method, CACHE_CENTRAL),
                                          RESTAPIRegistry::getTagSeconds(_method, API_TIMEOUT),
                                          !_methodName.isEmpty()
                                          ));
    }
}

int RESTAPIRegistry::getTagSeconds(const QMetaMethod& _method, const char* _type) {
    if (_method.tag() == nullptr || _method.tag()[0] == '\0')
        return 0;
    QString Tag = _method.tag();
    if (Tag.startsWith(_type) && Tag.size () > 12) {
        Tag = Tag.mid(10);
        if (_type == CACHE_INTERNAL && Tag == "INF")
            return -1;
        char Type = Tag.rbegin()->toLatin1();
        int  Number = Tag.mid(0,Tag.size() -1).toUShort();
        switch (Type) {
        case 'S': return Number;
        case 'M': return Number * 60;
        case 'H': return Number * 3600;
        default:
            throw exRESTRegistry("Invalid tag numer or type defined for api: " + _method.methodSignature());
        }
    }
    return 0;
}

QStringList RESTAPIRegistry::registeredAPIs(const QString& _module, bool _showParams, bool _showTypes, bool _prettifyTypes) {
    if (_showParams == false) {
#ifdef TARGOMAN_API_ENABLE_WEBSOCKET
        QStringList Methods = RESTAPIRegistry::Registry.keys();
        Methods.append(RESTAPIRegistry::WSRegistry.keys());
        return  Methods;
#else
        return  RESTAPIRegistry::Registry.keys();
#endif
    }

    QMap<QString, QString> Methods = RESTAPIRegistry::extractMethods(RESTAPIRegistry::Registry, _module, _showTypes, _prettifyTypes);
#ifdef TARGOMAN_API_ENABLE_WEBSOCKET
    Methods.unite(RESTAPIRegistry::extractMethods(RESTAPIRegistry::WSRegistry, _module, _showTypes, _prettifyTypes));
#endif
    return Methods.values();
}

void RESTAPIRegistry::dumpAPIs()
{
//    TargomanInfo(5, "\n" + RESTAPIRegistry::registeredAPIs("", true, true).join("\n"));

    struct stuAPIKey {
        QString Method;
        clsAPIObject* APIObject;

        stuAPIKey() { ; }

        stuAPIKey(const stuAPIKey &_other) :
            Method(_other.Method),
            APIObject(_other.APIObject) { ; }

        stuAPIKey(QString _method, clsAPIObject* _apiObject) :
            Method(_method),
            APIObject(_apiObject) { ; }
    };

    QHash<QString, QHash<QString, stuAPIKey>> APIs;
    foreach (const QString Key, RESTAPIRegistry::Registry.keys()) {

        QString Name = Key.split(" ").last();
        QString Method = Key.split(" ").first().toLower();
        clsAPIObject* APIObject = RESTAPIRegistry::Registry.value(Key);

        if (APIObject->isHidden())
            Method = "[HIDDEN] " + Method;

        APIs[Name].insert(Method, stuAPIKey(Method, APIObject));
    }

    QStringList Keys = APIs.keys();
    Keys.sort(); //Qt::CaseSensitivity::CaseInsensitive);

    TargomanDebug(5).noLabel().noquote().nospace() << endl << Targoman::API::Helpers::ClientConfigs::RESTServerAddress.value();

    int APIsIndex = 1;
    int MethodsIndex = 1;
    foreach (const QString Name, Keys) {
        bool IsLastAPI = (Name == Keys.last());

        TargomanDebug(5).noLabel().noquote().nospace()
                << "│";
        TargomanDebug(5).noLabel().noquote().nospace()
                << (IsLastAPI ? "└" : "├") << "──"
                << "[" << QString::number(APIsIndex++) << "] "
                << Name;

        QStringList Methods = APIs[Name].keys();
        Methods.sort(); //Qt::CaseSensitivity::CaseInsensitive);
        foreach (const QString Method, Methods) {
            stuAPIKey &API = APIs[Name][Method];

            bool IsLastMethod = (Method == Methods.last());

            TargomanDebug(5).noLabel().noquote().nospace()
                    << (IsLastAPI ? " " : "│") << "   "
                    << (IsLastMethod ? "└" : "├") << "──"
                    << "(" << QString::number(MethodsIndex++) << ") "
                    << Method.toUpper() // << " " << Name
                    << (API.APIObject->requiresJWT() ? " (JWT)" : "")
                    ;

            if (API.APIObject->ParamTypesName.isEmpty() == false) {
                int maxLen = 0;

                for (int ParamsIndex = 0; ParamsIndex < API.APIObject->ParamTypesName.count(); ParamsIndex++) {
                    maxLen = max(maxLen, API.APIObject->ParamTypesName[ParamsIndex].length());
                }

                for (int ParamsIndex = 0; ParamsIndex < API.APIObject->ParamTypesName.count(); ParamsIndex++) {
                    QVariant DefVal = API.APIObject->BaseMethod.DefaultValues[ParamsIndex];

//                    bool DefValIsValid = false;
//                    QString DefValStr;
//                    qvariant.cpp#4160:
//                    if (DefVal.userType() != QMetaType::UnknownType) {
//                        dbg << QMetaType::typeName(typeId) << ", ";
//                        bool userStream = false;
//                        bool canConvertToString = false;
//                        if (typeId >= QMetaType::User) {
//                            userStream = QMetaType::debugStream(dbg, constData(v.d), typeId);
//                            canConvertToString = v.canConvert<QString>();
//                        }
//                        if (!userStream && canConvertToString)
//                            dbg << v.toString();
//                        else if (!userStream)
//                            handlerManager[typeId]->debugStream(dbg, v);
//                    } else {
//                        dbg << "Invalid";
//                    }

                    bool IsLastParam = (ParamsIndex == API.APIObject->ParamTypesName.count()-1);

                    TargomanDebug(5).noLabel().noquote().nospace()
                            << (IsLastAPI ? " " : "│") << "   "
                            << (IsLastMethod ? " " : "│") << "   "
                            << (IsLastParam ?  "└" : "├") << "──"
                            << " " << QString::number(ParamsIndex + 1) << ": "
                            << API.APIObject->ParamTypesName[ParamsIndex].leftJustified(maxLen)
                            << " "
                            << API.APIObject->ParamNames[ParamsIndex]
                            << " = "
                            << DefVal
                            << " [" << (DefVal.isNull() ? "Null" : "Not Null") << "]"
                            << " [" << (DefVal.isValid() ? "Valid" : "Invalid") << "]"
                            << (ParamsIndex < API.APIObject->ParamTypesName.count()-1 ? "," : "")
                    ;
                }
    //                    << "    ParamTypesName: [" << API.APIObject->ParamTypesName.count() << "] " << API.APIObject->ParamTypesName.join(", ") << endl
    //                    << "    ParamNames:     [" << API.APIObject->ParamNames.count() << "] " << API.APIObject->ParamNames.join(", ") << endl
    //                    << "    DefaultValues:  [" << API.APIObject->BaseMethod.DefaultValues.count() << "] " << API.APIObject->BaseMethod.DefaultValues
    //                    << (API.APIObject->BaseMethod.DefaultValues.count() != API.APIObject->ParamNames.count()
    //                       ? "    ******************** ERROR IN COUNTERS ********************"
    //                       : "")
    //                    ;
            }

        }
    }
}

/****************************************************/
Cache_t InternalCache::Cache;
QMutex InternalCache::Lock;

QScopedPointer<intfCacheConnector> CentralCache::Connector;
QHash<QString, clsAPIObject*> RESTAPIRegistry::Registry;
#ifdef TARGOMAN_API_ENABLE_WEBSOCKET
QHash<QString, clsAPIObject*>  RESTAPIRegistry::WSRegistry;
#endif

intfCacheConnector::~intfCacheConnector() { ; }

void intfCacheConnector::setKeyVal(const QString& _key, const QVariant& _value, qint32 _ttl) {
    if (_value.type() >= TAPI_BASE_USER_DEFINED_TYPEID)
        this->setKeyValImpl(_key,
                            gUserDefinedTypesInfo.at(static_cast<int>(_value.type() - TAPI_BASE_USER_DEFINED_TYPEID))->toString(_value),
                            _ttl);
    else
        this->setKeyValImpl(_key, _value.toString(), _ttl);
}

} //namespace Server
} //namespace Targoman::API
