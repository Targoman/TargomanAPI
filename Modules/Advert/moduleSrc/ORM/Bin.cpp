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

#include "Bin.h"
#include "Defs.hpp"

#include "Interfaces/ORM/APIQueryBuilders.h"

namespace Targoman {
namespace API {
namespace Advertisement {
using namespace ORM;

QVariant Bin::apiGET(GET_METHOD_ARGS_IMPL_APICALL)
{
    Authorization::checkPriv(_JWT, this->privOn(EHTTP_GET,this->moduleBaseName()));

    return Targoman::API::Query::SelectOne(*this, GET_METHOD_CALL_ARGS_INTERNAL_CALL); //, ExtraFilters, CACHE_TIME);

//    return query.one();

    //    return this->selectFromTable({}, {}, GET_METHOD_CALL_ARGS_APICALL);
}

Bin::Bin() :
    clsTable(AdvertSchema,
              tblBin::Name,
              { ///<ColName                   Type                    Validation                                Default   UpBy   Sort  Filter Self  Virt   PK
                {tblBin::binID,               ORM_PRIMARY_KEY32},
                {tblBin::binType,             S(TAPI::enuAdvertType::Type), QFV,                                TAPI::enuAdvertType::Text, UPOwner},
                {tblBin::binTitle,            S(QString),             QFV.unicodeAlNum(true, ".,:،-_*()[] "),   QRequired, UPOwner},
                {tblBin::binDesc,             S(QString),             QFV.unicodeAlNum(true, ".,:،-_*()[] "),   QNull,     UPOwner},
                {tblBin::binPrettyURL,        S(QString),             QFV.url(),                                QRequired, UPOwner},
                {tblBin::binURL,              S(QString),             QFV.url(),                                QRequired, UPOwner},
                {tblBin::binShown,            S(quint64),             QFV.integer(),                            0,         UPNone},
                {tblBin::binClicks,           S(quint64),             QFV.integer(),                            0,         UPNone},
                {tblBin::binCreatedBy_usrID,  ORM_CREATED_BY},
                {tblBin::binCreationDateTime, ORM_CREATED_ON},
                {tblBin::binUpdatedBy_usrID,  ORM_UPDATED_BY},
                {tblBin::binStatus,           S(TAPI::enuAuditableStatus::Type),QFV,                            TAPI::enuAuditableStatus::Pending, UPStatus},
              },
              { ///< Col                        Reference Table                 ForeignCol         Rename      LeftJoin
                {tblBin::binID,                 R(AdvertSchema,tblBanners::Name),  tblBanners::bnrID, "Banner_",  true},
                ORM_RELATION_OF_CREATOR(tblBin::binCreatedBy_usrID),
                ORM_RELATION_OF_UPDATER(tblBin::binUpdatedBy_usrID),
              })
{
}

Banners::Banners() :
    clsTable("Advert",
              tblBanners::Name,
              { ///<ColName                       Type                          Validation                 Default   UpBy   Sort  Filter Self  Virt   PK
                {tblBanners::bnrImage,            S(TAPI::Base64Image_t),       QFV,                       QInvalid, UPOwner,false,false},
                {tblBanners::bnrSize,             S(TAPI::enuBannerSizes::Type),QFV,                       QInvalid, UPOwner },
                {tblBanners::bnrUpdatedBy_usrID,  ORM_UPDATED_BY},
              },
              {///< Col                           Reference Table             ForeignCol      Rename      LeftJoin
                ORM_RELATION_OF_UPDATER(tblBanners::bnrUpdatedBy_usrID),
              }
              )
{
}
}
}
}
