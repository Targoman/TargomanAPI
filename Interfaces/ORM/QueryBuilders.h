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
 * @author S. Mehran M. Ziabary <ziabary@targoman.com>
 * @author Kambiz Zandi <kambizzandi@gmail.com>
 */

#ifndef TARGOMAN_API_ORM_QUERYBUILDERS_H
#define TARGOMAN_API_ORM_QUERYBUILDERS_H

class TestQueryBuilders;

#include "Interfaces/ORM/Defs.hpp"
#include "libTargomanCommon/exTargomanBase.h"
#include "Interfaces/Common/GenericTypes.h"

#define DBNULLVALUE "NULL"

namespace Targoman {
namespace API {
namespace ORM {

class clsTable;
TARGOMAN_ADD_EXCEPTION_HANDLER(exQueryBuilder, Common::exTargomanBase);

TARGOMAN_DEFINE_ENUM(enuPreConditionOperator,
                     AND,
                     OR,
                     XOR)

TARGOMAN_DEFINE_ENUM(enuConditionOperator,
                     Equal,
                     NotEqual,
                     Greater,
                     GreaterEqual,
                     Less,
                     LessEqual,
                     Null,
                     NotNull,
                     Like)

class clsConditionData;
class clsCondition {
public:
    clsCondition();
    clsCondition(const clsCondition& _other);
    clsCondition(
            QString _col,
            enuConditionOperator::Type _operator,
            QVariant _value = {});
    clsCondition(
            QString _tableNameOrAlias,
            QString _col,
            enuConditionOperator::Type _operator,
            QVariant _value = {});
    clsCondition(
            QString _leftHandTableNameOrAlias,
            QString _leftHandCol,
            enuConditionOperator::Type _operator,
            QString _rightHandTableNameOrAlias,
            QString _rightHandCol);
    ~clsCondition();

    clsCondition& andCond(const clsCondition& _cond);
    clsCondition& orCond(const clsCondition& _cond);
    clsCondition& xorCond(const clsCondition& _cond);

    bool isEmpty() const;
    bool hasMany() const;

    QString buildConditionString(
            const QString &_mainTableNameOrAlias,
            const QMap<QString, stuRelatedORMField>& _filterables,
            bool _allowUseColumnAlias = false,
            quint8 _prettifierJustifyLen = 0) const;

protected:
    QSharedDataPointer<clsConditionData> Data;
    void addCondition(enuPreConditionOperator::Type _aggregator, const clsCondition& _nextCondition);

//    friend clsSelectQueryData;
    friend TestQueryBuilders;
};
/*
class clsJoinCondition : public clsCondition {
public:
    clsJoinCondition();
    ~clsJoinCondition();
    clsJoinCondition(const clsJoinCondition& _other);
//    clsJoinCondition(QString _col, enuConditionOperator::Type _operator, QVariant _value = {});
    clsJoinCondition(
            QString _tableName,
            QString _col,
            enuConditionOperator::Type _operator,
            QVariant _value = {});
    clsJoinCondition(
            QString _leftHandTableName,
            QString _leftHandCol,
            enuConditionOperator::Type _operator,
            QString _rightHandTableName,
            QString _rightHandCol);

    clsJoinCondition& andCond(const clsJoinCondition& _cond);
    clsJoinCondition& orCond(const clsJoinCondition& _cond);
    clsJoinCondition& xorCond(const clsJoinCondition& _cond);

    friend clsSelectQueryData;
    friend TestQueryBuilders;
};
*/

TARGOMAN_DEFINE_ENUM(enuAggregation,
    COUNT,
    DISTINCT_COUNT,
    SUM,
    AVG,
    MAX,
    MIN)

TARGOMAN_DEFINE_ENUM(enuConditionalAggregation,
    COUNTIF,
    SUMIF,
    AVGIF,
    MAXIF,
    MINIF)

TARGOMAN_DEFINE_ENUM(enuJoinType,
    LEFT,
    INNER,
    RIGHT,
    CROSS)

TARGOMAN_DEFINE_ENUM(enuOrderDir,
    Ascending,
    Descending)

class clsBaseQueryData;

template <class TDerrived, class TData>
class BaseQuery
{
public:
    BaseQuery(const BaseQuery<TDerrived, TData>& _other);
    BaseQuery(const clsTable& _table, const QString& _alias = {});
    ~BaseQuery();

    TDerrived& join(enuJoinType::Type _joinType, const QString& _foreignTable, const QString& _alias = {}, const clsCondition& _on = {});

    TDerrived& leftJoin(const QString& _foreignTable, const clsCondition& _on = {});
    TDerrived& leftJoin(const QString& _foreignTable, const QString& _alias, const clsCondition& _on = {});
    TDerrived& rightJoin(const QString& _foreignTable, const clsCondition& _on = {});
    TDerrived& rightJoin(const QString& _foreignTable, const QString& _alias, const clsCondition& _on = {});
    TDerrived& innerJoin(const QString& _foreignTable, const clsCondition& _on = {});
    TDerrived& innerJoin(const QString& _foreignTable, const QString& _alias, const clsCondition& _on = {});
    TDerrived& crossJoin(const QString& _foreignTable, const QString& _alias = {});

//    TDerrived& leftJoin(const BaseQuery& _nestedQuery, const QString _alias, const clsCondition& _on);
//    TDerrived& rightJoin(const BaseQuery& _nestedQuery, const QString _alias, const clsCondition& _on);
//    TDerrived& innerJoin(const BaseQuery& _nestedQuery, const QString _alias, const clsCondition& _on);
//    TDerrived& crossJoin(const BaseQuery& _nestedQuery, const QString _alias);
//    TDerrived& join(enuJoinType::Type _joinType, const BaseQuery& _nestedQuery, const QString _alias, const clsCondition& _on);

