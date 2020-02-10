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

#include "Accounting.h"
#include "Interfaces/AAA/AAA.hpp"
#include "Interfaces/Common/GenericEnums.hpp"
#include "Defs.hpp"

namespace Targoman {
namespace API {
namespace Advertisement {
using namespace ORM;

/******************************************************/
AccountOrders::AccountOrders() :
    clsTable(AdvertSchema,
              "tblAccountOrders",
              { ///<ColName              Type                    Validation                   Default    RO   Sort  Filter Self  Virt   PK
                {"acoID",               S(quint64),             QFV.integer().minValue(1),   ORM_PRIMARY_KEY},
                {"aco_usrID",           S(quint32),             QFV.integer().minValue(1),   QInvalid,  true},
                {"aco_ivtID",           S(quint16),             QFV.integer().minValue(1),   QInvalid,  true},
                {"acoCreationDateTime", S(TAPI::DateTime_t),    QFV,                         QNull,     true},
                {"acoValidFrom",        S(TAPI::DateTime_t),    QFV,                         QNull,     true},
                {"acoValidTo",          S(TAPI::DateTime_t),    QFV,                         QNull,     true},
                {"acoMaxPerDayClicks",  S(qint32),              QFV.integer().minValue(-1),  QInvalid,  true, false, false},
                {"acoMaxPerWeekClicks", S(qint32),              QFV.integer().minValue(-1),  QInvalid,  true, false, false},
                {"acoMaxPerMonthClicks",S(qint32),              QFV.integer().minValue(-1),  QInvalid,  true, false, false},
                {"acoMaxTotalClicks",   S(qint64),              QFV.integer().minValue(-1),  QInvalid,  true, false, false},
                {"acoStatus",           S(TAPI::enuAccountOrdersStatus::Type), QFV,          TAPI::enuAccountOrdersStatus::New},
              },
              { ///< Col             Reference Table                ForeignCol   Rename     LeftJoin
                {"aco_ivtID",        "Advert.tblInvoiceTemplate",   "ivtID"},
                //{"act_locID",        "Advert.tblLocations",     "locID"},
              })
{;}
/******************************************************/
AccountUsage::AccountUsage() :
    clsTable(AdvertSchema,
              "tblActiveAds",
              { ///<ColName              Type           Validation                   Default    RO   Sort  Filter Self  Virt   PK
                {"acg_usrID",            S(quint32),    QFV.integer().minValue(1),   ORM_PRIMARY_KEY},
                {"acg_binID",            S(quint32),    QFV.integer().minValue(1),   ORM_PRIMARY_KEY},
                {"acoMaxPerDayClicks",   S(quint32),    QFV.integer().minValue(1),   QInvalid,  true, false, false},
                {"acoMaxPerWeekClicks",  S(quint32),    QFV.integer().minValue(1),   QInvalid,  true, false, false},
                {"acoMaxPerMonthClicks", S(quint32),    QFV.integer().minValue(1),   QInvalid,  true, false, false},
                {"acoMaxTotalClicks",    S(quint64),    QFV.integer().minValue(1),   QInvalid,  true, false, false},
              },
              { ///< Col             Reference Table            ForeignCol   Rename     LeftJoin
                {"act_binID",        "Advert.tblBin",           "binID"},
                //{"act_locID",        "Advert.tblLocations",     "locID"},
              })
{;}
/******************************************************/
InvoiceTemplate::InvoiceTemplate() :
    clsTable(AdvertSchema,
              "tblInvoiceTemplate",
              { ///<ColName              Type                    Validation                   Default    RO   Sort  Filter Self  Virt   PK
                {"ivtID",                S(quint16),    QFV.integer().minValue(1),            ORM_PRIMARY_KEY},
                {"ivtTemplate",          S(quint32),    QFV.integer().minValue(1),            QInvalid, false,false,false},
                {"ivtStatus",            S(TAPI::enuInvoiceTemplateStatus::Type), QFV,        TAPI::enuInvoiceTemplateStatus::Active},
              },
              {
              }
    )
{;}

}
}
}
