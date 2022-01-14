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

#include "Voucher.h"
#include "User.h"
#include "Service.h"
#include "UserWallets.h"
#include "Payment/PaymentLogic.h"

//#include "Interfaces/ORM/APIQueryBuilders.h"

TAPI_REGISTER_TARGOMAN_ENUM(Targoman::API::AccountModule, enuVoucherType);

namespace Targoman::API::AccountModule::ORM {

Voucher::Voucher() :
    intfSQLBasedModule(
        AAASchema,
        tblVoucher::Name,
        {///< ColName                          Type                     Validation                  Default    UpBy    Sort   Filter Self  Virt   PK
            { tblVoucher::vchID,               ORM_PRIMARYKEY_64 },
            { tblVoucher::vch_usrID,           S(quint64),              QFV.integer().minValue(1),  QRequired, UPNone },
            { tblVoucher::vchDesc,             S(TAPI::JSON_t),         QFV/*.maxLenght(500)*/,     QRequired, UPNone, false, false },
            { tblVoucher::vchType,             S(Targoman::API::AccountModule::enuVoucherType::Type), QFV,                  Targoman::API::AccountModule::enuVoucherType::Expense, UPNone },
            { tblVoucher::vchTotalAmount,      S(quint64),              QFV,                        0,         UPNone },
            { tblVoucher::vchStatus,           ORM_STATUS_FIELD(Targoman::API::AAA::enuVoucherStatus, Targoman::API::AAA::enuVoucherStatus::New) },
            { tblVoucher::vchCreationDateTime, ORM_CREATED_ON },
        },
        {///< Col                     Reference Table              ForeignCol
            { tblVoucher::vch_usrID,  R(AAASchema, tblUser::Name), tblUser::usrID },
        }
    )
{}

QVariant Voucher::apiGET(GET_METHOD_ARGS_IMPL_APICALL)
{
    if (Authorization::hasPriv(_JWT, this->privOn(EHTTP_GET, this->moduleBaseName())) == false)
        this->setSelfFilters({{tblVoucher::vch_usrID, clsJWT(_JWT).usrID()}}, _filters);

    return /*Targoman::API::Query::*/this->Select(*this, GET_METHOD_CALL_ARGS_INTERNAL_CALL);
}

bool Voucher::apiDELETE(DELETE_METHOD_ARGS_IMPL_APICALL)
{
    TAPI::ORMFields_t ExtraFilters;

    if (Authorization::hasPriv(_JWT, this->privOn(EHTTP_DELETE, this->moduleBaseName())) == false){
        ExtraFilters.insert(tblVoucher::vchType, Targoman::API::AccountModule::enuVoucherType::toStr(Targoman::API::AccountModule::enuVoucherType::Withdrawal));

        ExtraFilters.insert(tblVoucher::vch_usrID, clsJWT(_JWT).usrID());
//        this->setSelfFilters({{tblVoucher::vch_usrID, clsJWT(_JWT).usrID()}}, ExtraFilters);
    }

    return /*Targoman::API::Query::*/this->DeleteByPks(*this, DELETE_METHOD_CALL_ARGS_INTERNAL_CALL, ExtraFilters);
}

Targoman::API::AAA::stuVoucher Voucher::apiCREATErequestIncrease(
        TAPI::JWT_t _JWT,
        quint32 _amount,
        Targoman::API::AccountModule::enuPaymentGatewayType::Type _gatewayType,
        QString _domain,
        quint64 _walletID,
        QString _paymentVerifyCallback
    )
{
    Targoman::API::AAA::stuVoucher Voucher;
    Voucher.Info.Items.append(Targoman::API::AAA::stuVoucherItem("INC_WALLET", _walletID));
    Voucher.Info.ToPay = _amount;
    Voucher.Info.Summary = "Increase wallet";

    Voucher.ID = /*Targoman::API::Query::*/this->Create(Voucher::instance(),
                                              clsJWT(_JWT).usrID(),
                                              TAPI::ORMFields_t({
                                                { tblVoucher::vch_usrID,clsJWT(_JWT).usrID() },
//                                                { tblVoucher::vchDesc, QJsonDocument(Voucher.Info.toJson()).toJson().constData() },
                                                { tblVoucher::vchDesc, Voucher.Info.toJson().toVariantMap() },
                                                { tblVoucher::vchTotalAmount, Voucher.Info.ToPay }
                                              }));

    try
    {
        if (_gatewayType == Targoman::API::AccountModule::enuPaymentGatewayType::COD)
        {
            //Do nothing as it will be created after information upload.
        }
        else
        {
            TAPI::MD5_t PaymentMD5;
            Voucher.PaymentLink = Targoman::API::AccountModule::Payment::PaymentLogic::createOnlinePaymentLink(
                                      _gatewayType,
                                      _domain,
                                      Voucher.ID,
                                      Voucher.Info.Summary,
                                      Voucher.Info.ToPay,
                                      _paymentVerifyCallback,
                                      PaymentMD5
                                      );
            Voucher.PaymentMD5 = PaymentMD5;
        }
    }
    catch (...)
    {
        /*Targoman::API::Query::*/this->Update(Voucher::instance(),
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

quint64 Voucher::apiCREATErequestWithdraw(TAPI::JWT_t _JWT, quint64 _amount, quint64 _walID, const QString& _desc)
{
    return this->callSP("spWithdrawal_Request", {
                            {"iWalletID",_walID},
                            {"iForUsrID", clsJWT(_JWT).usrID()},
                            {"iByUserID", clsJWT(_JWT).usrID()},
                            {"iAmount",_amount},
                            {"iDesc",_desc},
                        }).spDirectOutputs().value("oVoucherID").toULongLong();
}

quint64 Voucher::apiCREATErequestWithdrawFor(TAPI::JWT_t _JWT, quint64 _targetUsrID, quint64 _amount, TAPI::JSON_t _desc)
{
    Authorization::checkPriv(_JWT, {"AAA:RequestWithdraw"});

    return this->callSP("spWithdrawal_Request", {
                            {"iWalletID", 0},
                            {"iForUsrID", _targetUsrID},
                            {"iByUserID", clsJWT(_JWT).usrID()},
                            {"iAmount",_amount},
                            {"iDesc",_desc},
                        }).spDirectOutputs().value("oVoucherID").toULongLong();
}

} //namespace Targoman::API::AccountModule::ORM
