################################################################################
#   QBuildSystem
#
#   Copyright(c) 2021 by Targoman Intelligent Processing <http://tip.co.ir>
#
#   Redistribution and use in source and binary forms are allowed under the
#   terms of BSD License 2.0.
################################################################################
include($$QBUILD_PATH/templates/projectConfigs.pri)

addSubdirs(Common, Dependencies)
addSubdirs(Account, Dependencies)
addSubdirs(Advert, Dependencies)
addSubdirs(Ticketing, Dependencies)
addSubdirs(MT, Dependencies)
addSubdirs(TargomanMT, Dependencies)
#addSubdirs(NGT, Dependencies)
addSubdirs(TextProcessor, Dependencies)
addSubdirs(FormalityChecker, Dependencies)
addSubdirs(URLProcessor, Dependencies)

#addSubdirs(DM, Dependencies)
