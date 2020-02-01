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
 @author S. Mehran M. Ziabary <ziabary@targoman.com>
 */

#ifndef TAPI_GENERICTYPES_H
#define TAPI_GENERICTYPES_H

#include <QJsonDocument>
#include <QJsonObject>

#include "QHttp/qhttpfwd.hpp"
#include "libTargomanCommon/clsCountAndSpeed.h"

#include "Interfaces/Common/tmplAPIArg.h"

//I used TAPI as namespace in order to make Targoman::API shorter
namespace TAPI{

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
struct stuTable{
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

struct stuFileInfo{
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
    Files_t fromVariant(const QVariant& _value, const QByteArray& _paramName = {}) const;
};

/**********************************************************************/
TARGOMAN_DEFINE_ENUM(enuGenericStatus,
                     Active = 'A',
                     Removed = 'R'
                     )

/**********************************************************************/

TAPI_ADD_COMPLEX_TYPE(qhttp::THeaderHash, HEADERS_t);
TAPI_ADD_COMPLEX_TYPE(qhttp::THeaderHash, COOKIES_t);

TAPI_ADD_SIMPLE_TYPE(QJsonObject, JWT_t);
TAPI_ADD_SIMPLE_TYPE(QJsonDocument, JSON_t);

TAPI_ADD_SIMPLE_TYPE(QVariantMap, ORMFilters_t);
TAPI_ADD_SIMPLE_TYPE(QString, EncodedJWT_t);
TAPI_ADD_SIMPLE_TYPE(QString, ExtraPath_t);
TAPI_ADD_SIMPLE_TYPE(QString, RemoteIP_t);
TAPI_ADD_SIMPLE_TYPE(QString, MD5_t);
TAPI_ADD_SIMPLE_TYPE(QString, IPv4_t);
TAPI_ADD_SIMPLE_TYPE(QString, Email_t);
TAPI_ADD_SIMPLE_TYPE(QString, Mobile_t);
TAPI_ADD_SIMPLE_TYPE(QString, Sheba_t);
TAPI_ADD_SIMPLE_TYPE(QString, ISO639_2_t);
TAPI_ADD_SIMPLE_TYPE(QString, Base64Image_t);
TAPI_ADD_SIMPLE_TYPE(QString, Date_t);
TAPI_ADD_SIMPLE_TYPE(QString, Time_t);
TAPI_ADD_SIMPLE_TYPE(QString, DateTime_t);

/**********************************************************************/
extern void registerGenericTypes();
}

/**********************************************************************/
Q_DECLARE_METATYPE(TAPI::stuTable)
Q_DECLARE_METATYPE(TAPI::stuFileInfo)
Q_DECLARE_METATYPE(TAPI::Files_t)
Q_DECLARE_METATYPE(TAPI::HEADERS_t)
Q_DECLARE_METATYPE(TAPI::COOKIES_t)
Q_DECLARE_METATYPE(TAPI::JWT_t)
Q_DECLARE_METATYPE(TAPI::EncodedJWT_t)
Q_DECLARE_METATYPE(TAPI::JSON_t)
Q_DECLARE_METATYPE(TAPI::ExtraPath_t)
Q_DECLARE_METATYPE(TAPI::ORMFilters_t)
Q_DECLARE_METATYPE(TAPI::RemoteIP_t)
Q_DECLARE_METATYPE(TAPI::MD5_t)
Q_DECLARE_METATYPE(TAPI::IPv4_t)
Q_DECLARE_METATYPE(TAPI::Email_t)
Q_DECLARE_METATYPE(TAPI::Mobile_t)
Q_DECLARE_METATYPE(TAPI::Sheba_t)
Q_DECLARE_METATYPE(TAPI::ISO639_2_t)
Q_DECLARE_METATYPE(TAPI::Date_t)
Q_DECLARE_METATYPE(TAPI::Time_t)
Q_DECLARE_METATYPE(TAPI::DateTime_t)
Q_DECLARE_METATYPE(TAPI::Base64Image_t)
Q_DECLARE_METATYPE(TAPI::enuGenericStatus::Type);


#endif // TAPI_GENERICTYPES_H
