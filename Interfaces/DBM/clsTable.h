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

#ifndef TARGOMAN_API_ORM_CLSTABLE_H
#define TARGOMAN_API_ORM_CLSTABLE_H

#include "libQFieldValidator/QFieldValidator.h"
#include <functional>
#include "libTargomanDBM/clsDAC.h"
#include "Interfaces/Common/intfAPIArgManipulator.h"
//#include "Interfaces/Common/intfPureModule.h"
#include "Interfaces/DBM/Defs.hpp"

class testQueryBuilders;

namespace Targoman::API {

namespace DBM {
class clsTable;
}

//namespace Query {
//extern bool Update(Targoman::API::DBM::clsTable& _table, UPDATE_METHOD_ARGS_IMPL_INTERNAL_USER, const QVariantMap& _extraFilters);
//extern bool DeleteByPks(Targoman::API::DBM::clsTable& _table, DELETE_METHOD_ARGS_IMPL_INTERNAL_USER, const QVariantMap& _extraFilters, bool _realDelete);
//}

namespace DBM {

class clsCondition;

template <class itmplDerived> class clsBaseQueryData;
template <class itmplDerived, class itmplData> class tmplBaseQuery;

template <class itmplDerived> class clsQueryJoinTraitData;
template <class itmplDerived> class tmplQueryJoinTrait;

template <class itmplDerived> class clsQueryWhereTraitData;
template <class itmplDerived> class tmplQueryWhereTrait;

template <class itmplDerived> class clsQueryGroupAndHavingTraitData;
template <class itmplDerived> class tmplQueryGroupAndHavingTrait;

class clsSelectQueryData;
class SelectQuery;

class clsCreateQueryData;
class CreateQuery;

class clsUpdateQueryData;
class UpdateQuery;

class clsDeleteQueryData;
class DeleteQuery;

extern QString getInvalidatedAtQueryString(clsTable& _table, bool _makeWithUniqeIndex = true, bool _lookupFromRegistryFirst = true);

class clsTable //: public intfPureModule
{
protected:
    struct stuSelectItems {
        QStringList Cols;
        QStringList From;
        QStringList Where;
        QStringList OrderBy;
        QStringList GroupBy;
    };

public:
    clsTable(const QString& _domain,
             const QString& _schema,
             const QString& _name,
             const QList<clsORMField>& _cols,
             const QList<stuRelation>& _relations = {},
             const QList<stuDBIndex>& _indexes = {},
             const QVariantMap& _dbProperties = {});
    clsTable(const QString& _schema,
             const QString& _name,
             const QList<clsORMField>& _cols,
             const QList<stuRelation>& _relations = {},
             const QList<stuDBIndex>& _indexes = {},
             const QVariantMap& _dbProperties = {});

    virtual ~clsTable() = default; //for make class polymorphic

    QList<clsORMField> filterItems(qhttp::THttpMethod _method);
    void updateFilterParamType(const QString& _fieldTypeName, QMetaType::Type _typeID);
    void prepareFiltersList();

private:
    void prepareRelationsFiltersList(const QList<stuRelation> &_relations, QStringList &_appliedRelations);

public:
//    QVariant selectFromTableByID(quint64 _id,
//                                 QString _cols = {},
//                                 const QStringList& _extraJoins = {},
//                                 const QString& _groupBy = {});

//    QVariant selectFromTable(const QStringList& _extraJoins,
//                             const QString& _extraFilters,
//                             const TAPI::PKsByPath_t& _pksByPath={},
//                             quint64 _offset = 0,
//                             quint16 _limit = 10,
//                             QString _cols = {},
//                             const QString& _filters = {},
//                             const QString& _orderBy = {},
//                             const QString& _groupBy = {},
//                             bool _reportCount = false,
//                             quint32 _cacheTime = 0);

//    QVariant create(quint64 _actorUserID, const TAPI::ORMFields_t& _createInfo);

//    bool update(quint64 _actorUserID,
//                TAPI::PKsByPath_t _pksByPath,
//                const TAPI::ORMFields_t& _updateInfo,
//                const QVariantMap& _extraFilters={});

//    bool deleteByPKs(quint64 _actorUserID,
//                     const TAPI::PKsByPath_t& _pksByPath,
//                     QVariantMap _extraFilters={},
//                     bool _realDelete = false);

    DBManager::clsDACResult callSP(
        INTFAPICALLBOOM_DECL &APICALLBOOM_PARAM,
        const QString& _spName,
        const QVariantMap& _spArgs = QVariantMap(),
        const QString& _purpose = {},
        quint64* _executionTime = nullptr
    );

    DBManager::clsDACResult callSPCacheable(
        INTFAPICALLBOOM_DECL &APICALLBOOM_PARAM,
        quint32 _maxCacheTime,
        const QString& _spName,
        const QVariantMap& _spArgs = QVariantMap(),
        const QString& _purpose = {},
        quint64* _executionTime = nullptr
    );

