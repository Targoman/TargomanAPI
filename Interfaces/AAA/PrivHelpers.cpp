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

#include "PrivHelpers.h"
#include "Interfaces/Server/QJWT.h"
#include "libTargomanCommon/Helpers.hpp"
#include "Interfaces/Common/QtTypes.hpp"
#include "QtCUrl.h"
#include "clsJWT.hpp"
#include "intfAccountingBasedModule.h"

namespace Targoman::API::AAA {

using namespace DBManager;

stuActiveAccount PrivHelpers::digestPrivileges(const QJsonArray& _privs, quint64 _usrID, const QStringList& _services) {
    QJsonObject Privs;

    foreach (auto Service, _services)
        if (serviceAccounting(Service) == nullptr)
            throw exHTTPBadRequest("Service " + Service + " was not registered.");

    qint64 MinTTL = LLONG_MAX;
    foreach (auto Priv, _privs) {
        QJsonObject PrivObj = Priv.toObject();
        for (auto PrivIter = PrivObj.begin(); PrivIter != PrivObj.end(); ++PrivIter)
            if (PrivIter.key() == "ALL" || _services.contains("ALL") || _services.contains(PrivIter.key())) {
                Privs = Targoman::Common::mergeJsonObjects(Privs, PrivIter);
                if (PrivIter.key() != "ALL") {
                    Targoman::API::AAA::stuActiveCredit ActiveAccount = serviceAccounting(PrivIter.key())->activeAccountObject(_usrID);
                    if (ActiveAccount.TTL) {



                        ///@TODO: VERY IMPORTANT fix toJson(false)
                        Privs = Targoman::Common::mergeJsonObjects(Privs, QJsonObject({ { PrivIter.key(), ActiveAccount.toJson(/*false*/) } }).begin());



                        if (ActiveAccount.TTL > 0 && ActiveAccount.TTL < MinTTL)
                            MinTTL = ActiveAccount.TTL;
                    }
                }
            }
    }

    if (Privs.contains("ALL") == false)
        foreach (auto Service, _services)
            if (Privs.contains(Service) == false)
                throw exAuthorization("Not enough privileges to access <"+Service+">");

    return  { MinTTL, Privs };
}

bool PrivHelpers::hasPrivBase(const QJsonObject& _privs, const QString& _requiredAccess, bool _isSelf) {
    QStringList AccessItemParts = _requiredAccess.split(":");
    QJsonObject CurrCheckingPriv = _privs;
    foreach (auto Part, AccessItemParts) {
        if (Part.startsWith("CRUD~")) {
            QString RequiredAccess = Part.split('~').last();
            if (RequiredAccess.size() != 4)
                throw exAAA("Invalid access format for CRUD must be of CRUD~ACCESS");

            QString CheckingPriv = CurrCheckingPriv.value(PRIVItems::CRUD).toString();
            for (quint8 i=0; i<4; ++i)
                if (RequiredAccess[i] == '1' ||
                    RequiredAccess[i].toUpper() == 'T'
                    ) {
                    if (CheckingPriv[i] == '0' || CheckingPriv[i].toUpper() == 'F')
                        return false;
                    else
                        return (CheckingPriv[i].toUpper() == 'T' || _isSelf);
                }


            return true;
        }
        bool Found = false;
        for (auto PrivIter = CurrCheckingPriv.begin(); PrivIter != CurrCheckingPriv.end(); ++PrivIter) {
            if (PrivIter.key() == "ALL" && PrivIter.value().toInt() != 0)
                return true;
            if (PrivIter.key().toLower() == Part.toLower()) {
                CurrCheckingPriv = PrivIter.value().toObject();
                Found = true;
                break;
            }

            if (Found == false)
                return false;
        }
    }

    return true;
}

QJsonObject PrivHelpers::confirmPrivilegeBase(const QJsonObject& _privs, const QStringList& _requiredAccess) {
    if (_requiredAccess.isEmpty())
        return _privs;

    if (_privs.isEmpty())
        throw exAAA("Seems that Privs has not been set");

    foreach (auto AccessItem, _requiredAccess)
        if (AccessItem.size() && PrivHelpers::hasPrivBase(_privs, AccessItem) == false)
            throw exAuthorization("Not enough privileges on <" + AccessItem + ">");

    return _privs;
}

void PrivHelpers::validateToken(const QString& _token) {

    static QRegularExpression rxTokenValidator = QRegularExpression("^[a-zA-Z0-9\\-_ ]{8,32}$");

    if (_token.isEmpty())
        throw exNoTokenProvided("No token provided");

    if (rxTokenValidator.match(_token).hasMatch() == false)
        throw exInvalidToken("Invalid token provided");
}

QVariant PrivHelpers::getPrivValue(const QJsonObject& _privs, const QString& _selector, const QVariant &_defIfNotFoundAndAllIsDefined) {

    if (_privs.isEmpty() || _selector.isEmpty())
        return QVariant();

    QStringList AccessItemParts = _selector.split(":");
    QJsonObject CurrCheckingPriv = _privs;

    foreach (auto Part, AccessItemParts) {
        if (CurrCheckingPriv.contains(Part) == false) {
            if (CurrCheckingPriv.contains("ALL"))
                return _defIfNotFoundAndAllIsDefined;

            return QVariant();
        }

        CurrCheckingPriv = CurrCheckingPriv.value(Part).toObject();
    }

    return CurrCheckingPriv;
}

stuActiveAccount PrivHelpers::processUserObject(
    QJsonObject& _userObj,
    const QStringList& _requiredAccess,
    const QStringList& _services
) {
    if (_userObj.contains(DBM_SPRESULT_ROWS))
        _userObj = _userObj[DBM_SPRESULT_ROWS].toArray().at(0).toObject();

    if (_userObj.size()) {
        stuActiveAccount ActiveAccount =
                PrivHelpers::digestPrivileges(
                    _userObj[AAACommonItems::privs].toArray(),
                    static_cast<quint64>(_userObj[AAACommonItems::usrID].toDouble()),
                    _services);

        _userObj[AAACommonItems::privs] = PrivHelpers::confirmPrivilegeBase(ActiveAccount.Privs, _requiredAccess);

        return { ActiveAccount.TTL, _userObj };
    }

    return { -1, _userObj };
}

QByteArray PrivHelpers::getURL(const QString& _url) {

    QtCUrl CUrl;
    CUrl.setTextCodec("UTF-8");

    QtCUrl::Options Opt;
    Opt[CURLOPT_URL] = _url;
    Opt[CURLOPT_TIMEOUT] = 5;
    Opt[CURLOPT_CONNECTTIMEOUT] = 5;
    Opt[CURLOPT_FAILONERROR] = true;
    QString CUrlResult = CUrl.exec(Opt);

    if (CUrl.lastError().code() == CURLE_OPERATION_TIMEDOUT)
        throw exAccounting("Connection to <" + _url +"> timed out.");
    else if (CUrl.lastError().code() == CURLE_COULDNT_CONNECT)
        throw exAccounting("Connection to <" + _url +"> failed.");
    else if (CUrl.lastError().isOk() == false)
        throw exAccounting("Connection to <" + _url + "> error: " + CUrl.lastError().text());
    else
        return CUrlResult.toUtf8();
}

/******************************************************************************/
//TAPI::EncodedJWT_t clsJWT::createSigned(
//    QJsonObject _payload,
//    QJsonObject _privatePayload,
//    const qint64 _expiry,
//    const QString& _sessionID
//) {
//    return Server::QJWT::createSigned(_payload,
//                                      _privatePayload,
//                                      _expiry < 0 || _expiry == LLONG_MAX ? Server::QJWT::TTL.value() : static_cast<qint32>(_expiry),
//                                      _sessionID);
//}

//TAPI::EncodedJWT_t clsJWT::createSignedLogin(bool _remember, QJsonObject _payload, QJsonObject _privatePayload, const QString& _sessionID)
//{
//    return Server::QJWT::createSigned(_payload, _privatePayload,
//                                      _remember ? Server::QJWT::RememberLoginTTL.value() : Server::QJWT::NormalLoginTTL.value(),
//                                      _sessionID);
//}

} //namespace Targoman::API::AAA
