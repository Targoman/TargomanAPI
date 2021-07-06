################################################################################
#   QBuildSystem
#
#   Copyright(c) 2021 by Targoman Intelligent Processing <http://tip.co.ir>
#
#   Redistribution and use in source and binary forms are allowed under the
#   terms of BSD License 2.0.
################################################################################
LibName=TargomanAPIInterface

DIST_HEADERS += \
    Common/GenericTypes.h \
    Common/tmplAPIArg.h \
    Common/intfAPIArgManipulator.h \
    Common/HTTPExceptions.hpp \
    Common/intfAPIModule.h \
    Common/GenericEnums.hpp \
    Common/APIArgHelperMacros.hpp \
    Common/QtTypes.hpp \
    AAA/AAA.hpp \
    AAA/AAADefs.hpp \
    AAA/Accounting.h \
    AAA/Authentication.h \
    AAA/Authorization.h \
    AAA/clsJWT.hpp \
    AAA/PrivHelpers.h \
    AAA/Accounting_Interfaces.h \
    AAA/Accounting_Defs.hpp \
    ORM/clsORMField.h \
    ORM/clsRESTAPIWithActionLogs.h \
    ORM/clsTable.h \

PRIVATE_HEADERS += \

SOURCES += \
    AAA/Accounting.cpp \
    AAA/Authentication.cpp \
    AAA/Authorization.cpp \
    AAA/PrivHelpers.cpp \
    Common/GenericTypes.cpp \
    ORM/APIQueryBuilders.cpp \
    ORM/QueryBuilders.cpp \
    ORM/clsORMField.cpp \
    ORM/clsRESTAPIWithActionLogs.cpp \
    ORM/clsTable.cpp \
    Common/intf_impl.cpp \
    AAA/Accounting_Interfaces.cpp

OTHER_FILES += \
    Test/testBase.hpp \
    Test/testCommon.hpp \

INCLUDEPATH += $$BASE_PROJECT_PATH/App/ \
               $$BASE_PROJECT_PATH/3rdParty/E4MT/

################################################################################
include($$QBUILD_PATH/templates/libConfigs.pri)

HEADERS += \
    Common/APIArgHelperMacrosPrivate.h \
    Common/tmplNullable.hpp \
    ORM/APIQueryBuilders.h \
    ORM/Defs.hpp \
    ORM/QueryBuilders.h
