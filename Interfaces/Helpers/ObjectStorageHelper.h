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
 * @author S.Mehran M.Ziabary <ziabary@targoman.com>
 * @author Kambiz Zandi <kambizzandi@gmail.com>
 */

#ifndef TARGOMAN_API_OBJECTSTORAGEHELPER_H
#define TARGOMAN_API_OBJECTSTORAGEHELPER_H

#include <QString>
#include "libTargomanCommon/CmdIO.h"
#include "Interfaces/Common/APIArgHelperMacros.hpp"
#include "Interfaces/Common/GenericTypes.h"
#include <QJsonObject>

#include <aws/core/Aws.h>
#include <aws/s3/S3Client.h>
using namespace Aws;

namespace Targoman::API::ORM {
class intfUploadFiles;
}

namespace Targoman::API::Helpers {

namespace Private {

struct __static_s3_initializer__ {
    static inline bool Initialized = false;
    static inline SDKOptions options;

    __static_s3_initializer__()
    {
        if (Initialized)
            return;

        TargomanDebug(0, "Initialize S3 API");

        //The Aws::SDKOptions struct contains SDK configuration options.
        //An instance of Aws::SDKOptions is passed to the Aws::InitAPI and
        //Aws::ShutdownAPI methods. The same instance should be sent to both methods.
        options.loggingOptions.logLevel = Utils::Logging::LogLevel::Debug;

        //The AWS SDK for C++ must be initialized by calling Aws::InitAPI.
        InitAPI(options);

        Initialized = true;
    }
    ~__static_s3_initializer__()
    {
        if (Initialized == false)
            return;

        TargomanDebug(0, "Shutdown S3 API");

        //Before the application terminates, the SDK must be shut down.
        ShutdownAPI(options);

        Initialized = false;
    }
};
static __static_s3_initializer__ __static_s3_initializer__internal;

} //namespace Private

TAPI_DEFINE_VARIANT_ENABLED_STRUCT(stuSaveFileResult,
    SF_QString(FullFileUrl),
    SF_QString(MimeType),
    SF_quint8(IsUploaded),
    SF_quint64(uplID)
);

class ObjectStorageHelper
{
public:
    static Targoman::API::Helpers::stuSaveFileResult saveFile(
            Targoman::API::ORM::intfUploadFiles &_uploadFiles,
            const quint64 _currentUserID,
            const TAPI::stuFileInfo &_file
            );

//    static Targoman::API::Helpers::stuSaveFileResult saveFile(
//            Targoman::API::ORM::intfUploadFiles *_uploadFiles,
//            const quint64 _currentUserID,
//            QString &_fileName,
//            const QString &_base64Content
//            );

private:
    static bool uploadFileToS3(
            const QString &_fileName,
            const QString &_fullFileName,
            const QString &_bucket,
            const QString &_region = {}
            );
};

} //namespace Targoman::API::Helpers

TAPI_DECLARE_METATYPE(Targoman::API::Helpers::stuSaveFileResult) // -> TAPI_REGISTER_METATYPE() in ObjectStorageHelper.cpp

#endif // TARGOMAN_API_OBJECTSTORAGEHELPER_H
