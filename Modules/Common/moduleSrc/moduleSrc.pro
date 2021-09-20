################################################################################
#   QBuildSystem
#
#   Copyright(c) 2021 by Targoman Intelligent Processing <http://tip.co.ir>
#
#   Redistribution and use in source and binary forms are allowed under the
#   terms of BSD License 2.0.
################################################################################
ModuleName=Common

HEADERS += \
    Common.h \
    ORM/Defs.hpp \
    ORM/Alerts.h \

SOURCES += \
    Common.cpp \
    ORM/Alerts.cpp \

OTHER_FILES += \
    dumpDBSchema.sh \
    ORM/Schema.my.sql \

################################################################################
include($$QBUILD_PATH/templates/moduleConfigs.pri)
