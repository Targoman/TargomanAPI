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

#ifndef TARGOMAN_API_FAQ_H
#define TARGOMAN_API_FAQ_H

#include "Interfaces/API/intfSQLBasedModule.h"

namespace Targoman::API::ORM {

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-variable"

namespace tblFaqQuestions {
    constexpr char Name[] = "tblFaqQuestions";

    namespace Fields {
        TARGOMAN_CREATE_CONSTEXPR(qsnID);
        TARGOMAN_CREATE_CONSTEXPR(qsnBody);
    }

    namespace Relation {
        // constexpr char AAA[] = "aaa";
    }

    namespace Private {
        const QList<clsORMField> ORMFields = {
            //ColName           Type                    Validation  Default     UpBy   Sort  Filter Self  Virt   PK
            { Fields::qsnID,    ORM_PRIMARYKEY_32 },
            { Fields::qsnBody,  S(QString),             QFV,        QRequired,  UPAdmin },

        };

        inline const QList<stuRelation> Relations(Q_DECL_UNUSED const QString& _schema) {
            return {
                //Col                                Reference Table                 ForeignCol
            };
        };

        const QList<stuDBIndex> Indexes = {
        };

    } //namespace Private

    TAPI_DEFINE_STRUCT(DTO,
        SF_quint16                  (qsnID),
        SF_QString                  (qsnBody)
    );
}

namespace tblFaqAnswers {
    constexpr char Name[] = "tblFaqAnswers";

    namespace Fields {
        TARGOMAN_CREATE_CONSTEXPR(ansID);
        TARGOMAN_CREATE_CONSTEXPR(ans_qsnID);
        TARGOMAN_CREATE_CONSTEXPR(ansBody);
    }

    namespace Relation {
         constexpr char Question[] = "question";
    }

    namespace Private {
        const QList<clsORMField> ORMFields = {
            //ColName               Type                    Validation  Default     UpBy   Sort  Filter Self  Virt   PK
            { Fields::ansID,        ORM_PRIMARYKEY_32 },
            { Fields::ans_qsnID,    S(quint16),             QFV,        QRequired,  UPAdmin },
            { Fields::ansBody,      S(QString),             QFV,        QRequired,  UPAdmin },
        };

        inline const QList<stuRelation> Relations(Q_DECL_UNUSED const QString& _schema) {
            return {
                //Col                                Reference Table                 ForeignCol
                { Relation::Question,
                  { Fields::ans_qsnID,   R(_schema, tblFaqQuestions::Name),     tblFaqQuestions::Fields::qsnID } },
            };
        };

        const QList<stuDBIndex> Indexes = {
        };

    } //namespace Private

    TAPI_DEFINE_STRUCT(DTO,
        SF_quint16                  (ansID),
        SF_quint16                  (ans_qsnID),
        SF_QString                  (ansBody)
    );
}

#pragma GCC diagnostic pop

class intfFaqQuestions : public intfSQLBasedModule
{
    Q_OBJECT

public:
    intfFaqQuestions(
        const QString& _module,
        const QString& _schema
    );

private slots:
    QVariant ORMGET_ANONYMOUSE("Get Faq Questions")
    quint32 ORMCREATE_USER("Create a new Faq Question")
    bool ORMUPDATE_USER("Update Faq Question")
    bool ORMDELETE_USER("Delete a Faq Question")
};

class intfFaqAnswers : public intfSQLBasedModule
{
    Q_OBJECT

public:
    intfFaqAnswers(
        const QString& _module,
        const QString& _schema
    );

private slots:
    QVariant ORMGET_ANONYMOUSE("Get Faq Answers")
    quint32 ORMCREATE_USER("Create a new Faq Answer")
    bool ORMUPDATE_USER("Update Faq Answer")
    bool ORMDELETE_USER("Delete a Faq Answer")
};

} //namespace Targoman::API::ORM

/****************************************************/
//put this macro before module class definition (.h)
#define TARGOMAN_FAQ_PREPARENT \
    class FaqQuestions; \
    class FaqAnswers;

//put this macro after module class definition (.h)
#define TARGOMAN_FAQ_POSTPARENT(_module, _schema) \
    class FaqQuestions : public Targoman::API::ORM::intfFaqQuestions \
    { \
        Q_OBJECT \
        TARGOMAN_API_SUBMODULE_DEFINE_WO_CTOR(_module, FaqQuestions) \
    public: \
        FaqQuestions() : \
            intfFaqQuestions( \
                Targoman::Common::demangle(typeid(_module).name()).split("::").last(), \
                _schema \
            ) \
        { ; } \
    }; \
    class FaqAnswers : public Targoman::API::ORM::intfFaqAnswers \
    { \
        Q_OBJECT \
        TARGOMAN_API_SUBMODULE_DEFINE_WO_CTOR(_module, FaqAnswers) \
    public: \
        FaqAnswers() : \
            intfFaqAnswers( \
                Targoman::Common::demangle(typeid(_module).name()).split("::").last(), \
                _schema \
            ) \
        { ; } \
    };

//put this macro inside module class definition (.h) after TARGOMAN_API_MODULE_DEFINE
#define TARGOMAN_API_MODULE_DEFINE_FAQ(_module, _schema) \
    protected: \
    QScopedPointer<FaqQuestions>  _FaqQuestions; \
    QScopedPointer<FaqAnswers>    _FaqAnswers;

//put this macro before module class constructor (.cpp)
#define TARGOMAN_API_MODULE_IMPLEMENT_FAQ(_module, _schema) \
    TARGOMAN_API_SUBMODULE_IMPLEMENT(_module, FaqQuestions) \
    TARGOMAN_API_SUBMODULE_IMPLEMENT(_module, FaqAnswers)

//put this macro into module class constructor (.cpp)
#define TARGOMAN_API_MODULE_IMPLEMENT_CTOR_FAQ(_module, _schema) \
    this->_FaqQuestions.reset(&FaqQuestions::instance()); \
    this->_FaqAnswers.reset(&FaqAnswers::instance()); \
    this->addSubModule(this->_FaqQuestions.data()); \
    this->addSubModule(this->_FaqAnswers.data());

/****************************************************/
#endif // TARGOMAN_API_FAQ_H
