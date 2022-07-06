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

#include "URLHelper.h"
#include <QUrl>
#include <QUrlQuery>
#include <QRegularExpression>

namespace Targoman::API::Helpers {

QString URLHelper::domain(QString _url, bool _stripProtocol) {
    if (!_url.startsWith("http://") && !_url.startsWith("https://") && (_url.indexOf("://") < 0))
        _url = "http://" + _url;

    QUrl Url = QUrl(_url);

    QString Domain = Url.host().trimmed().toLower();

    if (Domain.endsWith("/"))
        Domain.chop(1);

    if (_stripProtocol) {
        if (Domain.startsWith("http://"))
            Domain = Domain.mid(7);
        else if (Domain.startsWith("https://"))
            Domain = Domain.mid(8);
    }

    return Domain;
};

QString URLHelper::addParameter(const QString &_url, const QString& _paramName, const QVariant& _value) {
    QUrl Url = QUrl(_url);

    QUrlQuery UrlQuery = QUrlQuery(Url);
    UrlQuery.addQueryItem(_paramName, _value.toString());

    Url.setQuery(UrlQuery);

    return Url.toString();
}

QString URLHelper::normalize(const QString &_url) {
    QUrl Url = QUrl(_url);
    QString Schema = Url.scheme();
    QString Result = Url.toString(QUrl::FormattingOptions(QUrl::RemoveScheme)).replace(QRegularExpression("//+"), "/");
    if (Result.startsWith("/") == false)
        Result = "/" + Result;
    Result = (Schema.isEmpty() ? "" : Schema + ":/") + Result;
    return Result;
}

} //namespace Targoman::API::Helpers
