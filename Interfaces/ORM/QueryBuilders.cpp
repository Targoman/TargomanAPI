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

#include "QueryBuilders.h"
#include "clsTable.h"

//using namespace Targoman::DBManager;

#include "Interfaces/AAA/clsJWT.hpp"
using namespace Targoman::API::AAA;

namespace Targoman {
namespace API {
namespace ORM {

stuRelation InvalidRelation("", "", "");

QString finalColName(const clsORMField& _col, const QString& _prefix) {
    return _prefix + (_col.renameAs().isEmpty() ? _col.name() : _col.renameAs());
}

QString makeColRenamedAs(const clsORMField& _col, const QString& _prefix) {
    return (_col.renameAs().isEmpty() && _prefix.isEmpty() ? "" : " AS `"+ finalColName(_col, _prefix) + "`");
};

QString makeColName(const QString& _tableName, const clsORMField& _col, bool _appendAs, const stuRelation& _relation) {
    QString ColName = _col.name();

    if (_relation.Column.size() && _relation.RenamingPrefix.size())
        ColName = ColName.replace(QRegularExpression("^" + _relation.RenamingPrefix), "");

    return (_relation.Column.isEmpty()
            ? (_col.isVirtual() ? "" : _tableName + ".")
            : (_relation.RenamingPrefix.isEmpty()
                ? _relation.ReferenceTable
                : _relation.RenamingPrefix
              ) + "."
           )
//           + "."
           + ColName
           + (_appendAs
              ? makeColRenamedAs(_col, _relation.RenamingPrefix)
              : ""
             );
};

QString makeValueAsSQL(const QVariant& _value, bool _qouteIfIsString = true, clsORMField* baseCol = nullptr)
{
    if (_value.isValid() == false)
        return QString(""); ///TODO: ? throw ?

    //--
    if (_value.userType() == QMetaTypeId<DBExpression>::qt_metatype_id())
        return _value.value<DBExpression>().toString();

    //--
//    if (_value.userType() == QMetaType::QJsonDocument)
//        return QString("'%1'").arg(_value.value<QJsonDocument>().toJson(QJsonDocument::Compact).constData());

    //--
    QVariant _v2 = _value;
    if (baseCol != nullptr)
        _v2 = baseCol->toDB(_value);

    QString v = _v2.value<QString>();

    if (_qouteIfIsString
            && (_value.userType() == QMetaType::QString
                || _value.userType() == QMetaType::QChar
                || _value.userType() == QMetaType::QJsonDocument
            )
        )
        return QString("'%1'").arg(v.replace("'", "''"));

    return v;
};
QVariant makeValueAsVariant(const QVariant& _value)
{
    if (_value.isValid() == false)
        return QVariant(); ///TODO: ? throw ?

    //--
    if (_value.userType() == QMetaTypeId<DBExpression>::qt_metatype_id())
        return _value.value<DBExpression>().toString();

    //--
//    if (_value.userType() == QMetaType::QJsonDocument)
//        return QString("'%1'").arg(_value.value<QJsonDocument>().toJson(QJsonDocument::Compact).constData());

    //--
    return _value;
};

//#define KZ_TRACE(msg) \
//    throw exQueryBuilder(QString(">>>>>>>>>>>>>>>> [%1:%2] %3").arg(__FUNCTION__).arg(__LINE__) \
//        .arg(msg));

/***************************************************************************************/
/* DBExpression ************************************************************************/
/***************************************************************************************/
class DBExpressionData : public QSharedData
{
public:
    DBExpressionData(const DBExpressionData& _other) :
        QSharedData(_other), Name(_other.Name), ExprType(_other.ExprType), Values(_other.Values)
    {}
    DBExpressionData(const QString& _name, enuDBExpressionType::Type _exprType, const QStringList& _values) :
        Name(_name), ExprType(_exprType), Values(_values)
    {}

public:
    QString Name;
    enuDBExpressionType::Type ExprType;
    QStringList Values;
};

/***************************************************************************************/
DBExpression::DBExpression() :
    Data(nullptr)
{}
DBExpression::DBExpression(const DBExpression& _other) :
    Data(_other.Data)
{}
DBExpression::DBExpression(const QString& _name, enuDBExpressionType::Type _exprType, const QStringList& _values) :
    Data(new DBExpressionData(_name, _exprType, _values))
{}
DBExpression::~DBExpression()
{}

DBExpression::operator QVariant() const {
    return QVariant::fromValue(*this);
}

QVariant DBExpression::operator =(DBExpression& _other) const {
    return QVariant::fromValue(_other);
}

QString DBExpression::name() {
    if (this->Data == nullptr)
        return "";

    return this->Data->Name;
}

QString DBExpression::toString() const {
    if (this->Data == nullptr)
        return "";

    QString ret = this->Data->Name;

    if (this->Data->ExprType == enuDBExpressionType::Function)
        ret += "(";

    if (this->Data->Values.length())
        ret += this->Data->Values.join(",");

    if (this->Data->ExprType == enuDBExpressionType::Function)
        ret += ")";

    return ret;
}

bool DBExpression::isValid() const {
    return (this->Data && this->Data->Name.length());
}

DBExpression DBExpression::VALUE(const QString _value) {
    return DBExpression(_value, enuDBExpressionType::Value);
}
DBExpression DBExpression::FUNCTION(const QString _func) {
    return DBExpression(_func, enuDBExpressionType::Function);
}

DBExpression DBExpression::NIL() {
    static DBExpression* DBEX_NULL = nullptr;
    if (DBEX_NULL)
        return *DBEX_NULL;

    ///TODO: check memory leak

    return *(DBEX_NULL = new DBExpression("NULL", enuDBExpressionType::Value));
}

DBExpression DBExpression::NOW() {
    static DBExpression* DBEX_NOW = nullptr;
    if (DBEX_NOW)
        return *DBEX_NOW;

    ///TODO: check memory leak

    return *(DBEX_NOW = new DBExpression("NOW", enuDBExpressionType::Function));
}

DBExpression DBExpression::CURDATE() {
    static DBExpression* DBEX_CURDATE = nullptr;
    if (DBEX_CURDATE)
        return *DBEX_CURDATE;

    ///TODO: check memory leak

    return *(DBEX_CURDATE = new DBExpression("CURDATE", enuDBExpressionType::Function));
}

QString makeExpressionIntervalValue(const QVariant _interval, enuDBExpressionIntervalUnit::Type _unit) {
    QString v = _interval.value<QString>();

    if (_interval.userType() == QMetaType::QString)
        return QString("'%1' %2").arg(v).arg(enuDBExpressionIntervalUnit::toStr(_unit));

    return QString("%1 %2").arg(v).arg(enuDBExpressionIntervalUnit::toStr(_unit));
}
DBExpression DBExpression::DATE_ADD(const QString _date, const QVariant _interval, enuDBExpressionIntervalUnit::Type _unit) {
    return DBExpression("DATE_ADD", enuDBExpressionType::Function, QStringList({ _date, "INTERVAL " + makeExpressionIntervalValue(_interval, _unit) }));
}
DBExpression DBExpression::DATE_ADD(const DBExpression& _date, const QVariant _interval, enuDBExpressionIntervalUnit::Type _unit) {
    return DBExpression("DATE_ADD", enuDBExpressionType::Function, QStringList({ _date.toString(), "INTERVAL " + makeExpressionIntervalValue(_interval, _unit) }));
}

DBExpression DBExpression::DATE_SUB(const QString _date, const QVariant _interval, enuDBExpressionIntervalUnit::Type _unit) {
    return DBExpression("DATE_SUB", enuDBExpressionType::Function, QStringList({ _date, "INTERVAL " + makeExpressionIntervalValue(_interval, _unit) }));
}
DBExpression DBExpression::DATE_SUB(const DBExpression& _date, const QVariant _interval, enuDBExpressionIntervalUnit::Type _unit) {
    return DBExpression("DATE_SUB", enuDBExpressionType::Function, QStringList({ _date.toString(), "INTERVAL " + makeExpressionIntervalValue(_interval, _unit) }));
}

/***************************************************************************************/
/***************************************************************************************/
/***************************************************************************************/
struct stuConditionData
{
//    union {
        clsCondition Condition;
//        struct {
            bool IsAggregator = false;
            QString TableNameOrAlias;
            QString Col;
            enuConditionOperator::Type Operator;
//            union {
                QVariant Value;
//                struct {
                    QString OtherTableNameOrAlias;
                    QString OtherCol;
//                };
//            };
//        };
//    };

    stuConditionData(QString _col, enuConditionOperator::Type _operator, QVariant _value = {}) :
        IsAggregator(false), Col(_col), Operator(_operator), Value(_value)
    {}
    stuConditionData(QString _tableNameOrAlias, QString _col, enuConditionOperator::Type _operator, QVariant _value = {}) :
        IsAggregator(false), TableNameOrAlias(_tableNameOrAlias), Col(_col), Operator(_operator), Value(_value)
    {}
//    stuConditionData(QString _col, enuConditionOperator::Type _operator, const DBExpression& _expression) :
//        IsAggregator(false), Col(_col), Operator(_operator)//, Value(_expression)
//    {
//        this->Value.setValue(_expression);
//    }
//    stuConditionData(QString _tableNameOrAlias, QString _col, enuConditionOperator::Type _operator, const DBExpression& _expression) :
//        IsAggregator(false), TableNameOrAlias(_tableNameOrAlias), Col(_col), Operator(_operator)//, Value(_expression)
//    {
//        this->Value.setValue(_expression);
//    }
    stuConditionData(QString _tableNameOrAlias, QString _col, enuConditionOperator::Type _operator, QString _otherTableNameOrAlias, QString _otherCol) :
        IsAggregator(false), TableNameOrAlias(_tableNameOrAlias), Col(_col), Operator(_operator), OtherTableNameOrAlias(_otherTableNameOrAlias), OtherCol(_otherCol)
    {}
    stuConditionData(bool _isAggregator, QString _col) :
        IsAggregator(true /*_isAggregator*/), Col(_col)
    {
        Q_UNUSED(_isAggregator);
    }
    stuConditionData(const clsCondition& _condition) :
        Condition(_condition), IsAggregator(false)
    {}
    bool isAggregator() const { return this->IsAggregator; } //Col.startsWith(" "); }
//    bool isOpenPar() const { return this->Col == "("; }
//    bool isClosePar() const { return this->Col == ")"; }
};

class clsConditionData : public QSharedData
{
//public:
//    clsConditionData(QString _col, enuConditionOperator::Type _operator = enuConditionOperator::Null, QVariant _value = {}) :
//        IsAggregator(false), Col(_col), Operator(_operator), Value(_value)
//    {}
//    clsConditionData(QString _tableAlias, QString _col, enuConditionOperator::Type _operator = enuConditionOperator::Null, QVariant _value = {}) :
//        IsAggregator(false), TableAlias(_tableAlias), Col(_col), Operator(_operator), Value(_value)
//    {}
//    bool isAggregator() const { return this->IsAggregator; } //Col.startsWith(" "); }
//    bool isOpenPar() const { return this->Col == "("; }
//    bool isClosePar() const { return this->Col == ")"; }

public:
    QList<stuConditionData> Conditions;
//    bool IsAggregator;
//    QString TableAlias;
//    QString Col;
//    enuConditionOperator::Type Operator;
//    QVariant Value;
//    clsCondition NextCondition;
};
/***************************************************************************************/
/* clsCondition ************************************************************************/
/***************************************************************************************/
clsCondition::clsCondition() :
    Data(new clsConditionData)
{}

clsCondition::clsCondition(const clsCondition& _other) :
    Data(_other.Data)
{}

clsCondition::clsCondition(
        QString _col,
        enuConditionOperator::Type _operator,
        QVariant _value) :
    Data(new clsConditionData) {
    this->Data->Conditions.append({ _col, _operator, _value });
}

clsCondition::clsCondition(
        QString _tableNameOrAlias,
        QString _col,
        enuConditionOperator::Type _operator,
        QVariant _value) :
    Data(new clsConditionData) {
    this->Data->Conditions.append({ _tableNameOrAlias, _col, _operator, _value });
}

//clsCondition::clsCondition(
//        QString _col,
//        enuConditionOperator::Type _operator,
//        const DBExpression& _expression) :
//    Data(new clsConditionData) {
//    this->Data->Conditions.append({ _col, _operator, QVariant::fromValue(_expression) });
//}

//clsCondition::clsCondition(
//        QString _tableNameOrAlias,
//        QString _col,
//        enuConditionOperator::Type _operator,
//        const DBExpression& _expression) :
//    Data(new clsConditionData) {
//    this->Data->Conditions.append({ _tableNameOrAlias, _col, _operator, QVariant::fromValue(_expression) });
//}

clsCondition::clsCondition(
        QString _leftHandTableNameOrAlias,
        QString _leftHandCol,
        enuConditionOperator::Type _operator,
        QString _rightHandTableNameOrAlias,
        QString _rightHandCol) :
    Data(new clsConditionData) {
    this->Data->Conditions.append({ _leftHandTableNameOrAlias, _leftHandCol, _operator, _rightHandTableNameOrAlias, _rightHandCol });
}

clsCondition::~clsCondition() {}

/***********************\
|* other conditions    *|
\***********************/
clsCondition& clsCondition::setCond(const clsCondition& _cond) {
    this->Data->Conditions.clear();
    this->Data->Conditions.append(stuConditionData(_cond));
    return *this;
}
clsCondition& clsCondition::andCond(const clsCondition& _cond) {
    this->addCondition(enuPreConditionOperator::AND, _cond);
    return *this;
}
clsCondition& clsCondition::orCond(const clsCondition& _cond)  {
    this->addCondition(enuPreConditionOperator::OR, _cond);
    return *this;
}
clsCondition& clsCondition::xorCond(const clsCondition& _cond) {
    this->addCondition(enuPreConditionOperator::XOR, _cond);
    return *this;
}
void clsCondition::addCondition(enuPreConditionOperator::Type _aggregator, const clsCondition& _nextCondition) {
    if (_nextCondition.isEmpty())
        throw exQueryBuilder(QString("aggregator '%1' must follow a valid condition").arg(enuPreConditionOperator::toStr(_aggregator)));

    if (this->Data->Conditions.isEmpty() == false)
        this->Data->Conditions.append({ true, enuPreConditionOperator::toStr(_aggregator) });

    this->Data->Conditions.append(stuConditionData(_nextCondition));
}

bool clsCondition::isEmpty() const { return this->Data->Conditions.isEmpty(); }
bool clsCondition::hasMany() const { return (this->Data->Conditions.length() > 1); }

/***********************\
|* parse and toStr     *|
\***********************/
/*
clsCondition& clsCondition::parse(
        const QString& _filters,
        const clsTable& _table
    )
{
    return clsCondition::parse(_filters, _table.Name, _table.FilterableColsMap);
}

clsCondition& clsCondition::parse(
        const QString& _filters,
        const QString& _mainTableNameOrAlias,
        const QMap<QString, stuRelatedORMField>& _filterables
    )
{
    QSharedPointer<clsCondition> condition = QSharedPointer<clsCondition>::create();

    QStringList Filters = _filters
                          .trimmed()
                          .replace("\\ ", "$SPACE$")
                          .split(" ", QString::SkipEmptyParts);

    return *condition;
}
*/

QString clsCondition::buildConditionString(
    const QString& _mainTableNameOrAlias,
    const QMap<QString, stuRelatedORMField>& _filterables,
    bool _allowUseColumnAlias //for having
//    quint8 _prettifierJustifyLen
) const {
    if (this->Data->Conditions.isEmpty())
        return "";

    std::function<QString(
        const QString& _tableNameOrAlias,
        const QString& _col
    )> makeColNameHelper = [
        &_mainTableNameOrAlias,
        &_filterables,
        &_allowUseColumnAlias //for having
    ] (
        const QString& _tableNameOrAlias,
        const QString& _col
    ) {
        if (_tableNameOrAlias.length() && (_tableNameOrAlias != _mainTableNameOrAlias))
            return QString("%1.%2")
                .arg(_tableNameOrAlias)
                .arg(_col);

        auto relatedORMField = _filterables.value(_col);
        if (relatedORMField.isValid() == false) {
            if (_allowUseColumnAlias) //for having
                return _col;

//            qDebug() << "(" << _filterables.keys().join("|") << ")";

            throw exQueryBuilder("Invalid column for filtering:: " + _col);
        }

        return makeColName(
            _tableNameOrAlias.length() ? _tableNameOrAlias : _mainTableNameOrAlias,
            relatedORMField.Col,
            false,
            relatedORMField.Relation);
    };

    QString CondStr;

    for (QList<stuConditionData>::const_iterator iter = this->Data->Conditions.begin();
         iter != this->Data->Conditions.end();
         ++iter)
    {
        stuConditionData conditionData = *iter;

        if (conditionData.isAggregator())
        {
            if (iter == this->Data->Conditions.end()-1)
                throw exQueryBuilder(QString("aggregator '%1' must follow a valid condition").arg(conditionData.Col));

            if (SQLPrettyLen)
                CondStr += "\n" + conditionData.Col.rightJustified(SQLPrettyLen);
            else
                CondStr += " " + conditionData.Col;

            CondStr += " ";
        }
        else if (conditionData.Condition.isEmpty() == false)
        {
            if (conditionData.Condition.hasMany())
            {
                CondStr += "(";
                if (SQLPrettyLen)
                    CondStr += "\n" + QString(SQLPrettyLen, ' ') + " ";
            }

            CondStr += conditionData.Condition.buildConditionString(
                _mainTableNameOrAlias,
                _filterables,
                _allowUseColumnAlias); //for having
//                _prettifierJustifyLen ? _prettifierJustifyLen : 0);

            if (conditionData.Condition.hasMany())
            {
                if (SQLPrettyLen)
                    CondStr += "\n" + QString(SQLPrettyLen, ' ') + " ";
                CondStr += ")";
            }
        }
        else
        {
            CondStr += makeColNameHelper(conditionData.TableNameOrAlias, conditionData.Col);

            if (conditionData.Operator == enuConditionOperator::Null)
                CondStr += " IS NULL";
            else if (conditionData.Operator == enuConditionOperator::NotNull)
                CondStr += " IS NOT NULL";
            else if (conditionData.Operator == enuConditionOperator::Like)
            {
                if (conditionData.Value.isValid() == false)
                    throw exQueryBuilder("Value of LIKE condition is empty");

                CondStr += " LIKE '";
                //makeValueAsSQL(
                CondStr += conditionData.Value.value<QString>();
                CondStr += "'";
            }
            else
            {
                if (SQLPrettyLen)
                    CondStr += " ";

                switch (conditionData.Operator) {
                    case enuConditionOperator::Equal:
                        CondStr += "=";
                        break;
                    case enuConditionOperator::NotEqual:
                        CondStr += "!=";
                        break;
                    case enuConditionOperator::Greater:
                        CondStr += ">";
                        break;
                    case enuConditionOperator::GreaterEqual:
                        CondStr += ">=";
                        break;
                    case enuConditionOperator::Less:
                        CondStr += "<";
                        break;
                    case enuConditionOperator::LessEqual:
                        CondStr += "<=";
                        break;
                    default:
                        break;
                }

                if (SQLPrettyLen)
                    CondStr += " ";

                if (conditionData.Value.isValid())
                    CondStr += makeValueAsSQL(conditionData.Value);
                else
                    CondStr += makeColNameHelper(conditionData.OtherTableNameOrAlias, conditionData.OtherCol);
            }
        }
    }

    if (CondStr.isEmpty())
        throw exQueryBuilder("Seems that condition is empty");

    return CondStr;
}
/***************************************************************************************/
/***************************************************************************************/
/***************************************************************************************/
struct stuColSpecs {
    QString Name;
    QString RenameAs;
//    union unnAggregation {
//        enuAggregation::Type Simple;
//        enuConditionalAggregation::Type Conditional;
//    };
//    NULLABLE_TYPE(unnAggregation) Aggregation = NULLABLE_NULL_VALUE;
    NULLABLE_TYPE(enuAggregation::Type) SimpleAggregation;
    NULLABLE_TYPE(enuConditionalAggregation::Type) ConditionalAggregation;
    clsCondition Condition;
    QVariant TrueValue;
    QVariant FalseValue;
//    DBExpression Expression;
    QVariant Expression;

