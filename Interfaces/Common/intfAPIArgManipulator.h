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

#ifndef TAPI_INTFAPIARGMANIPULATOR_H
#define TAPI_INTFAPIARGMANIPULATOR_H

#include "Interfaces/Common/HTTPExceptions.h"

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
    virtual ~intfAPIObject() {};
    virtual void invokeMethod(const QVariantList& _arguments, QGenericReturnArgument _returnArg) const = 0;
};

/**********************************************************************/
class intfAPIArgManipulator{
public:
    intfAPIArgManipulator(const QString& _realTypeName){
        this->PrettyTypeName = (_realTypeName.startsWith('Q') ? _realTypeName.mid(1) : _realTypeName).toLower();
        QByteArray RealTypeByteArray = _realTypeName.toLatin1();
        this->RealTypeName = new char[static_cast<uint>(RealTypeByteArray.size()+1)];
        strncpy(this->RealTypeName,
                _realTypeName.toLatin1().constData(),
                static_cast<uint>(RealTypeByteArray.size()));
        this->RealTypeName[RealTypeByteArray.size()] = 0;
    }
    virtual ~intfAPIArgManipulator(){;}

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

    QString     PrettyTypeName;
    char*       RealTypeName;
};

extern void registerUserDefinedType(const char* _typeName, intfAPIArgManipulator* _argManipulator);


#define TAPI_ADD_SIMPLE_TYPE(_type, _name) \
    class _name:public _type{public:_name(){;}_name(const _type& _other):_type(_other){;}}

#define TAPI_ADD_COMPLEX_TYPE(_type, _name) \
    class _name:public _type{ \
        public:_name(){;}_name(const _type& _other):_type(_other){;} \
        _name fromVariant (const QVariant& _value){_type Value = _type::fromVariant (_value); return *reinterpret_cast<_name*>(&Value);}}

#define TAPI_REGISTER_METATYPE(_complexity, _type, ...) \
    qRegisterMetaType<_type>(); \
    registerUserDefinedType(TARGOMAN_M2STR(_type), \
                            new tmplAPIArg<_type, _complexity>(TARGOMAN_M2STR(_type), \
                            __VA_ARGS__))

#define TAPI_VALIDATION_REQUIRED_TYPE_IMPL(_complexity, _type, _validationRule, _toVariant, ...) \
    TAPI_REGISTER_METATYPE( \
        _complexity, _type, \
        [](const _type& _value) -> QVariant {return _toVariant;}, \
        [](const QVariant& _value, const QByteArray& _paramName) -> _type { \
            static QFieldValidator Validator = QFieldValidator()._validationRule; \
            if(Validator.isValid(_value, _paramName) == false) throw exHTTPBadRequest(Validator.errorMessage()); \
            _type Value; Value=_value.toString();  return  Value; \
        }, nullptr, __VA_ARGS__ \
    )

#define TAPI_REGISTER_METATYPE_WITHOPTIONS(_complexity, _type, _lambdaToVariant, _lambdaFromVariant, _options, ...) \
    qRegisterMetaType<_type>(); \
    registerUserDefinedType(TARGOMAN_M2STR(_type), \
                            new tmplAPIArg<_type, _complexity>(TARGOMAN_M2STR(_type), \
                                                               _lambdaToVariant, \
                                                               _lambdaFromVariant, \
                                                               _options, \
                                                               __VA_ARGS__ \
    ))

#define TAPI_REGISTER_TARGOMAN_ENUM(_enum) \
    TAPI_REGISTER_METATYPE_WITHOPTIONS( \
        COMPLEXITY_Enum, _enum::Type, \
        [](_enum::Type _value) -> QVariant{return _enum::toStr(_value);}, \
        [](const QVariant& _value, const QByteArray& _paramName) -> _enum::Type { \
            if(_enum::options().contains(_value.toString())) return _enum::toEnum(_value.toString()); \
            else try { return _enum::toEnum(_value.toString(), true); } catch(...) { \
              throw exHTTPBadRequest(QString("%1(%2) is not a valid %3").arg( \
                      _paramName.size() ? _paramName.constData() : _value.toString()).arg(_value.toString()).arg( \
                      QString(TARGOMAN_M2STR(_enum)).startsWith("enu") ? QString(TARGOMAN_M2STR(_enum)).mid(3) : QString(TARGOMAN_M2STR(_enum)))); \
        }}, \
        []() -> QStringList { return _enum::options();}, \
        [](const QList<ORM::clsORMField>&){return QString("One of (%1)").arg(_enum::options().join('|'));}, \
        [](const QString& _value) -> QVariant { \
          if(_enum::options().contains(_value) == false) \
              throw exHTTPBadRequest(QString("%1(%2) is not a valid %3.").arg( \
                      _value).arg(_value).arg( \
                      QString(TARGOMAN_M2STR(_enum)).startsWith("enu") ? QString(TARGOMAN_M2STR(_enum)).mid(3) : QString(TARGOMAN_M2STR(_enum)))); \
          return QString(_enum::toEnum(_value)); \
        },\
        [](const QVariant& _value) -> QVariant { \
            return _enum::toStr(static_cast<_enum::Type>(_value.toString().toLatin1().at(0))); \
        } \
    )
}
}
#pragma clang diagnostic pop

#endif // TAPI_INTFAPIARGMANIPULATOR_H
