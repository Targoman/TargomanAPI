/******************************************************************************
#   TargomanAPI: REST API for Targoman
#
#   Copyright 2014-2019 by Targoman Intelligent Processing <http://tip.co.ir>
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
 */

#ifndef TARGOMAN_API_SERVER_CLSAPIOBJECT_HPP
#define TARGOMAN_API_SERVER_CLSAPIOBJECT_HPP

#include <QGenericArgument>
#include <QMetaMethod>

#include "Interfaces/Common/intfAPIModule.hpp"

namespace Targoman {
namespace API {
namespace Server {

#define PARAM_JWT       "TAPI::JWT_t"
#define PARAM_COOKIES   "TAPI::COOKIES_t"
#define PARAM_REMOTE_IP "TAPI::RemoteIP_t"
#define PARAM_HEADERS   "TAPI::HEADERS_t"
#define PARAM_EXTRAPATH "TAPI::ExtraPath_t"
#define PARAM_ORMFILTER "TAPI::ORMFilters_t"
#define PARAM_FILE      "TAPI::stuFileInfo"
#define PARAM_FILES     "TAPI::Files_t"

class QMetaMethodExtended : public QMetaMethod {
public:
    QMetaMethodExtended(QMetaMethod _metaMethod, QVariantList _defaultValues, QString _doc):
        QMetaMethod(_metaMethod),
        Doc(_doc),
        DefaultValues(_defaultValues)
    {}

    QString Doc;
    QVariantList DefaultValues;
};

class clsAPIObject : public intfAPIObject, public QObject
{
public:
    clsAPIObject(intfAPIModule* _module, QMetaMethodExtended _method, bool _async, qint32 _cache4Internal, qint32 _cache4Central, bool _hasExtraMethodName);
    ~clsAPIObject();

    inline QString makeCacheKey(const QVariantList& _args) const{
        return (this->BaseMethod.name() + QJsonValue::fromVariant(_args).toString().toUtf8()).constData();
    }

    inline bool requiresJWT() const {
        return this->ParamTypes.contains(PARAM_JWT);
    }

    inline bool requiresCookies() const {
        return this->ParamTypes.contains(PARAM_COOKIES);
    }

    inline bool requiresRemoteIP() const {
        return this->ParamTypes.contains(PARAM_REMOTE_IP);
    }

    inline bool requiresExtraPath() const {
        return this->ParamTypes.contains(PARAM_EXTRAPATH);
    }

    inline bool requiresHeaders() const {
        return this->ParamTypes.contains(PARAM_HEADERS);
    }

    inline bool requiresORMFilters() const {
        return this->ParamTypes.contains(PARAM_ORMFILTER);
    }

    inline QString paramType(quint8 _paramIndex) const {
        Q_ASSERT(_paramIndex < this->BaseMethod.parameterTypes().size());
        return this->BaseMethod.parameterTypes().at(_paramIndex).constData();
    }

    intfAPIArgManipulator* argSpecs(quint8 _paramIndex) const;

    inline bool isRequiredParam(quint8 _paramIndex) const {
        return _paramIndex < this->RequiredParamsCount;
    }

    inline QVariant defaultValue(quint8 _paramIndex) const {
        return this->BaseMethod.DefaultValues.at(_paramIndex);
    }

    QVariant invoke(const QStringList& _args,
                           QList<QPair<QString, QString>> _bodyArgs = {},
                           qhttp::THeaderHash _headers = {},
                           qhttp::THeaderHash _cookies = {},
                           QJsonObject _jwt = {},
                           QString _remoteIP = {},
                           QString _extraAPIPath = {}
                           ) const;


    void invokeMethod(const QVariantList& _arguments, QGenericReturnArgument _returnArg) const;

    bool isPolymorphic(const QMetaMethodExtended& _method);

private:
    void updateDefaultValues(const QMetaMethodExtended& _method);

private:
    QMetaMethodExtended         BaseMethod;
    QList<QMetaMethodExtended>  LessArgumentMethods;
    bool                        IsAsync;
    qint32                      Cache4Secs;
    qint32                      Cache4SecsCentral;
    QList<QByteArray>           ParamNames;
    QList<QString>              ParamTypes;
    quint8                      RequiredParamsCount;
    bool                        HasExtraMethodName;
    intfAPIModule*          Parent;

    friend class RESTAPIRegistry;
};

}
}
}
#endif // TARGOMAN_API_SERVER_CLSAPIOBJECT_HPP