    stuColSpecs(const stuColSpecs& _other) :
        Name(_other.Name),
        RenameAs(_other.RenameAs),
        SimpleAggregation(_other.SimpleAggregation),
        ConditionalAggregation(_other.ConditionalAggregation),
        Condition(_other.Condition),
        TrueValue(_other.TrueValue),
        FalseValue(_other.FalseValue),
        Expression(_other.Expression)
    {}
    stuColSpecs(
            const QString& _name,
            const QString& _renameAs = {},
            const NULLABLE_TYPE(enuAggregation::Type)& _aggregation_Simple = NULLABLE_NULL_VALUE,
            const NULLABLE_TYPE(enuConditionalAggregation::Type)& _ConditionalAggregation = NULLABLE_NULL_VALUE,
            const clsCondition& _condition = {},
            const QVariant& _trueValue = {},
            const QVariant& _falseValue = {}
        ) :
        Name(_name),
        RenameAs(_renameAs),
        SimpleAggregation(_aggregation_Simple),
        ConditionalAggregation(_ConditionalAggregation),
        Condition(_condition),
        TrueValue(_trueValue),
        FalseValue(_falseValue)
    {}
    stuColSpecs(
            const DBExpression& _expression,
            const QString& _renameAs = {}
        ) :
        RenameAs(_renameAs),
        Expression(_expression)
    {}
};

/*
struct stuJoin {
    bool IsWith = false;
    enuJoinType::Type JoinType;
//    union {
        QString ForeignTable;
        QString RelationName;
//    };
    QString Alias;
    clsCondition On;

    stuJoin(const enuJoinType::Type& _joinType, const QString& _foreignTable) :
        IsWith(false), JoinType(_joinType), ForeignTable(_foreignTable)
    {}

    stuJoin(const enuJoinType::Type& _joinType, const QString& _foreignTable, const QString& _alias) :
        IsWith(false), JoinType(_joinType), ForeignTable(_foreignTable), Alias(_alias)
    {}

    stuJoin(const enuJoinType::Type& _joinType, const QString& _foreignTable, const clsCondition& _on) :
        IsWith(false), JoinType(_joinType), ForeignTable(_foreignTable), On(_on)
    {}

    stuJoin(const enuJoinType::Type& _joinType, const QString& _foreignTable, const QString& _alias, const clsCondition& _on) :
        IsWith(false), JoinType(_joinType), ForeignTable(_foreignTable), Alias(_alias), On(_on)
    {}

    stuJoin(bool _isWith, const enuJoinType::Type& _joinType, const QString& _relationName, const QString& _alias = {}) :
        IsWith(_isWith), JoinType(_joinType), RelationName(_relationName), Alias(_alias)
    {}
};
*/

struct stuJoin {
    enuJoinType::Type JoinType;
    QString ForeignTable;
    QString Alias;
    clsCondition On;

    stuJoin(const enuJoinType::Type& _joinType, const QString& _foreignTable) :
        JoinType(_joinType), ForeignTable(_foreignTable)
    {}

    stuJoin(const enuJoinType::Type& _joinType, const QString& _foreignTable, const QString& _alias) :
        JoinType(_joinType), ForeignTable(_foreignTable), Alias(_alias)
    {}

    stuJoin(const enuJoinType::Type& _joinType, const QString& _foreignTable, const clsCondition& _on) :
        JoinType(_joinType), ForeignTable(_foreignTable), On(_on)
    {}

    stuJoin(const enuJoinType::Type& _joinType, const QString& _foreignTable, const QString& _alias, const clsCondition& _on) :
        JoinType(_joinType), ForeignTable(_foreignTable), Alias(_alias), On(_on)
    {}
};

struct stuOrderBy {
    QString Col;
    enuOrderDir::Type Dir;

//    stuOrderBy() {}
    stuOrderBy(const stuOrderBy& _other) : Col(_other.Col), Dir(_other.Dir) {}
    stuOrderBy(const QString& _col, const enuOrderDir::Type& _dir) : Col(_col), Dir(_dir) {}
};

TARGOMAN_DEFINE_ENUM(enuUnionType,
                     Normal,
                     All,
                     Distinct)

struct stuUnion {
    SelectQuery Query;
    enuUnionType::Type UnionType;

    stuUnion(const stuUnion& _other) : Query(_other.Query), UnionType(_other.UnionType) {}
    stuUnion(const SelectQuery& _query, enuUnionType::Type _unionType) : Query(_query), UnionType(_unionType) {}
};

/*
 * struct stuConditionalClause {
    bool IsAnd;
    clsCondition Condition;

//    stuConditionalClause() {}
    stuConditionalClause(const stuConditionalClause& _other) : IsAnd(_other.IsAnd), Condition(_other.Condition) {}
    stuConditionalClause(bool _isAnd, const clsCondition& _condition) : IsAnd(_isAnd), Condition(_condition) {}
};
*/

/***************************************************************************************/
/* clsBaseQueryData ********************************************************************/
/***************************************************************************************/
struct stuBaseQueryPreparedItems {
    QString     From;
//    QString     Where;
//    QStringList GroupBy;
//    QString     Having;
    bool        IsPrepared = false;
};

template <class itmplDerived>
class clsBaseQueryData : public QSharedData
{
public:
    clsBaseQueryData(clsTable& _table, const QString& _alias = {}) :
        Table(_table), Alias(_alias)
    {}

    virtual void prepare(bool _useBinding/*, quint8 _prettifierJustifyLen*/) {
        Q_UNUSED(_useBinding);
//        Q_UNUSED(_prettifierJustifyLen);

        if (this->BaseQueryPreparedItems.IsPrepared)
            return;

        this->BaseQueryPreparedItems.IsPrepared = true;

        /****************************************************************************/
        QString MainTableNameOrAlias = this->Alias.length() ? this->Alias : this->Table.Name;

        if (this->Alias.length())
            this->BaseQueryPreparedItems.From = this->Table.Schema + "." + this->Table.Name + " " + this->Alias;
        else
            this->BaseQueryPreparedItems.From = this->Table.Schema + "." + this->Table.Name;
    }

    friend clsQueryJoinTraitData<itmplDerived>;
    friend clsQueryWhereTraitData<itmplDerived>;
    friend clsQueryGroupAndHavingTraitData<itmplDerived>;

public:
    clsTable&   Table;
    QString     Alias;
    stuBaseQueryPreparedItems   BaseQueryPreparedItems;
};

/***************************************************************************************/
/* tmplBaseQuery ***********************************************************************/
/***************************************************************************************/
template <class itmplDerived, class itmplData>
tmplBaseQuery<itmplDerived, itmplData>::tmplBaseQuery() :
    Data(nullptr)
{}

template <class itmplDerived, class itmplData>
tmplBaseQuery<itmplDerived, itmplData>::tmplBaseQuery(const tmplBaseQuery<itmplDerived, itmplData>& _other) :
    Data(_other.Data)
{}

template <class itmplDerived, class itmplData>
tmplBaseQuery<itmplDerived, itmplData>::tmplBaseQuery(clsTable& _table, const QString& _alias) :
    Data(new itmplData(_table, _alias)) {
    if (_table.AllCols.isEmpty())
        throw exQueryBuilder("Call prepareFiltersList on table before creating a QueryBuilder");
}

template <class itmplDerived, class itmplData>
tmplBaseQuery<itmplDerived, itmplData>::~tmplBaseQuery() {
//    if (this->dac != nullptr)
//        delete this->dac;
//    this->dac = nullptr;
}

template <class itmplDerived, class itmplData>
bool tmplBaseQuery<itmplDerived, itmplData>::isValid() {
    return (this->Data != nullptr);
}

//template <class itmplDerived, class itmplData>
//clsDAC tmplBaseQuery<itmplDerived, itmplData>::DAC() {
//    return clsDAC(this->Data->Table.domain(), this->Data->Table.Schema);
//}

/***************************************************************************************/
/* clsQueryJoinTraitData ***************************************************************/
/***************************************************************************************/
struct stuQueryJoinTraitPreparedItems {
    QStringList Joins;
};

template <class itmplDerived>
class clsQueryJoinTraitData : public QSharedData
{
public:
    clsQueryJoinTraitData<itmplDerived>(const itmplDerived* _owner) :
        Owner(_owner), IsPrepared(false)
    {}

