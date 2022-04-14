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

#include "intfSQLBasedModule.h"

#include "Interfaces/DBM/clsTable.h"
#include "Interfaces/DBM/QueryBuilders.h"
using namespace Targoman::API::DBM;

namespace Targoman::API::API {

intfSQLBasedModule::intfSQLBasedModule(
        const QString& _module,
        const QString& _schema,
        const QString& _name,
        const QList<clsORMField>& _cols,
        const QList<stuRelation>& _relations,
        const QList<stuDBIndex>& _indexes,
        const QVariantMap& _dbProperties
    ) :
    intfPureModule(
        _module
    ),
    clsTable(
//        this->parentModuleName().length()
//            ? this->parentModuleName()
//        _module.length() ? _module
//            : moduleBaseName(),
        _schema,
        _name,
        _cols,
        _relations,
        _indexes,
        _dbProperties
    ) { ; }

intfSQLBasedModule::intfSQLBasedModule(
        const QString& _schema,
        const QString& _name,
        const QList<clsORMField>& _cols,
        const QList<stuRelation>& _relations,
        const QList<stuDBIndex>& _indexes,
        const QVariantMap& _dbProperties
    ) :
    intfSQLBasedModule(
        "",
        _schema,
        _name,
        _cols,
        _relations,
        _indexes,
        _dbProperties
    ) { ; }

QVariantMap intfSQLBasedModule::SelectOne(
    clsTable& _table,
    GET_METHOD_ARGS_IMPL_INTERNAL_CALL,
    const clsCondition& _extraFilters,
    quint16 _cacheTime,
    std::function<void(SelectQuery &_query)> _lambda_TouchQuery
) {
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
        .offset(_pageIndex * _pageSize)
        .limit(_pageSize)
        .setCacheTime(_cacheTime)
    ;

    if (_lambda_TouchQuery != nullptr)
        _lambda_TouchQuery(Query);

    QVariantMap Result = Query.one();

    _APICALLBOOM.addResponseHeaderNameToExpose(RESPONSE_HEADER_X_PAGINATION_TOTAL_COUNT);
    _APICALLBOOM.addResponseHeaderNameToExpose(RESPONSE_HEADER_X_PAGINATION_PAGE_COUNT);
    _APICALLBOOM.addResponseHeaderNameToExpose(RESPONSE_HEADER_X_PAGINATION_CURRENT_PAGE);
    _APICALLBOOM.addResponseHeaderNameToExpose(RESPONSE_HEADER_X_PAGINATION_PER_PAGE);

    _APICALLBOOM.addResponseHeader(RESPONSE_HEADER_X_PAGINATION_TOTAL_COUNT, QString::number(Result.count()));
    _APICALLBOOM.addResponseHeader(RESPONSE_HEADER_X_PAGINATION_PAGE_COUNT, QString::number(ceil((double)Result.count() / _pageSize)));
    _APICALLBOOM.addResponseHeader(RESPONSE_HEADER_X_PAGINATION_CURRENT_PAGE, QString::number(_pageIndex));
    _APICALLBOOM.addResponseHeader(RESPONSE_HEADER_X_PAGINATION_PER_PAGE, QString::number(_pageSize));

    return Result;
}

QVariantList intfSQLBasedModule::SelectAll(
    clsTable& _table,
    GET_METHOD_ARGS_IMPL_INTERNAL_CALL,
    const clsCondition& _extraFilters,
    quint16 _cacheTime,
    std::function<void(SelectQuery &_query)> _lambda_TouchQuery
) {
    Q_UNUSED(_reportCount)

    _table.prepareFiltersList();

    SelectQuery Query = SelectQuery(_table)
        .setPksByPath(_pksByPath)
        .offset(_pageIndex * _pageSize)
        .limit(_pageSize)
        .addCSVCols(_cols)
        .orderBy(_orderBy)
        .groupBy(_groupBy)
        .addFilters(_filters)
        .andWhere(_extraFilters)
        .setCacheTime(_cacheTime)
    ;

    if (_lambda_TouchQuery != nullptr)
        _lambda_TouchQuery(Query);

    QVariantList Result = Query.all();

    _APICALLBOOM.addResponseHeaderNameToExpose(RESPONSE_HEADER_X_PAGINATION_TOTAL_COUNT);
    _APICALLBOOM.addResponseHeaderNameToExpose(RESPONSE_HEADER_X_PAGINATION_PAGE_COUNT);
    _APICALLBOOM.addResponseHeaderNameToExpose(RESPONSE_HEADER_X_PAGINATION_CURRENT_PAGE);
    _APICALLBOOM.addResponseHeaderNameToExpose(RESPONSE_HEADER_X_PAGINATION_PER_PAGE);

    _APICALLBOOM.addResponseHeader(RESPONSE_HEADER_X_PAGINATION_TOTAL_COUNT, QString::number(Result.count()));
    _APICALLBOOM.addResponseHeader(RESPONSE_HEADER_X_PAGINATION_PAGE_COUNT, QString::number(ceil((double)Result.count() / _pageSize)));
    _APICALLBOOM.addResponseHeader(RESPONSE_HEADER_X_PAGINATION_CURRENT_PAGE, QString::number(_pageIndex));
    _APICALLBOOM.addResponseHeader(RESPONSE_HEADER_X_PAGINATION_PER_PAGE, QString::number(_pageSize));

    return Result;
}

TAPI::stuTable intfSQLBasedModule::SelectAllWithCount(
    clsTable& _table,
    GET_METHOD_ARGS_IMPL_INTERNAL_CALL,
    const clsCondition& _extraFilters,
    quint16 _cacheTime,
    std::function<void(SelectQuery &_query)> _lambda_TouchQuery
) {
    Q_UNUSED(_reportCount)

    _table.prepareFiltersList();

    SelectQuery Query = SelectQuery(_table)
        .setPksByPath(_pksByPath)
        .offset(_pageIndex * _pageSize)
        .limit(_pageSize)
        .addCSVCols(_cols)
        .orderBy(_orderBy)
        .groupBy(_groupBy)
        .addFilters(_filters)
        .andWhere(_extraFilters)
        .setCacheTime(_cacheTime)
    ;

    if (_lambda_TouchQuery != nullptr)
        _lambda_TouchQuery(Query);

    TAPI::stuTable Result = Query.allWithCount();

    _APICALLBOOM.addResponseHeaderNameToExpose(RESPONSE_HEADER_X_PAGINATION_TOTAL_COUNT);
    _APICALLBOOM.addResponseHeaderNameToExpose(RESPONSE_HEADER_X_PAGINATION_PAGE_COUNT);
    _APICALLBOOM.addResponseHeaderNameToExpose(RESPONSE_HEADER_X_PAGINATION_CURRENT_PAGE);
    _APICALLBOOM.addResponseHeaderNameToExpose(RESPONSE_HEADER_X_PAGINATION_PER_PAGE);

    _APICALLBOOM.addResponseHeader(RESPONSE_HEADER_X_PAGINATION_TOTAL_COUNT, QString::number(Result.TotalRows));
    _APICALLBOOM.addResponseHeader(RESPONSE_HEADER_X_PAGINATION_PAGE_COUNT, QString::number(ceil((double)Result.TotalRows / _pageSize)));
    _APICALLBOOM.addResponseHeader(RESPONSE_HEADER_X_PAGINATION_CURRENT_PAGE, QString::number(_pageIndex));
    _APICALLBOOM.addResponseHeader(RESPONSE_HEADER_X_PAGINATION_PER_PAGE, QString::number(_pageSize));

    return Result;
}

QVariant intfSQLBasedModule::Select(
    clsTable& _table,
    GET_METHOD_ARGS_IMPL_INTERNAL_CALL,
    const clsCondition& _extraFilters,
    quint16 _cacheTime,
    std::function<void(SelectQuery &_query)> _lambda_TouchQuery
) {
    if (_pksByPath.isEmpty()) {
        if (_reportCount)
            return this->SelectAllWithCount(
                        _table,
                        GET_METHOD_CALL_ARGS_INTERNAL_CALL_RAW,
                        _extraFilters,
                        _cacheTime,
                        _lambda_TouchQuery
                        )
                    .toVariant()
                ;

        return this->SelectAll(
                    _table,
                    GET_METHOD_CALL_ARGS_INTERNAL_CALL_RAW,
                    _extraFilters,
                    _cacheTime,
                    _lambda_TouchQuery
                    );
    }

    return this->SelectOne(
                _table,
                GET_METHOD_CALL_ARGS_INTERNAL_CALL_RAW,
                _extraFilters,
                _cacheTime,
                _lambda_TouchQuery
                );
}

quint64 intfSQLBasedModule::Create(
    clsTable& _table,
    CREATE_METHOD_ARGS_IMPL_INTERNAL_CALL
) {
    _table.prepareFiltersList();

    CreateQuery query = CreateQuery(_table);

    for (QVariantMap::const_iterator arg = _createInfo.constBegin(); arg != _createInfo.constEnd(); ++arg)
        query.addCol(arg.key());

    query.values(_createInfo);

    return query.execute(_userID);
}

bool intfSQLBasedModule::Update(
    clsTable& _table,
    UPDATE_METHOD_ARGS_IMPL_INTERNAL_CALL,
    const QVariantMap& _extraFilters
) {
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

bool intfSQLBasedModule::DeleteByPks(
    clsTable& _table,
    DELETE_METHOD_ARGS_IMPL_INTERNAL_CALL,
    const QVariantMap& _extraFilters,
    bool _realDelete
) {
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




quint64 intfSQLBasedModule::Create(
    clsTable& _table,
    CREATE_METHOD_ARGS_IMPL_INTERNAL_CALL_1
) {
    return this->Create(
        _table,
        CREATE_METHOD_CALL_ARGS_INTERNAL_CALL
    );
}

bool intfSQLBasedModule::Update(
    clsTable& _table,
    UPDATE_METHOD_ARGS_IMPL_INTERNAL_CALL_1,
    const QVariantMap& _extraFilters
) {
    return this->Update(
        _table,
        UPDATE_METHOD_CALL_ARGS_INTERNAL_CALL,
        _extraFilters
    );
}

bool intfSQLBasedModule::DeleteByPks(
    clsTable& _table,
    DELETE_METHOD_ARGS_IMPL_INTERNAL_CALL_1,
    const QVariantMap& _extraFilters,
    bool _realDelete
) {
    return this->DeleteByPks(
        _table,
        DELETE_METHOD_CALL_ARGS_INTERNAL_CALL,
        _extraFilters,
        _realDelete
    );
}

} // namespace Targoman::API::API
