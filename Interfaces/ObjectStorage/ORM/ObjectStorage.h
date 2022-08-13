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
#include "Interfaces/AAA/AAADefs.hpp"

using namespace Targoman::API::AAA;
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

TARGOMAN_DEFINE_ENUM(enuUploadGatewayType,
                     NFS    = 'N',
                     AWSS3  = '3',
                     )

TARGOMAN_DEFINE_ENUM(enuUploadGatewayStatus,
                     Active     = 'A',
                     Disabled   = 'D',
                     Removed    = 'R'
                     )

TARGOMAN_DEFINE_ENUM(enuUploadQueueStatus,
                     New        = 'N',
//                     Uploading  = 'U', //queue(s) are in uploading
                     Stored     = 'S', //queued before and upload to remote storage completed
                     Error      = 'E',
                     Removed    = 'R'
                     )
} //namespace Targoman::API::ObjectStorage::ORM

TAPI_DECLARE_METATYPE_ENUM(Targoman::API::ObjectStorage::ORM, enuUploadFileStatus);
TAPI_DECLARE_METATYPE_ENUM(Targoman::API::ObjectStorage::ORM, enuUploadGatewayType);
TAPI_DECLARE_METATYPE_ENUM(Targoman::API::ObjectStorage::ORM, enuUploadGatewayStatus);
TAPI_DECLARE_METATYPE_ENUM(Targoman::API::ObjectStorage::ORM, enuUploadQueueStatus);

namespace Targoman::API::ObjectStorage::ORM {

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-variable"

namespace tblUploadFiles {
    constexpr char Name[] = "tblUploadFiles";

    namespace Fields {
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
    }

    namespace Relation {
        // constexpr char AAA[] = "aaa";
    }

    namespace Private {
        const QList<clsORMField> ORMFields = {
            ///ColName                               Type                    Validation                  Default     UpBy    Sort  Filter Self  Virt   PK
            { Fields::uflID,                ORM_PRIMARYKEY_64 },
            { Fields::uflPath,              S(QString),             QFV,                        QRequired,  UPNone },
            { Fields::uflOriginalFileName,  S(QString),             QFV,                        QRequired,  UPNone },
            { Fields::uflCounter,           S(quint32),             QFV,                        QNull,      UPNone },
            { Fields::uflStoredFileName,    S(QString),             QFV,                        QRequired,  UPNone },
            { Fields::uflSize,              S(quint64),             QFV.integer().minValue(1),  QRequired,  UPNone },
            { Fields::uflFileType,          S(QString),             QFV,                        QNull,      UPNone },
            { Fields::uflMimeType,          S(QString),             QFV,                        QNull,      UPNone },
            { Fields::uflLocalFullFileName, S(QString),             QFV,                        QRequired,  UPNone },
            { Fields::uflStatus,            ORM_STATUS_FIELD(Targoman::API::ObjectStorage::ORM::enuUploadFileStatus, Targoman::API::ObjectStorage::ORM::enuUploadFileStatus::New) },
            { Fields::uflCreationDateTime,  ORM_CREATED_ON },
            { Fields::uflCreatedBy_usrID,   ORM_CREATED_BY },
            { Fields::uflUpdatedBy_usrID,   ORM_UPDATED_BY },
        };

        const QList<stuRelation> Relations = {
            ///Col                        Reference Table              ForeignCol       Rename     LeftJoin
            ORM_RELATION_OF_CREATOR(Fields::uflCreatedBy_usrID),
            ORM_RELATION_OF_UPDATER(Fields::uflUpdatedBy_usrID),
        };

        const QList<stuDBIndex> Indexes = {
        };

    } //namespace Private

    TAPI_DEFINE_STRUCT(DTO,
        SF_ORM_PRIMARYKEY_64        (uflID),
        SF_QString                  (uflPath),
        SF_QString                  (uflOriginalFileName),
        SF_quint32                  (uflCounter),
        SF_QString                  (uflStoredFileName),
        SF_quint64                  (uflSize),
        SF_QString                  (uflFileType),
        SF_QString                  (uflMimeType),
        SF_QString                  (uflLocalFullFileName),
        SF_ORM_STATUS_FIELD         (uflStatus, Targoman::API::ObjectStorage::ORM::enuUploadFileStatus, Targoman::API::ObjectStorage::ORM::enuUploadFileStatus::New),
        SF_ORM_CREATED_ON           (uflCreationDateTime),
        SF_ORM_CREATED_BY           (uflCreatedBy_usrID),
        SF_ORM_UPDATED_BY           (uflUpdatedBy_usrID)
    );
}

namespace tblUploadGateways {
    constexpr char Name[] = "tblUploadGateways";

