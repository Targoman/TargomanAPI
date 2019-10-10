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

#ifndef TARGOMAN_API_HELPERS_AAA_PRIVHELPERS_HPP
#define TARGOMAN_API_HELPERS_AAA_PRIVHELPERS_HPP

#include <QJsonObject>
#include <QJsonArray>
#include <QStringList>
#include "AAADefs.hpp"

namespace Targoman {
namespace API {
namespace Helpers {
namespace AAA {

class PrivHelpers{
public:
    static QJsonObject digestPrivileges(const QJsonArray& _privs, const QStringList& _requiredTLPs);
    static char hasPrivBase(const QJsonObject& _privs, const QString& _requiredAccess);
    static QJsonObject confirmPriviledgeBase(const QJsonObject& _privs, const QStringList& _requiredAccess);
    static void validateToken(const QString& _token);
    static QVariant getPrivValue(const QJsonObject& _privs, const QString& _selector);
    static QJsonObject processObjectPrivs(QJsonObject& _object, const QStringList& _requiredAccess = {}, const QStringList& _requiredTLPs = {});
};

}
}
}
}

using namespace Targoman::API::Helpers::AAA;

#endif // TARGOMAN_API_HELPERS_AAA_PRIVHELPERS_HPP
