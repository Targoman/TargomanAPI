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
    src/Modules/Account/ORM/User.h \
    src/Modules/Account/Account.h \
    src/Modules/Account/ORM/Roles.h \
    src/Modules/Account/ORM/ActiveSessions.h \
    src/Modules/Account/ORM/APITokenValidIPs.h \
    src/Modules/Account/ORM/ForgotPassRequest.h \
    src/Modules/Account/ORM/Invoice.h \
    src/Modules/Account/ORM/IPBin.h \
    src/Modules/Account/ORM/IPStats.h \
    src/Modules/Account/ORM/UserWallets.h \
    src/Modules/Account/ORM/WalletTransactions.h \
    src/Modules/Account/ORM/BlockingRules.h \
    src/Modules/Account/ORM/PaymentOrder.h \
    src/Modules/Account/ORM/APITokens.h \
    src/Helpers/AAA/GenericEnums.hpp \
    src/Modules/Advert/Advert.h \
    src/Modules/Advert/ORM/ActiveAds.h \
    src/Modules/Advert/ORM/Defs.hpp \
    src/Modules/Advert/ORM/Bin.h \
    src/Modules/Advert/ORM/Clicks.h \
    src/Modules/Advert/ORM/Props.h \
    src/Helpers/ORM/clsActionLogs.h \
    src/Helpers/ORM/clsTable.h \
    src/Modules/NGT/NGTv1.h \
    src/Modules/NGT/ORM/Defs.hpp

# +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-#
SOURCES += \
    src/main.cpp \
    src/appTargomanAPI.cpp \
    src/Configs.cpp \
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
    src/Modules/Account/ORM/Roles.cpp \
    src/Modules/Account/ORM/ActiveSessions.cpp \
    src/Modules/Account/ORM/APITokenValidIPs.cpp \
    src/Modules/Account/ORM/ForgotPassRequest.cpp \
    src/Modules/Account/ORM/Invoice.cpp \
    src/Modules/Account/ORM/IPBin.cpp \
    src/Modules/Account/ORM/IPStats.cpp \
    src/Modules/Account/ORM/UserWallets.cpp \
    src/Modules/Account/ORM/WalletTransactions.cpp \
    src/Modules/Account/ORM/BlockingRules.cpp \
    src/Modules/Account/ORM/PaymentOrders.cpp \
    src/Modules/Account/ORM/APITokens.cpp \
#    src/Modules/Account/ORM/ActionLogs.cpp \
    src/Modules/Advert/Advert.cpp \
    src/Modules/Advert/ORM/ActiveAds.cpp \
    src/Modules/Advert/ORM/Bin.cpp \
    src/Modules/Advert/ORM/Clicks.cpp \
    src/Modules/Advert/ORM/Props.cpp \
#    src/Modules/Advert/ORM/ActionLogs.cpp \
    src/Helpers/ORM/clsActionLogs.cpp \
    src/Helpers/ORM/clsTable.cpp \
    src/Modules/NGT/NGTv1.cpp

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
    conf/test.conf \
    Dockerfile \
    conf/api.server.conf
