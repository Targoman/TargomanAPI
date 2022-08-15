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
    Defs.hpp \
    MT.h

SOURCES += \
    MT.cpp

OTHER_FILES += \

LIBS += -lNLPHelper \
    -lMTInterface \

################################################################################
include($$QBUILD_PATH/templates/moduleConfigs.pri)

