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
    if(_relation.Column.size() && _relation.RenamingPrefix.size())
        ColName = ColName.replace(QRegularExpression("^" + _relation.RenamingPrefix), "");
    return  (_relation.Column.isEmpty() ?
                 _tableName :
                 (_relation.RenamingPrefix.isEmpty() ?
                      _relation.ReferenceTable :
                      _relation.RenamingPrefix
                      )
                 ) + "." + ColName +
            (_appendAs ? makeColRenamedAs(_col, _relation.RenamingPrefix) : "");
};

/***************************************************************************************/
/***************************************************************************************/
/***************************************************************************************/
class clsConditionData : public QSharedData
{
public:
    clsConditionData(QString _col, enuConditionOperator::Type _operator = enuConditionOperator::Null, QVariant _value = {}) :
        Col(_col), Operator(_operator), Value(_value)
    { ; }
    bool isAggregator() const { return this->Col.startsWith(" "); }
    bool isOpenPar() const { return  this->Col == " ( "; }
    bool isClosePar() const { return  this->Col == " ) "; }

public:
    QString Col;
    enuConditionOperator::Type Operator;
    QVariant Value;
    clsCondition NextCondition;
};

clsCondition& addCondition(clsCondition* _this, char _aggregator, const clsCondition& _nextCondition = clsCondition()) {
    clsCondition& _that = _this->Data ? _this->Data->NextCondition : *_this;
    QString AggregatorString;

    switch (_aggregator) {
        case '(': AggregatorString = " ( ";   break;
        case ')': AggregatorString = " ) ";   break;
        case '&': AggregatorString = " AND "; break;
        case '|': AggregatorString = " OR ";  break;
        case '^': AggregatorString = " XOR "; break;
        default:
            throw exQueryBuilder(QString("Invalid condition Aggregator character: '%1'").arg(_aggregator));
    }

    if (_aggregator != ')' && _nextCondition.isEmpty())
        throw exQueryBuilder(QString("aggregator '%1' must follow a valid condition").arg(_aggregator));

    _that.Data = new clsConditionData(AggregatorString);
    _that.Data->NextCondition = _nextCondition;

    return _that;
}

QString toStr(QString _tableName, const clsCondition& _this, const QMap<QString, stuFilteredCol>& _filterables) {
    quint16 OpenPars = 0;
    QString CondStr;
    bool CanStartWithLogical = false;

    std::function<void(const clsCondition& _this)> toStrHelper =
            [&OpenPars, &CondStr, &CanStartWithLogical, &toStrHelper, _filterables, _tableName](const clsCondition& _this) {
        if (_this.Data == nullptr)
            return;

        if (_this.Data->isAggregator()) {
            if (_this.Data->isOpenPar()) {
                OpenPars++;
                CanStartWithLogical = false;
            }
            else if (_this.Data->isClosePar()) {
                if(OpenPars == 0)
                    throw exQueryBuilder("Invalid close parenthesis without any open");
                OpenPars --;
                CanStartWithLogical = true;
            }
            else if(CondStr.isEmpty()) {
                if(CanStartWithLogical == false)
                    throw exQueryBuilder("Invalid logical expression prior to any rule");
                CanStartWithLogical = true;
            }

            CondStr += _this.Data->Col;
            toStrHelper(_this.Data->NextCondition);
        }
        else {
            auto FilteredCol = _filterables.value(_this.Data->Col);
            if (FilteredCol.isValid())
                CondStr += makeColName(_tableName, FilteredCol.Col, false, FilteredCol.Relation);
            else
                throw exHTTPBadRequest("Invalid column for filtring: " + _this.Data->Col);
        }
    };

    toStrHelper(_this);
    if (CondStr.isEmpty())
        throw exQueryBuilder("Seems that condition is empty");
    if (OpenPars)
        throw exQueryBuilder("Count of open and close parenthesis does not match");

    return CondStr;
}

/***************************************************************************************/
/***************************************************************************************/
/***************************************************************************************/
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
    QStringList Having;
};

struct stuQueryRelation {
    enuJoinType::Type JoinType;
    QString Table;
    clsCondition On;
};

