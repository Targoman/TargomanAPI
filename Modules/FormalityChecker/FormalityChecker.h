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
 * @author S. Mehran M. Ziabary <ziabary@targoman.com>
 */

#ifndef TARGOMAN_API_MODULES_FORMALITYCHECKER_FORMALITYCHECKER_H
#define TARGOMAN_API_MODULES_FORMALITYCHECKER_FORMALITYCHECKER_H

#include "Interfaces/API/intfPureModule.h"

namespace Targoman {
namespace API {

//#ifndef API
//#define API(_method, _name, _sig, _doc) api##_method##_name _sig; QString signOf##_method##_name() { return #_sig; } QString docOf##_method##_name() { return _doc; }
//#endif

class FormalityChecker : public intfPureModule
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID INTFPUREMODULE_IID)
    Q_INTERFACES(Targoman::API::API::intfPureModule)
    TARGOMAN_DEFINE_API_MODULE(FormalityChecker);

public:
    bool init();

private slots:
    QString REST_GET_OR_POST(
        check,
        (
            APICALLBOOM_TYPE_NO_JWT_DECL &APICALLBOOM_PARAM,
            const QString _text,
            const TAPI::ISO639_2_t& _lang
        ),
        "Normalizes input text"
    )

};

}
}

#endif // TARGOMAN_API_MODULES_FORMALITYCHECKER_FORMALITYCHECKER_H
