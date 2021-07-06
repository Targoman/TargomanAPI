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

#include <qglobal.h>

#ifndef SQLPrettyLen
#ifdef QT_DEBUG
#define SQLPrettyLen 18
#else
#define SQLPrettyLen 0
#endif
#endif

#include "libTargomanDBM/clsDAC.h"
using namespace Targoman::DBManager;

class TestQueryBuilders;

#include "Interfaces/ORM/Defs.hpp"
#include "libTargomanCommon/exTargomanBase.h"
#include "Interfaces/Common/GenericTypes.h"

namespace Targoman {
namespace API {
namespace ORM {

class clsTable;
class clsCreateQueryData;
class clsSelectQueryData;

TARGOMAN_ADD_EXCEPTION_HANDLER(exQueryBuilder, Common::exTargomanBase);
TARGOMAN_ADD_EXCEPTION_HANDLER(exQueryBuilderColumnNotFound, Common::exTargomanBase);
TARGOMAN_ADD_EXCEPTION_HANDLER(exQueryBuilderColumnNotProvided, Common::exTargomanBase);
TARGOMAN_ADD_EXCEPTION_HANDLER(exQueryBuilderValueNotProvided, Common::exTargomanBase);
TARGOMAN_ADD_EXCEPTION_HANDLER(exQueryBuilderWhereClauseNotProvided, Common::exTargomanBase);

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

struct stuBoundQueryString {
    QString QueryString;
//    QVariantMap BindingValues;
    QVariantList BindingValues;
};

/***************************************************************************************/
TARGOMAN_DEFINE_ENUM(enuDBExpressionType,
                     Value,
                     Function)

TARGOMAN_DEFINE_ENUM(enuDBExpressionIntervalUnit,
                     MICROSECOND,         // MICROSECONDS
                     SECOND,              // SECONDS
                     MINUTE,              // MINUTES
                     HOUR,                // HOURS
                     DAY,                 // DAYS
                     WEEK,                // WEEKS
                     MONTH,               // MONTHS
                     QUARTER,             // QUARTERS
                     YEAR,                // YEARS
                     SECOND_MICROSECOND,  // 'SECONDS.MICROSECONDS'
                     MINUTE_MICROSECOND,  // 'MINUTES:SECONDS.MICROSECONDS'
                     MINUTE_SECOND,       // 'MINUTES:SECONDS'
                     HOUR_MICROSECOND,    // 'HOURS:MINUTES:SECONDS.MICROSECONDS'
                     HOUR_SECOND,         // 'HOURS:MINUTES:SECONDS'
                     HOUR_MINUTE,         // 'HOURS:MINUTES'
                     DAY_MICROSECOND,     // 'DAYS HOURS:MINUTES:SECONDS.MICROSECONDS'
                     DAY_SECOND,          // 'DAYS HOURS:MINUTES:SECONDS'
                     DAY_MINUTE,          // 'DAYS HOURS:MINUTES'
                     DAY_HOUR,            // 'DAYS HOURS'
                     YEAR_MONTH           // 'YEARS-MONTHS'
)

class DBExpression : public QObject {
public:
    DBExpression();
    DBExpression(const DBExpression& _other);
    DBExpression(const QString& _name, enuDBExpressionType::Type _exprType);
    DBExpression(const QString& _name, enuDBExpressionType::Type _exprType, const QStringList& _values);
    virtual ~DBExpression();

    operator QVariant() const;
    QVariant operator =(DBExpression& _other) const;

//    QVariant toVariant() const;
//    void fromVariant(DBExpression& _other) const;

    QString toString() const;
    bool isValid() const;

    static const DBExpression& NIL();
    static const DBExpression& NOW();
    static const DBExpression& CURDATE();
    static const DBExpression& DATE_ADD(const QString _date, const QVariant _interval, enuDBExpressionIntervalUnit::Type _unit);
    static const DBExpression& DATE_ADD(const DBExpression& _date, const QVariant _interval, enuDBExpressionIntervalUnit::Type _unit);
    static const DBExpression& DATE_SUB(const QString _date, const QVariant _interval, enuDBExpressionIntervalUnit::Type _unit);
    static const DBExpression& DATE_SUB(const DBExpression& _date, const QVariant _interval, enuDBExpressionIntervalUnit::Type _unit);

protected:
    QString Name;
    enuDBExpressionType::Type ExprType;
    QStringList Values;
    friend clsSelectQueryData;
};

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

//    clsCondition(
//            QString _col,
//            enuConditionOperator::Type _operator,
//            const DBExpression& _expression);
//    clsCondition(
//            QString _tableNameOrAlias,
//            QString _col,
//            enuConditionOperator::Type _operator,
//            const DBExpression& _expression);

