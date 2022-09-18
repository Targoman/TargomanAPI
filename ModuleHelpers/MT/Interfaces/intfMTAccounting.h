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

#ifndef TARGOMAN_API_MODULEHELPERS_MT_INTERFACES_INTFMTACCOUNTING_H
#define TARGOMAN_API_MODULEHELPERS_MT_INTERFACES_INTFMTACCOUNTING_H

#include "Interfaces/AAA/Accounting_Defs.hpp"
#include "Interfaces/AAA/AAA.hpp"

namespace Targoman::API::ModuleHelpers::MT::Interfaces {

/******************************************************/
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-variable"

namespace tblAccountUnitsMTBase {

    namespace ExtraFields {
    }

    namespace ExtraRelation {
//        constexpr char AAA[] = "aaa";
    }

    namespace Private {
        const QList<clsORMField> ExtraORMFields = {
            ///ColName                                    Type        Validation                         Default    UpBy       Sort  Filter Self  Virt   PK
        };

        inline const QList<stuRelation> ExtraRelations(Q_DECL_UNUSED const QString& _schema) {
            ///Col                              Reference Table                      ForeignCol             Rename     LeftJoin
            return { };
        };

        const QList<stuDBIndex> ExtraIndexes = {
        };

    } //namespace Private

#define SF_tblAccountUnitsMTBase_DTO \
    SF_tblAccountUnitsBase_DTO

    TAPI_DEFINE_STRUCT(DTO,
        SF_tblAccountUnitsMTBase_DTO
    );
}

namespace tblAccountUnitsI18NMTBase {

    namespace ExtraFields {
    }

    namespace ExtraRelation {
//        constexpr char AAA[] = "aaa";
    }

    namespace Private {
        const QList<clsORMField> ExtraORMFields = {
            ///ColName                                    Type        Validation                         Default    UpBy       Sort  Filter Self  Virt   PK
        };

        inline const QList<stuRelation> ExtraRelations(Q_DECL_UNUSED const QString& _schema) {
            ///Col                              Reference Table                      ForeignCol             Rename     LeftJoin
            return { };
        };

        const QList<stuDBIndex> ExtraIndexes = {
        };

    } //namespace Private

#define SF_tblAccountUnitsI18NMTBase_DTO \
    SF_tblAccountUnitsI18NBase_DTO

    TAPI_DEFINE_STRUCT(DTO,
        SF_tblAccountUnitsI18NMTBase_DTO
    );
}

namespace tblAccountProductsMTBase {

    namespace ExtraFields {
    }

    namespace ExtraRelation {
//        constexpr char AAA[] = "aaa";
    }

    namespace Private {
        const QList<clsORMField> ExtraORMFields = {
            ///ColName                                    Type        Validation                         Default    UpBy       Sort  Filter Self  Virt   PK
        };

        inline const QList<stuRelation> ExtraRelations(Q_DECL_UNUSED const QString& _schema) {
            ///Col                              Reference Table                      ForeignCol             Rename     LeftJoin
            return { };
        };

        const QList<stuDBIndex> ExtraIndexes = {
        };

    } //namespace Private

#define SF_tblAccountProductsMTBase_DTO \
    SF_tblAccountProductsBase_DTO

    TAPI_DEFINE_STRUCT(DTO,
        SF_tblAccountProductsMTBase_DTO
    );
}

namespace tblAccountProductsI18NMTBase {

    namespace ExtraFields {
    }

    namespace ExtraRelation {
//        constexpr char AAA[] = "aaa";
    }

    namespace Private {
        const QList<clsORMField> ExtraORMFields = {
            ///ColName                                    Type        Validation                         Default    UpBy       Sort  Filter Self  Virt   PK
        };

        inline const QList<stuRelation> ExtraRelations(Q_DECL_UNUSED const QString& _schema) {
            ///Col                              Reference Table                      ForeignCol             Rename     LeftJoin
            return { };
        };

        const QList<stuDBIndex> ExtraIndexes = {
        };

    } //namespace Private

#define SF_tblAccountProductsI18NMTBase_DTO \
    SF_tblAccountProductsI18NBase_DTO

