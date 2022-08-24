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

#include "gtwAmunRNN.h"

namespace Targoman::API::ModuleHelpers::MT::Gateways {

TARGOMAN_API_MT_GATEWAY_IMPL(gtwAmunRNN)

//moved to MTHelper::preprocessText
//QString gtwAmunRNN::preprocessText(QString _sourceText) {
////        return preg_replace("/(\S)('(?:s|ll|d|t))/", "$1 $2", _sourceText);
//    return _sourceText.replace(QRegularExpression("/(\\S)('(?:s|ll|d|t))/"), "$1 $2");
//}

} // namespace Targoman::API::ModuleHelpers::MT::Gateways
