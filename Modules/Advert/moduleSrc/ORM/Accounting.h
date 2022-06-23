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

    namespace ExtraFields {
        TARGOMAN_CREATE_CONSTEXPR(prdExType);
        TARGOMAN_CREATE_CONSTEXPR(prdEx_locID);
        //prdExDuration
        TARGOMAN_CREATE_CONSTEXPR(prdExShowPerDay);
        TARGOMAN_CREATE_CONSTEXPR(prdExShowTotal);
        TARGOMAN_CREATE_CONSTEXPR(prdExClicksPerDay);
        TARGOMAN_CREATE_CONSTEXPR(prdExClicksPerMonth);
        TARGOMAN_CREATE_CONSTEXPR(prdExClicksTotal);
    }

    namespace ExtraRelation {
//        constexpr char AAA[] = "aaa";
    }

    namespace Private {
        const QList<clsORMField> ExtraORMFields = {
            ///ColName                                    Type        Validation                         Default    UpBy       Sort  Filter Self  Virt   PK
            { ExtraFields::prdExType,               S(Targoman::API::AdvertModule::enuProductType::Type), QFV, QRequired, UPOwner },
            { ExtraFields::prdEx_locID,             S(quint32), QFV.integer().minValue(1),         QRequired, UPOwner },
            { ExtraFields::prdExShowPerDay,         S(quint32), QFV.integer().minValue(-1),        -1,        UPOwner },
            { ExtraFields::prdExShowTotal,          S(quint64), QFV.integer().minValue(-1),        -1,        UPOwner },
            { ExtraFields::prdExClicksPerDay,       S(quint32), QFV.integer().minValue(-1),        -1,        UPOwner },
            { ExtraFields::prdExClicksPerMonth,     S(quint32), QFV.integer().minValue(-1),        -1,        UPOwner },
            { ExtraFields::prdExClicksTotal,        S(quint64), QFV.integer().minValue(-1),        -1,        UPOwner },
        };

        const QList<stuRelation> ExtraRelations = {
            ///Col                              Reference Table                      ForeignCol             Rename     LeftJoin
            { ExtraFields::prdEx_locID,   R(AdvertSchema, tblLocations::Name), tblLocations::Fields::locID },
        };

        const QList<stuDBIndex> ExtraIndexes = {
        };

    } //namespace Private

    TAPI_DEFINE_STRUCT(DTO,
        SF_tblAccountProductsBase_DTO,

        SF_Enum                     (prdExType, Targoman::API::AdvertModule::enuProductType, Targoman::API::AdvertModule::enuProductType::Advertise),
        SF_quint32                  (prdEx_locID),
        SF_quint32                  (prdExShowPerDay),
        SF_quint64                  (prdExShowTotal),
        SF_quint32                  (prdExClicksPerDay),
        SF_quint32                  (prdExClicksPerMonth),
        SF_quint64                  (prdExClicksTotal)
    );
}

namespace tblAccountSaleables {

    namespace ExtraFields {
        TARGOMAN_CREATE_CONSTEXPR(slbExShowPerDay);
        TARGOMAN_CREATE_CONSTEXPR(slbExShowTotal);
        TARGOMAN_CREATE_CONSTEXPR(slbExClicksPerDay);
        TARGOMAN_CREATE_CONSTEXPR(slbExClicksPerMonth);
        TARGOMAN_CREATE_CONSTEXPR(slbExClicksTotal);
    }

    namespace ExtraRelation {
//        constexpr char AAA[] = "aaa";
    }

    namespace Private {
        const QList<clsORMField> ExtraORMFields = {
            ///ColName                                     Type        Validation                  Default    UpBy       Sort  Filter Self  Virt   PK
            { ExtraFields::slbExShowPerDay,         S(quint32), QFV.integer().minValue(-1), -1,        UPOwner },
            { ExtraFields::slbExShowTotal,          S(quint64), QFV.integer().minValue(-1), -1,        UPOwner },
            { ExtraFields::slbExClicksPerDay,       S(quint32), QFV.integer().minValue(-1), -1,        UPOwner },
            { ExtraFields::slbExClicksPerMonth,     S(quint32), QFV.integer().minValue(-1), -1,        UPOwner },
            { ExtraFields::slbExClicksTotal,        S(quint64), QFV.integer().minValue(-1), -1,        UPOwner },
        };

        const QList<stuRelation> ExtraRelations = {
        };

        const QList<stuDBIndex> ExtraIndexes = {
        };

    } //namespace Private

    TAPI_DEFINE_STRUCT(DTO,
        SF_tblAccountSaleablesBase_DTO,

        SF_quint32                  (slbExShowPerDay),
        SF_quint64                  (slbExShowTotal),
        SF_quint32                  (slbExClicksPerDay),
        SF_quint32                  (slbExClicksPerMonth),
        SF_quint64                  (slbExClicksTotal)
    );
}

namespace tblAccountUserAsset {

