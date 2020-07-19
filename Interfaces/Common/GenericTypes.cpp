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

#include <QRegularExpression>
#include <QUrl>

#include "QFieldValidator.h"

#include "intfAPIArgManipulator.h"
#include "GenericTypes.h"
#include "HTTPExceptions.hpp"

using namespace TAPI;
using namespace Targoman::API;
using namespace Targoman::API::ORM;

QList<intfAPIArgManipulator*> Server::gOrderedMetaTypeInfo;
QList<intfAPIArgManipulator*> Server::gUserDefinedTypesInfo;

TAPI_REGISTER_METATYPE(
            COMPLEXITY_Complex,
            TAPI, stuTable,
            [](const stuTable& _value) -> QVariant{return _value.toVariant();}
);

TAPI_REGISTER_METATYPE(
            COMPLEXITY_File,
            TAPI, stuFileInfo,
            [](const stuFileInfo& _value) -> QVariant{return _value.toVariant();},
            [](const QVariant& _value, const QByteArray& _param) -> stuFileInfo {stuFileInfo TempValue; return TempValue.fromVariant(_value, _param);}
);

TAPI_REGISTER_METATYPE(
            COMPLEXITY_File,
            TAPI, Files_t,
            [](const Files_t& _value) -> QVariant{ return _value.toVariant();},
            [](const QVariant& _value, const QByteArray& _param) -> Files_t {Files_t TempValue; return TempValue.fromVariant(_value, _param);}
);

TAPI_REGISTER_METATYPE(
            COMPLEXITY_Complex,
            TAPI, HEADERS_t,
            [](const HEADERS_t& _value) -> QVariant {return _value.toVariant();},
            [](const QVariant& _value, const QByteArray&) -> HEADERS_t {HEADERS_t  TempValue;return TempValue.fromVariant(_value);}
);

TAPI_REGISTER_METATYPE(
            COMPLEXITY_Complex,
            TAPI, COOKIES_t,
            [](const COOKIES_t& _value) -> QVariant {return _value.toVariant();},
            [](const QVariant& _value, const QByteArray&) -> COOKIES_t {COOKIES_t  TempValue;return TempValue.fromVariant(_value);}
);

TAPI_REGISTER_METATYPE(
            COMPLEXITY_Object,
            TAPI, JWT_t,
            nullptr,
            [](const QVariant& _value, const QByteArray& _paramName) -> JWT_t {
                QJsonObject Obj;
                if(_value.canConvert<QVariantMap>())
                    Obj = Obj.fromVariantMap(_value.value<QVariantMap>());

                if(Obj.isEmpty())
                    throw exHTTPBadRequest(_paramName + " is not a valid JWT object");
                return  *reinterpret_cast<JWT_t*>(&Obj);
            }
);

TAPI_REGISTER_METATYPE(
            COMPLEXITY_Complex,
            TAPI, ORMFields_t,
            nullptr,
            [](const QVariant& _value, const QByteArray&) -> ORMFields_t {return _value.toMap();}
);

TAPI_REGISTER_METATYPE(
            COMPLEXITY_Object,
            TAPI, JSON_t,
            [](const JSON_t& _value) -> QVariant {return _value;},
            [](const QVariant& _value, const QByteArray& _paramName) -> JSON_t {
                if(_value.isValid() == false)
                    return QJsonDocument();

                if(_value.canConvert<QVariantMap>() ||
                   _value.canConvert<QVariantList>() ||
                   _value.canConvert<double>())
                    return QJsonDocument::fromVariant(_value);

                if(_value.toString().isEmpty())
                    return QJsonDocument();

                QJsonParseError Error;
                QJsonDocument Doc;
                Doc = Doc.fromJson(_value.toString().toUtf8(), &Error);

                if(Error.error != QJsonParseError::NoError)
                    throw exHTTPBadRequest(_paramName + " is not a valid Json: <"+_value.toString()+"> " + Error.errorString());
                return  Doc;
            },
            [](const QList<ORM::clsORMField>&){ return "A valid JSON object"; },
            [](const QVariant& _value) {
                if(_value.isNull())
                    return QJsonDocument();
                QJsonParseError Error;
                QJsonDocument Doc;
                if(_value.canConvert<QVariantMap>() ||
                   _value.canConvert<QVariantList>() ||
                   _value.canConvert<double>())
                    Doc = QJsonDocument::fromVariant(_value);
                else
                  Doc = Doc.fromJson(_value.toString().toUtf8(), &Error);

                qDebug()<<"===========>"<<_value;

                if(Error.error != QJsonParseError::NoError)
                    throw exHTTPBadRequest("is not a valid Json: <"+_value.toString()+">" + Error.errorString());
                return Doc;
            },
            [](const QVariant& _val) {
                return QString::fromUtf8(QJsonDocument::fromVariant(_val).toJson(QJsonDocument::Compact));
            }

);

