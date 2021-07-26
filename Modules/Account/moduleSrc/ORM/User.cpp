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

#include "Interfaces/ORM/APIQueryBuilders.h"

namespace Targoman::API::AAA {

using namespace ORM;
using namespace DBManager;

/*****************************************************************\
|* User **********************************************************|
\*****************************************************************/
//TODO BAD Gender causes assert
User::User() :
    clsTable(
        AAASchema,
        tblUser::Name,
        {///< ColName                       Type                            Validation                             Default    UpBy    Sort  Filter Self  Virt  PK
            //ORM_PRIMARYKEY_64 with self:true
            { tblUser::usrID,               S(quint64),                     QFV.integer().minValue(1),             QAuto,     UPNone, true, true,  true, false, true },
            { tblUser::usrGender,           S(TAPI::enuUserGender::Type),   QFV,                                   TAPI::enuUserGender::NotExpressed, UPOwner },
            { tblUser::usrName,             S(QString),                     QFV.unicodeAlNum().maxLenght(100),     QNull,     UPOwner },
            { tblUser::usrFamily,           S(QString),                     QFV.unicodeAlNum().maxLenght(100),     QNull,     UPOwner },
            { tblUser::usrEmail,            S(TAPI::Email_t),               QFV.emailNotFake(),                    QNull,     UPOwner },
            { tblUser::usrMobile,           S(TAPI::Mobile_t),              QFV,                                   QNull,     UPOwner },
            { tblUser::usrApprovalState,    S(TAPI::enuUserApproval::Type), QFV,                                   TAPI::enuUserApproval::None },
            //{ tblUser::usrPass,
            { tblUser::usr_rolID,           S(quint32),                     QFV.integer().minValue(1),             QRequired, UPAdmin },
            { tblUser::usrSpecialPrivs,     S(TAPI::PrivObject_t),          QFV,                                   QNull,     UPAdmin, false, false },
            { tblUser::usrLanguage,         S(QString),                     QFV.languageCode(),                    "fa",      UPOwner },
            { tblUser::usrMaxSessions,      S(qint32),                      QFV.integer().betweenValues(-1, 100),  -1,        UPAdmin },
            { tblUser::usrActiveSessions,   S(qint32),                      QFV.integer().betweenValues(-1, 1000), QInvalid,  UPNone },
            { tblUser::usrLastLogin,        S(TAPI::DateTime_t),            QFV,                                   QInvalid,  UPNone },
            { tblUser::usrStatus,           ORM_STATUS_FIELD(TAPI::enuUserStatus, TAPI::enuUserStatus::MustValidate) },
            { ORM_INVALIDATED_AT_FIELD },
            { tblUser::usrCreationDateTime, ORM_CREATED_ON },
            { tblUser::usrCreatedBy_usrID,  ORM_CREATED_BY_NULLABLE },
            { tblUser::usrUpdatedBy_usrID,  ORM_UPDATED_BY },
        },
        {///< Col                          Reference Table                        ForeignCol                    Rename LeftJoin
            { tblUser::usr_rolID,          R(AAASchema, tblRoles::Name),          tblRoles::rolID },
            { tblUser::usrID,              R(AAASchema, tblUserExtraInfo::Name),  tblUserExtraInfo::uei_usrID,  "",    true },
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

QVariant User::apiGET(GET_METHOD_ARGS_IMPL_APICALL)
{
    if (clsJWT(_JWT).usrID() != _pksByPath.toULongLong())
        Authorization::checkPriv(_JWT, {"Account:User:CRUD~0100"});

    return Targoman::API::Query::Select(*this, GET_METHOD_CALL_ARGS_INTERNAL_CALL);

//    return query.one();

    //    return this->selectFromTable({},{}, GET_METHOD_CALL_ARGS_APICALL);
}

quint64 User::apiCREATE(CREATE_METHOD_ARGS_IMPL_APICALL)
{
    Authorization::checkPriv(_JWT, this->privOn(EHTTP_PUT, this->moduleBaseName()));
    if (_createInfo.value(tblUser::usrEmail).toString().isEmpty() && _createInfo.value(tblUser::usrMobile).toString().isEmpty())
        throw exHTTPBadRequest("Either email or mobile must be provided to create user");

    return Targoman::API::Query::Create(*this, CREATE_METHOD_CALL_ARGS_INTERNAL_CALL);
}

bool User::apiUPDATE(UPDATE_METHOD_ARGS_IMPL_APICALL)
{
    Authorization::checkPriv(_JWT, this->privOn(EHTTP_PATCH, this->moduleBaseName()));

    return Targoman::API::Query::Update(*this, UPDATE_METHOD_CALL_ARGS_INTERNAL_CALL);
}

bool User::apiDELETE(DELETE_METHOD_ARGS_IMPL_APICALL)
{
    Authorization::checkPriv(_JWT, this->privOn(EHTTP_DELETE, this->moduleBaseName()));

    return Targoman::API::Query::DeleteByPks(*this, DELETE_METHOD_CALL_ARGS_INTERNAL_CALL);
}

bool User::apiUPDATEprofile(TAPI::JWT_t _JWT,
                            NULLABLE_TYPE(TAPI::enuUserGender::Type) _gender,
                            NULLABLE_TYPE(QString) _name,
                            NULLABLE_TYPE(QString) _family,
                            NULLABLE_TYPE(TAPI::ISO639_2_t) _lang,
                            NULLABLE_TYPE(TAPI::Email_t) _email,
                            NULLABLE_TYPE(TAPI::Mobile_t) _mobile,
                            NULLABLE_TYPE(TAPI::MD5_t) _pass,
                            NULLABLE_TYPE(QString) _salt){
    if (NULLABLE_HAS_VALUE(_email) || NULLABLE_HAS_VALUE(_mobile)){
        if (NULLABLE_IS_NULL(_pass) || _pass->isEmpty() || NULLABLE_IS_NULL(_salt))
            throw exHTTPBadRequest("Password and salt are required to change email");
        QFV.asciiAlNum().maxLenght(20).validate(*_salt, "salt");
    }
    if (NULLABLE_HAS_VALUE(_email) && _email->size())
        this->callSP("AAA.sp_CREATE_approvalRequest",{
                         {"iWhat2Approve", "E"},
                         {"iUserID", clsJWT(_JWT).usrID()},
                         {"iValue", *_email},
                         {"iPass", *_pass},
                         {"iSalt", *_salt},
                     });
    if (NULLABLE_HAS_VALUE(_mobile) && _mobile->size())
        this->callSP("AAA.sp_CREATE_approvalRequest",{
                         {"iWhat2Approve", "E"},
                         {"iUserID", clsJWT(_JWT).usrID()},
                         {"iValue", *_mobile},
                         {"iPass", *_pass},
                         {"iSalt", *_salt},
                     });

    QVariantMap ToUpdate;
    if (NULLABLE_HAS_VALUE(_name)) ToUpdate.insert(tblUser::usrName, *_name);
    if (NULLABLE_HAS_VALUE(_family)) ToUpdate.insert(tblUser::usrFamily, *_family);
    if (NULLABLE_HAS_VALUE(_lang)) ToUpdate.insert(tblUser::usrLanguage, *_lang);
    if (NULLABLE_HAS_VALUE(_gender)) ToUpdate.insert(tblUser::usrGender, *_gender);

    /*if(ToUpdate.size())
        return this->update(clsJWT(_JWT).usrID(), {{tblUser::usrID, clsJWT(_JWT).usrID()}}, ToUpdate );*/
    return true;
}

TAPI::RawData_t User::apiGETPhoto(TAPI::JWT_t _JWT, quint64 _usrID) {
//    auto Photo =  this->selectFromTable({},{}, QString::number(_usrID), 0, 1, tblUserExtraInfo::ueiPhoto).toMap().value(tblUserExtraInfo::ueiPhoto).toString().toLatin1();

    auto Photo = SelectQuery(*this)
        .addCol(tblUserExtraInfo::ueiPhoto)
        .where({ tblUserExtraInfo::uei_usrID, enuConditionOperator::Equal, _usrID })
        .one()
        .value(tblUserExtraInfo::ueiPhoto).toString().toLatin1();

    QString Mime = "image/png";
    QByteArray Image;
    if (Photo.size() && Photo.startsWith("data:image/")){
        Mime = Photo.split(';').first().mid(sizeof("data"));
        Image = QByteArray::fromBase64(Photo.mid(Photo.indexOf("base64,") + sizeof ("base64,") - 1));
    }

    return TAPI::RawData_t(Image, Mime);
}

/*****************************************************************\
|* UserExtraInfo *************************************************|
\*****************************************************************/
UserExtraInfo::UserExtraInfo() :
    clsTable (
        AAASchema,
        tblUserExtraInfo::Name,
        {///< ColName                               Type                            Validation      Default    UpBy   Sort  Filter Self  Virt   PK
            { tblUserExtraInfo::uei_usrID,          ORM_PRIMARYKEY_64 },
            { tblUserExtraInfo::ueiGender,          S(TAPI::enuUserGender::Type),   QFV,            TAPI::enuUserGender::NotExpressed,  UPOwner,false,false },
            { tblUserExtraInfo::ueiExtraInfo,       S(QString),                     QFV,            QNull,  UPOwner, false, false },
            { tblUserExtraInfo::ueiPhoto,           S(TAPI::Base64Image_t),         QFV,            QNull,  UPOwner, false, false },
            { tblUserExtraInfo::ueiIBAN,            S(TAPI::Sheba_t),               QFV.iban("IR"), QNull,  UPOwner, false, false },
            { tblUserExtraInfo::ueiEther,           S(TAPI::Ether_t),               QFV,            QNull,  UPOwner, false, false },
            { tblUserExtraInfo::ueiOAuthAccounts,   S(TAPI::JSON_t),                QFV,            QNull,  UPNone },
            { tblUserExtraInfo::ueiUpdatedBy_usrID, ORM_UPDATED_BY },
        },
        {///< Col                                   Reference Table                        ForeignCol           Rename          LeftJoin
            ORM_RELATION_OF_UPDATER(tblUserExtraInfo::ueiUpdatedBy_usrID),
        }
    )
{
}

bool UserExtraInfo::apiUPDATEPhoto(TAPI::JWT_t _JWT, TAPI::Base64Image_t _image){
    clsDACResult Result = this->execQuery(
                              "UPDATE " + this->Name
                              + QUERY_SEPARATOR
                              + "SET " + tblUserExtraInfo::ueiPhoto +" = ?, " +tblUserExtraInfo::ueiUpdatedBy_usrID + " = ?"
                              + QUERY_SEPARATOR
                              + "WHERE " + tblUserExtraInfo::uei_usrID + " = ?",
                              { _image, clsJWT(_JWT).usrID(), clsJWT(_JWT).usrID() }
        );

    return Result.numRowsAffected() > 0;
}

bool UserExtraInfo::apiUPDATESheba(TAPI::JWT_t _JWT, TAPI::Sheba_t _sheba){
    clsDACResult Result = this->execQuery(
                              "UPDATE " + this->Name
                              + QUERY_SEPARATOR
                              + "SET " + tblUserExtraInfo::ueiEther +" = ?, " +tblUserExtraInfo::ueiUpdatedBy_usrID + " = ?"
                              + QUERY_SEPARATOR
                              + "WHERE uei_usrID = ?",
                              { _sheba, clsJWT(_JWT).usrID(), clsJWT(_JWT).usrID() }
        );

    return Result.numRowsAffected() > 0;
}

bool UserExtraInfo::apiUPDATEEtherAdress(TAPI::JWT_t _JWT, TAPI::Ether_t _etherAddress){
    clsDACResult Result = this->execQuery(
                              "UPDATE " + this->Name
                              + QUERY_SEPARATOR
                              + "SET " + tblUserExtraInfo::ueiEther +" = ?, " +tblUserExtraInfo::ueiUpdatedBy_usrID + " = ?"
                              + QUERY_SEPARATOR
                              + "WHERE uei_usrID = ?",
                              { _etherAddress, clsJWT(_JWT).usrID(), clsJWT(_JWT).usrID() }
        );

    return Result.numRowsAffected() > 0;
}

} //namespace Targoman::API::AAA
