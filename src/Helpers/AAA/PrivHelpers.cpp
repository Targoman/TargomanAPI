/******************************************************************************
 * TargomanAAA: Authentication, Authorization, Accounting framework           *
 *                                                                            *
 * Copyright 2014-2019 by Targoman Intelligent Processing <http://tip.co.ir>  *
 *                                                                            *
 * TargomanAAA is free software: you can redistribute it and/or modify        *
 * it under the terms of the GNU Lesser General Public License as published   *
 * by the Free Software Foundation, either version 3 of the License, or       *
 * (at your option) any later version.                                        *
 *                                                                            *
 * TargomanAAA is distributed in the hope that it will be useful,             *
 * but WITHOUT ANY WARRANTY; without even the implied warranty of             *
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the              *
 * GNU Lesser General Public License for more details.                        *
 * You should have received a copy of the GNU Lesser General Public License   *
 * along with Targoman. If not, see <http://www.gnu.org/licenses/>.           *
 *                                                                            *
 ******************************************************************************/
/**
 * @author S. Mehran M. Ziabary <ziabary@targoman.com>
 */

#include "PrivHelpers.h"
#include "libTargomanCommon/Helpers.hpp"

namespace Targoman {
namespace API {
namespace Helpers {
namespace AAA {

using namespace DBManager;

QJsonObject PrivHelpers::digestPrivileges(const QJsonArray& _privs, const QStringList& _requiredTLPs) {
    QJsonObject Privs;
    foreach(auto Priv, _privs){
        QJsonObject PrivObj = Priv.toObject();
        for(auto PrivIter = PrivObj.begin(); PrivIter != PrivObj.end(); ++PrivIter)
            if(_requiredTLPs.isEmpty() || _requiredTLPs.contains(PrivIter.key()))
                Privs = Common::mergeJsonObjects(Privs, PrivIter);
    }
    foreach (auto TLP, _requiredTLPs)
        if(Privs.contains(TLP) == false)
            throw exAuthorization("Not enough priviledges to access <"+TLP+">");

    return  Privs;
}

char PrivHelpers::hasPrivBase(const QJsonObject& _privs, const QString& _requiredAccess){
    QStringList AccessItemParts = _requiredAccess.split(":");
    QJsonObject CurrCheckingPriv = _privs;
    foreach(auto Part, AccessItemParts){
        if(Part.startsWith("CRUD~")){
            QString RequiredAccess = Part.split('~').last();
            if(RequiredAccess.size() != 4)
                throw exAAA("Invalid access format for CRUD must be of CRUD~ACCESS");

            QString CheckingPriv = CurrCheckingPriv.value(PRIVItems::CRUD).toString();
            for(quint8 i=0; i<4; ++i)
                if (RequiredAccess[i] == '1' ||  RequiredAccess[i].toUpper() == 'T'){
                    if(CheckingPriv[i] == '0' || CheckingPriv[i].toUpper() == 'F')
                        return false;
                    else if (CheckingPriv[i].toUpper() == 'S')
                        return 'S';
                }


            return true;
        }
        bool Found = false;
        for (auto PrivIter = CurrCheckingPriv.begin(); PrivIter != CurrCheckingPriv.end(); ++PrivIter){
            if(PrivIter.key() == "ALL" && PrivIter.value().toBool())
                return true;
            if(PrivIter.key().toLower() == Part.toLower()){
                CurrCheckingPriv = PrivIter.value().toObject();
                Found = true;
                break;
            }

        if(Found == false)
            return false;
        }
    }

    return true;
}

QJsonObject PrivHelpers::confirmPriviledgeBase(const QJsonObject& _privs, const QStringList& _requiredAccess){
    if (_requiredAccess.isEmpty())
        return _privs;

    if(_privs.isEmpty())
        throw exAAA("Seems that Privs has not been set");

    foreach(auto AccessItem, _requiredAccess)
        if(AccessItem.size() && hasPrivBase(_privs, AccessItem) == false)
            throw exAuthorization("Not enough priviledges on <"+AccessItem+">");
    return _privs;
}

void PrivHelpers::validateToken(const QString& _token){
    static QRegularExpression rxTokenValidator = QRegularExpression("^[a-zA-Z0-9\\-_ ]{8,32}$");
    if(_token.isEmpty())
        throw exNoTokenProvided("No token provided");
    if(rxTokenValidator.match(_token).hasMatch() == false)
        throw exInvalidToken("Invalid token provided");
}

QVariant PrivHelpers::getPrivValue(const QJsonObject& _privs, const QString& _selector){
    QStringList AccessItemParts = _selector.split(":");
    QJsonObject CurrCheckingPriv = _privs;
    foreach(auto Part, AccessItemParts){
        if(CurrCheckingPriv.contains(Part) == false)
            return QVariant();
        CurrCheckingPriv = CurrCheckingPriv.value(Part).toObject();
    }
    return CurrCheckingPriv.value("");
}

QJsonObject PrivHelpers::processObjectPrivs(QJsonObject& _object, const QStringList& _requiredAccess, const QStringList& _requiredTLPs){
    _object = _object[DBM_SPRESULT_ROWS].toArray().at(0).toObject();

    if(_object.size())
        _object[AAACommonItems::privs] = PrivHelpers::confirmPriviledgeBase(PrivHelpers::digestPrivileges(_object[AAACommonItems::privs].toArray(), _requiredTLPs), _requiredAccess);

    return _object;
}


}
}
}
}

