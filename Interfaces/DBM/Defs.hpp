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

#ifndef TARGOMAN_API_DBM_DEFS_HPP
#define TARGOMAN_API_DBM_DEFS_HPP

#include <QString>
#include "libTargomanCommon/Macros.h"
#include "Interfaces/DBM/clsORMField.h"
//#include "Interfaces/Common/APIArgHelperMacros.hpp"

namespace Targoman::API {

static QString QUERY_SEPARATOR   = "\n";
static QString CURRENT_TIMESTAMP = "CURRENT_TIMESTAMP";

} //namespace Targoman::API


namespace Targoman::API::DBM {

struct stuRelation
{
    QString RelationName; //used by joinWith
    QString Column;
    QString ReferenceTable;
    QString ForeignColumn;
    QString RenamingPrefix;
    bool    IsLeftJoin;

    stuRelation(QString _col, QString _referenceTable, QString _foreignColumn, QString _renamingPrefix = {}, bool _isLeftJoin = false) :
        Column(_col),
        ReferenceTable(_referenceTable),
        ForeignColumn(_foreignColumn),
        RenamingPrefix(_renamingPrefix),
        IsLeftJoin(_isLeftJoin)
    { ; }

    stuRelation(QString _relationName, const stuRelation& _relation) :
        RelationName(_relationName),
        Column(_relation.Column),
        ReferenceTable(_relation.ReferenceTable),
        ForeignColumn(_relation.ForeignColumn),
        RenamingPrefix(_relation.RenamingPrefix),
        IsLeftJoin(_relation.IsLeftJoin)
    { ; }
};

TARGOMAN_DEFINE_ENUM(enuDBIndex,
    Primary,
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
    { ; }

    stuDBIndex(QString _col, enuDBIndex::Type _type = enuDBIndex::Key, QString _name = {}) :
        stuDBIndex(QStringList()<<_col, _type, _name)
    { ; }
};

extern stuRelation InvalidRelation;
extern QString COLS_KEY;

struct stuRelatedORMField
{
    clsORMField Col;
    stuRelation Relation;

    stuRelatedORMField(clsORMField _col, const stuRelation& _relation = InvalidRelation) :
        Col(_col), Relation(_relation)
    { ; }

    stuRelatedORMField() : Relation(InvalidRelation)
    { ; }

    inline bool isValid() { return this->Col.name().size();}
};

extern QString finalColName(const clsORMField& _col, const QString& _prefix = {});
//extern QString makeColRenamedAs(const clsORMField& _col, const QString& _prefix = {});
//extern QString makeColName(const QString& _tableName, const clsORMField& _col, bool _appendAs = false, const stuRelation& _relation = InvalidRelation);

unsigned int qHash(const Targoman::API::DBM::stuRelation& _relation);

} //namespace Targoman::API::DBM

bool operator == (const Targoman::API::DBM::stuRelation& _first, const Targoman::API::DBM::stuRelation& _second);

#endif // TARGOMAN_API_DBM_DEFS_HPP
