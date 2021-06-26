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

/***************************************************************************************/
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

/***************************************************************************************/
template <class TDerrived> class clsQueryJoinTraitData;
template <class TDerrived> class QueryJoinTrait;

template <class TDerrived> class clsQueryWhereTraitData;
template <class TDerrived> class QueryWhereTrait;

template <class TDerrived> class clsQueryGroupAndHavingTraitData;
template <class TDerrived> class QueryGroupAndHavingTrait;

template <class TDerrived> class clsBaseQueryData;

template <class TDerrived, class TData>
class BaseQuery
{
public:
    BaseQuery(const BaseQuery<TDerrived, TData>& _other);
    BaseQuery(clsTable& _table, const QString& _alias = {});
    ~BaseQuery();

protected:
    virtual void iAmAbstract() = 0;
    QSharedDataPointer<TData> Data;
    friend TestQueryBuilders;
    friend clsQueryJoinTraitData<TDerrived>;
    friend QueryJoinTrait<TDerrived>;
    friend clsQueryWhereTraitData<TDerrived>;
    friend QueryWhereTrait<TDerrived>;
    friend clsQueryGroupAndHavingTraitData<TDerrived>;
    friend QueryGroupAndHavingTrait<TDerrived>;
};

/***************************************************************************************/
template <class TDerrived>
class QueryJoinTrait
{
public:
    QueryJoinTrait(const QueryJoinTrait<TDerrived>& _other);
    QueryJoinTrait(const TDerrived* _owner);
    ~QueryJoinTrait();

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

protected:
    virtual void iAmAbstract() = 0;
    QSharedDataPointer<clsQueryJoinTraitData<TDerrived>> JoinTraitData;
    friend TestQueryBuilders;
};

/***************************************************************************************/
template <class TDerrived>
class QueryWhereTrait
{
public:
    QueryWhereTrait(const QueryWhereTrait<TDerrived>& _other);
    QueryWhereTrait(const TDerrived* _owner);
    ~QueryWhereTrait();

    TDerrived& where(const clsCondition& _condition);
    TDerrived& andWhere(const clsCondition& _condition);
    TDerrived& orWhere(const clsCondition& _condition);
    TDerrived& xorWhere(const clsCondition& _condition);

protected:
    virtual void iAmAbstract() = 0;
    QSharedDataPointer<clsQueryWhereTraitData<TDerrived>> WhereTraitData;
    friend TestQueryBuilders;
};

/***************************************************************************************/
template <class TDerrived>
class QueryGroupAndHavingTrait
{
public:
    QueryGroupAndHavingTrait(const QueryGroupAndHavingTrait<TDerrived>& _other);
    QueryGroupAndHavingTrait(const TDerrived* _owner);
    ~QueryGroupAndHavingTrait();

//    TDerrived& groupBy(const clsCondition& _condition);
    TDerrived& groupBy(const QString& _col);
    TDerrived& groupBy(const QStringList& _cols);

    TDerrived& having(const clsCondition& _condition);
    TDerrived& andHaving(const clsCondition& _condition);
    TDerrived& orHaving(const clsCondition& _condition);
    TDerrived& xorHaving(const clsCondition& _condition);

protected:
    virtual void iAmAbstract() = 0;
    QSharedDataPointer<clsQueryGroupAndHavingTraitData<TDerrived>> GroupAndHavingTraitData;
    friend TestQueryBuilders;
};

/***************************************************************************************/
class clsCreateQueryData;

class clsSelectQueryData;

class SelectQuery :
    public BaseQuery<SelectQuery, clsSelectQueryData>,
    public QueryJoinTrait<SelectQuery>,
    public QueryWhereTrait<SelectQuery>,
    public QueryGroupAndHavingTrait<SelectQuery>
{
public:
    SelectQuery(const SelectQuery& _other);
    SelectQuery(clsTable& _table, const QString& _alias = {});
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
    QVariantList all(QVariantMap _args = {}, quint16 _maxCount = 100, quint64 _from = 0);
    TAPI::stuTable allWithCount(QVariantMap _args = {}, quint16 _maxCount = 100, quint64 _from = 0);
//    quint64 count(QVariantMap _args = {});

private:
    virtual void iAmAbstract() {}
    QString buildQueryString(QVariantMap _args = {}, bool _selectOne = false, bool _reportCount = false, quint8 _prettifierJustifyLen = 0);
    friend clsCreateQueryData;
    friend TestQueryBuilders;
};

class ApiSelectQuery : public SelectQuery
{
public:
    ApiSelectQuery(clsTable& _table, GET_METHOD_ARGS_IMPL_WOJWT) :
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

/***************************************************************************************/
class CreateQuery :
    public BaseQuery<CreateQuery, clsCreateQueryData>
{
public:
    CreateQuery(const CreateQuery& _other);
    CreateQuery(clsTable& _table, const QString& _alias = {});
    ~CreateQuery();

public:
    CreateQuery& addCols(const QStringList& _cols);
    CreateQuery& addCol(const QString& _col);
    CreateQuery& values(const QVariantMap& _oneRecordValues);
    CreateQuery& values(const QList<QVariantMap>& _multipleRecordValues);
    CreateQuery& select(SelectQuery& _selectQuery);

    quint64 execute(QVariantMap _args = {});

private:
    virtual void iAmAbstract() {}
    QString buildQueryString(QVariantMap _args = {}, quint8 _prettifierJustifyLen = 0);
    friend TestQueryBuilders;
};

/***************************************************************************************/
class clsUpdateQueryData;

class UpdateQuery :
    public BaseQuery<UpdateQuery, clsUpdateQueryData>,
    public QueryJoinTrait<UpdateQuery>,
    public QueryWhereTrait<UpdateQuery>
{
public:
    UpdateQuery(const UpdateQuery& _other);
    UpdateQuery(clsTable& _table, const QString& _alias = {});
    ~UpdateQuery();

public:
//    UpdateQuery& Select(const SelectQuery& _selectClause);
    UpdateQuery& setNull(const QString& _col);
    UpdateQuery& set(const QString& _col, const QVariant& _value);
    UpdateQuery& set(const QString& _col, const QString& _otherTable, const QString& _otherCol);

    quint64 execute(QVariantMap _args = {});

private:
    virtual void iAmAbstract() {}
    QString buildQueryString(QVariantMap _args = {}, quint8 _prettifierJustifyLen = 0);
    friend TestQueryBuilders;
};

/***************************************************************************************/

//typedef BaseQuery<SelectQuery, clsSelectQueryData> SelectQuery;

}
}
}

#endif // TARGOMAN_API_ORM_QUERYBUILDERS_H