struct stuOrderBy {
    QString Col;
    enuOrderDir::Type Dir;

//    stuOrderBy() {}
    stuOrderBy(const stuOrderBy& _other) : Col(_other.Col), Dir(_other.Dir) {}
    stuOrderBy(const QString& _col, enuOrderDir::Type _dir) : Col(_col), Dir(_dir) {}
};

struct stuConditionalClause {
    bool IsAnd;
    clsCondition Condition;

//    stuConditionalClause() {}
    stuConditionalClause(const stuConditionalClause& _other) : IsAnd(_other.IsAnd), Condition(_other.Condition) {}
    stuConditionalClause(bool _isAnd, const clsCondition& _condition) : IsAnd(_isAnd), Condition(_condition) {}
};

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
            };

            if (_col.Condition.isEmpty()) {
                auto AggFunction = enuAggregation::toStr(_col.Aggregation->Simple) + '(';
                auto Field = this->Table.SelectableColsMap[_col.Name];
                if (_col.Aggregation.isNull()) {
                    if (this->GroupByCols.size())
                        AggFunction = "ANY_VALUE(";
                }
                else if(_col.Aggregation->Simple == enuAggregation::DISTINCT_COUNT)
                    AggFunction = "COUNT(DISTINCT ";

                if (Field.name().isNull())
                    return false;

                auto ColFinalName = makeColName(this->Table.Name, Field, true, _relation);
                if (AggFunction.size()) {
                    this->QueryItems.Cols.append(AggFunction + ColFinalName.split(' ').first()+ ") AS " + (
                                                     _col.RenameAs.size() ?
                                                         _col.RenameAs :
                                                         AggFunction.replace('(', "") + '_' + ColFinalName.split(' ').last()
                                                         )
                                                 );
                }
                else
                    this->QueryItems.Cols.append(updateRename(ColFinalName));
            }
            else {
                auto AggFunction = enuConditionalAggregation::toStr(_col.Aggregation->Conditional).replace("IF", "(IF(");
                this->QueryItems.Cols.append(AggFunction +
                                                toStr(this->Table.Name, _col.Condition, this->Table.FilterableColsMap) +
                                                ")) AS " + _col.RenameAs
                                             );
            }
            return true;
        };

        if (this->RequiredCols.isEmpty()) {
            foreach(auto Col, this->Table.BaseCols)
                this->QueryItems.Cols.append(makeColName(this->Table.Name, Col, true));
        }
        else {
            foreach(auto RequiredCol, this->RequiredCols) {
                addCol(RequiredCol);
            }
        }
//throw exHTTPBadRequest("*********** EXEXEXEXEXEXEX");

        QSet<stuRelation> UsedJoins;
        foreach(auto Relation, this->Table.ForeignKeys) {
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
            throw exHTTPBadRequest("Seems that some columns could not be resolved: Active Cols are: [" +this->QueryItems.Cols.join(", ")+ "]");








        ///TODO: fill this fields
//        QStringList this->QueryItems.From;
//        QStringList this->QueryItems.Where;
//        QStringList this->QueryItems.OrderBy;
//        QStringList this->QueryItems.GroupBy;
//        QStringList this->QueryItems.Having;







    }

    clsORMField colByName(const QString& _col) {
        return this->Table.SelectableColsMap[_col];
    }

public:
    const clsTable&               Table;
    QList<stuColSpecs>            RequiredCols;
    QList<stuQueryRelation>       Relations;
    QList<stuConditionalClause>   WhereClauses;
    QList<stuOrderBy>             OrderByCols;
    QStringList                   GroupByCols;
    QList<stuConditionalClause>   HavingClauses;

    TAPI::PKsByPath_t             PksByPath;
    quint64                       Offset;
    quint16                       Limit;

    stuSelectItems                QueryItems;
};

/***************************************************************************************/
/* clsCondition ************************************************************************/
/***************************************************************************************/
clsCondition::clsCondition() : Data(nullptr) {}
clsCondition::clsCondition(QString _col, enuConditionOperator::Type _operator, QVariant _value) : Data(new clsConditionData(_col, _operator, _value)) {}
clsCondition::clsCondition(const clsCondition& _other) : Data(_other.Data) {}
clsCondition::~clsCondition() {}

