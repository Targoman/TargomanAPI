################################################################################
#   QBuildSystem
#
#   Copyright(c) 2021 by Targoman Intelligent Processing <http://tip.co.ir>
#
#   Redistribution and use in source and binary forms are allowed under the
#   terms of BSD License 2.0.
################################################################################
ModuleName=Account

HEADERS += \
    Account.h \
    ORM/ActiveSessions.h \
    ORM/APITokens.h \
    ORM/APITokenValidIPs.h \
    ORM/ApprovalRequest.h \
    ORM/BlockingRules.h \
    ORM/ForgotPassRequest.h \
    ORM/IPBin.h \
    ORM/IPStats.h \
    ORM/PaymentGateways.h \
    ORM/User.h \
    ORM/UserWallets.h \
    ORM/WalletTransactions.h \
    ORM/Roles.h \
    ORM/Payments.h \
    Classes/PaymentLogic.h \
    ORM/Voucher.h \
    Classes/Defs.hpp \
    ORM/Service.h \
    PaymentGateways/gtwDevTest.h \
    PaymentGateways/gtwMellatBank.h \
    PaymentGateways/gtwZibal.h \
    PaymentGateways/intfPaymentGateway.h

SOURCES += \
    Account.cpp \
    ORM/ActiveSessions.cpp \
    ORM/APITokens.cpp \
    ORM/APITokenValidIPs.cpp \
    ORM/ApprovalRequest.cpp \
    ORM/BlockingRules.cpp \
    ORM/ForgotPassRequest.cpp \
    ORM/IPBin.cpp \
    ORM/IPStats.cpp \
    ORM/PaymentGateways.cpp \
    ORM/User.cpp \
    ORM/UserWallets.cpp \
    ORM/WalletTransactions.cpp \
    ORM/Roles.cpp \
    ORM/Payments.cpp \
    Classes/PaymentLogic.cpp \
    ORM/Voucher.cpp \
    ORM/Service.cpp \
    PaymentGateways/gtwDevTest.cpp \
    PaymentGateways/gtwMellatBank.cpp \
    PaymentGateways/gtwZibal.cpp \
    PaymentGateways/intfPaymentGateway.cpp

OTHER_FILES += \
    dumpDBSchema.sh \
    ORM/Schema.my.sql

################################################################################
include($$QBUILD_PATH/templates/moduleConfigs.pri)

DISTFILES += \
    ORM/Data.my.sql
