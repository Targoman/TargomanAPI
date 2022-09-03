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

#ifndef TARGOMAN_API_MODULES_COMMON_COMMON_H
#define TARGOMAN_API_MODULES_COMMON_COMMON_H

#include "Interfaces/ORM/intfMigrations.h"
#include "libTargomanCommon/Configuration/tmplConfigurable.h"
#include "Interfaces/API/intfSQLBasedModule.h"
#include "ORM/Defs.hpp"

namespace Targoman::API::CommonModule {

TARGOMAN_MIGRATIONS_PREPARENT;

class Common : public intfSQLBasedModule
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID INTFPUREMODULE_IID)
    Q_INTERFACES(Targoman::API::API::intfPureModule)
    //---------------------------------------------------------
    TARGOMAN_API_MODULE_DEFINE(Common); //, enuTokenActorType::USER);
    //---------------------------------------------------------
    TARGOMAN_API_MODULE_DEFINE_DB_CONFIGS(Common);
    //---------------------------------------------------------
    TARGOMAN_API_MODULE_DEFINE_MIGRATIONS(Common, CommonSchema);

protected slots:
};

TARGOMAN_MIGRATIONS_POSTPARENT(Common, CommonSchema);

} //namespace Targoman::API::CommonModule

#endif // TARGOMAN_API_MODULES_COMMON_COMMON_H
