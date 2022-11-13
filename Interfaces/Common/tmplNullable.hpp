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
//using namespace std;

#include <QSharedData>
#include <QVariant>
#include "libTargomanCommon/exTargomanBase.h"

namespace TAPI {

template <typename _itmplValueType>
class tmplNullable;

template <typename _itmplValueType>
class tmplNullableData : public QSharedData
{
public:
    tmplNullableData() :
        QSharedData(),
        IsNull(true)
    { ; }

    tmplNullableData(const tmplNullableData &_other) :
        QSharedData(_other),
        IsNull(_other.IsNull),
        Value(_other.Value)
    { ; }

public:
    bool isNull() const {
        return (this->IsNull);
    }

    bool hasValue() const {
        return (this->IsNull == false);
    }

    void setAsNull() {
        this->IsNull = true;
    }

    template <typename _otherType = _itmplValueType>
    void setValue(const tmplNullable<_otherType> &_value) {
        this->setValue(_value.value());
    }

    template <typename _otherType = _itmplValueType>
    void setValue(tmplNullable<_otherType> &&_value) {
        this->setValue(_value.value());
    }

    template <typename _otherType = _itmplValueType>
    void setValue(const _otherType &_value) {
        this->IsNull = false;
        this->Value = static_cast<_itmplValueType>(_value);
    }

    template <typename _otherType = _itmplValueType>
    void setValue(_otherType &&_value) {
        this->IsNull = false;
        this->Value = static_cast<_itmplValueType>(_value);
    }

    _itmplValueType& value() {
        return this->Value;
    }

protected:
    bool IsNull;
    _itmplValueType Value;
};

template <typename _itmplValueType>
class tmplNullable
{
public:
    constexpr tmplNullable() :
        Data(new tmplNullableData<_itmplValueType>())
    { ; }

    template <typename _otherType = _itmplValueType>
    constexpr tmplNullable(const tmplNullable<_otherType> &_other) :
        Data(_other.Data)
    { ; }

    constexpr tmplNullable( std::nullopt_t ) :
        Data(new tmplNullableData<_itmplValueType>())
    { ; }

    template <typename _otherType = _itmplValueType>
    constexpr tmplNullable(const _otherType &_value) :
        Data(new tmplNullableData<_itmplValueType>())
    {
        this->Data->setValue(_value);
    }

public:
    bool isNull() const {
        return (this->Data->isNull());
    }
    bool hasValue() const {
        return (this->Data->hasValue());
    }

    _itmplValueType& value() const {
        if (this->Data->isNull())
            throw Targoman::Common::exTargomanBase("Nullable data has not value");

        return this->Data->value();
    }

    _itmplValueType& value_or(const _itmplValueType &_value) const {
        if (this->Data->isNull())
            return _value;

        return this->Data->value();
    }

    //------------------------------------------
    //operators
    //------------------------------------------

    tmplNullable* operator->() noexcept {
        return this->Data.get();
    }

    tmplNullable& operator*() noexcept {
        return *this->Data;
    }

    operator QVariant() const {
        if (this->Data->isNull())
            return QVariant();

        return QVariant::fromValue(this->Data->value());
    }

    //assignment
    //------------------------------------------
    constexpr tmplNullable& operator=(std::nullopt_t) noexcept {
        this->Data->setAsNull();

        return *this;
    }

    template <typename _otherType = _itmplValueType>
    constexpr tmplNullable& operator=(const _otherType &_value) noexcept {
        this->Data->setValue(_value);

        return *this;
    }

//    template <typename _otherType = _itmplValueType>
//    constexpr tmplNullable& operator=(_otherType &&_value) noexcept {
//        this->Data->setValue(_value);

//        return *this;
//    }

    template <typename _otherType = _itmplValueType>
    constexpr tmplNullable& operator=(const tmplNullable<_otherType> &_value) noexcept {
        if (_value.isNull())
            this->Data->setNull();
        else
            this->Data->setValue(_value);

        return *this;
    }

    template <typename _otherType = _itmplValueType>
    constexpr tmplNullable& operator=(tmplNullable<_otherType> &&_value) noexcept {
        if (_value.isNull())
            this->Data->setNull();
        else
            this->Data->setValue(_value);

        return *this;
    }

protected:
    QSharedPointer<tmplNullableData<_itmplValueType>> Data;
};

} // namespace TAPI

//------------------------------------------
//comparison operators
//------------------------------------------

