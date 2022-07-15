#ifndef QUERYBUILDERS_HPP
#define QUERYBUILDERS_HPP

#include <QtTest/QtTest>
#include <QStringList>

#include <optional>
using namespace std;

//#include "Interfaces/AAA/Accounting_Interfaces.h"
//using namespace Targoman::API::AAA;

#include "Interfaces/DBM/clsTable.h"
#include "Interfaces/DBM/QueryBuilders.h"
using namespace Targoman::API::DBM;

//#include "Interfaces/API/intfSQLBasedModule.h"

using namespace Targoman;

//namespace A::B {
//class CCC
//{
//};
//}

class TestTable1 : public clsTable
{
public:
    TestTable1() :
        clsTable(
            "test",
            "t1",
            {///ColName    Type                             Validation                 Default     UpBy       Sort    Filter  Self    Virt    PK      Select  RenameAs
                { "colID1",  ORM_PRIMARYKEY_32 },
                { "colA1",   S(qreal),                        QFV.real().minValue(0),    QNull,      UPOwner },
                { "colB1",   S(TAPI::SaleableCode_t),         QFV,                       QRequired,  UPOwner },
                { "colC1",   S(quint32),                      QFV.integer().minValue(1), QNull,      UPOwner },
                { "colD1",   S(QString),                      QFV,                       QNull,      UPOwner },
                { "colE1",   S(TAPI::DateTime_t),             QFV,                       QNull,      UPOwner },
                { "colF1",   S(TAPI::JSON_t),                 QFV,                       QNull,      UPOwner,   true,   true,   false,  false,  false,  true,   "newNamerFor_F1" },
                { "colG1",   S(qreal),                        QFV.real().minValue(0),    QNull,      UPOwner },
                { "colH1",   S(qreal),                        QFV.real().minValue(0),    QNull,      UPOwner },
                { "colI1",   S(QChar),                        QFV,                       QNull,      UPOwner },
                { "status1", ORM_STATUS_FIELD(TAPI::enuGenericStatus, TAPI::enuGenericStatus::Active) },
                { ORM_INVALIDATED_AT_FIELD },
                { "CreatedBy_usrID",  ORM_CREATED_BY },
                { "CreationDateTime", ORM_CREATED_ON },
                { "UpdatedBy_usrID",  ORM_UPDATED_BY },
            },
            {///<               Col     Reference Table  ForeignCol  Rename  LeftJoin
//                           { "colC1", R("test", "t2"), "colA2" },
                { "rel_a",   { "colC1", R("test", "t2"), "colA2" } },
            }
        )
    { ; }

//    ModuleMethods_t listOfMethods() final
//    {
//        return this->Methods;
//    }

//    QString parentModuleName() const
//    {
//        return "aaa";
//    }

};

class TestTable2 : public clsTable
{
public:
    TestTable2() :
        clsTable(
            "test",
            "t2",
            {///ColName Type                             Validation                 Default     UpBy      Sort  Filter Self  Virt   PK
                { "colA2", ORM_PRIMARYKEY_32 },
                { "colB2", S(TAPI::SaleableCode_t),         QFV,                       QRequired,  UPOwner },
                { "colC2", S(quint32),                      QFV.integer().minValue(1), QRequired,  UPOwner },
                { "colD2", S(QString),                      QFV,                       QRequired,  UPOwner },
                { "colE2", S(TAPI::DateTime_t),             QFV,                       QNull,      UPOwner },
                { "colF2", S(TAPI::JSON_t),                 QFV,                       QNull,      UPOwner },
                { "colG2", S(qreal),                        QFV.real().minValue(0),    QRequired,  UPOwner },
                { "colH2", S(qreal),                        QFV.real().minValue(0),    QRequired,  UPOwner },
                { ORM_INVALIDATED_AT_FIELD },
            }
        )
    { ; }

//    ModuleMethods_t listOfMethods() final
//    {
//        return this->Methods;
//    }

//    QString parentModuleName() const
//    {
//        return "aaa";
//    }

};

#define QVERIFY_TRY(expression) \
    do { \
        QT_TRY { \
            QVERIFY(expression); \
        } QT_CATCH (const std::exception &exp) { \
            QTest::qFail(exp.what(), __FILE__, __LINE__);\
            return; \
        } \
    } while (false)


/*
void f1(QVariant v) { ; }
//    void f1(QString v) { ; }
void f1(const DBExpression& v) { ; }
void f1(const clsColSpecs& v) { ; }

void test_overloaded_function() {
    QString s1;
    f1(s1);

    f1("hello");

    quint32 s2;
    f1(s2);

    clsColSpecs s3;
    f1(s3);

    f1(DBExpression::NOW());
}
*/

class testQueryBuilders: public QObject
{
    Q_OBJECT

