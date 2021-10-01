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
    Docs/Models/Accounting.scxml \
    dumpDBSchema-all.sh \
    SwaggerUI/*
