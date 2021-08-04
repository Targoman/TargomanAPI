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

#ifndef TARGOMAN_API_ORM_CLSRESTAPIWITHACTIONLOGS_HPP
#define TARGOMAN_API_ORM_CLSRESTAPIWITHACTIONLOGS_HPP

#include "Interfaces/ORM/clsTable.h"
//#include "Interfaces/AAA/AAADefs.hpp"

namespace Targoman::API::ORM {

class clsRESTAPIWithActionLogs : public ORM::clsTable
{
    Q_OBJECT

public:
    clsRESTAPIWithActionLogs(const QString& _schema, const QString& _module);

private slots:
    QVariant ORMGET("Get ActionLogs information")

protected:
    QString Module;
};

} // namespace Targoman::API::ORM

#endif // TARGOMAN_API_ORM_CLSRESTAPIWITHACTIONLOGS_HPP
