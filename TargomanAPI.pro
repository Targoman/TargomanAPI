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
PRJDIR = "."
# +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-#
HEADERS += \
    src/appTargomanAPI.h \
    src/Configs.h \
    3rdParty/E4MT/fastText/src/args.h \
    3rdParty/E4MT/fastText/src/autotune.h \
    3rdParty/E4MT/fastText/src/densematrix.h \
    3rdParty/E4MT/fastText/src/dictionary.h \
    3rdParty/E4MT/fastText/src/fasttext.h \
    3rdParty/E4MT/fastText/src/loss.h \
    3rdParty/E4MT/fastText/src/matrix.h \
    3rdParty/E4MT/fastText/src/meter.h \
    3rdParty/E4MT/fastText/src/model.h \
    3rdParty/E4MT/fastText/src/productquantizer.h \
    3rdParty/E4MT/fastText/src/quantmatrix.h \
    3rdParty/E4MT/fastText/src/real.h \
    3rdParty/E4MT/fastText/src/utils.h \
    3rdParty/E4MT/fastText/src/vector.h \
    3rdParty/E4MT/src/clsFormalityChecker.h \
    3rdParty/QtCUrl/src/QtCUrl.h \
    src/Helpers/AAA/AAADefs.hpp \
    src/Helpers/AAA/Accounting.h \
    src/Helpers/AAA/Authentication.h \
    src/Helpers/AAA/Authorization.h \
    src/Helpers/AAA/clsJWT.hpp \
    src/Helpers/AAA/PrivHelpers.h \
    src/Modules/Translation/Classes/intfTranslatorBridge.hpp \
    src/Modules/Translation/Classes/intfTranslatorEngine.hpp \
    src/Modules/Translation/Classes/TranslationDispatcher.h \
    src/Modules/Translation/Engines/clsNMT.h \
    src/Modules/Translation/Translation.h \
    src/Modules/Translation/TranslationDefs.hpp \
    src/Helpers/AAA/AAA.hpp \
    src/Helpers/AAA/UserEnums.hpp \
    src/Modules/Account/ORM/User.h \
    src/Modules/Account/Account.h \
    src/Helpers/ORM/intfTable.h

# +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-#
SOURCES += \
    src/main.cpp \
    src/appTargomanAPI.cpp \
    src/Configs.cpp \
    3rdParty/E4MT/fastText/src/args.cc \
    3rdParty/E4MT/fastText/src/autotune.cc \
    3rdParty/E4MT/fastText/src/densematrix.cc \
    3rdParty/E4MT/fastText/src/dictionary.cc \
    3rdParty/E4MT/fastText/src/fasttext.cc \
    3rdParty/E4MT/fastText/src/loss.cc \
    3rdParty/E4MT/fastText/src/matrix.cc \
    3rdParty/E4MT/fastText/src/meter.cc \
    3rdParty/E4MT/fastText/src/model.cc \
    3rdParty/E4MT/fastText/src/productquantizer.cc \
    3rdParty/E4MT/fastText/src/quantmatrix.cc \
    3rdParty/E4MT/fastText/src/utils.cc \
    3rdParty/E4MT/fastText/src/vector.cc \
    3rdParty/E4MT/src/clsFormalityChecker.cpp \
    3rdParty/QtCUrl/src/QtCUrl.cpp \
    src/Helpers/AAA/Accounting.cpp \
    src/Helpers/AAA/Authentication.cpp \
    src/Helpers/AAA/Authorization.cpp \
    src/Helpers/AAA/PrivHelpers.cpp \
    src/Modules/Translation/Classes/TranslationDispatcher.cpp \
    src/Modules/Translation/Engines/clsNMT.cpp \
    src/Modules/Translation/Translation.cpp \
    src/Modules/Account/ORM/User.cpp \
    src/Modules/Account/Account.cpp \
    src/Helpers/ORM/intfTable.cpp

INCLUDEPATH+= \
  3rdParty/E4MT/ \

################################################################################
#                       DO NOT CHANGE ANYTHING BELOW                           #
################################################################################
ConfigFile = ./qmake/configs.pri
!exists($$ConfigFile){
error("**** $$ProjectName: Unable to find Configuration file $$ConfigFile ****")
}
include ($$ConfigFile)

TEMPLATE = lib
TARGET = $$ProjectName

TEMPLATE     = app
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
    conf/test.conf
