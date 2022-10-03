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
 * @author S.Mehran M.Ziabary <ziabary@targoman.com>
 * @author Kambiz Zandi <kambizzandi@gmail.com>
 */

#ifndef TARGOMAN_API_AAA_ACCOUNTING_INTERFACES_H
#define TARGOMAN_API_AAA_ACCOUNTING_INTERFACES_H

#include "Interfaces/AAA/Accounting_Defs.hpp"

#include "Interfaces/API/intfSQLBasedModule.h"
using namespace Targoman::API::API;

namespace Targoman::API::AAA {

/******************************************************\

If!TB: If NOT token base
If=TB: If IS token base

------------------|-------|-------|-------|---------
AccountUnits      | ANONY | USER  | API   |
------------------|-------|-------|-------|---------
 C                |       |  x    |       |
 R                |  x    |  x    |       |
 U                |       |  x    |       |
 D                |       |  x    |       |

------------------|-------|-------|-------|---------
AccountProducts   | ANONY | USER  | API   |
------------------|-------|-------|-------|---------
 C                |       |  x    |       |
 R                |  x    |  x    |       |
 U                |       |  x    |       |
 D                |       |  x    |       |

------------------|-------|-------|-------|---------
AccountSaleables  | ANONY | USER  | API   |
------------------|-------|-------|-------|---------
 C                |       |  x    |       |
 R                |  x    |  x    |       |
 U                |       |  x    |       |
 D                |       |  x    |       |

------------------|-------|-------|-------|---------
AccountUserAssets | ANONY | USER  | API   |
------------------|-------|-------|-------|---------
 C                |       |  x    |       |
 R                |       | If!TB | If=TB |
 U                |       |       |       |
 D                |       |       |       |

------------------|-------|-------|-------|---------
AccountAssetUsage | ANONY | USER  | API   |
------------------|-------|-------|-------|---------
 C                |       | If!TB | If=TB |
 R                |       |  x    | If=TB |
 U                |       |  x    | If=TB |
 D                |       |       |       |

------------------|-------|-------|-------|---------
AccountCoupons    | ANONY | USER  | API   |
------------------|-------|-------|-------|---------
 C                |       |  x    |       |
 R                |  x    |  x    |       |
 U                |       |  x    |       |
 D                |       |  x    |       |

------------------|-------|-------|-------|---------
AccountPrizes     | ANONY | USER  | API   |
------------------|-------|-------|-------|---------
 C                |       |       |       |
 R                |       |       |       |
 U                |       |       |       |
 D                |       |       |       |
------------------|-------|-------|-------|---------

\******************************************************/


/******************************************************/
//template <bool _itmplIsTokenBase>
//class intfAccountORMBase
//{
//public:
//    typedef typename std::conditional<_itmplIsTokenBase,
//                            APICALLBOOM_TYPE_JWT_API_DECL,
//                            APICALLBOOM_TYPE_JWT_USER_DECL>::type
//    APICALLBOOM_TYPE_JWT_TOKENBASE_DECL;

//    typedef Q_DECL_UNUSED typename std::conditional<_itmplIsTokenBase,
//                            APICALLBOOM_TYPE_JWT_API_DECL,
//                            APICALLBOOM_TYPE_JWT_USER_DECL>::type
//    APICALLBOOM_TYPE_JWT_TOKENBASE_IMPL;

//public:
//    bool IsTokenBase() { return _itmplIsTokenBase; }
//};

#define ACCORM_TOKENBASE_TYPES(_isTokenBase) \
    public: \
        typedef typename std::conditional<_isTokenBase, \
                                APICALLBOOM_TYPE_JWT_API_DECL, \
                                APICALLBOOM_TYPE_JWT_USER_DECL>::type \
        APICALLBOOM_TYPE_JWT_TOKENBASE_DECL; \
        typedef Q_DECL_UNUSED typename std::conditional<_isTokenBase, \
                                APICALLBOOM_TYPE_JWT_API_DECL, \
                                APICALLBOOM_TYPE_JWT_USER_DECL>::type \
        APICALLBOOM_TYPE_JWT_TOKENBASE_IMPL; \
    public: \
        bool IsTokenBase() { return _isTokenBase; }

/******************************************************/
/******************************************************/
/******************************************************/
class intfAccountUnitsI18N : public intfSQLBasedModule
{
    Q_OBJECT

public:
    intfAccountUnitsI18N(const QString& _schema,
                         const QList<DBM::clsORMField>& _exclusiveCols = {},
                         const QList<DBM::stuRelation>& _exclusiveRelations = {},
                         const QList<DBM::stuDBIndex>& _exclusiveIndexes = {});

public:
    //key: schema
    static QMap<QString, intfAccountUnitsI18N*> MyInstance;
//    friend class intfAccountUnits;
};

/******************************************************/
/******************************************************/
/******************************************************/
class intfAccountUnits : public intfSQLBasedModule
{
    Q_OBJECT

public:
    intfAccountUnits(const QString& _schema,
                     const QList<DBM::clsORMField>& _exclusiveCols = {},
                     const QList<DBM::stuRelation>& _exclusiveRelations = {},
                     const QList<DBM::stuDBIndex>& _exclusiveIndexes = {});

public:
    virtual ORMSelectQuery makeSelectQuery(INTFAPICALLBOOM_DECL &APICALLBOOM_PARAM, const QString &_alias = {}, bool _translate = true, bool _isRoot = true);

private slots:
    QVariant ORMGET_ANONYMOUSE("Get Available Units")
    quint32 ORMCREATE_USER("Create a new Unit by an authorized user")
    bool ORMUPDATE_USER("Update a Unit info by an authorized user")
    bool ORMDELETE_USER("Delete a Unit")
};

/******************************************************/
/******************************************************/
/******************************************************/
class intfAccountProductsI18N : public intfSQLBasedModule
{
    Q_OBJECT

public:
    intfAccountProductsI18N(const QString& _schema,
                            const QList<DBM::clsORMField>& _exclusiveCols = {},
                            const QList<DBM::stuRelation>& _exclusiveRelations = {},
                            const QList<DBM::stuDBIndex>& _exclusiveIndexes = {});

public:
    //key: schema
    static QMap<QString, intfAccountProductsI18N*> MyInstance;
//    friend class intfAccountProducts;
};

/******************************************************/
/******************************************************/
/******************************************************/
class intfAccountProducts : public intfSQLBasedModule
{
    Q_OBJECT

public:
    intfAccountProducts(const QString& _schema,
                        const QList<DBM::clsORMField>& _exclusiveCols = {},
                        const QList<DBM::stuRelation>& _exclusiveRelations = {},
                        const QList<DBM::stuDBIndex>& _exclusiveIndexes = {});

public:
    virtual ORMSelectQuery makeSelectQuery(INTFAPICALLBOOM_DECL &APICALLBOOM_PARAM, const QString &_alias = {}, bool _translate = true, bool _isRoot = true);

private slots:
    QVariant ORMGET_ANONYMOUSE("Get Available Products")
    quint32 ORMCREATE_USER("Create a new Product by an authorized user")
    bool ORMUPDATE_USER("Update a Product info by an authorized user")
    bool ORMDELETE_USER("Delete a Product")
};

/******************************************************/
/******************************************************/
/******************************************************/
class intfAccountSaleablesI18N : public intfSQLBasedModule
{
    Q_OBJECT

public:
    intfAccountSaleablesI18N(const QString& _schema,
                             const QList<DBM::clsORMField>& _exclusiveCols = {},
                             const QList<DBM::stuRelation>& _exclusiveRelations = {},
                             const QList<DBM::stuDBIndex>& _exclusiveIndexes = {});

public:
    //key: schema
    static QMap<QString, intfAccountSaleablesI18N*> MyInstance;
//    friend class intfAccountSaleables;
};

/******************************************************/
/******************************************************/
/******************************************************/
class intfAccountSaleables : public intfSQLBasedModule
{
    Q_OBJECT

public:
    intfAccountSaleables(const QString& _schema,
                         const QList<DBM::clsORMField>& _exclusiveCols = {},
                         const QList<DBM::stuRelation>& _exclusiveRelations = {},
                         const QList<DBM::stuDBIndex>& _exclusiveIndexes = {});

public:
    //key: schema
    static QMap<QString, intfAccountSaleables*> MyInstance;

public:
    virtual ORMSelectQuery makeSelectQuery(INTFAPICALLBOOM_DECL &APICALLBOOM_PARAM, const QString &_alias = {}, bool _translate = true, bool _isRoot = true);

private slots:
    QVariant ORMGET_ANONYMOUSE("Get Available Saleables")
    quint32 ORMCREATE_USER("Create a new Saleable by an authorized user")
    bool ORMUPDATE_USER("Update a Saleable info by an authorized user")
    bool ORMDELETE_USER("Delete a Saleable")
};

/******************************************************/
/******************************************************/
/******************************************************/
class intfAccountSaleablesFiles : public intfSQLBasedModule
{
    Q_OBJECT

public:
    intfAccountSaleablesFiles(const QString& _schema,
                              const QList<DBM::clsORMField>& _exclusiveCols = {},
                              const QList<DBM::stuRelation>& _exclusiveRelations = {},
                              const QList<DBM::stuDBIndex>& _exclusiveIndexes = {});

//public:
//    virtual ORMSelectQuery makeSelectQuery(INTFAPICALLBOOM_DECL &APICALLBOOM_PARAM, const QString &_alias = {}, bool _translate = true, bool _isRoot = true);

public:
    //key: schema
    static QMap<QString, intfAccountSaleablesFiles*> MyInstance;
//    friend class intfAccountUserAssets;

private slots:
    QVariant ORMGET_ANONYMOUSE("Get Available Saleables Files")
    quint32 ORMCREATE_USER("Create a new Saleables File by an authorized user")
    bool ORMUPDATE_USER("Update a Saleables File info by an authorized user")
    bool ORMDELETE_USER("Delete a Saleables File")
};

/******************************************************/
/******************************************************/
/******************************************************/
class baseintfAccountUserAssets : public intfSQLBasedModule
{
    Q_OBJECT

public:
    baseintfAccountUserAssets(const QString& _schema,
                              const QList<DBM::clsORMField>& _cols = {},
                              const QList<DBM::stuRelation>& _relations = {},
                              const QList<DBM::stuDBIndex>& _indexes = {});

protected:
//    QVariant ORMGET_INTERNAL

private slots:
    bool REST_UPDATE(
        disablePackage,
        (
            APICALLBOOM_TYPE_JWT_USER_DECL &APICALLBOOM_PARAM,
            TAPI::PKsByPath_t _pksByPath
        ),
        "Mark a user Asset banned by an authorized user"
    )

