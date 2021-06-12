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
 */

#ifndef TARGOMAN_API_ORM_CLSTABLE_H
#define TARGOMAN_API_ORM_CLSTABLE_H

#include "QFieldValidator.h"
#include <functional>
#include "libTargomanDBM/clsDAC.h"
#include "Interfaces/Common/intfAPIArgManipulator.h"
#include "Interfaces/Common/intfAPIModule.h"
#include "Interfaces/ORM/Defs.hpp"

namespace Targoman {
namespace API {
namespace ORM {

class clsSelectQueryData;

class clsTable : public intfAPIModule {
protected:
    struct stuSelectItems {
        QStringList Cols;
        QStringList From;
        QStringList Where;
        QStringList OrderBy;
        QStringList GroupBy;
    };

public:
    clsTable(const QString& _schema,
             const QString& _name,
             const QList<clsORMField>& _cols,
             const QList<stuRelation>& _foreignKeys = {},
             const QList<stuDBIndex>& _indexes = {});

    QList<clsORMField> filterItems(qhttp::THttpMethod _method);
    void updateFilterParamType(const QString& _fieldTypeName, QMetaType::Type _typeID);
    void prepareFiltersList();

    QVariant selectFromTableByID(quint64 _id,
                                 QString _cols = {},
                                 const QStringList& _extraJoins = {},
                                 const QString& _groupBy = {});

    QVariant selectFromTable(const QStringList& _extraJoins,
                             const QString& _extraFilters,
                             const TAPI::PKsByPath_t& _pksByPath={},
                             quint64 _offset = 0,
                             quint16 _limit = 10,
                             QString _cols = {},
                             const QString& _filters = {},
                             const QString& _orderBy = {},
                             const QString& _groupBy = {},
                             bool _reportCount = false,
                             quint32 _cacheTime = 0);

    bool update(quint64 _actorUserID,
                TAPI::PKsByPath_t _pksByPath,
                const TAPI::ORMFields_t& _updateInfo,
                const QVariantMap& _extraFilters={});

    bool deleteByPKs(quint64 _actorUserID,
                     const TAPI::PKsByPath_t& _pksByPath,
                     QVariantMap _extraFilters={},
                     bool _realDelete = false);

    QVariant create(quint64 _actorUserID, const TAPI::ORMFields_t& _createInfo);


    DBManager::clsDACResult callSP(const QString& _spName,
                                   const QVariantMap& _spArgs = QVariantMap(),
                                   const QString& _purpose = {},
                                   quint64* _executionTime = nullptr);

    DBManager::clsDACResult callSPCacheable(quint32 _maxCacheTime,
                                            const QString& _spName,
                                            const QVariantMap& _spArgs = QVariantMap(),
                                            const QString& _purpose = {},
                                            quint64* _executionTime = nullptr);

    DBManager::clsDACResult execQuery(const QString &_queryStr,
                                      const QVariantList &_params = QVariantList(),
                                      const QString& _purpose = "",
                                      quint64* _executionTime = nullptr);
    DBManager::clsDACResult execQuery(const QString &_queryStr,
                                      const QVariantMap &_params,
                                      const QString& _purpose = "",
                                      quint64* _executionTime = nullptr);
    DBManager::clsDACResult execQueryCacheable(quint32 _maxCacheTime,
                                               const QString &_queryStr,
                                               const QVariantList &_params = QVariantList(),
                                               const QString& _purpose = "",
                                               quint64* _executionTime = nullptr);
    DBManager::clsDACResult execQueryCacheable(quint32 _maxCacheTime,
                                               const QString &_queryStr,
                                               const QVariantMap &_params,
                                               const QString& _purpose = "",
                                               quint64* _executionTime = nullptr);


    void setSelfFilters(const QVariantMap& _requiredFilters, TAPI::Filter_t& _providedFilters);

    void setSelfFilters(const QVariantMap& _requiredFilters, QVariantMap& _extraFilters);

    QStringList privOn(qhttp::THttpMethod _method, QString _moduleName);
    static QString finalColName(const clsORMField& _col, const QString& _prefix = {});
protected:
    inline QString domain();

private:
    stuSelectItems makeListingQuery(const QString& _requiredCols = {},
                                    const QStringList& _extraJoins = {},
                                    QString _filters = {},
                                    const QString& _orderBy = {},
                                    const QString _groupBy = {}) const;

protected:
    QString Schema;
    QString Name;
    QString Domain;
    QMap<QString, clsORMField> SelectableColsMap;
    QMap<QString, stuFilteredCol> FilterableColsMap;
    QMap<QString, clsORMField> SortableColsMap;
    QList<clsORMField> AllCols;
    QList<clsORMField> BaseCols;
    QList<stuRelation> ForeignKeys;
    QList<stuDBIndex> Indexes;
    quint8  CountOfPKs;
    QMap<QString, std::function<QVariant(const QVariant& _value)>> Converters;

    static QHash<QString, clsTable*> Registry;

    friend clsSelectQueryData;
};

}
}
}

#endif // TARGOMAN_API_ORM_CLSTABLE_H
