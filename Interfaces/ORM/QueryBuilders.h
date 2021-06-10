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

#include "Interfaces/ORM/Defs.hpp"
#include "libTargomanCommon/exTargomanBase.h"
#include "Interfaces/Common/GenericTypes.h"

namespace Targoman {
namespace API {
namespace ORM {

class clsTable;
TARGOMAN_ADD_EXCEPTION_HANDLER(exQueryBuilder, Common::exTargomanBase);

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
    clsCondition(QString _col, enuConditionOperator::Type _operator, QVariant _value = {});
    ~clsCondition();

    static clsCondition& scope(const clsCondition& _cond);
    clsCondition& openPar(const clsCondition& _cond);
    clsCondition& closePar();
    clsCondition& andCond(const clsCondition& _cond);
    clsCondition& orCond(const clsCondition& _cond);
    clsCondition& xorCond(const clsCondition& _cond);

    bool isEmpty() const;

    QString buildQueryString();

private:
    QSharedDataPointer<clsConditionData> Data;
    friend clsCondition& addCondition(clsCondition* _this, char _aggregator, const clsCondition& _nextCondition);
    friend QString toStr(QString _tableName, const clsCondition& _this, const QMap<QString, stuFilteredCol>& _filterables);
};

class clsConditionData : public QSharedData
{
public:
    clsConditionData(QString _col, enuConditionOperator::Type _operator = enuConditionOperator::Null, QVariant _value = {});
    bool isAggregator() const;
    bool isOpenPar() const;
    bool isClosePar() const;

public:
    QString Col;
    enuConditionOperator::Type Operator;
    QVariant Value;
    clsCondition NextCondition;
};

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

struct stuColSpecs {
    QString Name;
    QString RenameAs = {};
    union unnAggregation {
        enuAggregation::Type Simple;
        enuConditionalAggregation::Type Conditional;
    };
    NULLABLE(unnAggregation) Aggregation = nullptr;
    clsCondition Condition = {};
};

struct stuSelectItems {
    QStringList Cols;
    QStringList From;
    QStringList Where;
    QStringList OrderBy;
    QStringList GroupBy;
};

struct stuQueryRelation {
    enuJoinType::Type JoinType;
    QString Table;
    clsCondition On;
};

class clsSelectQueryData : public QSharedData
{
public:
    clsSelectQueryData(const clsTable& _table);

    void prepare();
    clsORMField colByName(const QString& _col);

public:
    const clsTable&         Table;
    QList<stuColSpecs>      RequiredCols;
    QStringList             GroupByCols;
    stuSelectItems          QueryItems;
    QList<stuQueryRelation> Relations;
};

//class clsSelectQueryData;

class SelectQuery
{
public:
    SelectQuery(const clsTable& _table);
    ~SelectQuery();

    SelectQuery& addCols(const TAPI::Cols_t& _cols); //-> used by APPLY_GET_METHOD_CALL_ARGS_TO_QUERY
    SelectQuery& addCols(const QStringList& _cols);
    SelectQuery& addCol(const QString& _col, const QString& _renameAs = {});
    SelectQuery& addCol(enuAggregation::Type _aggFunc, const QString& _col, const QString& _renameAs = {});
    SelectQuery& addCol(enuConditionalAggregation::Type _aggFunc, const clsCondition& _condition, const QString& _renameAs = {});

    SelectQuery& join(enuJoinType::Type _joinType, const QString _table, const clsCondition& _on);
    SelectQuery& leftJoin(const QString _table, const clsCondition& _on);
    SelectQuery& rightJoin(const QString _table, const clsCondition& _on);
    SelectQuery& innerJoin(const QString _table, const clsCondition& _on);
    SelectQuery& crossJoin(const QString _table);

    SelectQuery& where(const clsCondition& _condition);
    SelectQuery& andWhere(const clsCondition& _condition);
    SelectQuery& orWhere(const clsCondition& _condition);

    SelectQuery& orderBy(const QString& _col, enuOrderDir::Type _dir = enuOrderDir::Ascending);

//    SelectQuery& groupBy(const clsCondition& _condition);
    SelectQuery& groupBy(const QString& _col);
    SelectQuery& groupBy(const QStringList& _cols);

    SelectQuery& having(const clsCondition& _condition);

    SelectQuery& pksByPath(TAPI::PKsByPath_t _pksByPath); //-> used by APPLY_GET_METHOD_CALL_ARGS_TO_QUERY
    SelectQuery& offset(quint64 _offset); //-> used by APPLY_GET_METHOD_CALL_ARGS_TO_QUERY
    SelectQuery& limit(quint16 _limit); //-> used by APPLY_GET_METHOD_CALL_ARGS_TO_QUERY

    QVariantMap one(QVariantMap _args = {});
    TAPI::stuTable all(QVariantMap _args = {}, quint16 _maxCount = 100, quint64 _from = 0);

    QString buildQueryString();

private:
    QSharedDataPointer<clsSelectQueryData> Data;
};

}
}
}
#endif // TARGOMAN_API_ORM_QUERYBUILDERS_H
