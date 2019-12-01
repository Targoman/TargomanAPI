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
#include "Helpers/AAA/AAA.hpp"
#include "Defs.hpp"

namespace Targoman {
namespace API {
namespace Advertisement {
using namespace QHttp;

void Bin::init()
{;}

QVariant Bin::apiGET(GET_METHOD_ARGS_IMPL)
{
    Authorization::checkPriv(_JWT,{"Advert:Bin:CRUD~0100"});

    return this->selectFromTable(AAADACInstance(), {}, {}, GET_METHOD_CALL_ARGS);
}

Bin::Bin() :
    clsTable("Advert",
              "tblBin",
              { ///<ColName             Type            Validation                      RO   Sort  Filter  PK
                {"binID",               T(quint32),             QFV.integer().minValue(1),      true,  true,  true, true},
                {"binType",             T(Targoman::API::enuAdvertType::Type)},
                {"binTitle",            T(QString),             QFV.unicodeAlNum(true, ":،-_*()")},
                {"binDesc",             T(QString),             QFV.unicodeAlNum(true, ":،-_*()")},
                {"binPrettyURL",        T(QString),             QFV.url()},
                {"binURL",              T(QString),             QFV.url()},
                {"binShown",            T(quint64),             QFV.integer(),                  true},
                {"binClicks",           T(quint64),             QFV.integer(),                  true},
                {"binCreatedBy_usrID",  T(quint32),             QFV.integer().minValue(1),      true},
                {"binCreationDateTime", T(QHttp::DateTime_t),   QFV,                            true},
                {"binUpdatedBy_usrID",  T(quint32),             QFV.integer().minValue(1)},
                {"binStatus",           T(Targoman::API::enuAuditableStatus::Type)},
              },
              { ///< Col                Reference Table        ForeignCol   Rename      LeftJoin
                {"adbCreatedBy_usrID",  "AAA.tblUser",         "usrID",     "Creator_", true},
                {"adbUpdatedBy_usrID",  "AAA.tblUser",         "usrID",     "Updater_", true},
                {"adbUpdatedBy_usrID",  "Advert.tblBanners",   "bnr_binID", "",         true},
              })
{
    this->registerMyRESTAPIs();
}

Banners::Banners() :
    clsTable("Advert",
              "tblBanners",
              { ///<ColName             Type                Validation        RO   Sort  Filter  PK
                {"bnrImage",            T(QHttp::Base64_t), QFV,             false,false,false},
                {"bnrSize",             T(Targoman::API::enuBannerSizes::Type)},
                {"bnrUpdatedBy_usrID",  T(quint32),         QFV.integer().minValue(1)},
              },
              {
                {"bnrUpdatedBy_usrID",  "AAA.tblUser",         "usrID",     "Updater_", true},
              }
              )
{
    this->registerMyRESTAPIs();
}
}
}
}
