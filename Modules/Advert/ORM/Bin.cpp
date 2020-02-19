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

#include "Bin.h"
#include "Defs.hpp"
#include "Modules/Account/ORM/User.h"

namespace Targoman {
namespace API {
namespace Advertisement {
using namespace ORM;

QVariant Bin::apiGET(GET_METHOD_ARGS_IMPL)
{
    Authorization::checkPriv(_JWT, this->privOn(EHTTP_GET,this->moduleBaseName()));
    return this->selectFromTable({}, {}, GET_METHOD_CALL_ARGS);
}

Bin::Bin() :
    clsTable(AdvertSchema,
              tblBin::Name,
              { ///<ColName                   Type                    Validation                                Default   UpBy   Sort  Filter Self  Virt   PK
                {tblBin::binID,               S(quint32),             QFV.integer().minValue(1),                ORM_PRIMARY_KEY},
                {tblBin::binType,             S(TAPI::enuAdvertType::Type), QFV,                                TAPI::enuAdvertType::Text, UPAll},
                {tblBin::binTitle,            S(QString),             QFV.unicodeAlNum(true, ".,:،-_*()[] "),   QInvalid, UPAll},
                {tblBin::binDesc,             S(QString),             QFV.unicodeAlNum(true, ".,:،-_*()[] "),   QNull,    UPAll},
                {tblBin::binPrettyURL,        S(QString),             QFV.url(),                                QInvalid, UPAll},
                {tblBin::binURL,              S(QString),             QFV.url(),                                QInvalid, UPAll},
                {tblBin::binShown,            S(quint64),             QFV.integer(),                            0,        UPNone},
                {tblBin::binClicks,           S(quint64),             QFV.integer(),                            0,        UPNone},
                {tblBin::binCreatedBy_usrID,  S(quint32),             QFV.integer().minValue(1),                QNull,    UPNone},
                {tblBin::binCreationDateTime, S(QHttp::DateTime_t),   QFV,                                      QNull,    UPNone},
                {tblBin::binUpdatedBy_usrID,  S(quint32),             QFV.integer().minValue(1),                QNull,    UPNone},
                {tblBin::binStatus,           S(TAPI::enuAuditableStatus::Type),QFV,                            TAPI::enuAuditableStatus::Pending, UPAll},
              },
              { ///< Col                        Reference Table                 ForeignCol         Rename      LeftJoin
                {tblBin::binID,                 R(AdvertSchema,tblUser::Name),  tblBanners::bnrID, "Banner_",  true},
                {tblBin::binCreatedBy_usrID,    R(AAASchema,tblUser::Name),     tblUser::usrID,    "Creator_", true},
                {tblBin::binUpdatedBy_usrID,    R(AAASchema,tblUser::Name),     tblUser::usrID,    "Updater_", true},
              })
{
}

Banners::Banners() :
    clsTable("Advert",
              tblBanners::Name,
              { ///<ColName                       Type                    Validation             Default   UpBy   Sort  Filter Self  Virt   PK
                {tblBanners::bnrImage,            S(QHttp::Base64_t),     QFV,                   QInvalid, UPAll,false,false},
                {tblBanners::bnrSize,             S(TAPI::enuBannerSizes::Type), QFV,            QInvalid, UPAll },
                {tblBanners::bnrUpdatedBy_usrID,  S(quint32),         QFV.integer().minValue(1), QNull,    UPNone},
              },
              {///< Col                             Reference Table             ForeignCol      Rename      LeftJoin
                {tblBanners::bnrUpdatedBy_usrID,    R(AAASchema,tblUser::Name), tblUser::usrID, "Updater_", true},
              }
              )
{
}
}
}
}
