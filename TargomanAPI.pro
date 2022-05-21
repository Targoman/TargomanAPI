################################################################################
#   QBuildSystem
#
#   Copyright(c) 2021 by Targoman Intelligent Processing <http://tip.co.ir>
#
#   Redistribution and use in source and binary forms are allowed under the
#   terms of BSD License 2.0.
################################################################################
include($$QBUILD_PATH/templates/projectConfigs.pri)

addSubdirs(Interfaces, Dependencies)
addSubdirs(App, Interfaces)
addSubdirs(ModuleHelpers, Interfaces)
addSubdirs(Modules, App)
addSubdirs(unitTest, Modules)

# +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-#
OTHER_FILES += \
    README.md \
    INSTALL \
    Dockerfile \
    conf/test.conf \
    conf/invalidPass.tbl \
    conf/api.server.conf \
    conf/api.conf \
    conf/migration.conf \
    Docs/Models/Accounting.scxml \
    runTargomanAPI-dev.sh \
    runTargomanAPI.sh \
    SwaggerUI/* \
    Docker/* \
    Deploy/* \
    .github/workflows/* \
    migrations/MigrationTool/db/* \
    migrations/MigrationTool/local/* \
    migrations/MigrationTool/local/.migrations \
    migrations/CommonFuncs/db/* \
    migrations/CommonFuncs/local/* \
    migrations/CommonFuncs/local/.migrations \
    migrations/I18N/db/* \
    migrations/I18N/local/* \
    migrations/I18N/local/.migrations \
    migrations/MT/db/* \
    migrations/MT/local/* \
    migrations/MT/local/.migrations \
    temp/sql-change-scripts.txt
