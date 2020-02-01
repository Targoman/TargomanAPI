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
 @author S. Mehran M. Ziabary <ziabary@targoman.com>
 */

#include "UserWallets.h"

namespace Targoman {
namespace API {
namespace AAA {

using namespace ORM;

void UserWallets::init()
{;}

QVariant UserWallets::apiGET(GET_METHOD_ARGS_IMPL)
{
    if(Authorization::hasPriv(_JWT, this->privOn(EHTTP_GET,this->moduleName())) == false)
        this->setSelfFilters({{"wal_usrID", clsJWT(_JWT).usrID()}}, _EXTRAPATH, _ORMFILTERS, _filters);

    return this->selectFromTable(AAADACInstance(), {}, {}, GET_METHOD_CALL_ARGS);
}

bool UserWallets::apiDELETE(DELETE_METHOD_ARGS_IMPL)
{
    if(Authorization::hasPriv(_JWT, this->privOn(EHTTP_DELETE,this->moduleName())) == false){
        _ORMFILTERS.insert("walDefault", 0);
        this->setSelfFilters({{"wal_usrID", clsJWT(_JWT).usrID()}}, _EXTRAPATH, _ORMFILTERS);
    }
    return this->deleteByPKs(AAADACInstance(), DELETE_METHOD_CALL_ARGS);
}

bool UserWallets::apiUPDATE(UPDATE_METHOD_ARGS_IMPL)
{
    Authorization::checkPriv(_JWT, this->privOn(EHTTP_PATCH,this->moduleName()));
    return this->update(AAADACInstance(), UPDATE_METHOD_CALL_ARGS);
}

quint64 UserWallets::apiCREATE(CREATE_METHOD_ARGS_IMPL)
{
    if(Authorization::hasPriv(_JWT, this->privOn(EHTTP_DELETE,this->moduleName())) == false){
        _ORMFILTERS.insert("walDefault", 0);
        this->setSelfFilters({{"wal_usrID", clsJWT(_JWT).usrID()}}, {}, _ORMFILTERS);
    }

    return this->create(AAADACInstance(), CREATE_METHOD_CALL_ARGS).toUInt();
}

bool UserWallets::apiUPDATEdefaultWallet(TAPI::JWT_t _JWT, quint64 _walID){
    bool IsPrivileged = Authorization::hasPriv(_JWT, this->privOn(EHTTP_PATCH,this->moduleName()));
    clsDACResult Result = AAADACInstance().execQuery(
                              "",
                              "UPDATE " + this->Name
                              + QUERY_SEPARATOR
                              + "SET walDefault = 1, ueiUpdatedBy_usrID = :usrID"
                              + QUERY_SEPARATOR
                              + "WHERE "
                              + (IsPrivileged ? "TRUE" : "wal_usrID = ")
                              + " AND walID = ?",
                              (IsPrivileged ?
                                   QVariantList({ clsJWT(_JWT).usrID(), _walID }) :
                                   QVariantList({ clsJWT(_JWT).usrID(), clsJWT(_JWT).usrID(), _walID })
                              )
        );

    return Result.numRowsAffected() > 0;
}

bool UserWallets::apiCREATEtransfer(TAPI::JWT_t _JWT,
                                    const QString& _destLogin,
                                    quint32 _amount,
                                    const TAPI::MD5_t& _pass,
                                    const QString& _salt,
                                    quint64 _walID){
    return static_cast<quint32>(AAADACInstance().callSP ("","AAA.sp_CREATE_transfer", {
                                                             /*{"iBy", Type},
                                                             {"iLogin", _emailOrMobile},
                                                             {"iPass", _pass},
                                                             {"iRole", _role},
                                                             {"iIP", _REMOTE_IP},
                                                             {"iName", _name.isEmpty()? QVariant() : _name},
                                                             {"iFamily", _family.isEmpty()? QVariant() : _family},
                                                             {"iSpecialPrivs", _specialPrivs.isEmpty()? QVariant() : _specialPrivs},
                                                             {"iMaxSessions", _maxSessions},*/
                                                         }).spDirectOutputs().value("oUserID").toDouble());
}

bool UserWallets::apiCREATEdeposit(TAPI::JWT_t _JWT, quint32 _amount, quint64 _walID){
    return static_cast<quint32>(AAADACInstance().callSP ("","AAA.sp_CREATE_transfer", {
                                                             /*{"iBy", Type},
                                                             {"iLogin", _emailOrMobile},
                                                             {"iPass", _pass},
                                                             {"iRole", _role},
                                                             {"iIP", _REMOTE_IP},
                                                             {"iName", _name.isEmpty()? QVariant() : _name},
                                                             {"iFamily", _family.isEmpty()? QVariant() : _family},
                                                             {"iSpecialPrivs", _specialPrivs.isEmpty()? QVariant() : _specialPrivs},
                                                             {"iMaxSessions", _maxSessions},*/
                                                         }).spDirectOutputs().value("oUserID").toDouble());
}

UserWallets::UserWallets() :
    clsTable("AAA",
              "tblUserWallets",
              { ///<ColName             Type                        Validation                          Default    RO   Sort  Filter Self  Virt   PK
                {"walID",               S(quint64),                 QFV.integer().minValue(1),          ORM_PRIMARY_KEY},
                {"wal_usrID",           S(quint32),                 QFV.integer().minValue(1),          QInvalid,   true},
                {"walName",             S(quint32),                 QFV.unicodeAlNum().maxLenght(100),  QInvalid},
                {"walDefault",          S(bool),                    QFV,                                false},
                {"walMinBalance",       S(qint64),                  QFV,                                0,         false,false,false},
                {"walLastBalance",      S(qint64),                  QFV,                                0,          true,false,false},
                {"walSumIncome",        S(qint64),                  QFV,                                0,          true,false,false},
                {"walSumExpenses",      S(qint64),                  QFV,                                0,          true,false,false},
                {"walCreatedBy_usrID",  S(quint32),                 QFV.integer().minValue(1),          QInvalid,   true},
                {"walCreationDateTime", S(TAPI::DateTime_t),        QFV,                                QNull,      true},
                {"walUpdatedBy_usrID",  S(quint32),                 QFV.integer().minValue(1),          QNull,      true},
                {"walStatus",           S(TAPI::enuUserWalletStatus::Type), QFV,                        TAPI::enuUserWalletStatus::Active},
              },
              { ///< Col                Reference Table     ForeignCol    Rename   LeftJoin
                {"wal_usrID",           "AAA.tblUser",      "usrID"},
                {"walCreatedBy_usrID",  "AAA.tblUser",      "usrID",     "Creator_", true},
                {"walUpdatedBy_usrID",  "AAA.tblUser",      "usrID",     "Updater_", true}
              })
{
    TAPI_REGISTER_TARGOMAN_ENUM(TAPI::enuUserWalletStatus);
}

}
}
}

