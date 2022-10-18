################################################################################
#   QBuildSystem
#
#   Copyright(c) 2021 by Targoman Intelligent Processing <http://tip.co.ir>
#
#   Redistribution and use in source and binary forms are allowed under the
#   terms of BSD License 2.0.
################################################################################
ModuleName=TextProcessor

HEADERS += \
    TextProcessor.h \

SOURCES += \
    TextProcessor.cpp

OTHER_FILES += \

LIBS += -lNLPHelper \

################################################################################
include($$BASE_PROJECT_PATH/pch/precompiledHeader.pri)

include($$QBUILD_PATH/templates/moduleConfigs.pri)
