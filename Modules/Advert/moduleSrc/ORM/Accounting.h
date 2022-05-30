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

#ifndef TARGOMAN_API_MODULES_ADVERT_ORM_ACCOUNTING_H
#define TARGOMAN_API_MODULES_ADVERT_ORM_ACCOUNTING_H

#include "Interfaces/AAA/Accounting_Defs.hpp"
#include "Interfaces/AAA/AAA.hpp"
#include "Defs.hpp"
#include "Locations.h"

namespace Targoman::API::AdvertModule {

//structures and enumes goes here

TARGOMAN_DEFINE_ENUM (enuProductType,
                      Advertise = 'A',
                      Reportage = 'R',
                      );

} //namespace Targoman::API::AdvertModule

TAPI_DECLARE_METATYPE_ENUM(Targoman::API::AdvertModule, enuProductType);

namespace Targoman::API::AdvertModule {
namespace ORM {

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-variable"

namespace tblAccountProducts {

    namespace Fields {
        TARGOMAN_CREATE_CONSTEXPR(prdType);
        TARGOMAN_CREATE_CONSTEXPR(prd_locID);
        TARGOMAN_CREATE_CONSTEXPR(prdShowPerDay);
        TARGOMAN_CREATE_CONSTEXPR(prdShowTotal);
        TARGOMAN_CREATE_CONSTEXPR(prdClicksPerDay);
        TARGOMAN_CREATE_CONSTEXPR(prdClicksPerMonth);
        TARGOMAN_CREATE_CONSTEXPR(prdClicksTotal);
    }

    namespace Relation {
//        constexpr char AAA[] = "aaa";
    }

    namespace Private {
        const QList<clsORMField> ORMFields = {
            ///< ColName                                    Type        Validation                         Default    UpBy       Sort  Filter Self  Virt   PK
            { Fields::prdType,               S(Targoman::API::AdvertModule::enuProductType::Type), QFV, QRequired, UPOwner },
            { Fields::prd_locID,             S(quint32), QFV.integer().minValue(1),         QRequired, UPOwner },
            { Fields::prdShowPerDay,         S(quint32), QFV.integer().minValue(-1),        -1,        UPOwner },
            { Fields::prdShowTotal,          S(quint64), QFV.integer().minValue(-1),        -1,        UPOwner },
            { Fields::prdClicksPerDay,       S(quint32), QFV.integer().minValue(-1),        -1,        UPOwner },
            { Fields::prdClicksPerMonth,     S(quint32), QFV.integer().minValue(-1),        -1,        UPOwner },
            { Fields::prdClicksTotal,        S(quint64), QFV.integer().minValue(-1),        -1,        UPOwner },
        };

        const QList<stuRelation> Relations = {
            ///< Col                              Reference Table                      ForeignCol             Rename     LeftJoin
            { Fields::prd_locID,   R(AdvertSchema, tblLocations::Name), tblLocations::Fields::locID },
        };

        const QList<stuDBIndex> Indexes = {
        };

    } //namespace Private

    TAPI_DEFINE_VARIANT_ENABLED_STRUCT(DTO,
        SF_tblAccountProductsBase_DTO,

        SF_Enum                     (prdType, Targoman::API::AdvertModule::enuProductType, Targoman::API::AdvertModule::enuProductType::Advertise),
        SF_quint32                  (prd_locID),
        SF_quint32                  (prdShowPerDay),
        SF_quint64                  (prdShowTotal),
        SF_quint32                  (prdClicksPerDay),
        SF_quint32                  (prdClicksPerMonth),
        SF_quint64                  (prdClicksTotal)
    );
}

namespace tblAccountSaleables {

    namespace Fields {
        TARGOMAN_CREATE_CONSTEXPR(slbShowPerDay);
        TARGOMAN_CREATE_CONSTEXPR(slbShowTotal);
        TARGOMAN_CREATE_CONSTEXPR(slbClicksPerDay);
        TARGOMAN_CREATE_CONSTEXPR(slbClicksPerMonth);
        TARGOMAN_CREATE_CONSTEXPR(slbClicksTotal);
    }

    namespace Relation {
//        constexpr char AAA[] = "aaa";
    }

