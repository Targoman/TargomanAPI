#ifndef TARGOMAN_API_ORM_DEFS_HPP
#define TARGOMAN_API_ORM_DEFS_HPP

#include "Interfaces/ORM/clsORMField.h"
#include "Interfaces/Common/APIArgHelperMacros.hpp"

#define GET_METHOD_ARGS_HEADER TAPI::JWT_t _JWT, TAPI::PKsByPath_t _pksByPath={}, quint64 _offset=0, quint16 _limit=10, TAPI::Cols_t _cols={}, TAPI::Filter_t _filters={}, TAPI::OrderBy_t _orderBy={}, TAPI::GroupBy_t _groupBy={}, bool _reportCount = true
#define GET_METHOD_ARGS_IMPL   TAPI::JWT_t _JWT, TAPI::PKsByPath_t _pksByPath   , quint64 _offset  , quint16 _limit   , TAPI::Cols_t _cols   , TAPI::Filter_t _filters   , TAPI::OrderBy_t _orderBy   , TAPI::GroupBy_t _groupBy   , bool _reportCount
#define GET_METHOD_CALL_ARGS   _pksByPath, _offset, _limit, _cols, _filters, _orderBy, _groupBy, _reportCount
#define ORMGET(_doc) apiGET (GET_METHOD_ARGS_HEADER); QString signOfGET(){ return TARGOMAN_M2STR((GET_METHOD_ARGS_HEADER)); } QString docOfGET(){ return _doc; }

#define APPLY_GET_METHOD_CALL_ARGS_TO_QUERY(_query) \
    _query \
        .pksByPath(_pksByPath) \
        .offset(_offset) \
        .limit(_limit) \
        .addCols(_cols) \
        .orderBy(_orderBy) \
        .groupBy(_groupBy)
//    _filters
//    _reportCount

#define DELETE_METHOD_ARGS_HEADER TAPI::JWT_t _JWT, TAPI::PKsByPath_t _pksByPath = {}
#define DELETE_METHOD_ARGS_IMPL   TAPI::JWT_t _JWT, TAPI::PKsByPath_t _pksByPath
#define DELETE_METHOD_CALL_ARGS   clsJWT(_JWT).usrID(), _pksByPath
#define ORMDELETE(_doc) apiDELETE (DELETE_METHOD_ARGS_HEADER); QString signOfDELETE(){ return TARGOMAN_M2STR((DELETE_METHOD_ARGS_HEADER)); } QString docOfDELETE(){ return _doc; }

#define UPDATE_METHOD_ARGS_HEADER TAPI::JWT_t _JWT, TAPI::PKsByPath_t _pksByPath = {}, const TAPI::ORMFields_t& _updateInfo= {}
#define UPDATE_METHOD_ARGS_IMPL   TAPI::JWT_t _JWT, TAPI::PKsByPath_t _pksByPath     , const TAPI::ORMFields_t& _updateInfo
#define UPDATE_METHOD_CALL_ARGS   clsJWT(_JWT).usrID(),  _pksByPath,  _updateInfo
#define ORMUPDATE(_doc) apiUPDATE (UPDATE_METHOD_ARGS_HEADER); QString signOfUPDATE(){ return TARGOMAN_M2STR((UPDATE_METHOD_ARGS_HEADER)); } QString docOfUPDATE(){ return _doc; }

#define CREATE_METHOD_ARGS_HEADER TAPI::JWT_t _JWT, TAPI::ORMFields_t _createInfo={}
#define CREATE_METHOD_ARGS_IMPL   TAPI::JWT_t _JWT, TAPI::ORMFields_t _createInfo
#define CREATE_METHOD_CALL_ARGS   clsJWT(_JWT).usrID(), _createInfo
#define ORMCREATE(_doc) apiCREATE (CREATE_METHOD_ARGS_HEADER); QString signOfCREATE(){ return TARGOMAN_M2STR((CREATE_METHOD_ARGS_HEADER)); } QString docOfCREATE(){ return _doc; }

namespace TAPI{
TAPI_ADD_STRING_TYPE(QString, Cols_t);
TAPI_ADD_STRING_TYPE(QString, Filter_t);
TAPI_ADD_STRING_TYPE(QString, OrderBy_t);
TAPI_ADD_STRING_TYPE(QString, GroupBy_t);
}

namespace Targoman {
namespace API {

static QString QUERY_SEPARATOR = "\n";
static QString CURRENT_DATETIME = "CURRENT_DATETIME";

}
}

namespace Targoman {
namespace API {
namespace ORM {

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

TARGOMAN_DEFINE_ENUM(enuDBIndex,
    //Primary,
    Unique,
    Key,
    FullText,
    Spatial
)

struct stuDBIndex {
    enuDBIndex::Type Type;
    QStringList Columns;
    QString Name;

    stuDBIndex(QStringList _cols, enuDBIndex::Type _type = enuDBIndex::Key, QString _name = {}) :
        Type(_type), Columns(_cols), Name(_name)
    {;}

    stuDBIndex(QString _col, enuDBIndex::Type _type = enuDBIndex::Key, QString _name = {}) :
        stuDBIndex(QStringList()<<_col, _type, _name)
    {;}
};

extern stuRelation InvalidRelation;
extern QString COLS_KEY;

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

extern QString finalColName(const clsORMField& _col, const QString& _prefix = {});
extern QString makeColRenamedAs(const clsORMField& _col, const QString& _prefix = {});
extern QString makeColName(const QString& _tableName, const clsORMField& _col, bool _appendAs = false, const stuRelation& _relation = InvalidRelation);


unsigned int qHash(const Targoman::API::ORM::stuRelation& _relation);

}
}
}

TAPI_DECLARE_METATYPE(TAPI::Cols_t);
TAPI_DECLARE_METATYPE(TAPI::Filter_t);
TAPI_DECLARE_METATYPE(TAPI::OrderBy_t);
TAPI_DECLARE_METATYPE(TAPI::GroupBy_t);

bool operator == (const Targoman::API::ORM::stuRelation& _first, const Targoman::API::ORM::stuRelation& _second);

#endif // TARGOMAN_API_ORM_DEFS_HPP
