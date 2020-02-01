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
 */

#include <QMap>
#include <QMutex>

#include "RESTAPIRegistry.h"
#include "APICache.hpp"

#include "Interfaces/ORM/clsORMField.h"

namespace Targoman {
namespace API {
namespace Server {

using namespace ORM;

/***********************************************************************************************/
#define DO_ON_TYPE_VALID(_isIntegral, _baseType)  new tmplAPIArg<_baseType, _isIntegral>(TARGOMAN_M2STR(_baseType))
#define DO_ON_TYPE_IGNORED(_isIntegral, _baseType) nullptr
#define DO_ON_TYPE_SELECTOR(_1,_2,_N,...) _N
#define DO_ON_TYPE_PROXY(_isIntegral, _baseType, ...) DO_ON_TYPE_SELECTOR(__VA_ARGS__, DO_ON_TYPE_IGNORED, DO_ON_TYPE_VALID)(_isIntegral, _baseType)
#define DO_ON_TYPE(_isIntegral, _typeName, _baseType) DO_ON_TYPE_PROXY(_isIntegral, _baseType, IGNORE_TYPE_##_typeName)

#define MAKE_INFO_FOR_VALID_INTEGRAL_METATYPE(_typeName, _id, _baseType) { _id, { DO_ON_TYPE(COMPLEXITY_Integral,  _typeName, _baseType) } },
#define MAKE_INFO_FOR_VALID_COMPLEX_METATYPE(_typeName, _id, _baseType)  { _id, { DO_ON_TYPE(COMPLEXITY_Complex, _typeName, _baseType) } },
#define MAKE_INVALID_METATYPE(_typeName, _id, _baseType) { _id, { nullptr } },

#define IGNORE_TYPE_Void ,
#define IGNORE_TYPE_Bool ,
#define IGNORE_TYPE_QByteArray ,
#define IGNORE_TYPE_QBitArray ,
#define IGNORE_TYPE_QLocale ,
#define IGNORE_TYPE_QRect ,
#define IGNORE_TYPE_QRectF ,
#define IGNORE_TYPE_QSize ,
#define IGNORE_TYPE_QSizeF ,
#define IGNORE_TYPE_QLine ,
#define IGNORE_TYPE_QLineF ,
#define IGNORE_TYPE_QPoint ,
#define IGNORE_TYPE_QPointF ,
#define IGNORE_TYPE_QEasingCurve ,
#define IGNORE_TYPE_QModelIndex ,
#define IGNORE_TYPE_QJsonValue ,
#define IGNORE_TYPE_QJsonObject ,
#define IGNORE_TYPE_QJsonArray ,
#define IGNORE_TYPE_QJsonDocument ,
#define IGNORE_TYPE_QPersistentModelIndex ,
#define IGNORE_TYPE_QPersistentModelIndex ,
#define IGNORE_TYPE_Nullptr ,
#define IGNORE_TYPE_QVariantHash ,
#define IGNORE_TYPE_QByteArrayList ,

const QMap<int, intfAPIArgManipulator*> MetaTypeInfoMap = {
    QT_FOR_EACH_STATIC_PRIMITIVE_TYPE(MAKE_INFO_FOR_VALID_INTEGRAL_METATYPE)
    QT_FOR_EACH_STATIC_PRIMITIVE_POINTER(MAKE_INVALID_METATYPE)
    QT_FOR_EACH_STATIC_CORE_CLASS(MAKE_INFO_FOR_VALID_COMPLEX_METATYPE)
    QT_FOR_EACH_STATIC_CORE_POINTER(MAKE_INVALID_METATYPE)
    QT_FOR_EACH_STATIC_CORE_TEMPLATE(MAKE_INFO_FOR_VALID_COMPLEX_METATYPE)
    QT_FOR_EACH_STATIC_GUI_CLASS(MAKE_INVALID_METATYPE)
    QT_FOR_EACH_STATIC_WIDGETS_CLASS(MAKE_INVALID_METATYPE)
    {qMetaTypeId<bool>(), { new tmplAPIArg<bool, COMPLEXITY_Integral>("bool",{},
                            [](const QVariant& _value, const QByteArray& _paramName) -> bool {
                                if(_value.toString() == "false" || _value.toString() == "0") return false;
                                if(_value.toString() == "true" || _value.toString() == "1") return true;
                                throw exHTTPBadRequest(_paramName + " is not a valid bool");
                            },
                            {},{},
                            [](const QString& _val) -> QVariant {return !(_val == "false" || _val == "0");}) } }
};

QJsonObject RESTAPIRegistry::CachedOpenAPI;

/***********************************************************************************************/
void RESTAPIRegistry::registerRESTAPI(intfAPIModule* _module, const QMetaMethod& _method){
    if ((_method.name().startsWith("api") == false &&
         _method.name().startsWith("asyncApi") == false)||
        _method.typeName() == nullptr)
        return;

    foreach(auto Item, _module->filterItems())
        Item.registerTypeIfNotRegisterd(_module);

    if(gOrderedMetaTypeInfo.isEmpty()){
        gOrderedMetaTypeInfo.reserve(MetaTypeInfoMap.lastKey());

        for(auto MetaTypeInfoMapIter = MetaTypeInfoMap.begin();
            MetaTypeInfoMapIter != MetaTypeInfoMap.end();
            ++MetaTypeInfoMapIter){
            for(int i = 0; i< MetaTypeInfoMapIter.key() - gOrderedMetaTypeInfo.size(); ++i)
                gOrderedMetaTypeInfo.append(nullptr);
            gOrderedMetaTypeInfo.append(MetaTypeInfoMapIter.value());
        }
    }

    try{
        RESTAPIRegistry::validateMethodInputAndOutput(_method);
        QString MethodName = _method.name().constData();
        MethodName = MethodName.mid(MethodName.indexOf("api",0,Qt::CaseInsensitive) + 3);

        QString MethodSignature, MethodDoc;
        bool Found = false;
        for (int i=0; i<_module->metaObject()->methodCount(); ++i)
            if(_module->metaObject()->method(i).name() == "signOf" + MethodName){
                _module->metaObject()->method(i).invoke(_module,
                                                        Qt::DirectConnection,
                                                        Q_RETURN_ARG(QString, MethodSignature)
                                                        );
                Found = true;
                break;
            }

        if(!Found)
            throw exRESTRegistry("Seems that you have not use API macro to define your API");

        for (int i=0; i<_module->metaObject()->methodCount(); ++i)
            if(_module->metaObject()->method(i).name() == "docOf" + MethodName){
                _module->metaObject()->method(i).invoke(_module,
                                                        Qt::DirectConnection,
                                                        Q_RETURN_ARG(QString, MethodDoc)
                                                        );
                Found = true;
                break;
            }

        if(!Found)
            throw exRESTRegistry("Seems that you have not use API macro to define your API");


        auto makeMethodName = [MethodName](int _start) -> QString{
            if(MethodName.size() >= _start)
                return MethodName.at(_start-1).toLower() + MethodName.mid(_start);
            return "";
        };

        constexpr char COMMA_SIGN[] = "$_COMMA_$";

        MethodSignature = MethodSignature.trimmed().replace("','", QString("'%1'").arg(COMMA_SIGN));
        bool DQuoteStarted = false, BackslashFound = false;
        QString CommaReplacedMethodSignature;
        for(int i=0; i< MethodSignature.size(); ++i){
            if(DQuoteStarted){
                if(MethodSignature.at(i).toLatin1() == ',')
                    CommaReplacedMethodSignature += COMMA_SIGN;
                else if (MethodSignature.at(i).toLatin1() == '\\') {
                    BackslashFound = true;
                }else if (MethodSignature.at(i).toLatin1() == '"') {
                    CommaReplacedMethodSignature += '"';
                    if(BackslashFound)
                        BackslashFound = false;
                    else
                        DQuoteStarted = false;
                }else
                    CommaReplacedMethodSignature += MethodSignature.at(i);
            }else if (MethodSignature.at(i).toLatin1() == '"'){
                CommaReplacedMethodSignature += '"';
                DQuoteStarted = true;
            }else
                CommaReplacedMethodSignature += MethodSignature.at(i);
        }


        QVariantList DefaultValues;
        quint8 ParamIndex = 0;
        bool ContainsFileInput = false;
        foreach(auto Param, CommaReplacedMethodSignature.split(',')){
            if (ParamIndex >= _method.parameterCount())
                break;

            if(_method.parameterType(ParamIndex) == qMetaTypeId<TAPI::stuFileInfo>() ||
               _method.parameterType(ParamIndex) == qMetaTypeId<TAPI::Files_t>())
                ContainsFileInput = true;

            if(Param.contains('=')){
                QString Value = Param.split('=').last().replace(COMMA_SIGN, ",").trimmed();
                if(Value.startsWith("(")) Value = Value.mid(1);
                if(Value.endsWith(")")) Value = Value.mid(0, Value.size() - 1);
                if(Value.startsWith("'")) Value = Value.mid(1);
                if(Value.endsWith("'")) Value = Value.mid(0, Value.size() - 1);
                if(Value.startsWith("\"")) Value = Value.mid(1);
                if(Value.endsWith("\"")) Value = Value.mid(0, Value.size() - 1);
                Value.replace("\\\"", "\"");
                Value.replace("\\'", "'");

                intfAPIArgManipulator* ArgSpecs;
                if(_method.parameterType(ParamIndex) < TAPI_BASE_USER_DEFINED_TYPEID)
                    ArgSpecs = gOrderedMetaTypeInfo.at(_method.parameterType(ParamIndex));
                else
                    ArgSpecs = gUserDefinedTypesInfo.at(_method.parameterType(ParamIndex) - TAPI_BASE_USER_DEFINED_TYPEID);

                Q_ASSERT(ArgSpecs);

                if(Value == "{}" || Value.contains("()"))
                    DefaultValues.append(ArgSpecs->defaultVariant());
                else if (ArgSpecs->complexity() == COMPLEXITY_Enum)
                    DefaultValues.append(Value.mid(Value.indexOf("::") + 2));
                else
                    DefaultValues.append(Value);
            }else
                DefaultValues.append(QVariant());
            ParamIndex++;
        }

        QMetaMethodExtended Method(_method, DefaultValues, MethodDoc);

        if(MethodName.startsWith("GET")){
            if(ContainsFileInput) throw exRESTRegistry("file input can not be used with GET method");
            RESTAPIRegistry::addRegistryEntry(RESTAPIRegistry::Registry, _module, Method, "GET", makeMethodName(sizeof("GET")));
        }else if(MethodName.startsWith("POST"))
            RESTAPIRegistry::addRegistryEntry(RESTAPIRegistry::Registry, _module, Method, "POST", makeMethodName(sizeof("POST")));
        else if(MethodName.startsWith("PUT"))
            RESTAPIRegistry::addRegistryEntry(RESTAPIRegistry::Registry, _module, Method, "PUT", makeMethodName(sizeof("PUT")));
        else if(MethodName.startsWith("CREATE"))
            RESTAPIRegistry::addRegistryEntry(RESTAPIRegistry::Registry, _module, Method, "PUT", makeMethodName(sizeof("CREATE")));
        else if(MethodName.startsWith("PATCH"))
            RESTAPIRegistry::addRegistryEntry(RESTAPIRegistry::Registry, _module, Method, "PATCH", makeMethodName(sizeof("PATCH")));
        else if (MethodName.startsWith("DELETE")){
            if(ContainsFileInput) throw exRESTRegistry("file input can not be used with DELETE method");
            RESTAPIRegistry::addRegistryEntry(RESTAPIRegistry::Registry, _module, Method, "DELETE", makeMethodName(sizeof("DELETE")));
        }else if (MethodName.startsWith("UPDATE"))
            RESTAPIRegistry::addRegistryEntry(RESTAPIRegistry::Registry, _module, Method, "PATCH", makeMethodName(sizeof("UPDATE")));
        else if (MethodName.startsWith("WS")){
#ifdef TARGOMAN_API_ENABLE_WEBSOCKET
            if(ContainsFileInput) throw exRESTRegistry("file input can not be used with WebSockets");
            RESTAPIRegistry::addRegistryEntry(RESTAPIRegistry::WSRegistry, _module, Method, "WS", makeMethodName(sizeof("WS")));
#else
            throw exRESTRegistry("Websockets are not enabled in this QRestServer please compile with websockets support");
#endif
        }else if(ContainsFileInput)
            throw exRESTRegistry("methods with file input must specifically contain HTTP method: " + MethodName);
        else{
            RESTAPIRegistry::addRegistryEntry(RESTAPIRegistry::Registry, _module, Method, "GET", MethodName.at(0).toLower() + MethodName.mid(1));
            RESTAPIRegistry::addRegistryEntry(RESTAPIRegistry::Registry, _module, Method, "POST", MethodName.at(0).toLower() + MethodName.mid(1));
        }
    }catch(Targoman::Common::exTargomanBase& ex){
        TargomanError("Fatal Error: "<<ex.what());
        throw;
    }
}

QMap<QString, QString> RESTAPIRegistry::extractMethods(QHash<QString, clsAPIObject*>& _registry, const QString& _module, bool _showTypes, bool _prettifyTypes)
{
    static auto type2Str = [_prettifyTypes](int _typeID) {
        if(_prettifyTypes == false || _typeID > 1023)
            return QString(QMetaType::typeName(_typeID));

        return gOrderedMetaTypeInfo.at(_typeID)->PrettyTypeName;
    };


    QMap<QString, QString> Methods;
    foreach(const QString& Key, _registry.keys()){
        QString Path = Key.split(" ").last();
        if(Path.startsWith(_module) == false)
            continue;

        clsAPIObject* APIObject = _registry.value(Key);
        QStringList Parameters;
        for(quint8 i=0; i< APIObject->BaseMethod.parameterCount(); ++i){
            static auto defaultValue = [](clsAPIObject* _apiObject, quint8 i) -> QString{
                if(_apiObject->RequiredParamsCount > i)
                    return "";
                intfAPIArgManipulator* ArgManipulator = _apiObject->argSpecs(i);
                if(ArgManipulator){
                    switch(ArgManipulator->complexity()){
                    case COMPLEXITY_Number:
                    case COMPLEXITY_Boolean:
                    case COMPLEXITY_Integral:
                        return QString(" = %1").arg(_apiObject->defaultValue(i).toString());
                    case COMPLEXITY_String:
                    case COMPLEXITY_Complex:
                    case COMPLEXITY_Enum:
                        return QString(" = \"%1\"").arg(_apiObject->defaultValue(i).toString().replace("\"", "\\\""));
                    case COMPLEXITY_File:
                        return "";
                    }
                    return " = " + (ArgManipulator->isPrimitiveType() ? QString("%1") : (QString("\"%1\""))).arg(
                                _apiObject->defaultValue(i).toString().replace("\"", "\\\""));
                }
                return "";
            };

            Parameters.append((_showTypes ? type2Str(APIObject->BaseMethod.parameterType(i)) + " " : "" ) + (
                                  APIObject->BaseMethod.parameterNames().at(i).startsWith('_') ?
                                      APIObject->BaseMethod.parameterNames().at(i).mid(1) :
                                      APIObject->BaseMethod.parameterNames().at(i)) +
                              defaultValue(APIObject, i)
                              );
        }

        Methods.insert(Path + Key.split(" ").first(),
                       QString("%1(%2) %3 %4").arg(
                           Key).arg(
                           Parameters.join(", ")).arg(
                           _showTypes ? QString("-> %1").arg(
                                            type2Str(APIObject->BaseMethod.returnType())) : "").arg(
                           APIObject->IsAsync ? "[async]" : ""
                                                )
                       );
    }

    return Methods;
}

QJsonObject RESTAPIRegistry::retriveOpenAPIJson(){
    if(RESTAPIRegistry::CachedOpenAPI.isEmpty() == false)
        return RESTAPIRegistry::CachedOpenAPI;
    /**/

    RESTAPIRegistry::CachedOpenAPI = ServerConfigs::BaseOpenAPIObject;

    if(RESTAPIRegistry::CachedOpenAPI.isEmpty())
        RESTAPIRegistry::CachedOpenAPI = QJsonObject({
                                                         {"swagger","2.0"},
                                                         {"info",QJsonObject({
                                                              {"version", ServerConfigs::Version.value()},
                                                              {"title", "NOT_SET"},
                                                              {"description", ""},
                                                              {"contact", QJsonObject({{"email", "sample@example.com"}})}
                                                          })},
                                                         {"host", QString("127.0.0.1:%1").arg(ServerConfigs::ListenPort.value())},
                                                         {"securityDefinitions", QJsonObject({
                                                              {"Bearer", QJsonObject({
                                                                   {"type", "apiKey"},
                                                                   {"name", "Authorization"},
                                                                   {"in", "header"},
                                                                   {"description", "For accessing the API a valid JWT token must be passed in all the queries in the 'Authorization' header.\n\n A valid JWT token is generated by the API and retourned as answer of a call     to the route /login giving a valid user & password.\n\nThe following syntax must be used in the 'Authorization' header :\n\nBearer xxxxxx.yyyyyyy.zzzzzz"},
                                                               })}
                                                          })},
                                                         {"basePath", ServerConfigs::BasePathWithVersion},
                                                         {"schemes", QJsonArray({"http", "https"})},
                                                     });

    if(RESTAPIRegistry::CachedOpenAPI.value("info").isObject() == false)
        throw exHTTPInternalServerError("Invalid OpenAPI base json");

    QJsonObject PathsObject;
    QJsonArray TagsArray;
    foreach(const QString& Key, RESTAPIRegistry::Registry.keys()){
        QString PathString = Key.split(" ").last();
        QString HTTPMethod = Key.split(" ").first().toLower();
        QStringList ExtraPathsStorage;

        clsAPIObject* APIObject = RESTAPIRegistry::Registry.value(Key);

        auto paramName = [APIObject](quint8 i) {
            return APIObject->BaseMethod.parameterNames().at(i).startsWith('_') ?
                        APIObject->BaseMethod.parameterNames().at(i).mid(1) :
                        APIObject->BaseMethod.parameterNames().at(i);
        };

        auto mapTypeToValidOpenAPIType = [](enuVarComplexity _complexity, const QString& _typeName){
            switch(_complexity){
            case COMPLEXITY_Number:
            case COMPLEXITY_Boolean:
            case COMPLEXITY_Integral:
                if(_typeName == "bool")
                    return "boolean";
                else if(_typeName.endsWith("int") || _typeName.endsWith("long"))
                    return "integer";
                else if(_typeName.endsWith("char"))
                    return "string";
                else
                    return  "number";
            case COMPLEXITY_String:
            case COMPLEXITY_Complex:
            case COMPLEXITY_Enum:
                return  "string";
            case COMPLEXITY_File:
                if(_typeName == "TAPI::Files_t")
                    return "string";
                return  "file";
            }
            return "ERROR";
        };

        auto fillParamTypeSpec = [mapTypeToValidOpenAPIType](intfAPIArgManipulator* _argMan,
                const QList<clsORMField>& _fields,
                QString _typeName,
                QJsonObject& ParamSpecs,
                QVariant _deafaultValue = {},
                bool addExample = false) -> void{
            ParamSpecs["description"] = _argMan->description(_fields);
            ParamSpecs["type"] = mapTypeToValidOpenAPIType(_argMan->complexity(), _typeName);
            if(_argMan->complexity() == COMPLEXITY_Enum)
                ParamSpecs["enum"] = QJsonArray::fromStringList(_argMan->options());

            if(_deafaultValue.isValid()){
                ParamSpecs["default"] = _deafaultValue.toString();
                if(addExample)
                    ParamSpecs["example"] = _deafaultValue.toString();
            }else{
                ParamSpecs["required"] = true;
                if(addExample)
                    ParamSpecs["example"] = QJsonValue();
            }

        };

        auto addParamSpecs = [APIObject, paramName, HTTPMethod, fillParamTypeSpec, mapTypeToValidOpenAPIType](QJsonArray& _parameters, quint8 _i, QStringList* _extraPathsStorage) -> void {
            QString ParamType = QMetaType::typeName(APIObject->BaseMethod.parameterType(_i));
            if(   ParamType == PARAM_HEADERS
                  || ParamType == PARAM_REMOTE_IP
                  || ParamType == PARAM_COOKIES
                  || ParamType == PARAM_JWT
                  )
                return;
            QJsonObject ParamSpecs;

            if(ParamType == PARAM_EXTRAPATH){
                if(_extraPathsStorage){
                    QList<clsORMField> PKs;
                    foreach(auto Item, APIObject->Parent->filterItems(qhttp::EHTTP_DELETE))
                        if(Item.isPrimaryKey())
                            PKs.append(Item);
                    foreach(auto PK, PKs){
                        _extraPathsStorage->append(PK.name());
                        intfAPIArgManipulator* ArgSpecs;
                        if(PK.parameterType()< TAPI_BASE_USER_DEFINED_TYPEID)
                            ArgSpecs = gOrderedMetaTypeInfo.at(PK.parameterType());
                        else
                            ArgSpecs = gUserDefinedTypesInfo.at(PK.parameterType()- TAPI_BASE_USER_DEFINED_TYPEID);

                        Q_ASSERT(ArgSpecs);
                        ParamSpecs["in"] = "path";
                        ParamSpecs["name"] = _extraPathsStorage->last();
                        ParamSpecs["required"] = true;
                        ParamSpecs["description"] = ArgSpecs->description(APIObject->Parent->filterItems());
                        ParamSpecs["type"] = mapTypeToValidOpenAPIType(ArgSpecs->complexity(), QMetaType::typeName(PK.parameterType()));
                        _parameters.append(ParamSpecs);
                    }
                }
                return;
            }

            if(HTTPMethod == "get" || HTTPMethod == "delete"){
                if(_extraPathsStorage){
                    if(ParamType == PARAM_ORMFILTER){
                        foreach(auto Item, APIObject->Parent->filterItems(HTTPMethod == "get" ? qhttp::EHTTP_GET : qhttp::EHTTP_DELETE)){
                            if(Item.isFilterable()== false || Item.isPrimaryKey())
                                continue;
                            QJsonObject ParamSpecs;
                            ParamSpecs["in"] = "query";
                            ParamSpecs["name"] = Item.name();
                            intfAPIArgManipulator* ArgSpecs;
                            if(Item.parameterType()< TAPI_BASE_USER_DEFINED_TYPEID)
                                ArgSpecs = gOrderedMetaTypeInfo.at(Item.parameterType());
                            else
                                ArgSpecs = gUserDefinedTypesInfo.at(Item.parameterType()- TAPI_BASE_USER_DEFINED_TYPEID);

                            Q_ASSERT(ArgSpecs);
                            fillParamTypeSpec(ArgSpecs,
                                              APIObject->Parent->filterItems(),
                                              QMetaType::typeName(Item.parameterType()),
                                              ParamSpecs,
                                              Item.defaultValue()
                                              );
                            _parameters.append(ParamSpecs);
                        }
                    }
                    return ;
                }
                if(ParamType == PARAM_ORMFILTER)
                    return;

                ParamSpecs["in"] = "query";
                ParamSpecs["name"] = QString(paramName(_i));
                ParamSpecs["required"] = APIObject->isRequiredParam(_i);

                fillParamTypeSpec(APIObject->argSpecs(_i),
                                  APIObject->Parent->filterItems(),
                                  QMetaType::typeName(APIObject->BaseMethod.parameterType(_i)),
                                  ParamSpecs,
                                  APIObject->defaultValue(_i)
                                  );
                _parameters.append(ParamSpecs);
            }
        };

        QStringList PathStringParts = PathString.split("/", QString::SkipEmptyParts);
        if(APIObject->HasExtraMethodName)
            PathStringParts.pop_back();
        QString TagName  =  PathStringParts.join("_");

        auto createPathInfo = [TagName, APIObject, HTTPMethod, addParamSpecs, paramName, fillParamTypeSpec](QStringList* _extraPathsStorage)->QJsonObject{
            QJsonObject PathInfo = QJsonObject({{"summary", APIObject->BaseMethod.Doc}});

            PathInfo["tags"] = QJsonArray({TagName});
            PathInfo["produces"] = QJsonArray({"application/json"});
            if(HTTPMethod != "get" && HTTPMethod != "delete"){
                quint8 RequiredFiles = 0;
                for(quint8 i=0; i< APIObject->BaseMethod.parameterCount(); ++i)
                    if(QMetaType::typeName(APIObject->BaseMethod.parameterType(i)) == QStringLiteral(PARAM_FILE))
                        RequiredFiles++;
                    else if (QMetaType::typeName(APIObject->BaseMethod.parameterType(i)) == QStringLiteral(PARAM_FILES))
                        RequiredFiles += 2;

                QJsonObject Properties;
                for(quint8 i=0; i< APIObject->BaseMethod.parameterCount(); ++i){
                    QString ParamType = QMetaType::typeName(APIObject->BaseMethod.parameterType(i));
                    if(   ParamType == PARAM_HEADERS
                          || ParamType == PARAM_REMOTE_IP
                          || ParamType == PARAM_COOKIES
                          || ParamType == PARAM_JWT
                          )
                        continue;

                    if(ParamType == PARAM_ORMFILTER){
                        foreach(auto Item, APIObject->Parent->filterItems(HTTPMethod == "put" ? qhttp::EHTTP_PUT :
                                                                          HTTPMethod == "patch" ? qhttp::EHTTP_PATCH :
                                                                          qhttp::EHTTP_BIND)){
                            if(Item.isReadOnly() && Item.isPrimaryKey() == false)
                                continue;
                            QJsonObject ParamSpecs;
                            intfAPIArgManipulator* ArgSpecs;
                            if(Item.parameterType()< TAPI_BASE_USER_DEFINED_TYPEID)
                                ArgSpecs = gOrderedMetaTypeInfo.at(Item.parameterType());
                            else
                                ArgSpecs = gUserDefinedTypesInfo.at(Item.parameterType()- TAPI_BASE_USER_DEFINED_TYPEID);

                            if(!ArgSpecs){
                                Item.registerTypeIfNotRegisterd(APIObject->Parent);
                                if(Item.parameterType()< TAPI_BASE_USER_DEFINED_TYPEID)
                                    ArgSpecs = gOrderedMetaTypeInfo.at(Item.parameterType());
                                else
                                    ArgSpecs = gUserDefinedTypesInfo.at(Item.parameterType()- TAPI_BASE_USER_DEFINED_TYPEID);
                            }

                            Q_ASSERT(ArgSpecs);
                            fillParamTypeSpec(ArgSpecs,
                                              APIObject->Parent->filterItems(),
                                              QMetaType::typeName(Item.parameterType()),
                                              ParamSpecs,
                                              HTTPMethod == "put" ? Item.toString(Item.defaultValue()) : QVariant(),
                                              Item.isPrimaryKey() == false
                                              );
                            Properties[Item.name()] = ParamSpecs;
                        }
                    }else{
                        QJsonObject ParamSpecs;
                        fillParamTypeSpec(APIObject->argSpecs(i),
                                          APIObject->Parent->filterItems(),
                                          QMetaType::typeName(APIObject->BaseMethod.parameterType(i)),
                                          ParamSpecs,
                                          APIObject->defaultValue(i),
                                          true);
                        Properties[paramName(i)] = ParamSpecs;
                    }
                }

                QJsonArray Parameters;
                for(quint8 i=0; i< APIObject->BaseMethod.parameterCount(); ++i)
                    addParamSpecs( Parameters, i, _extraPathsStorage);


                if(RequiredFiles > 0){
                    PathInfo["consumes"] = QJsonArray({"multipart/form-data"});
                    for(auto Prop = Properties.begin(); Prop != Properties.end(); ++Prop){
                        QJsonObject PropVal = Prop.value().toObject();
                        PropVal.insert("name", Prop.key());
                        PropVal.insert("in", "formData");
                        Parameters.append(PropVal);
                    }
                }else{
                    PathInfo["consumes"] = QJsonArray({"application/json"});
                    Parameters.append(QJsonObject({
                                                      {"in", "body"},
                                                      {"name", "body"},
                                                      {"description", "Pramaeter Object"},
                                                      {"required", true},
                                                      {"schema", QJsonObject({
                                                           {"type", "object"},
                                                           {"properties", Properties}
                                                       })}
                                                  }));
                }
                PathInfo["parameters"] = Parameters;
            }else{
                QJsonArray Parameters;
                for(quint8 i=0; i< APIObject->BaseMethod.parameterCount(); ++i){
                    addParamSpecs(Parameters, i, _extraPathsStorage);
                }
                if(Parameters.size())
                    PathInfo["parameters"] = Parameters;
            }

            static QJsonObject DefaultResponses = QJsonObject({
                                                                  {"200", QJsonObject({{"description", "Success."}})},
                                                                  {"400", QJsonObject({{"description", "Bad request."}})},
                                                                  {"404", QJsonObject({{"description", "Not found"}})},
                                                                  {"5XX", QJsonObject({{"description", "Unexpected error"}})},
                                                              });
            QJsonObject ResponseModel = DefaultResponses;
            if(HTTPMethod == "get"){
                QJsonObject Properties;
                foreach(auto Item, APIObject->Parent->filterItems()){
                    QJsonObject ParamSpecs;
                    intfAPIArgManipulator* ArgSpecs;
                    if(Item.parameterType()< TAPI_BASE_USER_DEFINED_TYPEID)
                        ArgSpecs = gOrderedMetaTypeInfo.at(Item.parameterType());
                    else
                        ArgSpecs = gUserDefinedTypesInfo.at(Item.parameterType()- TAPI_BASE_USER_DEFINED_TYPEID);

                    Q_ASSERT(ArgSpecs);
                    fillParamTypeSpec(ArgSpecs,
                                      APIObject->Parent->filterItems(),
                                      QMetaType::typeName(Item.parameterType()),
                                      ParamSpecs
                                      );

                    Properties.insert(Item.name(), ParamSpecs);
                }
                if(Properties.size())
                    ResponseModel["200"] =
                            QJsonObject({
                                            {"description", "Success"},
                                            {"schema", QJsonObject({
                                                 {"type", "object"},
                                                 {"properties", Properties}
                                             })}
                                        });
            }

            if(APIObject->requiresJWT()){
                ResponseModel["401"] = QJsonObject({{"description", "Authorization information is missing or invalid"}});
                ResponseModel["403"] = QJsonObject({{"description", "Access forbidden"}});
            }

            PathInfo["responses"] = ResponseModel;

            if(APIObject->requiresJWT()){
                PathInfo["security"] =  QJsonArray({
                                                       QJsonObject({
                                                           {"Bearer", QJsonArray()}
                                                       })
                                                   });
            }
            return PathInfo;
        };

        auto add2Paths = [PathString, HTTPMethod](QJsonObject& PathsObject, const QJsonObject& _currPathMethodInfo, QStringList* _extraPathsStorage){
            if(HTTPMethod == "delete" && _currPathMethodInfo.value("parameters").toArray().isEmpty())
                return;
            QString Path = PathString + (_extraPathsStorage ? ("/{" + _extraPathsStorage->join("},{") + "}") : "");
            if(PathsObject.contains(Path)){
                QJsonObject CurrPathObject = PathsObject.value(Path).toObject();
                CurrPathObject[HTTPMethod] = _currPathMethodInfo;
                PathsObject[Path] = CurrPathObject;
            }else
                PathsObject[Path] = QJsonObject({ {HTTPMethod, _currPathMethodInfo} });
        };

        if(APIObject->requiresExtraPath())
            add2Paths(PathsObject, createPathInfo(&ExtraPathsStorage), &ExtraPathsStorage);

        quint8 HasNonAutoParams = false;
        foreach(auto ParamType, APIObject->ParamTypes)
            if(/*ParamType != PARAM_HEADERS
                        && ParamType != PARAM_REMOTE_IP
                        && ParamType != PARAM_COOKIES
                        && ParamType != PARAM_JWT
                        &&*/ ParamType != PARAM_EXTRAPATH
               ){
                HasNonAutoParams = true;
                break;
            }


        if(HasNonAutoParams)
            add2Paths(PathsObject, createPathInfo(nullptr), nullptr);

        QList<clsORMField> Filters = APIObject->Parent->filterItems();
        QStringList ModelDescription;
        foreach(auto Filter, Filters){
            QString FilterInfo = "* " + Filter.name();
            QStringList Flags;
            if(Filter.isSortable()) Flags.append("Sortable");
            if(Filter.isFilterable()) Flags.append("Filterable");
            if(Filter.isPrimaryKey()) Flags.append("*PrimaryKey*");
            if(Filter.isSelfIdentifier()) Flags.append("**SelfIdentifier**");
            if(Filter.isVirtual()) Flags.append("Virtual");
            if(Flags.size())
                FilterInfo += " (" + Flags.join("|") + ")";
            ModelDescription.append(FilterInfo);
        }

        if(false && ModelDescription.size()){
            TagsArray.append(QJsonObject({
                                             {"name", TagName},
                                             {"description", QString("Model:\n" + ModelDescription.join("\n"))}
                                         }));
        }
    }

    RESTAPIRegistry::CachedOpenAPI["paths"] = PathsObject;
    //RESTAPIRegistry::CachedOpenAPI["tags"] = TagsArray;

    return RESTAPIRegistry::CachedOpenAPI;
}

QStringList RESTAPIRegistry::registeredAPIs(const QString& _module, bool _showParams, bool _showTypes, bool _prettifyTypes){
    if(_showParams == false){
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

QString RESTAPIRegistry::isValidType(int _typeID, bool _validate4Input){
    if(_typeID == 0 || _typeID == QMetaType::User || _typeID == 1025)
        return  "is not registered with Qt MetaTypes";
    if(_typeID < TAPI_BASE_USER_DEFINED_TYPEID && (_typeID >= gOrderedMetaTypeInfo.size() || gOrderedMetaTypeInfo.at(_typeID) == nullptr))
        return "is complex type and not supported";

    if(_typeID >= TAPI_BASE_USER_DEFINED_TYPEID){
        if((_typeID - TAPI_BASE_USER_DEFINED_TYPEID >= gUserDefinedTypesInfo.size() ||
            gUserDefinedTypesInfo.at(_typeID - TAPI_BASE_USER_DEFINED_TYPEID) == nullptr))
            return "is user defined but not registered";

        if(strcmp(gUserDefinedTypesInfo.at(_typeID - TAPI_BASE_USER_DEFINED_TYPEID)->RealTypeName, QMetaType::typeName(_typeID)))
            return QString("Seems that registration table is corrupted: %1:%2 <-> %3:%4").arg(
                        _typeID).arg(
                        gUserDefinedTypesInfo.at(_typeID - TAPI_BASE_USER_DEFINED_TYPEID)->RealTypeName).arg(
                        _typeID).arg(
                        QMetaType::typeName(_typeID));

        if(_validate4Input){
            if(!gUserDefinedTypesInfo.at(_typeID - TAPI_BASE_USER_DEFINED_TYPEID)->hasFromVariantMethod())
                return "has no fromVariant lambda so can not be used as input";
        }else{
            if(!gUserDefinedTypesInfo.at(_typeID - TAPI_BASE_USER_DEFINED_TYPEID)->hasToVariantMethod())
                return "has no toVariant lambda so can not be used as output";
        }
    }
    return "";
}

void RESTAPIRegistry::validateMethodInputAndOutput(const QMetaMethod& _method){
    if(_method.parameterCount() > 10)
        throw exRESTRegistry("Unable to register methods with more than 10 input args <"+_method.name()+">");

    QString ErrMessage;
    if ((ErrMessage = RESTAPIRegistry::isValidType(_method.returnType(), false)).size())
        throw exRESTRegistry(QString("Invalid return type(%1): %2").arg(_method.typeName()).arg(ErrMessage));

    ErrMessage.clear();

    for(int i=0; i<_method.parameterCount(); ++i){
        if ((ErrMessage = RESTAPIRegistry::isValidType(_method.parameterType(i), true)).size())
            throw exRESTRegistry(QString("Invalid parameter (%1 %2): %3").arg(
                                     _method.parameterTypes().at(i).constData()).arg(
                                     _method.parameterNames().at(i).constData()).arg(
                                     ErrMessage));

        for(int j=i+1; j<_method.parameterCount(); ++j){
            if(_method.parameterNames().at(j) == _method.parameterNames().at(i))
                throw exRESTRegistry(QString("Invalid duplicate parameter name %1 at %2 and %3").arg(
                                         _method.parameterNames().at(i).constData()).arg(
                                         i).arg(
                                         j
                                         ));
        }
    }
}

constexpr char CACHE_INTERNAL[] = "CACHEABLE_";
constexpr char CACHE_CENTRAL[]  = "CENTRALCACHE_";

void RESTAPIRegistry::addRegistryEntry(QHash<QString, clsAPIObject *>& _registry,
                                       intfAPIModule* _module,
                                       const QMetaMethodExtended& _method,
                                       const QString& _httpMethod,
                                       const QString& _methodName){
    QString MethodKey = RESTAPIRegistry::makeRESTAPIKey(_httpMethod, "/" + _module->moduleBaseName().replace("::", "/")+ '/' + _methodName);

    if(_registry.contains(MethodKey)) {
        if(RESTAPIRegistry::Registry.value(MethodKey)->isPolymorphic(_method))
            throw exRESTRegistry(QString("Polymorphism is not supported: %1").arg(_method.methodSignature().constData()));
        _registry.value(MethodKey)->updateDefaultValues(_method);
    } else {
        if(RESTAPIRegistry::getCacheSeconds(_method, CACHE_INTERNAL) > 0 && RESTAPIRegistry::getCacheSeconds(_method, CACHE_CENTRAL) >0)
            throw exRESTRegistry("Both internal and central cache can not be defined on an API");

        _registry.insert(MethodKey,
                         new clsAPIObject(_module,
                                          _method,
                                          QString(_method.name()).startsWith("async"),
                                          RESTAPIRegistry::getCacheSeconds(_method, CACHE_INTERNAL),
                                          RESTAPIRegistry::getCacheSeconds(_method, CACHE_CENTRAL),
                                          !_methodName.isEmpty()
                                          ));
    }
}

int RESTAPIRegistry::getCacheSeconds(const QMetaMethod& _method, const char* _type){
    if(_method.tag() == nullptr || _method.tag()[0] == '\0')
        return 0;
    QString Tag = _method.tag();
    if(Tag.startsWith(_type) && Tag.size () > 12){
        Tag = Tag.mid(10);
        if(_type == CACHE_INTERNAL && Tag == "INF")
            return -1;
        char Type = Tag.rbegin()->toLatin1();
        int  Number = Tag.mid(0,Tag.size() -1).toUShort();
        switch(Type){
        case 'S': return Number;
        case 'M': return Number * 60;
        case 'H': return Number * 3600;
        default:
            throw exRESTRegistry("Invalid CACHE numer or type defined for api: " + _method.methodSignature());
        }
    }
    return 0;
}


Cache_t InternalCache::Cache;
QMutex InternalCache::Lock;

QScopedPointer<intfCacheConnector> CentralCache::Connector;
QHash<QString, clsAPIObject*>  RESTAPIRegistry::Registry;
#ifdef TARGOMAN_API_ENABLE_WEBSOCKET
QHash<QString, clsAPIObject*>  RESTAPIRegistry::WSRegistry;
#endif

/****************************************************/
intfCacheConnector::~intfCacheConnector()
{;}

void Targoman::API::Server::intfCacheConnector::setKeyVal(const QString& _key, const QVariant& _value, qint32 _ttl)
{
    if(_value.type() >= TAPI_BASE_USER_DEFINED_TYPEID)
        this->setKeyValImpl(_key,
                            gUserDefinedTypesInfo.at(static_cast<int>(_value.type() - TAPI_BASE_USER_DEFINED_TYPEID))->toString(_value),
                            _ttl);
    else
        this->setKeyValImpl(_key, _value.toString(), _ttl);
}


}
}
}
