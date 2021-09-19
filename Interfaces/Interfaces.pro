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
    Common/GenericEnums.hpp \
    Common/APIArgHelperMacros.hpp \
    Common/QtTypes.hpp \
    AAA/AAA.hpp \
    AAA/AAADefs.hpp \
    AAA/Authentication.h \
    AAA/Authorization.h \
    AAA/clsJWT.hpp \
    AAA/PrivHelpers.h \
    AAA/Accounting_Interfaces.h \
    AAA/Accounting_Defs.hpp \
    AAA/intfAccountingBasedModule.h \
    DBM/clsORMField.h \
    DBM/clsTable.h \
    ORM/intfActionLogs.h \
    ORM/Alerts.h \
    API/intfPureModule.h \
    API/intfSQLBasedModule.h \
    API/intfSQLBasedWithActionLogsModule.h

PRIVATE_HEADERS += \

HEADERS += \
    Common/base.h \
    Common/tmplNullable.hpp \
    Common/APIArgHelperMacrosPrivate.h \
    Helpers/RESTClientHelper.h \
    Helpers/SecurityHelper.h \
    Helpers/URLHelper.h \
    DBM/Defs.hpp \
    DBM/clsORMField.h \
    DBM/clsTable.h \
    DBM/QueryBuilders.h \
    ORM/Alerts.h \
    ORM/intfActionLogs.h

SOURCES += \
    Common/base.cpp \
    Common/GenericTypes.cpp \
    Common/intfAPIArgManipulator.cpp \
    Helpers/RESTClientHelper.cpp \
    Helpers/SecurityHelper.cpp \
    Helpers/URLHelper.cpp \
    AAA/Authentication.cpp \
    AAA/Authorization.cpp \
    AAA/Accounting_Interfaces.cpp \
    AAA/PrivHelpers.cpp \
    AAA/intfAccountingBasedModule.cpp \
    DBM/clsORMField.cpp \
    DBM/clsTable.cpp \
    DBM/QueryBuilders.cpp \
    ORM/Alerts.cpp \
    API/intfPureModule.cpp \
    API/intfSQLBasedModule.cpp \
    API/intfSQLBasedWithActionLogsModule.cpp \
    ORM/intfActionLogs.cpp

OTHER_FILES += \
    Test/testBase.hpp \
    Test/testCommon.hpp

INCLUDEPATH += $$BASE_PROJECT_PATH/App/ \
               $$BASE_PROJECT_PATH/3rdParty/E4MT/

################################################################################
include($$QBUILD_PATH/templates/libConfigs.pri)
