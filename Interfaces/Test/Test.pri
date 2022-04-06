################################################################################
#   QBuildSystem
#
#   Copyright(c) 2021 by Targoman Intelligent Processing <http://tip.co.ir>
#
#   Redistribution and use in source and binary forms are allowed under the
#   terms of BSD License 2.0.
################################################################################

HEADERS += \
    $$BASE_PROJECT_PATH/Interfaces/Test/testBase.hpp \
    $$BASE_PROJECT_PATH/Interfaces/Test/testCommon.hpp \
    $$BASE_PROJECT_PATH/Interfaces/Server/clsSimpleCrypt.h \
#    $$BASE_PROJECT_PATH/Interfaces/AAA/PrivHelpers.h \
#    $$BASE_PROJECT_PATH/Interfaces/AAA/Authentication.h \
#    $$BASE_PROJECT_PATH/Interfaces/AAA/intfAccountingBasedModule.h \
    $$BASE_PROJECT_PATH/Interfaces/Server/QJWT.h \
    $$BASE_PROJECT_PATH/Interfaces/Helpers/RESTClientHelper.h \
    $$BASE_PROJECT_PATH/Interfaces/Helpers/SecurityHelper.h \

SOURCES += \
    $$BASE_PROJECT_PATH/Interfaces/Server/clsSimpleCrypt.cpp \
#    $$BASE_PROJECT_PATH/Interfaces/AAA/PrivHelpers.cpp \
#    $$BASE_PROJECT_PATH/Interfaces/AAA/Authentication.cpp \
#    $$BASE_PROJECT_PATH/Interfaces/AAA/intfAccountingBasedModule.cpp \
    $$BASE_PROJECT_PATH/Interfaces/Server/QJWT.cpp \
    $$BASE_PROJECT_PATH/Interfaces/Helpers/RESTClientHelper.cpp \
    $$BASE_PROJECT_PATH/Interfaces/Helpers/SecurityHelper.cpp \

DEFINES += UNITTEST_DB_HOST="127.0.0.1"
DEFINES += UNITTEST_DB_PORT=3306
DEFINES += UNITTEST_DB_USER="root"
DEFINES += UNITTEST_DB_PASSWORD="targoman123"
DEFINES += UNITTEST_DB_SCHEMA="AAA"