    namespace Fields {
        TARGOMAN_CREATE_CONSTEXPR(ugwID);
        TARGOMAN_CREATE_CONSTEXPR(ugwName);
        TARGOMAN_CREATE_CONSTEXPR(ugwType);
        TARGOMAN_CREATE_CONSTEXPR(ugwBucket);
        TARGOMAN_CREATE_CONSTEXPR(ugwEndpointUrl);
        TARGOMAN_CREATE_CONSTEXPR(ugwEndpointIsVirtualHosted);
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

    namespace Relation {
        // constexpr char AAA[] = "aaa";
    }

    namespace Private {
        const QList<clsORMField> ORMFields = {
            ///ColName                                       Type                        Validation                          Default     UpBy    Sort  Filter Self  Virt   PK
            { Fields::ugwID,                     ORM_PRIMARYKEY_32 },
            { Fields::ugwName,                   S(QString),                 QFV.maxLenght(50),                  QRequired,  UPAdmin },
            { Fields::ugwType,                   S(Targoman::API::ObjectStorage::ORM::enuUploadGatewayType::Type), QFV, QRequired,  UPAdmin },
            { Fields::ugwBucket,                 S(QString),                 QFV.maxLenght(256),                 QRequired,  UPAdmin },
            { Fields::ugwEndpointUrl,            S(QString),                 QFV.maxLenght(512),                 QRequired,  UPAdmin },
            { Fields::ugwEndpointIsVirtualHosted,S(bool),                    QFV,                                false,      UPOwner },
            { Fields::ugwMetaInfo,               S(TAPI::JSON_t),            QFV,                                QNull,      UPAdmin },
            // conditions
            { Fields::ugwAllowedFileTypes,       S(QString),                 QFV.unicodeAlNum().maxLenght(512),  QNull,      UPAdmin },
            { Fields::ugwAllowedMimeTypes,       S(QString),                 QFV.unicodeAlNum().maxLenght(1024), QNull,      UPAdmin },
            { Fields::ugwAllowedMinFileSize,     S(NULLABLE_TYPE(quint64)),  QFV,                                QNull,      UPAdmin },
            { Fields::ugwAllowedMaxFileSize,     S(NULLABLE_TYPE(quint64)),  QFV,                                QNull,      UPAdmin },
            { Fields::ugwMaxFilesCount,          S(NULLABLE_TYPE(quint64)),  QFV,                                QNull,      UPAdmin },
            { Fields::ugwMaxFilesSize,           S(NULLABLE_TYPE(quint64)),  QFV,                                QNull,      UPAdmin },
            // statistics
            { Fields::ugwCreatedFilesCount,      S(quint64),                 QFV,                                0,          UPAdmin },
            { Fields::ugwCreatedFilesSize,       S(quint64),                 QFV,                                0,          UPAdmin },
            { Fields::ugwDeletedFilesCount,      S(quint64),                 QFV,                                0,          UPAdmin },
            { Fields::ugwDeletedFilesSize,       S(quint64),                 QFV,                                0,          UPAdmin },
            { Fields::ugwLastActionTime,         S(NULLABLE_TYPE(TAPI::DateTime_t)), QFV,                        QNull,      UPAdmin },
            //------------------
            { Fields::ugwStatus,                 ORM_STATUS_FIELD(Targoman::API::ObjectStorage::ORM::enuUploadGatewayStatus, Targoman::API::ObjectStorage::ORM::enuUploadGatewayStatus::Active) },
            { Fields::ugwCreationDateTime,       ORM_CREATED_ON },
            { Fields::ugwCreatedBy_usrID,        ORM_CREATED_BY },
            { Fields::ugwUpdatedBy_usrID,        ORM_UPDATED_BY },
        };

        const QList<stuRelation> Relations = {
            ///Col                        Reference Table              ForeignCol       Rename     LeftJoin
            ORM_RELATION_OF_CREATOR(Fields::ugwCreatedBy_usrID),
            ORM_RELATION_OF_UPDATER(Fields::ugwUpdatedBy_usrID),
        };

