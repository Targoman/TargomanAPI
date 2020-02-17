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

#ifndef TAPI_INTFAPIARGMANIPULATOR_H
#define TAPI_INTFAPIARGMANIPULATOR_H

#include "Interfaces/Common/HTTPExceptions.hpp"

namespace Targoman {
namespace API {
namespace ORM {
class clsORMField;
}

enum enuVarComplexity {
    COMPLEXITY_Integral,
    COMPLEXITY_Number,
    COMPLEXITY_Boolean,
    COMPLEXITY_String,
    COMPLEXITY_Object,
    COMPLEXITY_Complex,
    COMPLEXITY_File,
    COMPLEXITY_Enum
};

class intfCacheConnector;
/**********************************************************************/
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wweak-vtables"

class intfAPIObject{
public:
    virtual ~intfAPIObject();
    virtual void invokeMethod(const QVariantList& _arguments, QGenericReturnArgument _returnArg) const = 0;
};

/**********************************************************************/
class intfAPIArgManipulator{
public:
    intfAPIArgManipulator(const QString& _realTypeName);
    virtual ~intfAPIArgManipulator();

    virtual QGenericArgument makeGenericArgument(const QVariant& _val, const QByteArray& _paramName, void** _argStorage) = 0;
    virtual QVariant invokeMethod(const intfAPIObject* _apiObject, const QVariantList& _arguments) = 0;
    virtual QVariant defaultVariant() const = 0;
    virtual void cleanup (void* _argStorage) = 0;
    virtual bool hasFromVariantMethod() const = 0;
    virtual bool hasToVariantMethod() const = 0;
    virtual QString toString(const QVariant _val) const = 0;
    virtual bool isPrimitiveType() const  = 0;
    virtual enuVarComplexity complexity() const = 0;
    virtual QStringList options() const = 0;
    virtual QString description(const QList<ORM::clsORMField>& _allFields) const = 0;
    virtual void validate(const QVariant& _val, const QByteArray& _paramName) const = 0;
    virtual QVariant toORMValue(const QString& _val) const = 0;
    virtual std::function<QVariant(const QVariant& _val)> fromORMValueConverter() const = 0;
    virtual bool isNullable() const = 0;

