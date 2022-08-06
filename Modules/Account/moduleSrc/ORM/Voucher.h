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

#ifndef TARGOMAN_API_MODULES_ACCOUNT_ORM_VOUCHER_H
#define TARGOMAN_API_MODULES_ACCOUNT_ORM_VOUCHER_H

#include "Classes/Defs.hpp"
#include "Interfaces/AAA/AAA.hpp"
#include "Interfaces/API/intfSQLBasedModule.h"

namespace Targoman::API::AccountModule {

//enumes goes here

} //namespace Targoman::API::AccountModule

namespace Targoman::API::AccountModule {

//structures goes here

namespace ORM {

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-variable"

namespace tblVoucher {
    constexpr char Name[] = "tblVoucher";

    namespace Fields {
        TARGOMAN_CREATE_CONSTEXPR(vchID);
        TARGOMAN_CREATE_CONSTEXPR(vch_usrID);
        TARGOMAN_CREATE_CONSTEXPR(vchType);
        TARGOMAN_CREATE_CONSTEXPR(vchDesc);
        TARGOMAN_CREATE_CONSTEXPR(vch_rootVchID);
        TARGOMAN_CREATE_CONSTEXPR(vchTotalAmount);
        TARGOMAN_CREATE_CONSTEXPR(vchTotalPayed);
        TARGOMAN_CREATE_CONSTEXPR(vchSettled);
        TARGOMAN_CREATE_CONSTEXPR(vchProcessResult);
        TARGOMAN_CREATE_CONSTEXPR(vchStatus);
        TARGOMAN_CREATE_CONSTEXPR(vchCreationDateTime);
    }

    namespace Relation {
//        constexpr char AAA[] = "aaa";
    }

    namespace Private {
        const QList<clsORMField> ORMFields = {
            //ColName                       Type                    Validation                  Default     UpBy        Sort    Filter Self  Virt   PK
            { Fields::vchID,                ORM_PRIMARYKEY_64 },
            { Fields::vch_usrID,            S(quint64),             QFV.integer().minValue(1),  QRequired,  UPNone },
            { Fields::vchType,              S(Targoman::API::AAA::enuVoucherType::Type), QFV,   QRequired /*Targoman::API::AAA::enuVoucherType::Invoice*/, UPNone },
            { Fields::vchDesc,              S(TAPI::JSON_t),        QFV/*.maxLenght(500)*/,     QRequired,  UPAdmin,    false,  false },
            { Fields::vch_rootVchID,        S(NULLABLE_TYPE(quint64)),QFV,                      QNull,      UPAdmin },
            { Fields::vchTotalAmount,       S(quint64),             QFV,                        0,          UPNone },
            { Fields::vchTotalPayed,        S(quint64),             QFV,                        QNull,      UPAdmin },
            { Fields::vchSettled,           S(bool),                QFV,                        false,      UPAdmin },
            { Fields::vchProcessResult,     S(TAPI::JSON_t),        QFV,                        QNull,      UPAdmin,    false,  false },
            { Fields::vchStatus,            ORM_STATUS_FIELD(Targoman::API::AAA::enuVoucherStatus, Targoman::API::AAA::enuVoucherStatus::New) },
            { Fields::vchCreationDateTime,  ORM_CREATED_ON },
        };

        const QList<stuRelation> Relations = {
            ///Col                     Reference Table              ForeignCol
            { Fields::vch_usrID,  R(AAASchema, tblUser::Name), tblUser::Fields::usrID },
        };

        const QList<stuDBIndex> Indexes = {
        };

    } //namespace Private

    TAPI_DEFINE_STRUCT(DTO,
        SF_ORM_PRIMARYKEY_64        (vchID),
        SF_quint64                  (vch_usrID),
        SF_NULLABLE_Enum            (vchType, Targoman::API::AAA::enuVoucherType),
        SF_JSON_t                   (vchDesc),
        SF_NULLABLE_quint64         (vch_rootVchID),
        SF_quint64                  (vchTotalAmount),
        SF_NULLABLE_quint64         (vchTotalPayed),
        SF_bool                     (vchSettled),
        SF_JSON_t                   (vchProcessResult),
        SF_ORM_STATUS_FIELD         (vchStatus, Targoman::API::AAA::enuVoucherStatus, Targoman::API::AAA::enuVoucherStatus::New),
        SF_DateTime_t               (vchCreationDateTime)
    );
}
#pragma GCC diagnostic pop

class Voucher : public intfSQLBasedModule
{
    Q_OBJECT
    TARGOMAN_DEFINE_API_SUBMODULE(Account, Voucher)

public:
    virtual ORMSelectQuery makeSelectQuery(INTFAPICALLBOOM_DECL &APICALLBOOM_PARAM, const QString &_alias = {}, bool _isRoot = true);

private slots:
    QVariant ORMGET("Get Voucher information")
    bool ORMDELETE("Delete a Voucher. Take note that User can just delete Vouchers with Payoff type")
};

} //namespace ORM
} //namespace Targoman::API::AccountModule

#endif // TARGOMAN_API_MODULES_ACCOUNT_ORM_VOUCHER_H