TAPI_REGISTER_METATYPE(
            COMPLEXITY_Object,
            TAPI, PrivObject_t,
            [](const PrivObject_t& _value) -> QVariant {return _value;},
            [](const QVariant& _value, const QByteArray& _paramName) -> PrivObject_t {
                if(_value.isValid() == false)
                    return QJsonDocument();

                QJsonDocument Doc;
                if(_value.canConvert<QVariantMap>() ||
                   _value.canConvert<QVariantList>() ||
                   _value.canConvert<double>())
                    Doc = QJsonDocument::fromVariant(_value);
                else if(_value.toString().isEmpty())
                    return QJsonDocument();
                else {
                    QJsonParseError Error;
                    QJsonDocument Doc;
                    Doc = Doc.fromJson(_value.toString().toUtf8(), &Error);

                    if(Error.error != QJsonParseError::NoError)
                        throw exHTTPBadRequest(_paramName + " is not a valid Json: <"+_value.toString()+">" + Error.errorString());
                }

                if(Doc.isNull() || Doc.isEmpty())
                    return Doc;
                if(Doc.isArray())
                    throw exHTTPBadRequest(_paramName + " is not a valid PrivObject: <"+_value.toString()+"> must be object not array");

                QJsonObject PrivObj = Doc.object();
                if(PrivObj.contains("ALL"))
                    throw exHTTPUnauthorized(_paramName + " is not a valid PrivObject: <"+_value.toString()+"> top level allowance is not valid");

                return  Doc;
            },
            [](const QList<ORM::clsORMField>&){ return "A valid Privilege JSON object"; },
            [](const QVariant& _value) {
                if(_value.isNull())
                    return QJsonDocument();
                QJsonParseError Error;
                QJsonDocument Doc;
                if(_value.canConvert<QVariantMap>() ||
                   _value.canConvert<QVariantList>() ||
                   _value.canConvert<double>())
                    Doc = QJsonDocument::fromVariant(_value);
                else
                  Doc = Doc.fromJson(_value.toString().toUtf8(), &Error);

                if(Error.error != QJsonParseError::NoError)
                    throw exHTTPBadRequest("is not a valid Privilege: <"+_value.toString()+">" + Error.errorString());
                return Doc;
            },
            [](const QVariant& _val) {
                return QString::fromUtf8(QJsonDocument::fromVariant(_val).toJson(QJsonDocument::Compact));
            }
);

TAPI_REGISTER_METATYPE(
            COMPLEXITY_String,
            TAPI, EncodedJWT_t,
            [](const EncodedJWT_t& _value) -> QVariant {return _value;},
            nullptr,
            [](const QList<ORM::clsORMField>&){ return "A signed JsonWebToken string"; }
);

TAPI_REGISTER_METATYPE(
            COMPLEXITY_String,
            TAPI, RemoteIP_t,
            [](const RemoteIP_t& _value) -> QVariant {return _value;},
            [](const QVariant& _value, const QByteArray&) -> RemoteIP_t {RemoteIP_t Value;Value=_value.toString();return  Value;}
);

TAPI_REGISTER_METATYPE(
            COMPLEXITY_String,
            TAPI, PKsByPath_t,
            [](const PKsByPath_t& _value) -> QVariant {return _value;},
            [](const QVariant& _value, const QByteArray&) -> PKsByPath_t {
                PKsByPath_t Value;
                QUrl URL = QUrl::fromPercentEncoding(("http://127.0.0.1/" + _value.toString()).toUtf8());
                Value=URL.path().remove(0,1);

                return  Value;
            }
);

