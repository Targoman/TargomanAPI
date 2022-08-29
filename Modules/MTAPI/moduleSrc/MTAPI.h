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
 * @author Kambiz Zandi <kambizzandi@gmail.com>
 */

#ifndef TARGOMAN_API_MODULES_MTAPI_MT_H
#define TARGOMAN_API_MODULES_MTAPI_MT_H

#include "libTargomanCommon/Configuration/tmplConfigurable.h"
#include "Interfaces/ORM/intfActionLogs.h"
#include "Interfaces/ORM/intfMigrations.h"
#include "Interfaces/API/intfSQLBasedModule.h"
#include "Interfaces/AAA/AAA.hpp"
#include "Interfaces/AAA/Accounting_Defs.hpp"
#include "Interfaces/ORM/intfFAQ.h"
using namespace Targoman::API::AAA;
using namespace Targoman::API::ORM;
#include "MTAPIDefs.hpp"
#include "ModuleHelpers/MT/Classes/clsDerivedHelperSubmodules.h"

using namespace Targoman::API::ModuleHelpers::MT::Classes;

namespace Targoman::API::MTAPIModule {

TARGOMAN_MIGRATIONS_PREPARENT;
TARGOMAN_ACTIONLOG_PREPARENT;
TARGOMAN_FAQ_PREPARENT;

class MTAPI : public intfSQLBasedModule
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID INTFPUREMODULE_IID)
    Q_INTERFACES(Targoman::API::API::intfPureModule)
    //---------------------------------------------------------
    TARGOMAN_API_MODULE_DEFINE(MTAPI, enuTokenActorType::API);
    //---------------------------------------------------------
    TARGOMAN_API_MODULE_DEFINE_DB_CONFIGS(MTAPI);
    //---------------------------------------------------------
    TARGOMAN_API_MODULE_DEFINE_MIGRATIONS(MTAPI, MTAPISchema);
    TARGOMAN_API_MODULE_DEFINE_ACTIONLOG(MTAPI, MTAPISchema);
    TARGOMAN_API_MODULE_DEFINE_FAQ(MTAPI, MTAPISchema);

public:
    virtual QMap<QString, stuModuleDBInfo> requiredDBs() const;
    virtual void initializeModule();

private slots:
    QVariantMap REST_GET_OR_POST(
        Translate,
        (
            APICALLBOOM_TYPE_JWT_DECL &APICALLBOOM_PARAM,
//            const TAPI::RemoteIP_t& _REMOTE_IP,
//            const QString& _token,
            QString _text,
            QString _dir,
            const QString& _engine = "NMT",
            bool _detailed = false,
            bool _detok = true,
            bool _dic = false,
            bool _dicFull = false
        ),
        "Translates input text if specified engine and language are found."
    )

//    QVariantMap REST_GET_OR_POST(
//        Test,
//        (
//            const TAPI::RemoteIP_t& _REMOTE_IP,
//            const QString& _token,
//            const QString& _arg
//        ),
//        "Test"
//    )

private:
    clsDerivedHelperSubmodules DerivedHelperSubmodules;
};

TARGOMAN_MIGRATIONS_POSTPARENT(MTAPI, MTAPISchema);
TARGOMAN_ACTIONLOG_POSTPARENT(MTAPI, MTAPISchema);
TARGOMAN_FAQ_POSTPARENT(MTAPI, MTAPISchema);

} //namespace Targoman::API::MTAPIModule

#endif // TARGOMAN_API_MODULES_MTAPI_MT_H
