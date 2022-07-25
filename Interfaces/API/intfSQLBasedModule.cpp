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
#include "Interfaces/AAA/AAADefs.hpp"
#include "Interfaces/DBM/clsTable.h"
#include "Interfaces/DBM/QueryBuilders.h"
#include "Interfaces/Helpers/IteratorHelper.hpp"
using namespace Targoman::API::AAA;
using namespace Targoman::API::DBM;
using namespace Targoman::API::Helpers;

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
    )
{ ; }

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
    )
{ ; }

QVariantMap intfSQLBasedModule::SelectOne(
    GET_METHOD_ARGS_IMPL_INTERNAL_BOOM,
    const clsCondition& _extraFilters,
    quint16 _cacheTime,
    std::function<void(ORMSelectQuery &_query)> _fnTouchQuery
) {
    Q_UNUSED(_reportCount)

    this->prepareFiltersList();

//    qDebug() << __FILE__ << ":" << __FUNCTION__ << "() :" << "filters:" << _filters;

    ORMSelectQuery Query = this->GetSelectQuery(APICALLBOOM_PARAM)
        .setPksByPath(_pksByPath)
        .addCSVCols(_cols)
        .orderBy(_orderBy)
        .groupBy(_groupBy)
        .addFilters(_filters)
        .andWhere(_extraFilters)
        .pageIndex(_pageIndex)
        .pageSize(_pageSize)
        .setCacheTime(_cacheTime)
    ;

    if (_fnTouchQuery != nullptr)
        _fnTouchQuery(Query);

    QVariantMap Result = Query.one();

//    APICALLBOOM_PARAM.addResponseHeaderNameToExpose(RESPONSE_HEADER_X_PAGINATION_TOTAL_COUNT);
//    APICALLBOOM_PARAM.addResponseHeaderNameToExpose(RESPONSE_HEADER_X_PAGINATION_PAGE_COUNT);
//    APICALLBOOM_PARAM.addResponseHeaderNameToExpose(RESPONSE_HEADER_X_PAGINATION_CURRENT_PAGE);
//    APICALLBOOM_PARAM.addResponseHeaderNameToExpose(RESPONSE_HEADER_X_PAGINATION_PER_PAGE);

//    APICALLBOOM_PARAM.addResponseHeader(RESPONSE_HEADER_X_PAGINATION_TOTAL_COUNT, QString::number(Result.count()));
//    APICALLBOOM_PARAM.addResponseHeader(RESPONSE_HEADER_X_PAGINATION_PAGE_COUNT, QString::number(ceil((double)Result.count() / _pageSize)));
//    APICALLBOOM_PARAM.addResponseHeader(RESPONSE_HEADER_X_PAGINATION_CURRENT_PAGE, QString::number(_pageIndex));
//    APICALLBOOM_PARAM.addResponseHeader(RESPONSE_HEADER_X_PAGINATION_PER_PAGE, QString::number(_pageSize));

    return Result;
}

QVariantList intfSQLBasedModule::SelectAll(
    GET_METHOD_ARGS_IMPL_INTERNAL_BOOM,
    const clsCondition& _extraFilters,
    quint16 _cacheTime,
    std::function<void(ORMSelectQuery &_query)> _fnTouchQuery
) {
    Q_UNUSED(_reportCount)

    this->prepareFiltersList();

    ORMSelectQuery Query = this->GetSelectQuery(APICALLBOOM_PARAM)
        .setPksByPath(_pksByPath)
        .pageIndex(_pageIndex)
        .pageSize(_pageSize)
        .addCSVCols(_cols)
        .orderBy(_orderBy)
        .groupBy(_groupBy)
        .addFilters(_filters)
        .andWhere(_extraFilters)
        .setCacheTime(_cacheTime)
    ;

    if (_fnTouchQuery != nullptr)
        _fnTouchQuery(Query);

    QVariantList Result = Query.all();

//    APICALLBOOM_PARAM.addResponseHeaderNameToExpose(RESPONSE_HEADER_X_PAGINATION_TOTAL_COUNT);
//    APICALLBOOM_PARAM.addResponseHeaderNameToExpose(RESPONSE_HEADER_X_PAGINATION_PAGE_COUNT);
//    APICALLBOOM_PARAM.addResponseHeaderNameToExpose(RESPONSE_HEADER_X_PAGINATION_CURRENT_PAGE);
//    APICALLBOOM_PARAM.addResponseHeaderNameToExpose(RESPONSE_HEADER_X_PAGINATION_PER_PAGE);

//    APICALLBOOM_PARAM.addResponseHeader(RESPONSE_HEADER_X_PAGINATION_TOTAL_COUNT, QString::number(Result.count()));
//    APICALLBOOM_PARAM.addResponseHeader(RESPONSE_HEADER_X_PAGINATION_PAGE_COUNT, QString::number(ceil((double)Result.count() / _pageSize)));
//    APICALLBOOM_PARAM.addResponseHeader(RESPONSE_HEADER_X_PAGINATION_CURRENT_PAGE, QString::number(_pageIndex));
//    APICALLBOOM_PARAM.addResponseHeader(RESPONSE_HEADER_X_PAGINATION_PER_PAGE, QString::number(_pageSize));

    return Result;
}

