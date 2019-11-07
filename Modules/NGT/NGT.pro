################################################################################
#   TargomanDBM: An extension to QSqlDatabase
#
#   Copyright 2014-2019 by Targoman Intelligent Processing <http://tip.co.ir>
#
#   TargomanDBM is free software: you can redistribute it and/or modify
#   it under the terms of the GNU Lesser General Public License as published by
#   the Free Software Foundation, either version 3 of the License, or
#   (at your option) any later version.
#
#   TargomanDBM is distributed in the hope that it will be useful,
#   but WITHOUT ANY WARRANTY; without even the implied warranty of
#   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#   GNU Lesser General Public License for more details.
#
#   You should have received a copy of the GNU Lesser General Public License
#   along with Targoman. If not, see <http://www.gnu.org/licenses/>.
################################################################################
PRJDIR = "../../"
TARGET = Module_NGT

HEADERS += \
    NGTv1.h \
    ORM/Defs.hpp

SOURCES += \
    NGTv1.cpp

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
CONFIG+= staticlib

DESTDIR =      $$BaseLibraryFolder
MOC_DIR      = $$BuildFolderPattern/$$TARGET/moc
OBJECTS_DIR  = $$BuildFolderPattern/$$TARGET/obj
RCC_DIR      = $$BuildFolderPattern/$$TARGET/rcc
UI_DIR       = $$BuildFolderPattern/$$TARGET/ui

QMAKE_CXXFLAGS_RELEASE += -fPIC
QMAKE_CXXFLAGS_DEBUG += -fPIC

DEFINES+=MODULE_$${upper($$TARGET)}_ENABLED
