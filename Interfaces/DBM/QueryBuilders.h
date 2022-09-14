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

#ifndef TARGOMAN_API_DBM_QUERYBUILDERS_H
#define TARGOMAN_API_DBM_QUERYBUILDERS_H

#include <qglobal.h>

#ifndef SQLPrettyLen
#ifdef QT_DEBUG
#define SQLPrettyLen 18
#else
#define SQLPrettyLen 0
#endif
#endif

class testQueryBuilders;

#include "Interfaces/DBM/Defs.hpp"
#include "Interfaces/Common/GenericTypes.h"
#include "libTargomanCommon/exTargomanBase.h"

namespace Targoman::API::DBM {

class clsTable;

class clsORMSelectQueryData;
class clsORMCreateQueryData;
class clsORMUpdateQueryData;
class clsORMDeleteQueryData;

class ORMSelectQuery;
class ORMCreateQuery;
class ORMUpdateQuery;
class ORMDeleteQuery;

//extern QString getInvalidatedAtQueryString(clsTable& _table, bool _makeWithUniqeIndex = true);

TARGOMAN_ADD_EXCEPTION_HANDLER(exQueryBuilder, Targoman::Common::exTargomanBase);
TARGOMAN_ADD_EXCEPTION_HANDLER(exQueryBuilderColumnNotFound, Targoman::Common::exTargomanBase);
TARGOMAN_ADD_EXCEPTION_HANDLER(exQueryBuilderColumnNotProvided, Targoman::Common::exTargomanBase);
TARGOMAN_ADD_EXCEPTION_HANDLER(exQueryBuilderValueNotProvided, Targoman::Common::exTargomanBase);
TARGOMAN_ADD_EXCEPTION_HANDLER(exQueryBuilderWhereClauseNotProvided, Targoman::Common::exTargomanBase);

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
                     Like,
                     In,
                     NotIn
                     )

TARGOMAN_DEFINE_ENUM(enuAggregation,
                     COUNT,
                     DISTINCT_COUNT,
                     SUM,
                     AVG,
                     MAX,
                     MIN,
                     LOWER,
                     UPPER,
                     )