        const QList<stuDBIndex> Indexes = {
        };

    } //namespace Private

    TAPI_DEFINE_STRUCT(DTO,
        SF_ORM_PRIMARYKEY_32        (ugwID),
        SF_QString                  (ugwName),
        SF_NULLABLE_Enum            (ugwType, enuUploadGatewayType),
        SF_QString                  (ugwBucket),
        SF_QString                  (ugwEndpointUrl),
        SF_bool                     (ugwEndpointIsVirtualHosted),
        SF_JSON_t                   (ugwMetaInfo),
        SF_QString                  (ugwAllowedFileTypes),
        SF_QString                  (ugwAllowedMimeTypes),
        SF_NULLABLE_quint64         (ugwAllowedMinFileSize),
        SF_NULLABLE_quint64         (ugwAllowedMaxFileSize),
        SF_NULLABLE_quint64         (ugwMaxFilesCount),
        SF_NULLABLE_quint64         (ugwMaxFilesSize),
        SF_quint64                  (ugwCreatedFilesCount),
        SF_quint64                  (ugwCreatedFilesSize),
        SF_quint64                  (ugwDeletedFilesCount),
        SF_quint64                  (ugwDeletedFilesSize),
        SF_DateTime_t               (ugwLastActionTime),
        SF_ORM_STATUS_FIELD         (ugwStatus, enuUploadGatewayStatus, enuUploadGatewayStatus::Active),
        SF_ORM_CREATED_ON           (ugwCreationDateTime),
        SF_ORM_CREATED_BY           (ugwCreatedBy_usrID),
        SF_ORM_UPDATED_BY           (ugwUpdatedBy_usrID)
    );
}

namespace tblUploadQueue {
    constexpr char Name[] = "tblUploadQueue";

    namespace Fields {
        TARGOMAN_CREATE_CONSTEXPR(uquID);
        TARGOMAN_CREATE_CONSTEXPR(uqu_uflID);
        TARGOMAN_CREATE_CONSTEXPR(uqu_ugwID);
        TARGOMAN_CREATE_CONSTEXPR(uquLockedAt);
        TARGOMAN_CREATE_CONSTEXPR(uquLockedBy);
        TARGOMAN_CREATE_CONSTEXPR(uquLastTryAt);
        TARGOMAN_CREATE_CONSTEXPR(uquStoredAt);
        TARGOMAN_CREATE_CONSTEXPR(uquResult);
        TARGOMAN_CREATE_CONSTEXPR(uquStatus);
        TARGOMAN_CREATE_CONSTEXPR(uquCreationDateTime);
        TARGOMAN_CREATE_CONSTEXPR(uquCreatedBy_usrID);
        TARGOMAN_CREATE_CONSTEXPR(uquUpdatedBy_usrID);
    }

    namespace Relation {
        constexpr char File[] = "file";
        constexpr char Gateway[] = "gateway";
    }

    namespace Private {
        const QList<clsORMField> ORMFields = {
            ///ColName                               Type                    Validation                  Default     UpBy    Sort  Filter Self  Virt   PK
            { Fields::uquID,                ORM_PRIMARYKEY_64 },
            { Fields::uqu_uflID,            S(quint64),             QFV.integer().minValue(1),  QRequired,  UPNone },
            { Fields::uqu_ugwID,            S(quint32),             QFV.integer().minValue(1),  QRequired,  UPNone },
            { Fields::uquLockedAt,          S(TAPI::DateTime_t),    QFV,                        QNull,      UPAdmin },
            { Fields::uquLockedBy,          S(QString),             QFV,                        QNull,      UPAdmin },
            { Fields::uquLastTryAt,         S(TAPI::DateTime_t),    QFV,                        QNull,      UPAdmin },
            { Fields::uquStoredAt,          S(TAPI::DateTime_t),    QFV,                        QNull,      UPAdmin },
            { Fields::uquResult,            S(QString),             QFV,                        QNull,      UPAdmin },
            { Fields::uquStatus,            ORM_STATUS_FIELD(Targoman::API::ObjectStorage::ORM::enuUploadQueueStatus, Targoman::API::ObjectStorage::ORM::enuUploadQueueStatus::New) },
            { Fields::uquCreationDateTime,  ORM_CREATED_ON },
            { Fields::uquCreatedBy_usrID,   ORM_CREATED_BY },
            { Fields::uquUpdatedBy_usrID,   ORM_UPDATED_BY },
        };

