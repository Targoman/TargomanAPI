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
#define TAPI_ADD_TYPE_SPECIALFROMVARIANT(_baseType, _typeName, _fromVariant) \
    INTERNAL_TAPI_ADD_TYPE_SPECIALFROMVARIANT(_baseType, _typeName, _fromVariant)

#define TAPI_ADD_TYPE(_baseType, _typeName) \
    TAPI_ADD_TYPE_SPECIALFROMVARIANT(_baseType, _typeName, this->fromVariant(_value) )

#define TAPI_ADD_STRING_TYPE(_baseType, _typeName) \
    TAPI_ADD_TYPE_SPECIALFROMVARIANT(_baseType, _typeName, _value.toString() )

/************************************************************/
#define TAPI_DECLARE_METATYPE(_type) \
    Q_DECLARE_METATYPE(_type) \
    Q_DECLARE_METATYPE(NULLABLE_TYPE(_type))

/************************************************************/
#define TAPI_DECLARE_METATYPE_ENUM(_enum) \
    INTERNAL_TAPI_DECLARE_METATYPE_ENUM(_enum)

/************************************************************/
#define TAPI_REGISTER_METATYPE(_complexity, _namespace, _type, ...) \
    TAPI_REGISTER_METATYPE_MACRO_SELECTOR(TAPI_REGISTER_METATYPE_, __VA_ARGS__)(_complexity, _namespace, _type, __VA_ARGS__)

/************************************************************/
#define TAPI_VALIDATION_REQUIRED_TYPE_IMPL(_complexity, _namespace, _type, _validationRule, _toVariant, ...) \
    TAPI_REGISTER_METATYPE( \
        _complexity, _namespace, _type, \
        [](const _namespace::_type& _value) -> QVariant {return _toVariant;}, \
        [](const QVariant& _value, const QByteArray& _paramName) -> _namespace::_type { \
            static QFieldValidator Validator = QFieldValidator()._validationRule; \
            if(Validator.isValid(_value, _paramName) == false) throw exHTTPBadRequest(Validator.errorMessage()); \
            _namespace::_type Value; Value=_value.toString();  return  Value; \
        }, __VA_ARGS__ \
    )

/************************************************************/
#define TAPI_REGISTER_TARGOMAN_ENUM(_namespace, _enum) \
    INTERNAL_TAPI_REGISTER_TARGOMAN_ENUM(_namespace, _enum)

/************************************************************/
#define SF_Generic(_type, _name, _def, _validator, _fromVariant, _toVariant) _type, _name, _def, _validator, _fromVariant, _toVariant
#define SF_Enum(_type, _name, _def)     _type::Type, _name, _def, v, v, static_cast<_type::Type>(v.toString().toLatin1().constData()[0])
#define SF_Struct(_type, _name, ...)    INTERNAL_SF_STRUCT(_type, _name, __VA_ARGS__)
#define SF_QString(_name, ...)          INTERNAL_SF(QString,           _name, STRING,   v,                 v.toString(),         __VA_ARGS__)

#define SF_quint8(_name, ...)           INTERNAL_SF(quint8,            _name, INTEGRAL,          INTERNAL_C2DBL(v), INTERNAL_V2uint8(v),  __VA_ARGS__)
#define SF_NULLABLE_quint8(_name, ...)  INTERNAL_SF(NULLABLE_TYPE(quint8),  _name, NULLABLE_INTEGRAL, INTERNAL_N2J(v),   INTERNAL_N2uint8(v),  __VA_ARGS__)
#define SF_quint16(_name, ...)          INTERNAL_SF(quint16,           _name, INTEGRAL,          INTERNAL_C2DBL(v), INTERNAL_V2uint16(v), __VA_ARGS__)
#define SF_NULLABLE_quint16(_name, ...) INTERNAL_SF(NULLABLE_TYPE(quint16), _name, NULLABLE_INTEGRAL, INTERNAL_N2J(v),   INTERNAL_N2uint16(v), __VA_ARGS__)
#define SF_quint32(_name, ...)          INTERNAL_SF(quint32,           _name, INTEGRAL,          INTERNAL_C2DBL(v), INTERNAL_V2uint32(v), __VA_ARGS__)
#define SF_NULLABLE_quint32(_name, ...) INTERNAL_SF(NULLABLE_TYPE(quint32), _name, NULLABLE_INTEGRAL, INTERNAL_N2J(v),   INTERNAL_N2uint32(v), __VA_ARGS__)
#define SF_quint64(_name, ...)          INTERNAL_SF(quint64,           _name, INTEGRAL,          INTERNAL_C2DBL(v), INTERNAL_V2uint64(v), __VA_ARGS__)
#define SF_NULLABLE_quint64(_name, ...) INTERNAL_SF(NULLABLE_TYPE(quint64), _name, NULLABLE_INTEGRAL, INTERNAL_N2J(v),   INTERNAL_N2uint64(v), __VA_ARGS__)

/************************************************************/
#define TAPI_DEFINE_VARIANT_ENABLED_STRUCT(_name, ...) struct _name { \
    TAPI_HELEPER_DEFINE_VARIANT_STRUCT_PARAMS(__VA_ARGS__) \
    _name(TAPI_HELEPER_DEFINE_VARIANT_STRUCT_CONS_INPUT(__VA_ARGS__)) : TAPI_HELEPER_DEFINE_VARIANT_STRUCT_CONS_INIT(__VA_ARGS__) {} \
    QJsonObject toJson() const { QJsonObject Obj; TAPI_HELEPER_DEFINE_VARIANT_STRUCT_TOJSON(__VA_ARGS__); return Obj;} \
    _name& fromJson(const QJsonObject& _obj){ TAPI_HELEPER_DEFINE_VARIANT_STRUCT_FROMJSON(__VA_ARGS__) return *this;} \
};

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
//#define NULLABLE_VALUE(_value) _value.isNull() ? QVariant() : *_value
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
#define NULLABLE_VALUE(_value) _value ? *_value : QVariant()
#define NULLABLE_NULL_VALUE std::nullopt
#define NULLABLE_IS_NULL(_nullable) (bool)_nullable == false
#define NULLABLE_HAS_VALUE(_nullable) (bool)_nullable
#define NULLABLE_INSTANTIATE_FROM_QVARIANT(_type, _val) (_val.isNull() ? NULLABLE_TYPE(_type)() : NULLABLE_TYPE(_type)(_val.value<_type>()))

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