TARGOMAN_DEFINE_ENUM(enuConditionalAggregation,
                     IF,
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

struct stuBoundQueryString
{
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

class DBExpressionData;

class DBExpression
{
public:
    DBExpression();
    DBExpression(const DBExpression& _other);
    DBExpression(const QString& _name, enuDBExpressionType::Type _exprType, const QStringList& _values = {});
    virtual ~DBExpression();

    operator QVariant() const;
//    QVariant operator =(const DBExpression& _other) const;

//    QVariant toVariant() const;
//    void fromVariant(DBExpression& _other) const;

    QString toString() const;
    bool isValid() const;

    QString name();

    static DBExpression VALUE(const QString _value);
    static DBExpression FUNCTION(const QString _func);

    static DBExpression NIL();
    static DBExpression NOW();
    static DBExpression CURDATE();
    static DBExpression DATE_ADD(const QString _date, const QVariant _interval, enuDBExpressionIntervalUnit::Type _unit);
    static DBExpression DATE_ADD(const DBExpression& _date, const QVariant _interval, enuDBExpressionIntervalUnit::Type _unit);
    static DBExpression DATE_SUB(const QString _date, const QVariant _interval, enuDBExpressionIntervalUnit::Type _unit);
    static DBExpression DATE_SUB(const DBExpression& _date, const QVariant _interval, enuDBExpressionIntervalUnit::Type _unit);

protected:
    QSharedDataPointer<DBExpressionData> Data;

    friend clsORMSelectQueryData;
};

class DBExpressionCase;

class DBExpressionCaseWhen {
public:
    DBExpressionCaseWhen(DBExpressionCase *_parent);
    DBExpressionCase& then(const QString &_then) const;

protected:
    DBExpressionCase *Parent;
};

class DBExpressionCase {
public:
    DBExpressionCase(const QString &_case = {});
    const DBExpressionCaseWhen when(const QString &_when);
    DBExpressionCase& else_(const QString &_else);
    operator DBExpression() const;

protected:
    QString Buffer;
    friend class DBExpressionCaseWhen;
};

/***************************************************************************************/
class clsColSpecsData;

class clsColSpecs;

class clsConditionData;

class clsCondition
{
public:
    clsCondition();
    clsCondition(const clsCondition& _other);
    virtual ~clsCondition();

    clsCondition(QString _col, enuConditionOperator::Type _operator, QVariant _value = {});
//    clsCondition(QString _col, enuConditionOperator::Type _operator, QString _value);
    clsCondition(QString _col, enuConditionOperator::Type _operator, const DBExpression& _value);
    clsCondition(QString _col, enuConditionOperator::Type _operator, const clsColSpecs& _rightHandColSpec);

    clsCondition(QString _tableNameOrAlias, QString _col, enuConditionOperator::Type _operator, QVariant _value = {});
//    clsCondition(QString _tableNameOrAlias, QString _col, enuConditionOperator::Type _operator, QString _value);
    clsCondition(QString _tableNameOrAlias, QString _col, enuConditionOperator::Type _operator, const DBExpression& _value);
    clsCondition(QString _tableNameOrAlias, QString _col, enuConditionOperator::Type _operator, const clsColSpecs& _rightHandColSpec);

    clsCondition(QString _leftHandTableNameOrAlias, QString _leftHandCol, enuConditionOperator::Type _operator, QString _rightHandTableNameOrAlias, QString _rightHandCol);
    clsCondition(QString _leftHandTableNameOrAlias, QString _leftHandCol, enuConditionOperator::Type _operator, QString _rightHandTableNameOrAlias, const clsColSpecs& _rightHandColSpec);

    clsCondition(const clsColSpecs& _colSpec, enuConditionOperator::Type _operator, QVariant _value = {});
//    clsCondition(const clsColSpecs& _colSpec, enuConditionOperator::Type _operator, QString _value);
    clsCondition(const clsColSpecs& _colSpec, enuConditionOperator::Type _operator, const DBExpression& _value);
    clsCondition(const clsColSpecs& _colSpec, enuConditionOperator::Type _operator, const clsColSpecs& _rightHandColSpecs);

    clsCondition(QString _tableNameOrAlias, const clsColSpecs& _colSpec, enuConditionOperator::Type _operator, QVariant _value = {});
//    clsCondition(QString _tableNameOrAlias, const clsColSpecs& _colSpec, enuConditionOperator::Type _operator, QString _value);
    clsCondition(QString _tableNameOrAlias, const clsColSpecs& _colSpec, enuConditionOperator::Type _operator, const DBExpression& _value);
    clsCondition(QString _tableNameOrAlias, const clsColSpecs& _colSpec, enuConditionOperator::Type _operator, const clsColSpecs& _rightHandColSpecs);

    clsCondition(QString _leftHandTableNameOrAlias, const clsColSpecs& _leftHandColSpecs, enuConditionOperator::Type _operator, QString _rightHandTableNameOrAlias, QString _rightHandCol);

//    static clsCondition& parse(
//        const QString& _filters,
//        const clsTable& _table
//    );
//    static clsCondition& parse(
//        const QString& _filters,
//        const QString& _mainTableNameOrAlias,
//        const QMap<QString, stuRelatedORMField>& _filterables
//    );

    clsCondition& setCond(const clsCondition& _cond);
    clsCondition& andCond(const clsCondition& _cond);
    clsCondition& orCond(const clsCondition& _cond);
    clsCondition& xorCond(const clsCondition& _cond);

    bool isEmpty() const;
    bool hasMany() const;

    QString buildConditionString(
            const QString &_tableName,
            const QString &_tableAlias,
            const QMap<QString, stuRelatedORMField> &_selectableColsMap,
            const QMap<QString, stuRelatedORMField> &_filterableColsMap,
            bool _allowUseColumnAlias,
            QStringList& _renamedColumns,
            bool *_statusColHasCriteria = nullptr) const;

protected:
    QSharedDataPointer<clsConditionData> Data;
    void addCondition(enuPreConditionOperator::Type _aggregator, const clsCondition& _nextCondition);

    friend testQueryBuilders;
};

/***************************************************************************************/
class clsColSpecs
{
public:
    clsColSpecs();
    clsColSpecs(const clsColSpecs& _other);
    ~clsColSpecs();

    clsColSpecs(
        const QString& _name,
        const QString& _renameAs // = {}
    );
    clsColSpecs(
        const DBExpression& _expression,
        const QString& _renameAs = {}
    );
    clsColSpecs(
        const enuAggregation::Type _aggregation_Simple,
        const QString& _name,
        const QString& _renameAs = {}
    );
    clsColSpecs(
        const enuConditionalAggregation::Type _ConditionalAggregation,
        const clsCondition& _condition,
        const QString& _renameAs = {},
        const QVariant& _trueValue = {},
        const QVariant& _falseValue = {}
    );

    QString buildColNameString(
        const QString &_tableName,
        const QString &_tableAlias,
        const QString &_otherTableAlias,
        const QMap<QString, stuRelatedORMField> &_selectableColsMap,
        const QMap<QString, stuRelatedORMField> &_filterableColsMap,
        bool _allowUseColumnAlias,
        QStringList &_renamedColumns,
        bool _appendAs,
        const stuRelation &_relation = InvalidRelation,
        /*OUT*/ bool *_isStatusColumn = nullptr
    );

    const QString name() const;
    const QString renameAs() const;

protected:
    QSharedDataPointer<clsColSpecsData> Data;

    friend clsCondition;
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
    tmplBaseQuery(itmplDerived* _derived,
                  const tmplBaseQuery<itmplDerived, itmplData>& _other);
    tmplBaseQuery(INTFAPICALLBOOM_DECL &APICALLBOOM_PARAM,
                  itmplDerived* _derived,
                  clsTable& _table,
                  const QString& _alias = {});
    tmplBaseQuery(INTFAPICALLBOOM_DECL &APICALLBOOM_PARAM,
                  itmplDerived* _derived,
                  const ORMSelectQuery& _fromQuery,
                  const QString& _alias = {});
    virtual ~tmplBaseQuery();
    bool isValid();

    virtual itmplDerived& addColsFromInlineJoinCols(Q_DECL_UNUSED const QList<clsColSpecs>& _colsSpecs) { return (itmplDerived&)*this; }

//    itmplDerived& from(const ORMSelectQuery& _nestedQuery, const QString _alias);

protected:
    virtual void iAmAbstract() = 0;
    QSharedDataPointer<itmplData> Data;
//    clsDAC DAC();

    const QStringList& getRenamedCols();
    void addRenamedCols(const QStringList& _cols, const QString& _alias = {});

//    friend clsBaseQueryData<itmplDerived>;
    friend clsBaseQueryData<ORMSelectQuery>;
    friend clsBaseQueryData<ORMCreateQuery>;
    friend clsBaseQueryData<ORMUpdateQuery>;
    friend clsBaseQueryData<ORMDeleteQuery>;

    friend clsQueryJoinTraitData<itmplDerived>;
    friend tmplQueryJoinTrait<itmplDerived>;
    friend clsQueryWhereTraitData<itmplDerived>;
    friend tmplQueryWhereTrait<itmplDerived>;
    friend clsQueryGroupAndHavingTraitData<itmplDerived>;
    friend tmplQueryGroupAndHavingTrait<itmplDerived>;

    friend testQueryBuilders;
};

/***************************************************************************************/

template <class itmplDerived>
class tmplQueryJoinTrait
{
public:
    tmplQueryJoinTrait(const tmplQueryJoinTrait<itmplDerived>& _other);
    tmplQueryJoinTrait(itmplDerived* _owner);
    virtual ~tmplQueryJoinTrait();

    itmplDerived& crossJoin(const QString& _foreignTable, const QString& _alias = {});
    itmplDerived& innerJoin(const QString& _foreignTable, const clsCondition& _on = {});
    itmplDerived& innerJoin(const QString& _foreignTable, const QString& _alias, const clsCondition& _on = {});
    itmplDerived& leftJoin(const QString& _foreignTable, const clsCondition& _on = {});
    itmplDerived& leftJoin(const QString& _foreignTable, const QString& _alias, const clsCondition& _on = {});
    itmplDerived& rightJoin(const QString& _foreignTable, const clsCondition& _on = {});
    itmplDerived& rightJoin(const QString& _foreignTable, const QString& _alias, const clsCondition& _on = {});
    itmplDerived& join(enuJoinType::Type _joinType, QString _foreignTable, const QString& _alias = {}, const clsCondition& _on = {});

    //-- -------------------------
    itmplDerived& crossJoin(const clsTable& _foreignTable, const QString& _alias = {});
    itmplDerived& innerJoin(const clsTable& _foreignTable, const clsCondition& _on = {});
    itmplDerived& innerJoin(const clsTable& _foreignTable, const QString& _alias, const clsCondition& _on = {});
    itmplDerived& leftJoin(const clsTable& _foreignTable, const clsCondition& _on = {});
    itmplDerived& leftJoin(const clsTable& _foreignTable, const QString& _alias, const clsCondition& _on = {});
    itmplDerived& rightJoin(const clsTable& _foreignTable, const clsCondition& _on = {});
    itmplDerived& rightJoin(const clsTable& _foreignTable, const QString& _alias, const clsCondition& _on = {});
    itmplDerived& join(enuJoinType::Type _joinType, clsTable _foreignTable, const QString _alias, const clsCondition& _on = {});

    //-- inline -------------------------
    itmplDerived& inlineCrossJoin(const QString& _foreignTable, const QString& _alias = {});
    itmplDerived& inlineInnerJoin(const QString& _foreignTable, const clsCondition& _on = {});
    itmplDerived& inlineInnerJoin(const QString& _foreignTable, const QString& _alias, const clsCondition& _on = {});
    itmplDerived& inlineLeftJoin(const QString& _foreignTable, const clsCondition& _on = {});
    itmplDerived& inlineLeftJoin(const QString& _foreignTable, const QString& _alias, const clsCondition& _on = {});
    itmplDerived& inlineRightJoin(const QString& _foreignTable, const clsCondition& _on = {});
    itmplDerived& inlineRightJoin(const QString& _foreignTable, const QString& _alias, const clsCondition& _on = {});
    itmplDerived& inlineJoin(enuJoinType::Type _joinType, QString _foreignTable, const QString _alias, const clsCondition& _on = {});

    //-- nested -------------------------
    itmplDerived& nestedCrossJoin(const ORMSelectQuery& _nestedQuery, const QString _alias);
    itmplDerived& nestedInnerJoin(const ORMSelectQuery& _nestedQuery, const QString _alias, const clsCondition& _on);
    itmplDerived& nestedLeftJoin(const ORMSelectQuery& _nestedQuery, const QString _alias, const clsCondition& _on);
    itmplDerived& nestedRightJoin(const ORMSelectQuery& _nestedQuery, const QString _alias, const clsCondition& _on);
    itmplDerived& nestedJoin(enuJoinType::Type _joinType, const ORMSelectQuery& _nestedQuery, const QString _alias, const clsCondition& _on = {});

    //-- with -------------------------
    itmplDerived& innerJoinWith(const QString& _relationName, const QString& _alias = {});
    itmplDerived& leftJoinWith(const QString& _relationName, const QString& _alias = {});
    itmplDerived& rightJoinWith(const QString& _relationName, const QString& _alias = {});
    itmplDerived& joinWith(enuJoinType::Type _joinType, const QString& _relationName, const QString& _alias = {});

protected:
    virtual void iAmAbstract() = 0;
    QSharedDataPointer<clsQueryJoinTraitData<itmplDerived>> JoinTraitData;
    friend testQueryBuilders;
};

/***************************************************************************************/
template <class itmplDerived>
class tmplQueryWhereTrait
{
public:
    tmplQueryWhereTrait(const tmplQueryWhereTrait<itmplDerived>& _other);
    tmplQueryWhereTrait(itmplDerived* _owner);
    virtual ~tmplQueryWhereTrait();

    itmplDerived& where(const clsCondition& _condition);
    itmplDerived& andWhere(const clsCondition& _condition);
    itmplDerived& orWhere(const clsCondition& _condition);
    itmplDerived& xorWhere(const clsCondition& _condition);

    itmplDerived& setPksByPath(quint32 _pksByPath);
    itmplDerived& setPksByPath(quint64 _pksByPath);
    itmplDerived& setPksByPath(TAPI::PKsByPath_t _pksByPath);

    itmplDerived& addFilters(const QString& _filters);
    itmplDerived& addFilters(const QStringList& _filters);

protected:
    virtual void iAmAbstract() = 0;
    QSharedDataPointer<clsQueryWhereTraitData<itmplDerived>> WhereTraitData;
    friend testQueryBuilders;
};

/***************************************************************************************/
template <class itmplDerived>
class tmplQueryGroupAndHavingTrait
{
public:
    tmplQueryGroupAndHavingTrait(const tmplQueryGroupAndHavingTrait<itmplDerived>& _other);
    tmplQueryGroupAndHavingTrait(itmplDerived* _owner);
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
    friend testQueryBuilders;
};

/***************************************************************************************/

class ORMSelectQuery :
    public tmplBaseQuery<ORMSelectQuery, clsORMSelectQueryData>,
    public tmplQueryJoinTrait<ORMSelectQuery>,
    public tmplQueryWhereTrait<ORMSelectQuery>,
    public tmplQueryGroupAndHavingTrait<ORMSelectQuery>
{
public:
    ORMSelectQuery();
    ORMSelectQuery(const ORMSelectQuery& _other);
    ORMSelectQuery(INTFAPICALLBOOM_DECL &APICALLBOOM_PARAM, clsTable& _table, const QString& _alias = {});
    //alias is mandatory for nested from query
    ORMSelectQuery(INTFAPICALLBOOM_DECL &APICALLBOOM_PARAM, const ORMSelectQuery& _fromQuery, const QString& _alias /*= {}*/);
    virtual ~ORMSelectQuery();

    virtual ORMSelectQuery& addColsFromInlineJoinCols(Q_DECL_UNUSED const QList<clsColSpecs>& _colsSpecs);

    ORMSelectQuery& addCol(const clsColSpecs& _colSpecs);
    ORMSelectQuery& addCols(const QList<clsColSpecs>& _colsSpecs);
    //TAPI::Cols_t -> QString
    ORMSelectQuery& addCSVCols(const QString& _commaSeperatedCols, const QString& _seperator=","); //-> used by APPLY_GET_METHOD_CALL_ARGS_APICALL_TO_QUERY
    ORMSelectQuery& addCols(const QStringList& _cols);
    ORMSelectQuery& addCol(const QString& _col, const QString& _renameAs = {});
    ORMSelectQuery& addCol(const DBExpression& _expr, const QString& _renameAs = {});
    ORMSelectQuery& addCol(enuAggregation::Type _aggFunc, const QString& _col, const QString& _renameAs = {});
    ORMSelectQuery& addCol(enuConditionalAggregation::Type _aggFunc, const clsCondition& _condition, const QString& _renameAs = {});
    ORMSelectQuery& addCol(enuConditionalAggregation::Type _aggFunc, const clsCondition& _condition, QVariant _trueValue, QVariant _falseValue, const QString& _renameAs = {});

    ORMSelectQuery& removeCols(const QStringList& _cols);
    ORMSelectQuery& removeCol(const QString& _col);

//    ORMSelectQuery& addCols(const QString& _tableAlias, const TAPI::Cols_t& _cols); //-> used by APPLY_GET_METHOD_CALL_ARGS_APICALL_TO_QUERY
//    ORMSelectQuery& addCols(const QString& _tableAlias, const QStringList& _cols);
//    ORMSelectQuery& addCol(const QString& _tableAlias, const QString& _col, const QString& _renameAs = {});
//    ORMSelectQuery& addCol(enuAggregation::Type _aggFunc, const QString& _tableAlias, const QString& _col, const QString& _renameAs = {});

//    ORMSelectQuery& from(const QString _table, const QString& _renameAs = {});
//    ORMSelectQuery& from(const ORMSelectQuery& _nestedQuery, const QString _alias);

    ORMSelectQuery& orderBy(QString _col, enuOrderDir::Type _dir = enuOrderDir::Ascending);

    ORMSelectQuery& addUnion(ORMSelectQuery& _query);
    ORMSelectQuery& addUnionAll(ORMSelectQuery& _query);
    ORMSelectQuery& addUnionDistinct(ORMSelectQuery& _query);

    ORMSelectQuery& pageIndex(quint16 _pageIndex); //-> used by APPLY_GET_METHOD_CALL_ARGS_APICALL_TO_QUERY
    ORMSelectQuery& pageSize(quint16 _pageSize); //-> used by APPLY_GET_METHOD_CALL_ARGS_APICALL_TO_QUERY

    ORMSelectQuery& setCacheTime(quint16 _cacheTime);
    void clearCache(QVariantMap _args = {});

    bool isPure();

    QVariantMap one(QVariantMap _args = {});
    template <typename T> T one(QVariantMap _args = {}) {
        QVariantMap info = this->one(_args);
        T t;
        t.fromJson(QJsonObject::fromVariantMap(info));
        return t;
    }
    QVariantMap tryOne(QVariantMap _args = {});
    template <typename T> T tryOne(QVariantMap _args = {}) noexcept {
        QT_TRY {
            QVariantMap info = this->one(_args);
            T t;
            t.fromJson(QJsonObject::fromVariantMap(info));
            return t;
        }
        QT_CATCH (const exHTTPNotFound &e) {
            return {};
        }
    }
    /*QVariantList*/ TAPI::stuTable all(QVariantMap _args = {}); //, quint16 _maxCount = 20, quint64 _from = 0);
    TAPI::stuTable allWithCount(QVariantMap _args = {}); //, quint16 _maxCount = 20, quint64 _from = 0);
//    quint64 count(QVariantMap _args = {});

//    QVariant execute(QVariantMap _args = {}, quint16 _maxCount = 20, quint64 _from = 0);

private:
    virtual void iAmAbstract() { ; }
    void prepare(QVariantMap _args = {}, bool _selectOne = false, bool _reportCount = false, bool _checkStatusCol = false);
    QString buildQueryString(QVariantMap _args = {}, bool _selectOne = false, bool _reportCount = false, bool _checkStatusCol = false);

    friend clsBaseQueryData<ORMSelectQuery>;
    friend clsBaseQueryData<ORMCreateQuery>;
    friend clsBaseQueryData<ORMUpdateQuery>;
    friend clsBaseQueryData<ORMDeleteQuery>;

    friend clsORMSelectQueryData;
    friend clsORMCreateQueryData;
    friend tmplQueryJoinTrait<ORMSelectQuery>;
    friend tmplQueryJoinTrait<ORMUpdateQuery>;
    friend tmplQueryJoinTrait<ORMDeleteQuery>;
//    friend clsORMUpdateQueryData;
//    friend ORMUpdateQuery;
//    friend clsORMDeleteQueryData;
//    friend ORMDeleteQuery;
    friend testQueryBuilders;
};

/***************************************************************************************/
class ORMCreateQuery :
    public tmplBaseQuery<ORMCreateQuery, clsORMCreateQueryData>
{
public:
    ORMCreateQuery(const ORMCreateQuery& _other);
    ORMCreateQuery(INTFAPICALLBOOM_DECL &APICALLBOOM_PARAM, clsTable& _table, const QString& _alias = {});
    virtual ~ORMCreateQuery();

public:
    ORMCreateQuery& options_ignore();
    ORMCreateQuery& addCols(const QStringList& _cols);
    ORMCreateQuery& addCol(const QString& _col);
    ORMCreateQuery& values(const QVariantMap& _oneRecordValues);
    ORMCreateQuery& values(const QList<QVariantMap>& _multipleRecordValues);
    ORMCreateQuery& select(const ORMSelectQuery& _selectQuery);

    quint64 execute(quint64 _currentUserID, QVariantMap _args = {}, bool _useBinding = true);

    stuBoundQueryString buildQueryString(quint64 _currentUserID, QVariantMap _args = {}, bool _useBinding = true);
private:
    virtual void iAmAbstract() { ; }
    friend testQueryBuilders;
};

/***************************************************************************************/
class clsORMUpdateQueryData;

class ORMUpdateQuery :
    public tmplBaseQuery<ORMUpdateQuery, clsORMUpdateQueryData>,
    public tmplQueryJoinTrait<ORMUpdateQuery>,
    public tmplQueryWhereTrait<ORMUpdateQuery>
{
public:
    ORMUpdateQuery(const ORMUpdateQuery& _other);
    ORMUpdateQuery(INTFAPICALLBOOM_DECL &APICALLBOOM_PARAM, clsTable& _table, const QString& _alias = {});
    virtual ~ORMUpdateQuery();

    ORMUpdateQuery& setNull(const QString& _col);
    ORMUpdateQuery& set(const QString& _col, const QVariant& _value);
    ORMUpdateQuery& set(const QString& _col, const QString& _otherTable, const QString& _otherCol);
//    ORMUpdateQuery& increament(const QString& _col, quint64 _value);
    ORMUpdateQuery& increament(const QString& _col, qreal _value);
//    ORMUpdateQuery& decreament(const QString& _col, quint64 _value);
    ORMUpdateQuery& decreament(const QString& _col, qreal _value);

    quint64 execute(quint64 _currentUserID, QVariantMap _args = {}, bool _useBinding = true);

private:
    virtual void iAmAbstract() { ; }
    stuBoundQueryString buildQueryString(quint64 _currentUserID, QVariantMap _args = {}, bool _useBinding = true);
    friend tmplQueryJoinTrait<ORMUpdateQuery>;
    friend testQueryBuilders;
};

/***************************************************************************************/
class clsORMDeleteQueryData;

class ORMDeleteQuery :
    public tmplBaseQuery<ORMDeleteQuery, clsORMDeleteQueryData>,
    public tmplQueryJoinTrait<ORMDeleteQuery>,
    public tmplQueryWhereTrait<ORMDeleteQuery>
{
public:
    ORMDeleteQuery(const ORMDeleteQuery& _other);
    ORMDeleteQuery(INTFAPICALLBOOM_DECL &APICALLBOOM_PARAM, clsTable& _table, const QString& _alias = {});
    virtual ~ORMDeleteQuery();

public:
    ORMDeleteQuery& addTarget(const QString& _targetTableName);

    quint64 execute(quint64 _currentUserID, QVariantMap _args = {}, bool _realDelete = false);

private:
    virtual void iAmAbstract() { ; }
    QString buildQueryString(quint64 _currentUserID, QVariantMap _args = {});
    friend tmplQueryJoinTrait<ORMDeleteQuery>;
    friend testQueryBuilders;
};

/***************************************************************************************/

} //namespace Targoman::API::DBM

Q_DECLARE_METATYPE(Targoman::API::DBM::DBExpression);
//Q_DECLARE_METATYPE(Targoman::API::DBM::DBExpressionWithValue);

#endif // TARGOMAN_API_DBM_QUERYBUILDERS_H
