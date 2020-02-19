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

#ifndef TARGOMAN_API_MODULES_TICKETING_TICKETING_H
#define TARGOMAN_API_MODULES_TICKETING_TICKETING_H

#include "libTargomanCommon/Configuration/tmplConfigurable.h"

#include "Interfaces/ORM/clsRESTAPIWithActionLogs.h"
#include "ORM/Defs.hpp"

namespace Targoman {
namespace API {

class Ticketing : public ORM::clsRESTAPIWithActionLogs
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID INTFAPIMODULE_IID)
    Q_INTERFACES(Targoman::API::intfAPIModule)
    TARGOMAN_API_MODULE_DB_CONFIGS(Ticketing);
    TARGOMAN_DEFINE_API_MODULE(Ticketing);

private:
    quint64 insertTicket(quint32 _targetUser,
                         quint32 _serviceID,
                         quint64 _inReplyTo,
                         TAPI::enuTicketType::Type _ticketType,
                         const QString& _title,
                         const QString& _body,
                         bool _hasAttachemnt,
                         quint64 _createdBy);

private slots:
    bool REST(PUT, NewMessage, (TAPI::JWT_t _JWT, const QString& _title, const QString& _bodyMarkdown, quint32 _serviceID, quint32 _targetUser = 0),
              "create new message targeting a user or all users (if target user is 0)")
    bool REST(PUT, NewFeedback, (TAPI::JWT_t _JWT,
                                 const QString& _title,
                                 const QString& _text,
                                 TAPI::enuTicketType::Type _ticketType,
                                 quint32 _serviceID,
                                 quint64 _inReplyTo = 0,
                                 TAPI::stuFileInfo _file = {}),
              "create a new/reply feedback with")
    stuDBInfo requiredDB() const {return stuDBInfo("Ticketing");}

};

}
}


#endif // TARGOMAN_API_MODULES_ACCOUNT_TICKETING_H
