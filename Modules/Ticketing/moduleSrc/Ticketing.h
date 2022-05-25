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
#include "Interfaces/ObjectStorage/ORM/ObjectStorage.h"
#include "libTargomanCommon/Configuration/tmplConfigurable.h"
#include "Interfaces/API/intfSQLBasedWithActionLogsModule.h"
#include "ORM/Defs.hpp"
using namespace Targoman::API::ORM;

namespace Targoman::API::TicketingModule {

TARGOMAN_MIGRATIONS_PREPARENT;
TARGOMAN_ACTIONLOG_PREPARENT;
TARGOMAN_OBJECTSTORAGE_PREPARENT;

class Ticketing : public intfSQLBasedWithActionLogsModule
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID INTFPUREMODULE_IID)
    Q_INTERFACES(Targoman::API::API::intfPureModule)
    TARGOMAN_API_MODULE_DB_CONFIGS(Ticketing);
    TARGOMAN_DEFINE_API_MODULE(Ticketing);
    TARGOMAN_API_DEFINE_MIGRATIONS(Ticketing, TicketingSchema);
    TARGOMAN_API_DEFINE_ACTIONLOG(Ticketing, TicketingSchema);
    TARGOMAN_API_DEFINE_OBJECTSTORAGE(Ticketing, TicketingSchema);

private:
    quint64 insertTicket(
            INTFAPICALLBOOM_DECL &APICALLBOOM_PARAM,
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
            APICALLBOOM_TYPE_JWT_DECL &APICALLBOOM_PARAM,
            const QString &_title,
            const QString &_body,
            quint32 _serviceID,
            quint64 _targetUserID = 0,
            quint32 _unitID = 0,
            const TAPI::stuFileInfo &_file = {}
        ),
        "create new message targeting a user or all users (if target user is 0)"
    )

    QVariantMap REST_PUT(
        newFeedback,
        (
            APICALLBOOM_TYPE_JWT_DECL &APICALLBOOM_PARAM,
            const QString &_title,
            const QString &_body,
            Targoman::API::TicketingModule::enuTicketType::Type _ticketType,
            quint32 _serviceID,
            quint64 _inReplyTicketID = 0,
            const TAPI::stuFileInfo &_file = {}
        ),
        "create a new/reply feedback with"
    )

#ifdef QT_DEBUG
protected slots:
    QVariant REST_POST(
        fixtureSetup,
        (
            APICALLBOOM_TYPE_JWT_DECL &APICALLBOOM_PARAM,
            QString _random = {}
        ),
        "Create sample data. give random=1 to auto generate random number"
    )

    QVariant REST_POST(
        fixtureCleanup,
        (
            APICALLBOOM_TYPE_JWT_DECL &APICALLBOOM_PARAM,
            QString _random = {}
        ),
        "Cleanup sample data"
    )
#endif

};

TARGOMAN_MIGRATIONS_POSTPARENT(Ticketing, TicketingSchema);
TARGOMAN_ACTIONLOG_POSTPARENT(Ticketing, TicketingSchema);
TARGOMAN_OBJECTSTORAGE_POSTPARENT(Ticketing, TicketingSchema);

} //namespace Targoman::API::TicketingModule

#endif // TARGOMAN_API_MODULES_ACCOUNT_TICKETING_H
