################################################################################
#   QBuildSystem
#
#   Copyright(c) 2021 by Targoman Intelligent Processing <http://tip.co.ir>
#
#   Redistribution and use in source and binary forms are allowed under the
#   terms of BSD License 2.0.
################################################################################
APP_NAME=tsapasswd
include(../../qmake/appConfigs.pri)
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
    Server/QtTypes.hpp \
    Server/OpenAPIGenerator.h \

# +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-#
SOURCES += \
    $$PRJDIR/3rdParty/QtCUrl/src/QtCUrl.cpp \
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
    Server/OpenAPIGenerator.cpp \
    Server/main.cpp
# +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-#
LIBS += -lTargomanAPIInterface \

DISTFILES += \
    conf/test.conf \
    conf/api.server.conf \



