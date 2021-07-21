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
      UpdatableBy(_o.UpdatableBy),
      Privs(_o.Privs)
{
    if (this->ParamTypeName.startsWith("NULLABLE_TYPE(")) {
        this->ParamTypeName
            .replace("(", "<")
            .replace(")", ">")
            .replace("NULLABLE_TYPE", NULLABLE_UNDERLAYER_CLASS_NAME)
        ;
    }
    this->ParameterType = static_cast<QMetaType::Type>(QMetaType::type(this->ParamTypeName.toUtf8()));
}

clsORMFieldData::clsORMFieldData(const QString& _name,
                const QString& _type,
                QVariant _defaultValue,
                const QFieldValidator& _extraValidator,
                enuUpdatableBy::Type _updatableBy,
                bool _isSortable,
                bool _isFilterable,
                bool _isSelfIdentifier,
                bool _isVirtual,
                bool _isPrimaryKey,
                bool _isSelectable,
                const QString& _renameAs):
    ParameterType(static_cast<QMetaType::Type>(QMetaType::type(_type.toUtf8()))),
    Name(_name),
    ParamTypeName(_type),
    DefaultValue(_defaultValue),
    ExtraValidator(_extraValidator),
    RenameAs(_renameAs),
    UpdatableBy(_updatableBy),
    Privs((_isSortable ? 0x01 : 0) +
          (_isFilterable ? 0x02 : 0) +
          (_isSelfIdentifier ? 0x4 : 0) +
          (_isPrimaryKey ? 0x8 : 0) +
          (_isVirtual ? 0x10 : 0) +
          (_isSelectable ? 0x20 : 0)
    )
{
    if (this->ParamTypeName.startsWith("NULLABLE_TYPE(")) {
        this->ParamTypeName
            .replace("(", "<")
            .replace(")", ">")
            .replace("NULLABLE_TYPE", NULLABLE_UNDERLAYER_CLASS_NAME)
        ;
    }
    this->ParameterType = static_cast<QMetaType::Type>(QMetaType::type(this->ParamTypeName.toUtf8()));
}

void clsORMField::registerTypeIfNotRegisterd(intfAPIModule* _module)
{
    if (Q_UNLIKELY(this->Data->ParameterType == QMetaType::UnknownType)) {
        this->Data->ParameterType = static_cast<QMetaType::Type>(QMetaType::type(this->Data->ParamTypeName.toLatin1()));
        if (this->Data->ParameterType == QMetaType::UnknownType)
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
//    qDebug() << "clsORMField::argSpecs()" << this->Data->Name << this->Data->ParamTypeName << this->Data->ParameterType;

    if (Q_UNLIKELY(this->Data->ParameterType == QMetaType::UnknownType))
        this->Data->ParameterType = static_cast<QMetaType::Type>(QMetaType::type(this->Data->ParamTypeName.toUtf8()));

    Q_ASSERT(this->Data->ParameterType != QMetaType::UnknownType);
    return (this->Data->ParameterType < TAPI_BASE_USER_DEFINED_TYPEID)
        ? *gOrderedMetaTypeInfo.at(this->Data->ParameterType)
        : *gUserDefinedTypesInfo.at(this->Data->ParameterType - TAPI_BASE_USER_DEFINED_TYPEID);
}

QString  clsORMField::toString(const QVariant& _value)
{
    return this->argSpecs().toString(_value);
}

QVariant clsORMField::toDB(const QVariant& _value)
{
    return this->argSpecs().fromORMValueConverter() ? this->argSpecs().fromORMValueConverter()(_value) : _value;
}

QVariant clsORMField::fromDB(const QString& _value)
{
    return this->argSpecs().toORMValue(_value);
}

}
}
}
