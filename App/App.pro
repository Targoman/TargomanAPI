
################################################################################
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
################################################################################
PRJDIR = ".."
# +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-#
HEADERS += \
    Interfaces/AAA/AAA.hpp \
    Interfaces/AAA/AAADefs.hpp \
    Interfaces/AAA/Accounting.h \
    Interfaces/AAA/Authentication.h \
    Interfaces/AAA/Authorization.h \
    Interfaces/AAA/clsJWT.hpp \
    Interfaces/AAA/GenericEnums.hpp \
    Interfaces/AAA/PrivHelpers.h \
    Interfaces/Common/GenericTypes.h \
    Interfaces/Common/HTTPExceptions.h \
    Interfaces/Common/tmplAPIArg.h \
    Interfaces/ORM/clsORMField.h \
    Interfaces/ORM/clsRESTAPIWithActionLogs.h \
    Interfaces/ORM/clsTable.h \
    Interfaces/Common/intfAPIArgManipulator.h \
    Interfaces/NLP/FormalityChecker.h \
    Server/RESTServer.h \
    Server/APICache.hpp \
    Server/appTargomanAPI.h \
    Server/clsRedisConnector.h \
    Server/clsRequestHandler.h \
    Server/clsSimpleCrypt.h \
    Server/intfCacheConnector.hpp \
    Server/NumericTypes.hpp \
    Server/QJWT.h \
    Server/RESTAPIRegistry.h \
    Server/RESTServer.h \
    Server/WebSocketServer.h \
    Server/ServerConfigs.h \
    Server/clsAPIObject.h \
    Interfaces/Common/intfAPIModule.hpp \
    Interfaces/NLP/TextProcessor.hpp

# +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-#
SOURCES += \
    Interfaces/AAA/Accounting.cpp \
    Interfaces/AAA/Authentication.cpp \
    Interfaces/AAA/Authorization.cpp \
    Interfaces/AAA/PrivHelpers.cpp \
    Interfaces/Common/GenericTypes.cpp \
    Interfaces/ORM/clsORMField.cpp \
    Interfaces/ORM/clsRESTAPIWithActionLogs.cpp \
    Interfaces/ORM/clsTable.cpp \
    Interfaces/NLP/FormalityChecker.cpp \
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
    $$PRJDIR/3rdParty/E4MT/src/clsFormalityChecker.cpp \

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

LIBS += -lTargomanCommon \
        -lTargomanDBM \
        -lQFieldValidator \
        -lQRESTServer \
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
