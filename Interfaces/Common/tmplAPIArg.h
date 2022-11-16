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

//#include "Interfaces/Common/GenericTypes.h"
#include "Interfaces/Common/HTTPExceptions.hpp"
#include "Interfaces/Common/intfAPIArgManipulator.h"
#include "Interfaces/Server/APICallBoom.h"
#include "libTargomanCommon/Logger.h"
#include "libTargomanCommon/CmdIO.h"

using namespace Targoman::API::Server;

using namespace Targoman::Common;

namespace Targoman::API {

namespace Server {
class intfCacheConnector;

extern QList<Targoman::API::Common::intfAPIArgManipulator*> gOrderedMetaTypeInfo;
extern QList<Targoman::API::Common::intfAPIArgManipulator*> gUserDefinedTypesInfo;

constexpr quint16 TAPI_BASE_USER_DEFINED_TYPEID = QMetaType::User + 3;

inline void registerUserDefinedType(const char* _typeName, Common::intfAPIArgManipulator* _argManipulator) {
    Q_ASSERT_X(QMetaType::type(_typeName), QString("registerUserDefinedType typeName(%1)").arg(_typeName).toStdString().c_str(), "Seems that registering syntax is erroneous");

//    TargomanDebug(5).noLabel() << "registerUserDefinedType" << _typeName;

    gUserDefinedTypesInfo.insert(QMetaType::type(_typeName) - TAPI_BASE_USER_DEFINED_TYPEID, _argManipulator);
}

}

namespace Common {

template <typename _itmplType, enuVarComplexity _itmplVarType, bool _itmplNullable, bool _isQtType = false>
class tmplAPIArg : public intfAPIArgManipulator
{
public:
    virtual ~tmplAPIArg() { ; }

    virtual QGenericArgument makeGenericArgument(const QVariant& _val, const QByteArray& _paramName, void** _argStorage) final
    {
        *_argStorage = nullptr;

        ///@TODO: check if _val is null
        /// test case: Account/User/extraInfo with not first param (e.g. education) value, then first param (birthDate) throws

//        if (_paramName == "birthDate")
//        {
//            try {
//                _itmplType vvv;

//                if (this->fnFromVariant == nullptr)
//                    vvv = _val.value<_itmplType>();
//                else
//                    vvv = this->fnFromVariant(_val, _paramName);

//            }  catch (std::exception &exp) {
//                qDebug() << exp.what();
//            }
//        }

        if (this->fnFromVariant == nullptr && (_val.canConvert<_itmplType>() == false))
            if (_val.isValid())
                throw exHTTPBadRequest("Invalid value specified for parameter.: " + _paramName);

        *_argStorage = new _itmplType;

        *(reinterpret_cast<_itmplType*>(*_argStorage)) =
                this->fnFromVariant == nullptr ? _val.value<_itmplType>() : this->fnFromVariant(_val, _paramName);

        return QGenericArgument(this->RealTypeName, *_argStorage);
    }

    inline QVariant invokeMethod(
            const intfAPIObject *_apiObject,
            INTFAPICALLBOOM_DECL *APICALLBOOM_PARAM,
            const QVariantList& _arguments
//            /*OUT*/ QVariantMap &_responseHeaders
        ) final
    {
        _itmplType Result;

        _apiObject->invokeMethod(
                    APICALLBOOM_PARAM,
                    _arguments,
                    QReturnArgument<_itmplType>(this->RealTypeName, Result)
//                    _responseHeaders
                    );

        return this->fnToVariant == nullptr ? QVariant::fromValue(Result) : this->fnToVariant(Result);
    }

    inline void validate(const QVariant& _val, const QByteArray& _paramName) const final
    {
        if (this->fnFromVariant == nullptr && !_val.canConvert<_itmplType>())
            throw exHTTPBadRequest("Invalid value specified for parameter.:: " + _paramName);

        if (this->fnFromVariant)
            this->fnFromVariant(_val, _paramName);
    }

    inline QVariant defaultVariant() const
    {
        _itmplType Default = {};
        return this->fnToVariant == nullptr ? QVariant::fromValue(Default) : this->fnToVariant(Default);
    }

    inline void cleanup (void* _argStorage) final {if (_argStorage) delete (reinterpret_cast<_itmplType*>(_argStorage));}
    inline bool hasFromVariantMethod() const final { return this->fnFromVariant != nullptr;}
    inline bool hasToVariantMethod() const final { return this->fnToVariant != nullptr;}
    inline bool isPrimitiveType() const final { return _itmplVarType == COMPLEXITY_Integral;}
    inline QStringList options() const final { return this->fnOptions ? this->fnOptions() : QStringList() ;}
    inline enuVarComplexity complexity() const final { return _itmplVarType;}
    inline bool isNullable() const final { return _itmplNullable;}
    inline QString description(const QList<DBM::clsORMField>& _allFields) const final { 
        return this->fnDescription ? this->fnDescription(_allFields) : QString("A value of type: %1").arg(this->PrettyTypeName);
    }
    inline QString toString(const QVariant _val) const final {
        if (this->hasFromVariantMethod() && this->hasToVariantMethod())
            return this->fnToVariant(this->fnFromVariant(_val, {})).toString();
        return QString();
    }

    inline QVariant toORMValue(const QString& _val) const final {
        return this->fnToORMValue == nullptr ?
                    _val :
                    this->fnToORMValue(_val);
    }
    
    inline std::function<QVariant(const QVariant& _val)> fromORMValueConverter() const final { return this->fnFromORMValue;}
    inline std::function<QVariant(const QVariant& _val)> toORMValueConverter() const final { return this->fnToORMValue;}

    static _itmplType fromVariant(QVariant _value, const QByteArray& _paramName = {}) {
        if (tmplAPIArg<_itmplType, _itmplVarType, _itmplNullable, _isQtType>::fnFromVariant)
            return tmplAPIArg<_itmplType, _itmplVarType, _itmplNullable, _isQtType>::fnFromVariant(_value, _paramName);
        return _value.value<_itmplType>();
    }

    static QVariant toVariant(_itmplType _value) {
        if (tmplAPIArg<_itmplType, _itmplVarType, _itmplNullable, _isQtType>::fnToVariant)
            return tmplAPIArg<_itmplType, _itmplVarType, _itmplNullable, _isQtType>::fnToVariant(_value);
        return QVariant::fromValue(_value);
    }

    static tmplAPIArg* instance(const char* _typeStr) {
        static tmplAPIArg* Instance = nullptr; return Q_LIKELY(Instance) ? Instance : (Instance = new tmplAPIArg(_typeStr));
    }
private:
    tmplAPIArg(const char* _typeStr) : intfAPIArgManipulator(_typeStr) {
        qRegisterMetaType<_itmplType>();
        if (!_isQtType)
            registerUserDefinedType(_typeStr, this);
    }

private:
    static std::function<QVariant(_itmplType _value)>                               fnToVariant;
    static std::function<_itmplType(QVariant _value, const QByteArray& _paramName)> fnFromVariant;
    static std::function<QVariant(const QVariant& _val)>                            fnToORMValue;
    static std::function<QVariant(const QVariant& _val)>                            fnFromORMValue;
    static std::function<QStringList()>                                             fnOptions;
    static std::function<QString(const QList<DBM::clsORMField>& _allFields)>        fnDescription;

    friend class intfCacheConnector;
};

} //namespace Common
} //namespace Targoman::API

#endif // TARGOMAN_API_TMPLAPIARG_HPP
