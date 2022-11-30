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

#include "User.h"
#include "../Account.h"
#include "Roles.h"
//#include "Interfaces/ORM/APIQueryBuilders.h"
#include "Interfaces/Helpers/PhoneHelper.h"
#include "libTargomanDBM/clsDAC.h"
#include "ApprovalRequest.h"

TAPI_REGISTER_TARGOMAN_ENUM(Targoman::API::AccountModule, enuUserExtraInfoJsonKey);

namespace Targoman::API::AccountModule::ORM {

using namespace Helpers;
using namespace DBManager;

/*****************************************************************\
|* User **********************************************************|
\*****************************************************************/
///@TODO: BAD Gender causes assert

TARGOMAN_API_SUBMODULE_IMPLEMENT(Account, User)

User::User() :
    intfSQLBasedModule(
        AAASchema,
        tblUser::Name,
        tblUser::Private::ORMFields,
        tblUser_Private_Relations,
        tblUser::Private::Indexes
    )
{
    this
        ->addDBProperty(
            ORM_TABLE_DBPROPERTY_INVALIDATE_QUERY,
            "UPDATE :tableName"
            "   SET :invalidatedAt = UNIX_TIMESTAMP()"
            " WHERE :invalidatedAt = 0"
            "   AND usrStatus = 'R'"
            "   AND ("
            "       (IFNULL(:usrEmail, '') <> '' AND IFNULL(usrEmail, '') = :usrEmail)"
            "    OR (IFNULL(:usrMobile, '') <> '' AND IFNULL(usrMobile, '') = :usrMobile)"
            "       )")
    ;
}

QVariant IMPL_ORMGET_USER(User) {
    if (_apiCallContext.getActorID() != _pksByPath.toULongLong())
        Authorization::checkPriv(_apiCallContext, this->privOn(EHTTP_GET, this->moduleBaseName()));
//        Authorization::checkPriv(_apiCallContext, { "Account:User:CRUD~0100" });

    if (_cols.isEmpty())
        _cols = QStringList({
            tblUser::Fields::usrEmail,
            tblUser::Fields::usrName,
            tblUser::Fields::usrFamily,
            tblUser::Fields::usrGender,
            tblUser::Fields::usrMobile,
            tblUser::Fields::usrApprovalState,
            tblUser::Fields::usr_rolID,
//            tblUser::Fields::usrSpecialPrivs,
            tblUser::Fields::usrSSID,
            tblUser::Fields::usrAddress,
            tblUser::Fields::usrLanguage,
            tblUser::Fields::usrEnableEmailAlerts,
            tblUser::Fields::usrEnableSMSAlerts,
//            tblUser::Fields::usrMaxSessions,
            tblUser::Fields::usrActiveSessions,
            tblUser::Fields::usrLastLogin,
            tblUser::Fields::usrStatus,
//            tblUser::Fields::usrCreationDateTime,
//            tblUser::Fields::usrCreatedBy_usrID,
//            tblUser::Fields::usrUpdatedBy_usrID,
            Targoman::API::CURRENT_TIMESTAMP,
            //-------------------
//            tblUserExtraInfo::Fields::uei_usrID,
            //tblUserExtraInfo::Fields::ueiGender,
            tblUserExtraInfo::Fields::ueiBirthDate,
//            tblUserExtraInfo::Fields::ueiPhoto,
//            tblUserExtraInfo::Fields::ueiOAuthAccounts,
            tblUserExtraInfo::Fields::ueiIBAN,
            tblUserExtraInfo::Fields::ueiEther,
            tblUserExtraInfo::Fields::ueiExtraInfo,
//            tblUserExtraInfo::Fields::ueiUpdatedBy_usrID,
        }).join(",");

    return this->Select(GET_METHOD_ARGS_CALL_VALUES,
                        {},
                        0,
                        [](ORMSelectQuery &_query) {
                            _query.leftJoinWith(tblUser::Relation::ExtraInfo);
                        });

//    return query.one();

    //    return this->selectFromTable({},{}, GET_METHOD_CALL_ARGS_APICALL);
}

quint64 IMPL_ORMCREATE_USER(User) {
    Authorization::checkPriv(_apiCallContext, this->privOn(EHTTP_PUT, this->moduleBaseName()));

    if (_createInfo.value(tblUser::Fields::usrEmail).toString().isEmpty()
            && _createInfo.value(tblUser::Fields::usrMobile).toString().isEmpty())
        throw exHTTPBadRequest("Either email or mobile must be provided to create user");

    return this->Create(CREATE_METHOD_ARGS_CALL_VALUES);
}

/*
 * this method only can call by admin user
 */
bool IMPL_ORMUPDATE_USER(User) {
    Authorization::checkPriv(_apiCallContext, this->privOn(EHTTP_PATCH, this->moduleBaseName()));

    return this->Update(UPDATE_METHOD_ARGS_CALL_VALUES);
}

//bool IMPL_ORMDELETE_USER(User) {
//    if (_apiCallContext.getActorID() != _pksByPath.toULongLong())
//        Authorization::checkPriv(_apiCallContext, this->privOn(EHTTP_DELETE, this->moduleBaseName()));

//    return this->DeleteByPks(DELETE_METHOD_ARGS_CALL_VALUES);
//}

bool IMPL_REST_DELETE(User, , (
    USER_DELETE_METHOD_ARGS_IMPL_APICALL,
    TAPI::MD5_t         _pass,
    QString             _salt
)) {
    if (_apiCallContext.getActorID() == _pksByPath.toULongLong()) {
        //check password if is set
        this->callSP(_apiCallContext,
                     "spUser_CheckPassword", {
                         { "iUserID", _apiCallContext.getActorID() },
                         { "iPass", _pass },
                         { "iSalt", _salt },
                         { "iThrowIfPassNotSet", 0 },
                     });
    } else
        Authorization::checkPriv(_apiCallContext, this->privOn(EHTTP_DELETE, this->moduleBaseName()));

    return this->DeleteByPks(DELETE_METHOD_ARGS_CALL_VALUES);
}

ORMSelectQuery User::getPhotoQuery(
    INTFAPICALLCONTEXT_IMPL &_apiCallContext,
    quint64 _usrID
) {

    UserExtraInfo::instance().prepareFiltersList();

    return UserExtraInfo::instance().makeSelectQuery(_apiCallContext)
        .addCol(tblUserExtraInfo::Fields::ueiPhoto)
        .where({ tblUserExtraInfo::Fields::uei_usrID, enuConditionOperator::Equal, _usrID })
    ;
}

TAPI::Base64Image_t IMPL_REST_GET(User, photo, (
    APICALLCONTEXT_TYPE_JWT_USER_IMPL &_apiCallContext,
    quint64 _usrID
)) {
    quint64 CurrentUserID = _apiCallContext.getActorID();

    if (CurrentUserID != _usrID)
        Authorization::checkPriv(_apiCallContext, { "Account:User:photo:CRUD~0100" });

    auto Photo = getPhotoQuery(_apiCallContext, _usrID)
            .setCacheTime(30)
            .one()
            .value(tblUserExtraInfo::Fields::ueiPhoto).toString().toLatin1();
            ;

    return TAPI::Base64Image_t(Photo);
}

bool IMPL_REST_UPDATE(User, photo, (
    APICALLCONTEXT_TYPE_JWT_USER_IMPL &_apiCallContext,
    TAPI::Base64Image_t _image
)) {
    quint64 CurrentUserID = _apiCallContext.getActorID();

    QString QueryString = QString()
          + "INSERT INTO"
          + " " + tblUserExtraInfo::Name
          + " SET ueiPhoto=?"
          + "   , uei_usrID=?"
          + " ON DUPLICATE KEY UPDATE"
          + "     ueiPhoto=?"
          + "   , ueiUpdatedBy_usrID=?"
          ;

    clsDACResult Result = UserExtraInfo::instance().execQuery(_apiCallContext,
                                                              QueryString, {
                                                                  _image,
                                                                  CurrentUserID,
                                                                  _image,
                                                                  CurrentUserID,
                                                              });

    bool OK = Result.numRowsAffected() > 0;

    if (OK)
        getPhotoQuery(_apiCallContext, CurrentUserID).clearCache();

    return OK;
}

bool IMPL_REST_POST(User, deletePhoto, (
    APICALLCONTEXT_TYPE_JWT_USER_IMPL &_apiCallContext
)) {
    quint64 CurrentUserID = _apiCallContext.getActorID();

    QString QueryString = QString()
          + "UPDATE"
          + " " + tblUserExtraInfo::Name
          + " SET ueiPhoto=NULL"
          + "   , ueiUpdatedBy_usrID=?"
          + " WHERE uei_usrID=?"
          ;

    clsDACResult Result = UserExtraInfo::instance().execQuery(_apiCallContext,
                                                              QueryString, {
                                                                  CurrentUserID,
                                                                  CurrentUserID,
                                                              });

    bool OK = Result.numRowsAffected() > 0;

    if (OK)
        getPhotoQuery(_apiCallContext, CurrentUserID).clearCache();

    return OK;
}

bool IMPL_REST_UPDATE(User, email, (
    APICALLCONTEXT_TYPE_JWT_USER_IMPL &_apiCallContext,
    TAPI::Email_t   _email,
    TAPI::MD5_t     _pass,
    QString         _salt
)) {
    quint64 CurrentUserID = _apiCallContext.getActorID();

    _email = _email.toLower().trimmed();

    if (_email.isEmpty())
        throw exHTTPBadRequest("Email is empty");

    if ((QFV.email().isValid(_email) == false) || (QFV.emailNotFake().isValid(_email) == false))
        throw exHTTPBadRequest("Email domain is suspicious. Please use a real email.");

//    clsJWT cJWT(_apiCallContext.getJWT());
//    if (cJWT.canChangePass()) { //hasPass
//        if (_pass.isEmpty() || _salt.isEmpty())
//            throw exHTTPBadRequest("Password and salt are required to change email");

//        QFV.asciiAlNum().maxLenght(20).validate(_salt, "salt");
//    }

    this->callSP(_apiCallContext,
                 "spApproval_Request", {
                     { "iBy", "E" },
                     { "iKey", _email },
                     { "iUserID", CurrentUserID },
                     { "iPass", _pass },
                     { "iSalt", _salt },
                     { "iThrowIfPassNotSet", 1 },
                     { "iResendApprovalTTLSecs", ApprovalRequest::EmailResendApprovalCodeTTL.value() },
                     { "iExpireApprovalTTLSecs", ApprovalRequest::EmailExpireApprovalCodeTTL.value() },
                     { "iUserLanguage", _apiCallContext.language() },
                 });

    return true;
}

bool IMPL_REST_UPDATE(User, mobile, (
    APICALLCONTEXT_TYPE_JWT_USER_IMPL &_apiCallContext,
    TAPI::Mobile_t  _mobile,
    TAPI::MD5_t     _pass,
    QString         _salt
)) {
    quint64 CurrentUserID = _apiCallContext.getActorID();

    _mobile = PhoneHelper::NormalizePhoneNumber(_mobile);

    if (_mobile.isEmpty())
        throw exHTTPBadRequest("Mobile is empty");

    if (QFV.mobile().isValid(_mobile) == false)
        throw exHTTPBadRequest("Invalid mobile.");

//    clsJWT cJWT(_apiCallContext.getJWT());
//    if (cJWT.canChangePass()) { //hasPass
//        if (_pass.isEmpty() || _salt.isEmpty())
//            throw exHTTPBadRequest("Password and salt are required to change email");

//        QFV.asciiAlNum().maxLenght(20).validate(_salt, "salt");
//    }

    this->callSP(_apiCallContext,
                 "spApproval_Request", {
                     { "iBy", "M" },
                     { "iKey", _mobile },
                     { "iUserID", CurrentUserID },
                     { "iPass", _pass },
                     { "iSalt", _salt },
                     { "iThrowIfPassNotSet", 1 },
                     { "iResendApprovalTTLSecs", ApprovalRequest::MobileResendApprovalCodeTTL.value() },
                     { "iExpireApprovalTTLSecs", ApprovalRequest::MobileExpireApprovalCodeTTL.value() },
                     { "iUserLanguage", _apiCallContext.language() },
                 });

    return true;
}

bool IMPL_REST_UPDATE(User, personalInfo, (
    APICALLCONTEXT_TYPE_JWT_USER_IMPL &_apiCallContext,
    QString             _name,
    QString             _family,
    TAPI::ISO639_2_t    _language,
    NULLABLE_TYPE(TAPI::enuGender::Type) _gender,
    NULLABLE_TYPE(bool) _enableEmailAlerts,
    NULLABLE_TYPE(bool) _enableSMSAlerts,
    QString             _ssid,
    QString             _address
)) {
    quint64 CurrentUserID = _apiCallContext.getActorID();

    QVariantMap ToUpdate;

    if (_name.isNull() == false)                    ToUpdate.insert(tblUser::Fields::usrName, _name);
    if (_family.isNull() == false)                  ToUpdate.insert(tblUser::Fields::usrFamily, _family);
    if (_language.isNull() == false)                ToUpdate.insert(tblUser::Fields::usrLanguage, _language);
    if (NULLABLE_HAS_VALUE(_gender))                ToUpdate.insert(tblUser::Fields::usrGender, NULLABLE_VALUE(_gender));
    if (NULLABLE_HAS_VALUE(_enableEmailAlerts))     ToUpdate.insert(tblUser::Fields::usrEnableEmailAlerts, NULLABLE_VALUE(_enableEmailAlerts) ? 1 : 0);
    if (NULLABLE_HAS_VALUE(_enableSMSAlerts))       ToUpdate.insert(tblUser::Fields::usrEnableSMSAlerts, NULLABLE_VALUE(_enableSMSAlerts) ? 1 : 0);
    if (_ssid.isNull() == false)                    ToUpdate.insert(tblUser::Fields::usrSSID, _ssid);
    if (_address.isNull() == false)                 ToUpdate.insert(tblUser::Fields::usrAddress, _address);

    if (ToUpdate.size())
        this->Update(_apiCallContext,
                     QString::number(CurrentUserID),
                     ToUpdate
                     );

    return true;
}

bool IMPL_REST_UPDATE(User, financialInfo, (
    APICALLCONTEXT_TYPE_JWT_USER_IMPL &_apiCallContext,
    TAPI::Sheba_t   _iban,
    TAPI::Ether_t   _ether
)) {
    quint64 CurrentUserID = _apiCallContext.getActorID();

    QStringList ToUpdate;
    QVariantList Params;

    if (_iban.isNull() == false) {
        ToUpdate.append(tblUserExtraInfo::Fields::ueiIBAN);
        Params.append(_iban);
    }

    if (_ether.isNull() == false) {
        ToUpdate.append(tblUserExtraInfo::Fields::ueiEther);
        Params.append(_ether);
    }

    if (ToUpdate.size()) {
        QString QueryString = QString()
              + "INSERT INTO"
              + " " + tblUserExtraInfo::Name
              + " SET"
              + " " + ToUpdate.join("=? ,") + "=?"
              + "   , uei_usrID=?"
              + " ON DUPLICATE KEY UPDATE"
              + " " + ToUpdate.join("=? ,") + "=?"
              + "   , ueiUpdatedBy_usrID=?"
              ;

        Params.append(CurrentUserID);

        clsDACResult Result = UserExtraInfo::instance().execQuery(_apiCallContext,
                                                                  QueryString,
                                                                  Params + Params
                                                                  );
    }

    return true;
}

bool IMPL_REST_UPDATE(User, extraInfo, (
    APICALLCONTEXT_TYPE_JWT_USER_IMPL &_apiCallContext,
    NULLABLE_TYPE(TAPI::Date_t)    _birthDate,
    QString         _job,
    QString         _education,
    QString         _fieldOfStudy,
//        QString         _language,
    QString         _theme
)) {
    quint64 CurrentUserID = _apiCallContext.getActorID();

    QVariantMap ToUpdateJson;
    QStringList ToRemoveJson;

    //userExtra info json field
    if (_job.isNull() == false) {
        if (_job.isEmpty())
            ToRemoveJson.append(enuUserExtraInfoJsonKey::toStr(enuUserExtraInfoJsonKey::Job));
        else
            ToUpdateJson.insert(enuUserExtraInfoJsonKey::toStr(enuUserExtraInfoJsonKey::Job), _job);
    }
    if (_education.isNull() == false) {
        if (_education.isEmpty())
            ToRemoveJson.append(enuUserExtraInfoJsonKey::toStr(enuUserExtraInfoJsonKey::Education));
        else
            ToUpdateJson.insert(enuUserExtraInfoJsonKey::toStr(enuUserExtraInfoJsonKey::Education), _education);
    }
    if (_fieldOfStudy.isNull() == false) {
        if (_fieldOfStudy.isEmpty())
            ToRemoveJson.append(enuUserExtraInfoJsonKey::toStr(enuUserExtraInfoJsonKey::FieldOfStudy));
        else
            ToUpdateJson.insert(enuUserExtraInfoJsonKey::toStr(enuUserExtraInfoJsonKey::FieldOfStudy), _fieldOfStudy);
    }
//    if (_language.isNull() == false)
//    {
//        if (_language.isEmpty())
//            ToRemoveJson.append(enuUserExtraInfoJsonKey::toStr(enuUserExtraInfoJsonKey::Language));
//        else
//            ToUpdateJson.insert(enuUserExtraInfoJsonKey::toStr(enuUserExtraInfoJsonKey::Language), _language);
//    }
    if (_theme.isNull() == false) {
        if (_theme.isEmpty())
            ToRemoveJson.append(enuUserExtraInfoJsonKey::toStr(enuUserExtraInfoJsonKey::Theme));
        else
            ToUpdateJson.insert(enuUserExtraInfoJsonKey::toStr(enuUserExtraInfoJsonKey::Theme), _theme);
    }

    QString JsonQueryString, UpdateQueryString;

    if (ToUpdateJson.size())
        UpdateQueryString = "'" + QJsonDocument(QJsonObject().fromVariantMap(ToUpdateJson)).toJson(QJsonDocument::Compact) + "'";

    if (ToRemoveJson.length()) {
        if (ToUpdateJson.size()) {
            JsonQueryString = QString()
                  + "JSON_MERGE_PATCH("
                  + "JSON_REMOVE(COALESCE(ueiExtraInfo, '{}'), '$." + ToRemoveJson.join("', '$.") + "'),"
                  + UpdateQueryString
                  + ")";
        } else {
            JsonQueryString = "JSON_REMOVE(COALESCE(ueiExtraInfo, '{}'), '$." + ToRemoveJson.join("', '$.") + "')";
        }

    } else if (ToUpdateJson.size()) {
        JsonQueryString = QString()
              + "JSON_MERGE_PATCH(COALESCE(ueiExtraInfo, '{}'),"
              + UpdateQueryString
              + ")";
    }

    //--------------------------------------
    QVariantList Params;

    //--------------------------------------
    QStringList ToUpdate;

//    if (_birthDate.isNull() == false)
//    {
//        ToUpdate.append(tblUserExtraInfo::Fields::ueiBirthDate);
//        Params.append(_birthDate);
//    }
    if (NULLABLE_HAS_VALUE(_birthDate)) {
        ToUpdate.append(tblUserExtraInfo::Fields::ueiBirthDate);
        Params.append(NULLABLE_VALUE(_birthDate));
    }

    //--------------------------------------
    Params.append(CurrentUserID);

    if ((ToUpdate.isEmpty() == false) || (JsonQueryString.isEmpty() == false)) {
        QString QueryString = QString("INSERT INTO %1 SET").arg(tblUserExtraInfo::Name);

        if (ToUpdate.isEmpty() == false) {
            QueryString += " " + ToUpdate.join("=? ,") + "=?";
            if (JsonQueryString.isEmpty() == false)
                QueryString += ",";
        }
        if (JsonQueryString.isEmpty() == false)
            QueryString += " ueiExtraInfo=" + (ToUpdateJson.size() ? UpdateQueryString : "NULL");

        QueryString += QString()
              + ", uei_usrID=?"
              + " ON DUPLICATE KEY UPDATE";

        if (ToUpdate.isEmpty() == false) {
            QueryString += " " + ToUpdate.join("=? ,") + "=?";
            if (JsonQueryString.isEmpty() == false)
                QueryString += ",";
        }
        if (JsonQueryString.isEmpty() == false)
            QueryString += " ueiExtraInfo=" + JsonQueryString;

        QueryString += ", ueiUpdatedBy_usrID=?";

        qDebug() << "******************************" << QueryString;

        clsDACResult Result = UserExtraInfo::instance().execQuery(_apiCallContext,
                                                                  QueryString,
                                                                  Params + Params
                                                                  );
    }

    //------------------------
    return true;
}

/*****************************************************************\
|* UserExtraInfo *************************************************|
\*****************************************************************/
TARGOMAN_API_SUBMODULE_IMPLEMENT(Account, UserExtraInfo)

UserExtraInfo::UserExtraInfo() :
    intfSQLBasedModule (
        AAASchema,
        tblUserExtraInfo::Name,
        tblUserExtraInfo::Private::ORMFields,
        tblUserExtraInfo::Private::Relations,
        tblUserExtraInfo::Private::Indexes
) { ; }

//bool IMPL_REST_UPDATE(UserExtraInfo, sheba, (APICALLCONTEXT_TYPE_JWT_USER_IMPL &_apiCallContext, TAPI::Sheba_t _sheba))
//{
//    clsDACResult Result = UserExtraInfo::instance().execQuery(
//                              "UPDATE " + this->Name
//                              + QUERY_SEPARATOR
//                              + "SET " + tblUserExtraInfo::Fields::ueiEther +" = ?, " +tblUserExtraInfo::Fields::ueiUpdatedBy_usrID + " = ?"
//                              + QUERY_SEPARATOR
//                              + "WHERE uei_usrID = ?",
//                              { _sheba, _apiCallContext.getActorID(), _apiCallContext.getActorID() }
//        );

//    return Result.numRowsAffected() > 0;
//}

//bool IMPL_REST_UPDATE(UserExtraInfo, etherAddress, (APICALLCONTEXT_TYPE_JWT_USER_IMPL &_apiCallContext, TAPI::Ether_t _etherAddress))
//{
//    clsDACResult Result = UserExtraInfo::instance().execQuery(
//                              "UPDATE " + this->Name
//                              + QUERY_SEPARATOR
//                              + "SET " + tblUserExtraInfo::Fields::ueiEther +" = ?, " +tblUserExtraInfo::Fields::ueiUpdatedBy_usrID + " = ?"
//                              + QUERY_SEPARATOR
//                              + "WHERE uei_usrID = ?",
//                              { _etherAddress, _apiCallContext.getActorID(), _apiCallContext.getActorID() }
//        );

//    return Result.numRowsAffected() > 0;
//}

} //namespace Targoman::API::AccountModule::ORM
