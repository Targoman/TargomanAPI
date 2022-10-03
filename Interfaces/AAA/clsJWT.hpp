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

#ifndef TARGOMAN_API_AAA_CLSJWT_HPP
#define TARGOMAN_API_AAA_CLSJWT_HPP

#include <QJsonObject>
#include <QVariantMap>
#include "libTargomanCommon/Macros.h"
#include "Interfaces/Common/GenericEnums.hpp"
#include "Interfaces/Common/GenericTypes.h"
#include "Interfaces/Server/QJWT.h"

namespace Targoman::API::AAA {

using namespace Server;

namespace JWTItems {
TARGOMAN_CREATE_CONSTEXPR(jti);
TARGOMAN_CREATE_CONSTEXPR(iat);
TARGOMAN_CREATE_CONSTEXPR(exp);
//
TARGOMAN_CREATE_CONSTEXPR(own); //owner usr id for token only
TARGOMAN_CREATE_CONSTEXPR(uid); //usrID -> actor id
TARGOMAN_CREATE_CONSTEXPR(typ); //enuTokenActorType::Type
//
TARGOMAN_CREATE_CONSTEXPR(priv);
TARGOMAN_CREATE_CONSTEXPR(privs);
TARGOMAN_CREATE_CONSTEXPR(usrLogin);
TARGOMAN_CREATE_CONSTEXPR(usrName);
TARGOMAN_CREATE_CONSTEXPR(usrFamily);
TARGOMAN_CREATE_CONSTEXPR(usrApproval);
TARGOMAN_CREATE_CONSTEXPR(usrStatus);
TARGOMAN_CREATE_CONSTEXPR(canChangePass);
TARGOMAN_CREATE_CONSTEXPR(rolID);
TARGOMAN_CREATE_CONSTEXPR(rolName);
}

class clsJWT {
public:
    clsJWT(const QJsonObject& _token) : Token(_token) { ; }
    inline QString session() const                              { return this->Token.value(JWTItems::jti).toString(); }
    inline quint64 issuedAt() const                             { return static_cast<quint64>(this->Token.value(JWTItems::iat).toDouble()); }
    inline quint64 expireAt() const                             { return static_cast<quint64>(this->Token.value(JWTItems::exp).toDouble()); }
    //
    inline quint64 actorID() const                              { return static_cast<quint64>(this->Token.value(JWTItems::uid).toDouble()); }
    inline TAPI::enuTokenActorType::Type actorType() const      {
        if (this->Token.contains(JWTItems::typ) == false)
            return TAPI::enuTokenActorType::USER;
        return TAPI::enuTokenActorType::toEnum(this->Token.value(JWTItems::typ).toString());
    }
    //
    inline QVariantMap privatePart() const                      { return this->Token.value(JWTItems::priv).toObject().toVariantMap(); }
    inline QVariantMap privs() const                            { return this->Token.value(JWTItems::privs).toObject().toVariantMap(); }
    inline QJsonObject privsObject() const                      { return this->Token.value(JWTItems::privs).toObject(); }
    inline QString login() const                                { return this->Token.value(JWTItems::usrLogin).toString(); }
    inline QString name() const                                 { return this->Token.value(JWTItems::usrName).toString(); }
    inline QString family() const                               { return this->Token.value(JWTItems::usrFamily).toString(); }
    inline TAPI::enuUserApproval::Type usrApproval() const      { return TAPI::enuUserApproval::toEnum(this->Token.value(JWTItems::usrApproval).toString().toLatin1().constData()); }
    inline TAPI::enuUserStatus::Type usrStatus() const          { return TAPI::enuUserStatus::toEnum(this->Token.value(JWTItems::usrStatus).toString().toLatin1().constData()); }
    inline bool canChangePass() const                           { return this->Token.value(JWTItems::canChangePass).toBool(); }
    inline QString rolName() const                              { return this->Token.value(JWTItems::rolName).toString(); }
    inline quint64 rolID() const                                { return static_cast<quint64>(this->Token.value(JWTItems::rolID).toDouble()); }

    inline quint64 ownerID() const                              { return static_cast<quint64>(this->Token.contains(JWTItems::own) ? this->Token.value(JWTItems::own).toDouble() : 0); }

    inline QJsonValue value(const QLatin1String& _key) const    { return this->Token.value(_key); }

    /**
     * @brief createSignedJWT creates an string containing HEADER.PAYLOAD.SIGNATURE as described by JWT standard.
     * @param _payload The payload to include in JWT. The payload object must not include enteries with following keys:
     *        - iat: reserved for initial time
     *        - exp: reserved for expiration time
     *        - jti: reserved for JWT session
     *        - prv: reserved for private payload
     * @param _privatePayload Optinally private object that will be included in JWT encrypted. There will be no restriction on key values
     * @param _expiry optinally a time in seconds for max life time
     * @param _sessionID optinally a session key for each user to be stored in `jti`
     * @return a base64 encoded string in form of HEADER.PAYLOAD.SIGNATURE
     */
//    static TAPI::EncodedJWT_t createSigned(QJsonObject _payload,
//                                           QJsonObject _privatePayload = {},
//                                           const qint64 _expiry = -1,
//                                           const QString& _sessionID = {});
    /**
     * @brief createSignedLogin creates an string containing HEADER.PAYLOAD.SIGNATURE as described by JWT standard used for login
     * @param _payload The payload to include in JWT. The payload object must not include enteries with following keys:
     *        - iat: reserved for initial time
     *        - exp: reserved for expiration time
     *        - jti: reserved for JWT session
     *        - prv: reserved for private payload
     * @param _privatePayload Optinally private object that will be included in JWT encrypted. There will be no restriction on key values
     * @param _sessionID optinally a session key for each user to be stored in `jti`
     * @return a base64 encoded string in form of HEADER.PAYLOAD.SIGNATURE
     */
//    static TAPI::EncodedJWT_t createSignedLogin(bool _remember, QJsonObject _payload, QJsonObject _privatePayload, const QString& _sessionID = {});
private:
    const QJsonObject& Token;
};

} //namespace Targoman::API::AAA

#endif // TARGOMAN_API_AAA_CLSJWT_HPP
