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
 * @author S. Mehran M. Ziabary <ziabary@targoman.com>
 * @author Kambiz Zandi <kambizzandi@gmail.com>
 */

#ifndef TARGOMAN_API_MIGRATIONS_H
#define TARGOMAN_API_MIGRATIONS_H

#include "Interfaces/API/intfSQLBasedModule.h"

namespace Targoman::API::ORM {

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-variable"

namespace tblMigrations {
    constexpr char Name[] = "tblMigrations";

    namespace Fields {
        TARGOMAN_CREATE_CONSTEXPR(migName);
        TARGOMAN_CREATE_CONSTEXPR(migAppliedAt);
        TARGOMAN_CREATE_CONSTEXPR(migRunType);
        TARGOMAN_CREATE_CONSTEXPR(migStatus);
    }

    inline QStringList ColumnNames(QString _tableAlias = "") {
        if (_tableAlias.isEmpty() == false)
            _tableAlias += ".";

        return {
            _tableAlias + Fields::migName,
            _tableAlias + Fields::migAppliedAt,
            _tableAlias + Fields::migRunType,
            _tableAlias + Fields::migStatus,
        };
    }

    namespace Relation {
        // constexpr char AAA[] = "aaa";
    }

    namespace Private {
        const QList<clsORMField> ORMFields = {
            ///< ColName                       Type                Validation  Default  UpBy   Sort  Filter Self  Virt   PK
            { Fields::migName,       S(QString),         QFV,        QNull,   UPNone },
            { Fields::migAppliedAt,  S(TAPI::DateTime_t),QFV,        QNull,   UPNone },
            { Fields::migRunType,    S(QString),         QFV,        QNull,   UPNone },
            { Fields::migStatus,     S(QString),         QFV,        QNull,   UPNone },

        };

        const QList<stuRelation> Relations = {
        };

        const QList<stuDBIndex> Indexes = {
        };

    } //namespace Private

    TAPI_DEFINE_VARIANT_ENABLED_STRUCT(DTO,
        SF_QString                  (migName),
        SF_DateTime_t               (migAppliedAt),
        SF_QString                  (migRunType),
        SF_QString                  (migStatus)
    );
}

#pragma GCC diagnostic pop

class intfMigrations : public intfSQLBasedModule
{
    Q_OBJECT

public:
    intfMigrations(
        const QString &_schema,
        const QString &_name
    );

private slots:
    QVariant ANONYMOUSE_ORMGET_HIDDEN("Get migrations")
};

} //namespace Targoman::API::ORM

/****************************************************/
//put this macro before module class definition (.h)
#define TARGOMAN_MIGRATIONS_PREPARENT class Migrations;

//put this macro after module class definition (.h)
#define TARGOMAN_MIGRATIONS_POSTPARENT(_module, _schema) \
class Migrations : public Targoman::API::ORM::intfMigrations \
{ \
    Q_OBJECT \
    TARGOMAN_DEFINE_API_SUBMODULE_WO_CTOR(_module, Migrations) \
public: \
    Migrations() : \
        intfMigrations( \
            Targoman::Common::demangle(typeid(_module).name()).split("::").last(), \
            _schema \
        ) \
    { ; } \
};

//put this macro inside module class definition (.h) after TARGOMAN_DEFINE_API_MODULE
#define TARGOMAN_API_DEFINE_MIGRATIONS(_module, _schema) \
protected: \
    QScopedPointer<Migrations>  _Migrations;

//put this macro into module class constructor (.cpp)
#define TARGOMAN_API_IMPLEMENT_MIGRATIONS(_module, _schema) \
    this->_Migrations   .reset(&Migrations   ::instance()); \
    this->addSubModule(this->_Migrations.data());

/****************************************************/
#endif // TARGOMAN_API_MIGRATIONS_H
