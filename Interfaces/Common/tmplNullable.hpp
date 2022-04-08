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

#ifndef TAPI_TMPLNULLABLE_HPP
#define TAPI_TMPLNULLABLE_HPP

#include <optional>
using namespace std;

#include <QSharedPointer>
#include <QVariant>

namespace TAPI {

/************************************************************/
//template<class itmplType>
//class tmplNullable : public QSharedPointer<itmplType> {
//public:
//    tmplNullable()                                    : QSharedPointer<itmplType>(nullptr) { ; }
//    tmplNullable(std::nullptr_t)                      : QSharedPointer<itmplType>(nullptr) { ; }
//    tmplNullable(itmplType _val)                      : QSharedPointer<itmplType>(new itmplType) { *this = _val; }
//    tmplNullable(itmplType* _ptr)                     : QSharedPointer<itmplType>(new itmplType) { *this = _ptr; };
//    tmplNullable(const tmplNullable<itmplType>& _val) : QSharedPointer<itmplType>(_val) { ; }
//    tmplNullable(const QVariant& _val)                : QSharedPointer<itmplType>(nullptr) {
//        if (_val.isNull() == false)
//            tmplNullable(_val.value<itmplType>());
//    }
//};

//template<class itmplType>
//class tmplNullable : public optional<itmplType> {
//public:
//    tmplNullable()                                    : optional<itmplType>() { ; }
//    tmplNullable(std::nullopt_t)                      : optional<itmplType>() { ; }
//    tmplNullable(std::nullptr_t)                      : optional<itmplType>() { ; }
//    tmplNullable(itmplType _val)                      : optional<itmplType>() { *this = _val; }
//    tmplNullable(itmplType* _ptr)                     : optional<itmplType>() { *this = _ptr; };
//    tmplNullable(const tmplNullable<itmplType>& _val) : optional<itmplType>(_val) { ; }
//    tmplNullable(const QVariant& _val)                : optional<itmplType>() {
//        if (_val.isNull() == false)
//            tmplNullable(_val.value<itmplType>());
//    }

//public:
//    bool isNull() const {
//        return this->operator bool();
//    }
//};

/************************************************************/
} // namespace API

#endif // TAPI_TMPLNULLABLE_HPP
