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
///TODO: BAD Gender causes assert
User::User() :
    intfSQLBasedModule(
        AAASchema,
        tblUser::Name,
        {///< ColName                           Type                            Validation                              Default     UpBy    Sort  Filter Self  Virt  PK
            //ORM_PRIMARYKEY_64 with self:true
            { tblUser::usrID,                   S(quint64),                     QFV.integer().minValue(1),              QAuto,      UPNone, true, true,  true, false, true },
            { tblUser::usrEmail,                S(TAPI::Email_t),               QFV.emailNotFake(),                     QNull,      UPOwner },
            { tblUser::usrName,                 S(QString),                     QFV.unicodeAlNum().maxLenght(128),      QNull,      UPOwner },
            { tblUser::usrFamily,               S(QString),                     QFV.unicodeAlNum().maxLenght(128),      QNull,      UPOwner },
            { tblUser::usrGender,               S(TAPI::enuGender::Type),       QFV,                                    TAPI::enuGender::NotExpressed, UPOwner },
            { tblUser::usrMobile,               S(TAPI::Mobile_t),              QFV,                                    QNull,      UPOwner },
            { tblUser::usrApprovalState,        S(TAPI::enuUserApproval::Type), QFV,                                    TAPI::enuUserApproval::None },
            //{ tblUser::usrPass,
            { tblUser::usr_rolID,               S(quint32),                     QFV.integer().minValue(1),              QRequired,  UPAdmin },
            { tblUser::usrSpecialPrivs,         S(TAPI::PrivObject_t),          QFV,                                    QNull,      UPAdmin, false, false },
            { tblUser::usrLanguage,             S(QString),                     QFV.languageCode(),                     "fa",       UPOwner },
            { tblUser::usrEnableEmailAlerts,    S(bool),                        QFV,                                    true,       UPOwner },
            { tblUser::usrEnableSMSAlerts,      S(bool),                        QFV,                                    true,       UPOwner },
            { tblUser::usrMaxSessions,          S(qint32),                      QFV.integer().betweenValues(-1, 100),   -1,         UPAdmin },
            { tblUser::usrActiveSessions,       S(qint32),                      QFV.integer().betweenValues(-1, 1000),  QInvalid,   UPNone },
            { tblUser::usrLastLogin,            S(TAPI::DateTime_t),            QFV,                                    QInvalid,   UPNone },
            { tblUser::usrStatus,               ORM_STATUS_FIELD(TAPI::enuUserStatus, TAPI::enuUserStatus::MustValidate) },
            { ORM_INVALIDATED_AT_FIELD },
            { tblUser::usrCreationDateTime,     ORM_CREATED_ON },
            { tblUser::usrCreatedBy_usrID,      ORM_CREATED_BY_NULLABLE },
            { tblUser::usrUpdatedBy_usrID,      ORM_UPDATED_BY },
        },
        {///< Col                               Reference Table                                     ForeignCol                      Rename  LeftJoin
            { tblUser::usr_rolID,               R(AAASchema, tblRoles::Name),                       tblRoles::rolID },
            { tblUser::Relation::ExtraInfo, { tblUser::usrID, R(AAASchema, tblUserExtraInfo::Name), tblUserExtraInfo::uei_usrID,    "",     true } },
            ORM_RELATION_OF_CREATOR(tblUser::usrCreatedBy_usrID),
            ORM_RELATION_OF_UPDATER(tblUser::usrUpdatedBy_usrID),
        },
        {
            { {
                  tblUser::usrEmail,
                  ORM_INVALIDATED_AT_FIELD_NAME,
              }, enuDBIndex::Unique },
            { {
                  tblUser::usrMobile,
                  ORM_INVALIDATED_AT_FIELD_NAME,
              }, enuDBIndex::Unique },
        }
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
    if (_APICALLBOOM.getUserID() != _pksByPath.toULongLong())
        Authorization::checkPriv(_APICALLBOOM.getJWT(), { "Account:User:CRUD~0100" });

    if (_cols.isEmpty())
        _cols = QStringList({
            tblUser::usrEmail,
            tblUser::usrName,
            tblUser::usrFamily,
            tblUser::usrGender,
            tblUser::usrMobile,
            tblUser::usrApprovalState,
            tblUser::usr_rolID,
//            tblUser::usrSpecialPrivs,
            tblUser::usrLanguage,
            tblUser::usrEnableEmailAlerts,
            tblUser::usrEnableSMSAlerts,
//            tblUser::usrMaxSessions,
            tblUser::usrActiveSessions,
            tblUser::usrLastLogin,
            tblUser::usrStatus,
//            tblUser::usrCreationDateTime,
//            tblUser::usrCreatedBy_usrID,
//            tblUser::usrUpdatedBy_usrID,
            Targoman::API::CURRENT_TIMESTAMP,
            //-------------------
//            tblUserExtraInfo::uei_usrID,
            //tblUserExtraInfo::ueiGender,
            tblUserExtraInfo::ueiBirthDate,
//            tblUserExtraInfo::ueiPhoto,
//            tblUserExtraInfo::ueiOAuthAccounts,
            tblUserExtraInfo::ueiIBAN,
            tblUserExtraInfo::ueiEther,
            tblUserExtraInfo::ueiExtraInfo,
//            tblUserExtraInfo::ueiUpdatedBy_usrID,
        }).join(",");

    return this->Select(
                *this,
                GET_METHOD_CALL_ARGS_INTERNAL_CALL,
                {},
                0,
                [](SelectQuery &_query) {
                    _query.leftJoinWith(tblUser::Relation::ExtraInfo);
    });

//    return query.one();

    //    return this->selectFromTable({},{}, GET_METHOD_CALL_ARGS_APICALL);
}

