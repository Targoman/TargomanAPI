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

#include "ApprovalRequest.h"
#include "User.h"
#include "../Account.h"
#include "Interfaces/Helpers/PhoneHelper.h"
using namespace Targoman::API::Helpers;

TAPI_REGISTER_TARGOMAN_ENUM(Targoman::API::AccountModule, enuAPRStatus);
TAPI_REGISTER_TARGOMAN_ENUM(Targoman::API::AccountModule, enuApprovalType);

using namespace Targoman::Common::Configuration;

namespace Targoman::API::AccountModule::ORM {

tmplConfigurable<quint32> ApprovalRequest::EmailResendApprovalCodeTTL(
    AAA::makeConfig("EmailResendApprovalCodeTTL"),
    "Time to live for the email resend approval code",
    static_cast<quint16>(120),
    ReturnTrueCrossValidator(),
    "",
    "",
    "email-resend-approval-code-ttl",
    enuConfigSource::Arg | enuConfigSource::File
);

tmplConfigurable<quint32> ApprovalRequest::MobileResendApprovalCodeTTL(
    AAA::makeConfig("MobileResendApprovalCodeTTL"),
    "Time to live for the mobile resend approval code",
    static_cast<quint16>(120),
    ReturnTrueCrossValidator(),
    "",
    "",
    "mobile-resend-approval-code-ttl",
    enuConfigSource::Arg | enuConfigSource::File
);

tmplConfigurable<quint32> ApprovalRequest::EmailExpireApprovalCodeTTL(
    AAA::makeConfig("EmailExpireApprovalCodeTTL"),
    "Time to live for the email expire approval code",
    static_cast<quint16>(24*60*60), //one day
    ReturnTrueCrossValidator(),
    "",
    "",
    "email-expire-approval-code-ttl",
    enuConfigSource::Arg | enuConfigSource::File
);

tmplConfigurable<quint32> ApprovalRequest::MobileExpireApprovalCodeTTL(
    AAA::makeConfig("MobileExpireApprovalCodeTTL"),
    "Time to live for the mobile expire approval code",
    static_cast<quint16>(15*60), //15 minutes
    ReturnTrueCrossValidator(),
    "",
    "",
    "mobile-expire-approval-code-ttl",
    enuConfigSource::Arg | enuConfigSource::File
);

TARGOMAN_API_SUBMODULE_IMPLEMENT(Account, ApprovalRequest)

ApprovalRequest::ApprovalRequest() :
    intfSQLBasedModule(
        AAASchema,
        tblApprovalRequest::Name,
        tblApprovalRequest::Private::ORMFields,
        tblApprovalRequest::Private::Relations,
        tblApprovalRequest::Private::Indexes
    ) { ; }

QVariant IMPL_ORMGET_USER(ApprovalRequest) {
    Authorization::checkPriv(APICALLBOOM_PARAM, this->privOn(EHTTP_GET, this->moduleBaseName()));

    return this->Select(GET_METHOD_ARGS_CALL_VALUES);

//    return query.one();

    //    return this->selectFromTable({}, {}, GET_METHOD_CALL_ARGS_APICALL);
}

bool IMPL_ORMDELETE_USER(ApprovalRequest) {
    Authorization::checkPriv(APICALLBOOM_PARAM, this->privOn(EHTTP_DELETE, this->moduleBaseName()));

    return this->DeleteByPks(DELETE_METHOD_ARGS_CALL_VALUES, {}, true);
//    return this->deleteByPKs(DELETE_METHOD_CALL_ARGS_APICALL, {}, true);
}

QVariant IMPL_REST_GET_OR_POST(ApprovalRequest, timerInfo, (
    APICALLBOOM_TYPE_JWT_ANONYMOUSE_IMPL &APICALLBOOM_PARAM,
    QString _emailOrMobile
)) {
    Authorization::validateIPAddress(APICALLBOOM_PARAM, APICALLBOOM_PARAM.getIP());

    enuApprovalType::Type Type;

    if (QFV.email().isValid(_emailOrMobile)) {
        if (QFV.emailNotFake().isValid(_emailOrMobile))
            Type = enuApprovalType::Email;
        else
            throw exHTTPBadRequest("Email domain is suspicious. Please use a real email.");
    } else if (QFV.mobile().isValid(_emailOrMobile)) {
        Type = enuApprovalType::Mobile;
        _emailOrMobile = PhoneHelper::NormalizePhoneNumber(_emailOrMobile);
    } else
        throw exHTTPBadRequest("emailOrMobile must be a valid email or mobile");

    QVariantMap Info = this->makeSelectQuery(APICALLBOOM_PARAM)
//                            .addCols(ApprovalRequest::instance().selectableColumnNames())
//                            .addCol(Targoman::API::CURRENT_TIMESTAMP)
                            .where({ tblApprovalRequest::Fields::aprApprovalKey, enuConditionOperator::Equal, _emailOrMobile })
                            .andWhere({ tblApprovalRequest::Fields::aprRequestedFor, enuConditionOperator::Equal, Type })
                            .andWhere({ tblApprovalRequest::Fields::aprStatus, enuConditionOperator::In, "'N', 'S', 'A', '1', '2', 'E'" })
                            .orderBy(tblApprovalRequest::Fields::aprRequestDate, enuOrderDir::Descending)
                            .one();

    tblApprovalRequest::DTO ApprovalRequestInfoDTO;
    ApprovalRequestInfoDTO.fromJson(QJsonObject::fromVariantMap(Info));

    if ((ApprovalRequestInfoDTO.aprStatus == enuAPRStatus::New) || ApprovalRequestInfoDTO.aprSentDate.isNull())
        throw exTargomanBase("Code not sent to the client");

    if ((ApprovalRequestInfoDTO.aprStatus == enuAPRStatus::Applied) || (ApprovalRequestInfoDTO.aprApplyDate.isNull() == false))
        throw exTargomanBase("Already applied before");

    if (ApprovalRequestInfoDTO.aprStatus == enuAPRStatus::Expired)
        throw exTargomanBase("Code expired");

    quint32 ConfigTTL = (Type == enuApprovalType::Email
                            ? ApprovalRequest::EmailResendApprovalCodeTTL.value()
                            : ApprovalRequest::MobileResendApprovalCodeTTL.value()
                        );

    QDateTime Now = Info.value(Targoman::API::CURRENT_TIMESTAMP).toDateTime();
    quint64 Secs = Now.toSecsSinceEpoch() - ApprovalRequestInfoDTO.aprSentDate.toSecsSinceEpoch();

    qDebug() << Now.toString() << ApprovalRequestInfoDTO.aprSentDate.toString() << Secs << ConfigTTL;

    if (Secs >= ConfigTTL)
        throw exTargomanBase("The remaining time is up");

    return QVariantMap({
                           { "key", _emailOrMobile },
                           { "config-ttl", ConfigTTL },
                           { "elapsed", Secs },
                           { "remain-ttl", ConfigTTL - Secs },
    });
}

} //namespace Targoman::API::AccountModule::ORM
