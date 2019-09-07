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
    src/Modules/Translation.h \
    src/Classes/TranslationDispatcher.h \
    src/Classes/intfTranslatorEngine.hpp \
    3rdParty/E4MT/fastText/src/dictionary.h \
    3rdParty/E4MT/fastText/src/args.h \
    3rdParty/E4MT/fastText/src/dictionary.h \
    3rdParty/E4MT/fastText/src/fasttext.h \
    3rdParty/E4MT/fastText/src/matrix.h \
    3rdParty/E4MT/fastText/src/model.h \
    3rdParty/E4MT/fastText/src/productquantizer.h \
    3rdParty/E4MT/fastText/src/qmatrix.h \
    3rdParty/E4MT/fastText/src/real.h \
    3rdParty/E4MT/fastText/src/utils.h \
    3rdParty/E4MT/fastText/src/vector.h \
    3rdParty/E4MT/src/clsFormalityChecker.h \

# +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-#
SOURCES += \
    src/main.cpp \
    src/appTargomanAPI.cpp \
    src/Configs.cpp \
    src/Modules/Translation.cpp \
    src/Classes/TranslationDispatcher.cpp \
    3rdParty/fastText/src/args.cc \
    3rdParty/fastText/src/dictionary.cc \
    3rdParty/fastText/src/fasttext.cc \
    3rdParty/fastText/src/matrix.cc \
    3rdParty/fastText/src/model.cc \
    3rdParty/fastText/src/productquantizer.cc \
    3rdParty/fastText/src/qmatrix.cc \
    3rdParty/fastText/src/utils.cc \
    3rdParty/fastText/src/vector.cc \
    3rdParty/E4MT/src/clsFormalityChecker.cpp \

INCLUDEPATH+= \
  3rdParty/E4MT/src/ \
  3rdParty/E4MT/fastText/ \

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
