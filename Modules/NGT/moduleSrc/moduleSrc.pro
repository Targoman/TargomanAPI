################################################################################
#   QBuildSystem
#
#   Copyright(c) 2021 by Targoman Intelligent Processing <http://tip.co.ir>
#
#   Redistribution and use in source and binary forms are allowed under the
#   terms of BSD License 2.0.
################################################################################
ModuleName=NGTv1

HEADERS += \
    NGTv1.h \
    ORM/Defs.hpp

SOURCES += \
    NGTv1.cpp

################################################################################
include($$QBUILD_PATH/templates/moduleConfigs.pri)
