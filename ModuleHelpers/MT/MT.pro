################################################################################
#   QBuildSystem
#
#   Copyright(c) 2021 by Targoman Intelligent Processing <http://tip.co.ir>
#
#   Redistribution and use in source and binary forms are allowed under the
#   terms of BSD License 2.0.
################################################################################
LibName=MTHelper

DIST_HEADERS += \

PRIVATE_HEADERS += \

HEADERS += \
    Classes/MTHelper.h \
    MTDefs.hpp \
    Classes/clsEngine.h \
    Gateways/intfTranslatorGateway.hpp \
    Gateways/intfBaseNMTGateway.h \
    Gateways/ActiveGateways.h \
    Gateways/gtwAmunRNN.h \
    Gateways/gtwMarianTransBPE.v1.4.h \
    Gateways/gtwMarianTransBPE.v1.6.h \
    Gateways/gtwMarianTransBPE.v1.7.h \
    Gateways/gtwMarianTransBPE.v1.10.h \
    Gateways/gtwMarianTransSPM.v1.10.h \
    Gateways/gtwLuaOpenNMT.h \
    Gateways/gtwTargomanSMT.h

#Gateways/gtwMarianTransBPE.v1.5.h \
#Gateways/gtwPROMT.h \

# $$BASE_PROJECT_PATH/3rdParty/E4MT/src/clsFormalityChecker.cpp \
SOURCES += \
    Classes/MTHelper.cpp \
    Classes/clsEngine.cpp \
    Gateways/intfBaseNMTGateway.cpp \
    Gateways/gtwAmunRNN.cpp \
    Gateways/gtwMarianTransBPE.v1.4.cpp \
    Gateways/gtwMarianTransBPE.v1.6.cpp \
    Gateways/gtwMarianTransBPE.v1.7.cpp \
    Gateways/gtwMarianTransBPE.v1.10.cpp \
    Gateways/gtwMarianTransSPM.v1.10.cpp \
    Gateways/gtwLuaOpenNMT.cpp \
    Gateways/gtwTargomanSMT.cpp

#Gateways/gtwMarianTransBPE.v1.5.cpp \
#Gateways/gtwPROMT.cpp \

OTHER_FILES += \
    migrations/db/* \
    migrations/db/.dbdiff.dev_.cfg \
    migrations/local/* \
    migrations/local/*.sh \
    migrations/local/.migrations

INCLUDEPATH += $$BASE_PROJECT_PATH/3rdParty/E4MT/

################################################################################
include($$QBUILD_PATH/templates/libConfigs.pri)
