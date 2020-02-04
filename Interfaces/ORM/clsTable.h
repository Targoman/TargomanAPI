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
#include "Interfaces/ORM/clsORMField.h"

namespace TAPI{
TAPI_ADD_SIMPLE_TYPE(QString, Cols_t);
TAPI_ADD_SIMPLE_TYPE(QString, Filter_t);
TAPI_ADD_SIMPLE_TYPE(QString, OrderBy_t);
TAPI_ADD_SIMPLE_TYPE(QString, GroupBy_t);
}

namespace Targoman {
namespace API {

#define GET_METHOD_ARGS_HEADER TAPI::JWT_t _JWT, TAPI::ExtraPath_t _EXTRAPATH = {}, TAPI::ORMFilters_t _ORMFILTERS= {}, quint64 _offset=0, quint16 _limit=10, TAPI::Cols_t _cols={}, TAPI::Filter_t _filters={}, TAPI::OrderBy_t _orderBy={}, TAPI::GroupBy_t _groupBy={}, bool _reportCount = true
#define GET_METHOD_ARGS_IMPL   TAPI::JWT_t _JWT, TAPI::ExtraPath_t _EXTRAPATH     , TAPI::ORMFilters_t _ORMFILTERS,     quint64 _offset  , quint16 _limit   , TAPI::Cols_t _cols   , TAPI::Filter_t _filters   , TAPI::OrderBy_t _orderBy   , TAPI::GroupBy_t _groupBy   , bool _reportCount
#define GET_METHOD_CALL_ARGS   _EXTRAPATH, _ORMFILTERS, _offset, _limit, _cols, _filters, _orderBy, _groupBy, _reportCount
#define ORMGET(_doc) apiGET (GET_METHOD_ARGS_HEADER); QString signOfGET(){ return TARGOMAN_M2STR((GET_METHOD_ARGS_HEADER)); } QString docOfGET(){ return _doc; }

#define DELETE_METHOD_ARGS_HEADER TAPI::JWT_t _JWT, TAPI::ExtraPath_t _EXTRAPATH = {}, TAPI::ORMFilters_t _ORMFILTERS= {}
#define DELETE_METHOD_ARGS_IMPL   TAPI::JWT_t _JWT, TAPI::ExtraPath_t _EXTRAPATH     , TAPI::ORMFilters_t _ORMFILTERS
#define DELETE_METHOD_CALL_ARGS   _EXTRAPATH, _ORMFILTERS
#define ORMDELETE(_doc) apiDELETE (DELETE_METHOD_ARGS_HEADER); QString signOfDELETE(){ return TARGOMAN_M2STR((DELETE_METHOD_ARGS_HEADER)); } QString docOfDELETE(){ return _doc; }

#define UPDATE_METHOD_ARGS_HEADER TAPI::JWT_t _JWT, TAPI::ORMFilters_t _ORMFILTERS= {}
#define UPDATE_METHOD_ARGS_IMPL   TAPI::JWT_t _JWT, TAPI::ORMFilters_t _ORMFILTERS
#define UPDATE_METHOD_CALL_ARGS   _ORMFILTERS
#define ORMUPDATE(_doc) apiUPDATE (UPDATE_METHOD_ARGS_HEADER); QString signOfUPDATE(){ return TARGOMAN_M2STR((UPDATE_METHOD_ARGS_HEADER)); } QString docOfUPDATE(){ return _doc; }

#define CREATE_METHOD_ARGS_HEADER TAPI::JWT_t _JWT, TAPI::ORMFilters_t _ORMFILTERS= {}
#define CREATE_METHOD_ARGS_IMPL   TAPI::JWT_t _JWT, TAPI::ORMFilters_t _ORMFILTERS
#define CREATE_METHOD_CALL_ARGS   _ORMFILTERS
#define ORMCREATE(_doc) apiCREATE (CREATE_METHOD_ARGS_HEADER); QString signOfCREATE(){ return TARGOMAN_M2STR((CREATE_METHOD_ARGS_HEADER)); } QString docOfCREATE(){ return _doc; }

namespace ORM {

class clsTable;

struct stuRelation{
    QString Column;
    QString ReferenceTable;
    QString ForeignColumn;
    QString RenamingPrefix;
    bool    LeftJoin;