    namespace Private {
        const QList<clsORMField> ORMFields = {
            ///< ColName                                     Type        Validation                  Default    UpBy       Sort  Filter Self  Virt   PK
            { Fields::slbShowPerDay,         S(quint32), QFV.integer().minValue(-1), -1,        UPOwner },
            { Fields::slbShowTotal,          S(quint64), QFV.integer().minValue(-1), -1,        UPOwner },
            { Fields::slbClicksPerDay,       S(quint32), QFV.integer().minValue(-1), -1,        UPOwner },
            { Fields::slbClicksPerMonth,     S(quint32), QFV.integer().minValue(-1), -1,        UPOwner },
            { Fields::slbClicksTotal,        S(quint64), QFV.integer().minValue(-1), -1,        UPOwner },
        };

        const QList<stuRelation> Relations = {
        };

        const QList<stuDBIndex> Indexes = {
        };

    } //namespace Private

    TAPI_DEFINE_VARIANT_ENABLED_STRUCT(DTO,
        SF_tblAccountSaleablesBase_DTO,

        SF_quint32                  (slbShowPerDay),
        SF_quint64                  (slbShowTotal),
        SF_quint32                  (slbClicksPerDay),
        SF_quint32                  (slbClicksPerMonth),
        SF_quint64                  (slbClicksTotal)
    );
}

namespace AccountAsset {
    TARGOMAN_CREATE_CONSTEXPR(Days);
    TARGOMAN_CREATE_CONSTEXPR(DayShow);
    TARGOMAN_CREATE_CONSTEXPR(TotalShow);
    TARGOMAN_CREATE_CONSTEXPR(DayClicks);
    TARGOMAN_CREATE_CONSTEXPR(MonthClicks);
    TARGOMAN_CREATE_CONSTEXPR(TotalClicks);
}

///<  ColName                                                    Type        Validation                  Default  UpBy    Sort   Filter Self  Virt   PK
#define ADVERT_DEFINE_ASSET_FIELDS(_prefix) \
    { QString("%1%2").arg(_prefix, AccountAsset::Days),          S(qint16),  QFV.integer().minValue(-1), 0,       UPNone, false, false }, \
    { QString("%1%2").arg(_prefix, AccountAsset::DayShow),       S(quint32), QFV.integer().minValue(0),  0,       UPNone, false, false }, \
    { QString("%1%2").arg(_prefix, AccountAsset::TotalShow),     S(quint64), QFV.integer().minValue(0),  0,       UPNone, false, false }, \
    { QString("%1%2").arg(_prefix, AccountAsset::DayClicks),     S(quint32), QFV.integer().minValue(0),  0,       UPNone, false, false }, \
    { QString("%1%2").arg(_prefix, AccountAsset::MonthClicks),   S(quint32), QFV.integer().minValue(0),  0,       UPNone, false, false }, \
    { QString("%1%2").arg(_prefix, AccountAsset::TotalClicks),   S(quint64), QFV.integer().minValue(0),  0,       UPNone, false, false } \

namespace tblAccountUserAsset {
//Extra fields goes here
}

#pragma GCC diagnostic pop

/******************************************************/
class AccountProducts : public intfAccountProducts
{
    Q_OBJECT
    TARGOMAN_DEFINE_API_SUBMODULE(Advert, AccountProducts)

public:
};

/******************************************************/
class AccountSaleables : public intfAccountSaleables
{
    Q_OBJECT
    TARGOMAN_DEFINE_API_SUBMODULE(Advert, AccountSaleables)

public:
};

/******************************************************/
class AccountUserAssets : public intfAccountUserAssets
{
    Q_OBJECT
    TARGOMAN_DEFINE_API_SUBMODULE(Advert, AccountUserAssets)

public:
};

/******************************************************/
class AccountAssetUsage : public intfAccountAssetUsage
{
    Q_OBJECT
    TARGOMAN_DEFINE_API_SUBMODULE(Advert, AccountAssetUsage)

public:
};

/******************************************************/
class AccountCoupons : public intfAccountCoupons
{
    Q_OBJECT
    TARGOMAN_DEFINE_API_SUBMODULE(Advert, AccountCoupons)

public:
};

/******************************************************/
//There is no Prize in the advertisement module
/*class clsAccountPrizes: public Accounting::intfAccountPrizes
{
    Q_OBJECT
    TARGOMAN_DEFINE_API_SUBMODULE(Advert,clsAccountPrizes)

private slots:
public:
};
*/

} //namespace ORM
} //namespace Targoman::API::AdvertModule

#endif // TARGOMAN_API_MODULES_ADVERT_ORM_ACCOUNTING_H
