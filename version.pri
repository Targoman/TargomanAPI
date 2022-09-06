################################################################################
#   QBuildSystem
#
#   Copyright(c) 2021 by Targoman Intelligent Processing <http://tip.co.ir>
#
#   Redistribution and use in source and binary forms are allowed under the
#   terms of BSD License 2.0.
################################################################################
ProjectName="TargomanAPI"

defined(GIT_VERSION, var) {
    VERSION = $$GIT_VERSION
} else {
    VERSION = 0.2
}

#!defined(GIT_VERSION) {
#    DEFINES += GIT_VERSION=$$system(git describe --always)
#}

#+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-++-+-+-
LIBS += -lTargomanCommon \
        -lTargomanDBM \
        -lQFieldValidator \
        -lqhttp \
        -lcurl \
        -lfasttext \
        -lTargomanTextProcessor

defined(QJsonRPC, var) equals(QJsonRPC, 1) {
    DEFINES += WITH_QJsonRPC
    LIBS += -lQJsonRPC
} else {
    DISABLED_DPES += QJsonRPC
}

!defined(NoWebSockets, var) | equals(NoWebSockets, 1) {
    QT += websockets
} else {
    DEFINES += TARGOMAN_API_ENABLE_WEBSOCKET
}

!defined(NoRedis, var) | equals(NoRedis, 1) {
    LIBS += -lhiredis
    DEFINES += TARGOMAN_API_ENABLE_REDIS_PROTOCOL
}

!defined(NoAwsS3, var) | equals(NoAwsS3, 1) {
    LIBS += -laws-cpp-sdk-core -laws-cpp-sdk-s3
    DEFINES += TARGOMAN_API_ENABLE_AWS_S3
}

defined(JWTDastreshte, var) equals(JWTDastreshte, 1) {
    DEFINES += TARGOMAN_API_ENABLE_JWT_DASTRESHTE
}

#+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-++-+-+-
QT += core sql network
QT -= gui
QMAKE_CXXFLAGS += -Wno-unknown-pragmas -Wno-padded

#+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-++-+-+-
# Qt5.5.1 on OSX needs both c++11 and c++14!! the c++14 is not enough
QMAKE_CXXFLAGS += -std=c++11 -std=c++14 -std=c++17
CONFIGS += c++11 c++14 c++17

#+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-++-+-+-
#following definitions are used on all Targoman projects
DEFINES += TARGOMAN_DEBUG_PROCESS_LINE=1
DEFINES += TARGOMAN_SHOW_WARNING=1
DEFINES += TARGOMAN_SHOW_INFO=1
DEFINES += TARGOMAN_SHOW_HAPPY=1
DEFINES += TARGOMAN_SHOW_NORMAL=1
