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
    ORM/ApprovalRequest.h \
    ORM/Auth.h \
    ORM/BlockingRules.h \
    ORM/Currency.h \
    ORM/ForgotPassRequest.h \
    ORM/IPBin.h \
    ORM/IPStats.h \
    ORM/PaymentGateways.h \
    ORM/TokenBin.h \
    ORM/User.h \
    ORM/UserWallets.h \
    ORM/WalletTransactions.h \
    ORM/Roles.h \
    ORM/Payments.h \
    ORM/Voucher.h \
    ORM/Service.h \
    Classes/Defs.hpp \
    Payment/Gateways/ActiveGateways.h \
    Payment/PaymentLogic.h \
    Payment/intfPaymentGateway.h \
    Payment/Gateways/gtwDevTest.h \
    Payment/Gateways/gtwZibal.h \
    Payment/Gateways/gtwTarjomyarPay.h \
    Payment/Gateways/gtwMellatBank.h

SOURCES += \
    Account.cpp \
    ORM/ActiveSessions.cpp \
    ORM/APITokens.cpp \
    ORM/ApprovalRequest.cpp \
    ORM/Auth.cpp \
    ORM/BlockingRules.cpp \
    ORM/Currency.cpp \
    ORM/ForgotPassRequest.cpp \
    ORM/IPBin.cpp \
    ORM/IPStats.cpp \
    ORM/PaymentGateways.cpp \
    ORM/TokenBin.cpp \
    ORM/User.cpp \
    ORM/UserWallets.cpp \
    ORM/WalletTransactions.cpp \
    ORM/Roles.cpp \
    ORM/Payments.cpp \
    ORM/Voucher.cpp \
    ORM/Service.cpp \
    Payment/PaymentLogic.cpp \
    Payment/intfPaymentGateway.cpp \
    Payment/Gateways/gtwDevTest.cpp \
    Payment/Gateways/gtwZibal.cpp \
    Payment/Gateways/gtwTarjomyarPay.cpp \
    Payment/Gateways/gtwMellatBank.cpp

OTHER_FILES +=

################################################################################
include($$BASE_PROJECT_PATH/pch/precompiledHeader.pri)

include($$QBUILD_PATH/templates/moduleConfigs.pri)
