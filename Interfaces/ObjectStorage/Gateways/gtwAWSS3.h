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

#ifndef TARGOMAN_API_OBJECTSTORAGE_GTWAWSS3_H
#define TARGOMAN_API_OBJECTSTORAGE_GTWAWSS3_H

#ifdef TARGOMAN_API_AWS_S3

#include "intfObjectStorageGateway.h"
#include "Interfaces/Common/APIArgHelperMacros.hpp"
#include "Interfaces/Common/GenericTypes.h"

#include "libTargomanCommon/Macros.h"
#include "libTargomanCommon/CmdIO.h"
using namespace Targoman::Common;

//#include <aws/core/utils/memory/stl/AWSStreamFwd.h>
#include <aws/core/Aws.h>
//#include <aws/core/auth/AWSAuthSigner.h>
#include <aws/core/auth/AWSCredentials.h>
#include <aws/s3/S3Client.h>
#include <aws/s3/model/PutObjectRequest.h>
using namespace Aws;

/** installing AmazonAWS S3 from repo (recommended) ************************************************
15.2) zypper ar -no-gpgcheck https://download.opensuse.org/repositories/home:/targoman/openSUSE_Leap_15.2/home:targoman.repo
15.3) zypper ar -no-gpgcheck https://download.opensuse.org/repositories/home:/targoman/15.3/home:targoman.repo

zypper in aws-sdk-cpp-s3-devel
*/

/** installing AmazonAWS S3 from source ************************************************************
git clone --recurse-submodules https://github.com/aws/aws-sdk-cpp
mkdir aws-build.s3
cd aws-build.s3
cmake ../aws-sdk-cpp -DCMAKE_BUILD_TYPE=Release -DCMAKE_INSTALL_PREFIX=/usr -DCMAKE_PREFIX_PATH=/usr -DBUILD_ONLY="s3"
make
sudo make install
*/

namespace Targoman::API::ObjectStorage::Gateways {

struct __static_s3_initializer__ {
    static inline bool Initialized = false;
    static inline SDKOptions options;

    __static_s3_initializer__()
    {
        if (Initialized)
            return;

        TargomanDebug(0, "Initializing S3 API");

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

        TargomanDebug(0, "Shutting down S3 API");

        //Before the application terminates, the SDK must be shut down.
        ShutdownAPI(options);

        Initialized = false;
    }
};
static __static_s3_initializer__ __static_s3_initializer__internal;

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-variable"
namespace AWSS3MetaInfoJsonKey {
TARGOMAN_CREATE_CONSTEXPR(Bucket);
TARGOMAN_CREATE_CONSTEXPR(EndpointUrl);
TARGOMAN_CREATE_CONSTEXPR(SecretKey);
TARGOMAN_CREATE_CONSTEXPR(AccessKey);
}
#pragma GCC diagnostic pop

class gtwAWSS3 : public intfObjectStorageGateway
{
public:
    static bool storeFile(
        const TAPI::JSON_t &_metaInfo,
        const QString &_fileName,
        const QString &_fileUUID,
        const QString &_fullFileName
    );
};

} //namespace Targoman::API::ObjectStorage::Gateways

#endif // TARGOMAN_API_AWS_S3

#endif // TARGOMAN_API_OBJECTSTORAGE_GTWAWSS3_H
