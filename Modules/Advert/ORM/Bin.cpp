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

#include "Bin.h"
#include "Defs.hpp"

namespace Targoman {
namespace API {
namespace Advertisement {
using namespace ORM;

QVariant Bin::apiGET(GET_METHOD_ARGS_IMPL)
{
    Authorization::checkPriv(_JWT,{"Advert:Bin:CRUD~0100"});

    return this->selectFromTable({}, {}, GET_METHOD_CALL_ARGS);
}

Bin::Bin() :
    clsTable(AdvertSchema,
              "tblBin",
              { ///<ColName             Type            Validation                      RO   Sort  Filter Self  Virt   PK
                {"binID",               S(quint32),             QFV.integer().minValue(1),      ORM_PRIMARY_KEY},
                {"binType",             S(Targoman::API::enuAdvertType::Type)},
                {"binTitle",            S(QString),             QFV.unicodeAlNum(true, ":،-_*()")},
                {"binDesc",             S(QString),             QFV.unicodeAlNum(true, ":،-_*()")},
                {"binPrettyURL",        S(QString),             QFV.url()},
                {"binURL",              S(QString),             QFV.url()},
                {"binShown",            S(quint64),             QFV.integer(),                  true},
                {"binClicks",           S(quint64),             QFV.integer(),                  true},
                {"binCreatedBy_usrID",  S(quint32),             QFV.integer().minValue(1),      true},
                {"binCreationDateTime", S(QHttp::DateTime_t),   QFV,                            true},
                {"binUpdatedBy_usrID",  S(quint32),             QFV.integer().minValue(1)},
                {"binStatus",           S(Targoman::API::enuAuditableStatus::Type)},
              },
              { ///< Col                Reference Table        ForeignCol   Rename      LeftJoin
                {"adbCreatedBy_usrID",  "AAA.tblUser",         "usrID",     "Creator_", true},
                {"adbUpdatedBy_usrID",  "AAA.tblUser",         "usrID",     "Updater_", true},
                //{"adbUpdatedBy_usrID",  "Advert.tblBanners",   "bnr_binID", "",         true},
              })
{
}

Banners::Banners() :
    clsTable("Advert",
              "tblBanners",
              { ///<ColName             Type                Validation        RO   Sort  Filter Self  Virt   PK
                {"bnrImage",            S(QHttp::Base64_t), QFV,             false,false,false},
                {"bnrSize",             S(Targoman::API::enuBannerSizes::Type)},
                {"bnrUpdatedBy_usrID",  S(quint32),         QFV.integer().minValue(1)},
              },
              {
                {"bnrUpdatedBy_usrID",  "AAA.tblUser",         "usrID",     "Updater_", true},
              }
              )
{
}
}
}
}