    TDerrived& joinWith(enuJoinType::Type _joinType, const QString& _relationName, const QString& _alias = {});
    TDerrived& leftJoinWith(const QString& _relationName, const QString& _alias = {});
    TDerrived& rightJoinWith(const QString& _relationName, const QString& _alias = {});
    TDerrived& innerJoinWith(const QString& _relationName, const QString& _alias = {});

    TDerrived& where(const clsCondition& _condition);
    TDerrived& andWhere(const clsCondition& _condition);
    TDerrived& orWhere(const clsCondition& _condition);
    TDerrived& xorWhere(const clsCondition& _condition);

//    TDerrived& groupBy(const clsCondition& _condition);
    TDerrived& groupBy(const QString& _col);
    TDerrived& groupBy(const QStringList& _cols);

    TDerrived& having(const clsCondition& _condition);
    TDerrived& andHaving(const clsCondition& _condition);
    TDerrived& orHaving(const clsCondition& _condition);
    TDerrived& xorHaving(const clsCondition& _condition);

protected:
    QSharedDataPointer<TData> Data;
    virtual QString buildQueryString(QVariantMap _args = {}, bool _selectOne = false, bool _reportCount = false, quint8 _prettifierJustifyLen = 0) = 0;
    friend TestQueryBuilders;
};

class clsSelectQueryData;

class SelectQuery : public BaseQuery<SelectQuery, clsSelectQueryData>
{
public:
    SelectQuery(const SelectQuery& _other);
    SelectQuery(const clsTable& _table, const QString& _alias = {});
    ~SelectQuery();

    SelectQuery& addCols(const TAPI::Cols_t& _commaSeperatedCols, const QString& _seperator=","); //-> used by APPLY_GET_METHOD_CALL_ARGS_TO_QUERY
    SelectQuery& addCols(const QStringList& _cols);
    SelectQuery& addCol(const QString& _col, const QString& _renameAs = {});
    SelectQuery& addCol(enuAggregation::Type _aggFunc, const QString& _col, const QString& _renameAs = {});
    SelectQuery& addCol(enuConditionalAggregation::Type _aggFunc, const clsCondition& _condition, const QString& _renameAs = {});
    SelectQuery& addCol(enuConditionalAggregation::Type _aggFunc, const clsCondition& _condition, QVariant _trueValue, QVariant _falseValue, const QString& _renameAs = {});

//    SelectQuery& addCols(const QString& _tableAlias, const TAPI::Cols_t& _cols); //-> used by APPLY_GET_METHOD_CALL_ARGS_TO_QUERY
//    SelectQuery& addCols(const QString& _tableAlias, const QStringList& _cols);
//    SelectQuery& addCol(const QString& _tableAlias, const QString& _col, const QString& _renameAs = {});
//    SelectQuery& addCol(enuAggregation::Type _aggFunc, const QString& _tableAlias, const QString& _col, const QString& _renameAs = {});

//    SelectQuery& from(const QString _table, const QString& _renameAs = {});
//    SelectQuery& from(const SelectQuery& _nestedQuery, const QString _alias);

    SelectQuery& orderBy(const QString& _col, enuOrderDir::Type _dir = enuOrderDir::Ascending);

    SelectQuery& pksByPath(TAPI::PKsByPath_t _pksByPath); //-> used by APPLY_GET_METHOD_CALL_ARGS_TO_QUERY
    SelectQuery& offset(quint64 _offset); //-> used by APPLY_GET_METHOD_CALL_ARGS_TO_QUERY
    SelectQuery& limit(quint16 _limit); //-> used by APPLY_GET_METHOD_CALL_ARGS_TO_QUERY

    QVariantMap one(QVariantMap _args = {});
    TAPI::stuTable all(QVariantMap _args = {}, quint16 _maxCount = 100, quint64 _from = 0);
    quint64 count(QVariantMap _args = {});

private:
//    QSharedDataPointer<clsSelectQueryData> Data;
    QString buildQueryString(QVariantMap _args = {}, bool _selectOne = false, bool _reportCount = false, quint8 _prettifierJustifyLen = 0);
    friend TestQueryBuilders;
};

class ApiSelectQuery : public SelectQuery
{
public:
    ApiSelectQuery(const clsTable& _table, GET_METHOD_ARGS_IMPL_WOJWT) :
        SelectQuery(_table)
    {
        this->pksByPath(_pksByPath);
        this->offset(_offset);
        this->limit(_limit);
        this->addCols(_cols);
        this->orderBy(_orderBy);
        this->groupBy(_groupBy);

        Q_UNUSED(_filters);
        Q_UNUSED(_reportCount);
    }
};

class clsUpdateQueryData;

class UpdateQuery : public BaseQuery<UpdateQuery, clsUpdateQueryData>
{
public:
    UpdateQuery(const UpdateQuery& _other);
    UpdateQuery(const clsTable& _table, const QString& _alias = {});
    ~UpdateQuery();

public:
//    UpdateQuery& Select(const SelectQuery& _selectClause);
    UpdateQuery& setNull(const QString& _col);
    UpdateQuery& set(const QString& _col, const QVariant& _value);
    UpdateQuery& set(const QString& _col, const QString& _otherTable, const QString& _otherCol);

    quint32 execute(QVariantMap _args = {});

private:
    QString buildQueryString(QVariantMap _args = {}, bool _selectOne = false, bool _reportCount = false, quint8 _prettifierJustifyLen = 0);
    friend TestQueryBuilders;
};

//typedef BaseQuery<SelectQuery, clsSelectQueryData> SelectQuery;

}
}
}
#endif // TARGOMAN_API_ORM_QUERYBUILDERS_H
