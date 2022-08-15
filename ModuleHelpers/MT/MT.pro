################################################################################
#   QBuildSystem
#
#   Copyright(c) 2021 by Targoman Intelligent Processing <http://tip.co.ir>
#
#   Redistribution and use in source and binary forms are allowed under the
#   terms of BSD License 2.0.
################################################################################
LibName=MTInterface

DIST_HEADERS += \

PRIVATE_HEADERS += \

HEADERS += \
    Classes/intfTranslatorBridge.hpp \
    Classes/intfTranslatorEngine.hpp \
    Classes/TranslationDispatcher.h \
    Engines/clsNMT.h \ \
    MTDefs.hpp

# $$BASE_PROJECT_PATH/3rdParty/E4MT/src/clsFormalityChecker.cpp \
SOURCES += \
    Classes/TranslationDispatcher.cpp \
    Engines/clsNMT.cpp \

OTHER_FILES += \

INCLUDEPATH += $$BASE_PROJECT_PATH/3rdParty/E4MT/

################################################################################
include($$QBUILD_PATH/templates/libConfigs.pri)
