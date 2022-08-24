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

#ifndef TARGOMAN_API_MODULES_MTSHOP_ORM_ACCOUNTING_H
#define TARGOMAN_API_MODULES_MTSHOP_ORM_ACCOUNTING_H

#include "Interfaces/AAA/Accounting_Defs.hpp"
#include "Interfaces/AAA/AAA.hpp"
#include "../MTShopDefs.hpp"

namespace Targoman::API::MTShopModule {

//structures and enumes goes here

} //namespace Targoman::API::MTShopModule

namespace Targoman::API::MTShopModule {
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
        SF_tblAccountProductsBase_DTO
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
        SF_tblAccountSaleablesBase_DTO
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
        SF_tblAccountUserAssetsBase_DTO
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
        SF_tblAccountAssetUsageBase_DTO
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
    Q_OBJECT
    TARGOMAN_API_SUBMODULE_DEFINE(MTShop, AccountUnits)

protected:
    QScopedPointer<intfAccountUnitsI18N> AccountUnitsI18N;
};

/******************************************************/
class AccountUnitsI18N : public intfAccountUnitsI18N
{
    Q_OBJECT
    TARGOMAN_API_SUBMODULE_DEFINE(MTShop, AccountUnitsI18N)

public:
};

/******************************************************/
/******************************************************/
/******************************************************/
class AccountProducts : public intfAccountProducts
{
    Q_OBJECT
    TARGOMAN_API_SUBMODULE_DEFINE(MTShop, AccountProducts)

protected:
    QScopedPointer<intfAccountProductsI18N> AccountProductsI18N;
};

/******************************************************/
class AccountProductsI18N : public intfAccountProductsI18N
{
    Q_OBJECT
    TARGOMAN_API_SUBMODULE_DEFINE(MTShop, AccountProductsI18N)

public:
};

/******************************************************/
/******************************************************/
/******************************************************/
class AccountSaleables : public intfAccountSaleables
{
    Q_OBJECT
    TARGOMAN_API_SUBMODULE_DEFINE(MTShop, AccountSaleables)

protected:
    QScopedPointer<intfAccountSaleablesI18N> AccountSaleablesI18N;
};

/******************************************************/
class AccountSaleablesI18N : public intfAccountSaleablesI18N
{
    Q_OBJECT
    TARGOMAN_API_SUBMODULE_DEFINE(MTShop, AccountSaleablesI18N)

//public:
};

/******************************************************/
class AccountSaleablesFiles : public intfAccountSaleablesFiles
{
    Q_OBJECT
    TARGOMAN_API_SUBMODULE_DEFINE(MTShop, AccountSaleablesFiles)
};

/******************************************************/
/******************************************************/
/******************************************************/
class AccountUserAssets : public intfAccountUserAssets
{
    Q_OBJECT
    TARGOMAN_API_SUBMODULE_DEFINE(MTShop, AccountUserAssets)

//public:
};

/******************************************************/
class AccountUserAssetsFiles : public intfAccountUserAssetsFiles
{
    Q_OBJECT
    TARGOMAN_API_SUBMODULE_DEFINE(MTShop, AccountUserAssetsFiles)

//public:
};

/******************************************************/
/******************************************************/
/******************************************************/
class AccountAssetUsage : public intfAccountAssetUsage
{
    Q_OBJECT
    TARGOMAN_API_SUBMODULE_DEFINE(MTShop, AccountAssetUsage)

public:
};

/******************************************************/
/******************************************************/
/******************************************************/
class AccountCoupons : public intfAccountCoupons
{
    Q_OBJECT
    TARGOMAN_API_SUBMODULE_DEFINE(MTShop, AccountCoupons)

public:
};

/******************************************************/
/******************************************************/
/******************************************************/
//There is no Prize in this module
/*class clsAccountPrizes: public Accounting::intfAccountPrizes
{
    Q_OBJECT
    TARGOMAN_API_SUBMODULE_DEFINE(MTShop,clsAccountPrizes)

private slots:
public:
};
*/

} //namespace ORM
} //namespace Targoman::API::MTShopModule

#endif // TARGOMAN_API_MODULES_MTSHOP_ORM_ACCOUNTING_H
