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
PRJDIR = ".."
# +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-#
HEADERS += \
    testCommon.hpp \
    testBase.hpp \
    Account/test.hpp \
    Account/ORM/activeSessions.hpp \
    Account/ORM/actionLogs.hpp

# +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-#
SOURCES += main.cpp \
    $$PRJDIR/3rdParty/QtCUrl/src/QtCUrl.cpp \


################################################################################
#                       DO NOT CHANGE ANYTHING BELOW                           #
################################################################################
ConfigFile = ../qmake/configs.pri
!exists($$ConfigFile){
error("**** $$ProjectName: Unable to find Configuration file $$ConfigFile ****")
}
include ($$ConfigFile)
QT += testlib
QT -= gui

TEMPLATE = app
CONFIG += qt console warn_on depend_includepath testcase
CONFIG -= app_bundle

DESTDIR = $$BaseUnitTestBinFolder
OBJECTS_DIR = $$BaseBuildFolder/unitTest/obj
MOC_DIR = $$BaseBuildFolder/unitTest/moc

LIBS += -lTargomanCommon \
        -lTargomanDBM \
        -lQFieldValidator \
        -lQRESTServer \
        -lqhttp \
        -lcurl


