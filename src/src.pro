
################################################################################
#   TargomanAPI: REST API for Targoman
#
#   Copyright 2014-2019 by Targoman Intelligent Processing <http://tip.co.ir>
#
#   TargomanAPI is free software: you can redistribute it and/or modify
#   it under the terms of the GNU AFFERO GENERAL PUBLIC LICENSE as published by
#   the Free Software Foundation, either version 3 of the License, or
#   (at your option) any later version.
#
#   TargomanAPI is distributed in the hope that it will be useful,
#   but WITHOUT ANY WARRANTY; without even the implied warranty of
#   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#   GNU AFFERO GENERAL PUBLIC LICENSE for more details.
#
#   You should have received a copy of the GNU AFFERO GENERAL PUBLIC LICENSE
#   along with Targoman. If not, see <http://www.gnu.org/licenses/>.
################################################################################
PRJDIR = ".."
# +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-#
HEADERS += \
    appTargomanAPI.h \
    Configs.h \
    ../3rdParty/E4MT/src/clsFormalityChecker.h \
    ../3rdParty/QtCUrl/src/QtCUrl.h \
    Helpers/AAA/AAADefs.hpp \
    Helpers/AAA/Accounting.h \
    Helpers/AAA/Authentication.h \
    Helpers/AAA/Authorization.h \
    Helpers/AAA/clsJWT.hpp \
    Helpers/AAA/PrivHelpers.h \
    Helpers/AAA/AAA.hpp \
    Helpers/AAA/GenericEnums.hpp \
    Helpers/ORM/clsActionLogs.h \
    Helpers/ORM/clsTable.h \

# +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-#
SOURCES += \
    main.cpp \
    appTargomanAPI.cpp \
    Configs.cpp \
    ../3rdParty/E4MT/src/clsFormalityChecker.cpp \
    ../3rdParty/QtCUrl/src/QtCUrl.cpp \
    Helpers/AAA/Accounting.cpp \
    Helpers/AAA/Authentication.cpp \
    Helpers/AAA/Authorization.cpp \
    Helpers/AAA/PrivHelpers.cpp \
    Helpers/ORM/clsActionLogs.cpp \
    Helpers/ORM/clsTable.cpp \

################################################################################
#                       DO NOT CHANGE ANYTHING BELOW                           #
################################################################################
ConfigFile = ../qmake/configs.pri
!exists($$ConfigFile){
error("**** $$ProjectName: Unable to find Configuration file $$ConfigFile ****")
}
include ($$ConfigFile)

TEMPLATE = app
TARGET = $$ProjectName

TARGET       = $$ProjectName
DESTDIR      = $$BaseBinFolder
MOC_DIR      = $$BaseBuildFolder/moc
OBJECTS_DIR  = $$BaseBuildFolder/obj
RCC_DIR      = $$BaseBuildFolder/rcc
UI_DIR       = $$BaseBuildFolder/ui

# +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-#
OTHER_FILES += \
    README.md \
    INSTALL \
    buildDependencies.sh

include($$PRJDIR/qmake/install.pri)

DISTFILES += \
    conf/test.conf \
    Dockerfile \
    conf/api.server.conf \
    build.sh

LIBS += -lTargomanCommon \
        -lTargomanDBM \
        -lQFieldValidator \
        -lQRESTServer \
        -lqhttp \
        -lfasttext \
        -lTargomanTextProcessor \
        -lcurl


ACTIVE_MODULES= \
  Account \
  Advert \
  NGT \
  Translation \

contains(ACTIVE_MODULES, Account) {
  DEFINES+=ENABLE_MODULE_ACCOUNT
  LIBS+=-lModule_Account
}

contains(ACTIVE_MODULES, Translation) {
  DEFINES+=ENABLE_MODULE_TRANSLATION
  LIBS+=-lModule_Translation
}

contains(ACTIVE_MODULES, Advert) {
  DEFINES+=ENABLE_MODULE_ADVERT
  LIBS+=-lModule_Advert \
}

contains(ACTIVE_MODULES, NGT) {
  DEFINES+=ENABLE_MODULE_NGT
  LIBS+=-lModule_NGT
}

