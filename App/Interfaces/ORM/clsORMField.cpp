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

#include "clsORMField.h"
#include "Server/ServerConfigs.h"
#include "Server/RESTAPIRegistry.h"
#include "Interfaces/Common/intfAPIArgManipulator.h"

namespace Targoman {
namespace API {
namespace ORM {

using namespace Targoman::API::Server;

clsORMFieldData::clsORMFieldData() : ParameterType(QMetaType::UnknownType) { }

clsORMFieldData::clsORMFieldData(const clsORMFieldData& _o)
    : QSharedData(_o),
      ParameterType(_o.ParameterType),
      Name(_o.Name),
      ParamTypeName(_o.ParamTypeName),
      DefaultValue(_o.DefaultValue),
      ExtraValidator(_o.ExtraValidator),
      RenameAs(_o.RenameAs),
      IsSortable(_o.IsSortable),
      IsFilterable(_o.IsFilterable),
      IsReadOnly(_o.IsReadOnly),
      IsSelfIdentifier(_o.IsSelfIdentifier),
      IsVirtual(_o.IsVirtual),
      IsPrimaryKey(_o.IsPrimaryKey)
{}


void clsORMField::registerTypeIfNotRegisterd(intfAPIModule* _module)
{
    if(Q_UNLIKELY(this->Data->ParameterType == QMetaType::UnknownType)){
        this->Data->ParameterType = static_cast<QMetaType::Type>(QMetaType::type(this->Data->ParamTypeName.toUtf8()));
        if(this->Data->ParameterType == QMetaType::UnknownType)
            throw exRESTRegistry("Unregistered type: <"+this->Data->ParamTypeName+">");
        _module->updateFilterParamType(this->Data->ParamTypeName, this->Data->ParameterType);
    }
}

void clsORMField::updateTypeID(QMetaType::Type _type)
{
    this->Data->ParameterType = _type;
}

void clsORMField::validate(const QVariant _value)
{
    this->argSpecs().validate(_value, this->Data->Name.toLatin1());
}

const intfAPIArgManipulator& clsORMField::argSpecs()
{
    if(Q_UNLIKELY(this->Data->ParameterType == QMetaType::UnknownType))
        this->Data->ParameterType = static_cast<QMetaType::Type>(QMetaType::type(this->Data->ParamTypeName.toUtf8()));

    Q_ASSERT(this->Data->ParameterType != QMetaType::UnknownType);
    return this->Data->ParameterType < TAPI_BASE_USER_DEFINED_TYPEID ?
                *gOrderedMetaTypeInfo.at(this->Data->ParameterType) :
                *gUserDefinedTypesInfo.at(this->Data->ParameterType - TAPI_BASE_USER_DEFINED_TYPEID);
}

QString  clsORMField::toString(const QVariant& _value)
{
    return this->argSpecs().toString(_value);
}

QVariant clsORMField::fromDB(const QVariant& _value)
{
    return this->argSpecs().fromORMValueConverter()(_value);
}

QVariant clsORMField::toDB(const QString& _value)
{
    return this->argSpecs().toORMValue(_value);
}

}
}
}
