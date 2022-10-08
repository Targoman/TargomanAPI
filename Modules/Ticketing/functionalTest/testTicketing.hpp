/******************************************************************************
 * TargomanAAA: Authentication, Authorization, Accounting framework           *
 *                                                                            *
 * Copyright 2014-2019 by Targoman Intelligent Processing <http://tip.co.ir>  *
 *                                                                            *
 * TargomanAAA is free software: you can redistribute it and/or modify        *
 * it under the terms of the GNU Lesser General Public License as published   *
 * by the Free Software Foundation, either version 3 of the License, or       *
 * (at your option) any later version.                                        *
 *                                                                            *
 * TargomanAAA is distributed in the hope that it will be useful,             *
 * but WITHOUT ANY WARRANTY; without even the implied warranty of             *
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the              *
 * GNU Lesser General Public License for more details.                        *
 * You should have received a copy of the GNU Lesser General Public License   *
 * along with Targoman. If not, see <http://www.gnu.org/licenses/>.           *
 *                                                                            *
 ******************************************************************************/
/**
 * @author S. Mehran M. Ziabary <ziabary@targoman.com>
 * @author Kambiz Zandi <kambizzandi@gmail.com>
 */

#ifndef TEST_TICKETING_HPP
#define TEST_TICKETING_HPP

#include <QRandomGenerator>
#include "Interfaces/Test/testCommon.hpp"
#include "Interfaces/AAA/clsJWT.hpp"
#include "ORM/actionLogs.hpp"
#include <cstdlib>
#include <unistd.h>
#include "../moduleSrc/ORM/Defs.hpp"

using namespace Targoman::API;
using namespace Targoman::API::AAA;

#include "Interfaces/DBM/QueryBuilders.h"
using namespace Targoman::API::DBM;

#include "Interfaces/AAA/Accounting_Defs.hpp"
using namespace Targoman::API::AAA;

#include "Interfaces/Helpers/SecurityHelper.h"
using namespace Targoman::API::Helpers;

class testTicketing : public clsBaseTest
{
    Q_OBJECT

public:
    testTicketing(const QString &_dbPrefix) : clsBaseTest(_dbPrefix) { ; }

    void cleanupUnitTestData() {
        clsDAC DAC;
        DAC.execQuery("", QString("UPDATE %1AAA.tblUser SET usrStatus='R' WHERE usrEmail IN(?,?)").arg(this->DBPrefix), { UT_UserEmail, UT_AdminUserEmail });
    }

    QString LastRandomNumber;
    QString CreatedUserEmail;
    QString CreatedAdminEmail;

    quint64 MainTicketID;
    quint64 Reply1TicketID;
    quint64 Reply2TicketID;

private slots:
    void initTestCase() {
        initUnitTestData(false);
    }

    void cleanupTestCase() {
        gEncodedAdminJWT = "";
        gEncodedJWT = "";
        cleanupUnitTestData();
    }

    /***************************************************************************************/
    /***************************************************************************************/
    /***************************************************************************************/
    void setupAccountFixture() {
        QT_TRY {
            QVariant Result = callAdminAPI(
                RESTClientHelper::POST,
                "Account/fixtureSetup",
            {},
            {
                { "random", 1 },
            });

            QVERIFY(Result.isValid());

            this->LastRandomNumber = Result.toMap().value("Random").toString();

            this->CreatedUserEmail = Result.toMap().value("User").toMap().value("email").toString();
            gUserID = Result.toMap().value("User").toMap().value("usrID").toULongLong();

            this->CreatedAdminEmail = Result.toMap().value("Admin").toMap().value("email").toString();
            gAdminUserID = Result.toMap().value("Admin").toMap().value("usrID").toULongLong();

        } QT_CATCH (const std::exception &exp) {
            QTest::qFail(exp.what(), __FILE__, __LINE__);
        }
    }

    void login_user() {
        //5d12d36cd5f66fe3e72f7b03cbb75333 = MD5(1234 + df6d2338b2b8fce1ec2f6dda0a630eb0 # 987)
        QVariant Result = callUserAPI(RESTClientHelper::POST,
                                "Account/login",{},{
                                    { "emailOrMobile", this->CreatedUserEmail },
                                    { "pass", "5d12d36cd5f66fe3e72f7b03cbb75333" },
                                    { "salt", "1234" },
                                });
        QVERIFY(Result.isValid());

        gEncodedJWT = Result.toMap().value("token").toString();
        gJWT = QJsonDocument::fromJson(QByteArray::fromBase64(gEncodedJWT.split('.').at(1).toLatin1())).object();

        QVERIFY(clsJWT(gJWT).actorID() == gUserID);
        QVERIFY(clsJWT(gJWT).usrStatus() == TAPI::enuUserStatus::Active);
    }

