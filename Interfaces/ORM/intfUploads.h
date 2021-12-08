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

#ifndef TARGOMAN_API_UPLOADS_H
#define TARGOMAN_API_UPLOADS_H

#include "Interfaces/API/intfSQLBasedModule.h"
#include "libTargomanCommon/Configuration/tmplConfigurable.h"
using namespace Targoman::Common;

#include "Interfaces/Helpers/ObjectStorageHelper.h"
using namespace Targoman::API::Helpers;

using namespace Targoman::API::DBM;
using namespace Targoman::API::API;

namespace Targoman::API::ORM {

TARGOMAN_DEFINE_ENHANCED_ENUM(enuS3Provider,
                              Amazon,
                              Arvan
                              )

TARGOMAN_DEFINE_ENUM(enuUploadStatus,
                     Queued    = 'Q',
                     Uploaded  = 'U',
                     Removed   = 'R'
                     )

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-variable"
namespace tblUploads {
    constexpr char Name[] = "tblUploads";
    TARGOMAN_CREATE_CONSTEXPR(uplID);
    TARGOMAN_CREATE_CONSTEXPR(uplURL);
    TARGOMAN_CREATE_CONSTEXPR(uplFileName);
    TARGOMAN_CREATE_CONSTEXPR(uplSize);
    TARGOMAN_CREATE_CONSTEXPR(uplMimeType);
    TARGOMAN_CREATE_CONSTEXPR(uplTempFullFileName);
    TARGOMAN_CREATE_CONSTEXPR(uplUploadLastErrorMessage);
    TARGOMAN_CREATE_CONSTEXPR(uplStatus);
    TARGOMAN_CREATE_CONSTEXPR(uplCreationDateTime);
    TARGOMAN_CREATE_CONSTEXPR(uplCreatedBy_usrID);
    TARGOMAN_CREATE_CONSTEXPR(uplUpdatedBy_usrID);
}
#pragma GCC diagnostic pop

class intfUploads : public intfSQLBasedModule
{
    Q_OBJECT

public:
    intfUploads(
            const QString &_schema,
            const QString &_name
        );

    struct stuObjectStorageConfigs
    {
        enuS3Provider::Type Provider;
        QString             Bucket;
        QString             Region;

        stuObjectStorageConfigs(const stuObjectStorageConfigs &_other) :
            Provider(_other.Provider),
            Bucket(_other.Bucket),
            Region(_other.Region)
        {}
        stuObjectStorageConfigs(const enuS3Provider::Type _provider, const QString &_bucket, const QString &_region) :
            Provider(_provider),
            Bucket(_bucket),
            Region(_region)
        {}
    };

    virtual stuObjectStorageConfigs getObjectStorageConfigs() const = 0;

protected slots:
//    QVariant ORMGET("Get Uploads.")

    Targoman::API::Helpers::stuSaveFileResult REST_PUT(
        save,
        (
            const TAPI::JWT_t &_JWT,
            QString _fileName,
            const QString &_base64Content
        ),
        "Saves the newly uploaded file to the file server and places the file header in the database,"
        " then returns the saved file ID in table."
    )
};

} //namespace Targoman::API::ORM

TAPI_DECLARE_METATYPE_ENUM(Targoman::API::ORM, enuUploadStatus);
TAPI_DECLARE_METATYPE_ENUM(Targoman::API::ORM, enuS3Provider);
ENUM_CONFIGURABLE(Targoman::API::ORM::enuS3Provider)

/****************************************************/
//put this macro before module class definition (.h)
#define TARGOMAN_UPLOAD_PREPARENT class Uploads;

//put this macro after module class definition (.h)
#define TARGOMAN_UPLOAD_POSTPARENT(_module, _schema) \
class Uploads : public Targoman::API::ORM::intfUploads \
{ \
    Q_OBJECT \
    TARGOMAN_DEFINE_API_SUBMODULE_WO_CTOR(_module, Uploads) \
public: \
    Uploads() : \
        intfUploads( \
            Targoman::Common::demangle(typeid(_module).name()).split("::").last(), \
            _schema \
        ) \
    {} \
    virtual intfUploads::stuObjectStorageConfigs getObjectStorageConfigs() const { \
        return intfUploads::stuObjectStorageConfigs( \
            _module::ObjectStorage::Provider.value(), \
            _module::ObjectStorage::Bucket.value(), \
            _module::ObjectStorage::Region.value() \
        ); \
    } \
};

//put this macro inside module class definition (.h) after TARGOMAN_DEFINE_API_MODULE
#define TARGOMAN_API_DEFINE_UPLOAD(_module, _schema) \
protected: \
    QScopedPointer<Uploads> _Uploads; \
public: \
    struct ObjectStorage { \
        static inline QString makeConfig(const QString& _name) { return QString("/Module_%1/ObjectStorage/%2").arg(TARGOMAN_M2STR(_module), _name); } \
        static Targoman::Common::Configuration::tmplConfigurable<enuS3Provider::Type> Provider; \
        static Targoman::Common::Configuration::tmplConfigurable<QString>             Bucket;   \
        static Targoman::Common::Configuration::tmplConfigurable<QString>             Region;   \
    };

//put this macro before module class constructor (.cpp)
#define TARGOMAN_API_UPLOAD_CONFIG_IMPL(_module, _schema)                   \
    using namespace Targoman::Common::Configuration;                        \
    tmplConfigurable<enuS3Provider::Type> _module::ObjectStorage::Provider( \
        _module::ObjectStorage::makeConfig("Provider"),                     \
        "Provider",                                                         \
        enuS3Provider::Amazon                                               \
    );                                                                      \
    tmplConfigurable<QString> _module::ObjectStorage::Bucket(               \
        _module::ObjectStorage::makeConfig("Bucket"),                       \
        "Bucket",                                                           \
        ""                                                                  \
    );                                                                      \
    tmplConfigurable<QString> _module::ObjectStorage::Region(               \
        _module::ObjectStorage::makeConfig("Region"),                       \
        "Region",                                                           \
        ""                                                                  \
    );                                                                      \

//put this macro into module class constructor (.cpp)
#define TARGOMAN_API_IMPLEMENT_UPLOAD(_module, _schema) \
    this->_Uploads.reset(&Uploads::instance()); \
    this->addSubModule(this->_Uploads.data());

/****************************************************/
#endif // TARGOMAN_API_UPLOADS_H
