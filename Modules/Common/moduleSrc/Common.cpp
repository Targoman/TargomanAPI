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
//#include "libQFieldValidator/QFieldValidator.h"
//#include "Interfaces/AAA/PrivHelpers.h"
//#include "Interfaces/Common/GenericEnums.hpp"
//#include "Interfaces/Common/HTTPExceptions.hpp"
//#include "Interfaces/Helpers/SecurityHelper.h"
//using namespace Targoman::API::Helpers;

//#include "Interfaces/Helpers/RESTClientHelper.h"
//using namespace Targoman::API::Helpers;

#include "ORM/Defs.hpp"
#include "ORM/Alerts.h"

namespace Targoman::API::CommonModule {

using namespace ORM;

TARGOMAN_IMPL_API_MODULE(Common)
TARGOMAN_API_MODULE_DB_CONFIG_IMPL(Common, CommonSchema);

Common::Common() :
    intfSQLBasedModule(
        CommonDomain,
        CommonSchema,
        ""
    )
{
    this->addSubModule(&Alerts::instance());
}

} //namespace Targoman::API::CommonModule
