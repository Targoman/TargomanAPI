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

template <typename T, bool IsConst, bool IsReverse>
class IteratorHelper_List_Data : public QSharedData
{
public:
    IteratorHelper_List_Data(const IteratorHelper_List_Data &_other) :
        QSharedData(_other),
        Target(_other.Target),
        WhereList(_other.WhereList)
    { ; }

    IteratorHelper_List_Data(const QList<T> &_target) :
//        QSharedData(),
        Target(_target)
    { ; }

    virtual ~IteratorHelper_List_Data() { ; }

public:
    const QList<T> &Target;
    QList<std::function<bool(T _value)>> WhereList;
};

template <typename T, bool IsConst, bool IsReverse>
class IteratorHelper_List
{
public:
    IteratorHelper_List(const IteratorHelper_List<T, IsConst, IsReverse> &_other) :
        Data(_other.Data)
    { ; }

    IteratorHelper_List(const QList<T> &_target) :
        Data(new IteratorHelper_List_Data<T, IsConst, IsReverse>(_target))
    { ; }

    virtual ~IteratorHelper_List()
    { ; }

public:
    IteratorHelper_List& where(std::function<bool(T _value)> _fn) {
        this->Data->WhereList.append(_fn);
        return *this;
    }

    //[RunCount, OkCount]
    std::tuple<int, int> runAll(std::function<bool(T _value)> _fn, bool _runOnce=false) {
        int RunCount = 0;
        int OkCount = 0;

        if (IsConst) {
            auto itFrom = (IsReverse ? this->Data->Target.constEnd()-1 : this->Data->Target.constBegin());
            auto itTo = (IsReverse ? this->Data->Target.constBegin()-1 : this->Data->Target.constEnd());

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

                if (IsReverse)
                    --itFrom;
                else
                    ++itFrom;
            }
        } else {
            auto itFrom = (IsReverse ? this->Data->Target.end()-1 : this->Data->Target.begin());
            auto itTo = (IsReverse ? this->Data->Target.begin()-1 : this->Data->Target.end());

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

                if (IsReverse)
                    --itFrom;
                else
                    ++itFrom;
            }
        }

        return { RunCount, OkCount };
    }

    //[RunCount, OkCount]
    std::tuple<int, int> runFirst(std::function<bool(T _value)> _fn) {
        return this->runAll(_fn, true);
    }

protected:
    QSharedDataPointer<IteratorHelper_List_Data<T, IsConst, IsReverse>> Data;
};

template <typename TContainer, typename TK, typename TV, bool IsConst, bool IsReverse>
class IteratorHelper_Pair_Data : public QSharedData
{
public:
    IteratorHelper_Pair_Data(const IteratorHelper_Pair_Data &_other) :
        QSharedData(_other),
        Target(_other.Target),
        WhereList(_other.WhereList)
    { ; }

    IteratorHelper_Pair_Data(const TContainer &_target) :
//        QSharedData(),
        Target(_target)
    { ; }

    virtual ~IteratorHelper_Pair_Data() { ; }

public:
    const TContainer &Target;
    QList<std::function<bool(TK _key, TV _value)>> WhereList;
};

template <typename TContainer, typename TK, typename TV, bool IsConst, bool IsReverse>
class IteratorHelper_Pair
{
public:
    IteratorHelper_Pair(const IteratorHelper_Pair<TContainer, TK, TV, IsConst, IsReverse> &_other) :
        Data(_other.Data)
    { ; }

    IteratorHelper_Pair(const TContainer &_target) :
        Data(new IteratorHelper_Pair_Data<TContainer, TK, TV, IsConst, IsReverse>(_target))
    { ; }

    virtual ~IteratorHelper_Pair()
    { ; }

public:
    IteratorHelper_Pair& where(std::function<bool(TK _key, TV _value)> _fn) {
        this->Data->WhereList.append(_fn);
        return *this;
    }

