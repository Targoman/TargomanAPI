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
            ? _tableName
            : (_relation.RenamingPrefix.isEmpty()
                ? _relation.ReferenceTable
                : _relation.RenamingPrefix
              )
           )
           + "."
           + ColName
           + (_appendAs
              ? makeColRenamedAs(_col, _relation.RenamingPrefix)
              : ""
             );
};

QString makeValueAsSQL(const QVariant& _value, bool _qouteIfIsString = true) {
    if (_value.isValid() == false)
        return QString(""); ///TODO: ? throw ?

    if (_value.userType() == QMetaTypeId<stuDBExpression>::qt_metatype_id())
    {
        stuDBExpression v = _value.value<stuDBExpression>();
        if (v.ExprType == enuDBExpressionType::Value)
            return v.Name;

        return QString("%1()").arg(v.Name);
    }
    else if (_value.userType() == QMetaTypeId<stuDBExpressionWithValue>::qt_metatype_id())
    {
        stuDBExpressionWithValue v = _value.value<stuDBExpressionWithValue>();
        if (v.Expression->ExprType == enuDBExpressionType::Value)
            return v.Expression->Name;

        QStringList Params;

        foreach (auto _param, v.Expression->Params) {
            if (v.Values.contains(_param) == false)
                throw exQueryBuilder(QString("DBExpression %1 param %2 not provided.").arg(v.Expression->Name).arg(_param));

            Params.append(makeValueAsSQL(v.Values.value(_param), false));
        }

        return v.Expression->Name + "(" + Params.join(",") + ")";
    }
    else
    {
        QString v = _value.value<QString>();

        if (_qouteIfIsString && (_value.userType() == QMetaType::QString))
            return QString("'%1'").arg(v);

        return v;
    }
};

#define KZ_TRACE(msg) \
    throw exQueryBuilder(QString(">>>>>>>>>>>>>>>> [%1:%2] %3").arg(__FUNCTION__).arg(__LINE__) \
        .arg(msg));

/***************************************************************************************/
//QHash<QString, stuDBExpression> DBExpression::Registry;

DBExpression::DBExpression() {
    this->doRegister("NULL",        enuDBExpressionType::Value);
    this->doRegister("NOW",         enuDBExpressionType::Function);
    this->doRegister("DATE_ADD",    enuDBExpressionType::Function, { "date", "interval" });
    this->doRegister("DATE_SUB",    enuDBExpressionType::Function, { "date", "interval" });
}
DBExpression::~DBExpression() {}

void DBExpression::doRegister(const QString& _name, enuDBExpressionType::Type _type, const QStringList& _params) {
    Q_ASSERT(this->Registry.contains(_name) == false);

    this->Registry.insert(_name, { _name, _type, _params });
}

