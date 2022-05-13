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
using namespace Targoman::API::Helpers;

TAPI_REGISTER_TARGOMAN_ENUM(Targoman::API::AccountModule, enuUserWalletStatus);

namespace Targoman::API::AccountModule::ORM {

UserWallets::UserWallets() :
    intfSQLBasedModule(
        AAASchema,
        tblUserWallets::Name,
        {///<ColName                                    Type                        Validation                          Default     UpBy     Sort   Filter Self  Virt   PK
            { tblUserWallets::walID,                    ORM_PRIMARYKEY_64 },
            { tblUserWallets::wal_usrID,                S(quint64),                 QFV.integer().minValue(1),          QRequired,  UPNone },
            { tblUserWallets::walName,                  S(QString),                 QFV.unicodeAlNum().maxLenght(100),  "default",  UPOwner },
            { tblUserWallets::walDefault,               S(bool),                    QFV,                                false,      UPOwner },
            { tblUserWallets::walMinBalance,            S(qint64),                  QFV,                                0,          UPAdmin, false, false },
            { tblUserWallets::walNotTransferableAmount, S(qint64),                  QFV,                                0,          UPAdmin, false, false },
            { tblUserWallets::walMaxTransferPerDay,     S(qint64),                  QFV,                                10000000,   UPAdmin, false, false },
            { tblUserWallets::walLastBalance,           S(qint64),                  QFV,                                QInvalid,   UPNone,  false, false },
            { tblUserWallets::walSumIncome,             S(qint64),                  QFV,                                QInvalid,   UPNone,  false, false },
            { tblUserWallets::walSumExpenses,           S(qint64),                  QFV,                                QInvalid,   UPNone,  false, false },
            { tblUserWallets::walSumCredit,             S(qint64),                  QFV,                                QInvalid,   UPNone,  false, false },
            { tblUserWallets::walSumDebit,              S(qint64),                  QFV,                                QInvalid,   UPNone,  false, false },
            { tblUserWallets::walStatus,                ORM_STATUS_FIELD(Targoman::API::AccountModule::enuUserWalletStatus, Targoman::API::AccountModule::enuUserWalletStatus::Active) },
            {  ORM_INVALIDATED_AT_FIELD },
            { tblUserWallets::walCreationDateTime,      ORM_CREATED_ON },
            { tblUserWallets::walCreatedBy_usrID,       ORM_CREATED_BY },
            { tblUserWallets::walUpdatedBy_usrID,       ORM_UPDATED_BY },
        },
        {///< Col                        Reference Table             ForeignCol     Rename   LeftJoin
            { tblUserWallets::wal_usrID, R(AAASchema,tblUser::Name), tblUser::usrID },
            ORM_RELATION_OF_CREATOR(tblUserWallets::walCreatedBy_usrID),
            ORM_RELATION_OF_UPDATER(tblUserWallets::walUpdatedBy_usrID),
        },
        {
            { {
                tblUserWallets::wal_usrID,
                tblUserWallets::walName,
                ORM_INVALIDATED_AT_FIELD_NAME,
              }, enuDBIndex::Unique },
        }
    ) { ; }

QVariant IMPL_ORMGET(UserWallets) {
    if (Authorization::hasPriv(_APICALLBOOM.getJWT(), this->privOn(EHTTP_GET, this->moduleBaseName())) == false)
        this->setSelfFilters({ { tblUserWallets::wal_usrID, _APICALLBOOM.getUserID() } }, _filters);

    return this->Select(*this, GET_METHOD_CALL_ARGS_INTERNAL_CALL);
}

quint64 IMPL_ORMCREATE(UserWallets) {
    if (Authorization::hasPriv(_APICALLBOOM.getJWT(), this->privOn(EHTTP_DELETE, this->moduleBaseName())) == false) {
        _createInfo.insert(tblUserWallets::walDefault, 0);

//        this->setSelfFilters({ { tblUserWallets::wal_usrID, _APICALLBOOM.getUserID() } }, _createInfo);
        _createInfo.insert(tblUserWallets::wal_usrID, _APICALLBOOM.getUserID());
    }

    return this->Create(*this, CREATE_METHOD_CALL_ARGS_INTERNAL_CALL);
}

bool IMPL_ORMUPDATE(UserWallets) {
    Authorization::checkPriv(_APICALLBOOM.getJWT(), this->privOn(EHTTP_PATCH, this->moduleBaseName()));

    return this->Update(*this, UPDATE_METHOD_CALL_ARGS_INTERNAL_CALL);
}

bool IMPL_ORMDELETE(UserWallets) {
    TAPI::ORMFields_t ExtraFilters;

    if (Authorization::hasPriv(_APICALLBOOM.getJWT(), this->privOn(EHTTP_DELETE, this->moduleBaseName())) == false) {
        ExtraFilters.insert(tblUserWallets::walDefault, 0);
        ExtraFilters.insert(tblUserWallets::wal_usrID, _APICALLBOOM.getUserID());
    }

    return this->DeleteByPks(*this, DELETE_METHOD_CALL_ARGS_INTERNAL_CALL, ExtraFilters);
}

/**
 * @callby:
 *     operator
 *     owner
 */
bool IMPL_REST_UPDATE(UserWallets, setAsDefault, (
    APICALLBOOM_TYPE_JWT_IMPL &APICALLBOOM_PARAM,
    quint64 _walID
)) {
    bool IsPrivileged = Authorization::hasPriv(_APICALLBOOM.getJWT(), this->privOn(EHTTP_PATCH, this->moduleBaseName()));

    this->callSP("spWallet_SetAsDefault",
                 {
                     { "iUserID", (IsPrivileged ? 0 : _APICALLBOOM.getUserID()) },
                     { "iWalID", _walID },
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

    this->callSP("spWallet_Transfer",
                 {
                     { "iFromUserID", _APICALLBOOM.getUserID() },
                     { "iPass", _pass },
                     { "iSalt", _salt },
                     { "iFromWalID", _fromWalID },
                     { "iAmount", _amount },
                     { "iToUserLogin", _destEmailOrMobile },
                 });

    return true;
}


QVariantList IMPL_REST_GET_OR_POST(UserWallets, availableGatewayTypesForRequestIncrease, (
    APICALLBOOM_TYPE_JWT_IMPL &APICALLBOOM_PARAM,
    quint32 _amount,
    QString _domain
)) {
    if (_amount == 0)
        throw exHTTPBadRequest("amount is zero");

    QVariantList Result = Payment::PaymentLogic::findAvailableGatewayTypes(
                _amount,
                _domain
                );

    if (Result.contains(Targoman::API::AccountModule::enuPaymentGatewayType::COD))
        Result.removeAt(Result.indexOf(Targoman::API::AccountModule::enuPaymentGatewayType::COD));

    return Result;
}

Targoman::API::AAA::stuVoucher IMPL_REST_CREATE(UserWallets, requestIncrease, (
    APICALLBOOM_TYPE_JWT_IMPL &APICALLBOOM_PARAM,
    quint32 _amount,
    Targoman::API::AccountModule::enuPaymentGatewayType::Type _gatewayType,
    QString _domain,
    QString _paymentVerifyCallback,
    quint64 _walID
)) {
    if (_gatewayType == Targoman::API::AccountModule::enuPaymentGatewayType::COD)
        throw exHTTPBadRequest("Call claimOfflinePayment for offline increase");

    Targoman::API::AAA::stuVoucher Voucher;

    Voucher.Info.UserID = _APICALLBOOM.getUserID();
    Voucher.Info.Items.append(Targoman::API::AAA::stuVoucherItem("INC_WALLET", _walID));
    Voucher.Info.Summary = "Increase wallet";
    Voucher.Info.ToPay = _amount;
    Voucher.Info.Sign = QString(voucherSign(QJsonDocument(Voucher.Info.toJson()).toJson()).toBase64());

    Voucher.ID = this->Create(Voucher::instance(),
                              _APICALLBOOM,
                              TAPI::ORMFields_t({
                                                    { tblVoucher::vch_usrID, _APICALLBOOM.getUserID() },
//                                                    { tblVoucher::vchDesc, QJsonDocument(Voucher.Info.toJson()).toJson().constData() },
                                                    { tblVoucher::vchDesc, Voucher.Info.toJson().toVariantMap() },
                                                    { tblVoucher::vchTotalAmount, Voucher.Info.ToPay },
                                                    { tblVoucher::vchType, Targoman::API::AccountModule::enuVoucherType::Credit },
                                                    { tblVoucher::vchStatus, Targoman::API::AAA::enuVoucherStatus::New },
                                                }));

    try {
        if (_gatewayType == Targoman::API::AccountModule::enuPaymentGatewayType::COD) {
            //Do nothing as it will be created after information upload.
        } else {
            TAPI::MD5_t PaymentMD5;
            Voucher.PaymentLink = Targoman::API::AccountModule::Payment::PaymentLogic::createOnlinePaymentLink(
                                      _gatewayType,
                                      _domain,
                                      Voucher.ID,
                                      Voucher.Info.Summary,
                                      Voucher.Info.ToPay,
                                      _paymentVerifyCallback,
                                      PaymentMD5,
                                      _walID
                                      );
            Voucher.PaymentMD5 = PaymentMD5;
        }
    } catch (...) {
        this->Update(Voucher::instance(),
                                     SYSTEM_USER_ID,
                                     {},
                                     TAPI::ORMFields_t({
                                        { tblVoucher::vchStatus, Targoman::API::AAA::enuVoucherStatus::Error }
                                     }),
                                     {
                                        { tblVoucher::vchID, Voucher.ID }
                                     });
        throw;
    }

    return Voucher;
}

quint64 IMPL_REST_POST(UserWallets, requestWithdrawal, (
    APICALLBOOM_TYPE_JWT_IMPL &APICALLBOOM_PARAM,
    quint64 _amount,
    quint64 _walID,
    const QString _desc
)) {
    return this->callSP("spWithdrawal_Request", {
                            { "iWalletID", _walID },
                            { "iForUsrID", _APICALLBOOM.getUserID() },
                            { "iByUserID", _APICALLBOOM.getUserID() },
                            { "iAmount", _amount },
                            { "iDesc", _desc },
                        }).spDirectOutputs().value("oVoucherID").toULongLong();
}

quint64 IMPL_REST_POST(UserWallets, requestWithdrawalFor, (
    APICALLBOOM_TYPE_JWT_IMPL &APICALLBOOM_PARAM,
    quint64 _amount,
    quint64 _targetUsrID,
    const QString _desc
)) {
//    Authorization::checkPriv(_APICALLBOOM.getJWT(), { "AAA:requestWithdrawal" });

    return this->callSP("spWithdrawal_Request", {
                            { "iWalletID", 0 },
                            { "iForUsrID", _targetUsrID },
                            { "iByUserID", _APICALLBOOM.getUserID() },
                            { "iAmount", _amount },
                            { "iDesc", _desc },
                        }).spDirectOutputs().value("oVoucherID").toULongLong();
}

bool IMPL_REST_POST(UserWallets, acceptWithdrawal, (
    APICALLBOOM_TYPE_JWT_IMPL &APICALLBOOM_PARAM,
    quint64 _voucherID
)) {
    Authorization::checkPriv(_APICALLBOOM.getJWT(), { "AAA:acceptWithdrawal" });

    this->callSP("spWithdrawal_Accept", {
                     { "iVoucherID", _voucherID },
                     { "iOperator_usrID", _APICALLBOOM.getUserID() },
                 });

    return true;
}

} //namespace Targoman::API::AccountModule::ORM