//compare to tmplNullable
//------------------------------------------
template <typename _itmplValueType, typename _otherType = _itmplValueType>
constexpr bool operator==(const TAPI::tmplNullable<_itmplValueType> &_lh, const TAPI::tmplNullable<_otherType> &_rh) noexcept {
    if (_lh.isNull() && _rh.isNull())
        return true;

    if (_lh.value() == _rh.value())
        return true;

    return false;
}
template <typename _itmplValueType, typename _otherType = _itmplValueType>
constexpr bool operator==(const TAPI::tmplNullable<_itmplValueType> &_lh, TAPI::tmplNullable<_otherType> &&_rh) noexcept {
    if (_lh.isNull() && _rh.isNull())
        return true;

    if (_lh.value() == _rh.value())
        return true;

    return false;
}

template <typename _itmplValueType, typename _otherType = _itmplValueType>
constexpr bool operator!=(const TAPI::tmplNullable<_itmplValueType> &_lh, const TAPI::tmplNullable<_otherType> &_rh) noexcept {
    if (_lh.isNull() != _rh.isNull())
        return true;

    if (_lh.value() != _rh.value())
        return true;

    return false;
}
template <typename _itmplValueType, typename _otherType = _itmplValueType>
constexpr bool operator!=(const TAPI::tmplNullable<_itmplValueType> &_lh, TAPI::tmplNullable<_otherType> &&_rh) noexcept {
    if (_lh.isNull() != _rh.isNull())
        return true;

    if (_lh.value() != _rh.value())
        return true;

    return false;
}

template <typename _itmplValueType, typename _otherType = _itmplValueType>
constexpr bool operator>(const TAPI::tmplNullable<_itmplValueType> &_lh, const TAPI::tmplNullable<_otherType> &_rh) noexcept {
    if (_lh.isNull() && _rh.isNull())
        return false;

    if (_lh.value() > _rh.value())
        return true;

    return false;
}
template <typename _itmplValueType, typename _otherType = _itmplValueType>
constexpr bool operator>(const TAPI::tmplNullable<_itmplValueType> &_lh, TAPI::tmplNullable<_otherType> &&_rh) noexcept {
    if (_lh.isNull() && _rh.isNull())
        return false;

    if (_lh.value() > _rh.value())
        return true;

    return false;
}

template <typename _itmplValueType, typename _otherType = _itmplValueType>
constexpr bool operator>=(const TAPI::tmplNullable<_itmplValueType> &_lh, const TAPI::tmplNullable<_otherType> &_rh) noexcept {
    if (_lh.isNull() && _rh.isNull())
        return false;

    if (_lh.value() >= _rh.value())
        return true;

    return false;
}
template <typename _itmplValueType, typename _otherType = _itmplValueType>
constexpr bool operator>=(const TAPI::tmplNullable<_itmplValueType> &_lh, TAPI::tmplNullable<_otherType> &&_rh) noexcept {
    if (_lh.isNull() && _rh.isNull())
        return false;

    if (_lh.value() >= _rh.value())
        return true;

    return false;
}

template <typename _itmplValueType, typename _otherType = _itmplValueType>
constexpr bool operator<(const TAPI::tmplNullable<_itmplValueType> &_lh, const TAPI::tmplNullable<_otherType> &_rh) noexcept {
    if (_lh.isNull() && _rh.isNull())
        return false;

    if (_lh.value() < _rh.value())
        return true;

    return false;
}
template <typename _itmplValueType, typename _otherType = _itmplValueType>
constexpr bool operator<(const TAPI::tmplNullable<_itmplValueType> &_lh, TAPI::tmplNullable<_otherType> &&_rh) noexcept {
    if (_lh.isNull() && _rh.isNull())
        return false;

    if (_lh.value() < _rh.value())
        return true;

    return false;
}

template <typename _itmplValueType, typename _otherType = _itmplValueType>
constexpr bool operator<=(const TAPI::tmplNullable<_itmplValueType> &_lh, const TAPI::tmplNullable<_otherType> &_rh) noexcept {
    if (_lh.isNull() && _rh.isNull())
        return false;

    if (_lh.value() <= _rh.value())
        return true;

    return false;
}
template <typename _itmplValueType, typename _otherType = _itmplValueType>
constexpr bool operator<=(const TAPI::tmplNullable<_itmplValueType> &_lh, TAPI::tmplNullable<_otherType> &&_rh) noexcept {
    if (_lh.isNull() && _rh.isNull())
        return false;

    if (_lh.value() <= _rh.value())
        return true;

    return false;
}

//compare to nullopt
//------------------------------------------
template <typename _itmplValueType, typename _otherType = _itmplValueType>
constexpr bool operator==(const TAPI::tmplNullable<_itmplValueType> &_lh, std::nullopt_t) noexcept {
    return _lh.isNull();
}
template <typename _itmplValueType, typename _otherType = _itmplValueType>
constexpr bool operator==(std::nullopt_t, const TAPI::tmplNullable<_itmplValueType> &_rh) noexcept {
    return _rh.isNull();
}