    virtual void prepare() {
        if (this->IsPrepared)
            return;
        this->IsPrepared = true;

        /****************************************************************************/
        QString MainTableNameOrAlias = this->Owner->Data->Alias.length() ? this->Owner->Data->Alias : this->Owner->Data->Table.Name;

        /****************************************************************************/
//        foreach (auto Join, UsedJoins) {
//            this->BaseQueryPreparedItems.Joins.append((Join.LeftJoin ? "LEFT JOIN " : "JOIN ")
//                + Join.ReferenceTable
//                + (Join.RenamingPrefix.size() ? " `" + Join.RenamingPrefix + "`" : "")
//                + " ON "
//                + (Join.RenamingPrefix.size() ? "`" + Join.RenamingPrefix + "`" : Join.ReferenceTable) + "." + Join.ForeignColumn
//                + " = "
//                + MainTableName + "." + Join.Column
//            );
//        }

        if (this->Joins.size())
        {
            QSet<QString> AppliedJoins;

            foreach (stuJoin Join, this->Joins) {
                if (Join.On.isEmpty()) {
                    if (Join.Alias.length())
                        throw exHTTPInternalServerError(QString("Condition not defined for join on table (%1) alias (%2).").arg(Join.ForeignTable).arg(Join.Alias));
                    else
                        throw exHTTPInternalServerError(QString("Condition not defined for join on table (%1).").arg(Join.ForeignTable));
                }

                //1: check duplication
                if (Join.Alias.length())
                {
                    if (AppliedJoins.contains(Join.Alias))
                        throw exHTTPInternalServerError(QString("Duplicated join on table (%1) alias (%2).").arg(Join.ForeignTable).arg(Join.Alias));
                    AppliedJoins.insert(Join.Alias);
                }
                else
                {
                    if (AppliedJoins.contains(Join.ForeignTable))
                        throw exHTTPInternalServerError(QString("Duplicated join on table (%1).").arg(Join.ForeignTable));
                    AppliedJoins.insert(Join.ForeignTable);
                }

                //2: find relation definition
                stuRelation* Relation = nullptr;
                foreach (stuRelation Rel, this->Owner->Data->Table.Relations)
                {
                    if (Rel.ReferenceTable == Join.ForeignTable) {
                        Relation = &Rel;
                        break;
                    }
                }
                if (Relation == nullptr) {
                    throw exHTTPInternalServerError(QString("Relation to table (%1) has not been defined.").arg(Join.ForeignTable));
                }

                clsTable* ForeignTable = clsTable::Registry[Relation->ReferenceTable];
                if (ForeignTable == nullptr)
                    throw exHTTPInternalServerError(QString("Reference table (%1) has not been registered.").arg(Relation->ReferenceTable));

//                Join.JoinType;
//                Join.ForeignTable;
//                Join.Alias;
//                Join.On;

                //3: create join clause
                QString j = enuJoinType::toStr(Join.JoinType);
                j += " JOIN";
                if (SQLPrettyLen)
                    j = j.rightJustified(SQLPrettyLen);
                j += " ";
                j += Relation->ReferenceTable;
                if (Join.Alias.size())
                    j += " " + Join.Alias;
                if (Join.JoinType != enuJoinType::CROSS)
                {
                    if (Join.On.isEmpty())
                        throw exHTTPInternalServerError("Condition part of relation not defined.");

                    if (SQLPrettyLen)
                        j += "\n" + QString("ON").rightJustified(SQLPrettyLen) + " ";
                    else
                        j += " ON ";

                    j += Join.On.buildConditionString(
                        MainTableNameOrAlias,
                        this->Owner->Data->Table.FilterableColsMap,
                        false
//                        _prettifierJustifyLen
                    );
                }
                this->PreparedItems.Joins.append(j);

                //4: append columns
//                QSet<stuRelation> UsedJoins;
//                foreach (stuRelation Relation, this->Table.Relations) {
//                    clsTable* ForeignTable = clsTable::Registry[Relation.ReferenceTable];
//                    if (ForeignTable == nullptr)
//                        throw exHTTPInternalServerError("Reference table has not been registered: " + Relation.ReferenceTable);

//                    bool Joined = false;
//                    if (this->RequiredCols.isEmpty())
//                        foreach(auto Col, ForeignTable->BaseCols)
//                            this->SelectQueryPreparedItems.Cols.append(makeColName(MainTableNameOrAlias, Col, true, Relation));
//                    else
//                        foreach(auto RequiredCol, this->RequiredCols)
//                            if (addCol(RequiredCol, Relation))
//                                Joined = true;

//                    if (Joined)
//                        UsedJoins.insert(Relation);
//                }
            }
        }
    }

public:
    const itmplDerived*             Owner;
    bool                            IsPrepared = false;
    QList<stuJoin>                  Joins;
    stuQueryJoinTraitPreparedItems  PreparedItems;
};

/***************************************************************************************/
/* tmplQueryJoinTrait **********************************************************************/
/***************************************************************************************/
template <class itmplDerived>
tmplQueryJoinTrait<itmplDerived>::tmplQueryJoinTrait(const tmplQueryJoinTrait<itmplDerived>& _other) :
    JoinTraitData(_other.JoinTraitData) {}

template <class itmplDerived>
tmplQueryJoinTrait<itmplDerived>::tmplQueryJoinTrait(const itmplDerived* _owner) :
    JoinTraitData(new clsQueryJoinTraitData<itmplDerived>(_owner)) {
}

template <class itmplDerived>
tmplQueryJoinTrait<itmplDerived>::~tmplQueryJoinTrait() {}

/***********************\
|* Join                *|
\***********************/
template <class itmplDerived>
itmplDerived& tmplQueryJoinTrait<itmplDerived>::join(enuJoinType::Type _joinType, const QString& _foreignTable, const QString& _alias, const clsCondition& _on)
{
    if (_foreignTable.isEmpty())
        throw exHTTPInternalServerError("Foreign Table is empty.");

    if ((_joinType == enuJoinType::CROSS) || !_on.isEmpty()) {
        this->JoinTraitData->Joins.append({ _joinType, _foreignTable, _alias, _on });
        return (itmplDerived&)*this;
    }

    //find relation definition
    stuRelation* Relation = nullptr;

    foreach (stuRelation Rel, this->JoinTraitData->Owner->Data->Table.Relations)
    {
        if (Rel.ReferenceTable == _foreignTable) {
            if (Relation != nullptr)
                throw exHTTPInternalServerError(QString("Multiple relations defined to table (%1).").arg(_foreignTable));

            Relation = &Rel;
//            break;
        }
    }

    if (Relation == nullptr)
        throw exHTTPInternalServerError(QString("Relation on table (%1) has not been defined.").arg(_foreignTable));

    clsTable* ForeignTable = clsTable::Registry[Relation->ReferenceTable];

    if (ForeignTable == nullptr)
        throw exHTTPInternalServerError(QString("Reference table (%1) has not been registered.").arg(Relation->ReferenceTable));

    clsCondition On(
        _alias.length() ? _alias : ForeignTable->Name,
        Relation->ForeignColumn,
        enuConditionOperator::Equal,
        this->JoinTraitData->Owner->Data->Alias.length() ? this->JoinTraitData->Owner->Data->Alias : this->JoinTraitData->Owner->Data->Table.Name,
        Relation->Column
    );

    this->JoinTraitData->Joins.append({ _joinType, _foreignTable, _alias, On });
    return (itmplDerived&)*this;
}

template <class itmplDerived> itmplDerived& tmplQueryJoinTrait<itmplDerived>::leftJoin (const QString& _foreignTable, const clsCondition& _on)                        { return this->join(enuJoinType::LEFT,  _foreignTable, {},     _on); }
template <class itmplDerived> itmplDerived& tmplQueryJoinTrait<itmplDerived>::leftJoin (const QString& _foreignTable, const QString& _alias, const clsCondition& _on) { return this->join(enuJoinType::LEFT,  _foreignTable, _alias, _on); }
template <class itmplDerived> itmplDerived& tmplQueryJoinTrait<itmplDerived>::rightJoin(const QString& _foreignTable, const clsCondition& _on)                        { return this->join(enuJoinType::RIGHT, _foreignTable, {},     _on); }
template <class itmplDerived> itmplDerived& tmplQueryJoinTrait<itmplDerived>::rightJoin(const QString& _foreignTable, const QString& _alias, const clsCondition& _on) { return this->join(enuJoinType::RIGHT, _foreignTable, _alias, _on); }
template <class itmplDerived> itmplDerived& tmplQueryJoinTrait<itmplDerived>::innerJoin(const QString& _foreignTable, const clsCondition& _on)                        { return this->join(enuJoinType::INNER, _foreignTable, {},     _on); }
template <class itmplDerived> itmplDerived& tmplQueryJoinTrait<itmplDerived>::innerJoin(const QString& _foreignTable, const QString& _alias, const clsCondition& _on) { return this->join(enuJoinType::INNER, _foreignTable, _alias, _on); }
template <class itmplDerived> itmplDerived& tmplQueryJoinTrait<itmplDerived>::crossJoin(const QString& _foreignTable, const QString& _alias)                          { return this->join(enuJoinType::CROSS, _foreignTable, _alias);      }

//template <class itmplDerived, class itmplData> itmplDerived& tmplQueryJoinTrait<itmplDerived>::leftJoin(const template <class itmplDerived, class itmplData> tmplQueryJoinTrait<itmplData>& _nestedQuery, const QString _alias, const clsCondition& _on)
//{
//    return this->join(enuJoinType::LEFT, _nestedQuery.buildQueryString(_currentUserID), _alias, _on);
//}
//template <class itmplDerived, class itmplData> tmplQueryJoinTrait<itmplData>& rightJoin(const template <class itmplDerived, class itmplData> tmplQueryJoinTrait<itmplData>& _nestedQuery, const QString _alias, const clsCondition& _on);
//template <class itmplDerived, class itmplData> tmplQueryJoinTrait<itmplData>& innerJoin(const template <class itmplDerived, class itmplData> tmplQueryJoinTrait<itmplData>& _nestedQuery, const QString _alias, const clsCondition& _on);
//template <class itmplDerived, class itmplData> tmplQueryJoinTrait<itmplData>& crossJoin(const template <class itmplDerived, class itmplData> tmplQueryJoinTrait<itmplData>& _nestedQuery, const QString _alias);
//template <class itmplDerived, class itmplData> tmplQueryJoinTrait<itmplData>& join(enuJoinType::Type _joinType, const template <class itmplDerived, class itmplData> tmplQueryJoinTrait<itmplData>& _nestedQuery, const QString _alias, const clsCondition& _on);

template <class itmplDerived>
itmplDerived& tmplQueryJoinTrait<itmplDerived>::joinWith(enuJoinType::Type _joinType, const QString& _relationName, const QString& _alias)
{
    if (_relationName.isEmpty())
        throw exHTTPInternalServerError("Relation Name is empty.");

//    qDebug() << "searching: " << _relationName;

    //find relation definition
    stuRelation* Relation = nullptr;

    for (QList<stuRelation>::iterator iter = this->JoinTraitData->Owner->Data->Table.Relations.begin();
         iter != this->JoinTraitData->Owner->Data->Table.Relations.end();
         iter++) {

        stuRelation Rel = *iter;

        if (Rel.RelationName.length() && (Rel.RelationName == _relationName)) {
//            qDebug() << "found: (" << Rel.RelationName << ") <-> (" << _relationName << ")";

            if (Relation != nullptr)
                throw exHTTPInternalServerError(QString("Multiple relations defined with name (%1).").arg(_relationName));

            Relation = &(*iter);
//            break;
        }
    }
    if (Relation == nullptr)
        throw exHTTPInternalServerError(QString("Relation with name (%1) has not been defined.").arg(_relationName));

//    qDebug() << "found: RelationName(" << Relation->RelationName << ") ReferenceTable(" << Relation->ReferenceTable << ")";

    clsTable* ForeignTable = clsTable::Registry[Relation->ReferenceTable];

    if (ForeignTable == nullptr)
        throw exHTTPInternalServerError(QString("Reference table (%1) has not been registered.").arg(Relation->ReferenceTable));

//    qDebug() << "foreign table found: " << ForeignTable->Name;

    clsCondition On(
        _alias.length() ? _alias : ForeignTable->Name,
        Relation->ForeignColumn,
        enuConditionOperator::Equal,
        this->JoinTraitData->Owner->Data->Alias.length() ? this->JoinTraitData->Owner->Data->Alias : this->JoinTraitData->Owner->Data->Table.Name,
        Relation->Column
    );

    this->join(_joinType, Relation->ReferenceTable, _alias, On);
    return (itmplDerived&)*this;
}
template <class itmplDerived> itmplDerived& tmplQueryJoinTrait<itmplDerived>::leftJoinWith (const QString& _relationName, const QString& _alias) { return this->joinWith(enuJoinType::LEFT,  _relationName, _alias); }
template <class itmplDerived> itmplDerived& tmplQueryJoinTrait<itmplDerived>::rightJoinWith(const QString& _relationName, const QString& _alias) { return this->joinWith(enuJoinType::RIGHT, _relationName, _alias); }
template <class itmplDerived> itmplDerived& tmplQueryJoinTrait<itmplDerived>::innerJoinWith(const QString& _relationName, const QString& _alias) { return this->joinWith(enuJoinType::INNER, _relationName, _alias); }

/***************************************************************************************/
/* clsQueryWhereTraitData **************************************************************/
/***************************************************************************************/
struct stuQueryWhereTraitPreparedItems {
    QString Where;
};

template <class itmplDerived>
class clsQueryWhereTraitData : public QSharedData
{
public:
    clsQueryWhereTraitData<itmplDerived>(const itmplDerived* _owner) :
        Owner(_owner), IsPrepared(false)
    {}