    APICALLBOOM_TYPE_NO_JWT_DECL APICALLBOOM_PARAM;
    TestTable1 t1;
    TestTable2 t2;
//    TAPI::JWT_t JWT;
    quint64 currentUserID;

private slots:
    void initTestCase() {
        QT_TRY {
            this->currentUserID = 9090;
            t1.prepareFiltersList();
            t2.prepareFiltersList();
        } QT_CATCH (const std::exception &exp) {
            QTest::qFail(exp.what(), __FILE__, __LINE__);
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
                            .orCond({ "newNamerFor_F1", enuConditionOperator::Equal, 123 })
                        )
                    )
                    .isEmpty() == false);
    };

    void condition_buildString_simple() {
        QT_TRY {
            clsCondition cnd =
                clsCondition({ "colA1", enuConditionOperator::Equal, 101 })
            ;

            QStringList RenamedCols;
            QString qry = cnd.buildConditionString(
                              t1.Name,
                              "",
                              t1.SelectableColsMap,
                              t1.FilterableColsMap,
                              false,
                              RenamedCols
                              );
//            if (SQLPrettyLen)
//                qDebug().nospace().noquote() << endl << endl << qry << endl;

            QCOMPARE("\n" + qry + "\n", R"(
t1.colA1 = 101
)");
        } QT_CATCH (const std::exception &exp) {
            QTest::qFail(exp.what(), __FILE__, __LINE__);
        }
    }

    void condition_buildString_multi() {
        QT_TRY {
            clsCondition cnd =
                clsCondition({ "colA1", enuConditionOperator::Equal, 101 })
                .andCond({ "colB1", enuConditionOperator::NotNull })
            ;

            QStringList RenamedCols;
            QString qry = cnd.buildConditionString(
                              t1.Name,
                              "",
                              t1.SelectableColsMap,
                              t1.FilterableColsMap,
                              false,
                              RenamedCols
                              );
//            if (SQLPrettyLen)
//                qDebug().nospace().noquote() << endl << endl << qry << endl;

            QCOMPARE("\n" + qry + "\n", R"(
t1.colA1 = 101
               AND t1.colB1 IS NOT NULL
)");
        } QT_CATCH (const std::exception &exp) {
            QTest::qFail(exp.what(), __FILE__, __LINE__);
        }
    }

    void condition_buildString_complex() {
        QT_TRY {
            clsCondition cnd =
                clsCondition({ "colA1", enuConditionOperator::Equal, 101 })
                .andCond({ "colB1", enuConditionOperator::NotNull })
                .orCond({ "colC1", enuConditionOperator::Equal, 102 })
                .andCond(
                    clsCondition("colD1", enuConditionOperator::Equal, 103)
                    .orCond({ "colE1", enuConditionOperator::Equal, 104 })
                    .xorCond(
                        clsCondition("newNamerFor_F1", enuConditionOperator::Equal, 105)
                        .orCond({ "colG1", enuConditionOperator::Equal, 106 })
                    )
                )
                .orCond({ "colH1", enuConditionOperator::Equal, 107 })
            ;

            QStringList RenamedCols;
            QString qry = cnd.buildConditionString(
                              t1.Name,
                              "",
                              t1.SelectableColsMap,
                              t1.FilterableColsMap,
                              false,
                              RenamedCols
                              );
//            if (SQLPrettyLen)
//                qDebug().nospace().noquote() << endl << endl << qry << endl;

            QCOMPARE("\n" + qry + "\n", R"(
t1.colA1 = 101
               AND t1.colB1 IS NOT NULL
                OR t1.colC1 = 102
               AND (
                   t1.colD1 = 103
                OR t1.colE1 = 104
               XOR (
                   t1.colF1 = ''
                OR t1.colG1 = 106
                   )
                   )
                OR t1.colH1 = 107
)");
        } QT_CATCH (const std::exception &exp) {
            QTest::qFail(exp.what(), __FILE__, __LINE__);
        }
    }

    void condition_buildString_DBExpression() {
//        auto v = QVariant::fromValue(
//                     stuDBExpressionWithValue(
//                         DBExpression::instance()["DATE_ADD"])
//                 .setValue("date", "aaa")
//                 .setValue("interval", "bbbbbbb")
//        );

//        QVariant v1(DBExpression::DATE_ADD(DBExpression::NOW(), 15, enuDBExpressionIntervalUnit::MINUTE));
//        QVariant v2 = DBExpression::DATE_ADD(DBExpression::NOW(), 15, enuDBExpressionIntervalUnit::MINUTE);

        QT_TRY {
            clsCondition cnd =
                clsCondition({ "colA1", enuConditionOperator::Equal, DBExpression::DATE_ADD(DBExpression::NOW(), 15, enuDBExpressionIntervalUnit::MINUTE) })
                .andCond({ "colB1", enuConditionOperator::Equal, DBExpression::DATE_ADD("colZZ", 45, enuDBExpressionIntervalUnit::MINUTE) })
                .andCond({ "colC1", enuConditionOperator::Equal, DBExpression::DATE_ADD("colZZ", "1:2", enuDBExpressionIntervalUnit::MINUTE_SECOND) })
            ;

            QStringList RenamedCols;
            QString qry = cnd.buildConditionString(
                              t1.Name,
                              "",
                              t1.SelectableColsMap,
                              t1.FilterableColsMap,
                              false,
                              RenamedCols
                              );

//            if (SQLPrettyLen)
//                qDebug().nospace().noquote() << endl << endl << qry << endl;

            QCOMPARE("\n" + qry + "\n", R"(
t1.colA1 = DATE_ADD(NOW(),INTERVAL 15 MINUTE)
               AND t1.colB1 = DATE_ADD(colZZ,INTERVAL 45 MINUTE)
               AND t1.colC1 = DATE_ADD(colZZ,INTERVAL '1:2' MINUTE_SECOND)
)");
        } QT_CATCH (const std::exception &exp) {
            QTest::qFail(exp.what(), __FILE__, __LINE__);
        }
    }

//    void condition_buildString_parse()
//    {
//        QT_TRY {
//            QString filters = "( colA1>=NOW() | colB1<DATE_ADD(NOW(),INTERVAL$SPACE$15$SPACE$Min) )";

//            clsCondition cnd = clsCondition::parse(filters, t1);

//            QString qry = cnd.buildConditionString(
//                        t1.Name,
//                        t1.SelectableColsMap,
//                        t1.FilterableColsMap,
//                        false
//                        );

//            if (SQLPrettyLen)
//                qDebug().nospace().noquote() << endl
//                                             << endl << "-- filters:" << endl << filters << endl
//                                             << endl << "-- tostring:" << endl << qry << endl;

