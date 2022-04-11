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

#ifndef TAPI_GENERICTYPES_H
#define TAPI_GENERICTYPES_H

#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QUrl>

#include "QHttp/qhttpfwd.hpp"
#include "libTargomanCommon/clsCountAndSpeed.h"
#include "Interfaces/Common/tmplAPIArg.h"
#include "Interfaces/Common/HTTPExceptions.hpp"

namespace Targoman::API::Server {

extern QList<Targoman::API::Common::intfAPIArgManipulator*> gOrderedMetaTypeInfo;
extern QList<Targoman::API::Common::intfAPIArgManipulator*> gUserDefinedTypesInfo;

} //namespace Targoman::API::Server

//I used TAPI as namespace in order to make Targoman::API shorter
namespace TAPI {

/**
 * @brief The stuStatistics struct holds server statistics about APIs
 */
struct stuStatistics {
    Targoman::Common::clsCountAndSpeed Connections;
    Targoman::Common::clsCountAndSpeed WSConnections;
    Targoman::Common::clsCountAndSpeed Errors;
    Targoman::Common::clsCountAndSpeed Blocked;
    Targoman::Common::clsCountAndSpeed Success;

    QHash<QByteArray, Targoman::Common::clsCountAndSpeed> APICallsStats;
    QHash<QByteArray, Targoman::Common::clsCountAndSpeed> APIInternalCacheStats;
    QHash<QByteArray, Targoman::Common::clsCountAndSpeed> APICentralCacheStats;

    QJsonObject toJson(bool _full = false) {
        auto count2Json = [](const Targoman::Common::clsCountAndSpeed& _value) {
            return QJsonObject({
                                   {"count", static_cast<double>(_value.count())},
                                   {"cps", _value.countPerSecond()}
                               });
        };
        auto hashToJson = [count2Json](const QHash<QByteArray, Targoman::Common::clsCountAndSpeed>& _hash) {
            QJsonObject RetObj;
            for (auto Iter=_hash.begin(); Iter != _hash.end(); ++Iter)
                RetObj.insert(Iter.key(), count2Json(Iter.value()));
            return RetObj;
        };
        QJsonObject Stats = {
            {"Connections", count2Json(this->Connections)},
            {"WSConnections", count2Json(this->WSConnections)},
            {"Errors", count2Json(this->Errors)},
            {"Blocked", count2Json(this->Blocked)},
            {"Success", count2Json(this->Success)},
        };

        if (_full) {
            Stats.insert("APICallsStats",           hashToJson(APICallsStats));
            Stats.insert("APIInternalCacheStats",   hashToJson(APIInternalCacheStats));
            Stats.insert("APICentralCacheStats",    hashToJson(APICentralCacheStats));
        }
        return  Stats;
    }
};

/**********************************************************************/
/**
 * @brief The stuTable struct
 */
struct stuTable {
    qint64 TotalRows;
    QVariantList Rows;
    stuTable(qint64 _totalRows = -1, const QVariantList& _rows = QVariantList()) :
        TotalRows(_totalRows),
        Rows(_rows)
    { ; }
    QVariant toVariant() const{
      return QVariantMap({
                           {"rows", this->Rows},
                           {"totalRows", this->TotalRows}
                         });
    }
};

struct stuFileInfo {
  QString Name;
  QString TempName;
  quint64 Size;
  QString Mime;

  stuFileInfo(const QString& _name = {}, const QString& _tmpName = {}, quint64 _size = 0, const QString& _mime = {}) :
    Name(_name),
    TempName(_tmpName),
    Size(_size),
    Mime(_mime)
  { ; }

  stuFileInfo(const stuFileInfo& _other) :
      Name(_other.Name),
      TempName(_other.TempName),
      Size(_other.Size),
      Mime(_other.Mime)
  { ; }

  QVariant toVariant() const;