    //[RunCount, OkCount]
    std::tuple<int, int> runAll(std::function<bool(TK _key, TV _value)> _fn, bool _runOnce=false) {
        int RunCount = 0;
        int OkCount = 0;

        if (IsConst) {
            auto itFrom = (IsReverse ? this->Data->Target.constEnd()-1 : this->Data->Target.constBegin());
            auto itTo = (IsReverse ? this->Data->Target.constBegin()-1 : this->Data->Target.constEnd());

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

                if (IsReverse)
                    --itFrom;
                else
                    ++itFrom;
            }
        } else {
            auto itFrom = (IsReverse ? this->Data->Target.end()-1 : this->Data->Target.begin());
            auto itTo = (IsReverse ? this->Data->Target.begin()-1 : this->Data->Target.end());

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

                if (IsReverse)
                    --itFrom;
                else
                    ++itFrom;
            }
        }

        return { RunCount, OkCount };
    }

    //[RunCount, OkCount]
    std::tuple<int, int> runFirst(std::function<bool(TK _key, TV _value)> _fn) {
        return this->runAll(_fn, true);
    }

protected:
    QSharedDataPointer<IteratorHelper_Pair_Data<TContainer, TK, TV, IsConst, IsReverse>> Data;
};

} //namespace Private

namespace IteratorHelper {
    //-- QList<T> --------------------------------------------------
    template <typename T>
    inline Private::IteratorHelper_List<T, false, false> Iterator(QList<T> &_target) {
        return Private::IteratorHelper_List<T, false, false>(_target);
    }
    template <typename T>
    inline Private::IteratorHelper_List<T, true, false> ConstIterator(const QList<T> &_target) {
        return Private::IteratorHelper_List<T, true, false>(_target);
    }
    template <typename T>
    inline Private::IteratorHelper_List<T, false, true> ReverseIterator(QList<T> &_target) {
        return Private::IteratorHelper_List<T, false, true>(_target);
    }
    template <typename T>
    inline Private::IteratorHelper_List<T, true, true> ConstReverseIterator(const QList<T> &_target) {
        return Private::IteratorHelper_List<T, true, true>(_target);
    }

    //-- QMap<TK, TV> --------------------------------------------------
    template <typename TK, typename TV>
    inline Private::IteratorHelper_Pair<QMap<TK, TV>, TK, TV, false, false> Iterator(QMap<TK, TV> &_target) {
        return Private::IteratorHelper_Pair<QMap<TK, TV>, TK, TV, false, false>(_target);
    }
    template <typename TK, typename TV>
    inline Private::IteratorHelper_Pair<QMap<TK, TV>, TK, TV, true, false> ConstIterator(const QMap<TK, TV> &_target) {
        return Private::IteratorHelper_Pair<QMap<TK, TV>, TK, TV, true, false>(_target);
    }
    template <typename TK, typename TV>
    inline Private::IteratorHelper_Pair<QMap<TK, TV>, TK, TV, false, true> ReverseIterator(QMap<TK, TV> &_target) {
        return Private::IteratorHelper_Pair<QMap<TK, TV>, TK, TV, false, true>(_target);
    }
    template <typename TK, typename TV>
    inline Private::IteratorHelper_Pair<QMap<TK, TV>, TK, TV, true, true> ConstReverseIterator(const QMap<TK, TV> &_target) {
        return Private::IteratorHelper_Pair<QMap<TK, TV>, TK, TV, true, true>(_target);
    }

    //-- QHash<TK, TV> --------------------------------------------------
    template <typename TK, typename TV>
    inline Private::IteratorHelper_Pair<QHash<TK, TV>, TK, TV, false, false> Iterator(QHash<TK, TV> &_target) {
        return Private::IteratorHelper_Pair<QHash<TK, TV>, TK, TV, false, false>(_target);
    }
    template <typename TK, typename TV>
    inline Private::IteratorHelper_Pair<QHash<TK, TV>, TK, TV, true, false> ConstIterator(const QHash<TK, TV> &_target) {
        return Private::IteratorHelper_Pair<QHash<TK, TV>, TK, TV, true, false>(_target);
    }
    template <typename TK, typename TV>
    inline Private::IteratorHelper_Pair<QHash<TK, TV>, TK, TV, false, true> ReverseIterator(QHash<TK, TV> &_target) {
        return Private::IteratorHelper_Pair<QHash<TK, TV>, TK, TV, false, true>(_target);
    }
    template <typename TK, typename TV>
    inline Private::IteratorHelper_Pair<QHash<TK, TV>, TK, TV, true, true> ConstReverseIterator(const QHash<TK, TV> &_target) {
        return Private::IteratorHelper_Pair<QHash<TK, TV>, TK, TV, true, true>(_target);
    }

} //namespace IteratorHelper

} //namespace Targoman::API::Helpers

#endif // TARGOMAN_API_ITERATORHELPER_HPP