    bool REST_UPDATE(
        setAsPrefered,
        (
            APICALLBOOM_TYPE_JWT_USER_DECL &APICALLBOOM_PARAM,
            TAPI::PKsByPath_t _pksByPath
        ),
        "Mark a user Asset as prefered"
    )
};

/******************************************************/
class baseintfAccountUserAssets_USER : public baseintfAccountUserAssets
{
    Q_OBJECT
    ACCORM_TOKENBASE_TYPES(false)

public:
    baseintfAccountUserAssets_USER(const QString& _schema,
                                   const QList<DBM::clsORMField>& _cols = {},
                                   const QList<DBM::stuRelation>& _relations = {},
                                   const QList<DBM::stuDBIndex>& _indexes = {});

private slots:
//    QVariant ORMGET_TOKENBASE("Get User Assets")
    QVariant ORMGET_USER("Get User Assets")
};

/******************************************************/
class baseintfAccountUserAssets_API : public baseintfAccountUserAssets
{
    Q_OBJECT
    ACCORM_TOKENBASE_TYPES(true)

public:
    baseintfAccountUserAssets_API(const QString& _schema,
                                  const QList<DBM::clsORMField>& _cols = {},
                                  const QList<DBM::stuRelation>& _relations = {},
                                  const QList<DBM::stuDBIndex>& _indexes = {});

private slots:
//    QVariant ORMGET_TOKENBASE("Get User Assets")
    QVariant ORMGET_API("Get User Assets")
};

/******************************************************/
template <bool _itmplIsTokenBase>
class intfAccountUserAssets :
    public std::conditional<_itmplIsTokenBase, baseintfAccountUserAssets_API, baseintfAccountUserAssets_USER>::type //intfSQLBasedModule// , public intfAccountORMBase<_itmplIsTokenBase>
{
public:
    intfAccountUserAssets(const QString& _schema,
                          const QList<DBM::clsORMField>& _exclusiveCols = {},
                          const QList<DBM::stuRelation>& _exclusiveRelations = {},
                          const QList<DBM::stuDBIndex>& _exclusiveIndexes = {});

public:
    virtual ORMSelectQuery makeSelectQuery(INTFAPICALLBOOM_DECL &APICALLBOOM_PARAM, const QString &_alias = {}, bool _translate = true, bool _isRoot = true);
};

/******************************************************/
/******************************************************/
/******************************************************/
class intfAccountUserAssetsFiles : public intfSQLBasedModule
{
    Q_OBJECT

public:
    intfAccountUserAssetsFiles(const QString& _schema,
                               const QList<DBM::clsORMField>& _exclusiveCols = {},
                               const QList<DBM::stuRelation>& _exclusiveRelations = {},
                               const QList<DBM::stuDBIndex>& _exclusiveIndexes = {});

public:
    //key: schema
    static QMap<QString, intfAccountUserAssetsFiles*> MyInstance;
//    friend class intfAccountUserAssets;

private slots:
    QVariant ORMGET_USER("Get User Assets Files")

