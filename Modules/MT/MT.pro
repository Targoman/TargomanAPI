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
PRJDIR = "../../"
TARGET = Module_MT

HEADERS += \
    Classes/intfTranslatorBridge.hpp \
    Classes/intfTranslatorEngine.hpp \
    Classes/TranslationDispatcher.h \
    Engines/clsNMT.h \
    TranslationDefs.hpp \
    ORM/ActionLogs.h \
    MT.h

SOURCES += \
    Classes/TranslationDispatcher.cpp \
    Engines/clsNMT.cpp \
    ORM/ActionLogs.cpp \
    MT.cpp

OTHER_FILES += \

################################################################################
#                       DO NOT CHANGE ANYTHING BELOW                           #
################################################################################
ConfigFile = $$PRJDIR/qmake/configs.pri
!exists($$ConfigFile){
error("**** libsrc: Unable to find Configuration file $$ConfigFile ****")
}
include ($$ConfigFile)

TEMPLATE = lib
#CONFIG(release): CONFIG+= staticlib

DESTDIR =      $$BaseLibraryFolder
MOC_DIR      = $$BuildFolderPattern/$$TARGET/moc
OBJECTS_DIR  = $$BuildFolderPattern/$$TARGET/obj
RCC_DIR      = $$BuildFolderPattern/$$TARGET/rcc
UI_DIR       = $$BuildFolderPattern/$$TARGET/ui

QMAKE_CXXFLAGS_RELEASE += -fPIC
QMAKE_CXXFLAGS_DEBUG += -fPIC

DEFINES+=MODULE_$${upper($$TARGET)}_ENABLED