    TAPI_DEFINE_STRUCT(DTO,
        SF_tblAccountProductsI18NMTBase_DTO
    );
}

namespace tblAccountSaleablesMTBase {

    namespace ExtraFields {
        TARGOMAN_CREATE_CONSTEXPR(slbValidityDurationInDays);
        TARGOMAN_CREATE_CONSTEXPR(slbValidityWords);
    }

    namespace ExtraRelation {
//        constexpr char AAA[] = "aaa";
    }

    namespace Private {
        const QList<clsORMField> ExtraORMFields = {
            //ColName                                   Type        Validation      Default UpBy       Sort  Filter Self  Virt   PK
            { ExtraFields::slbValidityDurationInDays,   S(quint32), QFV.integer(),  0,      UPOwner },
            { ExtraFields::slbValidityWords,            S(quint32), QFV.integer(),  0,      UPOwner },
        };

        inline const QList<stuRelation> ExtraRelations(Q_DECL_UNUSED const QString& _schema) {
            ///Col                              Reference Table                      ForeignCol             Rename     LeftJoin
            return { };
        };

        const QList<stuDBIndex> ExtraIndexes = {
        };

    } //namespace Private

#define SF_tblAccountSaleablesMTBase_DTO \
    SF_tblAccountSaleablesBase_DTO

    TAPI_DEFINE_STRUCT(DTO,
        SF_tblAccountSaleablesMTBase_DTO,

        SF_quint32                  (slbValidityDurationInDays),
        SF_quint32                  (slbValidityWords)
    );
}

namespace tblAccountSaleablesI18NMTBase {

    namespace ExtraFields {
    }

    namespace ExtraRelation {
//        constexpr char AAA[] = "aaa";
    }

    namespace Private {
        const QList<clsORMField> ExtraORMFields = {
            ///ColName                                    Type        Validation                         Default    UpBy       Sort  Filter Self  Virt   PK
        };

        inline const QList<stuRelation> ExtraRelations(Q_DECL_UNUSED const QString& _schema) {
            ///Col                              Reference Table                      ForeignCol             Rename     LeftJoin
            return { };
        };

        const QList<stuDBIndex> ExtraIndexes = {
        };

    } //namespace Private

#define SF_tblAccountSaleablesI18NMTBase_DTO \
    SF_tblAccountSaleablesI18NBase_DTO

    TAPI_DEFINE_STRUCT(DTO,
        SF_tblAccountSaleablesI18NMTBase_DTO
    );
}

namespace tblAccountSaleablesFilesMTBase {

    namespace ExtraFields {
    }

    namespace ExtraRelation {
    }

    namespace Private {
        const QList<clsORMField> ExtraORMFields = {
            ///ColName                                     Type        Validation                  Default    UpBy       Sort  Filter Self  Virt   PK
        };

        inline const QList<stuRelation> ExtraRelations(Q_DECL_UNUSED const QString& _schema) {
            ///Col                              Reference Table                      ForeignCol             Rename     LeftJoin
            return { };
        };

        const QList<stuDBIndex> ExtraIndexes = {
        };

    } //namespace Private

#define SF_tblAccountSaleablesFilesMTBase_DTO \
    SF_tblAccountSaleablesFilesBase_DTO

    TAPI_DEFINE_STRUCT(DTO,
        SF_tblAccountSaleablesFilesMTBase_DTO
    );
}

namespace tblAccountUserAssetsMTBase {

    namespace ExtraFields {
    }

    namespace ExtraRelation {
    //        constexpr char AAA[] = "aaa";
    }

    namespace Private {
        const QList<clsORMField> ExtraORMFields = {
            //ColName                           Type        Validation                  Default     UpBy    Sort    Filter Self  Virt   PK
        };

        inline const QList<stuRelation> ExtraRelations(Q_DECL_UNUSED const QString& _schema) {
            ///Col                              Reference Table                      ForeignCol             Rename     LeftJoin
            return { };
        };

