################################################################################
#   TargomanAPI: REST API for Targoman
#
#   Copyright 2014-2020 by Targoman Intelligent Processing <http://tip.co.ir>
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
PRJDIR = "../"
LibName=TargomanAPIInterface

HEADERS += \
    Common/GenericTypes.h \
    Common/tmplAPIArg.h \
    Common/intfAPIArgManipulator.h \
    AAA/AAA.hpp \
    AAA/AAADefs.hpp \
    AAA/Accounting.h \
    AAA/Authentication.h \
    AAA/Authorization.h \
    AAA/clsJWT.hpp \
    AAA/PrivHelpers.h \
    ORM/clsORMField.h \
    ORM/clsRESTAPIWithActionLogs.h \
    ORM/clsTable.h \
    NLP/FormalityChecker.h \
    NLP/TextProcessor.hpp \
    Common/HTTPExceptions.hpp \
    Common/intfAPIModule.h \
    Common/GenericEnums.hpp

SOURCES += \
    AAA/Accounting.cpp \
    AAA/Authentication.cpp \
    AAA/Authorization.cpp \
    AAA/PrivHelpers.cpp \
    Common/GenericTypes.cpp \
    ORM/clsORMField.cpp \
    ORM/clsRESTAPIWithActionLogs.cpp \
    ORM/clsTable.cpp \
    NLP/FormalityChecker.cpp \
    $$PRJDIR/3rdParty/E4MT/src/clsFormalityChecker.cpp \
    Common/intf_impl.cpp

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
TARGET = $$LibName
!CONFIG(debug): CONFIG+= staticlib

DESTDIR =      $$BaseLibraryFolder/
MOC_DIR      = $$BaseBuildFolder/$$LibName/moc
OBJECTS_DIR  = $$BaseBuildFolder/$$LibName/obj
RCC_DIR      = $$BaseBuildFolder/$$LibName/rcc
UI_DIR       = $$BaseBuildFolder/$$LibName/ui

QMAKE_CXXFLAGS_RELEASE += -fPIC
QMAKE_CXXFLAGS_DEBUG += -fPIC

