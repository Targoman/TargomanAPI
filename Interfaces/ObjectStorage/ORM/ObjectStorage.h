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

#ifndef TARGOMAN_API_OBJECTSTORAGE_H
#define TARGOMAN_API_OBJECTSTORAGE_H

#include "Interfaces/API/intfSQLBasedModule.h"
#include "libTargomanCommon/Configuration/tmplConfigurable.h"
using namespace Targoman::Common;

#include "Interfaces/Common/APIArgHelperMacros.hpp"

//#include "Interfaces/ObjectStorage/ObjectStorageManager.h"
//using namespace Targoman::API::Helpers;

using namespace Targoman::API::DBM;
using namespace Targoman::API::API;

namespace Targoman::API::ObjectStorage::ORM {

TARGOMAN_DEFINE_ENUM(enuUploadFileStatus,
                     New        = 'N', //new file and not queued (must be queued)
                     Removed    = 'R'
                     )
//                     Queued     = 'Q', //queued
//                     Uploading  = 'U', //queue(s) are in uploading
//                     Stored     = 'S', //queued before and uploaded to all matched remote storage gateways

TARGOMAN_DEFINE_ENUM(enuUploadQueueStatus,
                     New        = 'N',
                     Uploading  = 'U', //queue(s) are in uploading
                     Stored     = 'S', //queued before and upload to remote storage completed
                     Removed    = 'R'
                     )

TARGOMAN_DEFINE_ENUM(enuUploadGatewayType,
                     NFS    = 'N',
                     AWSS3  = '3',
                     )

TARGOMAN_DEFINE_ENUM(enuUploadGatewayStatus,
                     Active     = 'A',
                     Disabled   = 'D',
                     Removed    = 'R'
                     )
} //namespace Targoman::API::ObjectStorage::ORM

TAPI_DECLARE_METATYPE_ENUM(Targoman::API::ObjectStorage::ORM, enuUploadFileStatus);
TAPI_DECLARE_METATYPE_ENUM(Targoman::API::ObjectStorage::ORM, enuUploadQueueStatus);
TAPI_DECLARE_METATYPE_ENUM(Targoman::API::ObjectStorage::ORM, enuUploadGatewayType);
TAPI_DECLARE_METATYPE_ENUM(Targoman::API::ObjectStorage::ORM, enuUploadGatewayStatus);

namespace Targoman::API::ObjectStorage::ORM {

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-variable"

namespace tblUploadFiles {
    constexpr char Name[] = "tblUploadFiles";

    TARGOMAN_CREATE_CONSTEXPR(uflID);
    TARGOMAN_CREATE_CONSTEXPR(uflPath);
    TARGOMAN_CREATE_CONSTEXPR(uflOriginalFileName);
    TARGOMAN_CREATE_CONSTEXPR(uflCounter);
    TARGOMAN_CREATE_CONSTEXPR(uflStoredFileName);
    TARGOMAN_CREATE_CONSTEXPR(uflSize);
    TARGOMAN_CREATE_CONSTEXPR(uflFileType);
    TARGOMAN_CREATE_CONSTEXPR(uflMimeType);
    TARGOMAN_CREATE_CONSTEXPR(uflLocalFullFileName);
    TARGOMAN_CREATE_CONSTEXPR(uflStatus);
    TARGOMAN_CREATE_CONSTEXPR(uflCreationDateTime);
    TARGOMAN_CREATE_CONSTEXPR(uflCreatedBy_usrID);
    TARGOMAN_CREATE_CONSTEXPR(uflUpdatedBy_usrID);
    //TARGOMAN_CREATE_CONSTEXPR(uflUniqueMD5);

    TAPI_DEFINE_VARIANT_ENABLED_STRUCT(DTO,
        SF_quint64(uflID),
        SF_QString(uflPath),
        SF_QString(uflOriginalFileName),
        SF_quint32(uflCounter),
        SF_QString(uflStoredFileName),
        SF_quint64(uflSize),
        SF_QString(uflFileType),
        SF_QString(uflMimeType),
        SF_QString(uflLocalFullFileName),
        SF_quint64(uflCreatedBy_usrID)
    );
}

namespace tblUploadQueue {
    constexpr char Name[] = "tblUploadQueue";
    namespace Relation {
        constexpr char File[] = "file";
        constexpr char Gateway[] = "gateway";
    }

    TARGOMAN_CREATE_CONSTEXPR(uquID);
    TARGOMAN_CREATE_CONSTEXPR(uqu_uflID);
    TARGOMAN_CREATE_CONSTEXPR(uqu_ugwID);
    TARGOMAN_CREATE_CONSTEXPR(uquLockedAt);
    TARGOMAN_CREATE_CONSTEXPR(uquLastTryAt);
    TARGOMAN_CREATE_CONSTEXPR(uquStoredAt);
    TARGOMAN_CREATE_CONSTEXPR(uquStatus);
    TARGOMAN_CREATE_CONSTEXPR(uquCreationDateTime);
    TARGOMAN_CREATE_CONSTEXPR(uquCreatedBy_usrID);
    TARGOMAN_CREATE_CONSTEXPR(uquUpdatedBy_usrID);

