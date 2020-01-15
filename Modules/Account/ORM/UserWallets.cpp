/******************************************************************************
#   TargomanAPI: REST API for Targoman
#
#   Copyright 2014-2019 by Targoman Intelligent Processing <http://tip.co.ir>
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
#include "Helpers/AAA/AAA.hpp"

namespace Targoman {
namespace API {
namespace AAA {
using namespace QHttp;

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

bool UserWallets::apiUPDATEdefaultWallet(QHttp::JWT_t _JWT, quint64 _walID){

}

UserWallets::UserWallets() :
    clsTable("AAA",
              "tblUserWallets",
              { ///<ColName         Type                        Validation                          Default    RO   Sort  Filter Self  Virt   PK
                {"walID",           S(quint64),                 QFV.integer().minValue(1),          ORM_PRIMARY_KEY},
                {"wal_usrID",       S(quint32),                 QFV.integer().minValue(1),          QInvalid,   true},
                {"walName",         S(quint32),                 QFV.unicodeAlNum().maxLenght(100),  QInvalid},
                {"walDefault",      S(bool),                    QFV,                                false},
                {"walMinBalance",   S(qint64),                  QFV,                                0,         false,false,false},
                {"walLastBalance",  S(qint64),                  QFV,                                0,          true,false,false},
                {"walSumIncome",    S(qint64),                  QFV,                                0,          true,false,false},
                {"walSumExpenses",  S(qint64),                  QFV,                                0,          true,false,false},
                {"walStatus",       S(Targoman::API::enuUserWalletStatus::Type), QFV,               Targoman::API::enuUserWalletStatus::Active},
              },
              { ///< Col       Reference Table             ForeignCol    Rename   LeftJoin
                {"wal_usrID",  "AAA.tblUser",              "usrID"},
              })
{
    QHTTP_REGISTER_TARGOMAN_ENUM(Targoman::API::enuUserWalletStatus);
    this->registerMyRESTAPIs();
}

}
}
}

