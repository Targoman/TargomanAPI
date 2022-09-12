################################################################################
#   QBuildSystem
#
#   Copyright(c) 2021 by Targoman Intelligent Processing <http://tip.co.ir>
#
#   Redistribution and use in source and binary forms are allowed under the
#   terms of BSD License 2.0.
################################################################################
TEST_NAME = targomanmtFunctionalTest

HEADERS += \
    testTargomanMT.hpp

SOURCES += \
    $$BASE_PROJECT_PATH/3rdParty/QtCurl/libsrc/QtCUrl.cpp \
    main.cpp \

LIBS += -lcurl

BASE_TEST_PATH = $$BASE_PROJECT_PATH/Interfaces/Test
INCLUDEPATH += $$BASE_TEST_PATH
include($$BASE_TEST_PATH/Test.pri)

################################################################################
include($$QBUILD_PATH/templates/unitTestConfigs.pri)
LIBS -= -lTargomanAPI
