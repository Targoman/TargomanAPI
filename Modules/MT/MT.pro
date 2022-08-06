################################################################################
#   QBuildSystem
#
#   Copyright(c) 2021 by Targoman Intelligent Processing <http://tip.co.ir>
#
#   Redistribution and use in source and binary forms are allowed under the
#   terms of BSD License 2.0.
################################################################################
include($$QBUILD_PATH/templates/projectConfigs.pri)

addSubdirs(moduleSrc, Dependencies)
#addSubdirs(functionalTest, Dependencies)

OTHER_FILES += \
    migrations/db/* \
    migrations/db/.dbdiff.dev_.cfg \
    migrations/local/* \
    migrations/local/*.sh \
    migrations/local/.migrations

