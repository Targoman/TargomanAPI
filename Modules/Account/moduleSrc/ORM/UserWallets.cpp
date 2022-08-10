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
 * @author S. Mehran M. Ziabary <ziabary@targoman.com>
 * @author Kambiz Zandi <kambizzandi@gmail.com>
 */

#include "UserWallets.h"
#include "User.h"
#include "Voucher.h"
#include "Payment/PaymentLogic.h"
#include "Interfaces/Helpers/PhoneHelper.h"
#include "Interfaces/Helpers/URLHelper.h"
using namespace Targoman::API::Helpers;

TAPI_REGISTER_TARGOMAN_ENUM(Targoman::API::AccountModule, enuUserWalletStatus);

namespace Targoman::API::AccountModule::ORM {

UserWallets::UserWallets() :
    intfSQLBasedModule(
        AAASchema,
        tblUserWallets::Name,
        tblUserWallets::Private::ORMFields,
        tblUserWallets::Private::Relations,
        tblUserWallets::Private::Indexes
) { ; }

QVariant IMPL_ORMGET(UserWallets) {
    if (Authorization::hasPriv(APICALLBOOM_PARAM, this->privOn(EHTTP_GET, this->moduleBaseName())) == false)
        this->setSelfFilters({ { tblUserWallets::Fields::wal_usrID, APICALLBOOM_PARAM.getUserID() } }, _filters);

    return this->Select(GET_METHOD_ARGS_CALL_VALUES);
}

quint64 IMPL_ORMCREATE(UserWallets) {
    if (Authorization::hasPriv(APICALLBOOM_PARAM, this->privOn(EHTTP_DELETE, this->moduleBaseName())) == false) {
        _createInfo.insert(tblUserWallets::Fields::walDefault, 0);

//        this->setSelfFilters({ { tblUserWallets::Fields::wal_usrID, APICALLBOOM_PARAM.getUserID() } }, _createInfo);
        _createInfo.insert(tblUserWallets::Fields::wal_usrID, APICALLBOOM_PARAM.getUserID());
    }

    return this->Create(CREATE_METHOD_ARGS_CALL_VALUES);
}

bool IMPL_ORMUPDATE(UserWallets) {
    Authorization::checkPriv(APICALLBOOM_PARAM, this->privOn(EHTTP_PATCH, this->moduleBaseName()));

    return this->Update(UPDATE_METHOD_ARGS_CALL_VALUES);
}

bool IMPL_ORMDELETE(UserWallets) {
    TAPI::ORMFields_t ExtraFilters;

    if (Authorization::hasPriv(APICALLBOOM_PARAM, this->privOn(EHTTP_DELETE, this->moduleBaseName())) == false) {
        ExtraFilters.insert(tblUserWallets::Fields::walDefault, 0);
        ExtraFilters.insert(tblUserWallets::Fields::wal_usrID, APICALLBOOM_PARAM.getUserID());
    }

    return this->DeleteByPks(DELETE_METHOD_ARGS_CALL_VALUES, ExtraFilters);
}

/**
 * @callby:
 *     operator
 *     owner
 */
bool IMPL_REST_UPDATE(UserWallets, setAsDefault, (
    APICALLBOOM_TYPE_JWT_IMPL &APICALLBOOM_PARAM,
    TAPI::PKsByPath_t _pksByPath
)) {
    bool IsPrivileged = Authorization::hasPriv(APICALLBOOM_PARAM, this->privOn(EHTTP_PATCH, this->moduleBaseName()));

    this->callSP(APICALLBOOM_PARAM,
                 "spWallet_SetAsDefault", {
                     { "iUserID", (IsPrivileged ? 0 : APICALLBOOM_PARAM.getUserID()) },
                     { "iWalID", _pksByPath },
                 });

    return true;
}

bool IMPL_REST_CREATE(UserWallets, transfer, (
    APICALLBOOM_TYPE_JWT_IMPL &APICALLBOOM_PARAM,
    QString _destEmailOrMobile,
    quint32 _amount,
    TAPI::MD5_t _pass,
    QString _salt,
    quint64 _fromWalID
)) {
    QString Type = PhoneHelper::ValidateAndNormalizeEmailOrPhoneNumber(_destEmailOrMobile);

//    QFV.oneOf({QFV.emailNotFake(), QFV.mobile()}).validate(_destEmailOrMobile, "destEmailOrMobile");
    QFV.asciiAlNum().maxLenght(20).validate(_salt, "salt");

    this->callSP(APICALLBOOM_PARAM,
                 "spWallet_Transfer", {
                     { "iFromUserID", APICALLBOOM_PARAM.getUserID() },
                     { "iPass", _pass },
                     { "iSalt", _salt },
                     { "iFromWalID", _fromWalID },
                     { "iAmount", _amount },
                     { "iToUserLogin", _destEmailOrMobile },
                 });

    return true;
}

Targoman::API::AAA::stuVoucher IMPL_REST_CREATE(UserWallets, requestIncrease, (
    APICALLBOOM_TYPE_JWT_IMPL &APICALLBOOM_PARAM,
    quint32 _amount,
    Targoman::API::AccountModule::enuPaymentGatewayType::Type _gatewayType,
    QString _domain,
    QString _paymentVerifyCallback,
    quint64 _walID
)) {
    _domain = URLHelper::domain(_domain, true);

    if (_gatewayType == Targoman::API::AccountModule::enuPaymentGatewayType::COD)
        throw exHTTPBadRequest("requestIncrease is just for Online payments. For offline increase use claimOfflinePayment");

    stuVoucher Voucher;

    Voucher.Info.UserID = APICALLBOOM_PARAM.getUserID();
//    Voucher.Info.Type = enuPreVoucherType::IncreaseWallet;
    Voucher.Info.Items.append(Targoman::API::AAA::stuVoucherItem(VOUCHER_ITEM_NAME_INC_WALLET, _walID));
    Voucher.Info.Summary = "Increase wallet";
    Voucher.Info.ToPay = _amount;
    Voucher.Info.Sign = QString(sign(Voucher.Info));

    Voucher.Remained = _amount;

    Voucher.ID = Voucher::instance().Create(
                     APICALLBOOM_PARAM,
                     TAPI::ORMFields_t({
                                           { tblVoucher::Fields::vch_usrID, APICALLBOOM_PARAM.getUserID() },
                                           { tblVoucher::Fields::vchType, Targoman::API::AAA::enuVoucherType::Credit },
//                                           { tblVoucher::Fields::vchDesc, QJsonDocument(Voucher.Info.toJson()).toJson().constData() },
                                           { tblVoucher::Fields::vchDesc, Voucher.Info.toJson().toVariantMap() },
                                           { tblVoucher::Fields::vchTotalAmount, Voucher.Info.ToPay },
                                           { tblVoucher::Fields::vchStatus, Targoman::API::AAA::enuVoucherStatus::New },
                                       }));

    try {
        if (_gatewayType == Targoman::API::AccountModule::enuPaymentGatewayType::COD) {
            //Do nothing as it will be created after information upload.
        } else {
            TAPI::MD5_t PaymentKey;
            Voucher.PaymentLink = Targoman::API::AccountModule::Payment::PaymentLogic::createOnlinePaymentLink(
                                      APICALLBOOM_PARAM,
                                      _gatewayType,
                                      _domain,
                                      Voucher.ID,
                                      Voucher.Info.Summary,
                                      Voucher.Info.ToPay,
                                      _paymentVerifyCallback,
                                      PaymentKey,
                                      _walID
                                      );
            Voucher.PaymentKey = PaymentKey;
        }
    } catch (...) {
        Voucher::instance().Update(APICALLBOOM_PARAM, //SYSTEM_USER_ID,
                     {},
                     TAPI::ORMFields_t({
                        { tblVoucher::Fields::vchStatus, Targoman::API::AAA::enuVoucherStatus::Error }
                     }),
                     {
                        { tblVoucher::Fields::vchID, Voucher.ID }
                     });
        throw;
    }

    return Voucher;
}

quint64 IMPL_REST_POST(UserWallets, requestWithdrawal, (
    APICALLBOOM_TYPE_JWT_IMPL &APICALLBOOM_PARAM,
    quint64 _amount,
    quint64 _walID,
    const QString &_desc
)) {
    return this->callSP(APICALLBOOM_PARAM,
                        "spWithdrawal_Request", {
                            { "iWalletID", _walID },
                            { "iForUsrID", APICALLBOOM_PARAM.getUserID() },
                            { "iByUserID", APICALLBOOM_PARAM.getUserID() },
                            { "iAmount", _amount },
                            { "iDesc", _desc },
                        }).spDirectOutputs().value("oVoucherID").toULongLong();
}

/**
 * @callby:
 *     operator
 */
quint64 IMPL_REST_POST(UserWallets, requestWithdrawalFor, (
    APICALLBOOM_TYPE_JWT_IMPL &APICALLBOOM_PARAM,
    quint64 _amount,
    quint64 _targetUsrID,
    const QString &_desc
)) {
    Authorization::checkPriv(APICALLBOOM_PARAM, { "AAA:requestWithdrawal" });

    return this->callSP(APICALLBOOM_PARAM,
                        "spWithdrawal_Request", {
                            { "iWalletID", 0 },
                            { "iForUsrID", _targetUsrID },
                            { "iByUserID", APICALLBOOM_PARAM.getUserID() },
                            { "iAmount", _amount },
                            { "iDesc", _desc },
                        }).spDirectOutputs().value("oVoucherID").toULongLong();
}

/**
 * @callby:
 *     operator
 */
bool IMPL_REST_POST(UserWallets, acceptWithdrawal, (
    APICALLBOOM_TYPE_JWT_IMPL &APICALLBOOM_PARAM,
    quint64 _voucherID
)) {
    Authorization::checkPriv(APICALLBOOM_PARAM, { "AAA:acceptWithdrawal" });

    this->callSP(APICALLBOOM_PARAM,
                 "spWithdrawal_Accept", {
                     { "iVoucherID", _voucherID },
                     { "iOperator_usrID", APICALLBOOM_PARAM.getUserID() },
                 });

    return true;
}

} //namespace Targoman::API::AccountModule::ORM

/*
EXPLAIN ANALYZE
SELECT *
FROM tblWalletsTransactions
LEFT JOIN tblWalletsBalanceHistory
ON tblWalletsBalanceHistory.wbl_wltID = tblWalletsTransactions.wltID
LEFT JOIN tblVoucher
ON tblVoucher.vchID = tblWalletsTransactions.wlt_vchID
LEFT JOIN tblUserWallets
ON tblUserWallets.walID = tblWalletsTransactions.wlt_walID
ORDER BY wlt_walID, wltID -- , wltDateTime
-- wlt_walID, wltID
*/
