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

#ifndef TARGOMAN_API_CONFIGURATIONS_H
#define TARGOMAN_API_CONFIGURATIONS_H

#include "Interfaces/API/intfSQLBasedModule.h"

namespace Targoman::API::ORM {

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-variable"

namespace tblConfigurations {
    constexpr char Name[] = "tblConfigurations";

    namespace Fields {
        TARGOMAN_CREATE_CONSTEXPR(cfgKey);
        TARGOMAN_CREATE_CONSTEXPR(cfgValue);
    }

    namespace Relation {
        // constexpr char AAA[] = "aaa";
    }

    namespace Private {
        const QList<clsORMField> ORMFields = {
            //ColName               Type                Validation  Default     UpBy    Sort  Filter Self   Virt   PK
            { Fields::cfgKey,       S(QString),         QFV,        QRequired,  UPNone, true, true,  false, false, true },
            { Fields::cfgValue,     S(TAPI::JSON_t),    QFV,        QRequired,  UPOwner },
        };

        const QList<stuRelation> Relations = {
        };

        const QList<stuDBIndex> Indexes = {
        };

    } //namespace Private

    TAPI_DEFINE_STRUCT(DTO,
        SF_QString                  (cfgKey),
        SF_JSON_t                   (cfgValue)
    );
}

#pragma GCC diagnostic pop

class intfConfigurations : public intfSQLBasedModule
{
    Q_OBJECT

public:
    intfConfigurations(
        const QString &_module,
        const QString &_schema
    );

private slots:
    QVariant ORMGET_ANONYMOUSE("Get Configurations")
};

} //namespace Targoman::API::ORM

// .h:
//----------------------------------------------------
//put this macro before module class definition (.h)
#define TARGOMAN_CONFIGURATIONS_PREPARENT class Configurations;

//Q _OBJECT
//put this macro after module class definition (.h)
#define TARGOMAN_CONFIGURATIONS_POSTPARENT(_module, _schema) \
class Configurations : public Targoman::API::ORM::intfConfigurations \
{ \
    TARGOMAN_API_SUBMODULE_DEFINE_WO_CTOR(_module, Configurations) \
public: \
    Configurations() : \
        intfConfigurations( \
            Targoman::Common::demangle(typeid(_module).name()).split("::").last(), \
            _schema \
        ) \
    { ; } \
};

//put this macro inside module class definition (.h) after TARGOMAN_API_MODULE_DEFINE
#define TARGOMAN_API_MODULE_DEFINE_CONFIGURATIONS(_module, _schema) \
protected: \
    QScopedPointer<Configurations> _Configurations;

// .cpp:
//----------------------------------------------------
//put this macro before module class constructor (.cpp)
#define TARGOMAN_API_MODULE_IMPLEMENT_CONFIGURATIONS(_module, _schema) \
    TARGOMAN_API_SUBMODULE_IMPLEMENT(_module, Configurations)

//put this macro into module class constructor (.cpp)
#define TARGOMAN_API_MODULE_IMPLEMENT_CTOR_CONFIGURATIONS(_module, _schema) \
    this->_Configurations.reset(&Configurations::instance()); \
    this->addSubModule(this->_Configurations.data());

/****************************************************/
#endif // TARGOMAN_API_CONFIGURATIONS_H
