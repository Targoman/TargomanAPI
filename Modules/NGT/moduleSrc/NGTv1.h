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
 * @author S. Mehran M. Ziabary <ziabary@targoman.com>
 */

#ifndef TARGOMAN_API_MODULES_NGT_NGTV1_H
#define TARGOMAN_API_MODULES_NGT_NGTV1_H

#include "Interfaces/ORM/intfActionLogs.h"
#include "Interfaces/ORM/intfMigrations.h"
#include "libTargomanCommon/Configuration/tmplConfigurable.h"
#include "Interfaces/API/intfSQLBasedWithActionLogsModule.h"
#include "Interfaces/AAA/AAA.hpp"
#include "ORM/Defs.hpp"
#include "Interfaces/ORM/intfFAQ.h"

namespace TAPI {
struct stuNGTPriceInfo{
    quint32 basePPW;
    TAPI::MD5_t FileMD5;
};
}

namespace Targoman::API {

TARGOMAN_MIGRATIONS_PREPARENT;
TARGOMAN_ACTIONLOG_PREPARENT;
TARGOMAN_FAQ_PREPARENT;

class NGTv1 : public intfSQLBasedWithActionLogsModule
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID INTFPUREMODULE_IID)
    Q_INTERFACES(Targoman::API::API::intfPureModule)
    TARGOMAN_API_MODULE_DB_CONFIGS(NGTv1);
    TARGOMAN_DEFINE_API_MODULE(NGTv1)
    TARGOMAN_API_DEFINE_MIGRATIONS(NGTv1, NGTv1Schema);
    TARGOMAN_API_DEFINE_ACTIONLOG(NGTv1, NGTv1Schema);
    TARGOMAN_API_DEFINE_FAQ(NGTv1, NGTv1Schema);

private slots:
    //stuNGTPriceInfo API(POST, RetrievePriceInfo, ())
};

TARGOMAN_MIGRATIONS_POSTPARENT(NGTv1, NGTv1Schema);
TARGOMAN_ACTIONLOG_POSTPARENT(NGTv1, NGTv1Schema);
TARGOMAN_FAQ_POSTPARENT(NGTv1, NGTv1Schema);

} //namespace Targoman::API

Q_DECLARE_METATYPE(TAPI::stuNGTPriceInfo);

#endif // TARGOMAN_API_MODULES_NGT_NGTV1_H