        const QList<stuDBIndex> ExtraIndexes = {
        };

    } //namespace Private

#define SF_tblAccountUserAssetsMTBase_DTO \
    SF_tblAccountUserAssetsBase_DTO

    TAPI_DEFINE_STRUCT(DTO,
        SF_tblAccountUserAssetsMTBase_DTO
    );
}

namespace tblAccountUserAssetsFilesMTBase {

    namespace ExtraFields {
    }

    namespace ExtraRelation {
    }

    namespace Private {
        const QList<clsORMField> ExtraORMFields = {
            //ColName                           Type        Validation                  Default     UpBy    Sort    Filter Self  Virt   PK
        };

        inline const QList<stuRelation> ExtraRelations(Q_DECL_UNUSED const QString& _schema) {
            ///Col                              Reference Table                      ForeignCol             Rename     LeftJoin
            return { };
        };

        const QList<stuDBIndex> ExtraIndexes = {
        };

    } //namespace Private

#define SF_tblAccountUserAssetsFilesMTBase_DTO \
    SF_tblAccountUserAssetsFilesBase_DTO

    TAPI_DEFINE_STRUCT(DTO,
        SF_tblAccountUserAssetsFilesMTBase_DTO
    );
}

namespace tblAccountAssetUsageMTBase {

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

        inline const QList<stuRelation> ExtraRelations(Q_DECL_UNUSED const QString& _schema) {
            ///Col                              Reference Table                      ForeignCol             Rename     LeftJoin
            return { };
        };

        const QList<stuDBIndex> ExtraIndexes = {
        };

    } //namespace Private

#define SF_tblAccountAssetUsageMTBase_DTO \
    SF_tblAccountAssetUsageBase_DTO

    TAPI_DEFINE_STRUCT(DTO,
        SF_tblAccountAssetUsageMTBase_DTO
    );
}

namespace tblAccountCouponsMTBase {

    namespace ExtraFields {
    }

    namespace ExtraRelation {
    //        constexpr char AAA[] = "aaa";
    }

    namespace Private {
        const QList<clsORMField> ExtraORMFields = {
            ///ColName                  Type        Validation                  Default    UpBy       Sort  Filter Self  Virt   PK
        };

        inline const QList<stuRelation> ExtraRelations(Q_DECL_UNUSED const QString& _schema) {
            ///Col                              Reference Table                      ForeignCol             Rename     LeftJoin
            return { };
        };

        const QList<stuDBIndex> ExtraIndexes = {
        };

    } //namespace Private

#define SF_tblAccountCouponsMTBase_DTO \
    SF_tblAccountCouponsBase_DTO

    TAPI_DEFINE_STRUCT(DTO,
        SF_tblAccountCouponsMTBase_DTO
    );
}

#pragma GCC diagnostic pop

/******************************************************/
/******************************************************/
/******************************************************/
class intfAccountUnitsMT : public intfAccountUnits
{
    Q_OBJECT

public:
    intfAccountUnitsMT(const QString& _schema,
                       const QList<DBM::clsORMField>& _exclusiveCols = {},
                       const QList<DBM::stuRelation>& _exclusiveRelations = {},
                       const QList<DBM::stuDBIndex>& _exclusiveIndexes = {});
};

/******************************************************/
class intfAccountUnitsI18NMT : public intfAccountUnitsI18N
{
    Q_OBJECT

public:
    intfAccountUnitsI18NMT(const QString& _schema,
                           const QList<DBM::clsORMField>& _exclusiveCols = {},
                           const QList<DBM::stuRelation>& _exclusiveRelations = {},
                           const QList<DBM::stuDBIndex>& _exclusiveIndexes = {});
};

/******************************************************/
/******************************************************/
/******************************************************/
class intfAccountProductsMT : public intfAccountProducts
{
    Q_OBJECT

public:
    intfAccountProductsMT(const QString& _schema,
                          const QList<DBM::clsORMField>& _exclusiveCols = {},
                          const QList<DBM::stuRelation>& _exclusiveRelations = {},
                          const QList<DBM::stuDBIndex>& _exclusiveIndexes = {});
};

