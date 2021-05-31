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
 @author S. Mehran M. Ziabary <ziabary@targoman.com>
 */

#ifndef TARGOMAN_API_MODULES_ADVERTISEMENT_ORM_DEFS_HPP
#define TARGOMAN_API_MODULES_ADVERTISEMENT_ORM_DEFS_HPP

#include "libTargomanCommon/Macros.h"
#include "Interfaces/Common/intfAPIArgManipulator.h"

namespace TAPI {

TARGOMAN_DEFINE_ENUM (enuAdvertOrder,
                      First  = '1',
                      Second = '2',
                      Third  = '3',
                      HugeRequest = 'H',
                      Normal = 'R'
                               );

TARGOMAN_DEFINE_ENUM (enuAdvertType,
                      Text,
                      Image
                      );

TARGOMAN_DEFINE_ENUM (enuBannerSizes,
                      MedimRectangle_300x250   = 'A',
                      LargeRectangle_336x280   = 'B',
                      LeaderBoard_728x90       = 'C',
                      HalfPage_300x600         = 'D',
                      WideSkyscrapper_160x600  = 'E',
                      Skyscrapper_120x600      = 'F',
                      Square_250x250           = 'G',
                      SmallSquare_200x200      = 'H',
                      MainBanner_468x60        = 'I',
                      Portrait_300x1050        = 'J',
                      Billboard_970x250        = 'K',
                      LargeLeaderBoard_970x90  = 'L',
                      HalfBanner_234x60        = 'M',
                      VerticalBanner_120x240   = 'N',
                      SmallRectangle_180x150   = 'O',

                      Mobile_320x50            = 'P',
                      LargeMobile_320x100      = 'Q',
                      );

TARGOMAN_DEFINE_ENUM (enuAccountOrdersStatus,
                      New       = 'N',
                      Pending   = 'P',
                      Approved  = 'A',
                      Removed   = 'R',
                      );


}

constexpr char AdvertSchema[] = "Advert";
constexpr char AdvertDomain[] = "Advert";

TAPI_DECLARE_METATYPE_ENUM(TAPI::enuAdvertOrder);
TAPI_DECLARE_METATYPE_ENUM(TAPI::enuAdvertType);
TAPI_DECLARE_METATYPE_ENUM(TAPI::enuBannerSizes);
TAPI_DECLARE_METATYPE_ENUM(TAPI::enuAccountOrdersStatus);

#endif // TARGOMAN_API_MODULES_ADVERTISEMENT_ORM_DEFS_HPP

