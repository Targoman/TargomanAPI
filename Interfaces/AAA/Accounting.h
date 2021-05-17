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
 */

#ifndef TARGOMAN_API_AAA_ACCOUNTING_H
#define TARGOMAN_API_AAA_ACCOUNTING_H

#include "Interfaces/AAA/Accounting_Interfaces.h"

namespace Targoman {
namespace API {
namespace AAA {
namespace Accounting{

class clsRESTAPIWithAccounting : public ORM::clsRESTAPIWithActionLogs{
    Q_OBJECT

public:
    stuActiveServiceAccount activeAccountObject(quint32 _usrID);

protected:
    clsRESTAPIWithAccounting(const QString& _schema,
                             const QString& _module,
                             PackageRemainingCols_t _packageRemainingCols,
                             intfAccountPackages* _packages,
                             intfAccountUserPackages* _userPackages,
                             intfAccountUsage* _usage,
                             intfAccountDiscounts* _discounts = nullptr,
                             intfAccountPrizes* _prizes = nullptr);
    virtual ~clsRESTAPIWithAccounting();
    virtual stuServiceAccountInfo retrieveServiceAccountInfo(quint32 _usrID) = 0;
    virtual void breakPackage(quint64 _pkgID) = 0;
    virtual bool isUnlimited(const PackageRemaining_t& _limits) const = 0;
    virtual bool isEmpty(const PackageRemaining_t& _limits) const = 0;

    void hasCredit(const clsJWT& _jwt, const ServiceUsage_t& _requestedUsage);

private:
    stuActiveServiceAccount findActiveAccount(quint32 _usrID, const ServiceUsage_t& _requestedUsage = {});

private slots:
    TAPI::stuPreVoucher REST(POST, addToBasket, (TAPI::JWT_t _JWT,
                                                 TAPI::PackageCode_t _packageCode,
                                                 ///TODO: addittive
                                                 qint16 _qty = 1,
                                                 TAPI::DiscountCode_t _discountCode = {},
                                                 QString _referer = {},
                                                 TAPI::JSON_t _extraRefererParams = {},
                                                 TAPI::stuPreVoucher _lastPreVoucher = {}),
                                "add a package to basket and return updated pre-Voucher")
    ///TODO removeFromBasket
    ///TODO updateItemInBasket

protected:
    QScopedPointer<intfAccountPackages> AccountPackages;
    QScopedPointer<intfAccountUserPackages> AccountUserPackages;
    QScopedPointer<intfAccountUsage> AccountUsage;
    QScopedPointer<intfAccountDiscounts> AccountDiscounts;
    QScopedPointer<intfAccountPrizes> AccountPrizes;

private:
    QString ServiceName;

    PackageRemainingCols_t PackageRemainingCols;
    QStringList PackageRemainingColName;

};

extern clsRESTAPIWithAccounting* serviceAccounting(const QString& _serviceName);

}
}
}
}

#endif // TARGOMAN_API_AAA_ACCOUNTING_H
