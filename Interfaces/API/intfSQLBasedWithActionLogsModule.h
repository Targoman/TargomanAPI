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
#include "Interfaces/ORM/ActionLogs.h"

namespace Targoman::API::API {

template <class itmplDerivedClass, const char* itmplSchema>
class intfSQLBasedWithActionLogsModule : public /*intfPureModule*/ intfSQLBasedModule
{
//    Q_OBJECT

public:
    intfSQLBasedWithActionLogsModule(
        const QString& _module,
        const QString& _schema
    ) :
        intfSQLBasedModule(
            _module,
            _schema,
            ""
        )
    {
        ///TODO: complete this
//        this->ActionLogs = &ORM::ActionLogs<itmplDerivedClass, itmplSchema>::instance();
//        this->addSubModule(this->ActionLogs);
    }

//protected slots:
//    QVariant ORMGETWithName(actionLogs, "Get Action Logs")

protected:
    QScopedPointer<ORM::ActionLogs<itmplDerivedClass, itmplSchema>> ActionLogs;

//    QString Module;
};

} // namespace Targoman::API::API

#endif // TARGOMAN_API_API_INTFSQLBASEDWITHACTIONLOGSMODULE_H