/* static */ clsCondition& clsCondition::scope(const clsCondition& _cond) { return clsCondition().openPar(_cond).closePar(); }
clsCondition& clsCondition::openPar(const clsCondition& _cond) { return addCondition(this, '(', _cond); }
clsCondition& clsCondition::closePar() { return addCondition(this, ')'); }
clsCondition& clsCondition::andCond(const clsCondition& _cond) { return addCondition(this, '&', _cond); }
clsCondition& clsCondition::orCond(const clsCondition& _cond)  { return addCondition(this, '|', _cond); }
clsCondition& clsCondition::xorCond(const clsCondition& _cond) { return addCondition(this, '^', _cond); }
bool clsCondition::isEmpty() const { return (this->Data == nullptr || this->Data->Col.size() == 0); }

/***************************************************************************************/
/* SelectQuery *************************************************************************/
/***************************************************************************************/
SelectQuery::SelectQuery(const clsTable &_table) : Data(new clsSelectQueryData(_table)) {}
SelectQuery::SelectQuery(const SelectQuery& _other) : Data(_other.Data) {}
SelectQuery::~SelectQuery() {}

/***********************\
|* Columns             *|
\***********************/
//used by APPLY_GET_METHOD_CALL_ARGS_TO_QUERY
SelectQuery& SelectQuery::addCols(const TAPI::Cols_t& _cols)
{
    this->Data->RequiredCols.append({ _cols });
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
    this->Data->RequiredCols.append({ _col, _renameAs });
    return *this;
}

SelectQuery& SelectQuery::addCol(enuAggregation::Type _aggFunc, const QString& _col, const QString& _renameAs)
{
    this->Data->RequiredCols.append({ _col, _renameAs, { _aggFunc } });
    return *this;
}

SelectQuery& SelectQuery::addCol(enuConditionalAggregation::Type _aggFunc, const clsCondition& _condition, const QString& _renameAs)
{
    this->Data->RequiredCols.append({ {}, _renameAs, { _aggFunc }, _condition });
    return *this;
}

/***********************\
|* Join                *|
\***********************/
SelectQuery& SelectQuery::join(enuJoinType::Type _joinType, const QString _table, const clsCondition& _on)
{
    this->Data->Relations.append({ _joinType, _table, _on });
    return *this;
}

SelectQuery& SelectQuery::leftJoin(const QString _table, const clsCondition& _on) { return this->join(enuJoinType::LEFT, _table, _on); }
SelectQuery& SelectQuery::rightJoin(const QString _table, const clsCondition& _on) { return this->join(enuJoinType::RIGHT, _table, _on); }
SelectQuery& SelectQuery::innerJoin(const QString _table, const clsCondition& _on) { return this->join(enuJoinType::INNER, _table, _on); }
SelectQuery& SelectQuery::crossJoin(const QString _table) { return this->join(enuJoinType::CROSS, _table, {}); }

/***********************\
|* Where               *|
\***********************/
SelectQuery& SelectQuery::where(const clsCondition& _condition)
{
    this->Data->WhereClauses.append({ true, _condition });
    return *this;
}
SelectQuery& SelectQuery::andWhere(const clsCondition& _condition)
{
    this->Data->WhereClauses.append({ true, _condition });
    return *this;
}
SelectQuery& SelectQuery::orWhere(const clsCondition& _condition)
{
    this->Data->WhereClauses.append({ false, _condition });
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
    this->Data->HavingClauses.append({ true, _condition });
    return *this;
}
SelectQuery& SelectQuery::andHaving(const clsCondition& _condition)
{
    this->Data->HavingClauses.append({ true, _condition });
    return *this;
}
SelectQuery& SelectQuery::orHaving(const clsCondition& _condition)
{
    this->Data->HavingClauses.append({ false, _condition });
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
        WhereParts.append(this->Data->QueryItems.Where.join(" "));

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
        QueryParts.append(this->Data->QueryItems.Having.join(" "));
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

quint64 SelectQuery::count(QVariantMap _args)
{
    QString QueryString = this->buildQueryString(_args, false, true);

    //execute
    quint64 count = 0;

    return count;
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

/***************************************************************************************/

}
}
}

Q_DECLARE_METATYPE(Targoman::API::ORM::stuColSpecs::unnAggregation);
