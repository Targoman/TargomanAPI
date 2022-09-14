################################################################################
#   QBuildSystem
#
#   Copyright(c) 2021 by Targoman Intelligent Processing <http://tip.co.ir>
#
#   Redistribution and use in source and binary forms are allowed under the
#   terms of BSD License 2.0.
################################################################################
APP_NAME=targomanAPI
include($$QBUILD_PATH/templates/innerAppConfigs.pri)

# +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-#
HEADERS += \
    Server/RESTServer.h \
    Server/APICache.hpp \
    Server/StaticModule.h \
    Server/appTargomanAPI.h \
    Server/clsRedisConnector.h \
    Server/clsRequestHandler.h \
    Server/intfCacheConnector.hpp \
    Server/RESTAPIRegistry.h \
    Server/RESTServer.h \
    Server/WebSocketServer.h \
    Server/ServerConfigs.h \
    Server/clsAPIObject.h \
    Server/OpenAPIGenerator.h

# +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-#
SOURCES += \
    $$BASE_PROJECT_PATH/3rdParty/QtCurl/libsrc/QtCUrl.cpp \
    Server/ServerConfigs.cpp \
    Server/StaticModule.cpp \
    Server/clsAPIObject.cpp \
    Server/clsRedisConnector.cpp \
    Server/clsRequestHandler.cpp \
    Server/WebSocketServer.cpp \
    Server/RESTAPIRegistry.cpp \
    Server/RESTServer.cpp \
    Server/appTargomanAPI.cpp \
    Server/OpenAPIGenerator.cpp \
    Server/main.cpp

OTHER_FILES += \
    ../conf/api.conf \
    migrations/db/* \
    migrations/db/.dbdiff.dev_.cfg \
    migrations/local/* \
    migrations/local/.migrations

# +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-#
LIBS += -lTargomanAPIInterface

DISTFILES += \
    conf/test.conf \
    conf/api.server.conf
