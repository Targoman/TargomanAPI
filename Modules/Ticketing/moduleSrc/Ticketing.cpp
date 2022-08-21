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
#include "libQFieldValidator/QFieldValidator.h"
#include "ORM/Defs.hpp"
#include "ORM/Departments.h"
#include "ORM/Units.h"
#include "ORM/Tickets.h"
#include "ORM/TicketAttachments.h"
#include "Interfaces/AAA/AAA.hpp"
#include "Interfaces/AAA/PrivHelpers.h"
#include "Interfaces/Common/GenericEnums.hpp"
#include "Interfaces/ObjectStorage/ObjectStorageManager.h"
#include "Interfaces/ObjectStorage/Gateways/gtwNFS.h"
#include "Interfaces/ObjectStorage/Gateways/gtwAWSS3.h"
using namespace Targoman::API::ObjectStorage;

#include "Interfaces/Helpers/RESTClientHelper.h"
#include "Interfaces/Helpers/FixtureHelper.h"
using namespace Targoman::API::Helpers;

TAPI_REGISTER_TARGOMAN_ENUM(Targoman::API::TicketingModule, enuTicketType);

namespace Targoman::API::TicketingModule {

using namespace ORM;

TARGOMAN_API_MODULE_IMPLEMENT(Ticketing)
TARGOMAN_API_MODULE_IMPLEMENT_DB_CONFIG(Ticketing, TicketingSchema);
TARGOMAN_API_OBJECTSTORAGE_CONFIG_IMPL(Ticketing, TicketingSchema)

Ticketing::Ticketing() :
    intfSQLBasedModule( //intfSQLBasedWithActionLogsModule(
        TicketingDomain,
        TicketingSchema
) {
    TARGOMAN_API_MODULE_IMPLEMENT_MIGRATIONS(Ticketing, TicketingSchema)
    TARGOMAN_API_MODULE_IMPLEMENT_ACTIONLOG(Ticketing, TicketingSchema)
    TARGOMAN_API_MODULE_IMPLEMENT_OBJECTSTORAGE(Ticketing, TicketingSchema)
    TARGOMAN_API_MODULE_IMPLEMENT_FAQ(Ticketing, TicketingSchema)

    this->addSubModule(&Departments::instance());
    this->addSubModule(&Units::instance());
    this->addSubModule(&Tickets::instance());
    this->addSubModule(&TicketRead::instance());
    this->addSubModule(&TicketAttachments::instance());
}

quint64 Ticketing::insertTicket(
    INTFAPICALLBOOM_IMPL &APICALLBOOM_PARAM,
//    quint64 _createdBy,
    quint64 _targetUserID,
    quint32 _serviceID,
    quint64 _inReplyTicketID,
    enuTicketType::Type _ticketType,
    const QString &_title,
    const QString &_body,
    const TAPI::Files_t &_files,
    quint32 _unitID
) {
    TAPI::ORMFields_t CreateFields({
       { tblTickets::Fields::tktType, _ticketType },
       { tblTickets::Fields::tktTitle, _title },
       { tblTickets::Fields::tktBody, _body },
    });

    if (_targetUserID > 0)
        CreateFields.insert(tblTickets::Fields::tktTarget_usrID, _targetUserID);

    if (_serviceID > 0)
        CreateFields.insert(tblTickets::Fields::tkt_svcID, _serviceID);

    if (_inReplyTicketID > 0)
        CreateFields.insert(tblTickets::Fields::tktInReply_tktID, _inReplyTicketID);

    if (_unitID > 0)
        CreateFields.insert(tblTickets::Fields::tkt_untID, _unitID);

    quint64 TicketID = Tickets::instance().Create(APICALLBOOM_PARAM,
                                    CreateFields
                                    );

    if (_files.isEmpty() == false) {
        ORMCreateQuery QueryCreateAttachments = TicketAttachments::instance().makeCreateQuery(APICALLBOOM_PARAM)
                                             .addCol(tblTicketAttachments::Fields::tat_tktID)
                                             .addCol(tblTicketAttachments::Fields::tat_uplID)
                                             ;

        foreach (auto _file, _files) {
            if (_file.Size > 0) {
                try {
                    quint64 UploadedFileID = ObjectStorageManager::saveFile(
                                                 APICALLBOOM_PARAM,
                                                 UploadFiles::instance(),
                                                 UploadQueue::instance(),
                                                 UploadGateways::instance(),
                                                 _file,
                                                 "tickets"
                                                 );
                    if (UploadedFileID > 0)
                        QueryCreateAttachments.values(QVariantMap({
                                                                      { tblTicketAttachments::Fields::tat_tktID, TicketID },
                                                                      { tblTicketAttachments::Fields::tat_uplID, UploadedFileID },
                                                                  }));
                } catch (std::exception &exp) {
                    TargomanDebug(5, "ObjectStorageManager::saveFile(" << _file.Name << "):" << exp.what());
                }
            }
        }

        QueryCreateAttachments.execute(APICALLBOOM_PARAM.getActorID());
    }

    return TicketID;
}

QVariantMap IMPL_REST_PUT(Ticketing, newMessage, (
    APICALLBOOM_TYPE_JWT_IMPL &APICALLBOOM_PARAM,
    const QString &_title,
    const QString &_body,
    quint32 _serviceID,
    quint64 _targetUserID,
    quint32 _unitID,
    const TAPI::stuFileInfo &_file_1,
    const TAPI::stuFileInfo &_file_2,
    const TAPI::stuFileInfo &_file_3,
    const TAPI::stuFileInfo &_file_4
)) {
//    Authorization::checkPriv(APICALLBOOM_PARAM, { this->moduleBaseName() + ":canPUTNewMessage" });

    TAPI::Files_t Files;
    if (_file_1.Size > 0) Files.append(_file_1);
    if (_file_2.Size > 0) Files.append(_file_2);
    if (_file_3.Size > 0) Files.append(_file_3);
    if (_file_4.Size > 0) Files.append(_file_4);

    quint64 ID = this->insertTicket(
                     APICALLBOOM_PARAM,
                     _targetUserID,
                     _serviceID,
                     0,
                     _targetUserID ? enuTicketType::Message : enuTicketType::Broadcast,
                     _title,
                     _body,
                     Files,
                     _unitID
                     );

    return QVariantMap({
                           { "id", ID },
                       });
}

QVariantMap IMPL_REST_PUT(Ticketing, newFeedback, (
    APICALLBOOM_TYPE_JWT_IMPL &APICALLBOOM_PARAM,
    const QString &_title,
    const QString &_body,
    Targoman::API::TicketingModule::enuTicketType::Type _ticketType,
    quint32 _serviceID,
    quint64 _inReplyTicketID,
    const TAPI::stuFileInfo &_file_1,
    const TAPI::stuFileInfo &_file_2,
    const TAPI::stuFileInfo &_file_3,
    const TAPI::stuFileInfo &_file_4
)) {
    Authorization::checkPriv(APICALLBOOM_PARAM, {});

    if (_inReplyTicketID && (_ticketType != enuTicketType::Reply))
        throw exHTTPBadRequest("Reply tickets must have reply type");

    if (_ticketType == enuTicketType::Message
            || _ticketType == enuTicketType::Broadcast)
        throw exHTTPBadRequest("Message and Broadcast tickets must be sent via newMessage method");

    TAPI::Files_t Files;
    if (_file_1.Size > 0) Files.append(_file_1);
    if (_file_2.Size > 0) Files.append(_file_2);
    if (_file_3.Size > 0) Files.append(_file_3);
    if (_file_4.Size > 0) Files.append(_file_4);

    quint64 ID = this->insertTicket(
                     APICALLBOOM_PARAM,
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

/****************************************************************\
|** fixture *****************************************************|
\****************************************************************/
#ifdef QT_DEBUG
QVariant IMPL_REST_POST(Ticketing, fixtureSetup, (
    APICALLBOOM_TYPE_JWT_IMPL &APICALLBOOM_PARAM,
    QString _random
)) {
    QVariantMap Result;

    if (_random == "1")
        _random = QString::number(QRandomGenerator::global()->generate());

    if (_random.isEmpty() == false)
        Result.insert("Random", _random);

//    clsDAC DAC;
/*
    //-- upload gateways
    DBManager::clsDACResult ugwData = this->execQuery(R"(
            SELECT ugwType AS type,
                   COUNT(*) AS cnt
              FROM tblUploadGateways
             WHERE ugwName LIKE '%fixture%'
          GROUP BY ugwType
    )");
    QJsonArray ugwArray = ugwData.toJson(false).array();
    QVariantMap UploadGatewaysRows;
    foreach (auto ugwRow, ugwArray) {
        UploadGatewaysRows.insert(ugwRow["type"].toString(), ugwRow["cnt"].toVariant());
    }

    if (UploadGatewaysRows.contains(QChar(enuUploadGatewayType::NFS)) == false) {
        auto res = CreateQuery(UploadGateways::instance())
                .addCols({
                             tblUploadGateways::Fields::ugwName,
                             tblUploadGateways::Fields::ugwType,
                             tblUploadGateways::Fields::ugwMetaInfo,
                })
                .values({
                            { tblUploadGateways::Fields::ugwName, "fixture NFS 1" },
                            { tblUploadGateways::Fields::ugwType, enuUploadGatewayType::NFS },
                            { tblUploadGateways::Fields::ugwMetaInfo, QVariantMap({
                                { Gateways::NFSMetaInfoJsonKey::Path, ".../ticketing" },
                            })},
                        })
                .execute(APICALLBOOM_PARAM.getActorID())
                ;
        Result.insert("NFS", res);
    }

    if (UploadGatewaysRows.contains(QChar(enuUploadGatewayType::AWSS3)) == false) {
        auto res = CreateQuery(UploadGateways::instance())
                .addCols({
                             tblUploadGateways::Fields::ugwName,
                             tblUploadGateways::Fields::ugwType,
                             tblUploadGateways::Fields::ugwMetaInfo,
                })
                .values({
                            { tblUploadGateways::Fields::ugwName, "fixture S3 1" },
                            { tblUploadGateways::Fields::ugwType, enuUploadGatewayType::AWSS3 },
                            { tblUploadGateways::Fields::ugwMetaInfo, QVariantMap({
                                  { Gateways::AWSS3MetaInfoJsonKey::Bucket, "dev.tapi-ticketing" },
                                  { Gateways::AWSS3MetaInfoJsonKey::EndpointUrl, "https://TEST-S3.DOM" },
                                  { Gateways::AWSS3MetaInfoJsonKey::SecretKey, "0" },
                                  { Gateways::AWSS3MetaInfoJsonKey::AccessKey, "1" },
                            })},
                        })
                .execute(APICALLBOOM_PARAM.getActorID())
                ;
        Result.insert("S3", res);
    }
*/
    //-- newMessage
    QVariant res = RESTClientHelper::callAPI(
        APICALLBOOM_PARAM,
       RESTClientHelper::PUT,
       "Ticketing/newMessage",
       {},
       {
           { "serviceID", _random },
           { "title", FixtureHelper::MakeRandomizeName(_random, " ", "ticket", "title") },
           { "body", FixtureHelper::MakeRandomizeName(_random, " ", "ticket", "body") },
       },
       {
//           { "file_1", "../../README.md" },
       }
    );
    quint32 TicketID = res.toMap().value("id").toUInt();
    Result.insert("Ticket", res);

    //-- newFeedback(1)
    res = RESTClientHelper::callAPI(
        APICALLBOOM_PARAM,
        RESTClientHelper::PUT,
        "Ticketing/newFeedback",
        {},
        {
            { "serviceID", _random },
            { "title", FixtureHelper::MakeRandomizeName(_random, " ", "ticket reply (1)", "title") },
            { "body", FixtureHelper::MakeRandomizeName(_random, " ", "ticket reply (1)", "body") },
            { "ticketType", enuTicketType::Reply },
            { "inReplyTicketID", TicketID },
        },
        {
//            { "file_1", "../../README.md" },
        }
    );
    quint32 Reply1TicketID = res.toMap().value("id").toUInt();
    Result.insert("Reply(1)", res);

    //-- newFeedback(2)
    res = RESTClientHelper::callAPI(
        APICALLBOOM_PARAM,
        RESTClientHelper::PUT,
        "Ticketing/newFeedback",
        {},
        {
            { "serviceID", _random },
            { "title", FixtureHelper::MakeRandomizeName(_random, " ", "ticket reply (2)", "title") },
            { "body", FixtureHelper::MakeRandomizeName(_random, " ", "ticket reply (2)", "body") },
            { "ticketType", enuTicketType::Reply },
            { "inReplyTicketID", Reply1TicketID },
        },
        {
//            { "file_1", "../../README.md" },
        }
    );
//    quint32 Reply2TicketID = res.toMap().value("id").toUInt();
    Result.insert("Reply(2)", res);

    //----------------------------------------
    return Result;
}

QVariant IMPL_REST_POST(Ticketing, fixtureCleanup, (
    APICALLBOOM_TYPE_JWT_IMPL &APICALLBOOM_PARAM,
    QString _random
)) {
    QVariantMap Result;

    try {
        QString QueryString = R"(
            DELETE t
              FROM tblTickets t
             WHERE t.tkt_svcID=?
        ;)";
        clsDACResult DACResult = this->execQuery(APICALLBOOM_PARAM,
                                                 QueryString, {
                                                     _random.toInt()
                                                 });

        Result.insert("tblTickets", QVariantMap({
                                                   { "random", _random },
                                                   { "numRowsAffected", DACResult.numRowsAffected() },
                                               }));
    } catch (...) { ; }

    return Result;
}
#endif

}  // namespace Targoman::API::TicketingModule