    TAPI_DEFINE_VARIANT_ENABLED_STRUCT(DTO,
        SF_quint64(uquID),
        SF_quint64(uqu_uflID),
        SF_quint32(uqu_ugwID),
        SF_Enum(enuUploadQueueStatus, uquStatus, enuUploadQueueStatus::New)
    );
}

namespace tblUploadGateways {
    constexpr char Name[] = "tblUploadGateways";

    TARGOMAN_CREATE_CONSTEXPR(ugwID);
    TARGOMAN_CREATE_CONSTEXPR(ugwName);
    TARGOMAN_CREATE_CONSTEXPR(ugwType);
    TARGOMAN_CREATE_CONSTEXPR(ugwMetaInfo);
    // conditions
    TARGOMAN_CREATE_CONSTEXPR(ugwAllowedFileTypes);
    TARGOMAN_CREATE_CONSTEXPR(ugwAllowedMimeTypes);
    TARGOMAN_CREATE_CONSTEXPR(ugwAllowedMinFileSize);
    TARGOMAN_CREATE_CONSTEXPR(ugwAllowedMaxFileSize);
    TARGOMAN_CREATE_CONSTEXPR(ugwMaxFilesCount);
    TARGOMAN_CREATE_CONSTEXPR(ugwMaxFilesSize);
    // statistics
    TARGOMAN_CREATE_CONSTEXPR(ugwCreatedFilesCount);
    TARGOMAN_CREATE_CONSTEXPR(ugwCreatedFilesSize);
    TARGOMAN_CREATE_CONSTEXPR(ugwDeletedFilesCount);
    TARGOMAN_CREATE_CONSTEXPR(ugwDeletedFilesSize);
    TARGOMAN_CREATE_CONSTEXPR(ugwLastActionTime); //used for loadbalance
    //
    TARGOMAN_CREATE_CONSTEXPR(ugwStatus);
    TARGOMAN_CREATE_CONSTEXPR(ugwCreationDateTime);
    TARGOMAN_CREATE_CONSTEXPR(ugwCreatedBy_usrID);
    TARGOMAN_CREATE_CONSTEXPR(ugwUpdatedBy_usrID);

    TAPI_DEFINE_VARIANT_ENABLED_STRUCT(DTO,
        SF_quint32(ugwID),
        SF_NULLABLE_Enum(enuUploadGatewayType, ugwType),
        SF_JSON_t(ugwMetaInfo),
        SF_quint64(ugwCreatedFilesCount),
        SF_quint64(ugwCreatedFilesSize),
//        SF_NULLABLE_TYPE(TAPI::DateTime_t) ugwLastActionTime), //used for loadbalance
        SF_DateTime_t(ugwLastActionTime), //used for loadbalance
        SF_Enum(enuUploadGatewayStatus, ugwStatus, enuUploadGatewayStatus::Active)
    );
}

#pragma GCC diagnostic pop

namespace Private {

struct stuProcessUploadQueueInfo
{
    tblUploadFiles::DTO UploadFiles;
    tblUploadQueue::DTO UploadQueue;
    tblUploadGateways::DTO UploadGateways;

    void fromVariantMap(const QVariantMap &_info);
};

} //namespace Private

class intfUploadFiles : public intfSQLBasedModule
{
    Q_OBJECT

public:
    intfUploadFiles(
        const QString &_schema,
        const QString &_name
    );

    struct stuObjectStorageConfigs
    {
        QString TempLocalStoragePath;

        stuObjectStorageConfigs(const stuObjectStorageConfigs &_other) :
            TempLocalStoragePath(_other.TempLocalStoragePath)
        { ; }

        stuObjectStorageConfigs(const QString &_localStoragePath) :
            TempLocalStoragePath(_localStoragePath)
        { ; }
    };

    virtual stuObjectStorageConfigs getObjectStorageConfigs() const = 0;

private slots:
    QVariant ORMGET("Get uploaded files")
};

class intfUploadQueue : public intfSQLBasedModule
{
    Q_OBJECT

public:
    intfUploadQueue(
        const QString &_schema,
        const QString &_name
    );

private slots:
    QVariant ORMGET("Get upload queue")
};

class intfUploadGateways : public intfSQLBasedModule
{
    Q_OBJECT

public:
    intfUploadGateways(
        const QString &_schema,
        const QString &_name
    );

private slots:
    QVariant ORMGET("Get object storage gateway information")
    quint32 ORMCREATE("Create a new object storage gateway by priviledged user")
    bool ORMUPDATE("Update object storage gateway info by priviledged user")
    bool ORMDELETE("Delete an object storage gateway")
};

} //namespace Targoman::API::ObjectStorage::ORM

