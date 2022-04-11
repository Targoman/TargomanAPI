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

#ifndef TAPI_APIARGHELPERMACROS_HPP
#define TAPI_APIARGHELPERMACROS_HPP

#include <optional>
#include "APIArgHelperMacrosPrivate.h"

/************************************************************/
#define TAPI_ADD_TYPE_SPECIALFROMVARIANT(_baseType, _typeName, _setFromVariantLambda) \
    INTERNAL_TAPI_ADD_TYPE_SPECIALFROMVARIANT(_baseType, _typeName, _setFromVariantLambda)

#define TAPI_ADD_TYPE(_baseType, _typeName) \
    TAPI_ADD_TYPE_SPECIALFROMVARIANT(_baseType, _typeName, [=](const QVariant &_value, const QString &_paramName = {}) { \
        Q_UNUSED(_paramName); \
        this->fromVariant(_value); \
    })

#define TAPI_ADD_TYPE_STRING(_typeName) \
    TAPI_ADD_TYPE_SPECIALFROMVARIANT(QString, _typeName, [=](const QVariant &_value, const QString &_paramName = {}) { \
        Q_UNUSED(_paramName); \
        *this = _value.toString(); \
    })

/************************************************************/
#define TAPI_DECLARE_METATYPE(_type) \
    Q_DECLARE_METATYPE(_type) \
    Q_DECLARE_METATYPE(NULLABLE_TYPE(_type))

/************************************************************/
#define TAPI_DECLARE_METATYPE_ENUM(_namespace, _enum) \
    INTERNAL_TAPI_DECLARE_METATYPE_ENUM(_namespace, _enum)

/************************************************************/
#define TAPI_REGISTER_METATYPE(_complexity, _namespace, _type, ...) \
    TAPI_REGISTER_METATYPE_MACRO_SELECTOR(TAPI_REGISTER_METATYPE_, __VA_ARGS__)(_complexity, _namespace, _type, __VA_ARGS__)

#ifdef QT_DEBUG
#define _ENABLE_DEBUG_TAPI_REGISTER_JSON_DERIVED_METATYPE
#endif
#ifdef _ENABLE_DEBUG_TAPI_REGISTER_JSON_DERIVED_METATYPE
#define _DEBUG_TAPI_REGISTER_JSON_DERIVED_METATYPE_0(_type)                             qDebug() << "==============================" << TARGOMAN_M2STR(_type)
#define _DEBUG_TAPI_REGISTER_JSON_DERIVED_METATYPE_1(_type, _a1)                        _DEBUG_TAPI_REGISTER_JSON_DERIVED_METATYPE_0(_type)                         << "(" << _a1 << ")"
#define _DEBUG_TAPI_REGISTER_JSON_DERIVED_METATYPE_2(_type, _a1, _a2)                   _DEBUG_TAPI_REGISTER_JSON_DERIVED_METATYPE_1(_type, _a1)                    << _a2
#define _DEBUG_TAPI_REGISTER_JSON_DERIVED_METATYPE_3(_type, _a1, _a2, _a3)              _DEBUG_TAPI_REGISTER_JSON_DERIVED_METATYPE_2(_type, _a1, _a2)               << _a3
#define _DEBUG_TAPI_REGISTER_JSON_DERIVED_METATYPE_4(_type, _a1, _a2, _a3, _a4)         _DEBUG_TAPI_REGISTER_JSON_DERIVED_METATYPE_3(_type, _a1, _a2, _a3)          << _a4
#define _DEBUG_TAPI_REGISTER_JSON_DERIVED_METATYPE_5(_type, _a1, _a2, _a3, _a4, _a5)    _DEBUG_TAPI_REGISTER_JSON_DERIVED_METATYPE_4(_type, _a1, _a2, _a3, _a4)     << _a5
#else
#define _DEBUG_TAPI_REGISTER_JSON_DERIVED_METATYPE_0(_type)
#define _DEBUG_TAPI_REGISTER_JSON_DERIVED_METATYPE_1(_type, ...)
#define _DEBUG_TAPI_REGISTER_JSON_DERIVED_METATYPE_2(_type, ...)
#define _DEBUG_TAPI_REGISTER_JSON_DERIVED_METATYPE_3(_type, ...)
#define _DEBUG_TAPI_REGISTER_JSON_DERIVED_METATYPE_4(_type, ...)
#define _DEBUG_TAPI_REGISTER_JSON_DERIVED_METATYPE_5(_type, ...)
#endif