    clsCondition(
            QString _leftHandTableNameOrAlias,
            QString _leftHandCol,
            enuConditionOperator::Type _operator,
            QString _rightHandTableNameOrAlias,
            QString _rightHandCol);

    virtual ~clsCondition();

//    static clsCondition& parse(
//        const QString& _filters,
//        const clsTable& _table
//    );
//    static clsCondition& parse(
//        const QString& _filters,
//        const QString& _mainTableNameOrAlias,
//        const QMap<QString, stuRelatedORMField>& _filterables
//    );

    clsCondition& andCond(const clsCondition& _cond);
    clsCondition& orCond(const clsCondition& _cond);
    clsCondition& xorCond(const clsCondition& _cond);

    bool isEmpty() const;
    bool hasMany() const;

    QString buildConditionString(
            const QString &_mainTableNameOrAlias,
            const QMap<QString, stuRelatedORMField>& _filterables,
            bool _allowUseColumnAlias = false/*,
            quint8 _prettifierJustifyLen = 0*/) const;

protected:
    QSharedDataPointer<clsConditionData> Data;
    void addCondition(enuPreConditionOperator::Type _aggregator, const clsCondition& _nextCondition);

    friend TestQueryBuilders;
};

/***************************************************************************************/
template <class itmplDerived> class clsQueryJoinTraitData;
template <class itmplDerived> class tmplQueryJoinTrait;

template <class itmplDerived> class clsQueryWhereTraitData;
template <class itmplDerived> class tmplQueryWhereTrait;

template <class itmplDerived> class clsQueryGroupAndHavingTraitData;
template <class itmplDerived> class tmplQueryGroupAndHavingTrait;

template <class itmplDerived> class clsBaseQueryData;

template <class itmplDerived, class itmplData>
class tmplBaseQuery
{
public:
    tmplBaseQuery();
    tmplBaseQuery(const tmplBaseQuery<itmplDerived, itmplData>& _other);
    tmplBaseQuery(clsTable& _table, const QString& _alias = {});
    virtual ~tmplBaseQuery();
    bool isValid();

protected:
    virtual void iAmAbstract() = 0;
    QSharedDataPointer<itmplData> Data;
    clsDAC DAC();

    friend TestQueryBuilders;
    friend clsQueryJoinTraitData<itmplDerived>;
    friend tmplQueryJoinTrait<itmplDerived>;
    friend clsQueryWhereTraitData<itmplDerived>;
    friend tmplQueryWhereTrait<itmplDerived>;
    friend clsQueryGroupAndHavingTraitData<itmplDerived>;
    friend tmplQueryGroupAndHavingTrait<itmplDerived>;
};

/***************************************************************************************/
template <class itmplDerived>
class tmplQueryJoinTrait
{
public:
    tmplQueryJoinTrait(const tmplQueryJoinTrait<itmplDerived>& _other);
    tmplQueryJoinTrait(const itmplDerived* _owner);
    virtual ~tmplQueryJoinTrait();

    itmplDerived& join(enuJoinType::Type _joinType, const QString& _foreignTable, const QString& _alias = {}, const clsCondition& _on = {});

    itmplDerived& leftJoin(const QString& _foreignTable, const clsCondition& _on = {});
    itmplDerived& leftJoin(const QString& _foreignTable, const QString& _alias, const clsCondition& _on = {});
    itmplDerived& rightJoin(const QString& _foreignTable, const clsCondition& _on = {});
    itmplDerived& rightJoin(const QString& _foreignTable, const QString& _alias, const clsCondition& _on = {});
    itmplDerived& innerJoin(const QString& _foreignTable, const clsCondition& _on = {});
    itmplDerived& innerJoin(const QString& _foreignTable, const QString& _alias, const clsCondition& _on = {});
    itmplDerived& crossJoin(const QString& _foreignTable, const QString& _alias = {});

//    itmplDerived& leftJoin(const tmplBaseQuery& _nestedQuery, const QString _alias, const clsCondition& _on);
//    itmplDerived& rightJoin(const tmplBaseQuery& _nestedQuery, const QString _alias, const clsCondition& _on);
//    itmplDerived& innerJoin(const tmplBaseQuery& _nestedQuery, const QString _alias, const clsCondition& _on);
//    itmplDerived& crossJoin(const tmplBaseQuery& _nestedQuery, const QString _alias);
//    itmplDerived& join(enuJoinType::Type _joinType, const tmplBaseQuery& _nestedQuery, const QString _alias, const clsCondition& _on);