    virtual void prepare() {
        if (this->IsPrepared)
            return;
        this->IsPrepared = true;

        /****************************************************************************/
        QString MainTableNameOrAlias = this->Owner->Data->Alias.length() ? this->Owner->Data->Alias : this->Owner->Data->Table.Name;

        /****************************************************************************/
        this->PreparedItems.Where = this->WhereClauses.buildConditionString(
            MainTableNameOrAlias,
            this->Owner->Data->Table.FilterableColsMap,
            false/*,
            _prettifierJustifyLen*/);

        /****************************************************************************/
//        qDebug() << __FILE__ << ":" << __FUNCTION__ << "() :" << "filters:" << this->Filters;

        if (this->Filters.length()) {
            if (this->PreparedItems.Where.length())
                this->PreparedItems.Where += SQLPrettyLen ? "\n" + QString("AND").rightJustified(SQLPrettyLen) + " " : " AND ";

            QString Filters = this->Filters.join(" ");

            quint8 OpenParenthesis = 0;
            bool StatusColHasCriteria = false;
            bool CanStartWithLogical = false;
            QString LastLogical = "";
            Filters = Filters.replace("\\ ", "$SPACE$");
            foreach (auto Filter, Filters.split(" ", QString::SkipEmptyParts)) {
                QString Rule;
                Filter = Filter.trimmed ();
                if (Filter == "(") {
                    Rule = LastLogical + "(";
                    CanStartWithLogical = false;
                    LastLogical.clear();
                    OpenParenthesis++;
                }
                else if(Filter == ")") {
                    if(OpenParenthesis <= 0) throw exHTTPBadRequest("Invalid close parenthesis without any open");
                    Rule = " )";
                    OpenParenthesis--;
                    CanStartWithLogical = true;
                }
                else if (Filter == '+' || Filter == '|' || Filter == '*') {
                    if (CanStartWithLogical == false)
                        throw exHTTPBadRequest("Invalid logical expression prior to any rule");
                    if (Filter == '+') LastLogical = "AND ";
                    else if (Filter == '|') LastLogical = "OR ";
                    else if (Filter == '*') LastLogical = "XOR ";

                    CanStartWithLogical = false;
                    continue;
                }
                else {
                    static QRegularExpression rxFilterPattern("([a-zA-Z0-9\\_]+)([<>!=~]=?)(.+)");
                    Filter = Filter.replace("$SPACE$", " ");
                    QRegularExpressionMatch PatternMatches = rxFilterPattern.match(Filter);
                    if (PatternMatches.lastCapturedIndex() != 3)
                        throw exHTTPBadRequest("Invalid filter set: " + Filter);

                    Rule = SQLPrettyLen ? "\n" + QString(LastLogical).rightJustified(SQLPrettyLen) + " " : " " + LastLogical + " ";

                    stuRelatedORMField relatedORMField = this->Owner->Data->Table.FilterableColsMap.value(PatternMatches.captured(1).trimmed());
                    if (relatedORMField.isValid())
                        Rule += makeColName(this->Owner->Data->Table.Name, relatedORMField.Col, false, relatedORMField.Relation);
                    else
                        throw exHTTPBadRequest("Invalid column for filtring: " + PatternMatches.captured(1));

                    if (relatedORMField.Col.updatableBy() == enuUpdatableBy::__STATUS__)
                        StatusColHasCriteria = true;

                    ///TODO: complete UsedJoins
//                    if (relatedORMField.Relation.Column.size() && UsedJoins.contains(relatedORMField.Relation) == false)
//                        UsedJoins.insert(relatedORMField.Relation);

                    if (PatternMatches.captured(3) == "NULL") {
                        if (PatternMatches.captured(2) == "=")
                            Rule += " IS NULL";
                        else if (PatternMatches.captured(2) == "!=")
                            Rule += " IS NOT NULL";
                        else
                            throw exHTTPBadRequest("Invalid filter with NULL expression: " + Filter);

                        if (this->PreparedItems.Where.length())
                            this->PreparedItems.Where += " ";
                        this->PreparedItems.Where += Rule;

                        CanStartWithLogical = true;
                        LastLogical.clear();
                        continue;
                    }

                    if(PatternMatches.captured(2) == "<") Rule += " < ";
                    else if(PatternMatches.captured(2) == "<=") Rule += " <= ";
                    else if(PatternMatches.captured(2) == ">") Rule += " > ";
                    else if(PatternMatches.captured(2) == ">=") Rule += " >= ";
                    else if(PatternMatches.captured(2) == "!=") Rule += " != ";
                    else if(PatternMatches.captured(2) == "~=") Rule += " LIKE ";
                    else if(PatternMatches.captured(2) == "=") Rule += " = ";
                    else throw exHTTPBadRequest("Invalid filter criteria: " + Filter);

                    Rule += relatedORMField.Col.argSpecs().isPrimitiveType() ? "" : "'";
                    QString Value = PatternMatches.captured(3);
                    if (Value == "NOW()"
                            || Value.startsWith("DATE_ADD(")
                            || Value.startsWith("DATE_SUB(")
                        )
                        Rule += Value.replace("$SPACE$", " ");
                    else {
                        relatedORMField.Col.argSpecs().validate(Value, PatternMatches.captured(1).trimmed().toLatin1());
                        Rule += relatedORMField.Col.toDB(Value).toString();
                    }
                    Rule += relatedORMField.Col.argSpecs().isPrimitiveType() ? "" : "'";

                    CanStartWithLogical = true;
                    LastLogical.clear();
                }
                if (this->PreparedItems.Where.length())
                    this->PreparedItems.Where += " ";
                this->PreparedItems.Where += Rule;
            }
        }

        /****************************************************************************/
        if (this->PksByPath.size())
        {
            QStringList PrimaryKeyQueries = this->PksByPath.split(";");
            QStringList pkFilters;
            foreach (auto Query, PrimaryKeyQueries) {
                foreach (auto Col, this->Owner->Data->Table.BaseCols)
                if (Col.isPrimaryKey())
                {
                    if (Query.size())
                        pkFilters.append(makeColName(MainTableNameOrAlias, Col) + " = \"" + Query + "\"");
                    break;
                }
            }
            if (pkFilters.isEmpty())
                throw exQueryBuilder("pksByPath had no results");

            if (this->PreparedItems.Where.isEmpty())
                this->PreparedItems.Where = pkFilters.join(" AND ");
            else
                this->PreparedItems.Where = QString("%1 AND (%2)")
                                            .arg(pkFilters.join(" AND "))
                                            .arg(this->PreparedItems.Where);
        }

        /****************************************************************************/

    }

public:
    const itmplDerived*             Owner;
    bool                            IsPrepared = false;
    clsCondition                    WhereClauses;
    TAPI::PKsByPath_t               PksByPath;
    QStringList                     Filters;
    stuQueryWhereTraitPreparedItems PreparedItems;
};

/***************************************************************************************/
/* tmplQueryWhereTrait *********************************************************************/
/***************************************************************************************/
template <class itmplDerived>
tmplQueryWhereTrait<itmplDerived>::tmplQueryWhereTrait(const tmplQueryWhereTrait<itmplDerived>& _other) :
    WhereTraitData(_other.WhereTraitData) {}

template <class itmplDerived>
tmplQueryWhereTrait<itmplDerived>::tmplQueryWhereTrait(const itmplDerived* _owner) :
    WhereTraitData(new clsQueryWhereTraitData<itmplDerived>(_owner)) {
}

template <class itmplDerived>
tmplQueryWhereTrait<itmplDerived>::~tmplQueryWhereTrait() {}

/***********************\
|* Where               *|
\***********************/
template <class itmplDerived>
itmplDerived& tmplQueryWhereTrait<itmplDerived>::where(const clsCondition& _condition)
{
    if (_condition.isEmpty() == false)
        this->WhereTraitData->WhereClauses = _condition;

    return (itmplDerived&)*this;
}

template <class itmplDerived>
itmplDerived& tmplQueryWhereTrait<itmplDerived>::andWhere(const clsCondition& _condition)
{
    if (_condition.isEmpty() == false) {
        if (this->WhereTraitData->WhereClauses.isEmpty())
            this->WhereTraitData->WhereClauses = _condition;
        else
            this->WhereTraitData->WhereClauses.andCond(_condition);
    }

    return (itmplDerived&)*this;
}

template <class itmplDerived>
itmplDerived& tmplQueryWhereTrait<itmplDerived>::orWhere(const clsCondition& _condition)
{
    if (_condition.isEmpty() == false) {
        if (this->WhereTraitData->WhereClauses.isEmpty())
            this->WhereTraitData->WhereClauses = _condition;
        else
            this->WhereTraitData->WhereClauses.orCond(_condition);
    }

    return (itmplDerived&)*this;
}

template <class itmplDerived>
itmplDerived& tmplQueryWhereTrait<itmplDerived>::xorWhere(const clsCondition& _condition)
{
    if (_condition.isEmpty() == false) {
        if (this->WhereTraitData->WhereClauses.isEmpty())
            this->WhereTraitData->WhereClauses = _condition;
        else
            this->WhereTraitData->WhereClauses.xorCond(_condition);
    }

    return (itmplDerived&)*this;
}

/***********************\
|* PKsByPath           *|
\***********************/
template <class itmplDerived>
itmplDerived& tmplQueryWhereTrait<itmplDerived>::setPksByPath(TAPI::PKsByPath_t _pksByPath)
{
    this->WhereTraitData->PksByPath = _pksByPath.trimmed();

    return (itmplDerived&)*this;
}

/***********************\
|* Filters             *|
\***********************/
template <class itmplDerived>
itmplDerived& tmplQueryWhereTrait<itmplDerived>::addFilters(const QString& _filters)
{
    QString Filters = _filters.trimmed();

    if (Filters.isEmpty() == false) {
        this->WhereTraitData->Filters.append(Filters);

//        QString MainTableNameOrAlias = this->WhereTraitData->Owner->Data->Alias.length()
//                                       ? this->WhereTraitData->Owner->Data->Alias
//                                       : this->WhereTraitData->Owner->Data->Table.Name;

//        this->andWhere(clsCondition::parse(Filters, MainTableNameOrAlias, this->WhereTraitData->Owner->Data->Table.FilterableColsMap));
    }

    return (itmplDerived&)*this;
}

/***************************************************************************************/
/* clsQueryGroupAndHavingTraitData *****************************************************/
/***************************************************************************************/
struct stutmplQueryGroupAndHavingTraitPreparedItems {
    QStringList GroupBy;
    QString     Having;
};

template <class itmplDerived>
class clsQueryGroupAndHavingTraitData : public QSharedData
{
public:
    clsQueryGroupAndHavingTraitData<itmplDerived>(const itmplDerived* _owner) :
        Owner(_owner), IsPrepared(false)
    {}

    virtual void prepare() {
        if (this->IsPrepared)
            return;
        this->IsPrepared = true;

        /****************************************************************************/
        QString MainTableNameOrAlias = this->Owner->Data->Alias.length() ? this->Owner->Data->Alias : this->Owner->Data->Table.Name;

        /****************************************************************************/
        if (this->GroupByCols.length()) {
            foreach (auto groupByCol, this->GroupByCols) {
                this->PreparedItems.GroupBy.append(groupByCol);
            }
        }

        /****************************************************************************/
        this->PreparedItems.Having = this->HavingClauses.buildConditionString(
            MainTableNameOrAlias,
            this->Owner->Data->Table.FilterableColsMap,
            true/*,
            _prettifierJustifyLen*/);
    }

public:
    const itmplDerived* Owner;
    bool             IsPrepared = false;
    QStringList      GroupByCols;
    clsCondition     HavingClauses;
    stutmplQueryGroupAndHavingTraitPreparedItems PreparedItems;
};

/***************************************************************************************/
/* tmplQueryGroupAndHavingTrait ********************************************************/
/***************************************************************************************/
template <class itmplDerived>
tmplQueryGroupAndHavingTrait<itmplDerived>::tmplQueryGroupAndHavingTrait(const tmplQueryGroupAndHavingTrait<itmplDerived>& _other) :
    GroupAndHavingTraitData(_other.GroupAndHavingTraitData) {}

template <class itmplDerived>
tmplQueryGroupAndHavingTrait<itmplDerived>::tmplQueryGroupAndHavingTrait(const itmplDerived* _owner) :
    GroupAndHavingTraitData(new clsQueryGroupAndHavingTraitData<itmplDerived>(_owner)) {
}

template <class itmplDerived>
tmplQueryGroupAndHavingTrait<itmplDerived>::~tmplQueryGroupAndHavingTrait() {}

/***********************\
|* Group               *|
\***********************/
//template <class itmplDerived> itmplDerived& tmplQueryGroupAndHavingTrait<itmplDerived>::groupBy(const clsCondition& _condition);
template <class itmplDerived>
itmplDerived& tmplQueryGroupAndHavingTrait<itmplDerived>::groupBy(const QString& _col)
{
    if (_col.isEmpty() == false)
        this->GroupAndHavingTraitData->GroupByCols.append(_col);

    return (itmplDerived&)*this;
}

template <class itmplDerived>
itmplDerived& tmplQueryGroupAndHavingTrait<itmplDerived>::groupBy(const QStringList& _cols)
{
    if (_cols.length()) {
        foreach(auto Col, _cols)
            this->groupBy(Col);
    }

    return (itmplDerived&)*this;
}

/***********************\
|* Having              *|
\***********************/
template <class itmplDerived>
itmplDerived& tmplQueryGroupAndHavingTrait<itmplDerived>::having(const clsCondition& _condition)
{
//    if (this->GroupAndHavingTraitData->HavingClauses.isEmpty() != false)
//        throw exQueryBuilder("Having clauses are not empty. Please use andHaving, orHaving or xorHaving");

    this->GroupAndHavingTraitData->HavingClauses = _condition;
    return (itmplDerived&)*this;
}

template <class itmplDerived>
itmplDerived& tmplQueryGroupAndHavingTrait<itmplDerived>::andHaving(const clsCondition& _condition)
{
    if (this->GroupAndHavingTraitData->HavingClauses.isEmpty())
        this->GroupAndHavingTraitData->HavingClauses = _condition;
    else
        this->GroupAndHavingTraitData->HavingClauses.andCond(_condition);
    return (itmplDerived&)*this;
}

template <class itmplDerived>
itmplDerived& tmplQueryGroupAndHavingTrait<itmplDerived>::orHaving(const clsCondition& _condition)
{
    if (this->GroupAndHavingTraitData->HavingClauses.isEmpty())
        this->GroupAndHavingTraitData->HavingClauses = _condition;
    else
        this->GroupAndHavingTraitData->HavingClauses.orCond(_condition);
    return (itmplDerived&)*this;
}

template <class itmplDerived>
itmplDerived& tmplQueryGroupAndHavingTrait<itmplDerived>::xorHaving(const clsCondition& _condition)
{
    if (this->GroupAndHavingTraitData->HavingClauses.isEmpty())
        this->GroupAndHavingTraitData->HavingClauses = _condition;
    else
        this->GroupAndHavingTraitData->HavingClauses.xorCond(_condition);
    return (itmplDerived&)*this;
}

/***************************************************************************************/
/* clsSelectQueryData ******************************************************************/
/***************************************************************************************/
struct stuSelectQueryPreparedItems {
    QStringList Cols;
    QStringList OrderBy;
    QStringList Unions;
    bool        IsPrepared = false;
};

class clsSelectQueryData : public clsBaseQueryData<SelectQuery>
{
public:
    clsSelectQueryData(clsTable& _table, const QString& _alias = {}) : clsBaseQueryData<SelectQuery>(_table, _alias) {}
//    clsSelectQueryData(const clsSelectQueryData& _other) : Table(_other.Table), Alias(_other.Alias) {}
//    ~clsSelectQueryData() {}

