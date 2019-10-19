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

#ifndef TARGOMAN_API_HELPERS_ORM_INTFTABLE_HPP
#define TARGOMAN_API_HELPERS_ORM_INTFTABLE_HPP

#include "QFieldValidator.h"
#include <functional>
#include "libTargomanDBM/clsDAC.h"
#include "QHttp/QRESTServer.h"

namespace Targoman {
namespace API {
QHTTP_ADD_SIMPLE_TYPE(QString, Cols_t);
QHTTP_ADD_SIMPLE_TYPE(QString, Filter_t);
QHTTP_ADD_SIMPLE_TYPE(QString, OrderBy_t);
QHTTP_ADD_SIMPLE_TYPE(QString, GroupBy_t);

namespace Helpers {
namespace ORM {

class intfTable;

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

struct stuColumn{
    QString Name;
    QFieldValidator Validator;
    bool Sortable;
    bool Filterable;
    bool IsReadOnly;
    bool IsPrimaryKey;
    QString RenameAs;

    stuColumn(){;}

    stuColumn(const QString& _name,
              const QFieldValidator& _validator = QFV.allwaysValid(),
              bool  _sortable = true,
              bool  _filterable = true,
              bool _readOnly = false,
              bool _primaryKey = false,
              const QString& _renameAs = {}
              ):
        Name(_name),
        Validator(_validator),
        Sortable(_sortable),
        Filterable(_filterable),
        IsReadOnly(_readOnly),
        IsPrimaryKey(_primaryKey),
        RenameAs(_renameAs)
    {;}
};

class intfTable{
protected:
    struct stuSelectItems{
        QStringList Cols;
        QStringList From;
        QStringList Where;
        QStringList OrderBy;
        QStringList GroupBy;
    };

public:
    intfTable(const QString& _scheam,
              const QString& _name,
              const QList<stuColumn>& _cols,
              const QList<stuRelation>& _foreignKeys);

    QVariant selectFromTable(Targoman::DBManager::clsDAC& _db,
                             const QStringList& _extraJoins,
                             const QString& _extraFilters,
                             QString _extraPath,
                             quint64 _offset,
                             quint16 _limit,
                             const QString& _cols,
                             const QString& _filters,
                             const QString& _orderBy,
                             const QString& _groupBy,
                             bool _reportCount) const;

    bool update(Targoman::DBManager::clsDAC& _db,
                QVariantMap _primaryKeys,
                QVariantMap _updateInfo);
    QVariant create(Targoman::DBManager::clsDAC& _db,
                    QVariantMap _updateInfo);
    bool deleteByPKs(Targoman::DBManager::clsDAC& _db,
                     QVariantMap _primaryKeys);

private:
    stuSelectItems makeListingQuery(const QString& _requiredCols = {},
                                    const QStringList& _extraJoins = {},
                                    const QString _filters = {},
                                    const QString& _orderBy = {},
                                    const QString _groupBy = {}) const;
    QString makeColName(const stuColumn& _col, const stuRelation& _relation = InvalidRelation) const;
    QString makeColRenamedAs(const stuColumn& _col, const QString& _prefix = {})  const ;
    QString finalColName(const stuColumn& _col, const QString& _prefix = {}) const;

protected:
    QString Schema;
    QString Name;
    QMap<QString, stuColumn> Cols;
    QList<stuRelation> ForeignKeys;
    quint8  CountOfPKs;

    static QHash<QString, intfTable*> Registry;
};

#define GET_METHOD_ARGS_HEADER QHttp::JWT_t _JWT, QHttp::ExtraPath_t _EXTRAPATH = {}, quint64 _offset=0, quint16 _limit=10, Targoman::API::Cols_t _cols={"*"}, Targoman::API::Filter_t _filters={}, Targoman::API::OrderBy_t _orderBy={}, Targoman::API::GroupBy_t _groupBy={}, bool _reportCount = true
#define GET_METHOD_ARGS_IMPL   QHttp::JWT_t _JWT, QHttp::ExtraPath_t _EXTRAPATH     , quint64 _offset  , quint16 _limit   , Targoman::API::Cols_t _cols   , Targoman::API::Filter_t _filters   , Targoman::API::OrderBy_t _orderBy   , Targoman::API::GroupBy_t _groupBy   , bool _reportCount
#define GET_METHOD_CALL_ARGS   _EXTRAPATH, _offset, _limit, _cols, _filters, _orderBy, _groupBy, _reportCount
#define ORMGET(_doc) apiGET (GET_METHOD_ARGS_HEADER); QString signOfGET(){ return TARGOMAN_M2STR((GET_METHOD_ARGS_HEADER)); } QString docOfGET(){ return _doc; }

#ifndef API
#define API(_method, _name, _sig, _doc) api##_method##_name _sig; QString signOf##_method##_name(){ return #_sig; } QString docOf##_method##_name(){ return _doc; }
#endif

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

#endif // TARGOMAN_API_HELPERS_AAA_DEFS_HPP
