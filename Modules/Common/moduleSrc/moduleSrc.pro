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
    ORM/AlertTemplates.h \
    ORM/Defs.hpp \
    ORM/Alerts.h \

SOURCES += \
    Common.cpp \
    ORM/AlertTemplates.cpp \
    ORM/Alerts.cpp \

OTHER_FILES +=

################################################################################
include($$QBUILD_PATH/templates/moduleConfigs.pri)
