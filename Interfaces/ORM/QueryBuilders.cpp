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
    bool IsAggregator = false;
    QString TableAlias;
    QString Col;
    enuConditionOperator::Type Operator;
    QVariant Value;
    clsCondition Condition;

    stuConditionData(QString _col, enuConditionOperator::Type _operator = enuConditionOperator::Null, QVariant _value = {}) :
        IsAggregator(false), Col(_col), Operator(_operator), Value(_value)
    {}
    stuConditionData(QString _tableAlias, QString _col, enuConditionOperator::Type _operator = enuConditionOperator::Null, QVariant _value = {}) :
        IsAggregator(false), TableAlias(_tableAlias), Col(_col), Operator(_operator), Value(_value)
    {}
    stuConditionData(bool _isAggregator, QString _col) :
        IsAggregator(_isAggregator), Col(_col)
    {}
    stuConditionData(const clsCondition& _condition) :
        IsAggregator(false), Condition(_condition)
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
clsCondition::clsCondition()                                                                    : Data(new clsConditionData) {}
clsCondition::clsCondition(const clsCondition& _other)                                          : Data(_other.Data) {}
clsCondition::clsCondition(QString _col, enuConditionOperator::Type _operator, QVariant _value) : Data(new clsConditionData) {
    this->Data->Conditions.append({ _col.trimmed(), _operator, _value });
}
clsCondition::~clsCondition() {}

bool clsCondition::isEmpty() const { return this->Data->Conditions.isEmpty(); }
bool clsCondition::hasMany() const { return (this->Data->Conditions.length() > 1); }

//clsCondition& clsCondition::andScope(const clsCondition& _cond) {
//    return this->openPar(_cond).closePar();
//}
//clsCondition& clsCondition::orScope(const clsCondition& _cond) {
//    return this->openPar(_cond).closePar();
//}
//clsCondition& clsCondition::xorScope(const clsCondition& _cond) {
//    return this->openPar(_cond).closePar();
//}

//clsCondition& clsCondition::openPar(const clsCondition& _cond) {
//    this->Data.addAggregatorCondition(this, '(', _cond);
//    return *this;
//}
//clsCondition& clsCondition::closePar() {
//    this->Data.addAggregatorCondition(this, ')');
//    return *this;
//}

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
//        clsCondition& _that = _this->Data ? _this->Data->NextCondition : *_this;
//    QString AggregatorString;
//    switch (_aggregator) {
//        case '(': AggregatorString = "(";   break;
//        case ')': AggregatorString = ")";   break;
//        case '&': AggregatorString = "AND"; break;
//        case '|': AggregatorString = "OR";  break;
//        case '^': AggregatorString = "XOR"; break;
//        default:
//            throw exQueryBuilder(QString("Invalid condition Aggregator character: '%1'").arg(_aggregator));
//    }

    if (/*(_aggregator != ')' && */_nextCondition.isEmpty())
        throw exQueryBuilder(QString("aggregator '%1' must follow a valid condition").arg(enuPreConditionOperator::toStr(_aggregator)));

    if (this->Data->Conditions.isEmpty() == false)
        this->Data->Conditions.append({ true, enuPreConditionOperator::toStr(_aggregator) });

    this->Data->Conditions.append(stuConditionData(_nextCondition));

//    return _that;
//    return _that.Data->NextCondition;
}

//QString clsCondition::dump() {
//    return QString("col(%1)").arg(this->Data->Col);
    //    return QString("firstCondition.Col(%1) col(%2)").arg(this->Data->FirstCondition->Data->Col).arg(this->Data->Col);
//}

