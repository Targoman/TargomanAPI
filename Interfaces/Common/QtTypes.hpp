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

#ifndef TARGOMAN_API_SERVER_QTTYPES_HPP
#define TARGOMAN_API_SERVER_QTTYPES_HPP

#include "Interfaces/Common/HTTPExceptions.hpp"
#include "Interfaces/Common/tmplNullable.hpp"
#include "Interfaces/Common/tmplAPIArg.h"
#include <QJsonValue>

/*****************************************************/
#define IGNORE_TYPE_Void ,
#define IGNORE_TYPE_Char ,
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
#define IGNORE_TYPE_QUrl ,
#define IGNORE_TYPE_QUuid ,
#define IGNORE_TYPE_QRegularExpression ,
#define IGNORE_TYPE_QCborValue ,
#define IGNORE_TYPE_QCborArray ,
#define IGNORE_TYPE_QCborMap ,

#define DEFINE_SETFROMVARIANT_METHOD_ON_COMPLEXITY_Complex(_baseType)  \
namespace TAPI { \
    inline void setFromVariant(_baseType& _storage, const QVariant& _val){ _storage = _val.value<_baseType>(); } \
    inline void setFromVariant(TAPI::tmplNullable<_baseType>& _storage, const QVariant& _val){ \
        if (_val.isValid() && _val.isNull() == false) *_storage = _val.value<_baseType>(); \
    } \
}

#define TAPI_SPECIAL_MAKE_GENERIC_ON_NUMERIC_TYPE(_numericType, _convertor) \
namespace Targoman { namespace API { \
template<> inline QGenericArgument tmplAPIArg<_numericType, COMPLEXITY_Integral, false, true>::makeGenericArgument(const QVariant& _val, const QByteArray& _paramName, void** _argStorage){ \
    bool Result; *_argStorage = new _numericType; *(reinterpret_cast<_numericType*>(*_argStorage)) = static_cast<_numericType>(_val._convertor(&Result)); \
    if(!Result) throw exHTTPBadRequest("Invalid value specified for parameter: " + _paramName); \
    return QGenericArgument(this->RealTypeName, *_argStorage); \
} \
template<> inline QGenericArgument tmplAPIArg<TAPI::tmplNullable<_numericType>, COMPLEXITY_Integral, true>::makeGenericArgument(const QVariant& _val, const QByteArray& _paramName, void** _argStorage){ \
    bool Result = true; *_argStorage = new TAPI::tmplNullable<_numericType>; \
    if(_val.isValid() && _val.isNull() == false) **(reinterpret_cast<TAPI::tmplNullable<_numericType>*>(*_argStorage)) = static_cast<_numericType>(_val._convertor(&Result)); \
    if(!Result) throw exHTTPBadRequest("Invalid value specified for parameter: " + _paramName); \
    return QGenericArgument(this->RealTypeName, *_argStorage); \
} \
}} \
namespace TAPI { \
inline void setFromVariant(_numericType& _storage, const QVariant& _val){ \
    bool Result;_storage = static_cast<_numericType>(_val._convertor(&Result)); \
    if(!Result) throw Targoman::API::exHTTPBadRequest(QString("Invalid value specifiedfor base type: %1").arg(#_numericType)); \
} \
inline void setFromVariant(TAPI::tmplNullable<_numericType>& _storage, const QVariant& _val){ \
    bool Result = true; if(_val.isValid() && _val.isNull() == false) *_storage = static_cast<_numericType>(_val._convertor(&Result)); \
    if(!Result) throw Targoman::API::exHTTPBadRequest(QString("Invalid value specifiedfor base type: %1").arg(#_numericType)); \
} \
inline QJsonValue toJsonValue(const TAPI::tmplNullable<_numericType>& _val){ QJsonValue JsonVal; JsonVal = _val.isNull() ? QJsonValue() : static_cast<double>(*_val); return JsonVal; } \
}

TAPI_SPECIAL_MAKE_GENERIC_ON_NUMERIC_TYPE(quint8,  toUInt)
TAPI_SPECIAL_MAKE_GENERIC_ON_NUMERIC_TYPE(quint16, toUInt)
TAPI_SPECIAL_MAKE_GENERIC_ON_NUMERIC_TYPE(quint32, toUInt)
TAPI_SPECIAL_MAKE_GENERIC_ON_NUMERIC_TYPE(quint64, toULongLong)
TAPI_SPECIAL_MAKE_GENERIC_ON_NUMERIC_TYPE(qint8,   toInt)
TAPI_SPECIAL_MAKE_GENERIC_ON_NUMERIC_TYPE(qint16,  toInt)
TAPI_SPECIAL_MAKE_GENERIC_ON_NUMERIC_TYPE(qint32,  toInt)
TAPI_SPECIAL_MAKE_GENERIC_ON_NUMERIC_TYPE(qint64,  toLongLong)
TAPI_SPECIAL_MAKE_GENERIC_ON_NUMERIC_TYPE(double,  toDouble)
TAPI_SPECIAL_MAKE_GENERIC_ON_NUMERIC_TYPE(float,   toFloat)

#define REGISTER_ON_TYPE_VALID(_baseType)  \
    Q_DECLARE_METATYPE(TAPI::tmplNullable<_baseType>) \
    DEFINE_SETFROMVARIANT_METHOD_ON_COMPLEXITY_Complex(_baseType)

#define REGISTER_ON_TYPE_IGNORED(_baseType)
#define DO_ON_TYPE_SELECTOR(_1,_2,_N,...) _N
#define REGISTER_ON_TYPE_PROXY(_baseType, ...) DO_ON_TYPE_SELECTOR(__VA_ARGS__, REGISTER_ON_TYPE_IGNORED, REGISTER_ON_TYPE_VALID)(_baseType)
#define REGISTER_ON_TYPE(_typeName, _id, _baseType) REGISTER_ON_TYPE_PROXY(_baseType, IGNORE_TYPE_##_typeName)

//QT_FOR_EACH_STATIC_PRIMITIVE_TYPE(REGISTER_ON_TYPE)
Q_DECLARE_METATYPE(TAPI::tmplNullable<quint8>)
Q_DECLARE_METATYPE(TAPI::tmplNullable<quint16>)
Q_DECLARE_METATYPE(TAPI::tmplNullable<quint32>)
Q_DECLARE_METATYPE(TAPI::tmplNullable<quint64>)
Q_DECLARE_METATYPE(TAPI::tmplNullable<qint8>)
Q_DECLARE_METATYPE(TAPI::tmplNullable<qint16>)
Q_DECLARE_METATYPE(TAPI::tmplNullable<qint32>)
Q_DECLARE_METATYPE(TAPI::tmplNullable<qint64>)
Q_DECLARE_METATYPE(TAPI::tmplNullable<double>)
Q_DECLARE_METATYPE(TAPI::tmplNullable<float>)
Q_DECLARE_METATYPE(TAPI::tmplNullable<bool>)

QT_FOR_EACH_STATIC_CORE_CLASS(REGISTER_ON_TYPE)
QT_FOR_EACH_STATIC_CORE_TEMPLATE(REGISTER_ON_TYPE)

#endif // TARGOMAN_API_SERVER_QTTYPES_HPP