    virtual void prepare(bool _useBinding/*, quint8 _prettifierJustifyLen*/) {
        if (this->SelectQueryPreparedItems.IsPrepared)
            return;
        this->SelectQueryPreparedItems.IsPrepared = true;

        clsBaseQueryData<SelectQuery>::prepare(_useBinding/*, _prettifierJustifyLen*/);

        /****************************************************************************/
        QString MainTableNameOrAlias = this->Alias.length() ? this->Alias : this->Table.Name;

        /****************************************************************************/
        auto addCol = [this, &MainTableNameOrAlias/*, _prettifierJustifyLen*/](const stuColSpecs& _col, const stuRelation& _relation = InvalidRelation) {
            auto updateRename = [_col](QString _fieldString) {
                if (_col.RenameAs.isEmpty())
                    return _fieldString;
                if (_fieldString.contains(" AS "))
                    _fieldString.replace(QRegularExpression(" AS .*"), "");
                return _fieldString + " AS " + _col.RenameAs;
            }; //updateRename

            if (_col.Expression.isValid()) {
                DBExpression exp = _col.Expression.value<DBExpression>();
                this->SelectQueryPreparedItems.Cols.append(
                    exp.name() //Data->Name
                    + (_col.RenameAs.length()
                       ? " AS " + _col.RenameAs
                       : ""
                    )
                );
            }
            else if (NULLABLE_HAS_VALUE(_col.ConditionalAggregation)) {
                if (_col.Condition.isEmpty())
                    throw exQueryBuilder("Condition is not provided for conditional aggregation");

                if (_col.TrueValue.isValid() != _col.FalseValue.isValid())
                    throw exQueryBuilder(QString("Emptiness of TrueValue and FalseValue must be the same").arg(_col.Name));

                QStringList parts;

                QString AggFunction = enuConditionalAggregation::toStr(*_col.ConditionalAggregation);
                AggFunction.chop(2);
                AggFunction += "(";
                if (_col.TrueValue.isValid())
                    AggFunction += "IF(";
                if (SQLPrettyLen && _col.Condition.hasMany())
                    AggFunction += "\n" + QString(SQLPrettyLen, ' ') + " ";
                parts.append(AggFunction);

                parts.append(_col.Condition.buildConditionString(MainTableNameOrAlias, this->Table.FilterableColsMap, false/*, _prettifierJustifyLen*/));

                if (_col.TrueValue.isValid())
                {
                    if (SQLPrettyLen && _col.Condition.hasMany())
                        parts.append("\n" + QString(SQLPrettyLen, ' ') + " ");
                    parts.append(",");
                    parts.append(makeValueAsSQL(_col.TrueValue));
                    parts.append(",");
                    parts.append(makeValueAsSQL(_col.FalseValue));
                }

                parts.append(")");
                if (_col.TrueValue.isValid() != false)
                    parts.append(")");

                parts.append(" AS ");
                parts.append(_col.RenameAs);

                this->SelectQueryPreparedItems.Cols.append(parts.join(""));
            }
            else {
                if (_col.Name.isEmpty())
                    throw exQueryBuilder("Column name is not provided");

                QString AggFunction;

                if (NULLABLE_IS_NULL(_col.SimpleAggregation)) {
                   ///TODO: why using ANY_VALUE?
//                   if (this->GroupByCols.size())
//                       AggFunction = "ANY_VALUE(";
                }
                else if (*_col.SimpleAggregation == enuAggregation::DISTINCT_COUNT) {
                    AggFunction = "COUNT(DISTINCT ";
                }
                else {
                    AggFunction = enuAggregation::toStr(*_col.SimpleAggregation);
                    AggFunction += "(";
                }

                const stuRelatedORMField& relatedORMField = this->Table.SelectableColsMap[_col.Name];
                if (relatedORMField.Col.name().isNull())
                    throw exQueryBuilder("Invalid column for filtering: " + _col.Name);
//                    return false;

                QString ColFinalName = makeColName(
                                           MainTableNameOrAlias,
                                           relatedORMField.Col,
                                           false /*true*/,
                                           relatedORMField.Relation == InvalidRelation
                                                ? _relation
                                                : relatedORMField.Relation
                                        );

                if (AggFunction.size()) {
                   this->SelectQueryPreparedItems.Cols.append(AggFunction
                                                + ColFinalName.split(' ').first()
                                                + ") AS "
                                                + (_col.RenameAs.size()
                                                   ? _col.RenameAs
                                                   : AggFunction.replace('(', "") + '_' + ColFinalName.split(' ').last()
                                                  )
                                                );
                }
                else
                   this->SelectQueryPreparedItems.Cols.append(updateRename(ColFinalName));
            }

            return true;
        }; //addCol

        if (this->RequiredCols.isEmpty())
            foreach(auto Col, this->Table.BaseCols)
                this->SelectQueryPreparedItems.Cols.append(makeColName(MainTableNameOrAlias, Col, true));
        else {
//            qDebug() << MainTableNameOrAlias << "has RequiredCols";
//            int i = 0;
            foreach(stuColSpecs Col, this->RequiredCols) {
//                qDebug() << i++;
                addCol(Col);
            }
        }

//        QSet<stuRelation> UsedJoins;
//        foreach (stuRelation Relation, this->Table.Relations) {
//            clsTable* ForeignTable = clsTable::Registry[Relation.ReferenceTable];
//            if (ForeignTable == nullptr)
//                throw exHTTPInternalServerError("Reference table has not been registered: " + Relation.ReferenceTable);

//            bool Joined = false;
//            if (this->RequiredCols.isEmpty())
//                foreach(auto Col, ForeignTable->BaseCols)
//                    this->SelectQueryPreparedItems.Cols.append(makeColName(MainTableName, Col, true, Relation));
//            else
//                foreach(auto RequiredCol, this->RequiredCols)
//                    if (addCol(RequiredCol, Relation))
//                        Joined = true;

//            if (Joined)
//                UsedJoins.insert(Relation);
//        }

        if (this->SelectQueryPreparedItems.Cols.isEmpty())
            throw exHTTPBadRequest("No columns found to be reported");

//        if (RequiredCols.size() && RequiredCols.size() > this->SelectQueryPreparedItems.Cols.size())
//            throw exHTTPBadRequest("Seems that some columns could not be resolved: Active Cols are: [" + this->SelectQueryPreparedItems.Cols.join(", ") + "]");

        /****************************************************************************/
/*
        quint8 OpenParenthesis = 0;
        bool StatusColHasCriteria = false;
        bool CanStartWithLogical = false;
        QString LastLogical = "";
        _filters = _filters.replace("\\ ", "$SPACE$");
        foreach(auto Filter, _filters.split(" ", QString::SkipEmptyParts)){
            QString Rule;
            Filter = Filter.trimmed ();
            if(Filter == ")"){
                if(OpenParenthesis <= 0) throw exHTTPBadRequest("Invalid close parenthesis without any open");
                Rule = " )";
                OpenParenthesis--;
                CanStartWithLogical = true;
            }else if(Filter == "("){
                Rule = LastLogical + "(";
                CanStartWithLogical = false;
                LastLogical.clear();
                OpenParenthesis++;
            }else if(Filter == '+' || Filter == '|' || Filter == '*'){
                if(CanStartWithLogical == false) throw exHTTPBadRequest("Invalid logical expression prior to any rule");
                if(Filter == '+') LastLogical = "AND ";
                else if(Filter == '|') LastLogical = "OR ";
                else if(Filter == '*') LastLogical = "XOR ";

                CanStartWithLogical = false;
                continue;
            }else{
                static QRegularExpression rxFilterPattern("([a-zA-Z0-9\\_]+)([<>!=~]=?)(.+)");
                Filter = Filter.replace("$SPACE$", " ");
                QRegularExpressionMatch PatternMatches = rxFilterPattern.match(Filter);
                if(PatternMatches.lastCapturedIndex() != 3)
                    throw exHTTPBadRequest("Invalid filter set: " + Filter);

                Rule = LastLogical;

                stuRelatedORMField relatedORMField = this->FilterableColsMap.value(PatternMatches.captured(1).trimmed());
                if(relatedORMField.isValid())
                    Rule+=makeColName(this->Name, relatedORMField.Col, false, relatedORMField.Relation);
                else
                    throw exHTTPBadRequest("Invalid column for filtering: " + PatternMatches.captured(1));

                if(relatedORMField.Col.updatableBy() == enuUpdatableBy::__STATUS__)
                    StatusColHasCriteria = true;

                if(relatedORMField.Relation.Column.size() && UsedJoins.contains(relatedORMField.Relation) == false)
                    UsedJoins.insert(relatedORMField.Relation);

                if(PatternMatches.captured(3) == "NULL"){
                    if(PatternMatches.captured(2) == "=")
                        Rule += " IS NULL";
                    else if(PatternMatches.captured(2) == "!=")
                        Rule += " IS NOT NULL";
                    else
                        throw exHTTPBadRequest("Invalid filter with NULL expression: " + Filter);

                    SelectItems.Where.append(Rule);
                    CanStartWithLogical = true;
                    LastLogical.clear();
                    continue;
                }

                if(PatternMatches.captured(2) == "<") Rule += " < ";
                else if(PatternMatches.captured(2) == "<=") Rule += " <= ";
                else if(PatternMatches.captured(2) == ">") Rule += " > ";
                else if(PatternMatches.captured(2) == ">=") Rule += " >= ";
                else if(PatternMatches.captured(2) == "!=") Rule += " != ";
                else if(PatternMatches.captured(2) == "~=") Rule += " LIKE ";
                else if(PatternMatches.captured(2) == "=") Rule += " = ";
                else throw exHTTPBadRequest("Invalid filter criteria: " + Filter);

                Rule += relatedORMField.Col.argSpecs().isPrimitiveType() ? "" : "'";
                QString Value = PatternMatches.captured(3);
                if(Value == "NOW()"
                   || Value.startsWith("DATE_ADD(")
                   || Value.startsWith("DATE_SUB(")
                   )
                    Rule += Value.replace("$SPACE$", " ");
                else{
                    relatedORMField.Col.argSpecs().validate(Value, PatternMatches.captured(1).trimmed().toLatin1());
                    Rule += relatedORMField.Col.toDB(Value).toString();
                }
                Rule += relatedORMField.Col.argSpecs().isPrimitiveType() ? "" : "'";

                CanStartWithLogical = true;
                LastLogical.clear();
            }
            SelectItems.Where.append(Rule);
        }

        if(OpenParenthesis != 0)
            throw exHTTPBadRequest("count of open and closed parenthesises does not match");

        if(SelectItems.Where.isEmpty())
            SelectItems.Where.append("TRUE");

        if(StatusColHasCriteria == false)
            foreach(auto FCol, this->FilterableColsMap)
                if(FCol.Col.updatableBy() == enuUpdatableBy::__STATUS__){
                    if(FCol.Relation.LeftJoin)
                        SelectItems.Where.append(QString("AND (ISNULL(%1) OR %1!='R')").arg(makeColName(this->Name, FCol.Col, false, FCol.Relation)));
                    else
                        SelectItems.Where.append(QString("AND %1!='R'").arg(makeColName(this->Name, FCol.Col, false, FCol.Relation)));
                    if(FCol.Relation.Column.size())
                        UsedJoins.insert(FCol.Relation);
                }
*/

        /****************************************************************************/
        if (this->OrderByCols.length()) {
            foreach (auto orderBy, this->OrderByCols) {
                if (orderBy.Dir == enuOrderDir::Ascending)
                    this->SelectQueryPreparedItems.OrderBy.append(orderBy.Col);
                else
                    this->SelectQueryPreparedItems.OrderBy.append(QString("%1 DESC").arg(orderBy.Col));
            }
        }

        /****************************************************************************/
        if (this->UnionParts.length()) {
            foreach (auto unionPart, this->UnionParts) {
                QString cmd;
                switch (unionPart.UnionType) {
                    case enuUnionType::Normal:
                        cmd = "UNION";
                        break;
                    case enuUnionType::All:
                        cmd = "UNION ALL";
                        break;
                    case enuUnionType::Distinct:
                        cmd = "UNION DISTINCT";
                        break;
                }

                if (SQLPrettyLen)
                    this->SelectQueryPreparedItems.Unions.append(cmd.rightJustified(SQLPrettyLen));
                else
                    this->SelectQueryPreparedItems.Unions.append(cmd);

                this->SelectQueryPreparedItems.Unions.append(unionPart.Query.buildQueryString({}, false, false/*, _prettifierJustifyLen*/));
            }
        }

        /****************************************************************************/
    }

//    clsORMField colByName(const QString& _col) {
//        return this->Table.SelectableColsMap[_col];
//    }

public:
    QList<stuColSpecs>   RequiredCols;
    QList<stuOrderBy>    OrderByCols;
    QList<stuUnion>      UnionParts;

    quint64              Offset = 0;
    quint16              Limit = 0;
    quint16              CahceTime = 0;

