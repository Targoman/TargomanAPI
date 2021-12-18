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
    ORM/TicketAttachments.h \
    Ticketing.h \
    ORM/Tickets.h \
    ORM/Defs.hpp \

SOURCES += \
    ORM/TicketAttachments.cpp \
    Ticketing.cpp \
    ORM/Tickets.cpp \

OTHER_FILES += \
    dumpDBSchema.sh \
    ORM/Schema.my.sql \

LIBS += -laws-cpp-sdk-s3

################################################################################
include($$QBUILD_PATH/templates/moduleConfigs.pri)
