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

#ifndef TARGOMAN_API_MODULES_MT_ORM_ACCOUNTING_H
#define TARGOMAN_API_MODULES_MT_ORM_ACCOUNTING_H

#include "ModuleHelpers/MT/Interfaces/intfMTAccounting.h"
//#include "Interfaces/AAA/Accounting_Defs.hpp"
//#include "Interfaces/AAA/AAA.hpp"
#include "../MTDefs.hpp"

using namespace Targoman::API::ModuleHelpers::MT::Interfaces;

namespace Targoman::API::MTModule {

//structures and enumes goes here

} //namespace Targoman::API::MTModule

namespace Targoman::API::MTModule {
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
        SF_tblAccountUnitsMTBase_DTO
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
        SF_tblAccountUnitsI18NMTBase_DTO
    );
}

namespace tblAccountProducts {

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
        SF_tblAccountProductsMTBase_DTO
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
        SF_tblAccountProductsI18NMTBase_DTO
    );
}

namespace tblAccountSaleables {

    namespace ExtraFields {
    }

    namespace ExtraRelation {
//        constexpr char AAA[] = "aaa";
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
        SF_tblAccountSaleablesMTBase_DTO
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
        SF_tblAccountSaleablesI18NMTBase_DTO
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
        SF_tblAccountSaleablesFilesMTBase_DTO
    );
}

namespace tblAccountUserAssets {

    namespace ExtraFields {
    }

    namespace ExtraRelation {
    //        constexpr char AAA[] = "aaa";
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
        SF_tblAccountUserAssetsMTBase_DTO
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
        SF_tblAccountUserAssetsFilesMTBase_DTO
    );
}

namespace tblAccountAssetUsage {

    namespace ExtraFields {
        //usgRemainingDays
    }

    namespace ExtraRelation {
    //        constexpr char AAA[] = "aaa";
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
        SF_tblAccountAssetUsageMTBase_DTO
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
        SF_tblAccountCouponsMTBase_DTO
    );
}

#pragma GCC diagnostic pop

/******************************************************/
/******************************************************/
/******************************************************/
class AccountUnits : public intfAccountUnitsMT
{
    Q_OBJECT
    TARGOMAN_API_SUBMODULE_DEFINE(MT, AccountUnits)

protected:
    QScopedPointer<intfAccountUnitsI18NMT> AccountUnitsI18N;
};

/******************************************************/
class AccountUnitsI18N : public intfAccountUnitsI18NMT
{
    Q_OBJECT
    TARGOMAN_API_SUBMODULE_DEFINE(MT, AccountUnitsI18N)

public:
};

/******************************************************/
/******************************************************/
/******************************************************/
class AccountProducts : public intfAccountProductsMT
{
    Q_OBJECT
    TARGOMAN_API_SUBMODULE_DEFINE(MT, AccountProducts)

protected:
    QScopedPointer<intfAccountProductsI18NMT> AccountProductsI18N;
};

/******************************************************/
class AccountProductsI18N : public intfAccountProductsI18NMT
{
    Q_OBJECT
    TARGOMAN_API_SUBMODULE_DEFINE(MT, AccountProductsI18N)

public:
};

/******************************************************/
/******************************************************/
/******************************************************/
class AccountSaleables : public intfAccountSaleablesMT
{
    Q_OBJECT
    TARGOMAN_API_SUBMODULE_DEFINE(MT, AccountSaleables)

protected:
    QScopedPointer<intfAccountSaleablesI18NMT> AccountSaleablesI18N;
};

/******************************************************/
class AccountSaleablesI18N : public intfAccountSaleablesI18NMT
{
    Q_OBJECT
    TARGOMAN_API_SUBMODULE_DEFINE(MT, AccountSaleablesI18N)

//public:
};

/******************************************************/
class AccountSaleablesFiles : public intfAccountSaleablesFilesMT
{
    Q_OBJECT
    TARGOMAN_API_SUBMODULE_DEFINE(MT, AccountSaleablesFiles)
};

/******************************************************/
/******************************************************/
/******************************************************/
class AccountUserAssets : public intfAccountUserAssetsMT<true>
{
    Q_OBJECT
    TARGOMAN_API_SUBMODULE_DEFINE(MT, AccountUserAssets)

//public:
};

/******************************************************/
class AccountUserAssetsFiles : public intfAccountUserAssetsFilesMT
{
    Q_OBJECT
    TARGOMAN_API_SUBMODULE_DEFINE(MT, AccountUserAssetsFiles)

//public:
};

/******************************************************/
/******************************************************/
/******************************************************/
class AccountAssetUsage : public intfAccountAssetUsageMT<true>
{
    Q_OBJECT
    TARGOMAN_API_SUBMODULE_DEFINE(MT, AccountAssetUsage)

public:
};

/******************************************************/
/******************************************************/
/******************************************************/
class AccountCoupons : public intfAccountCouponsMT
{
    Q_OBJECT
    TARGOMAN_API_SUBMODULE_DEFINE(MT, AccountCoupons)

public:
};

/******************************************************/
/******************************************************/
/******************************************************/
//There is no Prize in this module
/*
class AccountPrizes : public intfAccountPrizesMT
{
    Q_OBJECT
    TARGOMAN_API_SUBMODULE_DEFINE(MT,clsAccountPrizes)

private slots:
public:
};
*/

} //namespace ORM
} //namespace Targoman::API::MTModule

#endif // TARGOMAN_API_MODULES_MT_ORM_ACCOUNTING_H
