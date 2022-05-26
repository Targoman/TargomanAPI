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

#ifndef TARGOMAN_API_SERVER_APICALLBOOM_H
#define TARGOMAN_API_SERVER_APICALLBOOM_H

#include "QString"
#include "QSharedDataPointer"
#include "QElapsedTimer"
#include "QHttp/qhttpfwd.hpp"
#include "libTargomanCommon/Macros.h"

#define APICALLBOOM_TYPE_BASE_STR           "APICallBoom<"

//just for use in api methods (.h)
#define APICALLBOOM_TYPE_NO_JWT_DECL        APICallBoom<false>
#define APICALLBOOM_TYPE_NO_JWT_DECL_STR    TARGOMAN_M2STR(APICALLBOOM_TYPE_NO_JWT_DECL)

//just for use in api methods (.cpp)
#define APICALLBOOM_TYPE_NO_JWT_IMPL        Q_DECL_UNUSED APICALLBOOM_TYPE_NO_JWT_DECL

//just for use in api methods (.h)
#define APICALLBOOM_TYPE_JWT_DECL           APICallBoom<true>
#define APICALLBOOM_TYPE_JWT_DECL_STR       TARGOMAN_M2STR(APICALLBOOM_TYPE_JWT_DECL)

//just for use in api methods (.cpp)
#define APICALLBOOM_TYPE_JWT_IMPL           Q_DECL_UNUSED APICALLBOOM_TYPE_JWT_DECL

#define INTFAPICALLBOOM                     intfAPICallBoom
//just for use in NON-api methods (.h)
#define INTFAPICALLBOOM_DECL                intfAPICallBoom
//just for use in NON-api methods (.cpp)
#define INTFAPICALLBOOM_IMPL                Q_DECL_UNUSED intfAPICallBoom

#define APICALLBOOM_PARAM                   _APICALLBOOM
#define APICALLBOOM_PARAM_STR               TARGOMAN_M2STR(APICALLBOOM_PARAM)

namespace Targoman::API::Server {

class APICallBoomData;

class intfAPICallBoom
{
public:
    intfAPICallBoom(std::function<void(const QString &_name, const QString &_desc, quint64 _nanoSecs)> _fnTiming);
    intfAPICallBoom(const intfAPICallBoom &_other);
    virtual ~intfAPICallBoom();

public:
    void initialize(
        const QString &_apiPath,
        const QJsonObject &_JWT,
        const QVariantMap &_headers,
        const QVariantMap &_cookies,
        const QString &_ip
    );

    virtual bool needJWT() = 0;
    void setJWT(/*TAPI::JWT_t*/QJsonObject &_JWT);
    /*TAPI::JWT_t*/QJsonObject &getJWT();
    quint64 getUserID();
    QJsonObject getJWTPrivsObject();

    QString getIP();

//    void setRequestAPIPath(const QString &_path);
    QString requestAPIPath() const;
    QString host() const;
    quint16 port() const;

    void setResponseHeaders(const QVariantMap &_headers);
    QVariantMap getResponseHeaders();
    void addResponseHeader(const QString &_header, const QVariant &_value, bool _multiValue=false);
    void addResponseHeaderNameToExpose(const QString &_header);

    //-----------------------
//    enum class TimerTypes {
//        api,
//        db,
//    };

    class ScopedTimer {
    public:
        ScopedTimer(intfAPICallBoom *_parent, const QString &_name, const QString &_desc) :
            Parent(_parent),
            Name(_name),
            Desc(_desc)
        {
            this->ElapsedTimer.start();
        }
        ~ScopedTimer() {
            if (this->Finished == false)
                this->Parent->addToTimings(this->Name, this->Desc, this->ElapsedTimer.nsecsElapsed());
        }
        void finish() {
            this->Parent->addToTimings(this->Name, this->Desc, this->ElapsedTimer.nsecsElapsed());
            this->Finished = true;
        }
        void mark(const QString &_mark, bool _resetTiming = false) {
            this->Parent->addToTimings(this->Name, _mark, this->ElapsedTimer.nsecsElapsed());
            if (_resetTiming)
                this->ElapsedTimer.restart();
        }
        void mark(const quint64 _mark, bool _resetTiming = false) {
            this->mark(QString::number(_mark), _resetTiming);
        }

    protected:
        intfAPICallBoom* Parent;
        QString Name;
        QString Desc;
        QElapsedTimer ElapsedTimer;
        bool Finished;
    };

    ScopedTimer createScopeTiming(const QString &_name, const QString &_desc = "") {
        return ScopedTimer(this, _name, _desc);
    }
protected:
    void addToTimings(const QString &_name, const QString &_desc, quint64 _nanoSecs);

protected:
    QExplicitlySharedDataPointer<APICallBoomData> Data;
};

template <bool _needJWT>
class APICallBoom : public intfAPICallBoom
{
public:
    APICallBoom(std::function<void(const QString &_name, const QString &_desc, quint64 _nanoSecs)> _fnTiming) :
        intfAPICallBoom(_fnTiming)
    { ; }

    virtual bool needJWT() { return _needJWT; }
};

} //namespace Targoman::API::Server

#endif // TARGOMAN_API_SERVER_APICALLBOOM_H
