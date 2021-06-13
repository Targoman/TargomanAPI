#ifndef TAPI_TMPLNULLABLE_HPP
#define TAPI_TMPLNULLABLE_HPP

#include <QSharedPointer>
#include <QVariant>

namespace TAPI {

/************************************************************/
template<class itmplType>
class tmplNullable : public QSharedPointer<itmplType> {
public:
    tmplNullable()                : QSharedPointer<itmplType>(nullptr) {}
    tmplNullable(std::nullptr_t)  : QSharedPointer<itmplType>(nullptr) {}
    tmplNullable(itmplType *_ptr) : QSharedPointer<itmplType>(_ptr) {};
    tmplNullable(itmplType _val)  : QSharedPointer<itmplType>(new itmplType) { this->operator=(_val); }
    tmplNullable(const tmplNullable<itmplType> &_val) : QSharedPointer<itmplType>(_val) { }
    tmplNullable(const QVariant& _val) {
        if (_val.isNull() == false)
            tmplNullable(_val.value<itmplType>());
    }
};

} // namespace API
#endif // TAPI_TMPLNULLABLE_HPP
