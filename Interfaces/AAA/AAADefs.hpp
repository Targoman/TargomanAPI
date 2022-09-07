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
 * @author S.Mehran M.Ziabary <ziabary@targoman.com>
 * @author Kambiz Zandi <kambizzandi@gmail.com>
 */

#ifndef TARGOMAN_API_AAA_DEFS_HPP
#define TARGOMAN_API_AAA_DEFS_HPP

#include <QJsonObject>

#include "libTargomanCommon/exTargomanBase.h"
#include "libTargomanDBM/clsDAC.h"

#include "Interfaces/Common/APIArgHelperMacros.hpp"
#include "Interfaces/Common/QtTypes.hpp"
#include "Interfaces/Common/GenericTypes.h"
#include "Interfaces/Common/GenericEnums.hpp"
#include "Interfaces/Common/HTTPExceptions.hpp"
#include "Interfaces/DBM/Defs.hpp"
#include "Interfaces/DBM/clsORMField.h"

using namespace qhttp;

using namespace Targoman::API::DBM;

namespace Targoman::API::AAA {

constexpr char AccountDomain[] = "Account";
constexpr char AAASchema[] = "AAA";

constexpr quint64 SYSTEM_USER_ID = 1;
#define makeAAADAC(_varName) DBManager::clsDAC _varName(AccountDomain, AAASchema)

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wweak-vtables"
TARGOMAN_ADD_EXCEPTION_HANDLER_WITH_CODE(ESTATUS_BAD_REQUEST, exAAA);
TARGOMAN_ADD_EXCEPTION_HANDLER_WITH_CODE(ESTATUS_UNAUTHORIZED, exAuthentication);
TARGOMAN_ADD_EXCEPTION_HANDLER_WITH_CODE(ESTATUS_FORBIDDEN, exAuthorization);
TARGOMAN_ADD_EXCEPTION_HANDLER_WITH_CODE(ESTATUS_METHOD_NOT_ALLOWED, exAccounting);
TARGOMAN_ADD_EXCEPTION_HANDLER_WITH_CODE(499, exNoTokenProvided);
TARGOMAN_ADD_EXCEPTION_HANDLER_WITH_CODE(498, exInvalidToken);
TARGOMAN_ADD_EXCEPTION_HANDLER_WITH_CODE(ESTATUS_PAYMENT_REQUIRED, exPaymentRequired);
#pragma clang diagnostic pop

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-variable"

namespace TOKENItems {
TARGOMAN_CREATE_CONSTEXPR(tokID);
TARGOMAN_CREATE_CONSTEXPR(usrID);
TARGOMAN_CREATE_CONSTEXPR(privs);
}

namespace PRIVItems {
TARGOMAN_CREATE_CONSTEXPR(CRUD);
}

namespace AAACommonItems {
TARGOMAN_CREATE_CONSTEXPR(privs);
TARGOMAN_CREATE_CONSTEXPR(acc);
TARGOMAN_CREATE_CONSTEXPR(usrID);
}

namespace tblUser {
    constexpr char Name[] = "tblUser";

    constexpr quint64 MinNormalUserID = 100;

    namespace Fields {
        TARGOMAN_CREATE_CONSTEXPR(usrID);
        TARGOMAN_CREATE_CONSTEXPR(usrGender);
        TARGOMAN_CREATE_CONSTEXPR(usrName);
        TARGOMAN_CREATE_CONSTEXPR(usrFamily);
        TARGOMAN_CREATE_CONSTEXPR(usrEmail);
        TARGOMAN_CREATE_CONSTEXPR(usrMobile);
        TARGOMAN_CREATE_CONSTEXPR(usrApprovalState);
        TARGOMAN_CREATE_CONSTEXPR(usrPass);
        TARGOMAN_CREATE_CONSTEXPR(usr_rolID);
        TARGOMAN_CREATE_CONSTEXPR(usrSpecialPrivs);
        TARGOMAN_CREATE_CONSTEXPR(usrSSID);
        TARGOMAN_CREATE_CONSTEXPR(usrAddress);
        TARGOMAN_CREATE_CONSTEXPR(usrLanguage);
        TARGOMAN_CREATE_CONSTEXPR(usrEnableEmailAlerts);
        TARGOMAN_CREATE_CONSTEXPR(usrEnableSMSAlerts);
        TARGOMAN_CREATE_CONSTEXPR(usrMaxSessions);
        TARGOMAN_CREATE_CONSTEXPR(usrActiveSessions);
        TARGOMAN_CREATE_CONSTEXPR(usrLastLogin);
        TARGOMAN_CREATE_CONSTEXPR(usrCreatedBy_usrID);
        TARGOMAN_CREATE_CONSTEXPR(usrCreationDateTime);
        TARGOMAN_CREATE_CONSTEXPR(usrUpdatedBy_usrID);
        TARGOMAN_CREATE_CONSTEXPR(usrStatus);
    }

    namespace Relation {
        constexpr char ExtraInfo[] = "ExtraInfo";
    }

