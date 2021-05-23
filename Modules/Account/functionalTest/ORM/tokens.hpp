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
 */
#ifndef TEST_ACCOUNT_ORM_APITOKENS_HPP
#define TEST_ACCOUNT_ORM_APITOKENS_HPP

#include "Interfaces/Test/testCommon.hpp"
#include "Interfaces/AAA/clsJWT.hpp"

using namespace Targoman::API::AAA;

class testAPITokens: public clsBaseTest
{
    Q_OBJECT

private slots:
    void APITokens_CREATE_Unpriviledged(){
        QVERIFY(callAPI(PUT, QString("Account/APITokens/"),{},{
                                {"aptToken", UT_NormalToken},
                             }) == gInvalid);
    }

    void APITokens_CREATE_Admin(){
        QVERIFY(callAdminAPI(PUT, QString("Account/APITokens/"),{},{
                                               {"aptToken", UT_NormalToken},
                             }) == gInvalid);
        QVERIFY(callAdminAPI(PUT, QString("Account/APITokens/"),{},{
                                 {"aptToken", UT_NormalToken},
                                 {"apt_svcID", gServiceID},
                             }) == gInvalid);

        QVERIFY(callAdminAPI(PUT, QString("Account/APITokens/"),{},{
                                 {"aptToken", UT_NormalToken},
                                 {"apt_usrID", clsJWT(gJWT).usrID()},
                             }) == gInvalid);

        QVERIFY(callAdminAPI(PUT, QString("Account/APITokens/"),{},{
                                 {"aptToken", UT_NormalToken},
                                 {"apt_svcID", gServiceID},
                                 {"apt_usrID", 0},
                             }) == gInvalid);

        QVERIFY(callAdminAPI(PUT, QString("Account/APITokens/"),{},{
                                 {"aptToken", UT_NormalToken},
                                 {"apt_svcID", 0},
                                 {"apt_usrID", clsJWT(gJWT).usrID()},
                             }) == gInvalid);


        QVERIFY((gAPITokenID =  callAdminAPI(PUT, QString("Account/APITokens/"),{},{
                                                 {"aptToken", UT_NormalToken},
                                                 {"apt_svcID", gServiceID},
                                                 {"apt_usrID", clsJWT(gJWT).usrID()},
                                             }).toUInt()) > 0);
        QVERIFY((gAPIAdminTokenID =  callAdminAPI(PUT, QString("Account/APITokens/"),{},{
                                                 {"aptToken", UT_AdminToken},
                                                 {"apt_svcID", gServiceID},
                                                 {"apt_usrID", clsJWT(gAdminJWT).usrID()},
                                             }).toUInt()) > 0);
    }

    void APITokens_UPDATE_Unprivileged(){
        QVERIFY(callAPI(PATCH, QString("Account/APITokens/%1").arg(gAPITokenID),{},{
                                 {"aptStatus", "Removed"}
                             })== gInvalid);
        QVERIFY(callAPI(PATCH, QString("Account/APITokens/%1").arg(gAPIAdminTokenID),{},{
                                 {"aptStatus", "Removed"}
                             })== gInvalid);
    }

    void APITokens_UPDATE_Admin(){
        QVERIFY(callAdminAPI(PATCH, QString("Account/APITokens/%1").arg(gAPIAdminTokenID),{},{
                                 {"aptStatus", "Removed"}
                             }).toBool());
    }

    void APITokens_DELETE_Unprivileged(){
        QVERIFY(callAPI(DELETE, QString("Account/APITokens/%1").arg(gAPIAdminTokenID)).toBool() == false);
        QVERIFY(callAPI(DELETE, QString("Account/APITokens/%1").arg(gAPITokenID)).toBool());
    }

    void APITokens_DELETE_Admin(){
        QVERIFY(callAdminAPI(DELETE, QString("Account/APITokens/")) == gInvalid);
        QVERIFY(callAdminAPI(DELETE, QString("Account/APITokens/%1").arg(gAPITokenID)).toBool() == false);
        QVERIFY(callAdminAPI(PATCH, QString("Account/APITokens/%1").arg(gAPITokenID),{},{
                                 {"svcStatus", "Active"}
                             }).toBool());
        QVERIFY(callAdminAPI(DELETE, QString("Account/APITokens/%1").arg(gAPITokenID)).toBool());
        QVERIFY(callAdminAPI(PATCH, QString("Account/APITokens/%1").arg(gAPITokenID),{},{
                                 {"svcStatus", "Active"}
                             }).toBool());
    }

    void APITokens_CREATE_recreate(){
        QVERIFY(callAdminAPI(PUT, QString("Account/APITokens/"),{},{
                                                 {"aptToken", UT_NormalToken},
                                                 {"apt_svcID", gServiceID},
                                                 {"apt_usrID", clsJWT(gJWT).usrID()},
                                             }) == gInvalid);
    }

    void APITokens_GET_Unpriviledged(){
        QVERIFY(callAPI(GET, QString("Account/APITokens/")).toMap().size() == 1);
        QVERIFY(callAPI(GET, QString("Account/APITokens/%1").arg(gAPITokenID), {{"cols", "aptToken"}}).toMap().value("aptToken") == UT_NormalToken);
        QVERIFY(callAPI(GET, QString("Account/APITokens/%1").arg(gAPIAdminTokenID), {{"cols", "aptToken"}}).toMap().isEmpty());
    }

    void APITokens_GET_Admin(){
        QVERIFY(callAdminAPI(GET, QString("Account/APITokens/"),{{"cols", "aptToken"}}).toMap().size());
        QVERIFY(callAPI(GET, QString("Account/APITokens/%1").arg(gAPITokenID), {{"cols", "aptToken"}}).toMap().value("aptToken") == UT_NormalToken);
        QVERIFY(callAPI(GET, QString("Account/APITokens/%1").arg(gAPIAdminTokenID), {{"cols", "aptToken"}}).toMap().value("aptToken") == UT_AdminToken);
    }
};


#endif // TEST_ACCOUNT_ORM_APITOKENS_HPP
