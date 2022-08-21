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

#ifndef TARGOMAN_API_SERVER_CLSAPIOBJECT_HPP
#define TARGOMAN_API_SERVER_CLSAPIOBJECT_HPP

#include <QGenericArgument>
#include <QMetaMethod>

#include "Interfaces/API/intfPureModule.h"
#include "Interfaces/Server/APICallBoom.h"

namespace Targoman::API::Server {

#define PARAM_JWT       "TAPI::JWT_t"
#define PARAM_COOKIES   "TAPI::COOKIES_t"
#define PARAM_REMOTE_IP "TAPI::RemoteIP_t"
#define PARAM_HEADERS   "TAPI::HEADERS_t"
#define PARAM_PKSBYPATH "TAPI::PKsByPath_t"
#define PARAM_ORMFIELDS "TAPI::ORMFields_t"
#define PARAM_FILE      "TAPI::stuFileInfo"
#define PARAM_FILES     "TAPI::Files_t"

class QMetaMethodExtended : public QMetaMethod {
public:
    QMetaMethodExtended(QMetaMethod _metaMethod, QVariantList _defaultValues, QString _doc, bool _hidden) :
        QMetaMethod(_metaMethod),
        Doc(_doc),
        DefaultValues(_defaultValues),
        Hidden(_hidden)
    { ; }

    QString Doc;
    QVariantList DefaultValues;
    bool Hidden;
};

class clsAPIObject : public intfAPIObject, public QObject
{
public:
    clsAPIObject(intfPureModule* _module,
                 QMetaMethodExtended _method,
                 bool _async,
                 qint32 _cache4Internal,
                 qint32 _cache4Central,
                 qint32 _ttl,
                 bool _hasExtraMethodName);
    ~clsAPIObject();

    inline QString makeCacheKey(const QVariantList& _args) const{
        return (this->BaseMethod.name() + QJsonValue::fromVariant(_args).toString().toUtf8()).constData();
    }

    inline bool isHidden() const {
        return this->BaseMethod.Hidden;
    }

//    intfPureModule* parentModule() {
//        return this->Parent;
//    }

    inline bool requiresJWT() const {
        return this->RequiresJWT; //ParamTypesName.contains(PARAM_JWT);
    }

    inline enuModuleActorType::Type moduleActorType() const {
        return this->Parent->actorType();
    }

//    inline bool requiresCookies() const {
//        return this->ParamTypesName.contains(PARAM_COOKIES);
//    }

//    inline bool requiresRemoteIP() const {
//        return this->ParamTypesName.contains(PARAM_REMOTE_IP);
//    }

    inline bool requiresPrimaryKey() const {
        return this->ParamTypesName.contains(PARAM_PKSBYPATH);
    }

//    inline bool requiresHeaders() const {
//        return this->ParamTypesName.contains(PARAM_HEADERS);
//    }

    inline bool requiresORMFields() const {
        return this->ParamTypesName.contains(PARAM_ORMFIELDS);
    }

    inline bool ttl() const {return this->TTL;}

    inline QString paramType(quint8 _paramIndex) const {
        Q_ASSERT(_paramIndex < this->/*BaseMethod.parameterType()*/ParamTypesID.size());
        return this->/*BaseMethod.parameterTypes()*/ParamTypesName.at(_paramIndex)/*.constData()*/;
    }

    intfAPIArgManipulator* argSpecs(quint8 _paramIndex) const;

    inline bool isRequiredParam(quint8 _paramIndex) const {
        return _paramIndex < this->RequiredParamsCount;
    }

    inline QVariant defaultValue(quint8 _paramIndex) const {
        return this->BaseMethod.DefaultValues.at(_paramIndex);
    }

    QVariant invoke(
        INTFAPICALLBOOM_DECL *APICALLBOOM_PARAM,
        bool _isUpdateMethod,
        const QStringList& _args,
//        /*OUT*/ QVariantMap &_responseHeaders,
        QList<QPair<QString, QString>> _bodyArgs = {},
//        qhttp::THeaderHash _headers = {},
//        qhttp::THeaderHash _cookies = {},
//        QJsonObject _jwt = {},
//        QString _remoteIP = {},
        QString _extraAPIPath = {}
    ) const;

    void invokeMethod(
        INTFAPICALLBOOM_DECL *APICALLBOOM_PARAM,
        const QVariantList& _arguments,
        QGenericReturnArgument _returnArg
//        /*OUT*/ QVariantMap &_responseHeaders
    ) const;

    bool isPolymorphic(const QMetaMethodExtended& _method);

private:
    void updateDefaultValues(const QMetaMethodExtended& _method);

private:
    QMetaMethodExtended         BaseMethod;
    QList<QMetaMethodExtended>  LessArgumentMethods;
    bool                        IsAsync;
    qint32                      Cache4Secs;
    qint32                      Cache4SecsCentral;
    qint32                      TTL;
    QList<QByteArray>           ParamNames;
    QList<QString>              ParamTypesName;
    QList<int>                  ParamTypesID;
    quint8                      RequiredParamsCount;
    bool                        HasExtraMethodName;
    intfPureModule*             Parent;
    bool                        RequiresJWT;

    friend class RESTAPIRegistry;
    friend class OpenAPIGenerator;
};

} //namespace Targoman::API::Server

#endif // TARGOMAN_API_SERVER_CLSAPIOBJECT_HPP
