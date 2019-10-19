/******************************************************************************
#   TargomanAPI: REST API for Targoman
#
#   Copyright 2014-2019 by Targoman Intelligent Processing <http://tip.co.ir>
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
 @author S. Mehran M. Ziabary <ziabary@targoman.com>
 */

#include "Advertisement.h"
#include "QFieldValidator.h"
#include "QHttp/QRESTServer.h"
#include "QHttp/intfAPIArgManipulator.h"
#include "Helpers/AAA/AAA.hpp"
#include "Helpers/AAA/PrivHelpers.h"
#include "Helpers/AAA/GenericEnums.hpp"
#include "Configs.h"

#include "ORM/ActiveAds.h"

using namespace Targoman;
using namespace Targoman::API;
using namespace Targoman::API::Helpers::AAA;
using namespace QHttp;

void Advertisement::init()
{}

Advertisement::Advertisement(){


    ActiveAds::instance().init();

    this->registerMyRESTAPIs();
}
