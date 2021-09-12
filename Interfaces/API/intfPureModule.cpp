/******************************************************************************
#   TargomanAPI: REST API for Targoman
#
#   Copyright 2014-2020 by Targoman Intelligent Processing <http://tip.co.ir>
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
 * @author Kambiz Zandi <kambizzandi@gmail.com>
 */

#include "intfPureModule.h"
#include "Interfaces/Common/APIArgHelperMacros.hpp"
#include "Interfaces/Common/intfAPIArgManipulator.h"

#include "Interfaces/DBM/clsTable.h"
using namespace Targoman::API::DBM;

TAPI_VALIDATION_REQUIRED_TYPE_IMPL(
        COMPLEXITY_String,
        TAPI,
        Cols_t,
        QFieldValidator::allwaysValid(),
        _value,
        [](const QList<clsORMField>& _fields) {
    QStringList Cols;
    foreach(auto Col, _fields)
        if(Col.isVirtual() == false)
            Cols.append(clsTable::finalColName(Col));
    return QString("Nothing for all or comma separated columns: (ex. %1,%2) \n"
                   "you can also use aggregation functions: (ex. COUNT(%3))\n"
                   "* COUNT\n"
                   "* COUNT_DISTINCT\n"
                   "* SUM\n"
                   "* AVG\n"
                   "* MAX\n"
                   "* MIN\n\n"
                   "Available Cols are: \n"
                   "* %4").arg(Cols.first()).arg(Cols.last()).arg(Cols.first()).arg(Cols.join("\n* "));
});

TAPI_VALIDATION_REQUIRED_TYPE_IMPL(COMPLEXITY_String, TAPI, Filter_t,
                                   QFieldValidator::allwaysValid(), _value,
                                   [](const QList<clsORMField>& _fields){
    return "Filtering rules where '+'=AND, '|'=OR, '*'=XOR. All parenthesis and logical operators must be bounded by space.\n"
           "Equality/Inequality operators are\n"
           "* =: equal\n"
           "* !=: not equal\n"
           "* <, <=, >, >=: inequal\n"
           "* ~=: LIKE on strings\n"
           "Take note that just columns listed in GroupBy field can be filtered\n"
           "example: \n"
           "* ( "+_fields.last().name()+"='1' | "+_fields.last().name()+"!='2' )";
});
TAPI_VALIDATION_REQUIRED_TYPE_IMPL(COMPLEXITY_String, TAPI, OrderBy_t,
                                   QFieldValidator::allwaysValid(), _value,
                                   [](const QList<clsORMField>& _fields){
    QStringList Cols;
    foreach(auto Col, _fields)
        if(Col.isSortable() && Col.isVirtual() == false)
            Cols.append(clsTable::finalColName(Col));
    return "Comma separated list of columns with +/- for ASC/DESC order prefix: (ex. +"+Cols.first()+",-"+Cols.last()+")\n* " + Cols.join("\n* ");
    //                                            return "Comma separated list of columns with +/- for ASC/DESC order prefix: (ex. +"+Cols.first()+",-"+Cols.last()+")";
});
TAPI_VALIDATION_REQUIRED_TYPE_IMPL(COMPLEXITY_String, TAPI, GroupBy_t,
                                   QFieldValidator::allwaysValid(), _value,
                                   [](const QList<clsORMField>& _fields){
    QStringList Cols;
    foreach(auto Col, _fields)
        if(Col.isFilterable())
            Cols.append(clsTable::finalColName(Col));
    return "Comma separated columns: \n* " + Cols.join(",\n* ");
    //                                               return "Comma separated columns" ;
});

namespace Targoman::API::API {

intfPureModule::intfPureModule(
        const QString& _moduleName,
        Targoman::Common::Configuration::intfModule *_parent
    ) :
    intfModule(_parent),
    ModuleName(_moduleName)
{}

intfPureModule::~intfPureModule(){}

/***************************************************************/
intfPureModule::stuDBInfo::stuDBInfo(QString _schema, quint16 _port, QString _host, QString _user, QString _pass) :
    Host(_host),
    Port(_port),
    User(_user),
    Pass(_pass),
    Schema(_schema)
{}

QString intfPureModule::stuDBInfo::toConnStr(bool _noSchema) {
    return QString("HOST=%1;PORT=%2;USER=%3;PASSWORD=%4;SCHEMA=%5").arg(
                this->Host).arg(
                this->Port).arg(
                this->User).arg(
                this->Pass).arg(
                _noSchema ? "" : this->Schema);
}

} //namespace Targoman::API::API
