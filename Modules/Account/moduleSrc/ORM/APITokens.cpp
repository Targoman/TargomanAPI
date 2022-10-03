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
#include "TokenBin.h"
//#include "User.h"
#include "../Account.h"
#include "Service.h"
#include "Interfaces/AAA/clsJWT.hpp"
#include "Interfaces/Helpers/SecurityHelper.h"
#include "Interfaces/Helpers/TokenHelper.h"
#include "Interfaces/Server/QJWT.h"
#include "libTargomanDBM/clsDAC.h"

TAPI_REGISTER_TARGOMAN_ENUM(Targoman::API::AccountModule, enuAPITokensStatus);

TAPI_REGISTER_METATYPE_TYPE_STRUCT(
    /* namespace          */ Targoman::API::AccountModule,
    /* type               */ stuRequestTokenResult
);

namespace Targoman::API::AccountModule::ORM {

using namespace AAA;
using namespace Helpers;
using namespace Server;
using namespace DBManager;

/******************************************************/
/******************************************************/
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

//void APITokens::initializeModule()
//{
//    APITokenServices::instance().prepareFiltersList();
//}

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

stuRequestTokenResult APITokens::create(
    INTFAPICALLBOOM_IMPL &APICALLBOOM_PARAM,
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
    quint64 APITokenID = 0;
    try {
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
        APITokenID = CreateQuery.execute(_userID); //APICALLBOOM_PARAM.getActorID());

    }  catch (const std::exception &_exp) {
        QString ExpStr = _exp.what();
        if (ExpStr.contains("Duplicate entry", Qt::CaseInsensitive))
            throw /*exHTTPConflict*/exHTTPBadRequest("This name has already been used");

        throw;
    }

    //3: create jwt
    QJsonObject Privs;

    QJsonObject Payload = {
//        { JWTItems::iat,             },
        { JWTItems::own,            static_cast<double>(_userID) }, //APICALLBOOM_PARAM.getActorID() },
        { JWTItems::uid,            static_cast<double>(APITokenID) },
        { JWTItems::privs,          Privs },
//        { JWTItems::usrStatus,      TAPI::enuUserStatus::toStr(_activeAccount.Privs["usrStatus"].toString()) },
//        { JWTItems::rolID,          _activeAccount.Privs["usr_rolID"] },
//        { JWTItems::rolName,        _activeAccount.Privs["rolName"] },
    };

    JWT = QJWT::createSigned(
        Payload,
        enuTokenActorType::API,
        QJsonObject({ { "svc", _services.join(",") } }),
        TokenHelper::APITokenTTL.value() * 24 * 3600
//        _activeAccount.Privs["ssnKey"].toString()
    );

    //4: update tblAPITokens
    this->makeUpdateQuery(APICALLBOOM_PARAM)
            .set(tblAPITokens::Fields::aptToken, JWT)
            .set(tblAPITokens::Fields::aptStatus, enuAPITokensStatus::Active)
            .setPksByPath(APITokenID)
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

QVariantMap APITokens::addServices(
    INTFAPICALLBOOM_IMPL &APICALLBOOM_PARAM,
    quint64 _tokenID,
    QMap<QString, quint64> &_services
) {
    if (_services.isEmpty())
        return {};

    tblAPITokens::DTO APITokensDTO = this->makeSelectQuery(APICALLBOOM_PARAM)
                                     .where({ tblAPITokens::Fields::aptID, enuConditionOperator::Equal, _tokenID })
                                     .one<tblAPITokens::DTO>();

    QList<quint32> CurrentServiceIDs;
    QStringList CurrentServiceNames;

    auto IDParts = APITokensDTO.ServiceIDs.split(",", QString::SkipEmptyParts);
    foreach (auto ID, IDParts)
        CurrentServiceIDs.append(ID.toUInt());

    auto NameParts = APITokensDTO.ServiceNames.split(",", QString::SkipEmptyParts);
    foreach (auto Name, NameParts) {
        CurrentServiceNames.append(Name);

        if (_services.contains(Name))
            _services.remove(Name);
    }

    if (_services.isEmpty())
        return {};

    //find service ids
    //----------------------------------------
    QStringList ServicesWithoutID;

    for (auto it = _services.constBegin();
         it != _services.constEnd();
         it++
    ) {
        if (it.value() == 0)
            ServicesWithoutID.append(it.key());
    }

    if (ServicesWithoutID.isEmpty() == false) {
        QVariantList Services = Service::instance().makeSelectQuery(APICALLBOOM_PARAM)
                                .where({ tblService::Fields::svcName, enuConditionOperator::Like, ServicesWithoutID.join(',') })
                                .all().Rows;

        foreach (auto Service, Services) {
            tblService::DTO ServiceDTO;
            ServiceDTO.fromJson(QJsonObject::fromVariantMap(Service.toMap()));

            _services[ServiceDTO.svcName] = ServiceDTO.svcID;
        }
    }

    //----------------------------------------
    for (auto it = _services.constBegin();
         it != _services.constEnd();
         it++
    ) {
        if (it.value() == 0)
            throw exHTTPBadRequest(QString("Service `%1` not found").arg(it.key()));
    }

    //----------------------------------------
    QVariantMap MethodResult;

    //create newtoken
    //----------------------------------------
    TAPI::JWT_t TokenJWTPayload;
    QJWT::extractAndDecryptPayload(APITokensDTO.aptToken, TokenJWTPayload);

    QJsonObject PrivatePayload;
    if (TokenJWTPayload.contains("prv"))
        PrivatePayload = TokenJWTPayload["prv"].toObject();
    PrivatePayload["svc"] = (CurrentServiceNames + _services.keys()).join(",");

    TokenJWTPayload["prv"] = PrivatePayload;
    QString NewToken = QJWT::encryptAndSigned(TokenJWTPayload);

    MethodResult.insert("token", NewToken);

    //save newtoken
    //----------------------------------------
    this->makeUpdateQuery(APICALLBOOM_PARAM)
            .set(tblAPITokens::Fields::aptToken, NewToken)
            .setPksByPath(APITokensDTO.aptID)
            .execute(APICALLBOOM_PARAM.getActorID())
            ;

    //save into tblAPITokenServices
    //----------------------------------------
    ORMCreateQuery CreateQueryServices = APITokenServices::instance().makeCreateQuery(APICALLBOOM_PARAM)
                                         .addCol(tblAPITokenServices::Fields::aptsvc_aptID)
                                         .addCol(tblAPITokenServices::Fields::aptsvc_svcID)
                                         ;

    for (auto it = _services.constBegin();
         it != _services.constEnd();
         it++
    ) {
        CreateQueryServices.values({
                                       { tblAPITokenServices::Fields::aptsvc_aptID, APITokensDTO.aptID },
                                       { tblAPITokenServices::Fields::aptsvc_svcID, it.value() },
                                   });
    }

    CreateQueryServices.execute(APICALLBOOM_PARAM.getActorID());

    //----------------------------------------
    MethodResult.insert("tokenid", _tokenID);
    MethodResult.insert("newServices", _services.keys().join(','));

    return MethodResult;
}

/***********************************************************\
|** api methods ********************************************|
\***********************************************************/
QVariant IMPL_ORMGET_USER(APITokens) {
    APITokenServices::instance().prepareFiltersList();

    if (Authorization::hasPriv(APICALLBOOM_PARAM, this->privOn(EHTTP_GET, this->moduleBaseName())) == false)
        this->setSelfFilters({{ tblAPITokens::Fields::apt_usrID, APICALLBOOM_PARAM.getActorID() }}, _filters);

    return this->Select(GET_METHOD_ARGS_CALL_VALUES);
}

QVariant IMPL_REST_GET(APITokens, byService, (
    APICALLBOOM_TYPE_JWT_USER_IMPL  &APICALLBOOM_PARAM,
    QStringList                     _services,
//    TAPI::PKsByPath_t               _pksByPath,
    quint64                         _pageIndex,
    quint16                         _pageSize,
    TAPI::Cols_t                    _cols,
    TAPI::Filter_t                  _filters,
    TAPI::OrderBy_t                 _orderBy
//    TAPI::GroupBy_t                 _groupBy,
//    bool                            _reportCount,
//    bool                            _translate
)) {
    TAPI::PKsByPath_t               _pksByPath = {};
    TAPI::GroupBy_t                 _groupBy = {};
    bool                            _reportCount = true;
    bool                            _translate = true;

    APITokenServices::instance().prepareFiltersList();

    if (Authorization::hasPriv(APICALLBOOM_PARAM, this->privOn(EHTTP_GET, this->moduleBaseName())) == false)
        this->setSelfFilters({{ tblAPITokens::Fields::apt_usrID, APICALLBOOM_PARAM.getActorID() }}, _filters);

    if (_services.isEmpty())
        throw exHTTPExpectationFailed("Services not provided");

    if ((_services.count() == 1) && (_services.at(0).indexOf(',') >= 0))
        _services = _services.at(0).split(',', QString::SkipEmptyParts);

    auto fnTouchQuery = [&APICALLBOOM_PARAM, _translate, &_services](ORMSelectQuery &_query) {
        _query.nestedInnerJoin(APITokenServices::instance().makeSelectQuery(APICALLBOOM_PARAM, "", _translate, false)
                               .addCol(tblAPITokenServices::Fields::aptsvc_aptID, tblAPITokenServices::Fields::aptsvc_aptID)
                               .innerJoinWith(tblAPITokenServices::Relation::Service)
                               .where({ tblService::Fields::svcName, enuConditionOperator::In, "'" + _services.join("','") + "'" })
                               .groupBy(tblAPITokenServices::Fields::aptsvc_aptID)
                               , "tmpSearchAPIServices"
                               , { "tmpSearchAPIServices", tblAPITokenServices::Fields::aptsvc_aptID,
                                   enuConditionOperator::Equal,
                                   tblAPITokens::Name, tblAPITokens::Fields::aptID }
                               );
    };

    return this->Select(GET_METHOD_ARGS_CALL_VALUES, {}, 0, fnTouchQuery);
}

QVariantMap IMPL_REST_UPDATE(APITokens, , (
    APICALLBOOM_TYPE_JWT_USER_DECL &APICALLBOOM_PARAM,
    QString _token,
    QString _name,
    NULLABLE_TYPE(TAPI::DateTime_t) _expireDate
)) {
    QVariantMap MethodResult;

    quint64 CurrentUserID = APICALLBOOM_PARAM.getActorID();

    TAPI::JWT_t TokenJWTPayload;
    QJWT::extractAndDecryptPayload(_token, TokenJWTPayload);

    if (CurrentUserID != TokenJWTPayload["own"].toDouble())
        throw exAuthorization("This API Token is not yours");

    //----------------------------------------
    enuTokenActorType::Type TokenType = enuTokenActorType::USER;
    if (TokenJWTPayload.contains("typ"))
        TokenType = enuTokenActorType::toEnum(TokenJWTPayload["typ"].toString());
    if (TokenType != enuTokenActorType::API)
        throw exHTTPForbidden("Only API Token allowed");

    //----------------------------------------
    auto UpdateQuery = this->makeUpdateQuery(APICALLBOOM_PARAM)
                       .where({ tblAPITokens::Fields::aptToken, enuConditionOperator::Equal, _token });

    int ToUpdateCount = 0;

    if (_name.isNull() == false) {
        ++ToUpdateCount;
        UpdateQuery.set(tblAPITokens::Fields::aptName, _name);
    }

    QString NewToken;

    if (NULLABLE_HAS_VALUE(_expireDate)) {
        TokenJWTPayload["exp"] = (NULLABLE_VALUE(_expireDate)).toSecsSinceEpoch();
        NewToken = QJWT::encryptAndSigned(TokenJWTPayload);

        ++ToUpdateCount;
        UpdateQuery
                .set(tblAPITokens::Fields::aptToken, NewToken)
                .set(tblAPITokens::Fields::aptExpiryDate, NULLABLE_VALUE(_expireDate))
                ;

        MethodResult.insert("token", NewToken);
    }

    if (ToUpdateCount == 0)
        throw exHTTPInternalServerError("Nothing to do");

    try {
        UpdateQuery.execute(CurrentUserID);
    }  catch (const std::exception &_exp) {
        QString ExpStr = _exp.what();
        if (ExpStr.contains("Duplicate entry", Qt::CaseInsensitive))
            throw /*exHTTPConflict*/exHTTPBadRequest("This name has already been used");

        throw;
    }

    if (NewToken.isEmpty() == false) {
        QString OldTokenMD5 = QCryptographicHash::hash(_token.toLatin1(), QCryptographicHash::Md5).toHex().constData();
        QString NewTokenMD5 = QCryptographicHash::hash(NewToken.toLatin1(), QCryptographicHash::Md5).toHex().constData();

        QString qry = QString()
              + "UPDATE " + tblTokenBin::Name
              + "   SET tkbTokenMD5=?"
              + " WHERE tkbTokenMD5=?"
              ;

        clsDACResult Result = TokenBin::instance().execQuery(APICALLBOOM_PARAM,
                                                             qry,
                                                             QVariantList({
                                                                              NewTokenMD5,
                                                                              OldTokenMD5
                                                                          })
                                                             );
    }

    MethodResult.insert("result", true);

    return MethodResult;
}

Targoman::API::AccountModule::stuRequestTokenResult IMPL_REST_POST(APITokens, request, (
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

QVariantMap IMPL_REST_POST(APITokens, addServices, (
    APICALLBOOM_TYPE_JWT_USER_IMPL &APICALLBOOM_PARAM,
    const QString &_token,
    const QStringList &_services
)) {
    if (_services.isEmpty())
        throw exHTTPBadRequest("Services is empty");

    quint64 CurrentUserID = APICALLBOOM_PARAM.getActorID();

    TAPI::JWT_t TokenJWTPayload;
    QJWT::extractAndDecryptPayload(_token, TokenJWTPayload);

    if (CurrentUserID != TokenJWTPayload["own"].toDouble())
        throw exAuthorization("This API Token is not yours");

    QMap<QString, quint64> ServiceMaps;
    foreach (auto Service, _services) {
        ServiceMaps[Service] = 0;
    }

    return this->addServices(
                APICALLBOOM_PARAM,
                TokenJWTPayload["uid"].toDouble(),
                ServiceMaps
                );
}

QVariantMap IMPL_REST_POST(APITokens, revoke, (
    APICALLBOOM_TYPE_JWT_USER_IMPL &APICALLBOOM_PARAM,
    const QString   &_token,
    TAPI::MD5_t     _pass,
    QString         _salt
)) {
    quint64 CurrentUserID = APICALLBOOM_PARAM.getActorID();

    TAPI::JWT_t TokenJWTPayload;
    QJWT::extractAndDecryptPayload(_token, TokenJWTPayload);

    if (CurrentUserID != TokenJWTPayload["own"].toDouble())
        throw exAuthorization("This API Token is not yours");

    //----------------------------------------
    enuTokenActorType::Type TokenType = enuTokenActorType::USER;
    if (TokenJWTPayload.contains("typ"))
        TokenType = enuTokenActorType::toEnum(TokenJWTPayload["typ"].toString());
    if (TokenType != enuTokenActorType::API)
        throw exHTTPForbidden("Only API Token allowed");

    //----------------------------------------
    QDateTime ExpireDateTime = QDateTime::fromSecsSinceEpoch(TokenJWTPayload["exp"].toDouble());

    //----------------------------------------

//    TargomanDebug(5) << "before" << TokenJWTPayload;

    int RevokeCounter = 0;
    if (TokenJWTPayload.contains("rvkcnt"))
        RevokeCounter = TokenJWTPayload["rvkcnt"].toInt();
    ++RevokeCounter;
    TokenJWTPayload["rvkcnt"] = RevokeCounter;

//    TargomanDebug(5) << "after" << TokenJWTPayload;

    //----------------------------------------
    QString NewToken = QJWT::encryptAndSigned(TokenJWTPayload);

    //----------------------------------------
    makeAAADAC(DAC);

    /*clsDACResult DACResult = */DAC.callSP({},
                                     "spToken_Revoke", {
                                         { "iUserID", APICALLBOOM_PARAM.getActorID() },
                                         { "iPass", _pass },
                                         { "iSalt", _salt },
                                         { "iToken", _token },
                                         { "iDueDateTime", ExpireDateTime },
                                         { "iNewToken", NewToken },
                                     });

    TokenHelper::tokenRevoked(_token,
                              ExpireDateTime,
                              NewToken
                              );

    QVariantMap Result({
        { "counter", RevokeCounter },
    });

    Result.insert("token", NewToken);

    return Result;
}

bool IMPL_REST_POST(APITokens, pause, (
    APICALLBOOM_TYPE_JWT_USER_IMPL &APICALLBOOM_PARAM,
    const QString &_token
)) {
    quint64 CurrentUserID = APICALLBOOM_PARAM.getActorID();

    TAPI::JWT_t TokenJWTPayload;
    QJWT::extractAndDecryptPayload(_token, TokenJWTPayload);

    if (CurrentUserID != TokenJWTPayload["own"].toDouble())
        throw exAuthorization("This API Token is not yours");

    //----------------------------------------
    enuTokenActorType::Type TokenType = enuTokenActorType::USER;
    if (TokenJWTPayload.contains("typ"))
        TokenType = enuTokenActorType::toEnum(TokenJWTPayload["typ"].toString());
    if (TokenType != enuTokenActorType::API)
        throw exHTTPForbidden("Only API Token allowed");

    //----------------------------------------
    makeAAADAC(DAC);
    clsDACResult Result = DAC.callSP({},
                                     "spToken_Pause", {
                                         { "iToken", _token },
                                         { "iUserID", APICALLBOOM_PARAM.getActorID() },
                                     });

    TokenHelper::tokenPaused(_token);

    return true;
}

bool IMPL_REST_POST(APITokens, resume, (
    APICALLBOOM_TYPE_JWT_USER_IMPL &APICALLBOOM_PARAM,
    const QString &_token
)) {
    quint64 CurrentUserID = APICALLBOOM_PARAM.getActorID();

    TAPI::JWT_t TokenJWTPayload;
    QJWT::extractAndDecryptPayload(_token, TokenJWTPayload);

    if (CurrentUserID != TokenJWTPayload["own"].toDouble())
        throw exAuthorization("This API Token is not yours");

    //----------------------------------------
    enuTokenActorType::Type TokenType = enuTokenActorType::USER;
    if (TokenJWTPayload.contains("typ"))
        TokenType = enuTokenActorType::toEnum(TokenJWTPayload["typ"].toString());
    if (TokenType != enuTokenActorType::API)
        throw exHTTPForbidden("Only API Token allowed");

    //----------------------------------------
    makeAAADAC(DAC);
    clsDACResult Result = DAC.callSP({},
                                     "spToken_Resume", {
                                         { "iToken", _token },
                                         { "iUserID", APICALLBOOM_PARAM.getActorID() },
                                     });

    TokenHelper::tokenResumed(_token);

    return true;
}

/******************************************************/
/******************************************************/
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
/******************************************************/
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
