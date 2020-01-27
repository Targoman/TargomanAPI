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

#include "URLProcessor.h"
#include "3rdParty/URLCanonicalizer/libsrc/libURLCanonicalizer/URLCanonicalizer.h"

using namespace Targoman::Common;
using namespace QHttp;

void URLProcessor::init()
{
}

QString URLProcessor::apiGETcanonicalize(const QString _url, bool _removeWWW)
{
    char Normalized[MAX_URL_SIZE+1];
    URLCanonicalizer::doJob (_url.toLatin1().constData(), Normalized, _removeWWW);
    return Normalized;
}

QString URLProcessor::apiGETconvertHexCodes (const QString _url, bool _convertAll)
{
    char Normalized[MAX_URL_SIZE+1];
    strncpy(Normalized, _url.toLatin1().constData(), MAX_URL_SIZE);
    return URLCanonicalizer::convertHexCodes(Normalized, _convertAll);
}

URLProcessor::URLProcessor()
{
    this->registerMyRESTAPIs();
}



