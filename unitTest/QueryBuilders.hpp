#ifndef QUERYBUILDERS_HPP
#define QUERYBUILDERS_HPP

#include <QtTest/QtTest>
#include <QStringList>

#include <optional>
using namespace std;

//#include "Interfaces/Test/testCommon.hpp"

#include "Interfaces/AAA/Accounting_Interfaces.h"
using namespace Targoman::API::AAA::Accounting;

#include "Interfaces/ORM/QueryBuilders.h"
using namespace Targoman::API::ORM;

using namespace Targoman;

class TestTable1 : public clsTable
{
    Q_OBJECT

public:
    TestTable1()
        : clsTable(
            "test",
            "t1",
            {///< ColName Type                             Validation                 Default     UpBy      Sort  Filter Self  Virt   PK
                { "colA1", ORM_PRIMARY_KEY32 },
                { "colB1", S(TAPI::SaleableCode_t),         QFV,                       QRequired,  UPOwner },
                { "colC1", S(quint32),                      QFV.integer().minValue(1), QNull,      UPOwner },
                { "colD1", S(QString),                      QFV,                       QNull,      UPOwner },
                { "colE1", S(TAPI::DateTime_t),             QFV,                       QNull,      UPOwner },
                { "colF1", S(TAPI::JSON_t),                 QFV,                       QNull,      UPOwner },
                { "colG1", S(qreal),                        QFV.real().minValue(0),    QNull,      UPOwner },
                { "colH1", S(qreal),                        QFV.real().minValue(0),    QNull,      UPOwner },
            },
            {///< Col     Reference Table  ForeignCol  Rename  LeftJoin
//                           { "colC1", R("test", "t2"), "colA2" },
                { "rel_a", { "colC1", R("test", "t2"), "colA2" } },
            }
        )
    {}

//    TARGOMAN_DEFINE_API_SUBMODULE(Test, TestTable)

    ModuleMethods_t listOfMethods() final
    {
        return this->Methods;
    }

    QString parentModuleName() const
    {
        return "aaa";
    }

};

class TestTable2 : public clsTable
{
    Q_OBJECT

public:
    TestTable2()
        : clsTable(
            "test",
            "t2",
            {///< ColName Type                             Validation                 Default     UpBy      Sort  Filter Self  Virt   PK
                { "colA2", ORM_PRIMARY_KEY32 },
                { "colB2", S(TAPI::SaleableCode_t),         QFV,                       QRequired,  UPOwner },
                { "colC2", S(quint32),                      QFV.integer().minValue(1), QRequired,  UPOwner },
                { "colD2", S(QString),                      QFV,                       QRequired,  UPOwner },
                { "colE2", S(TAPI::DateTime_t),             QFV,                       QNull,      UPOwner },
                { "colF2", S(TAPI::JSON_t),                 QFV,                       QNull,      UPOwner },
                { "colG2", S(qreal),                        QFV.real().minValue(0),    QRequired,  UPOwner },
                { "colH2", S(qreal),                        QFV.real().minValue(0),    QRequired,  UPOwner },
            }
        )
    {}

//    TARGOMAN_DEFINE_API_SUBMODULE(Test, TestTable)

    ModuleMethods_t listOfMethods() final
    {
        return this->Methods;
    }

    QString parentModuleName() const
    {
        return "aaa";
    }

};

#define QVERIFY_TRY(expression) \
    do { \
        QT_TRY { \
            QVERIFY(expression); \
        } QT_CATCH (const std::exception &e) { \
            QTest::qFail(e.what(), __FILE__, __LINE__);\
            return; \
        } \
    } while (false)

class TestQueryBuilders: public QObject
{
    Q_OBJECT

