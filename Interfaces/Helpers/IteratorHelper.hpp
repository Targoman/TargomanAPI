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
 * @author S. Mehran M. Ziabary <ziabary@targoman.com>
 * @author Kambiz Zandi <kambizzandi@gmail.com>
 */

#ifndef TARGOMAN_API_ITERATORHELPER_HPP
#define TARGOMAN_API_ITERATORHELPER_HPP

#include <QList>
#include <QMap>
#include <QHash>
#include <QSharedData>

namespace Targoman::API::Helpers {

namespace Private {

template <typename _itmplTypeItem, bool _itmplIsConst, bool _itmplIsReverse>
class IteratorHelper_List_Data : public QSharedData
{
public:
    IteratorHelper_List_Data(const IteratorHelper_List_Data &_other) :
        QSharedData(_other),
        Target(_other.Target),
        WhereList(_other.WhereList)
    { ; }

    IteratorHelper_List_Data(const QList<_itmplTypeItem> &_target) :
//        QSharedData(),
        Target(_target)
    { ; }

    virtual ~IteratorHelper_List_Data() { ; }

public:
    const QList<_itmplTypeItem> &Target;
    QList<std::function<bool(_itmplTypeItem _item)>> WhereList;
};

template <typename _itmplTypeItem, bool _itmplIsConst, bool _itmplIsReverse>
class IteratorHelper_List
{
public:
    IteratorHelper_List(const IteratorHelper_List<_itmplTypeItem, _itmplIsConst, _itmplIsReverse> &_other) :
        Data(_other.Data)
    { ; }

    IteratorHelper_List(const QList<_itmplTypeItem> &_target) :
        Data(new IteratorHelper_List_Data<_itmplTypeItem, _itmplIsConst, _itmplIsReverse>(_target))
    { ; }

    virtual ~IteratorHelper_List()
    { ; }

public:
    IteratorHelper_List& where(std::function<bool(_itmplTypeItem _item)> _fn) {
        this->Data->WhereList.append(_fn);
        return *this;
    }

    //[RunCount, OkCount]
    std::tuple<int, int> runAll(std::function<bool(_itmplTypeItem _item)> _fn, bool _runOnce=false) {
        int RunCount = 0;
        int OkCount = 0;

        if (_itmplIsConst) {
            auto itFrom = (_itmplIsReverse ? this->Data->Target.constEnd()-1 : this->Data->Target.constBegin());
            auto itTo = (_itmplIsReverse ? this->Data->Target.constBegin()-1 : this->Data->Target.constEnd());

            while (itFrom != itTo) {
                bool hasFalse = false;

                foreach (auto fnWhere, this->Data->WhereList) {
                    if (fnWhere(*itFrom) == false) {
                        hasFalse = true;
                        break;
                    }
                }

                if ((hasFalse == false) || this->Data->WhereList.isEmpty()) {
                    if (_fn(*itFrom))
                        ++OkCount;

                    ++RunCount;

                    if (_runOnce)
                        return { RunCount, OkCount };
                }

                if (_itmplIsReverse)
                    --itFrom;
                else
                    ++itFrom;
            }
        } else {
            auto itFrom = (_itmplIsReverse ? this->Data->Target.end()-1 : this->Data->Target.begin());
            auto itTo = (_itmplIsReverse ? this->Data->Target.begin()-1 : this->Data->Target.end());

            while (itFrom != itTo) {
                bool hasFalse = false;

                foreach (auto fnWhere, this->Data->WhereList) {
                    if (fnWhere(*itFrom) == false) {
                        hasFalse = true;
                        break;
                    }
                }

                if ((hasFalse == false) || this->Data->WhereList.isEmpty()) {
                    if (_fn(*itFrom))
                        ++OkCount;

                    ++RunCount;

                    if (_runOnce)
                        return { RunCount, OkCount };
                }

                if (_itmplIsReverse)
                    --itFrom;
                else
                    ++itFrom;
            }
        }

        return { RunCount, OkCount };
    }

    //[RunCount, OkCount]
    std::tuple<int, int> runFirst(std::function<bool(_itmplTypeItem _item)> _fn) {
        return this->runAll(_fn, true);
    }

protected:
    QSharedDataPointer<IteratorHelper_List_Data<_itmplTypeItem, _itmplIsConst, _itmplIsReverse>> Data;
};

template <typename _itmplTypeContainer, typename _itmplTypeKey, typename _itmplTypeValue, bool _itmplIsConst, bool _itmplIsReverse>
class IteratorHelper_Pair_Data : public QSharedData
{
public:
    IteratorHelper_Pair_Data(const IteratorHelper_Pair_Data &_other) :
        QSharedData(_other),
        Target(_other.Target),
        WhereList(_other.WhereList)
    { ; }

