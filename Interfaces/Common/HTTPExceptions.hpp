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
 */

#ifndef TAPI_HTTPEXCEPTIONS_H
#define TAPI_HTTPEXCEPTIONS_H

#include "libTargomanCommon/exTargomanBase.h"
#include "QHttp/qhttpfwd.hpp"

namespace Targoman {
namespace API {

/*******************************************************************************/
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wweak-vtables"

class exHTTPError : public Targoman::Common::exTargomanBase {
public:
    exHTTPError(const QString& _definition, quint16 _errorCode, const QString& _message) :
        Targoman::Common::exTargomanBase(_message, _errorCode),
        Definition(_definition)
    { ; }

    virtual void toEnsureAvoidanceOfUsingBaseClass()=0;

    inline const QString definition() const{return this->Definition;}
    inline const QString fullError() {
        return QString("%1(%2): %3").arg(
                    this->definition()).arg(
                    this->code()).arg(
                    this->what());}

protected:
  QString    Definition;
};

/*******************************************************************************/
#define TAPI_ADD_HTTP_EXCEPTION(_code,_name) \
    class _name : public exHTTPError{\
    public: _name (const QString& _message = "") : \
            exHTTPError (QString(TARGOMAN_M2STR(_name)).mid(2), _code, _message) \
            {} \
            void toEnsureAvoidanceOfUsingBaseClass() { ; } \
    }

/*******************************************************************************/
TAPI_ADD_HTTP_EXCEPTION(400, exHTTPBadRequest);
TAPI_ADD_HTTP_EXCEPTION(401, exHTTPUnauthorized);
TAPI_ADD_HTTP_EXCEPTION(402, exHTTPPaymentRequired);
TAPI_ADD_HTTP_EXCEPTION(403, exHTTPForbidden);
TAPI_ADD_HTTP_EXCEPTION(404, exHTTPNotFound);
TAPI_ADD_HTTP_EXCEPTION(405, exHTTPMethodNotAllowed);
TAPI_ADD_HTTP_EXCEPTION(406, exHTTPNotAcceptable);
TAPI_ADD_HTTP_EXCEPTION(407, exHTTPProxyAuthenticationRequired);
TAPI_ADD_HTTP_EXCEPTION(408, exHTTPRequestTimeout);
TAPI_ADD_HTTP_EXCEPTION(409, exHTTPConflict);
TAPI_ADD_HTTP_EXCEPTION(410, exHTTPGone);
TAPI_ADD_HTTP_EXCEPTION(411, exHTTPLengthRequired);
TAPI_ADD_HTTP_EXCEPTION(412, exHTTPPreconditionFailed);
TAPI_ADD_HTTP_EXCEPTION(413, exHTTPPayloadTooLarge);
TAPI_ADD_HTTP_EXCEPTION(414, exHTTPURITooLong);
TAPI_ADD_HTTP_EXCEPTION(415, exHTTPUnsupportedMediaType);
TAPI_ADD_HTTP_EXCEPTION(416, exHTTPRangeNotSatisfiable);
TAPI_ADD_HTTP_EXCEPTION(417, exHTTPExpectationFailed);
TAPI_ADD_HTTP_EXCEPTION(421, exHTTPMisdirectedRequest);
TAPI_ADD_HTTP_EXCEPTION(422, exHTTPUnprocessableEntity);
TAPI_ADD_HTTP_EXCEPTION(423, exHTTPLocked);
TAPI_ADD_HTTP_EXCEPTION(424, exHTTPFailedDependency);
TAPI_ADD_HTTP_EXCEPTION(426, exHTTPUpgradeRequired);
TAPI_ADD_HTTP_EXCEPTION(428, exHTTPPreconditionRequired);
TAPI_ADD_HTTP_EXCEPTION(429, exHTTPTooManyRequests);
TAPI_ADD_HTTP_EXCEPTION(431, exHTTPRequestHeaderFieldsTooLarge);
TAPI_ADD_HTTP_EXCEPTION(431, exHTTPLoginTimedout);
TAPI_ADD_HTTP_EXCEPTION(451, exHTTPUnavailableForLegalReasons);
TAPI_ADD_HTTP_EXCEPTION(498, exHTTPInvalidToken);
TAPI_ADD_HTTP_EXCEPTION(499, exHTTPTokenRequired);
//-----------------------------------------------------------------
TAPI_ADD_HTTP_EXCEPTION(500, exHTTPInternalServerError);
TAPI_ADD_HTTP_EXCEPTION(501, exHTTPNotImplemented);
TAPI_ADD_HTTP_EXCEPTION(502, exHTTPBadGateway);
TAPI_ADD_HTTP_EXCEPTION(503, exHTTPServiceUnavailable);
TAPI_ADD_HTTP_EXCEPTION(504, exHTTPGatewayTimeout);
TAPI_ADD_HTTP_EXCEPTION(505, exHTTPVersionNotSupported);
TAPI_ADD_HTTP_EXCEPTION(506, exHTTPVariantAlsoNegotiates);
TAPI_ADD_HTTP_EXCEPTION(507, exHTTPInsufficientStorage);
TAPI_ADD_HTTP_EXCEPTION(508, exHTTPLoopDetected);
TAPI_ADD_HTTP_EXCEPTION(510, exHTTPNotExtended);
TAPI_ADD_HTTP_EXCEPTION(511, exHTTPNetworkAuthenticationRequired);
TAPI_ADD_HTTP_EXCEPTION(520, exHTTPUnknownError);

#pragma clang diagnostic pop
/*******************************************************************************/

}
}
#endif // TAPI_HTTPEXCEPTIONS_H
