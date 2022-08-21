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

#ifndef TARGOMAN_API_MODULES_ADVERT_ORM_BIN_H
#define TARGOMAN_API_MODULES_ADVERT_ORM_BIN_H

#include "Interfaces/AAA/AAA.hpp"
#include "Interfaces/API/intfSQLBasedModule.h"
#include "Defs.hpp"

namespace Targoman::API::AdvertModule {

//structures and enumes goes here

namespace ORM {

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-variable"
namespace tblBanners {
    constexpr char Name[] = "tblBanners";

    namespace Fields {
        TARGOMAN_CREATE_CONSTEXPR(bnrID);
        TARGOMAN_CREATE_CONSTEXPR(bnrImage);
        TARGOMAN_CREATE_CONSTEXPR(bnrSize);
        TARGOMAN_CREATE_CONSTEXPR(bnrUpdatedBy_usrID);
    }

    namespace Relation {
//        constexpr char AAA[] = "aaa";
    }

    namespace Private {
        const QList<clsORMField> ORMFields = {
            ///<ColName                       Type                          Validation                  Default   UpBy   Sort  Filter Self  Virt   PK
                { Fields::bnrImage,            S(TAPI::Base64Image_t),       QFV,                        QInvalid, UPOwner,false,false},
                { Fields::bnrSize,             S(Targoman::API::AdvertModule::enuBannerSize::Type), QFV,QInvalid, UPOwner },
                { Fields::bnrUpdatedBy_usrID,  ORM_UPDATED_BY},
            };

        const QList<stuRelation> Relations = {
            ///Col                           Reference Table             ForeignCol      Rename      LeftJoin
                ORM_RELATION_OF_UPDATER(Fields::bnrUpdatedBy_usrID),
            };

        const QList<stuDBIndex> Indexes = {
        };

    } //namespace Private

    TAPI_DEFINE_STRUCT(DTO,
        SF_QString                  (bnrImage),
        SF_NULLABLE_Enum            (bnrSize, Targoman::API::AdvertModule::enuBannerSize),
        SF_ORM_UPDATED_BY           (bnrUpdatedBy_usrID)
    );
}

namespace tblBin {
    constexpr char Name[] = "tblBin";

    namespace Fields {
        TARGOMAN_CREATE_CONSTEXPR(binID);
        TARGOMAN_CREATE_CONSTEXPR(binType);
        TARGOMAN_CREATE_CONSTEXPR(binTitle);
        TARGOMAN_CREATE_CONSTEXPR(binDesc);
        TARGOMAN_CREATE_CONSTEXPR(binPrettyURL);
        TARGOMAN_CREATE_CONSTEXPR(binURL);
        TARGOMAN_CREATE_CONSTEXPR(binShown);
        TARGOMAN_CREATE_CONSTEXPR(binClicks);
        TARGOMAN_CREATE_CONSTEXPR(binCreatedBy_usrID);
        TARGOMAN_CREATE_CONSTEXPR(binCreationDateTime);
        TARGOMAN_CREATE_CONSTEXPR(binUpdatedBy_usrID);
        TARGOMAN_CREATE_CONSTEXPR(binStatus);
    }

    namespace Relation {
//        constexpr char AAA[] = "aaa";
    }

    namespace Private {
        const QList<clsORMField> ORMFields = {
            ///<ColName                   Type                    Validation                                Default   UpBy   Sort  Filter Self  Virt   PK
                { Fields::binID,               ORM_PRIMARYKEY_32},
                { Fields::binType,             S(Targoman::API::AdvertModule::enuAdvertType::Type), QFV,                                Targoman::API::AdvertModule::enuAdvertType::Text, UPOwner},
                { Fields::binTitle,            S(QString),             QFV.unicodeAlNum(true, ".,:،-_*()[] "),   QRequired, UPOwner},
                { Fields::binDesc,             S(QString),             QFV.unicodeAlNum(true, ".,:،-_*()[] "),   QNull,     UPOwner},
                { Fields::binPrettyURL,        S(QString),             QFV.url(),                                QRequired, UPOwner},
                { Fields::binURL,              S(QString),             QFV.url(),                                QRequired, UPOwner},
                { Fields::binShown,            S(quint64),             QFV.integer(),                            0,         UPNone},
                { Fields::binClicks,           S(quint64),             QFV.integer(),                            0,         UPNone},
                { Fields::binStatus,           ORM_STATUS_FIELD(TAPI::enuAuditableStatus, TAPI::enuAuditableStatus::Pending) },
                { Fields::binCreationDateTime, ORM_CREATED_ON },
                { Fields::binCreatedBy_usrID,  ORM_CREATED_BY },
                { Fields::binUpdatedBy_usrID,  ORM_UPDATED_BY },
            };

        const QList<stuRelation> Relations = {
            ///Col                        Reference Table                 ForeignCol         Rename      LeftJoin
                { Fields::binID,                 R(AdvertSchema,tblBanners::Name),  tblBanners::Fields::bnrID, "Banner_",  true},
                ORM_RELATION_OF_CREATOR(Fields::binCreatedBy_usrID),
                ORM_RELATION_OF_UPDATER(Fields::binUpdatedBy_usrID),
            };

        const QList<stuDBIndex> Indexes = {
        };

    } //namespace Private

    TAPI_DEFINE_STRUCT(DTO,
        SF_ORM_PRIMARYKEY_32        (binID),
        SF_Enum                     (binType, Targoman::API::AdvertModule::enuAdvertType, Targoman::API::AdvertModule::enuAdvertType::Text),
        SF_QString                  (binTitle),
        SF_QString                  (binDesc),
        SF_QString                  (binPrettyURL),
        SF_QString                  (binURL),
        SF_quint64                  (binShown),
        SF_quint64                  (binClicks),
        SF_ORM_STATUS_FIELD         (binStatus, TAPI::enuAuditableStatus, TAPI::enuAuditableStatus::Pending),
        SF_ORM_CREATED_ON           (binCreationDateTime),
        SF_ORM_CREATED_BY           (binCreatedBy_usrID),
        SF_ORM_UPDATED_BY           (binUpdatedBy_usrID)
    );
}
#pragma GCC diagnostic pop

class Banners : public intfSQLBasedModule
{
    Q_OBJECT
    TARGOMAN_API_SUBMODULE_DEFINE(Advert, Banners)
};

class Bin : public intfSQLBasedModule
{
    Q_OBJECT
    TARGOMAN_API_SUBMODULE_DEFINE(Advert, Bin)

private slots:
    QVariant ORMGET("Get Bin information")
};

} //namespace ORM
} //namespace Targoman::API::AdvertModule

#endif // TARGOMAN_API_MODULES_ADVERT_ORM_BIN_H