    IteratorHelper_Pair_Data(const _itmplTypeContainer &_target) :
//        QSharedData(),
        Target(_target)
    { ; }

    virtual ~IteratorHelper_Pair_Data() { ; }

public:
    const _itmplTypeContainer &Target;
    QList<std::function<bool(_itmplTypeKey _key, _itmplTypeValue _value)>> WhereList;
};

template <typename _itmplTypeContainer, typename _itmplTypeKey, typename _itmplTypeValue, bool _itmplIsConst, bool _itmplIsReverse>
class IteratorHelper_Pair
{
public:
    IteratorHelper_Pair(const IteratorHelper_Pair<_itmplTypeContainer, _itmplTypeKey, _itmplTypeValue, _itmplIsConst, _itmplIsReverse> &_other) :
        Data(_other.Data)
    { ; }

    IteratorHelper_Pair(const _itmplTypeContainer &_target) :
        Data(new IteratorHelper_Pair_Data<_itmplTypeContainer, _itmplTypeKey, _itmplTypeValue, _itmplIsConst, _itmplIsReverse>(_target))
    { ; }

    virtual ~IteratorHelper_Pair()
    { ; }

public:
    IteratorHelper_Pair& where(std::function<bool(_itmplTypeKey _key, _itmplTypeValue _value)> _fn) {
        this->Data->WhereList.append(_fn);
        return *this;
    }

    //[RunCount, OkCount]
    std::tuple<int, int> runAll(std::function<bool(_itmplTypeKey _key, _itmplTypeValue _value)> _fn, bool _runOnce=false) {
        int RunCount = 0;
        int OkCount = 0;

        if (_itmplIsConst) {
            auto itFrom = (_itmplIsReverse ? this->Data->Target.constEnd()-1 : this->Data->Target.constBegin());
            auto itTo = (_itmplIsReverse ? this->Data->Target.constBegin()-1 : this->Data->Target.constEnd());

            while (itFrom != itTo) {
                bool hasFalse = false;

                foreach (auto fnWhere, this->Data->WhereList) {
                    if (fnWhere(itFrom.key(), *itFrom) == false) {
                        hasFalse = true;
                        break;
                    }
                }

                if ((hasFalse == false) || this->Data->WhereList.isEmpty()) {
                    if (_fn(itFrom.key(), *itFrom))
                        ++OkCount;

                    ++RunCount;

                    if (_runOnce)
                        return { RunCount, OkCount };
                }

                if (_itmplIsReverse)
                    --itFrom;
                else
                    ++itFrom;
            }
        } else {
            auto itFrom = (_itmplIsReverse ? this->Data->Target.end()-1 : this->Data->Target.begin());
            auto itTo = (_itmplIsReverse ? this->Data->Target.begin()-1 : this->Data->Target.end());

            while (itFrom != itTo) {
                bool hasFalse = false;

                foreach (auto fnWhere, this->Data->WhereList) {
                    if (fnWhere(itFrom.key(), *itFrom) == false) {
                        hasFalse = true;
                        break;
                    }
                }

                if ((hasFalse == false) || this->Data->WhereList.isEmpty()) {
                    if (_fn(itFrom.key(), *itFrom))
                        ++OkCount;

                    ++RunCount;

                    if (_runOnce)
                        return { RunCount, OkCount };
                }

                if (_itmplIsReverse)
                    --itFrom;
                else
                    ++itFrom;
            }
        }

        return { RunCount, OkCount };
    }

    //[RunCount, OkCount]
    std::tuple<int, int> runFirst(std::function<bool(_itmplTypeKey _key, _itmplTypeValue _value)> _fn) {
        return this->runAll(_fn, true);
    }

protected:
    QSharedDataPointer<IteratorHelper_Pair_Data<_itmplTypeContainer, _itmplTypeKey, _itmplTypeValue, _itmplIsConst, _itmplIsReverse>> Data;
};

} //namespace Private

namespace IteratorHelper {
    //-- QList<T> --------------------------------------------------
    template <typename _itmplTypeItem>
    inline Private::IteratorHelper_List<_itmplTypeItem, false, false> Iterator(QList<_itmplTypeItem> &_target) {
        return Private::IteratorHelper_List<_itmplTypeItem, false, false>(_target);
    }
    template <typename _itmplTypeItem>
    inline Private::IteratorHelper_List<_itmplTypeItem, true, false> ConstIterator(const QList<_itmplTypeItem> &_target) {
        return Private::IteratorHelper_List<_itmplTypeItem, true, false>(_target);
    }
    template <typename _itmplTypeItem>
    inline Private::IteratorHelper_List<_itmplTypeItem, false, true> ReverseIterator(QList<_itmplTypeItem> &_target) {
        return Private::IteratorHelper_List<_itmplTypeItem, false, true>(_target);
    }
    template <typename _itmplTypeItem>
    inline Private::IteratorHelper_List<_itmplTypeItem, true, true> ConstReverseIterator(const QList<_itmplTypeItem> &_target) {
        return Private::IteratorHelper_List<_itmplTypeItem, true, true>(_target);
    }

