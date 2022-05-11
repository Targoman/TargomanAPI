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

#ifndef TARGOMAN_API_API_INTFSQLBASEDWITHACTIONLOGSMODULE_H
#define TARGOMAN_API_API_INTFSQLBASEDWITHACTIONLOGSMODULE_H

#include "Interfaces/API/intfSQLBasedModule.h"
#include "Interfaces/ORM/intfActionLogs.h"

//put this macro before module class definition (.h)
#define TARGOMAN_ACTIONLOG_PREPARENT class ActionLogs;

//put this macro after module class definition (.h)
#define TARGOMAN_ACTIONLOG_POSTPARENT(_module, _schema) \
class ActionLogs : public Targoman::API::ORM::intfActionLogs \
{ \
    Q_OBJECT \
    TARGOMAN_DEFINE_API_SUBMODULE_WO_CTOR(_module, ActionLogs) \
public: \
    ActionLogs() : \
        intfActionLogs( \
            Targoman::Common::demangle(typeid(_module).name()).split("::").last(), \
            _schema \
        ) \
    { ; } \
};

//put this macro inside module class definition (.h) after TARGOMAN_DEFINE_API_MODULE
#define TARGOMAN_API_DEFINE_ACTIONLOG(_module, _schema) \
protected: \
    QScopedPointer<ActionLogs> _ActionLogs;

//put this macro into module class constructor (.cpp)
#define TARGOMAN_API_IMPLEMENT_ACTIONLOG(_module, _schema) \
    this->_ActionLogs.reset(&ActionLogs::instance()); \
    this->addSubModule(this->_ActionLogs.data());

namespace Targoman::API::API {

class intfSQLBasedWithActionLogsModule : public intfSQLBasedModule
{
public:
    intfSQLBasedWithActionLogsModule(
        const QString& _module,
        const QString& _schema
    );
};

} // namespace Targoman::API::API

#endif // TARGOMAN_API_API_INTFSQLBASEDWITHACTIONLOGSMODULE_H
