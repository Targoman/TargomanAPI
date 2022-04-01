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
    ORM/Departments.h \
    ORM/Units.h \
    Ticketing.h \
    ORM/TicketAttachments.h \
    ORM/Tickets.h \
    ORM/Defs.hpp \

SOURCES += \
    ORM/Departments.cpp \
    ORM/Units.cpp \
    Ticketing.cpp \
    ORM/TicketAttachments.cpp \
    ORM/Tickets.cpp \

OTHER_FILES +=

################################################################################
include($$QBUILD_PATH/templates/moduleConfigs.pri)
