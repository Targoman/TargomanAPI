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

#ifndef TARGOMAN_API_MODULES_MT_ORM_CHARTS_H
#define TARGOMAN_API_MODULES_MT_ORM_CHARTS_H

#include "ModuleHelpers/MT/Interfaces/intfMTCharts.h"
#include "../MTDefs.hpp"

using namespace Targoman::API::ModuleHelpers::MT::Interfaces;

namespace Targoman::API::MTModule {

//structures and enumes goes here

namespace ORM {

class Charts : public intfMTCharts<true>
{
    Q_OBJECT
    TARGOMAN_API_SUBMODULE_DEFINE(MT, Charts)
};

} //namespace ORM
} //namespace Targoman::API::MTModule

#endif // TARGOMAN_API_MODULES_MT_ORM_CHARTS_H