const stuDBExpression &DBExpression::operator [](const QString& _name) {
    Q_ASSERT(DBExpression::Registry.contains(_name));

    return this->Registry[_name];
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
//    stuConditionData(QString _col, enuConditionOperator::Type _operator, const stuDBExpression& _expression) :
//        IsAggregator(false), Col(_col), Operator(_operator)//, Value(_expression)
//    {
//        this->Value.setValue(_expression);
//    }
//    stuConditionData(QString _tableNameOrAlias, QString _col, enuConditionOperator::Type _operator, const stuDBExpression& _expression) :
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
    Data(new clsConditionData) {}

clsCondition::clsCondition(const clsCondition& _other) :
    Data(_other.Data) {}

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
//        const stuDBExpression& _expression) :
//    Data(new clsConditionData) {
//    this->Data->Conditions.append({ _col, _operator, QVariant::fromValue(_expression) });
//}

//clsCondition::clsCondition(
//        QString _tableNameOrAlias,
//        QString _col,
//        enuConditionOperator::Type _operator,
//        const stuDBExpression& _expression) :
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

QString clsCondition::buildConditionString(
    const QString& _mainTableNameOrAlias,
    const QMap<QString, stuRelatedORMField>& _filterables,
    bool _allowUseColumnAlias, //for having
    quint8 _prettifierJustifyLen
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

            if (_prettifierJustifyLen)
                CondStr += "\n" + conditionData.Col.rightJustified(_prettifierJustifyLen);
            else
                CondStr += " " + conditionData.Col;

            CondStr += " ";
        }
        else if (conditionData.Condition.isEmpty() == false)
        {
            if (conditionData.Condition.hasMany())
            {
                CondStr += "(";
                if (_prettifierJustifyLen)
                    CondStr += "\n" + QString(_prettifierJustifyLen, ' ') + " ";
            }

            CondStr += conditionData.Condition.buildConditionString(
                _mainTableNameOrAlias,
                _filterables,
                _allowUseColumnAlias, //for having
                _prettifierJustifyLen ? _prettifierJustifyLen : 0);

            if (conditionData.Condition.hasMany())
            {
                if (_prettifierJustifyLen)
                    CondStr += "\n" + QString(_prettifierJustifyLen, ' ') + " ";
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
                if (_prettifierJustifyLen)
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

                if (_prettifierJustifyLen)
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

    stuColSpecs(const stuColSpecs& _other) :
        Name(_other.Name),
        RenameAs(_other.RenameAs),
        SimpleAggregation(_other.SimpleAggregation),
        ConditionalAggregation(_other.ConditionalAggregation),
        Condition(_other.Condition),
        TrueValue(_other.TrueValue),
        FalseValue(_other.FalseValue)
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
    clsBaseQueryData(clsTable& _table, const QString& _alias = {}) : Table(_table), Alias(_alias) {}
//    clsSelectQueryData(const clsSelectQueryData& _other) : Table(_other.Table), Alias(_other.Alias) {}
//    ~clsSelectQueryData() {}

    virtual void prepare(bool _useBinding, quint8 _prettifierJustifyLen) {
        Q_UNUSED(_useBinding);
        Q_UNUSED(_prettifierJustifyLen);

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
clsDAC tmplBaseQuery<itmplDerived, itmplData>::DAC() {
    return clsDAC(this->Data->Table.domain(), this->Data->Table.Schema);
}

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
    clsQueryJoinTraitData<itmplDerived>(const itmplDerived* _owner) : Owner(_owner), IsPrepared(false) {}

    virtual void prepare(quint8 _prettifierJustifyLen) {
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
                if (_prettifierJustifyLen)
                    j = j.rightJustified(_prettifierJustifyLen);
                j += " ";
                j += Relation->ReferenceTable;
                if (Join.Alias.size())
                    j += " " + Join.Alias;
                if (Join.JoinType != enuJoinType::CROSS)
                {
                    if (Join.On.isEmpty())
                        throw exHTTPInternalServerError("Condition part of relation not defined.");

                    if (_prettifierJustifyLen)
                        j += "\n" + QString("ON").rightJustified(_prettifierJustifyLen) + " ";
                    else
                        j += " ON ";

                    j += Join.On.buildConditionString(
                        MainTableNameOrAlias,
                        this->Owner->Data->Table.FilterableColsMap,
                        false,
                        _prettifierJustifyLen
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
    const itmplDerived*                Owner;
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
//    return this->join(enuJoinType::LEFT, _nestedQuery.buildQueryString(), _alias, _on);
//}
//template <class itmplDerived, class itmplData> tmplQueryJoinTrait<itmplData>& rightJoin(const template <class itmplDerived, class itmplData> tmplQueryJoinTrait<itmplData>& _nestedQuery, const QString _alias, const clsCondition& _on);
//template <class itmplDerived, class itmplData> tmplQueryJoinTrait<itmplData>& innerJoin(const template <class itmplDerived, class itmplData> tmplQueryJoinTrait<itmplData>& _nestedQuery, const QString _alias, const clsCondition& _on);
//template <class itmplDerived, class itmplData> tmplQueryJoinTrait<itmplData>& crossJoin(const template <class itmplDerived, class itmplData> tmplQueryJoinTrait<itmplData>& _nestedQuery, const QString _alias);
//template <class itmplDerived, class itmplData> tmplQueryJoinTrait<itmplData>& join(enuJoinType::Type _joinType, const template <class itmplDerived, class itmplData> tmplQueryJoinTrait<itmplData>& _nestedQuery, const QString _alias, const clsCondition& _on);

template <class itmplDerived> itmplDerived& tmplQueryJoinTrait<itmplDerived>::joinWith(enuJoinType::Type _joinType, const QString& _relationName, const QString& _alias)
{
    if (_relationName.isEmpty())
        throw exHTTPInternalServerError("Relation Name is empty.");

    //find relation definition
    stuRelation* Relation = nullptr;
    foreach (stuRelation Rel, this->JoinTraitData->Owner->Data->Table.Relations)
    {
        if (Rel.RelationName == _relationName) {
            if (Relation != nullptr)
                throw exHTTPInternalServerError(QString("Multiple relations defined with name (%1).").arg(_relationName));

            Relation = &Rel;
//            break;
        }
    }
    if (Relation == nullptr)
        throw exHTTPInternalServerError(QString("Relation with name (%1) has not been defined.").arg(_relationName));

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
    clsQueryWhereTraitData<itmplDerived>(const itmplDerived* _owner) : Owner(_owner), IsPrepared(false) {}

    virtual void prepare(quint8 _prettifierJustifyLen) {
        if (this->IsPrepared)
            return;
        this->IsPrepared = true;

        /****************************************************************************/
        QString MainTableNameOrAlias = this->Owner->Data->Alias.length() ? this->Owner->Data->Alias : this->Owner->Data->Table.Name;

        /****************************************************************************/
        this->PreparedItems.Where = this->WhereClauses.buildConditionString(
            MainTableNameOrAlias,
            this->Owner->Data->Table.FilterableColsMap,
            false,
            _prettifierJustifyLen);
    }

public:
    const itmplDerived*             Owner;
    bool                            IsPrepared = false;
    clsCondition                    WhereClauses;
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
//    if (this->WhereTraitData->WhereClauses.isEmpty() != false)
//        throw exQueryBuilder("Where clauses are not empty. Please use andWhere, orWhere or xorWhere");

    this->WhereTraitData->WhereClauses = _condition;
    return (itmplDerived&)*this;
}

template <class itmplDerived>
itmplDerived& tmplQueryWhereTrait<itmplDerived>::andWhere(const clsCondition& _condition)
{
    if (this->WhereTraitData->WhereClauses.isEmpty())
        this->WhereTraitData->WhereClauses = _condition;
    else
        this->WhereTraitData->WhereClauses.andCond(_condition);
    return (itmplDerived&)*this;
}

template <class itmplDerived>
itmplDerived& tmplQueryWhereTrait<itmplDerived>::orWhere(const clsCondition& _condition)
{
    if (this->WhereTraitData->WhereClauses.isEmpty())
        this->WhereTraitData->WhereClauses = _condition;
    else
        this->WhereTraitData->WhereClauses.orCond(_condition);
    return (itmplDerived&)*this;
}

template <class itmplDerived>
itmplDerived& tmplQueryWhereTrait<itmplDerived>::xorWhere(const clsCondition& _condition)
{
    if (this->WhereTraitData->WhereClauses.isEmpty())
        this->WhereTraitData->WhereClauses = _condition;
    else
        this->WhereTraitData->WhereClauses.xorCond(_condition);
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
    clsQueryGroupAndHavingTraitData<itmplDerived>(const itmplDerived* _owner) : Owner(_owner), IsPrepared(false) {}

    virtual void prepare(quint8 _prettifierJustifyLen) {
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
            true,
            _prettifierJustifyLen);
    }

public:
    const itmplDerived* Owner;
    bool             IsPrepared = false;
    QStringList      GroupByCols;
    clsCondition     HavingClauses;
    stutmplQueryGroupAndHavingTraitPreparedItems PreparedItems;
};

/***************************************************************************************/
/* tmplQueryGroupAndHavingTrait ************************************************************/
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
    this->GroupAndHavingTraitData->GroupByCols.append(_col);
    return (itmplDerived&)*this;
}

template <class itmplDerived>
itmplDerived& tmplQueryGroupAndHavingTrait<itmplDerived>::groupBy(const QStringList& _cols)
{
    foreach(auto Col, _cols)
        this->groupBy(Col);
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

    virtual void prepare(bool _useBinding, quint8 _prettifierJustifyLen) {
        if (this->SelectQueryPreparedItems.IsPrepared)
            return;
        this->SelectQueryPreparedItems.IsPrepared = true;

        clsBaseQueryData<SelectQuery>::prepare(_useBinding, _prettifierJustifyLen);

        /****************************************************************************/
        QString MainTableNameOrAlias = this->Alias.length() ? this->Alias : this->Table.Name;

        /****************************************************************************/
        auto addCol = [this, &MainTableNameOrAlias, _prettifierJustifyLen](const stuColSpecs& _col, const stuRelation& _relation = InvalidRelation) {
            auto updateRename = [_col](QString _fieldString) {
                if (_col.RenameAs.isEmpty())
                    return _fieldString;
                if (_fieldString.contains(" AS "))
                    _fieldString.replace(QRegularExpression(" AS .*"), "");
                return _fieldString + " AS " + _col.RenameAs;
            }; //updateRename

            if (NULLABLE_HAS_VALUE(_col.ConditionalAggregation)) {
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
                if (_prettifierJustifyLen && _col.Condition.hasMany())
                    AggFunction += "\n" + QString(_prettifierJustifyLen, ' ') + " ";
                parts.append(AggFunction);

                parts.append(_col.Condition.buildConditionString(MainTableNameOrAlias, this->Table.FilterableColsMap, false, _prettifierJustifyLen));

                if (_col.TrueValue.isValid())
                {
                    if (_prettifierJustifyLen && _col.Condition.hasMany())
                        parts.append("\n" + QString(_prettifierJustifyLen, ' ') + " ");
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

                auto ColFinalName = makeColName(
                            MainTableNameOrAlias,
                            relatedORMField.Col,
                            false /*true*/,
                            relatedORMField.Relation == InvalidRelation ? _relation : relatedORMField.Relation
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
        else
            foreach(stuColSpecs Col, this->RequiredCols)
                addCol(Col);

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

                if (_prettifierJustifyLen)
                    this->SelectQueryPreparedItems.Unions.append(cmd.rightJustified(_prettifierJustifyLen));
                else
                    this->SelectQueryPreparedItems.Unions.append(cmd);

                this->SelectQueryPreparedItems.Unions.append(unionPart.Query.buildQueryString({}, false, false, _prettifierJustifyLen));
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

    TAPI::PKsByPath_t    PksByPath;
    quint64              Offset = 0;
    quint16              Limit = 0;
    quint16              CahceTime = 0;

    stuSelectQueryPreparedItems SelectQueryPreparedItems;
};

/***************************************************************************************/
/* SelectQuery *************************************************************************/
/***************************************************************************************/
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
//used by APPLY_GET_METHOD_CALL_ARGS_TO_QUERY
SelectQuery& SelectQuery::addCols(const TAPI::Cols_t& _commaSeperatedCols, const QString& _seperator)
{
    return this->addCols(_commaSeperatedCols.split(_seperator));
}

SelectQuery& SelectQuery::addCols(const QStringList& _cols)
{
    foreach(auto Col, _cols)
        this->addCol(Col);
    return *this;
}

SelectQuery& SelectQuery::addCol(const QString& _col, const QString& _renameAs)
{
    this->Data->RequiredCols.append(stuColSpecs(_col, _renameAs));
    return *this;
}

SelectQuery& SelectQuery::addCol(enuAggregation::Type _aggFunc, const QString& _col, const QString& _renameAs)
{
    this->Data->RequiredCols.append({ _col, _renameAs, _aggFunc });
    return *this;
}

SelectQuery& SelectQuery::addCol(enuConditionalAggregation::Type _aggFunc, const clsCondition& _condition, const QString& _renameAs)
{
    if (_aggFunc == enuConditionalAggregation::COUNTIF)
        this->Data->RequiredCols.append({ {}, _renameAs, NULLABLE_NULL_VALUE, _aggFunc, _condition, 1, DBExpression_Null });
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
//used by APPLY_GET_METHOD_CALL_ARGS_TO_QUERY
SelectQuery& SelectQuery::pksByPath(TAPI::PKsByPath_t _pksByPath)
{
    this->Data->PksByPath = _pksByPath;
    return *this;
}

//used by APPLY_GET_METHOD_CALL_ARGS_TO_QUERY
SelectQuery& SelectQuery::offset(quint64 _offset)
{
    this->Data->Offset = _offset;
    return *this;
}

//used by APPLY_GET_METHOD_CALL_ARGS_TO_QUERY
SelectQuery& SelectQuery::limit(quint16 _limit)
{
    this->Data->Limit = _limit;
    return *this;
}

SelectQuery& SelectQuery::setCacheTime(quint16 _cacheTime)
{
    this->Data->CahceTime = _cacheTime;
    return *this;
}

/***********************\
|* Execute             *|
\***********************/
QString SelectQuery::buildQueryString(QVariantMap _args, bool _selectOne, bool _reportCount, quint8 _prettifierJustifyLen)
{
    //    this->Data->Table.prepareFiltersList();
    this->Data->prepare(false, _prettifierJustifyLen);
    this->WhereTraitData->prepare(_prettifierJustifyLen);
    this->JoinTraitData->prepare(_prettifierJustifyLen);
    this->GroupAndHavingTraitData->prepare(_prettifierJustifyLen);

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
    if (_prettifierJustifyLen) {
        QueryParts.append(QString("SELECT").rightJustified(_prettifierJustifyLen)
                          + " "
                          + this->Data->SelectQueryPreparedItems.Cols.join("\n" + QString(_prettifierJustifyLen - 1, ' ') + "," + " "));
    }
    else {
        QueryParts.append("SELECT");
        QueryParts.append(this->Data->SelectQueryPreparedItems.Cols.join(","));
    }

    //-----------
    if (_prettifierJustifyLen) {
        QueryParts.append(QString("FROM").rightJustified(_prettifierJustifyLen)
                          + " "
                          + this->Data->BaseQueryPreparedItems.From);
    }
    else {
        QueryParts.append("FROM");
        QueryParts.append(this->Data->BaseQueryPreparedItems.From);
    }

    if (this->JoinTraitData->PreparedItems.Joins.length()) {
        if (_prettifierJustifyLen)
            QueryParts.append(this->JoinTraitData->PreparedItems.Joins.join("\n"));
        else
            QueryParts.append(this->JoinTraitData->PreparedItems.Joins.join(" "));
    }

    //-----------
    QStringList WhereParts;

    if (this->WhereTraitData->PreparedItems.Where.size())
        WhereParts.append(this->WhereTraitData->PreparedItems.Where);

    if (this->Data->PksByPath.size())
    {
        QStringList PrimaryKeyQueries = this->Data->PksByPath.split(";");
        QStringList Filters;
        foreach (auto Query, PrimaryKeyQueries)
            foreach (auto Col, this->Data->Table.BaseCols)
                if (Col.isPrimaryKey())
                {
                    if (Query.size())
                        Filters.append(makeColName(this->Data->Table.Name, Col) + " = \"" + Query + "\"");
                    break;
                }

        if (WhereParts.isEmpty())
            WhereParts.append(Filters.join(" AND "));
        else
            WhereParts.append(QString("%1 AND (%2)")
                              .arg(Filters.join(" AND "))
                              .arg(WhereParts.join(" ")));
    }

    if (WhereParts.size())
    {
        if (_prettifierJustifyLen) {
            QueryParts.append(QString("WHERE").rightJustified(_prettifierJustifyLen)
                              + " "
                              + WhereParts.join("\n" + QString(_prettifierJustifyLen, ' ') + " "));
        }
        else {
            QueryParts.append("WHERE");
            QueryParts.append(WhereParts.join(" "));
        }
    }

    //-----------
    if (this->Data->SelectQueryPreparedItems.OrderBy.size())
    {
        if (_prettifierJustifyLen) {
            QueryParts.append(QString("ORDER BY").rightJustified(_prettifierJustifyLen)
                              + " "
                              + this->Data->SelectQueryPreparedItems.OrderBy.join("\n" + QString(_prettifierJustifyLen - 1, ' ') + "," + " "));
        }
        else {
            QueryParts.append("ORDER BY");
            QueryParts.append(this->Data->SelectQueryPreparedItems.OrderBy.join(","));
        }
    }

    //-----------
    if (this->GroupAndHavingTraitData->PreparedItems.GroupBy.size())
    {
        if (_prettifierJustifyLen) {
            QueryParts.append(QString("GROUP BY").rightJustified(_prettifierJustifyLen)
                              + " "
                              + this->GroupAndHavingTraitData->PreparedItems.GroupBy.join("\n" + QString(_prettifierJustifyLen - 1, ' ') + "," + " "));
        }
        else {
            QueryParts.append("GROUP BY");
            QueryParts.append(this->GroupAndHavingTraitData->PreparedItems.GroupBy.join(","));
        }
    }

    //-----------
    if (this->GroupAndHavingTraitData->PreparedItems.Having.size())
    {
        if (_prettifierJustifyLen) {
            QueryParts.append(QString("HAVING").rightJustified(_prettifierJustifyLen)
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
        if (this->Data->PksByPath.isEmpty())
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

                if (_prettifierJustifyLen) {
                    QueryParts.append(QString("LIMIT").rightJustified(_prettifierJustifyLen)
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
        else
            QueryParts.append("LIMIT 2"); //Limit is set to 2 in order to produce error if multi values are selected instead of one
    }

    //-----------
    if (this->Data->UnionParts.length()) {
        if (_prettifierJustifyLen)
            QueryParts.append(this->Data->SelectQueryPreparedItems.Unions.join("\n"));
        else
            QueryParts.append(this->Data->SelectQueryPreparedItems.Unions.join(" "));
    }

    //-----------
    QString QueryString = QueryParts.join(_prettifierJustifyLen ? "\n" : " ");

    if (_args.size())
    {
        for (QVariantMap::const_iterator arg = _args.begin(); arg != _args.end(); ++arg)
        {
            QueryString.replace(arg.key(), arg.value().toString());
        }
    }

    //-----------
    if (_reportCount) {
        if (_prettifierJustifyLen) {
            QueryParts.clear();
            QueryParts.append(QString("SELECT").rightJustified(_prettifierJustifyLen)
                              + " "
                              + "COUNT(*) AS cnt");
            QueryParts.append(QString("FROM").rightJustified(_prettifierJustifyLen)
                              + " "
                              + "(");
            QueryParts.append(QueryString);
            QueryParts.append(QString(")").rightJustified(_prettifierJustifyLen + 2)
                              + " qry");
            QueryString = QueryParts.join(_prettifierJustifyLen ? "\n" : " ");
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
    QString QueryString = this->buildQueryString(_args, false, true);

    //execute
    quint64 count = 0;

    ///TODO: run query

    return count;
}
*/

QVariantMap SelectQuery::one(QVariantMap _args)
{
    QString QueryString = this->buildQueryString(_args, true, false);

    QJsonDocument Result;

    if (this->Data->CahceTime > 0)
        Result = this->DAC().execQueryCacheable(this->Data->CahceTime, "", QueryString)
                 .toJson(true, this->Data->Table.Converters);
    else
        Result = this->DAC().execQuery("", QueryString)
                 .toJson(true, this->Data->Table.Converters);

    if (Result.object().isEmpty())
        throw exHTTPNotFound("No item could be found");

    return Result.toVariant().toMap();
}

QVariantList SelectQuery::all(QVariantMap _args, quint16 _maxCount, quint64 _from)
{
    this->Data->Offset = _from;
    this->Data->Limit = _maxCount;

    QString QueryString = this->buildQueryString(_args, false, false);

    QVariantList Result;

    if (this->Data->CahceTime > 0)
        Result = this->DAC().execQueryCacheable(this->Data->CahceTime, "", QueryString)
                 .toJson(false, this->Data->Table.Converters)
                 .toVariant()
                 .toList();
    else
        Result = this->DAC().execQuery("", QueryString)
                 .toJson(false, this->Data->Table.Converters)
                 .toVariant()
                 .toList();

    return Result;
}

TAPI::stuTable SelectQuery::allWithCount(QVariantMap _args, quint16 _maxCount, quint64 _from)
{
    this->Data->Offset = _from;
    this->Data->Limit = _maxCount;

    QString QueryString = this->buildQueryString(_args, false, false);
    QString CountingQueryString = this->buildQueryString(_args, false, true);

    TAPI::stuTable Result;

    if (this->Data->CahceTime > 0) {
        Result.TotalRows = this->DAC().execQueryCacheable(this->Data->CahceTime, "", CountingQueryString)
                           .value(0)
                           .toULongLong();

        Result.Rows = this->DAC().execQueryCacheable(this->Data->CahceTime, "", QueryString)
                      .toJson(false, this->Data->Table.Converters)
                      .toVariant()
                      .toList();
    }
    else {
        Result.TotalRows = this->DAC().execQuery("", CountingQueryString)
                           .value(0)
                           .toULongLong();

        Result.Rows = this->DAC().execQuery("", QueryString)
                      .toJson(false, this->Data->Table.Converters)
                      .toVariant()
                      .toList();
    }

    return Result;
}

/***************************************************************************************/
/* clsCreateQueryData ******************************************************************/
/***************************************************************************************/
struct stuCreateQueryPreparedItems {
    QStringList         Cols;
    QList<QStringList>  Values;
    QString             Select;

    bool                IsPrepared = false;
};

class clsCreateQueryData : public clsBaseQueryData<CreateQuery>
{
public:
    clsCreateQueryData(clsTable& _table, const QString& _alias = {}) : clsBaseQueryData<CreateQuery>(_table, _alias) {}
//    clsCreateQueryData(const clsCreateQueryData& _other) : Table(_other.Table), Alias(_other.Alias) {}
//    ~clsCreateQueryData() {}

    virtual void prepare(bool _useBinding, quint8 _prettifierJustifyLen) {
        if (this->CreateQueryPreparedItems.IsPrepared)
            return;
        this->CreateQueryPreparedItems.IsPrepared = true;

        clsBaseQueryData<CreateQuery>::prepare(_useBinding, _prettifierJustifyLen);

        /****************************************************************************/
        QString MainTableNameOrAlias = this->Alias.length() ? this->Alias : this->Table.Name;

        /****************************************************************************/
        if (this->Cols.isEmpty())
            throw exHTTPBadRequest("No columns provided for creating");

        QMap<QString, clsORMField> providedBaseCols;

        foreach (clsORMField baseCol, this->Table.BaseCols) {
            if (this->Cols.contains(baseCol.name())) {
                providedBaseCols.insert(baseCol.name(), baseCol);
                this->CreateQueryPreparedItems.Cols.append(makeColName(MainTableNameOrAlias, baseCol));
            }
            else if (baseCol.defaultValue() == QRequired)
                throw exQueryBuilder("Required field <" + baseCol.name() + "> not provided");
        }

        if (this->CreateQueryPreparedItems.Cols.isEmpty())
            throw exQueryBuilder("Nothing to do");

        if (this->Values.length()) {
            foreach (QVariantMap oneRecord, this->Values) {
                QStringList oneRecordToString;
                foreach (auto baseCol, providedBaseCols) {
                    bool dataProvided = false;

                    for (QVariantMap::const_iterator itr = oneRecord.begin(); itr != oneRecord.end(); itr++) {
                        if (itr.key() == baseCol.name()) {
                            dataProvided = true;

                            QVariant val = itr.value();
                            QString v = makeValueAsSQL(val);
                            oneRecordToString.append(v);

                            break;
                        }
                    }

                    if (!dataProvided) {
                        if (baseCol.defaultValue() == QRequired)
                            throw exQueryBuilder("Value for required field <" + baseCol.name() + "> not provided");

                        oneRecordToString.append(makeValueAsSQL(DBExpression_Null));
                    }
                }
                this->CreateQueryPreparedItems.Values.append(oneRecordToString);
            }
            if (this->CreateQueryPreparedItems.Values.isEmpty())
                throw exQueryBuilder("Values is empty");
        }
        else if (this->Select != nullptr)
            this->CreateQueryPreparedItems.Select = this->Select->buildQueryString({}, false, false, _prettifierJustifyLen);
        else
            throw exQueryBuilder("Values or Select not provided");
    }

public:
    QStringList Cols;
    QList<QVariantMap> Values;
    SelectQuery* Select = nullptr;

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

    if (this->Data->Select != nullptr)
        throw new exQueryBuilder("Select query is not empty");

    this->Data->Values.append(_oneRecordValues);

    return *this;
}
CreateQuery& CreateQuery::values(const QList<QVariantMap>& _multipleRecordValues)
{
    if (this->Data->Cols.isEmpty())
        throw new exQueryBuilder("Columns must be defined before values");

    if (this->Data->Select != nullptr)
        throw new exQueryBuilder("Select query is not empty");

    this->Data->Values.append(_multipleRecordValues);

    return *this;
}
/***********************\
|* Select              *|
\***********************/
CreateQuery& CreateQuery::select(SelectQuery& _selectQuery)
{
    if (this->Data->Select != nullptr)
        throw new exQueryBuilder("Columns must be defined before select query");

    if (this->Data->Values.length())
        throw new exQueryBuilder("Values is not empty");

    this->Data->Select = &_selectQuery;

    return *this;
}

/***********************\
|* Execute             *|
\***********************/
QString CreateQuery::buildQueryString(QVariantMap _args, bool _useBinding, quint8 _prettifierJustifyLen)
{
    this->Data->prepare(_useBinding, _prettifierJustifyLen);

    QStringList QueryParts;

    //-----------
    if (_prettifierJustifyLen) {
        QueryParts.append(QString("INSERT").rightJustified(_prettifierJustifyLen));
//        if (OPTIONS)
//        QueryParts.append(QString("INSERT").rightJustified(_prettifierJustifyLen)
//                          + " "
//                          //+ OPTIONS (IGNORE, ...)
//                          );
        QueryParts.append(QString("INTO").rightJustified(_prettifierJustifyLen)
                          + " "
                          + this->Data->BaseQueryPreparedItems.From
                          + " (");
        QueryParts.append(QString(_prettifierJustifyLen, ' ')
                          + " "
                          + this->Data->CreateQueryPreparedItems.Cols.join("\n" + QString(_prettifierJustifyLen - 1, ' ') + "," + " "));
        QueryParts.append(QString(_prettifierJustifyLen, ' ')
                          + " "
                          + ")");
    }
    else {
        QueryParts.append("INSERT");
        QueryParts.append("INTO");
        QueryParts.append(this->Data->BaseQueryPreparedItems.From);
        QueryParts.append("(");
        QueryParts.append(this->Data->CreateQueryPreparedItems.Cols.join(","));
        QueryParts.append(")");
    }

    //-----------
    if (this->Data->CreateQueryPreparedItems.Values.length()) {
        QStringList rows;
        foreach (QStringList row, this->Data->CreateQueryPreparedItems.Values) {
            if (_prettifierJustifyLen) {
                rows.append(QString(_prettifierJustifyLen, ' ')
                            + " "
                            + row.join("\n" + QString(_prettifierJustifyLen - 1, ' ') + "," + " "));
            }
            else {
                rows.append(row.join(","));
            }
        }

        if (_prettifierJustifyLen) {
            QueryParts.append(QString("VALUES").rightJustified(_prettifierJustifyLen)
                              + " "
                              + "(\n"
                              + rows.join("\n"
                                          + QString(_prettifierJustifyLen, ' ')
                                          + " )\n"
                                          + QString(_prettifierJustifyLen - 1, ' ')
                                          + ","
                                          + " (\n")
                              );
            QueryParts.append(QString(_prettifierJustifyLen, ' ')
                              + " "
                              + ")");
        }
        else {
            QueryParts.append("VALUES(");
            QueryParts.append(rows.join("),("));
            QueryParts.append("VALUES(");
        }
    }
    else {
    }

    //-----------
    QString QueryString = QueryParts.join(_prettifierJustifyLen ? "\n" : " ");

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

quint64 CreateQuery::execute(QVariantMap _args, bool _useBinding)
{
    QString QueryString = this->buildQueryString(_args, _useBinding);

    clsDACResult Result = this->DAC().execQuery("", QueryString);

    return Result.lastInsertId().toULongLong();
}

/***************************************************************************************/
/* clsUpdateQueryData ******************************************************************/
/***************************************************************************************/
struct stuUpdateQueryPreparedItems {
    bool        IsPrepared = false;
    QVariantMap BindingValues;
    QStringList SetCols;
};

class clsUpdateQueryData : public clsBaseQueryData<UpdateQuery>
{
public:
    clsUpdateQueryData(clsTable& _table, const QString& _alias = {}) : clsBaseQueryData<UpdateQuery>(_table, _alias) {}
//    clsUpdateQueryData(const clsUpdateQueryData& _other) : Table(_other.Table), Alias(_other.Alias) {}
//    ~clsUpdateQueryData() {}

    virtual void prepare(bool _useBinding, quint8 _prettifierJustifyLen) {
        if (this->UpdateQueryPreparedItems.IsPrepared)
            return;

        this->UpdateQueryPreparedItems.IsPrepared = true;

        clsBaseQueryData<UpdateQuery>::prepare(_useBinding, _prettifierJustifyLen);

        /****************************************************************************/
        QString MainTableNameOrAlias = this->Alias.length() ? this->Alias : this->Table.Name;

        /****************************************************************************/
        if (this->SetMaps.isEmpty())
            throw exHTTPBadRequest("No columns found for updating");

        QString equalSign = (_prettifierJustifyLen ? " = " : "=");

        for (QVariantMap::const_iterator itr = this->SetMaps.constBegin();
             itr != this->SetMaps.constEnd();
             itr++)
        {
            QString key = itr.key();
            QVariant val = itr.value();

            if (val.userType() == QMetaType::QStringList) {
                QStringList l = val.value<QStringList>();
                QString tableName = l.at(0);
                QString colName = l.at(1);

                if (tableName.isEmpty())
                    this->UpdateQueryPreparedItems.SetCols.append(QString("%1%2%3").arg(key).arg(equalSign).arg(colName));
                else
                    this->UpdateQueryPreparedItems.SetCols.append(QString("%1%2%3.%4").arg(key).arg(equalSign).arg(tableName).arg(colName));
            }
            else {
                QString v = makeValueAsSQL(val);

                if (_useBinding) {
                    this->UpdateQueryPreparedItems.SetCols.append(QString("%1%2:%3").arg(key).arg(equalSign).arg(key));
                    this->UpdateQueryPreparedItems.BindingValues.insert(":" + key, v);
                }
                else
                    this->UpdateQueryPreparedItems.SetCols.append(QString("%1%2%3").arg(key).arg(equalSign).arg(v));
            }
        }
    }

public:
    QVariantMap SetMaps;

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
    return this->set(_col, DBExpression_Null);
}
UpdateQuery& UpdateQuery::set(const QString& _col, const QVariant& _value)
{
    this->Data->SetMaps.insert(_col, _value);
    return *this;
}
UpdateQuery& UpdateQuery::set(const QString& _col, const QString& _otherTable, const QString& _otherCol)
{
    this->Data->SetMaps.insert(_col, QStringList({ _otherTable, _otherCol }));
    return *this;
}

/***********************\
|* Execute             *|
\***********************/
stuBoundQueryString UpdateQuery::buildQueryString(QVariantMap _args, bool _useBinding, quint8 _prettifierJustifyLen)
{
    stuBoundQueryString BoundQueryString;

    this->Data->prepare(_useBinding, _prettifierJustifyLen);
    this->WhereTraitData->prepare(_prettifierJustifyLen);
    this->JoinTraitData->prepare(_prettifierJustifyLen);

    if (this->WhereTraitData->PreparedItems.Where.isEmpty())
        throw exQueryBuilder("Where cluase of query is empty. This is very dangerous.");

    if (_useBinding)
        BoundQueryString.BindingValues = this->Data->UpdateQueryPreparedItems.BindingValues;

    QStringList QueryParts;

    //-----------
    if (_prettifierJustifyLen) {
        QueryParts.append(QString("UPDATE").rightJustified(_prettifierJustifyLen)
                          + " "
                          + this->Data->BaseQueryPreparedItems.From);
    }
    else {
        QueryParts.append("UPDATE");
        QueryParts.append(this->Data->BaseQueryPreparedItems.From);
    }

    if (this->JoinTraitData->PreparedItems.Joins.length()) {
        if (_prettifierJustifyLen)
            QueryParts.append(this->JoinTraitData->PreparedItems.Joins.join("\n"));
        else
            QueryParts.append(this->JoinTraitData->PreparedItems.Joins.join(" "));
    }

    //-----------
    if (_prettifierJustifyLen) {
        QueryParts.append(QString("SET").rightJustified(_prettifierJustifyLen)
                          + " "
                          + this->Data->UpdateQueryPreparedItems.SetCols.join("\n" + QString(_prettifierJustifyLen - 1, ' ') + "," + " "));
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
        if (_prettifierJustifyLen) {
            QueryParts.append(QString("WHERE").rightJustified(_prettifierJustifyLen)
                              + " "
                              + WhereParts.join("\n" + QString(_prettifierJustifyLen, ' ') + " "));
        }
        else {
            QueryParts.append("WHERE");
            QueryParts.append(WhereParts.join(" "));
        }
    }

    //-----------
    BoundQueryString.QueryString = QueryParts.join(_prettifierJustifyLen ? "\n" : " ");

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

quint64 UpdateQuery::execute(QVariantMap _args, bool _useBinding)
{
    stuBoundQueryString QueryString = this->buildQueryString(_args, _useBinding);

    clsDACResult Result = this->DAC().execQuery(
                              "",
                              QueryString.QueryString,
                              QueryString.BindingValues
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

    virtual void prepare(bool _useBinding, quint8 _prettifierJustifyLen) {
        if (this->DeleteQueryPreparedItems.IsPrepared)
            return;
        this->DeleteQueryPreparedItems.IsPrepared = true;

        clsBaseQueryData<DeleteQuery>::prepare(_useBinding, _prettifierJustifyLen);

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
QString DeleteQuery::buildQueryString(QVariantMap _args, quint8 _prettifierJustifyLen)
{
    this->Data->prepare(false, _prettifierJustifyLen);
    this->WhereTraitData->prepare(_prettifierJustifyLen);
    this->JoinTraitData->prepare(_prettifierJustifyLen);

    if (this->WhereTraitData->PreparedItems.Where.isEmpty())
        throw exQueryBuilder("Where cluase of query is empty. This is very dangerous.");

    //push
    QStringList QueryParts;

    //-----------
    if (_prettifierJustifyLen) {
        QueryParts.append(QString("DELETE").rightJustified(_prettifierJustifyLen)
                          + " "
                          + this->Data->BaseQueryPreparedItems.From);

        if (this->Data->DeleteQueryPreparedItems.Targets.length())
            QueryParts.append(QString(_prettifierJustifyLen - 1, ' ')
                              + ","
                              + " "
                              + this->Data->DeleteQueryPreparedItems.Targets.join("\n" + QString(_prettifierJustifyLen - 1, ' ') + "," + " "));

        QueryParts.append(QString("FROM").rightJustified(_prettifierJustifyLen)
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
        if (_prettifierJustifyLen)
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
        if (_prettifierJustifyLen) {
            QueryParts.append(QString("WHERE").rightJustified(_prettifierJustifyLen)
                              + " "
                              + WhereParts.join("\n" + QString(_prettifierJustifyLen, ' ') + " "));
        }
        else {
            QueryParts.append("WHERE");
            QueryParts.append(WhereParts.join(" "));
        }
    }

    //-----------
    QString QueryString = QueryParts.join(_prettifierJustifyLen ? "\n" : " ");

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

quint64 DeleteQuery::execute(QVariantMap _args)
{
    QString QueryString = this->buildQueryString(_args);

    clsDACResult Result = this->DAC().execQuery("", QueryString);

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
