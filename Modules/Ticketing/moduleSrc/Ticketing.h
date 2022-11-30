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
 * @author S. Mehran M. Ziabary <ziabary@targoman.com>
 * @author Kambiz Zandi <kambizzandi@gmail.com>
 */

#ifndef TARGOMAN_API_MODULES_TICKETING_TICKETING_H
#define TARGOMAN_API_MODULES_TICKETING_TICKETING_H

#include "Interfaces/ORM/intfActionLogs.h"
#include "Interfaces/ORM/intfMigrations.h"
#include "Interfaces/ORM/intfConfigurations.h"
#include "Interfaces/ORM/intfI18N.h"
#include "Interfaces/ObjectStorage/ORM/ObjectStorage.h"
#include "libTargomanCommon/Configuration/tmplConfigurable.h"
#include "Interfaces/API/intfSQLBasedModule.h"
#include "ORM/Defs.hpp"
#include "Interfaces/ORM/intfFAQ.h"
using namespace Targoman::API::ORM;

namespace Targoman::API::TicketingModule {

TARGOMAN_MIGRATIONS_PREPARENT;
TARGOMAN_CONFIGURATIONS_PREPARENT;
TARGOMAN_I18N_PREPARENT;
TARGOMAN_ACTIONLOG_PREPARENT;
TARGOMAN_OBJECTSTORAGE_PREPARENT;
TARGOMAN_FAQ_PREPARENT;

class Ticketing : public intfSQLBasedModule //intfSQLBasedWithActionLogsModule
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID INTFPUREMODULE_IID)
    Q_INTERFACES(Targoman::API::API::intfPureModule)
    //---------------------------------------------------------
    TARGOMAN_API_MODULE_DEFINE(Ticketing); //, enuTokenActorType::User);
    //---------------------------------------------------------
    TARGOMAN_API_MODULE_DEFINE_DB_CONFIGS(Ticketing);
    //---------------------------------------------------------
    TARGOMAN_API_MODULE_DEFINE_MIGRATIONS(Ticketing, TicketingSchema);
    TARGOMAN_API_MODULE_DEFINE_CONFIGURATIONS(Ticketing, TicketingSchema);
    TARGOMAN_API_MODULE_DEFINE_I18N(Ticketing, TicketingSchema);
    TARGOMAN_API_MODULE_DEFINE_ACTIONLOG(Ticketing, TicketingSchema);
    TARGOMAN_API_MODULE_DEFINE_OBJECTSTORAGE(Ticketing, TicketingSchema);
    TARGOMAN_API_MODULE_DEFINE_FAQ(Ticketing, TicketingSchema);

private:
    quint64 insertTicket(
            INTFAPICALLCONTEXT_DECL &_apiCallContext,
//            quint64 _createdBy,
            quint64 _targetUserID,
            quint32 _serviceID,
            quint64 _inReplyTicketID,
            enuTicketType::Type _ticketType,
            const QString &_title,
            const QString &_body,
            const TAPI::Files_t &_files = {},
            quint32 _unitID = 0
        );

private slots:
    QVariantMap REST_PUT(
        newMessage,
        (
            APICALLCONTEXT_TYPE_JWT_USER_DECL &_apiCallContext,
            const QString &_title,
            const QString &_body,
            quint32 _serviceID,
            quint64 _targetUserID = 0,
            quint32 _unitID = 0,
            const TAPI::stuFileInfo &_file_1 = {},
            const TAPI::stuFileInfo &_file_2 = {},
            const TAPI::stuFileInfo &_file_3 = {},
            const TAPI::stuFileInfo &_file_4 = {}
        ),
        "create new message targeting a user or all users (if target user is 0)"
    )

    QVariantMap REST_PUT(
        newFeedback,
        (
            APICALLCONTEXT_TYPE_JWT_USER_DECL &_apiCallContext,
            const QString &_title,
            const QString &_body,
            Targoman::API::TicketingModule::enuTicketType::Type _ticketType,
            quint32 _serviceID,
            quint64 _inReplyTicketID = 0,
            const TAPI::stuFileInfo &_file_1 = {},
            const TAPI::stuFileInfo &_file_2 = {},
            const TAPI::stuFileInfo &_file_3 = {},
            const TAPI::stuFileInfo &_file_4 = {}
        ),
        "create a new/reply feedback with"
    )

#ifdef QT_DEBUG
protected slots:
    QVariant REST_POST(
        fixtureSetup,
        (
            APICALLCONTEXT_TYPE_JWT_USER_DECL &_apiCallContext,
            QString _random = {}
        ),
        "Create sample data. give random=1 to auto generate random number"
    )

    QVariant REST_POST(
        fixtureCleanup,
        (
            APICALLCONTEXT_TYPE_JWT_USER_DECL &_apiCallContext,
            QString _random = {}
        ),
        "Cleanup sample data"
    )
#endif

};

TARGOMAN_MIGRATIONS_POSTPARENT(Ticketing, TicketingSchema);
TARGOMAN_CONFIGURATIONS_POSTPARENT(Ticketing, TicketingSchema);
TARGOMAN_I18N_POSTPARENT(Ticketing, TicketingSchema);
TARGOMAN_ACTIONLOG_POSTPARENT(Ticketing, TicketingSchema);
TARGOMAN_OBJECTSTORAGE_POSTPARENT(Ticketing, TicketingSchema);
TARGOMAN_FAQ_POSTPARENT(Ticketing, TicketingSchema);

} //namespace Targoman::API::TicketingModule

#endif // TARGOMAN_API_MODULES_ACCOUNT_TICKETING_H