/******************************************************/
class intfAccountProductsI18NMT : public intfAccountProductsI18N
{
    Q_OBJECT

public:
    intfAccountProductsI18NMT(const QString& _schema,
                              const QList<DBM::clsORMField>& _exclusiveCols = {},
                              const QList<DBM::stuRelation>& _exclusiveRelations = {},
                              const QList<DBM::stuDBIndex>& _exclusiveIndexes = {});
};

/******************************************************/
/******************************************************/
/******************************************************/
class intfAccountSaleablesMT : public intfAccountSaleables
{
    Q_OBJECT

public:
    intfAccountSaleablesMT(const QString& _schema,
                           const QList<DBM::clsORMField>& _exclusiveCols = {},
                           const QList<DBM::stuRelation>& _exclusiveRelations = {},
                           const QList<DBM::stuDBIndex>& _exclusiveIndexes = {});
};

/******************************************************/
class intfAccountSaleablesI18NMT : public intfAccountSaleablesI18N
{
    Q_OBJECT

public:
    intfAccountSaleablesI18NMT(const QString& _schema,
                               const QList<DBM::clsORMField>& _exclusiveCols = {},
                               const QList<DBM::stuRelation>& _exclusiveRelations = {},
                               const QList<DBM::stuDBIndex>& _exclusiveIndexes = {});
};

/******************************************************/
class intfAccountSaleablesFilesMT : public intfAccountSaleablesFiles
{
    Q_OBJECT

public:
    intfAccountSaleablesFilesMT(const QString& _schema,
                                const QList<DBM::clsORMField>& _exclusiveCols = {},
                                const QList<DBM::stuRelation>& _exclusiveRelations = {},
                                const QList<DBM::stuDBIndex>& _exclusiveIndexes = {});
};

/******************************************************/
/******************************************************/
/******************************************************/
class intfAccountUserAssetsMT : public intfAccountUserAssets
{
    Q_OBJECT

public:
    intfAccountUserAssetsMT(const QString& _schema,
                            const QList<DBM::clsORMField>& _exclusiveCols = {},
                            const QList<DBM::stuRelation>& _exclusiveRelations = {},
                            const QList<DBM::stuDBIndex>& _exclusiveIndexes = {});
};

/******************************************************/
class intfAccountUserAssetsFilesMT : public intfAccountUserAssetsFiles
{
    Q_OBJECT

public:
    intfAccountUserAssetsFilesMT(const QString& _schema,
                                 const QList<DBM::clsORMField>& _exclusiveCols = {},
                                 const QList<DBM::stuRelation>& _exclusiveRelations = {},
                                 const QList<DBM::stuDBIndex>& _exclusiveIndexes = {});
};

/******************************************************/
/******************************************************/
/******************************************************/
class intfAccountAssetUsageMT : public intfAccountAssetUsage
{
    Q_OBJECT

public:
    intfAccountAssetUsageMT(const QString& _schema,
                            const QList<DBM::clsORMField>& _exclusiveCols = {},
                            const QList<DBM::stuRelation>& _exclusiveRelations = {},
                            const QList<DBM::stuDBIndex>& _exclusiveIndexes = {});
};

/******************************************************/
/******************************************************/
/******************************************************/
class intfAccountCouponsMT : public intfAccountCoupons
{
    Q_OBJECT

public:
    intfAccountCouponsMT(const QString& _schema,
                         const QList<DBM::clsORMField>& _exclusiveCols = {},
                         const QList<DBM::stuRelation>& _exclusiveRelations = {},
                         const QList<DBM::stuDBIndex>& _exclusiveIndexes = {});
};

/******************************************************/
/******************************************************/
/******************************************************/
//There is no Prize in this module
/*class intfAccountPrizesMT : public intfAccountPrizes
{
    Q_OBJECT

public:
    intfAccountPrizesMT();
};
*/

} //namespace Targoman::API::ModuleHelpers::MT::Interfaces

#endif // TARGOMAN_API_MODULEHELPERS_MT_INTERFACES_INTFMTACCOUNTING_H
