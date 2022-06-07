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

//    std::tuple<typename QList<T>::const_reverse_iterator, typename QList<T>::const_reverse_iterator> getConstReverseIndexes() {
//        return { this->Data->Target.crbegin(), this->Data->Target.crend() };
//    }

    bool runAll(std::function<void(T _value)> _fn, bool _runOnce=false) {
//        std::conditional<IsConst,
//            std::conditional<IsReverse, typename QList<T>::const_reverse_iterator, typename QList<T>::const_iterator>,
//            std::conditional<IsReverse, typename QList<T>::reverse_iterator, typename QList<T>::iterator>
//        > itFrom, itTo;

        if (IsConst) {
            if (IsReverse) {
                auto itFrom = this->Data->Target.crbegin();
                auto itTo = this->Data->Target.crend();

                while (itFrom != itTo) {
                    bool hasFalse = false;

                    foreach (auto fnWhere, this->Data->WhereList) {
                        if (fnWhere(*itFrom) == false) {
                            hasFalse = true;
                            break;
                        }
                    }

                    if ((hasFalse == false) || this->Data->WhereList.isEmpty()) {
                        _fn(*itFrom);
                        if (_runOnce)
                            return true;
                    }

        //            if (this->Reverse)
        //                --itFrom;
        //            else
                        ++itFrom;
                }
            } else {
                auto itFrom = this->Data->Target.constBegin();
                auto itTo = this->Data->Target.constEnd();

                while (itFrom != itTo) {
                    bool hasFalse = false;

                    foreach (auto fnWhere, this->Data->WhereList) {
                        if (fnWhere(*itFrom) == false) {
                            hasFalse = true;
                            break;
                        }
                    }

                    if ((hasFalse == false) || this->Data->WhereList.isEmpty()) {
                        _fn(*itFrom);
                        if (_runOnce)
                            return true;
                    }

        //            if (this->Reverse)
        //                --itFrom;
        //            else
                        ++itFrom;
                }
            }
        } else {
            if (IsReverse) {
                auto itFrom = this->Data->Target.rbegin();
                auto itTo = this->Data->Target.rend();

                while (itFrom != itTo) {
                    bool hasFalse = false;

                    foreach (auto fnWhere, this->Data->WhereList) {
                        if (fnWhere(*itFrom) == false) {
                            hasFalse = true;
                            break;
                        }
                    }

                    if ((hasFalse == false) || this->Data->WhereList.isEmpty()) {
                        _fn(*itFrom);
                        if (_runOnce)
                            return true;
                    }

        //            if (this->Reverse)
        //                --itFrom;
        //            else
                        ++itFrom;
                }
            } else {
                auto itFrom = this->Data->Target.begin();
                auto itTo = this->Data->Target.end();

                while (itFrom != itTo) {
                    bool hasFalse = false;

                    foreach (auto fnWhere, this->Data->WhereList) {
                        if (fnWhere(*itFrom) == false) {
                            hasFalse = true;
                            break;
                        }
                    }

                    if ((hasFalse == false) || this->Data->WhereList.isEmpty()) {
                        _fn(*itFrom);
                        if (_runOnce)
                            return true;
                    }

        //            if (this->Reverse)
        //                --itFrom;
        //            else
                        ++itFrom;
                }
            }
        }

        return false;
    }

    bool runFirst(std::function<void(T _value)> _fn) {
        return this->runAll(_fn, true);
    }

protected:
//    typedef std::conditional<IsConst,
//        std::conditional<IsReverse, typename QList<T>::const_reverse_iterator, typename QList<T>::const_iterator>,
//        std::conditional<IsReverse, typename QList<T>::reverse_iterator, typename QList<T>::iterator>
//    > TIterator;

    QSharedDataPointer<IteratorHelper_List_Data<T, IsConst, IsReverse>> Data;

//    typedef typename QList<T>::iterator iterator;
//    typedef typename QList<T>::const_iterator const_iterator;
//    typedef typename QList<T>::reverse_iterator reverse_iterator;
//    typedef typename QList<T>::const_reverse_iterator const_reverse_iterator;
};