TAPI::stuTable intfSQLBasedModule::SelectAllWithCount(
    GET_METHOD_ARGS_IMPL_INTERNAL_BOOM,
    const clsCondition& _extraFilters,
    quint16 _cacheTime,
    std::function<void(ORMSelectQuery &_query)> _fnTouchQuery
) {
    Q_UNUSED(_reportCount)

    this->prepareFiltersList();

    ORMSelectQuery Query = this->GetSelectQuery(APICALLBOOM_PARAM)
        .setPksByPath(_pksByPath)
        .pageIndex(_pageIndex)
        .pageSize(_pageSize)
        .addCSVCols(_cols)
        .orderBy(_orderBy)
        .groupBy(_groupBy)
        .addFilters(_filters)
        .andWhere(_extraFilters)
        .setCacheTime(_cacheTime)
    ;

    if (_fnTouchQuery != nullptr)
        _fnTouchQuery(Query);

    TAPI::stuTable Result = Query.allWithCount();

//    APICALLBOOM_PARAM.addResponseHeaderNameToExpose(RESPONSE_HEADER_X_PAGINATION_TOTAL_COUNT);
//    APICALLBOOM_PARAM.addResponseHeaderNameToExpose(RESPONSE_HEADER_X_PAGINATION_PAGE_COUNT);
//    APICALLBOOM_PARAM.addResponseHeaderNameToExpose(RESPONSE_HEADER_X_PAGINATION_CURRENT_PAGE);
//    APICALLBOOM_PARAM.addResponseHeaderNameToExpose(RESPONSE_HEADER_X_PAGINATION_PER_PAGE);

//    APICALLBOOM_PARAM.addResponseHeader(RESPONSE_HEADER_X_PAGINATION_TOTAL_COUNT, QString::number(Result.TotalRows));
//    APICALLBOOM_PARAM.addResponseHeader(RESPONSE_HEADER_X_PAGINATION_PAGE_COUNT, QString::number(ceil((double)Result.TotalRows / _pageSize)));
//    APICALLBOOM_PARAM.addResponseHeader(RESPONSE_HEADER_X_PAGINATION_CURRENT_PAGE, QString::number(_pageIndex));
//    APICALLBOOM_PARAM.addResponseHeader(RESPONSE_HEADER_X_PAGINATION_PER_PAGE, QString::number(_pageSize));

    return Result;
}

QVariant intfSQLBasedModule::Select(
    GET_METHOD_ARGS_IMPL_INTERNAL_BOOM,
    const clsCondition& _extraFilters,
    quint16 _cacheTime,
    std::function<void(ORMSelectQuery &_query)> _fnTouchQuery
) {
    auto ServerTiming = APICALLBOOM_PARAM.createScopeTiming("db", "read");

    if (_pksByPath.isEmpty()) {
        if (_reportCount)
            return this->SelectAllWithCount(
                        GET_METHOD_ARGS_CALL_INTERNAL_BOOM,
                        _extraFilters,
                        _cacheTime,
                        _fnTouchQuery
                        )
                    .toVariant()
                ;

        return this->SelectAll(
                    GET_METHOD_ARGS_CALL_INTERNAL_BOOM,
                    _extraFilters,
                    _cacheTime,
                    _fnTouchQuery
                    );
    }

    return this->SelectOne(
                GET_METHOD_ARGS_CALL_INTERNAL_BOOM,
                _extraFilters,
                _cacheTime,
                _fnTouchQuery
                );
}