#define TAPI_REGISTER_JSON_DERIVED_METATYPE(_complexity, _namespace, _type) \
    TAPI_REGISTER_METATYPE(_complexity, _namespace, _type, \
        /* toVariantLambda    */ [](const _type& _value) -> QVariant { \
            _DEBUG_TAPI_REGISTER_JSON_DERIVED_METATYPE_2(_type, "toVariantLambda", _value); \
            return _value; \
        }, \
        /* fromVariantLambda  */ [](const QVariant& _value, const QByteArray& _paramName) -> _type { \
            _DEBUG_TAPI_REGISTER_JSON_DERIVED_METATYPE_3(_type, "fromVariantLambda", _paramName, _value); \
            return _type::fromVariant(_value); \
        }, \
        /* descriptionLambda  */ [](const QList<Targoman::API::DBM::clsORMField>&) { return "A valid JSON object"; }, \
        /* toORMValueLambda   */ [](const QVariant& _value) { \
            /*qDebug() << "JSON_t(3) =================================" << _value;*/  \
            if (_value.isValid() == false) { \
                /*qDebug() << "JSON_t(3.1) =================================" << _value;*/ \
                return QJsonDocument(); \
            } \
            if (_value.userType() == QMetaType::QJsonObject) { \
                auto Doc = QJsonDocument(); \
                auto _val = _value.value<QJsonObject>(); \
                Doc.setObject(_val); \
                /*qDebug() << "JSON_t(3.2) =================================" << _val << endl << "  =" << Doc;*/ \
                return Doc; \
            } \
            if (_value.userType() == QMetaType::QJsonArray) { \
                auto Doc = QJsonDocument(); \
                auto _val = _value.value<QJsonArray>(); \
                Doc.setArray(_val); \
                /*qDebug() << "JSON_t(3.3) =================================" << _val << endl << "  =" << Doc;*/ \
                return Doc; \
            } \
            if (_value.canConvert<QVariantMap>() || \
                   _value.canConvert<QVariantList>() || \
                   _value.canConvert<double>()) { \
                auto Doc = QJsonDocument::fromVariant(_value); \
                /*qDebug() << "JSON_t(3.4) =================================" << _value << endl << "  =" << Doc;*/ \
                return Doc; \
            } \
            if (_value.toString().isNull() || _value.toString().isEmpty()) { \
                /*qDebug() << "JSON_t(3.5) =================================" << _value;*/  \
                return QJsonDocument(); \
            } \
            QJsonParseError Error; \
            QJsonDocument Doc = QJsonDocument::fromJson(_value.toString().toUtf8(), &Error); \
            /*qDebug() << "JSON_t(3.6) =================================" << _value << endl << "  =" << Doc;*/ \
            if (Error.error != QJsonParseError::NoError) \
                throw exHTTPBadRequest("is not a valid Json: <"+_value.toString()+">" + QString("%1:%2").arg(Error.error).arg(Error.errorString())); \
            return Doc; \
        }, \
        /* fromORMValueLambda */ [](const QVariant& _value) { \
            QString ret = QString("%1").arg(QJsonDocument::fromVariant(_value).toJson(QJsonDocument::Compact).constData()); \
            /*qDebug() << "JSON_t(4) =================================" << _value << " -> " << ret;*/  \
            return ret; \
        } \
    )
//        QString ret = QString::fromUtf8(QJsonDocument::fromVariant(_value).toJson(QJsonDocument::Compact));

/************************************************************/
#define TAPI_VALIDATION_REQUIRED_TYPE_IMPL(_complexity, _namespace, _type, _validationRule, _toVariant, ...) \
    TAPI_REGISTER_METATYPE( \
        _complexity, _namespace, _type, \
        [](const _namespace::_type& _value) -> QVariant {return _toVariant;}, \
        [](const QVariant& _value, const QByteArray& _paramName) -> _namespace::_type { \
            static QFieldValidator Validator = QFieldValidator()._validationRule; \
            if (Validator.isValid(_value, _paramName) == false) throw exHTTPBadRequest(Validator.errorMessage()); \
            _namespace::_type Value; Value=_value.toString();  return  Value; \
        }, __VA_ARGS__ \
    )

/************************************************************/
#define TAPI_REGISTER_TARGOMAN_ENUM(_namespace, _enum) \
    INTERNAL_TAPI_REGISTER_TARGOMAN_ENUM(_namespace, _enum)