    stuSelectQueryPreparedItems SelectQueryPreparedItems;
};

/***************************************************************************************/
/* SelectQuery *************************************************************************/
/***************************************************************************************/
SelectQuery::SelectQuery() :
    tmplBaseQuery<SelectQuery, clsSelectQueryData>(),
    tmplQueryJoinTrait<SelectQuery>(this),
    tmplQueryWhereTrait<SelectQuery>(this),
    tmplQueryGroupAndHavingTrait<SelectQuery>(this)

{
    tmplQueryJoinTrait<SelectQuery>::JoinTraitData->Owner = this;
    tmplQueryWhereTrait<SelectQuery>::WhereTraitData->Owner = this;
    tmplQueryGroupAndHavingTrait<SelectQuery>::GroupAndHavingTraitData->Owner = this;
}
SelectQuery::SelectQuery(const SelectQuery& _other) :
    tmplBaseQuery<SelectQuery, clsSelectQueryData>(_other),
    tmplQueryJoinTrait<SelectQuery>(_other),
    tmplQueryWhereTrait<SelectQuery>(_other),
    tmplQueryGroupAndHavingTrait<SelectQuery>(_other)
{
    tmplQueryJoinTrait<SelectQuery>::JoinTraitData->Owner = this;
    tmplQueryWhereTrait<SelectQuery>::WhereTraitData->Owner = this;
    tmplQueryGroupAndHavingTrait<SelectQuery>::GroupAndHavingTraitData->Owner = this;
}
SelectQuery::SelectQuery(clsTable& _table, const QString& _alias) :
    tmplBaseQuery<SelectQuery, clsSelectQueryData>(_table, _alias),
    tmplQueryJoinTrait<SelectQuery>(this),
    tmplQueryWhereTrait<SelectQuery>(this),
    tmplQueryGroupAndHavingTrait<SelectQuery>(this)
{}
SelectQuery::~SelectQuery() {}

/***********************\
|* Columns             *|
\***********************/
//used by APPLY_GET_METHOD_CALL_ARGS_APICALL_TO_QUERY
SelectQuery& SelectQuery::addCols(const TAPI::Cols_t& _commaSeperatedCols, const QString& _seperator)
{
    QString sCols = _commaSeperatedCols.trimmed();

    if (sCols.isEmpty())
        return *this;

    QStringList cols = sCols.split(_seperator);

    if (cols.length() == 0)
        return *this;

    return this->addCols(cols);
}

SelectQuery& SelectQuery::addCols(const QStringList& _cols)
{
    if (_cols.length()) {
        foreach(QString col, _cols) {
            if (col.isEmpty() == false)
                this->addCol(col);
        }
    }

    return *this;
}

SelectQuery& SelectQuery::addCol(const QString& _col, const QString& _renameAs)
{
//    Q_ASSERT(_col.isEmpty() == false);

    if (_col.isEmpty() == false)
        this->Data->RequiredCols.append(stuColSpecs(_col, _renameAs));

    return *this;
}

SelectQuery& SelectQuery::addCol(const DBExpression& _expr, const QString& _renameAs)
{
    this->Data->RequiredCols.append(stuColSpecs(_expr, _renameAs));

    return *this;
}

SelectQuery& SelectQuery::addCol(enuAggregation::Type _aggFunc, const QString& _col, const QString& _renameAs)
{
    Q_ASSERT(_col.isEmpty() == false);

    this->Data->RequiredCols.append({ _col, _renameAs, _aggFunc });

    return *this;
}

SelectQuery& SelectQuery::addCol(enuConditionalAggregation::Type _aggFunc, const clsCondition& _condition, const QString& _renameAs)
{
    if (_aggFunc == enuConditionalAggregation::COUNTIF)
        this->Data->RequiredCols.append({ {}, _renameAs, NULLABLE_NULL_VALUE, _aggFunc, _condition, 1, DBExpression::NIL() });
    else
        this->Data->RequiredCols.append({ {}, _renameAs, NULLABLE_NULL_VALUE, _aggFunc, _condition });

    return *this;
}
SelectQuery& SelectQuery::addCol(enuConditionalAggregation::Type _aggFunc, const clsCondition& _condition, QVariant _trueValue, QVariant _falseValue, const QString& _renameAs)
{
    this->Data->RequiredCols.append({ {}, _renameAs, NULLABLE_NULL_VALUE, _aggFunc, _condition, _trueValue, _falseValue });

    return *this;
}

/***********************\
|* From                *|
\***********************/
//SelectQuery& from(const QString _table, const QString& _renameAs = {});
//SelectQuery& from(const SelectQuery& _nestedQuery);

/***********************\
|* Order               *|
\***********************/
SelectQuery& SelectQuery::orderBy(const QString& _col, enuOrderDir::Type _dir)
{
    if (_col.isEmpty() == false)
        this->Data->OrderByCols.append({ _col, _dir });
    return *this;
}

/***********************\
|* Union               *|
\***********************/
SelectQuery& SelectQuery::addUnion(SelectQuery& _query)
{
    this->Data->UnionParts.append({ _query, enuUnionType::Normal });
    return *this;
}
SelectQuery& SelectQuery::addUnionAll(SelectQuery& _query)
{
    this->Data->UnionParts.append({ _query, enuUnionType::All });
    return *this;
}
SelectQuery& SelectQuery::addUnionDistinct(SelectQuery& _query)
{
    this->Data->UnionParts.append({ _query, enuUnionType::Distinct });
    return *this;
}

/***********************\
|* Other               *|
\***********************/
//used by APPLY_GET_METHOD_CALL_ARGS_APICALL_TO_QUERY
SelectQuery& SelectQuery::offset(quint64 _offset)
{
    this->Data->Offset = _offset;
    return *this;
}

//used by APPLY_GET_METHOD_CALL_ARGS_APICALL_TO_QUERY
SelectQuery& SelectQuery::limit(quint16 _limit)
{
    this->Data->Limit = _limit;
    return *this;
}

SelectQuery& SelectQuery::setCacheTime(quint16 _cacheTime)
{
    ///BUG: enabling cache affectes on WasSP and rows count (damages dac result data)
//    this->Data->CahceTime = _cacheTime;

    return *this;
}

/***********************\
|* Execute             *|
\***********************/
QString SelectQuery::buildQueryString(QVariantMap _args, bool _selectOne, bool _reportCount/*, quint8 _prettifierJustifyLen*/)
{
    //this->Data->Table.prepareFiltersList();
    this->Data->prepare(false);
    this->WhereTraitData->prepare();
    this->GroupAndHavingTraitData->prepare();
    //it should be the last preparation call, as the previous preparation may cause an automatic join
    this->JoinTraitData->prepare();

    //push
    quint64 offset = this->Data->Offset;
    quint16 limit = this->Data->Limit;

    if (_reportCount)
    {
        this->Data->Offset = 0;
        this->Data->Limit = 0;
    }
    else if (_selectOne)
        this->Data->Limit = 1;

    QStringList QueryParts;

    //-----------
    if (SQLPrettyLen) {
        QueryParts.append(QString("SELECT").rightJustified(SQLPrettyLen)
                          + " "
                          + this->Data->SelectQueryPreparedItems.Cols.join("\n" + QString(SQLPrettyLen - 1, ' ') + "," + " "));
    }
    else {
        QueryParts.append("SELECT");
        QueryParts.append(this->Data->SelectQueryPreparedItems.Cols.join(","));
    }

    //-----------
    if (SQLPrettyLen) {
        QueryParts.append(QString("FROM").rightJustified(SQLPrettyLen)
                          + " "
                          + this->Data->BaseQueryPreparedItems.From);
    }
    else {
        QueryParts.append("FROM");
        QueryParts.append(this->Data->BaseQueryPreparedItems.From);
    }

    if (this->JoinTraitData->PreparedItems.Joins.length()) {
        if (SQLPrettyLen)
            QueryParts.append(this->JoinTraitData->PreparedItems.Joins.join("\n"));
        else
            QueryParts.append(this->JoinTraitData->PreparedItems.Joins.join(" "));
    }

    //-----------
    QStringList WhereParts;

    if (this->WhereTraitData->PreparedItems.Where.size())
        WhereParts.append(this->WhereTraitData->PreparedItems.Where);

    if (WhereParts.size())
    {
        if (SQLPrettyLen) {
            QueryParts.append(QString("WHERE").rightJustified(SQLPrettyLen)
                              + " "
                              + WhereParts.join("\n" + QString(SQLPrettyLen, ' ') + " "));
        }
        else {
            QueryParts.append("WHERE");
            QueryParts.append(WhereParts.join(" "));
        }
    }

    //-----------
    if (this->Data->SelectQueryPreparedItems.OrderBy.size())
    {
        if (SQLPrettyLen) {
            QueryParts.append(QString("ORDER BY").rightJustified(SQLPrettyLen)
                              + " "
                              + this->Data->SelectQueryPreparedItems.OrderBy.join("\n" + QString(SQLPrettyLen - 1, ' ') + "," + " "));
        }
        else {
            QueryParts.append("ORDER BY");
            QueryParts.append(this->Data->SelectQueryPreparedItems.OrderBy.join(","));
        }
    }

    //-----------
    if (this->GroupAndHavingTraitData->PreparedItems.GroupBy.size())
    {
        if (SQLPrettyLen) {
            QueryParts.append(QString("GROUP BY").rightJustified(SQLPrettyLen)
                              + " "
                              + this->GroupAndHavingTraitData->PreparedItems.GroupBy.join("\n" + QString(SQLPrettyLen - 1, ' ') + "," + " "));
        }
        else {
            QueryParts.append("GROUP BY");
            QueryParts.append(this->GroupAndHavingTraitData->PreparedItems.GroupBy.join(","));
        }
    }

    //-----------
    if (this->GroupAndHavingTraitData->PreparedItems.Having.size())
    {
        if (SQLPrettyLen) {
            QueryParts.append(QString("HAVING").rightJustified(SQLPrettyLen)
                              + " "
                              + this->GroupAndHavingTraitData->PreparedItems.Having);
        }
        else {
            QueryParts.append("HAVING");
            QueryParts.append(this->GroupAndHavingTraitData->PreparedItems.Having);
        }
    }

    //-----------
    if (_reportCount == false)
    {
        if (this->WhereTraitData->PksByPath.isEmpty())
        {
            if ((this->Data->Offset > 0) || (this->Data->Limit > 0))
            {
                QString sLimit;
                if (this->Data->Offset > 0)
                {
                    if (this->Data->Limit > 0)
                        sLimit = QString("%1,%2").arg(this->Data->Offset).arg(this->Data->Limit);
                    else
                        sLimit = QString("%1").arg(this->Data->Offset);
                }
                else //limit > 0
                    sLimit = QString("0,%1").arg(this->Data->Limit);

                if (SQLPrettyLen) {
                    QueryParts.append(QString("LIMIT").rightJustified(SQLPrettyLen)
                                      + " "
                                      + sLimit);
                }
                else {
                    QueryParts.append(QString("LIMIT")
                                      + " "
                                      + sLimit);
                }
            }
        }
        else {
            if (SQLPrettyLen)
                QueryParts.append(QString("LIMIT").rightJustified(SQLPrettyLen)
                                  + " "
                                  + "2");
            else
                QueryParts.append("LIMIT 2"); //Limit is set to 2 in order to produce error if multi values are selected instead of one
        }
    }

    //-----------
    if (this->Data->UnionParts.length()) {
        if (SQLPrettyLen)
            QueryParts.append(this->Data->SelectQueryPreparedItems.Unions.join("\n"));
        else
            QueryParts.append(this->Data->SelectQueryPreparedItems.Unions.join(" "));
    }

    //-----------
    QString QueryString = QueryParts.join(SQLPrettyLen ? "\n" : " ");

    if (_args.size())
    {
        for (QVariantMap::const_iterator arg = _args.begin(); arg != _args.end(); ++arg)
        {
            QueryString.replace(arg.key(), arg.value().toString());
        }
    }

    //-----------
    if (_reportCount) {
        if (SQLPrettyLen) {
            QueryParts.clear();
            QueryParts.append(QString("SELECT").rightJustified(SQLPrettyLen)
                              + " "
                              + "COUNT(*) AS cnt");
            QueryParts.append(QString("FROM").rightJustified(SQLPrettyLen)
                              + " "
                              + "(");
            QueryParts.append(QueryString);
            QueryParts.append(QString(")").rightJustified(SQLPrettyLen + 2)
                              + " qry");
            QueryString = QueryParts.join(SQLPrettyLen ? "\n" : " ");
        }
        else {
            QueryString = "SELECT COUNT(*) AS cnt FROM (" + QueryString + ") qry";
        }
    }

    //-----------
    //pull
    this->Data->Offset = offset;
    this->Data->Limit = limit;

    return QueryString;
}

/*
quint64 SelectQuery::count(QVariantMap _args)
{
    QString QueryString = this->buildQueryString(_currentUserID, _args, false, true);

    //execute
    quint64 count = 0;

    ///TODO: run query

    return count;
}
*/

QVariantMap SelectQuery::one(QVariantMap _args)
{
    QString QueryString = this->buildQueryString(_args, true, false);

#ifdef QT_DEBUG
    qDebug().nospace().noquote() << endl
                                 << endl << "-- Query:" << endl << QueryString << endl;
#endif

    QJsonDocument Result;

    clsDAC DAC(this->Data->Table.domain(), this->Data->Table.Schema);

    if (this->Data->CahceTime > 0)
        Result = DAC.execQueryCacheable(this->Data->CahceTime, "", QueryString)
                            .toJson(true, this->Data->Table.Converters);
    else
        Result = DAC.execQuery("", QueryString)
                            .toJson(true, this->Data->Table.Converters);

    if (Result.object().isEmpty())
        throw exHTTPNotFound("No item could be found");

//    qDebug() << "--- SelectQuery::one()" << __FILE__ << __LINE__ << Result;
//    qDebug() << "--- SelectQuery::one(){tovariant}" << __FILE__ << __LINE__ << Result.toVariant();
//    qDebug() << "--- SelectQuery::one(){tovariant.tomap}" << __FILE__ << __LINE__ << Result.toVariant().toMap();
//    QVariant cc = correctExecQueryResult(Result);


//    qDebug() << "--- SelectQuery::one(){CORRECT}" << __FILE__ << __LINE__ << cc;

    return Result.toVariant().toMap();

//    return correctExecQueryResult(Result).toMap();
}

QVariantList SelectQuery::all(QVariantMap _args, quint16 _maxCount, quint64 _from)
{
    this->Data->Offset = _from;
    this->Data->Limit = _maxCount;

    QString QueryString = this->buildQueryString(_args, false, false);

#ifdef QT_DEBUG
    qDebug().nospace().noquote() << endl
                                 << endl << "-- Query:" << endl << QueryString << endl;
#endif

    clsDAC DAC(this->Data->Table.domain(), this->Data->Table.Schema);

    QJsonDocument Result;

    if (this->Data->CahceTime > 0)
        Result = DAC.execQueryCacheable(this->Data->CahceTime, "", QueryString)
                            .toJson(false, this->Data->Table.Converters);
    else
        Result = DAC.execQuery("", QueryString)
                            .toJson(false, this->Data->Table.Converters);

//    TargomanDebug(5, "--- SelectQuery::all()" << __FILE__ << __LINE__ << Result.toJson());

    qDebug() << "--- SelectQuery::all()" << __FILE__ << __LINE__ << Result;

    return Result.toVariant().toList();
}

TAPI::stuTable SelectQuery::allWithCount(QVariantMap _args, quint16 _maxCount, quint64 _from)
{
    this->Data->Offset = _from;
    this->Data->Limit = _maxCount;

    QString QueryString = this->buildQueryString(_args, false, false);
    QString CountingQueryString = this->buildQueryString(_args, false, true);

#ifdef QT_DEBUG
    qDebug().nospace().noquote() << endl
                                 << endl << "-- Query:" << endl << QueryString << endl
                                 << endl << "-- Counting Query:" << endl << CountingQueryString << endl;
#endif

    QJsonDocument ResultRows;
    QJsonDocument ResultTotalRows;

    clsDAC DAC(this->Data->Table.domain(), this->Data->Table.Schema);

    if (this->Data->CahceTime > 0) {
        ResultTotalRows = DAC.execQueryCacheable(this->Data->CahceTime, "", CountingQueryString)
                                     .toJson(true);

        ResultRows = DAC.execQueryCacheable(this->Data->CahceTime, "", QueryString)
                                .toJson(false, this->Data->Table.Converters);
    }
    else {
        ResultTotalRows = DAC.execQuery("", CountingQueryString)
                                     .toJson(true);

        ResultRows = DAC.execQuery("", QueryString)
                                .toJson(false, this->Data->Table.Converters);
    }

    TAPI::stuTable Result;
    Result.TotalRows = ResultTotalRows
                       .toVariant()
                       .toMap()["cnt"]
                       .toULongLong();
    Result.Rows = ResultRows
                  .toVariant()
                  .toList();

    qDebug() << "--- SelectQuery::allWithCount()" << __FILE__ << __LINE__ << "Rows: " << Result.Rows << "Rows Count: " << Result.TotalRows;

    return Result;
}

/***************************************************************************************/
/* clsCreateQueryData ******************************************************************/
/***************************************************************************************/
struct stuCreateQueryPreparedItems {
    QStringList         Cols;
    QList<QStringList>  Values;
    QString             Select;

    QVariantList        BindingValues;
    bool                IsPrepared = false;
};

class clsCreateQueryData : public clsBaseQueryData<CreateQuery>
{
public:
    clsCreateQueryData(clsTable& _table, const QString& _alias = {}) : clsBaseQueryData<CreateQuery>(_table, _alias) {}
//    clsCreateQueryData(const clsCreateQueryData& _other) : Table(_other.Table), Alias(_other.Alias) {}
//    ~clsCreateQueryData() {}

