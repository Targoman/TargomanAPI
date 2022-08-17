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
    MTDefs.hpp \
    Classes/clsEngine.h \
    Classes/intfTranslatorBridge.hpp \
    Classes/TranslationDispatcher.h \
    Gateways/intfTranslatorGateway.hpp \
    Gateways/intfBaseNMTGateway.h \
    Gateways/gtwAmunRNN.h \
    Gateways/gtwMarianTransBPE.v1.4.h \
    Gateways/ActiveGateways.h

#    Gateways/intfBaseNMT.h \
#    Gateways/intfLuaOpenNMT.h \
#    Gateways/intfMarianTransBPE.v1.10.h \
#    Gateways/intfMarianTransBPE.v1.5.h \
#    Gateways/intfMarianTransBPE.v1.6.h \
#    Gateways/intfMarianTransBPE.v1.7.h \
#    Gateways/intfMarianTransSPM.v1.10.h \
#    Gateways/intfPROMT.h \
#    Gateways/intfTargomanSMT.h \

# $$BASE_PROJECT_PATH/3rdParty/E4MT/src/clsFormalityChecker.cpp \
SOURCES += \
    Classes/TranslationDispatcher.cpp \
    Classes/clsEngine.cpp \
    Gateways/gtwAmunRNN.cpp \
    Gateways/gtwMarianTransBPE.v1.4.cpp \
    Gateways/intfBaseNMTGateway.cpp

OTHER_FILES += \

INCLUDEPATH += $$BASE_PROJECT_PATH/3rdParty/E4MT/

################################################################################
include($$QBUILD_PATH/templates/libConfigs.pri)
