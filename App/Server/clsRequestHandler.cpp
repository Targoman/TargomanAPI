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

#include <QJsonObject>
#include <QJsonDocument>
#include <map>
#include <utility>
#include <QTcpSocket>
#include <QtConcurrent/QtConcurrent>

#include "libQFieldValidator/QFieldValidator.h"
#include "libQFieldValidator/Exceptions.h"

#include "clsRequestHandler.h"
#include "libTargomanCommon/CmdIO.h"
#include "Interfaces/API/intfPureModule.h"
#include "ServerConfigs.h"
#include "Interfaces/Server/QJWT.h"
#include "APICache.hpp"
#include "OpenAPIGenerator.h"

#include "Interfaces/Helpers/URLHelper.h"
using namespace Targoman::API::Helpers;

#include "Interfaces/AAA/Authentication.h"
using namespace Targoman::API::AAA;

using namespace TAPI;

namespace Targoman::API::Server {

using namespace qhttp::server;
using namespace Targoman::Common;

static quint64 RequestCounter = 1;

clsRequestHandler::clsRequestHandler(QHttpRequest *_req, QHttpResponse *_res, QObject* _parent) :
    QObject(_parent),
    Request(_req),
    Response(_res),
    RequestIndex(RequestCounter++)
{
    this->ElapsedTimer.start();
}

void clsRequestHandler::process(const QString& _api) {
    this->Request->onData([this](QByteArray _data) {
        try {
            TargomanLogInfo(7, "posted data: " << _data);

            QByteArray ContentType = this->Request->headers().value("content-type");

            if (ContentType.isEmpty())
                throw exHTTPBadRequest("No content-type header present");

            QByteArray ContentLengthStr = this->Request->headers().value("content-length");

            if (ContentLengthStr.isEmpty())
                throw exHTTPBadRequest("No content-length header present");

            qlonglong ContentLength = ContentLengthStr.toLongLong ();

            if (!ContentLength)
                throw exHTTPLengthRequired("content-length seems to be zero");

            if (ContentLength > ServerCommonConfigs::MaxUploadSize.value())
                throw exHTTPPayloadTooLarge(QString("Content-Size is too large: %d").arg(ContentLength));

            switch (this->Request->method()) {
                case qhttp::EHTTP_POST:
                case qhttp::EHTTP_PUT:
                case qhttp::EHTTP_PATCH:
                    break;
                default:
                    throw exHTTPBadRequest("Method: "+this->Request->methodString()+" is not supported or does not accept request body");
            }

            static constexpr char APPLICATION_JSON_HEADER[] = "application/json";
            static constexpr char APPLICATION_FORM_HEADER[] = "application/x-www-form-urlencoded";
            static constexpr char MULTIPART_BOUNDARY_HEADER[] = "multipart/form-data; boundary=";

            switch (ContentType.at(0)) {
                case 'a': {
                    if (ContentType != APPLICATION_JSON_HEADER && ContentType != APPLICATION_FORM_HEADER)
                        throw exHTTPBadRequest(("unsupported Content-Type: " + ContentType).constData());

                    if (_data.size() == ContentLength)
                        this->RemainingData = _data;
                    else if (this->RemainingData.size()) {
                        this->RemainingData += _data;
                        if (this->RemainingData.size() < ContentLength)
                            return;
                    } else {
                        this->RemainingData = _data;
                        return;
                    }

                    this->RemainingData = this->RemainingData.trimmed();

                    if (this->RemainingData.startsWith('{') || this->RemainingData.startsWith('[')) {
                        if (this->RemainingData.startsWith('{') == false || this->RemainingData.endsWith('}') == false)
                            throw exHTTPBadRequest("Invalid JSON Object");

                        QJsonParseError Error;
                        QJsonDocument JSON = QJsonDocument::fromJson(this->RemainingData, &Error);

                        if (JSON.isNull() || JSON.isObject() == false)
                            throw exHTTPBadRequest(QString("Invalid JSON Object: %1").arg(Error.errorString()));

                        QJsonObject JSONObject = JSON.object();

                        for (auto JSONObjectIter = JSONObject.begin();
                                JSONObjectIter != JSONObject.end();
                                ++JSONObjectIter
                        ) {
                            if (JSONObjectIter.value().isBool())
                                this->Request->addUserDefinedData(JSONObjectIter.key(), JSONObjectIter.value().toBool() ? "1" : "0");
                            else if (JSONObjectIter.value().isNull())
                                this->Request->addUserDefinedData(JSONObjectIter.key(), QString());
                            else if (JSONObjectIter.value().isArray())
                                this->Request->addUserDefinedData(JSONObjectIter.key(), QJsonDocument(JSONObjectIter.value().toArray()).toJson(QJsonDocument::Compact));
                            else if (JSONObjectIter.value().isObject())
                                this->Request->addUserDefinedData(JSONObjectIter.key(), QJsonDocument(JSONObjectIter.value().toObject()).toJson(QJsonDocument::Compact));
                            else if (JSONObjectIter.value().isDouble())
                                this->Request->addUserDefinedData(JSONObjectIter.key(), QString::number(JSONObjectIter.value().toDouble()));
                            else
                                this->Request->addUserDefinedData(JSONObjectIter.key(), JSONObjectIter.value().toString());
                        }
                    } else {
                        QList<QByteArray> Params = this->RemainingData.split('&');

                        static auto decodePercentEncoding = [](QByteArray& _value) {
                            _value = _value.replace("+"," ");
                            QUrl URL = QUrl::fromPercentEncoding("http://127.0.0.1/?key=" + _value);
                            _value = URL.query(QUrl::FullyDecoded).toUtf8();
                            _value = _value.mid(_value.indexOf('=') + 1);
                            return _value;
                        };

                        foreach (auto Param, Params) {
                            QList<QByteArray> ParamParts = Param.split('=');
                            if (ParamParts.size() != 2)
                                throw exHTTPBadRequest("Invalid Param: " + Param);
                            this->Request->addUserDefinedData(ParamParts.first(), decodePercentEncoding(ParamParts.last()));
                        }
                    }
                    break;
                }

                case 'm': {
                    if (this->MultipartFormDataHandler.isNull()) {
                        if (ContentType.startsWith(MULTIPART_BOUNDARY_HEADER) == false)
                            throw exHTTPBadRequest(("unsupported Content-Type: " + ContentType + " must be " + MULTIPART_BOUNDARY_HEADER).constData());

                        this->MultipartFormDataHandler.reset(
                                    new clsMultipartFormDataRequestHandler(
                                        this,
                                        ContentType.mid(sizeof(MULTIPART_BOUNDARY_HEADER) - 1)
                                        ));
                    }

                    qlonglong Fed = 0;
                    while (!this->MultipartFormDataHandler->stopped() && _data.size() > Fed) {
                        do
                        {
                            qulonglong Ret = this->MultipartFormDataHandler->feed(_data.mid(static_cast<int>(Fed)).constData(), _data.size() - Fed);
                            Fed += Ret;
                        } while (Fed < _data.size() && !this->MultipartFormDataHandler->stopped());
                    }
                    if (this->MultipartFormDataHandler->hasError())
                        throw exHTTPBadRequest(this->MultipartFormDataHandler->getErrorMessage());
                    break;
                }
                default:
                    throw exHTTPBadRequest(("unsupported Content-Type: " + ContentType).constData());
            } //switch

            TargomanLogInfo(7, "request user defined values: ");
            foreach (auto val, this->Request->userDefinedValues()) {
                TargomanLogInfo(7, val.first << " : " << val.second);
            }
        } catch (exHTTPError& ex) {
            this->sendError(static_cast<qhttp::TStatusCode>(ex.httpCode()), ex.what(), ex.additionalInfo(), {}, ex.httpCode() >= 500);
        } catch (exTargomanBase& ex) {
            this->sendError(static_cast<qhttp::TStatusCode>(ex.httpCode()), ex.what(), {}, {}, ex.httpCode() >= 500);
        } catch (exQFVRequiredParam &ex) {
            this->sendError(qhttp::ESTATUS_BAD_REQUEST, ex.what(), {}, {}, false);
        } catch (exQFVInvalidValue &ex) {
            this->sendError(qhttp::ESTATUS_BAD_REQUEST, ex.what(), {}, {}, false);
        } catch (std::exception &ex) {
            this->sendError(qhttp::ESTATUS_INTERNAL_SERVER_ERROR, ex.what(), {}, {}, true);
        }
    });

    this->Request->onEnd([this, _api]() {
        try {
            if (this->Request->method() == qhttp::EHTTP_OPTIONS)
                this->sendCORSOptions();
            else
                this->findAndCallAPI (_api);
        } catch (exHTTPError& ex) {
            this->sendError(static_cast<qhttp::TStatusCode>(ex.httpCode()), ex.what(), ex.additionalInfo(), {}, ex.httpCode() >= 500);
        } catch (exTargomanBase& ex) {
            this->sendError(static_cast<qhttp::TStatusCode>(ex.httpCode()), ex.what(), {}, {}, ex.httpCode() >= 500);
        } catch (exQFVRequiredParam &ex) {
            this->sendError(qhttp::ESTATUS_BAD_REQUEST, ex.what(), {}, {}, false);
        } catch (exQFVInvalidValue &ex) {
            this->sendError(qhttp::ESTATUS_BAD_REQUEST, ex.what(), {}, {}, false);
        } catch (std::exception &ex) {
            this->sendError(qhttp::ESTATUS_INTERNAL_SERVER_ERROR, ex.what(), {}, {}, true);
        } catch (...) {
            this->sendError(qhttp::ESTATUS_INTERNAL_SERVER_ERROR, "", {}, {}, true);
        }
    });
}

const qhttp::TStatusCode StatusCodeOnMethod[] = {
    qhttp::ESTATUS_ACCEPTED,           ///< EHTTP_DELETE         =  0,
    qhttp::ESTATUS_OK,                 ///< EHTTP_GET            =  1,
    qhttp::ESTATUS_EXPECTATION_FAILED, ///< EHTTP_HEAD           =  2,
    qhttp::ESTATUS_OK,                 ///< EHTTP_POST           =  3,
    qhttp::ESTATUS_CREATED,            ///< EHTTP_PUT            =  4,
    /* pathological */
    qhttp::ESTATUS_EXPECTATION_FAILED, ///< EHTTP_CONNECT        =  5,
    qhttp::ESTATUS_EXPECTATION_FAILED, ///< EHTTP_OPTIONS        =  6,
    qhttp::ESTATUS_EXPECTATION_FAILED, ///< EHTTP_TRACE          =  7,
    /* webdav */
    qhttp::ESTATUS_EXPECTATION_FAILED, ///< EHTTP_COPY           =  8,
    qhttp::ESTATUS_EXPECTATION_FAILED, ///< EHTTP_LOCK           =  9,
    qhttp::ESTATUS_EXPECTATION_FAILED, ///< EHTTP_MKCOL          = 10,
    qhttp::ESTATUS_EXPECTATION_FAILED, ///< EHTTP_MOVE           = 11,
    qhttp::ESTATUS_EXPECTATION_FAILED, ///< EHTTP_PROPFIND       = 12,
    qhttp::ESTATUS_EXPECTATION_FAILED, ///< EHTTP_PROPPATCH      = 13,
    qhttp::ESTATUS_EXPECTATION_FAILED, ///< EHTTP_SEARCH         = 14,
    qhttp::ESTATUS_EXPECTATION_FAILED, ///< EHTTP_UNLOCK         = 15,
    qhttp::ESTATUS_EXPECTATION_FAILED, ///< EHTTP_BIND           = 16,
    qhttp::ESTATUS_EXPECTATION_FAILED, ///< EHTTP_REBIND         = 17,
    qhttp::ESTATUS_EXPECTATION_FAILED, ///< EHTTP_UNBIND         = 18,
    qhttp::ESTATUS_EXPECTATION_FAILED, ///< EHTTP_ACL            = 19,
    /* subversion */
    qhttp::ESTATUS_EXPECTATION_FAILED, ///< EHTTP_REPORT         = 20,
    qhttp::ESTATUS_EXPECTATION_FAILED, ///< EHTTP_MKACTIVITY     = 21,
    qhttp::ESTATUS_EXPECTATION_FAILED, ///< EHTTP_CHECKOUT       = 22,
    qhttp::ESTATUS_EXPECTATION_FAILED, ///< EHTTP_MERGE          = 23,
    /* upnp */
    qhttp::ESTATUS_EXPECTATION_FAILED, ///< EHTTP_MSEARCH        = 24,
    qhttp::ESTATUS_EXPECTATION_FAILED, ///< EHTTP_NOTIFY         = 25,
    qhttp::ESTATUS_EXPECTATION_FAILED, ///< EHTTP_SUBSCRIBE      = 26,
    qhttp::ESTATUS_EXPECTATION_FAILED, ///< EHTTP_UNSUBSCRIBE    = 27,
    /* RFC-5789 */
    qhttp::ESTATUS_ACCEPTED,           ///< EHTTP_PATCH          = 28,
    qhttp::ESTATUS_EXPECTATION_FAILED, ///< EHTTP_PURGE          = 29,
    /* CalDAV */
    qhttp::ESTATUS_EXPECTATION_FAILED, ///< EHTTP_MKCALENDAR     = 30,
    /* RFC-2068, section 19.6.1.2 */
    qhttp::ESTATUS_EXPECTATION_FAILED, ///< EHTTP_LINK           = 31,
    qhttp::ESTATUS_EXPECTATION_FAILED, ///< EHTTP_UNLINK         = 32,
};

void clsRequestHandler::addToTimings(const QString &_name, const QString &_desc, quint64 _nanoSecs) {
    //add overal
    if (this->ServerTimings.contains(_name))
        this->ServerTimings[_name] += _nanoSecs;
    else
        this->ServerTimings.insert(_name, _nanoSecs);

    //add for desc
    if (_desc.isEmpty() == false) {
        QString Label = _name + ";desc=\"" + _desc + "\"";

        if (this->ServerTimings.contains(Label))
            this->ServerTimings[Label] += _nanoSecs;
        else
            this->ServerTimings.insert(Label, _nanoSecs);
    }
}

void clsRequestHandler::addToTimings(const QString &_name, quint64 _nanoSecs) {
    this->addToTimings(_name, "", _nanoSecs);
}

void clsRequestHandler::sendTimingsToResponse() {
#ifdef QT_DEBUG
    this->addToTimings("total", this->ElapsedTimer.nsecsElapsed());

//    this->Response->addHeaderValue("Access-Control-Expose-Headers", QStringLiteral("x-debug-time-elapsed"));
//    this->Response->addHeaderValue("x-debug-time-elapsed", QString::number(ceil(this->ElapsedTimer.nsecsElapsed() / 1000.0) / 1000) + " ms");

    QStringList Output;

//    Output << "app";

    for (QMap<QString, quint64>::const_iterator it = this->ServerTimings.constBegin();
         it != this->ServerTimings.constEnd();
         it++
    ) {
        QString Label = it.key();
//        if (Label.contains(";desc=") == false)
//            Label += ";desc=\"total\"";
        Output.append(QString("%1;dur=%2").arg(Label).arg(ceil(it.value() / 1000.0) / 1000));
    }

    this->Response->addHeaderValue("Server-Timing", Output.join(", "));

    TargomanLogInfo(7, "Server-Timing"
                    << " : "
                    << Output.join(", ")
                    );
#endif
}

clsRequestHandler::stuResult clsRequestHandler::run(
    clsAPIObject* _apiObject,
    QStringList& _queries,
    const QString& _pksByPath,
    const QString& _api
) {
    auto fnTiming = [=](const QString &_name, const QString &_desc, quint64 _nanoSecs) {
        this->addToTimings(_name, _desc, _nanoSecs);
    };

    enuTokenActorType::Type TokenActorType = _apiObject->tokenActorType();
    QScopedPointer<intfAPICallBoom> APICALLBOOM;

    if (TokenActorType == enuTokenActorType::USER) {
        if (_apiObject->tokenIsOptional())
            APICALLBOOM.reset(new APICALLBOOM_TYPE_JWT_USER_OR_ANONYMOUSE_DECL(fnTiming));
        else
            APICALLBOOM.reset(new APICALLBOOM_TYPE_JWT_USER_DECL(fnTiming));
    } else if (TokenActorType == enuTokenActorType::API) {
        if (_apiObject->tokenIsOptional())
            APICALLBOOM.reset(new APICALLBOOM_TYPE_JWT_API_OR_ANONYMOUSE_DECL(fnTiming));
        else
            APICALLBOOM.reset(new APICALLBOOM_TYPE_JWT_API_DECL(fnTiming));
    } else //enuTokenActorType::ANONYMOUSE
        APICALLBOOM.reset(new APICALLBOOM_TYPE_JWT_ANONYMOUSE_DECL(fnTiming));

    try {
        for (auto QueryIter = _queries.begin(); QueryIter != _queries.end(); ++QueryIter)
            *QueryIter = QueryIter->replace('+', ' ');

        if (!this->Request)
            throw exHTTPGone("Seems that client has gone");

        QString RemoteIP = this->toIPv4(this->Request->remoteAddress());
        qhttp::THeaderHash Headers = this->Request->headers();
        qhttp::THeaderHash Cookies;

        //JWT
        TAPI::JWT_t JWT;
        {
            auto JWTServerTiming = APICALLBOOM->createScopeTiming("jwt");

            QString BearerToken;
            QString Auth = Headers.value("authorization");
            if (Auth.startsWith("Bearer ")) {
                BearerToken = Auth.mid(sizeof("Bearer")).trimmed();
                Headers.remove("authorization");
            }

            if (BearerToken.isEmpty()) {
                if ((TokenActorType != enuTokenActorType::ANONYMOUSE)
                    && (_apiObject->tokenIsOptional() == false))
                throw exHTTPForbidden("No authentication header is present");

            } else if (TokenActorType == enuTokenActorType::ANONYMOUSE) {
//                throw exHTTPForbidden("The authentication header should not be sent for anonymouse apis");
                APICALLBOOM->addResponseHeader("x-tapi-jwt-warning", "no jwt needed");
                APICALLBOOM->addResponseHeaderNameToExpose("x-tapi-jwt-warning");

            } else {
                try {
                    QJWT::verifyJWT(
                                BearerToken,
                                RemoteIP,
                                TokenActorType,
                                JWT
                                );

                    //check svcs just for API tokens
                    if ((TokenActorType == enuTokenActorType::API)
                            && JWT.contains("prv") && JWT["prv"].toObject().contains("svc")) {
                        QString ModuleBaseName = _apiObject->parentModule()->moduleBaseName().split(":").first();
                        if (ModuleBaseName.isEmpty() == false) {
                            QStringList AllowedServices = JWT["prv"].toObject()["svc"].toString().split(",", QString::SkipEmptyParts);
                            if (AllowedServices.contains(ModuleBaseName) == false)
                                throw exHTTPForbidden(QString("Service `%1` not allowed by this token").arg(ModuleBaseName));
                        }
                    }

                } catch (exJWTExpired &exp) {
                    enuTokenActorType::Type TokenType = enuTokenActorType::USER;

                    if (JWT.contains("typ"))
                        TokenType = enuTokenActorType::toEnum(JWT["typ"].toString());

                    if (TokenType == enuTokenActorType::USER) {
                        auto RenewJWTServerTiming = APICALLBOOM->createScopeTiming("jwt", "renew");

                        bool IsRenewed = false;
                        QString NewToken = Authentication::renewExpiredJWT(
                                    JWT,
                                    RemoteIP,
                                    IsRenewed
                                    );

                        BearerToken = NewToken;

                        APICALLBOOM->addResponseHeader("x-auth-new-token", BearerToken);
                        APICALLBOOM->addResponseHeaderNameToExpose("x-auth-new-token");

                        if (IsRenewed == false) {
                            APICALLBOOM->addResponseHeader("x-auth-warning", "replace token");
                            APICALLBOOM->addResponseHeaderNameToExpose("x-auth-warning");
                        }
                    } else if (TokenType == enuTokenActorType::API)
                        throw exHTTPForbidden("API token is expired");
                    else
                        throw exHTTPForbidden(QString("Unknown token type `%1`").arg(TokenType));
                } //catch (exJWTExpired &exp)

                JWT["encodedJWT"] = BearerToken;
            } // if (BearerToken.isEmpty() == false) {
        } //jwt scope

        //--
        if (/*_apiObject->requiresCookies() && */Headers.value("cookie").size()) {
            foreach (auto Cookie, Headers.value("cookie").split(';')) {
                auto CookieParts = Cookie.split('=');
                Cookies.insert(CookieParts.first(), CookieParts.size() > 1 ? CookieParts.last() : QByteArray());
            }
        }

        Headers.remove("cookie");

        APICALLBOOM->initialize(
                    _api,
                    JWT,
                    Headers.toVariant().toMap(),
                    Cookies.toVariant().toMap(),
                    RemoteIP,
                    _queries,
                    this->Request->userDefinedValues()
                    );

        QVariant Result = _apiObject->invoke(
                              APICALLBOOM.data(),
                              this->Request->method() == qhttp::EHTTP_PATCH,
                              _queries,
//                              ResponseHeaders,
                              this->Request->userDefinedValues(),
//                              Headers,
//                              Cookies,
//                              JWT,
//                              this->toIPv4(this->Request->remoteAddress()),
                              _pksByPath
                              );

        return stuResult(Result, {}, APICALLBOOM->getResponseHeaders());
    } catch (exHTTPError& ex) {
        return stuResult(ex.what(), ex.additionalInfo(), APICALLBOOM->getResponseHeaders(), static_cast<qhttp::TStatusCode>(ex.httpCode()));
    } catch (exTargomanBase& ex) {
        return stuResult(ex.what(), {}, APICALLBOOM->getResponseHeaders(), static_cast<qhttp::TStatusCode>(ex.httpCode()));
    } catch (exQFVRequiredParam &ex) {
        return stuResult(ex.what(), {}, APICALLBOOM->getResponseHeaders(), qhttp::ESTATUS_BAD_REQUEST);
    } catch (exQFVInvalidValue &ex) {
        return stuResult(ex.what(), {}, APICALLBOOM->getResponseHeaders(), qhttp::ESTATUS_BAD_REQUEST);
    } catch (std::exception &ex) {
        return stuResult(ex.what(), {}, APICALLBOOM->getResponseHeaders(), qhttp::ESTATUS_INTERNAL_SERVER_ERROR);
    } catch (...) {
        return stuResult("INTERNAL SERVER ERROR!!!", {}, APICALLBOOM->getResponseHeaders(), qhttp::ESTATUS_INTERNAL_SERVER_ERROR);
    }
}

QString clsRequestHandler::host() const
{
    const qhttp::THeaderHash headers = this->Request->headers();

    if (headers.has("host") == false)
        return "127.0.0.1";

    QString Host = headers["host"];

    int idx;
    if ((idx = Host.indexOf(":")) >= 0)
        Host = Host.left(idx);

    return Host;
}

quint16 clsRequestHandler::port() const
{
    const qhttp::THeaderHash headers = this->Request->headers();

    if (headers.has("host") == false)
        return ServerCommonConfigs::ListenPort.value();

    QString Host = headers["host"];

    int idx;
    if ((idx = Host.indexOf(":")) < 0)
        return 80;

    return Host.mid(idx+1).toUInt();
}

void clsRequestHandler::findAndCallAPI(QString _api) {
    QStringList Queries = this->Request->url().query().split('&', QString::SkipEmptyParts);

    QString ExtraAPIPath;
    QString MethodString = this->Request->methodString();
    clsAPIObject* APIObject = RESTAPIRegistry::getAPIObject(MethodString, _api);

    if (!APIObject) {
        QString Path = _api;
        if (Path.endsWith('/'))
            Path.truncate(Path.size() - 1);
        ExtraAPIPath = Path.mid(Path.lastIndexOf('/') + 1);
        Path = Path.mid(0, Path.lastIndexOf('/'));
        APIObject = RESTAPIRegistry::getAPIObject(MethodString, Path);
    }

    if (!APIObject)
        return this->sendError(qhttp::ESTATUS_NOT_FOUND,
                               QString("API not found [%1] (%2)").arg(MethodString).arg(_api),
                               {},
                               {},
                               true);

    if (ServerCommonConfigs::MultiThreaded.value()) {
        this->connect(&this->FutureTimer, SIGNAL(timeout()), &this->FutureWatcher, SLOT(cancel()));

        this->connect(&this->FutureWatcher, &QFutureWatcher<stuResult>::canceled, [this]() {
            this->sendError(qhttp::ESTATUS_REQUEST_TIMEOUT, "Request Timed Out");
        });

        this->connect(&this->FutureWatcher, &QFutureWatcher<stuResult>::finished, [this]() {
            stuResult Result = this->FutureWatcher.result();

            if (Result.StatusCode == qhttp::ESTATUS_OK)
                this->sendResponse(StatusCodeOnMethod[this->Request->method()], Result.Result, Result.ResponseHeader);
            else
                this->sendError(Result.StatusCode, Result.Result.toString(), Result.AdditionalInfo, Result.ResponseHeader, Result.StatusCode >= 500);
        });

        this->FutureWatcher.setFuture(QtConcurrent::run(this, &clsRequestHandler::run, APIObject, Queries, ExtraAPIPath, _api));

        if (ServerCommonConfigs::APICallTimeout.value() > -1)
            this->FutureTimer.start(APIObject->ttl());
    } else
        run(APIObject, Queries, ExtraAPIPath, _api);
}

void clsRequestHandler::sendError(
    qhttp::TStatusCode _code,
    const QString &_message,
    const QJsonObject &_additionalInfo,
    QVariantMap _responseHeaders,
    bool _closeConnection
) {
    gServerStats.Errors.inc();

    QJsonObject Error({
                          { "error", QJsonObject({
                                { "code", _code },
                                { "message", _message },
                            })
                          }
                      });

    if (_additionalInfo.isEmpty() == false)
        Error.insert("additionalInfo", _additionalInfo);
//    for (QJsonObject::const_iterator it = _additionalInfo.constBegin();
//         it != _additionalInfo.constEnd();
//         it++
//    ) {
//        Error.insert(it.key(), it.value());
//    }

    this->sendResponseBase(_code,
                           Error,
                           _responseHeaders,
                           _closeConnection);
}

void clsRequestHandler::sendFile(QString _fullFileName) {
    _fullFileName = _fullFileName.replace(QRegularExpression("//+"), "/");
    QString BasePath;
    QString FileName;

    int Idx = _fullFileName.lastIndexOf("/");

    if (Idx == -1)
        FileName = _fullFileName;
    else {
        BasePath = _fullFileName.left(Idx + 1);
        FileName = _fullFileName.mid(Idx + 1);
    }

    this->sendFile(BasePath, FileName);
}

void clsRequestHandler::sendFile(QString _basePath, const QString &_path) {
    if (_basePath.right(1) != "/")
        _basePath += "/";

    if (QFile::exists(_basePath + _path) == false)
        throw exHTTPNotFound(_path);

    Q_ASSERT(this->FileHandler.isNull());

    this->FileHandler.reset(new QFile(_basePath + _path));
    this->FileHandler->open(QFile::ReadOnly);

    if (this->FileHandler->isReadable() == false)
        throw exHTTPForbidden(_path);

    QMimeType FileMIME = this->MIMEDB.mimeTypeForFile(_basePath + _path);
    qint64 FileSize = QFileInfo(*this->FileHandler).size();

    this->Response->addHeaderValue("content-type", FileMIME.name());
    this->Response->addHeaderValue("content-length", QString::number(FileSize));
    this->Response->addHeaderValue("Connection", QString("keep-alive"));

    this->Response->setStatusCode(qhttp::ESTATUS_OK);

#ifdef QT_DEBUG
    this->sendTimingsToResponse();
#endif

    QTimer::singleShot(10, this, &clsRequestHandler::slotSendFileData);
}

void clsRequestHandler::addHeaderValues(
    const QVariantMap &_responseHeaders
) {
    if (_responseHeaders.isEmpty() == false) {
        TargomanLogInfo(7, "Response Custom Header Values:");

        for (QVariantMap::const_iterator it = _responseHeaders.constBegin();
             it != _responseHeaders.constEnd();
             ++it
        ) {
            TargomanLogInfo(7, "    "
                               << it.key().toLatin1()
                               << " : "
                               << it->toString()
                               );

            this->Response->addHeaderValue(
                        it.key().toLatin1(),
                        it->toString()
                    );
        }
    }
}

void clsRequestHandler::sendResponse(qhttp::TStatusCode _code,
    const QVariant &_response,
    QVariantMap _responseHeaders
) {
    gServerStats.Success.inc();

    if (_response.isValid() == false)
        this->sendResponseBase(_code, QJsonObject({ { "result", QJsonValue(QJsonValue::Undefined) } }), _responseHeaders);
    else if (strcmp(_response.typeName(), "TAPI::RawData_t") == 0) {
        TAPI::RawData_t RawData = qvariant_cast<TAPI::RawData_t>(_response);

        TargomanLogInfo(7,
                        "Response ["
                        << this->Request->connection()->tcpSocket()->peerAddress().toString()
                        << ":"
                        << this->Request->connection()->tcpSocket()->peerPort()
                        << "]: (code:"
                        << _code
                        << "):"
                        << RawData.mime()
                        << ":RAW_DATA_SIZE("
                        << RawData.data().length()
                        << ")"
                        );

        this->Response->setStatusCode(_code);

        this->Response->addHeaderValue("content-length", RawData.data().length());
        this->Response->addHeaderValue("content-type", QString(RawData.mime()));
        this->Response->addHeaderValue("Access-Control-Allow-Origin", QStringLiteral("*"));
        this->Response->addHeaderValue("Connection", QStringLiteral("keep-alive"));

//        auto funcAddToHeaderArray = [&_responseHeaders](const QString &_header) {
//            if (_responseHeaders.contains("Access-Control-Expose-Headers"))
//                _responseHeaders["Access-Control-Expose-Headers"] = _responseHeaders["Access-Control-Expose-Headers"].toString() + "," + _header;
//            else
//                _responseHeaders["Access-Control-Expose-Headers"] = _header;
//        };

#ifdef QT_DEBUG
        this->sendTimingsToResponse();
//        funcAddToHeaderArray("x-debug-time-elapsed");

//        if (_responseHeaders.contains("x-debug-time-elapsed") == false)
//            _responseHeaders["x-debug-time-elapsed"] = QString::number(ceil(this->ElapsedTimer.nsecsElapsed() / 1000.0) / 1000) + " ms";
#endif

        this->addHeaderValues(_responseHeaders);

        this->Response->end(RawData.data());

        this->deleteLater();
    } else if (strcmp(_response.typeName(), "TAPI::ResponseRedirect_t") == 0) {
        TAPI::ResponseRedirect_t ResponseRedirect = qvariant_cast<TAPI::ResponseRedirect_t>(_response);
        this->redirect(ResponseRedirect.url(), ResponseRedirect.appendBase(), ResponseRedirect.statusCode());
    } else if (strcmp(_response.typeName(), "TAPI::FileData_t") == 0) {
        TAPI::FileData_t FileData = qvariant_cast<TAPI::FileData_t>(_response);
        this->sendFile(FileData.fileName());
    } else
        this->sendResponseBase(_code, QJsonObject({ { "result", QJsonValue::fromVariant(_response) } }), _responseHeaders);
}

void clsRequestHandler::sendCORSOptions() {
    this->Response->addHeaderValue("Access-Control-Allow-Origin", ServerCommonConfigs::AccessControl.value());
    this->Response->addHeaderValue("Access-Control-Allow-Credentials", QStringLiteral("true"));
    this->Response->addHeaderValue("Access-Control-Allow-Methods",
                                   QStringLiteral("GET, POST, PUT, PATCH, DELETE"));
    this->Response->addHeaderValue("Access-Control-Allow-Headers",
                                   QStringLiteral("authorization,"
                                           "access-control-allow-origin,"
                                           "Access-Control-Allow-Headers,"
                                           "DNT,X-CustomHeader,"
                                           "Keep-Alive,"
                                           "User-Agent,"
                                           "X-Requested-With,"
                                           "If-Modified-Since,"
                                           "Cache-Control,"
                                           "Content-Type"));
    this->Response->addHeaderValue("Access-Control-Max-Age", 1728000);
    this->Response->addHeaderValue("content-length", 0);
    this->Response->addHeaderValue("content-type", QStringLiteral("application/json; charset=utf-8"));
    this->Response->addHeaderValue("Connection", QStringLiteral("keep-alive"));

    this->Response->setStatusCode(qhttp::ESTATUS_NO_CONTENT);

#ifdef QT_DEBUG
    this->sendTimingsToResponse();
#endif

    this->Response->end();

    this->deleteLater();
}

void clsRequestHandler::redirect(const QString _path, bool _appendBase, qhttp::TStatusCode _statusCode) { //bool _permananet) {
    QString Path = _appendBase ? ServerCommonConfigs::BasePathWithVersion + _path : _path;
    Path = URLHelper::normalize(Path);

    this->Response->addHeaderValue("Location", Path);

    this->Response->setStatusCode(_statusCode); //_permananet ?  qhttp::ESTATUS_MOVED_PERMANENTLY : qhttp::ESTATUS_TEMPORARY_REDIRECT);

    this->Response->end();

    this->deleteLater();
}

void clsRequestHandler::sendResponseBase(
    qhttp::TStatusCode _code,
    QJsonObject _dataObject,
    QVariantMap _responseHeaders,
    bool _closeConnection
) {
    if (!this->Request->connection()->tcpSocket())
      return;

    QByteArray Data = QJsonDocument(_dataObject).toJson(ServerCommonConfigs::IndentedJson.value()
                                                        ? QJsonDocument::Indented
                                                        : QJsonDocument::Compact);

    TargomanLogInfo(7,
                    "Response ["
                    << this->Request->connection()->tcpSocket()->peerAddress().toString()
                    << ":"
                    << this->Request->connection()->tcpSocket()->peerPort()
                    << "]: (code:"
                    << _code
                    << "):"
                    << Data);

    this->Response->setStatusCode(_code);

    if (_closeConnection)
        this->Response->addHeader("connection", "close");

    this->Response->addHeaderValue("content-length", Data.length());
    this->Response->addHeaderValue("content-type", QStringLiteral("application/json; charset=utf-8"));
    this->Response->addHeaderValue("Access-Control-Allow-Origin", QStringLiteral("*"));
    this->Response->addHeaderValue("Connection", QStringLiteral("keep-alive"));

//    auto funcAddToHeaderArray = [&_responseHeaders](const QString &_header) {
//        if (_responseHeaders.contains("Access-Control-Expose-Headers"))
//            _responseHeaders["Access-Control-Expose-Headers"] = _responseHeaders["Access-Control-Expose-Headers"].toString() + "," + _header;
//        else
//            _responseHeaders["Access-Control-Expose-Headers"] = _header;
//    };

#ifdef QT_DEBUG
    this->sendTimingsToResponse();
//    funcAddToHeaderArray("x-debug-time-elapsed");

//    if (_responseHeaders.contains("x-debug-time-elapsed") == false)
//        _responseHeaders["x-debug-time-elapsed"] = QString::number(ceil(this->ElapsedTimer.nsecsElapsed() / 1000.0) / 1000) + " ms";
#endif

    this->addHeaderValues(_responseHeaders);

    this->Response->end(Data.constData());

    this->deleteLater();
}

void clsRequestHandler::slotSendFileData() {
    if (this->FileHandler.isNull() || this->FileHandler->atEnd()) {
//        this->Response->end();
        this->deleteLater();
        return;
    }

    auto FileData = this->FileHandler->read(ServerCommonConfigs::FileMaxChunk.value());
    this->Response->write(FileData);
    QTimer::singleShot(10, this, &clsRequestHandler::slotSendFileData);
}

/**************************************************************************/
void clsMultipartFormDataRequestHandler::onMultiPartBegin(const MultipartHeaders& _headers, void *_userData) {
    clsMultipartFormDataRequestHandler *Self = static_cast<clsMultipartFormDataRequestHandler*>(_userData);
    try {
        std::string ContentDisposition = _headers["Content-Disposition"];
        if (ContentDisposition.size()) {
            const char* pContentDisposition = ContentDisposition.c_str();
            const char* pBufferStart = pContentDisposition;
            enum enuLooking4{
                L4Type,
                L4Field,
                L4NextField,
                L4DQuote,
                L4Value,
            } Looking4 = L4Type;
            char StopChar = ';';
            std::string* pLastFieldValue = nullptr;
            std::string Dummy;

            while (pContentDisposition && *pContentDisposition != '\0') {
                if (*pContentDisposition == StopChar) {
                    switch (Looking4) {
                        case L4Type:
                            if (strncmp(pBufferStart, "form-data", static_cast<size_t>(pContentDisposition - pBufferStart)))
                                throw exHTTPBadRequest("Just form-data is allowed in multi-part request according to RFC7578");
                            Looking4 = L4Field;
                            pBufferStart = pContentDisposition+1;
                            StopChar = '=';
                            break;
                        case L4Field:
                            if (strncmp(pBufferStart, " name", static_cast<size_t>(pContentDisposition - pBufferStart)) == 0)
                                pLastFieldValue = &Self->LastItemName;
                            else if (strncmp(pBufferStart, " filename", static_cast<size_t>(pContentDisposition - pBufferStart)) == 0)
                                pLastFieldValue = &Self->LastFileName;
                            else
                                pLastFieldValue = &Dummy;
                            Looking4 = L4DQuote;
                            StopChar = '"';
                            break;
                        case L4NextField:
                            Looking4 = L4Field;
                            pBufferStart = pContentDisposition+1;
                            StopChar = '=';
                            break;
                        case L4DQuote:
                            Looking4 = L4Value;
                            StopChar = '"';
                            pBufferStart = pContentDisposition+1;
                            break;
                        case L4Value:
                            *pLastFieldValue = pBufferStart;
                            pLastFieldValue->erase(static_cast<size_t>(pContentDisposition - pBufferStart), std::string::npos);
                            StopChar=';';
                            Looking4 = L4NextField;
                            pBufferStart = pContentDisposition+2;
                            break;
                    }
                } else if (*pContentDisposition == '\r')
                    break;
                ++pContentDisposition;
            }

            if (Self->LastItemName.empty())
                throw exHTTPBadRequest(QString("No name provided for form field: ") + ContentDisposition.c_str());
            if (Self->ToBeStoredItemName.empty())
                Self->ToBeStoredItemName = Self->LastItemName;

            if (Self->LastFileName.size()) {
                Self->LastTempFile.reset(new QTemporaryFile);
                if (Self->LastTempFile.isNull() || Self->LastTempFile->open() == false)
                    throw exHTTPInternalServerError("unable to create temporary file");
                Self->LastTempFile->setAutoRemove(false);
                Self->LastMime = _headers["Content-Type"];
            }
        } else
            throw exHTTPBadRequest("No Content-Disposition header provided");
    } catch (exHTTPError& ex) {
        Self->pRequestHandler->sendError(static_cast<qhttp::TStatusCode>(ex.code()), ex.what(), ex.additionalInfo(), {}, ex.code() >= 500);
    } catch (exTargomanBase& ex) {
        Self->pRequestHandler->sendError(qhttp::ESTATUS_INTERNAL_SERVER_ERROR, ex.what(), {}, {}, true);
    }

}

void clsMultipartFormDataRequestHandler::onMultiPartData(const char *_buffer, long long _size, void *_userData) {
    clsMultipartFormDataRequestHandler *Self = static_cast<clsMultipartFormDataRequestHandler*>(_userData);
    if (Self->LastTempFile.isNull() == false) {
        Self->LastWrittenBytes += Self->LastTempFile->write(_buffer, _size);
        if (Self->LastWrittenBytes > ServerCommonConfigs::MaxUploadedFileSize.value())
            throw exHTTPPayloadTooLarge("Max file size limit reached");
    } else
        Self->LastValue = QString::fromUtf8(_buffer, static_cast<int>(_size));
}

void clsMultipartFormDataRequestHandler::onMultiPartEnd(void *_userData) {
    clsMultipartFormDataRequestHandler *Self = static_cast<clsMultipartFormDataRequestHandler*>(_userData);
    if (Self->ToBeStoredItemName != Self->LastItemName) {
        Self->storeDataInRequest();
        Self->SameNameItems.clear();
        Self->ToBeStoredItemName = Self->LastItemName;
    }
    if (Self->LastTempFile.isNull() == false) {
        Self->SameNameItems.append(
                    QJsonDocument(QJsonObject::fromVariantMap(TAPI::stuFileInfo(
                                                                  Self->LastFileName.c_str(),
                                                                  Self->LastTempFile->fileName(),
                                                                  static_cast<quint64>(Self->LastWrittenBytes),
                                                                  Self->LastMime.c_str()).toVariant().toMap())).toJson(QJsonDocument::Compact));
        Self->LastTempFile.reset();
    } else
        Self->SameNameItems.append(Self->LastValue);


    Self->LastFileName.clear();
    Self->LastItemName.clear();
    Self->LastMime.clear();
    Self->LastWrittenBytes = 0;
}

void clsMultipartFormDataRequestHandler::onDataEnd(void *_userData) {
    clsMultipartFormDataRequestHandler *Self = static_cast<clsMultipartFormDataRequestHandler*>(_userData);
    if (Self->SameNameItems.size())
        Self->storeDataInRequest();
}

void clsMultipartFormDataRequestHandler::storeDataInRequest() {
    if (this->SameNameItems.size() > 1)
        this->pRequestHandler->Request->addUserDefinedData(this->ToBeStoredItemName.c_str(), QString("[%1]").arg(this->SameNameItems.join(',')));
    else
        this->pRequestHandler->Request->addUserDefinedData(this->ToBeStoredItemName.c_str(), this->SameNameItems.at(0));
}

QString  clsRequestHandler::toIPv4(const QString _ip) {
    if (_ip.startsWith("::"))
        return _ip.mid(_ip.lastIndexOf(':') + 1);
    return _ip;
}

void clsUpdateAndPruneThread::run() {

    QTimer Timer;
    QObject::connect(&Timer, &QTimer::timeout, []() {
        gServerStats.Connections.snapshot(ServerCommonConfigs::StatisticsInterval.value());
        gServerStats.WSConnections.snapshot(ServerCommonConfigs::StatisticsInterval.value());
        gServerStats.Errors.snapshot(ServerCommonConfigs::StatisticsInterval.value());
        gServerStats.Blocked.snapshot(ServerCommonConfigs::StatisticsInterval.value());
        gServerStats.Success.snapshot(ServerCommonConfigs::StatisticsInterval.value());

        for (auto ListIter = gServerStats.APICallsStats.begin ();
             ListIter != gServerStats.APICallsStats.end ();
             ++ListIter)
            ListIter->snapshot(ServerCommonConfigs::StatisticsInterval.value());
        for (auto ListIter = gServerStats.APIInternalCacheStats.begin ();
             ListIter != gServerStats.APIInternalCacheStats.end ();
             ++ListIter)
            ListIter->snapshot(ServerCommonConfigs::StatisticsInterval.value());

        QList<Cache_t::const_iterator> ToDeleteIters;
        for (auto CacheIter = InternalCache::Cache.begin();
            CacheIter != InternalCache::Cache.end();
            ++CacheIter)
            if (CacheIter->InsertionTime.secsTo(QTime::currentTime()) > CacheIter->TTL)
                ToDeleteIters.append(CacheIter);

        if (ToDeleteIters.size()) {
            QMutexLocker Locker(&InternalCache::Lock);
            foreach (auto Iter, ToDeleteIters)
                InternalCache::Cache.erase(Iter);
        }
    });

    Timer.start(ServerCommonConfigs::StatisticsInterval.value() * 1000);
    this->exec();
}

} //namespace Targoman::API::Server
