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

//#include "Interfaces/Helpers/ObjectStorageHelper.h"
//using namespace Targoman::API::Helpers;

using namespace Targoman::API::DBM;
using namespace Targoman::API::API;

namespace Targoman::API::ORM {

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

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-variable"

namespace tblUploadFiles {
    constexpr char Name[] = "tblUploadFiles";
    TARGOMAN_CREATE_CONSTEXPR(uflID);
    TARGOMAN_CREATE_CONSTEXPR(uflFileName);
    TARGOMAN_CREATE_CONSTEXPR(uflFileUUID);
    TARGOMAN_CREATE_CONSTEXPR(uflSize);
    TARGOMAN_CREATE_CONSTEXPR(uflFileType);
    TARGOMAN_CREATE_CONSTEXPR(uflMimeType);
    TARGOMAN_CREATE_CONSTEXPR(uflLocalFullFileName);
    TARGOMAN_CREATE_CONSTEXPR(uflStatus);
    TARGOMAN_CREATE_CONSTEXPR(uflCreationDateTime);
    TARGOMAN_CREATE_CONSTEXPR(uflCreatedBy_usrID);
    TARGOMAN_CREATE_CONSTEXPR(uflUpdatedBy_usrID);
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
    TARGOMAN_CREATE_CONSTEXPR(uquStatus);
    TARGOMAN_CREATE_CONSTEXPR(uquCreationDateTime);
    TARGOMAN_CREATE_CONSTEXPR(uquCreatedBy_usrID);
    TARGOMAN_CREATE_CONSTEXPR(uquUpdatedBy_usrID);
}

namespace tblUploadGateways {
    constexpr char Name[] = "tblUploadGateways";
    TARGOMAN_CREATE_CONSTEXPR(ugwID);
    TARGOMAN_CREATE_CONSTEXPR(ugwType);
//    TARGOMAN_CREATE_CONSTEXPR(ugwBucket);
//    TARGOMAN_CREATE_CONSTEXPR(ugwEndpointUrl);
//    TARGOMAN_CREATE_CONSTEXPR(ugwSecretKey);
//    TARGOMAN_CREATE_CONSTEXPR(ugwAccessKey);
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
}

#pragma GCC diagnostic pop

namespace Private {
struct stuProcessUploadQueueInfo
{
    //Upload Queue
    quint64 uquID;
    quint64 uqu_uflID;
    quint32 uqu_ugwID;
    enuUploadQueueStatus::Type uquStatus;

    //Upload File
    quint64 uflID;
    QString uflFileName;
    QString uflFileUUID;
    quint64 uflSize;
    QString uflFileType;
    QString uflMimeType;
    QString uflLocalFullFileName;
//    enuUploadFileStatus::Type uflStatus;

    //Upload Gateway
    quint32 ugwID;
    enuUploadGatewayType::Type ugwType;
//    QString ugwBucket;
//    QString ugwEndpointUrl;
//    QString ugwSecretKey;
//    QString ugwAccessKey;
    TAPI::JSON_t ugwMetaInfo;
    quint64 ugwCreatedFilesCount;
    quint64 ugwCreatedFilesSize;
    NULLABLE_TYPE(TAPI::DateTime_t) ugwLastActionTime; //used for loadbalance
    enuUploadGatewayStatus::Type ugwStatus;

    void fromVariantMap(const QVariantMap& _info);
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
        QString LocalStoragePath;

        stuObjectStorageConfigs(const stuObjectStorageConfigs &_other) :
            LocalStoragePath(_other.LocalStoragePath)
        {}
        stuObjectStorageConfigs(const QString &_localStoragePath) :
            LocalStoragePath(_localStoragePath)
        {}
    };

    virtual stuObjectStorageConfigs getObjectStorageConfigs() const = 0;

protected slots:
};

class intfUploadQueue : public intfSQLBasedModule
{
    Q_OBJECT

public:
    intfUploadQueue(
            const QString &_schema,
            const QString &_name
        );
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
    bool ORMDELETE("Delete a object storage gateway")
};

} //namespace Targoman::API::ORM

TAPI_DECLARE_METATYPE_ENUM(Targoman::API::ORM, enuUploadFileStatus);
TAPI_DECLARE_METATYPE_ENUM(Targoman::API::ORM, enuUploadQueueStatus);
TAPI_DECLARE_METATYPE_ENUM(Targoman::API::ORM, enuUploadGatewayType);
TAPI_DECLARE_METATYPE_ENUM(Targoman::API::ORM, enuUploadGatewayStatus);

