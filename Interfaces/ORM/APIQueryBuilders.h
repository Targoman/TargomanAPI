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

#ifndef TARGOMAN_API_QUERYBUILDERS_H
#define TARGOMAN_API_QUERYBUILDERS_H

#include "QueryBuilders.h"

using namespace Targoman::API::ORM;

namespace Targoman::API::Query {

//extern QVariantMap SelectOne(clsTable& _table, GET_METHOD_ARGS_HEADER_INTERNAL_CALL, QString _extraFilters = {}, quint16 _cacheTime = 0);
//extern QVariantList SelectAll(clsTable& _table, GET_METHOD_ARGS_HEADER_INTERNAL_CALL, QString _extraFilters = {}, quint16 _cacheTime = 0);
//extern TAPI::stuTable SelectAllWithCount(clsTable& _table, GET_METHOD_ARGS_HEADER_INTERNAL_CALL, QString _extraFilters = {}, quint16 _cacheTime = 0);
extern QVariant Select(clsTable& _table, GET_METHOD_ARGS_HEADER_INTERNAL_CALL, const clsCondition& _extraFilters = {}, quint16 _cacheTime = 0);

extern quint64 Create(clsTable& _table, CREATE_METHOD_ARGS_HEADER_INTERNAL_CALL);

extern bool Update(clsTable& _table, UPDATE_METHOD_ARGS_HEADER_INTERNAL_CALL, const QVariantMap& _extraFilters = {});

extern bool DeleteByPks(clsTable& _table, DELETE_METHOD_ARGS_HEADER_INTERNAL_CALL, const QVariantMap& _extraFilters = {}, bool _realDelete = false);

} //namespace Targoman::API::Query

#endif // TARGOMAN_API_QUERYBUILDERS_H