    DBManager::clsDACResult execQuery(
        INTFAPICALLBOOM_DECL &APICALLBOOM_PARAM,
        const QString &_queryStr,
        const QVariantList &_params = QVariantList(),
        const QString& _purpose = "",
        quint64* _executionTime = nullptr
    );
    DBManager::clsDACResult execQuery(
        INTFAPICALLBOOM_DECL &APICALLBOOM_PARAM,
        const QString &_queryStr,
        const QVariantMap &_params,
        const QString& _purpose = "",
        quint64* _executionTime = nullptr
    );
    DBManager::clsDACResult execQueryCacheable(
        INTFAPICALLBOOM_DECL &APICALLBOOM_PARAM,
        quint32 _maxCacheTime,
        const QString &_queryStr,
        const QVariantList &_params = QVariantList(),
        const QString& _purpose = "",
        quint64* _executionTime = nullptr
    );
    DBManager::clsDACResult execQueryCacheable(
        INTFAPICALLBOOM_DECL &APICALLBOOM_PARAM,
        quint32 _maxCacheTime,
        const QString &_queryStr,
        const QVariantMap &_params,
        const QString& _purpose = "",
        quint64* _executionTime = nullptr
    );

    //TAPI::Filter_t -> QString
    void setSelfFilters(const QVariantMap& _requiredFilters, QString& _providedFilters);

//    void setSelfFilters(const QVariantMap& _requiredFilters, QVariantMap& _extraFilters);

    QStringList privOn(qhttp::THttpMethod _method, QString _moduleName);
    static QString finalColName(const clsORMField& _col, const QString& _prefix = {});

    clsTable* addDBProperty(const QString& _key, const QVariant& _value);
    const QVariant getDBProperty(const QString& _key);
    const QString getStatusColumnName();

    const QString schema() const;
    const QString name() const;
    const QString nameWithSchema() const;

protected:
    /*inline */const QString domain();

private:
//    stuSelectItems makeListingQuery(const QString& _requiredCols = {},
//                                    const QStringList& _extraJoins = {},
//                                    QString _filters = {},
//                                    const QString& _orderBy = {},
//                                    const QString _groupBy = {}) const;

private:
    QList<clsORMField> BaseCols;

protected:
    QString Schema;
    QString Name;
    QString Domain;

    QList<stuRelation> Relations;
    QList<stuDBIndex> Indexes;
    QVariantMap DBProperties;

    QList<stuRelatedORMField> AllCols;
    QMap<QString, stuRelatedORMField /*clsORMField*/> HasMasterNameColsMap;
public: //for accessing by intfSQLBasedModule
    QMap<QString, stuRelatedORMField /*clsORMField*/> SelectableColsMap;
protected:
    QMap<QString, stuRelatedORMField>                 FilterableColsMap;
    QMap<QString, stuRelatedORMField /*clsORMField*/> SortableColsMap;

    quint8 CountOfPKs;
    QMap<QString, std::function<QVariant(const QVariant& _value)>> Converters;

    static QHash<QString, clsTable*> Registry;

    friend clsCondition;
    template <class itmplDerived>                  friend class clsBaseQueryData;
    template <class itmplDerived, class itmplData> friend class tmplBaseQuery;
    template <class itmplDerived>                  friend class clsQueryJoinTraitData;
    template <class itmplDerived>                  friend class tmplQueryJoinTrait;
    template <class itmplDerived>                  friend class clsQueryWhereTraitData;
    template <class itmplDerived>                  friend class tmplQueryWhereTrait;
    template <class itmplDerived>                  friend class clsQueryGroupAndHavingTraitData;
    template <class itmplDerived>                  friend class tmplQueryGroupAndHavingTrait;
    friend clsSelectQueryData;
    friend SelectQuery;
    friend clsCreateQueryData;
    friend CreateQuery;
    friend clsUpdateQueryData;
    friend UpdateQuery;
    friend clsDeleteQueryData;
    friend DeleteQuery;
    friend QString getInvalidatedAtQueryString(clsTable& _table, bool _makeWithUniqeIndex, bool _lookupFromRegistryFirst);

    friend testQueryBuilders;

//    friend bool this->Update(clsTable& _table, UPDATE_METHOD_ARGS_IMPL_INTERNAL_USER, const QVariantMap& _extraFilters);
//    friend bool this->DeleteByPks(clsTable& _table, DELETE_METHOD_ARGS_IMPL_INTERNAL_USER, const QVariantMap& _extraFilters, bool _realDelete);
};

} //namespace DBM
} //namespace Targoman::API

#endif // TARGOMAN_API_ORM_CLSTABLE_H
