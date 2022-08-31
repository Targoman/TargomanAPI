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

#include "GenericTypes.h"
#include "tmplAPIArg.h"

#include <QRegularExpression>
#include <QUrl>
#include <QJsonObject>
#include <QJsonArray>

#include "libQFieldValidator/QFieldValidator.h"

#include "base.h"

#include "intfAPIArgManipulator.h"
#include "HTTPExceptions.hpp"

#include "Interfaces/Helpers/URLHelper.h"
using namespace Targoman::API::Helpers;

#include "Interfaces/DBM/clsORMField.h"
using namespace Targoman::API::DBM;

using namespace TAPI;
using namespace Targoman::API;
using namespace Targoman::API::Common;

namespace Targoman::API::Server {
    QList<intfAPIArgManipulator*> gOrderedMetaTypeInfo;
    QList<intfAPIArgManipulator*> gUserDefinedTypesInfo;
} // namespace Targoman::API::Server

TAPI_REGISTER_METATYPE(
    /* complexity         */ COMPLEXITY_Complex,
    /* namespace          */ TAPI,
    /* type               */ stuTable,
    /* fnToVariant        */ [](const stuTable& _value) -> QVariant{return _value.toVariant();}
);

TAPI_REGISTER_METATYPE(
    /* complexity         */ COMPLEXITY_File,
    /* namespace          */ TAPI,
    /* type               */ stuFileInfo,
    /* fnToVariant        */ [](const stuFileInfo& _value) -> QVariant{return _value.toVariant();},
    /* fnFromVariant      */ [](const QVariant& _value, const QByteArray& _param) -> stuFileInfo {stuFileInfo TempValue; return TempValue.fromVariant(_value, _param);}
);

TAPI_REGISTER_METATYPE(
    /* complexity         */ COMPLEXITY_File,
    /* namespace          */ TAPI,
    /* type               */ Files_t,
    /* fnToVariant        */ [](const Files_t& _value) -> QVariant{ return _value.toVariant();},
    /* fnFromVariant      */ [](const QVariant& _value, const QByteArray& _param) -> Files_t {Files_t TempValue; return TempValue.fromVariant(_value, _param);}
);

TAPI_REGISTER_METATYPE(
    /* complexity         */ COMPLEXITY_Complex,
    /* namespace          */ TAPI,
    /* type               */ RawData_t,
    /* fnToVariant        */ [](const RawData_t& _value) -> QVariant{return _value.toVariant();}
);

TAPI_REGISTER_METATYPE(
    /* complexity         */ COMPLEXITY_Complex,
    /* namespace          */ TAPI,
    /* type               */ FileData_t,
    /* fnToVariant        */ [](const FileData_t &_value) -> QVariant { return _value.toVariant(); }
);

TAPI_REGISTER_METATYPE(
    /* complexity         */ COMPLEXITY_Complex,
    /* namespace          */ TAPI,
    /* type               */ ResponseRedirect_t,
    /* fnToVariant        */ [](const ResponseRedirect_t &_value) -> QVariant { return _value.toVariant(); }
);

TAPI_REGISTER_METATYPE(
    /* complexity         */ COMPLEXITY_Complex,
    /* namespace          */ TAPI,
    /* type               */ HEADERS_t,
    /* fnToVariant        */ [](const HEADERS_t& _value) -> QVariant {return _value.toVariant();},
    /* fnFromVariant      */ [](const QVariant& _value, const QByteArray&) -> HEADERS_t {HEADERS_t  TempValue;return TempValue.fromVariant(_value);}
);

TAPI_REGISTER_METATYPE(
    /* complexity         */ COMPLEXITY_Complex,
    /* namespace          */ TAPI,
    /* type               */ COOKIES_t,
    /* fnToVariant        */ [](const COOKIES_t& _value) -> QVariant {return _value.toVariant();},
    /* fnFromVariant      */ [](const QVariant& _value, const QByteArray&) -> COOKIES_t {COOKIES_t  TempValue;return TempValue.fromVariant(_value);}
);

TAPI_REGISTER_METATYPE(
    /* complexity         */ COMPLEXITY_Object,
    /* namespace          */ TAPI,
    /* type               */ JWT_t,
    /* fnToVariant        */ nullptr,
    /* fnFromVariant      */ [](const QVariant& _value, Q_DECL_UNUSED const QString& _paramName = "") -> JWT_t {
        QJsonObject Obj;
        if (_value.canConvert<QVariantMap>())
            Obj = Obj.fromVariantMap(_value.value<QVariantMap>());

        if (Obj.isEmpty())
            throw exHTTPBadRequest(_paramName + " is not a valid JWT object");
        return  *reinterpret_cast<JWT_t*>(&Obj);
    }
);

