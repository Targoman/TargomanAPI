################################################################################
#   QBuildSystem
#
#   Copyright(c) 2021 by Targoman Intelligent Processing <http://tip.co.ir>
#
#   Redistribution and use in source and binary forms are allowed under the
#   terms of BSD License 2.0.
################################################################################
PRJDIR = ".."
# +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-#
HEADERS += \
    testCommon.hpp \
    testBase.hpp \
    Account/ORM/activeSessions.hpp \
    Account/ORM/actionLogs.hpp \
    Account/ORM/service.hpp \
    Account/ORM/roles.hpp \
    Account/ORM/tokens.hpp \
    Account/ORM/defs.hpp \
    Account/test.h

# +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-#
SOURCES += main.cpp \
           $$BASE_PROJECT_PATH/3rdParty/QtCurl/libsrc/QtCUrl.cpp \
           Account/test.cpp

# +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-#
LIBS += -lcurl

################################################################################
include($$QBUILD_PATH/templates/unitTestConfigs.pri)
