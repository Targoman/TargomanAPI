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

#ifndef TARGOMAN_API_MODULES_ACCOUNT_ORM_CURRENCY_H
#define TARGOMAN_API_MODULES_ACCOUNT_ORM_CURRENCY_H

#include "Interfaces/AAA/AAA.hpp"
#include "Interfaces/API/intfSQLBasedModule.h"

namespace Targoman::API::AccountModule {

//structures and enumes goes here

TARGOMAN_DEFINE_ENUM(enuCurrencyStatus,
                     Active   = 'A',
                     Deactive = 'D',
                     Removed  = 'R'
                     )
} //namespace Targoman::API::AccountModule

TAPI_DECLARE_METATYPE_ENUM(Targoman::API::AccountModule, enuCurrencyStatus);

namespace Targoman::API::AccountModule {
namespace ORM {

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-variable"
namespace tblCurrency {
    constexpr char Name[] = "tblCurrency";

    namespace Fields {
        TARGOMAN_CREATE_CONSTEXPR(curID);
        TARGOMAN_CREATE_CONSTEXPR(curName);
        TARGOMAN_CREATE_CONSTEXPR(curSymbol);
        TARGOMAN_CREATE_CONSTEXPR(curRate);
        TARGOMAN_CREATE_CONSTEXPR(curIsDefault);
        TARGOMAN_CREATE_CONSTEXPR(curStatus);
        TARGOMAN_CREATE_CONSTEXPR(curCreatedBy_usrID);
        TARGOMAN_CREATE_CONSTEXPR(curCreationDateTime);
        TARGOMAN_CREATE_CONSTEXPR(curUpdatedBy_usrID);
    } //namespace Fields

    namespace Relation {
//        constexpr char AAA[] = "aaa";
    }

    namespace Private {
        const QList<clsORMField> ORMFields = {
            //ColName                       Type                    Validation                          Default     UpBy     Sort   Filter Self  Virt   PK
            { Fields::curID,                ORM_PRIMARYKEY_32 },
            { Fields::curName,              S(QString),             QFV.unicodeAlNum().maxLenght(128),  QRequired,  UPAdmin },
            { Fields::curSymbol,            S(QString),             QFV.unicodeAlNum().maxLenght(32),   QRequired,  UPAdmin },
            { Fields::curRate,              S(qreal),               QFV.minValue(0.01),                 QRequired,  UPAdmin },
            { Fields::curIsDefault,         S(bool),                QFV,                                false,      UPAdmin },
            { Fields::curStatus,            ORM_STATUS_FIELD(Targoman::API::AccountModule::enuCurrencyStatus, Targoman::API::AccountModule::enuCurrencyStatus::Active) },
            { ORM_INVALIDATED_AT_FIELD },
            { Fields::curCreationDateTime,  ORM_CREATED_ON },
            { Fields::curCreatedBy_usrID,   ORM_CREATED_BY },
            { Fields::curUpdatedBy_usrID,   ORM_UPDATED_BY },
        };

        const QList<stuRelation> Relations = {
            //Col                   Reference Table                 ForeignCol     Rename   LeftJoin
            ORM_RELATION_OF_CREATOR(Fields::curCreatedBy_usrID),
            ORM_RELATION_OF_UPDATER(Fields::curUpdatedBy_usrID),
        };

        const QList<stuDBIndex> Indexes = {
        };

    } //namespace Private

    TAPI_DEFINE_STRUCT(DTO,
        SF_ORM_PRIMARYKEY_32        (curID),
        SF_QString                  (curName),
        SF_QString                  (curSymbol),
        SF_qreal                    (curRate),
        SF_bool                     (curIsDefault),
        SF_ORM_STATUS_FIELD         (curStatus, Targoman::API::AccountModule::enuCurrencyStatus, Targoman::API::AccountModule::enuCurrencyStatus::Active),
        SF_ORM_CREATED_ON           (curCreationDateTime),
        SF_ORM_CREATED_BY           (curCreatedBy_usrID),
        SF_ORM_UPDATED_BY           (curUpdatedBy_usrID)
    );

}
#pragma GCC diagnostic pop

class Currency : public intfSQLBasedModule
{
    Q_OBJECT
    TARGOMAN_DEFINE_API_SUBMODULE(Account, Currency)

private slots:
    QVariant ANONYMOUSE_ORMGET("Get Currency information")
    quint64 ORMCREATE("Create a new Currency")
    bool ORMUPDATE("Update Currency info by priviledged user")
    bool ORMDELETE("Delete a Currency. Take note that default Currency can not be deleted")

    bool REST_UPDATE(
        setAsDefault,
        (
            APICALLBOOM_TYPE_JWT_DECL &APICALLBOOM_PARAM,
            quint32 _curID
        ),
        "change default Currency"
    )

};

} //namespace ORM
} //namespace Targoman::API::AccountModule

#endif // TARGOMAN_API_MODULES_ACCOUNT_ORM_CURRENCY_H
