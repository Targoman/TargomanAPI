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
 * @author Kambiz Zandi <kambizzandi@gmail.com>
 */

#include "TargomanMT.h"

#include "libQFieldValidator/QFieldValidator.h"
#include "Interfaces/AAA/PrivHelpers.h"
#include "Interfaces/Common/GenericEnums.hpp"
#include "Interfaces/Common/HTTPExceptions.hpp"
#include "Interfaces/Helpers/SecurityHelper.h"
using namespace Targoman::API::Helpers;

#include "ModuleHelpers/MT/MTHelper.h"
using namespace Targoman::API::ModuleHelpers::MT;

#include "TargomanMTDefs.hpp"
#include "ORM/MTHelpers.h"
#include "ORM/Accounting.h"

#include "Interfaces/Helpers/RESTClientHelper.h"
#include "Interfaces/Helpers/FixtureHelper.h"
using namespace Targoman::API::Helpers;

using namespace Targoman::API::AAA;

namespace Targoman::API::TargomanMTModule {

using namespace ORM;

TARGOMAN_API_MODULE_IMPLEMENT(TargomanMT)
//---------------------------------------------------------
TARGOMAN_API_MODULE_IMPLEMENT_DB_CONFIG(TargomanMT, TargomanMTSchema)
//---------------------------------------------------------
TARGOMAN_API_MODULE_IMPLEMENT_MIGRATIONS(TargomanMT, TargomanMTSchema);
TARGOMAN_API_MODULE_IMPLEMENT_ACTIONLOG(TargomanMT, TargomanMTSchema);
TARGOMAN_API_MODULE_IMPLEMENT_OBJECTSTORAGE(TargomanMT, TargomanMTSchema)
TARGOMAN_API_MODULE_IMPLEMENT_FAQ(TargomanMT, TargomanMTSchema);

TargomanMT::TargomanMT() :
    intfAccountingBasedModule(
        TargomanMTDomain,
        TargomanMTSchema,
        false,
        {
            //           day                week   month                total
//            { "show",  { "slbShowPerDay",   {},    {},                  "slbShowTotal" } },
//            { "click", { "slbClicksPerDay", {},    "slbClicksPerMonth", "slbClicksTotal" } },
        },
        &AccountUnits::instance(),
        &AccountProducts::instance(),
        &AccountSaleables::instance(),
        &AccountSaleablesFiles::instance(),
        &AccountUserAssets::instance(),
        &AccountUserAssetsFiles::instance(),
        &AccountAssetUsage::instance(),
        &AccountCoupons::instance()
    ),
    DerivedHelperSubmodules(
        &CorrectionRules::instance(),
        &DigestedTranslationLogs::instance(),
        &MultiDic::instance(),
        &TokenStats::instance(),
        &TranslatedPhrases::instance(),
        &TranslationLogs::instance()
) {
    TARGOMAN_API_MODULE_IMPLEMENT_CTOR_MIGRATIONS(Targoman, TargomanMTSchema);
    TARGOMAN_API_MODULE_IMPLEMENT_CTOR_ACTIONLOG(Targoman, TargomanMTSchema);
    TARGOMAN_API_MODULE_IMPLEMENT_CTOR_OBJECTSTORAGE(Targoman, TargomanMTSchema)
    TARGOMAN_API_MODULE_IMPLEMENT_CTOR_FAQ(Targoman, TargomanMTSchema);

    this->addSubModule(&CorrectionRules::instance());
    this->addSubModule(&DigestedTranslationLogs::instance());
    this->addSubModule(&MultiDic::instance());
    this->addSubModule(&TokenStats::instance());
    this->addSubModule(&TranslatedPhrases::instance());
    this->addSubModule(&TranslationLogs::instance());

    this->addSubModule(AccountUnits.data());
    this->addSubModule(AccountProducts.data());
    this->addSubModule(AccountSaleables.data());
    this->addSubModule(AccountSaleablesFiles.data());
    this->addSubModule(AccountUserAssets.data());
    this->addSubModule(AccountUserAssetsFiles.data());
    this->addSubModule(AccountAssetUsages.data());
    this->addSubModule(AccountCoupons.data());
    //this->addSubModule(AccountPrizes); // There is no prize in Targoman module
}

QMap<QString, stuModuleDBInfo> TargomanMT::requiredDBs() const {
    QMap<QString, stuModuleDBInfo> RequiredDBs = intfSQLBasedModule::requiredDBs();

//    RequiredDBs.insert("MTHelper", MTHelper::instance().requiredDB());

    return RequiredDBs;
}

void TargomanMT::initializeModule() {
    MTHelper::instance().initialize();
}

stuServiceCreditsInfo TargomanMT::retrieveServiceCreditsInfo(quint64 _usrID) {
    ///@TODO: complete this
    return stuServiceCreditsInfo(
                {},
                NULLABLE_NULL_VALUE,
                NULLABLE_NULL_VALUE,
                {},
                {}
                );
}

void TargomanMT::breakCredit(quint64 _slbID) {
}

bool TargomanMT::isUnlimited(const UsageLimits_t& _limits) const {
}

bool TargomanMT::isEmpty(const UsageLimits_t& _limits) const {
}

void TargomanMT::computeAdditives(
    INTFAPICALLBOOM_IMPL    &APICALLBOOM_PARAM,
    INOUT stuAssetItem      &_assetItem,
    const stuVoucherItem    *_oldVoucherItem /*= nullptr*/
) {
    ///@TODO: [very important] complete this
//    qDebug() << "----------" << "_orderAdditives:" << _orderAdditives;
//    AssetItem.UnitPrice *= 1.1;
};

void TargomanMT::computeReferrer(
    INTFAPICALLBOOM_IMPL    &APICALLBOOM_PARAM,
    INOUT stuAssetItem      &_assetItem,
    const stuVoucherItem    *_oldVoucherItem /*= nullptr*/
) {
    ///@TODO: [very important] complete this

#if 0 //def QT_DEBUG
    ///::: SAMPLE CODE :::

    //1: add, modify or remove credit voucher for fp.com
    if (_oldVoucherItem != nullptr) {
        if (_oldVoucherItem->Qty != _assetItem.Qty) {
            //remove old
            QList<stuPendingVoucher>::iterator it = _assetItem.Private.PendingVouchers.begin();
            while (it != _assetItem.Private.PendingVouchers.end()) {
                if ((it->Name == PENDING_VOUCHER_NAME_REFERRER_PRIZE)
                    && (it->Info.contains("referrer"))
                ) {
                    it = _assetItem.Private.PendingVouchers.erase(it);
                    continue;
                }

                ++it;
            }
        }
    }

    if (_assetItem.Qty > 0) {
        _assetItem.Private.PendingVouchers.append({
            /* Name     */ PENDING_VOUCHER_NAME_REFERRER_PRIZE,
            /* Type     */ enuVoucherType::Prize, //Credit,
            /* Amount   */ 2000 * _assetItem.Qty,
            /* Info     */ {
                               { "referrer", _assetItem.Referrer },
                           },
        });
    }

    //2: add, modify or remove system discount
    this->computeSystemDiscount(APICALLBOOM_PARAM, _assetItem, {
                                  QString("referrer_%1").arg("fp.com"),
                                  "5% off by fp.com",
                                  5,
                                  enuDiscountType::Percent,
                                  10'000
                              }, _oldVoucherItem);

    //3: inc translate words max limit (30'000 -> 35'000)
//    int IncAmount = 5'000;
//    if (_assetItem.AdditionalInfo.contains("plus-max-words"))
//        _assetItem.AdditionalInfo["plus-max-words"] = IncAmount + _assetItem.AdditionalInfo["plus-max-words"].toInt();
//    else
//        _assetItem.AdditionalInfo.insert("plus-max-words", IncAmount);

    //4: inc days (30 -> 40)
    int IncDays = 10;
    if (_assetItem.AdditionalInfo.contains(ASSET_ITEM_ADDITIONAL_INTO_KEY_PLUS_MAX_DAYS))
        _assetItem.AdditionalInfo[ASSET_ITEM_ADDITIONAL_INTO_KEY_PLUS_MAX_DAYS] = IncDays + _assetItem.AdditionalInfo[ASSET_ITEM_ADDITIONAL_INTO_KEY_PLUS_MAX_DAYS].toInt();
    else
        _assetItem.AdditionalInfo.insert(ASSET_ITEM_ADDITIONAL_INTO_KEY_PLUS_MAX_DAYS, IncDays);
#endif
};

QVariantMap TargomanMT::getCustomUserAssetFieldsForQuery(
    INTFAPICALLBOOM_IMPL    &APICALLBOOM_PARAM,
    INOUT stuAssetItem      &_assetItem,
    const stuVoucherItem    *_oldVoucherItem /*= nullptr*/
) {
    ///@TODO: [very important] complete this

    QVariantMap Result;

//    if (_assetItem.AdditionalInfo.contains(ASSET_ITEM_ADDITIONAL_INTO_KEY_PLUS_MAX_DAYS))
//        Result.insert(tblAccountUserAssets::ExtraFields::uasDays, _assetItem.AdditionalInfo[ASSET_ITEM_ADDITIONAL_INTO_KEY_PLUS_MAX_DAYS].toInt());

    return Result;
}

/****************************************************************\
|** API Methods *************************************************|
\****************************************************************/
QVariantMap IMPL_REST_GET_OR_POST(TargomanMT, Translate, (
    APICALLBOOM_TYPE_JWT_USER_OR_ANONYMOUSE_IMPL &APICALLBOOM_PARAM,
    QString _text,
    QString _dir,
    bool _detailed,
    bool _detok,
    bool _dic,
    bool _dicFull
)) {
    QString _engine = "NMT";

    if (APICALLBOOM_PARAM.isAnonymouse()) {

    } else {

    }

    QTime Timer, OverallTime;
    Timer.start();
    OverallTime.start();

    int PreprocessTime = 0;

    _text = _text.trimmed();
    if (_text.isEmpty())
        throw exHTTPBadRequest("Input text must not be empty");

    _dir = _dir.replace('_', '2');

    TranslationDir_t Dir = MTHelper::dirLangs(_dir);
    if (Dir.first.isNull())
        throw exHTTPBadRequest("Invalid translation direction format");

    if (!MTHelper::instance().isValidEngine(_engine, Dir) == false)
        throw exHTTPBadRequest("Invalid engine/direction combination");

//    QJsonObject TokenInfo = Authorization::retrieveTokenInfo(_token,
//                                                             _REMOTE_IP, {
//                                                                 TARGOMAN_PRIV_PREFIX + _engine,
//                                                                 TARGOMAN_PRIV_PREFIX + _dir,
//                                                                 _dic ? (TARGOMAN_PRIV_PREFIX + "Dic") : QString(),
//                                                                 _dicFull ? (TARGOMAN_PRIV_PREFIX + "DicFull") : QString()
//                                                             });


//    QJsonObject Stats = this->execQuery(
//            "SELECT * FROM tblTokenStats "
//            "WHERE tks_tokID = ? "
//            "  AND tksEngine=? "
//            "  AND tksDir=? ",
//    {
//        {TokenInfo[TOKENItems::tokID]},
//        {_engine},
//        {_dir},
//    }
//    ).toJson(true).object ();

//    if (Stats.isEmpty())
//        this->execQuery("INSERT IGNORE INTO tblTokenStats (tks_tokID,tksEngine,tksDir) VALUES(?, ?, ?)", {
//        {TokenInfo[TOKENItems::tokID]},
//        {_engine},
//        {_dir},
//    });

    _text = MTHelper::instance().tokenize(_text, Dir.first);
    quint64 SourceWordCount = static_cast<quint64>(_text.split(' ').size());

//    QJsonObject Privs = Authorization::privObjectFromInfo(TokenInfo);

    //@TODO: fix
//    Accounting::checkCredit(Privs, TARGOMAN_QUOTA_PREFIX+_engine+"MaxPerDay", Stats["tksTodayWords"].toDouble()+ SourceWordCount);
//    Accounting::checkCredit(Privs, TARGOMAN_QUOTA_PREFIX+_engine+"MaxPerMonth", Stats["tksThisMonthWords"].toDouble()+ SourceWordCount);
//    Accounting::checkCredit(Privs, TARGOMAN_QUOTA_PREFIX+_engine+"MaxTotal", Stats["tksTotalWords"].toDouble()+ SourceWordCount);
//    Accounting::checkCredit(Privs, TARGOMAN_QUOTA_PREFIX+_dir+"MaxPerDay", Stats["tksTodayWords"].toDouble()+ SourceWordCount);
//    Accounting::checkCredit(Privs, TARGOMAN_QUOTA_PREFIX+_dir+"MaxPerMonth", Stats["tksThisMonthWords"].toDouble()+ SourceWordCount);
//    Accounting::checkCredit(Privs, TARGOMAN_QUOTA_PREFIX+_dir+"MaxTotal", Stats["tksTotalWords"].toDouble()+ SourceWordCount);
//    Accounting::checkCredit(Privs, TARGOMAN_QUOTA_PREFIX+"MaxPerDay", Stats["tksTodayWords"].toDouble()+ SourceWordCount);
//    Accounting::checkCredit(Privs, TARGOMAN_QUOTA_PREFIX+"MaxPerMonth", Stats["tksThisMonthWords"].toDouble()+ SourceWordCount);
//    Accounting::checkCredit(Privs, TARGOMAN_QUOTA_PREFIX+"MaxTotal", Stats["tksTotalWords"].toDouble()+ SourceWordCount);

    if (_dic) {
        if (Authorization::hasPriv(APICALLBOOM_PARAM, { TARGOMAN_PRIV_PREFIX + "Dic" })) {
            if (_dicFull && Authorization::hasPriv(APICALLBOOM_PARAM, { TARGOMAN_PRIV_PREFIX + "DicFull" }))
                throw exAuthorization("Not enought privileges to retrieve dictionary full response.");

            PreprocessTime = Timer.elapsed();
            Timer.restart();

            QVariantMap DicResponse = MTHelper::instance().retrieveDicResponse(_text, Dir.first);
            if (DicResponse.size()) {
                if (_detailed) {
                    DicResponse[RESULTItems::TIMES] = QVariantMap({
                                                                      { RESULTItems::TIMESItems::PRE, PreprocessTime },
                                                                      { RESULTItems::TIMESItems::TR, Timer.elapsed() },
                                                                      { RESULTItems::TIMESItems::POST, 0} ,
                                                                      { RESULTItems::TIMESItems::ALL, PreprocessTime+Timer.elapsed() },
                                                                  });
                }

                MTHelper::instance().addDicLog(Dir.first, SourceWordCount, _text);
                return DicResponse;
            }
        } else
            throw exAuthorization("Not enought privileges to retrieve dictionary response.");
    }

    PreprocessTime += Timer.elapsed();

    try {
        int InternalPreprocessTime = 0, InternalTranslationTime = 0, InternalPostprocessTime = 0;

        QVariantMap Translation = MTHelper::instance().doTranslation<TAPI::enuTokenActorType::USER>(
                                      APICALLBOOM_PARAM,
//                                      Privs,
                                      this->DerivedHelperSubmodules,
                                      _text,
                                      Dir,
                                      _engine,
                                      true,
                                      _detailed,
                                      _detok,
                                      InternalPreprocessTime,
                                      InternalTranslationTime
                                      );
        Timer.restart();

        if (_detailed) {
            Translation[RESULTItems::TIMES] = QVariantMap({
                                                              { RESULTItems::TIMESItems::PRE, InternalPreprocessTime + PreprocessTime },
                                                              { RESULTItems::TIMESItems::TR, InternalTranslationTime },
                                                              { RESULTItems::TIMESItems::POST, InternalPostprocessTime + Timer.elapsed() },
                                                              { RESULTItems::TIMESItems::ALL, OverallTime.elapsed() },
                                                          });
        } else
            Translation[RESULTItems::TIME] = OverallTime.elapsed();

//        MTHelper::instance().addTranslationLog(static_cast<quint64>(TokenInfo[TOKENItems::tokID].toInt()), _engine, _dir, SourceWordCount, _text, OverallTime.elapsed());

        if (Authorization::hasPriv(APICALLBOOM_PARAM, { TARGOMAN_PRIV_PREFIX + "ReportServer" }) == false)
            Translation.remove(RESULTItems::SERVERID);

        return Translation;

    } catch (exTargomanBase &_exp) {
        MTHelper::instance().addErrorLog(
                    //static_cast<quint64>(TokenInfo[TOKENItems::tokID].toInt()),
                    APICALLBOOM_PARAM.getActorID(),
                    _engine,
                    _dir,
                    SourceWordCount,
                    _text,
                    _exp.code());

        throw;
    }
}

} //namespace Targoman::API::TargomanMTModule