    ///@TODO: complete other orm actions
};

/******************************************************/
/******************************************************/
/******************************************************/
class baseintfAccountAssetUsage : public intfSQLBasedModule
{
    Q_OBJECT

public:
    baseintfAccountAssetUsage(const QString& _schema,
                              const QList<DBM::clsORMField>& _cols = {},
                              const QList<DBM::stuRelation>& _relations = {},
                              const QList<DBM::stuDBIndex>& _indexes = {});

private slots:
};

/******************************************************/
class baseintfAccountAssetUsage_USER : public baseintfAccountAssetUsage
{
    Q_OBJECT
    ACCORM_TOKENBASE_TYPES(false)

public:
    baseintfAccountAssetUsage_USER(const QString& _schema,
                                   const QList<DBM::clsORMField>& _cols = {},
                                   const QList<DBM::stuRelation>& _relations = {},
                                   const QList<DBM::stuDBIndex>& _indexes = {});

private slots:
//    QVariant ORMGET_TOKENBASE("Get User Assets")
    QVariant ORMGET_USER("Get User Assets")
};

/******************************************************/
class baseintfAccountAssetUsage_API : public baseintfAccountAssetUsage
{
    Q_OBJECT
    ACCORM_TOKENBASE_TYPES(true)

public:
    baseintfAccountAssetUsage_API(const QString& _schema,
                                  const QList<DBM::clsORMField>& _cols = {},
                                  const QList<DBM::stuRelation>& _relations = {},
                                  const QList<DBM::stuDBIndex>& _indexes = {});

private slots:
//    QVariant ORMGET_TOKENBASE("Get User Assets")
    QVariant ORMGET_API("Get User Assets")
};

/******************************************************/
template <bool _itmplIsTokenBase>
class intfAccountAssetUsage : public std::conditional<_itmplIsTokenBase, baseintfAccountAssetUsage_API, baseintfAccountAssetUsage_USER>::type //intfSQLBasedModule// , public intfAccountORMBase<_itmplIsTokenBase>
{
public:
    intfAccountAssetUsage(//bool _isTokenBase,
                     const QString& _schema,
                     const QList<DBM::clsORMField>& _exclusiveCols = {},
                     const QList<DBM::stuRelation>& _exclusiveRelations = {},
                     const QList<DBM::stuDBIndex>& _exclusiveIndexes = {});

};

/******************************************************/
/******************************************************/
/******************************************************/
class intfAccountCoupons : public intfSQLBasedModule
{
    Q_OBJECT

public:
    intfAccountCoupons(const QString& _schema,
                       const QList<DBM::clsORMField>& _exclusiveCols = {},
                       const QList<DBM::stuRelation>& _exclusiveRelations = {},
                       const QList<DBM::stuDBIndex>& _exclusiveIndexes = {});

private slots:
    QVariant ORMGET_USER("Get Active Discounts")
    quint32 ORMCREATE_USER("Create a new Discount by an authorized user")
    bool ORMUPDATE_USER("Update a Discount info by an authorized user")
    bool ORMDELETE_USER("Delete a Discount")
};

/******************************************************/
/******************************************************/
/******************************************************/
class intfAccountPrizes : public intfSQLBasedModule
{
    Q_OBJECT

public:
    intfAccountPrizes(const QString& _schema,
                      const QList<DBM::clsORMField>& _exclusiveCols = {},
                      const QList<DBM::stuRelation>& _exclusiveRelations = {},
                      const QList<DBM::stuDBIndex>& _exclusiveIndexes = {});

private slots:
    QVariant ORMGET_USER("Get Active Prizes")
    quint32 ORMCREATE_USER("Create a new Prizes by an authorized user")
    bool ORMUPDATE_USER("Update a Prizes info by an authorized user")
    bool ORMDELETE_USER("Delete a Prizes")
};

} //namespace Targoman::API::AAA

#endif // TARGOMAN_API_AAA_ACCOUNTING_INTERFACES_H
