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

#ifndef TARGOMAN_API_MODULES_ACCOUNT_ORM_APPROVALREQUEST_H
#define TARGOMAN_API_MODULES_ACCOUNT_ORM_APPROVALREQUEST_H

#include "Interfaces/AAA/AAA.hpp"
#include "Interfaces/API/intfSQLBasedModule.h"
#include "libTargomanCommon/Configuration/tmplConfigurable.h"

namespace Targoman::API::AccountModule {

//structures and enumes goes here

TARGOMAN_DEFINE_ENUM(enuAPRStatus,
                     New      = 'N',
                     Sent     = 'S',
                     Applied  = 'A',
                     FirstTry = '1',
                     SecondTry= '2',
                     Expired  = 'E',
                     Removed  = 'R',
                     )

TARGOMAN_DEFINE_ENUM(enuApprovalType,
                     Email      = 'E',
                     Mobile     = 'M',
                     )

} //namespace Targoman::API::AccountModule

TAPI_DECLARE_METATYPE_ENUM(Targoman::API::AccountModule, enuAPRStatus);
TAPI_DECLARE_METATYPE_ENUM(Targoman::API::AccountModule, enuApprovalType);

namespace Targoman::API::AccountModule {
namespace ORM {

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-variable"
namespace tblApprovalRequest {
    constexpr char Name[] = "tblApprovalRequest";

    namespace Fields {
        TARGOMAN_CREATE_CONSTEXPR(aprID);
        TARGOMAN_CREATE_CONSTEXPR(apr_usrID);
        TARGOMAN_CREATE_CONSTEXPR(aprRequestedFor);
        TARGOMAN_CREATE_CONSTEXPR(aprApprovalKey);
        TARGOMAN_CREATE_CONSTEXPR(aprApprovalCode);
        TARGOMAN_CREATE_CONSTEXPR(aprSentDate);
        TARGOMAN_CREATE_CONSTEXPR(aprApplyDate);
        TARGOMAN_CREATE_CONSTEXPR(aprStatus);
        TARGOMAN_CREATE_CONSTEXPR(aprRequestDate);
    }

    inline QStringList ColumnNames(QString _tableAlias = "") {
        if (_tableAlias.isEmpty() == false)
            _tableAlias += ".";

        return {
            _tableAlias + Fields::aprID,
            _tableAlias + Fields::apr_usrID,
            _tableAlias + Fields::aprRequestedFor,
            _tableAlias + Fields::aprApprovalKey,
            _tableAlias + Fields::aprApprovalCode,
            _tableAlias + Fields::aprSentDate,
            _tableAlias + Fields::aprApplyDate,
            _tableAlias + Fields::aprStatus,
            _tableAlias + Fields::aprRequestDate,
        };
    }

    namespace Relation {
        // constexpr char AAA[] = "aaa";
    }

    namespace Private {
        const QList<clsORMField> ORMFields = {
            //ColName                   Type                        Validation                          Default     UpBy    Sort    Filter  Self    Virt    PK      isSelectable renameAs
            { Fields::aprID,            ORM_PRIMARYKEY_64 },
            { Fields::apr_usrID,        S(NULLABLE_TYPE(quint64)),  QFV.integer().minValue(1),          QInvalid,   UPNone },
            { Fields::aprRequestedFor,  S(Targoman::API::AccountModule::enuApprovalType::Type), QFV,    Targoman::API::AccountModule::enuApprovalType::Email, UPNone },
            { Fields::aprApprovalKey,   S(QString),                 QFV.allwaysInvalid(),               QRequired,  UPNone, false, false },
            { Fields::aprApprovalCode,  S(QString),                 QFV.asciiAlNum().maxLenght(32),     QRequired,  UPNone, false, false,   false,  false,  false,  false },
            { Fields::aprSentDate,      S(NULLABLE_TYPE(TAPI::DateTime_t)),  QFV,                       QNull,      UPAdmin },
            { Fields::aprApplyDate,     S(NULLABLE_TYPE(TAPI::DateTime_t)),  QFV,                       QNull,      UPNone },
            { Fields::aprStatus,        ORM_STATUS_FIELD(Targoman::API::AccountModule::enuAPRStatus, Targoman::API::AccountModule::enuAPRStatus::New) },
            { ORM_INVALIDATED_AT_FIELD },
            { Fields::aprRequestDate,   ORM_CREATED_ON },
        };

        const QList<stuRelation> Relations = {
            ///< Col                               Reference Table                 ForeignCol        Rename     LeftJoin
            { Fields::apr_usrID,    R(AAASchema, tblUser::Name),    tblUser::Fields::usrID },
        };

        const QList<stuDBIndex> Indexes = {
            { {
                Fields::aprApprovalKey,
                Fields::aprApprovalCode,
                ORM_INVALIDATED_AT_FIELD_NAME,
              }, enuDBIndex::Unique },
        };

    } //namespace Private

    TAPI_DEFINE_VARIANT_ENABLED_STRUCT(DTO,
        SF_ORM_PRIMARYKEY_64        (aprID),
        SF_NULLABLE_quint64         (apr_usrID),
        SF_Enum                     (aprRequestedFor, Targoman::API::AccountModule::enuApprovalType, Targoman::API::AccountModule::enuApprovalType::Email),
        SF_QString                  (aprApprovalKey),
        SF_QString                  (aprApprovalCode),
        SF_DateTime_t               (aprSentDate),
        SF_DateTime_t               (aprApplyDate),
        SF_ORM_STATUS_FIELD         (aprStatus, Targoman::API::AccountModule::enuAPRStatus, Targoman::API::AccountModule::enuAPRStatus::New),
        SF_ORM_CREATED_ON           (aprRequestDate)
    );
}
#pragma GCC diagnostic pop

class ApprovalRequest : public intfSQLBasedModule
{
    Q_OBJECT
    TARGOMAN_DEFINE_API_SUBMODULE(Account, ApprovalRequest)

public:
    static Targoman::Common::Configuration::tmplConfigurable<quint32> EmailApprovalCodeTTL;
    static Targoman::Common::Configuration::tmplConfigurable<quint32> MobileApprovalCodeTTL;

private slots:
    QVariant ORMGET("Get ApprovalRequest information.")
    bool ORMDELETE("Delete an ApprovalRequest")

    QVariant REST_GET_OR_POST(
        timerInfo,
        (
            APICALLBOOM_TYPE_NO_JWT_DECL &APICALLBOOM_PARAM,
            QString _emailOrMobile
        ),
        "Returns TTL remained for Approval Request based on email or mobile"
    )
};

} //namespace ORM
} //namespace Targoman::API::AccountModule

#endif // TARGOMAN_API_MODULES_ACCOUNT_ORM_APPROVALREQUEST_H
