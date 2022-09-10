################################################################################
#   QBuildSystem
#
#   Copyright(c) 2021 by Targoman Intelligent Processing <http://tip.co.ir>
#
#   Redistribution and use in source and binary forms are allowed under the
#   terms of BSD License 2.0.
################################################################################
ModuleName=TargomanMT

HEADERS += \
    TargomanMTDefs.hpp \
    ORM/MTHelpers.h \
    ORM/Accounting.h \
    TargomanMT.h

SOURCES += \
    ORM/MTHelpers.cpp \
    ORM/Accounting.cpp \
    TargomanMT.cpp

OTHER_FILES += \

LIBS += -lNLPHelper \
    -lMTHelper \

################################################################################
include($$QBUILD_PATH/templates/moduleConfigs.pri)