quint64 intfSQLBasedModule::Create(
    CREATE_METHOD_ARGS_IMPL_INTERNAL_BOOM
) {
    auto ServerTiming = APICALLBOOM_PARAM.createScopeTiming("db", "create");

    this->prepareFiltersList();

    TAPI::ORMFields_t CreateInfo;
    //key: language
    QMap<QString, TAPI::ORMFields_t> TranslateCreateInfo;

    //1: extract _translate fields
    TAPI::ORMFields_t::iterator it = _createInfo.begin();
    while (it != _createInfo.end()) {
        if (it.key().endsWith("_translate")) {
            QString ColName = it.key().chopped(QString("_translate").length());

            QVariantMap ValuesByLanguage = it.value().toMap();
            it = _createInfo.erase(it);

            IteratorHelper::ConstIterator(ValuesByLanguage)
                    .runAll([&ColName, &TranslateCreateInfo](QString _key, QVariant _value) -> bool {
                        TranslateCreateInfo[_key].insert(ColName, _value);
                        return true;
                    });

            continue;
        } else
            it++;
    }

    if ((TranslateCreateInfo.isEmpty() == false) && (clsTable::Registry.contains(this->nameWithSchema() + "_translate") == false))
        throw exHTTPBadRequest(QString("Table %1 cannot be translated").arg(this->nameWithSchema()));

    //2: run wo/ _translate
    ORMCreateQuery query = this->GetCreateQuery(APICALLBOOM_PARAM);

    for (QVariantMap::const_iterator arg = _createInfo.constBegin(); arg != _createInfo.constEnd(); ++arg)
        query.addCol(arg.key());

    query.values(_createInfo);

    quint64 LastID = query.execute(APICALLBOOM_PARAM.getUserID(SYSTEM_USER_ID));

    //3: run _translate
    if ((LastID > 0) && (TranslateCreateInfo.isEmpty() == false)) {
        intfSQLBasedModule* TranslateModule = dynamic_cast<intfSQLBasedModule*>(clsTable::Registry[this->nameWithSchema() + "_translate"]);
        if (TranslateModule) {
            IteratorHelper::ConstIterator(TranslateCreateInfo)
                    .runAll([&TranslateModule, &APICALLBOOM_PARAM, LastID](QString _key, QVariant _value) -> bool {
                        TAPI::ORMFields_t TranslateInfo;
                        TranslateInfo.insert("pid", LastID);
                        TranslateInfo.insert("language", _key);

                        IteratorHelper::ConstIterator(_value.toMap())
                                .runAll([&TranslateInfo](QString _key, QVariant _value) -> bool {
                                    TranslateInfo.insert(_key, _value);
                                    return true;
                                });

                        /*quint64 TranslateLastID = */TranslateModule->Create(
                                                      APICALLBOOM_PARAM,
                                                      TranslateInfo
                                                      );

                        return true;
                    });
        }
    }

    //--
    return LastID;
}