quint64 IMPL_ORMCREATE(User) {
    Authorization::checkPriv(_APICALLBOOM.getJWT(), this->privOn(EHTTP_PUT, this->moduleBaseName()));
    if (_createInfo.value(tblUser::usrEmail).toString().isEmpty() && _createInfo.value(tblUser::usrMobile).toString().isEmpty())
        throw exHTTPBadRequest("Either email or mobile must be provided to create user");

    return this->Create(*this, CREATE_METHOD_CALL_ARGS_INTERNAL_CALL);
}

/*
 * this method only can call by admin user
 */
bool IMPL_ORMUPDATE(User) {
    Authorization::checkPriv(_APICALLBOOM.getJWT(), this->privOn(EHTTP_PATCH, this->moduleBaseName()));

    return this->Update(*this, UPDATE_METHOD_CALL_ARGS_INTERNAL_CALL);
}

bool IMPL_ORMDELETE(User) {
    Authorization::checkPriv(_APICALLBOOM.getJWT(), this->privOn(EHTTP_DELETE, this->moduleBaseName()));

    return this->DeleteByPks(*this, DELETE_METHOD_CALL_ARGS_INTERNAL_CALL);
}

SelectQuery User::getPhotoQuery(quint64 _usrID) {

    UserExtraInfo::instance().prepareFiltersList();

    return SelectQuery(UserExtraInfo::instance())
        .addCol(tblUserExtraInfo::ueiPhoto)
        .where({ tblUserExtraInfo::uei_usrID, enuConditionOperator::Equal, _usrID })
    ;
}

TAPI::Base64Image_t IMPL_REST_GET(User, photo, (
    APICALLBOOM_TYPE_JWT_IMPL &APICALLBOOM_PARAM,
    quint64 _usrID
)) {
    quint64 CurrentUserID = _APICALLBOOM.getUserID();

    if (CurrentUserID != _usrID)
        Authorization::checkPriv(_APICALLBOOM.getJWT(), { "Account:User:photo:CRUD~0100" });

    auto Photo = getPhotoQuery(_usrID)
            .setCacheTime(30)
            .one()
            .value(tblUserExtraInfo::ueiPhoto).toString().toLatin1();
            ;

    return TAPI::Base64Image_t(Photo);
}

bool IMPL_REST_UPDATE(User, photo, (
    APICALLBOOM_TYPE_JWT_IMPL &APICALLBOOM_PARAM,
    TAPI::Base64Image_t _image
)) {
    quint64 CurrentUserID = _APICALLBOOM.getUserID();

    QString qry = QString()
          + "INSERT INTO"
          + " " + tblUserExtraInfo::Name
          + " SET ueiPhoto=?"
          + "   , uei_usrID=?"
          + " ON DUPLICATE KEY UPDATE"
          + "     ueiPhoto=?"
          + "   , ueiUpdatedBy_usrID=?"
          ;

    clsDACResult Result = UserExtraInfo::instance().execQuery(
                              qry,
                              {
                                  _image,
                                  CurrentUserID,
                                  _image,
                                  CurrentUserID,
                              });

    bool OK = Result.numRowsAffected() > 0;

    if (OK)
        getPhotoQuery(CurrentUserID).clearCache();

    return OK;
}

bool IMPL_REST_POST(User, deletePhoto, (
    APICALLBOOM_TYPE_JWT_IMPL &APICALLBOOM_PARAM
)) {
    quint64 CurrentUserID = _APICALLBOOM.getUserID();

    QString qry = QString()
          + "UPDATE"
          + " " + tblUserExtraInfo::Name
          + " SET ueiPhoto=NULL"
          + "   , ueiUpdatedBy_usrID=?"
          + " WHERE uei_usrID=?"
          ;

    clsDACResult Result = UserExtraInfo::instance().execQuery(
                              qry,
                              {
                                  CurrentUserID,
                                  CurrentUserID,
                              });

    bool OK = Result.numRowsAffected() > 0;

    if (OK)
        getPhotoQuery(CurrentUserID).clearCache();

    return OK;
}

