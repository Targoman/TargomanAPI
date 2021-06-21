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

#define KZ_TRACE(msg) \
    throw exQueryBuilder(QString(">>>>>>>>>>>>>>>> [%1:%2] %3").arg(__FUNCTION__).arg(__LINE__) \
        .arg(msg));

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
    const QString& _tableNameOrAlias,
    const QMap<QString, stuFilteredCol>& _filterables,
    bool _allowUseColumnAlias,
    quint8 _prettifierJustifyLen
) const {
    if (this->Data->Conditions.isEmpty())
        return "";

    QString CondStr;

    for (QList<stuConditionData>::const_iterator iter = this->Data->Conditions.begin();
         iter != this->Data->Conditions.end();
         ++iter)
    {
        stuConditionData conditionData = *iter;

        if (CondStr.isEmpty() == false)
            CondStr += " ";

        if (conditionData.isAggregator())
        {
            if (iter == this->Data->Conditions.end()-1)
                throw exQueryBuilder(QString("aggregator '%1' must follow a valid condition").arg(conditionData.Col));

            if (_prettifierJustifyLen)
                CondStr += "\n" + conditionData.Col.rightJustified(_prettifierJustifyLen);
            else
                CondStr += conditionData.Col;
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
                        _tableNameOrAlias,
                        _filterables,
                        _allowUseColumnAlias,
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
            if (conditionData.TableNameOrAlias.length() && (conditionData.TableNameOrAlias != _tableNameOrAlias))
            {
                CondStr += conditionData.TableNameOrAlias;
                CondStr += ".";
                CondStr += conditionData.Col;
            }
            else
            {
                if (_allowUseColumnAlias)
                {
                    CondStr += conditionData.TableNameOrAlias.length() ? conditionData.TableNameOrAlias : _tableNameOrAlias;
                    CondStr += conditionData.Col;
                }
                else
                {
                    auto FilteredCol = _filterables.value(conditionData.Col);
                    if (FilteredCol.isValid() == false)
                        throw exQueryBuilder("Invalid column for filtering: " + conditionData.Col);

                    CondStr += makeColName(
                                conditionData.TableNameOrAlias.length() ? conditionData.TableNameOrAlias : _tableNameOrAlias,
                                FilteredCol.Col,
                                false,
                                FilteredCol.Relation);
                }
            }

            if (conditionData.Operator == enuConditionOperator::Null)
                CondStr += " IS NULL";
            else if (conditionData.Operator == enuConditionOperator::NotNull)
                CondStr += " IS NOT NULL";
            else if (conditionData.Operator == enuConditionOperator::Like)
            {
                if (conditionData.Value.isValid() == false)
                    throw exQueryBuilder("Value of LIKE condition is empty");

                CondStr += " LIKE '";
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
                {
                    QString v = conditionData.Value.value<QString>();
                    if (conditionData.Value.userType() == QMetaType::QString)
                        v = "'" + v + "'";
                    CondStr += v;
                }
                else
                {
                    if (conditionData.OtherTableNameOrAlias.length() && (conditionData.OtherTableNameOrAlias != _tableNameOrAlias))
                    {
                        CondStr += conditionData.OtherTableNameOrAlias;
                        CondStr += ".";
                        CondStr += conditionData.Col;
                    }
                    else
                    {
                        if (_allowUseColumnAlias)
                        {
                            CondStr += conditionData.TableNameOrAlias.length() ? conditionData.TableNameOrAlias : _tableNameOrAlias;
                            CondStr += conditionData.Col;
                        }
                        else
                        {
                            auto FilteredCol = _filterables.value(conditionData.Col);
                            if (FilteredCol.isValid() == false)
                                throw exQueryBuilder("Invalid column for filtering: " + conditionData.Col);

                            CondStr += makeColName(
                                        conditionData.TableNameOrAlias.length() ? conditionData.TableNameOrAlias : _tableNameOrAlias,
                                        FilteredCol.Col,
                                        false,
                                        FilteredCol.Relation);
                        }
                    }
                }
            }

//                qDebug() << endl << endl << CondStr << endl << endl;
//                KZ_TRACE(QString("condstr(%1) col(%2)").arg(CondStr).arg(conditionData.Col));
        }
    }

//    qDebug() << endl << endl << "---------------" << endl << CondStr << endl << endl;

    if (CondStr.isEmpty())
        throw exQueryBuilder("Seems that condition is empty");

    return CondStr;
}
/***************************************************************************************/
/* clsCondition ********************************************************************/
/***************************************************************************************/
/*clsCondition::clsCondition() :
    clsCondition() {}
clsCondition::clsCondition(const clsCondition& _other) :
    clsCondition(_other) {}

clsCondition::clsCondition(
        QString _tableName,
        QString _col,
        enuConditionOperator::Type _operator,
        QVariant _value) :
    clsCondition(_foreignCol.trimmed(), _operator, _value) {}
clsCondition::clsCondition(
        QString _leftHandTableName,
        QString _leftHandCol,
        enuConditionOperator::Type _operator,
        QString _rightHandTableName,
        QString _rightHandCol)
    clsCondition(_foreignCol.trimmed(), _operator, QString("{:{:{:%1").arg(_otherCol)) {
    Q_UNUSED(_otherIsColumnName)
}

clsCondition::~clsCondition() {}

clsCondition& clsCondition::andCond(const clsCondition& _cond) {
    clsCondition::andCond(_cond);
    return *this;
}
clsCondition& clsCondition::orCond(const clsCondition& _cond)  {
    clsCondition::orCond(_cond);
    return *this;
}
clsCondition& clsCondition::xorCond(const clsCondition& _cond) {
    clsCondition::xorCond(_cond);
    return *this;
}*/
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

