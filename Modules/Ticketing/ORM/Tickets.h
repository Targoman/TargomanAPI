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

#ifndef TARGOMAN_API_MODULES_TICKETING_ORM_ACTIVEADS_H
#define TARGOMAN_API_MODULES_TICKETING_ORM_ACTIVEADS_H

#include "Interfaces/ORM/clsTable.h"

namespace Targoman {
namespace API {

TARGOMAN_DEFINE_ENUM(enuTicketStatus,
                     New       = 'N',
                     Removed   = 'R'
                     )
class Tickets : public ORM::clsTable
{
    Q_OBJECT
private slots:
    QVariant ORMGET("Get Tickets")


private:
    TARGOMAN_DEFINE_API_SUBMODULE(Ticketing, Tickets)
};

class TicketRead : public ORM::clsTable
{
    Q_OBJECT
private:
    TARGOMAN_DEFINE_API_SUBMODULE(Ticketing, TicketRead)
};
}
}

Q_DECLARE_METATYPE(Targoman::API::enuTicketStatus::Type);

#endif // TARGOMAN_API_MODULES_TICKETING_ORM_ACTIVEADS_H
