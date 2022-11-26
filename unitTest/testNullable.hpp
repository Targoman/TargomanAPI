#ifndef TESTMACROS_HPP
#define TESTMACROS_HPP

//#include <QtGlobal>
//#include <QString>
#include <QtTest/QtTest>
//#include <optional>
using namespace std;

#include "Interfaces/Common/APIArgHelperMacros.hpp"
#include "Interfaces/Common/QtTypes.hpp"
#include "Interfaces/Common/GenericTypes.h"
#include "Interfaces/Helpers/IteratorHelper.hpp"
#include "Interfaces/Helpers/URLHelper.h"

using namespace Targoman;
using namespace Targoman::API::Helpers;
using namespace TAPI;

class Code_t : public QString  {
public:
    Code_t() { ; }
    Code_t(const QString& _other) : QString(_other) { ; }
};

class testNullable : public QObject
{
    Q_OBJECT

private:
    void func1(tmplNullable<Code_t> s = NULLABLE_NULL_VALUE) {
    }

    void func2() {
        QString s = "aaa";

        func1(s);

        tmplNullable<Code_t> s2;
        s2 = s;

        tmplNullable<Code_t> s3 = s;

        bool b = s3 == nullopt;

        if ("aaa" == s2) {

        }
    }

private slots:
    void testOptional() {
        optional<quint32> a;
        a = 1234;
    }

    void testQSharedPointer() {
        QSharedPointer<quint32> a = QSharedPointer<quint32>::create();
        *a = 1234;
    }

    void test_bad_Nullable() {
        std::optional<int> i1 = 1000;

        //-> OK : stored 1000
        std::map<std::string, std::optional<int>> aa;
        aa.insert({ "b_1", i1 });

        //-> OK : stored 1000
        std::map<QString, std::optional<int>> bb;
        bb.insert({ "b_1", i1 });

        //-> FAILED : stored null
        QMap<QString, std::optional<int>> cc;
        cc.insert( "b_1", i1 );

        //-> FAILED : stored null
        QList<std::optional<int>> dd;
        dd.append(i1);
    }

    void test_tmplNullable() {
        tmplNullable<quint32> a;

        a = std::nullopt;
        qDebug() << "a=null" << a;

        a = 1234;
        qDebug() << "a=1234" << a;

        quint32 b = a.value();
        qDebug() << "b=a.val" << b;

        tmplNullable<quint32> c = a;
        qDebug() << "c=a" << c;

        bool z = a > c;
        qDebug() << "a > c : z" << z;
        qDebug() << "a > c" << (a > c);

        quint32 o = (a < c)
                ? NULLABLE_VALUE(a)
                : NULLABLE_VALUE(c);

        QMap<QString, tmplNullable<quint32>> aa;
        aa.insert("a", a);
        aa.insert("b", b);
        aa.insert("c", c);
        tmplNullable<quint32> d;
        aa.insert("d", d);
        qDebug() << "a,b,c,d(null)" << aa;


//        TAPI::setFromVariant(a, QVariant());

        const tmplNullable<quint32> a2 = std::nullopt;

//        const std::optional<int> a3 = std::nullopt;

        tmplNullable<quint8> u1, u2;
        if (u1 < u2) {
            u1 = u2;
        }

        if (u1 < 12) { ; }

        if (34 < u2) { ; }
    }

//    void test_tmplNullable_JWT_t() {
//        tmplNullable<TAPI::JWT_t) a;
//        a = NULLABLE_NULL_VALUE;

//        TAPI::setFromVariant(a, QVariant());
//    }

};

#endif // TESTMACROS_HPP