struct stuSelectItems {
    QStringList Cols;
    QStringList From;
    QString     Where;
    QStringList OrderBy;
    QStringList GroupBy;
    QString     Having;
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
/* clsSelectQueryData ******************************************************************/
/***************************************************************************************/
class clsSelectQueryData : public QSharedData
{
public:
    clsSelectQueryData(const clsTable& _table, const QString& _alias = {}) : Table(_table), Alias(_alias) {}
//    clsSelectQueryData(const clsSelectQueryData& _other) : Table(_other.Table), Alias(_other.Alias) {}
//    ~clsSelectQueryData() {}

    void prepare(quint8 _prettifierJustifyLen) {
        /****************************************************************************/
        QString MainTableNameOrAlias;
        if (this->Alias.length()) {
            MainTableNameOrAlias = this->Alias;
            this->QueryItems.From.append(this->Table.Schema + "." + this->Table.Name + " " + this->Alias);
        }
        else {
            MainTableNameOrAlias = this->Table.Name;
            this->QueryItems.From.append(this->Table.Schema + "." + this->Table.Name);
        }
//        qDebug() << "Alias: " << this->Alias;
//        qDebug() << "TableName: " << this->Table.Name;
//        qDebug() << "MainTableNameOrAlias: " << MainTableNameOrAlias;

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
                    parts.append(_col.TrueValue.value<QString>());
                    parts.append(",");
                    parts.append(_col.FalseValue.value<QString>());
                }

                parts.append(")");
                if (_col.TrueValue.isValid() != false)
                    parts.append(")");

                parts.append(" AS ");
                parts.append(_col.RenameAs);

                this->QueryItems.Cols.append(parts.join(""));
            }
            else {
                QString AggFunction;

                if (NULLABLE_IS_NULL(_col.SimpleAggregation)) {
                   ///TODO: why using ANY_VALUE?
                   if (this->GroupByCols.size())
                       AggFunction = "ANY_VALUE(";
                }
                else if (*_col.SimpleAggregation == enuAggregation::DISTINCT_COUNT) {
                    AggFunction = "COUNT(DISTINCT ";
                }
                else {
                    AggFunction = enuAggregation::toStr(*_col.SimpleAggregation);
                    AggFunction += "(";
                }

                const stuFilteredCol& filteredCol = this->Table.SelectableColsMap[_col.Name];
//                qDebug() << _col.Name << ":" << Field.name();
                if (filteredCol.Col.name().isNull())
                    throw exQueryBuilder("Invalid column for filtering: " + _col.Name);
//                    return false;

                auto ColFinalName = makeColName(
                            MainTableNameOrAlias,
                            filteredCol.Col,
                            false /*true*/,
                            filteredCol.Relation == InvalidRelation ? _relation : filteredCol.Relation
                            );
qDebug() << _col.Name << ":" << filteredCol.Col.name() << ":" << ColFinalName;

//                qDebug() << MainTableNameOrAlias;
//                qDebug() << Field.name();
//                qDebug() << ColFinalName;

                if (AggFunction.size()) {
                   this->QueryItems.Cols.append(AggFunction
                                                + ColFinalName.split(' ').first()
                                                + ") AS "
                                                + (_col.RenameAs.size()
                                                   ? _col.RenameAs
                                                   : AggFunction.replace('(', "") + '_' + ColFinalName.split(' ').last()
                                                  )
                                                );
                }
                else
                   this->QueryItems.Cols.append(updateRename(ColFinalName));
            }

