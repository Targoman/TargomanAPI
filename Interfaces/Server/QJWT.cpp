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
 * @author Kambiz Zandi <kambizzandi@gmail.com>
 */

#include <QMessageAuthenticationCode>
#include <QJsonDocument>
#include <QJsonObject>
#include <QStringList>
#include "QJWT.h"
#include "libTargomanDBM/clsDAC.h"
#include "libTargomanCommon/Configuration/Validators.hpp"
#include "Interfaces/Server/clsSimpleCrypt.h"
#include "Interfaces/Helpers/TokenHelper.h"

namespace Targoman::API::Server {

using namespace TAPI;
using namespace Helpers;
using namespace Common;
using namespace Targoman::Common::Configuration;

tmplConfigurable<QString> QJWT::Secret(
    QJWT::makeConfig("JWTSecret"),
    "Secret to be used for hashing JWT",
    "~5KHeTc7.C^Ln^<X~YnE4<Kr",
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
    static_cast<quint16>(300),
    ReturnTrueCrossValidator(),
    "",
    "TTL",
    "jwt-ttl",
    enuConfigSource::Arg | enuConfigSource::File);

tmplConfigurable<quint32> QJWT::NormalLoginTTL(
    QJWT::makeConfig("NormalLoginTTL"),
    "Time to live for the login token",
    static_cast<quint32>(24*60*60),
    ReturnTrueCrossValidator(),
    "",
    "",
    "jwt-login-ttl",
    enuConfigSource::Arg | enuConfigSource::File);

tmplConfigurable<quint32> QJWT::RememberLoginTTL(
    QJWT::makeConfig("RememberLoginTTL"),
    "Time to live for the login token when remembered",
    static_cast<quint32>(7*24*60*60),
    ReturnTrueCrossValidator(),
    "",
    "TTL",
    "jwt-remember-ttl",
    enuConfigSource::Arg | enuConfigSource::File);

thread_local static clsSimpleCrypt* SimpleCryptInstance = nullptr;
static clsSimpleCrypt* simpleCryptInstance() {
    if (Q_UNLIKELY(!SimpleCryptInstance)) {
        SimpleCryptInstance = new clsSimpleCrypt(QJWT::SimpleCryptKey.value());
        SimpleCryptInstance->setIntegrityProtectionMode(clsSimpleCrypt::ProtectionHash);
    }

    return SimpleCryptInstance;
}

QString QJWT::createSigned(
    INOUT QJsonObject &_payload,
    enuTokenActorType::Type _tokenType,
    QJsonObject _privatePayload,
    const qint64 _expiry,
    const QString &_sessionID,
    const QString &_remoteIP
) {
    _payload["typ"] = enuTokenActorType::toStr(_tokenType);

    if (_payload.contains("iat") == false)
        _payload["iat"] = static_cast<qint64>(QDateTime::currentDateTime().toTime_t());

    if ((_tokenType == enuTokenActorType::API) && (_expiry <= 0 || _expiry == LLONG_MAX))
        throw exHTTPExpectationFailed("Invalid expiry value");

//    if (_expiry >= 0)
        _payload["exp"] = _payload["iat"].toInt() + ((_expiry < 0 || _expiry == LLONG_MAX) ? QJWT::TTL.value() : _expiry);
//    else
//        _payload.remove("exp");

    bool ssnRemember = true;
    if (_tokenType == enuTokenActorType::USER) {
        if (_payload.contains("ssnexp") == false)
            _payload["ssnexp"] = _payload["iat"].toInt()
                    + (qint32)(ssnRemember ? QJWT::RememberLoginTTL.value() : QJWT::NormalLoginTTL.value());
    } else
        _payload.remove("ssnexp");

    if (_sessionID.size())
        _payload["jti"] = _sessionID;
    else
        _payload.remove("jti");

    //---------------------------------
    QJsonObject PayloadForSign = _payload;

    if (_remoteIP.isEmpty() == false)
        _privatePayload["cip"] = _remoteIP;
    else
        _privatePayload.remove("cip");

    if (_privatePayload.isEmpty() == false)
//        PayloadForSign["prv"] = simpleCryptInstance()->encryptToString(QJsonDocument(_privatePayload).toJson(QJsonDocument::Compact));
        PayloadForSign["prv"] = _privatePayload;
    else
        PayloadForSign.remove("prv");

    return QJWT::encryptAndSigned(PayloadForSign);
}

QString QJWT::encryptAndSigned(QJsonObject _payload)
{
    if (_payload.contains("prv")) {
        QJsonDocument Doc = QJsonDocument(_payload["prv"].toObject());
        auto Json = Doc.toJson(QJsonDocument::Compact);
        _payload.remove("prv");
        _payload["prv"] = simpleCryptInstance()->encryptToString(Json);
    }

    QByteArray Head = QString("{\"typ\":\"JWT\",\"alg\":\"%1\"}").arg(enuJWTHashAlgs::toStr(QJWT::HashAlgorithm.value())).toUtf8();
    QByteArray Body = QJsonDocument(_payload).toJson(QJsonDocument::Compact);
    QByteArray Data = Head.toBase64() + "." + Body.toBase64();
    QByteArray Sign = QJWT::hash(Data).toBase64();

//    TargomanDebug(5) << "-----------------------------------------------------------" << endl
//                     << "Head:" << Head << endl
//                     << "Body:" << Body << endl
//                     << "Data:" << Data << endl
//                     << "Sign:" << Sign;

    return Data + "." + Sign;
}

void QJWT::extractAndDecryptPayload(
    const QString &_jwt,
    TAPI::JWT_t &_jWTPayload
) {
    QStringList JWTParts = _jwt.split('.');

    if (JWTParts.length() != 3)
        throw exHTTPForbidden("Invalid JWT Token");

    if (QJWT::hash((JWTParts.at(0) + "." + JWTParts.at(1)).toUtf8()).toBase64() != JWTParts[2])
        throw exHTTPForbidden("JWT signature verification failed");

    QJsonParseError Error;
    QJsonDocument Payload = QJsonDocument::fromJson(QByteArray::fromBase64(JWTParts.at(1).toLatin1()), &Error);

    if (Payload.isNull())
        throw exHTTPForbidden("Invalid JWT payload: " + Error.errorString());

    _jWTPayload = Payload.object();

    if (_jWTPayload.empty())
        throw exHTTPForbidden("Invalid JWT payload: empty object");

    if (_jWTPayload.contains("prv")) {
        QString Decrypted = simpleCryptInstance()->decryptToString(_jWTPayload.value("prv").toString());

        if (Decrypted.isEmpty())
            throw exHTTPExpectationFailed(QString("Invalid empty private JWT payload: DEC ErrNo: %1")
                                          .arg(simpleCryptInstance()->lastError()));

        QJsonDocument Private = QJsonDocument::fromJson(Decrypted.toUtf8(), &Error);

        if (Private.isNull())
            throw exHTTPExpectationFailed("Invalid private JWT payload: " + Error.errorString());

        QJsonObject PrivateObject = Private.object();
        _jWTPayload["prv"] = PrivateObject;
    }
}

void QJWT::verifyJWT(
    const QString &_jwt,
    Q_DECL_UNUSED const QString &_remoteIP,
//    const enuTokenActorType::Type &_acceptableActorType,
    bool _tokenAllowUSER,
    bool _tokenAllowAPI,
    TAPI::enuTokenActorType::Type &_jwtTokenActorType,
    TAPI::JWT_t &_jWTPayload
) {
    QJWT::extractAndDecryptPayload(_jwt, _jWTPayload);

    //-- check actor type -----
    _jwtTokenActorType = enuTokenActorType::USER;

    if (_jWTPayload.contains("typ"))
        _jwtTokenActorType = enuTokenActorType::toEnum(_jWTPayload["typ"].toString());

    if ((_jwtTokenActorType == enuTokenActorType::USER) && (_tokenAllowUSER == false))
        throw exHTTPForbidden("Token type `USER` not acceptable by this module. expected: API");

    if ((_jwtTokenActorType == enuTokenActorType::API) && (_tokenAllowAPI == false))
        throw exHTTPForbidden("Token type `API` not acceptable by this module. expected: USER");

    //-- check client ip -----
//    if (_jWTPayload.contains("prv")) {
//        QJsonObject PrivateObject = _jWTPayload["prv"].toObject();

//        if (PrivateObject.contains("cip") && (PrivateObject["cip"].toString() != _remoteIP))
//            throw exHTTPForbidden("Invalid client IP");
//    }

    uint currentDateTime = QDateTime::currentDateTime().toTime_t();

    //-- check large expiration -----
    if (_jwtTokenActorType == enuTokenActorType::USER) {
        if (_jWTPayload.contains("ssnexp") == false)
            throw exHTTPForbidden("Invalid ssnexp in JWT");

        if (static_cast<quint64>(_jWTPayload.value("ssnexp").toInt()) <= currentDateTime)
            throw exHTTPUnauthorized("Session expired");
    }

    //-- check small expiration -----
    if (_jWTPayload.contains("exp") == false)
        throw exHTTPForbidden("Invalid exp in JWT");

    if (static_cast<quint64>(_jWTPayload.value("exp").toInt()) <= currentDateTime)
        throw exJWTExpired("JWT expired");

    enuTokenBanType TokenBanType = TokenHelper::GetTokenBanType(_jwt);
    if (TokenBanType == enuTokenBanType::Block)
        throw exHTTPForbidden("Token is blocked");
    else if (TokenBanType == enuTokenBanType::Pause)
        throw exHTTPForbidden("Token is paused");
}

QByteArray QJWT::hash(const QByteArray& _data)
{
    switch (QJWT::HashAlgorithm.value()) {
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

} //namespace Targoman::API::Server

ENUM_CONFIGURABLE_IMPL(Targoman::API::Server::enuJWTHashAlgs)