  static stuFileInfo fromVariant(const QVariant& _value, const QByteArray& _paramName = {});
};

class Files_t : public QList<stuFileInfo>{
public:
    QVariant toVariant() const;
    Files_t& fromVariant(const QVariant& _value, const QByteArray& _paramName = {});
};

class RawData_t {
public:
    RawData_t() = default;
    ~RawData_t() = default;
    RawData_t(const RawData_t &) = default;
    RawData_t &operator=(const RawData_t &) = default;
    RawData_t(const QByteArray& _data, const QString& _mime);
    QVariant toVariant() const;
    const QString& mime() { return  this->Mime; }
    const QByteArray& data() { return  this->Data; }

private:
    QString Mime;
    QByteArray Data;
};

class FileData_t {
public:
    FileData_t() = default;
    ~FileData_t() = default;
    FileData_t(const FileData_t &) = default;
    FileData_t &operator=(const FileData_t &) = default;
    FileData_t(const QString &_fileName);
    QVariant toVariant() const;
    const QString &fileName() { return this->FileName; }
//    const QString &mime() { return this->Mime; }

private:
    QString FileName;
//    QString Mime;
};

/**********************************************************************/
TARGOMAN_DEFINE_ENUM(enuGenericStatus,
                     Active = 'A',
                     Removed = 'R'
                     )

/**********************************************************************/

TAPI_ADD_TYPE_SPECIALFROMVARIANT(
    /* baseType             */ QJsonObject,
    /* typeName             */ JWT_t,
    /* setFromVariantLambda */ [=](const QVariant &_value, const QString &_paramName = {}) {
        Q_UNUSED(_paramName);
        this->fromVariantMap(_value.toMap());
    }
);
TAPI_ADD_TYPE_SPECIALFROMVARIANT(
    /* baseType             */ QVariantMap,
    /* typeName             */ ORMFields_t,
    /* setFromVariantLambda */ [=](const QVariant &_value, const QString &_paramName = {}) {
        Q_UNUSED(_paramName);
        *this = _value.toMap();
    }
);
TAPI_ADD_TYPE_SPECIALFROMVARIANT(
    /* baseType             */ QDate,
    /* typeName             */ Date_t,
    /* setFromVariantLambda */ [=](const QVariant &_value, const QString &_paramName = {}) {
        Q_UNUSED(_paramName);
        *this = _value.toDate();
    }
);
TAPI_ADD_TYPE_SPECIALFROMVARIANT(
    /* baseType             */ QTime,
    /* typeName             */ Time_t,
    /* setFromVariantLambda */ [=](const QVariant &_value, const QString &_paramName = {}) {
        Q_UNUSED(_paramName);
        *this = _value.toTime();
    }
);
TAPI_ADD_TYPE_SPECIALFROMVARIANT(
    /* baseType             */ QDateTime,
    /* typeName             */ DateTime_t,
    /* setFromVariantLambda */ [=](const QVariant &_value, const QString &_paramName = {}) {
        Q_UNUSED(_paramName);
        *this = _value.toDateTime();
    }
);

//        *this = QJsonDocument::fromVariant(_value)
#define TAPI_ADD_TYPE_QJSONDOCUMENT(_type) \
    TAPI_ADD_TYPE_SPECIALFROMVARIANT( \
    /* baseType             */ QJsonDocument, \
    /* typeName             */ _type, \
    /* setFromVariantLambda */ [=](const QVariant &_value, const QString &_paramName = {}) { \
        _DEBUG_TAPI_REGISTER_JSON_DERIVED_METATYPE_3(_type, "***** setFromVariantLambda", _paramName, _value); \
        if (_value.isValid() == false) \
        { \
            this->setObject({}); \
            _DEBUG_TAPI_REGISTER_JSON_DERIVED_METATYPE_3(_type, "setFromVariantLambda.1", _paramName, _value); \
            return; \
        } \
        if (_value.userType() == QMetaType::QJsonObject) \
        { \
            auto _val = _value.value<QJsonObject>(); \
            this->setObject(_val); \
            _DEBUG_TAPI_REGISTER_JSON_DERIVED_METATYPE_5(_type, "setFromVariantLambda.2", _paramName, _val, endl, *this); \
            return; \
        } \
        if (_value.userType() == QMetaType::QJsonArray) \
        { \
            auto _val = _value.value<QJsonArray>(); \
            this->setArray(_val); \
            _DEBUG_TAPI_REGISTER_JSON_DERIVED_METATYPE_5(_type, "setFromVariantLambda.3", _paramName, _val, endl, *this); \
            return; \
        } \
        if (_value.canConvert<QVariantMap>() || \
               _value.canConvert<QVariantList>() || \
               _value.canConvert<double>() \
            ) \
        { \
            *this = QJsonDocument::fromVariant(_value); \
            _DEBUG_TAPI_REGISTER_JSON_DERIVED_METATYPE_5(_type, "setFromVariantLambda.4", _paramName, _value, endl, *this); \
            return; \
        } \
        if (_value.toString().isEmpty()) \
        { \
            _DEBUG_TAPI_REGISTER_JSON_DERIVED_METATYPE_3(_type, "setFromVariantLambda.5", _paramName, _value); \
            return; \
        } \
        QJsonParseError Error; \
        QJsonDocument Doc; \
        *this = Doc.fromJson(_value.toString().toUtf8(), &Error); \
        _DEBUG_TAPI_REGISTER_JSON_DERIVED_METATYPE_5(_type, "setFromVariantLambda.6", _paramName, _value, endl, *this); \
        if (Error.error != QJsonParseError::NoError) \
            throw Targoman::API::exHTTPBadRequest(_paramName + " is not a valid JSON: <" + _value.toString() + "> " + Error.errorString()); \
        return; \
    } \
);

TAPI_ADD_TYPE_QJSONDOCUMENT(JSON_t);
TAPI_ADD_TYPE_QJSONDOCUMENT(PrivObject_t);
TAPI_ADD_TYPE_QJSONDOCUMENT(SaleableAdditive_t);

TAPI_ADD_TYPE(qhttp::THeaderHash, HEADERS_t);
TAPI_ADD_TYPE(qhttp::THeaderHash, COOKIES_t);
TAPI_ADD_TYPE_STRING(EncodedJWT_t);
TAPI_ADD_TYPE_STRING(CommaSeparatedStringList_t);
TAPI_ADD_TYPE_STRING(RemoteIP_t);
TAPI_ADD_TYPE_STRING(PKsByPath_t);
TAPI_ADD_TYPE_STRING(MD5_t);
TAPI_ADD_TYPE_STRING(IPv4_t);
TAPI_ADD_TYPE_STRING(Email_t);
TAPI_ADD_TYPE_STRING(Mobile_t);
TAPI_ADD_TYPE_STRING(Sheba_t);
TAPI_ADD_TYPE_STRING(Ether_t);
TAPI_ADD_TYPE_STRING(ISO639_2_t);
TAPI_ADD_TYPE_STRING(Base64Image_t);
TAPI_ADD_TYPE_STRING(URL_t);
TAPI_ADD_TYPE_STRING(ProductCode_t);
TAPI_ADD_TYPE_STRING(SaleableCode_t);
TAPI_ADD_TYPE_STRING(CouponCode_t);
TAPI_ADD_TYPE_STRING(DBTinyText_t);
TAPI_ADD_TYPE_STRING(DBText_t);
TAPI_ADD_TYPE_STRING(DBMediumText_t);
TAPI_ADD_TYPE_STRING(DBLongText_t);
TAPI_ADD_TYPE_STRING(String_t);

} //namespace TAPI