            return true;
        }; //addCol

        if (this->RequiredCols.isEmpty())
            foreach(auto Col, this->Table.BaseCols)
                this->QueryItems.Cols.append(makeColName(MainTableNameOrAlias, Col, true));
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
//                    this->QueryItems.Cols.append(makeColName(MainTableName, Col, true, Relation));
//            else
//                foreach(auto RequiredCol, this->RequiredCols)
//                    if (addCol(RequiredCol, Relation))
//                        Joined = true;

//            if (Joined)
//                UsedJoins.insert(Relation);
//        }

        if (this->QueryItems.Cols.isEmpty())
            throw exHTTPBadRequest("No columns found to be reported");

//        if (RequiredCols.size() && RequiredCols.size() > this->QueryItems.Cols.size())
//            throw exHTTPBadRequest("Seems that some columns could not be resolved: Active Cols are: [" + this->QueryItems.Cols.join(", ") + "]");

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

                stuFilteredCol FilteredCol = this->FilterableColsMap.value(PatternMatches.captured(1).trimmed());
                if(FilteredCol.isValid())
                    Rule+=makeColName(this->Name, FilteredCol.Col, false, FilteredCol.Relation);
                else
                    throw exHTTPBadRequest("Invalid column for filtering: " + PatternMatches.captured(1));

                if(FilteredCol.Col.updatableBy() == enuUpdatableBy::__STATUS__)
                    StatusColHasCriteria = true;

                if(FilteredCol.Relation.Column.size() && UsedJoins.contains(FilteredCol.Relation) == false)
                    UsedJoins.insert(FilteredCol.Relation);

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

                Rule += FilteredCol.Col.argSpecs().isPrimitiveType() ? "" : "'";
                QString Value = PatternMatches.captured(3);
                if(Value == "NOW()"
                   || Value.startsWith("DATE_ADD(")
                   || Value.startsWith("DATE_SUB(")
                   )
                    Rule += Value.replace("$SPACE$", " ");
                else{
                    FilteredCol.Col.argSpecs().validate(Value, PatternMatches.captured(1).trimmed().toLatin1());
                    Rule += FilteredCol.Col.toDB(Value).toString();
                }
                Rule += FilteredCol.Col.argSpecs().isPrimitiveType() ? "" : "'";

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
//        foreach (auto Join, UsedJoins) {
//            this->QueryItems.From.append((Join.LeftJoin ? "LEFT JOIN " : "JOIN ")
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
                foreach (stuRelation Rel, this->Table.Relations)
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
                        this->Table.FilterableColsMap,
                        false,
                        _prettifierJustifyLen
                    );
                }
                this->QueryItems.From.append(j);

                //4: append columns
//                QSet<stuRelation> UsedJoins;
//                foreach (stuRelation Relation, this->Table.Relations) {
//                    clsTable* ForeignTable = clsTable::Registry[Relation.ReferenceTable];
//                    if (ForeignTable == nullptr)
//                        throw exHTTPInternalServerError("Reference table has not been registered: " + Relation.ReferenceTable);

//                    bool Joined = false;
//                    if (this->RequiredCols.isEmpty())
//                        foreach(auto Col, ForeignTable->BaseCols)
//                            this->QueryItems.Cols.append(makeColName(MainTableNameOrAlias, Col, true, Relation));
//                    else
//                        foreach(auto RequiredCol, this->RequiredCols)
//                            if (addCol(RequiredCol, Relation))
//                                Joined = true;

