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

#include "Classes/PaymentLogic.h"

#include "Interfaces/ORM/APIQueryBuilders.h"

namespace Targoman {
namespace API {
namespace AAA {

using namespace ORM;

QVariant Voucher::apiGET(GET_METHOD_ARGS_IMPL)
{
    if (Authorization::hasPriv(_JWT, this->privOn(EHTTP_GET,this->moduleBaseName())) == false)
        this->setSelfFilters({{tblVoucher::vch_usrID, clsJWT(_JWT).usrID()}}, _filters);

    return Targoman::API::Query::SelectOne(*this, GET_METHOD_CALL_ARGS); //, ExtraFilters, CACHE_TIME);

//    return query.one();

    //    return this->selectFromTable({}, {}, GET_METHOD_CALL_ARGS);
}

bool Voucher::apiDELETE(DELETE_METHOD_ARGS_IMPL)
{
    TAPI::ORMFields_t ExtraFilters;

    if(Authorization::hasPriv(_JWT, this->privOn(EHTTP_DELETE,this->moduleBaseName())) == false){
        ExtraFilters.insert(tblVoucher::vchType, TAPI::enuVoucherType::toStr(TAPI::enuVoucherType::Withdrawal));
        this->setSelfFilters({{tblVoucher::vch_usrID, clsJWT(_JWT).usrID()}}, ExtraFilters);
    }

    return this->deleteByPKs(DELETE_METHOD_CALL_ARGS, ExtraFilters);
}

TAPI::stuVoucher Voucher::apiCREATErequestIncrease(TAPI::JWT_t _JWT,
                                                   quint32 _amount,
                                                   QString _callBack,
                                                   quint64 _walletID,
                                                   TAPI::enuPaymentGateway::Type _gateway)
{
    if(_callBack.size() && _callBack != "OFFLINE")
        QFV.url().validate(_callBack, "callBack");

    TAPI::stuVoucher Voucher;
    Voucher.Info.Items.append(TAPI::stuVoucherItem("INC_WALLET", _walletID));
    Voucher.Info.ToPay = _amount;
    Voucher.Info.Summary = "Increase wallet";

    Voucher.ID = Targoman::API::Query::Create(Voucher::instance(),
                                              _JWT,
                                              TAPI::ORMFields_t({
                                                { tblVoucher::vch_usrID,clsJWT(_JWT).usrID() },
                                                { tblVoucher::vchDesc, QJsonDocument(Voucher.Info.toJson()).toJson().constData() },
                                                { tblVoucher::vchTotalAmount, Voucher.Info.ToPay }
                                              }));
//    Voucher.ID = Voucher::instance().create(clsJWT(_JWT).usrID(), TAPI::ORMFields_t({
//                                                {tblVoucher::vch_usrID,clsJWT(_JWT).usrID()},
//                                                {tblVoucher::vchDesc, QJsonDocument(Voucher.Info.toJson()).toJson().constData()},
//                                                {tblVoucher::vchTotalAmount, Voucher.Info.ToPay}
//                                            })).toULongLong();

    try {
        if(_callBack == "OFFLINE") {
            //Do nothing as it will be created after information upload.
        } else {
            Voucher.PaymentLink = PaymentLogic::createOnlinePaymentLink(_gateway, Voucher.ID, Voucher.Info.Summary, Voucher.Info.ToPay, _callBack);
        }
    } catch (...) {
        Voucher::instance().update(SYSTEM_USER_ID, {}, TAPI::ORMFields_t({
                                       {tblVoucher::vchStatus, TAPI::enuVoucherStatus::Error}
                                   }), {
                                       {tblVoucher::vchID, Voucher.ID}
                                   });
        throw;
    }

    return Voucher;
}

quint64 Voucher::apiCREATErequestWithdraw(TAPI::JWT_t _JWT, quint64 _amount, quint64 _walID, const QString& _desc)
{
    return this->callSP("sp_CREATE_withdrawalRequest", {
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

    return this->callSP("sp_CREATE_withdrawalRequest", {
                            {"iWalletID", 0},
                            {"iForUsrID", _targetUsrID},
                            {"iByUserID", clsJWT(_JWT).usrID()},
                            {"iAmount",_amount},
                            {"iDesc",_desc},
                        }).spDirectOutputs().value("oVoucherID").toULongLong();
}

Voucher::Voucher() :
    clsTable(AAASchema,
             tblVoucher::Name,
{ ///<ColName                       Type                 Validation                                  Default    UpBy   Sort  Filter Self  Virt   PK
{tblVoucher::vchID,               ORM_PRIMARY_KEY64},
{tblVoucher::vchCreationDateTime, S(TAPI::DateTime_t), QFV,                                        QAuto,     UPNone},
{tblVoucher::vch_usrID,           S(quint64),          QFV.integer().minValue(1),                  QRequired, UPNone},
{tblVoucher::vchDesc,             S(QString),          QFV.maxLenght(500),                         QRequired, UPNone,false,false},
{tblVoucher::vchType,             S(TAPI::enuVoucherType::Type), QFV,                              TAPI::enuVoucherType::Expense,UPNone},
{tblVoucher::vchTotalAmount,      S(quint64),          QFV,                                        0,UPNone},
{tblVoucher::vchStatus,           S(TAPI::enuVoucherStatus::Type), QFV,                            TAPI::enuVoucherStatus::New,UPStatus},
},
{ ///< Col       Reference Table   ForeignCol
{tblVoucher::vch_usrID,  R(AAASchema,tblUser::Name),     tblUser::usrID},
})
{
}

}
}
}