    virtual void prepare(quint64 _currentUserID, bool _useBinding/*, quint8 _prettifierJustifyLen*/) {
        if (this->CreateQueryPreparedItems.IsPrepared)
            return;
        this->CreateQueryPreparedItems.IsPrepared = true;

        clsBaseQueryData<CreateQuery>::prepare(_useBinding/*, _prettifierJustifyLen*/);

        /****************************************************************************/
        QString MainTableNameOrAlias = this->Alias.length() ? this->Alias : this->Table.Name;

        /****************************************************************************/
        if (this->Cols.isEmpty())
            throw exHTTPBadRequest("No columns provided for creating");

        QList<clsORMField> providedBaseCols;
//        QList<clsORMField> extraBaseCols;
        QList<QVariant> extraBaseColsValues;

        //1- check Cols by BaseCols
        foreach (auto col, this->Cols) {
            bool found = false;
            foreach (clsORMField baseCol, this->Table.BaseCols) {
//                qDebug() << "compare" << col << baseCol.name();
                if (col == baseCol.name()) {
                    if (baseCol.defaultValue() == QInvalid || baseCol.defaultValue() == QAuto)
                        throw exHTTPInternalServerError("Invalid change to read-only column <" + col + ">");

                    providedBaseCols.append(baseCol);

                    this->CreateQueryPreparedItems.Cols.append(makeColName(MainTableNameOrAlias, baseCol));

                    found = true;

                    break;
                }
            }
            if (found == false)
                throw exQueryBuilderColumnNotFound("Column <" + col + "> not found in table <" + this->Table.Name + "> base columns");
        }

        //2- check BaseCols (required, ...)
        foreach (clsORMField baseCol, this->Table.BaseCols) {
            if (this->Cols.contains(baseCol.name()) == false) {
                if (baseCol.defaultValue() == QRequired)
                    throw exQueryBuilderColumnNotProvided("Required field <" + baseCol.name() + "> not provided");

                if (baseCol.updatableBy() == enuUpdatableBy::__CREATOR__) {
//                    extraBaseCols.append(baseCol);
                    this->CreateQueryPreparedItems.Cols.append(makeColName(MainTableNameOrAlias, baseCol));
                    extraBaseColsValues.append(baseCol.toDB(_currentUserID));
                }
            }
        }

        if (this->CreateQueryPreparedItems.Cols.isEmpty())
            throw exQueryBuilder("Nothing to do");

        if (this->Values.length()) {
            foreach (QVariantMap oneRecord, this->Values) {
                QStringList oneRecordToString;
                foreach (clsORMField baseCol, providedBaseCols) {
                    bool dataProvided = false;

                    for (QVariantMap::const_iterator itr = oneRecord.constBegin(); itr != oneRecord.constEnd(); itr++) {
                        if (itr.key() == baseCol.name()) {
                            dataProvided = true;

                            QVariant val = itr.value();

                            if (val.userType() != QMetaTypeId<DBExpression>::qt_metatype_id())
                                baseCol.validate(val);

//                            qDebug() << itr.key() << ": val(" << val << ") makeAsSQL(" << v << ") toDB(" << baseCol.toDB(val) << ")";

                            if (val.userType() == QMetaTypeId<DBExpression>::qt_metatype_id() || _useBinding == false) {
                                QString v = makeValueAsSQL(val, _useBinding == false, &baseCol);
                                oneRecordToString.append(v);
                            }
                            else {
                                oneRecordToString.append("?");
//                                QVariant v = makeValueAsVariant(val);
//                                QString l = QString::fromUtf8(v.value<QJsonDocument>().toJson(QJsonDocument::Compact));
qDebug() << itr.key() << ": val(" << val << ") toDB(" << baseCol.toDB(val) << ")"; // << " lambda(" << l << ")";
                                this->CreateQueryPreparedItems.BindingValues.append(baseCol.toDB(val));
                            }

                            break;
                        }
                    }

                    if (!dataProvided) {
                        throw exQueryBuilderValueNotProvided("Value for declared field <" + baseCol.name() + "> not provided");

//                        if (baseCol.defaultValue() == QRequired)
//                            throw exQueryBuilderValueNotProvided("Value for required field <" + baseCol.name() + "> not provided");

//                        if (_useBinding) {
//                            oneRecordToString.append("?");
//                            this->CreateQueryPreparedItems.BindingValues.append(makeValueAsSQL(DBExpression::NIL()));
//                        }
//                        else
//                            oneRecordToString.append(makeValueAsSQL(DBExpression::NIL()));
                    }
                }

                if (extraBaseColsValues.length()) {
                    foreach (auto val, extraBaseColsValues) {
                        ///TODO: baseCol.toDB()

                        if (_useBinding) {
                            oneRecordToString.append("?");
                            QVariant v = makeValueAsVariant(val);
                            this->CreateQueryPreparedItems.BindingValues.append(v);
                        }
                        else {
                            QString v = makeValueAsSQL(val, _useBinding == false);
                            oneRecordToString.append(v);
                        }
                    }
                }

                this->CreateQueryPreparedItems.Values.append(oneRecordToString);
            }
            if (this->CreateQueryPreparedItems.Values.isEmpty())
                throw exQueryBuilder("Values is empty");
        }
        else if (this->Select.isValid()) {
            if (extraBaseColsValues.length()) {
                foreach (auto val, extraBaseColsValues) {
                    this->Select.addCol(DBExpression(val.toString(), enuDBExpressionType::Value));
                }
            }

            this->CreateQueryPreparedItems.Select = this->Select.buildQueryString({}, false, false/*, _prettifierJustifyLen*/);
        }
        else
            throw exQueryBuilder("Values or Select not provided");
    }

public:
    bool Options_Ignore = false;
    QStringList Cols;
    QList<QVariantMap> Values;
    SelectQuery Select;

    stuCreateQueryPreparedItems CreateQueryPreparedItems;
};

/***************************************************************************************/
/* CreateQuery *************************************************************************/
/***************************************************************************************/
CreateQuery::CreateQuery(const CreateQuery& _other) :
    tmplBaseQuery<CreateQuery, clsCreateQueryData>(_other)
{}

CreateQuery::CreateQuery(clsTable& _table, const QString& _alias) :
    tmplBaseQuery<CreateQuery, clsCreateQueryData>(_table, _alias)
{}

CreateQuery::~CreateQuery() {}

/***********************\
|* Options             *|
\***********************/
CreateQuery& CreateQuery::options_ignore()
{
    this->Data->Options_Ignore = true;
    return *this;
}

/***********************\
|* Columns             *|
\***********************/
CreateQuery& CreateQuery::addCols(const QStringList& _cols)
{
//    if (this->Data->Values.length())
//        throw new exQueryBuilder("Columns must be defined before values");

//    if (this->Data->Select != nullptr)
//        throw new exQueryBuilder("Columns must be defined before select query");

    foreach(auto Col, _cols)
        this->addCol(Col);
    return *this;
}

CreateQuery& CreateQuery::addCol(const QString& _col)
{
//    if (this->Data->Values.length())
//        throw new exQueryBuilder("Columns must be defined before values");

//    if (this->Data->Select != nullptr)
//        throw new exQueryBuilder("Columns must be defined before select query");

    this->Data->Cols.append(_col);
    return *this;
}
/***********************\
|* Values              *|
\***********************/
CreateQuery& CreateQuery::values(const QVariantMap& _oneRecordValues)
{

    if (this->Data->Cols.isEmpty())
        throw new exQueryBuilder("Columns must be defined before values");

    if (this->Data->Select.isValid())
        throw new exQueryBuilder("Select query is not empty");

    this->Data->Values.append(_oneRecordValues);
//    qDebug() << "----------------1:" << _oneRecordValues;
//    qDebug() << "----------------2:" << this->Data->Values;

    return *this;
}
CreateQuery& CreateQuery::values(const QList<QVariantMap>& _multipleRecordValues)
{
    if (this->Data->Cols.isEmpty())
        throw new exQueryBuilder("Columns must be defined before values");

    if (this->Data->Select.isValid())
        throw new exQueryBuilder("Select query is not empty");

    this->Data->Values.append(_multipleRecordValues);

    return *this;
}
/***********************\
|* Select              *|
\***********************/
CreateQuery& CreateQuery::select(const SelectQuery& _selectQuery)
{
    if (this->Data->Cols.isEmpty())
        throw new exQueryBuilder("Columns must be defined before select query");

    if (this->Data->Values.length())
        throw new exQueryBuilder("Values is not empty");

    this->Data->Select = _selectQuery;

    return *this;
}

/***********************\
|* Execute             *|
\***********************/
stuBoundQueryString CreateQuery::buildQueryString(quint64 _currentUserID, QVariantMap _args, bool _useBinding/*, quint8 _prettifierJustifyLen*/)
{
    stuBoundQueryString BoundQueryString;

    this->Data->prepare(_currentUserID, _useBinding/*, _prettifierJustifyLen*/);

    if (_useBinding)
        BoundQueryString.BindingValues = this->Data->CreateQueryPreparedItems.BindingValues;

    QStringList QueryParts;

    //-----------
    QStringList options;
    if (this->Data->Options_Ignore)
        options.append("IGNORE");

    if (SQLPrettyLen) {
        if (options.isEmpty())
            QueryParts.append(QString("INSERT").rightJustified(SQLPrettyLen));
        else
            QueryParts.append(QString("INSERT").rightJustified(SQLPrettyLen)
                              + " "
                              + options.join(" "));

        QueryParts.append(QString("INTO").rightJustified(SQLPrettyLen)
                          + " "
                          + this->Data->BaseQueryPreparedItems.From);
        QueryParts.append(QString(SQLPrettyLen, ' ')
                          + " "
                          + "(");
        QueryParts.append(QString(SQLPrettyLen, ' ')
                          + " "
                          + this->Data->CreateQueryPreparedItems.Cols.join("\n" + QString(SQLPrettyLen - 1, ' ') + "," + " "));
        QueryParts.append(QString(SQLPrettyLen, ' ')
                          + " "
                          + ")");
    }
    else {
        QueryParts.append("INSERT");
        if (options.length())
            QueryParts.append(options.join(" "));
        QueryParts.append("INTO");
        QueryParts.append(this->Data->BaseQueryPreparedItems.From);
        QueryParts.append("(" + this->Data->CreateQueryPreparedItems.Cols.join(",") + ")");
    }

    //-----------
    if (this->Data->CreateQueryPreparedItems.Values.length()) {
        QStringList rows;
        foreach (QStringList row, this->Data->CreateQueryPreparedItems.Values) {
            if (SQLPrettyLen) {
                if (_useBinding)
                    rows.append(row.join(", "));
                else
                    rows.append(QString(SQLPrettyLen, ' ')
                                + " "
                                + row.join("\n" + QString(SQLPrettyLen - 1, ' ') + "," + " "));
            }
            else {
                rows.append(row.join(","));
            }
        }

        if (SQLPrettyLen) {
            QueryParts.append(QString("VALUES").rightJustified(SQLPrettyLen)
                + " "
                + "("
                + (_useBinding ? "" : "\n")
                + rows.join((_useBinding ? "" : "\n")
                    + (_useBinding ? "" : QString(SQLPrettyLen, ' '))
                    + (_useBinding ? "" : " ")
                    + ")"
                    + "\n"
                    + QString(SQLPrettyLen - 1, ' ')
                    + ","
                    + " ("
                    + (_useBinding ? "" : "\n"))
                + (_useBinding ? "" : "\n")
                + (_useBinding ? "" : QString(SQLPrettyLen, ' '))
                + (_useBinding ? "" : " ")
                + ")");
        }
        else {
            QueryParts.append("VALUES(");
            QueryParts.append(rows.join("),("));
            QueryParts.append("VALUES(");
        }
    }
    else {
        QueryParts.append(this->Data->CreateQueryPreparedItems.Select);
    }

    //-----------
    BoundQueryString.QueryString = QueryParts.join(SQLPrettyLen ? "\n" : " ");

    if (_args.size())
    {
        for (QVariantMap::const_iterator arg = _args.begin(); arg != _args.end(); ++arg)
        {
            BoundQueryString.QueryString.replace(arg.key(), arg.value().toString());
        }
    }

    //-----------
    return BoundQueryString;
}

quint64 CreateQuery::execute(quint64 _currentUserID, QVariantMap _args, bool _useBinding)
{
    stuBoundQueryString BoundQueryString = this->buildQueryString(_currentUserID, _args, _useBinding);

#ifdef QT_DEBUG
    if (_useBinding) {
        QStringList BindingValuesList;
        foreach (auto b, BoundQueryString.BindingValues) {
            BindingValuesList.append(b.toString());
        }

        qDebug().nospace().noquote() << endl
                                     << endl << "-- Query:" << endl << BoundQueryString.QueryString << endl
                                     << endl << "-- Binding Values:" << endl << BoundQueryString.BindingValues << endl
                                     << "-- [" << BindingValuesList.join(", ") << "]" << endl;
    }
    else {
        qDebug().nospace().noquote() << endl
                                     << endl << "-- Query:" << endl << BoundQueryString.QueryString << endl;
    }
#endif

    clsDAC DAC(this->Data->Table.domain(), this->Data->Table.Schema);

    clsDACResult Result = DAC.execQuery(
                              "",
                              BoundQueryString.QueryString,
                              BoundQueryString.BindingValues
                              );

//    qDebug() << "--- CreateQuery::execute()" << __FILE__ << __LINE__ << Result.toJson(false);

    return Result.lastInsertId().toULongLong();
}

/***************************************************************************************/
/* clsUpdateQueryData ******************************************************************/
/***************************************************************************************/
struct stuUpdateQueryPreparedItems {
    QStringList     SetCols;

    QVariantList    BindingValues;
    bool            IsPrepared = false;
};

class clsUpdateQueryData : public clsBaseQueryData<UpdateQuery>
{
public:
    clsUpdateQueryData(clsTable& _table, const QString& _alias = {}) : clsBaseQueryData<UpdateQuery>(_table, _alias) {}
//    clsUpdateQueryData(const clsUpdateQueryData& _other) : Table(_other.Table), Alias(_other.Alias) {}
//    ~clsUpdateQueryData() {}

