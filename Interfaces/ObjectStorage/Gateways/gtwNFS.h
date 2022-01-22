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

#ifndef TARGOMAN_API_GTWNFS_H
#define TARGOMAN_API_GTWNFS_H

#include "intfObjectStorageGateway.h"
#include "libTargomanCommon/Macros.h"
//using namespace Targoman::Common;

#include "Interfaces/Common/APIArgHelperMacros.hpp"
#include "Interfaces/Common/GenericTypes.h"

namespace Targoman::API::Extensions::ObjectStorage {

TARGOMAN_DEFINE_ENUM(enuNFSMetaInfo,
                     Path
                     );

class gtwNFS : public intfObjectStorageGateway
{
public:
    static bool storeFile(
        const TAPI::JSON_t &_metaInfo,
        const QString &_fileName,
        const QString &_fileUUID,
        const QString &_fullFileName
    );
};

} //namespace Targoman::API::Extensions::ObjectStorage

TAPI_DECLARE_METATYPE_ENUM(Targoman::API::Extensions::ObjectStorage, enuNFSMetaInfo);

#endif // TARGOMAN_API_GTWNFS_H
