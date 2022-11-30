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

#ifndef TARGOMAN_API_I18N_H
#define TARGOMAN_API_I18N_H

#include "Interfaces/API/intfSQLBasedModule.h"

namespace Targoman::API::ORM {

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-variable"

namespace tblI18N {
    constexpr char Name[] = "tblI18N";

    namespace Fields {
        TARGOMAN_CREATE_CONSTEXPR(i18nKey);
        TARGOMAN_CREATE_CONSTEXPR(i18nValue);
    }

    namespace Relation {
        // constexpr char AAA[] = "aaa";
    }

    namespace Private {
        const QList<clsORMField> ORMFields = {
            //ColName               Type                Validation  Default     UpBy    Sort  Filter Self   Virt   PK
            { Fields::i18nKey,      S(QString),         QFV,        QRequired,  UPNone, true, true,  false, false, true },
            { Fields::i18nValue,    S(TAPI::JSON_t),    QFV,        QRequired,  UPOwner },
        };

        const QList<stuRelation> Relations = {
        };

        const QList<stuDBIndex> Indexes = {
        };

    } //namespace Private

    TAPI_DEFINE_STRUCT(DTO,
        SF_QString                  (i18nKey),
        SF_JSON_t                   (i18nValue)
    );
}
#pragma GCC diagnostic pop

class intfI18N : public intfSQLBasedModule
{
    Q_OBJECT

public:
    intfI18N(
            const QString &_module,
            const QString &_schema
            );

public:
    QString translated(
            INTFAPICALLCONTEXT_DECL &_apiCallContext,
            const QString &_key);

private slots:
    QVariant ORMGET_ANONYMOUSE("Get I18N")
};

} //namespace Targoman::API::ORM

// .h:
//----------------------------------------------------
//put this macro before module class definition (.h)
#define TARGOMAN_I18N_PREPARENT class I18N;

//Q _OBJECT
//put this macro after module class definition (.h)
#define TARGOMAN_I18N_POSTPARENT(_module, _schema) \
class I18N : public Targoman::API::ORM::intfI18N \
{ \
    TARGOMAN_API_SUBMODULE_DEFINE_WO_CTOR(_module, I18N) \
public: \
    I18N() : \
        intfI18N( \
            Targoman::Common::demangle(typeid(_module).name()).split("::").last(), \
            _schema \
        ) \
    { ; } \
};

//put this macro inside module class definition (.h) after TARGOMAN_API_MODULE_DEFINE
#define TARGOMAN_API_MODULE_DEFINE_I18N(_module, _schema) \
protected: \
    QScopedPointer<I18N> _I18N; \
    virtual Targoman::API::ORM::intfI18N* i18n();

// .cpp:
//----------------------------------------------------
//put this macro before module class constructor (.cpp)
#define TARGOMAN_API_MODULE_IMPLEMENT_I18N(_module, _schema) \
    TARGOMAN_API_SUBMODULE_IMPLEMENT(_module, I18N) \
    Targoman::API::ORM::intfI18N* _module::i18n() { return dynamic_cast<Targoman::API::ORM::intfI18N*>(_I18N.data()); }

//put this macro into module class constructor (.cpp)
#define TARGOMAN_API_MODULE_IMPLEMENT_CTOR_I18N(_module, _schema) \
    this->_I18N.reset(&I18N::instance()); \
    this->addSubModule(this->_I18N.data());

/****************************************************/
#endif // TARGOMAN_API_I18N_H
