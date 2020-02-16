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

#include "Ticketing.h"
#include "QFieldValidator.h"
#include "Interfaces/AAA/AAA.hpp"
#include "Interfaces/AAA/PrivHelpers.h"
#include "Interfaces/Common/GenericEnums.hpp"

#include "ORM/Defs.hpp"
#include "ORM/Tickets.h"

namespace Targoman {
namespace API {

TARGOMAN_API_MODULE_DB_CONFIG_IMPL(Ticketing);
using namespace TAPI;

quint64 Ticketing::insertTicket(quint32 _targetUser,
                  quint32 _serviceID,
                  quint64 _inReplyTo,
                  TAPI::enuTicketType::Type _ticketType,
                  const QString& _title,
                  const QString& _body,
                  bool _hasAttachemnt,
                  quint64 _createdBy){
    return this->execQuery(
                                     "INSERT INTO tblTickets "
                                     "   SET tblTickets.tktTarget_usrID =?,"
                                     "       tblTickets.tkt_svcID =?,"
                                     "       tblTickets.tktInReply_tktID =?,"
                                     "       tblTickets.tktType =?,"
                                     "       tblTickets.tktTitle =?,"
                                     "       tblTickets.tktBodyMarkdown =?,"
                                     "       tblTickets.tktHasAttachment =?,"
                                     "       tblTickets.tktCreatedBy_usrID =?"
                                     , {
                                         _targetUser ? _targetUser : QVariant(),
                                         _serviceID ? _serviceID : QVariant(),
                                         _inReplyTo ? _inReplyTo : QVariant(),
                                         QString("%1").arg(_ticketType),
                                         _title,
                                         _body,
                                         _hasAttachemnt,
                                         _createdBy
                                     }
                                     ).lastInsertId().toULongLong();
}

bool Ticketing::apiPUTNewMessage(TAPI::JWT_t _JWT, const QString& _title, const QString& _bodyMarkdown, quint32 _serviceID, quint32 _targetUser){
    Authorization::checkPriv(_JWT, {this->moduleBaseName() + ":canPUTNewMessage"});

    return this->insertTicket(_targetUser, _serviceID, 0, _targetUser ? enuTicketType::Message : enuTicketType::Broadcast, _title, _bodyMarkdown, false, clsJWT(_JWT).usrID()) > 0;
}

bool Ticketing::apiPUTNewFeedback(TAPI::JWT_t _JWT,
                                  const QString& _title,
                                  const QString& _text,
                                  enuTicketType::Type _ticketType,
                                  quint32 _serviceID,
                                  quint64 _inReplyTo,
                                  TAPI::stuFileInfo _file){
    Authorization::checkPriv(_JWT, {});

    if(_inReplyTo && (_ticketType != enuTicketType::Reply))
        throw exHTTPBadRequest("Reply tickets must have reply type");

    if(_ticketType == enuTicketType::Message || _ticketType == enuTicketType::Broadcast)
        throw exHTTPBadRequest("Message and Broadcast tickets must be sent via newMessage method");

    quint64 TicketID = this->insertTicket(_inReplyTo, _serviceID, 0, _ticketType, _title, _text, _file.Size > 0, clsJWT(_JWT).usrID());

    if(_file.Size > 0){
        //TODO move to tiockt attachemnts with ID
        //QFile::rename(_file.TempName, )
    }

    //TODO return  true if file moved
    return true;
}

Ticketing::Ticketing() :
    ORM::clsRESTAPIWithActionLogs ("Ticketing", "Ticketing"){
    TAPI_REGISTER_TARGOMAN_ENUM(TAPI::enuTicketType);

    this->addSubModule(new ORM::Tickets);
    this->addSubModule(new ORM::TicketRead);
}

}
}
