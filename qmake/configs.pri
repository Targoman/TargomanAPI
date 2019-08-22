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
include(version.pri)

#+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-++-+-+-
CONFIG(debug, debug|release): DEFINES += TARGOMAN_SHOW_DEBUG=1
CONFIG(release){
    QMAKE_CXXFLAGS_RELEASE -= -O2
    QMAKE_CXXFLAGS_RELEASE += -O3
}
DEFINES += TARGOMAN_DEBUG_PROCESS_LINE=1
DEFINES += TARGOMAN_SHOW_WARNING=1
DEFINES += TARGOMAN_SHOW_INFO=1
DEFINES += TARGOMAN_SHOW_HAPPY=1
DEFINES += TARGOMAN_SHOW_NORMAL=1

DEFINES += PROJ_VERSION=$$VERSION

#+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-++-+-
contains(QT_ARCH, x86_64){
    LibFolderPattern     = $$PREFIX/lib64
} else {
    LibFolderPattern     = $$PREFIX/lib
}
LibIncludeFolderPattern = ./include
BinFolderPattern        = ./bin
BuildFolderPattern      = ./build
TestBinFolder           = ./test
UnitTestBinFolder       = ./unitTest
ConfigFolderPattern     = ./conf

BaseLibraryFolder        = $$PRJDIR/out/$$LibFolderPattern
BaseLibraryIncludeFolder = $$PRJDIR/out/$$LibIncludeFolderPattern
BaseBinFolder            = $$PRJDIR/out/$$BinFolderPattern
BaseTestBinFolder        = $$PRJDIR/out/$$TestBinFolder
BaseUnitTestBinFolder    = $$PRJDIR/out/$$UnitTestBinFolder
BaseBuildFolder          = $$PRJDIR/out/$$BuildFolderPattern/$$ProjectName
BaseConfigFolder         = $$PRJDIR/out/$$ConfigFolderPattern

#+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-++-+-
INCLUDEPATH += $$PRJDIR \
               $$PRJDIR/src \
               $$PRJDIR/libsrc \
               $$BaseLibraryIncludeFolder \
               $$PREFIX/include \
               $(HOME)/local/include \
               $$DependencyIncludePaths/

#+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-++-+-
DependencyLibPaths      +=   $$BaseLibraryFolder \
                             $$PRJDIR/out/lib64 \
                             $$PRJDIR/out/lib \
                             $$PREFIX/lib64 \
                             $$PREFIX/lib \
                             $(HOME)/local/lib \
                             $(HOME)/local/lib64 \

#+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-++-+-+-
FullDependencySearchPaths = $$DependencyLibPaths
unix:
  FullDependencySearchPaths+=  /usr/lib \
                               /usr/lib64 \
                               /usr/local/lib \
                               /usr/local/lib64 \
                               /lib/x86_64-linux-gnu


QMAKE_LIBDIR +=  $$FullDependencySearchPaths

#+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-++-+-
defineTest(addSubdirs) {
    for(subdirs, 1) {
        entries = $$files($$subdirs)
        for(entry, entries) {
            name = $$replace(entry, [/\\\\], _)
            SUBDIRS += $$name
            eval ($${name}.subdir = $$entry)
            for(dep, 2):eval ($${name}.depends += $$replace(dep, [/\\\\], _))
            export ($${name}.subdir)
            export ($${name}.depends)
        }
    }
    export (SUBDIRS)
}
#+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-++-+-+-
DEPS_BUILT = $$PRJDIR/out/.depsBuilt
Dependencies.target  = $$DEPS_BUILT
Dependencies.depends = FORCE
unix: Dependencies.commands = $$PRJDIR/buildDependencies.sh $$PRJDIR $$DEPS_BUILT;
win32: error(submodule auto-compile has not yet been implemented for windows)

PRE_TARGETDEPS += $$DEPS_BUILT
QMAKE_EXTRA_TARGETS += Dependencies
QMAKE_DISTCLEAN += $$DEPS_BUILT

HEADERS+= $$VERSIONINGHEADER