    virtual void prepare(quint64 _currentUserID, bool _useBinding/*, quint8 _prettifierJustifyLen*/) {
        if (this->UpdateQueryPreparedItems.IsPrepared)
            return;

        this->UpdateQueryPreparedItems.IsPrepared = true;

        clsBaseQueryData<UpdateQuery>::prepare(_useBinding/*, _prettifierJustifyLen*/);

        /****************************************************************************/
        QString MainTableNameOrAlias = this->Alias.length() ? this->Alias : this->Table.Name;

        /****************************************************************************/
        if (this->SetMaps.isEmpty())
            throw exHTTPBadRequest("No columns found for updating");

        QList<QString> providedCols;

        QString equalSign = (SQLPrettyLen ? " = " : "=");

        foreach (auto Col, this->SetMaps) {
            QString key = Col.first;
            QVariant val = Col.second;

            stuRelatedORMField& relatedORMField = this->Table.SelectableColsMap[key];
            if (relatedORMField.isValid() == false) {
                qDebug() << "valid columns: " << this->Table.SelectableColsMap.keys().join(", ");
                throw exQueryBuilderColumnNotFound("Column <" + key + "> not found");
            }

            if (relatedORMField.Col.isReadOnly())
                throw exHTTPInternalServerError("Invalid change to read-only column <" + key + ">");

            QString colName = makeColName(MainTableNameOrAlias, relatedORMField.Col);

            if (val.userType() == QMetaType::QStringList) {
                QStringList l = val.value<QStringList>();
                QString tableName = l.at(0);
                QString colName = l.at(1);

                if (tableName.isEmpty())
                    this->UpdateQueryPreparedItems.SetCols.append(QString("%1%2%3").arg(colName).arg(equalSign).arg(colName));
                else
                    this->UpdateQueryPreparedItems.SetCols.append(QString("%1%2%3.%4").arg(colName).arg(equalSign).arg(tableName).arg(colName));
            }
            else {
                providedCols.append(key);

                if (val.userType() != QMetaTypeId<DBExpression>::qt_metatype_id())
                    relatedORMField.Col.validate(val);


                if (val.userType() == QMetaTypeId<DBExpression>::qt_metatype_id() || _useBinding == false) {
                    QString v = makeValueAsSQL(val, _useBinding == false, &relatedORMField.Col);
                    this->UpdateQueryPreparedItems.SetCols.append(QString("%1%2%3").arg(colName).arg(equalSign).arg(v));
                }
                else {
                    this->UpdateQueryPreparedItems.SetCols.append(QString("%1%2?").arg(colName).arg(equalSign));
                    this->UpdateQueryPreparedItems.BindingValues.append(relatedORMField.Col.toDB(val));
                }
            }
        }

        foreach (clsORMField baseCol, this->Table.BaseCols) {
            if (providedCols.contains(baseCol.name()) == false && baseCol.updatableBy() == enuUpdatableBy::__UPDATER__ ) {
                auto colName = makeColName(MainTableNameOrAlias, baseCol);

                if (_useBinding == false)
                    this->UpdateQueryPreparedItems.SetCols.append(QString("%1%2%3").arg(colName).arg(equalSign).arg(baseCol.toDB(_currentUserID).toString()));
                else {
                    this->UpdateQueryPreparedItems.SetCols.append(QString("%1%2?").arg(colName).arg(equalSign));
                    this->UpdateQueryPreparedItems.BindingValues.append(baseCol.toDB(_currentUserID));
                }
            }
        }

    }

public:
//    QVariantMap SetMaps; error in key order: sorted by QMap
//    QVariantHash SetMaps; error in key order: sorted by QHash iterator
    QList<QPair<QString, QVariant>> SetMaps;

    stuUpdateQueryPreparedItems UpdateQueryPreparedItems;
};

/***************************************************************************************/
/* UpdateQuery *************************************************************************/
/***************************************************************************************/
UpdateQuery::UpdateQuery(const UpdateQuery& _other) :
    tmplBaseQuery<UpdateQuery, clsUpdateQueryData>(_other),
    tmplQueryJoinTrait<UpdateQuery>(_other),
    tmplQueryWhereTrait<UpdateQuery>(_other)
{
    tmplQueryJoinTrait<UpdateQuery>::JoinTraitData->Owner = this;
    tmplQueryWhereTrait<UpdateQuery>::WhereTraitData->Owner = this;
}
UpdateQuery::UpdateQuery(clsTable& _table, const QString& _alias) :
    tmplBaseQuery<UpdateQuery, clsUpdateQueryData>(_table, _alias),
    tmplQueryJoinTrait<UpdateQuery>(this),
    tmplQueryWhereTrait<UpdateQuery>(this)
{}
UpdateQuery::~UpdateQuery() {}

/***********************\
|* Set                 *|
\***********************/
UpdateQuery& UpdateQuery::setNull(const QString& _col)
{
    return this->set(_col, DBExpression::NIL());
}
UpdateQuery& UpdateQuery::set(const QString& _col, const QVariant& _value)
{
    this->Data->SetMaps.append({ _col, _value });
    return *this;
}
UpdateQuery& UpdateQuery::set(const QString& _col, const QString& _otherTable, const QString& _otherCol)
{
    this->Data->SetMaps.append({ _col, QStringList({ _otherTable, _otherCol }) });
    return *this;
}

/***********************\
|* Execute             *|
\***********************/
stuBoundQueryString UpdateQuery::buildQueryString(quint64 _currentUserID, QVariantMap _args, bool _useBinding/*, quint8 _prettifierJustifyLen*/)
{
    stuBoundQueryString BoundQueryString;

    this->Data->prepare(_currentUserID, _useBinding/*, _prettifierJustifyLen*/);
    this->WhereTraitData->prepare();
    //it should be the last preparation call, as the previous preparation may cause an automatic join
    this->JoinTraitData->prepare();

    if (this->WhereTraitData->PreparedItems.Where.isEmpty())
        throw exQueryBuilderWhereClauseNotProvided("Where cluase of query is empty. This is very dangerous.");

    if (_useBinding)
        BoundQueryString.BindingValues = this->Data->UpdateQueryPreparedItems.BindingValues;

    QStringList QueryParts;

    //-----------
    if (SQLPrettyLen) {
        QueryParts.append(QString("UPDATE").rightJustified(SQLPrettyLen)
                          + " "
                          + this->Data->BaseQueryPreparedItems.From);
    }
    else {
        QueryParts.append("UPDATE");
        QueryParts.append(this->Data->BaseQueryPreparedItems.From);
    }

    if (this->JoinTraitData->PreparedItems.Joins.length()) {
        if (SQLPrettyLen)
            QueryParts.append(this->JoinTraitData->PreparedItems.Joins.join("\n"));
        else
            QueryParts.append(this->JoinTraitData->PreparedItems.Joins.join(" "));
    }

    //-----------
    if (SQLPrettyLen) {
        QueryParts.append(QString("SET").rightJustified(SQLPrettyLen)
                          + " "
                          + this->Data->UpdateQueryPreparedItems.SetCols.join("\n" + QString(SQLPrettyLen - 1, ' ') + "," + " "));
    }
    else {
        QueryParts.append("SET");
        QueryParts.append(this->Data->UpdateQueryPreparedItems.SetCols.join(","));
    }

    //-----------
    QStringList WhereParts;

    if (this->WhereTraitData->PreparedItems.Where.size())
        WhereParts.append(this->WhereTraitData->PreparedItems.Where);

    if (WhereParts.size())
    {
        if (SQLPrettyLen) {
            QueryParts.append(QString("WHERE").rightJustified(SQLPrettyLen)
                              + " "
                              + WhereParts.join("\n" + QString(SQLPrettyLen, ' ') + " "));
        }
        else {
            QueryParts.append("WHERE");
            QueryParts.append(WhereParts.join(" "));
        }
    }

    //-----------
    BoundQueryString.QueryString = QueryParts.join(SQLPrettyLen ? "\n" : " ");

    if (_args.size())
    {
        for (QVariantMap::const_iterator arg = _args.begin(); arg != _args.end(); ++arg)
        {
            BoundQueryString.QueryString.replace(arg.key(), arg.value().toString());
        }
    }

    //-----------

    return BoundQueryString;
}

quint64 UpdateQuery::execute(quint64 _currentUserID, QVariantMap _args, bool _useBinding)
{
    stuBoundQueryString BoundQueryString = this->buildQueryString(_currentUserID, _args, _useBinding);

#ifdef QT_DEBUG
    if (_useBinding) {
        QStringList BindingValuesList;
        foreach (auto b, BoundQueryString.BindingValues) {
            BindingValuesList.append(b.toString());
        }

        qDebug().nospace().noquote() << endl
                                     << endl << "-- Query:" << endl << BoundQueryString.QueryString << endl
                                     << endl << "-- Binding Values:" << endl << BoundQueryString.BindingValues << endl
                                     << "-- [" << BindingValuesList.join(", ") << "]" << endl;
    }
    else {
        qDebug().nospace().noquote() << endl
                                     << endl << "-- Query:" << endl << BoundQueryString.QueryString << endl;
    }
#endif

    clsDAC DAC(this->Data->Table.domain(), this->Data->Table.Schema);

    clsDACResult Result = DAC.execQuery(
                              "",
                              BoundQueryString.QueryString,
                              BoundQueryString.BindingValues
                              );

    return Result.numRowsAffected();
}

/***************************************************************************************/
/* clsDeleteQueryData ******************************************************************/
/***************************************************************************************/
struct stuDeleteQueryPreparedItems {
    QStringList Targets;
    bool        IsPrepared = false;
};

class clsDeleteQueryData : public clsBaseQueryData<DeleteQuery>
{
public:
    clsDeleteQueryData(clsTable& _table, const QString& _alias = {}) : clsBaseQueryData<DeleteQuery>(_table, _alias) {}
//    clsDeleteQueryData(const clsDeleteQueryData& _other) : Table(_other.Table), Alias(_other.Alias) {}
//    ~clsDeleteQueryData() {}

    virtual void prepare(quint64 _currentUserID, bool _useBinding/*, quint8 _prettifierJustifyLen*/) {
        if (this->DeleteQueryPreparedItems.IsPrepared)
            return;
        this->DeleteQueryPreparedItems.IsPrepared = true;

        clsBaseQueryData<DeleteQuery>::prepare(_useBinding/*, _prettifierJustifyLen*/);

        /****************************************************************************/
        QString MainTableNameOrAlias = this->Alias.length() ? this->Alias : this->Table.Name;

        /****************************************************************************/
        this->DeleteQueryPreparedItems.Targets = this->Targets;

        /****************************************************************************/
    }

public:
    QStringList Targets;
    stuDeleteQueryPreparedItems DeleteQueryPreparedItems;
};

/***************************************************************************************/
/* DeleteQuery *************************************************************************/
/***************************************************************************************/
DeleteQuery::DeleteQuery(const DeleteQuery& _other) :
    tmplBaseQuery<DeleteQuery, clsDeleteQueryData>(_other),
    tmplQueryJoinTrait<DeleteQuery>(_other),
    tmplQueryWhereTrait<DeleteQuery>(_other)
{
    tmplQueryJoinTrait<DeleteQuery>::JoinTraitData->Owner = this;
    tmplQueryWhereTrait<DeleteQuery>::WhereTraitData->Owner = this;
}
DeleteQuery::DeleteQuery(clsTable& _table, const QString& _alias) :
    tmplBaseQuery<DeleteQuery, clsDeleteQueryData>(_table, _alias),
    tmplQueryJoinTrait<DeleteQuery>(this),
    tmplQueryWhereTrait<DeleteQuery>(this)
{}
DeleteQuery::~DeleteQuery() {}

/***********************\
|* Target              *|
\***********************/
DeleteQuery& DeleteQuery::addTarget(const QString& _targetTableName)
{
    this->Data->Targets.append(_targetTableName);
    return *this;
}

/***********************\
|* Execute             *|
\***********************/
QString DeleteQuery::buildQueryString(quint64 _currentUserID, QVariantMap _args/*, quint8 _prettifierJustifyLen*/)
{
    this->Data->prepare(_currentUserID, false/*, _prettifierJustifyLen*/);
    this->WhereTraitData->prepare();
    //it should be the last preparation call, as the previous preparation may cause an automatic join
    this->JoinTraitData->prepare();

    if (this->WhereTraitData->PreparedItems.Where.isEmpty())
        throw exQueryBuilderWhereClauseNotProvided("Where cluase of query is empty. This is very dangerous.");

    //push
    QStringList QueryParts;

    //-----------
    if (SQLPrettyLen) {
        QueryParts.append(QString("DELETE").rightJustified(SQLPrettyLen)
                          + " "
                          + this->Data->BaseQueryPreparedItems.From);

        if (this->Data->DeleteQueryPreparedItems.Targets.length())
            QueryParts.append(QString(SQLPrettyLen - 1, ' ')
                              + ","
                              + " "
                              + this->Data->DeleteQueryPreparedItems.Targets.join("\n" + QString(SQLPrettyLen - 1, ' ') + "," + " "));

        QueryParts.append(QString("FROM").rightJustified(SQLPrettyLen)
                          + " "
                          + this->Data->BaseQueryPreparedItems.From);
    }
    else {
        QueryParts.append("DELETE");
        QueryParts.append(this->Data->BaseQueryPreparedItems.From);

        if (this->Data->DeleteQueryPreparedItems.Targets.length())
            QueryParts.append("," + this->Data->DeleteQueryPreparedItems.Targets.join(","));

        QueryParts.append("FROM");
        QueryParts.append(this->Data->BaseQueryPreparedItems.From);
    }

    //-----------
    if (this->JoinTraitData->PreparedItems.Joins.length()) {
        if (SQLPrettyLen)
            QueryParts.append(this->JoinTraitData->PreparedItems.Joins.join("\n"));
        else
            QueryParts.append(this->JoinTraitData->PreparedItems.Joins.join(" "));
    }

    //-----------
    QStringList WhereParts;

    if (this->WhereTraitData->PreparedItems.Where.size())
        WhereParts.append(this->WhereTraitData->PreparedItems.Where);

    if (WhereParts.size())
    {
        if (SQLPrettyLen) {
            QueryParts.append(QString("WHERE").rightJustified(SQLPrettyLen)
                              + " "
                              + WhereParts.join("\n" + QString(SQLPrettyLen, ' ') + " "));
        }
        else {
            QueryParts.append("WHERE");
            QueryParts.append(WhereParts.join(" "));
        }
    }

    //-----------
    QString QueryString = QueryParts.join(SQLPrettyLen ? "\n" : " ");

    if (_args.size())
    {
        for (QVariantMap::const_iterator arg = _args.begin(); arg != _args.end(); ++arg)
        {
            QueryString.replace(arg.key(), arg.value().toString());
        }
    }

    //-----------

    return QueryString;
}

quint64 DeleteQuery::execute(quint64 _currentUserID, QVariantMap _args)
{
    QString QueryString = this->buildQueryString(_currentUserID, _args);

#ifdef QT_DEBUG
    qDebug().nospace().noquote() << endl
                                 << endl << "-- Query:" << endl << QueryString << endl;
#endif

    clsDAC DAC(this->Data->Table.domain(), this->Data->Table.Schema);

    clsDACResult Result = DAC.execQuery("", QueryString);

    return Result.numRowsAffected();
}

/***************************************************************************************/

template class tmplBaseQuery<SelectQuery, clsSelectQueryData>;
template class tmplQueryJoinTrait<SelectQuery>;
template class tmplQueryWhereTrait<SelectQuery>;
template class tmplQueryGroupAndHavingTrait<SelectQuery>;

template class tmplBaseQuery<CreateQuery, clsCreateQueryData>;

template class tmplBaseQuery<UpdateQuery, clsUpdateQueryData>;
template class tmplQueryJoinTrait<UpdateQuery>;
template class tmplQueryWhereTrait<UpdateQuery>;

template class tmplBaseQuery<DeleteQuery, clsDeleteQueryData>;
template class tmplQueryJoinTrait<DeleteQuery>;
template class tmplQueryWhereTrait<DeleteQuery>;

}
}
}

//Q_DECLARE_METATYPE(Targoman::API::ORM::stuColSpecs::unnAggregation);
