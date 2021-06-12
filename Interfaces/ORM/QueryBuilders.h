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

class TestSelectQuery;

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
    clsCondition(const clsCondition& _other);
    ~clsCondition();

    static clsCondition& scope(const clsCondition& _cond);
    clsCondition& openPar(const clsCondition& _cond);
    clsCondition& closePar();
    clsCondition& andCond(const clsCondition& _cond);
    clsCondition& orCond(const clsCondition& _cond);
    clsCondition& xorCond(const clsCondition& _cond);

    bool isEmpty() const;

private:
    QSharedDataPointer<clsConditionData> Data;
    friend clsCondition& addCondition(clsCondition* _this, char _aggregator, const clsCondition& _nextCondition);
    friend QString toStr(QString _tableName, const clsCondition& _this, const QMap<QString, stuFilteredCol>& _filterables);
};

//class clsConditionData : public QSharedData
//{
//public:
//    clsConditionData(QString _col, enuConditionOperator::Type _operator = enuConditionOperator::Null, QVariant _value = {});
//    bool isAggregator() const;
//    bool isOpenPar() const;
//    bool isClosePar() const;

//public:
//    QString Col;
//    enuConditionOperator::Type Operator;
//    QVariant Value;
//    clsCondition NextCondition;
//};

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

class clsSelectQueryData;
class SelectQuery
{
public:
    SelectQuery(const clsTable& _table);
    SelectQuery(const SelectQuery& _other);
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
    SelectQuery& andHaving(const clsCondition& _condition);
    SelectQuery& orHaving(const clsCondition& _condition);

    SelectQuery& pksByPath(TAPI::PKsByPath_t _pksByPath); //-> used by APPLY_GET_METHOD_CALL_ARGS_TO_QUERY
    SelectQuery& offset(quint64 _offset); //-> used by APPLY_GET_METHOD_CALL_ARGS_TO_QUERY
    SelectQuery& limit(quint16 _limit); //-> used by APPLY_GET_METHOD_CALL_ARGS_TO_QUERY

    quint64 count(QVariantMap _args = {});
    QVariantMap one(QVariantMap _args = {});
    TAPI::stuTable all(QVariantMap _args = {}, quint16 _maxCount = 100, quint64 _from = 0);

private:
    QString buildQueryString(QVariantMap _args = {}, bool _selectOne = false, bool _reportCount = false);
    QSharedDataPointer<clsSelectQueryData> Data;

//#ifdef TARGOMAN_TEST_MODE
//    quint32 testPrivateMember;
    friend TestSelectQuery;
//#endif
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

}
}
}
#endif // TARGOMAN_API_ORM_QUERYBUILDERS_H
