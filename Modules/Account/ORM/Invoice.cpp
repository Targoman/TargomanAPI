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
//    bool IsSelf = _EXTRAPATH.split(',').size() == 2 && clsJWT(_JWT).usrID() != _EXTRAPATH.split(',').last().toUInt();
//    if(_EXTRAPATH.isEmpty() || IsSelf == false)
//        Authorization::checkPriv(_JWT,{"Account:Invoice:CRUD~0100"});

//    return this->selectFromTable(AAADACInstance(), {},
//                                 IsSelf ? "" : QString("+inv_usrID=%1").arg(clsJWT(_JWT).usrID()),
//                                 GET_METHOD_CALL_ARGS);
}

Invoice::Invoice() :
    clsTable("AAA",
              "tblInvoice",
              { ///<ColName             Type                 Validation                                  RO   Sort  Filter Self  Virt   PK
                {"invID",               S(quint64),          QFV.integer().minValue(1),                  ORM_PRIMARY_KEY},
                {"invCreationDateTime", S(QHttp::DateTime_t),QFV,                                        true},
                {"invServiceCode",      S(QString),          QFV.asciiAlNum().minLenght(4).maxLenght(4)},
                {"inv_usrID",           S(quint32),          QFV.integer().minValue(1),                  true},
                {"invDesc",             S(QHttp::JSON_t),    QFV.allwaysInvalid(),                       false,false,false},
                {"invPaymentType",      S(Targoman::API::enuInvoiceType::Type)},
                {"invStatus",           S(Targoman::API::enuInvoiceType::Type)},
              },
              { ///< Col       Reference Table   ForeignCol
                {"inv_usrID",  "AAA.tblUser",    "usrID"},
              })
{
    QHTTP_REGISTER_TARGOMAN_ENUM(Targoman::API::enuInvoiceStatus);
    QHTTP_REGISTER_TARGOMAN_ENUM(Targoman::API::enuInvoiceType);

    this->registerMyRESTAPIs();
}

}
}
}


