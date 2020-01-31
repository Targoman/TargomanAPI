/******************************************************************************
#   TargomanAPI: REST API for Targoman
#
#   Copyright 2014-2019 by Targoman Intelligent Processing <http://tip.co.ir>
#
#   TargomanAPI is free software: you can redistribute it and/or modify
#   it under the terms of the GNU AFFERO GENERAL PUBLIC LICENSE as published by
#   the Free Software Foundation, either version 3 of the License, or
#   (at your option) any later version.
#
#   TargomanAPI is distributed in the hope that it will be useful,
#   but WITHOUT ANY WARRANTY; without even the implied warranty of
#   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#   GNU AFFERO GENERAL PUBLIC LICENSE for more details.
#
#   You should have received a copy of the GNU AFFERO GENERAL PUBLIC LICENSE
#   along with Targoman. If not, see <http://www.gnu.org/licenses/>.
 ******************************************************************************/
/**
 * @author S.Mehran M.Ziabary <ziabary@targoman.com>
 */

#ifndef TARGOMAN_API_ORM_CLSTABLE_H
#define TARGOMAN_API_ORM_CLSTABLE_H

#include "QFieldValidator.h"
#include <functional>
#include "libTargomanDBM/clsDAC.h"
#include "Interfaces/Common/intfAPIModule.hpp"
#include "Interfaces/ORM/clsORMField.h"

namespace TAPI{
TAPI_ADD_SIMPLE_TYPE(QString, Cols_t);
TAPI_ADD_SIMPLE_TYPE(QString, Filter_t);
TAPI_ADD_SIMPLE_TYPE(QString, OrderBy_t);
TAPI_ADD_SIMPLE_TYPE(QString, GroupBy_t);
}