    itmplDerived& joinWith(enuJoinType::Type _joinType, const QString& _relationName, const QString& _alias = {});
    itmplDerived& leftJoinWith(const QString& _relationName, const QString& _alias = {});
    itmplDerived& rightJoinWith(const QString& _relationName, const QString& _alias = {});
    itmplDerived& innerJoinWith(const QString& _relationName, const QString& _alias = {});

protected:
    virtual void iAmAbstract() = 0;
    QSharedDataPointer<clsQueryJoinTraitData<itmplDerived>> JoinTraitData;
    friend TestQueryBuilders;
};

/***************************************************************************************/
template <class itmplDerived>
class tmplQueryWhereTrait
{
public:
    tmplQueryWhereTrait(const tmplQueryWhereTrait<itmplDerived>& _other);
    tmplQueryWhereTrait(const itmplDerived* _owner);
    virtual ~tmplQueryWhereTrait();

    itmplDerived& where(const clsCondition& _condition);
    itmplDerived& andWhere(const clsCondition& _condition);
    itmplDerived& orWhere(const clsCondition& _condition);
    itmplDerived& xorWhere(const clsCondition& _condition);

    itmplDerived& setPksByPath(TAPI::PKsByPath_t _pksByPath);
    itmplDerived& addFilters(const QString& _filters);

protected:
    virtual void iAmAbstract() = 0;
    QSharedDataPointer<clsQueryWhereTraitData<itmplDerived>> WhereTraitData;
    friend TestQueryBuilders;
};

/***************************************************************************************/
template <class itmplDerived>
class tmplQueryGroupAndHavingTrait
{
public:
    tmplQueryGroupAndHavingTrait(const tmplQueryGroupAndHavingTrait<itmplDerived>& _other);
    tmplQueryGroupAndHavingTrait(const itmplDerived* _owner);
    virtual ~tmplQueryGroupAndHavingTrait();

//    itmplDerived& groupBy(const clsCondition& _condition);
    itmplDerived& groupBy(const QString& _col);
    itmplDerived& groupBy(const QStringList& _cols);

