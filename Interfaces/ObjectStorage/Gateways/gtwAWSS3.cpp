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

#ifdef TARGOMAN_API_AWS_S3

#include <string>
#include <fstream>
#include "QHttp/qhttpfwd.hpp"
using namespace qhttp;

using namespace Targoman::API::Common;

namespace Targoman::API::ObjectStorage::Gateways {

bool gtwAWSS3::storeFile(
        const TAPI::JSON_t &_metaInfo,
        const QString &_fileName,
        const QString &_fileUUID,
        const QString &_fullFileName
    )
{
    QString Bucket = _metaInfo[AWSS3MetaInfoJsonKey::Bucket].toString();
    QString EndpointUrl = _metaInfo[AWSS3MetaInfoJsonKey::EndpointUrl].toString();
    QString SecretKey = _metaInfo[AWSS3MetaInfoJsonKey::SecretKey].toString();
    QString AccessKey = _metaInfo[AWSS3MetaInfoJsonKey::AccessKey].toString();

    ///TODO: use MultipartUploader for files larger than 400 MB

    //upload to s3
    S3::Model::PutObjectRequest Request;

    Request.SetBucket(Bucket.toStdString());
    Request.SetKey(QString("%1_%2").arg(_fileUUID).arg(_fileName).toStdString());

    //----------------------------------------
    std::shared_ptr<Aws::IOStream> InputData =
            Aws::MakeShared<Aws::FStream>("SampleAllocationTag",
                                          _fullFileName.toStdString().c_str(),
                                          std::ios_base::in | std::ios_base::binary);

    Request.SetBody(InputData);

    //----------------------------------------
    Aws::Auth::AWSCredentials AWSCredentials(AccessKey.toStdString(), SecretKey.toStdString());

    Aws::Client::ClientConfiguration S3ClientConfig;

//    if (_region.isEmpty() == false)
//        S3ClientConfig.region = _region.toStdString();

    S3ClientConfig.endpointOverride = EndpointUrl.toStdString();

    S3::S3Client S3Client(AWSCredentials, S3ClientConfig);

    S3::Model::PutObjectOutcome Outcome = S3Client.PutObject(Request);

    if (Outcome.IsSuccess() == false)
        throw exTargomanBase(QString("Could not save file to the s3 server: %1").arg(Outcome.GetError().GetMessage().c_str()), ESTATUS_REQUEST_TIMEOUT);

    return true;
}

} //namespace Targoman::API::ObjectStorage::Gateways

#endif // TARGOMAN_API_AWS_S3