TAPI_REGISTER_METATYPE(
    /* complexity         */ COMPLEXITY_Complex,
    /* namespace          */ TAPI,
    /* type               */ ORMFields_t,
    /* fnToVariant        */ nullptr,
    /* fnFromVariant      */ [](const QVariant& _value, const QByteArray&) -> ORMFields_t {return _value.toMap();}
);

TAPI_REGISTER_METATYPE_TYPE_JSON_DERIVED(
    /* namespace          */ TAPI,
    /* type               */ JSON_t
);

TAPI_REGISTER_METATYPE_TYPE_JSON_DERIVED(
    /* namespace          */ TAPI,
    /* type               */ PrivObject_t
);
/*
    [](const PrivObject_t& _value) -> QVariant {return _value;}
    [](const QVariant& _value, const QByteArray& _paramName) -> PrivObject_t {
        if (_value.isValid() == false)
            return QJsonDocument();

        QJsonDocument Doc;
        if (_value.canConvert<QVariantMap>() ||
           _value.canConvert<QVariantList>() ||
           _value.canConvert<double>())
            Doc = QJsonDocument::fromVariant(_value);
        else if (_value.toString().isEmpty())
            return QJsonDocument();
        else {
            QJsonParseError Error;
            QJsonDocument Doc;
            Doc = Doc.fromJson(_value.toString().toUtf8(), &Error);

            if (Error.error != QJsonParseError::NoError)
                throw exHTTPBadRequest(_paramName + " is not a valid Json: <"+_value.toString()+">" + Error.errorString());
        }

        if (Doc.isNull() || Doc.isEmpty())
            return Doc;
        if (Doc.isArray())
            throw exHTTPBadRequest(_paramName + " is not a valid PrivObject: <"+_value.toString()+"> must be object not array");

        QJsonObject PrivObj = Doc.object();
        if (PrivObj.contains("ALL"))
            throw exHTTPUnauthorized(_paramName + " is not a valid PrivObject: <"+_value.toString()+"> top level allowance is not valid");

        return  Doc;
    },
    [](const QList<DBM::clsORMField>&) { return "A valid Privilege JSON object"; },
    [](const QVariant& _value) {
        if (_value.isNull() || _value.toString().isEmpty()) //OJO why?!!!
            return QJsonDocument();
        QJsonParseError Error;
        QJsonDocument Doc;
        if (_value.canConvert<QVariantMap>() ||
           _value.canConvert<QVariantList>() ||
           _value.canConvert<double>())
            Doc = QJsonDocument::fromVariant(_value);
        else
          Doc = Doc.fromJson(_value.toString().toUtf8(), &Error);

        if (Error.error != QJsonParseError::NoError)
            throw exHTTPBadRequest("is not a valid Privilege: <"+_value.toString()+">" + Error.errorString());
        return Doc;
    },
    [](const QVariant& _val) {
        return QString::fromUtf8(QJsonDocument::fromVariant(_val).toJson(QJsonDocument::Compact));
    }
);
*/

//TAPI_REGISTER_METATYPE_TYPE_JSON_DERIVED(
//    /* namespace          */ TAPI,
//    /* type               */ MultiLanguageString_t
//);

TAPI_REGISTER_METATYPE_TYPE_JSON_DERIVED(
    /* namespace          */ TAPI,
    /* type               */ SaleableAdditive_t
);