/****************************************************/
//put this macro before module class definition (.h)
#define TARGOMAN_OBJECTSTORAGE_PREPARENT \
    class UploadFiles; \
    class UploadQueue; \
    class UploadGateways;

//put this macro after module class definition (.h)
#define TARGOMAN_OBJECTSTORAGE_POSTPARENT(_module, _schema) \
class UploadFiles : public Targoman::API::ORM::intfUploadFiles \
{ \
    Q_OBJECT \
    TARGOMAN_DEFINE_API_SUBMODULE_WO_CTOR(_module, UploadFiles) \
public: \
    UploadFiles() : \
        intfUploadFiles( \
            Targoman::Common::demangle(typeid(_module).name()).split("::").last(), \
            _schema \
        ) \
    {} \
    virtual intfUploadFiles::stuObjectStorageConfigs getObjectStorageConfigs() const { \
        return intfUploadFiles::stuObjectStorageConfigs( \
            _module::ObjectStorage::LocalStoragePath.value() \
        ); \
    } \
}; \
class UploadQueue : public Targoman::API::ORM::intfUploadQueue \
{ \
    Q_OBJECT \
    TARGOMAN_DEFINE_API_SUBMODULE_WO_CTOR(_module, UploadQueue) \
public: \
    UploadQueue() : \
        intfUploadQueue( \
            Targoman::Common::demangle(typeid(_module).name()).split("::").last(), \
            _schema \
        ) \
    {} \
}; \
class UploadGateways : public Targoman::API::ORM::intfUploadGateways \
{ \
    Q_OBJECT \
    TARGOMAN_DEFINE_API_SUBMODULE_WO_CTOR(_module, UploadGateways) \
public: \
    UploadGateways() : \
        intfUploadGateways( \
            Targoman::Common::demangle(typeid(_module).name()).split("::").last(), \
            _schema \
        ) \
    {} \
};

//put this macro inside module class definition (.h) after TARGOMAN_DEFINE_API_MODULE
#define TARGOMAN_API_DEFINE_OBJECTSTORAGE(_module, _schema) \
protected: \
    QScopedPointer<UploadFiles>     _UploadFiles;    \
    QScopedPointer<UploadQueue>     _UploadQueue; \
    QScopedPointer<UploadGateways>  _UploadGateways; \
public: \
    struct ObjectStorage { \
        static inline QString makeConfig(const QString& _name) { return QString("/Module_%1/ObjectStorage/%2").arg(TARGOMAN_M2STR(_module), _name); } \
        static Targoman::Common::Configuration::tmplConfigurable<QString>   LocalStoragePath; \
    };

//put this macro before module class constructor (.cpp)
#define TARGOMAN_API_OBJECTSTORAGE_CONFIG_IMPL(_module, _schema)                \
    using namespace Targoman::Common::Configuration;                            \
    tmplConfigurable<QString> _module::ObjectStorage::LocalStoragePath(         \
        _module::ObjectStorage::makeConfig("LocalStoragePath"),                 \
        "LocalStoragePath",                                                     \
        QString("%1/tapi/%2/objectstorage").arg(QDir::homePath()).arg(TARGOMAN_M2STR(_module))  \
    );                                                                          \
//    QString("/var/spool/tapi/%1/objectstorage").arg(TARGOMAN_M2STR(_module))

//put this macro into module class constructor (.cpp)
#define TARGOMAN_API_IMPLEMENT_OBJECTSTORAGE(_module, _schema) \
    this->_UploadFiles   .reset(&UploadFiles   ::instance()); \
    this->_UploadQueue   .reset(&UploadQueue   ::instance()); \
    this->_UploadGateways.reset(&UploadGateways::instance()); \
    \
    this->addSubModule(this->_UploadFiles.data());    \
    this->addSubModule(this->_UploadQueue.data());    \
    this->addSubModule(this->_UploadGateways.data()); \

//    UploadFiles    ::instance().prepareFiltersList(); \
//    UploadQueue    ::instance().prepareFiltersList(); \
//    UploadGateways ::instance().prepareFiltersList();

/****************************************************/
#endif // TARGOMAN_API_OBJECTSTORAGE_H
