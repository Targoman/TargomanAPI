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
 @author S. Mehran M. Ziabary <ziabary@targoman.com>
 */

#include "Invoice.h"
#include "Helpers/AAA/AAA.hpp"

namespace Targoman {
namespace API {
namespace AAA {
using namespace QHttp;

void Invoice::init()
{;}

QVariant Invoice::apiGET(GET_METHOD_ARGS_IMPL)
{
    if(Authorization::hasPriv(_JWT, this->privOn(EHTTP_GET,this->moduleName())) == false)
        this->setSelfFilters({{"inv_usrID", clsJWT(_JWT).usrID()}}, _EXTRAPATH, _ORMFILTERS, _filters);

    return this->selectFromTable(AAADACInstance(), {}, {}, GET_METHOD_CALL_ARGS);
}

bool Invoice::apiDELETE(DELETE_METHOD_ARGS_IMPL)
{
    if(Authorization::hasPriv(_JWT, this->privOn(EHTTP_DELETE,this->moduleName())) == false){
        _ORMFILTERS.insert("invPaymentType", enuInvoiceType::toStr(enuInvoiceType::Withdrawal));
        this->setSelfFilters({{"inv_usrID", clsJWT(_JWT).usrID()}}, _EXTRAPATH, _ORMFILTERS);
    }

    return this->deleteByPKs(AAADACInstance(), DELETE_METHOD_CALL_ARGS);
}

quint64 Invoice::apiCREATEwithdrawal(QHttp::JWT_t _JWT,
                                     quint64 _amount,
                                     quint64 _walletID)
{
    ORMFilters_t _ORMFILTERS;
    if(Authorization::hasPriv(_JWT, this->privOn(EHTTP_DELETE,this->moduleName())) == false){
        this->setSelfFilters({{"inv_usrID", clsJWT(_JWT).usrID()}}, {}, _ORMFILTERS);
    }
    _ORMFILTERS.insert("invPaymentType", QString(enuInvoiceType::Withdrawal));
    _ORMFILTERS.insert("invTotalAmount", _amount);
    _ORMFILTERS.insert("invServiceCode", "WALT");
    _ORMFILTERS.insert("invDesc", QJsonDocument(QJsonObject{
                                                    {"walletID", static_cast<double>(_walletID)}
                                                }).toJson());

    return this->create(AAADACInstance(), CREATE_METHOD_CALL_ARGS).toULongLong();
}

Invoice::Invoice() :
    clsTable("AAA",
              "tblInvoice",
              { ///<ColName             Type                 Validation                                  Default    RO   Sort  Filter Self  Virt   PK
                {"invID",               S(quint64),          QFV.integer().minValue(1),                  ORM_PRIMARY_KEY},
                {"invCreationDateTime", S(QHttp::DateTime_t),QFV,                                        QNull,     true},
                {"inv_svcID",           S(QString),          QFV.asciiAlNum().minLenght(4).maxLenght(4), QInvalid},
                {"inv_usrID",           S(quint32),          QFV.integer().minValue(1),                  QInvalid,  true},
                {"invDesc",             S(QHttp::JSON_t),    QFV.allwaysInvalid(),                       QNull,    false,false,false},
                {"invPaymentType",      S(Targoman::API::enuInvoiceType::Type), QFV,                     Targoman::API::enuInvoiceType::Payment},
                {"invTotalAmount",      S(quint64),          QFV,                                        0},
                {"invStatus",           S(Targoman::API::enuInvoiceStatus::Type), QFV,                   Targoman::API::enuInvoiceStatus::New},
              },
              { ///< Col       Reference Table   ForeignCol
                {"inv_usrID",  "AAA.tblUser",     "usrID"},
                {"inv_svcID",  "AAA.tblServices", "svcID"},
              })
{
    QHTTP_REGISTER_TARGOMAN_ENUM(Targoman::API::enuInvoiceStatus);
    QHTTP_REGISTER_TARGOMAN_ENUM(Targoman::API::enuInvoiceType);

    this->registerMyRESTAPIs();
}

}
}
}


