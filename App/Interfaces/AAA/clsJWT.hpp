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

#ifndef TARGOMAN_API_AAA_CLSJWT_HPP
#define TARGOMAN_API_AAA_CLSJWT_HPP

#include <QJsonObject>
#include <QVariantMap>
#include "libTargomanCommon/Macros.h"
#include "GenericEnums.hpp"

namespace Targoman {
namespace API {
namespace AAA {

namespace JWTItems{
    TARGOMAN_CREATE_CONSTEXPR(usrLogin);
    TARGOMAN_CREATE_CONSTEXPR(usrName);
    TARGOMAN_CREATE_CONSTEXPR(usrFamily);
    TARGOMAN_CREATE_CONSTEXPR(rolName);
    TARGOMAN_CREATE_CONSTEXPR(rolID);
    TARGOMAN_CREATE_CONSTEXPR(privs);
    TARGOMAN_CREATE_CONSTEXPR(usrID);
    TARGOMAN_CREATE_CONSTEXPR(usrApproval);
    TARGOMAN_CREATE_CONSTEXPR(usrStatus);
    TARGOMAN_CREATE_CONSTEXPR(jti);
    TARGOMAN_CREATE_CONSTEXPR(priv);
    TARGOMAN_CREATE_CONSTEXPR(canChangePass);
}

class clsJWT{
public:
    clsJWT(const QJsonObject& _token) : Token(_token){;}
    inline QString login(){return this->Token.value(JWTItems::usrLogin).toString();}
    inline QString name(){return this->Token.value(JWTItems::usrName).toString();}
    inline QString family(){return this->Token.value(JWTItems::usrFamily).toString();}
    inline QString rolName(){return this->Token.value(JWTItems::rolName).toString();}
    inline quint64 rolID(){return static_cast<quint64>(this->Token.value(JWTItems::rolID).toDouble());}
    inline QVariantMap privs(){return this->Token.value(JWTItems::privs).toObject().toVariantMap();}
    inline quint64 usrID(){return static_cast<quint64>(this->Token.value(JWTItems::usrID).toDouble());}
    inline TAPI::enuUserApproval::Type usrApproval(){return TAPI::enuUserApproval::toEnum(this->Token.value(JWTItems::usrApproval).toString().toLatin1().constData());}
    inline TAPI::enuUserStatus::Type usrStatus(){return TAPI::enuUserStatus::toEnum(this->Token.value(JWTItems::usrStatus).toString().toLatin1().constData());}
    inline QString session(){return this->Token.value(JWTItems::jti).toString();}
    inline QVariantMap privatePart(){return this->Token.value(JWTItems::priv).toObject().toVariantMap();}
    inline QJsonValue value(const QLatin1String& _key){return this->Token.value(_key);}
    inline bool canChangePass(){return this->Token.value(JWTItems::canChangePass).toBool();}

private:
    const QJsonObject& Token;
};

}
}
}
#endif // TARGOMAN_API_AAA_CLSJWT_HPP
