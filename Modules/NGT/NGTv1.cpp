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

#include "NGTv1.h"
#include "ORM/Defs.hpp"

using namespace QHttp;
using namespace Targoman::DBManager;
using namespace Targoman::API;

void Ngtv1::init()
{
}

Ngtv1::Ngtv1() :
    Helpers::ORM::clsRESTAPIWithActionLogs (NGTDACInstance(), "NGT", "Targoman"){

    /*
    QHTTP_REGISTER_TARGOMAN_ENUM(Targoman::API::enuAdvertType);
    QHTTP_REGISTER_TARGOMAN_ENUM(Targoman::API::enuAdvertOrder);
    QHTTP_REGISTER_TARGOMAN_ENUM(Targoman::API::enuBannerSizes);


//    Advertisement::ActionLogs::instance().init();
    Advertisement::ActiveAds::instance().init();
    Advertisement::Bin::instance().init();
    Advertisement::Clicks::instance().init();
    Advertisement::Props::instance().init();

    this->registerMyRESTAPIs();*/
}


