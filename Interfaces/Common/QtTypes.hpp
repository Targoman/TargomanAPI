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
#include <math.h>

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
    inline void setFromVariant(_baseType& _storage, const QVariant& _val, Q_DECL_UNUSED const QString& _paramName = "") { \
        _storage = _val.value<_baseType>(); \
    } \
    inline void setFromVariant(NULLABLE_TYPE(_baseType)& _storage, const QVariant& _val, Q_DECL_UNUSED const QString& _paramName = "") { \
        _storage = NULLABLE_NULL_VALUE; \
        if (_val.isValid() && _val.isNull() == false) \
            _storage = _val.value<_baseType>(); \
    } \
}

//https://cpp.hotexamples.com/examples/-/QString/toLongLong/cpp-qstring-tolonglong-method-examples.html
inline QVariant readNumber(const QString& _str, bool *ok) {
    //m_settings->locale()
    QString negativeSign = "-";
    QString decimalSymbol = ".";
    QString thousandsSeparator = "'";

    bool isInt = false;
    QString str = _str.trimmed();
    bool neg = str.indexOf(negativeSign) == 0;
    if (neg)
        str.remove(0, negativeSign.length());

    /* will hold the scientific notation portion of the number.
    Example, with 2.34E+23, exponentialPart == "E+23"
    */
    QString exponentialPart;
    int EPos  = str.indexOf('E', 0, Qt::CaseInsensitive);

    if (EPos != -1) {
        exponentialPart = str.mid(EPos);
        str = str.left(EPos);
    }

    int pos;
    int fracPos;
    QString major;
    QString minor;
    if ((pos = str.indexOf(decimalSymbol)) != -1) {
        major = str.left(pos);
        minor = str.mid(pos + decimalSymbol.length());
        isInt = false;
    } else if (((pos = str.indexOf(' ')) != -1) &&
               ((fracPos = str.indexOf('/')) != -1)) {
        // try to parse fractions of this form:
        // [0-9]+ [0-9]+/[1-9][0-9]?
        major = str.left(pos);
        QString numerator = str.mid(pos + 1, (fracPos - pos - 1));
        QString denominator = str.mid(fracPos + 1);
        double minorVal = numerator.toDouble() / denominator.toDouble();
        if (minorVal > 1) {
            // assume major is just a plain number
            double wholePart = floor(minorVal);
            minorVal -= wholePart;
            major = QString("%1").arg(major.toInt() + (int)wholePart);
        }
        minor = QString::number(minorVal, 'f').mid(2);     // chop off the "0." part
        // kDebug() <<"fraction:" << major <<"." << minor;
    } else {
        major = str;
        isInt = (EPos == -1); // only, if no exponential part was found
    }

    // Remove thousand separators
    int thlen = thousandsSeparator.length();
    int lastpos = 0;
    while ((pos = major.indexOf(thousandsSeparator)) > 0) {
        // e.g. 12,,345,,678,,922 Acceptable positions (from the end) are 5, 10, 15... i.e. (3+thlen)*N
        int fromEnd = major.length() - pos;
        if (fromEnd % (3 + thlen) != 0 // Needs to be a multiple, otherwise it's an error
                || pos - lastpos > 3 // More than 3 digits between two separators -> error
                || pos == 0          // Can't start with a separator
                || (lastpos > 0 && pos - lastpos != 3)) { // Must have exactly 3 digits between two separators
            if (ok) *ok = false;
            return QVariant();
        }

        lastpos = pos;
        major.remove(pos, thlen);
    }
    if (lastpos > 0 && major.length() - lastpos != 3) { // Must have exactly 3 digits after the last separator
        if (ok) *ok = false;
        return QVariant();
    }

    // log10(2^63) ~= 18
    if (isInt && major.length() > 19) isInt = false;

    QString tot;
    if (neg) tot = '-';
    tot += major;
    if (!isInt) tot += '.' + minor + exponentialPart;

    return isInt ? QVariant(tot.toLongLong(ok)) : QVariant(tot.toDouble(ok));
}

