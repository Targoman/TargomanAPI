/******************************************************************************
#   TargomanAPI: REST API for Targoman
#
#   Copyright 2014-2019 by Targoman Intelligent Processing <http://tip.co.ir>
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

#include "Authentication.h"
#include "PrivHelpers.h"

namespace Targoman {
namespace API {
namespace AAA {
namespace Authentication{

QJsonObject login(const QString&     _ip,
                  const QString&     _login,
                  const QString&     _pass,
                  const QString&     _salt,
                  bool               _rememberMe,
                  const QStringList& _requiredTLPs,
                  const QJsonObject& _info, const QString& _fingerPrint)
{
    QJsonObject UserInfo =  AAADACInstance().callSP ("","AAA.sp_UPDATE_login", {
                                                         {"iLogin", _login},
                                                         {"iIP", _ip},
                                                         {"iPass", _pass},
                                                         {"iSalt", _salt},
                                                         {"iInfo", _info},
                                                         {"iFingerPrint", _fingerPrint.isEmpty() ? QVariant() : _fingerPrint},
                                                         {"iRemember", _rememberMe ? "1" : "0"},
                                                         {"iOAuthInfo", QVariant()}
                                                     }).toJson(true).object();
    return PrivHelpers::processObjectPrivs(UserInfo, {}, _requiredTLPs);
}

QJsonObject updatePrivs(const QString& _ip, const QString& _ssid, const QString& _requiredTLPs)
{
    QJsonObject UserInfo =  AAADACInstance().callSP ("","AAA.sp_UPDATE_sessionActivity", {
                                                         {"iIP", _ip},
                                                         {"iSSID", _ssid},
                                                     }).toJson(true).object();
    return PrivHelpers::processObjectPrivs(UserInfo, {}, _requiredTLPs.split(',', QString::SkipEmptyParts));
}

QString retrievePhoto(const QString _url){
    try{
        QByteArray Photo = PrivHelpers::getURL(_url);
        if(_url.endsWith(".jpg") || _url.endsWith(".jpeg"))
            return  "data:image/jpeg;base64," + Photo.toBase64();
        if(_url.endsWith(".png"))
            return  "data:image/png;base64," + Photo.toBase64();
    }catch(...)
    {;}
    return QByteArray();
}

stuOAuthInfo retrieveGoogleUserInfo(const QString& _authToken)
{
    stuOAuthInfo Info;
    QByteArray Json = PrivHelpers::getURL("https://www.googleapis.com/oauth2/v3/tokeninfo?id_token="+_authToken);
    QJsonParseError JsonError;
    QJsonDocument Doc = QJsonDocument::fromJson(Json,& JsonError);
    if(JsonError.error != QJsonParseError::NoError || Doc.isObject() == false)
        throw exAuthentication("Invalid Google Token");

    QJsonObject Obj = Doc.object();

    if(Obj.contains("email") == false
       || Obj.contains("email_verified") == false
       || Obj.value("email_verified").toBool() == false )
        throw exAuthentication("Invalid Google Token: " + Doc.toJson());

    Info.Type   = enuOAuthType::Google;
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
    QByteArray Json = PrivHelpers::getURL("https://api.linkedin.com/v1/people/~?format=json&oauth_token="+_authToken);
    QJsonParseError JsonError;
    QJsonDocument Doc = QJsonDocument::fromJson(Json,& JsonError);
    if(JsonError.error != QJsonParseError::NoError || Doc.isObject() == false)
        throw exAuthentication("Invalid Linkedin Token");

    QJsonObject Obj = Doc.object();

    if(Obj.contains("id") == false)
        throw exAuthentication("Invalid Linkedin Token: " + Doc.toJson());

    Info.Type   = enuOAuthType::Linkedin;
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

}
}
}
}

