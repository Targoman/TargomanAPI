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
 * @author Kambiz Zandi <kambizzandi@gmail.com>
 */

#ifndef TAPI_BASE_H
#define TAPI_BASE_H

#include <stdio.h>
#include <stdlib.h>
#include <execinfo.h>
#include <cxxabi.h>
//#include <QString>
//#include <QTextStream>
//#include "libTargomanCommon/CmdIO.h"
//#include "libTargomanCommon/TLogger.h"
//using namespace Targoman::Common;

#define TAPI_DISABLE_COPY(Class) \
    Q_DISABLE_COPY(Class)
//    Class &operator=(const Class) Q_DECL_EQ_DELETE;

void print_stacktrace(FILE *out = stderr, unsigned int max_frames = 63);

#endif // TAPI_BASE_H
