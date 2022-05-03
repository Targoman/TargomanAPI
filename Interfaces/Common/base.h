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
 * @author Kambiz Zandi <kambizzandi@gmail.com>
 */

#ifndef TAPI_BASE_H
#define TAPI_BASE_H

#include <stdio.h>
#include <stdlib.h>
#include <execinfo.h>
#include <cxxabi.h>
//#include <QString>
//#include <QTextStream>
//#include "libTargomanCommon/CmdIO.h"
//#include "libTargomanCommon/Logger.h"
//using namespace Targoman::Common;

#define TAPI_DISABLE_COPY(Class) \
    Q_DISABLE_COPY(Class)
//    Class &operator=(const Class) Q_DECL_EQ_DELETE;

void print_stacktrace(FILE *out = stderr, unsigned int max_frames = 63);

/*
//inline QTextStream &hex(QTextStream &stream) {
//    stream.setIntegerBase(16);
//    return stream;
//}
//inline QTextStream &reset(QTextStream &stream) {
//    stream.reset();
//    return stream;
//}

class TLog {
public:
    enum struct enuType {
        Debug,
        Info,
        Warning,
        Happy,
        Error
    };

    inline TLog(unsigned char _level, enuType _type) :
        Level(_level),
        Type(_type),
        Log(true),
        AddSpace(true),
        Quote(true)
    { ; }

    virtual inline ~TLog() {
        switch (this->Type) {
            case enuType::Debug:
                if (this->Log) {
                    TargomanLogDebug(this->Level, this->Buffer);
                } else {
                    TargomanDebug(this->Level, this->Buffer);
                }
                break;

            case enuType::Info:
                if (this->Log) {
                    TargomanLogInfo(this->Level, this->Buffer);
                } else {
                    TargomanInfo(this->Level, this->Buffer);
                }
                break;

            case enuType::Warning:
                if (this->Log) {
                    TargomanLogWarn(this->Level, this->Buffer);
                } else {
                    TargomanWarn(this->Level, this->Buffer);
                }
                break;

            case enuType::Happy:
                if (this->Log) {
                    TargomanLogHappy(this->Level, this->Buffer);
                } else {
                    TargomanHappy(this->Level, this->Buffer);
                }
                break;

            case enuType::Error:
                if (this->Log) {
                    TargomanLogError(this->Buffer);
                } else {
                    TargomanError(this->Buffer);
                }
                break;
        }
    }

public:
    inline TLog& nolog() {
        this->Log = false;
        return *this;
    }
    inline TLog& nospace() {
        this->AddSpace = false;
        return *this;
    }
    inline TLog& maybeSpace() {
        if (this->AddSpace)
            QTextStream(&Buffer) << " ";
        return *this;
    }
    inline TLog& noquote() {
        this->Quote = false;
        return *this;
    }
    inline TLog& maybeQuote(char c = '"') {
        if (this->Quote)
            QTextStream(&Buffer) << c;
        return *this;
    }

    inline TLog& operator<<(QChar t) { putUcs4(t.unicode()); return maybeSpace(); }
    inline TLog& operator<<(bool t) { QTextStream(&Buffer) << (t ? "true" : "false"); return maybeSpace(); }
    inline TLog& operator<<(char t) { QTextStream(&Buffer) << t; return maybeSpace(); }
    inline TLog& operator<<(signed short t) { QTextStream(&Buffer) << t; return maybeSpace(); }
    inline TLog& operator<<(unsigned short t) { QTextStream(&Buffer) << t; return maybeSpace(); }
#ifdef Q_COMPILER_UNICODE_STRINGS
    inline TLog& operator<<(char16_t t) { return *this << QChar(ushort(t)); }
    inline TLog& operator<<(char32_t t) { putUcs4(t); return maybeSpace(); }
#endif
    inline TLog& operator<<(signed int t) { QTextStream(&Buffer) << t; return maybeSpace(); }
    inline TLog& operator<<(unsigned int t) { QTextStream(&Buffer) << t; return maybeSpace(); }
    inline TLog& operator<<(signed long t) { QTextStream(&Buffer) << t; return maybeSpace(); }
    inline TLog& operator<<(unsigned long t) { QTextStream(&Buffer) << t; return maybeSpace(); }
    inline TLog& operator<<(qint64 t) { QTextStream(&Buffer) << t; return maybeSpace(); }
    inline TLog& operator<<(quint64 t) { QTextStream(&Buffer) << t; return maybeSpace(); }
    inline TLog& operator<<(float t) { QTextStream(&Buffer) << t; return maybeSpace(); }
    inline TLog& operator<<(double t) { QTextStream(&Buffer) << t; return maybeSpace(); }
    inline TLog& operator<<(const char* t) { QTextStream(&Buffer) << QString::fromUtf8(t); return maybeSpace(); }
#if QT_STRINGVIEW_LEVEL < 2
    inline TLog& operator<<(const QString & t) { maybeQuote(); QTextStream(&Buffer) << t; maybeQuote(); return maybeSpace(); }
    inline TLog& operator<<(const QStringRef & t) { maybeQuote(); QTextStream(&Buffer) << t; maybeQuote(); return maybeSpace(); }
#endif
    inline TLog& operator<<(QStringView s) { maybeQuote(); QTextStream(&Buffer) << s; maybeQuote(); return maybeSpace(); }
    inline TLog& operator<<(QLatin1String t) { maybeQuote(); QTextStream(&Buffer) << t; maybeQuote(); return maybeSpace(); }
    inline TLog& operator<<(const QByteArray & t) { maybeQuote(); QTextStream(&Buffer) << t; maybeQuote(); return maybeSpace(); }
    inline TLog& operator<<(const void * t) { QTextStream(&Buffer) << t; return maybeSpace(); }
#ifdef Q_COMPILER_NULLPTR
    inline TLog& operator<<(std::nullptr_t) { QTextStream(&Buffer) << "(nullptr)"; return maybeSpace(); }
#endif

    inline TLog& operator<<(QTextStreamFunction f) {
        QTextStream ts(&Buffer);
        ts << f;
        return *this;
    }

    inline TLog& operator<<(QTextStreamManipulator m) {
        QTextStream ts(&Buffer);
        ts << m;
        return *this;
    }

protected:
    inline void putUcs4(uint ucs4) {
        maybeQuote('\'');
        if (ucs4 < 0x20) {
            QTextStream(&Buffer) << "\\x" << hex << ucs4 << reset;
        } else if (ucs4 < 0x80) {
            QTextStream(&Buffer) << char(ucs4);
        } else {
            if (ucs4 < 0x10000)
                QTextStream(&Buffer) << "\\u" << qSetFieldWidth(4);
            else
                QTextStream(&Buffer) << "\\U" << qSetFieldWidth(8);
            QTextStream(&Buffer) << "" << hex << qSetPadChar(QLatin1Char('0')) << ucs4 << reset;
        }
        maybeQuote('\'');
    }

protected:
    unsigned char Level;
    enuType Type;
    bool Log;
    bool AddSpace;
    bool Quote;
    QString Buffer;
};

inline TLog tDebug(unsigned char _level = 0) {
    return TLog(_level, TLog::enuType::Debug);
}
inline TLog tInfo(unsigned char _level = 0) {
    return TLog(_level, TLog::enuType::Info);
}
inline TLog tWarning(unsigned char _level = 0) {
    return TLog(_level, TLog::enuType::Warning);
}
inline TLog tHappy(unsigned char _level = 0) {
    return TLog(_level, TLog::enuType::Happy);
}
inline TLog tError(unsigned char _level = 0) {
    return TLog(_level, TLog::enuType::Error);
}
*/

#endif // TAPI_BASE_H