    void login_admin() {
        //5d12d36cd5f66fe3e72f7b03cbb75333 = MD5(1234 + df6d2338b2b8fce1ec2f6dda0a630eb0 # 987)
        QVariant Result = callUserAPI(RESTClientHelper::POST,
                                "Account/login",{},{
                                    { "emailOrMobile", this->CreatedAdminEmail },
                                    { "pass", "5d12d36cd5f66fe3e72f7b03cbb75333" },
                                    { "salt", "1234" },
                                });
        QVERIFY(Result.isValid());

        gEncodedAdminJWT = Result.toMap().value("token").toString();
        gAdminJWT = QJsonDocument::fromJson(QByteArray::fromBase64(gEncodedAdminJWT.split('.').at(1).toLatin1())).object();

        QVERIFY(clsJWT(gAdminJWT).actorID() == gAdminUserID);
        QVERIFY(clsJWT(gAdminJWT).usrStatus() == TAPI::enuUserStatus::Active);
    }

    /***************************************************************************************/
//    void UploadFiles_save()
//    {
//        QT_TRY {
//            QVariant Result = callAdminAPI(
//                RESTClientHelper::PUT,
//                "Ticketing/UploadFiles/save",
//                {},
//                {},
//                {},
//                {
//                    { "file", "../../README.md" },
//                }
//            );

//        } QT_CATCH (const std::exception &exp) {
//            QTest::qFail(exp.what(), __FILE__, __LINE__);
//        }
//    }

    void Ticket_newMessage() {
        QT_TRY {
            QVariant Result = callUserAPI(
                RESTClientHelper::PUT,
                "Ticketing/newMessage",
                {},
                {
                    { "serviceID", this->LastRandomNumber },
                    { "title", "this is ticket title" },
                    { "body", "this is ticket body" },
                },
                {
                    { "file_1", "../../README.md" },
                }
            );

//            qDebug() << Result;

            this->MainTicketID = Result.toMap().value("id").toUInt();

            QVERIFY(this->MainTicketID > 0);

        } QT_CATCH (const std::exception &exp) {
            QTest::qFail(exp.what(), __FILE__, __LINE__);
        }
    }

    void Ticket_newFeedback_1() {
        QT_TRY {
            QVariant Result = callUserAPI(
                RESTClientHelper::PUT,
                "Ticketing/newFeedback",
                {},
                {
                    { "serviceID", this->LastRandomNumber },
                    { "title", "this is ticket reply title" },
                    { "body", "this is ticket reply body" },
                    { "ticketType", Targoman::API::TicketingModule::enuTicketType::Reply },
                    { "inReplyTicketID", this->MainTicketID },
                },
                {
                    { "file_1", "../../README.md" },
                }
            );

            qDebug() << Result;

            this->Reply1TicketID = Result.toMap().value("id").toUInt();

            QVERIFY(this->Reply1TicketID > 0);

        } QT_CATCH (const std::exception &exp) {
            QTest::qFail(exp.what(), __FILE__, __LINE__);
        }
    }

    void Ticket_newFeedback_2() {
        QT_TRY {
            QVariant Result = callUserAPI(
                RESTClientHelper::PUT,
                "Ticketing/newFeedback",
                {},
                {
                    { "serviceID", this->LastRandomNumber },
                    { "title", "this is ticket reply title" },
                    { "body", "this is ticket reply body" },
                    { "ticketType", Targoman::API::TicketingModule::enuTicketType::Reply },
                    { "inReplyTicketID", this->Reply1TicketID },
                },
                {
                    { "file_1", "../../README.md" },
                }
            );

            qDebug() << Result;

            this->Reply2TicketID = Result.toMap().value("id").toUInt();

            QVERIFY(this->Reply2TicketID > 0);

        } QT_CATCH (const std::exception &exp) {
            QTest::qFail(exp.what(), __FILE__, __LINE__);
        }
    }

    void Tickets_Get() {
        QT_TRY {
            QVariant Result = callUserAPI(
                RESTClientHelper::GET,
                QString("Ticketing/Tickets/%1").arg(this->MainTicketID),
                {},
                {}
            );

            qDebug() << Result;

            QVERIFY(Result.isValid());

        } QT_CATCH (const std::exception &exp) {
            QTest::qFail(exp.what(), __FILE__, __LINE__);
        }
    }
    void Tickets_List() {
        QT_TRY {
            QVariant Result = callUserAPI(
                RESTClientHelper::GET,
                "Ticketing/Tickets",
                {},
                {}
            );

            qDebug() << Result;

            QVERIFY(Result.isValid());

        } QT_CATCH (const std::exception &exp) {
            QTest::qFail(exp.what(), __FILE__, __LINE__);
        }
    }

    void Tickets_setAsRead_1() {
        QT_TRY {
            QVariant Result = callUserAPI(
                RESTClientHelper::PATCH,
                "Ticketing/Tickets/setAsRead/" + QString::number(this->MainTicketID),
                {},
                {}
            );

            qDebug() << Result;

            QVERIFY(Result.isValid());

        } QT_CATCH (const std::exception &exp) {
            QTest::qFail(exp.what(), __FILE__, __LINE__);
        }
    }

