################################################################################
#   QBuildSystem
#
#   Copyright(c) 2021 by Targoman Intelligent Processing <http://tip.co.ir>
#
#   Redistribution and use in source and binary forms are allowed under the
#   terms of BSD License 2.0.
################################################################################
LibName=NLPHelper

DIST_HEADERS += \
    FormalityChecker.h \
    TextProcessor.hpp \

PRIVATE_HEADERS += \

SOURCES += \
    FormalityChecker.cpp \
    $$BASE_PROJECT_PATH/3rdParty/E4MT/src/clsFormalityChecker.cpp \

OTHER_FILES += \

INCLUDEPATH += $$BASE_PROJECT_PATH/3rdParty/E4MT/

################################################################################
include($$BASE_PROJECT_PATH/pch/precompiledHeader.pri)

include($$QBUILD_PATH/templates/libConfigs.pri)