QString clsCondition::buildConditionString(QString _tableName, const QMap<QString, stuFilteredCol>& _filterables) const {
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

            CondStr += conditionData.Col;
        }
        else if (conditionData.Condition.isEmpty() == false)
        {
            if (conditionData.Condition.hasMany())
                CondStr += "(";

            CondStr += conditionData.Condition.buildConditionString(_tableName, _filterables);

            if (conditionData.Condition.hasMany())
                CondStr += ")";
        }
        else
        {
            auto FilteredCol = _filterables.value(conditionData.Col);

            if (FilteredCol.isValid() == false)
                throw exHTTPBadRequest("Invalid column for filtering: " + conditionData.Col);

            CondStr += makeColName(_tableName, FilteredCol.Col, false, FilteredCol.Relation);

            if (conditionData.Operator == enuConditionOperator::Null)
                CondStr += " IS NULL";
            else if (conditionData.Operator == enuConditionOperator::NotNull)
                CondStr += " IS NOT NULL";
            else if (conditionData.Operator == enuConditionOperator::Like)
            {
                CondStr += " LIKE '";
                CondStr += conditionData.Value.value<QString>();
                CondStr += "'";
            }
            else
            {
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
                QString v = conditionData.Value.value<QString>();
                if (conditionData.Value.userType() == QMetaType::QString)
                    v = "'" + v + "'";
                CondStr += v;
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
/***************************************************************************************/
/***************************************************************************************/
struct stuColSpecs {
    QString Name;
    QString RenameAs = {};
//    union unnAggregation {
//        enuAggregation::Type Simple;
//        enuConditionalAggregation::Type Conditional;
//    };
//    NULLABLE_TYPE(unnAggregation) Aggregation = NULLABLE_NULL_VALUE;
    NULLABLE_TYPE(enuAggregation::Type) Aggregation_Simple = NULLABLE_NULL_VALUE;
    NULLABLE_TYPE(enuConditionalAggregation::Type) Aggregation_Conditional = NULLABLE_NULL_VALUE;
    clsCondition Condition = {};
    QVariant TrueValue = {};
    QVariant FalseValue = {};

//    stuColSpecs(const stuColSpecs& _other) :
//        Name(_other.Name),
//        RenameAs(_other.RenameAs),
//        Aggregation_Simple(_other.Aggregation_Simple),
//        Aggregation_Conditional(_other.Aggregation_Conditional),
//        Condition(_other.Condition),
//        TrueValue(_other.TrueValue),
//        FalseValue(_other.FalseValue)
//    {}
//    stuColSpecs(
//        const QString& _name,
//        const QString& _renameAs = {},
//        const NULLABLE_TYPE(unnAggregation)& _aggregation = NULLABLE_NULL_VALUE,
//        const clsCondition& _condition = {},
//        const QVariant& _trueValue = {},
//        const QVariant& _falseValue = {}
//        ) :
//        Name(_name),
//        RenameAs(_renameAs),
//        Aggregation(_aggregation),
//        Condition(_condition),
//        TrueValue(_trueValue),
//        FalseValue(_falseValue)
//    {}
};

struct stuSelectItems {
    QStringList Cols;
    QStringList From;
    QString     Where;
    QStringList OrderBy;
    QStringList GroupBy;
    QString     Having;
};

struct stuQueryRelation {
    enuJoinType::Type JoinType;
    QString ForeignTable;
    QString Alias;
    clsCondition On;

    stuQueryRelation(const enuJoinType::Type& _joinType, const QString& _foreignTable) :
        JoinType(_joinType), ForeignTable(_foreignTable)
    {}

    stuQueryRelation(const enuJoinType::Type& _joinType, const QString& _foreignTable, const QString& _alias) :
        JoinType(_joinType), ForeignTable(_foreignTable), Alias(_alias)
    {}

    stuQueryRelation(const enuJoinType::Type& _joinType, const QString& _foreignTable, const clsCondition& _on) :
        JoinType(_joinType), ForeignTable(_foreignTable), On(_on)
    {}

    stuQueryRelation(const enuJoinType::Type& _joinType, const QString& _foreignTable, const QString& _alias, const clsCondition& _on) :
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
    clsSelectQueryData(const clsTable& _table) : Table(_table) {}
//    clsSelectQueryData(const clsSelectQueryData& _other) : Table(_other.Table) {}
//    ~clsSelectQueryData() {}

    void prepare() {
        auto addCol = [this](const stuColSpecs& _col, const stuRelation& _relation = InvalidRelation) {
            auto updateRename = [_col](QString _fieldString) {
                if (_col.RenameAs.isEmpty())
                    return _fieldString;
                if (_fieldString.contains(" AS "))
                    _fieldString.replace(QRegularExpression(" AS .*"), "");
                return _fieldString + " AS " + _col.RenameAs;
            }; //updateRename

            if (_col.Condition.isEmpty()) {
                QString AggFunction;

//                KZ_TRACE(QString("(%1) (%2)").arg(_col.Name).arg(_col.Aggregation.isNull()));

                if (NULLABLE_IS_NULL(_col.Aggregation_Simple) && NULLABLE_IS_NULL(_col.Aggregation_Conditional)) {
                   ///TODO: why using ANY_VALUE?
                   if (this->GroupByCols.size())
                       AggFunction = "ANY_VALUE(";
                }
                else if (NULLABLE_HAS_VALUE(_col.Aggregation_Simple)) {
//                    KZ_TRACE(QString("(%1) (%2)").arg(_col.Name).arg(*_col.Aggregation_Simple));
                    if (*_col.Aggregation_Simple == enuAggregation::DISTINCT_COUNT) {
//                        KZ_TRACE(QString("(%1) (%2)").arg(_col.Name).arg(*_col.Aggregation_Simple));
                        AggFunction = "COUNT(DISTINCT ";
                    }
                    else {
//                       KZ_TRACE(QString("(%1) (%2) (%3)").arg(_col.Name).arg(*_col.Aggregation_Simple).arg(enuAggregation::options().join("|")));
                        AggFunction = enuAggregation::toStr(*_col.Aggregation_Simple);
                        AggFunction += "(";
                    }
                }

                //KZ_TRACE(QString("(%1) (%2)").arg(_col.Name).arg(this->Table.SelectableColsMap.size()));
                auto Field = this->Table.SelectableColsMap[_col.Name];

                //KZ_TRACE(QString("(%1) (%2)").arg(_col.Name).arg(Field.name()));
                if (Field.name().isNull())
                   return false;

                auto ColFinalName = makeColName(this->Table.Name, Field, true, _relation);
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

                //KZ_TRACE(QString("(%1)").arg(this->QueryItems.Cols.join("|")));
            } //if (_col.Condition.isEmpty())
            else {
//                KZ_TRACE(QString("(%1)").arg(this->QueryItems.Cols.join("|")));

                if (_col.TrueValue.isValid() != _col.FalseValue.isValid())
                    throw exQueryBuilder(QString("Both TrueValue and FalseValue must be empty or not empty").arg(_col.Name));

                QStringList parts;

                QString AggFunction = enuConditionalAggregation::toStr(*_col.Aggregation_Conditional);
                AggFunction.chop(2);
                AggFunction += "(";
                if (_col.TrueValue.isValid())
                    AggFunction += "IF(";
                parts.append(AggFunction);

                parts.append(_col.Condition.buildConditionString(this->Table.Name, this->Table.FilterableColsMap));

                if (_col.TrueValue.isValid())
                {
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

            return true;
        }; //addCol

        if (this->RequiredCols.isEmpty())
            foreach(auto Col, this->Table.BaseCols)
                this->QueryItems.Cols.append(makeColName(this->Table.Name, Col, true));
        else
            foreach(stuColSpecs Col, this->RequiredCols)
                addCol(Col);

//        KZ_TRACE(QString("(%1)").arg(this->QueryItems.Cols.join("|")));

        QSet<stuRelation> UsedJoins;
        foreach (stuRelation Relation, this->Table.ForeignKeys) {
            clsTable* ForeignTable = clsTable::Registry[Relation.ReferenceTable];
            if (ForeignTable == nullptr)
                throw exHTTPInternalServerError("Reference table has not been registered: " + Relation.ReferenceTable);

            bool Joined = false;
            if (this->RequiredCols.isEmpty())
                foreach(auto Col, ForeignTable->BaseCols)
                    this->QueryItems.Cols.append(makeColName(this->Table.Name, Col, true, Relation));
            else
                foreach(auto RequiredCol, this->RequiredCols)
                    if (addCol(RequiredCol, Relation))
                        Joined = true;

            if (Joined)
                UsedJoins.insert(Relation);
        }

        if (this->QueryItems.Cols.isEmpty())
            throw exHTTPBadRequest("No columns found to be reported");

        if (RequiredCols.size() && RequiredCols.size() > this->QueryItems.Cols.size())
            throw exHTTPBadRequest("Seems that some columns could not be resolved: Active Cols are: [" + this->QueryItems.Cols.join(", ") + "]");

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
        this->QueryItems.From.append(this->Table.Schema + "." + this->Table.Name);

        foreach (auto Join, UsedJoins) {
            this->QueryItems.From.append((Join.LeftJoin ? "LEFT JOIN " : "JOIN ")
                + Join.ReferenceTable
                + (Join.RenamingPrefix.size() ? " `" + Join.RenamingPrefix + "`" : "")
                + " ON "
                + (Join.RenamingPrefix.size() ? "`" + Join.RenamingPrefix + "`" : Join.ReferenceTable) + "." + Join.ForeignColumn
                + " = "
                + this->Table.Name + "." + Join.Column
            );
        }

        if (this->Relations.size())
        {
            foreach (auto Relation, this->Relations) {
                QString j = enuJoinType::toStr(Relation.JoinType);
                j += " JOIN ";
                j += Relation.ForeignTable;
                if (Relation.Alias.size())
                    j += " `" + Relation.Alias + "`";
                if (Relation.JoinType != enuJoinType::CROSS)
                {
                    j += " ON ";
                    if (Relation.On.isEmpty())
                    {
                        //auto create 'on' clause

                        if (Relation.Alias.size())
                            j += " `" + Relation.Alias + "`";
                        else
                            j += Relation.ForeignTable;

                        ///TODO: find primary and foreign column names
                        throw exQueryBuilder("find primary and foreign column names");
//                        j += "." + Relation.ForeignColumn;
                    }
                    else
                    {
                        j += Relation.On.buildConditionString(
                            Relation.Alias.size() ? Relation.Alias : this->Table.Name,
                            this->Table.FilterableColsMap);
                    }
                }
                this->QueryItems.From.append(j);
            }
        }

        /****************************************************************************/
        this->QueryItems.Where = this->WhereClauses.buildConditionString(this->Table.Name, this->Table.FilterableColsMap);

        /****************************************************************************/
//        QStringList this->QueryItems.OrderBy;

        /****************************************************************************/
//        QStringList this->QueryItems.GroupBy;

        /****************************************************************************/
        this->QueryItems.Having = this->HavingClauses.buildConditionString(this->Table.Name, this->Table.FilterableColsMap);
    }

    clsORMField colByName(const QString& _col) {
        return this->Table.SelectableColsMap[_col];
    }

public:
    const clsTable&               Table;
    QList<stuColSpecs>            RequiredCols;
    QList<stuQueryRelation>       Relations;
    clsCondition                  WhereClauses;
    QList<stuOrderBy>             OrderByCols;
    QStringList                   GroupByCols;
    clsCondition                  HavingClauses;

    TAPI::PKsByPath_t             PksByPath;
    quint64                       Offset = 0;
    quint16                       Limit = 0;

    stuSelectItems                QueryItems;
};

/***************************************************************************************/
/* SelectQuery *************************************************************************/
/***************************************************************************************/
SelectQuery::SelectQuery(const SelectQuery& _other) : Data(_other.Data) {}
SelectQuery::SelectQuery(const clsTable &_table) : Data(new clsSelectQueryData(_table)) {
    if (_table.AllCols.isEmpty())
        throw exQueryBuilder("Call prepareFiltersList on table before creating a QueryBuilder");
}
SelectQuery::~SelectQuery() {}

/***********************\
|* Columns             *|
\***********************/
//used by APPLY_GET_METHOD_CALL_ARGS_TO_QUERY
SelectQuery& SelectQuery::addCols(const TAPI::Cols_t& _commaSeperatedCols)
{
    ///TODO: split _commaSeperatedCols by comma -> iterate and addCol

    stuColSpecs colspec = { _commaSeperatedCols };
    this->Data->RequiredCols.append(colspec);
    return *this;
}

SelectQuery& SelectQuery::addCols(const QStringList& _cols)
{
    foreach(auto Col, _cols)
        this->addCol(Col);
    return *this;
}

SelectQuery& SelectQuery::addCol(const QString& _col, const QString& _renameAs)
{
    stuColSpecs colspec{ _col, _renameAs };
    this->Data->RequiredCols.append(colspec);
    return *this;
}

SelectQuery& SelectQuery::addCol(enuAggregation::Type _aggFunc, const QString& _col, const QString& _renameAs)
{
    stuColSpecs colspec = { _col, _renameAs, _aggFunc };
    this->Data->RequiredCols.append(colspec);
    return *this;
}

SelectQuery& SelectQuery::addCol(enuConditionalAggregation::Type _aggFunc, const clsCondition& _condition, const QString& _renameAs)
{
    stuColSpecs colspec = { {}, _renameAs, NULLABLE_NULL_VALUE, _aggFunc, _condition };
    this->Data->RequiredCols.append(colspec);
    return *this;
}
SelectQuery& SelectQuery::addCol(enuConditionalAggregation::Type _aggFunc, const clsCondition& _condition, QVariant _trueValue, QVariant _falseValue, const QString& _renameAs)
{
    stuColSpecs colspec = { {}, _renameAs, NULLABLE_NULL_VALUE, _aggFunc, _condition, _trueValue, _falseValue };
    this->Data->RequiredCols.append(colspec);
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
SelectQuery& SelectQuery::leftJoin(const QString& _foreignTable, const clsCondition& _on) { return this->join(enuJoinType::LEFT, _foreignTable, {}, _on); }
SelectQuery& SelectQuery::leftJoin(const QString& _foreignTable, const QString& _alias, const clsCondition& _on) { return this->join(enuJoinType::LEFT, _foreignTable, _alias, _on); }
SelectQuery& SelectQuery::rightJoin(const QString& _foreignTable, const clsCondition& _on) { return this->join(enuJoinType::RIGHT, _foreignTable, {}, _on); }
SelectQuery& SelectQuery::rightJoin(const QString& _foreignTable, const QString& _alias, const clsCondition& _on) { return this->join(enuJoinType::RIGHT, _foreignTable, _alias, _on); }
SelectQuery& SelectQuery::innerJoin(const QString& _foreignTable, const clsCondition& _on) { return this->join(enuJoinType::INNER, _foreignTable, {}, _on); }
SelectQuery& SelectQuery::innerJoin(const QString& _foreignTable, const QString& _alias, const clsCondition& _on) { return this->join(enuJoinType::INNER, _foreignTable, _alias, _on); }
SelectQuery& SelectQuery::crossJoin(const QString& _foreignTable, const QString& _alias) { return this->join(enuJoinType::CROSS, _foreignTable, _alias); }
SelectQuery& SelectQuery::join(enuJoinType::Type _joinType, const QString& _foreignTable, const QString& _alias, const clsCondition& _on)
{
    this->Data->Relations.append({ _joinType, _foreignTable, _alias, _on });
    return *this;
}

//    SelectQuery& leftJoin(const SelectQuery& _nestedQuery, const QString _alias, const clsCondition& _on);
//    SelectQuery& rightJoin(const SelectQuery& _nestedQuery, const QString _alias, const clsCondition& _on);
//    SelectQuery& innerJoin(const SelectQuery& _nestedQuery, const QString _alias, const clsCondition& _on);
//    SelectQuery& crossJoin(const SelectQuery& _nestedQuery, const QString _alias);
//    SelectQuery& join(enuJoinType::Type _joinType, const SelectQuery& _nestedQuery, const QString _alias, const clsCondition& _on);

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
QString SelectQuery::buildQueryString(QVariantMap _args, bool _selectOne, bool _reportCount)
{
//    this->Data->Table.prepareFiltersList();
    this->Data->prepare();

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
    QueryParts.append("SELECT");
    QueryParts.append(this->Data->QueryItems.Cols.join(","));

    //-----------
    QueryParts.append("FROM");
    QueryParts.append(this->Data->QueryItems.From.join(" "));

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
        QueryParts.append("WHERE");
        QueryParts.append(WhereParts.join(" "));
    }

    //-----------
    if (this->Data->QueryItems.OrderBy.size())
    {
        QueryParts.append("ORDER BY");
        QueryParts.append(this->Data->QueryItems.OrderBy.join(","));
    }

    //-----------
    if (this->Data->QueryItems.GroupBy.size())
    {
        QueryParts.append("GROUP BY");
        QueryParts.append(this->Data->QueryItems.GroupBy.join(","));
    }

    //-----------
    if (this->Data->QueryItems.Having.size())
    {
        QueryParts.append("HAVING");
        QueryParts.append(this->Data->QueryItems.Having);
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
    QString QueryString = QueryParts.join(" ");

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