#define TAPI_SPECIAL_MAKE_GENERIC_ON_NUMERIC_TYPE(_numericType, _convertor) \
namespace Targoman::API::Common { \
    template <> inline QGenericArgument tmplAPIArg<_numericType, COMPLEXITY_Integral, false, true>::makeGenericArgument( \
        const QVariant& _val, \
        const QByteArray& _paramName, \
        void** _argStorage \
    ) { \
        bool Result; \
        *_argStorage = new _numericType; \
        _numericType ConvertedVal; \
        if (_val.userType() == QMetaType::QString) { \
            QString StrVal = _val.toString(); \
            ConvertedVal = static_cast<_numericType>(readNumber(StrVal, &Result)._convertor(&Result)); \
        } else \
            ConvertedVal = static_cast<_numericType>(_val._convertor(&Result)); \
        *(reinterpret_cast<_numericType*>(*_argStorage)) = ConvertedVal; \
        if (!Result) \
            throw exHTTPBadRequest("Invalid value specified for parameter: " + _paramName); \
        return QGenericArgument(this->RealTypeName, *_argStorage); \
    } \
    template <> inline QGenericArgument tmplAPIArg<NULLABLE_TYPE(_numericType), COMPLEXITY_Integral, true>::makeGenericArgument( \
        const QVariant& _val, \
        const QByteArray& _paramName, \
        void** _argStorage \
    ) { \
        bool Result = true; \
        *_argStorage = new NULLABLE_TYPE(_numericType); \
        if (_val.isValid() && (_val != NULLABLE_NULL_VALUE_AS_STRING)) { \
            _numericType ConvertedVal; \
            if (_val.userType() == QMetaType::QString) { \
                QString StrVal = _val.toString(); \
                ConvertedVal = static_cast<_numericType>(readNumber(StrVal, &Result)._convertor(&Result)); \
            } else \
                ConvertedVal = static_cast<_numericType>(_val._convertor(&Result)); \
            *(reinterpret_cast<NULLABLE_TYPE(_numericType)*>(*_argStorage)) = ConvertedVal; \
        } \
        if (!Result) \
            throw exHTTPBadRequest("Invalid value specified for NULLABLE parameter: " + _paramName); \
        return QGenericArgument(this->RealTypeName, *_argStorage); \
    } \
} \
namespace TAPI { \
    inline void setFromVariant(_numericType& _storage, const QVariant& _val, Q_DECL_UNUSED const QString& _paramName = "") { \
        bool Result; \
        _storage = static_cast<_numericType>(_val._convertor(&Result)); \
        if (!Result) \
            throw Targoman::API::exHTTPBadRequest(QString("Invalid value (%1) specified for %2:%3").arg(_val.toString()).arg(_paramName).arg(#_numericType)); \
    } \
    inline void setFromVariant(NULLABLE_TYPE(_numericType)& _storage, const QVariant& _val, Q_DECL_UNUSED const QString& _paramName = "") { \
        _storage = NULLABLE_NULL_VALUE; \
        bool Result = true; \
        if (_val.isValid() && _val.isNull() == false) \
            _storage = static_cast<_numericType>(_val._convertor(&Result)); \
        if (!Result) \
            throw Targoman::API::exHTTPBadRequest(QString("Invalid value (%1) specified for %2:NULLABLE<%3>").arg(_val.toString()).arg(_paramName).arg(#_numericType)); \
    } \
    inline QJsonValue toJsonValue(const NULLABLE_TYPE(_numericType)& _val) { \
        qDebug() << "toJsonValue(?)" << NULLABLE_GET_OR_DEFAULT(_val, 99999999); \
        QJsonValue JsonVal; JsonVal = NULLABLE_IS_NULL(_val) ? QJsonValue() : static_cast<double>(*_val); return JsonVal; \
    } \
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
    Q_DECLARE_METATYPE(NULLABLE_TYPE(_baseType)) \
    DEFINE_SETFROMVARIANT_METHOD_ON_COMPLEXITY_Complex(_baseType)

#define REGISTER_ON_TYPE_IGNORED(_baseType)
#define DO_ON_TYPE_SELECTOR(_1,_2,_N,...) _N
#define REGISTER_ON_TYPE_PROXY(_baseType, ...) DO_ON_TYPE_SELECTOR(__VA_ARGS__, REGISTER_ON_TYPE_IGNORED, REGISTER_ON_TYPE_VALID)(_baseType)
#define REGISTER_ON_TYPE(_typeName, _id, _baseType) REGISTER_ON_TYPE_PROXY(_baseType, IGNORE_TYPE_##_typeName)

//QT_FOR_EACH_STATIC_PRIMITIVE_TYPE(REGISTER_ON_TYPE)
Q_DECLARE_METATYPE(NULLABLE_TYPE(quint8))
Q_DECLARE_METATYPE(NULLABLE_TYPE(quint16))
Q_DECLARE_METATYPE(NULLABLE_TYPE(quint32))
Q_DECLARE_METATYPE(NULLABLE_TYPE(quint64))
Q_DECLARE_METATYPE(NULLABLE_TYPE(qint8))
Q_DECLARE_METATYPE(NULLABLE_TYPE(qint16))
Q_DECLARE_METATYPE(NULLABLE_TYPE(qint32))
Q_DECLARE_METATYPE(NULLABLE_TYPE(qint64))

Q_DECLARE_METATYPE(NULLABLE_TYPE(double))
//Q_DECLARE_METATYPE(NULLABLE_TYPE(qreal))

Q_DECLARE_METATYPE(NULLABLE_TYPE(float))
Q_DECLARE_METATYPE(NULLABLE_TYPE(bool))



QT_FOR_EACH_STATIC_CORE_CLASS(REGISTER_ON_TYPE)
QT_FOR_EACH_STATIC_CORE_TEMPLATE(REGISTER_ON_TYPE)

#endif // TARGOMAN_API_SERVER_QTTYPES_HPP
