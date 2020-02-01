################################################################################
#   TargomanDBM: An extension to QSqlDatabase
#
#   Copyright 2014-2020 by Targoman Intelligent Processing <http://tip.co.ir>
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
ModuleName=Account

HEADERS += \
    Account.h \
    ORM/ActiveSessions.h \
    ORM/APITokens.h \
    ORM/APITokenValidIPs.h \
    ORM/ApprovalRequest.h \
    ORM/BlockingRules.h \
    ORM/ForgotPassRequest.h \
    ORM/Invoice.h \
    ORM/IPBin.h \
    ORM/IPStats.h \
    ORM/PaymentOrders.h \
    ORM/Roles.h \
    ORM/Services.h \
    ORM/User.h \
    ORM/UserWallets.h \
    ORM/WalletTransactions.h \


SOURCES += \
    Account.cpp \
    ORM/ActiveSessions.cpp \
    ORM/APITokens.cpp \
    ORM/APITokenValidIPs.cpp \
    ORM/ApprovalRequest.cpp \
    ORM/BlockingRules.cpp \
    ORM/ForgotPassRequest.cpp \
    ORM/Invoice.cpp \
    ORM/IPBin.cpp \
    ORM/IPStats.cpp \
    ORM/PaymentOrders.cpp \
    ORM/Roles.cpp \
    ORM/Services.cpp \
    ORM/User.cpp \
    ORM/UserWallets.cpp \
    ORM/WalletTransactions.cpp \

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
CONFIG += plugin
TARGET = Module_$$ModuleName
!CONFIG(debug): CONFIG+= staticlib

DESTDIR =      $$BaseModulesFolder/
MOC_DIR      = $$BaseBuildFolder/$$ModuleName/moc
OBJECTS_DIR  = $$BaseBuildFolder/$$ModuleName/obj
RCC_DIR      = $$BaseBuildFolder/$$ModuleName/rcc
UI_DIR       = $$BaseBuildFolder/$$ModuleName/ui

QMAKE_CXXFLAGS_RELEASE += -fPIC
QMAKE_CXXFLAGS_DEBUG += -fPIC

