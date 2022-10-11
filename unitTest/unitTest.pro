################################################################################
#   QBuildSystem
#
#   Copyright(c) 2021 by Targoman Intelligent Processing <http://tip.co.ir>
#
#   Redistribution and use in source and binary forms are allowed under the
#   terms of BSD License 2.0.
################################################################################
PRJDIR = ".."

HEADERS +=  \
    testMacros.hpp \
    testQueryBuilders.hpp \
    $$BASE_PROJECT_PATH/App/Server/RESTAPIRegistry.h \
    $$BASE_PROJECT_PATH/App/Server/ServerConfigs.h \
    $$BASE_PROJECT_PATH/App/Server/clsAPIObject.h

SOURCES += \
    main.cpp \
    $$BASE_PROJECT_PATH/App/Server/RESTAPIRegistry.cpp \
    $$BASE_PROJECT_PATH/App/Server/ServerConfigs.cpp \
    $$BASE_PROJECT_PATH/App/Server/clsAPIObject.cpp

BASE_TEST_PATH = $$BASE_PROJECT_PATH/Interfaces/Test
INCLUDEPATH += $$BASE_TEST_PATH
include($$BASE_TEST_PATH/Test.pri)

################################################################################
include($$QBUILD_PATH/templates/unitTestConfigs.pri)
LIBS -= -lTargomanAPI

include($$BASE_PROJECT_PATH/pch/precompiledHeader.pri)