/**********************************************************************/
TAPI_DECLARE_METATYPE(TAPI::stuTable)
TAPI_DECLARE_METATYPE(TAPI::stuFileInfo)
TAPI_DECLARE_METATYPE(TAPI::Files_t)
TAPI_DECLARE_METATYPE(TAPI::RawData_t)
TAPI_DECLARE_METATYPE(TAPI::FileData_t)
TAPI_DECLARE_METATYPE(TAPI::HEADERS_t)
TAPI_DECLARE_METATYPE(TAPI::COOKIES_t)
TAPI_DECLARE_METATYPE(TAPI::JWT_t)
TAPI_DECLARE_METATYPE(TAPI::PrivObject_t)
TAPI_DECLARE_METATYPE(TAPI::EncodedJWT_t)
TAPI_DECLARE_METATYPE(TAPI::CommaSeparatedStringList_t)
TAPI_DECLARE_METATYPE(TAPI::JSON_t)
TAPI_DECLARE_METATYPE(TAPI::SaleableAdditive_t)
TAPI_DECLARE_METATYPE(TAPI::PKsByPath_t)
TAPI_DECLARE_METATYPE(TAPI::ORMFields_t)
TAPI_DECLARE_METATYPE(TAPI::RemoteIP_t)
TAPI_DECLARE_METATYPE(TAPI::MD5_t)
TAPI_DECLARE_METATYPE(TAPI::IPv4_t)
TAPI_DECLARE_METATYPE(TAPI::Email_t)
TAPI_DECLARE_METATYPE(TAPI::Mobile_t)
TAPI_DECLARE_METATYPE(TAPI::Sheba_t)
TAPI_DECLARE_METATYPE(TAPI::Ether_t)
TAPI_DECLARE_METATYPE(TAPI::ISO639_2_t)
TAPI_DECLARE_METATYPE(TAPI::Date_t)
TAPI_DECLARE_METATYPE(TAPI::Time_t)
TAPI_DECLARE_METATYPE(TAPI::DateTime_t)
TAPI_DECLARE_METATYPE(TAPI::Base64Image_t)
TAPI_DECLARE_METATYPE(TAPI::URL_t)
TAPI_DECLARE_METATYPE(TAPI::ProductCode_t)
TAPI_DECLARE_METATYPE(TAPI::SaleableCode_t)
TAPI_DECLARE_METATYPE(TAPI::CouponCode_t)
TAPI_DECLARE_METATYPE(TAPI::DBTinyText_t)
TAPI_DECLARE_METATYPE(TAPI::DBText_t)
TAPI_DECLARE_METATYPE(TAPI::DBMediumText_t)
TAPI_DECLARE_METATYPE(TAPI::DBLongText_t)
TAPI_DECLARE_METATYPE(TAPI::String_t)

TAPI_DECLARE_METATYPE_ENUM(TAPI, enuGenericStatus);

#endif // TAPI_GENERICTYPES_H
