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
using namespace Targoman::API::Helpers;

TAPI_REGISTER_TARGOMAN_ENUM(Targoman::API::AccountModule, enuUserExtraInfoJsonKey);

namespace Targoman::API::AccountModule::ORM {

//using namespace DBManager;

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

QVariant IMPL_ORMGET(User) {
    if (APICALLBOOM_PARAM.getActorID() != _pksByPath.toULongLong())
        Authorization::checkPriv(APICALLBOOM_PARAM, { "Account:User:CRUD~0100" });

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

quint64 IMPL_ORMCREATE(User) {
    Authorization::checkPriv(APICALLBOOM_PARAM, this->privOn(EHTTP_PUT, this->moduleBaseName()));
    if (_createInfo.value(tblUser::Fields::usrEmail).toString().isEmpty() && _createInfo.value(tblUser::Fields::usrMobile).toString().isEmpty())
        throw exHTTPBadRequest("Either email or mobile must be provided to create user");

    return this->Create(CREATE_METHOD_ARGS_CALL_VALUES);
}

/*
 * this method only can call by admin user
 */
bool IMPL_ORMUPDATE(User) {
    Authorization::checkPriv(APICALLBOOM_PARAM, this->privOn(EHTTP_PATCH, this->moduleBaseName()));

    return this->Update(UPDATE_METHOD_ARGS_CALL_VALUES);
}

bool IMPL_ORMDELETE(User) {
    Authorization::checkPriv(APICALLBOOM_PARAM, this->privOn(EHTTP_DELETE, this->moduleBaseName()));

    return this->DeleteByPks(DELETE_METHOD_ARGS_CALL_VALUES);
}

ORMSelectQuery User::getPhotoQuery(
    INTFAPICALLBOOM_IMPL &APICALLBOOM_PARAM,
    quint64 _usrID
) {

    UserExtraInfo::instance().prepareFiltersList();

    return UserExtraInfo::instance().makeSelectQuery(APICALLBOOM_PARAM)
        .addCol(tblUserExtraInfo::Fields::ueiPhoto)
        .where({ tblUserExtraInfo::Fields::uei_usrID, enuConditionOperator::Equal, _usrID })
    ;
}

TAPI::Base64Image_t IMPL_REST_GET(User, photo, (
    APICALLBOOM_TYPE_JWT_USER_IMPL &APICALLBOOM_PARAM,
    quint64 _usrID
)) {
    quint64 CurrentUserID = APICALLBOOM_PARAM.getActorID();

    if (CurrentUserID != _usrID)
        Authorization::checkPriv(APICALLBOOM_PARAM, { "Account:User:photo:CRUD~0100" });

    auto Photo = getPhotoQuery(APICALLBOOM_PARAM, _usrID)
            .setCacheTime(30)
            .one()
            .value(tblUserExtraInfo::Fields::ueiPhoto).toString().toLatin1();
            ;

    return TAPI::Base64Image_t(Photo);
}

bool IMPL_REST_UPDATE(User, photo, (
    APICALLBOOM_TYPE_JWT_USER_IMPL &APICALLBOOM_PARAM,
    TAPI::Base64Image_t _image
)) {
    quint64 CurrentUserID = APICALLBOOM_PARAM.getActorID();

    QString qry = QString()
          + "INSERT INTO"
          + " " + tblUserExtraInfo::Name
          + " SET ueiPhoto=?"
          + "   , uei_usrID=?"
          + " ON DUPLICATE KEY UPDATE"
          + "     ueiPhoto=?"
          + "   , ueiUpdatedBy_usrID=?"
          ;

    clsDACResult Result = UserExtraInfo::instance().execQuery(APICALLBOOM_PARAM,
                                                              qry, {
                                                                  _image,
                                                                  CurrentUserID,
                                                                  _image,
                                                                  CurrentUserID,
                                                              });

    bool OK = Result.numRowsAffected() > 0;

    if (OK)
        getPhotoQuery(APICALLBOOM_PARAM, CurrentUserID).clearCache();

    return OK;
}

bool IMPL_REST_POST(User, deletePhoto, (
    APICALLBOOM_TYPE_JWT_USER_IMPL &APICALLBOOM_PARAM
)) {
    quint64 CurrentUserID = APICALLBOOM_PARAM.getActorID();

    QString qry = QString()
          + "UPDATE"
          + " " + tblUserExtraInfo::Name
          + " SET ueiPhoto=NULL"
          + "   , ueiUpdatedBy_usrID=?"
          + " WHERE uei_usrID=?"
          ;

    clsDACResult Result = UserExtraInfo::instance().execQuery(APICALLBOOM_PARAM,
                                                              qry, {
                                                                  CurrentUserID,
                                                                  CurrentUserID,
                                                              });

    bool OK = Result.numRowsAffected() > 0;

    if (OK)
        getPhotoQuery(APICALLBOOM_PARAM, CurrentUserID).clearCache();

    return OK;
}

bool IMPL_REST_UPDATE(User, email, (
    APICALLBOOM_TYPE_JWT_USER_IMPL &APICALLBOOM_PARAM,
    TAPI::Email_t   _email,
    TAPI::MD5_t     _psw,
    QString         _salt
)) {
    quint64 CurrentUserID = APICALLBOOM_PARAM.getActorID();

    _email = _email.toLower().trimmed();

    if (_email.isEmpty())
        throw exHTTPBadRequest("Email is empty");

    if ((QFV.email().isValid(_email) == false) || (QFV.emailNotFake().isValid(_email) == false))
        throw exHTTPBadRequest("Email domain is suspicious. Please use a real email.");

    if (_psw.isEmpty() || _salt.isEmpty())
        throw exHTTPBadRequest("Password and salt are required to change email");

    QFV.asciiAlNum().maxLenght(20).validate(_salt, "salt");

    this->callSP(APICALLBOOM_PARAM,
                 "spApproval_Request", {
                     { "iBy", "E" },
                     { "iUserID", CurrentUserID },
                     { "iKey", _email },
                     { "iPass", _psw },
                     { "iSalt", _salt },
                 });

    return true;
}

bool IMPL_REST_UPDATE(User, mobile, (
    APICALLBOOM_TYPE_JWT_USER_IMPL &APICALLBOOM_PARAM,
    TAPI::Mobile_t  _mobile,
    TAPI::MD5_t     _psw,
    QString         _salt
)) {
    quint64 CurrentUserID = APICALLBOOM_PARAM.getActorID();

    _mobile = PhoneHelper::NormalizePhoneNumber(_mobile);

    if (_mobile.isEmpty())
        throw exHTTPBadRequest("Mobile is empty");

    if (QFV.mobile().isValid(_mobile) == false)
        throw exHTTPBadRequest("Invalid mobile.");

    if (_psw.isEmpty() || _salt.isEmpty())
        throw exHTTPBadRequest("Password and salt are required to change email");

    QFV.asciiAlNum().maxLenght(20).validate(_salt, "salt");

    this->callSP(APICALLBOOM_PARAM,
                 "spApproval_Request", {
                     { "iBy", "M" },
                     { "iUserID", CurrentUserID },
                     { "iKey", _mobile },
                     { "iPass", _psw },
                     { "iSalt", _salt },
                 });

    return true;
}

bool IMPL_REST_UPDATE(User, personalInfo, (
    APICALLBOOM_TYPE_JWT_USER_IMPL &APICALLBOOM_PARAM,
    QString             _name,
    QString             _family,
    TAPI::ISO639_2_t    _language,
    NULLABLE_TYPE(TAPI::enuGender::Type) _gender,
    NULLABLE_TYPE(bool) _enableEmailAlerts,
    NULLABLE_TYPE(bool) _enableSMSAlerts
)) {
    quint64 CurrentUserID = APICALLBOOM_PARAM.getActorID();

    QVariantMap ToUpdate;

    if (_name.isNull() == false)                    ToUpdate.insert(tblUser::Fields::usrName, _name);
    if (_family.isNull() == false)                  ToUpdate.insert(tblUser::Fields::usrFamily, _family);
    if (_language.isNull() == false)                ToUpdate.insert(tblUser::Fields::usrLanguage, _language);
    if (NULLABLE_HAS_VALUE(_gender))                ToUpdate.insert(tblUser::Fields::usrGender, *_gender);
    if (NULLABLE_HAS_VALUE(_enableEmailAlerts))     ToUpdate.insert(tblUser::Fields::usrEnableEmailAlerts, *_enableEmailAlerts ? 1 : 0);
    if (NULLABLE_HAS_VALUE(_enableSMSAlerts))       ToUpdate.insert(tblUser::Fields::usrEnableSMSAlerts, *_enableSMSAlerts ? 1 : 0);

    if (ToUpdate.size())
        this->Update(APICALLBOOM_PARAM,
                     QString::number(CurrentUserID),
                     ToUpdate
                     );

    return true;
}

bool IMPL_REST_UPDATE(User, financialInfo, (
    APICALLBOOM_TYPE_JWT_USER_IMPL &APICALLBOOM_PARAM,
    TAPI::Sheba_t   _iban,
    TAPI::Ether_t   _ether
)) {
    quint64 CurrentUserID = APICALLBOOM_PARAM.getActorID();

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
        QString qry = QString()
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

        clsDACResult Result = UserExtraInfo::instance().execQuery(APICALLBOOM_PARAM,
                                                                  qry,
                                                                  Params + Params
                                                                  );
    }

    return true;
}

bool IMPL_REST_UPDATE(User, extraInfo, (
    APICALLBOOM_TYPE_JWT_USER_IMPL &APICALLBOOM_PARAM,
    NULLABLE_TYPE(TAPI::Date_t)    _birthDate,
    QString         _job,
    QString         _education,
    QString         _fieldOfStudy,
//        QString         _language,
    QString         _theme
)) {
    quint64 CurrentUserID = APICALLBOOM_PARAM.getActorID();

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

    QString jsonQry, updateQuery;

    if (ToUpdateJson.size())
        updateQuery = "'" + QJsonDocument(QJsonObject().fromVariantMap(ToUpdateJson)).toJson(QJsonDocument::Compact) + "'";

    if (ToRemoveJson.length()) {
        if (ToUpdateJson.size()) {
            jsonQry = QString()
                  + "JSON_MERGE_PATCH("
                  + "JSON_REMOVE(COALESCE(ueiExtraInfo, '{}'), '$." + ToRemoveJson.join("', '$.") + "'),"
                  + updateQuery
                  + ")";
        } else {
            jsonQry = "JSON_REMOVE(COALESCE(ueiExtraInfo, '{}'), '$." + ToRemoveJson.join("', '$.") + "')";
        }

    } else if (ToUpdateJson.size()) {
        jsonQry = QString()
              + "JSON_MERGE_PATCH(COALESCE(ueiExtraInfo, '{}'),"
              + updateQuery
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
        Params.append(NULLABLE_GET(_birthDate));
    }

    //--------------------------------------
    Params.append(CurrentUserID);
    if ((ToUpdate.isEmpty() == false) || (jsonQry.isEmpty() == false)) {
        QString qry = QString("INSERT INTO %1 SET").arg(tblUserExtraInfo::Name);

        if (ToUpdate.isEmpty() == false) {
            qry += " " + ToUpdate.join("=? ,") + "=?";
            if (jsonQry.isEmpty() == false)
                qry += ",";
        }
        if (jsonQry.isEmpty() == false)
            qry += " ueiExtraInfo=" + (ToUpdateJson.size() ? updateQuery : "NULL");

        qry += QString()
              + ", uei_usrID=?"
              + " ON DUPLICATE KEY UPDATE";

        if (ToUpdate.isEmpty() == false) {
            qry += " " + ToUpdate.join("=? ,") + "=?";
            if (jsonQry.isEmpty() == false)
                qry += ",";
        }
        if (jsonQry.isEmpty() == false)
            qry += " ueiExtraInfo=" + jsonQry;

        qry += ", ueiUpdatedBy_usrID=?";

        qDebug() << "******************************" << qry;

        clsDACResult Result = UserExtraInfo::instance().execQuery(APICALLBOOM_PARAM,
                                                                  qry,
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

//bool IMPL_REST_UPDATE(UserExtraInfo, sheba, (APICALLBOOM_TYPE_JWT_USER_IMPL &APICALLBOOM_PARAM, TAPI::Sheba_t _sheba))
//{
//    clsDACResult Result = UserExtraInfo::instance().execQuery(
//                              "UPDATE " + this->Name
//                              + QUERY_SEPARATOR
//                              + "SET " + tblUserExtraInfo::Fields::ueiEther +" = ?, " +tblUserExtraInfo::Fields::ueiUpdatedBy_usrID + " = ?"
//                              + QUERY_SEPARATOR
//                              + "WHERE uei_usrID = ?",
//                              { _sheba, APICALLBOOM_PARAM.getActorID(), APICALLBOOM_PARAM.getActorID() }
//        );

//    return Result.numRowsAffected() > 0;
//}

//bool IMPL_REST_UPDATE(UserExtraInfo, etherAddress, (APICALLBOOM_TYPE_JWT_USER_IMPL &APICALLBOOM_PARAM, TAPI::Ether_t _etherAddress))
//{
//    clsDACResult Result = UserExtraInfo::instance().execQuery(
//                              "UPDATE " + this->Name
//                              + QUERY_SEPARATOR
//                              + "SET " + tblUserExtraInfo::Fields::ueiEther +" = ?, " +tblUserExtraInfo::Fields::ueiUpdatedBy_usrID + " = ?"
//                              + QUERY_SEPARATOR
//                              + "WHERE uei_usrID = ?",
//                              { _etherAddress, APICALLBOOM_PARAM.getActorID(), APICALLBOOM_PARAM.getActorID() }
//        );

//    return Result.numRowsAffected() > 0;
//}

} //namespace Targoman::API::AccountModule::ORM