TAPI_VALIDATION_REQUIRED_TYPE_IMPL(COMPLEXITY_String, TAPI, MD5_t, optional(QFV.md5()), _value, [](const QList<clsORMField>&){ return "A valid MD5 string"; });
TAPI_VALIDATION_REQUIRED_TYPE_IMPL(COMPLEXITY_String, TAPI, CommaSeparatedStringList_t, optional(QFV.asciiAlNum(false, ",")), _value, [](const QList<clsORMField>&){ return "A valid comma separated string list"; });
TAPI_VALIDATION_REQUIRED_TYPE_IMPL(COMPLEXITY_String, TAPI, Email_t, optional(QFV.email()), _value, [](const QList<clsORMField>&){ return "A valid email"; });
TAPI_VALIDATION_REQUIRED_TYPE_IMPL(COMPLEXITY_String, TAPI, Mobile_t, optional(QFV.mobile()), _value, [](const QList<clsORMField>&){ return "A valid mobile number with/without country code"; });
TAPI_VALIDATION_REQUIRED_TYPE_IMPL(COMPLEXITY_String, TAPI, IPv4_t, optional(QFV.ipv4()), _value, [](const QList<clsORMField>&){ return "A valid IP version 4"; });
TAPI_VALIDATION_REQUIRED_TYPE_IMPL(COMPLEXITY_String, TAPI, ISO639_2_t, optional(QFV.maxLenght(2).languageCode()), _value, [](const QList<clsORMField>&){ return "A valid ISO639 two-letter language code"; });
TAPI_VALIDATION_REQUIRED_TYPE_IMPL(COMPLEXITY_String, TAPI, Base64Image_t, optional(QFV.base64Image()), _value, [](const QList<clsORMField>&){ return "A valid base64 encoded png/jpg image"; });
TAPI_VALIDATION_REQUIRED_TYPE_IMPL(COMPLEXITY_String, TAPI, Sheba_t, optional(QFV.iban("IR")), _value, [](const QList<clsORMField>&){ return "A valid Iranian sheba code"; });
TAPI_VALIDATION_REQUIRED_TYPE_IMPL(COMPLEXITY_String, TAPI, Date_t, optional(QFV.date()), _value, [](const QList<clsORMField>&){ return "A valid gregorian date"; });
TAPI_VALIDATION_REQUIRED_TYPE_IMPL(COMPLEXITY_String, TAPI, Time_t, optional(QFV.time()), _value, [](const QList<clsORMField>&){ return "A valid time"; });
TAPI_VALIDATION_REQUIRED_TYPE_IMPL(COMPLEXITY_String, TAPI, DateTime_t, optional(QFV.dateTime()), _value, [](const QList<clsORMField>&){ return "A valid datetime"; });
TAPI_VALIDATION_REQUIRED_TYPE_IMPL(COMPLEXITY_String, TAPI, URL_t, optional(QFV.url()), _value, [](const QList<clsORMField>&){ return "A valid URL"; });
TAPI_VALIDATION_REQUIRED_TYPE_IMPL(COMPLEXITY_String, TAPI, PackageCode_t, optional(QFV.matches(QRegularExpression("[a-zA-Z\\-_0-9]{3,10}"))), _value, [](const QList<clsORMField>&){ return "A valid PackageCode"; });
TAPI_VALIDATION_REQUIRED_TYPE_IMPL(COMPLEXITY_String, TAPI, DiscountCode_t, optional(QFV.matches(QRegularExpression("[a-zA-Z\\-_0-9]{5,10}"))), _value, [](const QList<clsORMField>&){ return "A valid DiscountCode"; });

TAPI_REGISTER_TARGOMAN_ENUM(TAPI, enuGenericStatus);


stuFileInfo stuFileInfo::fromVariant(const QVariant& _value, const QByteArray& _paramName)
{
     QVariantMap Value = _value.toMap();
     if(Value.isEmpty() || !Value.contains("name") || !Value.contains("tmpname") || !Value.contains("size") || !Value.contains("mime"))
         throw exHTTPBadRequest(_paramName + " is not valid File information");

     return stuFileInfo(
                 Value["name"].toString(),
                 Value["tmpname"].toString(),
                 Value["size"].toULongLong(),
                 Value["mime"].toString()
                 );
}

QVariant stuFileInfo::toVariant() const{
    return QVariantMap({
                      {"name", this->Name},
                      {"tmpname", this->TempName},
                      {"size", this->Size},
                      {"mime", this->Mime},
                       });
}

QVariant Files_t::toVariant() const
{
    QVariantList Files;
    foreach(auto Val, *this)
        Files.append(Val.toVariant());
    return Files;
}

Files_t& Files_t::fromVariant(const QVariant& _value, const QByteArray& _paramName)
{
    foreach(auto ListItem, _value.toList())
        this->append(TAPI::stuFileInfo::fromVariant(ListItem, _paramName));
    return *this;
}
