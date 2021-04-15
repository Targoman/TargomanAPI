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
    AAA/AAA.hpp \
    AAA/AAADefs.hpp \
    AAA/Accounting.h \
    AAA/Authentication.h \
    AAA/Authorization.h \
    AAA/clsJWT.hpp \
    AAA/PrivHelpers.h \
    ORM/clsORMField.h \
    ORM/clsRESTAPIWithActionLogs.h \
    ORM/clsTable.h \
    NLP/FormalityChecker.h \
    NLP/TextProcessor.hpp \
    Common/HTTPExceptions.hpp \
    Common/intfAPIModule.h \
    Common/GenericEnums.hpp \
    Common/APIArgHelperMacros.hpp \
    AAA/Accounting_Interfaces.h \
    AAA/Accounting_Defs.hpp

PRIVATE_HEADERS += \

SOURCES += \
    AAA/Accounting.cpp \
    AAA/Authentication.cpp \
    AAA/Authorization.cpp \
    AAA/PrivHelpers.cpp \
    Common/GenericTypes.cpp \
    ORM/clsORMField.cpp \
    ORM/clsRESTAPIWithActionLogs.cpp \
    ORM/clsTable.cpp \
    NLP/FormalityChecker.cpp \
    $$PRJDIR/3rdParty/E4MT/src/clsFormalityChecker.cpp \
    Common/intf_impl.cpp \
    AAA/Accounting_Interfaces.cpp

OTHER_FILES += \

INCLUDEPATH += ../App/

################################################################################
include($$QBUILD_PATH/templates/libConfigs.pri)
