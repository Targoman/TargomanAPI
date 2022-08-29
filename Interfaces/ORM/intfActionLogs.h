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

#ifndef TARGOMAN_API_ACTIONLOGS_H
#define TARGOMAN_API_ACTIONLOGS_H

#include "Interfaces/API/intfSQLBasedModule.h"
#include "Interfaces/AAA/AAADefs.hpp"

using namespace Targoman::API::DBM;
using namespace Targoman::API::API;

namespace Targoman::API::ORM {

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-variable"

namespace tblActionLogs {
    constexpr char Name[] = "tblActionLogs";

    namespace Fields {
        TARGOMAN_CREATE_CONSTEXPR(atlID);
        TARGOMAN_CREATE_CONSTEXPR(atlBy_usrID);
        TARGOMAN_CREATE_CONSTEXPR(atlInsertionDateTime);
        TARGOMAN_CREATE_CONSTEXPR(atlType);
        TARGOMAN_CREATE_CONSTEXPR(atlDescription);
    }

    namespace Relation {
        // constexpr char AAA[] = "aaa";
    }

    namespace Private {
        const QList<clsORMField> ORMFields = {
            ///ColName                               Type                Validation                      Default  UpBy   Sort  Filter Self  Virt   PK
            { Fields::atlID,                 ORM_PRIMARYKEY_64},
            { Fields::atlBy_usrID,           S(quint64),         QFV.integer().minValue(1),      {},      UPNone },
            { Fields::atlInsertionDateTime,  S(TAPI::DateTime_t),QFV,                            {},      UPNone },
            { Fields::atlType,               S(QString),         QFV.asciiAlNum().maxLenght(50), {},      UPNone },
            { Fields::atlDescription,        S(QString),         QFV.allwaysInvalid(),           {},      UPNone, false, false },
        };

        const QList<stuRelation> Relations = {
//            { Fields::atlBy_usrID,           R(AAA::AAASchema,  "tblUser"),  "usrID",    "By_" },
        };

        const QList<stuDBIndex> Indexes = {
        };

    } //namespace Private

    TAPI_DEFINE_STRUCT(DTO,
        SF_ORM_PRIMARYKEY_64        (atlID),
        SF_quint64                  (atlBy_usrID),
        SF_DateTime_t               (atlInsertionDateTime),
        SF_QString                  (atlType),
        SF_QString                  (atlDescription)
    );
}

#pragma GCC diagnostic pop

class intfActionLogs : public intfSQLBasedModule
{
    Q_OBJECT

public:
    intfActionLogs(
        const QString& _module,
        const QString& _schema
    );

protected slots:
    QVariant ORMGET("Get Action Logs.")
};

} //namespace Targoman::API::ORM

/****************************************************/
//put this macro before module class definition (.h)
#define TARGOMAN_ACTIONLOG_PREPARENT class ActionLogs;

//put this macro after module class definition (.h)
#define TARGOMAN_ACTIONLOG_POSTPARENT(_module, _schema) \
class ActionLogs : public Targoman::API::ORM::intfActionLogs \
{ \
    Q_OBJECT \
    TARGOMAN_API_SUBMODULE_DEFINE_WO_CTOR(_module, ActionLogs) \
public: \
    ActionLogs() : \
        intfActionLogs( \
            Targoman::Common::demangle(typeid(_module).name()).split("::").last(), \
            _schema \
        ) \
    { ; } \
};

//put this macro inside module class definition (.h) after TARGOMAN_API_MODULE_DEFINE
#define TARGOMAN_API_MODULE_DEFINE_ACTIONLOG(_module, _schema) \
protected: \
    QScopedPointer<ActionLogs> _ActionLogs;

//put this macro before module class constructor (.cpp)
#define TARGOMAN_API_MODULE_IMPLEMENT_ACTIONLOG(_module, _schema) \
    TARGOMAN_API_SUBMODULE_IMPLEMENT(_module, ActionLogs)

//put this macro into module class constructor (.cpp)
#define TARGOMAN_API_MODULE_IMPLEMENT_CTOR_ACTIONLOG(_module, _schema) \
    this->_ActionLogs.reset(&ActionLogs::instance()); \
    this->addSubModule(this->_ActionLogs.data());

/****************************************************/

#endif // TARGOMAN_API_ACTIONLOGS_H
