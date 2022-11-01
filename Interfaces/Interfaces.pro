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
    Server/ServerCommon.h \
    Server/clsSimpleCrypt.h \
    Server/QJWT.h \
    Server/APICallBoom.h \
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
    ObjectStorage/ORM/ObjectStorage.h \
    ObjectStorage/ObjectStorageManager.h

PRIVATE_HEADERS += \

HEADERS += \
    API/intfModuleHelper.h \
    Helpers/IteratorHelper.hpp \
    Helpers/JSonHelper.h \
    Helpers/TokenHelper.h \
    ORM/intfFAQ.h \
    ORM/intfMigrations.h \
    Server/APICallBoom.h \
    Server/ServerCommon.h \
    Server/clsSimpleCrypt.h \
    Server/QJWT.h \
    Common/base.h \
    Common/APIArgHelperMacrosPrivate.h \
    Common/tmplNullable.hpp \
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
    API/intfModuleHelper.cpp \
    Helpers/JSonHelper.cpp \
    Helpers/TokenHelper.cpp \
    ORM/intfFAQ.cpp \
    ORM/intfMigrations.cpp \
    Server/APICallBoom.cpp \
    Server/ServerCommon.cpp \
    Server/clsSimpleCrypt.cpp \
    Server/QJWT.cpp \
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
    ORM/intfActionLogs.cpp \
    ORM/intfAlerts.cpp \
    ObjectStorage/ORM/ObjectStorage.cpp \
    ObjectStorage/Gateways/intfObjectStorageGateway.cpp \
    ObjectStorage/Gateways/gtwAWSS3.cpp \
    ObjectStorage/Gateways/gtwNFS.cpp \
    ObjectStorage/ObjectStorageManager.cpp

OTHER_FILES += \
    Test/testBase.hpp \
    Test/testCommon.hpp \
    migrations/.gitkeep \
    migrations/accounting/.gitkeep \
    migrations/accounting/db/*.sql \
    migrations/accounting/local/*.sh \
    migrations/accounting/local/.migrations \
    migrations/objectstorage/.gitkeep \
    migrations/objectstorage/db/*.sql \
    migrations/objectstorage/local/*.sh \
    migrations/objectstorage/local/.migrations \
    migrations/actionlogs/.gitkeep \
    migrations/actionlogs/db/*.sql \
    migrations/actionlogs/local/*.sh \
    migrations/actionlogs/local/.migrations \
    migrations/faq/.gitkeep \
    migrations/faq/db/*.sql \
    migrations/faq/local/*.sh \
    migrations/faq/local/.migrations \
    migrations/services/.gitkeep \
    migrations/services/db/*.sql \
    migrations/services/local/*.sh \
    migrations/services/local/.migrations

INCLUDEPATH += \
#    $$BASE_PROJECT_PATH/App/ \
#    $$BASE_PROJECT_PATH/3rdParty/E4MT/

################################################################################
include($$BASE_PROJECT_PATH/pch/precompiledHeader.pri)

include($$QBUILD_PATH/templates/libConfigs.pri)
