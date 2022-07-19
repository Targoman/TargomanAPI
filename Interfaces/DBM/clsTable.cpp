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

#include <QSet>
#include <QRegularExpressionMatch>
#include "clsTable.h"
#include <QRegularExpression>
#include "libQFieldValidator/QFieldValidator.h"
#include "Interfaces/Common/HTTPExceptions.hpp"
#include "Interfaces/Common/GenericTypes.h"
#include "Interfaces/AAA/AAADefs.hpp"
//#include "libTargomanCommon/Configuration/intfConfigurableModule.hpp"
#include "Interfaces/API/intfPureModule.h"
#include "Interfaces/DBM/QueryBuilders.h"

using namespace Targoman::DBManager;
using namespace Targoman::API;

bool operator == (const Targoman::API::DBM::stuRelation& _first, const Targoman::API::DBM::stuRelation& _second) {
    return _first.ReferenceTable == _second.ReferenceTable && _first.Column == _second.Column && _first.ForeignColumn == _second.ForeignColumn;
}

namespace Targoman::API::DBM {

QString COLS_KEY = "cols";

uint qHash(const Targoman::API::DBM::stuRelation& _relation) {
    return qHash(_relation.Column + _relation.ForeignColumn + _relation.ReferenceTable + _relation.RenamingPrefix);
}

QHash<QString, clsTable*> clsTable::Registry;

clsTable::clsTable(const QString& _domain,
                   const QString& _schema,
                   const QString& _name,
                   const QList<clsORMField>& _cols,
                   const QList<stuRelation>& _relations,
                   const QList<stuDBIndex>& _indexes,
                   const QVariantMap& _dbProperties) :
    BaseCols(_cols),
    Schema(_schema),
    Name(_name),
    Domain(_domain),
    Relations(_relations),
    Indexes(_indexes),
    DBProperties(_dbProperties),
    CountOfPKs(0) {
    foreach (auto Col, this->BaseCols) {
        if (Col.name() == ORM_INVALIDATED_AT_FIELD_NAME)
            this->addDBProperty(
                ORM_TABLE_DBPROPERTY_INVALIDATE_AT_FIELD_NAME,
                Col.name());
        else if (Col.updatableBy() == enuUpdatableBy::__STATUS__)
            this->addDBProperty(
                ORM_TABLE_DBPROPERTY_STATUS_FIELD_NAME,
                Col.name());
    }

                                   ///name                      type                 extraValidator defaultValue updatableBy isSortable isFilterable isSelfIdentifier isVirtual isPrimaryKey isSelectable renameAs
    this->BaseCols.append(clsORMField(CURRENT_TIMESTAMP + "()", S(TAPI::DateTime_t), QFV,           QNull,       UPNone,     false,     false,       false,           true,     false,       true,        CURRENT_TIMESTAMP));

//    TargomanDebug(5).noLabel() << "register clsTable derived:" << Schema + "." + Name;

    clsTable::Registry.insert(Schema + "." + Name, this);
}

clsTable::clsTable(
    const QString& _schema,
    const QString& _name,
    const QList<clsORMField>& _cols,
    const QList<stuRelation>& _relations,
    const QList<stuDBIndex>& _indexes,
    const QVariantMap& _dbProperties
) :
    clsTable(
        "",
        _schema,
        _name,
        _cols,
        _relations,
        _indexes,
        _dbProperties
) { ; }

clsTable* clsTable::addDBProperty(const QString& _key, const QVariant& _value) {
    this->DBProperties.insert(_key, _value);
    return this;
}
const QVariant clsTable::getDBProperty(const QString& _key) {
    if (this->DBProperties.contains(_key))
        return this->DBProperties[_key];

    return QVariant();
}

QList<clsORMField> clsTable::filterItems(THttpMethod _method) {
    QList<clsORMField> Filters;
    switch (_method) {
    case qhttp::EHTTP_GET:
    case qhttp::EHTTP_DELETE:
        this->prepareFiltersList();
        foreach (auto Filter, this->BaseCols)
            if (Filter.isPrimaryKey())
                Filters.append(Filter);
        if (_method == qhttp::EHTTP_GET)
            Filters.append(clsORMField(COLS_KEY, S(TAPI::Cols_t), QFV, ORM_SELF_VIRTUAL));
        break;
    case qhttp::EHTTP_PATCH:
        foreach (auto Filter, this->BaseCols)
            if (Filter.updatableBy() != enuUpdatableBy::__CREATOR__
               && Filter.updatableBy() != enuUpdatableBy::__UPDATER__
               && (Filter.isPrimaryKey() || Filter.isReadOnly() == false))
                Filters.append(Filter);
        break;
    case qhttp::EHTTP_PUT:
        foreach (auto Filter, this->BaseCols)
            if (Filter.updatableBy() != enuUpdatableBy::__CREATOR__
               && Filter.updatableBy() != enuUpdatableBy::__UPDATER__
               && Filter.defaultValue() != QInvalid
               && Filter.defaultValue() != QAuto
               )
                Filters.append(Filter);
        break;
    default:
//          Filters = this->AllCols;
        foreach (stuRelatedORMField baseCol, this->AllCols)
            Filters.append(baseCol.Col);
        break;
    }
    return Filters;
}

void clsTable::updateFilterParamType(const QString& _fieldTypeName, QMetaType::Type _typeID) {
    foreach (auto Col, this->BaseCols) {
        if (Col.paramTypeName() == _fieldTypeName)
            Col.updateTypeID(_typeID);
    }
}

void clsTable::prepareFiltersList() {
    if (this->AllCols.size())
        return;

//    TargomanDebug(5).noLabel() << "prepareFiltersList:" << this->Schema + "." + this->Name;

    foreach (clsORMField Col, this->BaseCols) {
        if (Col.isPrimaryKey())
            ++this->CountOfPKs;

        QString FinalColName = this->finalColName(Col);
        clsORMField NewCol = clsORMField(Col, FinalColName);

        stuRelatedORMField relatedORMField = stuRelatedORMField(NewCol);

        this->AllCols.append(relatedORMField);

        if (NewCol.masterName().length())
            this->HasMasterNameColsMap.insert(NewCol.masterName(), relatedORMField);

        if (Col.isSelectable())
            this->SelectableColsMap.insert(FinalColName, relatedORMField);

        if (Col.isFilterable())
            this->FilterableColsMap.insert(FinalColName, relatedORMField);

        if (Col.isSortable())
            this->SortableColsMap.insert(FinalColName, relatedORMField);
    }

    QStringList AppliedRelations;
    this->prepareRelationsFiltersList(this->Relations, AppliedRelations);

    foreach (stuRelatedORMField baseCol, this->AllCols) {
        const intfAPIArgManipulator& argSpecs = baseCol.Col.argSpecs();
        if (argSpecs.toORMValueConverter() && !this->Converters.contains(this->finalColName(baseCol.Col)))
            this->Converters.insert(this->finalColName(baseCol.Col), argSpecs.toORMValueConverter());
    }
}

void clsTable::prepareRelationsFiltersList(const QList<stuRelation> &_relations, QStringList &_appliedRelations) {
    if (_relations.isEmpty())
        return;

    foreach (stuRelation Relation, _relations) {
        QStringList parts;
        if (Relation.RenamingPrefix.length())
            parts.append(Relation.RenamingPrefix);
        parts.append(Relation.ReferenceTable);
        QString RelationFullName = parts.join("_");
        if (_appliedRelations.contains(RelationFullName))
            continue;
        _appliedRelations.append(RelationFullName);

        if (clsTable::Registry.contains(Relation.ReferenceTable) == false)
            throw exHTTPInternalServerError("Reference table has not been registered: " + Relation.ReferenceTable + " (Relation defined in: " + this->Name + ")");

        clsTable* ForeignTable = clsTable::Registry[Relation.ReferenceTable];
        //if (ForeignTable == nullptr)

        foreach (clsORMField Col, ForeignTable->BaseCols) {
            QString FinalColName = this->finalColName(Col, Relation.RenamingPrefix);
            clsORMField NewCol = clsORMField(Col, FinalColName);

            stuRelatedORMField relatedORMField = stuRelatedORMField(NewCol, Relation);

            this->AllCols.append(relatedORMField);

            if (NewCol.masterName().length())
                this->HasMasterNameColsMap.insert(NewCol.masterName(), relatedORMField);

            if (Col.isSelectable())
                this->SelectableColsMap.insert(FinalColName, relatedORMField);

            if (Col.isFilterable())
                this->FilterableColsMap.insert(FinalColName, relatedORMField);

            if (Col.isSortable())
                this->SortableColsMap.insert(FinalColName, relatedORMField);
        }

        this->prepareRelationsFiltersList(ForeignTable->Relations, _appliedRelations);
    }
}

const QString clsTable::schema() const
{
    return this->Schema;
}

const QString clsTable::name() const
{
    return this->Name;
}
const QString clsTable::nameWithSchema() const {
    if (this->Schema.isEmpty())
        return this->Name;

    return QString("%1.%2").arg(this->Schema).arg(this->Name);
}

const QStringList clsTable::SelectableColumnNames(QString _tableAlias) const {
    if (_tableAlias.isEmpty() == false)
        _tableAlias += ".";

    QStringList ColNames;

    foreach (clsORMField Col, this->BaseCols) {
        if (Col.isSelectable() == false)
            continue;

        QString FinalColName = this->finalColName(Col, _tableAlias);
        ColNames.append(FinalColName);
    }

    return ColNames;
}

ORMSelectQuery clsTable::GetSelectQuery(INTFAPICALLBOOM_IMPL &APICALLBOOM_PARAM, const QString &_alias, Q_DECL_UNUSED bool _isRoot) {
    return ORMSelectQuery(APICALLBOOM_PARAM, *this, _alias);
}

ORMCreateQuery clsTable::GetCreateQuery(INTFAPICALLBOOM_IMPL &APICALLBOOM_PARAM) {
    return ORMCreateQuery(APICALLBOOM_PARAM, *this);
}

ORMUpdateQuery clsTable::GetUpdateQuery(INTFAPICALLBOOM_IMPL &APICALLBOOM_PARAM, const QString& _alias) {
    return ORMUpdateQuery(APICALLBOOM_PARAM, *this, _alias);
}

ORMDeleteQuery clsTable::GetDeleteQuery(INTFAPICALLBOOM_IMPL &APICALLBOOM_PARAM, const QString& _alias) {
    return ORMDeleteQuery(APICALLBOOM_PARAM, *this, _alias);
}

const QString clsTable::domain() {
    if (this->Domain.length())
        return this->Domain;

//    Targoman::Common::Configuration::intfModule* PModule = dynamic_cast<Targoman::Common::Configuration::intfModule*>(this);
    Targoman::API::API::intfPureModule* PModule = dynamic_cast<Targoman::API::API::intfPureModule*>(this);
    if (PModule != nullptr)
        return (this->Domain = (PModule->parentModuleName().length() ? PModule->parentModuleName() : PModule->moduleBaseName()));

//    return Q_LIKELY(this->Domain.size())
//            ? this->Domain
//            : this->Domain = (this->parentModuleName().size() ? this->parentModuleName() : this->moduleBaseName());

    return "";
}

const QString clsTable::getStatusColumnName() {
    return this->getDBProperty(ORM_TABLE_DBPROPERTY_STATUS_FIELD_NAME).toString();
}

clsDACResult clsTable::callSP(
    INTFAPICALLBOOM_IMPL &APICALLBOOM_PARAM,
    const QString& _spName,
    const QVariantMap& _spArgs,
    const QString& _purpose,
    quint64* _executionTime
) {
    auto ServerTiming = APICALLBOOM_PARAM.createScopeTiming("db", "sp");

    clsDAC DAC(this->domain(), this->Schema);
    return DAC.callSP({}, _spName, _spArgs, _purpose, _executionTime);
}

clsDACResult clsTable::callSPCacheable(
    INTFAPICALLBOOM_IMPL &APICALLBOOM_PARAM,
    quint32 _maxCacheTime,
    const QString& _spName,
    const QVariantMap& _spArgs,
    const QString& _purpose,
    quint64* _executionTime
) {
    auto ServerTiming = APICALLBOOM_PARAM.createScopeTiming("db", "cachable sp");

    clsDAC DAC(this->domain(), this->Schema);
    return DAC.callSPCacheable(_maxCacheTime, {}, _spName, _spArgs, _purpose, _executionTime);
}

clsDACResult clsTable::execQuery(
    INTFAPICALLBOOM_IMPL &APICALLBOOM_PARAM,
    const QString& _queryStr,
    const QVariantList& _params,
    const QString& _purpose,
    quint64* _executionTime
) {
    auto ServerTiming = APICALLBOOM_PARAM.createScopeTiming("db", "exec list");

    clsDAC DAC(this->domain(), this->Schema);
    return DAC.execQuery({}, _queryStr, _params, _purpose, _executionTime);
}

clsDACResult clsTable::execQuery(
    INTFAPICALLBOOM_IMPL &APICALLBOOM_PARAM,
    const QString& _queryStr,
    const QVariantMap& _params,
    const QString& _purpose,
    quint64* _executionTime
) {
    auto ServerTiming = APICALLBOOM_PARAM.createScopeTiming("db", "exec map");

    clsDAC DAC(this->domain(), this->Schema);
    return DAC.execQuery({}, _queryStr, _params, _purpose, _executionTime);
}

clsDACResult clsTable::execQueryCacheable(
    INTFAPICALLBOOM_IMPL &APICALLBOOM_PARAM,
    quint32 _maxCacheTime,
    const QString& _queryStr,
    const QVariantList& _params,
    const QString& _purpose,
    quint64* _executionTime
) {
    auto ServerTiming = APICALLBOOM_PARAM.createScopeTiming("db", "cachable exec list");

    clsDAC DAC(this->domain(), this->Schema);
    return DAC.execQueryCacheable(_maxCacheTime, {}, _queryStr, _params, _purpose, _executionTime);
}

clsDACResult clsTable::execQueryCacheable(
    INTFAPICALLBOOM_IMPL &APICALLBOOM_PARAM,
    quint32 _maxCacheTime,
    const QString& _queryStr,
    const QVariantMap& _params,
    const QString& _purpose,
    quint64* _executionTime
) {
    auto ServerTiming = APICALLBOOM_PARAM.createScopeTiming("db", "cachable exec map");

    clsDAC DAC(this->domain(), this->Schema);
    return DAC.execQueryCacheable(_maxCacheTime, {}, _queryStr, _params, _purpose, _executionTime);
}

//TAPI::Filter_t -> QString
///@TODO: IMPORTANT! deprecated?
void clsTable::setSelfFilters(const QVariantMap& _requiredFilters, QString& _providedFilters) {
    QStringList Filters;

    for (auto FilterIter = _requiredFilters.begin(); FilterIter != _requiredFilters.end(); ++FilterIter)
        Filters.append(QString("%1=%2").arg(FilterIter.key()).arg(FilterIter.value().toString()));

    QString Postfix = _providedFilters.size() ? " )" : "";

    _providedFilters.insert(0, QString("( %1 )%2").arg(Filters.join(" + ")).arg(_providedFilters.size() ? " + ( " : ""));
    _providedFilters += Postfix;
}

//void clsTable::setSelfFilters(const QVariantMap& _requiredFilters, QVariantMap& _extraFilters)
//{
//    for (auto FilterIter = _requiredFilters.begin(); FilterIter != _requiredFilters.end(); ++FilterIter)
//        _extraFilters.insert(FilterIter.key(), FilterIter.value());
//}

QStringList clsTable::privOn(qhttp::THttpMethod _method, QString _moduleName) {
    QString CRUD;
    switch (_method) {
    case qhttp::EHTTP_PUT:      CRUD = "T000";break;
    case qhttp::EHTTP_GET:      CRUD = "0T00";break;
    case qhttp::EHTTP_PATCH:    CRUD = "00T0";break;
    case qhttp::EHTTP_DELETE:   CRUD = "000T";break;
    default:
        Q_ASSERT_X(false, "privOn", "Invalid method on ORM");
    }
    return {_moduleName.replace("/", ":") + ":CRUD~" + CRUD };
}

QString clsTable::finalColName(const clsORMField &_col, const QString &_prefix) {
    return Targoman::API::DBM::finalColName(_col, _prefix);
}
/*
clsTable::stuSelectItems clsTable::makeListingQuery(const QString& _requiredCols, const QStringList& _extraJoins, QString _filters, const QString& _orderBy, const QString _groupBy) const
{
    if (_requiredCols.size())
        QFV.asciiAlNum(false, ",()").validate(_requiredCols, "cols");

    QFV.optional(QFV.asciiAlNum(false, ",\\+\\-")).validate(_orderBy, "orderBy");
    QFV.optional(QFV.asciiAlNum(false, ",")).validate(_groupBy, "groupBy");

    clsTable::stuSelectItems SelectItems;

    //-----------------------------------------------------
    QStringList RequiredCols = _requiredCols.size() ? _requiredCols.split(",", QString::SkipEmptyParts) : QStringList("*");

    auto addCol = [this, _groupBy, RequiredCols](clsTable::stuSelectItems& _selectItems, auto _col, const stuRelation& _relation = InvalidRelation) {
        foreach (auto RequiredCol, RequiredCols) {
            QString ColName;
            QString Function;
            if (RequiredCol.contains('(')) {
                if (RequiredCol.endsWith(')') == false)
                    throw exHTTPBadRequest("Aggroupation rule found without closing parenthesis: " + RequiredCol);
                Function = RequiredCol.split('(').first();
                ColName  = RequiredCol.split('(').last().replace(')', "");
                if (Function != "COUNT"
                   && Function != "COUNT_DISTINCT"
                   && Function != "SUM"
                   && Function != "AVG"
                   && Function != "MAX"
                   && Function != "MIN"
                   )
                    throw exHTTPBadRequest("Invalid agroupation rule: " + RequiredCol);
            } else {
                if (_groupBy.size())
                    Function = "ANY_VALUE";
                ColName = RequiredCol;
            }

            if (ColName == _relation.RenamingPrefix + _col.name () || ColName == _relation.RenamingPrefix + _col.renameAs()) {
                if (Function.size()) {
                    QString ColFinalName = makeColName(this->Name, _col, true, _relation);
                    if (Function == "COUNT_DISTINCT")
                        _selectItems.Cols.append("COUNT(DISTINCT " + ColFinalName.split(' ').first()+ ") AS COUNT_" + ColFinalName.split(' ').last());
                    else
                        _selectItems.Cols.append(Function + "(" + ColFinalName.split(' ').first()+ ") AS " + Function + "_" + ColFinalName.split(' ').first().split('.').last());
                } else
                    _selectItems.Cols.append(makeColName(this->Name, _col, true, _relation));
                return true;
            }
        }
        return false;
    };
    foreach (auto Col, this->BaseCols) {
        if (RequiredCols.isEmpty() || RequiredCols.contains("*"))
            SelectItems.Cols.append(makeColName(this->Name, Col, true));
        else
            addCol(SelectItems, Col);
    }

    QSet<stuRelation> UsedJoins;
    foreach (auto Relation, this->Relations) {
        clsTable* ForeignTable = clsTable::Registry[Relation.ReferenceTable];
        if (ForeignTable == nullptr)
            throw exHTTPInternalServerError("Reference table has not been registered: " + Relation.ReferenceTable);

        bool Joined = false;
        foreach (auto Col, ForeignTable->BaseCols) {
            if (RequiredCols.isEmpty() || RequiredCols.contains("*")) {
                SelectItems.Cols.append(makeColName(this->Name, Col, true, Relation));
                Joined = true;
            } else {
                if (addCol(SelectItems, Col, Relation))
                    Joined = true;
            }
        }

        if (Joined)
            UsedJoins.insert(Relation);
    }

    if (SelectItems.Cols.isEmpty())
        throw exHTTPBadRequest("No columns found to be reported");

    if (RequiredCols.size() && RequiredCols.size() > SelectItems.Cols.size())
        throw exHTTPBadRequest("Seems that some columns could not be resolved: Actives are: [" +SelectItems.Cols.join(", ")+ "]");

    //-----------------------------------------------------
    quint8 OpenParenthesis = 0;
    bool StatusColHasCriteria = false;
    bool CanStartWithLogical = false;
    QString LastLogical = "";
    _filters = _filters.replace("\\ ", "$SPACE$");
    foreach (auto Filter, _filters.split(" ", QString::SkipEmptyParts)) {
        QString Rule;
        Filter = Filter.trimmed ();
        if (Filter == "(") {
            Rule = LastLogical + "(";
            CanStartWithLogical = false;
            LastLogical.clear();
            OpenParenthesis++;
        } else if (Filter == ")") {
            if (OpenParenthesis <= 0) throw exHTTPBadRequest("Invalid close parenthesis without any open");
            Rule = " )";
            OpenParenthesis--;
            CanStartWithLogical = true;
        } else if (Filter == '+' || Filter == '|' || Filter == '*') {
            if (CanStartWithLogical == false) throw exHTTPBadRequest("Invalid logical expression prior to any rule");
            if (Filter == '+') LastLogical = "AND ";
            else if (Filter == '|') LastLogical = "OR ";
            else if (Filter == '*') LastLogical = "XOR ";

            CanStartWithLogical = false;
            continue;
        } else {
            static QRegularExpression rxFilterPattern("([a-zA-Z0-9\\_]+)([<>!=~]=?)(.+)");
            Filter = Filter.replace("$SPACE$", " ");
            QRegularExpressionMatch PatternMatches = rxFilterPattern.match(Filter);
            if (PatternMatches.lastCapturedIndex() != 3)
                throw exHTTPBadRequest("Invalid filter set: " + Filter);

            Rule = LastLogical;

            stuRelatedORMField relatedORMField = this->FilterableColsMap.value(PatternMatches.captured(1).trimmed());
            if (relatedORMField.isValid())
                Rule+=makeColName(this->Name, relatedORMField.Col, false, relatedORMField.Relation);
            else
                throw exHTTPBadRequest("Invalid column for filtring: " + PatternMatches.captured(1));

            if (relatedORMField.Col.updatableBy() == enuUpdatableBy::__STATUS__)
                StatusColHasCriteria = true;

            if (relatedORMField.Relation.Column.size() && UsedJoins.contains(relatedORMField.Relation) == false)
                UsedJoins.insert(relatedORMField.Relation);

            if (PatternMatches.captured(3) == "NULL") {
                if (PatternMatches.captured(2) == "=")
                    Rule += " IS NULL";
                else if (PatternMatches.captured(2) == "!=")
                    Rule += " IS NOT NULL";
                else
                    throw exHTTPBadRequest("Invalid filter with NULL expression: " + Filter);

                SelectItems.Where.append(Rule);
                CanStartWithLogical = true;
                LastLogical.clear();
                continue;
            }

            if (PatternMatches.captured(2) == "<") Rule += " < ";
            else if (PatternMatches.captured(2) == "<=") Rule += " <= ";
            else if (PatternMatches.captured(2) == ">") Rule += " > ";
            else if (PatternMatches.captured(2) == ">=") Rule += " >= ";
            else if (PatternMatches.captured(2) == "!=") Rule += " != ";
            else if (PatternMatches.captured(2) == "~=") Rule += " LIKE ";
            else if (PatternMatches.captured(2) == "=") Rule += " = ";
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
        SelectItems.Where.append(Rule);
    }

    if (OpenParenthesis != 0)
        throw exHTTPBadRequest("count of open and closed parenthesises does not match");

    if (SelectItems.Where.isEmpty())
        SelectItems.Where.append("TRUE");

    if (StatusColHasCriteria == false)
        foreach (auto FCol, this->FilterableColsMap)
            if (FCol.Col.updatableBy() == enuUpdatableBy::__STATUS__) {
                if (FCol.Relation.IsLeftJoin)
                    SelectItems.Where.append(QString("AND (ISNULL(%1) OR %1!='R')").arg(makeColName(this->Name, FCol.Col, false, FCol.Relation)));
                else
                    SelectItems.Where.append(QString("AND %1!='R'").arg(makeColName(this->Name, FCol.Col, false, FCol.Relation)));
                if (FCol.Relation.Column.size())
                    UsedJoins.insert(FCol.Relation);
            }

    //-----------------------------------------------------
    SelectItems.From.append(this->Schema + "." + this->Name);
    foreach (auto Join, UsedJoins) {
        SelectItems.From.append((Join.IsLeftJoin ? "LEFT JOIN " : "JOIN ")
                                + Join.ReferenceTable
                                + (Join.RenamingPrefix.size() ? " `" + Join.RenamingPrefix + "`" : "")
                                + " ON "
                                + (Join.RenamingPrefix.size() ? "`" + Join.RenamingPrefix + "`" : Join.ReferenceTable) + "." + Join.ForeignColumn
                                + " = "
                                + this->Name + "." + Join.Column
                                );
    }

    if (_extraJoins.size())
        SelectItems.From.append(_extraJoins);

    //-----------------------------------------------------
    foreach (auto OrderByCriteria, _orderBy.split(",", QString::SkipEmptyParts)) {
        QString Direction = "ASC";
        if (OrderByCriteria.startsWith("-")) {
            Direction = "DESC";
            OrderByCriteria = OrderByCriteria.mid(1);
        } else if (OrderByCriteria.startsWith("+"))
            OrderByCriteria = OrderByCriteria.mid(1);

        if (this->SortableColsMap.contains(OrderByCriteria) == false)
            throw exHTTPBadRequest("Invalid Order by <"+OrderByCriteria+"> not found in columns");

        SelectItems.OrderBy.append(OrderByCriteria + " " + Direction);
    }

    //-----------------------------------------------------
    foreach (auto GroupByCriteria, _groupBy.split(",", QString::SkipEmptyParts)) {
        stuRelatedORMField Filter = this->FilterableColsMap.value(GroupByCriteria.trimmed());
        if (Filter.isValid())
            SelectItems.GroupBy.append(makeColName(this->Name, Filter.Col, false, Filter.Relation));
        else
            throw exHTTPBadRequest("Invalid group by <"+GroupByCriteria+"> not found in columns");
    }

    return SelectItems;
}
*/
/*
QVariant clsTable::selectFromTableByID(quint64 _id, QString _cols, const QStringList& _extraJoins, const QString& _groupBy) {
    return this->selectFromTable(_extraJoins, {}, QString::number(_id), 0, 2, _cols, {}, {}, _groupBy, false, 0);
}
*/
/*
QVariant clsTable::selectFromTable(const QStringList& _extraJoins,
                                   const QString& _extraFilters,
                                   const TAPI::PKsByPath_t& _pksByPath,
                                   quint64 _offset,
                                   quint16 _limit,
                                   QString _cols,
                                   const QString& _filters,
                                   const QString& _orderBy,
                                   const QString& _groupBy,
                                   bool _reportCount,
                                   quint32 _cacheTime) {
    this->prepareFiltersList();
//    if (_ormFields.contains(COLS_KEY))
//        _cols = _ormFields.value(COLS_KEY).toString();

    stuSelectItems SelectItems = this->makeListingQuery(_cols, _extraJoins, _filters + " " + _extraFilters, _orderBy, _groupBy);
    if (_pksByPath.isEmpty()) {
        TAPI::stuTable Table;

        clsDAC DAC(this->domain(), this->Schema);
        QString QueryString = QString("SELECT ")
                              + (_reportCount ? "SQL_CALC_FOUND_ROWS" : "")
                              + QUERY_SEPARATOR
                              + SelectItems.Cols.join("," + QUERY_SEPARATOR)
                              + QUERY_SEPARATOR
                              + "FROM "
                              + QUERY_SEPARATOR
                              + SelectItems.From.join(QUERY_SEPARATOR)
                              + QUERY_SEPARATOR
                              + "WHERE "
                              + (SelectItems.Where.isEmpty() ? "TRUE" : SelectItems.Where.join(QUERY_SEPARATOR))
                              + QUERY_SEPARATOR
                              + (SelectItems.GroupBy.size() ? "GROUP BY " : "")
                              + SelectItems.GroupBy.join(',')
                              + QUERY_SEPARATOR
                              + (SelectItems.OrderBy.size() ? "ORDER BY " : "")
                              + SelectItems.OrderBy.join(',')
                              + QUERY_SEPARATOR
                              + QString("LIMIT %1,%2").arg(_offset).arg(_limit);

        if (_cacheTime)
            Table.Rows = DAC.execQueryCacheable(_cacheTime, "", QueryString).toJson(false, this->Converters).toVariant().toList();
        else
            Table.Rows = DAC.execQuery("", QueryString).toJson(false, this->Converters).toVariant().toList();

        if (_reportCount)
            Table.TotalRows = static_cast<qint64>(DAC.execQuery("","SELECT FOUND_ROWS() AS cnt").toJson(true).object().value("cnt").toDouble());
        return Table.toVariant();
    } else {
        QStringList PrimaryKeyQueries = _pksByPath.split(";");
        QStringList Filters;
        foreach (auto Query, PrimaryKeyQueries)
            foreach (auto Col, this->BaseCols)
                if (Col.isPrimaryKey()) {
                    if (Query.size())
                        Filters.append(makeColName(this->Name, Col) + " = \"" + Query + "\"");
                    break;
                }

        QString QueryString = QString("SELECT ")
                              + QUERY_SEPARATOR
                              + SelectItems.Cols.join(',' + QUERY_SEPARATOR)
                              + QUERY_SEPARATOR
                              + "FROM "
                              + QUERY_SEPARATOR
                              + SelectItems.From.join(QUERY_SEPARATOR)
                              + QUERY_SEPARATOR
                              + "WHERE "
                              + (Filters.isEmpty() ? "TRUE" : Filters.join(" AND "))
                              + QUERY_SEPARATOR
                              + "LIMIT 2"; //Limit is set to 2 in roder to produce error if multi values are selected instead of one

        clsDAC DAC(this->domain(), this->Schema);
        QJsonDocument Result;
        if (_cacheTime)
            Result = DAC.execQueryCacheable(_cacheTime, "", QueryString)
                        .toJson(true, this->Converters);
        else
            Result = DAC.execQuery("", QueryString)
                        .toJson(true, this->Converters);

        if (Result.object().isEmpty())
            throw exHTTPNotFound("No item could be found");

        return Result.toVariant();
    }
}
*/
/*
QVariant clsTable::create(quint64 _actorUserID, const TAPI::ORMFields_t& _createInfo) {
    this->prepareFiltersList();
    QStringList  CreateCommands;
    QVariantList Values;

    const auto isEmpty = [](const QVariant& _value) {
        return  _value.isNull()
                || (_value.canConvert<QVariantList>() && _value.toList().isEmpty())
                || (_value.canConvert<QVariantMap>() && _value.toMap().isEmpty())
                || (_value.canConvert<QString>() && _value.toString().isEmpty());
    };

    foreach (auto Item, this->BaseCols)
        if (Item.defaultValue() == QRequired && (_createInfo.contains(Item.name()) == false || isEmpty(_createInfo.value(Item.name()))))
            throw exHTTPBadRequest("Required field <"+ Item.name() +"> not provided: ");

    for (auto InfoIter = _createInfo.begin(); InfoIter != _createInfo.end(); ++InfoIter) {
        if (InfoIter->isValid() == false)
            continue;
        if (this->SelectableColsMap.contains(InfoIter.key()) == false)
            throw exHTTPInternalServerError("Invalid create option: " + InfoIter.key());
        stuRelatedORMField& relatedORMField = this->SelectableColsMap[InfoIter.key()];
        if (relatedORMField.Col.defaultValue() == QInvalid || relatedORMField.Col.defaultValue() == QAuto)
            throw exHTTPInternalServerError("Invalid change to read-only column <" + InfoIter.key() + ">");

        relatedORMField.Col.validate(InfoIter.value());
        CreateCommands.append(makeColName(this->Name, relatedORMField.Col) + "=?");
        Values.append(relatedORMField.Col.toDB(InfoIter.value()));
    }

    foreach (auto Col, this->BaseCols)
        if (Col.updatableBy() == enuUpdatableBy::__CREATOR__) {
            CreateCommands.append(makeColName(this->Name, Col) + "=?");
            Values.append(Col.toDB(_actorUserID));
        }

    clsDAC DAC(this->domain(), this->Schema);
    try {
        clsDACResult Result = DAC.execQuery("",
                                            QString("INSERT INTO ") + this->Schema + "." + this->Name
                                            + QUERY_SEPARATOR
                                            + "SET "
                                            + CreateCommands.join("," + QUERY_SEPARATOR)
                                            + QUERY_SEPARATOR
                                            ,Values);
        return Result.lastInsertId();
    } catch (DBManager::exTargomanDBMUnableToExecuteQuery &e) {
        QStringList FKs;
        foreach (auto FK, this->Relations)
            if (FK.RenamingPrefix.isEmpty())
                FKs.append(FK.Column);

        QRegularExpressionMatch Matches;
        if (e.what().contains(QRegularExpression(QString("FOREIGN KEY \\(`(%1)`\\)").arg(FKs.join('|'))), &Matches))
            throw exHTTPBadRequest("not a valid " + Matches.captured() + " provided");
        else if (e.what().contains(QRegularExpression(QString("Duplicate entry '[^']+'")), &Matches))
            throw exHTTPBadRequest(Matches.captured());
        else
            throw;
    }
}
*/
/*
bool clsTable::update(quint64 _actorUserID,
                      TAPI::PKsByPath_t _pksByPath,
                      const TAPI::ORMFields_t& _updateInfo,
                      const QVariantMap& _extraFilters) {
    this->prepareFiltersList();
    if (_pksByPath.isEmpty() && _extraFilters.isEmpty())
        throw exHTTPBadRequest("No key provided to update");

    if (_updateInfo.isEmpty())
        throw exHTTPBadRequest("No change provided to update");

    QStringList PrimaryKeyQueries = _pksByPath.split(";");
    QStringList Filters;
    foreach (auto Query, PrimaryKeyQueries)
        foreach (auto Col, this->BaseCols)
            if (Col.isPrimaryKey()) {
                if (Query.size())
                    Filters.append(makeColName(this->Name, Col) + " = \"" + Query + "\"");
                break;
            }

    QStringList  UpdateCommands;
    QVariantList Values;
    for (auto InfoIter = _updateInfo.begin(); InfoIter != _updateInfo.end(); ++InfoIter) {
        if (InfoIter->isValid() == false)
            continue;
        stuRelatedORMField& relatedORMField = this->SelectableColsMap[InfoIter.key()];
        if (relatedORMField.Col.isReadOnly())
            throw exHTTPInternalServerError("Invalid change to read-only column <" + InfoIter.key() + ">");

        relatedORMField.Col.validate(InfoIter.value());
        UpdateCommands.append(makeColName(this->Name, relatedORMField.Col) + "=?");
        Values.append(relatedORMField.Col.toDB(InfoIter.value()));
    }

    foreach (auto FCol, this->FilterableColsMap)
        if (FCol.Col.updatableBy() == enuUpdatableBy::__UPDATER__) {
            if (FCol.Relation.Column.isEmpty()) {
                UpdateCommands.append(makeColName(this->Name, FCol.Col) + "=?");
                Values.append(_actorUserID);
            }
        }

    for (auto FilterIter = _extraFilters.begin(); FilterIter != _extraFilters.end(); FilterIter++) {
        if (FilterIter->isValid() == false)
            continue;
        const stuRelatedORMField& relatedORMField = this->SelectableColsMap[FilterIter.key()];
        if (relatedORMField.Col.isFilterable() == false)
            throw exHTTPInternalServerError("Invalid non-filterable column <" + FilterIter.key() + ">");
        Filters.append(makeColName(this->Name, relatedORMField.Col) + "=?");
        Values.append(FilterIter.value());
    }

    try {
        clsDAC DAC(this->domain(), this->Schema);
        clsDACResult Result = DAC.execQuery("",
                                            QString("UPDATE ") + this->Schema + "." + this->Name
                                            + QUERY_SEPARATOR
                                            + "SET "
                                            + UpdateCommands.join("," + QUERY_SEPARATOR)
                                            + QUERY_SEPARATOR
                                            + "WHERE "
                                            + QUERY_SEPARATOR
                                            + Filters.join(" AND " + QUERY_SEPARATOR)
                                            ,Values);

        return Result.numRowsAffected() > 0;

    } catch (DBManager::exTargomanDBMUnableToExecuteQuery &e) {
        QStringList FKs;
        foreach (auto FK, this->Relations)
            if (FK.RenamingPrefix.isEmpty())
                FKs.append(FK.Column);

        QRegularExpressionMatch Matches;
        if (e.what().contains(QRegularExpression(QString("FOREIGN KEY \\(`(%1)`\\)").arg(FKs.join('|'))), &Matches))
            throw exHTTPBadRequest("not a valid " + Matches.captured() + " provided");
        else if (e.what().contains(QRegularExpression(QString("Duplicate entry '[^']+'")), &Matches))
            throw exHTTPBadRequest(Matches.captured());
        else
            throw;
    }
}
*/
/*
bool clsTable::deleteByPKs(quint64 _actorUserID, const TAPI::PKsByPath_t& _pksByPath, QVariantMap _extraFilters, bool _realDelete) {
    this->prepareFiltersList();

    if (this->update(_actorUserID, _pksByPath, TAPI::ORMFields_t({{this->BaseCols.last().name(), "Removed"}}), _extraFilters) == 0)
        return false;

    if (_realDelete == false)
        return true;

    QStringList PrimaryKeyQueries = _pksByPath.split(";");
    QStringList Filters;
    QVariantList Values;
    foreach (auto Query, PrimaryKeyQueries)
        foreach (auto Col, this->BaseCols)
            if (Col.isPrimaryKey()) {
                if (Query.size())
                    _extraFilters.insert(this->finalColName(Col), Query);
                break;
            }

    for (auto FilterIter = _extraFilters.begin(); FilterIter != _extraFilters.end(); ++FilterIter) {
        if (FilterIter->isValid() == false)
            continue;
        const stuRelatedORMField& relatedORMField = this->SelectableColsMap[FilterIter.key()];
        Filters.append(makeColName(this->Name, relatedORMField.Col) + "=?");
        Values.append(FilterIter.value());
    }

    clsDAC DAC(this->domain(), this->Schema);
    clsDACResult Result = DAC.execQuery("",
                                        QString("DELETE FROM ") + this->Schema + "." +this->Name
                                        + QUERY_SEPARATOR
                                        + "WHERE "
                                        + QUERY_SEPARATOR
                                        + Filters.join(" AND " + QUERY_SEPARATOR)
                                        , Values);

    return Result.numRowsAffected() > 0;
}
*/

} //namespace Targoman::API::DBM
