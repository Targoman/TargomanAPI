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

#include "intfI18N.h"
#include "Interfaces/AAA/Authorization.h"
#include "Interfaces/AAA/AAADefs.hpp"
#include "Interfaces/AAA/clsJWT.hpp"
#include "Interfaces/Helpers/LanguageHelper.h"

using namespace Targoman::API::AAA;

namespace Targoman::API::ORM {

using namespace Helpers;

intfI18N::intfI18N(
    const QString &_module,
    const QString &_schema
) :
    intfSQLBasedModule(
        _module,
        _schema,
        tblI18N::Name,
        tblI18N::Private::ORMFields,
        tblI18N::Private::Relations,
        tblI18N::Private::Indexes
    ) { ; }

QString intfI18N::translated(
    INTFAPICALLCONTEXT_IMPL &_apiCallContext,
    const QString &_key
) {
    auto Data = this->makeSelectQuery(_apiCallContext)
                .addCol(tblI18N::Fields::i18nKey)
                .addCol(DBExpression::VALUE(QString("COALESCE("
                                                    + LanguageHelper::getI18NClauseForCoalesce(_apiCallContext, tblI18N::Name, "", tblI18N::Fields::i18nValue) + ","
                                                    "JSON_UNQUOTE(JSON_EXTRACT(%1.%2, '$.default')),"
                                                    "'%3')")
                                            .arg(tblI18N::Name)
                                            .arg(tblI18N::Fields::i18nValue)
                                            .arg(_key)
                                            ),
                        "Translated")
                .where({ tblI18N::Fields::i18nKey,
                         enuConditionOperator::Equal,
                         _key
                       })
                .tryOne();

    if (Data.isEmpty())
        return _key;

    return Data["Translated"].toString().trimmed();
}

QVariant IMPL_ORMGET_ANONYMOUSE(intfI18N) {
    return this->Select(GET_METHOD_ARGS_CALL_VALUES);
}

} //namespace Targoman::API::ORM
