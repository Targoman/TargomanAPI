################################################################################
#   QBuildSystem
#
#   Copyright(c) 2021 by Targoman Intelligent Processing <http://tip.co.ir>
#
#   Redistribution and use in source and binary forms are allowed under the
#   terms of BSD License 2.0.
################################################################################
PRJDIR = ".."

include($$BASE_PROJECT_PATH/Modules/Account/functionalTest/functionalTest.pri)

# +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-#
SOURCES += main.cpp \
           $$BASE_PROJECT_PATH/3rdParty/QtCurl/libsrc/QtCUrl.cpp \
           Account/test.cpp

# +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-#
LIBS += -lcurl

################################################################################
include($$QBUILD_PATH/templates/unitTestConfigs.pri)
LIBS -= -lTargomanAPI