    itmplDerived& having(const clsCondition& _condition);
    itmplDerived& andHaving(const clsCondition& _condition);
    itmplDerived& orHaving(const clsCondition& _condition);
    itmplDerived& xorHaving(const clsCondition& _condition);

protected:
    virtual void iAmAbstract() = 0;
    QSharedDataPointer<clsQueryGroupAndHavingTraitData<itmplDerived>> GroupAndHavingTraitData;
    friend TestQueryBuilders;
};

/***************************************************************************************/
class SelectQuery :
    public tmplBaseQuery<SelectQuery, clsSelectQueryData>,
    public tmplQueryJoinTrait<SelectQuery>,
    public tmplQueryWhereTrait<SelectQuery>,
    public tmplQueryGroupAndHavingTrait<SelectQuery>
{
public:
    SelectQuery();
    SelectQuery(const SelectQuery& _other);
    SelectQuery(clsTable& _table, const QString& _alias = {});
    virtual ~SelectQuery();

    SelectQuery& addCols(const TAPI::Cols_t& _commaSeperatedCols, const QString& _seperator=","); //-> used by APPLY_GET_METHOD_CALL_ARGS_APICALL_TO_QUERY
    SelectQuery& addCols(const QStringList& _cols);
    SelectQuery& addCol(const QString& _col, const QString& _renameAs = {});
    SelectQuery& addCol(const DBExpression& _expr, const QString& _renameAs = {});
    SelectQuery& addCol(enuAggregation::Type _aggFunc, const QString& _col, const QString& _renameAs = {});
    SelectQuery& addCol(enuConditionalAggregation::Type _aggFunc, const clsCondition& _condition, const QString& _renameAs = {});
    SelectQuery& addCol(enuConditionalAggregation::Type _aggFunc, const clsCondition& _condition, QVariant _trueValue, QVariant _falseValue, const QString& _renameAs = {});

//    SelectQuery& addCols(const QString& _tableAlias, const TAPI::Cols_t& _cols); //-> used by APPLY_GET_METHOD_CALL_ARGS_APICALL_TO_QUERY
//    SelectQuery& addCols(const QString& _tableAlias, const QStringList& _cols);
//    SelectQuery& addCol(const QString& _tableAlias, const QString& _col, const QString& _renameAs = {});
//    SelectQuery& addCol(enuAggregation::Type _aggFunc, const QString& _tableAlias, const QString& _col, const QString& _renameAs = {});

//    SelectQuery& from(const QString _table, const QString& _renameAs = {});
//    SelectQuery& from(const SelectQuery& _nestedQuery, const QString _alias);

    SelectQuery& orderBy(const QString& _col, enuOrderDir::Type _dir = enuOrderDir::Ascending);

    SelectQuery& addUnion(SelectQuery& _query);
    SelectQuery& addUnionAll(SelectQuery& _query);
    SelectQuery& addUnionDistinct(SelectQuery& _query);

    SelectQuery& offset(quint64 _offset); //-> used by APPLY_GET_METHOD_CALL_ARGS_APICALL_TO_QUERY
    SelectQuery& limit(quint16 _limit); //-> used by APPLY_GET_METHOD_CALL_ARGS_APICALL_TO_QUERY
    SelectQuery& setCacheTime(quint16 _cacheTime);

    QVariantMap one(QVariantMap _args = {});
    QVariantList all(QVariantMap _args = {}, quint16 _maxCount = 100, quint64 _from = 0);
    TAPI::stuTable allWithCount(QVariantMap _args = {}, quint16 _maxCount = 100, quint64 _from = 0);
//    quint64 count(QVariantMap _args = {});

private:
    virtual void iAmAbstract() {}
    QString buildQueryString(QVariantMap _args = {}, bool _selectOne = false, bool _reportCount = false/*, quint8 _prettifierJustifyLen = 0*/);
    friend clsSelectQueryData;
    friend clsCreateQueryData;
    friend TestQueryBuilders;
};

/***************************************************************************************/
class CreateQuery :
    public tmplBaseQuery<CreateQuery, clsCreateQueryData>
{
public:
    CreateQuery(const CreateQuery& _other);
    CreateQuery(clsTable& _table, const QString& _alias = {});
    virtual ~CreateQuery();

public:
    CreateQuery& options_ignore();
    CreateQuery& addCols(const QStringList& _cols);
    CreateQuery& addCol(const QString& _col);
    CreateQuery& values(const QVariantMap& _oneRecordValues);
    CreateQuery& values(const QList<QVariantMap>& _multipleRecordValues);
    CreateQuery& select(const SelectQuery& _selectQuery);

    quint64 execute(quint64 _currentUserID, QVariantMap _args = {}, bool _useBinding = true);

    stuBoundQueryString buildQueryString(quint64 _currentUserID, QVariantMap _args = {}, bool _useBinding = true/*, quint8 _prettifierJustifyLen = 0*/);
private:
    virtual void iAmAbstract() {}
    friend TestQueryBuilders;
};

/***************************************************************************************/
class clsUpdateQueryData;

class UpdateQuery :
    public tmplBaseQuery<UpdateQuery, clsUpdateQueryData>,
    public tmplQueryJoinTrait<UpdateQuery>,
    public tmplQueryWhereTrait<UpdateQuery>
{
public:
    UpdateQuery(const UpdateQuery& _other);
    UpdateQuery(clsTable& _table, const QString& _alias = {});
    virtual ~UpdateQuery();

//    UpdateQuery& Select(const SelectQuery& _selectClause);
    UpdateQuery& setNull(const QString& _col);
    UpdateQuery& set(const QString& _col, const QVariant& _value);
    UpdateQuery& set(const QString& _col, const QString& _otherTable, const QString& _otherCol);

    quint64 execute(quint64 _currentUserID, QVariantMap _args = {}, bool _useBinding = true);

private:
    virtual void iAmAbstract() {}
    stuBoundQueryString buildQueryString(quint64 _currentUserID, QVariantMap _args = {}, bool _useBinding = true/*, quint8 _prettifierJustifyLen = 0*/);
    friend TestQueryBuilders;
};

/***************************************************************************************/
class clsDeleteQueryData;

class DeleteQuery :
    public tmplBaseQuery<DeleteQuery, clsDeleteQueryData>,
    public tmplQueryJoinTrait<DeleteQuery>,
    public tmplQueryWhereTrait<DeleteQuery>
{
public:
    DeleteQuery(const DeleteQuery& _other);
    DeleteQuery(clsTable& _table, const QString& _alias = {});
    virtual ~DeleteQuery();

public:
    DeleteQuery& addTarget(const QString& _targetTableName);

    quint64 execute(quint64 _currentUserID, QVariantMap _args = {});

private:
    virtual void iAmAbstract() {}
    QString buildQueryString(quint64 _currentUserID, QVariantMap _args = {}/*, quint8 _prettifierJustifyLen = 0*/);
    friend TestQueryBuilders;
};

/***************************************************************************************/

}
}
}

Q_DECLARE_METATYPE(Targoman::API::ORM::DBExpression);
//Q_DECLARE_METATYPE(Targoman::API::ORM::DBExpressionWithValue);

#endif // TARGOMAN_API_ORM_QUERYBUILDERS_H
