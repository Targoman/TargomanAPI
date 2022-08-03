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

#include "ObjectStorage.h"
#include "Interfaces/AAA/Authorization.h"
#include "Interfaces/AAA/AAADefs.hpp"
#include "Interfaces/AAA/clsJWT.hpp"
using namespace Targoman::API::AAA;

#include "Interfaces/Common/QtTypes.hpp"

TAPI_REGISTER_TARGOMAN_ENUM(Targoman::API::ObjectStorage::ORM, enuUploadFileStatus);
TAPI_REGISTER_TARGOMAN_ENUM(Targoman::API::ObjectStorage::ORM, enuUploadGatewayType);
TAPI_REGISTER_TARGOMAN_ENUM(Targoman::API::ObjectStorage::ORM, enuUploadGatewayStatus);
TAPI_REGISTER_TARGOMAN_ENUM(Targoman::API::ObjectStorage::ORM, enuUploadQueueStatus);

namespace Targoman::API::ObjectStorage::ORM {

namespace Private {

void stuProcessUploadQueueInfo::fromVariantMap(const QVariantMap& _info) {
    QJsonObject JsonInfo = QJsonObject::fromVariantMap(_info);

    this->UploadFiles.fromJson(JsonInfo);
    this->UploadGateways.fromJson(JsonInfo);
    this->UploadQueue.fromJson(JsonInfo);
}

} //namespace Private

/******************************************************************/
/******************************************************************/
/******************************************************************/
intfUploadFiles::intfUploadFiles(
    const QString& _module,
    const QString& _schema
) :
    intfSQLBasedModule(
        _module,
        _schema,
        tblUploadFiles::Name,
        tblUploadFiles::Private::ORMFields,
        tblUploadFiles::Private::Relations,
        tblUploadFiles::Private::Indexes
) { ; }

QVariant IMPL_ORMGET(intfUploadFiles) {
    Authorization::checkPriv(APICALLBOOM_PARAM, this->privOn(EHTTP_GET, this->moduleBaseName()));
    return this->Select(GET_METHOD_ARGS_CALL_VALUES);
}

/******************************************************************/
/******************************************************************/
/******************************************************************/
intfUploadGateways::intfUploadGateways(
    const QString& _module,
    const QString& _schema
) :
    intfSQLBasedModule(
        _module,
        _schema,
        tblUploadGateways::Name,
        tblUploadGateways::Private::ORMFields,
        tblUploadGateways::Private::Relations,
        tblUploadGateways::Private::Indexes
) { ; }

QVariant IMPL_ORMGET(intfUploadGateways) {
    Authorization::checkPriv(APICALLBOOM_PARAM, this->privOn(EHTTP_GET, this->moduleBaseName()));
    return this->Select(GET_METHOD_ARGS_CALL_VALUES);
}

quint32 IMPL_ORMCREATE(intfUploadGateways) {
    Authorization::checkPriv(APICALLBOOM_PARAM, this->privOn(EHTTP_PUT, this->moduleBaseName()));

//    if (_createInfo.contains(tblintfUploadGateways::Fields::ugwAllowedDomainName))
//        _createInfo[tblintfUploadGateways::Fields::ugwAllowedDomainName] = URLHelper::domain(_createInfo[tblUploadGateways::Fields::ugwAllowedDomainName].toString());

    return this->Create(CREATE_METHOD_ARGS_CALL_VALUES);
}

bool IMPL_ORMUPDATE(intfUploadGateways) {
    Authorization::checkPriv(APICALLBOOM_PARAM, this->privOn(EHTTP_PATCH, this->moduleBaseName()));
    return this->Update(UPDATE_METHOD_ARGS_CALL_VALUES);
}

bool IMPL_ORMDELETE(intfUploadGateways) {
    Authorization::checkPriv(APICALLBOOM_PARAM, this->privOn(EHTTP_DELETE, this->moduleBaseName()));
    return this->DeleteByPks(DELETE_METHOD_ARGS_CALL_VALUES);
}

/******************************************************************/
/******************************************************************/
/******************************************************************/
intfUploadQueue::intfUploadQueue(
    const QString& _module,
    const QString& _schema
) :
    intfSQLBasedModule(
        _module,
        _schema,
        tblUploadQueue::Name,
        tblUploadQueue::Private::ORMFields,
        tblUploadQueue::Private::Relations(_schema),
        tblUploadQueue::Private::Indexes
) { ; }

QVariant IMPL_ORMGET(intfUploadQueue) {
    Authorization::checkPriv(APICALLBOOM_PARAM, this->privOn(EHTTP_GET, this->moduleBaseName()));
    return this->Select(GET_METHOD_ARGS_CALL_VALUES);
}

} //namespace Targoman::API::ObjectStorage::ORM