namespace Targoman {
namespace API {
namespace ORM {

#define GET_METHOD_ARGS_HEADER TAPI::JWT_t _JWT, TAPI::ExtraPath_t _EXTRAPATH = {}, TAPI::ORMFilters_t _ORMFILTERS= {}, quint64 _offset=0, quint16 _limit=10, TAPI::Cols_t _cols={}, TAPI::Filter_t _filters={}, TAPI::OrderBy_t _orderBy={}, TAPI::GroupBy_t _groupBy={}, bool _reportCount = true
#define GET_METHOD_ARGS_IMPL   TAPI::JWT_t _JWT, TAPI::ExtraPath_t _EXTRAPATH     , TAPI::ORMFilters_t _ORMFILTERS, quint64 _offset  , quint16 _limit   , TAPI::Cols_t _cols   , TAPI::Filter_t _filters   , TAPI::OrderBy_t _orderBy   , TAPI::GroupBy_t _groupBy   , bool _reportCount
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

#ifndef API
#define API(_method, _name, _sig, _doc) api##_method##_name _sig; QString signOf##_method##_name(){ return #_sig; } QString docOf##_method##_name(){ return _doc; }
#endif

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

extern stuRelation InvalidRelation;
extern QString QUERY_SEPARATOR;
extern QString COLS_KEY;

extern void extInitTableTypes(clsTable* _table, const QString& _schema, const QString& _name);
extern QList<clsORMField> extFilterItems(clsTable* _table, qhttp::THttpMethod _method);
extern void extUpdateFilterParamType(clsTable* _table, const QString& _fieldTypeName, QMetaType::Type _typeID);
extern void extPrepareFiltersList(clsTable* _table);
extern QVariant extSelectFromTable(clsTable* _table,
                                   Targoman::DBManager::clsDAC& _db,
                                   const QStringList& _extraJoins,
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
extern bool extUpdate(clsTable* _table,
                      Targoman::DBManager::clsDAC& _db,
                      const QVariantMap& _ORMFILTERS,
                      QVariantMap _updateInfo = {});
extern QVariant extCreate(clsTable* _table,
                          Targoman::DBManager::clsDAC& _db,
                          const QVariantMap& _ORMFILTERS,
                          QVariantMap _createInfo = {});
extern bool extDeleteByPKs(clsTable* _table,
                           Targoman::DBManager::clsDAC& _db,
                           TAPI::ExtraPath_t _EXTRAPATH,
                           TAPI::ORMFilters_t _ORMFILTERS,
                           bool _realDelete = false);

extern void extSetSelfFilters(clsTable* _table,
                              const QVariantMap& _requiredFilters,
                              TAPI::ExtraPath_t _EXTRAPATH,
                              TAPI::ORMFilters_t& _ORMFILTERS,
                              TAPI::Filter_t& _filters);

extern void extSetSelfFilters(clsTable* _table,
                              const QVariantMap& _requiredFilters,
                              TAPI::ExtraPath_t _EXTRAPATH,
                              TAPI::ORMFilters_t& _ORMFILTERS);

extern QStringList extPrivOn(qhttp::THttpMethod _method, QString _moduleName);

class clsTable: public intfAPIModule {
public:
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
    clsTable(const QString& _schema,
             const QString& _name,
             const QList<clsORMField>& _cols,
             const QList<stuRelation>& _foreignKeys) :
        Schema(_schema),
        Name(_name),
        BaseCols(_cols),
        ForeignKeys(_foreignKeys),
        CountOfPKs(0){
        extInitTableTypes(this, _schema, _name);
    }


    inline QList<clsORMField> filterItems(qhttp::THttpMethod _method){
        return extFilterItems(this, _method);
    }
    inline void updateFilterParamType(const QString& _fieldTypeName, QMetaType::Type _typeID){
        extUpdateFilterParamType (this, _fieldTypeName, _typeID);
    }

    inline void prepareFiltersList(){
        extPrepareFiltersList(this);
    }

    inline QVariant selectFromTable(Targoman::DBManager::clsDAC& _db,
                                    const QStringList& _extraJoins,
                                    const QString& _extraFilters,
                                    const TAPI::ExtraPath_t& _extraPath,
                                    const TAPI::ORMFilters_t& _ORMFILTERS,
                                    quint64 _offset,
                                    quint16 _limit,
                                    QString _cols,
                                    const QString& _filters,
                                    const QString& _orderBy,
                                    const QString& _groupBy,
                                    bool _reportCount){
        return extSelectFromTable(this,
                                  _db,
                                  _extraJoins,
                                  _extraFilters,
                                  _extraPath,
                                  _ORMFILTERS,
                                  _offset,
                                  _limit,
                                  _cols,
                                  _filters,
                                  _orderBy,
                                  _groupBy,
                                  _reportCount);
    }

    inline bool update(Targoman::DBManager::clsDAC& _db,
                       const QVariantMap& _ORMFILTERS,
                       QVariantMap _updateInfo = {}){
        return extUpdate(this, _db, _ORMFILTERS, _updateInfo);
    }
    inline QVariant create(Targoman::DBManager::clsDAC& _db,
                           const QVariantMap& _ORMFILTERS,
                           QVariantMap _createInfo = {}){
        return extCreate (this, _db, _ORMFILTERS, _createInfo);
    }
    inline bool deleteByPKs(Targoman::DBManager::clsDAC& _db,
                            TAPI::ExtraPath_t _EXTRAPATH,
                            TAPI::ORMFilters_t _ORMFILTERS,
                            bool _realDelete = false){
        return extDeleteByPKs (this, _db, _EXTRAPATH, _ORMFILTERS, _realDelete);
    }

    inline void setSelfFilters(const QVariantMap& _requiredFilters,
                               TAPI::ExtraPath_t _EXTRAPATH,
                               TAPI::ORMFilters_t& _ORMFILTERS,
                               TAPI::Filter_t& _filters){
        return extSetSelfFilters (this, _requiredFilters, _EXTRAPATH, _ORMFILTERS, _filters);
    }

    inline void setSelfFilters(const QVariantMap& _requiredFilters,
                               TAPI::ExtraPath_t _EXTRAPATH,
                               TAPI::ORMFilters_t& _ORMFILTERS){
        return extSetSelfFilters (this, _requiredFilters, _EXTRAPATH, _ORMFILTERS);
    }

    inline QStringList privOn(qhttp::THttpMethod _method, QString _moduleName){
        return extPrivOn(_method, _moduleName);
    }

public:
    QString Schema;
    QString Name;
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