    bool prettyOut = true;

//private slots:
    void testOptional()
    {
        optional<quint32> a;
        *a = 1234;

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

    void testQSharedPointer()
    {
        QSharedPointer<quint32> a = QSharedPointer<quint32>::create();
        *a = 1234;
    }

    void testNullable()
    {
        NULLABLE_TYPE(quint32) a;
        *a = 1234;

        TAPI::setFromVariant(a, QVariant());
    }

    TestTable1 t1;
    TestTable2 t2;

private slots:
    void initTestCase() {
        QT_TRY {
            t1.prepareFiltersList();
            t2.prepareFiltersList();
        } QT_CATCH (const std::exception &e) {
            QTest::qFail(e.what(), __FILE__, __LINE__);
        }
    };

    void condition1() {
        QVERIFY_TRY(clsCondition().isEmpty() == true);
    };
//    void condition2() {
//        QVERIFY_TRY(clsCondition().closePar().isEmpty() == false);
//    };
//    void condition3() {
//        QVERIFY_TRY(clsCondition().openPar(clsCondition({ "cola",  enuConditionOperator::Less, 123 })).isEmpty() == false);
//    };
    void condition4() {
        QVERIFY_TRY(clsCondition().andCond(clsCondition("colA1", enuConditionOperator::Equal, 123)).isEmpty() == false);
    };
    void condition5() {
        QVERIFY_TRY(clsCondition().orCond(clsCondition("colA1", enuConditionOperator::Equal, 123)).isEmpty() == false);
    };
    void condition6() {
        QVERIFY_TRY(clsCondition().xorCond(clsCondition("colA1", enuConditionOperator::Equal, 123)).isEmpty() == false);
    };
    void condition7() {
        QVERIFY_TRY(clsCondition("colA1", enuConditionOperator::Equal, 123)
                    .orCond({ "colB1", enuConditionOperator::Equal, 123 })
                    .andCond(
                        clsCondition("colC1", enuConditionOperator::Equal, 123)
                        .orCond({ "colD1", enuConditionOperator::Equal, 123 })
                        .xorCond(
                            clsCondition("colE1", enuConditionOperator::Equal, 123)
                            .orCond({ "colF1", enuConditionOperator::Equal, 123 })
                        )
                    )
                    .isEmpty() == false);
    };

    void condition_buildString_simple()
    {
        QT_TRY {
            clsCondition cnd =
                clsCondition({ "colA1", enuConditionOperator::Equal, 101 })
            ;

            QString qry = cnd.buildConditionString(t1.Name, t1.FilterableColsMap, false, prettyOut ? 18 : 0);
//            if (prettyOut)
//                qDebug().nospace().noquote() << endl << endl << qry << endl;

            QCOMPARE("\n" + qry + "\n", R"(
t1.colA1 = 101
)");
        } QT_CATCH (const std::exception &e) {
            QTest::qFail(e.what(), __FILE__, __LINE__);
        }
    }

    void condition_buildString_multi()
    {
        QT_TRY {
            clsCondition cnd =
                clsCondition({ "colA1", enuConditionOperator::Equal, 101 })
                .andCond({ "colB1", enuConditionOperator::NotNull })
            ;

            QString qry = cnd.buildConditionString(t1.Name, t1.FilterableColsMap, false, prettyOut ? 18 : 0);
//            if (prettyOut)
//                qDebug().nospace().noquote() << endl << endl << qry << endl;

            QCOMPARE("\n" + qry + "\n", R"(
t1.colA1 = 101
               AND t1.colB1 IS NOT NULL
)");
        } QT_CATCH (const std::exception &e) {
            QTest::qFail(e.what(), __FILE__, __LINE__);
        }
    }

    void condition_buildString_complex()
    {
        QT_TRY {
            clsCondition cnd =
                clsCondition({ "colA1", enuConditionOperator::Equal, 101 })
                .andCond({ "colB1", enuConditionOperator::NotNull })
                .orCond({ "colC1", enuConditionOperator::Equal, 102 })
                .andCond(
                    clsCondition("colD1", enuConditionOperator::Equal, 103)
                    .orCond({ "colE1", enuConditionOperator::Equal, 104 })
                    .xorCond(
                        clsCondition("colF1", enuConditionOperator::Equal, 105)
                        .orCond({ "colG1", enuConditionOperator::Equal, 106 })
                    )
                )
                .orCond({ "colH1", enuConditionOperator::Equal, 107 })
            ;

            QString qry = cnd.buildConditionString(t1.Name, t1.FilterableColsMap, false, prettyOut ? 18 : 0);
//            if (prettyOut)
//                qDebug().nospace().noquote() << endl << endl << qry << endl;

            QCOMPARE("\n" + qry + "\n", R"(
t1.colA1 = 101
               AND t1.colB1 IS NOT NULL
                OR t1.colC1 = 102
               AND (
                   t1.colD1 = 103
                OR t1.colE1 = 104
               XOR (
                   t1.colF1 = 105
                OR t1.colG1 = 106
                   )
                   )
                OR t1.colH1 = 107
)");
        } QT_CATCH (const std::exception &e) {
            QTest::qFail(e.what(), __FILE__, __LINE__);
        }
    }

    void condition_buildString_DBExpression()
    {
//        auto v = QVariant::fromValue(
//                     stuDBExpressionWithValue(
//                         DBExpression::instance()["DATE_ADD"])
//                 .setValue("date", "aaa")
//                 .setValue("interval", "bbbbbbb")
//        );

//        QVariant v1(DBExpression::DATE_ADD(DBExpression::NOW(), "15 Min"));
//        QVariant v2 = DBExpression::DATE_ADD(DBExpression::NOW(), "15 Min");

        QT_TRY {
            clsCondition cnd =
                clsCondition({ "colA1", enuConditionOperator::Equal, DBExpression::DATE_ADD(DBExpression::NOW(), "15 Min") })
                .andCond({ "colB1", enuConditionOperator::Equal, DBExpression::DATE_ADD("colZZ", "45 Min") })
            ;

            QString qry = cnd.buildConditionString(t1.Name, t1.FilterableColsMap, false, prettyOut ? 18 : 0);

//            if (prettyOut)
//                qDebug().nospace().noquote() << endl << endl << qry << endl;

            QCOMPARE("\n" + qry + "\n", R"(
t1.colA1 = DATE_ADD(NOW(),INTERVAL 15 Min)
               AND t1.colB1 = DATE_ADD(colZZ,INTERVAL 45 Min)
)");
        } QT_CATCH (const std::exception &e) {
            QTest::qFail(e.what(), __FILE__, __LINE__);
        }
    }

    /***************************************************************************************/
    /* SelectQuery *************************************************************************/
    /***************************************************************************************/
    void queryString_SELECT_EmptyCol() {
        QT_TRY {
            SelectQuery query = SelectQuery(t1);
            QString qry = query.buildQueryString({}, false, false, prettyOut ? 18 : 0);
//            if (prettyOut)
//                qDebug().nospace().noquote() << endl << endl << qry << endl;

            QCOMPARE("\n" + qry + "\n", R"(
            SELECT t1.colA1
                 , t1.colB1
                 , t1.colC1
                 , t1.colD1
                 , t1.colE1
                 , t1.colF1
                 , t1.colG1
                 , t1.colH1
                 , t1.CURRENT_DATETIME
              FROM test.t1
)");
        } QT_CATCH (const std::exception &e) {
            QTest::qFail(e.what(), __FILE__, __LINE__);
        }
    }

    void queryString_SELECT_simple() {
        QT_TRY {
            SelectQuery query = SelectQuery(t1)
                .addCols(QStringList({
                    "colA1",
                    "colB1",
                    "colC1",
                }))
                .addCol("colD1", "ren_colD1")
            ;
            QString qry = query.buildQueryString({}, false, false, prettyOut ? 18 : 0);
//            if (prettyOut)
//                qDebug().nospace().noquote() << endl << endl << qry << endl;

            QCOMPARE("\n" + qry + "\n", R"(
            SELECT t1.colA1
                 , t1.colB1
                 , t1.colC1
                 , t1.colD1 AS ren_colD1
              FROM test.t1
)");
        } QT_CATCH (const std::exception &e) {
            QTest::qFail(e.what(), __FILE__, __LINE__);
        }
    }

    void queryString_SELECT_simpleWithAlias() {
        QT_TRY {
            SelectQuery query = SelectQuery(t1, "alias_t1")
                .addCols(QStringList({
                    "colA1",
                    "colB1",
                    "colC1",
                }))
                .addCol("colD1", "ren_colD1")
            ;
            QString qry = query.buildQueryString({}, false, false, prettyOut ? 18 : 0);
//            if (prettyOut)
//                qDebug().nospace().noquote() << endl << endl << qry << endl;

            QCOMPARE("\n" + qry + "\n", R"(
            SELECT alias_t1.colA1
                 , alias_t1.colB1
                 , alias_t1.colC1
                 , alias_t1.colD1 AS ren_colD1
              FROM test.t1 alias_t1
)");
        } QT_CATCH (const std::exception &e) {
            QTest::qFail(e.what(), __FILE__, __LINE__);
        }
    }

    void queryString_SELECT_aggregateCols() {
        QT_TRY {
            SelectQuery query = SelectQuery(t1)
//                .addCols(QStringList({
//                    "colA1",
//                    "colB1",
//                    "colC1",
//                }))
                .addCol(enuAggregation::AVG, "colA1", "avg_colA1")
                .addCol(enuAggregation::SUM, "colB1", "sum_colB1")
//                .leftJoin("t2",
//                    clsCondition({ "t2.pk", enuConditionOperator::Equal, "t1.fk" })
//                    .andCond({ "t2.col2", enuConditionOperator::Equal, "123"})
//                )
//                .where({ "colA1", enuConditionOperator::Equal, 123 })
//                .andWhere({ "colB1", enuConditionOperator::GreaterEqual, "abc" })
//                .groupBy(QStringList({ "colC1", "slbStatus" }))
            ;

            QString qry = query.buildQueryString({}, false, false, prettyOut ? 18 : 0);

//            if (prettyOut)
//                qDebug().nospace().noquote() << endl << endl << qry << endl;

            QCOMPARE("\n" + qry + "\n", R"(
            SELECT AVG(t1.colA1) AS avg_colA1
                 , SUM(t1.colB1) AS sum_colB1
              FROM test.t1
)");
        } QT_CATCH (const std::exception &e) {
            QTest::qFail(e.what(), __FILE__, __LINE__);
        }
    }

    void queryString_SELECT_aggregateConditionalCols() {
        QT_TRY {
            SelectQuery query = SelectQuery(t1)
                .addCols(QStringList({
                    "colA1",
                    "colC1",
                }))
                .addCol("colB1", "alias_colB1")
                .addCol(enuConditionalAggregation::COUNTIF,
                        { "colB1", enuConditionOperator::Equal, 123 },
                        "countif_colB"
                        )
                .addCol(enuConditionalAggregation::COUNTIF,
                        clsCondition("colB1", enuConditionOperator::Equal, 123)
                        .andCond({ "colC1", enuConditionOperator::Like, "test it"})
                        .xorCond(
                            clsCondition("colF1", enuConditionOperator::Equal, 105)
                            .orCond({ "colG1", enuConditionOperator::Equal, 106 })
                        )
                        ,
                        "countif_colB1"
                        )
                .addCol(enuConditionalAggregation::SUMIF,
                        clsCondition({ "colC1", enuConditionOperator::Greater, 123 })
                        .andCond({ "t2", "colD2", enuConditionOperator::Equal, 456}),
                        10,
                        20,
                        "sumif__t1_colC1__t2_colD2"
                        )
                .leftJoin(R("test", "t2"))
            ;

            QString qry = query.buildQueryString({}, false, false, prettyOut ? 18 : 0);

//            if (prettyOut)
//                qDebug().nospace().noquote() << endl << endl << qry << endl;

            QCOMPARE("\n" + qry + "\n", R"(
            SELECT t1.colA1
                 , t1.colC1
                 , t1.colB1 AS alias_colB1
                 , COUNT(IF(t1.colB1 = 123,1,NULL)) AS countif_colB
                 , COUNT(IF(
                   t1.colB1 = 123
               AND t1.colC1 LIKE 'test it'
               XOR (
                   t1.colF1 = 105
                OR t1.colG1 = 106
                   )
                   ,1,NULL)) AS countif_colB1
                 , SUM(IF(
                   t1.colC1 > 123
               AND t2.colD2 = 456
                   ,10,20)) AS sumif__t1_colC1__t2_colD2
              FROM test.t1
         LEFT JOIN test.t2
                ON t2.colA2 = t1.colC1
)");
        } QT_CATCH (const std::exception &e) {
            QTest::qFail(e.what(), __FILE__, __LINE__);
        }
    }

    void queryString_SELECT_join() {
        QT_TRY {
            SelectQuery query = SelectQuery(t1)
                .addCols(QStringList({
                    "colA1",
                }))
                .leftJoin("test.t2")
            ;

            QString qry = query.buildQueryString({}, false, false, prettyOut ? 18 : 0);

//            if (prettyOut)
//                qDebug().nospace().noquote() << endl << endl << qry << endl;

            QCOMPARE("\n" + qry + "\n", R"(
            SELECT t1.colA1
              FROM test.t1
         LEFT JOIN test.t2
                ON t2.colA2 = t1.colC1
)");
        } QT_CATCH (const std::exception &e) {
            QTest::qFail(e.what(), __FILE__, __LINE__);
        }
    }

    void queryString_SELECT_join_WithAlias() {
        QT_TRY {
            SelectQuery query = SelectQuery(t1, "alias_t1")
                .addCols(QStringList({
                    "colA1",
//                    "colB1",
//                    "colC1",
//                    "colA2",
                }))
                .leftJoin("test.t2", "alias_t2")
                .leftJoin("test.t2", "alias_2_t2",
                    clsCondition({ "alias_2_t2", "colA2", enuConditionOperator::Equal, "t1", "colB" })
                    .andCond({ "alias_2_t2", "colB2", enuConditionOperator::Equal, "test string" })
                    .andCond({ "t77777777", "ffffff7", enuConditionOperator::Equal, 456})
                    .andCond({ "t77777777", "ffffff8", enuConditionOperator::Equal, "456"})
//                    .andCond({ "t77777777", "ffffff9", enuConditionOperator::Equal, DBExpression::_NULL()})
                )
//                .where({ "colA1", enuConditionOperator::Equal, 123 })
//                .andWhere({ "t2", "colA2", enuConditionOperator::GreaterEqual, "abc" })
//                .groupBy(QStringList({ "colC1", "slbStatus" }))
            ;

            QString qry = query.buildQueryString({}, false, false, prettyOut ? 18 : 0);

//            if (prettyOut)
//                qDebug().nospace().noquote() << endl << endl << qry << endl;

            QCOMPARE("\n" + qry + "\n", R"(
            SELECT alias_t1.colA1
              FROM test.t1 alias_t1
         LEFT JOIN test.t2 alias_t2
                ON alias_t2.colA2 = alias_t1.colC1
         LEFT JOIN test.t2 alias_2_t2
                ON alias_2_t2.colA2 = t1.colB
               AND alias_2_t2.colB2 = 'test string'
               AND t77777777.ffffff7 = 456
               AND t77777777.ffffff8 = '456'
)");
        } QT_CATCH (const std::exception &e) {
            QTest::qFail(e.what(), __FILE__, __LINE__);
        }
    }

    void queryString_SELECT_joinWith() {
        QT_TRY {
            SelectQuery query = SelectQuery(t1)
                .addCols(QStringList({
                    "colA1",
//                    "colB1",
//                    "colC1",
//                    "colA2",
                }))
                .leftJoinWith("rel_a")
//                .leftJoin("t2")
//                .leftJoin("t2", "alias_1_t2")
//                .leftJoin("t2", "alias_2_t2",
//                    clsCondition({ "alias_2_t2", "colA2", enuConditionOperator::Equal, "t1", "colB" })
//                    .andCond({ "alias_2_t2", "colB2", enuConditionOperator::Equal, "test string" })
//                    .andCond({ "t77777777", "ffffff7", enuConditionOperator::Equal, "456"})
//                )
//                .where({ "colA1", enuConditionOperator::Equal, 123 })
//                .andWhere({ "t2", "colA2", enuConditionOperator::GreaterEqual, "abc" })
//                .groupBy(QStringList({ "colC1", "slbStatus" }))
            ;

            QString qry = query.buildQueryString({}, false, false, prettyOut ? 18 : 0);

//            if (prettyOut)
//                qDebug().nospace().noquote() << endl << endl << qry << endl;

            QCOMPARE("\n" + qry + "\n", R"(
            SELECT t1.colA1
              FROM test.t1
         LEFT JOIN test.t2
                ON t2.colA2 = t1.colC1
)");
        } QT_CATCH (const std::exception &e) {
            QTest::qFail(e.what(), __FILE__, __LINE__);
        }
    }

    void queryString_SELECT_joinWith_WithAlias() {
        QT_TRY {
            SelectQuery query = SelectQuery(t1, "alias_t1")
                .addCols(QStringList({
                    "colA1",
//                    "colB1",
//                    "colC1",
//                    "colA2",
                }))
                .leftJoinWith("rel_a", "alias_t2")
//                .leftJoin("t2")
//                .leftJoin("t2", "alias_1_t2")
//                .leftJoin("t2", "alias_2_t2",
//                    clsCondition({ "alias_2_t2", "colA2", enuConditionOperator::Equal, "t1", "colB" })
//                    .andCond({ "alias_2_t2", "colB2", enuConditionOperator::Equal, "test string" })
//                    .andCond({ "t77777777", "ffffff7", enuConditionOperator::Equal, "456"})
//                )
                .where({ "colA1", enuConditionOperator::Equal, 123 })
//                .andWhere({ "t2", "colA2", enuConditionOperator::GreaterEqual, "abc" })
//                .groupBy(QStringList({ "colC1", "slbStatus" }))
            ;

            QString qry = query.buildQueryString({}, false, false, prettyOut ? 18 : 0);

//            if (prettyOut)
//                qDebug().nospace().noquote() << endl << endl << qry << endl;

            QCOMPARE("\n" + qry + "\n", R"(
            SELECT alias_t1.colA1
              FROM test.t1 alias_t1
         LEFT JOIN test.t2 alias_t2
                ON alias_t2.colA2 = alias_t1.colC1
             WHERE alias_t1.colA1 = 123
)");
        } QT_CATCH (const std::exception &e) {
            QTest::qFail(e.what(), __FILE__, __LINE__);
        }
    }

    void queryString_SELECT_autojoin_by_col() {
        QT_TRY {
            SelectQuery query = SelectQuery(t1, "alias_t1")
                .addCols(QStringList({
                    "colA1",
                    "colB1",
                    "colA2",
                }))
            ;

            QString qry = query.buildQueryString({}, false, false, prettyOut ? 18 : 0);

            if (prettyOut)
                qDebug().nospace().noquote() << endl << endl << qry << endl;

            QCOMPARE("\n" + qry + "\n", R"(
                                        CAUTION: NOT COMPLETED
                                        CAUTION: NOT COMPLETED
                                        CAUTION: NOT COMPLETED
                                        CAUTION: NOT COMPLETED
)");
        } QT_CATCH (const std::exception &e) {
            QTest::qFail(e.what(), __FILE__, __LINE__);
        }
    }

    void queryString_SELECT_order_and_group_and_having() {
        QT_TRY {
            SelectQuery query = SelectQuery(t1, "alias_t1")
                .addCols(QStringList({
                    "colA1",
//                    "colB1",
//                    "colC1",
//                    "colA2",
                }))
                .addCol("colB1", "alias_colB1")
                .addCol(enuConditionalAggregation::COUNTIF,
                        { "colB1", enuConditionOperator::Equal, 123 },
                        "countif_colB"
                        )
                .orderBy("colA1")
                .orderBy("colB1", enuOrderDir::Descending)
                .groupBy(QStringList({ "colA1", "slbStatus" }))
                .having(
                    clsCondition("colB1", enuConditionOperator::Equal, 123)
                    .andCond({ "colC1", enuConditionOperator::Like, "test it"})
                    .xorCond(
                        clsCondition("colF1", enuConditionOperator::Equal, 105)
                        .orCond({ "alias_colB1", enuConditionOperator::Equal, 106 })
                    )
                )
                .offset(20)
                .limit(100)
            ;

            QString qry = query.buildQueryString({}, true, false, prettyOut ? 18 : 0);

//            if (prettyOut)
//                qDebug().nospace().noquote() << endl << endl << qry << endl;

            QCOMPARE("\n" + qry + "\n", R"(
            SELECT alias_t1.colA1
                 , alias_t1.colB1 AS alias_colB1
                 , COUNT(IF(alias_t1.colB1 = 123,1,NULL)) AS countif_colB
              FROM test.t1 alias_t1
          ORDER BY colA1
                 , colB1 DESC
          GROUP BY colA1
                 , slbStatus
            HAVING alias_t1.colB1 = 123
               AND alias_t1.colC1 LIKE 'test it'
               XOR (
                   alias_t1.colF1 = 105
                OR alias_colB1 = 106
                   )
             LIMIT 20,1
)");
        } QT_CATCH (const std::exception &e) {
            QTest::qFail(e.what(), __FILE__, __LINE__);
        }
    }

    void queryString_SELECT_COUNT_order_and_group_and_having() {
        QT_TRY {
            SelectQuery query = SelectQuery(t1, "alias_t1")
                .addCols(QStringList({
                    "colA1",
//                    "colB1",
//                    "colC1",
//                    "colA2",
                }))
                .addCol("colB1", "alias_colB1")
                .addCol(enuConditionalAggregation::COUNTIF,
                        { "colB1", enuConditionOperator::Equal, 123 },
                        "countif_colB"
                        )
                .orderBy("colA1")
                .orderBy("colB1", enuOrderDir::Descending)
                .groupBy(QStringList({ "colA1", "slbStatus" }))
                .having(
                    clsCondition("colB1", enuConditionOperator::Equal, 123)
                    .andCond({ "colC1", enuConditionOperator::Like, "test it"})
                    .xorCond(
                        clsCondition("colF1", enuConditionOperator::Equal, 105)
                        .orCond({ "alias_colB1", enuConditionOperator::Equal, 106 })
                    )
                )
                .offset(20)
                .limit(100)
            ;

            QString qry = query.buildQueryString({}, false, true, prettyOut ? 18 : 0);

//            if (prettyOut)
//                qDebug().nospace().noquote() << endl << endl << qry << endl;

            QCOMPARE("\n" + qry + "\n", R"(
            SELECT COUNT(*) AS cnt
              FROM (
            SELECT alias_t1.colA1
                 , alias_t1.colB1 AS alias_colB1
                 , COUNT(IF(alias_t1.colB1 = 123,1,NULL)) AS countif_colB
              FROM test.t1 alias_t1
          ORDER BY colA1
                 , colB1 DESC
          GROUP BY colA1
                 , slbStatus
            HAVING alias_t1.colB1 = 123
               AND alias_t1.colC1 LIKE 'test it'
               XOR (
                   alias_t1.colF1 = 105
                OR alias_colB1 = 106
                   )
                   ) qry
)");
        } QT_CATCH (const std::exception &e) {
            QTest::qFail(e.what(), __FILE__, __LINE__);
        }
    }

    void queryString_SELECT_union() {
        QT_TRY {
            SelectQuery query = SelectQuery(t1, "alias_t1")
                .addCols(QStringList({
                    "colA1",
//                    "colB1",
//                    "colC1",
//                    "colA2",
                }))
                .addCol("colB1", "alias_colB1")
                .addCol(enuConditionalAggregation::COUNTIF,
                        { "colB1", enuConditionOperator::Equal, 123 },
                        "countif_colB"
                        )
                .orderBy("colA1")
                .orderBy("colB1", enuOrderDir::Descending)
                .groupBy(QStringList({ "colA1", "slbStatus" }))
                .having(
                    clsCondition("colB1", enuConditionOperator::Equal, 123)
                    .andCond({ "colC1", enuConditionOperator::Like, "test it"})
                    .xorCond(
                        clsCondition("colF1", enuConditionOperator::Equal, 105)
                        .orCond({ "alias_colB1", enuConditionOperator::Equal, 106 })
                    )
                )
                .offset(20)
                .limit(100)
                .addUnionAll(
                    SelectQuery(t2)
                    .addCol("colA2")
                )
            ;

            QString qry = query.buildQueryString({}, true, false, prettyOut ? 18 : 0);

//            if (prettyOut)
//                qDebug().nospace().noquote() << endl << endl << qry << endl;

            QCOMPARE("\n" + qry + "\n", R"(
            SELECT alias_t1.colA1
                 , alias_t1.colB1 AS alias_colB1
                 , COUNT(IF(alias_t1.colB1 = 123,1,NULL)) AS countif_colB
              FROM test.t1 alias_t1
          ORDER BY colA1
                 , colB1 DESC
          GROUP BY colA1
                 , slbStatus
            HAVING alias_t1.colB1 = 123
               AND alias_t1.colC1 LIKE 'test it'
               XOR (
                   alias_t1.colF1 = 105
                OR alias_colB1 = 106
                   )
             LIMIT 20,1
         UNION ALL
            SELECT t2.colA2
              FROM test.t2
)");
        } QT_CATCH (const std::exception &e) {
            QTest::qFail(e.what(), __FILE__, __LINE__);
        }
    }

    /***************************************************************************************/
    /* CreateQuery *************************************************************************/
    /***************************************************************************************/
    void queryString_CREATE_values_unknown_column() {
        QT_TRY {
            CreateQuery query = CreateQuery(t1) //, "alias_t1")
                .addCol("colA1")
                .addCol("colB1")
                .addCol("colX1")
                .values(QVariantMap({
                    { "colD1", 111 },
                    { "colZ1", "111" },
                    { "colB1", DBExpression::_NULL() },
                }))
            ;

            stuBoundQueryString qry = query.buildQueryString({}, false, prettyOut ? 18 : 0);

//            if (prettyOut) {
//                qDebug().nospace().noquote() << endl
//                                             << endl << "-- Query:" << endl << qry.QueryString << endl
//                                             << endl << "-- Binding Values:" << endl << qry.BindingValues << endl;
//            }

            QCOMPARE("\n" + qry.QueryString + "\n", R"(
            INSERT
              INTO test.t1 (
                   t1.colA1
                 , t1.colB1
                   )
            VALUES (
                   NULL
                 , NULL
                   )
)");
        } QT_CATCH (const std::exception &e) {
            QTest::qFail(e.what(), __FILE__, __LINE__);
        }
    }

    void queryString_CREATE_values_required_column_not_provided() {
        QT_TRY {
            CreateQuery query = CreateQuery(t1) //, "alias_t1")
                .addCol("colA1")
//                .addCol("colB1") -> req
                .addCol("colX1")
                .values(QVariantMap({
                    {"colD1", 111},
                    {"colZ1", "111"},
                    {"colA1", DBExpression::_NULL()},
                }))
            ;

            stuBoundQueryString qry = query.buildQueryString({}, false, prettyOut ? 18 : 0);

//            if (prettyOut) {
//                qDebug().nospace().noquote() << endl
//                                             << endl << "-- Query:" << endl << qry.QueryString << endl
//                                             << endl << "-- Binding Values:" << endl << qry.BindingValues << endl;
//            }

            QCOMPARE("\n" + qry.QueryString + "\n", R"(
MUST BE THROWN AN EXCEPTION
)");
        } QT_CATCH (const std::exception &e) {
            QTest::qFail(e.what(), __FILE__, __LINE__);
        }
    }

    void queryString_CREATE_values_single() {
        QT_TRY {
            CreateQuery query = CreateQuery(t1) //, "alias_t1")
                .addCol("colA1")
                .addCol("colB1")
                .addCol("colC1")
                .addCol("colD1")
                .values(QVariantMap({
                    {"colD1", 111},
                    {"colZ1", "111"},
                    {"colA1", DBExpression::_NULL()},
                }))
            ;

            stuBoundQueryString qry = query.buildQueryString({}, false, prettyOut ? 18 : 0);

//            if (prettyOut) {
//                qDebug().nospace().noquote() << endl
//                                             << endl << "-- Query:" << endl << qry.QueryString << endl
//                                             << endl << "-- Binding Values:" << endl << qry.BindingValues << endl;
//            }

            QCOMPARE("\n" + qry.QueryString + "\n", R"(
MUST BE THROWN AN EXCEPTION
)");
        } QT_CATCH (const std::exception &e) {
            QTest::qFail(e.what(), __FILE__, __LINE__);
        }
    }

    void queryString_CREATE_values_multi() {
        QT_TRY {
            CreateQuery query = CreateQuery(t1) //, "alias_t1")
                .addCol("colA1")
                .addCol("colB1")
                .addCol("colC1")
                .addCol("colD1")
                .values(QVariantMap({
                    {"colB1", 111},
                    {"colZ1", "111"},
                    {"colA1", DBExpression::_NULL()},
                }))
                .values(QVariantMap({
                    {"colB1", 222},
                    {"colZ1", "222"},
                    {"colA1", DBExpression::_NULL()},
                }))
                .values(QList<QVariantMap>({
                    {
                        {"colB1", 333},
                        {"colZ1", "333"},
                        {"colA1", DBExpression::_NULL()},
                    },
                    {
                        {"colB1", 444},
                        {"colZ1", "444"},
                        {"colA1", DBExpression::_NULL()},
                    },
                }))
            ;

            stuBoundQueryString qry = query.buildQueryString({}, false, prettyOut ? 18 : 0);

//            if (prettyOut) {
//                qDebug().nospace().noquote() << endl
//                                             << endl << "-- Query:" << endl << qry.QueryString << endl
//                                             << endl << "-- Binding Values:" << endl << qry.BindingValues << endl;
//            }

            QCOMPARE("\n" + qry.QueryString + "\n", R"(
            INSERT
              INTO test.t1 (
                   t1.colA1
                 , t1.colB1
                 , t1.colC1
                 , t1.colD1
                   )
            VALUES (
                   NULL
                 , 111
                 , NULL
                 , NULL
                   )
                 , (
                   NULL
                 , 222
                 , NULL
                 , NULL
                   )
                 , (
                   NULL
                 , 333
                 , NULL
                 , NULL
                   )
                 , (
                   NULL
                 , 444
                 , NULL
                 , NULL
                   )
)");
        } QT_CATCH (const std::exception &e) {
            QTest::qFail(e.what(), __FILE__, __LINE__);
        }
    }

    void queryString_CREATE_values_multi_use_binding() {
        QT_TRY {
            CreateQuery query = CreateQuery(t1) //, "alias_t1")
                .addCol("colA1")
                .addCol("colB1")
                .addCol("colC1")
                .addCol("colD1")
                .values(QVariantMap({
                    {"colB1", 111},
                    {"colZ1", "111"},
                    {"colA1", DBExpression::_NULL()},
                }))
                .values(QVariantMap({
                    {"colB1", 222},
                    {"colZ1", "222"},
                    {"colA1", DBExpression::_NULL()},
                }))
                .values(QList<QVariantMap>({
                    {
                        {"colB1", 333},
                        {"colZ1", "333"},
                        {"colA1", DBExpression::_NULL()},
                    },
                    {
                        {"colB1", 444},
                        {"colZ1", "444"},
                        {"colA1", DBExpression::_NULL()},
                    },
                }))
            ;

            stuBoundQueryString qry = query.buildQueryString({}, true, prettyOut ? 18 : 0);

            if (prettyOut) {
                qDebug().nospace().noquote() << endl
                                             << endl << "-- Query:" << endl << qry.QueryString << endl
                                             << endl << "-- Binding Values:" << endl << qry.BindingValues << endl;
            }

            QCOMPARE("\n" + qry.QueryString + "\n", R"(
            INSERT
              INTO test.t1 (
                   t1.colA1
                 , t1.colB1
                 , t1.colC1
                 , t1.colD1
                   )
            VALUES (?, ?, ?, ?)
                 , (?, ?, ?, ?)
                 , (?, ?, ?, ?)
                 , (?, ?, ?, ?)
)");
        } QT_CATCH (const std::exception &e) {
            QTest::qFail(e.what(), __FILE__, __LINE__);
        }
    }

    void queryString_CREATE_values_from_select() {
        QT_TRY {
            CreateQuery query = CreateQuery(t1) //, "alias_t1")
                .addCol("colA1")
                .addCol("colB1")
                .addCol("colC1")
                .addCol("colD1")
                .select(SelectQuery(t2)
                    .addCols(QStringList({
                        "colA2",
                        "colB2",
                        "colC2",
                        "colD2",
                    }))
                    .where({ "colE2", enuConditionOperator::Greater, 123 })
                )
            ;

            stuBoundQueryString qry = query.buildQueryString({}, false, prettyOut ? 18 : 0);

//            if (prettyOut) {
//                qDebug().nospace().noquote() << endl
//                                             << endl << "-- Query:" << endl << qry.QueryString << endl
//                                             << endl << "-- Binding Values:" << endl << qry.BindingValues << endl;
//            }

            QCOMPARE("\n" + qry.QueryString + "\n", R"(
            INSERT
              INTO test.t1 (
                   t1.colA1
                 , t1.colB1
                 , t1.colC1
                 , t1.colD1
                   )
            SELECT t2.colA2
                 , t2.colB2
                 , t2.colC2
                 , t2.colD2
              FROM test.t2
             WHERE t2.colE2 > 123
)");
        } QT_CATCH (const std::exception &e) {
            QTest::qFail(e.what(), __FILE__, __LINE__);
        }
    }

    /***************************************************************************************/
    /* UpdateQuery *************************************************************************/
    /***************************************************************************************/
    void queryString_UPDATE_error_on_no_where() {
        QT_TRY {
            UpdateQuery query = UpdateQuery(t1) //, "alias_t1")
                .set("colB", 123)
                .set("colC", "123")
                .setNull("colD")
            ;

            stuBoundQueryString qry = query.buildQueryString({}, false, prettyOut ? 18 : 0);

//            if (prettyOut)
//                qDebug().nospace().noquote() << endl << endl << qry.QueryString << endl;

            QCOMPARE("\n" + qry.QueryString + "\n", R"(
MUST BE THROWN AN EXCEPTION
)");
        } QT_CATCH (const std::exception &e) {
            QTest::qFail(e.what(), __FILE__, __LINE__);
        }
    }

    void queryString_UPDATE_simple() {
        QT_TRY {
            UpdateQuery query = UpdateQuery(t1) //, "alias_t1")
                .set("colB1", 123)
                .set("colC1", "123")
                .setNull("colD1")
                .leftJoinWith("rel_a", "alias_t2")
                .leftJoin("test.t2")
                .where({ "colA1", enuConditionOperator::Equal, 123 })
//                .having({ "aaaaaaa", enuConditionOperator::Greater, 456 })
//                .andHaving({ "bbbbbbb", enuConditionOperator::NotNull })
            ;

            stuBoundQueryString qry = query.buildQueryString({}, false, prettyOut ? 18 : 0);

//            if (prettyOut)
//                qDebug().nospace().noquote() << endl << endl << qry.QueryString << endl;

            QCOMPARE("\n" + qry.QueryString + "\n", R"(
            UPDATE test.t1
         LEFT JOIN test.t2 alias_t2
                ON alias_t2.colA2 = t1.colC1
         LEFT JOIN test.t2
                ON t2.colA2 = t1.colC1
               SET colB1 = 123
                 , colC1 = '123'
                 , colD1 = NULL
             WHERE t1.colA1 = 123
)");
        } QT_CATCH (const std::exception &e) {
            QTest::qFail(e.what(), __FILE__, __LINE__);
        }
    }

   void queryString_UPDATE_simple_use_binding() {
        QT_TRY {
            UpdateQuery query = UpdateQuery(t1) //, "alias_t1")
                .set("colB1", 123)
                .set("colC1", "123")
                .setNull("colD1")
                .leftJoinWith("rel_a", "alias_t2")
                .leftJoin("test.t2")
                .where({ "colA1", enuConditionOperator::Equal, 123 })
                .andWhere({ "colE1", enuConditionOperator::Equal, DBExpression::NOW() })
//                .having({ "aaaaaaa", enuConditionOperator::Greater, 456 })
//                .andHaving({ "bbbbbbb", enuConditionOperator::NotNull })
            ;

            stuBoundQueryString qry = query.buildQueryString({}, true, prettyOut ? 18 : 0);

            if (prettyOut) {
                qDebug().nospace().noquote() << endl
                                             << endl << "-- Query:" << endl << qry.QueryString << endl
                                             << endl << "-- Binding Values:" << endl << qry.BindingValues << endl;
            }

            QCOMPARE("\n" + qry.QueryString + "\n", R"(
            UPDATE test.t1
         LEFT JOIN test.t2 alias_t2
                ON alias_t2.colA2 = t1.colC1
         LEFT JOIN test.t2
                ON t2.colA2 = t1.colC1
               SET colB1 = ?
                 , colC1 = ?
                 , colD1 = ?
             WHERE t1.colA1 = 123
               AND t1.colE1 = NOW()
)");
        } QT_CATCH (const std::exception &e) {
            QTest::qFail(e.what(), __FILE__, __LINE__);
        }
    }

    /***************************************************************************************/
    /* DeleteQuery *************************************************************************/
    /***************************************************************************************/
    void queryString_DELETE_error_on_no_where() {
        QT_TRY {
            DeleteQuery query = DeleteQuery(t1) //, "alias_t1")
            ;

            QString qry = query.buildQueryString({}, prettyOut ? 18 : 0);

            if (prettyOut)
                qDebug().nospace().noquote() << endl << endl << qry << endl;

            QCOMPARE("\n" + qry + "\n", R"(
MUST BE THROWN AN EXCEPTION
)");
        } QT_CATCH (const std::exception &e) {
            QTest::qFail(e.what(), __FILE__, __LINE__);
        }
    }

    void queryString_DELETE_simple() {
        QT_TRY {
            DeleteQuery query = DeleteQuery(t1) //, "alias_t1")
                .where({ "colA1", enuConditionOperator::Equal, 123 })
            ;

            QString qry = query.buildQueryString({}, prettyOut ? 18 : 0);

//            if (prettyOut)
//                qDebug().nospace().noquote() << endl << endl << qry << endl;

            QCOMPARE("\n" + qry + "\n", R"(
            DELETE test.t1
              FROM test.t1
             WHERE t1.colA1 = 123
)");
        } QT_CATCH (const std::exception &e) {
            QTest::qFail(e.what(), __FILE__, __LINE__);
        }
    }

    void queryString_DELETE_join() {
        QT_TRY {
            DeleteQuery query = DeleteQuery(t1) //, "alias_t1")
                .leftJoinWith("rel_a")
                .where({ "colA1", enuConditionOperator::Equal, 123 })
            ;

            QString qry = query.buildQueryString({}, prettyOut ? 18 : 0);

//            if (prettyOut)
//                qDebug().nospace().noquote() << endl << endl << qry << endl;

            QCOMPARE("\n" + qry + "\n", R"(
            DELETE test.t1
              FROM test.t1
         LEFT JOIN test.t2
                ON t2.colA2 = t1.colC1
             WHERE t1.colA1 = 123
)");
        } QT_CATCH (const std::exception &e) {
            QTest::qFail(e.what(), __FILE__, __LINE__);
        }
    }

    void queryString_DELETE_multi_target() {
        QT_TRY {
            DeleteQuery query = DeleteQuery(t1) //, "alias_t1")
                .addTarget("t2")
                .addTarget("t3")
                .addTarget("t4")
                .leftJoinWith("rel_a")
                .where({ "colA1", enuConditionOperator::Equal, 123 })
            ;

            QString qry = query.buildQueryString({}, prettyOut ? 18 : 0);

//            if (prettyOut)
//                qDebug().nospace().noquote() << endl << endl << qry << endl;

            QCOMPARE("\n" + qry + "\n", R"(
            DELETE test.t1
                 , t2
                 , t3
                 , t4
              FROM test.t1
         LEFT JOIN test.t2
                ON t2.colA2 = t1.colC1
             WHERE t1.colA1 = 123
)");
        } QT_CATCH (const std::exception &e) {
            QTest::qFail(e.what(), __FILE__, __LINE__);
        }
    }

};

#endif // QUERYBUILDERS_HPP
