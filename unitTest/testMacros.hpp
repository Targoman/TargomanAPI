#ifndef TESTMACROS_HPP
#define TESTMACROS_HPP

//#include <QtGlobal>
#include <QtTest/QtTest>
#include <optional>
using namespace std;

#include "Interfaces/Common/APIArgHelperMacros.hpp"
#include "Interfaces/Common/QtTypes.hpp"
#include "Interfaces/Common/GenericTypes.h"
using namespace Targoman;

class testMacros: public QObject
{
    Q_OBJECT

private slots:
    void testOptional() {
        optional<quint32> a;
        a = 1234;

//            std::optional<enuAggregation::Type> z1;
//            z1 = enuAggregation::MAX;
//            *zzz = enuAggregation::MAX;

//            NULLABLE_TYPE(enuAggregation::Type) a1, a2;
//            TAPI::tmplNullable<enuAggregation::Type> a1, a2;
//            enuAggregation::Type a3 = a1;
//            std::optional<enuAggregation::Type> a1, a2;
//            if (a1 < a2)
//            {
//            }
    }

    void testQSharedPointer() {
        QSharedPointer<quint32> a = QSharedPointer<quint32>::create();
        *a = 1234;
    }

    void testNullable() {
        NULLABLE_TYPE(quint32) a;
        a = 1234;

        TAPI::setFromVariant(a, QVariant());
    }

    void testJSON_t() {
        TAPI::JSON_t a;
//        a = 1234;

//        QVariant v = a;

        QVariantMap val = { {"aa", "11"}, {"bb", "22"} };
        TAPI::setFromVariant(a, val);

        qDebug() << a << "[aa]" << a["aa"];
    }

};

#endif // TESTMACROS_HPP
