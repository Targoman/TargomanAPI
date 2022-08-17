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
    Engines/clsEngine.h \
    MTDefs.hpp \
    Classes/intfTranslatorBridge.hpp \
    Classes/intfTranslatorEngine.hpp \
    Classes/TranslationDispatcher.h \
    Engines/intfBaseNMTGateway.h \
    Engines/gtwAmunRNN.h \
    Engines/gtwMarianTransBPE.v1.4.h \
    Engines/ActiveGateways.h

#    Engines/intfBaseNMT.h \
#    Engines/intfLuaOpenNMT.h \
#    Engines/intfMarianTransBPE.v1.10.h \
#    Engines/intfMarianTransBPE.v1.5.h \
#    Engines/intfMarianTransBPE.v1.6.h \
#    Engines/intfMarianTransBPE.v1.7.h \
#    Engines/intfMarianTransSPM.v1.10.h \
#    Engines/intfPROMT.h \
#    Engines/intfTargomanSMT.h \

# $$BASE_PROJECT_PATH/3rdParty/E4MT/src/clsFormalityChecker.cpp \
SOURCES += \
    Classes/TranslationDispatcher.cpp \
    Engines/clsEngine.cpp \
    Engines/gtwAmunRNN.cpp \
    Engines/gtwMarianTransBPE.v1.4.cpp \
    Engines/intfBaseNMTGateway.cpp

OTHER_FILES += \

INCLUDEPATH += $$BASE_PROJECT_PATH/3rdParty/E4MT/

################################################################################
include($$QBUILD_PATH/templates/libConfigs.pri)
