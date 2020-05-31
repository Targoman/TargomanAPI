/******************************************************************************
#   TargomanAPI: REST API for Targoman
#
#   Copyright 2014-2020 by Targoman Intelligent Processing <http://tip.co.ir>
#
#   TargomanAPI is free software: you can redistribute it and/or modify
#   it under the terms of the GNU AFFERO GENERAL PUBLIC LICENSE as published by
#   the Free Software Foundation, either version 3 of the License, or
#   (at your option) any later version.
#
#   TargomanAPI is distributed in the hope that it will be useful,
#   but WITHOUT ANY WARRANTY; without even the implied warranty of
#   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#   GNU AFFERO GENERAL PUBLIC LICENSE for more details.
#
#   You should have received a copy of the GNU AFFERO GENERAL PUBLIC LICENSE
#   along with Targoman. If not, see <http://www.gnu.org/licenses/>.
 ******************************************************************************/
/**
 * @author S.Mehran M.Ziabary <ziabary@targoman.com>
 */

#include <QMessageAuthenticationCode>
#include <QJsonDocument>
#include <QJsonObject>
#include <QStringList>

#include "QJWT.h"

#include "libTargomanCommon/Configuration/Validators.hpp"
#include "ServerConfigs.h"
#include "clsSimpleCrypt.h"

namespace Targoman {
namespace API {
namespace Server {

using namespace Common;
using namespace Common::Configuration;

tmplConfigurable<QString> QJWT::Secret(
        QJWT::makeConfig("JWTSecret"),
        "Secret to be used for hashing JWT",
        "~5KHeTc7.C^Ln%<X~YnE4<Kr",
        ReturnTrueCrossValidator(),
        "",
        "SECRET",
        "jwt-secret",
        enuConfigSource::Arg | enuConfigSource::File);

tmplConfigurable<enuJWTHashAlgs::Type> QJWT::HashAlgorithm(
        QJWT::makeConfig("JWTHashAlgorithm"),
        "Hash algorithm toe be used for JWT",
        enuJWTHashAlgs::HS256,
        ReturnTrueCrossValidator(),
        "",
        "ALGORITHM",
        "jwt-hash-alg",
        enuConfigSource::Arg | enuConfigSource::File);

tmplConfigurable<quint64> QJWT::SimpleCryptKey(
        QJWT::makeConfig("SimpleCryptKey"),
        "Secret to be used for encrypting private JWT objects",
        static_cast<quint64>(43121109170974191),
        ReturnTrueCrossValidator(),
        "",
        "SECRET",
        "jwt-private-key",
        enuConfigSource::Arg | enuConfigSource::File);

tmplConfigurable<quint16> QJWT::TTL(
        QJWT::makeConfig("TTL"),
        "Time to live for the token keep it small enough to be updated by Ux Logic",
        static_cast<quint16>(600),
        ReturnTrueCrossValidator(),
        "",
        "TTL",
        "jwt-ttl",
        enuConfigSource::Arg | enuConfigSource::File);

thread_local static clsSimpleCrypt* SimpleCryptInstance = nullptr;
static clsSimpleCrypt* simpleCryptInstance(){
    if(Q_UNLIKELY(!SimpleCryptInstance)){
        SimpleCryptInstance = new clsSimpleCrypt(QJWT::SimpleCryptKey.value());
        SimpleCryptInstance->setIntegrityProtectionMode(clsSimpleCrypt::ProtectionHash);
    }
    return SimpleCryptInstance;
}

QString QJWT::createSigned(QJsonObject _payload, QJsonObject _privatePayload, const qint32 _expiry, const QString& _sessionID)
{
    const QString Header = QString("{\"typ\":\"JWT\",\"alg\":\"%1\"}").arg(enuJWTHashAlgs::toStr(QJWT::HashAlgorithm.value()));

    _payload["iat"] = static_cast<qint64>(QDateTime::currentDateTime().toTime_t());
    if(_expiry >= 0)
        _payload["exp"] = _payload["iat"].toInt() + _expiry;
    else
        _payload.remove("exp");

    if(_sessionID.size())
        _payload["jti"] = _sessionID;
    else
        _payload.remove("jti");

    if(!_privatePayload.isEmpty())
        _payload["prv"] = simpleCryptInstance()->encryptToString(QJsonDocument(_privatePayload).toJson());
    else
        _payload.remove("prv");

    QByteArray Data   = Header.toUtf8().toBase64() + "." + QJsonDocument(_payload).toJson().toBase64();

    return Data + "." + QJWT::hash(Data).toBase64();
}

QJsonObject QJWT::verifyReturnPayload(const QString& _jwt)
{
    QStringList JWTParts = _jwt.split('.');
    if(JWTParts.length() != 3)
        throw exHTTPForbidden("Invalid JWT Token");
    if(QJWT::hash((JWTParts.at(0) + "." + JWTParts.at(1)).toUtf8()).toBase64() != JWTParts[2])
        throw exHTTPForbidden("JWT signature verification failed");
    QJsonParseError Error;
    QJsonDocument Payload = QJsonDocument::fromJson(QByteArray::fromBase64(JWTParts.at(1).toLatin1()), &Error);
    if(Payload.isNull())
        throw exHTTPForbidden("Invalid JWT payload: " + Error.errorString());

    QJsonObject JWTPayload = Payload.object();
    if(JWTPayload.empty())
        throw exHTTPForbidden("Invalid JWT payload: empty object");
    if(JWTPayload.contains("exp") &&
            static_cast<quint64>(JWTPayload.value("exp").toInt()) <= QDateTime::currentDateTime().toTime_t())
            throw exHTTPUnauthorized("JWT expired");

    if(JWTPayload.contains("prv")){
        QString Decrypted= simpleCryptInstance()->decryptToString(JWTPayload.value("prv").toString());
        if(Decrypted.isEmpty())
            throw exHTTPExpectationFailed(QString("Invalid empty private JWT payload: DEC ErrNo: %1").arg(simpleCryptInstance()->lastError()));

        QJsonDocument Private = QJsonDocument::fromJson(Decrypted.toUtf8(), &Error);
        if(Private.isNull())
            throw exHTTPExpectationFailed("Invalid private JWT payload: " + Error.errorString());

        JWTPayload["prv"] = Private.object();
    }

    return JWTPayload;
}

QByteArray QJWT::hash(const QByteArray& _data)
{
    switch(QJWT::HashAlgorithm.value()){
    case enuJWTHashAlgs::HS256:
        return QMessageAuthenticationCode::hash(_data, QJWT::Secret.value().toUtf8(), QCryptographicHash::Sha256);
    case enuJWTHashAlgs::HS384:
        return QMessageAuthenticationCode::hash(_data, QJWT::Secret.value().toUtf8(), QCryptographicHash::Sha384);
    case enuJWTHashAlgs::HS512:
        return QMessageAuthenticationCode::hash(_data, QJWT::Secret.value().toUtf8(), QCryptographicHash::Sha512);
    default:
        throw exHTTPInternalServerError("Invalid JWT encryption algorithm");
    }
}

}
}
}
//ENUM_CONFIGURABLE_IMPL(Targoman::API::Server::enuJWTHashAlgs)