template <typename _itmplValueType, typename _otherType = _itmplValueType>
constexpr bool operator!=(const TAPI::tmplNullable<_itmplValueType> &_lh, std::nullopt_t) noexcept {
    return !_lh.isNull();
}
template <typename _itmplValueType, typename _otherType = _itmplValueType>
constexpr bool operator!=(std::nullopt_t, const TAPI::tmplNullable<_itmplValueType> &_rh) noexcept {
    return !_rh.isNull();
}

//compare to value
//------------------------------------------
template <typename _itmplValueType, typename _otherType = _itmplValueType>
constexpr bool operator==(const TAPI::tmplNullable<_itmplValueType> &_lh, const _otherType &_rh) noexcept {
    if (_lh.isNull())
        return false;

    if (_lh.value() == _rh)
        return true;

    return false;
}
template <typename _itmplValueType, typename _otherType = _itmplValueType>
constexpr bool operator==(const TAPI::tmplNullable<_itmplValueType> &_lh, _otherType &&_rh) noexcept {
    if (_lh.isNull())
        return false;

    if (_lh.value() == _rh)
        return true;

    return false;
}
template <typename _itmplValueType, typename _otherType>
constexpr bool operator==(const _otherType &_lh, const TAPI::tmplNullable<_itmplValueType> &_rh) noexcept {
    if (_rh.isNull())
        return false;

    if (_rh.value() == _lh)
        return true;

    return false;
}
//template <typename _itmplValueType, typename _otherType>
//constexpr bool operator==(_otherType &&_lh, const TAPI::tmplNullable<_itmplValueType> &_rh) noexcept {
//    if (_rh.isNull())
//        return false;

//    if (_rh.value() == _lh)
//        return true;

//    return false;
//}

template <typename _itmplValueType, typename _otherType = _itmplValueType>
constexpr bool operator!=(const TAPI::tmplNullable<_itmplValueType> &_lh, const _otherType &_rh) noexcept {
    if (_lh.isNull())
        return true;

    if (_lh.value() != _rh)
        return true;

    return false;
}
template <typename _itmplValueType, typename _otherType = _itmplValueType>
constexpr bool operator!=(const TAPI::tmplNullable<_itmplValueType> &_lh, _otherType &&_rh) noexcept {
    if (_lh.isNull())
        return true;

    if (_lh.value() != _rh)
        return true;

    return false;
}
template <typename _itmplValueType, typename _otherType>
constexpr bool operator!=(const _otherType &_lh, const TAPI::tmplNullable<_itmplValueType> &_rh) noexcept {
    if (_rh.isNull())
        return true;

    if (_rh.value() != _lh)
        return true;

    return false;
}
//template <typename _itmplValueType, typename _otherType>
//constexpr bool operator!=(_otherType &&_lh, const TAPI::tmplNullable<_itmplValueType> &_rh) noexcept {
//    if (_rh.isNull())
//        return true;

//    if (_rh.value() != _lh)
//        return true;

//    return false;
//}

template <typename _itmplValueType, typename _otherType = _itmplValueType>
constexpr bool operator>(const TAPI::tmplNullable<_itmplValueType> &_lh, const _otherType &_rh) noexcept {
    if (_lh.isNull())
        return false;
//        throw Targoman::Common::exTargomanBase("Nullable is null");

    if (_lh.value() > _rh)
        return true;

    return false;
}
template <typename _itmplValueType, typename _otherType = _itmplValueType>
constexpr bool operator>(const TAPI::tmplNullable<_itmplValueType> &_lh, _otherType &&_rh) noexcept {
    if (_lh.isNull())
        return false;
//        throw Targoman::Common::exTargomanBase("Nullable is null");

    if (_lh.value() > _rh)
        return true;

    return false;
}
template <typename _itmplValueType, typename _otherType>
constexpr bool operator>(const _otherType &_lh, const TAPI::tmplNullable<_itmplValueType> &_rh) noexcept {
    if (_rh.isNull())
        return false;
//        throw Targoman::Common::exTargomanBase("Nullable is null");

    if (_rh.value() > _lh)
        return true;

    return false;
}
//template <typename _itmplValueType, typename _otherType>
//constexpr bool operator>(_otherType &&_lh, const TAPI::tmplNullable<_itmplValueType> &_rh) noexcept {
//    if (_rh.isNull())
//        return false;
//        throw Targoman::Common::exTargomanBase("Nullable is null");

//    if (_rh.value() > _lh)
//        return true;