/************************************************************/
#define SF_Generic(_type, _name, _def, _validator, _fromVariant, _toVariant) _type, _name, _def, _validator, _fromVariant, _toVariant
#define SF_Enum(_type, _name, _def)     _type::Type, _name, _def, v, _type::toStr(v), static_cast<_type::Type>(v.toString().toLatin1().constData()[0])
#define SF_Struct(_type, _name, ...)    INTERNAL_SF_STRUCT(_type, _name, __VA_ARGS__)

///TODO: complete this and test with stuPaymentGateway
//#define SF_JSON(_name)                  SF_Generic(
//    /* type        */ TAPI::JSON_t,
//    /* name        */ _name,
//    /* def         */ TAPI::JSON_t(),
//    /* validator   */ v.isEmpty() == false,
//    /* fromVariant */ [](auto v) { return QJsonValue::fromVariant(v); }(v),
//    /* toVariant   */ v
//)

//                                                  _type,                  _name, _typeGroup,        _fromVariant,      _toVariant,           _def, _validator
#define SF_String(_name, _type, ...)    INTERNAL_SF(_type,                  _name, STRING,            v,                 v.toString(),         __VA_ARGS__)
#define SF_QString(_name, ...)          SF_String(_name,    QString,      __VA_ARGS__)
#define SF_MD5_t(_name, ...)            SF_String(_name,    TAPI::MD5_t,  __VA_ARGS__)

#define SF_quint8(_name, ...)           INTERNAL_SF(quint8,                 _name, INTEGRAL,          INTERNAL_C2DBL(v), INTERNAL_V2uint8(v),  __VA_ARGS__)
#define SF_NULLABLE_quint8(_name, ...)  INTERNAL_SF(NULLABLE_TYPE(quint8),  _name, NULLABLE_INTEGRAL, INTERNAL_N2J(v),   INTERNAL_N2uint8(v),  __VA_ARGS__)
#define SF_quint16(_name, ...)          INTERNAL_SF(quint16,                _name, INTEGRAL,          INTERNAL_C2DBL(v), INTERNAL_V2uint16(v), __VA_ARGS__)
#define SF_NULLABLE_quint16(_name, ...) INTERNAL_SF(NULLABLE_TYPE(quint16), _name, NULLABLE_INTEGRAL, INTERNAL_N2J(v),   INTERNAL_N2uint16(v), __VA_ARGS__)
#define SF_quint32(_name, ...)          INTERNAL_SF(quint32,                _name, INTEGRAL,          INTERNAL_C2DBL(v), INTERNAL_V2uint32(v), __VA_ARGS__)
#define SF_NULLABLE_quint32(_name, ...) INTERNAL_SF(NULLABLE_TYPE(quint32), _name, NULLABLE_INTEGRAL, INTERNAL_N2J(v),   INTERNAL_N2uint32(v), __VA_ARGS__)
#define SF_quint64(_name, ...)          INTERNAL_SF(quint64,                _name, INTEGRAL,          INTERNAL_C2DBL(v), INTERNAL_V2uint64(v), __VA_ARGS__)
#define SF_NULLABLE_quint64(_name, ...) INTERNAL_SF(NULLABLE_TYPE(quint64), _name, NULLABLE_INTEGRAL, INTERNAL_N2J(v),   INTERNAL_N2uint64(v), __VA_ARGS__)
#define SF_qreal(_name, ...)            INTERNAL_SF(qreal,                  _name, INTEGRAL,          INTERNAL_C2DBL(v), INTERNAL_V2DBL(v),    __VA_ARGS__)
#define SF_NULLABLE_qreal(_name, ...)   INTERNAL_SF(NULLABLE_TYPE(qreal),   _name, NULLABLE_INTEGRAL, INTERNAL_N2J(v),   INTERNAL_N2DBL(v),    __VA_ARGS__)

/************************************************************/
#define TAPI_DEFINE_VARIANT_ENABLED_STRUCT(_name, ...) struct _name { \
    TAPI_HELEPER_DEFINE_VARIANT_STRUCT_PARAMS(__VA_ARGS__) \
    _name(TAPI_HELEPER_DEFINE_VARIANT_STRUCT_CONS_INPUT(__VA_ARGS__)) : \
        TAPI_HELEPER_DEFINE_VARIANT_STRUCT_CONS_INIT(__VA_ARGS__) \
    {} \
    QJsonObject toJson() const { \
        QJsonObject Obj; \
        TAPI_HELEPER_DEFINE_VARIANT_STRUCT_TOJSON(__VA_ARGS__); \
        return Obj; \
    } \
    _name& fromJson(const QJsonObject& _obj) { \
        TAPI_HELEPER_DEFINE_VARIANT_STRUCT_FROMJSON(__VA_ARGS__); \
        return *this; \
    } \
};

