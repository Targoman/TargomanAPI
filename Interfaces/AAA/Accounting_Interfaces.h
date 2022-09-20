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

/******************************************************/
/*
class intfAccountORMBase
{
public:
    intfAccountORMBase(bool _isTokenBase) :
        IsTokenBase(_isTokenBase)
    { ; }

public:
    bool IsTokenBase;
};
*/
/******************************************************/
/******************************************************/
/******************************************************/
class intfAccountUnitsI18N : public intfSQLBasedModule//, public intfAccountORMBase
{
    Q_OBJECT

public:
    intfAccountUnitsI18N(//bool _isTokenBase,
                         const QString& _schema,
                         const QList<DBM::clsORMField>& _exclusiveCols = {},
                         const QList<DBM::stuRelation>& _exclusiveRelations = {},
                         const QList<DBM::stuDBIndex>& _exclusiveIndexes = {});

public:
    //key: schema
    static QMap<QString, intfAccountUnitsI18N*> MyInstance;
//    friend class intfAccountUnits;
};

/******************************************************/
class intfAccountUnits : public intfSQLBasedModule//, public intfAccountORMBase
{
    Q_OBJECT

public:
    intfAccountUnits(//bool _isTokenBase,
                     const QString& _schema,
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
class intfAccountProductsI18N : public intfSQLBasedModule//, public intfAccountORMBase
{
    Q_OBJECT

public:
    intfAccountProductsI18N(//bool _isTokenBase,
                            const QString& _schema,
                            const QList<DBM::clsORMField>& _exclusiveCols = {},
                            const QList<DBM::stuRelation>& _exclusiveRelations = {},
                            const QList<DBM::stuDBIndex>& _exclusiveIndexes = {});

public:
    //key: schema
    static QMap<QString, intfAccountProductsI18N*> MyInstance;
//    friend class intfAccountProducts;
};

/******************************************************/
class intfAccountProducts : public intfSQLBasedModule//, public intfAccountORMBase
{
    Q_OBJECT

public:
    intfAccountProducts(//bool _isTokenBase,
                        const QString& _schema,
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
class intfAccountSaleablesI18N : public intfSQLBasedModule//, public intfAccountORMBase
{
    Q_OBJECT

public:
    intfAccountSaleablesI18N(//bool _isTokenBase,
                             const QString& _schema,
                             const QList<DBM::clsORMField>& _exclusiveCols = {},
                             const QList<DBM::stuRelation>& _exclusiveRelations = {},
                             const QList<DBM::stuDBIndex>& _exclusiveIndexes = {});

public:
    //key: schema
    static QMap<QString, intfAccountSaleablesI18N*> MyInstance;
//    friend class intfAccountSaleables;
};

/******************************************************/
class intfAccountSaleables : public intfSQLBasedModule//, public intfAccountORMBase
{
    Q_OBJECT

public:
    intfAccountSaleables(//bool _isTokenBase,
                         const QString& _schema,
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
class intfAccountSaleablesFiles : public intfSQLBasedModule//, public intfAccountORMBase
{
    Q_OBJECT

public:
    intfAccountSaleablesFiles(//bool _isTokenBase,
                              const QString& _schema,
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
class intfAccountUserAssets : public intfSQLBasedModule//, public intfAccountORMBase
{
    Q_OBJECT

public:
    intfAccountUserAssets(//bool _isTokenBase,
                          const QString& _schema,
                          const QList<DBM::clsORMField>& _exclusiveCols = {},
                          const QList<DBM::stuRelation>& _exclusiveRelations = {},
                          const QList<DBM::stuDBIndex>& _exclusiveIndexes = {});

public:
    virtual ORMSelectQuery makeSelectQuery(INTFAPICALLBOOM_DECL &APICALLBOOM_PARAM, const QString &_alias = {}, bool _translate = true, bool _isRoot = true);

private slots:
    QVariant ORMGET_USER("Get User Assets")

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
class intfAccountUserAssetsFiles : public intfSQLBasedModule//, public intfAccountORMBase
{
    Q_OBJECT

public:
    intfAccountUserAssetsFiles(//bool _isTokenBase,
                               const QString& _schema,
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
class intfAccountAssetUsage : public intfSQLBasedModule//, public intfAccountORMBase
{
    Q_OBJECT

public:
    intfAccountAssetUsage(//bool _isTokenBase,
                          const QString& _schema,
                          const QList<DBM::clsORMField>& _exclusiveCols = {},
                          const QList<DBM::stuRelation>& _exclusiveRelations = {},
                          const QList<DBM::stuDBIndex>& _exclusiveIndexes = {});
private slots:
    QVariant ORMGET_USER("Get user Usage on each Asset")
};

/******************************************************/
/******************************************************/
/******************************************************/
class intfAccountCoupons : public intfSQLBasedModule//, public intfAccountORMBase
{
    Q_OBJECT

public:
    intfAccountCoupons(//bool _isTokenBase,
                       const QString& _schema,
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
class intfAccountPrizes : public intfSQLBasedModule//, public intfAccountORMBase
{
    Q_OBJECT

public:
    intfAccountPrizes(//bool _isTokenBase,
                      const QString& _schema,
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
