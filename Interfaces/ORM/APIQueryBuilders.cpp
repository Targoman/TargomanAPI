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
 * @author Kambiz Zandi <kambizzandi@gmail.com>
 */

#include "APIQueryBuilders.h"
#include "clsTable.h"
using namespace Targoman::API::ORM;

#include "Interfaces/AAA/clsJWT.hpp"
using namespace Targoman::API::AAA;

namespace Targoman {
namespace API {
namespace Query {

QVariantMap SelectOne(clsTable& _table, GET_METHOD_ARGS_IMPL_WOJWT, QString _extraFilters, quint16 _cacheTime)
{
    SelectQuery query = SelectQuery(_table)
        .pksByPath(_pksByPath)
        .offset(_offset)
        .limit(_limit)
        .addCols(_cols)
        .orderBy(_orderBy)
        .groupBy(_groupBy)
        .filters(_filters)
        .filters(_extraFilters)
        .setCacheTime(_cacheTime)
    ;

    return query.one();
}

QVariantList SelectAll(clsTable& _table, GET_METHOD_ARGS_IMPL_WOJWT, QString _extraFilters, quint16 _cacheTime)
{
    SelectQuery query = SelectQuery(_table)
        .pksByPath(_pksByPath)
        .offset(_offset)
        .limit(_limit)
        .addCols(_cols)
        .orderBy(_orderBy)
        .groupBy(_groupBy)
        .filters(_filters)
        .filters(_extraFilters)
        .setCacheTime(_cacheTime)
    ;

    return query.all();
}

TAPI::stuTable SelectAllWithCount(clsTable& _table, GET_METHOD_ARGS_IMPL_WOJWT, QString _extraFilters, quint16 _cacheTime)
{
    SelectQuery query = SelectQuery(_table)
        .pksByPath(_pksByPath)
        .offset(_offset)
        .limit(_limit)
        .addCols(_cols)
        .orderBy(_orderBy)
        .groupBy(_groupBy)
        .filters(_filters)
        .filters(_extraFilters)
        .setCacheTime(_cacheTime)
    ;

    return query.allWithCount();
}

quint64 Create(clsTable& _table, CREATE_METHOD_ARGS_IMPL)
{
    CreateQuery query = CreateQuery(_table);

    for (QVariantMap::const_iterator arg = _createInfo.constBegin(); arg != _createInfo.constEnd(); ++arg)
        query.addCol(arg.key());

    query.values(_createInfo);

    clsJWT JWT(_JWT);
    return query.execute(JWT.usrID());
}

}
}
}
