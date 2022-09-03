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

#ifndef TARGOMAN_API_MODULES_COMMON_ORM_ALERTTEMPLATES_H
#define TARGOMAN_API_MODULES_COMMON_ORM_ALERTTEMPLATES_H

#include "Interfaces/API/intfSQLBasedModule.h"

namespace Targoman::API::CommonModule {

//structures and enumes goes here

TARGOMAN_DEFINE_ENUM(enuAlertTemplateMedia,
                     Email  = 'E',
                     SMS    = 'S',
                     Push   = 'P',
                     All    = 'A'
                     )

} //namespace Targoman::API::CommonModule {

TAPI_DECLARE_METATYPE_ENUM(Targoman::API::CommonModule, enuAlertTemplateMedia);

namespace Targoman::API::CommonModule {
namespace ORM {

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-variable"

namespace tblAlertTemplates {
    constexpr char Name[] = "tblAlertTemplates";

    namespace Fields {
        TARGOMAN_CREATE_CONSTEXPR(altlID);
        TARGOMAN_CREATE_CONSTEXPR(altCode);
        TARGOMAN_CREATE_CONSTEXPR(altMedia);
        TARGOMAN_CREATE_CONSTEXPR(altLanguage);
        TARGOMAN_CREATE_CONSTEXPR(altTitleTemplate);
        TARGOMAN_CREATE_CONSTEXPR(altBodyTemplate);
        TARGOMAN_CREATE_CONSTEXPR(altParamsPrefix);
        TARGOMAN_CREATE_CONSTEXPR(altParamsSuffix);
    }

    namespace Relation {
    }

    namespace Private {
        const QList<clsORMField> ORMFields = {
            //ColName                       Type                    validation                          Default     UpBy        Sort  Filter Self  Virt   PK
            { Fields::altlID,               ORM_PRIMARYKEY_32 },
            { Fields::altCode,              S(QString),             QFV.asciiAlNum().maxLenght(50),     QRequired,  UPAdmin },
            { Fields::altMedia,             S(Targoman::API::CommonModule::enuAlertTemplateMedia::Type), QFV, QRequired, UPAdmin },
            { Fields::altLanguage,          S(TAPI::ISO639_2_t),    QFV,                                QRequired,  UPAdmin },
            { Fields::altTitleTemplate,     S(QString),             QFV,                                QNull,      UPAdmin },
            { Fields::altBodyTemplate,      S(TAPI::DBText_t),      QFV,                                QRequired,  UPAdmin },
            { Fields::altParamsPrefix,      S(QString),             QFV,                                QRequired,  UPAdmin },
            { Fields::altParamsSuffix,      S(QString),             QFV,                                QNull,      UPAdmin },
        };

        const QList<stuRelation> Relations = {
            //Col                           Reference Table                 ForeignCol                  Rename      LeftJoin
        };

        const QList<stuDBIndex> Indexes = {
        };

    } //namespace Private

    TAPI_DEFINE_STRUCT(DTO,
        SF_ORM_PRIMARYKEY_32        (altlID),
        SF_QString                  (altCode),
        SF_NULLABLE_Enum            (altMedia, Targoman::API::CommonModule::enuAlertTemplateMedia),
        SF_QString                  (apt_usrID),
        SF_QString                  (altLanguage),
        SF_QString                  (altTitleTemplate),
        SF_QString                  (altBodyTemplate),
        SF_QString                  (altParamsPrefix),
        SF_QString                  (altParamsSuffix)
    );
}

#pragma GCC diagnostic pop

/******************************************************/
class AlertTemplates : public intfSQLBasedModule
{
    Q_OBJECT
    TARGOMAN_API_SUBMODULE_DEFINE(Common, AlertTemplates)

private slots:
#ifdef QT_DEBUG
    QVariant ORMGET_ANONYMOUSE("Get Alert Templates")
#else
    QVariant ORMGET_USER("Get Alert Templates")
#endif
    quint32 ORMCREATE_USER("Create a new Alert Template")
    bool ORMUPDATE_USER("Update Alert Template")
    bool ORMDELETE_USER("Delete an Alert Template")
};

} //namespace ORM
} //namespace Targoman::API::CommonModule

#endif // TARGOMAN_API_MODULES_COMMON_ORM_ALERTTEMPLATES_H