    //-- QMap<_itmplTypeKey, _itmplTypeValue> --------------------------------------------------
    template <typename _itmplTypeKey, typename _itmplTypeValue>
    inline Private::IteratorHelper_Pair<QMap<_itmplTypeKey, _itmplTypeValue>, _itmplTypeKey, _itmplTypeValue, false, false> Iterator(QMap<_itmplTypeKey, _itmplTypeValue> &_target) {
        return Private::IteratorHelper_Pair<QMap<_itmplTypeKey, _itmplTypeValue>, _itmplTypeKey, _itmplTypeValue, false, false>(_target);
    }
    template <typename _itmplTypeKey, typename _itmplTypeValue>
    inline Private::IteratorHelper_Pair<QMap<_itmplTypeKey, _itmplTypeValue>, _itmplTypeKey, _itmplTypeValue, true, false> ConstIterator(const QMap<_itmplTypeKey, _itmplTypeValue> &_target) {
        return Private::IteratorHelper_Pair<QMap<_itmplTypeKey, _itmplTypeValue>, _itmplTypeKey, _itmplTypeValue, true, false>(_target);
    }
    template <typename _itmplTypeKey, typename _itmplTypeValue>
    inline Private::IteratorHelper_Pair<QMap<_itmplTypeKey, _itmplTypeValue>, _itmplTypeKey, _itmplTypeValue, false, true> ReverseIterator(QMap<_itmplTypeKey, _itmplTypeValue> &_target) {
        return Private::IteratorHelper_Pair<QMap<_itmplTypeKey, _itmplTypeValue>, _itmplTypeKey, _itmplTypeValue, false, true>(_target);
    }
    template <typename _itmplTypeKey, typename _itmplTypeValue>
    inline Private::IteratorHelper_Pair<QMap<_itmplTypeKey, _itmplTypeValue>, _itmplTypeKey, _itmplTypeValue, true, true> ConstReverseIterator(const QMap<_itmplTypeKey, _itmplTypeValue> &_target) {
        return Private::IteratorHelper_Pair<QMap<_itmplTypeKey, _itmplTypeValue>, _itmplTypeKey, _itmplTypeValue, true, true>(_target);
    }

    //-- QHash<_itmplTypeKey, _itmplTypeValue> --------------------------------------------------
    template <typename _itmplTypeKey, typename _itmplTypeValue>
    inline Private::IteratorHelper_Pair<QHash<_itmplTypeKey, _itmplTypeValue>, _itmplTypeKey, _itmplTypeValue, false, false> Iterator(QHash<_itmplTypeKey, _itmplTypeValue> &_target) {
        return Private::IteratorHelper_Pair<QHash<_itmplTypeKey, _itmplTypeValue>, _itmplTypeKey, _itmplTypeValue, false, false>(_target);
    }
    template <typename _itmplTypeKey, typename _itmplTypeValue>
    inline Private::IteratorHelper_Pair<QHash<_itmplTypeKey, _itmplTypeValue>, _itmplTypeKey, _itmplTypeValue, true, false> ConstIterator(const QHash<_itmplTypeKey, _itmplTypeValue> &_target) {
        return Private::IteratorHelper_Pair<QHash<_itmplTypeKey, _itmplTypeValue>, _itmplTypeKey, _itmplTypeValue, true, false>(_target);
    }
    template <typename _itmplTypeKey, typename _itmplTypeValue>
    inline Private::IteratorHelper_Pair<QHash<_itmplTypeKey, _itmplTypeValue>, _itmplTypeKey, _itmplTypeValue, false, true> ReverseIterator(QHash<_itmplTypeKey, _itmplTypeValue> &_target) {
        return Private::IteratorHelper_Pair<QHash<_itmplTypeKey, _itmplTypeValue>, _itmplTypeKey, _itmplTypeValue, false, true>(_target);
    }
    template <typename _itmplTypeKey, typename _itmplTypeValue>
    inline Private::IteratorHelper_Pair<QHash<_itmplTypeKey, _itmplTypeValue>, _itmplTypeKey, _itmplTypeValue, true, true> ConstReverseIterator(const QHash<_itmplTypeKey, _itmplTypeValue> &_target) {
        return Private::IteratorHelper_Pair<QHash<_itmplTypeKey, _itmplTypeValue>, _itmplTypeKey, _itmplTypeValue, true, true>(_target);
    }

} //namespace IteratorHelper

} //namespace Targoman::API::Helpers

#endif // TARGOMAN_API_ITERATORHELPER_HPP
