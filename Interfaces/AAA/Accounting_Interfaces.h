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
 */

#ifndef TARGOMAN_API_AAA_ACCOUNTING_INTERFACES_H
#define TARGOMAN_API_AAA_ACCOUNTING_INTERFACES_H

#include "Interfaces/AAA/Accounting_Defs.hpp"
#include "Interfaces/ORM/clsRESTAPIWithActionLogs.h"

namespace Targoman {
namespace API {
namespace AAA {
namespace Accounting{

class intfAccountPackages: public ORM::clsTable
{
    Q_OBJECT
public:
    intfAccountPackages(const QString& _schema,
                        const QString& _name,
                        const QList<ORM::clsORMField>& _cols,
                        const QList<ORM::stuRelation>& _foreignKeys);

private slots:
    QVariant ORMGET("Get Available Packages")
    bool ORMDELETE("Delete a package")
    bool ORMUPDATE("Update a package info by priviledged user")
    quint32 ORMCREATE("Create a new package by priviledged user")
};

/******************************************************/
class intfAccountUsage: public ORM::clsTable
{
    Q_OBJECT
public:
    intfAccountUsage(const QString& _schema,
                     const QString& _name,
                     const QList<ORM::clsORMField>& _cols,
                     const QList<ORM::stuRelation>& _foreignKeys);
private slots:
    QVariant ORMGET("Get User Usage on each package")
};

/******************************************************/
class intfAccountUserPackages: public ORM::clsTable
{
    Q_OBJECT
public:
    intfAccountUserPackages(const QString& _schema,
                            const QString& _name,
                            const QList<ORM::clsORMField>& _cols,
                            const QList<ORM::stuRelation>& _foreignKeys);
private slots:
    QVariant ORMGET("Get User Packages")
    bool REST(UPDATE, disablePackage, (TAPI::JWT_t _JWT, TAPI::PKsByPath_t _pksByPath),
              "Mark a user package banned by priviledged user")
    bool REST(UPDATE, setAsPrefered, (TAPI::JWT_t _JWT, TAPI::PKsByPath_t _pksByPath),
              "Mark a user package as prefered")
};

/******************************************************/
class intfAccountDiscounts: public ORM::clsTable
{
    Q_OBJECT
public:
    intfAccountDiscounts(const QString& _schema,
                         const QString& _name,
                         const QList<ORM::clsORMField>& _cols,
                         const QList<ORM::stuRelation>& _foreignKeys);
private slots:
    QVariant ORMGET("Get Active Discounts")
    bool ORMDELETE("Delete a Discount")
    bool ORMUPDATE("Update a Discount info by priviledged user")
    quint32 ORMCREATE("Create a new Discount by priviledged user")
};

class intfAccountPrizes: public ORM::clsTable
{
    Q_OBJECT
public:
    intfAccountPrizes(const QString& _schema,
                         const QString& _name,
                         const QList<ORM::clsORMField>& _cols,
                         const QList<ORM::stuRelation>& _foreignKeys);
private slots:
    QVariant ORMGET("Get Active Prizes")
    bool ORMDELETE("Delete a Prizes")
    bool ORMUPDATE("Update a Prizes info by priviledged user")
    quint32 ORMCREATE("Create a new Prizes by priviledged user")
};

}
}
}
}

#endif // TARGOMAN_API_AAA_ACCOUNTING_INTERFACES_H