/****************************************************/
//put this macro before module class definition (.h)
#define TARGOMAN_OBJECTSTORAGE_PREPARENT \
    class UploadFiles; \
    class UploadQueue; \
    class UploadGateways;

//put this macro after module class definition (.h)
#define TARGOMAN_OBJECTSTORAGE_POSTPARENT(_module, _schema) \
class UploadFiles : public Targoman::API::ObjectStorage::ORM::intfUploadFiles \
{ \
    Q_OBJECT \
    TARGOMAN_DEFINE_API_SUBMODULE_WO_CTOR(_module, UploadFiles) \
public: \
    UploadFiles() : \
        intfUploadFiles( \
            Targoman::Common::demangle(typeid(_module).name()).split("::").last(), \
            _schema \
        ) \
    { ; } \
    virtual intfUploadFiles::stuObjectStorageConfigs getObjectStorageConfigs() const { \
        return intfUploadFiles::stuObjectStorageConfigs( \
            _module::ObjectStorage::TempLocalStoragePath.value() \
        ); \
    } \
}; \
class UploadQueue : public Targoman::API::ObjectStorage::ORM::intfUploadQueue \
{ \
    Q_OBJECT \
    TARGOMAN_DEFINE_API_SUBMODULE_WO_CTOR(_module, UploadQueue) \
public: \
    UploadQueue() : \
        intfUploadQueue( \
            Targoman::Common::demangle(typeid(_module).name()).split("::").last(), \
            _schema \
        ) \
    { ; } \
}; \
class UploadGateways : public Targoman::API::ObjectStorage::ORM::intfUploadGateways \
{ \
    Q_OBJECT \
    TARGOMAN_DEFINE_API_SUBMODULE_WO_CTOR(_module, UploadGateways) \
public: \
    UploadGateways() : \
        intfUploadGateways( \
            Targoman::Common::demangle(typeid(_module).name()).split("::").last(), \
            _schema \
        ) \
    { ; } \
};

//put this macro inside module class definition (.h) after TARGOMAN_DEFINE_API_MODULE
#define TARGOMAN_API_DEFINE_OBJECTSTORAGE(_module, _schema) \
protected: \
    QScopedPointer<UploadFiles>     _UploadFiles;    \
    QScopedPointer<UploadQueue>     _UploadQueue;    \
    QScopedPointer<UploadGateways>  _UploadGateways; \
public: \
    struct ObjectStorage { \
        static inline QString makeConfig(const QString& _name) { return QString("/Module_%1/ObjectStorage/%2").arg(TARGOMAN_M2STR(_module), _name); } \
        static Targoman::Common::Configuration::tmplConfigurable<QString>   TempLocalStoragePath; \
    };

//put this macro before module class constructor (.cpp)
#define TARGOMAN_API_OBJECTSTORAGE_CONFIG_IMPL(_module, _schema)                                \
    using namespace Targoman::Common::Configuration;                                            \
    tmplConfigurable<QString> _module::ObjectStorage::TempLocalStoragePath(                     \
        _module::ObjectStorage::makeConfig("TempLocalStoragePath"),                             \
        "Path to store uploading files that shares by instances on same server",                \
        QString("%1/tapi/%2/objectstorage").arg(QDir::homePath()).arg(TARGOMAN_M2STR(_module)), \
        ReturnTrueCrossValidator(),                                                             \
        "",                                                                                     \
        "TempLocalStoragePath",                                                                 \
        QString("%1-temp-local-storage-path").arg(TARGOMAN_M2STR(_module)).toLower(),           \
        enuConfigSource::Arg | enuConfigSource::File                                            \
    );
//    QString("/var/spool/tapi/%1/objectstorage").arg(TARGOMAN_M2STR(_module))

//put this macro into module class constructor (.cpp)
#define TARGOMAN_API_IMPLEMENT_OBJECTSTORAGE(_module, _schema)  \
    this->_UploadFiles   .reset(&UploadFiles   ::instance());   \
    this->_UploadQueue   .reset(&UploadQueue   ::instance());   \
    this->_UploadGateways.reset(&UploadGateways::instance());   \
    this->addSubModule(this->_UploadFiles.data());              \
    this->addSubModule(this->_UploadQueue.data());              \
    this->addSubModule(this->_UploadGateways.data());

//    UploadFiles    ::instance().prepareFiltersList();
//    UploadQueue    ::instance().prepareFiltersList();
//    UploadGateways ::instance().prepareFiltersList();

/****************************************************/
#endif // TARGOMAN_API_OBJECTSTORAGE_H
