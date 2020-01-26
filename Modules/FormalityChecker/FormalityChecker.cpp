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

#include "FormalityChecker.h"
#include "3rdParty/E4MT/src/clsFormalityChecker.h"
#include "libTargomanTextProcessor/TextProcessor.h"
#include "libTargomanCommon/Configuration/ConfigManager.h"
#include "Configs.h"

using namespace Targoman::Apps;
using namespace Targoman::NLPLibs;
using namespace Targoman::Common;
using namespace Targoman::Common::Configuration;
using namespace QHttp;

static clsFormalityChecker* Checker = nullptr;

void FormalityChecker::init()
{
    Checker = new clsFormalityChecker;
}

QString FormalityChecker::apiCheck(const QString _text, const QHttp::ISO639_2_t& _lang)
{
    Q_ASSERT(Checker);
    return Checker->check(_lang, _text);
}

FormalityChecker::FormalityChecker()
{
    this->registerMyRESTAPIs();
}



