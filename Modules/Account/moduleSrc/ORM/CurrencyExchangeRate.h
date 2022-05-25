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

#ifndef TARGOMAN_API_MODULES_ACCOUNT_ORM_CURRENCYEXCHANGERATE_H
#define TARGOMAN_API_MODULES_ACCOUNT_ORM_CURRENCYEXCHANGERATE_H

#include "Interfaces/AAA/AAA.hpp"
#include "Interfaces/API/intfSQLBasedModule.h"

namespace Targoman::API::AccountModule {

//structures and enumes goes here

TARGOMAN_DEFINE_ENUM(enuCurrencyExchangeRateStatus,
                     Active   = 'A',
                     Deactive = 'D',
                     Removed  = 'R'
                     )
} //namespace Targoman::API::AccountModule

TAPI_DECLARE_METATYPE_ENUM(Targoman::API::AccountModule, enuCurrencyExchangeRateStatus);

namespace Targoman::API::AccountModule {
namespace ORM {

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-variable"
namespace tblCurrencyExchangeRate {

    constexpr char Name[] = "tblCurrencyExchangeRate";

    namespace Fields {
        TARGOMAN_CREATE_CONSTEXPR(exrID);
        TARGOMAN_CREATE_CONSTEXPR(exrName);
        TARGOMAN_CREATE_CONSTEXPR(exrSymbol);
        TARGOMAN_CREATE_CONSTEXPR(exrRate);
        TARGOMAN_CREATE_CONSTEXPR(exrIsDefault);
        TARGOMAN_CREATE_CONSTEXPR(exrCreatedBy_usrID);
        TARGOMAN_CREATE_CONSTEXPR(exrCreationDateTime);
        TARGOMAN_CREATE_CONSTEXPR(exrUpdatedBy_usrID);
        TARGOMAN_CREATE_CONSTEXPR(exrStatus);
    } //namespace Fields

    inline QStringList ColumnNames() {
        return {
            Fields::exrID,
            Fields::exrName,
            Fields::exrSymbol,
            Fields::exrRate,
            Fields::exrIsDefault,
            Fields::exrCreatedBy_usrID,
            Fields::exrCreationDateTime,
            Fields::exrUpdatedBy_usrID,
            Fields::exrStatus,
        };
    }

    namespace Private {
        const QList<clsORMField> ORMFields = {
            //ColName                       Type                    Validation                          Default     UpBy     Sort   Filter Self  Virt   PK
            { Fields::exrID,                ORM_PRIMARYKEY_32 },
            { Fields::exrName,              S(QString),             QFV.unicodeAlNum().maxLenght(128),  QRequired,  UPAdmin },
            { Fields::exrSymbol,            S(QString),             QFV.unicodeAlNum().maxLenght(32),   QRequired,  UPAdmin },
            { Fields::exrRate,              S(qreal),               QFV.minValue(0.01),                 QRequired,  UPAdmin },
            { Fields::exrIsDefault,         S(bool),                QFV,                                false,      UPAdmin },
            { Fields::exrStatus,            ORM_STATUS_FIELD(Targoman::API::AccountModule::enuCurrencyExchangeRateStatus, Targoman::API::AccountModule::enuCurrencyExchangeRateStatus::Active) },
            { ORM_INVALIDATED_AT_FIELD },
            { Fields::exrCreationDateTime,  ORM_CREATED_ON },
            { Fields::exrCreatedBy_usrID,   ORM_CREATED_BY },
            { Fields::exrUpdatedBy_usrID,   ORM_UPDATED_BY },
        };

        const QList<stuRelation> Relations = {
            //Col                   Reference Table                 ForeignCol     Rename   LeftJoin
            ORM_RELATION_OF_CREATOR(Fields::exrCreatedBy_usrID),
            ORM_RELATION_OF_UPDATER(Fields::exrUpdatedBy_usrID),
        };

        const QList<stuDBIndex> Indexes = {
        };

    } //namespace Private

    TAPI_DEFINE_VARIANT_ENABLED_STRUCT(DTO,
        SF_ORM_PRIMARYKEY_32(exrID),
        SF_QString          (exrName),
        SF_QString          (exrSymbol),
        SF_qreal            (exrRate),
        SF_bool             (exrIsDefault),
        SF_ORM_STATUS_FIELD (exrStatus, Targoman::API::AccountModule::enuCurrencyExchangeRateStatus, Targoman::API::AccountModule::enuCurrencyExchangeRateStatus::Active),
        SF_ORM_CREATED_ON   (exrCreationDateTime),
        SF_ORM_CREATED_BY   (exrCreatedBy_usrID),
        SF_ORM_UPDATED_BY   (exrUpdatedBy_usrID)
    );

} //namespace tblCurrencyExchangeRate
#pragma GCC diagnostic pop

class CurrencyExchangeRate : public intfSQLBasedModule
{
    Q_OBJECT
    TARGOMAN_DEFINE_API_SUBMODULE(Account, CurrencyExchangeRate)

private slots:
    QVariant ANONYMOUSE_ORMGET("Get CurrencyExchangeRate information")
    quint64 ORMCREATE("Create a new CurrencyExchangeRate")
    bool ORMUPDATE("Update CurrencyExchangeRate info by priviledged user")
    bool ORMDELETE("Delete a CurrencyExchangeRate. Take note that default CurrencyExchangeRate can not be deleted")

    bool REST_UPDATE(
        setAsDefault,
        (
            APICALLBOOM_TYPE_JWT_DECL &APICALLBOOM_PARAM,
            quint32 _exrID
        ),
        "change default CurrencyExchangeRate"
    )

};

} //namespace ORM
} //namespace Targoman::API::AccountModule

#endif // TARGOMAN_API_MODULES_ACCOUNT_ORM_CURRENCYEXCHANGERATE_H
