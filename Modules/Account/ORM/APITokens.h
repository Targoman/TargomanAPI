/******************************************************************************
#   TargomanAPI: REST API for Targoman
#
#   Copyright 2014-2020 by Targoman Intelligent Processing <http://tip.co.ir>
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

#ifndef TARGOMAN_API_MODULES_ACCOUNT_ORM_APITOKENS_H
#define TARGOMAN_API_MODULES_ACCOUNT_ORM_APITOKENS_H

#include "Interfaces/ORM/clsTable.h"
#include "Interfaces/AAA/AAA.hpp"

namespace TAPI{
TARGOMAN_DEFINE_ENUM(enuAPITokensStatus,
                     Active   = 'A',
                     Pending  = 'P',
                     CreditFinished    = 'C',
                     Removed  = 'R'
                                )
}

namespace Targoman {
namespace API {
namespace AAA {

class APITokens : public ORM::clsTable
{
    Q_OBJECT
private slots:
    QVariant ORMGET("Get APITokens information")
    bool ORMDELETE("Delete an APIToken")
    bool ORMUPDATE("Update token info by priviledged user")
    quint32 ORMCREATE("Create a new APITokens by priviledged user")

    private:
        TARGOMAN_DEFINE_API_SUBMODULE(Account,APITokens)
};

}
}
}

TAPI_DECLARE_METATYPE(TAPI::enuAPITokensStatus::Type);

#endif // TARGOMAN_API_MODULES_ACCOUNT_ORM_APITOKENS_H
