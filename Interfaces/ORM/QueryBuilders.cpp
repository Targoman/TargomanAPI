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

/***********************************************************************/
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
            throw exQueryBuilder(QString("Invalid condition Aggregator character: ") + _aggregator);
    }

    if (_aggregator != ')' && _nextCondition.isEmpty())
        throw exQueryBuilder("aggregator must follow a valid condition");

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

/***********************************************************************/
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

/***********************************************************************/
class clsSelectQueryData : public QSharedData
{
public:
    clsSelectQueryData(const clsTable& _table) :
        Table(_table)
    {}

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

        if (this->RequiredCols.isEmpty())
            foreach(auto Col, this->Table.BaseCols)
                this->QueryItems.Cols.append(makeColName(this->Table.Name, Col, true));
        else
            foreach(auto RequiredCol, this->RequiredCols)
                addCol(RequiredCol);

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
    }

    clsORMField colByName(const QString& _col) {
        return this->Table.SelectableColsMap[_col];
    }

public:
    const clsTable&         Table;
    QList<stuColSpecs>      RequiredCols;
    QStringList             GroupByCols;
    stuSelectItems          QueryItems;
    QList<stuQueryRelation> Relations;
};

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
bool clsCondition::isEmpty() const { return this->Data->Col.size(); }

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
    throw Common::exTargomanMustBeImplemented(__FUNCTION__);
}
SelectQuery& SelectQuery::andWhere(const clsCondition& _condition)
{
    throw Common::exTargomanMustBeImplemented(__FUNCTION__);
}
SelectQuery& SelectQuery::orWhere(const clsCondition& _condition)
{
    throw Common::exTargomanMustBeImplemented(__FUNCTION__);
}

/***********************\
|* Order               *|
\***********************/
SelectQuery& SelectQuery::orderBy(const QString& _col, enuOrderDir::Type _dir)
{
    throw Common::exTargomanMustBeImplemented(__FUNCTION__);
}

/***********************\
|* Group               *|
\***********************/
//SelectQuery& SelectQuery::groupBy(const clsCondition& _condition);
SelectQuery& SelectQuery::groupBy(const QString& _col)
{
    throw Common::exTargomanMustBeImplemented(__FUNCTION__);
}
SelectQuery& SelectQuery::groupBy(const QStringList& _cols)
{
    throw Common::exTargomanMustBeImplemented(__FUNCTION__);
}

/***********************\
|* Having              *|
\***********************/
SelectQuery& SelectQuery::having(const clsCondition& _condition)
{
    throw Common::exTargomanMustBeImplemented(__FUNCTION__);
}

/***********************\
|* Other               *|
\***********************/
//used by APPLY_GET_METHOD_CALL_ARGS_TO_QUERY
SelectQuery& SelectQuery::pksByPath(TAPI::PKsByPath_t _pksByPath)
{
    throw Common::exTargomanMustBeImplemented(__FUNCTION__);
}

//used by APPLY_GET_METHOD_CALL_ARGS_TO_QUERY
SelectQuery& SelectQuery::offset(quint64 _offset)
{
    throw Common::exTargomanMustBeImplemented(__FUNCTION__);
}

//used by APPLY_GET_METHOD_CALL_ARGS_TO_QUERY
SelectQuery& SelectQuery::limit(quint16 _limit)
{
    throw Common::exTargomanMustBeImplemented(__FUNCTION__);
}

/***********************\
|* Execute             *|
\***********************/
QVariantMap SelectQuery::one(QVariantMap _args)
{
    this->Data->prepare();

    QVariantMap Result;
    return Result;
}

TAPI::stuTable SelectQuery::all(QVariantMap _args, quint16 _maxCount, quint64 _from)
{
    this->Data->prepare();

    TAPI::stuTable Result;
    return Result;
}

/***************************************************************************************/
}
}
}

Q_DECLARE_METATYPE(Targoman::API::ORM::stuColSpecs::unnAggregation);