        inline const QList<stuRelation> Relations(const QString &_schema) {
            ///<                                       Col                             Reference Table                         ForeignCol              Rename      LeftJoin
            return {
                { Relation::File,       { Fields::uqu_uflID,    R(_schema, tblUploadFiles::Name),       tblUploadFiles::Fields::uflID } },
                { Relation::Gateway,    { Fields::uqu_ugwID,    R(_schema, tblUploadGateways::Name),    tblUploadGateways::Fields::ugwID } },
                ORM_RELATION_OF_CREATOR(Fields::uquCreatedBy_usrID),
                ORM_RELATION_OF_UPDATER(Fields::uquUpdatedBy_usrID),
            };
        };

        const QList<stuDBIndex> Indexes = {
        };

    } //namespace Private

    TAPI_DEFINE_STRUCT(DTO,
        SF_ORM_PRIMARYKEY_64        (uquID),
        SF_quint64                  (uqu_uflID),
        SF_quint32                  (uqu_ugwID),
        SF_DateTime_t               (uquLockedAt),
        SF_QString                  (uquLockedBy),
        SF_DateTime_t               (uquLastTryAt),
        SF_DateTime_t               (uquStoredAt),
        SF_QString                  (uquResult),
        SF_ORM_STATUS_FIELD         (uquStatus, enuUploadQueueStatus, enuUploadQueueStatus::New),
        SF_ORM_CREATED_ON           (uquCreationDateTime),
        SF_ORM_CREATED_BY           (uquCreatedBy_usrID),
        SF_ORM_UPDATED_BY           (uquUpdatedBy_usrID)
    );
}

#pragma GCC diagnostic pop

namespace Private {

struct stuProcessUploadQueueInfo
{
    tblUploadFiles::DTO UploadFiles;
    tblUploadGateways::DTO UploadGateways;
    tblUploadQueue::DTO UploadQueue;

    void fromVariantMap(const QVariantMap &_info);
};

} //namespace Private

class intfUploadFiles : public intfSQLBasedModule
{
    Q_OBJECT

public:
    intfUploadFiles(
        const QString& _module,
        const QString& _schema
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

class intfUploadGateways : public intfSQLBasedModule
{
    Q_OBJECT

public:
    intfUploadGateways(
        const QString& _module,
        const QString& _schema
    );

private slots:
    QVariant ORMGET("Get object storage gateway information")
    quint32 ORMCREATE("Create a new object storage gateway by an authorized user")
    bool ORMUPDATE("Update object storage gateway info by an authorized user")
    bool ORMDELETE("Delete an object storage gateway")
};

class intfUploadQueue : public intfSQLBasedModule
{
    Q_OBJECT

public:
    intfUploadQueue(
        const QString& _module,
        const QString& _schema
    );

private slots:
    QVariant ORMGET("Get upload queue")
};

} //namespace Targoman::API::ObjectStorage::ORM

/****************************************************/
//put this macro before module class definition (.h)
#define TARGOMAN_OBJECTSTORAGE_PREPARENT \
    class UploadFiles; \
    class UploadGateways; \
    class UploadQueue;

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
};

//put this macro inside module class definition (.h) after TARGOMAN_DEFINE_API_MODULE
#define TARGOMAN_API_DEFINE_OBJECTSTORAGE(_module, _schema) \
protected: \
    QScopedPointer<UploadFiles>     _UploadFiles;    \
    QScopedPointer<UploadGateways>  _UploadGateways; \
    QScopedPointer<UploadQueue>     _UploadQueue;    \
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
    this->_UploadGateways.reset(&UploadGateways::instance());   \
    this->_UploadQueue   .reset(&UploadQueue   ::instance());   \
    this->addSubModule(this->_UploadFiles.data());              \
    this->addSubModule(this->_UploadGateways.data());           \
    this->addSubModule(this->_UploadQueue.data());

//    UploadFiles    ::instance().prepareFiltersList();
//    UploadGateways ::instance().prepareFiltersList();
//    UploadQueue    ::instance().prepareFiltersList();

/****************************************************/
#endif // TARGOMAN_API_OBJECTSTORAGE_H
