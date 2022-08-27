################################################################################
#   QBuildSystem
#
#   Copyright(c) 2021 by Targoman Intelligent Processing <http://tip.co.ir>
#
#   Redistribution and use in source and binary forms are allowed under the
#   terms of BSD License 2.0.
################################################################################
ModuleName=MTAPI

HEADERS += \
    MTAPI.h \
    MTAPIDefs.hpp \
    ORM/MTHelpers.h

SOURCES += \
    MTAPI.cpp \
    ORM/MTHelpers.cpp

OTHER_FILES += \

LIBS += -lNLPHelper \
    -lMTHelper \

################################################################################
include($$QBUILD_PATH/templates/moduleConfigs.pri)

