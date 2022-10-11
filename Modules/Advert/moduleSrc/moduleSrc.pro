################################################################################
#   QBuildSystem
#
#   Copyright(c) 2021 by Targoman Intelligent Processing <http://tip.co.ir>
#
#   Redistribution and use in source and binary forms are allowed under the
#   terms of BSD License 2.0.
################################################################################
ModuleName=Advert

HEADERS += \
    Advert.h \
    ORM/ActiveAds.h \
    ORM/Defs.hpp \
    ORM/Bin.h \
    ORM/Clicks.h \
    ORM/Props.h \
    ORM/Accounting.h \
    ORM/Locations.h \

SOURCES += \
    Advert.cpp \
    ORM/ActiveAds.cpp \
    ORM/Bin.cpp \
    ORM/Clicks.cpp \
    ORM/Props.cpp \
    ORM/Accounting.cpp \
    ORM/Locations.cpp \

OTHER_FILES +=

################################################################################
include($$QBUILD_PATH/templates/moduleConfigs.pri)

include($$BASE_PROJECT_PATH/pch/precompiledHeader.pri)