bool IMPL_REST_UPDATE(User, email, (
    APICALLBOOM_TYPE_JWT_IMPL &APICALLBOOM_PARAM,
    TAPI::Email_t   _email,
    TAPI::MD5_t     _psw,
    QString         _salt
)) {
    quint64 CurrentUserID = _APICALLBOOM.getUserID();

    _email = _email.toLower().trimmed();

    if (_email.isEmpty())
        throw exHTTPBadRequest("Email is empty");

    if ((QFV.email().isValid(_email) == false) || (QFV.emailNotFake().isValid(_email) == false))
        throw exHTTPBadRequest("Email domain is suspicious. Please use a real email.");

    if (_psw.isEmpty() || _salt.isEmpty())
        throw exHTTPBadRequest("Password and salt are required to change email");

    QFV.asciiAlNum().maxLenght(20).validate(_salt, "salt");

    this->callSP("spApproval_Request", {
                     { "iBy", "E" },
                     { "iUserID", CurrentUserID },
                     { "iKey", _email },
                     { "iPass", _psw },
                     { "iSalt", _salt },
                 });

    return true;
}

bool IMPL_REST_UPDATE(User, mobile, (
    APICALLBOOM_TYPE_JWT_IMPL &APICALLBOOM_PARAM,
    TAPI::Mobile_t  _mobile,
    TAPI::MD5_t     _psw,
    QString         _salt
)) {
    quint64 CurrentUserID = _APICALLBOOM.getUserID();

    _mobile = PhoneHelper::NormalizePhoneNumber(_mobile);

    if (_mobile.isEmpty())
        throw exHTTPBadRequest("Mobile is empty");

    if (QFV.mobile().isValid(_mobile) == false)
        throw exHTTPBadRequest("Invalid mobile.");

    if (_psw.isEmpty() || _salt.isEmpty())
        throw exHTTPBadRequest("Password and salt are required to change email");

    QFV.asciiAlNum().maxLenght(20).validate(_salt, "salt");

    this->callSP("spApproval_Request", {
                     { "iBy", "M" },
                     { "iUserID", CurrentUserID },
                     { "iKey", _mobile },
                     { "iPass", _psw },
                     { "iSalt", _salt },
                 });

    return true;
}

bool IMPL_REST_UPDATE(User, personalInfo, (
    APICALLBOOM_TYPE_JWT_IMPL &APICALLBOOM_PARAM,
    QString             _name,
    QString             _family,
    TAPI::ISO639_2_t    _language,
    NULLABLE_TYPE(TAPI::enuGender::Type) _gender,
    NULLABLE_TYPE(bool) _enableEmailAlerts,
    NULLABLE_TYPE(bool) _enableSMSAlerts
)) {
    quint64 CurrentUserID = _APICALLBOOM.getUserID();

    QVariantMap ToUpdate;

    if (_name.isNull() == false)                    ToUpdate.insert(tblUser::usrName, _name);
    if (_family.isNull() == false)                  ToUpdate.insert(tblUser::usrFamily, _family);
    if (_language.isNull() == false)                ToUpdate.insert(tblUser::usrLanguage, _language);
    if (NULLABLE_HAS_VALUE(_gender))                ToUpdate.insert(tblUser::usrGender, *_gender);
    if (NULLABLE_HAS_VALUE(_enableEmailAlerts))     ToUpdate.insert(tblUser::usrEnableEmailAlerts, *_enableEmailAlerts ? 1 : 0);
    if (NULLABLE_HAS_VALUE(_enableSMSAlerts))       ToUpdate.insert(tblUser::usrEnableSMSAlerts, *_enableSMSAlerts ? 1 : 0);

    if (ToUpdate.size())
        this->Update(*this,
                     _APICALLBOOM,
                     QString("%1").arg(CurrentUserID),
                     ToUpdate
                     );

    return true;
}

bool IMPL_REST_UPDATE(User, financialInfo, (
    APICALLBOOM_TYPE_JWT_IMPL &APICALLBOOM_PARAM,
    TAPI::Sheba_t   _iban,
    TAPI::Ether_t   _ether
)) {
    quint64 CurrentUserID = _APICALLBOOM.getUserID();

    QStringList ToUpdate;
    QVariantList Params;

    if (_iban.isNull() == false) {
        ToUpdate.append(tblUserExtraInfo::ueiIBAN);
        Params.append(_iban);
    }

    if (_ether.isNull() == false) {
        ToUpdate.append(tblUserExtraInfo::ueiEther);
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

        clsDACResult Result = UserExtraInfo::instance().execQuery(
                                  qry,
                                  Params
                                  + Params
                              );
    }

    return true;
}