//                    if (Joined)
//                        UsedJoins.insert(Relation);
//                }
            }
        }

        /****************************************************************************/
        this->QueryItems.Where = this->WhereClauses.buildConditionString(MainTableNameOrAlias, this->Table.FilterableColsMap, false, _prettifierJustifyLen);

        /****************************************************************************/
//        QStringList this->QueryItems.OrderBy;

        /****************************************************************************/
//        QStringList this->QueryItems.GroupBy;

        /****************************************************************************/
        this->QueryItems.Having = this->HavingClauses.buildConditionString(MainTableNameOrAlias, this->Table.FilterableColsMap, true, _prettifierJustifyLen);
    }

//    clsORMField colByName(const QString& _col) {
//        return this->Table.SelectableColsMap[_col];
//    }

public:
    const clsTable&      Table;
    QString              Alias;

    QList<stuColSpecs>   RequiredCols;
    QList<stuJoin>       Joins;
    clsCondition         WhereClauses;
    QList<stuOrderBy>    OrderByCols;
    QStringList          GroupByCols;
    clsCondition         HavingClauses;

    TAPI::PKsByPath_t    PksByPath;
    quint64              Offset = 0;
    quint16              Limit = 0;

    stuSelectItems       QueryItems;
};

/***************************************************************************************/
/* SelectQuery *************************************************************************/
/***************************************************************************************/
SelectQuery::SelectQuery(const SelectQuery& _other) : Data(_other.Data) {}
SelectQuery::SelectQuery(const clsTable &_table, const QString& _alias) :
    Data(new clsSelectQueryData(_table, _alias))
{
    if (_table.AllCols.isEmpty())
        throw exQueryBuilder("Call prepareFiltersList on table before creating a QueryBuilder");
}
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
|* Join                *|
\***********************/
SelectQuery& SelectQuery::join(enuJoinType::Type _joinType, const QString& _foreignTable, const QString& _alias, const clsCondition& _on)
{
    if (_foreignTable.isEmpty())
        throw exHTTPInternalServerError("Foreign Table is empty.");

    if ((_joinType == enuJoinType::CROSS) || !_on.isEmpty()) {
        this->Data->Joins.append({ _joinType, _foreignTable, _alias, _on });
        return *this;
    }

    //find relation definition
    stuRelation* Relation = nullptr;
    foreach (stuRelation Rel, this->Data->Table.Relations)
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
        this->Data->Alias.length() ? this->Data->Alias : this->Data->Table.Name,
        Relation->Column
    );

    this->Data->Joins.append({ _joinType, _foreignTable, _alias, On });
    return *this;
}

SelectQuery& SelectQuery::leftJoin (const QString& _foreignTable, const clsCondition& _on)                          { return this->join(enuJoinType::LEFT,  _foreignTable, {},     _on); }
SelectQuery& SelectQuery::leftJoin (const QString& _foreignTable, const QString& _alias, const clsCondition& _on)   { return this->join(enuJoinType::LEFT,  _foreignTable, _alias, _on); }
SelectQuery& SelectQuery::rightJoin(const QString& _foreignTable, const clsCondition& _on)                          { return this->join(enuJoinType::RIGHT, _foreignTable, {},     _on); }
SelectQuery& SelectQuery::rightJoin(const QString& _foreignTable, const QString& _alias, const clsCondition& _on)   { return this->join(enuJoinType::RIGHT, _foreignTable, _alias, _on); }
SelectQuery& SelectQuery::innerJoin(const QString& _foreignTable, const clsCondition& _on)                          { return this->join(enuJoinType::INNER, _foreignTable, {},     _on); }
SelectQuery& SelectQuery::innerJoin(const QString& _foreignTable, const QString& _alias, const clsCondition& _on)   { return this->join(enuJoinType::INNER, _foreignTable, _alias, _on); }
SelectQuery& SelectQuery::crossJoin(const QString& _foreignTable, const QString& _alias)                            { return this->join(enuJoinType::CROSS, _foreignTable, _alias);      }

//SelectQuery& SelectQuery::leftJoin(const SelectQuery& _nestedQuery, const QString _alias, const clsCondition& _on)
//{
//    return this->join(enuJoinType::LEFT, _nestedQuery.buildQueryString(), _alias, _on);
//}
//SelectQuery& rightJoin(const SelectQuery& _nestedQuery, const QString _alias, const clsCondition& _on);
//SelectQuery& innerJoin(const SelectQuery& _nestedQuery, const QString _alias, const clsCondition& _on);
//SelectQuery& crossJoin(const SelectQuery& _nestedQuery, const QString _alias);
//SelectQuery& join(enuJoinType::Type _joinType, const SelectQuery& _nestedQuery, const QString _alias, const clsCondition& _on);

