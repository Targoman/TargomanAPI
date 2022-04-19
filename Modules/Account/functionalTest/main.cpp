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

#include <QtTest>
#include "testBase.hpp"
#include "testAccount.hpp"
#include "testAccountFixture.hpp"
#include "testJWT.hpp"

//#include "Interfaces/Helpers/RESTClientHelper.h"
//using namespace Targoman::API::Helpers;

TAPI_MARSHAL_TEST_VARIABLES

/*
//https://cpp.hotexamples.com/examples/-/QString/toLongLong/cpp-qstring-tolonglong-method-examples.html
QVariant readNumber(const QString& _str, bool *ok) {
    //m_settings->locale()
    QString negativeSign = "-";
    QString decimalSymbol = ".";
    QString thousandsSeparator = "'";

    bool isInt = false;
    QString str = _str.trimmed();
    bool neg = str.indexOf(negativeSign) == 0;
    if (neg)
        str.remove(0, negativeSign.length());

//    will hold the scientific notation portion of the number. Example, with 2.34E+23, exponentialPart == "E+23"

    QString exponentialPart;
    int EPos  = str.indexOf('E', 0, Qt::CaseInsensitive);

    if (EPos != -1) {
        exponentialPart = str.mid(EPos);
        str = str.left(EPos);
    }

    int pos;
    int fracPos;
    QString major;
    QString minor;
    if ((pos = str.indexOf(decimalSymbol)) != -1) {
        major = str.left(pos);
        minor = str.mid(pos + decimalSymbol.length());
        isInt = false;
    } else if (((pos = str.indexOf(' ')) != -1) &&
               ((fracPos = str.indexOf('/')) != -1)) {
        // try to parse fractions of this form:
        // [0-9]+ [0-9]+/[1-9][0-9]?
        major = str.left(pos);
        QString numerator = str.mid(pos + 1, (fracPos - pos - 1));
        QString denominator = str.mid(fracPos + 1);
        double minorVal = numerator.toDouble() / denominator.toDouble();
        if (minorVal > 1) {
            // assume major is just a plain number
            double wholePart = floor(minorVal);
            minorVal -= wholePart;
            major = QString("%1").arg(major.toInt() + (int)wholePart);
        }
        minor = QString::number(minorVal, 'f').mid(2);     // chop off the "0." part
        // kDebug() <<"fraction:" << major <<"." << minor;
    } else {
        major = str;
        isInt = (EPos == -1); // only, if no exponential part was found
    }

    // Remove thousand separators
    int thlen = thousandsSeparator.length();
    int lastpos = 0;
    while ((pos = major.indexOf(thousandsSeparator)) > 0) {
        // e.g. 12,,345,,678,,922 Acceptable positions (from the end) are 5, 10, 15... i.e. (3+thlen)*N
        int fromEnd = major.length() - pos;
        if (fromEnd % (3 + thlen) != 0 // Needs to be a multiple, otherwise it's an error
                || pos - lastpos > 3 // More than 3 digits between two separators -> error
                || pos == 0          // Can't start with a separator
                || (lastpos > 0 && pos - lastpos != 3)) { // Must have exactly 3 digits between two separators
            if (ok) *ok = false;
            return QVariant();
        }

        lastpos = pos;
        major.remove(pos, thlen);
    }
    if (lastpos > 0 && major.length() - lastpos != 3) { // Must have exactly 3 digits after the last separator
        if (ok) *ok = false;
        return QVariant();
    }

    // log10(2^63) ~= 18
    if (isInt && major.length() > 19) isInt = false;

    QString tot;
    if (neg) tot = '-';
    tot += major;
    if (!isInt) tot += '.' + minor + exponentialPart;

    return isInt ? QVariant(tot.toLongLong(ok)) : QVariant(tot.toDouble(ok));
}

QGenericArgument makeGenericArgument(const QVariant& _val, const QByteArray& _paramName, void** _argStorage) {
    bool Result = true;
    *_argStorage = new quint32;

    quint32 ConvertedVal;
    if (_val.userType() == QMetaType::QString) {
        QString StrVal = _val.toString();
        ConvertedVal = static_cast<quint32>(readNumber(StrVal, &Result).toUInt(&Result));
    } else
        ConvertedVal = static_cast<quint32>(_val.toUInt(&Result));

    *(reinterpret_cast<quint32*>(*_argStorage)) = ConvertedVal;

    if (!Result)
        throw exHTTPBadRequest("Invalid value specified for parameter:: " + _paramName);

    return QGenericArgument("quint32", *_argStorage);
}

QGenericArgument makeGenericArgument_NULLABLE(const QVariant& _val, const QByteArray& _paramName, void** _argStorage) {
    bool Result = true;
    *_argStorage = new NULLABLE_TYPE(quint32);

    if (_val.isValid() && _val.isNull() == false)
        *(reinterpret_cast<NULLABLE_TYPE(quint32)*>(*_argStorage)) = static_cast<quint32>(_val.toUInt(&Result));

    if (!Result)
        throw exHTTPBadRequest("Invalid value specified for parameter:: " + _paramName);

    return QGenericArgument("quint32", *_argStorage);
}

void testMakeGenericArgument() {
    QVariant _val0 = "2.05762e+09";
    QByteArray _paramName0 = "hello 1";

//    QVector<void*> ArgStorage(1, {});
//    QGenericArgument aaa = makeGenericArgument(_val0, _paramName0, &ArgStorage[0]);

    void* _argStorage0;
    QGenericArgument aaa0 = makeGenericArgument(_val0, _paramName0, &_argStorage0);

    QVariant _val1 = {};
    QByteArray _paramName1 = "hello 2";
    void* _argStorage1;
    QGenericArgument aaa1 = makeGenericArgument_NULLABLE(_val1, _paramName1, &_argStorage1);
}
*/

