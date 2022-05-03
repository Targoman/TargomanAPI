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

#include "Authentication.h"
#include "PrivHelpers.h"
#include "Interfaces/AAA/clsJWT.hpp"
#include "Interfaces/Server/QJWT.h"

namespace Targoman::API::AAA::Authentication {

stuActiveAccount login(
    const QString&     _ip,
    const QString&     _login,
    const QString&     _pass,
    const QString&     _salt,
    const QStringList& _requiredServices,
    bool               _rememberMe,
    const QJsonObject& _info,
    const QString&     _fingerPrint
) {
    makeAAADAC(DAC);

    QJsonObject UserInfo = DAC.callSP({},
                                      "spLogin", {
                                          { "iLogin", _login },
                                          { "iIP", _ip },
                                          { "iPass", _pass },
                                          { "iSalt", _salt },
                                          { "iInfo", _info },
                                          { "iRemember", _rememberMe ? "1" : "0" },
                                          { "iOAuthInfo", QVariant() },
                                          { "iFingerPrint", _fingerPrint.isEmpty() ? QVariant() : _fingerPrint },
                                      })
                           .toJson(true)
                           .object();

    return PrivHelpers::processUserObject(UserInfo, {}, _requiredServices);
}

/*
stuActiveAccount updatePrivs(const QString& _ip, const QString& _ssid, const QString& _requiredServices) {
    makeAAADAC(DAC);

    QJsonObject UserInfo = DAC.callSP({},
                                      "spSession_UpdateActivity", {
                                          {"iIP", _ip},
                                          {"iSSID", _ssid},
                                      }).toJson(true).object();

    return PrivHelpers::processUserObject(UserInfo, {}, _requiredServices.split(',', QString::SkipEmptyParts));
}
*/

QString renewExpiredJWT(
    INOUT TAPI::JWT_t &_JWTPayload,
    const QString &_ip,
    /*OUT*/ bool &_isRenewed
) {
    clsJWT JWT(_JWTPayload);

    QString SessionKey =  JWT.session().trimmed();
    if (SessionKey.isEmpty())
        throw exHTTPUnauthorized("Session key not provided in jwt");

    //check session
    QString Qry = R"(
        SELECT ssnJWT
             , ssnStatus
             , ssn_usrID
             , ssnLastRenew
             , usrStatus
          FROM tblActiveSessions
     LEFT JOIN tblUser
            ON tblUser.usrID = tblActiveSessions.ssn_usrID
         WHERE ssnKey=?
)";
    makeAAADAC(DAC);
    QJsonDocument Result = DAC.execQuery({}, Qry, { SessionKey }).toJson(true);

    if (Result.object().isEmpty())
        throw exHTTPUnauthorized("Active session not found");

    QVariantMap SessionInfo = Result.toVariant().toMap();

    QString ssnStatus = SessionInfo["ssnStatus"].toString();
    QString usrStatus = SessionInfo["usrStatus"].toString();
    //session status
    if (ssnStatus.isEmpty())
        throw exHTTPUnauthorized("Invalid Session");
    if (ssnStatus == "E")
        throw exHTTPUnauthorized("Session expired");
    if (ssnStatus == "F")
        throw exHTTPUnauthorized("You were fired out. contact admin");
    if (ssnStatus == "G")
        throw exHTTPUnauthorized("You were logged out");
    //user status
    if (usrStatus == "B")
        throw exTargomanBase("User Blocked. Ask administrator", 405);
    if (usrStatus == "R")
        throw exTargomanBase("User Removed. Ask administrator", 405);
    if (usrStatus != "A")
        throw exTargomanBase("Invalid Session State", 501);

    QString ssnJWT = SessionInfo["ssnJWT"].toString();
    TAPI::JWT_t ssnJWTPayload;
    QJWT::extractAndDecryptPayload(ssnJWT, ssnJWTPayload);

    if (JWT.expireAt() < static_cast<quint64>(ssnJWTPayload.value("exp").toInt())) {
        uint currentDateTime = QDateTime::currentDateTime().toTime_t();
        if (static_cast<quint64>(ssnJWTPayload.value("exp").toInt()) > currentDateTime) {
            _isRenewed = false;
            //resend last jwt
            TargomanDebug(5, "Newer JWT returned to client (j1 -> j0)");
            _JWTPayload = ssnJWTPayload;
            return ssnJWT;
        }

        TargomanDebug(5, "j0 and j1 expired!");
    }