SelectQuery& SelectQuery::joinWith(enuJoinType::Type _joinType, const QString& _relationName, const QString& _alias)
{
    if (_relationName.isEmpty())
        throw exHTTPInternalServerError("Relation Name is empty.");

    //find relation definition
    stuRelation* Relation = nullptr;
    foreach (stuRelation Rel, this->Data->Table.Relations)
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
        this->Data->Alias.length() ? this->Data->Alias : this->Data->Table.Name,
        Relation->Column
    );

    this->join(_joinType, Relation->ReferenceTable, _alias, On);
    return *this;
}
SelectQuery& SelectQuery::leftJoinWith (const QString& _relationName, const QString& _alias) { return this->joinWith(enuJoinType::LEFT,  _relationName, _alias); }
SelectQuery& SelectQuery::rightJoinWith(const QString& _relationName, const QString& _alias) { return this->joinWith(enuJoinType::RIGHT, _relationName, _alias); }
SelectQuery& SelectQuery::innerJoinWith(const QString& _relationName, const QString& _alias) { return this->joinWith(enuJoinType::INNER, _relationName, _alias); }

/***********************\
|* Where               *|
\***********************/
SelectQuery& SelectQuery::where(const clsCondition& _condition)
{
    this->Data->WhereClauses = _condition;
    return *this;
}
SelectQuery& SelectQuery::andWhere(const clsCondition& _condition)
{
    if (this->Data->WhereClauses.isEmpty())
        this->Data->WhereClauses = _condition;
    else
        this->Data->WhereClauses.andCond(_condition);
    return *this;
}
SelectQuery& SelectQuery::orWhere(const clsCondition& _condition)
{
    if (this->Data->WhereClauses.isEmpty())
        this->Data->WhereClauses = _condition;
    else
        this->Data->WhereClauses.orCond(_condition);
    return *this;
}
SelectQuery& SelectQuery::xorWhere(const clsCondition& _condition)
{
    if (this->Data->WhereClauses.isEmpty())
        this->Data->WhereClauses = _condition;
    else
        this->Data->WhereClauses.xorCond(_condition);
    return *this;
}

/***********************\
|* Order               *|
\***********************/
SelectQuery& SelectQuery::orderBy(const QString& _col, enuOrderDir::Type _dir)
{
    this->Data->OrderByCols.append({ _col, _dir });
    return *this;
}

/***********************\
|* Group               *|
\***********************/
//SelectQuery& SelectQuery::groupBy(const clsCondition& _condition);
SelectQuery& SelectQuery::groupBy(const QString& _col)
{
    this->Data->GroupByCols.append(_col);
    return *this;
}
SelectQuery& SelectQuery::groupBy(const QStringList& _cols)
{
    foreach(auto Col, _cols)
        this->groupBy(Col);
    return *this;
}

