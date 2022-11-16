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

#include "Common.h"
#include "ORM/Alerts.h"
#include "ORM/AlertTemplates.h"

namespace Targoman::API::CommonModule {

using namespace ORM;

TARGOMAN_API_MODULE_IMPLEMENT(Common)
//---------------------------------------------------------
TARGOMAN_API_MODULE_IMPLEMENT_DB_CONFIG(Common, CommonSchema);
//---------------------------------------------------------
TARGOMAN_API_MODULE_IMPLEMENT_MIGRATIONS(Common, CommonSchema)
TARGOMAN_API_MODULE_IMPLEMENT_CONFIGURATIONS(Common, CommonSchema)
TARGOMAN_API_MODULE_IMPLEMENT_I18N(Common, CommonSchema)

Common::Common() :
    intfSQLBasedModule(
        CommonDomain,
        CommonSchema,
        ""
) {
    TARGOMAN_API_MODULE_IMPLEMENT_CTOR_MIGRATIONS(Common, CommonSchema)
    TARGOMAN_API_MODULE_IMPLEMENT_CTOR_CONFIGURATIONS(Common, CommonSchema)
    TARGOMAN_API_MODULE_IMPLEMENT_CTOR_I18N(Common, CommonSchema)

    this->addSubModule(&Alerts::instance());
    this->addSubModule(&AlertTemplates::instance());
}

} //namespace Targoman::API::CommonModule