bool IMPL_REST_UPDATE(User, extraInfo, (
    APICALLBOOM_TYPE_JWT_IMPL &APICALLBOOM_PARAM,
    NULLABLE_TYPE(TAPI::Date_t)    _birthDate,
    QString         _job,
    QString         _education,
    QString         _fieldOfStudy,
//        QString         _language,
    QString         _theme
)) {
    quint64 CurrentUserID = _APICALLBOOM.getUserID();

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
//        ToUpdate.append(tblUserExtraInfo::ueiBirthDate);
//        Params.append(_birthDate);
//    }
    if (NULLABLE_HAS_VALUE(_birthDate)) {
        ToUpdate.append(tblUserExtraInfo::ueiBirthDate);
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

        clsDACResult Result = UserExtraInfo::instance().execQuery(
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
UserExtraInfo::UserExtraInfo() :
    intfSQLBasedModule (
        AAASchema,
        tblUserExtraInfo::Name,
        {///< ColName                               Type                                            Validation      Default    UpBy   Sort  Filter Self  Virt   PK
            { tblUserExtraInfo::uei_usrID,          ORM_PRIMARYKEY_64 },
//            { tblUserExtraInfo::ueiGender,          S(NULLABLE_TYPE(TAPI::enuGender::Type)),    QFV,            TAPI::enuGender::NotExpressed,  UPOwner,false,false },
            { tblUserExtraInfo::ueiBirthDate,       S(NULLABLE_TYPE(TAPI::Date_t)),                 QFV,            QNull,  UPOwner },
            { tblUserExtraInfo::ueiPhoto,           S(NULLABLE_TYPE(TAPI::Base64Image_t)),          QFV,            QNull,  UPOwner, false, false },
            { tblUserExtraInfo::ueiOAuthAccounts,   S(NULLABLE_TYPE(TAPI::JSON_t)),                 QFV,            QNull,  UPNone },
            { tblUserExtraInfo::ueiIBAN,            S(NULLABLE_TYPE(TAPI::Sheba_t)),                QFV.iban("IR"), QNull,  UPOwner, false, false },
            { tblUserExtraInfo::ueiEther,           S(NULLABLE_TYPE(TAPI::Ether_t)),                QFV,            QNull,  UPOwner, false, false },
            { tblUserExtraInfo::ueiExtraInfo,       S(NULLABLE_TYPE(TAPI::JSON_t)),                 QFV,            QNull,  UPOwner, false, false },
            { tblUserExtraInfo::ueiUpdatedBy_usrID, ORM_UPDATED_BY },
        },
        {///< Col                                   Reference Table                 ForeignCol      Rename  LeftJoin
            ORM_RELATION_OF_UPDATER(tblUserExtraInfo::ueiUpdatedBy_usrID),
        }
    )
{ ; }

//bool IMPL_REST_UPDATE(UserExtraInfo, sheba, (APICALLBOOM_TYPE_JWT_IMPL &APICALLBOOM_PARAM, TAPI::Sheba_t _sheba))
//{
//    clsDACResult Result = UserExtraInfo::instance().execQuery(
//                              "UPDATE " + this->Name
//                              + QUERY_SEPARATOR
//                              + "SET " + tblUserExtraInfo::ueiEther +" = ?, " +tblUserExtraInfo::ueiUpdatedBy_usrID + " = ?"
//                              + QUERY_SEPARATOR
//                              + "WHERE uei_usrID = ?",
//                              { _sheba, _APICALLBOOM.getUserID(), _APICALLBOOM.getUserID() }
//        );

//    return Result.numRowsAffected() > 0;
//}

//bool IMPL_REST_UPDATE(UserExtraInfo, etherAddress, (APICALLBOOM_TYPE_JWT_IMPL &APICALLBOOM_PARAM, TAPI::Ether_t _etherAddress))
//{
//    clsDACResult Result = UserExtraInfo::instance().execQuery(
//                              "UPDATE " + this->Name
//                              + QUERY_SEPARATOR
//                              + "SET " + tblUserExtraInfo::ueiEther +" = ?, " +tblUserExtraInfo::ueiUpdatedBy_usrID + " = ?"
//                              + QUERY_SEPARATOR
//                              + "WHERE uei_usrID = ?",
//                              { _etherAddress, _APICALLBOOM.getUserID(), _APICALLBOOM.getUserID() }
//        );

//    return Result.numRowsAffected() > 0;
//}

} //namespace Targoman::API::AccountModule::ORM