//    return false;
//}

template <typename _itmplValueType, typename _otherType = _itmplValueType>
constexpr bool operator>=(const TAPI::tmplNullable<_itmplValueType> &_lh, const _otherType &_rh) noexcept {
    if (_lh.isNull())
        return false;
//        throw Targoman::Common::exTargomanBase("Nullable is null");

    if (_lh.value() >= _rh)
        return true;

    return false;
}
template <typename _itmplValueType, typename _otherType = _itmplValueType>
constexpr bool operator>=(const TAPI::tmplNullable<_itmplValueType> &_lh, _otherType &&_rh) noexcept {
    if (_lh.isNull())
        return false;
//        throw Targoman::Common::exTargomanBase("Nullable is null");

    if (_lh.value() >= _rh)
        return true;

    return false;
}
template <typename _itmplValueType, typename _otherType>
constexpr bool operator>=(const _otherType &_lh, const TAPI::tmplNullable<_itmplValueType> &_rh) noexcept {
    if (_rh.isNull())
        return false;
//        throw Targoman::Common::exTargomanBase("Nullable is null");

    if (_rh.value() >= _lh)
        return true;

    return false;
}
//template <typename _itmplValueType, typename _otherType>
//constexpr bool operator>=(_otherType &&_lh, const TAPI::tmplNullable<_itmplValueType> &_rh) noexcept {
//    if (_rh.isNull())
//        return false;
//        throw Targoman::Common::exTargomanBase("Nullable is null");

//    if (_rh.value() >= _lh)
//        return true;

//    return false;
//}

template <typename _itmplValueType, typename _otherType = _itmplValueType>
constexpr bool operator<(const TAPI::tmplNullable<_itmplValueType> &_lh, const _otherType &_rh) noexcept {
    if (_lh.isNull())
        return false;
//        throw Targoman::Common::exTargomanBase("Nullable is null");

    if (_lh.value() < _rh)
        return true;

    return false;
}
template <typename _itmplValueType, typename _otherType = _itmplValueType>
constexpr bool operator<(const TAPI::tmplNullable<_itmplValueType> &_lh, _otherType &&_rh) noexcept {
    if (_lh.isNull())
        return false;
//        throw Targoman::Common::exTargomanBase("Nullable is null");

    if (_lh.value() < _rh)
        return true;

    return false;
}
template <typename _itmplValueType, typename _otherType>
constexpr bool operator<(const _otherType &_lh, const TAPI::tmplNullable<_itmplValueType> &_rh) noexcept {
    if (_rh.isNull())
        return false;
//        throw Targoman::Common::exTargomanBase("Nullable is null");

    if (_rh.value() < _lh)
        return true;

    return false;
}
//template <typename _itmplValueType, typename _otherType>
//constexpr bool operator<(_otherType &&_lh, const TAPI::tmplNullable<_itmplValueType> &_rh) noexcept {
//    if (_rh.isNull())
//        return false;
//        throw Targoman::Common::exTargomanBase("Nullable is null");

//    if (_rh.value() < _lh)
//        return true;

//    return false;
//}

template <typename _itmplValueType, typename _otherType = _itmplValueType>
constexpr bool operator<=(const TAPI::tmplNullable<_itmplValueType> &_lh, const _otherType &_rh) noexcept {
    if (_lh.isNull())
        return false;
//        throw Targoman::Common::exTargomanBase("Nullable is null");

    if (_lh.value() <= _rh)
        return true;

    return false;
}
template <typename _itmplValueType, typename _otherType = _itmplValueType>
constexpr bool operator<=(const TAPI::tmplNullable<_itmplValueType> &_lh, _otherType &&_rh) noexcept {
    if (_lh.isNull())
        return false;
//        throw Targoman::Common::exTargomanBase("Nullable is null");

    if (_lh.value() <= _rh)
        return true;

    return false;
}
template <typename _itmplValueType, typename _otherType>
constexpr bool operator<=(const _otherType &_lh, const TAPI::tmplNullable<_itmplValueType> &_rh) noexcept {
    if (_rh.isNull())
        return false;
//        throw Targoman::Common::exTargomanBase("Nullable is null");

    if (_rh.value() <= _lh)
        return true;

    return false;
}
//template <typename _itmplValueType, typename _otherType>
//constexpr bool operator<=(_otherType &&_lh, const TAPI::tmplNullable<_itmplValueType> &_rh) noexcept {
//    if (_rh.isNull())
//        return false;
//        throw Targoman::Common::exTargomanBase("Nullable is null");

//    if (_rh.value() <= _lh)
//        return true;

//    return false;
//}

#endif // TAPI_TAPI::tmplNullable_HPP
