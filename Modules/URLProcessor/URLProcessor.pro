################################################################################
#   QBuildSystem
#
#   Copyright(c) 2021 by Targoman Intelligent Processing <http://tip.co.ir>
#
#   Redistribution and use in source and binary forms are allowed under the
#   terms of BSD License 2.0.
################################################################################
ModuleName=URLProcessor

HEADERS += \
    URLProcessor.h \

SOURCES += \
    ../../3rdParty/URLCanonicalizer/libsrc/libURLCanonicalizer/URLCanonicalizer.cpp \
    URLProcessor.cpp \

OTHER_FILES += \

################################################################################
include($$QBUILD_PATH/templates/moduleConfigs.pri)

include($$BASE_PROJECT_PATH/pch/precompiledHeader.pri)
