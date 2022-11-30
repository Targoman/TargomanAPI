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

#include "LanguageHelper.h"

namespace Targoman::API::Helpers {

QString LanguageHelper::getI18NClauseForCoalesce(
    INTFAPICALLCONTEXT_IMPL &_apiCallContext,
    const QString &_i18nTableName,
    const QString &_fieldName,
    const QString &_i18nDataFieldName
) {
    QString FieldPath = "$";
    if (_fieldName.isEmpty() == false)
        FieldPath += "." + _fieldName;

    QString Result = QString("JSON_UNQUOTE(JSON_EXTRACT(%1.%2, '%3.\"%4\"'))")
                     .arg(_i18nTableName)
                     .arg(_i18nDataFieldName)
                     .arg(FieldPath)
                     .arg(_apiCallContext.language())
                     ;

    if (_apiCallContext.language().contains("-"))
        Result += ","
                  + QString("JSON_UNQUOTE(JSON_EXTRACT(%1.%2, '%3.%4'))")
                  .arg(_i18nTableName)
                  .arg(_i18nDataFieldName)
                  .arg(FieldPath)
                  .arg(_apiCallContext.language().split("-").first());

    return Result;
}

} //namespace Targoman::API::Helpers