    namespace ExtraFields {
        TARGOMAN_CREATE_CONSTEXPR(uasExDays);
        TARGOMAN_CREATE_CONSTEXPR(uasExDayShow);
        TARGOMAN_CREATE_CONSTEXPR(uasExTotalShow);
        TARGOMAN_CREATE_CONSTEXPR(uasExDayClicks);
        TARGOMAN_CREATE_CONSTEXPR(uasExMonthClicks);
        TARGOMAN_CREATE_CONSTEXPR(uasExTotalClicks);
    }

    namespace ExtraRelation {
    //        constexpr char AAA[] = "aaa";
    }

    namespace Private {
        const QList<clsORMField> ExtraORMFields = {
            //ColName                           Type        Validation                  Default     UpBy    Sort    Filter Self  Virt   PK
            { ExtraFields::uasExDays,           S(qint16),  QFV.integer().minValue(-1), 0,          UPAdmin, false,  false },
            { ExtraFields::uasExDayShow,        S(quint32), QFV.integer().minValue(0),  0,          UPAdmin, false,  false },
            { ExtraFields::uasExTotalShow,      S(quint64), QFV.integer().minValue(0),  0,          UPAdmin, false,  false },
            { ExtraFields::uasExDayClicks,      S(quint32), QFV.integer().minValue(0),  0,          UPAdmin, false,  false },
            { ExtraFields::uasExMonthClicks,    S(quint32), QFV.integer().minValue(0),  0,          UPAdmin, false,  false },
            { ExtraFields::uasExTotalClicks,    S(quint64), QFV.integer().minValue(0),  0,          UPAdmin, false,  false }
        };

        const QList<stuRelation> ExtraRelations = {
        };

        const QList<stuDBIndex> ExtraIndexes = {
        };

    } //namespace Private

    TAPI_DEFINE_STRUCT(DTO,
        SF_tblAccountUserAssetsBase_DTO,

        SF_qint16                   (uasExDays),
        SF_quint32                  (uasExDayShow),
        SF_quint64                  (uasExTotalShow),
        SF_quint32                  (uasExDayClicks),
        SF_quint32                  (uasExMonthClicks),
        SF_quint64                  (uasExTotalClicks)
    );
}

namespace tblAccountAssetUsage {

    namespace ExtraFields {
        //usgExRemainingDays
        TARGOMAN_CREATE_CONSTEXPR(usgExDays);
        TARGOMAN_CREATE_CONSTEXPR(usgExDayShow);
        TARGOMAN_CREATE_CONSTEXPR(usgExTotalShow);
        TARGOMAN_CREATE_CONSTEXPR(usgExDayClicks);
        TARGOMAN_CREATE_CONSTEXPR(usgExMonthClicks);
        TARGOMAN_CREATE_CONSTEXPR(usgExTotalClicks);
    }

    namespace ExtraRelation {
    //        constexpr char AAA[] = "aaa";
    }

    namespace Private {
        const QList<clsORMField> ExtraORMFields = {
            //ColName                           Type        Validation                  Default     UpBy    Sort    Filter Self  Virt   PK
            { ExtraFields::usgExDays,           S(qint16),  QFV.integer().minValue(-1), 0,          UPAdmin, false,  false },
            { ExtraFields::usgExDayShow,        S(quint32), QFV.integer().minValue(0),  0,          UPAdmin, false,  false },
            { ExtraFields::usgExTotalShow,      S(quint64), QFV.integer().minValue(0),  0,          UPAdmin, false,  false },
            { ExtraFields::usgExDayClicks,      S(quint32), QFV.integer().minValue(0),  0,          UPAdmin, false,  false },
            { ExtraFields::usgExMonthClicks,    S(quint32), QFV.integer().minValue(0),  0,          UPAdmin, false,  false },
            { ExtraFields::usgExTotalClicks,    S(quint64), QFV.integer().minValue(0),  0,          UPAdmin, false,  false }
        };

        const QList<stuRelation> ExtraRelations = {
        };

        const QList<stuDBIndex> ExtraIndexes = {
        };

    } //namespace Private

    TAPI_DEFINE_STRUCT(DTO,
        SF_tblAccountAssetUsageBase_DTO,

        SF_qint16                   (usgExDays),
        SF_quint32                  (usgExDayShow),
        SF_quint64                  (usgExTotalShow),
        SF_quint32                  (usgExDayClicks),
        SF_quint32                  (usgExMonthClicks),
        SF_quint64                  (usgExTotalClicks)
    );
}

namespace tblAccountCoupons {

    namespace ExtraFields {
    }

    namespace ExtraRelation {
    //        constexpr char AAA[] = "aaa";
    }

    namespace Private {
        const QList<clsORMField> ExtraORMFields = {
            ///ColName                  Type        Validation                  Default    UpBy       Sort  Filter Self  Virt   PK
        };

        const QList<stuRelation> ExtraRelations = {
        };

        const QList<stuDBIndex> ExtraIndexes = {
        };

    } //namespace Private

    TAPI_DEFINE_STRUCT(DTO,
        SF_tblAccountCouponsBase_DTO
    );
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
