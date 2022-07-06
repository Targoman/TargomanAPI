#ifndef TESTMACROS_HPP
#define TESTMACROS_HPP

//#include <QtGlobal>
#include <QtTest/QtTest>
#include <optional>
using namespace std;

#include "Interfaces/Common/APIArgHelperMacros.hpp"
#include "Interfaces/Common/QtTypes.hpp"
#include "Interfaces/Common/GenericTypes.h"
#include "Interfaces/Helpers/IteratorHelper.hpp"
#include "Interfaces/Helpers/URLHelper.h"

using namespace Targoman;
using namespace Targoman::API::Helpers;

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

    void testIteratorHelper_QList_All() {
        QList<QString> List;
        List << "A" << "B" << "C" << "D";

        qDebug() << "Iterator:";
        IteratorHelper::Iterator(List)
                .where([](auto _item) -> bool {
                    return true;
                })
                .runAll([](auto _item) -> bool {
                    qDebug() << _item;
                    return true;
                })
        ;

        qDebug() << "Const Iterator:";
        IteratorHelper::ConstIterator(List)
                .where([](auto _item) -> bool {
                    return true;
                })
                .runAll([](auto _item) -> bool {
                    qDebug() << _item;
                    return true;
                })
        ;

        qDebug() << "Reverse Iterator:";
        IteratorHelper::ReverseIterator(List)
                .where([](auto _item) -> bool {
                    return true;
                })
                .runAll([](auto _item) -> bool {
                    qDebug() << _item;
                    return true;
                })
        ;

        qDebug() << "Const Reverse Iterator:";
        IteratorHelper::ConstReverseIterator(List)
                .where([](auto _item) -> bool {
                    return true;
                })
                .runAll([](auto _item) -> bool {
                    qDebug() << _item;
                    return true;
                })
        ;
    }

    void testIteratorHelper_QList_First() {
        QList<QString> List;
        List << "A" << "B" << "C" << "D";

        qDebug() << "Iterator:";
        IteratorHelper::Iterator(List)
                .where([](auto _item) -> bool {
                    return true;
                })
                .runFirst([](auto _item) -> bool {
                    qDebug() << _item;
                    return true;
                })
        ;

        qDebug() << "Const Iterator:";
        IteratorHelper::ConstIterator(List)
                .where([](auto _item) -> bool {
                    return true;
                })
                .runFirst([](auto _item) -> bool {
                    qDebug() << _item;
                    return true;
                })
        ;

        qDebug() << "Reverse Iterator:";
        IteratorHelper::ReverseIterator(List)
                .where([](auto _item) -> bool {
                    return true;
                })
                .runFirst([](auto _item) -> bool {
                    qDebug() << _item;
                    return true;
                })
        ;

        qDebug() << "Const Reverse Iterator:";
        IteratorHelper::ConstReverseIterator(List)
                .where([](auto _item) -> bool {
                    return true;
                })
                .runFirst([](auto _item) -> bool {
                    qDebug() << _item;
                    return true;
                })
        ;
    }

    void testIteratorHelper_QMap_empty() {
        QMap<QString, QString> Map;

        qDebug() << "Iterator:";
        IteratorHelper::Iterator(Map)
                .where([](auto _key, auto _item) -> bool {
                    return true;
                })
                .runAll([](auto _key, auto _item) -> bool {
                    qDebug() << _key << ":" << _item;
                    return true;
                })
        ;
    }

    void testIteratorHelper_QMap_empty_reverse() {
        QMap<QString, QString> Map;

        qDebug() << "Iterator:";
        IteratorHelper::ReverseIterator(Map)
                .where([](auto _key, auto _item) -> bool {
                    return true;
                })
                .runAll([](auto _key, auto _item) -> bool {
                    qDebug() << _key << ":" << _item;
                    return true;
                })
        ;
    }

    void testIteratorHelper_QMap_All() {
        QMap<QString, QString> Map;
        Map.insert("A", "FA");
        Map.insert("B", "FB");
        Map.insert("C", "FC");
        Map.insert("D", "FD");

        qDebug() << "Iterator:";
        IteratorHelper::Iterator(Map)
                .where([](auto _key, auto _item) -> bool {
                    return true;
                })
                .runAll([](auto _key, auto _item) -> bool {
                    qDebug() << _key << ":" << _item;
                    return true;
                })
        ;

        qDebug() << "Const Iterator:";
        IteratorHelper::ConstIterator(Map)
                .where([](auto _key, auto _item) -> bool {
                    return true;
                })
                .runAll([](auto _key, auto _item) -> bool {
                    qDebug() << _key << ":" << _item;
                    return true;
                })
        ;

        qDebug() << "Reverse Iterator:";
        IteratorHelper::ReverseIterator(Map)
                .where([](auto _key, auto _item) -> bool {
                    return true;
                })
                .runAll([](auto _key, auto _item) -> bool {
                    qDebug() << _key << ":" << _item;
                    return true;
                })
        ;

        qDebug() << "Const Reverse Iterator:";
        IteratorHelper::ConstReverseIterator(Map)
                .where([](auto _key, auto _item) -> bool {
                    return true;
                })
                .runAll([](auto _key, auto _item) -> bool {
                    qDebug() << _key << ":" << _item;
                    return true;
                })
        ;
    }

    void testIteratorHelper_QMap_First() {
        QMap<QString, QString> Map;
        Map.insert("A", "FA");
        Map.insert("B", "FB");
        Map.insert("C", "FC");
        Map.insert("D", "FD");

        qDebug() << "Iterator:";
        IteratorHelper::Iterator(Map)
                .where([](auto _key, auto _item) -> bool {
                    return true;
                })
                .runFirst([](auto _key, auto _item) -> bool {
                    qDebug() << _key << ":" << _item;
                    return true;
                })
        ;

        qDebug() << "Const Iterator:";
        IteratorHelper::ConstIterator(Map)
                .where([](auto _key, auto _item) -> bool {
                    return true;
                })
                .runFirst([](auto _key, auto _item) -> bool {
                    qDebug() << _key << ":" << _item;
                    return true;
                })
        ;

        qDebug() << "Reverse Iterator:";
        IteratorHelper::ReverseIterator(Map)
                .where([](auto _key, auto _item) -> bool {
                    return true;
                })
                .runFirst([](auto _key, auto _item) -> bool {
                    qDebug() << _key << ":" << _item;
                    return true;
                })
        ;

        qDebug() << "Const Reverse Iterator:";
        IteratorHelper::ConstReverseIterator(Map)
                .where([](auto _key, auto _item) -> bool {
                    return true;
                })
                .runFirst([](auto _key, auto _item) -> bool {
                    qDebug() << _key << ":" << _item;
                    return true;
                })
        ;
    }

    void testIteratorHelper_QHash_All() {
        QHash<QString, QString> Map;
        Map.insert("A", "FA");
        Map.insert("B", "FB");
        Map.insert("C", "FC");
        Map.insert("D", "FD");

        qDebug() << "Iterator:";
        IteratorHelper::Iterator(Map)
                .where([](auto _key, auto _item) -> bool {
                    return true;
                })
                .runAll([](auto _key, auto _item) -> bool {
                    qDebug() << _key << ":" << _item;
                    return true;
                })
        ;

        qDebug() << "Const Iterator:";
        IteratorHelper::ConstIterator(Map)
                .where([](auto _key, auto _item) -> bool {
                    return true;
                })
                .runAll([](auto _key, auto _item) -> bool {
                    qDebug() << _key << ":" << _item;
                    return true;
                })
        ;

        qDebug() << "Reverse Iterator:";
        IteratorHelper::ReverseIterator(Map)
                .where([](auto _key, auto _item) -> bool {
                    return true;
                })
                .runAll([](auto _key, auto _item) -> bool {
                    qDebug() << _key << ":" << _item;
                    return true;
                })
        ;

        qDebug() << "Const Reverse Iterator:";
        IteratorHelper::ConstReverseIterator(Map)
                .where([](auto _key, auto _item) -> bool {
                    return true;
                })
                .runAll([](auto _key, auto _item) -> bool {
                    qDebug() << _key << ":" << _item;
                    return true;
                })
        ;
    }

    void testIteratorHelper_QHash_First() {
        QHash<QString, QString> Map;
        Map.insert("A", "FA");
        Map.insert("B", "FB");
        Map.insert("C", "FC");
        Map.insert("D", "FD");

        qDebug() << "Iterator:";
        IteratorHelper::Iterator(Map)
                .where([](auto _key, auto _item) -> bool {
                    return true;
                })
                .runFirst([](auto _key, auto _item) -> bool {
                    qDebug() << _key << ":" << _item;
                    return true;
                })
        ;

        qDebug() << "Const Iterator:";
        IteratorHelper::ConstIterator(Map)
                .where([](auto _key, auto _item) -> bool {
                    return true;
                })
                .runFirst([](auto _key, auto _item) -> bool {
                    qDebug() << _key << ":" << _item;
                    return true;
                })
        ;

        qDebug() << "Reverse Iterator:";
        IteratorHelper::ReverseIterator(Map)
                .where([](auto _key, auto _item) -> bool {
                    return true;
                })
                .runFirst([](auto _key, auto _item) -> bool {
                    qDebug() << _key << ":" << _item;
                    return true;
                })
        ;

        qDebug() << "Const Reverse Iterator:";
        IteratorHelper::ConstReverseIterator(Map)
                .where([](auto _key, auto _item) -> bool {
                    return true;
                })
                .runFirst([](auto _key, auto _item) -> bool {
                    qDebug() << _key << ":" << _item;
                    return true;
                })
        ;
    }

    void test_URLHelper() {
        qDebug() << URLHelper::domain("Dev.Test", false);
        qDebug() << URLHelper::domain("http://Dev.Test", false);
        qDebug() << URLHelper::domain("https://Dev.Test", false);

        qDebug() << URLHelper::domain("Dev.Test", true);
        qDebug() << URLHelper::domain("http://Dev.Test", true);
        qDebug() << URLHelper::domain("https://Dev.Test", true);
    }

};

#endif // TESTMACROS_HPP
