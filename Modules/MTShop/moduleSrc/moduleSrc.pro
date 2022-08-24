################################################################################
#   QBuildSystem
#
#   Copyright(c) 2021 by Targoman Intelligent Processing <http://tip.co.ir>
#
#   Redistribution and use in source and binary forms are allowed under the
#   terms of BSD License 2.0.
################################################################################
ModuleName=MTShop

HEADERS += \
    MTShop.h \
    MTShopDefs.hpp \
    ORM/Accounting.h

SOURCES += \
    MTShop.cpp \
    ORM/Accounting.cpp

OTHER_FILES +=

################################################################################
include($$QBUILD_PATH/templates/moduleConfigs.pri)
