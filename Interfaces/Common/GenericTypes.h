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
#include <QUrl>

#include "QHttp/qhttpfwd.hpp"
#include "libTargomanCommon/clsCountAndSpeed.h"
#include "Interfaces/Common/tmplAPIArg.h"

namespace Targoman {
namespace API {
namespace Server {

extern QList<Targoman::API::Common::intfAPIArgManipulator*> gOrderedMetaTypeInfo;
extern QList<Targoman::API::Common::intfAPIArgManipulator*> gUserDefinedTypesInfo;

}
}
}

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

    QJsonObject toJson(bool _full = false){
        auto count2Json = [](const Targoman::Common::clsCountAndSpeed& _value){
            return QJsonObject({
                                   {"count", static_cast<double>(_value.count())},
                                   {"cps", _value.countPerSecond()}
                               });
        };
        auto hashToJson = [count2Json](const QHash<QByteArray, Targoman::Common::clsCountAndSpeed>& _hash){
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

        if(_full){
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
    stuTable(qint64 _totalRows = -1, const QVariantList& _rows = QVariantList()):
        TotalRows(_totalRows),
        Rows(_rows)
    {}
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
  {}

  stuFileInfo(const stuFileInfo& _other) :
      Name(_other.Name),
      TempName(_other.TempName),
      Size(_other.Size),
      Mime(_other.Mime)
  {}

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

/**********************************************************************/
TARGOMAN_DEFINE_ENUM(enuGenericStatus,
                     Active = 'A',
                     Removed = 'R'
                     )

/**********************************************************************/

} //namespace TAPI

TARGOMAN_ADD_TYPE(TAPI, qhttp::THeaderHash, HEADERS_t);
TARGOMAN_ADD_TYPE(TAPI, qhttp::THeaderHash, COOKIES_t);

TARGOMAN_ADD_TYPE_SPECIALFROMVARIANT(TAPI, QJsonObject, JWT_t,       this->fromVariantMap(_value.toMap()));
TARGOMAN_ADD_TYPE_SPECIALFROMVARIANT(TAPI, QVariantMap, ORMFields_t, _value.toMap());

TARGOMAN_ADD_TYPE_SPECIALFROMVARIANT(TAPI, QJsonDocument, PrivObject_t,       *this = QJsonDocument::fromVariant(_value));
TARGOMAN_ADD_TYPE_SPECIALFROMVARIANT(TAPI, QJsonDocument, SaleableAdditive_t, *this = QJsonDocument::fromVariant(_value));

TARGOMAN_ADD_TYPE_STRING(TAPI, EncodedJWT_t);
TARGOMAN_ADD_TYPE_STRING(TAPI, PKsByPath_t);
TARGOMAN_ADD_TYPE_STRING(TAPI, ProductCode_t);
TARGOMAN_ADD_TYPE_STRING(TAPI, SaleableCode_t);
TARGOMAN_ADD_TYPE_STRING(TAPI, CouponCode_t);
TARGOMAN_ADD_TYPE_STRING(TAPI, RemoteIP_t);

TARGOMAN_ADD_TYPE_SPECIALFROMVARIANT(TAPI, QDate,       Date_t,      _value.toDate());
TARGOMAN_ADD_TYPE_SPECIALFROMVARIANT(TAPI, QTime,       Time_t,      _value.toTime());
TARGOMAN_ADD_TYPE_SPECIALFROMVARIANT(TAPI, QDateTime,   DateTime_t,  _value.toDateTime());

//TARGOMAN_ADD_TEMPLATED_TYPE_SPECIALFROMVARIANT(QList, T, JSONLIST_t, *this = QJsonDocument::fromVariant(_value));
TARGOMAN_ADD_TYPE_SPECIALFROMVARIANT(TAPI, QJsonDocument, JSON_t,             *this = QJsonDocument::fromVariant(_value));

TARGOMAN_ADD_TYPE_STRING(TAPI, CommaSeparatedStringList_t);
TARGOMAN_ADD_TYPE_STRING(TAPI, MD5_t);
TARGOMAN_ADD_TYPE_STRING(TAPI, IPv4_t);
TARGOMAN_ADD_TYPE_STRING(TAPI, Email_t);
TARGOMAN_ADD_TYPE_STRING(TAPI, Mobile_t);
TARGOMAN_ADD_TYPE_STRING(TAPI, Sheba_t);
TARGOMAN_ADD_TYPE_STRING(TAPI, Ether_t);
TARGOMAN_ADD_TYPE_STRING(TAPI, ISO639_2_t);
TARGOMAN_ADD_TYPE_STRING(TAPI, Base64Image_t);
TARGOMAN_ADD_TYPE_STRING(TAPI, URL_t);
TARGOMAN_ADD_TYPE_STRING(TAPI, DBTinyText_t);
TARGOMAN_ADD_TYPE_STRING(TAPI, DBText_t);
TARGOMAN_ADD_TYPE_STRING(TAPI, DBMediumText_t);
TARGOMAN_ADD_TYPE_STRING(TAPI, DBLongText_t);
TARGOMAN_ADD_TYPE_STRING(TAPI, String_t);

#define SF_MD5_t(_name, ...)            SF_String(_name,    TAPI::MD5_t,  __VA_ARGS__)

TARGOMAN_DECLARE_METATYPE(TAPI::CommaSeparatedStringList_t)
TARGOMAN_DECLARE_METATYPE(TAPI::JSON_t)
TARGOMAN_DECLARE_METATYPE(TAPI::MD5_t)
TARGOMAN_DECLARE_METATYPE(TAPI::IPv4_t)
TARGOMAN_DECLARE_METATYPE(TAPI::Email_t)
TARGOMAN_DECLARE_METATYPE(TAPI::Mobile_t)
TARGOMAN_DECLARE_METATYPE(TAPI::Sheba_t)
TARGOMAN_DECLARE_METATYPE(TAPI::Ether_t)
TARGOMAN_DECLARE_METATYPE(TAPI::ISO639_2_t)
TARGOMAN_DECLARE_METATYPE(TAPI::Date_t)
TARGOMAN_DECLARE_METATYPE(TAPI::Time_t)
TARGOMAN_DECLARE_METATYPE(TAPI::DateTime_t)
TARGOMAN_DECLARE_METATYPE(TAPI::Base64Image_t)
TARGOMAN_DECLARE_METATYPE(TAPI::URL_t)
TARGOMAN_DECLARE_METATYPE(TAPI::DBTinyText_t)
TARGOMAN_DECLARE_METATYPE(TAPI::DBText_t)
TARGOMAN_DECLARE_METATYPE(TAPI::DBMediumText_t)
TARGOMAN_DECLARE_METATYPE(TAPI::DBLongText_t)
TARGOMAN_DECLARE_METATYPE(TAPI::String_t)

TARGOMAN_DECLARE_METATYPE(TAPI::HEADERS_t)
TARGOMAN_DECLARE_METATYPE(TAPI::COOKIES_t)
TARGOMAN_DECLARE_METATYPE(TAPI::JWT_t)
TARGOMAN_DECLARE_METATYPE(TAPI::ORMFields_t)
TARGOMAN_DECLARE_METATYPE(TAPI::PrivObject_t)
TARGOMAN_DECLARE_METATYPE(TAPI::SaleableAdditive_t)
TARGOMAN_DECLARE_METATYPE(TAPI::EncodedJWT_t)
TARGOMAN_DECLARE_METATYPE(TAPI::PKsByPath_t)
TARGOMAN_DECLARE_METATYPE(TAPI::ProductCode_t)
TARGOMAN_DECLARE_METATYPE(TAPI::SaleableCode_t)
TARGOMAN_DECLARE_METATYPE(TAPI::CouponCode_t)
TARGOMAN_DECLARE_METATYPE(TAPI::RemoteIP_t)

TARGOMAN_DECLARE_METATYPE(TAPI::stuTable)
TARGOMAN_DECLARE_METATYPE(TAPI::stuFileInfo)
TARGOMAN_DECLARE_METATYPE(TAPI::Files_t)
TARGOMAN_DECLARE_METATYPE(TAPI::RawData_t)

TARGOMAN_DECLARE_METATYPE_ENUM(TAPI, enuGenericStatus);

#endif // TAPI_GENERICTYPES_H
