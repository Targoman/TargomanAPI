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

#include "clsRESTAPIWithActionLogs.h"
#include "Interfaces/AAA/AAA.hpp"
#include "Interfaces/ORM/APIQueryBuilders.h"

namespace Targoman::API::ORM {

clsRESTAPIWithActionLogs::clsRESTAPIWithActionLogs(const QString& _module, const QString& _schema) :
    clsRESTAPIWithDatabase(
        _module,
        _schema,
        "tblActionLogs",
        {///< ColName                   Type                    Validation                      Default  UpBy   Sort  Filter Self  Virt   PK
            { "atlID",                  ORM_PRIMARYKEY_64},
            { "atlBy_usrID",            S(quint64),             QFV.integer().minValue(1),      {},      UPNone},
            { "atlInsertionDateTime",   S(TAPI::DateTime_t),    QFV,                            {},      UPNone},
            { "atlType",                S(QString),             QFV.asciiAlNum().maxLenght(50), {},      UPNone},
            { "atlDescription",         S(QString),             QFV.allwaysInvalid(),           {},      UPNone, false,false},
        },
        {
            { "atlBy_usrID",            R(AAA::AAASchema,  "tblUser"),  "usrID",    "By_" },
        }
    )
{}

QVariant clsRESTAPIWithActionLogs::apiGETactionLogs(
        GET_METHOD_ARGS_IMPL_APICALL
//        TAPI::JWT_t _JWT
//        , TAPI::PKsByPath_t _pksByPath
//        , quint64 _offset
//        , quint16 _limit
//        , TAPI::Cols_t _cols
//        , TAPI::Filter_t _filters
//        , TAPI::OrderBy_t _orderBy
//        , TAPI::GroupBy_t _groupBy
//        , bool _reportCount
        )
{
    Authorization::checkPriv(_JWT, { this->Module + ":ActionLogs:CRUD~0100" });
    return Targoman::API::Query::Select(*this, GET_METHOD_CALL_ARGS_INTERNAL_CALL);
}

} // namespace Targoman::API::ORM
