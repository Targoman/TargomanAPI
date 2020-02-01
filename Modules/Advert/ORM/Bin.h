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

#ifndef TARGOMAN_API_MODULES_ADVERT_ORM_BIN_H
#define TARGOMAN_API_MODULES_ADVERT_ORM_BIN_H

#include "Interfaces/ORM/clsTable.h"
#include "Interfaces/AAA/AAA.hpp"

namespace Targoman {
namespace API {
namespace Advertisement {

class Bin : public ORM::clsTable
{
    Q_OBJECT
private slots:
    QVariant ORMGET("Get Bin information")

private:
    TARGOMAN_DEFINE_API_SUBMODULE(Advert,Bin)
};

class Banners : public ORM::clsTable
{
    Q_OBJECT
public:
    void init();

private:
    TARGOMAN_DEFINE_API_SUBMODULE(Advert,Banners)
};
}
}
}
#endif // TARGOMAN_API_MODULES_ADVERT_ORM_BIN_H
