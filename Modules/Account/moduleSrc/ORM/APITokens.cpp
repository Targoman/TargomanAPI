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

#include "APITokens.h"
#include "User.h"
#include "../Account.h"
#include "Service.h"
#include "Interfaces/AAA/clsJWT.hpp"
#include "Interfaces/Helpers/SecurityHelper.h"
using namespace Targoman::API::AAA;
using namespace Targoman::API::Helpers;

TAPI_REGISTER_TARGOMAN_ENUM(Targoman::API::AccountModule, enuAPITokensStatus);

TAPI_REGISTER_METATYPE_TYPE_STRUCT(
    /* namespace          */ Targoman::API::AccountModule,
    /* type               */ stuRequestTokenResult
);

namespace Targoman::API::AccountModule::ORM {

/******************************************************/
TARGOMAN_API_SUBMODULE_IMPLEMENT(Account, APITokens);

APITokens::APITokens() :
    intfSQLBasedModule(
        AAASchema,
        tblAPITokens::Name,
        tblAPITokens::Private::ORMFields,
        tblAPITokens::Private::Relations,
        tblAPITokens::Private::Indexes
) { ; }

ORMSelectQuery APITokens::makeSelectQuery(INTFAPICALLBOOM_IMPL &APICALLBOOM_PARAM, const QString &_alias, Q_DECL_UNUSED bool _translate, Q_DECL_UNUSED bool _isRoot) {

    ORMSelectQuery Query = intfSQLBasedModule::makeSelectQuery(APICALLBOOM_PARAM, _alias, _translate);

    if (_isRoot) {
        Query.addCols(this->selectableColumnNames())
                .nestedLeftJoin(APITokenServices::instance().makeSelectQuery(APICALLBOOM_PARAM, "", _translate, false)
                                .addCol(tblAPITokenServices::Fields::aptsvc_aptID, tblAPITokenServices::Fields::aptsvc_aptID)
                                .addCol(DBExpression::VALUE("GROUP_CONCAT(tblAPITokenServices.aptsvc_svcID)"), "ServiceIDs")
                                .addCol(DBExpression::VALUE("GROUP_CONCAT(tblService.svcName)"), "ServiceNames")
                                .innerJoinWith(tblAPITokenServices::Relation::Service)
                                .groupBy(tblAPITokenServices::Fields::aptsvc_aptID)
                                , "tmpServices"
                                , { "tmpServices", tblAPITokenServices::Fields::aptsvc_aptID,
                                    enuConditionOperator::Equal,
                                    tblAPITokens::Name, tblAPITokens::Fields::aptID }
                                )
                .addCol("tmpServices.ServiceIDs")
                .addCol("tmpServices.ServiceNames")
        ;
    }

    return Query;
}

QVariant IMPL_ORMGET_USER(APITokens) {
    if (Authorization::hasPriv(APICALLBOOM_PARAM, this->privOn(EHTTP_GET, this->moduleBaseName())) == false)
        this->setSelfFilters({{tblAPITokens::Fields::apt_usrID, APICALLBOOM_PARAM.getActorID()}}, _filters);

    return this->Select(GET_METHOD_ARGS_CALL_VALUES);
}

//quint64 IMPL_ORMCREATE_USER(APITokens) {
//    Authorization::checkPriv(APICALLBOOM_PARAM, this->privOn(EHTTP_PUT, this->moduleBaseName()));

//    return this->Create(CREATE_METHOD_ARGS_CALL_VALUES);
//}

//bool IMPL_ORMUPDATE_USER(APITokens) {
//    Authorization::checkPriv(APICALLBOOM_PARAM, this->privOn(EHTTP_PATCH, this->moduleBaseName()));

//    return this->Update(UPDATE_METHOD_ARGS_CALL_VALUES);
//}

//bool IMPL_ORMDELETE_USER(APITokens) {
//    TAPI::ORMFields_t ExtraFilters;
//    if (Authorization::hasPriv(APICALLBOOM_PARAM, this->privOn(EHTTP_DELETE, this->moduleBaseName())) == false)
//        ExtraFilters.insert(tblAPITokens::Fields::apt_usrID, APICALLBOOM_PARAM.getActorID());
////    this->setSelfFilters({{tblAPITokens::Fields::apt_usrID, APICALLBOOM_PARAM.getActorID()}}, ExtraFilters);

//    return this->DeleteByPks(DELETE_METHOD_ARGS_CALL_VALUES, ExtraFilters);
//}

stuRequestTokenResult APITokens::create(
    INTFAPICALLBOOM_IMPL &APICALLBOOM_PARAM,
//    APICALLBOOM_TYPE_JWT_USER_IMPL &APICALLBOOM_PARAM,
    quint64 _userID,
    const QString &_name,
    const QStringList &_services
) {
    APITokenServices::instance().prepareFiltersList();

    //1: list service ids
    QList<quint32> ServiceIDs;
    if (_services.isEmpty() == false) {
        TAPI::stuTable ServicesRows = Service::instance().makeSelectQuery(APICALLBOOM_PARAM)
                                      .where({ tblService::Fields::svcName,
                                               enuConditionOperator::In,
                                               "'" + _services.join("','") + "'" })
                                      .all();

        if (ServicesRows.Rows.count() != _services.count())
            throw exHTTPExpectationFailed("Some services were not found");

        foreach (QVariant Row, ServicesRows.Rows) {
            tblService::DTO ServiceDTO;
            ServiceDTO.fromJson(QJsonObject::fromVariantMap(Row.toMap()));

//            if (ServiceDTO.svcAcceptableTokenType != enuTokenActorType::API)
//                throw exHTTPExpectationFailed(QString("Acceptable Token Type of service `%1` is not API").arg(ServiceDTO.svcName));

            ServiceIDs.append(ServiceDTO.svcID);
        }
    }

    //temp
    TAPI::EncodedJWT_t JWT = "TEMP_" + SecurityHelper::UUIDtoMD5();

    //2: insert into tblAPITokens
    ORMCreateQuery CreateQuery = this->makeCreateQuery(APICALLBOOM_PARAM)
                                 .addCol(tblAPITokens::Fields::aptToken)
                                 .addCol(tblAPITokens::Fields::aptName)
                                 .addCol(tblAPITokens::Fields::apt_usrID)
//                           .addCol(tblAPITokens::Fields::aptLang)
//                           .addCol(tblAPITokens::Fields::aptValidateIP)
//                           .addCol(tblAPITokens::Fields::aptExtraPrivileges)
//                           .addCol(tblAPITokens::Fields::aptExpiryDate)
//                           .addCol(tblAPITokens::Fields::aptLastActivity)
//                           .addCol(tblAPITokens::Fields::aptAccessCount)
                           .addCol(tblAPITokens::Fields::aptStatus)
//                           .addCol(tblAPITokens::Fields::aptCreationDateTime)
//                           .addCol(tblAPITokens::Fields::aptCreatedBy_usrID)
//                           .addCol(tblAPITokens::Fields::aptUpdatedBy_usrID)

                           .values({
                                       { tblAPITokens::Fields::aptToken, JWT },
                                       { tblAPITokens::Fields::aptName, _name },
                                       { tblAPITokens::Fields::apt_usrID, _userID },
                                       { tblAPITokens::Fields::aptStatus, enuAPITokensStatus::Pending },

                                   })
                           ;
    qint64 APITokenID = CreateQuery.execute(_userID); //APICALLBOOM_PARAM.getActorID());

    //3: create jwt
    QJsonObject Privs;

    QJsonObject Payload = {
//        { JWTItems::iat,             },
        { JWTItems::own,            QString::number(_userID) }, //APICALLBOOM_PARAM.getActorID() },
        { JWTItems::uid,            APITokenID },
        { JWTItems::privs,          Privs },
//        { JWTItems::usrStatus,      TAPI::enuUserStatus::toStr(_activeAccount.Privs["usrStatus"].toString()) },
//        { JWTItems::rolID,          _activeAccount.Privs["usr_rolID"] },
//        { JWTItems::rolName,        _activeAccount.Privs["rolName"] },
    };

    qint64 TTL = 1 * 365 * 24 * 3600; //1 year

    JWT = QJWT::createSigned(
        Payload,
        enuTokenActorType::API,
        QJsonObject({ { "svc", _services.join(",") } }),
        TTL
//        _activeAccount.Privs["ssnKey"].toString()
    );

    //4: update tblAPITokens
    this->makeUpdateQuery(APICALLBOOM_PARAM)
            .set(tblAPITokens::Fields::aptToken, JWT)
            .set(tblAPITokens::Fields::aptStatus, enuAPITokensStatus::Active)
            .setPksByPath((quint64)APITokenID)
            .execute(_userID) //APICALLBOOM_PARAM.getActorID())
            ;

    //5: create tblAPITokenServices
    if (ServiceIDs.isEmpty() == false) {
        ORMCreateQuery CreateQueryServices = APITokenServices::instance().makeCreateQuery(APICALLBOOM_PARAM)
                                             .addCol(tblAPITokenServices::Fields::aptsvc_aptID)
                                             .addCol(tblAPITokenServices::Fields::aptsvc_svcID)
                                             ;

        foreach (quint32 SvcID, ServiceIDs) {
            CreateQueryServices.values({
                                           { tblAPITokenServices::Fields::aptsvc_aptID, APITokenID },
                                           { tblAPITokenServices::Fields::aptsvc_svcID, SvcID },
                                       });
        }

        CreateQueryServices.execute(_userID);
    }

    //--
    return stuRequestTokenResult(APITokenID, JWT);
}

Targoman::API::AccountModule::stuRequestTokenResult IMPL_REST_GET_OR_POST(APITokens, request, (
    APICALLBOOM_TYPE_JWT_USER_IMPL &APICALLBOOM_PARAM,
    const QString &_name,
    const QStringList &_services
)) {
    return this->create(
                APICALLBOOM_PARAM,
                APICALLBOOM_PARAM.getActorID(),
                _name,
                _services
                );
}

/******************************************************/
TARGOMAN_API_SUBMODULE_IMPLEMENT(Account, APITokenServices)

APITokenServices::APITokenServices() :
    intfSQLBasedModule(
        AAASchema,
        tblAPITokenServices::Name,
        tblAPITokenServices::Private::ORMFields,
        tblAPITokenServices::Private::Relations,
        tblAPITokenServices::Private::Indexes
) { ; }

//QVariant IMPL_ORMGET_USER(APITokenServices) {
//    if (Authorization::hasPriv(APICALLBOOM_PARAM, this->privOn(EHTTP_GET, this->moduleBaseName())) == false)
//        this->setSelfFilters({{tblAPITokenServices::Fields::apt_usrID, APICALLBOOM_PARAM.getActorID()}}, _filters);

//    return this->Select(GET_METHOD_ARGS_CALL_VALUES);
//}

//quint64 IMPL_ORMCREATE_USER(APITokenServices) {
//    Authorization::checkPriv(APICALLBOOM_PARAM, this->privOn(EHTTP_PUT, this->moduleBaseName()));

//    return this->Create(CREATE_METHOD_ARGS_CALL_VALUES);
//}

//bool IMPL_ORMDELETE_USER(APITokenServices) {
//    TAPI::ORMFields_t ExtraFilters;

//    if (Authorization::hasPriv(APICALLBOOM_PARAM, this->privOn(EHTTP_DELETE, this->moduleBaseName())) == false)
//        ExtraFilters.insert(tblAPITokenServices::Fields::apt_usrID, APICALLBOOM_PARAM.getActorID());

//    return this->DeleteByPks(DELETE_METHOD_ARGS_CALL_VALUES, ExtraFilters);
//}

/******************************************************/
TARGOMAN_API_SUBMODULE_IMPLEMENT(Account, APITokenValidIPs)

APITokenValidIPs::APITokenValidIPs() :
    intfSQLBasedModule(
        AAASchema,
        tblAPITokenValidIPs::Name,
        tblAPITokenValidIPs::Private::ORMFields,
        tblAPITokenValidIPs::Private::Relations,
        tblAPITokenValidIPs::Private::Indexes
) { ; }

QVariant IMPL_ORMGET_USER(APITokenValidIPs) {
    if (Authorization::hasPriv(APICALLBOOM_PARAM, this->privOn(EHTTP_GET, this->moduleBaseName())) == false)
        this->setSelfFilters({{tblAPITokens::Fields::apt_usrID, APICALLBOOM_PARAM.getActorID()}}, _filters);

    auto fnTouchQuery = [](ORMSelectQuery &_query) {
        _query.innerJoin(tblAPITokens::Name);
    };

    return this->Select(GET_METHOD_ARGS_CALL_VALUES, {}, 0, fnTouchQuery);
}

quint64 IMPL_ORMCREATE_USER(APITokenValidIPs) {
    Authorization::checkPriv(APICALLBOOM_PARAM, this->privOn(EHTTP_PUT, this->moduleBaseName()));

    return this->Create(CREATE_METHOD_ARGS_CALL_VALUES);
}

bool IMPL_ORMUPDATE_USER(APITokenValidIPs) {
    TAPI::ORMFields_t ExtraFilters;
    if (Authorization::hasPriv(APICALLBOOM_PARAM, this->privOn(EHTTP_PATCH,this->moduleBaseName())) == false)
        ExtraFilters.insert(tblAPITokens::Fields::apt_usrID, APICALLBOOM_PARAM.getActorID());

//    this->setSelfFilters({{tblAPITokens::Fields::apt_usrID, APICALLBOOM_PARAM.getActorID()}}, ExtraFilters);

    return this->Update(UPDATE_METHOD_ARGS_CALL_VALUES, ExtraFilters);
}

bool IMPL_ORMDELETE_USER(APITokenValidIPs) {
    TAPI::ORMFields_t ExtraFilters;

    if (Authorization::hasPriv(APICALLBOOM_PARAM, this->privOn(EHTTP_DELETE, this->moduleBaseName())) == false)
        ExtraFilters.insert(tblAPITokens::Fields::apt_usrID, APICALLBOOM_PARAM.getActorID());
//    this->setSelfFilters({{tblAPITokens::Fields::apt_usrID, APICALLBOOM_PARAM.getActorID()}}, ExtraFilters);

    return this->DeleteByPks(DELETE_METHOD_ARGS_CALL_VALUES, ExtraFilters, true);
//    return this->deleteByPKs(DELETE_METHOD_CALL_ARGS_APICALL, ExtraFilters, true);
}

/******************************************************/
} //namespace Targoman::API::AccountModule::ORM
