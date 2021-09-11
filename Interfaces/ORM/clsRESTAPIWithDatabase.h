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

#ifndef TARGOMAN_API_ORM_CLSRESTAPIWITHDATABASE_H
#define TARGOMAN_API_ORM_CLSRESTAPIWITHDATABASE_H

#include "Interfaces/ORM/clsRESTAPI.h"

namespace Targoman::API::ORM {

class clsRESTAPIWithDatabase : public clsRESTAPI
{
    Q_OBJECT

public:
    clsRESTAPIWithDatabase(
            const QString& _module,
            const QString& _schema,
            const QString& _name,
            const QList<clsORMField>& _cols = {},
            const QList<stuRelation>& _relations = {},
            const QList<stuDBIndex>& _indexes = {},
            const QVariantMap& _dbProperties = {}
            );

//#ifdef QT_DEBUG
//protected slots:
//    QVariant REST(
//        POST,
//        fixtureSetup,
//        (
//                TAPI::RemoteIP_t _REMOTE_IP
//        ),
//        "Create sample data"
//    )

//    QVariant REST(
//        POST,
//        fixtureCleanup,
//        (
//                TAPI::RemoteIP_t _REMOTE_IP
//        ),
//        "Cleanup sample data"
//    )
//protected:
//    virtual QVariant fixtureSetup(TAPI::RemoteIP_t _REMOTE_IP) { Q_UNUSED(_REMOTE_IP); return QVariant(); }
//    virtual QVariant fixtureCleanup(TAPI::RemoteIP_t _REMOTE_IP) { Q_UNUSED(_REMOTE_IP); return QVariant(); }
//#endif
};

} // namespace Targoman::API::ORM

#endif // TARGOMAN_API_ORM_CLSRESTAPIWITHDATABASE_H
