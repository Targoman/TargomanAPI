################################################################################
#   QBuildSystem
#
#   Copyright(c) 2021 by Targoman Intelligent Processing <http://tip.co.ir>
#
#   Redistribution and use in source and binary forms are allowed under the
#   terms of BSD License 2.0.
################################################################################
PRJDIR = ".."

#DEFINES += TARGOMAN_TEST_MODE

# +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-#
HEADERS +=  \
    $$BASE_PROJECT_PATH/App/Server/RESTServer.h \
    $$BASE_PROJECT_PATH/App/Server/APICache.hpp \
    $$BASE_PROJECT_PATH/App/Server/clsRedisConnector.h \
    $$BASE_PROJECT_PATH/App/Server/clsRequestHandler.h \
    $$BASE_PROJECT_PATH/App/Server/clsSimpleCrypt.h \
    $$BASE_PROJECT_PATH/App/Server/intfCacheConnector.hpp \
    $$BASE_PROJECT_PATH/App/Server/QJWT.h \
    $$BASE_PROJECT_PATH/App/Server/RESTAPIRegistry.h \
    $$BASE_PROJECT_PATH/App/Server/RESTServer.h \
    $$BASE_PROJECT_PATH/App/Server/ServerConfigs.h \
    $$BASE_PROJECT_PATH/App/Server/clsAPIObject.h \
    $$BASE_PROJECT_PATH/App/Server/OpenAPIGenerator.h \
 \    #$$BASE_PROJECT_PATH/App/Server/WebSocketServer.h \
    testMacros.hpp \
    testQueryBuilders.hpp

SOURCES += main.cpp \
    $$BASE_PROJECT_PATH/3rdParty/QtCurl/libsrc/QtCUrl.cpp \
    $$BASE_PROJECT_PATH/App/Server/ServerConfigs.cpp \
    $$BASE_PROJECT_PATH/App/Server/clsAPIObject.cpp \
    $$BASE_PROJECT_PATH/App/Server/clsRedisConnector.cpp \
    $$BASE_PROJECT_PATH/App/Server/clsRequestHandler.cpp \
    $$BASE_PROJECT_PATH/App/Server/clsSimpleCrypt.cpp \
    $$BASE_PROJECT_PATH/App/Server/QJWT.cpp \
    $$BASE_PROJECT_PATH/App/Server/RESTAPIRegistry.cpp \
    $$BASE_PROJECT_PATH/App/Server/RESTServer.cpp \
    $$BASE_PROJECT_PATH/App/Server/OpenAPIGenerator.cpp \
#    QueryBuilders.cpp \
     #$$BASE_PROJECT_PATH/App/Server/WebSocketServer.cpp \

# +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-#
LIBS += -lcurl

################################################################################
include($$QBUILD_PATH/templates/unitTestConfigs.pri)
LIBS -= -lTargomanAPI
LIBS += -lTargomanAPIInterface