/***********************\
|* Having              *|
\***********************/
SelectQuery& SelectQuery::having(const clsCondition& _condition)
{
    this->Data->HavingClauses = _condition;
    return *this;
}
SelectQuery& SelectQuery::andHaving(const clsCondition& _condition)
{
    if (this->Data->HavingClauses.isEmpty())
        this->Data->HavingClauses = _condition;
    else
        this->Data->HavingClauses.andCond(_condition);
    return *this;
}
SelectQuery& SelectQuery::orHaving(const clsCondition& _condition)
{
    if (this->Data->HavingClauses.isEmpty())
        this->Data->HavingClauses = _condition;
    else
        this->Data->HavingClauses.orCond(_condition);
    return *this;
}
SelectQuery& SelectQuery::xorHaving(const clsCondition& _condition)
{
    if (this->Data->HavingClauses.isEmpty())
        this->Data->HavingClauses = _condition;
    else
        this->Data->HavingClauses.xorCond(_condition);
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

/***********************\
|* Execute             *|
\***********************/
QString SelectQuery::buildQueryString(QVariantMap _args, bool _selectOne, bool _reportCount, quint8 _prettifierJustifyLen)
{
    //    this->Data->Table.prepareFiltersList();
    this->Data->prepare(_prettifierJustifyLen);

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
                          + this->Data->QueryItems.Cols.join("\n" + QString(_prettifierJustifyLen - 1, ' ') + "," + " "));
    }
    else {
        QueryParts.append("SELECT");
        QueryParts.append(this->Data->QueryItems.Cols.join(","));
    }

    //-----------
    if (_prettifierJustifyLen) {
        QueryParts.append(QString("FROM").rightJustified(_prettifierJustifyLen)
                          + " "
                          + this->Data->QueryItems.From.join("\n"));
    }
    else {
        QueryParts.append("FROM");
        QueryParts.append(this->Data->QueryItems.From.join(" "));
    }

    //-----------
    QStringList WhereParts;

    if (this->Data->QueryItems.Where.size())
        WhereParts.append(this->Data->QueryItems.Where);

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
    if (this->Data->QueryItems.OrderBy.size())
    {
        if (_prettifierJustifyLen) {
            QueryParts.append(QString("ORDER BY").rightJustified(_prettifierJustifyLen)
                              + " "
                              + this->Data->QueryItems.OrderBy.join("\n" + QString(_prettifierJustifyLen - 1, ' ') + "," + " "));
        }
        else {
            QueryParts.append("ORDER BY");
            QueryParts.append(this->Data->QueryItems.OrderBy.join(","));
        }
    }

    //-----------
    if (this->Data->QueryItems.GroupBy.size())
    {
        if (_prettifierJustifyLen) {
            QueryParts.append(QString("GROUP BY").rightJustified(_prettifierJustifyLen)
                              + " "
                              + this->Data->QueryItems.GroupBy.join("\n" + QString(_prettifierJustifyLen - 1, ' ') + "," + " "));
        }
        else {
            QueryParts.append("GROUP BY");
            QueryParts.append(this->Data->QueryItems.GroupBy.join(","));
        }
    }

    //-----------
    if (this->Data->QueryItems.Having.size())
    {
        if (_prettifierJustifyLen) {
            QueryParts.append(QString("HAVING").rightJustified(_prettifierJustifyLen)
                              + " "
                              + this->Data->QueryItems.Having);
        }
        else {
            QueryParts.append("HAVING");
            QueryParts.append(this->Data->QueryItems.Having);
        }
    }

    //-----------
    if (_reportCount == false)
    {
        if (this->Data->PksByPath.isEmpty())
        {
            if ((this->Data->Offset > 0) || (this->Data->Limit > 0))
            {
                QueryParts.append("LIMIT");
                if (this->Data->Offset > 0)
                {
                    if (this->Data->Limit > 0)
                        QueryParts.append(QString("%1,%2").arg(this->Data->Offset).arg(this->Data->Limit));
                    else
                        QueryParts.append(QString("%1").arg(this->Data->Offset));
                }
                else //limit > 0
                    QueryParts.append(QString("0,%1").arg(this->Data->Limit));
            }
        }
        else
            QueryParts.append("LIMIT 2"); //Limit is set to 2 in order to produce error if multi values are selected instead of one
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
    if (_reportCount)
        QueryString = "SELECT COUNT(*) AS cnt FROM (" + QueryString + ") qry";

    //-----------
    //pull
    this->Data->Offset = offset;
    this->Data->Limit = limit;

    return QueryString;
}

QVariantMap SelectQuery::one(QVariantMap _args)

{
    QString QueryString = this->buildQueryString(_args, true, false);

    //execute
    QVariantMap Result;

    return Result;
}

TAPI::stuTable SelectQuery::all(QVariantMap _args, quint16 _maxCount, quint64 _from)
{
    this->Data->Offset = _from;
    this->Data->Limit = _maxCount;

    QString QueryString = this->buildQueryString(_args, false, false);

    //execute
    TAPI::stuTable Result;

    return Result;
}

quint64 SelectQuery::count(QVariantMap _args)
{
    QString QueryString = this->buildQueryString(_args, false, true);

    //execute
    quint64 count = 0;

    return count;
}
/***************************************************************************************/

}
}
}

//Q_DECLARE_METATYPE(Targoman::API::ORM::stuColSpecs::unnAggregation);
