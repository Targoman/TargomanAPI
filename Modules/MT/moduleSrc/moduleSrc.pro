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
    MT.h \
    MTDefs.hpp \
    ORM/MTHelpers.h \
    ORM/Accounting.h

SOURCES += \
    MT.cpp \
    ORM/MTHelpers.cpp \
    ORM/Accounting.cpp

OTHER_FILES += \

LIBS += -lNLPHelper \
    -lMTHelper \

################################################################################
include($$QBUILD_PATH/templates/moduleConfigs.pri)

include($$BASE_PROJECT_PATH/pch/precompiledHeader.pri)
