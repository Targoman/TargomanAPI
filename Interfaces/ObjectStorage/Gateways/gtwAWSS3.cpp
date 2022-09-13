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

#include "gtwAWSS3.h"

#ifdef TARGOMAN_API_ENABLE_AWS_S3

#include <string>
#include <fstream>
#include "QHttp/qhttpfwd.hpp"
using namespace qhttp;

//#include <aws/core/utils/memory/stl/AWSStreamFwd.h>
#include <aws/core/Aws.h>
//#include <aws/core/auth/AWSAuthSigner.h>
#include <aws/core/auth/AWSCredentials.h>
#include <aws/s3/S3Client.h>
#include <aws/s3/model/PutObjectRequest.h>
using namespace Aws;

using namespace Targoman::API::Common;

namespace Targoman::API::ObjectStorage::Gateways {

void gtwAWSS3::InitS3()
{
    static bool Initialized = false;
    static SDKOptions options;

    if (Initialized)
        return;

    TargomanDebug(0, "Initializing S3 API");

    //The Aws::SDKOptions struct contains SDK configuration options.
    //An instance of Aws::SDKOptions is passed to the Aws::InitAPI and
    //Aws::ShutdownAPI methods. The same instance should be sent to both methods.
#ifdef QT_DEBUG
    options.loggingOptions.logLevel = Utils::Logging::LogLevel::Trace;
#else
    options.loggingOptions.logLevel = Utils::Logging::LogLevel::Off;
#endif

    //The AWS SDK for C++ must be initialized by calling Aws::InitAPI.
    InitAPI(options);

    Initialized = true;
}

bool gtwAWSS3::storeFile(
//    const TAPI::JSON_t &_metaInfo,
    const ORM::tblUploadGateways::DTO &_uploadGateway,
    const QString &_fullFileName,
    const QString &_path,
    const QString &_fileName
) {
    TargomanLogDebug(5, "*** gtwAWSS3::storeFile: BEGIN");

    gtwAWSS3::InitS3();

    TargomanLogDebug(5, "*** gtwAWSS3::storeFile: ["
                     << "_fullFileName:" << _fullFileName
                     << ",_path:" << _path
                     << ",_fileName:" << _fileName
                     << "]"
                     );

    TargomanLogDebug(5, "*** gtwAWSS3::storeFile: #" << __LINE__ << "QFile::exists");
    if (QFile::exists(_fullFileName) == false)
        throw exTargomanBase(QString("File %1 not found").arg(_fullFileName));

//    QString Bucket = _uploadGateway.ugwMetaInfo[AWSS3MetaInfoJsonKey::Bucket].toString();
//    QString EndpointUrl = _uploadGateway.ugwMetaInfo[AWSS3MetaInfoJsonKey::EndpointUrl].toString();

    TargomanLogDebug(5, "*** gtwAWSS3::storeFile: #" << __LINE__);
    QVariantMap MetaInfo = _uploadGateway.ugwMetaInfo.object().toVariantMap();

    TargomanLogDebug(5, "*** gtwAWSS3::storeFile: #" << __LINE__);
    if (MetaInfo.contains(AWSS3MetaInfoJsonKey::AccessKey) == false)
        throw exTargomanBase(QString("%1 not defined in gateway information").arg(AWSS3MetaInfoJsonKey::AccessKey));
    QString AccessKey = MetaInfo[AWSS3MetaInfoJsonKey::AccessKey].toString();

    TargomanLogDebug(5, "*** gtwAWSS3::storeFile: #" << __LINE__);
    if (MetaInfo.contains(AWSS3MetaInfoJsonKey::SecretKey) == false)
        throw exTargomanBase(QString("%1 not defined in gateway information").arg(AWSS3MetaInfoJsonKey::SecretKey));
    QString SecretKey = MetaInfo[AWSS3MetaInfoJsonKey::SecretKey].toString();

    TargomanLogDebug(5, "*** gtwAWSS3::storeFile: #" << __LINE__);
    Aws::Auth::AWSCredentials AWSCredentials(AccessKey.toStdString().c_str(), SecretKey.toStdString().c_str());

    TargomanLogDebug(5, "*** gtwAWSS3::storeFile: #" << __LINE__);
    Aws::Client::ClientConfiguration S3ClientConfig;

//    if (_region.isEmpty() == false)
//        S3ClientConfig.region = _region.toStdString();

    TargomanLogDebug(5, "*** gtwAWSS3::storeFile: #" << __LINE__);
    S3ClientConfig.httpRequestTimeoutMs = 1000;
    S3ClientConfig.requestTimeoutMs = 1000;
    S3ClientConfig.connectTimeoutMs = 1000;

    TargomanLogDebug(5, "*** gtwAWSS3::storeFile: #" << __LINE__);
    S3ClientConfig.endpointOverride = _uploadGateway.ugwEndpointUrl.toStdString().c_str();
//    S3ClientConfig.enableHostPrefixInjection = false;
//    S3ClientConfig.enableEndpointDiscovery = false;

    TargomanLogDebug(5, "*** gtwAWSS3::storeFile: #" << __LINE__);
    S3::S3Client S3Client(
                AWSCredentials,
                S3ClientConfig,
                Aws::Client::AWSAuthV4Signer::PayloadSigningPolicy::Never,
                /* useVirtualAddressing */ _uploadGateway.ugwEndpointIsVirtualHosted
                );

    //----------------------------------------
    ///@TODO: use MultipartUploader for files larger than 400 MB

    TargomanLogDebug(5, "*** gtwAWSS3::storeFile: #" << __LINE__);
    S3::Model::PutObjectRequest Request;

    TargomanLogDebug(5, "*** gtwAWSS3::storeFile: #" << __LINE__);
    if (_uploadGateway.ugwBucket.isEmpty() == false)
        Request.SetBucket(_uploadGateway.ugwBucket.toStdString().c_str());

    QString Key = QString("%1/%2").arg(_path).arg(_fileName);
    TargomanLogDebug(5, "*** gtwAWSS3::storeFile: #" << __LINE__ << Key);
    Request.SetKey(Key.toStdString().c_str());

    TargomanLogDebug(5, "*** gtwAWSS3::storeFile: #" << __LINE__);
    Request.SetACL(S3::Model::ObjectCannedACL::public_read);

    TargomanLogDebug(5, "*** gtwAWSS3::storeFile: #" << __LINE__);
    std::shared_ptr<Aws::IOStream> InputData = Aws::MakeShared<Aws::FStream>(
                QString("SampleAllocationTag_%1").arg(_path).toStdString().c_str(),
                _fullFileName.toStdString().c_str(),
                std::ios_base::in | std::ios_base::binary);

    TargomanLogDebug(5, "*** gtwAWSS3::storeFile: #" << __LINE__);
    Request.SetBody(InputData);

    //----------------------------------------
    TargomanLogDebug(5, "*** gtwAWSS3::storeFile: #" << __LINE__ << "S3Client.PutObject");
    S3::Model::PutObjectOutcome Outcome = S3Client.PutObject(Request);

    TargomanLogDebug(5, "*** gtwAWSS3::storeFile: #" << __LINE__);
    if (Outcome.IsSuccess() == false)
        throw exTargomanBase(QString("Could not save file to the s3 server: %1").arg(Outcome.GetError().GetMessage().c_str()), ESTATUS_REQUEST_TIMEOUT);

    TargomanLogDebug(5, "*** gtwAWSS3::storeFile: END");

    return true;
}

} //namespace Targoman::API::ObjectStorage::Gateways

#endif // TARGOMAN_API_ENABLE_AWS_S3
