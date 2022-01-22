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

#include "Ticketing.h"
#include "ORM/Defs.hpp"
#include "ORM/Tickets.h"
#include "ORM/TicketAttachments.h"
#include "Interfaces/AAA/AAA.hpp"
#include "Interfaces/AAA/PrivHelpers.h"
#include "Interfaces/Common/GenericEnums.hpp"
#include "Interfaces/Helpers/ObjectStorageHelper.h"
#include "libQFieldValidator/QFieldValidator.h"

using namespace Targoman::API::Helpers;

TAPI_REGISTER_TARGOMAN_ENUM(Targoman::API::TicketingModule, enuTicketType);

namespace Targoman::API::TicketingModule {

using namespace ORM;

TARGOMAN_API_MODULE_DB_CONFIG_IMPL(Ticketing, TicketingSchema);
TARGOMAN_API_OBJECTSTORAGE_CONFIG_IMPL(Ticketing, TicketingSchema)

Ticketing::Ticketing() :
    intfSQLBasedWithActionLogsModule(TicketingDomain, TicketingSchema)
{
    TARGOMAN_API_IMPLEMENT_ACTIONLOG(Ticketing, TicketingSchema)
    TARGOMAN_API_IMPLEMENT_OBJECTSTORAGE(Ticketing, TicketingSchema)

    this->addSubModule(&Tickets::instance());
    this->addSubModule(&TicketRead::instance());
    this->addSubModule(&TicketAttachments::instance());
}

quint64 Ticketing::insertTicket(
        quint64 _createdBy,
        quint64 _targetUserID,
        quint32 _serviceID,
        quint64 _inReplyTicketID,
        enuTicketType::Type _ticketType,
        const QString &_title,
        const QString &_body,
        const TAPI::Files_t &_files
    )
{
    TAPI::ORMFields_t CreateFields({
       { tblTickets::tktType, _ticketType },
       { tblTickets::tktTitle, _title },
       { tblTickets::tktBody, _body },
    });

    if (_targetUserID > 0)
        CreateFields.insert(tblTickets::tktTarget_usrID, _targetUserID);

    if (_serviceID > 0)
        CreateFields.insert(tblTickets::tkt_svcID, _serviceID);

    if (_inReplyTicketID > 0)
        CreateFields.insert(tblTickets::tktInReply_tktID, _inReplyTicketID);

    quint64 TicketID = this->Create(Tickets::instance(), _createdBy, CreateFields);

    if (_files.isEmpty() == false)
    {
        CreateQuery QueryCreateAttachments = CreateQuery(TicketAttachments::instance())
                                             .addCol(tblTicketAttachments::tat_tktID)
                                             .addCol(tblTicketAttachments::tat_uplID)
                                             ;

        foreach(auto _file, _files)
        {
            try
            {
                quint64 UploadedFileID = ObjectStorageHelper::saveFile(
                                             _createdBy,
                                             UploadFiles::instance(),
                                             UploadQueue::instance(),
                                             UploadGateways::instance(),
                                             _file
                                             );
                if (UploadedFileID > 0)
                    QueryCreateAttachments.values(QVariantMap({
                                                                  { tblTicketAttachments::tat_tktID, TicketID },
                                                                  { tblTicketAttachments::tat_uplID, UploadedFileID },
                                                              }));
            }
            catch (std::exception &exp)
            {
                TargomanDebug(5, "ObjectStorageHelper::saveFile(" << _file.Name << "):" << exp.what());
            }
        }

        QueryCreateAttachments.execute(_createdBy);
    }

    return TicketID;
}

QVariantMap Ticketing::apiPUTnewMessage(
        TAPI::JWT_t _JWT,
        const QString &_title,
        const QString &_body,
        quint32 _serviceID,
        quint64 _targetUserID,
        const TAPI::stuFileInfo &_file
    )
{
    Authorization::checkPriv(_JWT, { this->moduleBaseName() + ":canPUTNewMessage" });

    TAPI::Files_t Files;
    Files.append(_file);

    quint64 ID = this->insertTicket(
                     clsJWT(_JWT).usrID(),
                     _targetUserID,
                     _serviceID,
                     0,
                     _targetUserID ? enuTicketType::Message : enuTicketType::Broadcast,
                     _title,
                     _body,
                     Files
                     );

    return QVariantMap({
                           { "id", ID },
                       });
}

QVariantMap Ticketing::apiPUTnewFeedback(
        TAPI::JWT_t _JWT,
        const QString &_title,
        const QString &_body,
        Targoman::API::TicketingModule::enuTicketType::Type _ticketType,
        quint32 _serviceID,
        quint64 _inReplyTicketID,
        const TAPI::stuFileInfo &_file
    )
{
  Authorization::checkPriv(_JWT, {});

  if (_inReplyTicketID && (_ticketType != enuTicketType::Reply))
    throw exHTTPBadRequest("Reply tickets must have reply type");

  if (_ticketType == enuTicketType::Message ||
      _ticketType == enuTicketType::Broadcast)
    throw exHTTPBadRequest(
        "Message and Broadcast tickets must be sent via newMessage method");

  TAPI::Files_t Files;
  Files.append(_file);

  quint64 ID = this->insertTicket(
                   clsJWT(_JWT).usrID(),
                   0,
                   _serviceID,
                   _inReplyTicketID,
                   _ticketType,
                   _title,
                   _body,
                   Files
                   );

  return QVariantMap({
                         { "id", ID },
                     });
}

}  // namespace Targoman::API::TicketingModule
