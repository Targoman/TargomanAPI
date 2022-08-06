################################################################################
#   QBuildSystem
#
#   Copyright(c) 2021 by Targoman Intelligent Processing <http://tip.co.ir>
#
#   Redistribution and use in source and binary forms are allowed under the
#   terms of BSD License 2.0.
################################################################################
ModuleName=MT

HEADERS += \
    Classes/intfTranslatorBridge.hpp \
    Classes/intfTranslatorEngine.hpp \
    Classes/TranslationDispatcher.h \
    Engines/clsNMT.h \
    MT.h \
    MTDefs.hpp

SOURCES += \
    Classes/TranslationDispatcher.cpp \
    Engines/clsNMT.cpp \
    MT.cpp

OTHER_FILES += \

################################################################################
include($$QBUILD_PATH/templates/moduleConfigs.pri)