    namespace Private {
        const QList<clsORMField> ORMFields = {
            //ColName                       Type                            Validation                              Default     UpBy    Sort  Filter Self  Virt  PK
            //ORM_PRIMARYKEY_64 with self:true
            { Fields::usrID,                S(quint64),                     QFV.integer().minValue(1),              QAuto,      UPNone, true, true,  true, false, true },
            { Fields::usrEmail,             S(TAPI::Email_t),               QFV.emailNotFake(),                     QNull,      UPOwner },
            { Fields::usrName,              S(QString),                     QFV.unicodeAlNum().maxLenght(128),      QNull,      UPOwner },
            { Fields::usrFamily,            S(QString),                     QFV.unicodeAlNum().maxLenght(128),      QNull,      UPOwner },
            { Fields::usrGender,            S(TAPI::enuGender::Type),       QFV,                                    TAPI::enuGender::NotExpressed, UPOwner },
            { Fields::usrMobile,            S(TAPI::Mobile_t),              QFV,                                    QNull,      UPOwner },
            { Fields::usrApprovalState,     S(TAPI::enuUserApproval::Type), QFV,                                    TAPI::enuUserApproval::None },
            //usrPass : not define here
            { Fields::usr_rolID,            S(quint32),                     QFV.integer().minValue(1),              QRequired,  UPAdmin },
            { Fields::usrSpecialPrivs,      S(TAPI::PrivObject_t),          QFV,                                    QNull,      UPAdmin, false, false },
            { Fields::usrSSID,              S(QString),                     QFV.unicodeAlNum().maxLenght(10),       QNull,      UPOwner },
            { Fields::usrAddress,           S(QString),                     QFV.unicodeAlNum().maxLenght(1024),     QNull,      UPOwner },
            { Fields::usrLanguage,          S(QString),                     QFV.languageCode(),                     "fa",       UPOwner },
            { Fields::usrEnableEmailAlerts, S(bool),                        QFV,                                    true,       UPOwner },
            { Fields::usrEnableSMSAlerts,   S(bool),                        QFV,                                    true,       UPOwner },
            { Fields::usrMaxSessions,       S(qint32),                      QFV.integer().betweenValues(-1, 100),   -1,         UPAdmin },
            { Fields::usrActiveSessions,    S(qint32),                      QFV.integer().betweenValues(-1, 1000),  QInvalid,   UPNone },
            { Fields::usrLastLogin,         S(TAPI::DateTime_t),            QFV,                                    QInvalid,   UPNone },
            { Fields::usrStatus,            ORM_STATUS_FIELD(TAPI::enuUserStatus, TAPI::enuUserStatus::MustValidate) },
            { ORM_INVALIDATED_AT_FIELD },
            { Fields::usrCreationDateTime,  ORM_CREATED_ON },
            { Fields::usrCreatedBy_usrID,   ORM_CREATED_BY_NULLABLE },
            { Fields::usrUpdatedBy_usrID,   ORM_UPDATED_BY },
        };

///Col                               Reference Table                                     ForeignCol                      Rename  LeftJoin
#define tblUser_Private_Relations \
    { \
        { tblUser::Fields::usr_rolID,               R(AAASchema, tblRoles::Name),                       tblRoles::Fields::rolID }, \
        { tblUser::Relation::ExtraInfo, { tblUser::Fields::usrID, R(AAASchema, tblUserExtraInfo::Name), tblUserExtraInfo::Fields::uei_usrID,    "",     true } }, \
        ORM_RELATION_OF_CREATOR(tblUser::Fields::usrCreatedBy_usrID), \
        ORM_RELATION_OF_UPDATER(tblUser::Fields::usrUpdatedBy_usrID), \
    }

        const QList<stuDBIndex> Indexes = {
            { {
                  Fields::usrEmail,
                  ORM_INVALIDATED_AT_FIELD_NAME,
              }, enuDBIndex::Unique },
            { {
                  Fields::usrMobile,
                  ORM_INVALIDATED_AT_FIELD_NAME,
              }, enuDBIndex::Unique },
        };

    } //namespace Private

    TAPI_DEFINE_STRUCT(DTO,
        SF_ORM_PRIMARYKEY_64        (usrID),
        SF_QString                  (usrEmail),
        SF_QString                  (usrName),
        SF_QString                  (usrFamily),
        SF_Enum                     (usrGender, TAPI::enuGender, TAPI::enuGender::NotExpressed),
        SF_QString                  (usrMobile),
        SF_Enum                     (usrApprovalState, TAPI::enuUserApproval, TAPI::enuUserApproval::None),
        SF_quint32                  (usr_rolID),
        SF_JSON_t                   (usrSpecialPrivs),
        SF_QString                  (usrSSID),
        SF_QString                  (usrAddress),
        SF_QString                  (usrLanguage),
        SF_bool                     (usrEnableEmailAlerts),
        SF_bool                     (usrEnableSMSAlerts),
        SF_qint32                   (usrMaxSessions),
        SF_qint32                   (usrActiveSessions),
        SF_DateTime_t               (usrLastLogin),
        SF_ORM_STATUS_FIELD         (usrStatus, TAPI::enuUserStatus, TAPI::enuUserStatus::MustValidate),
        SF_ORM_CREATED_ON           (usrCreationDateTime),
        SF_ORM_CREATED_BY_NULLABLE  (usrCreatedBy_usrID),
        SF_NULLABLE_quint64         (usrUpdatedBy_usrID)
    );

}
#pragma GCC diagnostic pop

} //namespace Targoman::API::AAA

#endif // TARGOMAN_API_AAA_DEFS_HPP
