/******************************************************************************
 * TargomanAAA: Authentication, Authorization, Accounting framework           *
 *                                                                            *
 * Copyright 2014-2020 by Targoman Intelligent Processing <http://tip.co.ir>  *
 *                                                                            *
 * TargomanAAA is free software: you can redistribute it and/or modify        *
 * it under the terms of the GNU Lesser General Public License as published   *
 * by the Free Software Foundation, either version 3 of the License, or       *
 * (at your option) any later version.                                        *
 *                                                                            *
 * TargomanAAA is distributed in the hope that it will be useful,             *
 * but WITHOUT ANY WARRANTY; without even the implied warranty of             *
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the              *
 * GNU Lesser General Public License for more details.                        *
 * You should have received a copy of the GNU Lesser General Public License   *
 * along with Targoman. If not, see <http://www.gnu.org/licenses/>.           *
 *                                                                            *
 ******************************************************************************/
/**
 * @author Kambiz Zandi <kambizzandi@gmail.com>
 */

#include "APIQueryBuilders.h"

#include "Interfaces/DBM/clsTable.h"
using namespace Targoman::API::DBM;

#include "Interfaces/AAA/clsJWT.hpp"
using namespace Targoman::API::AAA;

namespace Targoman::API::Query {

QVariantMap SelectOne(clsTable& _table, GET_METHOD_ARGS_IMPL_INTERNAL_CALL, const clsCondition& _extraFilters, quint16 _cacheTime, std::function<void(SelectQuery &_query)> _lambda_TouchQuery) {
    Q_UNUSED(_userID)
    Q_UNUSED(_reportCount)

    _table.prepareFiltersList();

//    qDebug() << __FILE__ << ":" << __FUNCTION__ << "() :" << "filters:" << _filters;

    SelectQuery Query = SelectQuery(_table)
        .setPksByPath(_pksByPath)
        .addCSVCols(_cols)
        .orderBy(_orderBy)
        .groupBy(_groupBy)
        .addFilters(_filters)
        .andWhere(_extraFilters)
        .offset(_offset)
        .limit(_limit)
        .setCacheTime(_cacheTime)
    ;

    if (_lambda_TouchQuery != nullptr)
        _lambda_TouchQuery(Query);

    return Query.one();
}

QVariantList SelectAll(clsTable& _table, GET_METHOD_ARGS_IMPL_INTERNAL_CALL, const clsCondition& _extraFilters, quint16 _cacheTime, std::function<void(SelectQuery &_query)> _lambda_TouchQuery) {
    Q_UNUSED(_userID)
    Q_UNUSED(_reportCount)

    _table.prepareFiltersList();

    SelectQuery Query = SelectQuery(_table)
        .setPksByPath(_pksByPath)
        .offset(_offset)
        .limit(_limit)
        .addCSVCols(_cols)
        .orderBy(_orderBy)
        .groupBy(_groupBy)
        .addFilters(_filters)
        .andWhere(_extraFilters)
        .setCacheTime(_cacheTime)
    ;

    if (_lambda_TouchQuery != nullptr)
        _lambda_TouchQuery(Query);

    return Query.all();
}

TAPI::stuTable SelectAllWithCount(clsTable& _table, GET_METHOD_ARGS_IMPL_INTERNAL_CALL, const clsCondition& _extraFilters, quint16 _cacheTime, std::function<void(SelectQuery &_query)> _lambda_TouchQuery) {
    Q_UNUSED(_userID)
    Q_UNUSED(_reportCount)

    _table.prepareFiltersList();

    SelectQuery Query = SelectQuery(_table)
        .setPksByPath(_pksByPath)
        .offset(_offset)
        .limit(_limit)
        .addCSVCols(_cols)
        .orderBy(_orderBy)
        .groupBy(_groupBy)
        .addFilters(_filters)
        .andWhere(_extraFilters)
        .setCacheTime(_cacheTime)
    ;

    if (_lambda_TouchQuery != nullptr)
        _lambda_TouchQuery(Query);

    return Query.allWithCount();
}

QVariant Select(clsTable& _table, GET_METHOD_ARGS_IMPL_INTERNAL_CALL, const clsCondition& _extraFilters, quint16 _cacheTime, std::function<void(SelectQuery &_query)> _lambda_TouchQuery) {
    if (_pksByPath.isEmpty()) {
        if (_reportCount)
            return SelectAllWithCount(_table, GET_METHOD_CALL_ARGS_INTERNAL_CALL_RAW, _extraFilters, _cacheTime, _lambda_TouchQuery).toVariant();

        return SelectAll(_table, GET_METHOD_CALL_ARGS_INTERNAL_CALL_RAW, _extraFilters, _cacheTime, _lambda_TouchQuery);
    }

    return SelectOne(_table, GET_METHOD_CALL_ARGS_INTERNAL_CALL_RAW, _extraFilters, _cacheTime, _lambda_TouchQuery);
}

quint64 Create(clsTable& _table, CREATE_METHOD_ARGS_IMPL_INTERNAL_CALL) {
    _table.prepareFiltersList();

    CreateQuery query = CreateQuery(_table);

    for (QVariantMap::const_iterator arg = _createInfo.constBegin(); arg != _createInfo.constEnd(); ++arg)
        query.addCol(arg.key());

    query.values(_createInfo);

    return query.execute(_userID);
}

bool Update(clsTable& _table, UPDATE_METHOD_ARGS_IMPL_INTERNAL_CALL, const QVariantMap& _extraFilters) {
    _table.prepareFiltersList();

    if (_pksByPath.isEmpty() && _extraFilters.isEmpty())
        throw exHTTPBadRequest("No key provided to update");

    if (_updateInfo.isEmpty())
        throw exHTTPBadRequest("No change provided to update");

    UpdateQuery query = UpdateQuery(_table)
        .setPksByPath(_pksByPath)
//        .addFilters(_extraFilters)
    ;

    for (auto FilterIter = _extraFilters.begin(); FilterIter != _extraFilters.end(); FilterIter++) {
        if (FilterIter->isValid() == false)
            continue;
        const stuRelatedORMField& relatedORMField = _table.SelectableColsMap[FilterIter.key()];
        if (relatedORMField.Col.isFilterable() == false)
            throw exHTTPInternalServerError("Invalid non-filterable column <" + FilterIter.key() + ">");
        query.andWhere({ relatedORMField.Col.name(), enuConditionOperator::Equal, FilterIter.value() });
    }

    for (QVariantMap::const_iterator arg = _updateInfo.constBegin(); arg != _updateInfo.constEnd(); ++arg) {
        if (arg->isValid()) {
            query.set(arg.key(), arg.value());
        }
    }

    return query.execute(_userID) > 0;
}

bool DeleteByPks(clsTable& _table, DELETE_METHOD_ARGS_IMPL_INTERNAL_CALL, const QVariantMap& _extraFilters, bool _realDelete) {
    _table.prepareFiltersList();

    if (_pksByPath.isEmpty() && _extraFilters.isEmpty())
        throw exHTTPBadRequest("No key provided to delete");

//    QString statusColumn = _table.getStatusColumnName();
//    if (statusColumn.isEmpty() == false) {
//        if (Update(_table, _userID, _pksByPath, TAPI::ORMFields_t({
//                { statusColumn, "Removed" }
//            }), _extraFilters) == 0)
//            return false;
//    }

//    if (_realDelete == false)
//        return true;

    DeleteQuery query = DeleteQuery(_table)
        .setPksByPath(_pksByPath)
//        .addFilters(_extraFilters)
    ;

    for (auto FilterIter = _extraFilters.begin(); FilterIter != _extraFilters.end(); FilterIter++) {
        if (FilterIter->isValid() == false)
            continue;
        const stuRelatedORMField& relatedORMField = _table.SelectableColsMap[FilterIter.key()];
        if (relatedORMField.Col.isFilterable() == false)
            throw exHTTPInternalServerError("Invalid non-filterable column <" + FilterIter.key() + ">");
        query.andWhere({ relatedORMField.Col.name(), enuConditionOperator::Equal, FilterIter.value() });
    }

    return query.execute(_userID, {}, _realDelete) > 0;
}

} //namespace Targoman::API::Query
