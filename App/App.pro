
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
PRJDIR = ".."
# +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-#
HEADERS += \
    Server/RESTServer.h \
    Server/APICache.hpp \
    Server/appTargomanAPI.h \
    Server/clsRedisConnector.h \
    Server/clsRequestHandler.h \
    Server/clsSimpleCrypt.h \
    Server/intfCacheConnector.hpp \
    Server/QJWT.h \
    Server/RESTAPIRegistry.h \
    Server/RESTServer.h \
    Server/WebSocketServer.h \
    Server/ServerConfigs.h \
    Server/clsAPIObject.h \
    Server/QtTypes.hpp

# +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-#
SOURCES += \
    Server/ServerConfigs.cpp \
    Server/clsAPIObject.cpp \
    Server/clsRedisConnector.cpp \
    Server/clsRequestHandler.cpp \
    Server/clsSimpleCrypt.cpp \
    Server/QJWT.cpp \
    Server/WebSocketServer.cpp \
    Server/RESTAPIRegistry.cpp \
    Server/RESTServer.cpp \
    Server/appTargomanAPI.cpp \
    Server/main.cpp \
    $$PRJDIR/3rdParty/QtCUrl/src/QtCUrl.cpp \

################################################################################
#                       DO NOT CHANGE ANYTHING BELOW                           #
################################################################################
ConfigFile = ../qmake/configs.pri
!exists($$ConfigFile){
error("**** $$ProjectName: Unable to find Configuration file $$ConfigFile ****")
}
include ($$ConfigFile)

TEMPLATE = app
TARGET = $$ProjectName

TARGET       = $$ProjectName
DESTDIR      = $$BaseBinFolder
MOC_DIR      = $$BaseBuildFolder/moc
OBJECTS_DIR  = $$BaseBuildFolder/obj
RCC_DIR      = $$BaseBuildFolder/rcc
UI_DIR       = $$BaseBuildFolder/ui

# +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-#
OTHER_FILES += \
    README.md \
    INSTALL \
    buildDependencies.sh

include($$PRJDIR/qmake/install.pri)

DISTFILES += \
    conf/test.conf \
    Dockerfile \
    conf/api.server.conf \
    build.sh

LIBS += -lTargomanAPIInterface \
        -lTargomanCommon \
        -lTargomanDBM \
        -lQFieldValidator \
        -lqhttp \
        -lcurl \
        -lTargomanTextProcessor \
        -lfasttext

#defined (TARGOMAN_API_REDIS_PROTOCOL) {
  LIBS+= -lhiredis
#}

#defined (TARGOMAN_API_REDIS_PROTOCOL) {
  QT+= websockets
#}