/*
            [](const SaleableAdditive_t& _value) -> QVariant {return _value;}
            [](const QVariant& _value, const QByteArray& _paramName) -> SaleableAdditive_t {
                if (_value.isValid() == false)
                    return QJsonDocument();

                QJsonDocument Doc;
                if (_value.canConvert<QVariantMap>() ||
                   _value.canConvert<QVariantList>() ||
                   _value.canConvert<double>())
                    Doc = QJsonDocument::fromVariant(_value);
                else if (_value.toString().isEmpty())
                    return QJsonDocument();
                else {
                    QJsonParseError Error;
                    QJsonDocument Doc;
                    Doc = Doc.fromJson(_value.toString().toUtf8(), &Error);

                    if (Error.error != QJsonParseError::NoError)
                        throw exHTTPBadRequest(_paramName + " is not a valid Json: <"+_value.toString()+">" + Error.errorString());
                }

                if (Doc.isNull() || Doc.isEmpty())
                    return Doc;
                if (Doc.isArray())
                    throw exHTTPBadRequest(_paramName + " is not a valid PrivObject: <"+_value.toString()+"> must be object not array");

                QJsonObject PrivObj = Doc.object();





/// ///@TODO:





throw Targoman::Common::exTargomanMustBeImplemented("SaleableAdditive_t not implemented yet!");

                return  Doc;
            },
            [](const QList<DBM::clsORMField>&) { return "A valid Saleable Additive JSON object"; },
            [](const QVariant& _value) {




/// ///@TODO:






                if (_value.isNull() || _value.toString().isEmpty()) //OJO why?!!!
                    return QJsonDocument();
                QJsonParseError Error;
                QJsonDocument Doc;
                if (_value.canConvert<QVariantMap>() ||
                   _value.canConvert<QVariantList>() ||
                   _value.canConvert<double>())
                    Doc = QJsonDocument::fromVariant(_value);
                else
                  Doc = Doc.fromJson(_value.toString().toUtf8(), &Error);

                if (Error.error != QJsonParseError::NoError)
                    throw exHTTPBadRequest("is not a valid Saleable Additive: <"+_value.toString()+">" + Error.errorString());
                return Doc;
            },
            [](const QVariant& _val) {
                return QString::fromUtf8(QJsonDocument::fromVariant(_val).toJson(QJsonDocument::Compact));
            }
);
*/

TAPI_REGISTER_METATYPE(
    /* complexity         */ COMPLEXITY_String,
    /* namespace          */ TAPI,
    /* type               */ EncodedJWT_t,
    /* fnToVariant        */ [](const EncodedJWT_t& _value) -> QVariant {return _value;},
    /* fnFromVariant      */ nullptr,
    [](const QList<DBM::clsORMField>&) { return "A signed JsonWebToken string"; }
);

TAPI_REGISTER_METATYPE(
    /* complexity         */ COMPLEXITY_String,
    /* namespace          */ TAPI,
    /* type               */ RemoteIP_t,
    /* fnToVariant        */ [](const RemoteIP_t& _value) -> QVariant {return _value;},
    /* fnFromVariant      */ [](const QVariant& _value, const QByteArray&) -> RemoteIP_t {RemoteIP_t Value;Value=_value.toString();return  Value;}
);

TAPI_REGISTER_METATYPE(
    /* complexity         */ COMPLEXITY_String,
    /* namespace          */ TAPI,
    /* type               */ PKsByPath_t,
    /* fnToVariant        */ [](const PKsByPath_t& _value) -> QVariant {return _value;},
    /* fnFromVariant      */ [](const QVariant& _value, const QByteArray&) -> PKsByPath_t {
        PKsByPath_t Value;
        QUrl URL = QUrl::fromPercentEncoding(("http://127.0.0.1/" + _value.toString()).toUtf8());
        Value=URL.path().remove(0,1);
        return  Value;
    }
);

TAPI_REGISTER_METATYPE(
    /* complexity         */ COMPLEXITY_String,
    /* namespace          */ TAPI,
    /* type               */ CouponCode_t,
    /* fnToVariant        */ [](const CouponCode_t& _value) -> QVariant {return _value.mid(0, _value.indexOf('!'));},
    /* fnFromVariant      */ [](const QVariant& _value, const QByteArray&) -> CouponCode_t {
        auto ValueStr = _value.toString();
        return ValueStr.mid(0, ValueStr.indexOf('!'));
    }
);

TAPI_REGISTER_METATYPE(
    /* complexity         */ COMPLEXITY_String,
    /* namespace          */ TAPI,
    /* type               */ Date_t,
    /* fnToVariant        */ [](const Date_t& _value) -> QVariant {return _value;},
    /* fnFromVariant      */ [](const QVariant& _value, Q_DECL_UNUSED const QString& _paramName = "") -> Date_t {
        if (_value.canConvert<QDate>() == false)
            throw exHTTPBadRequest(_paramName + " is not a valid Date: <"+_value.toString()+">");
        return _value.toDate();
    }
)

TAPI_REGISTER_METATYPE(
    /* complexity         */ COMPLEXITY_String,
    /* namespace          */ TAPI,
    /* type               */ Time_t,
    /* fnToVariant        */ [](const Time_t& _value) -> QVariant {return _value;},
    /* fnFromVariant      */ [](const QVariant& _value, Q_DECL_UNUSED const QString& _paramName = "") -> Time_t {
        if (_value.canConvert<QDate>() == false)
            throw exHTTPBadRequest(_paramName + " is not a valid Time: <"+_value.toString()+">");
        return _value.toTime();
    }
)

