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
 * @author S. Mehran M. Ziabary <ziabary@targoman.com>
 * @author Kambiz Zandi <kambizzandi@gmail.com>
 */

#ifndef TARGOMAN_API_MODULES_MT_TRANSLATIONDEFS_HPP
#define TARGOMAN_API_MODULES_MT_TRANSLATIONDEFS_HPP

namespace Targoman::API::MTModule {

namespace RESULTItems {
    constexpr char ERRNO[]= "errno";
    constexpr char MESSAGE[]= "message";
    constexpr char CLASS[]= "class";
    constexpr char CACHE[]= "byCache";

    constexpr char SIMPLE[]= "simple";
    constexpr char SERVERID[]= "serverID";
    constexpr char TRANSLATION[]= "tr";
    constexpr char TIMES[]= "times";
    constexpr char TIME[]= "time";

    namespace TRANSLATIONItems {
        constexpr char PHRASES[]= "phrases";
        constexpr char ALIGNMENTS[]= "alignments";
        constexpr char BASE[]= "base";

    }

    namespace TIMESItems {
        constexpr char PRE[]= "pre";
        constexpr char TR[]= "tr";
        constexpr char POST[]= "post";
        constexpr char ALL[]= "all";
    }
}

} //namespace Targoman::API::MTModule

#endif // TARGOMAN_API_MODULES_MT_TRANSLATIONDEFS_HPP
