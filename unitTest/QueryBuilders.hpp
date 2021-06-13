#ifndef QUERYBUILDERS_HPP
#define QUERYBUILDERS_HPP

#include <QtTest/QtTest>
#include <QStringList>

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
                { "colI", S(NULLABLE(quint32)),            QFV,                       QNull,      UPOwner},
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

class TestSelectQuery : public SelectQuery
{
public:
    TestSelectQuery(const clsTable& _table) : SelectQuery(_table) {}
    TestSelectQuery(const SelectQuery& _other) : SelectQuery(_other) {}
    TestSelectQuery(const TestSelectQuery& _other) : SelectQuery(_other) {}
    ~TestSelectQuery() {}

    QString queryString() {
        return this->buildQueryString();
    }

//private:
//    friend SelectQuery;
//    SelectQuery* Query;
//    using SelectQuery::Data;
};

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

class QueryBuilders: public QObject
{
    Q_OBJECT

private slots:
    void condition1() {
        QVERIFY_TRY(clsCondition().isEmpty() == true);
    };
    void condition2() {
        QVERIFY_TRY(clsCondition().closePar().isEmpty() == false);
    };
    void condition3() {
        QVERIFY_TRY(clsCondition().openPar(clsCondition({ "cola",  enuConditionOperator::Less, 123 })).isEmpty() == false);
    };
    void condition4() {
        QVERIFY_TRY(clsCondition().andCond(clsCondition()).isEmpty() == false);
    };
    void condition5() {
        QVERIFY_TRY(clsCondition().orCond(clsCondition()).isEmpty() == false);
    };
    void condition6() {
        QVERIFY_TRY(clsCondition().xorCond(clsCondition()).isEmpty() == false);
    };

    void equalityOfConditionQueryString() {
//        QString qry;

//        qry = clsCondition("cola",  enuConditionOperator::Less, 123).buildQueryString();
//        QVERIFY(qry == "cola < 123");

//        qry = clsCondition("cola",  enuConditionOperator::Equal, "test").buildQueryString();
//        QVERIFY(qry == "cola = 'test'");

//        qry = clsCondition::scope(
//                clsCondition("cola", enuConditionOperator::Null)
//                .orCond({ "colb", enuConditionOperator::Equal, 123 })
//            )
//            .buildQueryString()
//        ;
//        QVERIFY(qry == "(cola IS NULL OR colb = 123)");

//        qry = clsCondition::scope(
//                clsCondition("cola", enuConditionOperator::Null)
//                .orCond(clsCondition::scope(
//                    clsCondition("colb", enuConditionOperator::Null)
//                    .andCond({ "colc", enuConditionOperator::Equal, 123 })
//                ))
//            )
//            .buildQueryString()
//        ;
//        QVERIFY(qry == "(cola IS NULL OR (colb IS NULL AND colc = 123))");
    }

    void equalityOfQueryString1() {
        QT_TRY {
            TestTable t;
            t.prepareFiltersList();
//            TestSelectQuery query = TestSelectQuery(t)
//                .addCols(QStringList({
//                    "colA",
//                    "colB",
//                    "colC",
//                }))
//            ;
//            QString qry = query.queryString();
//            QCOMPARE(qry, "SELECT colA,colB,colC FROM tblTest");
        } QT_CATCH (const std::exception &e) {
            QTest::qFail(e.what(), __FILE__, __LINE__);
        }
    }




public:
    void equalityOfQueryString2() {
        TestTable t;
        TestSelectQuery query = TestSelectQuery(t)
            .addCols(QStringList({
                "colA",
                "colB",
                "colC",
            }))
            .addCol(enuAggregation::AVG, "colA", "avg_colA")
            .addCol(enuConditionalAggregation::COUNTIF,
                    { "colB", enuConditionOperator::Equal, 123 },
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
            QString qry = query.queryString();
            QCOMPARE(qry, "SELECT colA,colB,colC,AVG(colA) AS colA,COUNT(colB=123) AS countif_colB,SUM(IF(colC>123,10,20)) AS sumif_colC FROM tblTest");
        } QT_CATCH (const std::exception &e) {
            QTest::qFail(e.what(), __FILE__, __LINE__);
        }
    }
};

#endif // QUERYBUILDERS_HPP