    void Tickets_setAsRead_2() {
        QT_TRY {
            QVariant Result = callUserAPI(
                RESTClientHelper::PATCH,
                "Ticketing/Tickets/setAsRead/" + QString::number(this->MainTicketID),
                {},
                {}
            );

            qDebug() << Result;

            QVERIFY(Result.isValid());

        } QT_CATCH (const std::exception &exp) {
            QTest::qFail(exp.what(), __FILE__, __LINE__);
        }
    }

    void Tickets_List_by_baseTicketID() {
        QT_TRY {
            QVariant Result = callUserAPI(
                RESTClientHelper::GET,
                "Ticketing/Tickets",
                {
                    { "baseTicketID", this->MainTicketID },
                },
                {
                });

            qDebug() << Result;

            QVERIFY(Result.isValid());

        } QT_CATCH (const std::exception &exp) {
            QTest::qFail(exp.what(), __FILE__, __LINE__);
        }
    }

    void Tickets_List_by_inReplyTicketID() {
        QT_TRY {
            QVariant Result = callUserAPI(
                RESTClientHelper::GET,
                "Ticketing/Tickets",
                {
                    { "inReplyTicketID", this->MainTicketID },
                },
                {
                });

            qDebug() << Result;

            QVERIFY(Result.isValid());

        } QT_CATCH (const std::exception &exp) {
            QTest::qFail(exp.what(), __FILE__, __LINE__);
        }
    }

    void TicketAttachments_List() {
        QT_TRY {
            QVariant Result = callUserAPI(
                RESTClientHelper::GET,
                "Ticketing/TicketAttachments",
                {
                    { "filters", QString("tat_tktID=%1").arg(this->Reply2TicketID) },
                },
                {}
            );

            qDebug() << Result;

            QVERIFY(Result.isValid());

        } QT_CATCH (const std::exception &exp) {
            QTest::qFail(exp.what(), __FILE__, __LINE__);
        }
    }

private slots:
private:
    /***************************************************************************************/
    /* cleanup *****************************************************************************/
    /***************************************************************************************/
    void cleanupTickets() {
        clsDAC DAC("Ticketing");

        try {
            QString QueryString = R"(
                DELETE t
                  FROM tblTickets t
                 WHERE t.tktID = ?
            ;)";
            clsDACResult DACResult = DAC.execQuery("", QueryString, {
                                                       this->Reply2TicketID
                                                   });
            qDebug() << QVariantMap({{ "(2) numRowsAffected", DACResult.numRowsAffected() }});
        } catch (std::exception &exp) {
            qDebug() << "(2)" << exp.what();
        }

        try {
            QString QueryString = R"(
                DELETE t
                  FROM tblTickets t
                 WHERE t.tktID = ?
            ;)";
            clsDACResult DACResult = DAC.execQuery("", QueryString, {
                                                       this->Reply1TicketID
                                                   });
            qDebug() << QVariantMap({{ "(1) numRowsAffected", DACResult.numRowsAffected() }});
        } catch (std::exception &exp) {
            qDebug() << "(1)" << exp.what();
        }

        try {
            QString QueryString = R"(
                DELETE t
                  FROM tblTickets t
                 WHERE t.tktID = ?
            ;)";
            clsDACResult DACResult = DAC.execQuery("", QueryString, {
                                                       this->MainTicketID
                                                   });
            qDebug() << QVariantMap({{ "(MAIN) numRowsAffected", DACResult.numRowsAffected() }});
        } catch (std::exception &exp) {
            qDebug() << "(MAIN)" << exp.what();
        }
    }

    void cleanupOrphanUploadedFiles() {
        clsDAC DAC("Ticketing");

        try {
            QString QueryString = R"(
                DELETE tblUploadFiles
                  FROM tblUploadFiles
             LEFT JOIN (
                SELECT tat_uplID
                     , COUNT(*) AS _cnt
                  FROM tblTicketAttachments
              GROUP BY tat_uplID
                       ) tmpCnt
                    ON tmpCnt.tat_uplID = tblUploadFiles.uflID
                 WHERE tmpCnt._cnt IS NULL
            ;)";
            clsDACResult DACResult = DAC.execQuery("", QueryString);
            qDebug() << QVariantMap({{ "(ORPHAN) numRowsAffected", DACResult.numRowsAffected() }});
        } catch (std::exception &exp) {
            qDebug() << "(ORPHAN)" << exp.what();
        }
    }

    void cleanupAccountFixture() {
        QT_TRY {
            QVariant Result = callAdminAPI(
                RESTClientHelper::POST,
                "Account/fixtureCleanup",
                {},
                {
                    { "random", this->LastRandomNumber },
                });

            QVERIFY(Result.isValid());

        } QT_CATCH (const std::exception &exp) {
            QTest::qFail(exp.what(), __FILE__, __LINE__);
        }
    }
};

#endif // TEST_TICKETING_HPP
