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

#include "gtwNFS.h"
#include <QDir>
#include "QHttp/qhttpfwd.hpp"
using namespace qhttp;

using namespace Targoman::API::Common;

namespace Targoman::API::ObjectStorage::Gateways {

bool gtwNFS::storeFile(
        const TAPI::JSON_t &_metaInfo,
        const QString &_fileName,
        const QString &_fileUUID,
        const QString &_fullFileName
    ) {
    QString StoragePath = _metaInfo[NFSMetaInfoJsonKey::Path].toString();

    QDir FullPath(StoragePath);
    if (FullPath.exists() == false)
        throw exTargomanBase("NFS path not exists", ESTATUS_INTERNAL_SERVER_ERROR);

    QString FullFileName = QString("%1/%2_%3").arg(StoragePath).arg(_fileUUID).arg(_fileName);

    return QFile::copy(_fullFileName, FullFileName);
}

} //namespace Targoman::API::ObjectStorage::Gateways
