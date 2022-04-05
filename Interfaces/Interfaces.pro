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
    Server/clsSimpleCrypt.h \
    Server/QJWT.h \
    Common/GenericTypes.h \
    Common/tmplAPIArg.h \
    Common/intfAPIArgManipulator.h \
    Common/HTTPExceptions.hpp \
    Common/GenericEnums.hpp \
    Common/APIArgHelperMacros.hpp \
    Common/QtTypes.hpp \
    Common/ServerCommon.h \
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
    API/intfSQLBasedWithActionLogsModule.h \
    ObjectStorage/ORM/ObjectStorage.h \
    ObjectStorage/ObjectStorageManager.h

PRIVATE_HEADERS += \

HEADERS += \
    Server/clsSimpleCrypt.h \
    Server/QJWT.h \
    Common/ServerCommon.h \
    Common/base.h \
    Common/tmplNullable.hpp \
    Common/APIArgHelperMacrosPrivate.h \
    Helpers/FixtureHelper.h \
    Helpers/RESTClientHelper.h \
    Helpers/SecurityHelper.h \
    Helpers/URLHelper.h \
    Helpers/PhoneHelper.h \
    AAA/AAA.hpp \
    AAA/AAADefs.hpp \
    AAA/Authentication.h \
    AAA/Authorization.h \
    AAA/clsJWT.hpp \
    AAA/PrivHelpers.h \
    AAA/Accounting_Interfaces.h \
    AAA/Accounting_Defs.hpp \
    AAA/intfAccountingBasedModule.h \
    DBM/Defs.hpp \
    DBM/clsORMField.h \
    DBM/clsTable.h \
    DBM/QueryBuilders.h \
    ORM/intfActionLogs.h \
    ORM/intfAlerts.h \
    ObjectStorage/ORM/ObjectStorage.h \
    ObjectStorage/Gateways/intfObjectStorageGateway.h \
    ObjectStorage/Gateways/gtwAWSS3.h \
    ObjectStorage/Gateways/gtwNFS.h \
    ObjectStorage/ObjectStorageManager.h

SOURCES += \
    Server/clsSimpleCrypt.cpp \
    Server/QJWT.cpp \
    Common/ServerCommon.cpp \
    Common/base.cpp \
    Common/GenericTypes.cpp \
    Common/intfAPIArgManipulator.cpp \
    Helpers/FixtureHelper.cpp \
    Helpers/PhoneHelper.cpp \
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
    API/intfPureModule.cpp \
    API/intfSQLBasedModule.cpp \
    API/intfSQLBasedWithActionLogsModule.cpp \
    ORM/intfActionLogs.cpp \
    ORM/intfAlerts.cpp \
    ObjectStorage/ORM/ObjectStorage.cpp \
    ObjectStorage/Gateways/intfObjectStorageGateway.cpp \
    ObjectStorage/Gateways/gtwAWSS3.cpp \
    ObjectStorage/Gateways/gtwNFS.cpp \
    ObjectStorage/ObjectStorageManager.cpp

OTHER_FILES += \
    Test/testBase.hpp \
    Test/testCommon.hpp

INCLUDEPATH += $$BASE_PROJECT_PATH/App/ \
               $$BASE_PROJECT_PATH/3rdParty/E4MT/

################################################################################
include($$QBUILD_PATH/templates/libConfigs.pri)
