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

#ifndef TARGOMAN_API_MODULES_TARGOMANMT_TARGOMAN_H
#define TARGOMAN_API_MODULES_TARGOMANMT_TARGOMAN_H

#include "Interfaces/ORM/intfActionLogs.h"
#include "Interfaces/ORM/intfMigrations.h"
#include "Interfaces/ObjectStorage/ORM/ObjectStorage.h"
#include "libTargomanCommon/Configuration/tmplConfigurable.h"
#include "Interfaces/AAA/intfAccountingBasedModule.h"
#include "Interfaces/AAA/AAA.hpp"
#include "Interfaces/AAA/Accounting_Defs.hpp"
#include "Interfaces/ORM/intfFAQ.h"
using namespace Targoman::API::AAA;
using namespace Targoman::API::ORM;
#include "TargomanMTDefs.hpp"
#include "ModuleHelpers/MT/Classes/clsDerivedHelperSubmodules.h"

using namespace Targoman::API::ModuleHelpers::MT::Classes;

namespace Targoman::API::TargomanMTModule {

constexpr char ASSET_ITEM_ADDITIONAL_INTO_KEY_PLUS_MAX_DAYS[] = "plus-max-days";

TARGOMAN_MIGRATIONS_PREPARENT;
TARGOMAN_ACTIONLOG_PREPARENT;
TARGOMAN_OBJECTSTORAGE_PREPARENT;
TARGOMAN_FAQ_PREPARENT;

class TargomanMT : public intfAccountingBasedModule
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID INTFPUREMODULE_IID)
    Q_INTERFACES(Targoman::API::API::intfPureModule)
    //---------------------------------------------------------
    TARGOMAN_API_MODULE_DEFINE(TargomanMT); //, enuTokenActorType::API);
    //---------------------------------------------------------
    TARGOMAN_API_MODULE_DEFINE_DB_CONFIGS(TargomanMT);
    //---------------------------------------------------------
    TARGOMAN_API_MODULE_DEFINE_MIGRATIONS(TargomanMT, TargomanMTSchema);
    TARGOMAN_API_MODULE_DEFINE_ACTIONLOG(TargomanMT, TargomanMTSchema);
    TARGOMAN_API_MODULE_DEFINE_OBJECTSTORAGE(TargomanMT, TargomanMTSchema);
    TARGOMAN_API_MODULE_DEFINE_FAQ(TargomanMT, TargomanMTSchema);

public:
    virtual QMap<QString, stuModuleDBInfo> requiredDBs() const;
    virtual void initializeModule();

protected:
    virtual stuServiceCreditsInfo retrieveServiceCreditsInfo(quint64 _usrID);

    virtual void breakCredit(quint64 _slbID);
    virtual bool isUnlimited(const UsageLimits_t& _limits) const;
    virtual bool isEmpty(const UsageLimits_t& _limits) const;

    virtual void computeAdditives(
        INTFAPICALLBOOM_DECL    &APICALLBOOM_PARAM,
        INOUT stuAssetItem      &_assetItem,
        const stuVoucherItem    *_oldVoucherItem = nullptr
    );

    virtual void computeReferrer(
        INTFAPICALLBOOM_DECL    &APICALLBOOM_PARAM,
        INOUT stuAssetItem      &_assetItem,
        const stuVoucherItem    *_oldVoucherItem = nullptr
    );

    virtual QVariantMap getCustomUserAssetFieldsForQuery(
        INTFAPICALLBOOM_DECL    &APICALLBOOM_PARAM,
        INOUT stuAssetItem      &_assetItem,
        const stuVoucherItem    *_oldVoucherItem = nullptr
    );

private slots:
    QVariantMap REST_GET_OR_POST(
        Translate,
        (
            APICALLBOOM_TYPE_JWT_USER_OR_ANONYMOUSE_DECL &APICALLBOOM_PARAM,
            QString _text,
            QString _dir,
            bool _detailed = false,
            bool _detok = true,
            bool _dic = false,
            bool _dicFull = false
        ),
        "Translates input text"
    )

private:
    clsDerivedHelperSubmodules<TAPI::enuTokenActorType::USER> DerivedHelperSubmodules;
};

TARGOMAN_MIGRATIONS_POSTPARENT(TargomanMT, TargomanMTSchema);
TARGOMAN_ACTIONLOG_POSTPARENT(TargomanMT, TargomanMTSchema);
TARGOMAN_OBJECTSTORAGE_POSTPARENT(TargomanMT, TargomanMTSchema);
TARGOMAN_FAQ_POSTPARENT(TargomanMT, TargomanMTSchema);

} //namespace Targoman::API::TargomanMTModule

#endif // TARGOMAN_API_MODULES_TARGOMANMT_TARGOMAN_H
