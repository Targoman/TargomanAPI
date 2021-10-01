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

#ifndef TARGOMAN_API_SERVER_RESTSERVER_H
#define TARGOMAN_API_SERVER_RESTSERVER_H

#include <QObject>
#include <QScopedPointer>
#include <QHostAddress>
#include <QJsonObject>

#include "libTargomanCommon/Macros.h"
#include "libTargomanCommon/Configuration/tmplConfigurable.h"
#include "libTargomanCommon/Configuration/clsFileBasedConfig.hpp"
#include "libTargomanCommon/Configuration/tmplConfigurableMultiMap.hpp"

#include "Interfaces/Common/GenericTypes.h"

#include "Interfaces/Common/intfAPIArgManipulator.h"

namespace Targoman::API {

//class intfAPIArgManipulator;

namespace Server {

/********************************************************************************/
/**
 * @brief The enuIPBlackListStatus enum is defined to be used as result of fnIsInBlackList_t
 */
TARGOMAN_DEFINE_ENHANCED_ENUM(enuIPBlackListStatus,
                              Ok,
                              Banned,
                              Restricted)

/**
 * @brief The fnIsInBlackList_t type is used to define a IP checker lambda
 */
using fnIsInBlackList_t = std::function<enuIPBlackListStatus::Type (QHostAddress)>;

/********************************************************************************/
class RESTServer : public QObject
{    
public:
    instanceGetter(RESTServer)
    /**
     * @brief start will start REST server based on prior configurations
     */
    void start(fnIsInBlackList_t _fnIPInBlackList = {});

    /**
     * @brief stop will stop server and allow reconfiguration
     */
    void stop();

    /**
     * @brief shutdown will shutdown server and remove any timer and allocated resources
     */
    void shutdown();

    /**
     * @brief stats return statistics of the server
     * @return
     */
    TAPI::stuStatistics stats();

    /**
     * @brief registeredAPIs will return a list of all auto-registered API calls
     * @param _showParams if set to `true` will list API parameters else just API name will be output
     * @param _showTypes if set to `true` will show input types of API parameters and return type else just names will be shown
     * @param _prettifyTypes if set to true a pretty and general form of Qt Types will be printed else will print original QMetaType names
     * @return a list of all APIs registered in the server
     */
    QStringList registeredAPIs(bool _showParams = true, bool _showTypes = true, bool _prettifyTypes = true);
    void registerUserDefinedType(const char* _typeName, Common::intfAPIArgManipulator* _argManipulator);

private:
    bool validateConnection(const QHostAddress& _peerAddress, quint16 _peerPort);
private:
    RESTServer();

private:
    fnIsInBlackList_t fnIPInBlackList;
    bool IsStarted;
};

} //namespace Server
} //namespace Targoman::API

#endif // TARGOMAN_API_SERVER_RESTSERVER_H
