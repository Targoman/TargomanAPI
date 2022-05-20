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

#ifndef TARGOMAN_API_API_INTFSQLBASEDMODULE_H
#define TARGOMAN_API_API_INTFSQLBASEDMODULE_H

#include "Interfaces/API/intfPureModule.h"
//#include "Interfaces/DBM/Defs.hpp"
#include "Interfaces/DBM/clsTable.h"
#include "Interfaces/DBM/QueryBuilders.h"
using namespace Targoman::API::DBM;

namespace Targoman::API::API {

class intfSQLBasedModule : public intfPureModule, public clsTable
{
public:
    intfSQLBasedModule(
            const QString& _module,
            const QString& _schema,
            const QString& _name,
            const QList<clsORMField>& _cols = {},
            const QList<stuRelation>& _relations = {},
            const QList<stuDBIndex>& _indexes = {},
            const QVariantMap& _dbProperties = {}
        );
    intfSQLBasedModule(
            const QString& _schema,
            const QString& _name,
            const QList<clsORMField>& _cols = {},
            const QList<stuRelation>& _relations = {},
            const QList<stuDBIndex>& _indexes = {},
            const QVariantMap& _dbProperties = {}
        );

private:
    QVariantMap SelectOne(
            clsTable& _table,
            GET_METHOD_ARGS_DECL_INTERNAL_USER,
            const clsCondition& _extraFilters = {},
            quint16 _cacheTime = 0,
            std::function<void(SelectQuery &_query)> _lambda_TouchQuery = nullptr
        );
    QVariantList SelectAll(
            clsTable& _table,
            GET_METHOD_ARGS_IMPL_INTERNAL_USER,
            const clsCondition& _extraFilters = {},
            quint16 _cacheTime = 0,
            std::function<void(SelectQuery &_query)> _lambda_TouchQuery = nullptr
        );
    TAPI::stuTable SelectAllWithCount(
            clsTable& _table,
            GET_METHOD_ARGS_IMPL_INTERNAL_USER,
            const clsCondition& _extraFilters = {},
            quint16 _cacheTime = 0,
            std::function<void(SelectQuery &_query)> _lambda_TouchQuery = nullptr
        );

public:
    QVariant Select(
            clsTable& _table,
            GET_METHOD_ARGS_DECL_INTERNAL_USER,
            const clsCondition& _extraFilters = {},
            quint16 _cacheTime = 0,
            std::function<void(SelectQuery &_query)> _lambda_TouchQuery = nullptr
        );
    QVariant Select(
            GET_METHOD_ARGS_DECL_INTERNAL_USER,
            const clsCondition& _extraFilters = {},
            quint16 _cacheTime = 0,
            std::function<void(SelectQuery &_query)> _lambda_TouchQuery = nullptr
        );

    quint64 Create(
            clsTable& _table,
            CREATE_METHOD_ARGS_DECL_INTERNAL_USER
        );
    quint64 Create(
            CREATE_METHOD_ARGS_DECL_INTERNAL_USER
        );
    quint64 Create(
            clsTable& _table,
            CREATE_METHOD_ARGS_DECL_INTERNAL_BOOM
        );
    quint64 Create(
            CREATE_METHOD_ARGS_DECL_INTERNAL_BOOM
        );

    bool Update(
            clsTable& _table,
            UPDATE_METHOD_ARGS_DECL_INTERNAL_USER,
            const QVariantMap& _extraFilters = {}
        );
    bool Update(
            UPDATE_METHOD_ARGS_DECL_INTERNAL_USER,
            const QVariantMap& _extraFilters = {}
        );
    bool Update(
            clsTable& _table,
            UPDATE_METHOD_ARGS_DECL_INTERNAL_BOOM,
            const QVariantMap& _extraFilters = {}
        );
    bool Update(
            UPDATE_METHOD_ARGS_DECL_INTERNAL_BOOM,
            const QVariantMap& _extraFilters = {}
        );

    bool DeleteByPks(
            clsTable& _table,
            DELETE_METHOD_ARGS_DECL_INTERNAL_USER,
            const QVariantMap& _extraFilters = {},
            bool _realDelete = false
        );
    bool DeleteByPks(
            DELETE_METHOD_ARGS_DECL_INTERNAL_USER,
            const QVariantMap& _extraFilters = {},
            bool _realDelete = false
        );
    bool DeleteByPks(
            clsTable& _table,
            DELETE_METHOD_ARGS_DECL_INTERNAL_BOOM,
            const QVariantMap& _extraFilters = {},
            bool _realDelete = false
        );
    bool DeleteByPks(
            DELETE_METHOD_ARGS_DECL_INTERNAL_BOOM,
            const QVariantMap& _extraFilters = {},
            bool _realDelete = false
        );

};

} // namespace Targoman::API::API

#endif // TARGOMAN_API_API_INTFSQLBASEDMODULE_H
