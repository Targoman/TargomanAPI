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

#ifndef TARGOMAN_API_ORM_CLSRESTAPIWITHACTIONLOGS_HPP
#define TARGOMAN_API_ORM_CLSRESTAPIWITHACTIONLOGS_HPP

#include "Interfaces/ORM/clsTable.h"

namespace Targoman {
namespace API {
namespace ORM {

class clsRESTAPIWithActionLogs : public clsTable {
    Q_OBJECT
public:
    inline clsRESTAPIWithActionLogs(const QString& _schema, const QString& _module)  :
        clsTable(_schema,
                "tblActionLogs",
                { ///<ColName             Type                  Validation                      Default  RO   Sort  Filter Self  Virt   PK
                  {"atlID",               S(quint64),           QFV.integer().minValue(1),      ORM_PRIMARY_KEY},
                  {"atlBy_usrID",         S(quint32),           QFV.integer().minValue(1),      {},     true},
                  {"atlInsertionDateTime",S(TAPI::DateTime_t), QFV,                             {},     true},
                  {"atlType",             S(QString),           QFV.asciiAlNum().maxLenght(50), {},     true},
                  {"atlDescription",      S(QString),           QFV.allwaysInvalid(),           {},     true, false,false},
                },
                {
                    {"atlBy_usrID",        "AAA.tblUser",      "usrID",     "By_"},
                }),
        Module(_module)
    {;}
private slots:
    QVariant apiGETActionLogs(GET_METHOD_ARGS_HEADER);
    QString signOfGETActionLogs(){ return TARGOMAN_M2STR((GET_METHOD_ARGS_HEADER)); }
    QString docOfGETActionLogs(){ return "Get ActionLogs information"; }

private:
    QString Module;
};

}
}
}

#endif // TARGOMAN_API_ORM_CLSRESTAPIWITHACTIONLOGS_HPP
