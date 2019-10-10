/******************************************************************************
 * TargomanAAA: Authentication, Authorization, Accounting framework           *
 *                                                                            *
 * Copyright 2014-2019 by Targoman Intelligent Processing <http://tip.co.ir>  *
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
#include "intfTable.h"
#include "QHttp/HTTPExceptions.h"
#include "QHttp/GenericTypes.h"
#include <QRegularExpression>

namespace Targoman {
namespace API {
namespace Helpers {
namespace ORM {

using namespace QHttp;

static QString QUERY_SEPARATOR = "\n";

QHash<QString, intfTable*> intfTable::Registry;
static bool TypesRegistered = false;

intfTable::intfTable(const QString& _schema,
                     const QString& _name,
                     const QString& _prefix,
                     const QList<Targoman::API::Helpers::ORM::stuColumn>& _cols,
                     const QList<stuRelation>& _foreignKeys) :
    Schema(_schema),
    Name(_name),
    Prefix(_prefix),
    Cols(_cols),
    ForeignKeys(_foreignKeys)
{
    intfTable::Registry.insert(Schema + "." + Name, this);
    if(TypesRegistered)
        return;

    QHTTP_VALIDATION_REQUIRED_TYPE_IMPL(COMPLEXITY_String, Targoman::API::Cols_t,    QFieldValidator::allwaysValid(), _value);
    QHTTP_VALIDATION_REQUIRED_TYPE_IMPL(COMPLEXITY_String, Targoman::API::Filter_t,  QFieldValidator::allwaysValid(), _value);
    QHTTP_VALIDATION_REQUIRED_TYPE_IMPL(COMPLEXITY_String, Targoman::API::OrderBy_t, QFieldValidator::allwaysValid(), _value);
    QHTTP_VALIDATION_REQUIRED_TYPE_IMPL(COMPLEXITY_String, Targoman::API::GroupBy_t, QFieldValidator::allwaysValid(), _value);
    TypesRegistered = true;
}

QString intfTable::finalColName(const stuColumn& _col, const QString& _prefix) const{
    return _prefix + (_col.RenameAs.isEmpty() ? _col.Name : _col.RenameAs);
}

QString intfTable::makeColRenamedAs(const stuColumn& _col, const QString& _prefix) const {
    return (_col.RenameAs.isEmpty() && _prefix.isEmpty() ? "" : " AS `"+ this->finalColName(_col, _prefix) + "`");
};

QString intfTable::makeColName(const stuColumn& _col, const stuRelation& _relation) const {
    return  (_relation.Column.isEmpty() ?
                 this->Name :
                 (_relation.RenamingPrefix.isEmpty() ?
                      _relation.ReferenceTable :
                      _relation.RenamingPrefix
                      )
                 ) + "." + _col.Name + this->makeColRenamedAs(_col, _relation.RenamingPrefix);
};

QVariant intfTable::selectFromTable(DBManager::clsDAC& _db,
                                    const QStringList& _extraJoins,
                                    const QString& _extraFilters,
                                    QString _extraPath,
                                    quint64 _offset,
                                    quint16 _limit,
                                    const QString& _cols,
                                    const QString& _filters,
                                    const QString& _orderBy,
                                    const QString& _groupBy,
                                    bool _reportCount) const
{
    stuSelectItems SelectItems = this->makeListingQuery(_cols, _extraJoins, _filters + "," + _extraFilters, _orderBy, _groupBy);
    if(_extraPath.isEmpty()){
        QHttp::stuTable Table;
        Table.Rows = _db.execQuery("",
                                   QString("SELECT ")
                                   + (_reportCount ? "SQL_CALC_FOUND_ROWS" : "")
                                   + QUERY_SEPARATOR
                                   + SelectItems.Cols.join("," + QUERY_SEPARATOR)
                                   + QUERY_SEPARATOR
                                   + "FROM "
                                   + QUERY_SEPARATOR
                                   + SelectItems.From.join(QUERY_SEPARATOR)
                                   + QUERY_SEPARATOR
                                   + "WHERE "
                                   + SelectItems.Where.join(QUERY_SEPARATOR)
                                   + QUERY_SEPARATOR
                                   + (SelectItems.GroupBy.size() ? "GROUP BY " : "")
                                   + SelectItems.GroupBy.join(',')
                                   + QUERY_SEPARATOR
                                   + (SelectItems.GroupBy.size() ? "ORDER BY " : "")
                                   + SelectItems.OrderBy.join(',')
                                   + QUERY_SEPARATOR
                                   + QString("LIMIT %1,%2").arg(_offset).arg(_limit)
                                   ).toJson(false).toVariant().toList();
        if(_reportCount)
            Table.TotalRows = static_cast<qint64>(_db.execQuery("","SELECT FOUND_ROWS() AS cnt").toJson(true).object().value("cnt").toDouble());
        return Table.toVariant();
    }else{
        QStringList PrimaryKeyQueries = _extraPath.split(",");
        QStringList PrimaryKeyCriterias;
        quint8 Offset = 0;
        foreach(auto Query, PrimaryKeyQueries){
            for(quint8 i=Offset;i<this->Cols.size(); ++i)
                if(this->Cols.at(i).PrimaryKey){
                    PrimaryKeyCriterias.append(this->finalColName(this->Cols.at(i)) + " = \"" + Query + "\"");
                    Offset = i;
                }
        }

        QJsonDocument Result = _db.execQuery("",
                             QString("SELECT ")
                             + QUERY_SEPARATOR
                             + SelectItems.Cols.join(',' + QUERY_SEPARATOR)
                             + QUERY_SEPARATOR
                             + "FROM "
                             + QUERY_SEPARATOR
                             + SelectItems.From.join(QUERY_SEPARATOR)
                             + QUERY_SEPARATOR
                             + "WHERE ("
                             + PrimaryKeyCriterias.join(" AND ")
                             + ") AND "
                             + QUERY_SEPARATOR
                             + SelectItems.Where.join(QUERY_SEPARATOR)
                             + QUERY_SEPARATOR
                             + "LIMIT 2" //Limit is set to 2 in roder to produce error if multi values are selected instead of one
                             ).toJson(false);
        if(Result.isEmpty())
            throw exHTTPBadRequest("No item could be found");

        return Result.toVariant();
    }
}

intfTable::stuSelectItems intfTable::makeListingQuery(const QString& _requiredCols, const QStringList& _extraJoins, const QString _filters, const QString& _orderBy, const QString _groupBy) const
{
    if(_requiredCols != "*")
        QFV.asciiAlNum(false, ",").validate(_requiredCols, "cols");

    QFV.optional(QFV.asciiAlNum(false, ",\\+\\-")).validate(_orderBy, "orderBy");
    QFV.optional(QFV.asciiAlNum(false, ",")).validate(_groupBy, "groupBy");

    intfTable::stuSelectItems SelectItems;

    /****************************************************************************/
    QStringList RequiredCols = _requiredCols.split(",", QString::SkipEmptyParts);
    QStringList SortableCols, FilterableCols;

    foreach(auto Col, this->Cols){
        if(RequiredCols.contains("*"))
            SelectItems.Cols.append(this->makeColName(Col));
        else{
            if(RequiredCols.contains(Col.Name) || RequiredCols.contains(Col.RenameAs))
                SelectItems.Cols.append(this->makeColName(Col));
        }
        if(Col.Sortable) SortableCols.append(this->finalColName(Col));
        if(Col.Filterable) FilterableCols.append(this->finalColName(Col));
    }

    QList<stuRelation> UsedJoins;
    foreach(auto Relation, this->ForeignKeys){
        intfTable* ForeignTable = this->Registry[Relation.ReferenceTable];
        if(ForeignTable == nullptr)
            throw exHTTPInternalServerError("Reference table has not been registered: " + Relation.ReferenceTable);

        bool Joined = false;
        foreach(auto Col, ForeignTable->Cols){
            if(RequiredCols.contains("*")){
                SelectItems.Cols.append(this->makeColName(Col, Relation));
                Joined = true;
            }else{
                if(RequiredCols.contains(Relation.RenamingPrefix + Col.Name) || RequiredCols.contains(Relation.RenamingPrefix + Col.RenameAs)){
                    SelectItems.Cols.append(this->makeColName(Col, Relation));
                    Joined = true;
                }
            }
            if(Col.Sortable) SortableCols.append(this->finalColName(Col, Relation.RenamingPrefix));
            if(Col.Filterable) FilterableCols.append(this->finalColName(Col, Relation.RenamingPrefix));
        }

        if(Joined)
            UsedJoins.append(Relation);
    }

    if(SelectItems.Cols.isEmpty())
        throw exHTTPBadRequest("No columns found to be reported");

    if(RequiredCols.size() && RequiredCols.size() > SelectItems.Cols.size())
        throw exHTTPBadRequest("Seems that some columns could not be resolved: Actives are: [" +SelectItems.Cols.join(", ")+ "]");

    /****************************************************************************/

    SelectItems.From.append(this->Schema + "." + this->Name);
    foreach(auto Join, UsedJoins){
        SelectItems.From.append((Join.LeftJoin ? "LEFT JOIN " : "JOIN ")
                                + Join.ReferenceTable
                                + " "
                                + Join.RenamingPrefix
                                + " ON "
                                + (Join.RenamingPrefix.size() ? Join.RenamingPrefix : Join.ReferenceTable) + "." + Join.ForeignColumn
                                + " = "
                                + this->Name + "." + Join.Column
                                );
    }

    SelectItems.From.append(_extraJoins);

    /****************************************************************************/
    quint8 OpenParenthesis = 0;
    bool CanStartWithLogical = false;
    foreach(auto Filter, _filters.split(",", QString::SkipEmptyParts)){
        QString Rule;
        Filter = Filter.trimmed ();
        if(Filter == ")"){
            if(OpenParenthesis == 0) throw exHTTPBadRequest("Invalid close parenthesis without any open");
            Rule = " )";
            OpenParenthesis--;
            CanStartWithLogical = true;
            continue;
        }else if(Filter.endsWith("(")){
            if(Filter != '('){
                if(CanStartWithLogical == false) throw exHTTPBadRequest("Invalid logical expression prior to any rule");
                if(Filter.startsWith('+')) Rule = "AND ";
                else if(Filter.startsWith('|')) Rule = "OR ";
                else if(Filter.startsWith('*')) Rule = "XOR ";
                else throw exHTTPBadRequest("Invalid expression before parenthesis");

                Rule +="(";
                CanStartWithLogical = false;
                OpenParenthesis++;
            }
        }

        static QRegularExpression rxFilterPattern("([\\+\\|\\*]?)([a-zA-Z0-9\\_]+)([<>!=~]=?)(.+)");
        Filter = Filter.replace("$COMMA$", ",");
        QRegularExpressionMatch PatternMatches = rxFilterPattern.match(Filter);
        if(PatternMatches.lastCapturedIndex() != 4)
            throw exHTTPBadRequest("Invalid filter set: " + Filter);

        if(PatternMatches.captured(1).length() && CanStartWithLogical == false)
            throw exHTTPBadRequest("Invalid AND/OR/XOR: " + Filter);

        if(PatternMatches.captured(1) == '+') Rule = " AND ";
        else if(PatternMatches.captured(1) == '|') Rule = " OR ";
        else if(PatternMatches.captured(1) == '*') Rule = " XOR ";

        if(FilterableCols.contains(PatternMatches.captured(2).trimmed()))
            Rule+=PatternMatches.captured(2);
        else
            throw exHTTPBadRequest("Invalid column for filtring: " + PatternMatches.captured(2));

        if(PatternMatches.captured(4) == "NULL"){
            if(PatternMatches.captured(3) == "=")
                Rule = " IS NULL";
            else if(PatternMatches.captured(3) == "!=")
                Rule = " IS NOT NULL";
            else
                throw exHTTPBadRequest("Invalid filter with NULL expression: " + Filter);
            continue;
        }

        if(PatternMatches.captured(3) == "<") Rule += " < ";
        else if(PatternMatches.captured(3) == "<=") Rule += " <= ";
        else if(PatternMatches.captured(3) == ">") Rule += " > ";
        else if(PatternMatches.captured(3) == ">=") Rule += " >= ";
        else if(PatternMatches.captured(3) == "!=") Rule += " != ";
        else if(PatternMatches.captured(3) == "~=") Rule += " LIKE ";
        else if(PatternMatches.captured(3) == "=") Rule += " = ";
        else throw exHTTPBadRequest("Invalid filter criteria: " + Filter);

        Rule += PatternMatches.captured(4);
        CanStartWithLogical = true;
        SelectItems.Where.append(Rule);
    }

    if(OpenParenthesis != 0)
        throw exHTTPBadRequest("count of open and closed parenthesises does not match");

    if(SelectItems.Where.isEmpty())
        SelectItems.Where.append("TRUE");

    /****************************************************************************/
    foreach(auto OrderByCriteria, _orderBy.split(",", QString::SkipEmptyParts)) {
        QString Direction = "ASC";
        if(OrderByCriteria.startsWith("-")){
            Direction = "DESC";
            OrderByCriteria = OrderByCriteria.mid(1);
        }else if(OrderByCriteria.startsWith("+"))
            OrderByCriteria = OrderByCriteria.mid(1);

        bool Found = false;
        foreach(auto Col, SortableCols)
            if(Col == OrderByCriteria){
                Found = true;
                break;
            }
        if(Found == false)
            throw exHTTPBadRequest("Invalid Order by <"+OrderByCriteria+"> not found in columns");
        SelectItems.OrderBy.append(OrderByCriteria + " " + Direction);
    }

    /****************************************************************************/
    foreach(auto GroupBy, _groupBy.split(",", QString::SkipEmptyParts)) {
        bool Found = false;
        foreach(auto Col, SortableCols)
            if(Col == GroupBy){
                Found = true;
                break;
            }
        if(Found == false)
            throw exHTTPBadRequest("Invalid group by <"+GroupBy+"> not found in columns");
        SelectItems.GroupBy.append(GroupBy);
    }

    return SelectItems;
}


}
}
}
}