TAPI_REGISTER_METATYPE(
    /* complexity         */ COMPLEXITY_String,
    /* namespace          */ TAPI,
    /* type               */ DateTime_t,
    /* fnToVariant        */ [](const DateTime_t& _value) -> QVariant { return _value; },
    /* fnFromVariant      */ [](const QVariant& _value, Q_DECL_UNUSED const QString& _paramName = "") -> DateTime_t {
        if (_value.canConvert<QDate>() == false) {
            print_stacktrace();
            throw exHTTPBadRequest(_paramName + " is not a valid DateTime: <" + _value.toString() + ">");
        }
        return _value.toDateTime();
    }
)

TAPI_VALIDATION_REQUIRED_TYPE_IMPL(COMPLEXITY_String, TAPI, MD5_t,                      optional(QFV.md5()),                                                       _value, [](const QList<clsORMField>&) { return "A valid MD5 string"; });
TAPI_VALIDATION_REQUIRED_TYPE_IMPL(COMPLEXITY_String, TAPI, CommaSeparatedStringList_t, optional(QFV.asciiAlNum(false, ",")),                                      _value, [](const QList<clsORMField>&) { return "A valid comma separated string list"; });
TAPI_VALIDATION_REQUIRED_TYPE_IMPL(COMPLEXITY_String, TAPI, Email_t,                    optional(QFV.email()),                                                     _value, [](const QList<clsORMField>&) { return "A valid email"; });
TAPI_VALIDATION_REQUIRED_TYPE_IMPL(COMPLEXITY_String, TAPI, Mobile_t,                   optional(QFV.mobile()),                                                    _value, [](const QList<clsORMField>&) { return "A valid mobile number with/without country code"; });
TAPI_VALIDATION_REQUIRED_TYPE_IMPL(COMPLEXITY_String, TAPI, IPv4_t,                     optional(QFV.ipv4()),                                                      _value, [](const QList<clsORMField>&) { return "A valid IP version 4"; });
TAPI_VALIDATION_REQUIRED_TYPE_IMPL(COMPLEXITY_String, TAPI, ISO639_2_t,                 optional(QFV.maxLenght(2).languageCode()),                                 _value, [](const QList<clsORMField>&) { return "A valid ISO639 two-letter language code"; });
TAPI_VALIDATION_REQUIRED_TYPE_IMPL(COMPLEXITY_String, TAPI, Base64Image_t,              optional(QFV.base64Image()),                                               _value, [](const QList<clsORMField>&) { return "A valid base64 encoded png/jpg image"; });
TAPI_VALIDATION_REQUIRED_TYPE_IMPL(COMPLEXITY_String, TAPI, Sheba_t,                    optional(QFV.iban("IR")),                                                  _value, [](const QList<clsORMField>&) { return "A valid Iranian sheba code"; });
///@TODO: Ether/Bitcoin validator
TAPI_VALIDATION_REQUIRED_TYPE_IMPL(COMPLEXITY_String, TAPI, Ether_t,                    optional(QFV.allwaysValid()),                                              _value, [](const QList<clsORMField>&) { return "A valid ethercoin address"; });
TAPI_VALIDATION_REQUIRED_TYPE_IMPL(COMPLEXITY_String, TAPI, URL_t,                      optional(QFV.url()),                                                       _value, [](const QList<clsORMField>&) { return "A valid URL"; });
TAPI_VALIDATION_REQUIRED_TYPE_IMPL(COMPLEXITY_String, TAPI, ProductCode_t,              optional(QFV.matches(QRegularExpression("[a-zA-Z\\-_0-9]{3,10}"))),        _value, [](const QList<clsORMField>&) { return "A valid ProductCode"; });
TAPI_VALIDATION_REQUIRED_TYPE_IMPL(COMPLEXITY_String, TAPI, SaleableCode_t,             optional(QFV.matches(QRegularExpression("[a-zA-Z\\-_0-9]{3,10}"))),        _value, [](const QList<clsORMField>&) { return "A valid SaleableCode"; });
TAPI_VALIDATION_REQUIRED_TYPE_IMPL(COMPLEXITY_String, TAPI, DBTinyText_t,               optional(QFV.matches(QRegularExpression("(?s).*")).maxLenght(255)),        _value, [](const QList<clsORMField>&) { return "A valid tiny text"; });
TAPI_VALIDATION_REQUIRED_TYPE_IMPL(COMPLEXITY_String, TAPI, DBText_t,                   optional(QFV.matches(QRegularExpression("(?s).*")).maxLenght(65535)),      _value, [](const QList<clsORMField>&) { return "A valid text"; });
TAPI_VALIDATION_REQUIRED_TYPE_IMPL(COMPLEXITY_String, TAPI, DBMediumText_t,             optional(QFV.matches(QRegularExpression("(?s).*")).maxLenght(16777215)),   _value, [](const QList<clsORMField>&) { return "A valid medium text"; });
TAPI_VALIDATION_REQUIRED_TYPE_IMPL(COMPLEXITY_String, TAPI, DBLongText_t,               optional(QFV.matches(QRegularExpression("(?s).*")).maxLenght(4294967295)), _value, [](const QList<clsORMField>&) { return "A valid long text"; });
TAPI_VALIDATION_REQUIRED_TYPE_IMPL(COMPLEXITY_String, TAPI, String_t,                   optional(QFV.matches(QRegularExpression("(?s).*"))),                       _value, [](const QList<clsORMField>&) { return "A valid String"; });

