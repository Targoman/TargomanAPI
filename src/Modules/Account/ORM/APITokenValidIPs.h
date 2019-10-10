/******************************************************************************
#   TargomanAPI: REST API for Targoman
#
#   Copyright 2014-2019 by Targoman Intelligent Processing <http://tip.co.ir>
#
#   TargomanAPI is free software: you can redistribute it and/or modify
#   it under the terms of the GNU AFFERO GENERAL PUBLIC LICENSE as published by
#   the Free Software Foundation, either version 3 of the License, or
#   (at your option) any later version.
#
#   TargomanAPI is distributed in the hope that it will be useful,
#   but WITHOUT ANY WARRANTY; without even the implied warranty of
#   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#   GNU AFFERO GENERAL PUBLIC LICENSE for more details.
#
#   You should have received a copy of the GNU AFFERO GENERAL PUBLIC LICENSE
#   along with Targoman. If not, see <http://www.gnu.org/licenses/>.
 ******************************************************************************/
/**
 @author S. Mehran M. Ziabary <ziabary@targoman.com>
 */

#ifndef TARGOMAN_API_MODULES_AAA_ORM_APITOKENVALIDIPS_H
#define TARGOMAN_API_MODULES_AAA_ORM_APITOKENVALIDIPS_H

#include "QHttp/intfRESTAPIHolder.h"
#include "libTargomanDBM/clsDAC.h"
#include "Helpers/ORM/intfTable.h"

namespace Targoman {
namespace API {

class APITokenValidIPs : public QHttp::intfRESTAPIHolder, private intfTable
{
    Q_OBJECT
public:
    void init();

private slots:
    QVariant ORMGET("Get APITokenValidIPs information")

private:
    APITokenValidIPs();
    TARGOMAN_DEFINE_SINGLETON_SUBMODULE(Account,APITokenValidIPs);
};

}
}
#endif // TARGOMAN_API_MODULES_AAA_ORM_APITOKENVALIDIPS_H
