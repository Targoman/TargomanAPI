#ifndef QUERYBUILDERS_HPP
#define QUERYBUILDERS_HPP

#include <QtTest/QtTest>
#include "Interfaces/ORM/QueryBuilders.h"

using namespace Targoman::API::ORM;
class QueryBuilders: public QObject
{
    Q_OBJECT

private slots:
    void condition() {
        QVERIFY(clsCondition().isEmpty());
        QVERIFY(clsCondition().closePar().isEmpty() == false);
        QVERIFY(clsCondition().openPar(clsCondition()).isEmpty() == false);
        QVERIFY(clsCondition().andCond(clsCondition()).isEmpty() == false);
        QVERIFY(clsCondition().orCond(clsCondition()).isEmpty() == false);
        QVERIFY(clsCondition().xorCond(clsCondition()).isEmpty() == false);

    };
};

#endif // QUERYBUILDERS_HPP