//TAPI_REGISTER_TARGOMAN_ENUM(TAPI, enuTokenActorType);
TAPI_REGISTER_TARGOMAN_ENUM(TAPI, enuGenericStatus);

/**********************************************************************************/
QVariant stuTable::toVariant(bool _compactList) const {

    if ((_compactList == false) && (this->HasCount == false))
        return this->Rows;

    QVariantMap Result;

    if (_compactList) {
        QStringList Headers;
        QVariantList RowsArray;

        int RowIndex = 0;
        foreach (QVariant Row, this->Rows) {

            QVariantMap RowMap = Row.toMap();

            QVariantList OneRowsAsArray;

            for (auto It = RowMap.constBegin(); It != RowMap.constEnd(); It++) {
                QString ColName = It.key();
                QVariant ColValue = It.value();

                if (RowIndex == 0) {
                    Headers.append(ColName);
                }

                OneRowsAsArray.append(ColValue);
            }

            RowsArray.append(QVariant(OneRowsAsArray));

            ++RowIndex;
        }

        if (Headers.isEmpty() == false)
            Result.insert("cols", Headers);

        Result.insert("rows", RowsArray);
    } else
        Result.insert("rows", this->Rows);

    if (this->HasCount) {
        Result.insert("totalRows", this->TotalRows);
        Result.insert("pageCount", this->PageCount);
        Result.insert("hasMore", this->HasMore);
    }

    return Result;
}

/**********************************************************************************/
stuFileInfo stuFileInfo::fromVariant(const QVariant& _value, const QByteArray& _paramName) {
     QVariantMap Value = _value.toMap();
     if (Value.isEmpty() || !Value.contains("name") || !Value.contains("tmpname") || !Value.contains("size") || !Value.contains("mime"))
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
                           { "name", this->Name },
                           { "tmpname", this->TempName },
                           { "size", this->Size },
                           { "mime", this->Mime },
                       });
}

/**********************************************************************************/
QVariant Files_t::toVariant() const
{
    QVariantList Files;
    foreach (auto Val, *this)
        Files.append(Val.toVariant());
    return Files;
}

Files_t& Files_t::fromVariant(const QVariant& _value, const QByteArray& _paramName) {
    foreach (auto ListItem, _value.toList())
        this->append(TAPI::stuFileInfo::fromVariant(ListItem, _paramName));
    return *this;
}

/**********************************************************************************/
RawData_t::RawData_t(const QByteArray& _data, const QString& _mime) :
    Mime(_mime),
    Data(_data) { ; }

QVariant RawData_t::toVariant() const {
    return QVariant::fromValue(*this);
}

/**********************************************************************************/
FileData_t::FileData_t(const QString &_fileName) :
    FileName(_fileName)
{
    FileName = FileName.replace(QRegularExpression("//+"), "/");
}

QVariant FileData_t::toVariant() const {
    return QVariant::fromValue(*this);
}

/**********************************************************************************/
ResponseRedirect_t::ResponseRedirect_t(const QString &_url,
                                       bool _appendBase,
                                       qhttp::TStatusCode _statusCode) :
    Url(_url),
    AppendBase(_appendBase),
    StatusCode(_statusCode)
{
    this->Url = URLHelper::normalize(_url);
}

QVariant ResponseRedirect_t::toVariant() const {
    return QVariant::fromValue(*this);
}
