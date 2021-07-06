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

namespace Targoman {
namespace API {
namespace Query {

extern QVariantMap SelectOne(clsTable& _table, GET_METHOD_ARGS_IMPL_WOJWT, QString _extraFilters = {}, quint16 _cacheTime = 0);
extern QVariantList SelectAll(clsTable& _table, GET_METHOD_ARGS_IMPL_WOJWT, QString _extraFilters = {}, quint16 _cacheTime = 0);
extern TAPI::stuTable SelectAllWithCount(clsTable& _table, GET_METHOD_ARGS_IMPL_WOJWT, QString _extraFilters = {}, quint16 _cacheTime = 0);

extern quint64 Create(clsTable& _table, CREATE_METHOD_ARGS_IMPL);

}
}
}

#endif // TARGOMAN_API_QUERYBUILDERS_H
