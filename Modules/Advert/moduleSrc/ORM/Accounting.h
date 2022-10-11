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

namespace tblAccountUnits {

    namespace ExtraFields {
    }

    namespace ExtraRelation {
//        constexpr char AAA[] = "aaa";
    }

    namespace Private {
        const QList<clsORMField> ExtraORMFields = {
            ///ColName                                    Type        Validation                         Default    UpBy       Sort  Filter Self  Virt   PK
        };

        const QList<stuRelation> ExtraRelations = {
            ///Col                              Reference Table                      ForeignCol             Rename     LeftJoin
        };

        const QList<stuDBIndex> ExtraIndexes = {
        };

    } //namespace Private

    TAPI_DEFINE_STRUCT(DTO,
        SF_tblAccountUnitsBase_DTO
    );
}

namespace tblAccountUnitsI18N {

    namespace ExtraFields {
    }

    namespace ExtraRelation {
//        constexpr char AAA[] = "aaa";
    }

    namespace Private {
        const QList<clsORMField> ExtraORMFields = {
            ///ColName                                    Type        Validation                         Default    UpBy       Sort  Filter Self  Virt   PK
        };

        const QList<stuRelation> ExtraRelations = {
            ///Col                              Reference Table                      ForeignCol             Rename     LeftJoin
        };

        const QList<stuDBIndex> ExtraIndexes = {
        };

    } //namespace Private

    TAPI_DEFINE_STRUCT(DTO,
        SF_tblAccountUnitsI18NBase_DTO
    );
}

namespace tblAccountProducts {

    namespace ExtraFields {
        TARGOMAN_CREATE_CONSTEXPR(prdType);
        TARGOMAN_CREATE_CONSTEXPR(prd_locID);
        //prdDuration
        TARGOMAN_CREATE_CONSTEXPR(prdShowPerDay);
        TARGOMAN_CREATE_CONSTEXPR(prdShowTotal);
        TARGOMAN_CREATE_CONSTEXPR(prdClicksPerDay);
        TARGOMAN_CREATE_CONSTEXPR(prdClicksPerMonth);
        TARGOMAN_CREATE_CONSTEXPR(prdClicksTotal);
    }

    namespace ExtraRelation {
//        constexpr char AAA[] = "aaa";
    }

    namespace Private {
        const QList<clsORMField> ExtraORMFields = {
            ///ColName                                    Type        Validation                         Default    UpBy       Sort  Filter Self  Virt   PK
            { ExtraFields::prdType,               S(Targoman::API::AdvertModule::enuProductType::Type), QFV, QRequired, UPOwner },
            { ExtraFields::prd_locID,             S(quint32), QFV.integer().minValue(1),         QRequired, UPOwner },
            { ExtraFields::prdShowPerDay,         S(quint32), QFV.integer().minValue(-1),        -1,        UPOwner },
            { ExtraFields::prdShowTotal,          S(quint64), QFV.integer().minValue(-1),        -1,        UPOwner },
            { ExtraFields::prdClicksPerDay,       S(quint32), QFV.integer().minValue(-1),        -1,        UPOwner },
            { ExtraFields::prdClicksPerMonth,     S(quint32), QFV.integer().minValue(-1),        -1,        UPOwner },
            { ExtraFields::prdClicksTotal,        S(quint64), QFV.integer().minValue(-1),        -1,        UPOwner },
        };

        const QList<stuRelation> ExtraRelations = {
            ///Col                              Reference Table                      ForeignCol             Rename     LeftJoin
            { ExtraFields::prd_locID,   R(AdvertSchema, tblLocations::Name), tblLocations::Fields::locID },
        };

        const QList<stuDBIndex> ExtraIndexes = {
        };

    } //namespace Private

