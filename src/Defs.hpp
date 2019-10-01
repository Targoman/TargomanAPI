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
#ifndef TARGOMAN_APPS_DEFS_HPP
#define TARGOMAN_APPS_DEFS_HPP

#include "libTargomanCommon/Macros.h"

namespace Targoman{
namespace Apps {

/***************************************************************/
constexpr char RESULT_ERRNO[]= "errno";
constexpr char RESULT_MESSAGE[]= "message";
constexpr char RESULT_CLASS[]= "class";
constexpr char RESULT_CACHE[]= "byCache";

constexpr char RESULT_SIMPLE[]= "simple";
constexpr char RESULT_SERVERID[]= "serverID";

constexpr char RESULT_TRANSLATION[]= "tr";
constexpr char RESULT_TRANSLATION_PHRASES[]= "phrases";
constexpr char RESULT_TRANSLATION_ALIGNMENTS[]= "alignments";
constexpr char RESULT_TRANSLATION_BASE[]= "base";

constexpr char RESULT_TIMES[]= "times";
constexpr char RESULT_TIMES_PRE[]= "pre";
constexpr char RESULT_TIMES_TR[]= "tr";
constexpr char RESULT_TIMES_POST[]= "post";
constexpr char RESULT_TIMES_ALL[]= "all";
/***************************************************************/

}
}

#endif // TARGOMAN_APPS_DEFS_HPP