//            QCOMPARE("\n" + qry + "\n", R"(
//?????????????????????
//)");
//        } QT_CATCH (const std::exception &exp) {
//            QTest::qFail(exp.what(), __FILE__, __LINE__);
//        }
//    }

    /***************************************************************************************/
    /* SelectQuery *************************************************************************/
    /***************************************************************************************/
    void queryString_SELECT_EmptyCol() {
        QT_TRY {
            ORMSelectQuery query = t1.GetSelectQuery(APICALLBOOM_PARAM)
            ;

            QString qry = query.buildQueryString({}, false, false, true);

//            if (SQLPrettyLen)
//                qDebug().nospace().noquote() << endl << endl << qry << endl;

            QCOMPARE("\n" + qry + "\n", R"(
            SELECT t1.colID1
                 , t1.colA1
                 , t1.colB1
                 , t1.colC1
                 , t1.colD1
                 , t1.colE1
                 , t1.colF1 AS `newNamerFor_F1`
                 , t1.colG1
                 , t1.colH1
                 , t1.colI1
                 , t1.status1
                 , t1.CreatedBy_usrID
                 , t1.CreationDateTime
                 , t1.UpdatedBy_usrID
                 , CURRENT_TIMESTAMP() AS `CURRENT_TIMESTAMP`
              FROM test.t1
             WHERE t1.status1 != 'R'
               AND t1._InvalidatedAt = 0
)");
        } QT_CATCH (const std::exception &exp) {
            QTest::qFail(exp.what(), __FILE__, __LINE__);
        }
    }

    void queryString_SELECT_EmptyCol_with_where() {
        QT_TRY {
            ORMSelectQuery query = t1.GetSelectQuery(APICALLBOOM_PARAM)
                .where({ "colA1", enuConditionOperator::Equal, 123 })
                .andWhere({ "colB1", enuConditionOperator::Equal, 456 })
            ;

            QString qry = query.buildQueryString({}, false, false, true);

//            if (SQLPrettyLen)
//                qDebug().nospace().noquote() << endl << endl << qry << endl;

            QCOMPARE("\n" + qry + "\n", R"(
            SELECT t1.colID1
                 , t1.colA1
                 , t1.colB1
                 , t1.colC1
                 , t1.colD1
                 , t1.colE1
                 , t1.colF1 AS `newNamerFor_F1`
                 , t1.colG1
                 , t1.colH1
                 , t1.colI1
                 , t1.status1
                 , t1.CreatedBy_usrID
                 , t1.CreationDateTime
                 , t1.UpdatedBy_usrID
                 , CURRENT_TIMESTAMP() AS `CURRENT_TIMESTAMP`
              FROM test.t1
             WHERE t1.status1 != 'R'
               AND t1._InvalidatedAt = 0
               AND (
                   t1.colA1 = 123
               AND t1.colB1 = 456
                   )
)");
        } QT_CATCH (const std::exception &exp) {
            QTest::qFail(exp.what(), __FILE__, __LINE__);
        }
    }

    void queryString_SELECT_simple() {
        QT_TRY {
            ORMSelectQuery query = t1.GetSelectQuery(APICALLBOOM_PARAM)
                .addCols(QStringList({
                    "colA1",
                    "colB1",
                    "colC1",
                }))
                .addCol("colD1", "ren_colD1")
            ;

            QString qry = query.buildQueryString({}, false, false, true);

//            if (SQLPrettyLen)
//                qDebug().nospace().noquote() << endl << endl << qry << endl;

            QCOMPARE("\n" + qry + "\n", R"(
            SELECT t1.colA1
                 , t1.colB1
                 , t1.colC1
                 , t1.colD1 AS `ren_colD1`
              FROM test.t1
             WHERE t1.status1 != 'R'
               AND t1._InvalidatedAt = 0
)");
        } QT_CATCH (const std::exception &exp) {
            QTest::qFail(exp.what(), __FILE__, __LINE__);
        }
    }

    void queryString_SELECT_simpleWithAlias() {
        QT_TRY {
            ORMSelectQuery query = t1.GetSelectQuery(APICALLBOOM_PARAM, "alias_t1")
                .addCols(QStringList({
                    "colA1",
                    "colB1",
                    "colC1",
                }))
                .addCol("colD1", "ren_colD1")
            ;

            QString qry = query.buildQueryString({}, false, false, true);

//            if (SQLPrettyLen)
//                qDebug().nospace().noquote() << endl << endl << qry << endl;

            QCOMPARE("\n" + qry + "\n", R"(
            SELECT alias_t1.colA1
                 , alias_t1.colB1
                 , alias_t1.colC1
                 , alias_t1.colD1 AS `ren_colD1`
              FROM test.t1 alias_t1
             WHERE alias_t1.status1 != 'R'
               AND alias_t1._InvalidatedAt = 0
)");
        } QT_CATCH (const std::exception &exp) {
            QTest::qFail(exp.what(), __FILE__, __LINE__);
        }
    }

    void queryString_SELECT_aggregateCols() {
        QT_TRY {
            ORMSelectQuery query = t1.GetSelectQuery(APICALLBOOM_PARAM)
                .addCol(enuAggregation::AVG, "colA1", "avg_colA1")
                .addCol(enuAggregation::SUM, "colB1", "sum_colB1")
            ;

            QString qry = query.buildQueryString({}, false, false, true);

//            if (SQLPrettyLen)
//                qDebug().nospace().noquote() << endl << endl << qry << endl;

            QCOMPARE("\n" + qry + "\n", R"(
            SELECT AVG(t1.colA1) AS `avg_colA1`
                 , SUM(t1.colB1) AS `sum_colB1`
              FROM test.t1
             WHERE t1.status1 != 'R'
               AND t1._InvalidatedAt = 0
)");
        } QT_CATCH (const std::exception &exp) {
            QTest::qFail(exp.what(), __FILE__, __LINE__);
        }
    }

    void queryString_SELECT_aggregateConditionalCols() {
        QT_TRY {
            ORMSelectQuery query = t1.GetSelectQuery(APICALLBOOM_PARAM)
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
                            clsCondition("newNamerFor_F1", enuConditionOperator::Equal, 105)
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
                .leftJoin("t2")
            ;

            QString qry = query.buildQueryString({}, false, false, true);

//            if (SQLPrettyLen)
//                qDebug().nospace().noquote() << endl << endl << qry << endl;

            QCOMPARE("\n" + qry + "\n", R"(
            SELECT t1.colA1
                 , t1.colC1
                 , t1.colB1 AS `alias_colB1`
                 , COUNT(IF (t1.colB1 = 123,1,NULL)) AS `countif_colB`
                 , COUNT(IF (
                   t1.colB1 = 123
               AND t1.colC1 LIKE 'test it'
               XOR (
                   t1.colF1 = ''
                OR t1.colG1 = 106
                   )
                   ,1,NULL)) AS `countif_colB1`
                 , SUM(IF (
                   t1.colC1 > 123
               AND t2.colD2 = 456
                   ,10,20)) AS `sumif__t1_colC1__t2_colD2`
              FROM test.t1
         LEFT JOIN test.t2
                ON t2.colA2 = t1.colC1
             WHERE t1.status1 != 'R'
               AND t1._InvalidatedAt = 0
)");
        } QT_CATCH (const std::exception &exp) {
            QTest::qFail(exp.what(), __FILE__, __LINE__);
        }
    }

    void queryString_SELECT_join() {
        QT_TRY {
            ORMSelectQuery query = t1.GetSelectQuery(APICALLBOOM_PARAM
                                            , "________"
                                            )
                .addCols(QStringList({
                    "colA1",
                    "colA2",
//                    "newNamerFor_F1",
                    "ZZZZZZZZZ.colA2",
                }))
                .addCol("newNamerFor_F1")
                .addCol("newNamerFor_F1", "OTHER_F1_NAME")
                .addCol(enuAggregation::SUM, "newNamerFor_F1", "SUM_F1_NAME")
                .addCol(Targoman::API::CURRENT_TIMESTAMP)
                .leftJoin("t2")
                .leftJoin("t2", "ZZZZZZZZZ")
                .andWhere({ "newNamerFor_F1",   enuConditionOperator::Equal, QVariantMap({ { "wa1", "wb1" } }) })
//                .andWhere({ "OTHER_F1_NAME",    enuConditionOperator::Equal, QVariantMap({ { "wa2", "wb2" } }) })
//                .andWhere({ "SUM_F1_NAME",      enuConditionOperator::Equal, QVariantMap({ { "wa3", "wb3" } }) })
                .groupBy("newNamerFor_F1")
                .andHaving({ "newNamerFor_F1",  enuConditionOperator::Equal, QVariantMap({ { "ha1", "hb1" } }) })
                .andHaving({ "OTHER_F1_NAME",   enuConditionOperator::Equal, QVariantMap({ { "ha2", "hb2" } }) })
                .andHaving({ "SUM_F1_NAME",     enuConditionOperator::Equal, QVariantMap({ { "ha3", "hb3" } }) })
                .orderBy("newNamerFor_F1")
            ;

            QString qry = query.buildQueryString({}, false, false, true);

//            if (SQLPrettyLen)
//                qDebug().nospace().noquote() << endl << endl << qry << endl;

            QCOMPARE("\n" + qry + "\n", R"(
            SELECT ________.colA1
                 , t2.colA2
                 , ZZZZZZZZZ.colA2
                 , ________.colF1 AS `newNamerFor_F1`
                 , ________.colF1 AS `OTHER_F1_NAME`
                 , SUM(________.colF1) AS `SUM_F1_NAME`
                 , CURRENT_TIMESTAMP() AS `CURRENT_TIMESTAMP`
              FROM test.t1 ________
         LEFT JOIN test.t2
                ON t2.colA2 = ________.colC1
         LEFT JOIN test.t2 ZZZZZZZZZ
                ON ZZZZZZZZZ.colA2 = ________.colC1
             WHERE ________.status1 != 'R'
               AND ________._InvalidatedAt = 0
               AND (
                   ________.colF1 = '{"wa1":"wb1"}'
                   )
          GROUP BY newNamerFor_F1
            HAVING ________.colF1 = '{"ha1":"hb1"}'
               AND OTHER_F1_NAME = '{"ha2":"hb2"}'
               AND SUM_F1_NAME = '{"ha3":"hb3"}'
          ORDER BY newNamerFor_F1
)");
        } QT_CATCH (const std::exception &exp) {
            QTest::qFail(exp.what(), __FILE__, __LINE__);
        }
    }
