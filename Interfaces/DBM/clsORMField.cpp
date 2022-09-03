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
#include "Interfaces/Common/base.h"
#include "Interfaces/Common/intfAPIArgManipulator.h"
#include "Interfaces/Common/tmplAPIArg.h"
#include "Interfaces/Server/ServerCommon.h"
#include "Interfaces/API/intfPureModule.h"
//#include "Server/ServerConfigs.h"
//#include "Server/RESTAPIRegistry.h"

using namespace Targoman::API::Common;
using namespace Targoman::API::API;
using namespace Targoman::API::Server;

namespace Targoman::API::DBM {

clsORMFieldData::clsORMFieldData() :
    ParameterType(QMetaType::UnknownType) { ; }

clsORMFieldData::clsORMFieldData(const clsORMFieldData& _o) :
    QSharedData(_o),
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

clsORMFieldData::clsORMFieldData(
    const QString& _name,
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
    bool _isMultiLanguage,
    const QString& _renameAs
) :
    ParameterType(static_cast<QMetaType::Type>(QMetaType::type(_type.toUtf8()))),
    Name(_name),
    ParamTypeName(_type),
    DefaultValue(_defaultValue),
    ExtraValidator(_extraValidator),
    RenameAs(_renameAs),
    UpdatableBy(_updatableBy),
    Privs((_isSortable          ? enuBitLocation::Sortable       : 0) +
          (_isFilterable        ? enuBitLocation::Filterable     : 0) +
          (_isSelfIdentifier    ? enuBitLocation::SelfIdentifier : 0) +
          (_isPrimaryKey        ? enuBitLocation::PrimaryKey     : 0) +
          (_isVirtual           ? enuBitLocation::Virtual        : 0) +
          (_isSelectable        ? enuBitLocation::Selectable     : 0) +
          (_isMultiLanguage     ? enuBitLocation::MultiLanguage  : 0)
) {
    if (this->ParamTypeName.startsWith("NULLABLE_TYPE(")) {
        this->ParamTypeName
            .replace("(", "<")
            .replace(")", ">")
            .replace("NULLABLE_TYPE", NULLABLE_UNDERLAYER_CLASS_NAME)
        ;
    }
    this->ParameterType = static_cast<QMetaType::Type>(QMetaType::type(this->ParamTypeName.toUtf8()));
}

/****************************************************************/
clsORMField::clsORMField() :
    Data(new clsORMFieldData) { ; }

clsORMField::clsORMField(const clsORMField& _other, const QString& _newName) :
    Data(_other.Data) {
    if (_newName.length() && (this->Data->Name != _newName)) {
//        print_stacktrace();
        this->Data.detach();
        this->Data->MasterName = this->Data->Name;
        this->Data->Name = _newName;
        this->Data->RenameAs = "";
    }
}

clsORMField::clsORMField(
    const QString& _name,
    const QString& _type,
    const QFieldValidator& _extraValidator,
    QVariant _defaultValue,
    enuUpdatableBy::Type _updatableBy,
    bool _isSortable,
    bool _isFilterable,
    bool _isSelfIdentifier,
    bool _isVirtual,
    bool _isPrimaryKey,
    bool _isSelectable,
    const QString& _renameAs
) :
    Data(new clsORMFieldData(
             _name,
             _type,
             _defaultValue,
             _extraValidator,
             _updatableBy,
             _isSortable,
             _isFilterable,
             _isSelfIdentifier,
             _isVirtual,
             _isPrimaryKey,
             _isSelectable,
             false,
             _renameAs
             )
) { ; }

void clsORMField::registerTypeIfNotRegisterd(intfPureModule* _module) {
    if (Q_UNLIKELY(this->Data->ParameterType == QMetaType::UnknownType)) {
        this->Data->ParameterType = static_cast<QMetaType::Type>(QMetaType::type(this->Data->ParamTypeName.toLatin1()));

        if (this->Data->ParameterType == QMetaType::UnknownType)
            throw exRESTRegistry("Unregistered type: <"+this->Data->ParamTypeName+">");

        _module->updateFilterParamType(this->Data->ParamTypeName, this->Data->ParameterType);
    }
}

void clsORMField::updateTypeID(QMetaType::Type _type) {
    this->Data->ParameterType = _type;
}

void clsORMField::validate(const QVariant _value) {
    this->argSpecs().validate(_value, this->Data->Name.toLatin1());
}

const intfAPIArgManipulator& clsORMField::argSpecs() {
//    if (this->Data->ParameterType == QMetaType::UnknownType)
//        qDebug() << "clsORMField::argSpecs()" << this->Data->Name << this->Data->ParamTypeName << this->Data->ParameterType;

    if (Q_UNLIKELY(this->Data->ParameterType == QMetaType::UnknownType))
        this->Data->ParameterType = static_cast<QMetaType::Type>(QMetaType::type(this->Data->ParamTypeName.toUtf8()));

    Q_ASSERT(this->Data->ParameterType != QMetaType::UnknownType);

    return (this->Data->ParameterType < TAPI_BASE_USER_DEFINED_TYPEID)
        ? *gOrderedMetaTypeInfo.at(this->Data->ParameterType)
        : *gUserDefinedTypesInfo.at(this->Data->ParameterType - TAPI_BASE_USER_DEFINED_TYPEID);
}

QString clsORMField::toString(const QVariant& _value) {
    return this->argSpecs().toString(_value);
}

QVariant clsORMField::toDB(const QVariant& _value) {
//    qDebug() << "***********************" << __FUNCTION__ << _value << this->name() << this->parameterType() << this->paramTypeName()
//             << (this->argSpecs().fromORMValueConverter() ? "has fromORMValueConverter" : "return _value")
//             << QMetaType::QChar << _value.toString();

    if (this->argSpecs().fromORMValueConverter())
        return this->argSpecs().fromORMValueConverter()(_value);

    if (this->parameterType() == QMetaType::QChar)
        return QVariant(_value.value<QChar>());

    if (this->parameterType() == QMetaType::Bool)
        return ((_value == 1) || (_value == "1") || (_value == true));

//    if (this->parameterType() == QMetaType::Int)       return _value.toInt();
//    if (this->parameterType() == QMetaType::UInt)      return _value.toUInt();
//    if (this->parameterType() == QMetaType::LongLong)  return _value.toLongLong();
//    if (this->parameterType() == QMetaType::ULongLong) return _value.toULongLong();
//    if (this->parameterType() == QMetaType::Double)    return _value.toDouble();
//    if (this->parameterType() == QMetaType::Long)      return _value.toLongLong();
//    if (this->parameterType() == QMetaType::Short)     return _value.toInt(); //Short();
//    if (this->parameterType() == QMetaType::Char)      return _value.toChar();
//    if (this->parameterType() == QMetaType::ULong)     return _value.toULongLong();
//    if (this->parameterType() == QMetaType::UShort)    return _value.toUInt(); //Short();
//    if (this->parameterType() == QMetaType::UChar)     return _value.toUInt(); //Char();
//    if (this->parameterType() == QMetaType::Float)     return _value.toFloat();
//    if (this->parameterType() == QMetaType::SChar)     return _value.toChar();

    return _value;
}

QVariant clsORMField::fromDB(const QString& _value) {
    return this->argSpecs().toORMValue(_value);
}

clsORMField& clsORMField::setMultiLanguage() {
    this->Data->setMultiLanguage(true);
//    this->Data->setSelectable(false);

    return *this;
}

} //namespace Targoman::API::DBM
