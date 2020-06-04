################################################################################
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
################################################################################
# files moved to this project file in order to be included in unitTests
# +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-#
HEADERS += \
    $$PRJDIR/App/Server/RESTServer.h \
    $$PRJDIR/App/Server/APICache.hpp \
    $$PRJDIR/App/Server/appTargomanAPI.h \
    $$PRJDIR/App/Server/clsRedisConnector.h \
    $$PRJDIR/App/Server/clsRequestHandler.h \
    $$PRJDIR/App/Server/clsSimpleCrypt.h \
    $$PRJDIR/App/Server/intfCacheConnector.hpp \
    $$PRJDIR/App/Server/QJWT.h \
    $$PRJDIR/App/Server/RESTAPIRegistry.h \
    $$PRJDIR/App/Server/RESTServer.h \
    $$PRJDIR/App/Server/WebSocketServer.h \
    $$PRJDIR/App/Server/ServerConfigs.h \
    $$PRJDIR/App/Server/clsAPIObject.h \
    $$PRJDIR/App/Server/QtTypes.hpp \
    $$PRJDIR/App/Server/OpenAPIGenerator.h \

# +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-#
SOURCES += \
    $$PRJDIR/App/Server/ServerConfigs.cpp \
    $$PRJDIR/App/Server/clsAPIObject.cpp \
    $$PRJDIR/App/Server/clsRedisConnector.cpp \
    $$PRJDIR/App/Server/clsRequestHandler.cpp \
    $$PRJDIR/App/Server/clsSimpleCrypt.cpp \
    $$PRJDIR/App/Server/QJWT.cpp \
    $$PRJDIR/App/Server/WebSocketServer.cpp \
    $$PRJDIR/App/Server/RESTAPIRegistry.cpp \
    $$PRJDIR/App/Server/RESTServer.cpp \
    $$PRJDIR/App/Server/appTargomanAPI.cpp \
    $$PRJDIR/App/Server/OpenAPIGenerator.cpp \
    $$PRJDIR/3rdParty/QtCUrl/src/QtCUrl.cpp \