    //-- else: renew -----
    QStringList Services = JWT.privatePart().value("svc").toString().split(',', QString::SkipEmptyParts);
    _isRenewed = true;
    quint32 Duration = JWT.expireAt() - JWT.issuedAt();
    QJsonObject UserInfo = DAC.callSP({},
                                      "spSessionRetrieveInfo", {
                                          { "iSSID", JWT.session() },
                                          { "iIP", _ip },
                                          { "iIssuance", JWT.issuedAt() },
                                      }).toJson(true).object();

    stuActiveAccount ActiveAccount = PrivHelpers::processUserObject(UserInfo, {}, Services);

    _JWTPayload["iat"] = ActiveAccount.Privs["Issuance"];
    _JWTPayload["privs"] = ActiveAccount.Privs["privs"];

    QString NewJWT = Server::QJWT::createSigned(
            _JWTPayload,
            _JWTPayload.contains("prv") ? _JWTPayload["prv"].toObject() : QJsonObject(),
            Duration,
            _JWTPayload["jti"].toString(),
            _ip
    );

    //-- save to active session -----
    DAC.callSP({},
               "spSession_UpdateJWT", {
                   { "iSSID", SessionKey },
                   { "iJWT", NewJWT },
                   { "iIssuance", _JWTPayload["iat"].toInt() },
               });

    return NewJWT;
}

QString retrievePhoto(const QString _url)
{
    try {
        QByteArray Photo = PrivHelpers::getURL(_url);

        if (_url.endsWith(".jpg") || _url.endsWith(".jpeg"))
            return  "data:image/jpeg;base64," + Photo.toBase64();

        if (_url.endsWith(".png"))
            return  "data:image/png;base64," + Photo.toBase64();
    } catch (...) { ; }

    return QByteArray();
}

stuOAuthInfo retrieveGoogleUserInfo(const QString& _authToken)
{
    stuOAuthInfo Info;
    QByteArray Json = PrivHelpers::getURL("https://www.googleapis.com/oauth2/v3/tokeninfo?id_token=" + _authToken);
    QJsonParseError JsonError;
    QJsonDocument Doc = QJsonDocument::fromJson(Json,& JsonError);
    if (JsonError.error != QJsonParseError::NoError || Doc.isObject() == false)
        throw exAuthentication("Invalid Google Token");

    QJsonObject Obj = Doc.object();

    if (Obj.contains("email") == false
            || Obj.contains("email_verified") == false
            || Obj.value("email_verified").toBool() == false )
        throw exAuthentication("Invalid Google Token: " + Doc.toJson());

    Info.Type   = TAPI::enuOAuthType::Google;
    Info.ID     = Obj.value("kid").toString();
    Info.Email  = Obj.value("email").toString();
    Info.Photo  = (Obj.contains("") ? retrievePhoto(Obj.value("picture").toString()) : QString());
    Info.Name   = Obj.value("given_name").toString();
    Info.Family = Obj.value("family_name").toString();

    return Info;
}

stuOAuthInfo retrieveLinkedinUserInfo(const QString& _authToken)
{
    stuOAuthInfo Info;
    QByteArray Json = PrivHelpers::getURL("https://api.linkedin.com/v1/people/~?format=json&oauth_token=" + _authToken);
    QJsonParseError JsonError;
    QJsonDocument Doc = QJsonDocument::fromJson(Json,& JsonError);

    if (JsonError.error != QJsonParseError::NoError || Doc.isObject() == false)
        throw exAuthentication("Invalid Linkedin Token");

    QJsonObject Obj = Doc.object();

    if (Obj.contains("id") == false)
        throw exAuthentication("Invalid Linkedin Token: " + Doc.toJson());

    Info.Type   = TAPI::enuOAuthType::Linkedin;
    Info.ID     = Obj.value("id").toString();
    Info.Email  = QString();
    Info.Photo  = (Obj.contains("") ? retrievePhoto(Obj.value("picture").toString()) : QString());
    Info.Name   = Obj.value("firstName").toString();
    Info.Family = Obj.value("lastName").toString();
    return Info;
}

stuOAuthInfo retrieveYahooUserInfo(const QString& _authToken)
{
    throw exAuthentication("Authentication by Yahoo is not implemented yet");
    Q_UNUSED(_authToken);
}

stuOAuthInfo retrieveGitHubUserInfo(const QString& _authToken)
{
    Q_UNUSED(_authToken);
    throw exAuthentication("Authentication by Github is not implemented yet");
}

} //namespace Targoman::API::AAA::Authentication