//private:
    void queryString_SELECT_join_WithAlias() {
        QT_TRY {
            ORMSelectQuery query = t1.GetSelectQuery(APICALLBOOM_PARAM, "alias_t1")
                .addCols(QStringList({
                    "colA1",
                }))
                .leftJoin("test.t2", "alias_t2")
                .leftJoin("test.t2", "alias_2_t2",
                    clsCondition({ "alias_2_t2", "colA2", enuConditionOperator::Equal, "t1", "colB1" })
                    .andCond({ "alias_2_t2", "colB2", enuConditionOperator::Equal, "test string" })
//                    .andCond({ "t77777777", "ffffff7", enuConditionOperator::Equal, 456})
//                    .andCond({ "t77777777", "ffffff8", enuConditionOperator::Equal, "456"})
                )
            ;

            QString qry = query.buildQueryString({}, false, false, true);

//            if (SQLPrettyLen)
//                qDebug().nospace().noquote() << endl << endl << qry << endl;

            QCOMPARE("\n" + qry + "\n", R"(
            SELECT alias_t1.colA1
              FROM test.t1 alias_t1
         LEFT JOIN test.t2 alias_t2
                ON alias_t2.colA2 = alias_t1.colC1
         LEFT JOIN test.t2 alias_2_t2
                ON alias_2_t2.colA2 = t1.colB1
               AND alias_2_t2.colB2 = 'test string'
             WHERE alias_t1.status1 != 'R'
               AND alias_t1._InvalidatedAt = 0
)");
        } QT_CATCH (const std::exception &exp) {
            QTest::qFail(exp.what(), __FILE__, __LINE__);
        }
    }

    void queryString_SELECT_joinWith() {
        QT_TRY {
            ORMSelectQuery query = t1.GetSelectQuery(APICALLBOOM_PARAM)
                .addCols(QStringList({
                    "colA1",
                }))
                .leftJoinWith("rel_a")
            ;

            QString qry = query.buildQueryString({}, false, false, true);

//            if (SQLPrettyLen)
//                qDebug().nospace().noquote() << endl << endl << qry << endl;

            QCOMPARE("\n" + qry + "\n", R"(
            SELECT t1.colA1
              FROM test.t1
         LEFT JOIN test.t2
                ON t2.colA2 = t1.colC1
             WHERE t1.status1 != 'R'
               AND t1._InvalidatedAt = 0
)");
        } QT_CATCH (const std::exception &exp) {
            QTest::qFail(exp.what(), __FILE__, __LINE__);
        }
    }

    void queryString_SELECT_joinWith_WithAlias() {
        QT_TRY {
            ORMSelectQuery query = t1.GetSelectQuery(APICALLBOOM_PARAM, "alias_t1")
                .addCols(QStringList({
                    "colA1",
                }))
                .leftJoinWith("rel_a", "alias_t2")
                .where({ "colA1", enuConditionOperator::Equal, 123 })
            ;

            QString qry = query.buildQueryString({}, false, false, true);

//            if (SQLPrettyLen)
//                qDebug().nospace().noquote() << endl << endl << qry << endl;

            QCOMPARE("\n" + qry + "\n", R"(
            SELECT alias_t1.colA1
              FROM test.t1 alias_t1
         LEFT JOIN test.t2 alias_t2
                ON alias_t2.colA2 = alias_t1.colC1
             WHERE alias_t1.status1 != 'R'
               AND alias_t1._InvalidatedAt = 0
               AND (
                   alias_t1.colA1 = 123
                   )
)");
        } QT_CATCH (const std::exception &exp) {
            QTest::qFail(exp.what(), __FILE__, __LINE__);
        }
    }

    void queryString_SELECT_autojoin_by_col() {
        QT_TRY {
            ORMSelectQuery query = t1.GetSelectQuery(APICALLBOOM_PARAM, "alias_t1")
                .addCols(QStringList({
                    "colA1",
                    "colB1",
                    "colA2",
                }))
            ;

            QString qry = query.buildQueryString({}, false, false, true);

//            if (SQLPrettyLen)
//                qDebug().nospace().noquote() << endl << endl << qry << endl;

            QCOMPARE("\n" + qry + "\n", R"(
                                        CAUTION: NOT COMPLETED
                                        CAUTION: NOT COMPLETED
                                        CAUTION: NOT COMPLETED
                                        CAUTION: NOT COMPLETED
)");
        } QT_CATCH (const std::exception &exp) {
            QTest::qFail(exp.what(), __FILE__, __LINE__);
        }
    }

    void queryString_SELECT_order_and_group_and_having() {
        QT_TRY {
            ORMSelectQuery query = t1.GetSelectQuery(APICALLBOOM_PARAM, "alias_t1")
                .addCols(QStringList({
                    "colA1",
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
                        clsCondition("newNamerFor_F1", enuConditionOperator::Equal, 105)
                        .orCond({ "alias_colB1", enuConditionOperator::Equal, 106 })
                    )
                )
                .pageIndex(20)
                .pageSize(100)
            ;

            QString qry = query.buildQueryString({}, true, false, true);

//            if (SQLPrettyLen)
//                qDebug().nospace().noquote() << endl << endl << qry << endl;

            QCOMPARE("\n" + qry + "\n", R"(
            SELECT alias_t1.colA1
                 , alias_t1.colB1 AS `alias_colB1`
                 , COUNT(IF (alias_t1.colB1 = 123,1,NULL)) AS `countif_colB`
              FROM test.t1 alias_t1
             WHERE alias_t1.status1 != 'R'
               AND alias_t1._InvalidatedAt = 0
          GROUP BY colA1
                 , slbStatus
            HAVING alias_t1.colB1 = 123
               AND alias_t1.colC1 LIKE 'test it'
               XOR (
                   alias_t1.colF1 = ''
                OR alias_colB1 = 106
                   )
          ORDER BY colA1
                 , colB1 DESC
             LIMIT 2000,100
)");
        } QT_CATCH (const std::exception &exp) {
            QTest::qFail(exp.what(), __FILE__, __LINE__);
        }
    }

    void queryString_SELECT_COUNT_order_and_group_and_having() {
        QT_TRY {
            ORMSelectQuery query = t1.GetSelectQuery(APICALLBOOM_PARAM, "alias_t1")
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
                        clsCondition("newNamerFor_F1", enuConditionOperator::Equal, 105)
                        .orCond({ "alias_colB1", enuConditionOperator::Equal, 106 })
                    )
                )
                .pageIndex(20)
                .pageSize(100)
            ;

            QString qry = query.buildQueryString({}, false, true, true);

//            if (SQLPrettyLen)
//                qDebug().nospace().noquote() << endl << endl << qry << endl;

            QCOMPARE("\n" + qry + "\n", R"(
            SELECT COUNT(*) AS `cnt`
              FROM (
            SELECT alias_t1.colA1
                 , alias_t1.colB1 AS `alias_colB1`
                 , COUNT(IF (alias_t1.colB1 = 123,1,NULL)) AS `countif_colB`
              FROM test.t1 alias_t1
             WHERE alias_t1.status1 != 'R'
               AND alias_t1._InvalidatedAt = 0
          GROUP BY colA1
                 , slbStatus
            HAVING alias_t1.colB1 = 123
               AND alias_t1.colC1 LIKE 'test it'
               XOR (
                   alias_t1.colF1 = ''
                OR alias_colB1 = 106
                   )
          ORDER BY colA1
                 , colB1 DESC
                   ) `qry`
)");
        } QT_CATCH (const std::exception &exp) {
            QTest::qFail(exp.what(), __FILE__, __LINE__);
        }
    }

    void queryString_SELECT_union() {
        QT_TRY {
            ORMSelectQuery query = t1.GetSelectQuery(APICALLBOOM_PARAM, "alias_t1")
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
                        clsCondition("newNamerFor_F1", enuConditionOperator::Equal, 105)
                        .orCond({ "alias_colB1", enuConditionOperator::Equal, 106 })
                    )
                )
                .pageIndex(20)
                .pageSize(100)
                .addUnionAll(
                    t2.GetSelectQuery(APICALLBOOM_PARAM)
                    .addCol("colA2")
                )
            ;

            QString qry = query.buildQueryString({}, true, false, true);

