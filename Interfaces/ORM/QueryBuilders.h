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

TARGOMAN_DEFINE_ENUM(enuConditinOperator,
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

class clsCondition{
public:
    clsCondition();
    clsCondition(QString _col, enuConditinOperator::Type _operator, QVariant _value = {});
    ~clsCondition();

    clsCondition& openPar(const clsCondition & _cond);
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

TARGOMAN_DEFINE_ENUM(enuOrderDir,
                     Ascending,
                     Decending)

class clsSelectQueryData;
class SelectQuery
{
public:
    SelectQuery(const clsTable& _table);
    ~SelectQuery();

    QVariant one(QVariantList _args = {});
    QVariant one(QVariantMap _args = {});
    TAPI::stuTable all(QVariantList _args = {}, quint16 _maxCount = 100, quint64 _from = 0);
    TAPI::stuTable all(QVariantMap _args = {}, quint16 _maxCount = 100, quint64 _from = 0);

    SelectQuery& cols(const QStringList& _cols);
    SelectQuery& addCol(const QString& _col, const QString& _renameAs = {});
    SelectQuery& addCol(enuAggregation::Type _aggFunc, const QString& _col, const QString& _renameAs = {});
    SelectQuery& addCol(enuConditionalAggregation::Type _aggFunc, const clsCondition& _condition, const QString& _renameAs = {});

    SelectQuery& rightJoin(const QString _table, const clsCondition& _condition = {});
    SelectQuery& leftJoin(const QString _table, const clsCondition& _condition = {});
    SelectQuery& crossJoin(const QString _table, const clsCondition& _condition = {});
    SelectQuery& innerJoin(const QString _table, const clsCondition& _condition = {});

    SelectQuery& where(const clsCondition& _condition);
    SelectQuery& andWhere(const clsCondition& _condition);
    SelectQuery& orWhere(const clsCondition& _condition);

    SelectQuery& groupBy(const clsCondition& _condition);
    SelectQuery& orderBy(const QString& _col, enuOrderDir::Type _dir);

private:
    QSharedDataPointer<clsSelectQueryData> Data;
};

}
}
}
#endif // TARGOMAN_API_ORM_QUERYBUILDERS_H
