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
 @author S. Mehran M. Ziabary <ziabary@targoman.com>
 */
#ifndef TARGOMAN_API_MODULES_TRANSLATION_H
#define TARGOMAN_API_MODULES_TRANSLATION_H

#include "libTargomanCommon/Configuration/tmplConfigurable.h"

#include "Interfaces/ORM/clsRESTAPIWithActionLogs.h"
#include "Interfaces/AAA/AAA.hpp"

namespace Targoman {
namespace API {
namespace Modules {

class MT  : public ORM::clsRESTAPIWithActionLogs
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID INTFAPIMODULE_IID)
    Q_INTERFACES(Targoman::API::intfAPIModule)
    TARGOMAN_API_MODULE_DB_CONFIGS(Account)
public:
    void init();

private slots:
    QVariantMap REST(,Translate,
                    (const TAPI::RemoteIP_t& _REMOTE_IP,
                     const QString& _token,
                     QString _text,
                     QString _dir,
                     const QString& _engine = "NMT",
                     bool _detailed = false,
                     bool _detok = true,
                     bool _dic=false,
                     bool _dicFull = false),
                    "Translates input text if specified engine and language are found.")

    QVariantMap REST(,Test,(const TAPI::RemoteIP_t& _REMOTE_IP, const QString& _token, const QString& _arg),"Test ")

    private:
    TARGOMAN_DEFINE_API_MODULE(MT)
};

}
}
}

#endif // TARGOMAN_API_MODULES_TRANSLATION_H
