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
#include "Interfaces/Common/APIArgHelperMacros.hpp"

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
class intfAPIArgManipulator {
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
    virtual std::function<QVariant(const QVariant& _val)> toORMValueConverter() const = 0;
    virtual bool isNullable() const = 0;

    QString     PrettyTypeName;
    char*       RealTypeName;
};

extern void registerUserDefinedType(const char* _typeName, intfAPIArgManipulator* _argManipulator);
#pragma clang diagnostic pop

}
}

#endif // TAPI_INTFAPIARGMANIPULATOR_H
