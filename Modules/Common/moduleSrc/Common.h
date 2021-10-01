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

#include "libTargomanCommon/Configuration/tmplConfigurable.h"
#include "Interfaces/API/intfSQLBasedModule.h"
//#include "Interfaces/AAA/AAA.hpp"
//#include "Interfaces/AAA/Accounting_Defs.hpp"
//#include "Interfaces/ORM/intfAlerts.h"
//using namespace Targoman::API::ORM;

namespace Targoman::API::CommonModule {

class Common : public intfSQLBasedModule
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID INTFPUREMODULE_IID)
    Q_INTERFACES(Targoman::API::API::intfPureModule)
    TARGOMAN_API_MODULE_DB_CONFIGS(Common);
    TARGOMAN_DEFINE_API_MODULE(Common);

protected slots:
};

} //namespace Targoman::API::CommonModule

#endif // TARGOMAN_API_MODULES_COMMON_COMMON_H