//            if (SQLPrettyLen)
//                qDebug().nospace().noquote() << endl << endl << qry << endl;

            QCOMPARE("\n" + qry + "\n", R"(
            SELECT alias_t1.colA1
                 , alias_t1.colB1 AS `alias_colB1`
                 , COUNT(IF (alias_t1.colB1 = 123,1,NULL)) AS `countif_colB`
              FROM test.t1 alias_t1
             WHERE alias_t1.status1 != 'R'
               AND alias_t1._InvalidatedAt = 0
          GROUP BY colA1
                 , slbStatus
            HAVING alias_t1.colB1 = 123
               AND alias_t1.colC1 LIKE 'test it'
               XOR (
                   alias_t1.colF1 = ''
                OR alias_colB1 = 106
                   )
          ORDER BY colA1
                 , colB1 DESC
             LIMIT 2000,100
         UNION ALL
            SELECT t2.colA2
              FROM test.t2
             WHERE t2._InvalidatedAt = 0
)");
        } QT_CATCH (const std::exception &exp) {
            QTest::qFail(exp.what(), __FILE__, __LINE__);
        }
    }

    void queryString_SELECT_filters() {
        QT_TRY {
            ORMSelectQuery query = t1.GetSelectQuery(APICALLBOOM_PARAM, "alias_t1")
                .addCols(QStringList({
                    "colA1",
                    "colB1",
                }))
                .addFilters("( colC1<123 + CreationDateTime>=NOW() | colE1<DATE_ADD(NOW(),INTERVAL$SPACE$15$SPACE$Min) )")
            ;

            QString qry = query.buildQueryString({}, true, false, true);

//            if (SQLPrettyLen)
//                qDebug().nospace().noquote() << endl << endl << qry << endl;

            QCOMPARE("\n" + qry + "\n", R"(
            SELECT alias_t1.colA1
                 , alias_t1.colB1
              FROM test.t1 alias_t1
             WHERE alias_t1.status1 != 'R'
               AND alias_t1._InvalidatedAt = 0
               AND (
                   (
                   alias_t1.colC1 < 123
               AND alias_t1.CreationDateTime >= NOW()
                OR alias_t1.colE1 < DATE_ADD(NOW(),INTERVAL 15 Min)
                   )
                   )
             LIMIT 0,1
)");
        } QT_CATCH (const std::exception &exp) {
            QTest::qFail(exp.what(), __FILE__, __LINE__);
        }
    }

    void queryString_SELECT_nested_join() {
        QT_TRY {
            ORMSelectQuery query = t1.GetSelectQuery(APICALLBOOM_PARAM, "alias_t1")
                .addCols(QStringList({
                    "colA1",
                }))
                .nestedLeftJoin(t2.GetSelectQuery(APICALLBOOM_PARAM)
                    .addCol("colB2")
                    .addCol("colC2")
                    .addCol(enuAggregation::COUNT, "colA2", "cnt")
                    .groupBy("colB2")
                    .groupBy("colC2")
                    , "tmp_t2_count"
                    , { "tmp_t2_count", "colB2", enuConditionOperator::Equal, "alias_t1", "colB1" })
                .addCol("tmp_t2_count.cnt")
            ;

            QString qry = query.buildQueryString({}, true, false, true);

//            if (SQLPrettyLen)
//                qDebug().nospace().noquote() << endl << endl << qry << endl;

            QCOMPARE("\n" + qry + "\n", R"(
            SELECT alias_t1.colA1
                 , tmp_t2_count.cnt
              FROM test.t1 alias_t1
         LEFT JOIN (
            SELECT t2.colB2
                 , t2.colC2
                 , COUNT(t2.colA2) AS `cnt`
              FROM test.t2
             WHERE t2._InvalidatedAt = 0
          GROUP BY colB2
                 , colC2
                   ) tmp_t2_count
                ON tmp_t2_count.colB2 = alias_t1.colB1
             WHERE alias_t1.status1 != 'R'
               AND alias_t1._InvalidatedAt = 0
             LIMIT 0,1
)");
        } QT_CATCH (const std::exception &exp) {
            QTest::qFail(exp.what(), __FILE__, __LINE__);
        }
    }

    void queryString_SELECT_colspec_as_condition() {
        QT_TRY {
            ORMSelectQuery query = t1.GetSelectQuery(APICALLBOOM_PARAM, "alias_t1")
                .addCols(QStringList({
                    "colA1",
                }))
                .andWhere({
                              { enuAggregation::SUM, "colB1" },
                              enuConditionOperator::GreaterEqual,
                              123
                          })
                .andWhere({
                              { enuAggregation::SUM, "colC1" },
                              enuConditionOperator::GreaterEqual,
                              clsColSpecs(enuAggregation::SUM, "colD1")
                          })
                .andWhere({
                              "colE1",
                              enuConditionOperator::GreaterEqual,
                              clsColSpecs(enuAggregation::SUM, "newNamerFor_F1")
                          })
                .andWhere({
                              { enuAggregation::LOWER, "colC1" },
                              enuConditionOperator::Equal,
                              "abcdef"
                          })
            ;

            QString qry = query.buildQueryString({}, true, false, true);

//            if (SQLPrettyLen)
//                qDebug().nospace().noquote() << endl << endl << qry << endl;

            QCOMPARE("\n" + qry + "\n", R"(
            SELECT alias_t1.colA1
              FROM test.t1 alias_t1
             WHERE alias_t1.status1 != 'R'
               AND alias_t1._InvalidatedAt = 0
               AND (
                   SUM(alias_t1.colB1) >= 123
               AND SUM(alias_t1.colC1) >= SUM(alias_t1.colD1)
               AND alias_t1.colE1 >= SUM(alias_t1.colF1)
               AND LOWER(alias_t1.colC1) = 'abcdef'
                   )
             LIMIT 0,1
)");
        } QT_CATCH (const std::exception &exp) {
            QTest::qFail(exp.what(), __FILE__, __LINE__);
        }
    }

    /***************************************************************************************/
    /* CreateQuery *************************************************************************/
    /***************************************************************************************/
    void queryString_CREATE_values_unknown_column() {
        QT_TRY {
            ORMCreateQuery query = t1.GetCreateQuery(APICALLBOOM_PARAM) //, "alias_t1")
                .addCol("colA1")
                .addCol("colX1")
                .addCol("colB1")
                .values(QVariantMap({
                    { "colD1", 111 },
                    { "colZ1", "111" },
                    { "colB1", DBExpression::NIL() },
                }))
            ;

            QVERIFY_EXCEPTION_THROWN(
                        query.buildQueryString(this->currentUserID, {}, false),
                        exQueryBuilderColumnNotFound);

        } QT_CATCH (const std::exception &exp) {
            QTest::qFail(exp.what(), __FILE__, __LINE__);
        }
    }

    void queryString_CREATE_values_required_column_not_provided() {
        QT_TRY {
            ORMCreateQuery query = t1.GetCreateQuery(APICALLBOOM_PARAM) //, "alias_t1")
                .addCol("colA1")
//                .addCol("colB1") -> req
//                .addCol("colX1")
                .values(QVariantMap({
                    {"colD1", 111},
                    {"colZ1", "111"},
                    {"colA1", DBExpression::NIL()},
                }))
            ;

            QVERIFY_EXCEPTION_THROWN(
                        query.buildQueryString(this->currentUserID, {}, false),
                        exQueryBuilderColumnNotProvided);

        } QT_CATCH (const std::exception &exp) {
            QTest::qFail(exp.what(), __FILE__, __LINE__);
        }
    }

    void queryString_CREATE_values_single() {
        QT_TRY {
            ORMCreateQuery query = t1.GetCreateQuery(APICALLBOOM_PARAM) //, "alias_t1")
                .addCol("colC1")
                .addCol("colB1")
                .addCol("colD1")
                .addCol("colA1")
                .addCol("newNamerFor_F1")
                .values(QVariantMap({
                    { "colD1", "hasan" },
                    { "colZ1", "222" },
                    { "colA1", DBExpression::NIL() },
                    { "colB1", DBExpression::NOW() },
                    { "colC1", DBExpression::CURDATE() },
                    { "newNamerFor_F1", QVariantMap({ { "a", "b" }, { "c", "d" } }) },
                }))
            ;

            stuBoundQueryString qry = query.buildQueryString(this->currentUserID, {}, false);

//            if (SQLPrettyLen)
//                qDebug().nospace().noquote() << endl
//                                             << endl << "-- Query:" << endl << qry.QueryString << endl;

            QCOMPARE("\n" + qry.QueryString + "\n", R"(
            INSERT
              INTO test.t1
                   (
                   t1.colC1
                 , t1.colB1
                 , t1.colD1
                 , t1.colA1
                 , t1.colF1
                 , t1.status1
                 , t1.CreatedBy_usrID
                   )
            VALUES (
                   CURDATE()
                 , NOW()
                 , 'hasan'
                 , NULL
                 , '{"a":"b","c":"d"}'
                 , 'A'
                 , 9090
                   )
)");
        } QT_CATCH (const std::exception &exp) {
            QTest::qFail(exp.what(), __FILE__, __LINE__);
        }
    }

    void queryString_CREATE_with_options() {
        QT_TRY {
            ORMCreateQuery query = t1.GetCreateQuery(APICALLBOOM_PARAM) //, "alias_t1")
                .options_ignore()
                .addCol("colA1")
                .addCol("colB1")
                .values(QVariantMap({
                    {"colB1", 111},
                    {"colA1", DBExpression::NIL()},
                }))
            ;

            stuBoundQueryString qry = query.buildQueryString(this->currentUserID, {}, true);

            QStringList BindingValuesList;
            foreach (auto b, qry.BindingValues) {
                BindingValuesList.append(b.toString());
            }

//            if (SQLPrettyLen) {
//                qDebug().nospace().noquote() << endl
//                                             << endl << "-- Query:" << endl << qry.QueryString << endl
//                                             << endl << "-- Binding Values:" << endl << qry.BindingValues << endl
//                                             << "-- [" << BindingValuesList.join(", ") << "]" << endl;
//            }

            QCOMPARE("\n" + qry.QueryString + "\n", R"(
            INSERT IGNORE
              INTO test.t1
                   (
                   t1.colA1
                 , t1.colB1
                 , t1.status1
                 , t1.CreatedBy_usrID
                   )
            VALUES (NULL, ?, ?, ?)
)");

            QCOMPARE(BindingValuesList.join(", "), "111, A, 9090");
        } QT_CATCH (const std::exception &exp) {
            QTest::qFail(exp.what(), __FILE__, __LINE__);
        }
    }

    void queryString_CREATE_values_multi() {
        QT_TRY {
            ORMCreateQuery query = t1.GetCreateQuery(APICALLBOOM_PARAM) //, "alias_t1")
                .addCol("newNamerFor_F1")
                .addCol("colB1")
                .addCol("colA1")
                .addCol("colC1")
                .values(QVariantMap({
                    {"colC1", DBExpression::CURDATE()},
                    {"colB1", 111},
                    {"colZ1", "11Z"},
                    {"colA1", DBExpression::NOW()},
                    { "newNamerFor_F1", QVariantMap({ { "a", "b" }, { "c", "d1" } }) },
                }))
                .values(QVariantMap({
                    {"colB1", 222},
                    {"colZ1", "22Z"},
                    {"colC1", DBExpression::CURDATE()},
                    {"colA1", DBExpression::NOW()},
                    { "newNamerFor_F1", QVariantMap({ { "a", "b" }, { "c", "d2" } }) },
                }))
                .values(QList<QVariantMap>({
                    {
                        { "colB1", 333 },
                        { "colC1", DBExpression::CURDATE() },
                        { "colZ1", "33Z" },
                        { "colA1", DBExpression::NOW() },
                        { "newNamerFor_F1", QVariantMap({ { "a", "b" }, { "c", "d3" } }) },
                    },
                    {
                        { "colB1", 444 },
                        { "colZ1", "44Z" },
                        { "colA1", DBExpression::NOW() },
                        { "colC1", DBExpression::CURDATE() },
                        { "newNamerFor_F1", QVariantMap({ { "a", "b'b" }, { "c", "d4" } }) },
                    },
                }))
            ;

            stuBoundQueryString qry = query.buildQueryString(this->currentUserID, {}, false);

//            if (SQLPrettyLen)
//                qDebug().nospace().noquote() << endl
//                                             << endl << "-- Query:" << endl << qry.QueryString << endl;

            QCOMPARE("\n" + qry.QueryString + "\n", R"(
            INSERT
              INTO test.t1
                   (
                   t1.colF1
                 , t1.colB1
                 , t1.colA1
                 , t1.colC1
                 , t1.status1
                 , t1.CreatedBy_usrID
                   )
            VALUES (
                   '{"a":"b","c":"d1"}'
                 , 111
                 , NOW()
                 , CURDATE()
                 , 'A'
                 , 9090
                   )
                 , (
                   '{"a":"b","c":"d2"}'
                 , 222
                 , NOW()
                 , CURDATE()
                 , 'A'
                 , 9090
                   )
                 , (
                   '{"a":"b","c":"d3"}'
                 , 333
                 , NOW()
                 , CURDATE()
                 , 'A'
                 , 9090
                   )
                 , (
                   '{"a":"b''b","c":"d4"}'
                 , 444
                 , NOW()
                 , CURDATE()
                 , 'A'
                 , 9090
                   )
)");
        } QT_CATCH (const std::exception &exp) {
            QTest::qFail(exp.what(), __FILE__, __LINE__);
        }
    }

    void queryString_CREATE_values_multi_use_binding() {
        QT_TRY {
            ORMCreateQuery query = t1.GetCreateQuery(APICALLBOOM_PARAM) //, "alias_t1")
                .addCol("colG1")
                .addCol("colB1")
                .addCol("colA1")
                .addCol("colC1")
                .addCol("newNamerFor_F1")
                .values(QVariantMap({
                    {"colC1", DBExpression::CURDATE()},
                    {"colB1", 111},
                    {"colZ1", "11Z"},
                    {"colA1", DBExpression::NOW()},
                    {"colG1", 112},
                    { "newNamerFor_F1", QVariantMap({ { "a", "b" }, { "c", "d" } }) },
                }))
                .values(QVariantMap({
                    {"colB1", 222},
                    {"colZ1", "22Z"},
                    {"colC1", DBExpression::CURDATE()},
                    {"colA1", DBExpression::NOW()},
                    {"colG1", 212},
                    { "newNamerFor_F1", QVariantMap({ { "a", "b" }, { "c", "d" } }) }
                }))
                .values(QList<QVariantMap>({
                    {
                        {"colB1", 333},
                        {"colC1", DBExpression::CURDATE()},
                        {"colZ1", "33Z"},
                        {"colA1", DBExpression::NOW()},
                        {"colG1", 312},
                        { "newNamerFor_F1", QVariantMap({ { "a", "b" }, { "c", "d" } }) }
                    },
                    {
                        {"colB1", 444},
                        {"colZ1", "44Z"},
                        {"colA1", DBExpression::NOW()},
                        {"colC1", DBExpression::CURDATE()},
                        {"colG1", 412},
                        { "newNamerFor_F1", QVariantMap({ { "a", "b'b" }, { "c", "d" } }) }
                    },
                }))
            ;

            stuBoundQueryString qry = query.buildQueryString(this->currentUserID, {}, true);

            QStringList BindingValuesList;
            foreach (auto b, qry.BindingValues) {
                BindingValuesList.append(b.toString());
            }

//            if (SQLPrettyLen) {
//                qDebug().nospace().noquote() << endl
//                                             << endl << "-- Query:" << endl << qry.QueryString << endl
//                                             << endl << "-- Binding Values:" << endl << qry.BindingValues << endl
//                                             << "-- [" << BindingValuesList.join(", ") << "]" << endl;
//            }

            QCOMPARE("\n" + qry.QueryString + "\n", R"(
            INSERT
              INTO test.t1
                   (
                   t1.colG1
                 , t1.colB1
                 , t1.colA1
                 , t1.colC1
                 , t1.colF1
                 , t1.status1
                 , t1.CreatedBy_usrID
                   )
            VALUES (?, ?, NOW(), CURDATE(), ?, ?, ?)
                 , (?, ?, NOW(), CURDATE(), ?, ?, ?)
                 , (?, ?, NOW(), CURDATE(), ?, ?, ?)
                 , (?, ?, NOW(), CURDATE(), ?, ?, ?)
)");

//            QCOMPARE(BindingValuesList.join(", "), R"(112, 111, {"a":"b","c":"d"}, 'A', 9090, 212, 222, {"a":"b","c":"d"}, 'A', 9090, 312, 333, {"a":"b","c":"d"}, 'A', 9090, 412, 444, {"a":"b\'b","c":"d"}, 'A', 9090)");

        } QT_CATCH (const std::exception &exp) {
            QTest::qFail(exp.what(), __FILE__, __LINE__);
        }
    }

    void queryString_CREATE_values_from_select() {
        QT_TRY {
            ORMCreateQuery query = t1.GetCreateQuery(APICALLBOOM_PARAM) //, "alias_t1")
                .addCol("colA1")
                .addCol("colB1")
                .addCol("colC1")
                .addCol("colD1")
                .select(t2.GetSelectQuery(APICALLBOOM_PARAM)
                    .addCols(QStringList({
                        "colA2",
                        "colB2",
                        "colC2",
                        "colD2",
                    }))
                    .where({ "colE2", enuConditionOperator::Greater, 123 })
                )
            ;

            stuBoundQueryString qry = query.buildQueryString(this->currentUserID, {}, false);

//            if (SQLPrettyLen) {
//                qDebug().nospace().noquote() << endl
//                                             << endl << "-- Query:" << endl << qry.QueryString << endl
//                                             << endl << "-- Binding Values:" << endl << qry.BindingValues << endl;
//            }

            QCOMPARE("\n" + qry.QueryString + "\n", R"(
            INSERT
              INTO test.t1
                   (
                   t1.colA1
                 , t1.colB1
                 , t1.colC1
                 , t1.colD1
                 , t1.status1
                 , t1.CreatedBy_usrID
                   )
            SELECT t2.colA2
                 , t2.colB2
                 , t2.colC2
                 , t2.colD2
                 , 'A'
                 , 9090
              FROM test.t2
             WHERE t2._InvalidatedAt = 0
               AND (
                   t2.colE2 > 123
                   )
)");
        } QT_CATCH (const std::exception &exp) {
            QTest::qFail(exp.what(), __FILE__, __LINE__);
        }
    }

    /***************************************************************************************/
    /* UpdateQuery *************************************************************************/
    /***************************************************************************************/
    void queryString_UPDATE_throw_on_no_where() {
        QT_TRY {
            ORMUpdateQuery query = t1.GetUpdateQuery(APICALLBOOM_PARAM) //, "alias_t1")
                .set("colB1", 123)
                .set("colC1", "123")
                .setNull("colD1")
            ;

            QVERIFY_EXCEPTION_THROWN(
                        query.buildQueryString(this->currentUserID, {}, false),
                        exQueryBuilderWhereClauseNotProvided);

        } QT_CATCH (const std::exception &exp) {
            QTest::qFail(exp.what(), __FILE__, __LINE__);
        }
    }

    void queryString_UPDATE_simple() {
        QT_TRY {
            ORMUpdateQuery query = t1.GetUpdateQuery(APICALLBOOM_PARAM) //, "alias_t1")
                .setNull("colD1")
                .set("colC1", "v c1")
                .set("colB1", 123)
                .leftJoinWith("rel_a", "alias_t2")
                .leftJoin("test.t2")
                .where({ "colA1", enuConditionOperator::Equal, 123 })
//                .having({ "aaaaaaa", enuConditionOperator::Greater, 456 })
//                .andHaving({ "bbbbbbb", enuConditionOperator::NotNull })
            ;

            stuBoundQueryString qry = query.buildQueryString(this->currentUserID, {}, false);

//            if (SQLPrettyLen)
//                qDebug().nospace().noquote() << endl << endl << qry.QueryString << endl;

            QCOMPARE("\n" + qry.QueryString + "\n", R"(
            UPDATE test.t1
         LEFT JOIN test.t2 alias_t2
                ON alias_t2.colA2 = t1.colC1
         LEFT JOIN test.t2
                ON t2.colA2 = t1.colC1
               SET t1.colD1 = NULL
                 , t1.colC1 = 'v c1'
                 , t1.colB1 = 123
                 , t1.UpdatedBy_usrID = 9090
             WHERE t1._InvalidatedAt = 0
               AND (
                   t1.colA1 = 123
                   )
)");
        } QT_CATCH (const std::exception &exp) {
            QTest::qFail(exp.what(), __FILE__, __LINE__);
        }
    }

    void queryString_UPDATE_simple_use_binding() {
        QT_TRY {
            ORMUpdateQuery query = t1.GetUpdateQuery(APICALLBOOM_PARAM) //, "alias_t1")
                .setNull("colD1")
                .set("colC1", "v c1")
                .set("colB1", 123)
                .set("colI1", 'T')
                .leftJoinWith("rel_a", "alias_t2")
                .leftJoin("test.t2")
                .where({ "colA1", enuConditionOperator::Equal, 123 })
                .andWhere({ "colE1", enuConditionOperator::Equal, DBExpression::NOW() })
//                .having({ "aaaaaaa", enuConditionOperator::Greater, 456 })
//                .andHaving({ "bbbbbbb", enuConditionOperator::NotNull })
            ;

            stuBoundQueryString qry = query.buildQueryString(this->currentUserID, {}, true);

            QStringList BindingValuesList;
            foreach (auto b, qry.BindingValues) {
                BindingValuesList.append(b.toString());
            }

//            if (SQLPrettyLen) {
//                qDebug().nospace().noquote() << endl
//                                             << endl << "-- Query:" << endl << qry.QueryString << endl
//                                             << endl << "-- Binding Values:" << endl << qry.BindingValues << endl
//                                             << "-- [" << BindingValuesList.join(", ") << "]" << endl;
//            }

            QCOMPARE("\n" + qry.QueryString + "\n", R"(
            UPDATE test.t1
         LEFT JOIN test.t2 alias_t2
                ON alias_t2.colA2 = t1.colC1
         LEFT JOIN test.t2
                ON t2.colA2 = t1.colC1
               SET t1.colD1 = NULL
                 , t1.colC1 = ?
                 , t1.colB1 = ?
                 , t1.colI1 = ?
                 , t1.UpdatedBy_usrID = ?
             WHERE t1._InvalidatedAt = 0
               AND (
                   t1.colA1 = 123
               AND t1.colE1 = NOW()
                   )
)");

            QCOMPARE(BindingValuesList.join(", "), "v c1, 123, T, 9090");
        } QT_CATCH (const std::exception &exp) {
            QTest::qFail(exp.what(), __FILE__, __LINE__);
        }
    }

    void queryString_UPDATE_increase() {
        QT_TRY {
            ORMUpdateQuery query = t1.GetUpdateQuery(APICALLBOOM_PARAM) //, "alias_t1")
                .set("colB1", DBExpression::VALUE("colB1 + 10"))
                .where({ "colA1", enuConditionOperator::Equal, 123 })
            ;

            stuBoundQueryString qry = query.buildQueryString(this->currentUserID, {}, false);

//            if (SQLPrettyLen)
//                qDebug().nospace().noquote() << endl << endl << qry.QueryString << endl;

            QCOMPARE("\n" + qry.QueryString + "\n", R"(
            UPDATE test.t1
               SET t1.colB1 = colB1 + 10
                 , t1.UpdatedBy_usrID = 9090
             WHERE t1._InvalidatedAt = 0
               AND (
                   t1.colA1 = 123
                   )
)");
        } QT_CATCH (const std::exception &exp) {
            QTest::qFail(exp.what(), __FILE__, __LINE__);
        }
    }

    /***************************************************************************************/
    /* DeleteQuery *************************************************************************/
    /***************************************************************************************/
    void queryString_DELETE_throw_on_no_where() {
        QT_TRY {
            ORMDeleteQuery query = t1.GetDeleteQuery(APICALLBOOM_PARAM) //, "alias_t1")
            ;

            QVERIFY_EXCEPTION_THROWN(
                        query.buildQueryString(this->currentUserID, {}),
                        exQueryBuilderWhereClauseNotProvided);

        } QT_CATCH (const std::exception &exp) {
            QTest::qFail(exp.what(), __FILE__, __LINE__);
        }
    }

    void queryString_DELETE_simple() {
        QT_TRY {
            ORMDeleteQuery query = t1.GetDeleteQuery(APICALLBOOM_PARAM) //, "alias_t1")
                .where({ "colA1", enuConditionOperator::Equal, 123 })
            ;

            QString qry = query.buildQueryString(this->currentUserID, {});

//            if (SQLPrettyLen)
//                qDebug().nospace().noquote() << endl << endl << qry << endl;

            QCOMPARE("\n" + qry + "\n", R"(
            DELETE test.t1
              FROM test.t1
             WHERE t1._InvalidatedAt = 0
               AND (
                   t1.colA1 = 123
                   )
)");
        } QT_CATCH (const std::exception &exp) {
            QTest::qFail(exp.what(), __FILE__, __LINE__);
        }
    }

    void queryString_DELETE_join() {
        QT_TRY {
            ORMDeleteQuery query = t1.GetDeleteQuery(APICALLBOOM_PARAM) //, "alias_t1")
                .leftJoinWith("rel_a")
                .where({ "colA1", enuConditionOperator::Equal, 123 })
            ;

            QString qry = query.buildQueryString(this->currentUserID, {});

//            if (SQLPrettyLen)
//                qDebug().nospace().noquote() << endl << endl << qry << endl;

            QCOMPARE("\n" + qry + "\n", R"(
            DELETE test.t1
              FROM test.t1
         LEFT JOIN test.t2
                ON t2.colA2 = t1.colC1
             WHERE t1._InvalidatedAt = 0
               AND (
                   t1.colA1 = 123
                   )
)");
        } QT_CATCH (const std::exception &exp) {
            QTest::qFail(exp.what(), __FILE__, __LINE__);
        }
    }

    void queryString_DELETE_multi_target() {
        QT_TRY {
            ORMDeleteQuery query = t1.GetDeleteQuery(APICALLBOOM_PARAM) //, "alias_t1")
                .addTarget("t2")
                .addTarget("t3")
                .addTarget("t4")
                .leftJoinWith("rel_a")
                .where({ "colA1", enuConditionOperator::Equal, 123 })
            ;

            QString qry = query.buildQueryString(this->currentUserID, {});

//            if (SQLPrettyLen)
//                qDebug().nospace().noquote() << endl << endl << qry << endl;

            QCOMPARE("\n" + qry + "\n", R"(
            DELETE test.t1
                 , t2
                 , t3
                 , t4
              FROM test.t1
         LEFT JOIN test.t2
                ON t2.colA2 = t1.colC1
             WHERE t1._InvalidatedAt = 0
               AND (
                   t1.colA1 = 123
                   )
)");
        } QT_CATCH (const std::exception &exp) {
            QTest::qFail(exp.what(), __FILE__, __LINE__);
        }
    }

//    void testDemangle()
//    {
//        QString Name = typeid(A::B::CCC).name();
//        QString Demangled = Targoman::Common::demangle(Name.toStdString().c_str());
//        qDebug() << Name << Demangled;
//    }

    void test_case_1() {
        auto v = DBExpressionCase()
                 .when("a")
                 .then("b")
                 .when("c")
                 .then("d")
                 .else_("e")
                 .operator DBExpression()
                 .toString();
        qDebug() << v;
        QVERIFY(v == "CASE WHEN a THEN b WHEN c THEN d ELSE e END");
    }

    void test_case_2() {
        auto v = DBExpressionCase("ccc")
                 .when("a")
                 .then("b")
                 .when("c")
                 .then("d")
                 .else_("e")
                 .operator DBExpression()
                 .toString();
        qDebug() << v;
        QVERIFY(v == "CASE ccc WHEN a THEN b WHEN c THEN d ELSE e END");
    }

};

#endif // QUERYBUILDERS_HPP
