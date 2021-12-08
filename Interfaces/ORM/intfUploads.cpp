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

#include "intfUploads.h"
#include "Interfaces/AAA/Authorization.h"
#include "Interfaces/AAA/AAADefs.hpp"
#include "Interfaces/AAA/clsJWT.hpp"
using namespace Targoman::API::AAA;

TAPI_REGISTER_TARGOMAN_ENUM(Targoman::API::ORM, enuUploadStatus);
TAPI_REGISTER_TARGOMAN_ENUM(Targoman::API::ORM, enuS3Provider);
ENUM_CONFIGURABLE_IMPL(Targoman::API::ORM::enuS3Provider)

namespace Targoman::API::ORM {

intfUploads::intfUploads(
        const QString &_schema,
        const QString &_name
    ) :
    intfSQLBasedModule(
        _schema,
        _name,
        tblUploads::Name,
        {///< ColName                                   Type                    Validation                  Default     UpBy    Sort  Filter Self  Virt   PK
            { tblUploads::uplID,                        ORM_PRIMARYKEY_64 },
            { tblUploads::uplURL,                       S(QString),             QFV,                        QRequired,  UPNone },
            { tblUploads::uplFileName,                  S(QString),             QFV,                        QRequired,  UPNone },
            { tblUploads::uplSize,                      S(quint64),             QFV.integer().minValue(1),  QRequired,  UPNone },
            { tblUploads::uplMimeType,                  S(QString),             QFV,                        QNull,      UPNone },
            { tblUploads::uplTempFullFileName,          S(QString),             QFV,                        QRequired,  UPNone },
            { tblUploads::uplUploadLastErrorMessage,    S(QString),             QFV,                        QNull,      UPNone },
            { tblUploads::uplStatus,                    ORM_STATUS_FIELD(Targoman::API::ORM::enuUploadStatus, Targoman::API::ORM::enuUploadStatus::Queued) },
            { tblUploads::uplCreationDateTime,          ORM_CREATED_ON },
            { tblUploads::uplCreatedBy_usrID,           ORM_CREATED_BY },
            { tblUploads::uplUpdatedBy_usrID,           ORM_UPDATED_BY },
        },
        {///< Col                        Reference Table              ForeignCol       Rename     LeftJoin
            ORM_RELATION_OF_CREATOR(tblUploads::uplCreatedBy_usrID),
            ORM_RELATION_OF_UPDATER(tblUploads::uplUpdatedBy_usrID),
        }
    )
{}

/*
QVariant intfUploads::apiGET(GET_METHOD_ARGS_IMPL_APICALL)
{
//    Authorization::checkPriv(_JWT, { this->ModuleName + ":Uploads:CRUD~0100" });
    Authorization::checkPriv(_JWT, this->privOn(EHTTP_GET, this->moduleBaseName()));

    return this->Select(*this, GET_METHOD_CALL_ARGS_INTERNAL_CALL);
}
*/

stuSaveFileResult intfUploads::apiPUTsave(
        const TAPI::JWT_t &_JWT,
        QString _fileName,
        const QString &_base64Content
    )
{
    Authorization::checkPriv(_JWT, this->privOn(EHTTP_PUT, this->moduleBaseName()));

    stuSaveFileResult SaveFileResult = ObjectStorageHelper::saveFile(
                                           this,
                                           clsJWT(_JWT).usrID(),
                                           _fileName,
                                           _base64Content
                                           );
    return SaveFileResult;
}

} //namespace Targoman::API::ORM
