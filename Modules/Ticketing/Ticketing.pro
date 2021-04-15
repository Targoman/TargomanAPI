################################################################################
#   QBuildSystem
#
#   Copyright(c) 2021 by Targoman Intelligent Processing <http://tip.co.ir>
#
#   Redistribution and use in source and binary forms are allowed under the
#   terms of BSD License 2.0.
################################################################################
ModuleName=Ticketing

HEADERS += \
    Ticketing.h \
    ORM/Tickets.h \
    ORM/Defs.hpp \

SOURCES += \
    Ticketing.cpp \
    ORM/Tickets.cpp \

OTHER_FILES += \

################################################################################
include($$QBUILD_PATH/templates/moduleConfigs.pri)
