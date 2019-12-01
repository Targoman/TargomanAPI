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
#include "clsTable.h"
#include "QHttp/HTTPExceptions.h"
#include "QHttp/GenericTypes.h"
#include <QRegularExpression>
#include "Helpers/AAA/clsJWT.hpp"

namespace Targoman {
namespace API {
namespace Helpers {
namespace ORM {

using namespace QHttp;
using namespace Targoman::DBManager;
using namespace AAA;

QHash<QString, clsTable*> clsTable::Registry;
static bool TypesRegistered = false;

clsTable::clsTable(const QString& _schema,
                   const QString& _name,
                   const QList<QHttp::stuORMField>& _cols,
                   const QList<stuRelation>& _foreignKeys) :
    Schema(_schema),
    Name(_name),
    ForeignKeys(_foreignKeys),
    CountOfPKs(0)
{
    qint8 ColIndex = 0;

    foreach(auto Col, _cols){
        if(Col.PKIndex == 0){
            this->CountOfPKs++;
            Col.PKIndex = ++ColIndex;
        }
        this->Cols.insert(Col.Name, Col);
        this->Filters.append(Col);
    }
    this->Filters.append(QHttp::stuORMField(COLS_KEY, T(Targoman::API::Cols_t), QFV, true));
    clsTable::Registry.insert(Schema + "." + Name, this);
    if(TypesRegistered)
        return;

    QHTTP_VALIDATION_REQUIRED_TYPE_IMPL(COMPLEXITY_String, Targoman::API::Cols_t,    QFieldValidator::allwaysValid(), _value);
    QHTTP_VALIDATION_REQUIRED_TYPE_IMPL(COMPLEXITY_String, Targoman::API::Filter_t,  QFieldValidator::allwaysValid(), _value);
    QHTTP_VALIDATION_REQUIRED_TYPE_IMPL(COMPLEXITY_String, Targoman::API::OrderBy_t, QFieldValidator::allwaysValid(), _value);
    QHTTP_VALIDATION_REQUIRED_TYPE_IMPL(COMPLEXITY_String, Targoman::API::GroupBy_t, QFieldValidator::allwaysValid(), _value);
    TypesRegistered = true;
}

void clsTable::updateFilterParamType(const QString& _fieldTypeName, int _typeID)
{
    for(auto ColIter = this->Cols.begin(); ColIter != this->Cols.end(); ++ColIter)
        if(ColIter.value().ParamTypeName == _fieldTypeName)
            ColIter.value().ParameterType = _typeID;
    for(auto ColIter = this->Filters.begin(); ColIter != this->Filters.end(); ++ColIter)
        if(ColIter->ParamTypeName == _fieldTypeName)
            ColIter->ParameterType = _typeID;
}

QString clsTable::finalColName(const QHttp::stuORMField& _col, const QString& _prefix) const{
    return _prefix + (_col.RenameAs.isEmpty() ? _col.Name : _col.RenameAs);
}

QString clsTable::makeColRenamedAs(const QHttp::stuORMField& _col, const QString& _prefix) const {
    return (_col.RenameAs.isEmpty() && _prefix.isEmpty() ? "" : " AS `"+ this->finalColName(_col, _prefix) + "`");
};

QString clsTable::makeColName(const QHttp::stuORMField& _col, const stuRelation& _relation) const {
    return  (_relation.Column.isEmpty() ?
                 this->Name :
                 (_relation.RenamingPrefix.isEmpty() ?
                      _relation.ReferenceTable :
                      _relation.RenamingPrefix
                      )
                 ) + "." + _col.Name + this->makeColRenamedAs(_col, _relation.RenamingPrefix);
};

QVariant clsTable::selectFromTable(DBManager::clsDAC& _db,
                                   const QStringList& _extraJoins,
                                   const QString& _extraFilters,
                                   const QHttp::ExtraPath_t& _extraPath,
                                   const QHttp::DirectFilters_t& _directFilters,
                                   quint64 _offset,
                                   quint16 _limit,
                                   QString _cols,
                                   const QString& _filters,
                                   const QString& _orderBy,
                                   const QString& _groupBy,
                                   bool _reportCount) const
{
    if(_directFilters.contains(COLS_KEY))
        _cols = _directFilters.value(COLS_KEY).toString();

    stuSelectItems SelectItems = this->makeListingQuery(_cols, _extraJoins, _filters + " " + _extraFilters, _orderBy, _groupBy);
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
                                   + (SelectItems.OrderBy.size() ? "ORDER BY " : "")
                                   + SelectItems.OrderBy.join(',')
                                   + QUERY_SEPARATOR
                                   + QString("LIMIT %1,%2").arg(_offset).arg(_limit)
                                   ).toJson(false).toVariant().toList();
        if(_reportCount)
            Table.TotalRows = static_cast<qint64>(_db.execQuery("","SELECT FOUND_ROWS() AS cnt").toJson(true).object().value("cnt").toDouble());
        return Table.toVariant();
    }else{
        QStringList PrimaryKeyQueries = _extraPath.split(",");
        QStringList Filters;
        quint8 PKIndex = 1;
        foreach(auto Query, PrimaryKeyQueries){
            QList<QHttp::stuORMField> ColValues = this->Cols.values();
            foreach(auto Col, this->Cols)
                if(Col.PKIndex == PKIndex){
                    if(Query.size())
                        Filters.append(this->finalColName(Col) + " = \"" + Query + "\"");
                    break;
                }
            PKIndex++;
        }
        for(auto FilterIter = _directFilters.begin(); FilterIter != _directFilters.end(); ++FilterIter){
            if(FilterIter.key() == COLS_KEY) continue;
            const QHttp::stuORMField& Col = this->Cols.value(FilterIter.key());
            if(Col.Filterable)
                Filters.append(FilterIter.key() + " = \"" + FilterIter.value().toString() + "\"");
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
                                             + "WHERE "
                                             + Filters.join(" AND ")
                                             + QUERY_SEPARATOR
                                             + "LIMIT 2" //Limit is set to 2 in roder to produce error if multi values are selected instead of one
                                             ).toJson(true);
        if(Result.isEmpty())
            throw exHTTPBadRequest("No item could be found");

        return Result.toVariant();
    }
}

bool clsTable::update(DBManager::clsDAC& _db, QVariantMap _primaryKeys, QVariantMap _updateInfo)
{
    QStringList  UpdateCommands;
    QVariantList Values;
    for(auto InfoIter = _updateInfo.begin(); InfoIter != _updateInfo.end(); ++InfoIter){
        if(InfoIter->isValid() == false)
            continue;
        const QHttp::stuORMField& Column = this->Cols.value(InfoIter.key());
        if(Column.IsReadOnly)
            throw exHTTPInternalServerError("Invalid change to read-only column <" + InfoIter.key() + ">");

        this->Cols.value(InfoIter.key()).validate(InfoIter.value());
        UpdateCommands.append(Column.Name + "=?");
        Values.append(InfoIter.value());
    }

    QStringList  PKs;
    for(auto PKIter = _primaryKeys.begin(); PKIter != _primaryKeys.end(); ++PKIter){
        if(PKIter->isValid() == false)
            continue;
        const QHttp::stuORMField& Column = this->Cols[PKIter.key()];
        if(Column.PKIndex <= 0)
            throw exHTTPInternalServerError("Column <"+ PKIter.key() +"> is not primary key");
        PKs.append(Column.Name + "=?");
        Values.append(PKIter.value());
    }

    clsDACResult Result = _db.execQuery("",
                                        QString("UPDATE ") + this->Name
                                        + QUERY_SEPARATOR
                                        + "SET "
                                        + UpdateCommands.join("," + QUERY_SEPARATOR)
                                        + QUERY_SEPARATOR
                                        + "WHERE "
                                        + QUERY_SEPARATOR
                                        + PKs.join(" AND " + QUERY_SEPARATOR)
                                        ,Values);

    return Result.numRowsAffected() > 0;
}

QVariant clsTable::create(clsDAC& _db, QVariantMap _createInfo)
{
    QStringList  CreateCommands;
    QVariantList Values;
    for(auto InfoIter = _createInfo.begin(); InfoIter != _createInfo.end(); ++InfoIter){
        if(InfoIter->isValid() == false)
            continue;
        const QHttp::stuORMField& Column = this->Cols[InfoIter.key()];
        if(Column.IsReadOnly)
            throw exHTTPInternalServerError("Invalid change to read-only column <" + InfoIter.key() + ">");
        this->Cols[InfoIter.key()].validate(InfoIter.value());
        CreateCommands.append(Column.Name + "=?");
        Values.append(InfoIter.value());
    }

    clsDACResult Result = _db.execQuery("",
                                        QString("INSERT INTO ") + this->Name
                                        + QUERY_SEPARATOR
                                        + "SET "
                                        + CreateCommands.join("," + QUERY_SEPARATOR)
                                        + QUERY_SEPARATOR
                                        ,Values);

    return Result.lastInsertId();
}

bool clsTable::deleteByPKs(clsDAC& _db, QVariantMap _primaryKeys)
{
    if(this->update(_db, _primaryKeys, {{this->Cols.last().Name, "Removed"}}) == false)
        return false;

    QVariantList Values;
    QStringList  PKs;
    for(auto PKIter = _primaryKeys.begin(); PKIter != _primaryKeys.end(); ++PKIter){
        if(PKIter->isValid() == false)
            continue;
        const QHttp::stuORMField& Column = this->Cols[PKIter.key()];
        if(Column.PKIndex < 0)
            throw exHTTPInternalServerError("Column <"+ PKIter.key() +"> is not primary key");
        PKs.append(Column.Name + "=?");
        Values.append(PKIter.value());
    }

    clsDACResult Result = _db.execQuery("",
                                        QString("DELETE FROM ") + this->Name
                                        + QUERY_SEPARATOR
                                        + "WHERE "
                                        + QUERY_SEPARATOR
                                        + PKs.join(" AND " + QUERY_SEPARATOR)
                                        ,Values);

    return Result.numRowsAffected() > 0;
}

bool clsTable::isSelfGet(const QVariantMap& _requiredFilters, ExtraPath_t _EXTRAPATH, DirectFilters_t _DIRECTFILTERS, Targoman::API::Filter_t _filters)
{
    for(auto FilterIter = _requiredFilters.begin(); FilterIter != _requiredFilters.end(); ++FilterIter)
        if((_EXTRAPATH.size() && _DIRECTFILTERS.value(FilterIter.key(), 0) != FilterIter.value())
           || (_EXTRAPATH.isEmpty() && _filters.contains(QString("%1=%2").arg(FilterIter.key()).arg(FilterIter.value().toString())) == false)
           )
            return false;
    return true;
}

clsTable::stuSelectItems clsTable::makeListingQuery(const QString& _requiredCols, const QStringList& _extraJoins, QString _filters, const QString& _orderBy, const QString _groupBy) const
{
    if(_requiredCols.size())
        QFV.asciiAlNum(false, ",").validate(_requiredCols, "cols");

    QFV.optional(QFV.asciiAlNum(false, ",\\+\\-")).validate(_orderBy, "orderBy");
    QFV.optional(QFV.asciiAlNum(false, ",")).validate(_groupBy, "groupBy");

    clsTable::stuSelectItems SelectItems;

    /****************************************************************************/
    QStringList RequiredCols = _requiredCols.size() ? _requiredCols.split(",", QString::SkipEmptyParts) : QStringList("*");
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
        clsTable* ForeignTable = this->Registry[Relation.ReferenceTable];
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
                                + " `"
                                + Join.RenamingPrefix
                                + "` ON `"
                                + (Join.RenamingPrefix.size() ? Join.RenamingPrefix : Join.ReferenceTable) + "`." + Join.ForeignColumn
                                + " = "
                                + this->Name + "." + Join.Column
                                );
    }

    if(_extraJoins.size())
        SelectItems.From.append(_extraJoins);

    /****************************************************************************/
    quint8 OpenParenthesis = 0;
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

            if(FilterableCols.contains(PatternMatches.captured(1).trimmed()))
                Rule+=PatternMatches.captured(1);
            else
                throw exHTTPBadRequest("Invalid column for filtring: " + PatternMatches.captured(1));

            if(PatternMatches.captured(3) == "NULL"){
                if(PatternMatches.captured(2) == "=")
                    Rule = " IS NULL";
                else if(PatternMatches.captured(2) == "!=")
                    Rule = " IS NOT NULL";
                else
                    throw exHTTPBadRequest("Invalid filter with NULL expression: " + Filter);
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

            Rule += PatternMatches.captured(3);
            CanStartWithLogical = true;
            LastLogical.clear();
        }
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
