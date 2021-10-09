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

#ifndef TAPI_TMPLAPIARG_HPP
#define TAPI_TMPLAPIARG_HPP

#include "Interfaces/Common/HTTPExceptions.hpp"
#include "Interfaces/Common/intfAPIArgManipulator.h"
#include "Interfaces/DBM/clsTable.h"

using namespace Targoman::Common;

namespace Targoman::API {

using namespace DBM;

namespace Server {
class intfCacheConnector;
}

namespace Common {

template<typename _itmplType, enuVarComplexity _itmplVarType, bool _itmplNullable, bool _isQtType = false>
class tmplAPIArg : public intfAPIArgManipulator {
public:
    virtual ~tmplAPIArg(){}

    virtual QGenericArgument makeGenericArgument(const QVariant& _val, const QByteArray& _paramName, void** _argStorage) final{
        *_argStorage = nullptr;
        if(this->fromVariantLambda == nullptr && !_val.canConvert<_itmplType>())
                throw exHTTPBadRequest("Invalid value specified for parameter.: " + _paramName);
        *_argStorage = new _itmplType;
        *(reinterpret_cast<_itmplType*>(*_argStorage)) =
                this->fromVariantLambda == nullptr ? _val.value<_itmplType>() : this->fromVariantLambda(_val, _paramName);
        return QGenericArgument(this->RealTypeName, *_argStorage);
    }

    inline QVariant invokeMethod(const intfAPIObject *_apiObject, const QVariantList& _arguments) final {
           _itmplType Result;
           _apiObject->invokeMethod(_arguments, QReturnArgument<_itmplType >(this->RealTypeName, Result));
           return this->toVariantLambda == nullptr ? QVariant::fromValue(Result) : this->toVariantLambda(Result);
    }

    inline void validate(const QVariant& _val, const QByteArray& _paramName) const final {
        if(this->fromVariantLambda == nullptr && !_val.canConvert<_itmplType>())
                throw exHTTPBadRequest("Invalid value specified for parameter.:: " + _paramName);
        if(this->fromVariantLambda)
            this->fromVariantLambda(_val, _paramName);
    }

    inline QVariant defaultVariant() const {
        _itmplType Default;
        return this->toVariantLambda == nullptr ? QVariant::fromValue(Default) : this->toVariantLambda(Default);
    }

    inline void cleanup (void* _argStorage) final {if(_argStorage) delete (reinterpret_cast<_itmplType*>(_argStorage));}
    inline bool hasFromVariantMethod() const final {return this->fromVariantLambda != nullptr;}
    inline bool hasToVariantMethod() const final {return this->toVariantLambda != nullptr;}
    inline bool isPrimitiveType() const final { return _itmplVarType == COMPLEXITY_Integral;}
    inline QStringList options() const final { return this->optionsLambda ? this->optionsLambda() : QStringList() ;}
    inline enuVarComplexity complexity() const final { return _itmplVarType;}
    inline bool isNullable() const final {return _itmplNullable;}
    inline QString description(const QList<DBM::clsORMField>& _allFields) const final
    {
        if (this->descriptionLambda)
        {
            QStringList FieldsNames;
            foreach (auto Field, _allFields)
                FieldsNames.append(clsTable::finalColName(Field)/*Field.Name()*/);
            return this->descriptionLambda(FieldsNames /*_allFields*/);
        }
        return QString("A value of type: %1").arg(this->PrettyTypeName);
    }
    inline QString toString(const QVariant _val) const final {
        if(this->hasFromVariantMethod() && this->hasToVariantMethod())
            return this->toVariantLambda(this->fromVariantLambda(_val, {})).toString();
        return QString();
    }

    inline QVariant toORMValue(const QString& _val) const final {
        return this->toORMValueLambda == nullptr ?
                    _val :
                    this->toORMValueLambda(_val);
    }
    
    inline std::function<QVariant(const QVariant& _val)> fromORMValueConverter() const final {return this->fromORMValueLambda;}
    inline std::function<QVariant(const QVariant& _val)> toORMValueConverter() const final {return this->toORMValueLambda;}

    static _itmplType fromVariant(QVariant _value, const QByteArray& _paramName = {}){
        if(tmplAPIArg<_itmplType, _itmplVarType, _itmplNullable, _isQtType>::fromVariantLambda)
            return tmplAPIArg<_itmplType, _itmplVarType, _itmplNullable, _isQtType>::fromVariantLambda(_value, _paramName);
        return _value.value<_itmplType>();
    }

    static QVariant toVariant(_itmplType _value){
        if(tmplAPIArg<_itmplType, _itmplVarType, _itmplNullable, _isQtType>::toVariantLambda)
            return tmplAPIArg<_itmplType, _itmplVarType, _itmplNullable, _isQtType>::toVariantLambda(_value);
        return QVariant::fromValue(_value);
    }

    static tmplAPIArg* instance(const char* _typeStr){
        static tmplAPIArg* Instance = nullptr; return Q_LIKELY(Instance) ? Instance : (Instance = new tmplAPIArg(_typeStr));
    }
private:
    tmplAPIArg(const char* _typeStr) : intfAPIArgManipulator(_typeStr) {
        qRegisterMetaType<_itmplType>();
        if (!_isQtType)
            registerUserDefinedType(_typeStr, this);
    }

private:
    static std::function<QVariant(_itmplType _value)> toVariantLambda;
    static std::function<_itmplType(QVariant _value, const QByteArray& _paramName)> fromVariantLambda;
    static std::function<QVariant(const QVariant& _val)> toORMValueLambda;
    static std::function<QVariant(const QVariant& _val)> fromORMValueLambda;
    static std::function<QStringList()> optionsLambda;
    static std::function<QString(/*const QList<DBM::clsORMField>& _allFields*/ const QStringList& _fieldsNames)> descriptionLambda;

    friend class intfCacheConnector;
};

} //namespace Common
} //namespace Targoman::API

#endif // TARGOMAN_API_TMPLAPIARG_HPP