#define SET_FIELD_FROM_VARIANT_MAP(_varName, _infoRec, _table, _tableFieldName) \
    QT_TRY { \
        TAPI::setFromVariant(_varName, _infoRec.value(_table::_tableFieldName)); \
    } \
    QT_CATCH (const std::exception &exp) { \
        TargomanDebug(5, "*** SET_FIELD_FROM_VARIANT_MAP *** ERROR: fieldName:" << #_tableFieldName << exp.what()); \
        QT_RETHROW; \
    }

#define SET_FIELD_FROM_VARIANT_MAP_SAME_NAME(_storage, _varName, _infoRec, _table) \
    SET_FIELD_FROM_VARIANT_MAP(_storage _varName, _infoRec, _table, _varName)

#define C2DBL(v) INTERNAL_C2DBL(v)
#define C2U64(v) INTERNAL_C2DBL(v)
#define V2U64(v) INTERNAL_V2U64(v)
#define C2U32(v) C2DBL(v)
#define V2U32(v) INTERNAL_V2U32(v)

/************************************************************/
// METHOD 1: tmplNullable(QSharedPointer)
//#define TAPI_HELEPER_QSP_M2STR_PREFIX "TAPI::tmplNullable<"
//#define TAPI_HELEPER_QSP_M2STR_POSTFIX ">"
//#define NULLABLE_UNDERLAYER_CLASS_NAME "TAPI::tmplNullable"
//#define NULLABLE_TYPE(_type) TAPI::tmplNullable<_type>
//#define NULLABLE_VAR(_type, _name) NULLABLE_TYPE(_type) _name = NULLABLE_TYPE(_type)::create();
//#define NULLABLE_GET(_value) _value.isNull() ? QVariant() : *_value
//#define NULLABLE_NULL_VALUE nullptr
//#define NULLABLE_IS_NULL(_nullable) _nullable.isNull()
//#define NULLABLE_HAS_VALUE(_nullable) _nullable.isNull() == false
//#define NULLABLE_INSTANTIATE_FROM_QVARIANT(_type, _val) NULLABLE_TYPE(_type)(_val)

// METHOD 2: std::optional
#define TAPI_HELEPER_QSP_M2STR_PREFIX "std::optional<"
#define TAPI_HELEPER_QSP_M2STR_POSTFIX ">"
#define NULLABLE_UNDERLAYER_CLASS_NAME "std::optional"
#define NULLABLE_TYPE(_type) std::optional<_type>
#define NULLABLE_VAR(_type, _name) NULLABLE_TYPE(_type) _name
#define NULLABLE_GET(_value) (_value.has_value() ? *_value : QVariant())
#define NULLABLE_GET_OR_DEFAULT(_value, _def) (_value.has_value() ? *_value : _def)
#define NULLABLE_SET(_var, _value) (_var = _value)
#define NULLABLE_NULL_VALUE std::nullopt
#define NULLABLE_RESET(_var) (_var = NULLABLE_NULL_VALUE)
#define NULLABLE_IS_NULL(_nullable) (_nullable.has_value() == false)
#define NULLABLE_HAS_VALUE(_nullable) _nullable.has_value()
#define NULLABLE_INSTANTIATE_FROM_QVARIANT(_type, _val) (_val.isNull() ? NULLABLE_TYPE(_type)() : NULLABLE_TYPE(_type)(_val.template value<_type>()))

/************************************************************/
#define N2J(_value)    [](auto v) { return toJsonValue(v); } (_value)
#define N2S8(_value)   INTERNAL_N2int8(_value)
#define N2S16(_value)  INTERNAL_N2int16(_value)
#define N2S32(_value)  INTERNAL_N2int32(_value)
#define N2S64(_value)  INTERNAL_N2int64(_value)
#define N2U8(_value)   INTERNAL_N2uint8(_value)
#define N2U16(_value)  INTERNAL_N2uint16(_value)
#define N2U32(_value)  INTERNAL_N2uint32(_value)
#define N2U64(_value)  INTERNAL_N2uint64(_value)
#define N2DBL(_value)  INTERNAL_NULLABLE_FROM_JSONVALUE_TO_TYPE(double,  _value)
#define QSP_M2STR(_type) TAPI_HELEPER_QSP_M2STR_PREFIX #_type TAPI_HELEPER_QSP_M2STR_POSTFIX

#endif // TAPI_APIARGHELPERMACROS_HPP
