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
 * @author S.Mehran M.Ziabary <ziabary@targoman.com>
 */

#include <QRegularExpression>
#include <QUrl>

#include "QFieldValidator.h"

#include "intfAPIArgManipulator.h"
#include "GenericTypes.h"
#include "HTTPExceptions.h"

namespace TAPI {

using namespace Targoman::API;
using namespace ORM;

static bool GenericTypesRegistered = false;

void registerGenericTypes()
{
    if(GenericTypesRegistered)
        return;

    GenericTypesRegistered = true;

    TAPI_REGISTER_METATYPE(
                COMPLEXITY_Complex,
                TAPI::stuTable,
                [](const TAPI::stuTable& _value) -> QVariant{return _value.toVariant();},
                nullptr
    );

    TAPI_REGISTER_METATYPE(
                COMPLEXITY_File,
                TAPI::stuFileInfo,
                [](const TAPI::stuFileInfo& _value) -> QVariant{return _value.toVariant();},
                [](const QVariant& _value, const QByteArray& _param) -> TAPI::stuFileInfo {TAPI::stuFileInfo TempValue; return TempValue.fromVariant(_value, _param);}
    );

    TAPI_REGISTER_METATYPE(
                COMPLEXITY_File,
                TAPI::Files_t,
                [](const TAPI::Files_t& _value) -> QVariant{ return _value.toVariant();},
                [](const QVariant& _value, const QByteArray& _param) -> TAPI::Files_t {TAPI::Files_t TempValue; return TempValue.fromVariant(_value, _param);}
    );

    TAPI_REGISTER_METATYPE(
                COMPLEXITY_Complex,
                TAPI::COOKIES_t,
                [](const TAPI::COOKIES_t& _value) -> QVariant {return _value.toVariant();},
                [](const QVariant& _value, const QByteArray&) -> TAPI::COOKIES_t {TAPI::COOKIES_t  TempValue;return TempValue.fromVariant(_value);}
    );

    TAPI_REGISTER_METATYPE(
                COMPLEXITY_Complex,
                TAPI::HEADERS_t,
                [](const TAPI::HEADERS_t& _value) -> QVariant {return _value.toVariant();},
                [](const QVariant& _value, const QByteArray&) -> TAPI::HEADERS_t {TAPI::HEADERS_t  TempValue;return TempValue.fromVariant(_value);}
    );

    TAPI_REGISTER_METATYPE(
                COMPLEXITY_String,
                TAPI::JWT_t,
                nullptr,
                [](const QVariant& _value, const QByteArray& _paramName) -> TAPI::JWT_t {
                    QJsonObject Obj;
                    if(_value.canConvert<QVariantMap>())
                        Obj = Obj.fromVariantMap(_value.value<QVariantMap>());

                    if(Obj.isEmpty())
                        throw exHTTPBadRequest(_paramName + " is not a valid JWT object");
                    return  *reinterpret_cast<TAPI::JWT_t*>(&Obj);
                }
    );

    TAPI_REGISTER_METATYPE(
                COMPLEXITY_Complex,
                TAPI::ORMFilters_t,
                nullptr,
                [](const QVariant& _value, const QByteArray&) -> TAPI::ORMFilters_t {return _value.toMap();}
    );

    TAPI_REGISTER_METATYPE(
                COMPLEXITY_String,
                TAPI::JSON_t,
                [](const TAPI::JSON_t& _value) -> QVariant {return _value;},
                [](const QVariant& _value, const QByteArray& _paramName) -> TAPI::JSON_t {
                    if(_value.isValid() == false)
                        return QJsonDocument();

                    if(_value.canConvert<QVariantMap>() ||
                       _value.canConvert<QVariantList>() ||
                       _value.canConvert<double>())
                        return QJsonDocument::fromVariant(_value);

                    QJsonParseError Error;
                    QJsonDocument Doc;
                    Doc = Doc.fromJson(_value.toString().toUtf8(), &Error);

                    if(Error.error != QJsonParseError::NoError)
                        throw exHTTPBadRequest(_paramName + " is not a valid Json: <"+_value.toString()+">" + Error.errorString());
                    return  Doc;
                },
                nullptr,
                [](const QList<ORM::clsORMField>&){ return "A valid JSON string"; }
    );

    TAPI_REGISTER_METATYPE(
                COMPLEXITY_String,
                TAPI::EncodedJWT_t,
                [](const TAPI::EncodedJWT_t& _value) -> QVariant {return _value;},
                nullptr,
                nullptr,
                [](const QList<ORM::clsORMField>&){ return "A signed JsonWebToken string"; }
    );

    TAPI_REGISTER_METATYPE(
                COMPLEXITY_String,
                TAPI::RemoteIP_t,
                [](const TAPI::RemoteIP_t& _value) -> QVariant {return _value;},
                [](const QVariant& _value, const QByteArray&) -> TAPI::RemoteIP_t {TAPI::RemoteIP_t Value;Value=_value.toString();return  Value;}
    );


    TAPI_REGISTER_METATYPE(
                COMPLEXITY_String,
                TAPI::ExtraPath_t,
                [](const TAPI::ExtraPath_t& _value) -> QVariant {return _value;},
                [](const QVariant& _value, const QByteArray&) -> TAPI::ExtraPath_t {
                    TAPI::ExtraPath_t Value;
                    QUrl URL = QUrl::fromPercentEncoding(("http://127.0.0.1/" + _value.toString()).toUtf8());
                    Value=URL.path().remove(0,1);

                    return  Value;
                }
    );

    TAPI_VALIDATION_REQUIRED_TYPE_IMPL(COMPLEXITY_String, TAPI::MD5_t, optional(QFV.md5()), _value, [](const QList<clsORMField>&){ return "A valid MD5 string"; });
    TAPI_VALIDATION_REQUIRED_TYPE_IMPL(COMPLEXITY_String, TAPI::Email_t, optional(QFV.email()), _value, [](const QList<clsORMField>&){ return "A valid email"; });
    TAPI_VALIDATION_REQUIRED_TYPE_IMPL(COMPLEXITY_String, TAPI::Mobile_t, optional(QFV.mobile()), _value, [](const QList<clsORMField>&){ return "A valid mobile number with/without country code"; });
    TAPI_VALIDATION_REQUIRED_TYPE_IMPL(COMPLEXITY_String, TAPI::IPv4_t, optional(QFV.ipv4()), _value, [](const QList<clsORMField>&){ return "A valid IP version 4"; });
    TAPI_VALIDATION_REQUIRED_TYPE_IMPL(COMPLEXITY_String, TAPI::ISO639_2_t, optional(QFV.maxLenght(2).languageCode()), _value, [](const QList<clsORMField>&){ return "A valid ISO639 two-letter language code"; });
    TAPI_VALIDATION_REQUIRED_TYPE_IMPL(COMPLEXITY_String, TAPI::Base64Image_t, optional(QFV.base64Image()), _value, [](const QList<clsORMField>&){ return "A valid base64 encoded png/jpg image"; });
    TAPI_VALIDATION_REQUIRED_TYPE_IMPL(COMPLEXITY_String, TAPI::Sheba_t, optional(QFV.iban()), _value, [](const QList<clsORMField>&){ return "A valid Iranian sheba code"; });
    TAPI_VALIDATION_REQUIRED_TYPE_IMPL(COMPLEXITY_String, TAPI::Date_t, optional(QFV.date()), _value, [](const QList<clsORMField>&){ return "A valid gregorian date"; });
    TAPI_VALIDATION_REQUIRED_TYPE_IMPL(COMPLEXITY_String, TAPI::Time_t, optional(QFV.time()), _value, [](const QList<clsORMField>&){ return "A valid time"; });
    TAPI_VALIDATION_REQUIRED_TYPE_IMPL(COMPLEXITY_String, TAPI::DateTime_t, optional(QFV.dateTime()), _value, [](const QList<clsORMField>&){ return "A valid datetime"; });
}

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

Files_t Files_t::fromVariant(const QVariant& _value, const QByteArray& _paramName) const
{
    Files_t Files;
    foreach(auto ListItem, _value.toList())
        Files.append(TAPI::stuFileInfo::fromVariant(ListItem, _paramName));
    return Files;
}

}

