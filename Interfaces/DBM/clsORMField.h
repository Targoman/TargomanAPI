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

#ifndef TARGOMAN_API_DBM_CLSORMFIELD_H
#define TARGOMAN_API_DBM_CLSORMFIELD_H

#include "libQFieldValidator/QFieldValidator.h"
#include "libTargomanCommon/Macros.h"

namespace Targoman::API {

namespace Common {
class intfAPIArgManipulator;
}

namespace API {
class intfPureModule;
}

using namespace Common;
using namespace API;

namespace DBM {

#define S(_type) #_type
#define R(_schema, _table) QString("%1.%2").arg(_schema, _table)

TARGOMAN_DEFINE_ENUM(enuUpdatableBy,
                     SYSTEM,
                     ADMIN,
                     OWNER,
                     NONE,
                     __UPDATER__,
                     __CREATOR__,
                     __STATUS__
                     );

class clsORMFieldData : public QSharedData
{
public:
    clsORMFieldData();
    clsORMFieldData(const QString& _name,
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
                    const QString& _renameAs);
    clsORMFieldData(const clsORMFieldData& _o);
    ~clsORMFieldData() { ; }

    enum enuBitLocation : quint16 {
        Sortable        = 0x0001,
        Filterable      = 0x0002,
        SelfIdentifier  = 0x0004,
        PrimaryKey      = 0x0008,
        Virtual         = 0x0010,
        Selectable      = 0x0020,
        MultiLanguage   = 0x0040,
    };

    inline bool isSortable()        { return this->Privs & enuBitLocation::Sortable; }
    inline bool isFilterable()      { return this->Privs & enuBitLocation::Filterable; }
    inline bool isSelfIdentifier()  { return this->Privs & enuBitLocation::SelfIdentifier; }
    inline bool isPrimaryKey()      { return this->Privs & enuBitLocation::PrimaryKey; }
    inline bool isVirtual()         { return this->Privs & enuBitLocation::Virtual; }
    inline bool isSelectable()      { return this->Privs & enuBitLocation::Selectable; }
    inline bool isMultiLanguage()   { return this->Privs & enuBitLocation::MultiLanguage; }

    inline void setSelectable(bool _value)      { if (_value) this->Privs |= enuBitLocation::Selectable; else this->Privs &= ~enuBitLocation::Selectable; }
    inline void setMultiLanguage(bool _value)   { if (_value) this->Privs |= enuBitLocation::MultiLanguage; else this->Privs &= ~enuBitLocation::MultiLanguage; }

public:
    QMetaType::Type         ParameterType;
    QString                 Name;
    QString                 ParamTypeName;
    QVariant                DefaultValue;
    QFieldValidator         ExtraValidator;
    QString                 RenameAs;
    enuUpdatableBy::Type    UpdatableBy;
    QString                 MasterName;

private:
    quint16         Privs;
};

class clsORMField
{
public:
    clsORMField();
    clsORMField(const clsORMField& _other, const QString& _newName = {});
    clsORMField(const QString& _name,
                const QString& _type,
                const QFieldValidator& _extraValidator = QFV.allwaysValid(),
                QVariant _defaultValue = {},
                enuUpdatableBy::Type _updatableBy = enuUpdatableBy::OWNER,
                bool _isSortable = true,
                bool _isFilterable = true,
                bool _isSelfIdentifier = false,
                bool _isVirtual = false,
                bool _isPrimaryKey = false,
                bool _isSelectable = true,
                const QString& _renameAs = {});

    void registerTypeIfNotRegisterd(intfPureModule* _module);
    void updateTypeID(QMetaType::Type _type);
    void validate(const QVariant _value);

    inline QString         name() const                         { return this->Data->Name; }
    inline QString         masterName() const                   { return this->Data->MasterName; }
    inline int             parameterType() const                { return this->Data->ParameterType; }
    inline QString         paramTypeName() const                { return this->Data->ParamTypeName; }
    inline QFieldValidator extraValidator() const               { return this->Data->ExtraValidator; }
    inline bool            isSelectable() const                 { return this->Data->isSelectable(); }
    inline bool            isSortable() const                   { return this->Data->isSortable(); }
    inline bool            isFilterable() const                 { return this->Data->isFilterable(); }
    inline enuUpdatableBy::Type updatableBy() const             { return this->Data->UpdatableBy; }
    inline bool            isSelfIdentifier() const             { return this->Data->isSelfIdentifier(); }
    inline bool            isPrimaryKey() const                 { return this->Data->isPrimaryKey(); }
    inline bool            isVirtual() const                    { return this->Data->isVirtual(); }
    inline bool            isReadOnly(bool _forAll=true) const  { return this->Data->UpdatableBy == (_forAll ? enuUpdatableBy::NONE : enuUpdatableBy::ADMIN); }
    inline QString         renameAs() const                     { return this->Data->RenameAs; }
    inline QVariant        defaultValue() const                 { return this->Data->DefaultValue; }
    QString                toString(const QVariant& _value);
    QVariant               fromDB(const QString& _value);
    QVariant               toDB(const QVariant& _value);

    const intfAPIArgManipulator& argSpecs();

