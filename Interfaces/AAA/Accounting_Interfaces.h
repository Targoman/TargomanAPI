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
class intfAccountUnitsTranslate : public intfSQLBasedModule
{
    Q_OBJECT

public:
    intfAccountUnitsTranslate(const QString& _schema,
                              const QList<DBM::clsORMField>& _exclusiveCols = {},
                              const QList<DBM::stuRelation>& _exclusiveRelations = {},
                              const QList<DBM::stuDBIndex>& _exclusiveIndexes = {});

protected:
    static intfAccountUnitsTranslate* myInstance;
    friend class intfAccountUnits;
};

class intfAccountUnits : public intfSQLBasedModule
{
    Q_OBJECT

public:
    intfAccountUnits(const QString& _schema,
                     const QList<DBM::clsORMField>& _exclusiveCols = {},
                     const QList<DBM::stuRelation>& _exclusiveRelations = {},
                     const QList<DBM::stuDBIndex>& _exclusiveIndexes = {});

public:
    virtual ORMSelectQuery GetSelectQuery(INTFAPICALLBOOM_DECL &APICALLBOOM_PARAM, const QString& _alias = {});

private slots:
    QVariant ANONYMOUSE_ORMGET("Get Available Units")
    quint32 ORMCREATE("Create a new Unit by priviledged user")
    bool ORMUPDATE("Update a Unit info by priviledged user")
    bool ORMDELETE("Delete a Unit")
};

/******************************************************/
class intfAccountProductsTranslate : public intfSQLBasedModule
{
    Q_OBJECT

public:
    intfAccountProductsTranslate(const QString& _schema,
                                 const QList<DBM::clsORMField>& _exclusiveCols = {},
                                 const QList<DBM::stuRelation>& _exclusiveRelations = {},
                                 const QList<DBM::stuDBIndex>& _exclusiveIndexes = {});

protected:
    static intfAccountProductsTranslate* myInstance;
    friend class intfAccountProducts;
};

class intfAccountProducts : public intfSQLBasedModule
{
    Q_OBJECT

public:
    intfAccountProducts(const QString& _schema,
                        const QList<DBM::clsORMField>& _exclusiveCols = {},
                        const QList<DBM::stuRelation>& _exclusiveRelations = {},
                        const QList<DBM::stuDBIndex>& _exclusiveIndexes = {});

public:
    virtual ORMSelectQuery GetSelectQuery(INTFAPICALLBOOM_DECL &APICALLBOOM_PARAM, const QString& _alias = {});

private slots:
#ifdef QT_DEBUG
    QVariant ANONYMOUSE_ORMGET("Get Available Products")
#else
    QVariant ORMGET("Get Available Products")
#endif

    quint32 ORMCREATE("Create a new Product by priviledged user")
    bool ORMUPDATE("Update a Product info by priviledged user")
    bool ORMDELETE("Delete a Product")
};

/******************************************************/
class intfAccountSaleablesTranslate : public intfSQLBasedModule
{
    Q_OBJECT

public:
    intfAccountSaleablesTranslate(const QString& _schema,
                                  const QList<DBM::clsORMField>& _exclusiveCols = {},
                                  const QList<DBM::stuRelation>& _exclusiveRelations = {},
                                  const QList<DBM::stuDBIndex>& _exclusiveIndexes = {});

protected:
    static intfAccountSaleablesTranslate* myInstance;
    friend class intfAccountSaleables;
};

class intfAccountSaleables : public intfSQLBasedModule
{
    Q_OBJECT

public:
    intfAccountSaleables(const QString& _schema,
                         const QList<DBM::clsORMField>& _exclusiveCols = {},
                         const QList<DBM::stuRelation>& _exclusiveRelations = {},
                         const QList<DBM::stuDBIndex>& _exclusiveIndexes = {});

public:
    virtual ORMSelectQuery GetSelectQuery(INTFAPICALLBOOM_DECL &APICALLBOOM_PARAM, const QString& _alias = {});

private slots:
#ifdef QT_DEBUG
    QVariant ANONYMOUSE_ORMGET("Get Available Saleables")
#else
    QVariant ORMGET("Get Available Saleables")
#endif

    quint32 ORMCREATE("Create a new Saleable by priviledged user")
    bool ORMUPDATE("Update a Saleable info by priviledged user")
    bool ORMDELETE("Delete a Saleable")
};

/******************************************************/
class intfAccountUserAssets : public intfSQLBasedModule
{
    Q_OBJECT

public:
    intfAccountUserAssets(const QString& _schema,
                          const QList<DBM::clsORMField>& _exclusiveCols = {},
                          const QList<DBM::stuRelation>& _exclusiveRelations = {},
                          const QList<DBM::stuDBIndex>& _exclusiveIndexes = {});

private slots:
    QVariant ORMGET("Get User Assets")
    bool REST_UPDATE(
        disablePackage,
        (
            APICALLBOOM_TYPE_JWT_DECL &APICALLBOOM_PARAM,
            TAPI::PKsByPath_t _pksByPath
        ),
        "Mark a user Asset banned by priviledged user"
    )

    bool REST_UPDATE(
        setAsPrefered,
        (
            APICALLBOOM_TYPE_JWT_DECL &APICALLBOOM_PARAM,
            TAPI::PKsByPath_t _pksByPath
        ),
        "Mark a user Asset as prefered"
    )
};

/******************************************************/
class intfAccountAssetUsage : public intfSQLBasedModule
{
    Q_OBJECT

public:
    intfAccountAssetUsage(const QString& _schema,
                          const QList<DBM::clsORMField>& _exclusiveCols = {},
                          const QList<DBM::stuRelation>& _exclusiveRelations = {},
                          const QList<DBM::stuDBIndex>& _exclusiveIndexes = {});
private slots:
    QVariant ORMGET("Get user Usage on each Asset")
};

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
    QVariant ORMGET("Get Active Discounts")
    quint32 ORMCREATE("Create a new Discount by priviledged user")
    bool ORMUPDATE("Update a Discount info by priviledged user")
    bool ORMDELETE("Delete a Discount")
};

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
    QVariant ORMGET("Get Active Prizes")
    quint32 ORMCREATE("Create a new Prizes by priviledged user")
    bool ORMUPDATE("Update a Prizes info by priviledged user")
    bool ORMDELETE("Delete a Prizes")
};

} //namespace Targoman::API::AAA

#endif // TARGOMAN_API_AAA_ACCOUNTING_INTERFACES_H
