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
                { "colA", ORM_PRIMARY_KEY32 },
                { "colB", S(TAPI::SaleableCode_t),         QFV,                       QRequired,  UPOwner },
                { "colC", S(quint32),                      QFV.integer().minValue(1), QRequired,  UPOwner },
                { "colD", S(QString),                      QFV,                       QRequired,  UPOwner },
                { "colE", S(TAPI::DateTime_t),             QFV,                       QNull,      UPOwner },
                { "colF", S(TAPI::JSON_t),                 QFV,                       QNull,      UPOwner },
                { "colG", S(qreal),                        QFV.real().minValue(0),    QRequired,  UPOwner },
                { "colH", S(qreal),                        QFV.real().minValue(0),    QRequired,  UPOwner },
            },
            {///< Col     Reference Table  ForeignCol  Rename  LeftJoin
                { "rel_a", { "colC", R("test", "t2"), "tcolID" } },
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
                { "tcolA", ORM_PRIMARY_KEY32 },
                { "tcolB", S(TAPI::SaleableCode_t),         QFV,                       QRequired,  UPOwner },
                { "tcolC", S(quint32),                      QFV.integer().minValue(1), QRequired,  UPOwner },
                { "tcolD", S(QString),                      QFV,                       QRequired,  UPOwner },
                { "tcolE", S(TAPI::DateTime_t),             QFV,                       QNull,      UPOwner },
                { "tcolF", S(TAPI::JSON_t),                 QFV,                       QNull,      UPOwner },
                { "tcolG", S(qreal),                        QFV.real().minValue(0),    QRequired,  UPOwner },
                { "tcolH", S(qreal),                        QFV.real().minValue(0),    QRequired,  UPOwner },
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

/*
template<class TClient>
class Attorney : public TClient {
public:
    Attorney(TClient& client) : client_(client) {}
private:
    TClient& client_;
};

class TestSelectQuery : public Attorney<SelectQuery>
{
public:
    using Attorney<SelectQuery>::Attorney;
    using SelectQuery::Data;

    QString buildQueryString()
    {
        this->Data->prepare();
    }

//private:
//    friend SelectQuery;
//    SelectQuery* Query;
//    using SelectQuery::Data;
};
*/

//namespace Targoman {
//namespace API {
//namespace ORM {

//class clsSelectQueryData;

//}
//}
//}

//class TestSelectQuery : public SelectQuery
//{
//public:
//    TestSelectQuery(const clsTable& _table) : SelectQuery(_table) {}
//    TestSelectQuery(const SelectQuery& _other) : SelectQuery(_other) {}
//    TestSelectQuery(const TestSelectQuery& _other) : SelectQuery(_other) {}
//    ~TestSelectQuery() {}

//    QString queryString() {
//        return this->buildQueryString();
//    }

//private:
//    friend SelectQuery;
//    SelectQuery* Query;
//    using SelectQuery::Data;
//};

//#define TEST_FIREND_buildQueryStringForTest friend QString buildQueryStringForTest(const SelectQuery& _this);

//class TestSelectQuery
//{
//public:
//    static QString buildQueryString(SelectQuery* _this)
//    {
//        _this->Data->prepare();
//    }
//};

//class clsSelectQueryData;

//QString buildQueryStringForTest(const SelectQuery& _this)
//{
//    _this.Data->prepare();
//}

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

//private slots:
    void testOptional()
    {
        optional<quint32> a;
        *a = 1234;

        //    std::optional<enuAggregation::Type> z1;
        //    z1 = enuAggregation::MAX;
        //    *zzz = enuAggregation::MAX;

        //    NULLABLE_TYPE(enuAggregation::Type) a1, a2;
        //    TAPI::tmplNullable<enuAggregation::Type> a1, a2;
        //    enuAggregation::Type a3 = a1;
        //    std::optional<enuAggregation::Type> a1, a2;
        //    if (a1 < a2)
        //    {
        //    }
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
        QVERIFY_TRY(clsCondition().andCond(clsCondition("colA", enuConditionOperator::Equal, 123)).isEmpty() == false);
    };
    void condition5() {
        QVERIFY_TRY(clsCondition().orCond(clsCondition("colA", enuConditionOperator::Equal, 123)).isEmpty() == false);
    };
    void condition6() {
        QVERIFY_TRY(clsCondition().xorCond(clsCondition("colA", enuConditionOperator::Equal, 123)).isEmpty() == false);
    };
    void condition7() {
        QVERIFY_TRY(clsCondition("colA", enuConditionOperator::Equal, 123)
                    .orCond({ "colB", enuConditionOperator::Equal, 123 })
                    .andCond(
                        clsCondition("colC", enuConditionOperator::Equal, 123)
                        .orCond({ "colD", enuConditionOperator::Equal, 123 })
                        .xorCond(
                            clsCondition("colE", enuConditionOperator::Equal, 123)
                            .orCond({ "colF", enuConditionOperator::Equal, 123 })
                        )
                    )
                    .isEmpty() == false);
    };

    void condition_buildString()
    {
        QT_TRY {
            clsCondition cnd = clsCondition({ "colA", enuConditionOperator::Equal, 101 })
                .andCond({ "colB", enuConditionOperator::NotNull })
                .orCond({ "colC", enuConditionOperator::Equal, 102 })
                .andCond(
                    clsCondition("colD", enuConditionOperator::Equal, 103)
                    .orCond({ "colE", enuConditionOperator::Equal, 104 })
                    .xorCond(
                        clsCondition("colF", enuConditionOperator::Equal, 105)
                        .orCond({ "colG", enuConditionOperator::Equal, 106 })
                    )
                )
                .orCond({ "colH", enuConditionOperator::Equal, 107 })
            ;

            QCOMPARE(cnd.buildConditionString(t1.Name, t1.FilterableColsMap),
                     "t1.colA=101 AND t1.colB IS NOT NULL OR t1.colC=102 AND (t1.colD=103 OR t1.colE=104 XOR (t1.colF=105 OR t1.colG=106)) OR t1.colH=107");
        } QT_CATCH (const std::exception &e) {
            QTest::qFail(e.what(), __FILE__, __LINE__);
        }
    }

    void queryString_EmptyCol() {
        QT_TRY {
            SelectQuery query = SelectQuery(t1);
            QString qry = query.buildQueryString();
            QCOMPARE(qry, "SELECT t1.colA,t1.colB,t1.colC,t1.colD,t1.colE,t1.colF,t1.colG,t1.colH,t1.CURRENT_DATETIME,test.t2.tcolA,test.t2.tcolB,test.t2.tcolC,test.t2.tcolD,test.t2.tcolE,test.t2.tcolF,test.t2.tcolG,test.t2.tcolH,test.t2.CURRENT_DATETIME FROM test.t1");
        } QT_CATCH (const std::exception &e) {
            QTest::qFail(e.what(), __FILE__, __LINE__);
        }
    }

    void queryString_simple() {
        QT_TRY {
            SelectQuery query = SelectQuery(t1)
                .addCols(QStringList({
                    "colA",
                    "colB",
                    "colC",
                }))
                .addCol("colD", "ren_colD")
            ;
            QString qry = query.buildQueryString();
            QCOMPARE(qry, "SELECT t1.colA,t1.colB,t1.colC,t1.colD AS ren_colD FROM test.t1");
        } QT_CATCH (const std::exception &e) {
            QTest::qFail(e.what(), __FILE__, __LINE__);
        }
    }

    void queryString_simpleWithAlias() {
        QT_TRY {
            SelectQuery query = SelectQuery(t1, "alias_t1")
                .addCols(QStringList({
                    "colA",
                    "colB",
                    "colC",
                }))
                .addCol("colD", "ren_colD")
            ;
            QString qry = query.buildQueryString();
            QCOMPARE(qry, "SELECT alias_t1.colA,alias_t1.colB,alias_t1.colC,alias_t1.colD AS ren_colD FROM test.t1 alias_t1");
        } QT_CATCH (const std::exception &e) {
            QTest::qFail(e.what(), __FILE__, __LINE__);
        }
    }

    void queryString_aggregateCols() {
        SelectQuery query = SelectQuery(t1)
//            .addCols(QStringList({
//                "colA",
//                "colB",
//                "colC",
//            }))
            .addCol(enuAggregation::AVG, "colA", "avg_colA")
            .addCol(enuAggregation::SUM, "colB", "sum_colB")
//            .leftJoin("t2",
//                clsCondition({ "t2.pk", enuConditionOperator::Equal, "t1.fk" })
//                    .andCond({ "t2.col2", enuConditionOperator::Equal, "123"})
//            )
//            .where({ "colA", enuConditionOperator::Equal, 123 })
//            .andWhere({ "colB", enuConditionOperator::GreaterEqual, "abc" })
//            .groupBy(QStringList({ "colC", "slbStatus" }))
        ;
        QT_TRY {
            QString qry = query.buildQueryString();
            QCOMPARE(qry, "SELECT AVG(t1.colA) AS avg_colA,SUM(t1.colB) AS sum_colB FROM test.t1");
        } QT_CATCH (const std::exception &e) {
            QTest::qFail(e.what(), __FILE__, __LINE__);
        }
    }

    void queryString_aggregateConditionalCols() {
        SelectQuery query = SelectQuery(t1)
//            .addCols(QStringList({
//                "colA",
//                "colB",
//                "colC",
//            }))
//            .addCol(enuAggregation::AVG, "colA", "avg_colA")
//            .addCol(enuConditionalAggregation::COUNTIF,
//                    { "colB", enuConditionOperator::Equal, 123 },
//                    "countif_colB"
//                    )
            .addCol(enuConditionalAggregation::COUNTIF,
                    { "colB", enuConditionOperator::Equal, 123 },
                    "countif_colB"
                    )
            .addCol(enuConditionalAggregation::COUNTIF,
                    clsCondition("colB", enuConditionOperator::Equal, 123)
                        .andCond({ "colC", enuConditionOperator::Like, "test it"})
                        .xorCond(
                            clsCondition("colF", enuConditionOperator::Equal, 105)
                            .orCond({ "colG", enuConditionOperator::Equal, 106 })
                        )
                    ,
                    "countif_colB"
                    )
            .addCol(enuConditionalAggregation::SUMIF,
                    { "colC", enuConditionOperator::Greater, 123 },
                    10,
                    20,
                    "sumif_colC"
                    )
//            .leftJoin("t2",
//                clsCondition({ "t2.pk", enuConditionOperator::Equal, "t1.fk" })
//                    .andCond({ "t2.col2", enuConditionOperator::Equal, "123"})
//            )
//            .where({ "colA", enuConditionOperator::Equal, 123 })
//            .andWhere({ "colB", enuConditionOperator::GreaterEqual, "abc" })
//            .groupBy(QStringList({ "colC", "slbStatus" }))
        ;
        QT_TRY {
            QString qry = query.buildQueryString();
            QCOMPARE(qry, "SELECT COUNT(t1.colB=123) AS countif_colB,COUNT(t1.colB=123 AND t1.colC LIKE 'test it' XOR (t1.colF=105 OR t1.colG=106)) AS countif_colB,SUM(IF(t1.colC>123,10,20)) AS sumif_colC FROM test.t1");
        } QT_CATCH (const std::exception &e) {
            QTest::qFail(e.what(), __FILE__, __LINE__);
        }
    }

    void queryString_join() {
        SelectQuery query = SelectQuery(t1, "alias_t1")
            .addCols(QStringList({
                "colA",
                "colB",
                "colC",
            }))
            .leftJoinWith("rel_a", "alias_t2")
            .leftJoin("t2", "alias_1_t2")
            .leftJoin("t2", "alias_2_t2",
                clsCondition({ "alias_2_t2", "tcolA", enuConditionOperator::Equal, "t1", "colB" })
                    .andCond({ "alias_2_t2", "tB", enuConditionOperator::Equal, "test string" })
//                    .andCond({ "t77777777", "ffffff7", enuConditionOperator::Equal, "456"})
            )
            .where({ "colA", enuConditionOperator::Equal, 123 })
            .andWhere({ "t2", "tA", enuConditionOperator::GreaterEqual, "abc" })
//            .groupBy(QStringList({ "colC", "slbStatus" }))
        ;
        QT_TRY {
            QString qry = query.buildQueryString();
            QCOMPARE(qry, "QQQQQQQQQQQQ");
        } QT_CATCH (const std::exception &e) {
            QTest::qFail(e.what(), __FILE__, __LINE__);
        }
    }

};

#endif // QUERYBUILDERS_HPP
