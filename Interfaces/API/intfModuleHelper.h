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

#ifndef TARGOMAN_API_INTFMODULEHELPER_H
#define TARGOMAN_API_INTFMODULEHELPER_H

#include <QObject>
#include "Interfaces/Server/ServerCommon.h"
using namespace Targoman::API::Server;

#define TARGOMAN_API_MODULEHELPER_DEFINE_DB_CONFIGS(_module) \
    struct DB { \
        static inline QString makeConfig(const QString& _name) { return QString("/ModuleHelper_%1/DB/%2").arg(TARGOMAN_M2STR(_module), _name); } \
        static Targoman::Common::Configuration::tmplConfigurable<QString>       Host;   \
        static Targoman::Common::Configuration::tmplRangedConfigurable<quint16> Port;   \
        static Targoman::Common::Configuration::tmplConfigurable<QString>       User;   \
        static Targoman::Common::Configuration::tmplConfigurable<QString>       Pass;   \
        static Targoman::Common::Configuration::tmplConfigurable<QString>       Schema; \
    }; \
public: \
    virtual stuModuleDBInfo requiredDB() const { \
        return stuModuleDBInfo( \
            DB::Schema.value(), \
            DB::Port.value(), \
            DB::Host.value(), \
            DB::User.value(), \
            DB::Pass.value() \
        ); \
    }

#define TARGOMAN_API_MODULEHELPER_IMPLEMENT_DB_CONFIG(_module, _schema)\
    using namespace Targoman::Common::Configuration;        \
    tmplConfigurable<QString> _module::DB::Host(            \
        _module::DB::makeConfig("Host"),                    \
        "Database Host address",                            \
        ""                                                  \
    );                                                      \
    tmplRangedConfigurable<quint16> _module::DB::Port(      \
        _module::DB::makeConfig("Port"),                    \
        "Database port",                                    \
        10, 65000,                                          \
        3306                                                \
    );                                                      \
    tmplConfigurable<QString> _module::DB::User(            \
        _module::DB::makeConfig("User"),                    \
        "Database username for connection",                 \
        "root"                                              \
    );                                                      \
    tmplConfigurable<QString> _module::DB::Pass(            \
        _module::DB::makeConfig("Pass"),                    \
        "Database password",                                \
        ""                                                  \
    );                                                      \
    tmplConfigurable<QString> _module::DB::Schema(          \
        _module::DB::makeConfig("Schema"),                  \
        "Database schema",                                  \
        _schema                                             \
    );                                                      \

namespace Targoman::API::API {

class intfModuleHelper : public QObject
{
    Q_OBJECT

public:
    intfModuleHelper();
};

} // namespace Targoman::API::API

#endif // TARGOMAN_API_INTFMODULEHELPER_H