bool intfSQLBasedModule::Update(
    UPDATE_METHOD_ARGS_IMPL_INTERNAL_BOOM,
    const QVariantMap& _extraFilters
) {
    auto ServerTiming = APICALLBOOM_PARAM.createScopeTiming("db", "update");

    this->prepareFiltersList();

    if (_pksByPath.isEmpty() && _extraFilters.isEmpty())
        throw exHTTPBadRequest("No key provided to update");

    if (_updateInfo.isEmpty())
        throw exHTTPBadRequest("No change provided to update");

    TAPI::ORMFields_t UpdateInfo;
    //key: language
    QMap<QString, TAPI::ORMFields_t> TranslateUpdateInfo;

    //1: extract _translate fields
    TAPI::ORMFields_t::iterator it = _updateInfo.begin();
    while (it != _updateInfo.end()) {
        if (it.key().endsWith("_translate")) {
            QString ColName = it.key().chopped(QString("_translate").length());

            QVariantMap ValuesByLanguage = it.value().toMap();
            it = _updateInfo.erase(it);

            IteratorHelper::ConstIterator(ValuesByLanguage)
                    .runAll([&ColName, &TranslateUpdateInfo](QString _key, QVariant _value) -> bool {
                        TranslateUpdateInfo[_key].insert(ColName, _value);
                        return true;
                    });

            continue;
        } else
            it++;
    }

    if ((TranslateUpdateInfo.isEmpty() == false) && (clsTable::Registry.contains(this->nameWithSchema() + "_translate") == false))
        throw exHTTPBadRequest(QString("Table %1 cannot be translated").arg(this->nameWithSchema()));

    //2: run wo/ _translate
    ORMUpdateQuery query = this->GetUpdateQuery(APICALLBOOM_PARAM)
        .setPksByPath(_pksByPath)
//        .addFilters(_extraFilters)
    ;

    for (auto FilterIter = _extraFilters.begin(); FilterIter != _extraFilters.end(); FilterIter++) {
        if (FilterIter->isValid() == false)
            continue;
        const stuRelatedORMField& relatedORMField = this->SelectableColsMap[FilterIter.key()];
        if (relatedORMField.Col.isFilterable() == false)
            throw exHTTPInternalServerError("Invalid non-filterable column <" + FilterIter.key() + ">");
        query.andWhere({ relatedORMField.Col.name(), enuConditionOperator::Equal, FilterIter.value() });
    }

    for (QVariantMap::const_iterator arg = _updateInfo.constBegin(); arg != _updateInfo.constEnd(); ++arg) {
        if (arg->isValid()) {
            query.set(arg.key(), arg.value());
        }
    }

    quint64 NumRowsAffected = query.execute(APICALLBOOM_PARAM.getUserID(SYSTEM_USER_ID)) > 0;

    //3: run _translate
    if ((NumRowsAffected > 0) && (TranslateUpdateInfo.isEmpty() == false)) {
        intfSQLBasedModule* TranslateModule = dynamic_cast<intfSQLBasedModule*>(clsTable::Registry[this->nameWithSchema() + "_translate"]);
        if (TranslateModule) {
            IteratorHelper::ConstIterator(TranslateUpdateInfo)
                    .runAll([&TranslateModule, &APICALLBOOM_PARAM, _pksByPath](QString _key, QVariant _value) -> bool {
                        TAPI::ORMFields_t TranslateInfo;
//                        TranslateInfo.insert("pid", _pksByPath);
//                        TranslateInfo.insert("language", _key);

                        IteratorHelper::ConstIterator(_value.toMap())
                                .runAll([&TranslateInfo](QString _key, QVariant _value) -> bool {
                                    TranslateInfo.insert(_key, _value);
                                    return true;
                                });

                        //PK: id,language
                        TAPI::PKsByPath_t TranslatePKs = QString("%1,%2").arg(_pksByPath).arg(_key);

                        /*quint64 TranslateRowsCount = */TranslateModule->Update(
                                                         APICALLBOOM_PARAM,
                                                         TranslatePKs,
                                                         TranslateInfo
                                                         );

                        return true;
                    });
        }
    }

    //--
    return NumRowsAffected;
}

bool intfSQLBasedModule::DeleteByPks(
    DELETE_METHOD_ARGS_IMPL_INTERNAL_BOOM,
    const QVariantMap& _extraFilters,
    bool _realDelete
) {
    auto ServerTiming = APICALLBOOM_PARAM.createScopeTiming("db", "delete");

    this->prepareFiltersList();

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

    ORMDeleteQuery query = this->GetDeleteQuery(APICALLBOOM_PARAM)
        .setPksByPath(_pksByPath)
//        .addFilters(_extraFilters)
    ;

    for (auto FilterIter = _extraFilters.begin(); FilterIter != _extraFilters.end(); FilterIter++) {
        if (FilterIter->isValid() == false)
            continue;
        const stuRelatedORMField& relatedORMField = this->SelectableColsMap[FilterIter.key()];
        if (relatedORMField.Col.isFilterable() == false)
            throw exHTTPInternalServerError("Invalid non-filterable column <" + FilterIter.key() + ">");
        query.andWhere({ relatedORMField.Col.name(), enuConditionOperator::Equal, FilterIter.value() });
    }

    return query.execute(APICALLBOOM_PARAM.getUserID(SYSTEM_USER_ID), {}, _realDelete) > 0;
}

} // namespace Targoman::API::API
