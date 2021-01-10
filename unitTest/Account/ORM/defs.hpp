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
#ifndef TEST_ACCOUNT_ORM_DEFS_HPP
#define TEST_ACCOUNT_ORM_DEFS_HPP

constexpr char UT_RoleName[] = "UnitTest_Service_Role";
constexpr char UT_ServiceName[] = "UnitTest_Service";
constexpr char UT_AdminToken[] = "Sample_UnitTest_Admin_Token";
constexpr char UT_NormalToken[] = "Sample_UnitTest_Token";

extern unsigned int gServiceRoleID;
extern unsigned int gServiceID;
extern unsigned int gAPITokenID;
extern unsigned int gAPIAdminTokenID;

#endif // TEST_ACCOUNT_ORM_DEFS_HPP
