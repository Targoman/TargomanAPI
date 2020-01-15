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

#ifndef TARGOMAN_API_HELPERS_ORM_INTFTABLE_H
#define TARGOMAN_API_HELPERS_ORM_INTFTABLE_H

#include "QFieldValidator.h"
#include <functional>
#include "libTargomanDBM/clsDAC.h"
#include "QHttp/QRESTServer.h"
#include "QHttp/intfRESTAPIHolder.h"

namespace Targoman {
namespace API {
QHTTP_ADD_SIMPLE_TYPE(QString, Cols_t);
QHTTP_ADD_SIMPLE_TYPE(QString, Filter_t);
QHTTP_ADD_SIMPLE_TYPE(QString, OrderBy_t);
QHTTP_ADD_SIMPLE_TYPE(QString, GroupBy_t);

#define GET_METHOD_ARGS_HEADER QHttp::JWT_t _JWT, QHttp::ExtraPath_t _EXTRAPATH = {}, QHttp::ORMFilters_t _ORMFILTERS= {}, quint64 _offset=0, quint16 _limit=10, Targoman::API::Cols_t _cols={}, Targoman::API::Filter_t _filters={}, Targoman::API::OrderBy_t _orderBy={}, Targoman::API::GroupBy_t _groupBy={}, bool _reportCount = true
#define GET_METHOD_ARGS_IMPL   QHttp::JWT_t _JWT, QHttp::ExtraPath_t _EXTRAPATH     , QHttp::ORMFilters_t _ORMFILTERS, quint64 _offset  , quint16 _limit   , Targoman::API::Cols_t _cols   , Targoman::API::Filter_t _filters   , Targoman::API::OrderBy_t _orderBy   , Targoman::API::GroupBy_t _groupBy   , bool _reportCount
#define GET_METHOD_CALL_ARGS   _EXTRAPATH, _ORMFILTERS, _offset, _limit, _cols, _filters, _orderBy, _groupBy, _reportCount
#define ORMGET(_doc) apiGET (GET_METHOD_ARGS_HEADER); QString signOfGET(){ return TARGOMAN_M2STR((GET_METHOD_ARGS_HEADER)); } QString docOfGET(){ return _doc; }

#define DELETE_METHOD_ARGS_HEADER QHttp::JWT_t _JWT, QHttp::ExtraPath_t _EXTRAPATH = {}, QHttp::ORMFilters_t _ORMFILTERS= {}
#define DELETE_METHOD_ARGS_IMPL   QHttp::JWT_t _JWT, QHttp::ExtraPath_t _EXTRAPATH     , QHttp::ORMFilters_t _ORMFILTERS
#define DELETE_METHOD_CALL_ARGS   _EXTRAPATH, _ORMFILTERS
#define ORMDELETE(_doc) apiDELETE (DELETE_METHOD_ARGS_HEADER); QString signOfDELETE(){ return TARGOMAN_M2STR((DELETE_METHOD_ARGS_HEADER)); } QString docOfDELETE(){ return _doc; }

#define UPDATE_METHOD_ARGS_HEADER QHttp::JWT_t _JWT, QHttp::ORMFilters_t _ORMFILTERS= {}
#define UPDATE_METHOD_ARGS_IMPL   QHttp::JWT_t _JWT, QHttp::ORMFilters_t _ORMFILTERS
#define UPDATE_METHOD_CALL_ARGS   _ORMFILTERS
#define ORMUPDATE(_doc) apiUPDATE (UPDATE_METHOD_ARGS_HEADER); QString signOfUPDATE(){ return TARGOMAN_M2STR((UPDATE_METHOD_ARGS_HEADER)); } QString docOfUPDATE(){ return _doc; }

#define CREATE_METHOD_ARGS_HEADER QHttp::JWT_t _JWT, QHttp::ORMFilters_t _ORMFILTERS= {}
#define CREATE_METHOD_ARGS_IMPL   QHttp::JWT_t _JWT, QHttp::ORMFilters_t _ORMFILTERS
#define CREATE_METHOD_CALL_ARGS   _ORMFILTERS
#define ORMCREATE(_doc) apiCREATE (CREATE_METHOD_ARGS_HEADER); QString signOfCREATE(){ return TARGOMAN_M2STR((CREATE_METHOD_ARGS_HEADER)); } QString docOfCREATE(){ return _doc; }

#ifndef API
#define API(_method, _name, _sig, _doc) api##_method##_name _sig; QString signOf##_method##_name(){ return #_sig; } QString docOf##_method##_name(){ return _doc; }
#endif

namespace Helpers {
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
};

static stuRelation InvalidRelation("","","");
static QString QUERY_SEPARATOR = "\n";
static QString COLS_KEY = "cols";

class clsTable: public QHttp::intfRESTAPIHolder {
protected:
    struct stuSelectItems{
        QStringList Cols;
        QStringList From;
        QStringList Where;
        QStringList OrderBy;
        QStringList GroupBy;
    };

