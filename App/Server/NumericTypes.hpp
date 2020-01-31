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


#ifndef TARGOMAN_API_SERVER_NUMERICTYPES_HPP
#define TARGOMAN_API_SERVER_NUMERICTYPES_HPP

#include "Interfaces/Common/HTTPExceptions.hpp"
#include "Interfaces/Common/tmplAPIArg.h"

namespace Targoman {
namespace API {

#define TAPI_SPECIAL_MAKE_GENERIC_ON_NUMERIC_TYPE(_numericType, _convertor) \
template<> inline QGenericArgument tmplAPIArg<_numericType,COMPLEXITY_Integral>::makeGenericArgument(const QVariant& _val, const QByteArray& _paramName, void** _argStorage){ \
    bool Result; *_argStorage = new _numericType; *(reinterpret_cast<_numericType*>(*_argStorage)) = static_cast<_numericType>(_val._convertor(&Result)); \
    if(!Result) throw exHTTPBadRequest("Invalid value specified for parameter: " + _paramName); \
    return QGenericArgument(this->RealTypeName, *_argStorage); \
}

TAPI_SPECIAL_MAKE_GENERIC_ON_NUMERIC_TYPE(quint8,  toUInt)
TAPI_SPECIAL_MAKE_GENERIC_ON_NUMERIC_TYPE(quint16, toUInt)
TAPI_SPECIAL_MAKE_GENERIC_ON_NUMERIC_TYPE(quint32, toUInt)
TAPI_SPECIAL_MAKE_GENERIC_ON_NUMERIC_TYPE(quint64, toULongLong)
TAPI_SPECIAL_MAKE_GENERIC_ON_NUMERIC_TYPE(qint8,   toInt)
TAPI_SPECIAL_MAKE_GENERIC_ON_NUMERIC_TYPE(qint16,  toInt)
TAPI_SPECIAL_MAKE_GENERIC_ON_NUMERIC_TYPE(qint32,  toInt)
TAPI_SPECIAL_MAKE_GENERIC_ON_NUMERIC_TYPE(qint64,  toLongLong)
TAPI_SPECIAL_MAKE_GENERIC_ON_NUMERIC_TYPE(qreal,   toDouble)
TAPI_SPECIAL_MAKE_GENERIC_ON_NUMERIC_TYPE(float,   toFloat)

}
}

#endif // TARGOMAN_API_SERVER_NUMERICTYPES_HPP