    stuRelation(QString _col, QString _referenceTable, QString _foreignCol, QString _renamingPrefix = {}, bool _isLeftJoin = false) :
        Column(_col), ReferenceTable(_referenceTable), ForeignColumn(_foreignCol), RenamingPrefix(_renamingPrefix), LeftJoin(_isLeftJoin)
    {;}

    bool operator == (const stuRelation& _other) { return ReferenceTable == _other.ReferenceTable && Column == _other.Column && ForeignColumn == _other.ForeignColumn;}
};

static stuRelation InvalidRelation("","","");
static QString QUERY_SEPARATOR = "\n";
static QString COLS_KEY = "cols";

class clsTable : public intfAPIModule {
protected:
    struct stuSelectItems{
        QStringList Cols;
        QStringList From;
        QStringList Where;
        QStringList OrderBy;
        QStringList GroupBy;
    };

    struct stuFilteredCol{
        clsORMField Col;
        stuRelation Relation;

        stuFilteredCol(clsORMField _col, const stuRelation& _relation = InvalidRelation) :
            Col(_col), Relation(_relation)
        {}

        stuFilteredCol() : Relation(InvalidRelation)
        {}

        inline bool isValid(){ return this->Col.name().size();}
    };

public:
    clsTable( const QString& _schema,
              const QString& _name,
              const QList<clsORMField>& _cols,
              const QList<stuRelation>& _foreignKeys);

    QList<clsORMField> filterItems(qhttp::THttpMethod _method);
    void updateFilterParamType(const QString& _fieldTypeName, QMetaType::Type _typeID);
    void prepareFiltersList();

    QVariant selectFromTable(const QStringList& _extraJoins,
                             const QString& _extraFilters,
                             const TAPI::ExtraPath_t& _extraPath,
                             const TAPI::ORMFilters_t& _ORMFILTERS,
                             quint64 _offset,
                             quint16 _limit,
                             QString _cols,
                             const QString& _filters,
                             const QString& _orderBy,
                             const QString& _groupBy,
                             bool _reportCount);

    bool update(const QVariantMap& _ORMFILTERS,
                QVariantMap _updateInfo = {});
    QVariant create(const QVariantMap& _ORMFILTERS,
                    QVariantMap _createInfo = {});
    bool deleteByPKs(TAPI::ExtraPath_t _EXTRAPATH,
                     TAPI::ORMFilters_t _ORMFILTERS,
                     bool _realDelete = false);

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


    void setSelfFilters(const QVariantMap& _requiredFilters,
                        TAPI::ExtraPath_t _EXTRAPATH,
                        TAPI::ORMFilters_t& _ORMFILTERS,
                        TAPI::Filter_t& _filters);

    void setSelfFilters(const QVariantMap& _requiredFilters,
                        TAPI::ExtraPath_t _EXTRAPATH,
                        TAPI::ORMFilters_t& _ORMFILTERS);

    QStringList privOn(qhttp::THttpMethod _method, QString _moduleName);

private:
    stuSelectItems makeListingQuery(const QString& _requiredCols = {},
                                    const QStringList& _extraJoins = {},
                                    QString _filters = {},
                                    const QString& _orderBy = {},
                                    const QString _groupBy = {}) const;
    QString makeColName(const clsORMField& _col, bool _appendAS = false, const stuRelation& _relation = InvalidRelation) const;
    QString makeColRenamedAs(const clsORMField& _col, const QString& _prefix = {})  const ;
    static QString finalColName(const clsORMField& _col, const QString& _prefix = {});
    inline QString domain();

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
    quint8  CountOfPKs;
    QMap<QString, std::function<QVariant(const QVariant& _value)>> Converters;

    static QHash<QString, clsTable*> Registry;
};

}
}
}

Q_DECLARE_METATYPE(TAPI::Cols_t);
Q_DECLARE_METATYPE(TAPI::Filter_t);
Q_DECLARE_METATYPE(TAPI::OrderBy_t);
Q_DECLARE_METATYPE(TAPI::GroupBy_t);

#endif // TARGOMAN_API_ORM_CLSTABLE_H
