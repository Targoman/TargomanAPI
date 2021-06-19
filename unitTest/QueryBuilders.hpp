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

class TestTable : public clsTable
{
    Q_OBJECT

public:
    TestTable()
        : clsTable(
            "test",
            "tblTest",
            {///< ColName Type                             Validation                 Default     UpBy      Sort  Filter Self  Virt   PK
                { "colA", ORM_PRIMARY_KEY32 },
                { "colB", S(TAPI::SaleableCode_t),         QFV,                       QRequired,  UPOwner },
                { "colC", S(quint32),                      QFV.integer().minValue(1), QRequired,  UPOwner },
                { "colD", S(QString),                      QFV,                       QRequired,  UPOwner },
                { "colE", S(TAPI::DateTime_t),             QFV,                       QNull,      UPOwner },
                { "colF", S(TAPI::JSON_t),                 QFV,                       QNull,      UPOwner },
                { "colG", S(qreal),                        QFV.real().minValue(0),    QRequired,  UPOwner },
                { "colH", S(TAPI::SaleableAdditive_t),     QFV,                       QNull,      UPOwner },
                { "colI", S(NULLABLE_TYPE(quint32)),            QFV,                       QNull,      UPOwner},
                { "colJ", S(TAPI::enuGenericStatus::Type), QFV,                       TAPI::enuGenericStatus::Active, UPStatus },
                { "colK", ORM_CREATED_BY },
                { "colL", ORM_CREATED_ON },
                { "colM", ORM_UPDATED_BY },
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

private slots:
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
            TestTable t;
            t.prepareFiltersList();

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

            QCOMPARE(cnd.buildConditionString(t.Name, t.FilterableColsMap),
                     "tblTest.colA=101 AND tblTest.colB IS NOT NULL OR tblTest.colC=102 AND (tblTest.colD=103 OR tblTest.colE=104 XOR (tblTest.colF=105 OR tblTest.colG=106)) OR tblTest.colH=107");
        } QT_CATCH (const std::exception &e) {
            QTest::qFail(e.what(), __FILE__, __LINE__);
        }
    }

    void queryString_EmptyCol() {
        QT_TRY {
            TestTable t;
            t.prepareFiltersList();
            SelectQuery query = SelectQuery(t)
            ;
            QString qry = query.buildQueryString();
            QCOMPARE(qry, "QQQQQQQQQQQQ");
        } QT_CATCH (const std::exception &e) {
            QTest::qFail(e.what(), __FILE__, __LINE__);
        }
    }

    void queryString_simple() {
        QT_TRY {
            TestTable t;
            t.prepareFiltersList();
            SelectQuery query = SelectQuery(t)
                .addCols(QStringList({
                    "colA",
                    "colB",
                    "colC",
                }))
                .addCol("colD", "ren_colD")
            ;
            QString qry = query.buildQueryString();
            QCOMPARE(qry, "QQQQQQQQQQQQ");
        } QT_CATCH (const std::exception &e) {
            QTest::qFail(e.what(), __FILE__, __LINE__);
        }
    }

    void queryString_aggregateCols() {
        TestTable t;
        t.prepareFiltersList();
        SelectQuery query = SelectQuery(t)
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
            QCOMPARE(qry, "QQQQQQQQQQQQ");
        } QT_CATCH (const std::exception &e) {
            QTest::qFail(e.what(), __FILE__, __LINE__);
        }
    }

    void queryString_aggregateConditionalCols() {
        TestTable t;
        t.prepareFiltersList();

        SelectQuery query = SelectQuery(t)
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
            QCOMPARE(qry, "QQQQQQQQQQQQ");
        } QT_CATCH (const std::exception &e) {
            QTest::qFail(e.what(), __FILE__, __LINE__);
        }
    }

    void queryString_join() {
        TestTable t;
        t.prepareFiltersList();

        SelectQuery query = SelectQuery(t)
            .addCols(QStringList({
                "colA",
                "colB",
                "colC",
            }))
            .leftJoin("t2", "alias_t2",
//                "alias_t2.tA=tblTest.colB AND alias_t2.tB='colB' AND t77777777.ffffff7=456"
                clsCondition({ "t2", "tA", enuConditionOperator::Equal, "tblTest", "colB" })
                    .andCond({ "t2", "tB", enuConditionOperator::Equal, "col B" })
                    .andCond({ "t77777777", "ffffff7", enuConditionOperator::Equal, "456"})
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
