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

#ifndef TARGOMAN_API_MODULES_ACCOUNT_ORM_APITOKENS_H
#define TARGOMAN_API_MODULES_ACCOUNT_ORM_APITOKENS_H

#include "QHttp/intfRESTAPIHolder.h"
#include "libTargomanDBM/clsDAC.h"
#include "Helpers/ORM/clsTable.h"

namespace Targoman {
namespace API {
TARGOMAN_DEFINE_ENUM(enuAPITokensStatus,
                     Active   = 'A',
                     Pending  = 'P',
                     CreditFinished    = 'C',
                     Removed  = 'R'
                     )
namespace AAA {

class APITokens : public clsTable
{
    Q_OBJECT
public:
    void init();

private slots:
    QVariant ORMGET("Get APITokens information")
    bool API(UPDATE,,(QHttp::JWT_t _JWT,
                      quint64 _tokenID,
                      QString _token = {},
                      quint32 _userID = {},
                      quint32 _rolID = {},
                      QHttp::ISO639_2_t _lang = {},
                      bool    _validateIP = false,
                      QHttp::Date_t _expiryDate = {},
                      QHttp::JSON_t _extraPrivs = {},
                      Targoman::API::enuAPITokensStatus::Type _status = {}),
             "Update APITokens by priviledged user")

    quint32 API(CREATE,,(QHttp::JWT_t _JWT,
                         QString _token,
                         quint32 _userID,
                         quint32 _rolID,
                         QHttp::ISO639_2_t _lang = {},
                         bool    _validateIP = false,
                         QHttp::Date_t _expiryDate = {},
                         QHttp::JSON_t _extraPrivs = {},
                         Targoman::API::enuAPITokensStatus::Type _status = {}),
             "Create a new APITokens by priviledged user")

    bool API(DELETE,,(QHttp::JWT_t _JWT, QHttp::ExtraPath_t _EXTRAPATH),
             "Delete APITokens")

private:
    APITokens();
    TARGOMAN_DEFINE_SINGLETON_SUBMODULE(Account,APITokens);
};

}
}
}

Q_DECLARE_METATYPE(Targoman::API::enuAPITokensStatus::Type);

#endif // TARGOMAN_API_MODULES_ACCOUNT_ORM_APITOKENS_H
