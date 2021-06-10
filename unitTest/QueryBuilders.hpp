#ifndef QUERYBUILDERS_HPP
#define QUERYBUILDERS_HPP

#include <QtTest/QtTest>
#include <QStringList>

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
            "tblTestTable",
            {///< ColName                                          Type                     Validation                              Default     UpBy      Sort  Filter Self  Virt   PK
                { "slbID",                  ORM_PRIMARY_KEY32 },
                { "slbCode",                S(TAPI::SaleableCode_t), QFV,                                    QRequired,  UPOwner },
                { "slb_prdID",              S(quint32),              QFV.integer().minValue(1),              QRequired,  UPOwner },
                { "slbName",                S(QString),              QFV,                                    QRequired,  UPOwner },
                { "slbDesc",                S(QString),              QFV,                                    QNull,      UPOwner },
                { "slbType",                S(TAPI::enuSaleableType::Type), QFV,                             TAPI::enuSaleableType::Normal, UPOwner },
                { "slbCanBePurchasedSince", S(TAPI::DateTime_t),     QFV,                                    QNow,       UPOwner },
                { "slbNotAvailableSince",   S(TAPI::DateTime_t),     QFV,                                    QNull,      UPOwner },
                { "slbPrivs",               S(TAPI::JSON_t),         QFV,                                    QNull,      UPOwner },
                { "slbBasePrice",           S(qreal),                QFV.real().minValue(0),                 QRequired,  UPOwner },
                { "slbAdditives",           S(TAPI::SaleableAdditive_t), QFV,                                QNull,      UPOwner },
                { "slbProductCount",        S(quint32),              QFV.integer().minValue(1),              QRequired,  UPOwner},
                { "slbMaxSaleCountPerUser", S(NULLABLE(quint32)),    QFV,                                    QNull,      UPOwner},
                { "slbInStockCount",        S(quint32),              QFV.integer().minValue(0),              QRequired,  UPAdmin },
                { "slbOrderedCount",        S(quint32),              QFV,                                    QInvalid,   UPNone },
                { "slbReturnedCount",       S(quint32),              QFV,                                    QInvalid,   UPNone },
                { "slbVoucherTemplate",     S(QString),              QFV,                                    QNull,      UPOwner },
                { "slbStatus",              S(TAPI::enuGenericStatus::Type), QFV,                            TAPI::enuGenericStatus::Active, UPStatus },
                { "slbCreatedBy_usrID",     ORM_CREATED_BY },
                { "slbCreationDateTime",    ORM_CREATED_ON },
                { "slbUpdatedBy_usrID",     ORM_UPDATED_BY },
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

    void equalityOfConditionQueryString() {
        QString qry;

        qry = clsCondition("cola",  enuConditionOperator::Less, 123).buildQueryString();
        QVERIFY(qry == "cola < 123");

        qry = clsCondition("cola",  enuConditionOperator::Equal, "test").buildQueryString();
        QVERIFY(qry == "cola = 'test'");

        qry = clsCondition::scope(
                clsCondition("cola", enuConditionOperator::Null)
                .orCond({ "colb", enuConditionOperator::Equal, 123 })
            )
            .buildQueryString()
        ;
        QVERIFY(qry == "(cola IS NULL OR colb = 123)");

        qry = clsCondition::scope(
                clsCondition("cola", enuConditionOperator::Null)
                .orCond(clsCondition::scope(
                    clsCondition("colb", enuConditionOperator::Null)
                    .andCond({ "colc", enuConditionOperator::Equal, 123 })
                ))
            )
            .buildQueryString()
        ;
        QVERIFY(qry == "(cola IS NULL OR (colb IS NULL AND colc = 123))");
    }

    void equalityOfQueryString() {
        QString qry;

        TestTable t;
        SelectQuery query = SelectQuery(t)
            .addCols(QStringList({
                "slbID",
                "slbCode",
                "slb_prdID",
                "slbName",
                "slbDesc",
                "slbType",
                "slbCanBePurchasedSince",
                "slbNotAvailableSince",
                "slbPrivs",
                "slbBasePrice",
                "slbAdditives",
                "slbProductCount",
                "slbMaxSaleCountPerUser",
                "slbInStockCount",
                "slbOrderedCount",
                "slbReturnedCount",
                "slbVoucherTemplate",
                "slbStatus",
                "slbCreatedBy_usrID",
                "slbCreationDateTime",
                "slbUpdatedBy_usrID",
            }))
            .leftJoin("t2",
                clsCondition({ "t2.pk", enuConditionOperator::Equal, "t1.fk" })
                    .andCond({ "t2.col2", enuConditionOperator::Equal, "123"})
            )
            .where({ "slbID", enuConditionOperator::Equal, 123 })
            .andWhere({ "slbCode", enuConditionOperator::GreaterEqual, "abc" })
            .groupBy(QStringList({ "slbID", "slbStatus" }))
        ;
        qry = query.buildQueryString();
        QVERIFY(qry == "...");
    }
};

#endif // QUERYBUILDERS_HPP