    TAPI_DEFINE_STRUCT(DTO,
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

namespace tblAccountProductsI18N {

    namespace ExtraFields {
    }

    namespace ExtraRelation {
//        constexpr char AAA[] = "aaa";
    }

    namespace Private {
        const QList<clsORMField> ExtraORMFields = {
            ///ColName                                    Type        Validation                         Default    UpBy       Sort  Filter Self  Virt   PK
        };

        const QList<stuRelation> ExtraRelations = {
            ///Col                              Reference Table                      ForeignCol             Rename     LeftJoin
        };

        const QList<stuDBIndex> ExtraIndexes = {
        };

    } //namespace Private

    TAPI_DEFINE_STRUCT(DTO,
        SF_tblAccountProductsI18NBase_DTO
    );
}

namespace tblAccountSaleables {

    namespace ExtraFields {
        TARGOMAN_CREATE_CONSTEXPR(slbShowPerDay);
        TARGOMAN_CREATE_CONSTEXPR(slbShowTotal);
        TARGOMAN_CREATE_CONSTEXPR(slbClicksPerDay);
        TARGOMAN_CREATE_CONSTEXPR(slbClicksPerMonth);
        TARGOMAN_CREATE_CONSTEXPR(slbClicksTotal);
    }

    namespace ExtraRelation {
//        constexpr char AAA[] = "aaa";
    }

    namespace Private {
        const QList<clsORMField> ExtraORMFields = {
            ///ColName                                     Type        Validation                  Default    UpBy       Sort  Filter Self  Virt   PK
            { ExtraFields::slbShowPerDay,         S(quint32), QFV.integer().minValue(-1), -1,        UPOwner },
            { ExtraFields::slbShowTotal,          S(quint64), QFV.integer().minValue(-1), -1,        UPOwner },
            { ExtraFields::slbClicksPerDay,       S(quint32), QFV.integer().minValue(-1), -1,        UPOwner },
            { ExtraFields::slbClicksPerMonth,     S(quint32), QFV.integer().minValue(-1), -1,        UPOwner },
            { ExtraFields::slbClicksTotal,        S(quint64), QFV.integer().minValue(-1), -1,        UPOwner },
        };

        const QList<stuRelation> ExtraRelations = {
        };

        const QList<stuDBIndex> ExtraIndexes = {
        };

    } //namespace Private

    TAPI_DEFINE_STRUCT(DTO,
        SF_tblAccountSaleablesBase_DTO,

        SF_quint32                  (slbShowPerDay),
        SF_quint64                  (slbShowTotal),
        SF_quint32                  (slbClicksPerDay),
        SF_quint32                  (slbClicksPerMonth),
        SF_quint64                  (slbClicksTotal)
    );
}

namespace tblAccountSaleablesI18N {

    namespace ExtraFields {
    }

    namespace ExtraRelation {
//        constexpr char AAA[] = "aaa";
    }

    namespace Private {
        const QList<clsORMField> ExtraORMFields = {
            ///ColName                                    Type        Validation                         Default    UpBy       Sort  Filter Self  Virt   PK
        };

        const QList<stuRelation> ExtraRelations = {
            ///Col                              Reference Table                      ForeignCol             Rename     LeftJoin
        };

        const QList<stuDBIndex> ExtraIndexes = {
        };

    } //namespace Private

    TAPI_DEFINE_STRUCT(DTO,
        SF_tblAccountSaleablesI18NBase_DTO
    );
}

namespace tblAccountSaleablesFiles {

    namespace ExtraFields {
    }

    namespace ExtraRelation {
    }

    namespace Private {
        const QList<clsORMField> ExtraORMFields = {
            ///ColName                                     Type        Validation                  Default    UpBy       Sort  Filter Self  Virt   PK
        };

        const QList<stuRelation> ExtraRelations = {
        };

        const QList<stuDBIndex> ExtraIndexes = {
        };

    } //namespace Private

    TAPI_DEFINE_STRUCT(DTO,
        SF_tblAccountSaleablesFilesBase_DTO
    );
}

namespace tblAccountUserAssets {

    namespace ExtraFields {
        TARGOMAN_CREATE_CONSTEXPR(uasDays);
        TARGOMAN_CREATE_CONSTEXPR(uasDayShow);
        TARGOMAN_CREATE_CONSTEXPR(uasTotalShow);
        TARGOMAN_CREATE_CONSTEXPR(uasDayClicks);
        TARGOMAN_CREATE_CONSTEXPR(uasMonthClicks);
        TARGOMAN_CREATE_CONSTEXPR(uasTotalClicks);
    }

    namespace ExtraRelation {
    //        constexpr char AAA[] = "aaa";
    }

    namespace Private {
        const QList<clsORMField> ExtraORMFields = {
            //ColName                           Type        Validation                  Default     UpBy    Sort    Filter Self  Virt   PK
            { ExtraFields::uasDays,           S(qint16),  QFV.integer().minValue(-1), 0,          UPAdmin, false,  false },
            { ExtraFields::uasDayShow,        S(quint32), QFV.integer().minValue(0),  0,          UPAdmin, false,  false },
            { ExtraFields::uasTotalShow,      S(quint64), QFV.integer().minValue(0),  0,          UPAdmin, false,  false },
            { ExtraFields::uasDayClicks,      S(quint32), QFV.integer().minValue(0),  0,          UPAdmin, false,  false },
            { ExtraFields::uasMonthClicks,    S(quint32), QFV.integer().minValue(0),  0,          UPAdmin, false,  false },
            { ExtraFields::uasTotalClicks,    S(quint64), QFV.integer().minValue(0),  0,          UPAdmin, false,  false }
        };

        const QList<stuRelation> ExtraRelations = {
        };

        const QList<stuDBIndex> ExtraIndexes = {
        };

    } //namespace Private

    TAPI_DEFINE_STRUCT(DTO,
        SF_tblAccountUserAssetsBase_DTO,

        SF_qint16                   (uasDays),
        SF_quint32                  (uasDayShow),
        SF_quint64                  (uasTotalShow),
        SF_quint32                  (uasDayClicks),
        SF_quint32                  (uasMonthClicks),
        SF_quint64                  (uasTotalClicks)
    );
}

namespace tblAccountUserAssetsFiles {

    namespace ExtraFields {
    }

    namespace ExtraRelation {
    }

    namespace Private {
        const QList<clsORMField> ExtraORMFields = {
            //ColName                           Type        Validation                  Default     UpBy    Sort    Filter Self  Virt   PK
        };

        const QList<stuRelation> ExtraRelations = {
        };

        const QList<stuDBIndex> ExtraIndexes = {
        };

    } //namespace Private

    TAPI_DEFINE_STRUCT(DTO,
        SF_tblAccountUserAssetsFilesBase_DTO
    );
}

namespace tblAccountAssetUsage {

    namespace ExtraFields {
        //usgRemainingDays
        TARGOMAN_CREATE_CONSTEXPR(usgDays);
        TARGOMAN_CREATE_CONSTEXPR(usgDayShow);
        TARGOMAN_CREATE_CONSTEXPR(usgTotalShow);
        TARGOMAN_CREATE_CONSTEXPR(usgDayClicks);
        TARGOMAN_CREATE_CONSTEXPR(usgMonthClicks);
        TARGOMAN_CREATE_CONSTEXPR(usgTotalClicks);
    }

    namespace ExtraRelation {
    //        constexpr char AAA[] = "aaa";
    }

    namespace Private {
        const QList<clsORMField> ExtraORMFields = {
            //ColName                           Type        Validation                  Default     UpBy    Sort    Filter Self  Virt   PK
            { ExtraFields::usgDays,           S(qint16),  QFV.integer().minValue(-1), 0,          UPAdmin, false,  false },
            { ExtraFields::usgDayShow,        S(quint32), QFV.integer().minValue(0),  0,          UPAdmin, false,  false },
            { ExtraFields::usgTotalShow,      S(quint64), QFV.integer().minValue(0),  0,          UPAdmin, false,  false },
            { ExtraFields::usgDayClicks,      S(quint32), QFV.integer().minValue(0),  0,          UPAdmin, false,  false },
            { ExtraFields::usgMonthClicks,    S(quint32), QFV.integer().minValue(0),  0,          UPAdmin, false,  false },
            { ExtraFields::usgTotalClicks,    S(quint64), QFV.integer().minValue(0),  0,          UPAdmin, false,  false }
        };

        const QList<stuRelation> ExtraRelations = {
        };

        const QList<stuDBIndex> ExtraIndexes = {
        };

    } //namespace Private

    TAPI_DEFINE_STRUCT(DTO,
        SF_tblAccountAssetUsageBase_DTO,

        SF_qint16                   (usgDays),
        SF_quint32                  (usgDayShow),
        SF_quint64                  (usgTotalShow),
        SF_quint32                  (usgDayClicks),
        SF_quint32                  (usgMonthClicks),
        SF_quint64                  (usgTotalClicks)
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
/******************************************************/
/******************************************************/
class AccountUnits : public intfAccountUnits
{
//    Q _OBJECT
    TARGOMAN_API_SUBMODULE_DEFINE(Advert, AccountUnits)

protected:
    QScopedPointer<intfAccountUnitsI18N> AccountUnitsI18N;
};

/******************************************************/
class AccountUnitsI18N : public intfAccountUnitsI18N
{
//    Q _OBJECT
    TARGOMAN_API_SUBMODULE_DEFINE(Advert, AccountUnitsI18N)

public:
};

/******************************************************/
/******************************************************/
/******************************************************/
class AccountProducts : public intfAccountProducts
{
//    Q _OBJECT
    TARGOMAN_API_SUBMODULE_DEFINE(Advert, AccountProducts)

protected:
    QScopedPointer<intfAccountProductsI18N> AccountProductsI18N;
};

/******************************************************/
class AccountProductsI18N : public intfAccountProductsI18N
{
//    Q _OBJECT
    TARGOMAN_API_SUBMODULE_DEFINE(Advert, AccountProductsI18N)

public:
};

/******************************************************/
/******************************************************/
/******************************************************/
class AccountSaleables : public intfAccountSaleables
{
//    Q _OBJECT
    TARGOMAN_API_SUBMODULE_DEFINE(Advert, AccountSaleables)

protected:
    QScopedPointer<intfAccountSaleablesI18N> AccountSaleablesI18N;
};

/******************************************************/
class AccountSaleablesI18N : public intfAccountSaleablesI18N
{
//    Q _OBJECT
    TARGOMAN_API_SUBMODULE_DEFINE(Advert, AccountSaleablesI18N)

//public:
};

/******************************************************/
class AccountSaleablesFiles : public intfAccountSaleablesFiles
{
//    Q _OBJECT
    TARGOMAN_API_SUBMODULE_DEFINE(Advert, AccountSaleablesFiles)
};

/******************************************************/
/******************************************************/
/******************************************************/
class AccountUserAssets : public intfAccountUserAssets<false>
{
//    Q _OBJECT
    TARGOMAN_API_SUBMODULE_DEFINE(Advert, AccountUserAssets)

//public:
};

/******************************************************/
class AccountUserAssetsFiles : public intfAccountUserAssetsFiles
{
//    Q _OBJECT
    TARGOMAN_API_SUBMODULE_DEFINE(Advert, AccountUserAssetsFiles)

//public:
};

/******************************************************/
/******************************************************/
/******************************************************/
class AccountAssetUsage : public intfAccountAssetUsage<false>
{
//    Q _OBJECT
    TARGOMAN_API_SUBMODULE_DEFINE(Advert, AccountAssetUsage)

public:
};

/******************************************************/
/******************************************************/
/******************************************************/
class AccountCoupons : public intfAccountCoupons
{
//    Q _OBJECT
    TARGOMAN_API_SUBMODULE_DEFINE(Advert, AccountCoupons)

public:
};

/******************************************************/
/******************************************************/
/******************************************************/
//There is no Prize in the advertisement module
/*
class clsAccountPrizes: public intfAccountPrizes
{
    Q_OBJECT
    TARGOMAN_API_SUBMODULE_DEFINE(Advert, clsAccountPrizes)

private slots:
public:
};
*/

} //namespace ORM
} //namespace Targoman::API::AdvertModule

#endif // TARGOMAN_API_MODULES_ADVERT_ORM_ACCOUNTING_H
