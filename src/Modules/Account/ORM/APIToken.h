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

#ifndef TARGOMAN_API_MODULES_AAA_ORM_APITOKEN_H
#define TARGOMAN_API_MODULES_AAA_ORM_APITOKEN_H

#include "QHttp/intfRESTAPIHolder.h"
#include "libTargomanDBM/clsDAC.h"
#include "Helpers/ORM/intfTable.h"

namespace Targoman {
namespace API {

TARGOMAN_DEFINE_ENUM(enuAPITokenStatus,
                     Active   = 'A',
                     Pending  = 'P',
                     CreditFinished    = 'C',
                     Removed  = 'R'
                     )

class APIToken : public QHttp::intfRESTAPIHolder, private intfTable
{
    Q_OBJECT
public:
    void init();

private slots:
    QVariant ORMGET("Get APIToken information")
    bool API(UPDATE,,(QHttp::JWT_t _JWT,
                      quint64 _tokenID,
                      QString _token = {},
                      quint32 _userID = {},
                      quint32 _rolID = {},
                      QHttp::ISO639_2_t _lang = {},
                      bool    _validateIP = false,
                      QHttp::Date_t _expiryDate = {},
                      QHttp::JSON_t _extraPrivs = {},
                      Targoman::API::enuAPITokenStatus::Type _status = {}),
             "Update APIToken by priviledged user")

    quint32 API(CREATE,,(QHttp::JWT_t _JWT,
                         QString _token,
                         quint32 _userID,
                         quint32 _rolID,
                         QHttp::ISO639_2_t _lang = {},
                         bool    _validateIP = false,
                         QHttp::Date_t _expiryDate = {},
                         QHttp::JSON_t _extraPrivs = {},
                         Targoman::API::enuAPITokenStatus::Type _status = {}),
             "Create a new APIToken by priviledged user")

    bool API(DELETE,,(QHttp::JWT_t _JWT, QHttp::ExtraPath_t _EXTRAPATH),
             "Delete APIToken")

private:
    APIToken();
    TARGOMAN_DEFINE_SINGLETON_SUBMODULE(Account,APIToken);
};

}
}
Q_DECLARE_METATYPE(Targoman::API::enuAPITokenStatus::Type);

#endif // TARGOMAN_API_MODULES_AAA_ORM_APITOKEN_H
