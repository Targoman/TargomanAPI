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
 @author S. Mehran M. Ziabary <ziabary@targoman.com>
 */

#include "Invoice.h"
#include "User.h"
#include "Services.h"
#include "UserWallets.h"

TAPI_REGISTER_TARGOMAN_ENUM(TAPI,enuInvoiceType);

namespace Targoman {
namespace API {
namespace AAA {

using namespace ORM;

QVariant Invoice::apiGET(GET_METHOD_ARGS_IMPL)
{
    if(Authorization::hasPriv(_JWT, this->privOn(EHTTP_GET,this->moduleBaseName())) == false)
        this->setSelfFilters({{tblInvoice::inv_usrID, clsJWT(_JWT).usrID()}}, _EXTRAPATH, _ORMFILTERS, _filters);

    return this->selectFromTable({}, {}, GET_METHOD_CALL_ARGS);
}

bool Invoice::apiDELETE(DELETE_METHOD_ARGS_IMPL)
{
    if(Authorization::hasPriv(_JWT, this->privOn(EHTTP_DELETE,this->moduleBaseName())) == false){
        _ORMFILTERS.insert(tblInvoice::invPaymentType, TAPI::enuInvoiceType::toStr(TAPI::enuInvoiceType::Withdrawal));
        this->setSelfFilters({{tblInvoice::inv_usrID, clsJWT(_JWT).usrID()}}, _EXTRAPATH, _ORMFILTERS);
    }

    return this->deleteByPKs(DELETE_METHOD_CALL_ARGS);
}

quint64 Invoice::apiCREATEwithdraw(TAPI::JWT_t _JWT,
                                     quint64 _amount,
                                     quint64 _walletID)
{
    TAPI::ORMFilters_t _ORMFILTERS;
    if(Authorization::hasPriv(_JWT, this->privOn(EHTTP_DELETE,this->moduleBaseName())) == false){
        this->setSelfFilters({{tblInvoice::inv_usrID, clsJWT(_JWT).usrID()}}, {}, _ORMFILTERS);
    }
    _ORMFILTERS.insert("invPaymentType", TAPI::enuInvoiceType::toStr(TAPI::enuInvoiceType::Withdrawal));
    _ORMFILTERS.insert("invTotalAmount", _amount);
    _ORMFILTERS.insert("inv_usrID", clsJWT(_JWT).usrID());
    _ORMFILTERS.insert("invDesc", QJsonDocument(QJsonObject{
                                                    {tblUserWallets::walID, static_cast<double>(_walletID)}
                                                }).toJson());

    return this->create(CREATE_METHOD_CALL_ARGS).toULongLong();
}

Invoice::Invoice() :
    clsTable(AAASchema,
              tblInvoice::Name,
              { ///<ColName             Type                 Validation                                  Default    UpBy   Sort  Filter Self  Virt   PK
                {tblInvoice::invID,               S(quint64),          QFV.integer().minValue(1),                  ORM_PRIMARY_KEY},
                {tblInvoice::invCreationDateTime, S(TAPI::DateTime_t), QFV,                                        QAuto,     UPNone},
                {tblInvoice::inv_svcID,           S(QString),          QFV.asciiAlNum().minLenght(4).maxLenght(4), QInvalid,  UPNone},
                {tblInvoice::inv_ordID,           S(quint64),          QFV.integer().minValue(1),                  QInvalid,  UPNone},
                {tblInvoice::inv_usrID,           S(quint32),          QFV.integer().minValue(1),                  QInvalid,  UPNone},
                {tblInvoice::invDesc,             S(TAPI::JSON_t),     QFV.allwaysInvalid(),                       QNull,     UPNone,false,false},
                {tblInvoice::invPaymentType,      S(TAPI::enuInvoiceType::Type), QFV,                              TAPI::enuInvoiceType::Payment,UPNone},
                {tblInvoice::invTotalAmount,      S(quint64),          QFV,                                        0,UPNone},
                {tblInvoice::invStatus,           S(TAPI::enuInvoiceStatus::Type), QFV,                            TAPI::enuInvoiceStatus::New,UPStatus},
              },
              { ///< Col       Reference Table   ForeignCol
                {tblInvoice::inv_usrID,  R(AAASchema,tblUser::Name),     tblUser::usrID},
                {tblInvoice::inv_svcID,  R(AAASchema,tblServices::Name), tblServices::svcID},
              })
{
}

}
}
}


