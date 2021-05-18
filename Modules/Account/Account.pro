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
    ORM/User.h \
    ORM/UserWallets.h \
    ORM/WalletTransactions.h \
    ORM/Roles.h \
    PaymentGateways/Zibal.hpp \
    PaymentGateways/intfPaymentGateway.hpp \
    ORM/Payments.h \
    Classes/PaymentLogic.h \
    ORM/Voucher.h \
    Classes/Defs.hpp \
    ORM/Service.h


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
    ORM/User.cpp \
    ORM/UserWallets.cpp \
    ORM/WalletTransactions.cpp \
    ORM/Roles.cpp \
    ORM/Payments.cpp \
    Classes/PaymentLogic.cpp \
    ORM/Voucher.cpp \
    ORM/Service.cpp

OTHER_FILES += \
    ORM/Schema.my.sql \

################################################################################
include($$QBUILD_PATH/templates/moduleConfigs.pri)