int main(int _argc, char *_argv[]) {
//    testMakeGenericArgument(); return 0;
    qDebug() << "--------------------------------------------------";
    qDebug() << "-- test module: Account --------------------------";
    qDebug() << "--------------------------------------------------";

    //---------------------
    QString DBPrefix;
    int progArgsCount = 0;
    char **progArgs = findDBPrefixFromArguments(_argc, _argv, DBPrefix, progArgsCount);

    //---------------------
    QCoreApplication App(progArgsCount, progArgs);
    App.setAttribute(Qt::AA_Use96Dpi, true);

    clsDAC::addDBEngine(enuDBEngines::MySQL);
    clsDAC::setConnectionString(QString("HOST=%1;PORT=%2;USER=%3;PASSWORD=%4;SCHEMA=%5%6")
                                .arg(TARGOMAN_M2STR(UNITTEST_DB_HOST))
                                .arg(TARGOMAN_M2STR(UNITTEST_DB_PORT))
                                .arg(TARGOMAN_M2STR(UNITTEST_DB_USER))
                                .arg(TARGOMAN_M2STR(UNITTEST_DB_PASSWORD))
                                .arg(DBPrefix)
                                .arg(TARGOMAN_M2STR(UNITTEST_DB_SCHEMA))
                                );

    bool BreakOnFirstFail = true;
    int FailedTests = 0;

    try {
        FailedTests += QTest::qExec(new testBase(DBPrefix), progArgsCount, progArgs);
        if (BreakOnFirstFail && !FailedTests) FailedTests += QTest::qExec(new testAccount(DBPrefix), progArgsCount, progArgs);
        if (BreakOnFirstFail && !FailedTests) FailedTests += QTest::qExec(new testAccountFixture(DBPrefix), progArgsCount, progArgs);
        if (BreakOnFirstFail && !FailedTests) FailedTests += QTest::qExec(new testJWT(DBPrefix), progArgsCount, progArgs);

//        if (BreakOnFirstFail && !FailedTests) FailedTests += QTest::qExec(new testActionLogs(DBPrefix), progArgsCount, progArgs);
//        if (BreakOnFirstFail && !FailedTests) FailedTests += QTest::qExec(new testActiveSessions(DBPrefix), progArgsCount, progArgs);
//        if (BreakOnFirstFail && !FailedTests) FailedTests += QTest::qExec(new testRoles(DBPrefix), progArgsCount, progArgs);
//        if (BreakOnFirstFail && !FailedTests) FailedTests += QTest::qExec(new testService(DBPrefix), progArgsCount, progArgs);
//        if (BreakOnFirstFail && !FailedTests) FailedTests += QTest::qExec(new testAPITokens(DBPrefix), progArgsCount, progArgs);
    } catch (exTargomanBase &e) {
        ++FailedTests;
        qDebug() << "*** EXCEPTION ***" << QString("error(%1):%2").arg(e.code()).arg(e.what());
    } catch (std::exception &e) {
        ++FailedTests;
        qDebug() << "*** EXCEPTION ***" << e.what();
    }

    if (FailedTests > 0)
        qDebug() << "total number of failed tests: " << FailedTests;
    else
        qDebug() << "all tests passed :)";

    clsDAC::shutdown();

    return FailedTests;
}
