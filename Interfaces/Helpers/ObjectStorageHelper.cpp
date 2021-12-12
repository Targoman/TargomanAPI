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

#include <QMimeDatabase>

#include "ORM/ObjectStorage.h"
using namespace Targoman::API::ORM;

#include <fstream>
//#include <aws/core/utils/memory/stl/AWSStreamFwd.h>
#include <aws/core/Aws.h>
#include <aws/s3/S3Client.h>
#include <aws/s3/model/PutObjectRequest.h>
using namespace Aws;
#include "QHttp/qhttpfwd.hpp"
using namespace qhttp;

/** installing AmazonAWS S3 from source ************************************************************
git clone --recurse-submodules https://github.com/aws/aws-sdk-cpp
mkdir aws-build.s3
cd aws-build.s3
cmake ../aws-sdk-cpp -DCMAKE_BUILD_TYPE=Release -DCMAKE_INSTALL_PREFIX=/usr -DCMAKE_PREFIX_PATH=/usr -DBUILD_ONLY="s3"
make
sudo make install
*/

TAPI_REGISTER_METATYPE(
    /* complexity         */ COMPLEXITY_Object,
    /* namespace          */ Targoman::API::Helpers,
    /* type               */ stuSaveFileResult,
    /* toVariantLambda    */ [](const stuSaveFileResult& _value) -> QVariant {
        return _value.toJson().toVariantMap();
    }
    /* fromVariantLambda  */ /*[](const QVariant& _value, const QByteArray& _paramName) -> stuSaveFileResult {
        if (_value.isValid() == false)
            return stuSaveFileResult();

        if (_value.canConvert<QVariantMap>()
    //                || _value.canConvert<QVariantList>()
    //                || _value.canConvert<double>()
            )
        {
            auto ret = QJsonDocument::fromVariant(_value);
            return stuSaveFileResult().fromJson(ret.object());
        }

        if (_value.toString().isEmpty())
            return stuSaveFileResult();

        QJsonParseError Error;
        QJsonDocument Doc;
        Doc = Doc.fromJson(_value.toString().toUtf8(), &Error);

        if (Error.error != QJsonParseError::NoError)
            throw exHTTPBadRequest(_paramName + " is not a valid Prevoucher: <"+_value.toString()+">" + Error.errorString());
        if (Doc.isObject() == false)
            throw exHTTPBadRequest(_paramName + " is not a valid Prevoucher object: <"+_value.toString()+">");
        return stuSaveFileResult().fromJson(Doc.object());
    }*/
);

namespace Targoman::API::Helpers {

stuSaveFileResult ObjectStorageHelper::saveFile(
        intfUploadFiles &_uploadFiles,
        const quint64 _currentUserID,
        const TAPI::stuFileInfo &_file
    )
{
#ifdef AAAAAAAAAAAAAA
 //    auto _objectStorageConfigs = _uploadFiles->getObjectStorageConfigs();
    Targoman::API::ORM::intfUploadFiles::stuObjectStorageConfigs _objectStorageConfigs = _uploadFiles->getObjectStorageConfigs();

    //1: save file to the /tmp
    QByteArray Content = QByteArray::fromBase64(_base64Content.toLatin1());
    if (Content.length() == 0)
        throw exTargomanBase("The content is empty.", ESTATUS_BAD_REQUEST);

    _fileName = _fileName.trimmed();
    if ((_fileName.indexOf('/') >= 0) || (_fileName.indexOf('\\') >= 0))
        throw exTargomanBase("Invalid character in file name.", ESTATUS_BAD_REQUEST);

    QString FullFileName = QString("/tmp/TAPI/%1/%2").arg(_uploadFiles->schema()).arg(_fileName);

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
            FullFileUrl += ".arvan_____XYZ.com";
            break;

        default:
            break;
    }
    FullFileUrl = FullFileUrl.toLower();
    FullFileUrl += QString("/%1").arg(_fileName);

