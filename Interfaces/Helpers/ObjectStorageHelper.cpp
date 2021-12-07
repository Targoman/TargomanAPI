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

#include "ObjectStorageHelper.h"

#include <fstream>
//#include <aws/core/utils/memory/stl/AWSStreamFwd.h>
#include <aws/core/Aws.h>
#include <aws/s3/S3Client.h>
#include <aws/s3/model/PutObjectRequest.h>
using namespace Aws;

#include <QMimeDatabase>

/** installing AmazonAWS S3 ************************************************************
git clone --recurse-submodules https://github.com/aws/aws-sdk-cpp
mkdir aws-build.s3
cd aws-build.s3
cmake ../aws-sdk-cpp -DCMAKE_BUILD_TYPE=Release -DCMAKE_INSTALL_PREFIX=/usr -DCMAKE_PREFIX_PATH=/usr -DBUILD_ONLY="s3"
make
sudo make install
*/

namespace Targoman::API::Helpers {

quint64 ObjectStorageHelper::saveFile(
        intfUploads *_uploads,
        const quint64 _currentUserID,
        QString &_fileName,
        const QString &_base64Content
    )
{
//    auto _objectStorageConfigs = _uploads->getObjectStorageConfigs();
    Targoman::API::ORM::intfUploads::stuObjectStorageConfigs _objectStorageConfigs = _uploads->getObjectStorageConfigs();

    QByteArray Content = QByteArray::fromBase64(_base64Content.toLatin1());

    _fileName = _fileName.trimmed();
    if ((_fileName.indexOf('/') >= 0) || (_fileName.indexOf('\\') >= 0))
        throw exTargomanBase("Invalid character in file name.");

    //1: save file to the /tmp
    if (Content.length() == 0)
        throw exTargomanBase("The content is empty.");

    QString FullFileName = QString("/tmp/TAPI/%1/%2").arg(_uploads->schema()).arg(_fileName);
    if (QFile::exists(FullFileName))
        throw exTargomanBase("A file already exists with the same name.");

    QString FullPath = QString("/tmp/TAPI/%1").arg(_uploads->schema());
    QDir(FullPath).mkpath(".");

    QFile File(FullFileName);
    if (File.open(QFile::WriteOnly) == false)
        throw exTargomanBase("Could not create temp file.");

    File.write(Content);
    File.close();

    //2: upload to s3
    try
    {
        S3::Model::PutObjectRequest Request;

        Request.SetBucket(_objectStorageConfigs.Bucket.toStdString());
        Request.SetKey(_fileName.toStdString());

        std::shared_ptr<Aws::IOStream> InputData =
                Aws::MakeShared<Aws::FStream>("SampleAllocationTag",
                                              FullFileName.toStdString().c_str(),
                                              std::ios_base::in | std::ios_base::binary);

        Request.SetBody(InputData);

        //----------------------------------------
        Aws::Client::ClientConfiguration S3ClientConfig;
        if (_objectStorageConfigs.Region.isEmpty() == false)
            S3ClientConfig.region = _objectStorageConfigs.Region.toStdString();

        S3::S3Client S3Client(S3ClientConfig);

        Aws::S3::Model::PutObjectOutcome Outcome = S3Client.PutObject(Request);

        if (Outcome.IsSuccess() == false)
            throw exTargomanBase(QString("Could not save file to the s3 server: %1").arg(Outcome.GetError().GetMessage().c_str()));

        //----------------------------------------
        QMimeDatabase MimeDB;
        QString MimeType = MimeDB.mimeTypeForFile(FullFileName).name();

        QString FullFileUrl = _objectStorageConfigs.Bucket;
        FullFileUrl += ".s3";
        if (_objectStorageConfigs.Region.isEmpty() == false)
            FullFileUrl += QString(".%1").arg(_objectStorageConfigs.Region);
        switch (_objectStorageConfigs.Provider)
        {
            case Targoman::API::ORM::enuS3Provider::Amazon:
                FullFileUrl += ".amazonaws.com";
                break;

            ///todo: domain for arvan s3
            case Targoman::API::ORM::enuS3Provider::Arvan:
                FullFileUrl += ".arvancloud.com";
                break;

            default:
                break;
        }
        FullFileUrl += QString("/%1").arg(_fileName);

        //----------------------------------------
        QFile::remove(FullFileName);

        //2: create new row in tblUploads
        return _uploads->Create(*_uploads,
                            _currentUserID,
                            TAPI::ORMFields_t({
                                                  { tblUploads::uplURL, FullFileUrl },
                                                  { tblUploads::uplFileName, _fileName },
                                                  { tblUploads::uplSize, Content.length() },
                                                  { tblUploads::uplMimeType, MimeType },
                                              }));
    }
    catch(...)
    {
        QFile::remove(FullFileName);

        throw;
    }
}

} //namespace Targoman::API::Helpers
