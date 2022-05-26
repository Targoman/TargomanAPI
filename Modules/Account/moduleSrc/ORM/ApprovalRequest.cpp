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
#include "Interfaces/Helpers/PhoneHelper.h"
using namespace Targoman::API::Helpers;

TAPI_REGISTER_TARGOMAN_ENUM(Targoman::API::AccountModule, enuAPRStatus);
TAPI_REGISTER_TARGOMAN_ENUM(Targoman::API::AccountModule, enuApprovalType);

using namespace Targoman::Common::Configuration;

namespace Targoman::API::AccountModule::ORM {

tmplConfigurable<quint32> ApprovalRequest::EmailApprovalCodeTTL(
    AAA::makeConfig("EmailApprovalCodeTTL"),
    "Time to live for the email approval code",
    static_cast<quint16>(2*24*60*60), //2 days
    ReturnTrueCrossValidator(),
    "",
    "",
    "email-approval-code-ttl",
    enuConfigSource::Arg | enuConfigSource::File
);

tmplConfigurable<quint32> ApprovalRequest::MobileApprovalCodeTTL(
    AAA::makeConfig("MobileApprovalCodeTTL"),
    "Time to live for the mobile approval code",
    static_cast<quint16>(2*60), //2 minutes
    ReturnTrueCrossValidator(),
    "",
    "",
    "mobile-approval-code-ttl",
    enuConfigSource::Arg | enuConfigSource::File
);

ApprovalRequest::ApprovalRequest() :
    intfSQLBasedModule(
        AAASchema,
        tblApprovalRequest::Name,
        {///< ColName                               Type                        Validation                          Default     UpBy   Sort  Filter Self  Virt   PK isSelectable renameAs
            { tblApprovalRequest::aprID,            ORM_PRIMARYKEY_64 },
            { tblApprovalRequest::apr_usrID,        S(NULLABLE_TYPE(quint64)),  QFV.integer().minValue(1),          QInvalid,   UPNone },
            { tblApprovalRequest::aprRequestedFor,  S(Targoman::API::AccountModule::enuApprovalType::Type), QFV,    Targoman::API::AccountModule::enuApprovalType::Email, UPNone },
//            { tblApprovalRequest::aprIsForLogin,   S(bool),                    QFV,                                false,      UPNone },
            { tblApprovalRequest::aprApprovalKey,   S(QString),                 QFV.allwaysInvalid(),               QRequired,  UPNone, false, false },
            { tblApprovalRequest::aprApprovalCode,  S(QString),                 QFV.asciiAlNum().maxLenght(32),     QRequired,  UPNone, false, false, false, false, false, false },
            { tblApprovalRequest::aprRequestDate,   ORM_CREATED_ON },
            { tblApprovalRequest::aprSentDate,      S(NULLABLE_TYPE(TAPI::DateTime_t)),  QFV,                       QNull,      UPAdmin },
            { tblApprovalRequest::aprApplyDate,     S(NULLABLE_TYPE(TAPI::DateTime_t)),  QFV,                       QNull,      UPNone },
            { tblApprovalRequest::aprStatus,        ORM_STATUS_FIELD(Targoman::API::AccountModule::enuAPRStatus, Targoman::API::AccountModule::enuAPRStatus::New) },
            { ORM_INVALIDATED_AT_FIELD },
        },
        {///< Col                               Reference Table                 ForeignCol        Rename     LeftJoin
            { tblApprovalRequest::apr_usrID,    R(AAASchema, tblUser::Name),    tblUser::usrID },
        },
        {
            { {
                tblApprovalRequest::aprApprovalKey,
                tblApprovalRequest::aprApprovalCode,
                ORM_INVALIDATED_AT_FIELD_NAME,
              }, enuDBIndex::Unique },
        }
    ) { ; }

QVariant IMPL_ORMGET(ApprovalRequest) {
    Authorization::checkPriv(_APICALLBOOM.getJWT(), this->privOn(EHTTP_GET, this->moduleBaseName()));

    return this->Select(GET_METHOD_ARGS_CALL_INTERNAL_BOOM);

//    return query.one();

    //    return this->selectFromTable({}, {}, GET_METHOD_CALL_ARGS_APICALL);
}

bool IMPL_ORMDELETE(ApprovalRequest) {
    Authorization::checkPriv(_APICALLBOOM.getJWT(), this->privOn(EHTTP_DELETE, this->moduleBaseName()));

    return this->DeleteByPks(DELETE_METHOD_ARGS_CALL_INTERNAL_BOOM, {}, true);
//    return this->deleteByPKs(DELETE_METHOD_CALL_ARGS_APICALL, {}, true);
}

QVariant IMPL_REST_GET_OR_POST(ApprovalRequest, timerInfo, (
    APICALLBOOM_TYPE_NO_JWT_IMPL &APICALLBOOM_PARAM,
    QString _emailOrMobile
)) {
    Authorization::validateIPAddress(_APICALLBOOM.getIP());

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

    QVariantMap Info = SelectQuery(*this)
                         .addCols({
//                                      tblApprovalRequest::aprID,
//                                      tblApprovalRequest::apr_usrID,
//                                      tblApprovalRequest::aprRequestedFor,
                                      tblApprovalRequest::aprApprovalKey,
                                      tblApprovalRequest::aprRequestDate,
                                      tblApprovalRequest::aprSentDate,
                                      tblApprovalRequest::aprApplyDate,
                                      Targoman::API::CURRENT_TIMESTAMP,
                                  })
                         .where({ tblApprovalRequest::aprApprovalKey, enuConditionOperator::Equal, _emailOrMobile })
                         .andWhere({ tblApprovalRequest::aprRequestedFor, enuConditionOperator::Equal, Type })
                         .andWhere({ tblApprovalRequest::aprStatus, enuConditionOperator::In, "'N', 'S', 'A', '1', '2', 'E'" })
                         .orderBy(tblApprovalRequest::aprRequestDate, enuOrderDir::Descending)
                         .one();

    if (Info.isEmpty())
        throw exTargomanBase("Approval Request Not found");

//    quint64 aprID;
//    quint64 apr_usrID;
//    TAPI::DateTime_t aprRequestDate;
    NULLABLE_TYPE(TAPI::DateTime_t) aprSentDate;
    NULLABLE_TYPE(TAPI::DateTime_t) aprApplyDate;
    Targoman::API::AccountModule::enuAPRStatus::Type aprStatus;

//    SET_FIELD_FROM_VARIANT_MAP(aprID,           Info, tblApprovalRequest, aprID);
//    SET_FIELD_FROM_VARIANT_MAP(apr_usrID,       Info, tblApprovalRequest, apr_usrID);
//    SET_FIELD_FROM_VARIANT_MAP(aprRequestDate,  Info, tblApprovalRequest, aprRequestDate);
//    SET_FIELD_FROM_VARIANT_MAP(aprSentDate,     Info, tblApprovalRequest, aprSentDate);
    aprSentDate = Info.value(tblApprovalRequest::aprSentDate).toDateTime();
    SET_FIELD_FROM_VARIANT_MAP(aprApplyDate,    Info, tblApprovalRequest, aprApplyDate);
    SET_FIELD_FROM_VARIANT_MAP(aprStatus,       Info, tblApprovalRequest, aprStatus);

    if ((aprStatus == enuAPRStatus::New) || NULLABLE_IS_NULL(aprSentDate))
        throw exTargomanBase("Code not sent to the client");

    if ((aprStatus == enuAPRStatus::Applied) || NULLABLE_HAS_VALUE(aprApplyDate))
        throw exTargomanBase("Already applied before");

    if (aprStatus == enuAPRStatus::Expired)
        throw exTargomanBase("Code expired");

    quint32 ConfigTTL = (Type == enuApprovalType::Email
                            ? ApprovalRequest::EmailApprovalCodeTTL.value()
                            : ApprovalRequest::MobileApprovalCodeTTL.value()
                        );

    QDateTime Now = Info.value(Targoman::API::CURRENT_TIMESTAMP).toDateTime();
    quint64 Secs = Now.toSecsSinceEpoch() - aprSentDate->toSecsSinceEpoch();

    qDebug() << Now.toString() << aprSentDate->toString() << Secs << ConfigTTL;

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