    inline clsORMField& setMultiLanguage() {
        this->Data->setMultiLanguage(true);
        this->Data->setSelectable(false);
        return *this;
    }
    inline bool isMultiLanguage() { return this->Data->isMultiLanguage(); }

private:
    QExplicitlySharedDataPointer<clsORMFieldData> Data;
};

static const QVariant QNull       = QVariant(QVariant::Line);
static const QVariant QInvalid    = QVariant(QVariant::Invalid);
static const QVariant QRequired   = QVariant(QVariant::RectF);
static const QVariant QAuto       = QVariant(QVariant::PointF);
static const QVariant QNow        = QAuto; //QVariant(QVariant::PointF);
static const QVariant QDBInternal = QAuto;

constexpr enuUpdatableBy::Type UPSystem = enuUpdatableBy::SYSTEM; //internal update by system
constexpr enuUpdatableBy::Type UPAdmin  = enuUpdatableBy::ADMIN;  //privileged user even if its not owner
constexpr enuUpdatableBy::Type UPOwner  = enuUpdatableBy::OWNER;  //creator or record owner
constexpr enuUpdatableBy::Type UPNone   = enuUpdatableBy::NONE;
constexpr enuUpdatableBy::Type UPStatus = enuUpdatableBy::__STATUS__;

///                      Default   UPBy     Sort  Filter Self   Virt   PK
#define ORM_PRIMARY_KEY  QAuto,    UPNone,  true, true,  false, false, true
#define ORM_SELF_REAL    QInvalid, UPOwner, true, true,  true,  false
#define ORM_SELF_VIRTUAL QInvalid, UPOwner, true, true,  true,  true

#define ORM_INVALIDATED_AT_FIELD_NAME "_InvalidatedAt"

///                                 type                                validator                   Default         UPBy    Sort  Filter Self  Virt   PK Sel
#define ORM_PRIMARYKEY_16           S(quint16),                         QFV.integer().minValue(1),  ORM_PRIMARY_KEY
#define ORM_PRIMARYKEY_32           S(quint32),                         QFV.integer().minValue(1),  ORM_PRIMARY_KEY
#define ORM_PRIMARYKEY_64           S(quint64),                         QFV.integer().minValue(1),  ORM_PRIMARY_KEY
#define ORM_CREATED_ON              S(TAPI::DateTime_t),                QFV,                        QAuto,          UPNone
#define ORM_CREATED_BY              S(quint64),                         QFV.integer().minValue(1),  QInvalid,       enuUpdatableBy::__CREATOR__
#define ORM_CREATED_BY_NULLABLE     S(NULLABLE_TYPE(quint64)),          QFV.integer().minValue(1),  QInvalid,       enuUpdatableBy::__CREATOR__
#define ORM_UPDATED_ON              S(NULLABLE_TYPE(TAPI::DateTime_t)), QFV,                        QAuto,          UPNone
#define ORM_UPDATED_BY              S(NULLABLE_TYPE(quint64)),          QFV.integer().minValue(1),  QInvalid,       enuUpdatableBy::__UPDATER__
#define ORM_STATUS_FIELD(T, Def)    S(T::Type),                         QFV,                        Def,            UPStatus
#define ORM_INVALIDATED_AT_FIELD    ORM_INVALIDATED_AT_FIELD_NAME, S(quint32), QFV,                 QDBInternal,    UPSystem, true, true, false, false, false, false

#define ORM_RELATION_OF_CREATOR_NAME "Creator"
#define ORM_RELATION_OF_UPDATER_NAME "Editor"

//              ColName             type                validator   Default     UPBy    Sort    Filter  Self    Virt    PK      Sel
#define ORM_MULTILANGUAGE(_name, _def, _upby) \
    clsORMField(_name,              S(QString),         QFV,        _def,       _upby).setMultiLanguage(), \
    {           _name##_translate,  S(TAPI::JSON_t),    QFV,        QNull,      _upby,  false,  false,  false,  true,   false,  false }

///                                  Relation Name                   Col Reference Table              ForeignCol      Rename      IsLeftJoin
#define ORM_RELATION_OF_CREATOR(F) { ORM_RELATION_OF_CREATOR_NAME, { F,  R(AAASchema, tblUser::Name), tblUser::Fields::usrID, "Creator_", true } }
#define ORM_RELATION_OF_UPDATER(F) { ORM_RELATION_OF_UPDATER_NAME, { F,  R(AAASchema, tblUser::Name), tblUser::Fields::usrID, "Updater_", true } }

#define ORM_TABLE_DBPROPERTY_INVALIDATE_QUERY           "InvalidateQuery"
#define ORM_TABLE_DBPROPERTY_STATUS_FIELD_NAME          "StatusFieldName"
#define ORM_TABLE_DBPROPERTY_INVALIDATE_AT_FIELD_NAME   "InvalidatedAtFieldName"

// used for TAPI_DEFINE_STRUCT
#define SF_ORM_PRIMARYKEY_16(_name)           SF_quint16                  (_name)
#define SF_ORM_PRIMARYKEY_32(_name)           SF_quint32                  (_name)
#define SF_ORM_PRIMARYKEY_64(_name)           SF_quint64                  (_name)
#define SF_ORM_CREATED_ON(_name)              SF_DateTime_t               (_name)
#define SF_ORM_CREATED_BY(_name)              SF_quint64                  (_name)
#define SF_ORM_CREATED_BY_NULLABLE(_name)     SF_NULLABLE_quint64         (_name)
#define SF_ORM_UPDATED_ON(_name)              SF_DateTime_t               (_name)
#define SF_ORM_UPDATED_BY(_name)              SF_NULLABLE_quint64         (_name)
#define SF_ORM_STATUS_FIELD(_name, T, Def)    SF_Enum                     (_name, T, Def)

} //namespace DBM
} //namespace Targoman::API

#endif // TARGOMAN_API_DBM_CLSORMFIELD_H
