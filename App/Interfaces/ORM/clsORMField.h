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

#ifndef TARGOMAN_API_ORM_CLSORMFIELD_H
#define TARGOMAN_API_ORM_CLSORMFIELD_H

#include "QFieldValidator.h"

namespace Targoman {
namespace API {
class intfRESTAPIHolder;
class intfAPIArgManipulator;

namespace ORM {

#define S(_type) #_type

class clsORMFieldData : public QSharedData{
public:
    clsORMFieldData();
    clsORMFieldData(const QString& _name,
                    const QString& _type,
                    QVariant _defaultValue,
                    const QFieldValidator& _extraValidator,
                    bool _isReadOnly,
                    bool _isSortable,
                    bool _isFilterable,
                    bool _isSelfIdentifier,
                    bool _isVirtual,
                    bool _isPrimaryKey,
                    const QString& _renameAs);
    clsORMFieldData(const clsORMFieldData& _o);
    ~clsORMFieldData() { }

public:
    QString         Name;
    QMetaType::Type ParameterType;
    QString         ParamTypeName;
    QVariant        DefaultValue;
    QFieldValidator ExtraValidator;
    bool            IsSortable;
    bool            IsFilterable;
    bool            IsReadOnly;
    bool            IsSelfIdentifier;
    bool            IsVirtual;
    bool            IsPrimaryKey;
    QString         RenameAs;
};

class clsORMField{
public:
    clsORMField();
    clsORMField(const clsORMField& _other, const QString& _newName = QString());
    clsORMField(const QString& _name,
                const QString& _type,
                const QFieldValidator& _extraValidator = QFV.allwaysValid(),
                QVariant _defaultValue = {},
                bool _isReadOnly = false,
                bool _isSortable = true,
                bool _isFilterable = true,
                bool _isSelfIdentifier = false,
                bool _isVirtual = false,
                bool _isPrimaryKey = false,
                const QString& _renameAs = {});
    ~clsORMField();
    void registerTypeIfNotRegisterd(intfRESTAPIHolder* _module);
    void updateTypeID(QMetaType::Type _type);
    void validate(const QVariant _value);
    inline QString         name() const {return this->Data->Name;}
    inline int             parameterType() const {return this->Data->ParameterType;}
    inline QString         paramTypeName() const {return this->Data->ParamTypeName;}
    inline QFieldValidator extraValidator() const {return this->Data->ExtraValidator;}
    inline bool            isSortable() const {return this->Data->IsSortable;}
    inline bool            isFilterable() const {return this->Data->IsFilterable;}
    inline bool            isReadOnly() const {return this->Data->IsReadOnly;}
    inline bool            isSelfIdentifier() const {return this->Data->IsSelfIdentifier;}
    inline bool            isVirtual() const {return this->Data->IsVirtual;}
    inline bool            isPrimaryKey() const {return this->Data->IsPrimaryKey;}
    inline QString         renameAs() const {return this->Data->RenameAs;}
    inline QVariant        defaultValue() const {return this->Data->DefaultValue;}
    QString                toString(const QVariant& _value);
    QVariant               toDB(const QString& _value);
    QVariant               fromDB(const QVariant& _value);
    const intfAPIArgManipulator& argSpecs();

private:
    QExplicitlySharedDataPointer<clsORMFieldData> Data;
};

static const QVariant       QNull    = QVariant();
static const QVariant       QInvalid = QVariant(QVariant::Invalid);

///                         Default     RO   Sort  Filter Self  Virt   PK
#define ORM_PRIMARY_KEY     QInvalid,   true, true, true, false, false, true
#define ORM_SELF_REAL       QInvalid,   true, true, true, true, false
#define ORM_SELF_VIRTUAL    QInvalid,   true, true, true, true, true

}
}
}
#endif // TARGOMAN_API_ORM_CLSORMFIELD_H