template <class TContainer, typename TK, typename TV, bool IsConst, bool IsReverse>
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

    bool runAll(std::function<void(TK _key, TV _value)> _fn, bool _runOnce=false) {
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
                    _fn(itFrom.key(), *itFrom);
                    if (_runOnce)
                        return true;
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
                    _fn(itFrom.key(), *itFrom);
                    if (_runOnce)
                        return true;
                }

                if (IsReverse)
                    --itFrom;
                else
                    ++itFrom;
            }
        }

        return false;
    }

    bool runFirst(std::function<void(TK _key, TV _value)> _fn) {
        return this->runAll(_fn, true);
    }

protected:
    QSharedDataPointer<IteratorHelper_Pair_Data<TContainer, TK, TV, IsConst, IsReverse>> Data;
};

namespace IteratorHelper {
    //-- QList<T> --------------------------------------------------
    template <typename T>
    inline IteratorHelper_List<T, false, false> Iterator(const QList<T> &_target) {
        return IteratorHelper_List<T, false, false>(_target);
    }
    template <typename T>
    inline IteratorHelper_List<T, true, false> ConstIterator(const QList<T> &_target) {
        return IteratorHelper_List<T, true, false>(_target);
    }
    template <typename T>
    inline IteratorHelper_List<T, false, true> ReverseIterator(const QList<T> &_target) {
        return IteratorHelper_List<T, false, true>(_target);
    }
    template <typename T>
    inline IteratorHelper_List<T, true, true> ConstReverseIterator(const QList<T> &_target) {
        return IteratorHelper_List<T, true, true>(_target);
    }

//    //-- QMap<TK, TV> --------------------------------------------------
    template <typename TK, typename TV>
    inline IteratorHelper_Pair<QMap<TK, TV>, TK, TV, false, false> Iterator(QMap<TK, TV> &_target) {
        return IteratorHelper_Pair<QMap<TK, TV>, TK, TV, false, false>(_target);
    }
    template <typename TK, typename TV>
    inline IteratorHelper_Pair<QMap<TK, TV>, TK, TV, true, false> ConstIterator(QMap<TK, TV> &_target) {
        return IteratorHelper_Pair<QMap<TK, TV>, TK, TV, true, false>(_target);
    }
    template <typename TK, typename TV>
    inline IteratorHelper_Pair<QMap<TK, TV>, TK, TV, false, true> ReverseIterator(QMap<TK, TV> &_target) {
        return IteratorHelper_Pair<QMap<TK, TV>, TK, TV, false, true>(_target);
    }
    template <typename TK, typename TV>
    inline IteratorHelper_Pair<QMap<TK, TV>, TK, TV, true, true> ConstReverseIterator(QMap<TK, TV> &_target) {
        return IteratorHelper_Pair<QMap<TK, TV>, TK, TV, true, true>(_target);
    }

//    //-- QHash<TK, TV> --------------------------------------------------
    template <typename TK, typename TV>
    inline IteratorHelper_Pair<QHash<TK, TV>, TK, TV, false, false> Iterator(QHash<TK, TV> &_target) {
        return IteratorHelper_Pair<QHash<TK, TV>, TK, TV, false, false>(_target);
    }
    template <typename TK, typename TV>
    inline IteratorHelper_Pair<QHash<TK, TV>, TK, TV, true, false> ConstIterator(QHash<TK, TV> &_target) {
        return IteratorHelper_Pair<QHash<TK, TV>, TK, TV, true, false>(_target);
    }
    template <typename TK, typename TV>
    inline IteratorHelper_Pair<QHash<TK, TV>, TK, TV, false, true> ReverseIterator(QHash<TK, TV> &_target) {
        return IteratorHelper_Pair<QHash<TK, TV>, TK, TV, false, true>(_target);
    }
    template <typename TK, typename TV>
    inline IteratorHelper_Pair<QHash<TK, TV>, TK, TV, true, true> ConstReverseIterator(QHash<TK, TV> &_target) {
        return IteratorHelper_Pair<QHash<TK, TV>, TK, TV, true, true>(_target);
    }

} //namespace IteratorHelper

} //namespace Targoman::API::Helpers

#endif // TARGOMAN_API_ITERATORHELPER_HPP