    struct stuFilteredCol{
        QHttp::clsORMField Col;
        stuRelation Relation;

        stuFilteredCol(QHttp::clsORMField _col, const stuRelation& _relation = InvalidRelation) :
            Col(_col), Relation(_relation)
        {}

        stuFilteredCol() : Relation(InvalidRelation)
        {}
        inline bool isValid(){ return this->Col.name().size();}
    };

public:
    clsTable(const QString& _scheam,
              const QString& _name,
              const QList<QHttp::clsORMField>& _cols,
              const QList<stuRelation>& _foreignKeys);

    QList<QHttp::clsORMField> filterItems(qhttp::THttpMethod _method);
    void updateFilterParamType(const QString& _fieldTypeName, QMetaType::Type _typeID);
    void prepareFiltersList();

    QVariant selectFromTable(Targoman::DBManager::clsDAC& _db,
                             const QStringList& _extraJoins,
                             const QString& _extraFilters,
                             const QHttp::ExtraPath_t& _extraPath,
                             const QHttp::ORMFilters_t& _ORMFILTERS,
                             quint64 _offset,
                             quint16 _limit,
                             QString _cols,
                             const QString& _filters,
                             const QString& _orderBy,
                             const QString& _groupBy,
                             bool _reportCount);

    bool update(Targoman::DBManager::clsDAC& _db,
                const QVariantMap& _ORMFILTERS,
                QVariantMap _updateInfo = {});
    QVariant create(Targoman::DBManager::clsDAC& _db,
                    const QVariantMap& _ORMFILTERS,
                    QVariantMap _createInfo = {});
    bool deleteByPKs(Targoman::DBManager::clsDAC& _db,
                     QHttp::ExtraPath_t _EXTRAPATH,
                     QHttp::ORMFilters_t _ORMFILTERS,
                     bool _realDelete = false);

    void setSelfFilters(const QVariantMap& _requiredFilters,
                    QHttp::ExtraPath_t _EXTRAPATH,
                    QHttp::ORMFilters_t& _ORMFILTERS,
                    Targoman::API::Filter_t& _filters);

    void setSelfFilters(const QVariantMap& _requiredFilters,
                    QHttp::ExtraPath_t _EXTRAPATH,
                    QHttp::ORMFilters_t& _ORMFILTERS);

    QStringList privOn(qhttp::THttpMethod _method, QString _moduleName);

private:
    stuSelectItems makeListingQuery(const QString& _requiredCols = {},
                                    const QStringList& _extraJoins = {},
                                    QString _filters = {},
                                    const QString& _orderBy = {},
                                    const QString _groupBy = {}) const;
    QString makeColName(const QHttp::clsORMField& _col, bool _appendAS = false, const stuRelation& _relation = InvalidRelation) const;
    QString makeColRenamedAs(const QHttp::clsORMField& _col, const QString& _prefix = {})  const ;
    static QString finalColName(const QHttp::clsORMField& _col, const QString& _prefix = {});

protected:
    QString Schema;
    QString Name;
    QMap<QString, QHttp::clsORMField> SelectableColsMap;
    QMap<QString, stuFilteredCol> FilterableColsMap;
    QMap<QString, QHttp::clsORMField> SortableColsMap;
    QList<QHttp::clsORMField> AllCols;
    QList<QHttp::clsORMField> BaseCols;
    QList<stuRelation> ForeignKeys;
    quint8  CountOfPKs;
    QMap<QString, std::function<QVariant(const QVariant& _value)>> Converters;

    static QHash<QString, clsTable*> Registry;
};

}
}
}
}

Q_DECLARE_METATYPE(Targoman::API::Cols_t);
Q_DECLARE_METATYPE(Targoman::API::Filter_t);
Q_DECLARE_METATYPE(Targoman::API::OrderBy_t);
Q_DECLARE_METATYPE(Targoman::API::GroupBy_t);

using namespace Targoman::API;
using namespace Targoman::API::Helpers::ORM;
using namespace QHttp;
using namespace qhttp;

#endif // TARGOMAN_API_HELPERS_AAA_DEFS_HPP
