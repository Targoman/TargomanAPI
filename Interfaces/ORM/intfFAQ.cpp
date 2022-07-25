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

#include "intfFAQ.h"
#include "Interfaces/AAA/Authorization.h"
#include "Interfaces/AAA/AAADefs.hpp"
#include "Interfaces/AAA/clsJWT.hpp"

using namespace Targoman::API::AAA;

namespace Targoman::API::ORM {

/*********************************************************\
|** intfFaqQuestions *************************************|
\*********************************************************/
intfFaqQuestions::intfFaqQuestions(
    const QString& _module,
    const QString& _schema
) :
intfSQLBasedModule(
    _module,
    _schema,
    tblFaqQuestions::Name,
    tblFaqQuestions::Private::ORMFields,
    tblFaqQuestions::Private::Relations(_schema),
    tblFaqQuestions::Private::Indexes
) { ; }

QVariant IMPL_ANONYMOUSE_ORMGET(intfFaqQuestions) {
    return this->Select(GET_METHOD_ARGS_CALL_INTERNAL_BOOM);
}
quint32 IMPL_ORMCREATE(intfFaqQuestions) {
    Authorization::checkPriv(APICALLBOOM_PARAM, this->privOn(EHTTP_PUT, this->moduleBaseName()));
    return this->Create(CREATE_METHOD_ARGS_CALL_INTERNAL_BOOM);
}

bool IMPL_ORMUPDATE(intfFaqQuestions) {
    Authorization::checkPriv(APICALLBOOM_PARAM, this->privOn(EHTTP_PATCH, this->moduleBaseName()));
    return this->Update(UPDATE_METHOD_ARGS_CALL_INTERNAL_BOOM);
}

bool IMPL_ORMDELETE(intfFaqQuestions) {
    Authorization::checkPriv(APICALLBOOM_PARAM, this->privOn(EHTTP_DELETE, this->moduleBaseName()));
    return this->DeleteByPks(DELETE_METHOD_ARGS_CALL_INTERNAL_BOOM);
}

/*********************************************************\
|** intfFaqAnswers ***************************************|
\*********************************************************/
intfFaqAnswers::intfFaqAnswers(
    const QString& _module,
    const QString& _schema
) :
intfSQLBasedModule(
    _module,
    _schema,
    tblFaqAnswers::Name,
    tblFaqAnswers::Private::ORMFields,
    tblFaqAnswers::Private::Relations(_schema),
    tblFaqAnswers::Private::Indexes
) { ; }

QVariant IMPL_ANONYMOUSE_ORMGET(intfFaqAnswers) {
    return this->Select(GET_METHOD_ARGS_CALL_INTERNAL_BOOM);
}
quint32 IMPL_ORMCREATE(intfFaqAnswers) {
    Authorization::checkPriv(APICALLBOOM_PARAM, this->privOn(EHTTP_PUT, this->moduleBaseName()));
    return this->Create(CREATE_METHOD_ARGS_CALL_INTERNAL_BOOM);
}

bool IMPL_ORMUPDATE(intfFaqAnswers) {
    Authorization::checkPriv(APICALLBOOM_PARAM, this->privOn(EHTTP_PATCH, this->moduleBaseName()));
    return this->Update(UPDATE_METHOD_ARGS_CALL_INTERNAL_BOOM);
}

bool IMPL_ORMDELETE(intfFaqAnswers) {
    Authorization::checkPriv(APICALLBOOM_PARAM, this->privOn(EHTTP_DELETE, this->moduleBaseName()));
    return this->DeleteByPks(DELETE_METHOD_ARGS_CALL_INTERNAL_BOOM);
}

} //namespace Targoman::API::ORM
