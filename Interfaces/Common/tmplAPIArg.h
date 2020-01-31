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

#ifndef TAPI_TMPLAPIARG_HPP
#define TAPI_TMPLAPIARG_HPP

#include "Interfaces/Common/HTTPExceptions.hpp"
#include "Interfaces/Common/intfAPIArgManipulator.h"

namespace Targoman {
namespace API {
namespace Server {
class intfCacheConnector;
}

template<typename _itmplType, enuVarComplexity _itmplVarType>
class tmplAPIArg : public intfAPIArgManipulator{
public:
    tmplAPIArg(const QString& _typeName,
               std::function<QVariant(const _itmplType& _value)> _toVariant = {},
               std::function<_itmplType(const QVariant& _value, const QByteArray& _paramName)> _fromVariant = {},
               std::function<QStringList()> _options = {},
               std::function<QString(const QList<ORM::clsORMField>& _allFields)> _description = {},
               std::function<QVariant(const QString& _val)> _toORMValue = {},
               std::function<QVariant(const QVariant& _val)> _fromORMValue = {}
               ) :
        intfAPIArgManipulator(_typeName),
        toVariantLambda(_toVariant),
        fromVariantLambda(_fromVariant),
        toORMValueLambda(_toORMValue),
        fromORMValueLambda(_fromORMValue),
        optionsLambda(_options),
        descriptionLambda(_description)
    {}
    virtual ~tmplAPIArg(){;}

    virtual QGenericArgument makeGenericArgument(const QVariant& _val, const QByteArray& _paramName, void** _argStorage) final{
        *_argStorage = nullptr;
        if(this->fromVariantLambda == nullptr && !_val.canConvert<_itmplType>())
                throw exHTTPBadRequest("Invalid value specified for parameter: " + _paramName);
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
                throw exHTTPBadRequest("Invalid value specified for parameter: " + _paramName);
        if(this->fromVariantLambda)
            this->fromVariantLambda(_val, _paramName);
    }
    inline QVariant defaultVariant() const final {
        _itmplType Default;
        return this->toVariantLambda == nullptr ? QVariant::fromValue(Default) : this->toVariantLambda(Default);
    }
    inline void cleanup (void* _argStorage) final {if(_argStorage) delete (reinterpret_cast<_itmplType*>(_argStorage));}
    inline bool hasFromVariantMethod() const final {return this->fromVariantLambda != nullptr;}
    inline bool hasToVariantMethod() const final {return this->toVariantLambda != nullptr;}
    inline bool isPrimitiveType() const final { return _itmplVarType == COMPLEXITY_Integral;}
    inline QStringList options() const final { return this->optionsLambda ? this->optionsLambda() : QStringList() ;}
    inline enuVarComplexity complexity() const final { return _itmplVarType;}
    inline QString description(const QList<ORM::clsORMField>& _allFields) const final { return this->descriptionLambda ? this->descriptionLambda(_allFields) : QString("A value of type: %1").arg(this->PrettyTypeName);}
    inline QString toString(const QVariant _val) const final {
        if(this->hasFromVariantMethod() && this->hasToVariantMethod())
            return this->toVariantLambda(this->fromVariantLambda(_val, {})).toString();
        return QString();
    }
    inline QVariant toORMValue(const QString& _val) const final {
        return this->toORMValueLambda  == nullptr ?
                    _val :
                    this->toORMValueLambda(_val);
    }

    inline std::function<QVariant(const QVariant& _val)> fromORMValueConverter() const final {return this->fromORMValueLambda;}

private:
    std::function<QVariant(_itmplType _value)> toVariantLambda;
    std::function<_itmplType(QVariant _value, const QByteArray& _paramName)> fromVariantLambda;
    std::function<QVariant(QString _value)> toORMValueLambda;
    std::function<QVariant(const QVariant& _val)> fromORMValueLambda;
    std::function<QStringList()> optionsLambda;
    std::function<QString(const QList<ORM::clsORMField>& _allFields)> descriptionLambda;

    friend class intfCacheConnector;
};

}
}
#endif // TARGOMAN_API_TMPLAPIARG_HPP