    //----------------------------------------
    QVariant OldData = _uploadFiles.Select(_uploadFiles,
                                      /* currentUserID */ _currentUserID,
                                      /* pksByPath     */ {},
                                      /* offset        */ 0,
                                      /* limit         */ 10,
                                      /* cols          */ {},
                                      /* filters       */ QString("%1=%2").arg(tblUploadFiles::uplURL).arg(FullFileUrl.replace(" ", "$SPACE$")),
                                      /* orderBy       */ {},
                                      /* groupBy       */ {},
                                      /* reportCount   */ false
                                      );
    if (OldData.isValid() && (OldData.toList().length() > 0))
//    if (QFile::exists(FullFileName))
        throw exTargomanBase("A file already exists with the same name.", ESTATUS_CONFLICT);

    QString FullPath = QString("/tmp/TAPI/%1").arg(_uploadFiles->schema());
    QDir(FullPath).mkpath(".");

    QFile File(FullFileName);
    if (File.open(QFile::WriteOnly) == false)
        throw exTargomanBase("Could not create temp file.", ESTATUS_CONFLICT);

    File.write(Content);
    File.close();

    stuSaveFileResult SaveFileResult;
    SaveFileResult.FullFileUrl = FullFileUrl;
    SaveFileResult.MimeType = MimeType;
    QString SaveErrorMessage = "";

    try
    {
        SaveFileResult.IsUploaded = ObjectStorageHelper::uploadFileToS3(
            _fileName,
            FullFileName,
            _objectStorageConfigs.Bucket,
            _objectStorageConfigs.Region
        );
    }
    catch (const std::exception &exp)
    {
        SaveErrorMessage = exp.what();
        SaveFileResult.IsUploaded = false;
    }

    if (SaveFileResult.IsUploaded)
        QFile::remove(FullFileName);

    //------------------------------------------
    TAPI::ORMFields_t CreateInfo({
        { tblUploadFiles::uplURL, FullFileUrl },
        { tblUploadFiles::uplFileName, _fileName },
        { tblUploadFiles::uplSize, Content.length() },
        { tblUploadFiles::uplMimeType, MimeType },
        { tblUploadFiles::uplTempFullFileName, FullFileName },
        { tblUploadFiles::uplStatus, SaveFileResult.IsUploaded
          ? enuUploadFileStatus::Uploaded
          : enuUploadFileStatus::Queued },
    });

    if (SaveErrorMessage.isEmpty() == false)
        CreateInfo.insert(tblUploadFiles::uplUploadLastErrorMessage, SaveErrorMessage);

    SaveFileResult.uplID = _uploadFiles->Create(*_uploadFiles, _currentUserID, CreateInfo);

    return SaveFileResult;
#endif
}

bool ObjectStorageHelper::uploadFileToS3(
        const QString &_fileName,
        const QString &_fullFileName,
        const QString &_bucket,
        const QString &_region
        )
{
#ifdef AAAAAAAAAAAAAA
    //upload to s3
    S3::Model::PutObjectRequest Request;

    Request.SetBucket(_bucket.toStdString());
    Request.SetKey(_fileName.toStdString());

    //----------------------------------------
    std::shared_ptr<Aws::IOStream> InputData =
            Aws::MakeShared<Aws::FStream>("SampleAllocationTag",
                                          _fullFileName.toStdString().c_str(),
                                          std::ios_base::in | std::ios_base::binary);

    Request.SetBody(InputData);

    //----------------------------------------
    Aws::Client::ClientConfiguration S3ClientConfig;
    if (_region.isEmpty() == false)
        S3ClientConfig.region = _region.toStdString();

    S3ClientConfig.endpointOverride

    S3::S3Client S3Client(S3ClientConfig);

    Aws::S3::Model::PutObjectOutcome Outcome = S3Client.PutObject(Request);

    if (Outcome.IsSuccess() == false)
        throw exTargomanBase(QString("Could not save file to the s3 server: %1").arg(Outcome.GetError().GetMessage().c_str()), ESTATUS_REQUEST_TIMEOUT);

    return true;
#endif
}

} //namespace Targoman::API::Helpers
