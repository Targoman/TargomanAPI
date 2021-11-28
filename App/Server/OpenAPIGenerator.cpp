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

#include "OpenAPIGenerator.h"
#include "ServerConfigs.h"
#include "Interfaces/DBM/clsORMField.h"

namespace Targoman::API::Server {

using namespace DBM;

QJsonObject OpenAPIGenerator::CachedJson;

QJsonObject initializeObject(const QString &_host = "127.0.0.1", const quint16 _port = 80)
{

    if (ServerConfigs::BaseOpenAPIObjectFile.value().size())
    {
        QFile File(ServerConfigs::BaseOpenAPIObjectFile.value());
        File.open(QIODevice::ReadOnly);
        if (File.isReadable() == false)
            throw exTargomanAPI("Unable to open: <" + ServerConfigs::BaseOpenAPIObjectFile.value() + "> for reading");
        QJsonDocument JsonDoc = QJsonDocument::fromBinaryData(File.readAll());
        if(JsonDoc.isNull() || JsonDoc.isObject() == false)
            throw exTargomanAPI("Invalid json reading from: <" + ServerConfigs::BaseOpenAPIObjectFile.value() + ">");
        return JsonDoc.object();
    }
    else
    {
        return QJsonObject({
                               { "swagger","2.0" },
                               { "info",QJsonObject({
                                    { "version", ServerConfigs::Version.value() },
                                    { "title", "Targoman API" },
                                    { "description", "" },
                                    { "contact", QJsonObject({{"email", "sample@example.com"}}) }
                                }) },
                               { "host", QString("%1:%2").arg(_host).arg(_port) },
                               { "securityDefinitions", QJsonObject({
                                    { "Bearer", QJsonObject({
                                         { "type", "apiKey" },
                                         { "name", "Authorization" },
                                         { "in", "header" },
                                         { "description",
                                           "For accessing the API a valid JWT token must be passed in all the queries in the 'Authorization' header.\n\n "
                                           "A valid JWT token is generated by the API and returned as answer of a call "
                                           "to the route /login giving a valid user & password.\n\n"
                                           "The following syntax must be used in the 'Authorization' header :\n\n"
                                           "Bearer xxxxxx.yyyyyyy.zzzzzz" },
                                     }) }
                                }) },
                               { "basePath", ServerConfigs::BasePathWithVersion },
                               { "definitions", QJsonObject() },
                               { "schemes", QJsonArray({"http", "https"}) },
                           });

    }
}

QJsonObject OpenAPIGenerator::retrieveJson(const QString &_host, const quint16 _port)
{
    if (OpenAPIGenerator::CachedJson.isEmpty() == false)
    {
        QJsonObject Return = OpenAPIGenerator::CachedJson;

        Return["host"] = QString("%1:%2").arg(_host).arg(_port);

        return Return;
    }

    OpenAPIGenerator::CachedJson = initializeObject(_host, _port);

    if (OpenAPIGenerator::CachedJson.value("info").isObject() == false)
        throw exHTTPInternalServerError("Invalid OpenAPI base json");

    QJsonObject PathsObject;
    QJsonArray TagsArray;
    foreach(const QString& Key, RESTAPIRegistry::Registry.keys()){
        QString PathString = Key.split(" ").last();
        QString HTTPMethod = Key.split(" ").first().toLower();
        QStringList PKInPathStorage;

        clsAPIObject* APIObject = RESTAPIRegistry::Registry.value(Key);

        auto paramName = [APIObject](quint8 i) {
            return APIObject->BaseMethod.parameterNames().at(i).startsWith('_') ?
                        APIObject->BaseMethod.parameterNames().at(i).mid(1) :
                        APIObject->BaseMethod.parameterNames().at(i);
        };

        auto mapTypeToValidOpenAPIType = [](enuVarComplexity _complexity, QString _typeName){
            if (_typeName.startsWith(TAPI_HELEPER_QSP_M2STR_PREFIX))
                _typeName = _typeName.mid(sizeof(TAPI_HELEPER_QSP_M2STR_PREFIX), _typeName.size() - static_cast<int>(sizeof(TAPI_HELEPER_QSP_M2STR_PREFIX)) - 1);

            switch(_complexity)
            {
                case COMPLEXITY_Number:
                case COMPLEXITY_Boolean:
                case COMPLEXITY_Integral:
                    if (_typeName == "bool")
                        return "boolean";
                    else if (_typeName.endsWith("int8") || _typeName.endsWith("int16") || _typeName.endsWith("int32") || _typeName.endsWith("int64") || _typeName.endsWith("long"))
                        return "integer";
                    else if (_typeName.endsWith("char"))
                        return "string";
                    else
                        return "number";
                case COMPLEXITY_String:
                case COMPLEXITY_Complex:
                case COMPLEXITY_Enum:
                    return  "string";
                case COMPLEXITY_Object:
                    return "object";
                case COMPLEXITY_File:
                    if(_typeName == "TAPI::Files_t")
                        return "string";
                    return "file";
            }
            return "ERROR";
        };

        auto addExtraParamsByType = [](QJsonObject& _paramSpecs, enuVarComplexity _complexity, QString _typeName) {
            if (_typeName.startsWith(TAPI_HELEPER_QSP_M2STR_PREFIX))
                _typeName = _typeName.mid(sizeof(TAPI_HELEPER_QSP_M2STR_PREFIX), _typeName.size() - static_cast<int>(sizeof(TAPI_HELEPER_QSP_M2STR_PREFIX)) - 1);

            switch(_complexity)
            {
                case COMPLEXITY_Number:
                case COMPLEXITY_Integral:
                    if (_typeName.endsWith("int8")) {
                        _paramSpecs["format"] = "int32";
                        _paramSpecs["minimum"] = _typeName.startsWith("uint") ? 0 : -128;
                        _paramSpecs["maximum"] = _typeName.startsWith("uint") ? 255 : 127;
                    }
                    else if (_typeName.endsWith("int16")) {
                        _paramSpecs["format"] = "int32";
                        _paramSpecs["minimum"] = _typeName.startsWith("uint") ? 0 : -32768 ;
                        _paramSpecs["maximum"] = _typeName.startsWith("uint") ? 65535 : 32767;
                    }
                    else if (_typeName.endsWith("int32")) {
                        _paramSpecs["format"] = "int32";
                        if (_typeName.startsWith("uint"))
                            _paramSpecs["minimum"] = 0;
                    }
                    else if (_typeName.endsWith("int64")) {
                        _paramSpecs["format"] = "int64";
                        if (_typeName.startsWith("uint"))
                            _paramSpecs["minimum"] = 0;
                    }
                    else if (_typeName.endsWith("float")) _paramSpecs["format"] = "int64";
                    else if (_typeName.endsWith("double")) _paramSpecs["format"] = "double";
                    break;
                case COMPLEXITY_Complex:
                case COMPLEXITY_String:
                    if (_typeName != "string") {
                        QString Format = _typeName.split("::").last().toLower();
                        if (Format.endsWith("_t")) Format = Format.mid(0, Format.size() - 2);
                        if (Format.startsWith('q')) Format = Format.mid(1);
                        _paramSpecs["format"] = Format;
                    }
                    //ParamSpecs["pattern"] = ;
                    break;
                default:
                    break;
            }
        };

        auto fillParamTypeSpec = [mapTypeToValidOpenAPIType, addExtraParamsByType](
                intfAPIArgManipulator* _argMan,
                const QList<clsORMField>& _fields,
                QString _typeName,
                QJsonObject& ParamSpecs,
                QVariant _defaultValue = {},
                bool addExample = false
            ) -> void
        {
            ParamSpecs["description"] = (_argMan->isNullable() ? "Null to keep as is or "  : "") + _argMan->description(_fields);
            ParamSpecs["type"] = mapTypeToValidOpenAPIType(_argMan->complexity(), _typeName);
            addExtraParamsByType(ParamSpecs, _argMan->complexity(), _typeName);
            if(_argMan->isNullable())
                ParamSpecs["nullable"] = true;
            if(_argMan->complexity() == COMPLEXITY_Enum)
                ParamSpecs["enum"] = QJsonArray::fromStringList(_argMan->options());

            if(_defaultValue.isValid() && _defaultValue != QRequired){
                ParamSpecs["default"] = _defaultValue == QNull || _argMan->isNullable() ? QJsonValue() : _defaultValue.toString();
                if(addExample){
                    if(_argMan->isNullable())
                        ParamSpecs["example"] = QJsonValue();
                    if(_argMan->complexity() == COMPLEXITY_Object)
                        ParamSpecs["example"] = QJsonObject();
                    else
                        ParamSpecs["example"] = _defaultValue.toString();
                }
            }else{
                if(_argMan->isNullable() == false){
                    if(addExample){
                        ParamSpecs["required"] = true;
                        ParamSpecs["example"] = _argMan->defaultVariant().toString();
                    }else
                        ParamSpecs["example"] = QJsonValue();
                }else if (addExample)
                    ParamSpecs["example"] = QJsonValue();
                if(_defaultValue == QRequired)
                    ParamSpecs["required"] = true;
            }
        };

        auto addParamSpecs = [APIObject, paramName, HTTPMethod, fillParamTypeSpec, mapTypeToValidOpenAPIType](
                             QJsonArray& _parameters, quint8 _i, QStringList* _pksByPathsStorage) -> void {
            QString ParamType = QMetaType::typeName(APIObject->BaseMethod.parameterType(_i));
            if (   ParamType == PARAM_HEADERS
                  || ParamType == PARAM_REMOTE_IP
                  || ParamType == PARAM_COOKIES
                  || ParamType == PARAM_JWT
                  )
                return;
            QJsonObject ParamSpecs;

            if(ParamType == PARAM_PKSBYPATH){
                if(_pksByPathsStorage){
                    QList<clsORMField> PKs;
                    foreach(auto Item, APIObject->Parent->filterItems(qhttp::EHTTP_DELETE))
                        if(Item.isPrimaryKey())
                            PKs.append(Item);
                    foreach(auto PK, PKs){
                        _pksByPathsStorage->append(PK.name());
                        intfAPIArgManipulator* ArgSpecs;
                        if(PK.parameterType()< TAPI_BASE_USER_DEFINED_TYPEID)
                            ArgSpecs = gOrderedMetaTypeInfo.at(PK.parameterType());
                        else
                            ArgSpecs = gUserDefinedTypesInfo.at(PK.parameterType()- TAPI_BASE_USER_DEFINED_TYPEID);

                        Q_ASSERT(ArgSpecs);
                        ParamSpecs["in"] = "path";
                        ParamSpecs["name"] = _pksByPathsStorage->last();
                        ParamSpecs["required"] = true;
                        ParamSpecs["description"] = ArgSpecs->description(APIObject->Parent->filterItems());
                        ParamSpecs["type"] = mapTypeToValidOpenAPIType(ArgSpecs->complexity(), QMetaType::typeName(PK.parameterType()));
                        _parameters.append(ParamSpecs);
                    }
                }
                return;
            }

            if(HTTPMethod == "get" || HTTPMethod == "delete"){
                if(_pksByPathsStorage){
                    if(ParamType == PARAM_ORMFIELDS){
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
                if(ParamType == PARAM_ORMFIELDS)
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
        if (APIObject->HasExtraMethodName)
            PathStringParts.pop_back();
        QString TagName  =  PathStringParts.join("_");

        auto createPathInfo = [TagName, APIObject, HTTPMethod, addParamSpecs, paramName, fillParamTypeSpec](QStringList* _pksByPathsStorage)->QJsonObject{
            QJsonObject PathInfo = QJsonObject({{"summary", APIObject->BaseMethod.Doc}});

            PathInfo["tags"] = QJsonArray({TagName});
            PathInfo["produces"] = QJsonArray({"application/json"});
            if (HTTPMethod != "get" && HTTPMethod != "delete"){
                quint8 RequiredFiles = 0;
                for (quint8 i=0; i< APIObject->BaseMethod.parameterCount(); ++i)
                    if (QMetaType::typeName(APIObject->BaseMethod.parameterType(i)) == QStringLiteral(PARAM_FILE))
                        RequiredFiles++;
                    else if (QMetaType::typeName(APIObject->BaseMethod.parameterType(i)) == QStringLiteral(PARAM_FILES))
                        RequiredFiles += 2;

                QJsonObject Properties;
                for (quint8 i=0; i< APIObject->BaseMethod.parameterCount(); ++i){
                    QString ParamType = QMetaType::typeName(APIObject->BaseMethod.parameterType(i));
                    if (     ParamType == PARAM_HEADERS
                          || ParamType == PARAM_REMOTE_IP
                          || ParamType == PARAM_COOKIES
                          || ParamType == PARAM_JWT
                          || ParamType == PARAM_PKSBYPATH
                          )
                        continue;

                    if (ParamType == PARAM_ORMFIELDS){
                        auto Items = APIObject->Parent->filterItems(HTTPMethod == "put"
                                                                    ? qhttp::EHTTP_PUT
                                                                    : HTTPMethod == "patch"
                                                                      ? qhttp::EHTTP_PATCH
                                                                      : qhttp::EHTTP_BIND);
                        foreach(auto Item, Items){
                            QJsonObject ParamSpecs;
                            intfAPIArgManipulator* ArgSpecs;
                            if(Item.parameterType()< TAPI_BASE_USER_DEFINED_TYPEID)
                                ArgSpecs = gOrderedMetaTypeInfo.at(Item.parameterType());
                            else
                                ArgSpecs = gUserDefinedTypesInfo.at(Item.parameterType()- TAPI_BASE_USER_DEFINED_TYPEID);

                            if (!ArgSpecs){
                                Item.registerTypeIfNotRegisterd(APIObject->Parent);
                                if(Item.parameterType()< TAPI_BASE_USER_DEFINED_TYPEID)
                                    ArgSpecs = gOrderedMetaTypeInfo.at(Item.parameterType());
                                else
                                    ArgSpecs = gUserDefinedTypesInfo.at(Item.parameterType()- TAPI_BASE_USER_DEFINED_TYPEID);
                            }

                            Q_ASSERT(ArgSpecs);
                            QVariant DefaultValue = QVariant();
                            if(Item.defaultValue() == QAuto)
                                continue;
                            if(Item.defaultValue() == QInvalid)
                                throw exTargomanAPI("Invalid Item exported for PUT method: " + Item.name());
                            if(HTTPMethod == "put"){
                                if(Item.defaultValue() == QNull)
                                    DefaultValue = QNull;
                                else if (Item.defaultValue() == QRequired)
                                    DefaultValue = QRequired;
                                else
                                    DefaultValue = Item.toString(Item.defaultValue());
                            }else if(Item.isReadOnly())
                                continue;

                            fillParamTypeSpec(ArgSpecs,
                                              APIObject->Parent->filterItems(),
                                              QMetaType::typeName(Item.parameterType()),
                                              ParamSpecs,
                                              DefaultValue,
                                              Item.isPrimaryKey() == true || HTTPMethod == "put"
                                              );
                            Properties[Item.name()] = ParamSpecs;
                        }
                    }
                    else {
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
                    addParamSpecs( Parameters, i, _pksByPathsStorage);


                if(RequiredFiles > 0){
                    PathInfo["consumes"] = QJsonArray({"multipart/form-data"});
                    for(auto Prop = Properties.begin(); Prop != Properties.end(); ++Prop){
                        QJsonObject PropVal = Prop.value().toObject();
                        PropVal.insert("name", Prop.key());
                        PropVal.insert("in", "formData");
                        Parameters.append(PropVal);
                    }
                }else{
                    QJsonArray RequiredFields;
                    for(auto PropertyIter = Properties.begin();
                        PropertyIter != Properties.end();
                        PropertyIter++
                        )
                        if(PropertyIter->toObject().value("required").toBool())
                            RequiredFields.append(PropertyIter.key());
                    PathInfo["consumes"] = QJsonArray({"application/json"});
                    Parameters.append(QJsonObject({
                                                      { "in", "body" },
                                                      { "name", "body" },
                                                      { "description", "Parameter Object" },
                                                      { "required", true },
                                                      { "schema", QJsonObject({
                                                            { "required", RequiredFields },
                                                            { "type", "object" },
                                                            { "properties", Properties },
                                                        }) },
                                                  }));
                }
                PathInfo["parameters"] = Parameters;
            }
            else {
                QJsonArray Parameters;
                for(quint8 i=0; i< APIObject->BaseMethod.parameterCount(); ++i)
                    addParamSpecs(Parameters, i, _pksByPathsStorage);

                if(Parameters.size())
                    PathInfo["parameters"] = Parameters;
            }

            static QJsonObject DefaultResponses = QJsonObject({
                                                                  {"200", QJsonObject({{"description", "Success."}})},
                                                                  {"400", QJsonObject({{"description", "Bad request."}})},
                                                                  {"404", QJsonObject({{"description", "Not found"}})},
                                                                  //{"5XX", QJsonObject({{"description", "Unexpected error"}})},
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

        auto add2Paths = [PathString, HTTPMethod](QJsonObject& PathsObject, const QJsonObject& _currPathMethodInfo, QStringList* _pksByPathsStorage, bool _multiPK = false){
            if(HTTPMethod == "delete" && _currPathMethodInfo.value("parameters").toArray().isEmpty())
                return;
            QString Path = PathString;
            if(_pksByPathsStorage){
                if (_multiPK)
                    Path += "/{" + _pksByPathsStorage->join("_1};{") + "_1},{" + _pksByPathsStorage->join("_2};{") + "_2},...";
                else
                    Path += "/{" + _pksByPathsStorage->join("};{") + "}";
            }

            if(PathsObject.contains(Path)){
                QJsonObject CurrPathObject = PathsObject.value(Path).toObject();
                CurrPathObject[HTTPMethod] = _currPathMethodInfo;
                PathsObject[Path] = CurrPathObject;
            }else
                PathsObject[Path] = QJsonObject({ {HTTPMethod, _currPathMethodInfo} });
        };

        if(APIObject->requiresPrimaryKey()){
            add2Paths(PathsObject, createPathInfo(&PKInPathStorage), &PKInPathStorage, false);
           /* if(HTTPMethod == "delete"){
                PKInPathStorage.clear();
                add2Paths(PathsObject, createPathInfo(&PKInPathStorage), &PKInPathStorage, true);
            }*/
        }

        if (!APIObject->requiresPrimaryKey() || HTTPMethod == "get")
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

        /*if(false && ModelDescription.size()){
            TagsArray.append(QJsonObject({
                                             {"name", TagName},
                                             {"description", QString("Model:\n" + ModelDescription.join("\n"))}
                                         }));
        }*/
    }

    OpenAPIGenerator::CachedJson["paths"] = PathsObject;
    //OpenAPIGenerator::CachedJson["tags"] = TagsArray;

    return OpenAPIGenerator::CachedJson;
}

} //namespace Targoman::API::Server