    QString     PrettyTypeName;
    char*       RealTypeName;
};

extern void registerUserDefinedType(const char* _typeName, intfAPIArgManipulator* _argManipulator);

#define TAPI_ADD_SIMPLE_TYPE(_type, _name) \
    class _name:public _type{public:_name(){;}_name(const _type& _other):_type(_other){;}}\

#define TAPI_ADD_COMPLEX_TYPE(_type, _name) \
    class _name:public _type{ \
        public:_name(){;}_name(const _type& _other):_type(_other){;} \
        _name fromVariant (const QVariant& _value){_type Value = _type::fromVariant (_value); return *reinterpret_cast<_name*>(&Value);} \
    }

#define TAPI_DECLARE_METATYPE(_type) \
    Q_DECLARE_METATYPE(_type) \
    Q_DECLARE_METATYPE(QSharedPointer<_type>)

#define QSP_M2STR_POSTFIX ">"
#define QSP_M2STR_PREFIX "QSharedPointer<"
#define QSP_M2STR(_type) QSP_M2STR_PREFIX #_type QSP_M2STR_POSTFIX

#define TAPI_REGISTER_METATYPE_FULL(_complexity, _namespace, _type, _lambdaToVariant, _lambdaFromVariant, _lambdaDesc, _lambdaToORM, _lambdaFromORM, _lambdaOptions) \
namespace Targoman {namespace API { \
    template<> std::function<QVariant(_namespace::_type _value)> tmplAPIArg<_namespace::_type, _complexity, false>::toVariantLambda = _lambdaToVariant; \
    template<> std::function<_namespace::_type(QVariant _value, const QByteArray& _paramName)> tmplAPIArg<_namespace::_type, _complexity, false>::fromVariantLambda = _lambdaFromVariant; \
    template<> std::function<QString(const QList<ORM::clsORMField>& _allFields)> tmplAPIArg<_namespace::_type, _complexity, false>::descriptionLambda = _lambdaDesc; \
    template<> std::function<QVariant(QString _value)> tmplAPIArg<_namespace::_type, _complexity, false>::toORMValueLambda = _lambdaToORM; \
    template<> std::function<QVariant(const QVariant& _val)> tmplAPIArg<_namespace::_type, _complexity, false>::fromORMValueLambda = _lambdaFromORM; \
    template<> std::function<QStringList()> tmplAPIArg<_namespace::_type, _complexity, false>::optionsLambda = _lambdaOptions; \
    static tmplAPIArg<_namespace::_type, _complexity, false>* Dangling_##_namespace##_type = tmplAPIArg<_namespace::_type, _complexity, false>::instance(TARGOMAN_M2STR(_namespace::_type)); \
    \
    template<> std::function<QVariant(QSharedPointer<_namespace::_type> _value)> tmplAPIArg<QSharedPointer<_namespace::_type>, _complexity, true>::toVariantLambda = \
        [](QSharedPointer<_namespace::_type> _value){return _value.isNull() ? QVariant() : tmplAPIArg<_namespace::_type, _complexity, false>::toVariant(*_value);}; \
    template<> std::function<QSharedPointer<_namespace::_type>(QVariant _value, const QByteArray& _paramName)> tmplAPIArg<QSharedPointer<_namespace::_type>, _complexity, true>::fromVariantLambda = \
        [](const QVariant& _value, const QByteArray& _paramName) -> QSharedPointer<_namespace::_type> { \
            if(!_value.isValid() || _value.isNull()) return QSharedPointer<_namespace::_type>(); \
            QSharedPointer<_namespace::_type> Value(new _namespace::_type); *Value = tmplAPIArg<_namespace::_type, _complexity, false>::fromVariant(_value, _paramName); \
            return Value; \
        }; \
    template<> std::function<QVariant(QString _value)> tmplAPIArg<QSharedPointer<_namespace::_type>, _complexity, true>::toORMValueLambda = _lambdaToORM; \
    template<> std::function<QVariant(const QVariant& _val)> tmplAPIArg<QSharedPointer<_namespace::_type>, _complexity, true>::fromORMValueLambda = _lambdaFromORM; \
    template<> std::function<QStringList()> tmplAPIArg<QSharedPointer<_namespace::_type>, _complexity, true>::optionsLambda = _lambdaOptions; \
    template<> std::function<QString(const QList<ORM::clsORMField>& _allFields)> tmplAPIArg<QSharedPointer<_namespace::_type>, _complexity, true>::descriptionLambda = _lambdaDesc; \
    static tmplAPIArg<QSharedPointer<_namespace::_type>, _complexity, true>* Dangling_QSP_##_namespace##_type = tmplAPIArg<QSharedPointer<_namespace::_type>, _complexity, true>::instance(QSP_M2STR(_namespace::_type)); \
}}

#define TAPI_REGISTER_METATYPE_FromORM(_complexity, _namespace, _type, _lambdaToVariant, _lambdaFromVariant, _lambdaDesc, _lambdaToORM, _lambdaFromORM) \
    TAPI_REGISTER_METATYPE_FULL(_complexity, _namespace, _type, _lambdaToVariant, _lambdaFromVariant, _lambdaDesc, _lambdaToORM, _lambdaFromORM, nullptr)

#define TAPI_REGISTER_METATYPE_ToORM(_complexity, _namespace, _type, _lambdaToVariant, _lambdaFromVariant, _lambdaDesc, _lambdaToORM) \
    TAPI_REGISTER_METATYPE_FromORM(_complexity, _namespace, _type, _lambdaToVariant, _lambdaFromVariant, _lambdaDesc, _lambdaToORM, nullptr)

#define TAPI_REGISTER_METATYPE_Desc(_complexity, _namespace, _type, _lambdaToVariant, _lambdaFromVariant, _lambdaDesc) \
    TAPI_REGISTER_METATYPE_ToORM(_complexity, _namespace, _type, _lambdaToVariant, _lambdaFromVariant, _lambdaDesc, nullptr)

#define TAPI_REGISTER_METATYPE_FromVariant(_complexity, _namespace, _type, _lambdaToVariant, _lambdaFromVariant) \
    TAPI_REGISTER_METATYPE_Desc(_complexity, _namespace, _type, _lambdaToVariant, _lambdaFromVariant, nullptr)

#define TAPI_REGISTER_METATYPE_ToVariant(_complexity, _namespace, _type, _lambdaToVariant) \
    TAPI_REGISTER_METATYPE_FromVariant(_complexity, _namespace, _type, _lambdaToVariant, nullptr)

#define TAPI_REGISTER_METATYPE_MACRO_SELECTOR(_fn,...) TARGOMAN_MACRO_ARG_COUNT_HELPER(__VA_ARGS__,\
    _fn##Multi,_fn##Multi,_fn##Multi,_fn##Multi,_fn##Multi,_fn##Multi,_fn##Multi,_fn##Multi,_fn##Multi,_fn##Multi,  \
    _fn##Multi,_fn##Multi,_fn##Multi,_fn##Multi,_fn##Multi,_fn##Multi,_fn##Multi,_fn##Multi,_fn##Multi,_fn##Multi,  \
    _fn##Multi,_fn##Multi,_fn##Multi,_fn##Multi,_fn##Multi,_fn##Multi,_fn##Multi,_fn##Multi,_fn##Multi,_fn##Multi,  \
    _fn##Multi,_fn##Multi,_fn##Multi,_fn##Multi,_fn##Multi,_fn##Multi,_fn##Multi,_fn##Multi,_fn##Multi,_fn##Multi,  \
    _fn##Multi,_fn##Multi,_fn##Multi,_fn##Multi,_fn##Multi,_fn##Multi,_fn##Multi,_fn##Multi,_fn##Multi,_fn##Multi,  \
    _fn##Multi,_fn##Multi,_fn##Multi,_fn##FULL,_fn##FromORM,_fn##ToORM,_fn##Desc,_fn##FromVariant,_fn##ToVariant, None)

/**********************************************************************/
#define TAPI_REGISTER_METATYPE(_complexity, _namespace, _type, ...) \
    TAPI_REGISTER_METATYPE_MACRO_SELECTOR(TAPI_REGISTER_METATYPE_, __VA_ARGS__)(_complexity, _namespace, _type, __VA_ARGS__)

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

#define TAPI_REGISTER_TARGOMAN_ENUM_IMPL(_namespace, _enum, _lambdaToVariant, _lambdaFromVariant, _lambdaDesc, _lambdaToORM, _lambdaFromORM, _lambdaOptions) \
namespace Targoman {namespace API { \
    template<> std::function<QVariant(_namespace::_enum::Type _value)> tmplAPIArg<_namespace::_enum::Type, COMPLEXITY_Enum, false>::toVariantLambda = _lambdaToVariant; \
    template<> std::function<_namespace::_enum::Type(QVariant _value, const QByteArray& _paramName)> tmplAPIArg<_namespace::_enum::Type, COMPLEXITY_Enum, false>::fromVariantLambda = _lambdaFromVariant; \
    template<> std::function<QVariant(QString _value)> tmplAPIArg<_namespace::_enum::Type, COMPLEXITY_Enum, false>::toORMValueLambda = _lambdaToORM; \
    template<> std::function<QVariant(const QVariant& _val)> tmplAPIArg<_namespace::_enum::Type, COMPLEXITY_Enum, false>::fromORMValueLambda = _lambdaFromORM; \
    template<> std::function<QStringList()> tmplAPIArg<_namespace::_enum::Type, COMPLEXITY_Enum, false>::optionsLambda = _lambdaOptions; \
    template<> std::function<QString(const QList<ORM::clsORMField>& _allFields)> tmplAPIArg<_namespace::_enum::Type, COMPLEXITY_Enum, false>::descriptionLambda = _lambdaDesc; \
    static tmplAPIArg<_namespace::_enum::Type, COMPLEXITY_Enum, false>* Dangling_##_namespace##_enum = tmplAPIArg<_namespace::_enum::Type, COMPLEXITY_Enum, false>::instance(TARGOMAN_M2STR(_namespace::_enum::Type)); \
    \
    template<> std::function<QVariant(QSharedPointer<_namespace::_enum::Type> _value)> tmplAPIArg<QSharedPointer<_namespace::_enum::Type>, COMPLEXITY_Enum, true>::toVariantLambda = \
        [](QSharedPointer<_namespace::_enum::Type> _value){return _value.isNull() ? QVariant() : tmplAPIArg<_namespace::_enum::Type, COMPLEXITY_Enum, false>::toVariant(*_value);}; \
    template<> std::function<QSharedPointer<_namespace::_enum::Type>(QVariant _value, const QByteArray& _paramName)> tmplAPIArg<QSharedPointer<_namespace::_enum::Type>, COMPLEXITY_Enum, true>::fromVariantLambda = \
        [](const QVariant& _value, const QByteArray& _paramName) -> QSharedPointer<_namespace::_enum::Type> { \
            if(!_value.isValid() || _value.isNull()) return QSharedPointer<_namespace::_enum::Type>(); \
            QSharedPointer<_namespace::_enum::Type> Value(new _namespace::_enum::Type); *Value = tmplAPIArg<_namespace::_enum::Type, COMPLEXITY_Enum, false>::fromVariant(_value, _paramName); \
            return Value; \
        }; \
    template<> std::function<QVariant(QString _value)> tmplAPIArg<QSharedPointer<_namespace::_enum::Type>, COMPLEXITY_Enum, true>::toORMValueLambda = _lambdaToORM; \
    template<> std::function<QVariant(const QVariant& _val)> tmplAPIArg<QSharedPointer<_namespace::_enum::Type>, COMPLEXITY_Enum, true>::fromORMValueLambda = _lambdaFromORM; \
    template<> std::function<QStringList()> tmplAPIArg<QSharedPointer<_namespace::_enum::Type>, COMPLEXITY_Enum, true>::optionsLambda = _lambdaOptions; \
    template<> std::function<QString(const QList<ORM::clsORMField>& _allFields)> tmplAPIArg<QSharedPointer<_namespace::_enum::Type>, COMPLEXITY_Enum, true>::descriptionLambda = _lambdaDesc; \
    static tmplAPIArg<QSharedPointer<_namespace::_enum::Type>, COMPLEXITY_Enum, true>* Dangling_QSP_##_namespace##_enum = tmplAPIArg<QSharedPointer<_namespace::_enum::Type>, COMPLEXITY_Enum, true>::instance(QSP_M2STR(_namespace::_enum::Type)); \
}}

#define TAPI_REGISTER_TARGOMAN_ENUM(_namespace, _enum) \
    TAPI_REGISTER_TARGOMAN_ENUM_IMPL( \
        _namespace, _enum, \
        [](_namespace::_enum::Type _value) -> QVariant{return _namespace::_enum::toStr(_value);}, \
        [](const QVariant& _value, const QByteArray& _paramName) -> _namespace::_enum::Type { \
            if(_namespace::_enum::options().contains(_value.toString())) return _namespace::_enum::toEnum(_value.toString()); \
            else try { return _namespace::_enum::toEnum(_value.toString(), true); } catch(...) { \
              throw exHTTPBadRequest(QString("%1(%2) is not a valid %3").arg( \
                      _paramName.size() ? _paramName.constData() : _value.toString()).arg(_value.toString()).arg( \
                      QString(TARGOMAN_M2STR(_enum)).startsWith("enu") ? QString(TARGOMAN_M2STR(_enum)).mid(3) : QString(TARGOMAN_M2STR(_enum)))); \
        }}, \
        [](const QList<ORM::clsORMField>&) -> QString {return QString("One of (%1)").arg(_namespace::_enum::options().join('|'));}, \
        [](const QString& _value) -> QVariant { \
          if(_namespace::_enum::options().contains(_value) == false) \
              throw exHTTPBadRequest(QString("%1(%2) is not a valid %3.").arg( \
                      _value).arg(_value).arg( \
                      QString(TARGOMAN_M2STR(_enum)).startsWith("enu") ? QString(TARGOMAN_M2STR(_enum)).mid(3) : QString(TARGOMAN_M2STR(_enum)))); \
          return QString(_namespace::_enum::toEnum(_value)); \
        },\
        [](const QVariant& _value) -> QVariant { \
            return _namespace::_enum::toStr(static_cast<_namespace::_enum::Type>(_value.toString().toLatin1().at(0))); \
        }, \
        []() -> QStringList { return _namespace::_enum::options();} \
    )

#define NULLABLE(_type) QSharedPointer<_type>
#define NULLABLE_VALUE(_value) _value.isNull() ? QVariant() : *_value
}
}
#pragma clang diagnostic pop

#endif // TAPI_INTFAPIARGMANIPULATOR_H
