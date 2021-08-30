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
 * @author Kambiz Zandi <kambizzandi@gmail.com>
 */

#ifndef APIARGHELPERMACROSPRIVATE_H
#define APIARGHELPERMACROSPRIVATE_H

#include <QString>
#include "tmplNullable.hpp"

#define TAPI_HELEPER_DEFINE_VARIANT_STRUCT_MACRO_ARG_COUNT( \
    _01,_02,_03,_04,_05,_06,\
    _11,_12,_13,_14,_15,_16,\
    _21,_22,_23,_24,_25,_26,\
    _31,_32,_33,_34,_35,_36,\
    _41,_42,_43,_44,_45,_46,\
    _51,_52,_53,_54,_55,_56,\
    _61,_62,_63,_64,_65,_66,\
    _71,_72,_73,_74,_75,_76,\
    _81,_82,_83,_84,_85,_86,\
    _91,_92,_93,_94,_95,_96,\
    _A1,_A2,_A3,_A4,_A5,_A6,\
    _B1,_B2,_B3,_B4,_B5,_B6,\
    _C1,_C2,_C3,_C4,_C5,_C6,\
    _D1,_D2,_D3,_D4,_D5,_D6,\
    _E1,_E2,_E3,_E4,_E5,_E6,\
    _F1,_F2,_F3,_F4,_F5,_F6,\
    _G1,_G2,_G3,_G4,_G5,_G6,\
    _H1,_H2,_H3,_H4,_H5,_H6,\
    _I1,_I2,_I3,_I4,_I5,_I6,\
    _J1,_J2,_J3,_J4,_J5,_J6,\
    _K1,_K2,_K3,_K4,_K5, N,...) N


#define TAPI_HELEPER_DEFINE_VARIANT_STRUCT_MACRO_SELECTOR(_fn,...) TAPI_HELEPER_DEFINE_VARIANT_STRUCT_MACRO_ARG_COUNT(__VA_ARGS__,\
    _fn##FAIL01,_fn##FAIL02,_fn##FAIL03,_fn##FAIL04,_fn##FAIL05,_fn##20,  \
    _fn##FAIL11,_fn##FAIL12,_fn##FAIL13,_fn##FAIL14,_fn##FAIL15,_fn##19,  \
    _fn##FAIL21,_fn##FAIL22,_fn##FAIL23,_fn##FAIL24,_fn##FAIL25,_fn##18,  \
    _fn##FAIL31,_fn##FAIL32,_fn##FAIL33,_fn##FAIL34,_fn##FAIL35,_fn##17,  \
    _fn##FAIL41,_fn##FAIL42,_fn##FAIL43,_fn##FAIL44,_fn##FAIL45,_fn##16,  \
    _fn##FAIL51,_fn##FAIL52,_fn##FAIL53,_fn##FAIL54,_fn##FAIL55,_fn##15,  \
    _fn##FAIL61,_fn##FAIL62,_fn##FAIL63,_fn##FAIL64,_fn##FAIL65,_fn##14,  \
    _fn##FAIL71,_fn##FAIL72,_fn##FAIL73,_fn##FAIL74,_fn##FAIL75,_fn##13,  \
    _fn##FAIL81,_fn##FAIL82,_fn##FAIL83,_fn##FAIL84,_fn##FAIL85,_fn##12,  \
    _fn##FAIL91,_fn##FAIL92,_fn##FAIL93,_fn##FAIL94,_fn##FAIL95,_fn##11,  \
    _fn##FAILA1,_fn##FAILA2,_fn##FAILA3,_fn##FAILA4,_fn##FAILA5,_fn##10,  \
    _fn##FAILB1,_fn##FAILB2,_fn##FAILB3,_fn##FAILB4,_fn##FAILB5,_fn##9,   \
    _fn##FAILC1,_fn##FAILC2,_fn##FAILC3,_fn##FAILC4,_fn##FAILC5,_fn##8,   \
    _fn##FAILD1,_fn##FAILD2,_fn##FAILD3,_fn##FAILD4,_fn##FAILD5,_fn##7,   \
    _fn##FAILE1,_fn##FAILE2,_fn##FAILE3,_fn##FAILE4,_fn##FAILE5,_fn##6,   \
    _fn##FAILF1,_fn##FAILF2,_fn##FAILF3,_fn##FAILF4,_fn##FAILF5,_fn##5,   \
    _fn##FAILG1,_fn##FAILG2,_fn##FAILG3,_fn##FAILG4,_fn##FAILG5,_fn##4,   \
    _fn##FAILH1,_fn##FAILH2,_fn##FAILH3,_fn##FAILH4,_fn##FAILH5,_fn##3,   \
    _fn##FAILI1,_fn##FAILI2,_fn##FAILI3,_fn##FAILI4,_fn##FAILI5,_fn##2,   \
    _fn##FAILJ1,_fn##FAILJ2,_fn##FAILJ3,_fn##FAILJ4,_fn##FAILJ5,_fn##1,   \
    _fn##FAILK1,_fn##FAILK2,_fn##FAILK3,_fn##FAILK4,_fn##FAILK5,None)

#define TAPI_HELEPER_VARIANTSTRUCT_PARAMS1(t, n, d, i, to, fr)       t n;
#define TAPI_HELEPER_VARIANTSTRUCT_PARAMS2(t, n, d, i, to, fr, ...)  t n; TAPI_HELEPER_VARIANTSTRUCT_PARAMS1(__VA_ARGS__)
#define TAPI_HELEPER_VARIANTSTRUCT_PARAMS3(t, n, d, i, to, fr, ...)  t n; TAPI_HELEPER_VARIANTSTRUCT_PARAMS2(__VA_ARGS__)
#define TAPI_HELEPER_VARIANTSTRUCT_PARAMS4(t, n, d, i, to, fr, ...)  t n; TAPI_HELEPER_VARIANTSTRUCT_PARAMS3(__VA_ARGS__)
#define TAPI_HELEPER_VARIANTSTRUCT_PARAMS5(t, n, d, i, to, fr, ...)  t n; TAPI_HELEPER_VARIANTSTRUCT_PARAMS4(__VA_ARGS__)
#define TAPI_HELEPER_VARIANTSTRUCT_PARAMS6(t, n, d, i, to, fr, ...)  t n; TAPI_HELEPER_VARIANTSTRUCT_PARAMS5(__VA_ARGS__)
#define TAPI_HELEPER_VARIANTSTRUCT_PARAMS7(t, n, d, i, to, fr, ...)  t n; TAPI_HELEPER_VARIANTSTRUCT_PARAMS6(__VA_ARGS__)
#define TAPI_HELEPER_VARIANTSTRUCT_PARAMS8(t, n, d, i, to, fr, ...)  t n; TAPI_HELEPER_VARIANTSTRUCT_PARAMS7(__VA_ARGS__)
#define TAPI_HELEPER_VARIANTSTRUCT_PARAMS9(t, n, d, i, to, fr, ...)  t n; TAPI_HELEPER_VARIANTSTRUCT_PARAMS8(__VA_ARGS__)
#define TAPI_HELEPER_VARIANTSTRUCT_PARAMS10(t, n, d, i, to, fr, ...) t n; TAPI_HELEPER_VARIANTSTRUCT_PARAMS9(__VA_ARGS__)
#define TAPI_HELEPER_VARIANTSTRUCT_PARAMS11(t, n, d, i, to, fr, ...) t n; TAPI_HELEPER_VARIANTSTRUCT_PARAMS10(__VA_ARGS__)
#define TAPI_HELEPER_VARIANTSTRUCT_PARAMS12(t, n, d, i, to, fr, ...) t n; TAPI_HELEPER_VARIANTSTRUCT_PARAMS11(__VA_ARGS__)
#define TAPI_HELEPER_VARIANTSTRUCT_PARAMS13(t, n, d, i, to, fr, ...) t n; TAPI_HELEPER_VARIANTSTRUCT_PARAMS12(__VA_ARGS__)
#define TAPI_HELEPER_VARIANTSTRUCT_PARAMS14(t, n, d, i, to, fr, ...) t n; TAPI_HELEPER_VARIANTSTRUCT_PARAMS13(__VA_ARGS__)
#define TAPI_HELEPER_VARIANTSTRUCT_PARAMS15(t, n, d, i, to, fr, ...) t n; TAPI_HELEPER_VARIANTSTRUCT_PARAMS14(__VA_ARGS__)
#define TAPI_HELEPER_VARIANTSTRUCT_PARAMS16(t, n, d, i, to, fr, ...) t n; TAPI_HELEPER_VARIANTSTRUCT_PARAMS15(__VA_ARGS__)
#define TAPI_HELEPER_VARIANTSTRUCT_PARAMS17(t, n, d, i, to, fr, ...) t n; TAPI_HELEPER_VARIANTSTRUCT_PARAMS16(__VA_ARGS__)
#define TAPI_HELEPER_VARIANTSTRUCT_PARAMS18(t, n, d, i, to, fr, ...) t n; TAPI_HELEPER_VARIANTSTRUCT_PARAMS17(__VA_ARGS__)
#define TAPI_HELEPER_VARIANTSTRUCT_PARAMS19(t, n, d, i, to, fr, ...) t n; TAPI_HELEPER_VARIANTSTRUCT_PARAMS18(__VA_ARGS__)
#define TAPI_HELEPER_VARIANTSTRUCT_PARAMS20(t, n, d, i, to, fr, ...) t n; TAPI_HELEPER_VARIANTSTRUCT_PARAMS19(__VA_ARGS__)

#define TAPI_HELEPER_VARIANTSTRUCT_CONS_INPUT1(t,  n, d, i, to, fr)      const t& _##n=d
#define TAPI_HELEPER_VARIANTSTRUCT_CONS_INPUT2(t,  n, d, i, to, fr, ...) const t& _##n=d, TAPI_HELEPER_VARIANTSTRUCT_CONS_INPUT1(__VA_ARGS__)
#define TAPI_HELEPER_VARIANTSTRUCT_CONS_INPUT3(t,  n, d, i, to, fr, ...) const t& _##n=d, TAPI_HELEPER_VARIANTSTRUCT_CONS_INPUT2(__VA_ARGS__)
#define TAPI_HELEPER_VARIANTSTRUCT_CONS_INPUT4(t,  n, d, i, to, fr, ...) const t& _##n=d, TAPI_HELEPER_VARIANTSTRUCT_CONS_INPUT3(__VA_ARGS__)
#define TAPI_HELEPER_VARIANTSTRUCT_CONS_INPUT5(t,  n, d, i, to, fr, ...) const t& _##n=d, TAPI_HELEPER_VARIANTSTRUCT_CONS_INPUT4(__VA_ARGS__)
#define TAPI_HELEPER_VARIANTSTRUCT_CONS_INPUT6(t,  n, d, i, to, fr, ...) const t& _##n=d, TAPI_HELEPER_VARIANTSTRUCT_CONS_INPUT5(__VA_ARGS__)
#define TAPI_HELEPER_VARIANTSTRUCT_CONS_INPUT7(t,  n, d, i, to, fr, ...) const t& _##n=d, TAPI_HELEPER_VARIANTSTRUCT_CONS_INPUT6(__VA_ARGS__)
#define TAPI_HELEPER_VARIANTSTRUCT_CONS_INPUT8(t,  n, d, i, to, fr, ...) const t& _##n=d, TAPI_HELEPER_VARIANTSTRUCT_CONS_INPUT7(__VA_ARGS__)
#define TAPI_HELEPER_VARIANTSTRUCT_CONS_INPUT9(t,  n, d, i, to, fr, ...) const t& _##n=d, TAPI_HELEPER_VARIANTSTRUCT_CONS_INPUT8(__VA_ARGS__)
#define TAPI_HELEPER_VARIANTSTRUCT_CONS_INPUT10(t, n, d, i, to, fr, ...) const t& _##n=d, TAPI_HELEPER_VARIANTSTRUCT_CONS_INPUT9(__VA_ARGS__)
#define TAPI_HELEPER_VARIANTSTRUCT_CONS_INPUT11(t, n, d, i, to, fr, ...) const t& _##n=d, TAPI_HELEPER_VARIANTSTRUCT_CONS_INPUT10(__VA_ARGS__)
#define TAPI_HELEPER_VARIANTSTRUCT_CONS_INPUT12(t, n, d, i, to, fr, ...) const t& _##n=d, TAPI_HELEPER_VARIANTSTRUCT_CONS_INPUT11(__VA_ARGS__)
#define TAPI_HELEPER_VARIANTSTRUCT_CONS_INPUT13(t, n, d, i, to, fr, ...) const t& _##n=d, TAPI_HELEPER_VARIANTSTRUCT_CONS_INPUT12(__VA_ARGS__)
#define TAPI_HELEPER_VARIANTSTRUCT_CONS_INPUT14(t, n, d, i, to, fr, ...) const t& _##n=d, TAPI_HELEPER_VARIANTSTRUCT_CONS_INPUT13(__VA_ARGS__)
#define TAPI_HELEPER_VARIANTSTRUCT_CONS_INPUT15(t, n, d, i, to, fr, ...) const t& _##n=d, TAPI_HELEPER_VARIANTSTRUCT_CONS_INPUT14(__VA_ARGS__)
#define TAPI_HELEPER_VARIANTSTRUCT_CONS_INPUT16(t, n, d, i, to, fr, ...) const t& _##n=d, TAPI_HELEPER_VARIANTSTRUCT_CONS_INPUT15(__VA_ARGS__)
#define TAPI_HELEPER_VARIANTSTRUCT_CONS_INPUT17(t, n, d, i, to, fr, ...) const t& _##n=d, TAPI_HELEPER_VARIANTSTRUCT_CONS_INPUT16(__VA_ARGS__)
#define TAPI_HELEPER_VARIANTSTRUCT_CONS_INPUT18(t, n, d, i, to, fr, ...) const t& _##n=d, TAPI_HELEPER_VARIANTSTRUCT_CONS_INPUT17(__VA_ARGS__)
#define TAPI_HELEPER_VARIANTSTRUCT_CONS_INPUT19(t, n, d, i, to, fr, ...) const t& _##n=d, TAPI_HELEPER_VARIANTSTRUCT_CONS_INPUT18(__VA_ARGS__)
#define TAPI_HELEPER_VARIANTSTRUCT_CONS_INPUT20(t, n, d, i, to, fr, ...) const t& _##n=d, TAPI_HELEPER_VARIANTSTRUCT_CONS_INPUT19(__VA_ARGS__)

#define TAPI_HELEPER_VARIANTSTRUCT_CONS_INIT1(t, n,d,i,to,fr)      n(_##n)
#define TAPI_HELEPER_VARIANTSTRUCT_CONS_INIT2(t, n,d,i,to,fr, ...) n(_##n), TAPI_HELEPER_VARIANTSTRUCT_CONS_INIT1(__VA_ARGS__)
#define TAPI_HELEPER_VARIANTSTRUCT_CONS_INIT3(t, n,d,i,to,fr, ...) n(_##n), TAPI_HELEPER_VARIANTSTRUCT_CONS_INIT2(__VA_ARGS__)
#define TAPI_HELEPER_VARIANTSTRUCT_CONS_INIT4(t, n,d,i,to,fr, ...) n(_##n), TAPI_HELEPER_VARIANTSTRUCT_CONS_INIT3(__VA_ARGS__)
#define TAPI_HELEPER_VARIANTSTRUCT_CONS_INIT5(t, n,d,i,to,fr, ...) n(_##n), TAPI_HELEPER_VARIANTSTRUCT_CONS_INIT4(__VA_ARGS__)
#define TAPI_HELEPER_VARIANTSTRUCT_CONS_INIT6(t, n,d,i,to,fr, ...) n(_##n), TAPI_HELEPER_VARIANTSTRUCT_CONS_INIT5(__VA_ARGS__)
#define TAPI_HELEPER_VARIANTSTRUCT_CONS_INIT7(t, n,d,i,to,fr, ...) n(_##n), TAPI_HELEPER_VARIANTSTRUCT_CONS_INIT6(__VA_ARGS__)
#define TAPI_HELEPER_VARIANTSTRUCT_CONS_INIT8(t, n,d,i,to,fr, ...) n(_##n), TAPI_HELEPER_VARIANTSTRUCT_CONS_INIT7(__VA_ARGS__)
#define TAPI_HELEPER_VARIANTSTRUCT_CONS_INIT9(t, n,d,i,to,fr, ...) n(_##n), TAPI_HELEPER_VARIANTSTRUCT_CONS_INIT8(__VA_ARGS__)
#define TAPI_HELEPER_VARIANTSTRUCT_CONS_INIT10(t,n,d,i,to,fr, ...) n(_##n), TAPI_HELEPER_VARIANTSTRUCT_CONS_INIT9(__VA_ARGS__)
#define TAPI_HELEPER_VARIANTSTRUCT_CONS_INIT11(t,n,d,i,to,fr, ...) n(_##n), TAPI_HELEPER_VARIANTSTRUCT_CONS_INIT10(__VA_ARGS__)
#define TAPI_HELEPER_VARIANTSTRUCT_CONS_INIT12(t,n,d,i,to,fr, ...) n(_##n), TAPI_HELEPER_VARIANTSTRUCT_CONS_INIT11(__VA_ARGS__)
#define TAPI_HELEPER_VARIANTSTRUCT_CONS_INIT13(t,n,d,i,to,fr, ...) n(_##n), TAPI_HELEPER_VARIANTSTRUCT_CONS_INIT12(__VA_ARGS__)
#define TAPI_HELEPER_VARIANTSTRUCT_CONS_INIT14(t,n,d,i,to,fr, ...) n(_##n), TAPI_HELEPER_VARIANTSTRUCT_CONS_INIT13(__VA_ARGS__)
#define TAPI_HELEPER_VARIANTSTRUCT_CONS_INIT15(t,n,d,i,to,fr, ...) n(_##n), TAPI_HELEPER_VARIANTSTRUCT_CONS_INIT14(__VA_ARGS__)
#define TAPI_HELEPER_VARIANTSTRUCT_CONS_INIT16(t,n,d,i,to,fr, ...) n(_##n), TAPI_HELEPER_VARIANTSTRUCT_CONS_INIT15(__VA_ARGS__)
#define TAPI_HELEPER_VARIANTSTRUCT_CONS_INIT17(t,n,d,i,to,fr, ...) n(_##n), TAPI_HELEPER_VARIANTSTRUCT_CONS_INIT16(__VA_ARGS__)
#define TAPI_HELEPER_VARIANTSTRUCT_CONS_INIT18(t,n,d,i,to,fr, ...) n(_##n), TAPI_HELEPER_VARIANTSTRUCT_CONS_INIT17(__VA_ARGS__)
#define TAPI_HELEPER_VARIANTSTRUCT_CONS_INIT19(t,n,d,i,to,fr, ...) n(_##n), TAPI_HELEPER_VARIANTSTRUCT_CONS_INIT18(__VA_ARGS__)
#define TAPI_HELEPER_VARIANTSTRUCT_CONS_INIT20(t,n,d,i,to,fr, ...) n(_##n), TAPI_HELEPER_VARIANTSTRUCT_CONS_INIT19(__VA_ARGS__)

//extern QString toCammel(const QString& _name);
inline QString toCammel(const QString& _name)
{
    return _name.mid(0,1).toLower() + _name.mid(1);
}

#define TAPI_HELEPER_DEFINE_VARIANT_STRUCT_TOJSON_METHOD(n, i, to) \
    if ([](auto v)->bool{return i;}(n)) Obj[toCammel(#n)]=[](auto v)->QJsonValue{return to;}(n);
#define TAPI_HELEPER_VARIANTSTRUCT_TOJSON1(t, n,d,i,to,fr)     TAPI_HELEPER_DEFINE_VARIANT_STRUCT_TOJSON_METHOD(n, i, to);
#define TAPI_HELEPER_VARIANTSTRUCT_TOJSON2(t, n,d,i,to,fr,...) TAPI_HELEPER_DEFINE_VARIANT_STRUCT_TOJSON_METHOD(n, i, to); TAPI_HELEPER_VARIANTSTRUCT_TOJSON1(__VA_ARGS__)
#define TAPI_HELEPER_VARIANTSTRUCT_TOJSON3(t, n,d,i,to,fr,...) TAPI_HELEPER_DEFINE_VARIANT_STRUCT_TOJSON_METHOD(n, i, to); TAPI_HELEPER_VARIANTSTRUCT_TOJSON2(__VA_ARGS__)
#define TAPI_HELEPER_VARIANTSTRUCT_TOJSON4(t, n,d,i,to,fr,...) TAPI_HELEPER_DEFINE_VARIANT_STRUCT_TOJSON_METHOD(n, i, to); TAPI_HELEPER_VARIANTSTRUCT_TOJSON3(__VA_ARGS__)
#define TAPI_HELEPER_VARIANTSTRUCT_TOJSON5(t, n,d,i,to,fr,...) TAPI_HELEPER_DEFINE_VARIANT_STRUCT_TOJSON_METHOD(n, i, to); TAPI_HELEPER_VARIANTSTRUCT_TOJSON4(__VA_ARGS__)
#define TAPI_HELEPER_VARIANTSTRUCT_TOJSON6(t, n,d,i,to,fr,...) TAPI_HELEPER_DEFINE_VARIANT_STRUCT_TOJSON_METHOD(n, i, to); TAPI_HELEPER_VARIANTSTRUCT_TOJSON5(__VA_ARGS__)
#define TAPI_HELEPER_VARIANTSTRUCT_TOJSON7(t, n,d,i,to,fr,...) TAPI_HELEPER_DEFINE_VARIANT_STRUCT_TOJSON_METHOD(n, i, to); TAPI_HELEPER_VARIANTSTRUCT_TOJSON6(__VA_ARGS__)
#define TAPI_HELEPER_VARIANTSTRUCT_TOJSON8(t, n,d,i,to,fr,...) TAPI_HELEPER_DEFINE_VARIANT_STRUCT_TOJSON_METHOD(n, i, to); TAPI_HELEPER_VARIANTSTRUCT_TOJSON7(__VA_ARGS__)
#define TAPI_HELEPER_VARIANTSTRUCT_TOJSON9(t, n,d,i,to,fr,...) TAPI_HELEPER_DEFINE_VARIANT_STRUCT_TOJSON_METHOD(n, i, to); TAPI_HELEPER_VARIANTSTRUCT_TOJSON8(__VA_ARGS__)
#define TAPI_HELEPER_VARIANTSTRUCT_TOJSON10(t,n,d,i,to,fr,...) TAPI_HELEPER_DEFINE_VARIANT_STRUCT_TOJSON_METHOD(n, i, to); TAPI_HELEPER_VARIANTSTRUCT_TOJSON9(__VA_ARGS__)
#define TAPI_HELEPER_VARIANTSTRUCT_TOJSON11(t,n,d,i,to,fr,...) TAPI_HELEPER_DEFINE_VARIANT_STRUCT_TOJSON_METHOD(n, i, to); TAPI_HELEPER_VARIANTSTRUCT_TOJSON10(__VA_ARGS__)
#define TAPI_HELEPER_VARIANTSTRUCT_TOJSON12(t,n,d,i,to,fr,...) TAPI_HELEPER_DEFINE_VARIANT_STRUCT_TOJSON_METHOD(n, i, to); TAPI_HELEPER_VARIANTSTRUCT_TOJSON11(__VA_ARGS__)
#define TAPI_HELEPER_VARIANTSTRUCT_TOJSON13(t,n,d,i,to,fr,...) TAPI_HELEPER_DEFINE_VARIANT_STRUCT_TOJSON_METHOD(n, i, to); TAPI_HELEPER_VARIANTSTRUCT_TOJSON12(__VA_ARGS__)
#define TAPI_HELEPER_VARIANTSTRUCT_TOJSON14(t,n,d,i,to,fr,...) TAPI_HELEPER_DEFINE_VARIANT_STRUCT_TOJSON_METHOD(n, i, to); TAPI_HELEPER_VARIANTSTRUCT_TOJSON13(__VA_ARGS__)
#define TAPI_HELEPER_VARIANTSTRUCT_TOJSON15(t,n,d,i,to,fr,...) TAPI_HELEPER_DEFINE_VARIANT_STRUCT_TOJSON_METHOD(n, i, to); TAPI_HELEPER_VARIANTSTRUCT_TOJSON14(__VA_ARGS__)
#define TAPI_HELEPER_VARIANTSTRUCT_TOJSON16(t,n,d,i,to,fr,...) TAPI_HELEPER_DEFINE_VARIANT_STRUCT_TOJSON_METHOD(n, i, to); TAPI_HELEPER_VARIANTSTRUCT_TOJSON15(__VA_ARGS__)
#define TAPI_HELEPER_VARIANTSTRUCT_TOJSON17(t,n,d,i,to,fr,...) TAPI_HELEPER_DEFINE_VARIANT_STRUCT_TOJSON_METHOD(n, i, to); TAPI_HELEPER_VARIANTSTRUCT_TOJSON16(__VA_ARGS__)
#define TAPI_HELEPER_VARIANTSTRUCT_TOJSON18(t,n,d,i,to,fr,...) TAPI_HELEPER_DEFINE_VARIANT_STRUCT_TOJSON_METHOD(n, i, to); TAPI_HELEPER_VARIANTSTRUCT_TOJSON17(__VA_ARGS__)
#define TAPI_HELEPER_VARIANTSTRUCT_TOJSON19(t,n,d,i,to,fr,...) TAPI_HELEPER_DEFINE_VARIANT_STRUCT_TOJSON_METHOD(n, i, to); TAPI_HELEPER_VARIANTSTRUCT_TOJSON18(__VA_ARGS__)
#define TAPI_HELEPER_VARIANTSTRUCT_TOJSON20(t,n,d,i,to,fr,...) TAPI_HELEPER_DEFINE_VARIANT_STRUCT_TOJSON_METHOD(n, i, to); TAPI_HELEPER_VARIANTSTRUCT_TOJSON19(__VA_ARGS__)

#define TAPI_HELEPER_DEFINE_VARIANT_STRUCT_FROMJSON_METHOD(t, n, d, fr) \
    n=_obj.contains(toCammel(#n))?[](auto v)->t{return fr;}(_obj.value(toCammel(#n))):d
#define TAPI_HELEPER_VARIANTSTRUCT_FROMJSON1(t, n,d,i,to,fr)     TAPI_HELEPER_DEFINE_VARIANT_STRUCT_FROMJSON_METHOD(t, n, d, fr);
#define TAPI_HELEPER_VARIANTSTRUCT_FROMJSON2(t, n,d,i,to,fr,...) TAPI_HELEPER_DEFINE_VARIANT_STRUCT_FROMJSON_METHOD(t, n, d, fr); TAPI_HELEPER_VARIANTSTRUCT_FROMJSON1(__VA_ARGS__)
#define TAPI_HELEPER_VARIANTSTRUCT_FROMJSON3(t, n,d,i,to,fr,...) TAPI_HELEPER_DEFINE_VARIANT_STRUCT_FROMJSON_METHOD(t, n, d, fr); TAPI_HELEPER_VARIANTSTRUCT_FROMJSON2(__VA_ARGS__)
#define TAPI_HELEPER_VARIANTSTRUCT_FROMJSON4(t, n,d,i,to,fr,...) TAPI_HELEPER_DEFINE_VARIANT_STRUCT_FROMJSON_METHOD(t, n, d, fr); TAPI_HELEPER_VARIANTSTRUCT_FROMJSON3(__VA_ARGS__)
#define TAPI_HELEPER_VARIANTSTRUCT_FROMJSON5(t, n,d,i,to,fr,...) TAPI_HELEPER_DEFINE_VARIANT_STRUCT_FROMJSON_METHOD(t, n, d, fr); TAPI_HELEPER_VARIANTSTRUCT_FROMJSON4(__VA_ARGS__)
#define TAPI_HELEPER_VARIANTSTRUCT_FROMJSON6(t, n,d,i,to,fr,...) TAPI_HELEPER_DEFINE_VARIANT_STRUCT_FROMJSON_METHOD(t, n, d, fr); TAPI_HELEPER_VARIANTSTRUCT_FROMJSON5(__VA_ARGS__)
#define TAPI_HELEPER_VARIANTSTRUCT_FROMJSON7(t, n,d,i,to,fr,...) TAPI_HELEPER_DEFINE_VARIANT_STRUCT_FROMJSON_METHOD(t, n, d, fr); TAPI_HELEPER_VARIANTSTRUCT_FROMJSON6(__VA_ARGS__)
#define TAPI_HELEPER_VARIANTSTRUCT_FROMJSON8(t, n,d,i,to,fr,...) TAPI_HELEPER_DEFINE_VARIANT_STRUCT_FROMJSON_METHOD(t, n, d, fr); TAPI_HELEPER_VARIANTSTRUCT_FROMJSON7(__VA_ARGS__)
#define TAPI_HELEPER_VARIANTSTRUCT_FROMJSON9(t, n,d,i,to,fr,...) TAPI_HELEPER_DEFINE_VARIANT_STRUCT_FROMJSON_METHOD(t, n, d, fr); TAPI_HELEPER_VARIANTSTRUCT_FROMJSON8(__VA_ARGS__)
#define TAPI_HELEPER_VARIANTSTRUCT_FROMJSON10(t,n,d,i,to,fr,...) TAPI_HELEPER_DEFINE_VARIANT_STRUCT_FROMJSON_METHOD(t, n, d, fr); TAPI_HELEPER_VARIANTSTRUCT_FROMJSON9(__VA_ARGS__)
#define TAPI_HELEPER_VARIANTSTRUCT_FROMJSON11(t,n,d,i,to,fr,...) TAPI_HELEPER_DEFINE_VARIANT_STRUCT_FROMJSON_METHOD(t, n, d, fr); TAPI_HELEPER_VARIANTSTRUCT_FROMJSON10(__VA_ARGS__)
#define TAPI_HELEPER_VARIANTSTRUCT_FROMJSON12(t,n,d,i,to,fr,...) TAPI_HELEPER_DEFINE_VARIANT_STRUCT_FROMJSON_METHOD(t, n, d, fr); TAPI_HELEPER_VARIANTSTRUCT_FROMJSON11(__VA_ARGS__)
#define TAPI_HELEPER_VARIANTSTRUCT_FROMJSON13(t,n,d,i,to,fr,...) TAPI_HELEPER_DEFINE_VARIANT_STRUCT_FROMJSON_METHOD(t, n, d, fr); TAPI_HELEPER_VARIANTSTRUCT_FROMJSON12(__VA_ARGS__)
#define TAPI_HELEPER_VARIANTSTRUCT_FROMJSON14(t,n,d,i,to,fr,...) TAPI_HELEPER_DEFINE_VARIANT_STRUCT_FROMJSON_METHOD(t, n, d, fr); TAPI_HELEPER_VARIANTSTRUCT_FROMJSON13(__VA_ARGS__)
#define TAPI_HELEPER_VARIANTSTRUCT_FROMJSON15(t,n,d,i,to,fr,...) TAPI_HELEPER_DEFINE_VARIANT_STRUCT_FROMJSON_METHOD(t, n, d, fr); TAPI_HELEPER_VARIANTSTRUCT_FROMJSON14(__VA_ARGS__)
#define TAPI_HELEPER_VARIANTSTRUCT_FROMJSON16(t,n,d,i,to,fr,...) TAPI_HELEPER_DEFINE_VARIANT_STRUCT_FROMJSON_METHOD(t, n, d, fr); TAPI_HELEPER_VARIANTSTRUCT_FROMJSON15(__VA_ARGS__)
#define TAPI_HELEPER_VARIANTSTRUCT_FROMJSON17(t,n,d,i,to,fr,...) TAPI_HELEPER_DEFINE_VARIANT_STRUCT_FROMJSON_METHOD(t, n, d, fr); TAPI_HELEPER_VARIANTSTRUCT_FROMJSON16(__VA_ARGS__)
#define TAPI_HELEPER_VARIANTSTRUCT_FROMJSON18(t,n,d,i,to,fr,...) TAPI_HELEPER_DEFINE_VARIANT_STRUCT_FROMJSON_METHOD(t, n, d, fr); TAPI_HELEPER_VARIANTSTRUCT_FROMJSON17(__VA_ARGS__)
#define TAPI_HELEPER_VARIANTSTRUCT_FROMJSON19(t,n,d,i,to,fr,...) TAPI_HELEPER_DEFINE_VARIANT_STRUCT_FROMJSON_METHOD(t, n, d, fr); TAPI_HELEPER_VARIANTSTRUCT_FROMJSON18(__VA_ARGS__)
#define TAPI_HELEPER_VARIANTSTRUCT_FROMJSON20(t,n,d,i,to,fr,...) TAPI_HELEPER_DEFINE_VARIANT_STRUCT_FROMJSON_METHOD(t, n, d, fr); TAPI_HELEPER_VARIANTSTRUCT_FROMJSON19(__VA_ARGS__)

#define TAPI_HELEPER_DEFINE_VARIANT_STRUCT_PARAMS(...) \
    TAPI_HELEPER_DEFINE_VARIANT_STRUCT_MACRO_SELECTOR(TAPI_HELEPER_VARIANTSTRUCT_PARAMS, __VA_ARGS__)(__VA_ARGS__)

#define TAPI_HELEPER_DEFINE_VARIANT_STRUCT_CONS_INPUT(...) \
    TAPI_HELEPER_DEFINE_VARIANT_STRUCT_MACRO_SELECTOR(TAPI_HELEPER_VARIANTSTRUCT_CONS_INPUT, __VA_ARGS__)(__VA_ARGS__)

#define TAPI_HELEPER_DEFINE_VARIANT_STRUCT_CONS_INIT(...) \
    TAPI_HELEPER_DEFINE_VARIANT_STRUCT_MACRO_SELECTOR(TAPI_HELEPER_VARIANTSTRUCT_CONS_INIT, __VA_ARGS__)(__VA_ARGS__)

#define TAPI_HELEPER_DEFINE_VARIANT_STRUCT_TOJSON(...) \
    TAPI_HELEPER_DEFINE_VARIANT_STRUCT_MACRO_SELECTOR(TAPI_HELEPER_VARIANTSTRUCT_TOJSON, __VA_ARGS__)(__VA_ARGS__)

#define TAPI_HELEPER_DEFINE_VARIANT_STRUCT_FROMJSON(...) \
    TAPI_HELEPER_DEFINE_VARIANT_STRUCT_MACRO_SELECTOR( TAPI_HELEPER_VARIANTSTRUCT_FROMJSON, __VA_ARGS__)(__VA_ARGS__)

//#define TARGOMAN_FLATTEN_NAMESPACE(_namespace) _namespace

/**********************************************************/
#define TAPI_REGISTER_METATYPE_FULL(_complexity, _namespace, _type, _toVariantLambda, _fromVariantLambda, _descriptionLambda, _toORMValueLambda, _fromORMValueLambda, _lambdaOptions) \
    namespace Targoman::API { \
        template<> std::function<QVariant(_namespace::_type _value)> tmplAPIArg<_namespace::_type, _complexity, false>::toVariantLambda = _toVariantLambda; \
        template<> std::function<_namespace::_type(QVariant _value, const QByteArray& _paramName)> tmplAPIArg<_namespace::_type, _complexity, false>::fromVariantLambda = _fromVariantLambda; \
        template<> std::function<QString(const QList<ORM::clsORMField>& _allFields)> tmplAPIArg<_namespace::_type, _complexity, false>::descriptionLambda = _descriptionLambda; \
        template<> std::function<QVariant(const QVariant&)> tmplAPIArg<_namespace::_type, _complexity, false>::toORMValueLambda = _toORMValueLambda; \
        template<> std::function<QVariant(const QVariant&)> tmplAPIArg<_namespace::_type, _complexity, false>::fromORMValueLambda = _fromORMValueLambda; \
        template<> std::function<QStringList()> tmplAPIArg<_namespace::_type, _complexity, false>::optionsLambda = _lambdaOptions; \
        template<> std::function<QVariant(NULLABLE_TYPE(_namespace::_type) _value)> tmplAPIArg<NULLABLE_TYPE(_namespace::_type), _complexity, true>::toVariantLambda = \
            [](NULLABLE_TYPE(_namespace::_type) _value){return NULLABLE_IS_NULL(_value) ? QVariant() : tmplAPIArg<_namespace::_type, _complexity, false>::toVariant(*_value);}; \
        template<> std::function<NULLABLE_TYPE(_namespace::_type)(QVariant _value, const QByteArray& _paramName)> tmplAPIArg<NULLABLE_TYPE(_namespace::_type), _complexity, true>::fromVariantLambda = \
            [](const QVariant& _value, const QByteArray& _paramName) -> NULLABLE_TYPE(_namespace::_type) { \
                if (!_value.isValid() || _value.isNull()) \
                    return NULLABLE_TYPE(_namespace::_type)(); \
                NULLABLE_VAR(_namespace::_type, Value); \
                Value = tmplAPIArg<_namespace::_type, _complexity, false>::fromVariant(_value, _paramName); \
                return Value; \
            }; \
        template<> std::function<QVariant(const QVariant&)> tmplAPIArg<NULLABLE_TYPE(_namespace::_type), _complexity, true>::toORMValueLambda = _toORMValueLambda; \
        template<> std::function<QVariant(const QVariant&)> tmplAPIArg<NULLABLE_TYPE(_namespace::_type), _complexity, true>::fromORMValueLambda = _fromORMValueLambda; \
        template<> std::function<QStringList()> tmplAPIArg<NULLABLE_TYPE(_namespace::_type), _complexity, true>::optionsLambda = _lambdaOptions; \
        template<> std::function<QString(const QList<ORM::clsORMField>& _allFields)> tmplAPIArg<NULLABLE_TYPE(_namespace::_type), _complexity, true>::descriptionLambda = _descriptionLambda; \
    } \
    namespace _namespace { \
        using namespace Targoman::API; \
        static tmplAPIArg<_type, _complexity, false>* Dangling_##_type = tmplAPIArg<_type, _complexity, false>::instance(TARGOMAN_M2STR(_namespace::_type)); \
        static tmplAPIArg<NULLABLE_TYPE(_type), _complexity, true>* Dangling_QSP_##_type = tmplAPIArg<NULLABLE_TYPE(_type), _complexity, true>::instance(QSP_M2STR(_namespace::_type)); \
    }
//            NULLABLE_TYPE(_namespace::_type) Value(new _namespace::_type);

#define TAPI_REGISTER_METATYPE_FromORM(_complexity, _namespace, _type, _toVariantLambda, _fromVariantLambda, _descriptionLambda, _toORMValueLambda, _fromORMValueLambda) \
    TAPI_REGISTER_METATYPE_FULL(_complexity, _namespace, _type, _toVariantLambda, _fromVariantLambda, _descriptionLambda, _toORMValueLambda, _fromORMValueLambda, nullptr)

#define TAPI_REGISTER_METATYPE_ToORM(_complexity, _namespace, _type, _toVariantLambda, _fromVariantLambda, _descriptionLambda, _toORMValueLambda) \
    TAPI_REGISTER_METATYPE_FromORM(_complexity, _namespace, _type, _toVariantLambda, _fromVariantLambda, _descriptionLambda, _toORMValueLambda, nullptr)

#define TAPI_REGISTER_METATYPE_Desc(_complexity, _namespace, _type, _toVariantLambda, _fromVariantLambda, _descriptionLambda) \
    TAPI_REGISTER_METATYPE_ToORM(_complexity, _namespace, _type, _toVariantLambda, _fromVariantLambda, _descriptionLambda, nullptr)

#define TAPI_REGISTER_METATYPE_FromVariant(_complexity, _namespace, _type, _toVariantLambda, _fromVariantLambda) \
    TAPI_REGISTER_METATYPE_Desc(_complexity, _namespace, _type, _toVariantLambda, _fromVariantLambda, nullptr)

#define TAPI_REGISTER_METATYPE_ToVariant(_complexity, _namespace, _type, _toVariantLambda) \
    TAPI_REGISTER_METATYPE_FromVariant(_complexity, _namespace, _type, _toVariantLambda, nullptr)

#define TAPI_REGISTER_METATYPE_MACRO_ARG_COUNT(_01,_02,_03,_04,_05,_06, N ,...) N

#define TAPI_REGISTER_METATYPE_MACRO_SELECTOR(_fn,...) TAPI_REGISTER_METATYPE_MACRO_ARG_COUNT(__VA_ARGS__,\
    _fn##FULL,_fn##FromORM,_fn##ToORM,_fn##Desc,_fn##FromVariant,_fn##ToVariant, None)

#define TAPI_REGISTER_TARGOMAN_ENUM_IMPL(_namespace, _enum, _toVariantLambda, _fromVariantLambda, _descriptionLambda, _toORMValueLambda, _fromORMValueLambda, _lambdaOptions) \
    namespace Targoman::API { \
        template<> std::function<QVariant(_namespace::_enum::Type _value)> tmplAPIArg<_namespace::_enum::Type, COMPLEXITY_Enum, false>::toVariantLambda = _toVariantLambda; \
        template<> std::function<_namespace::_enum::Type(QVariant _value, const QByteArray& _paramName)> tmplAPIArg<_namespace::_enum::Type, COMPLEXITY_Enum, false>::fromVariantLambda = _fromVariantLambda; \
        template<> std::function<QVariant(const QVariant&)> tmplAPIArg<_namespace::_enum::Type, COMPLEXITY_Enum, false>::toORMValueLambda = _toORMValueLambda; \
        template<> std::function<QVariant(const QVariant&)> tmplAPIArg<_namespace::_enum::Type, COMPLEXITY_Enum, false>::fromORMValueLambda = _fromORMValueLambda; \
        template<> std::function<QStringList()> tmplAPIArg<_namespace::_enum::Type, COMPLEXITY_Enum, false>::optionsLambda = _lambdaOptions; \
        template<> std::function<QString(const QList<ORM::clsORMField>& _allFields)> tmplAPIArg<_namespace::_enum::Type, COMPLEXITY_Enum, false>::descriptionLambda = _descriptionLambda; \
        template<> std::function<QVariant(NULLABLE_TYPE(_namespace::_enum::Type) _value)> tmplAPIArg<NULLABLE_TYPE(_namespace::_enum::Type), COMPLEXITY_Enum, true>::toVariantLambda = \
            [](NULLABLE_TYPE(_namespace::_enum::Type) _value){return NULLABLE_IS_NULL(_value) ? QVariant() : tmplAPIArg<_namespace::_enum::Type, COMPLEXITY_Enum, false>::toVariant(*_value);}; \
        template<> std::function<NULLABLE_TYPE(_namespace::_enum::Type)(QVariant _value, const QByteArray& _paramName)> tmplAPIArg<NULLABLE_TYPE(_namespace::_enum::Type), COMPLEXITY_Enum, true>::fromVariantLambda = \
            [](const QVariant& _value, const QByteArray& _paramName) -> NULLABLE_TYPE(_namespace::_enum::Type) { \
                if (!_value.isValid() || _value.isNull()) return NULLABLE_TYPE(_namespace::_enum::Type)(); \
                NULLABLE_VAR(_namespace::_enum::Type, Value); \
                Value = tmplAPIArg<_namespace::_enum::Type, COMPLEXITY_Enum, false>::fromVariant(_value, _paramName); \
                return Value; \
            }; \
        template<> std::function<QVariant(const QVariant&)> tmplAPIArg<NULLABLE_TYPE(_namespace::_enum::Type), COMPLEXITY_Enum, true>::toORMValueLambda = _toORMValueLambda; \
        template<> std::function<QVariant(const QVariant&)> tmplAPIArg<NULLABLE_TYPE(_namespace::_enum::Type), COMPLEXITY_Enum, true>::fromORMValueLambda = _fromORMValueLambda; \
        template<> std::function<QStringList()> tmplAPIArg<NULLABLE_TYPE(_namespace::_enum::Type), COMPLEXITY_Enum, true>::optionsLambda = _lambdaOptions; \
        template<> std::function<QString(const QList<ORM::clsORMField>& _allFields)> tmplAPIArg<NULLABLE_TYPE(_namespace::_enum::Type), COMPLEXITY_Enum, true>::descriptionLambda = _descriptionLambda; \
    } \
    namespace _namespace { \
        using namespace Targoman::API; \
        static tmplAPIArg<_enum::Type, COMPLEXITY_Enum, false>* Dangling_##_enum = tmplAPIArg<_enum::Type, COMPLEXITY_Enum, false>::instance(TARGOMAN_M2STR(_namespace::_enum::Type)); \
        static tmplAPIArg<NULLABLE_TYPE(_enum::Type), COMPLEXITY_Enum, true>* Dangling_QSP_##_enum = tmplAPIArg<NULLABLE_TYPE(_enum::Type), COMPLEXITY_Enum, true>::instance(QSP_M2STR(_namespace::_enum::Type)); \
    }
//NULLABLE_TYPE(_namespace::_enum::Type) Value(new _namespace::_enum::Type);

/************************************************************/
#define INTERNAL_TAPI_ADD_TYPE_SPECIALFROMVARIANT(_baseType, _typeName, _fromVariant) \
    class _typeName : public _baseType { \
    public: \
        _typeName() {} \
        _typeName(const _baseType& _other):_baseType(_other) {} \
        void customFromVariant (const QVariant& _value) { _fromVariant; } \
    }; \
    inline void setFromVariant(_typeName& _storage, const QVariant& _val) { \
        _storage.customFromVariant(_val); \
    } \
    inline void setFromVariant(NULLABLE_TYPE(_typeName)& _storage, const QVariant& _val) { \
        if (_val.isValid() && _val.isNull() == false) { \
            _typeName var; \
            var.customFromVariant(_val); \
            NULLABLE_SET(_storage, var); \
        } \
        else \
            NULLABLE_RESET(_storage); \
    }

/************************************************************/
#define INTERNAL_TAPI_REGISTER_TARGOMAN_ENUM(_namespace, _enum) \
    TAPI_REGISTER_TARGOMAN_ENUM_IMPL( \
        /* namespace          */ _namespace, \
        /* enum               */ _enum, \
        /* toVariantLambda    */ [](_namespace::_enum::Type _value) -> QVariant { \
            return _namespace::_enum::toStr(_value); \
        }, \
        /* fromVariantLambda  */ [](const QVariant& _value, const QByteArray& _paramName) -> _namespace::_enum::Type { \
            QVariant _val = (_value.userType() == QMetaType::QString ? _value : _namespace::_enum::toStr(_value.value<_namespace::_enum::Type>())); \
            if (_namespace::_enum::options().contains(_val.toString())) \
                return _namespace::_enum::toEnum(_val.toString()); \
            else \
                try { return _namespace::_enum::toEnum(_val.toString(), true); } \
                catch (...) { \
                    throw exHTTPBadRequest(QString("%1(%2) is not a valid %3") \
                        .arg(_paramName.size() ? _paramName.constData() : _val.toString()) \
                        .arg(_val.toString()) \
                        .arg(QString(TARGOMAN_M2STR(_enum)).startsWith("enu") ? QString(TARGOMAN_M2STR(_enum)).mid(3) : QString(TARGOMAN_M2STR(_enum))) \
                    ); \
                } \
        }, \
        /* descriptionLambda  */ [](const QList<ORM::clsORMField>&) -> QString { \
            return QString("One of (%1)").arg(_namespace::_enum::options().join('|')); \
        }, \
        /* toORMValueLambda   */ [](const QVariant& _value) -> QVariant { \
            return _namespace::_enum::toStr(static_cast<_namespace::_enum::Type>(_value.toString().toLatin1().at(0))); \
        }, \
        /* fromORMValueLambda */ [](const QVariant& _value) -> QVariant { \
            QVariant _val = (_value.userType() == QMetaType::QString ? _value : _namespace::_enum::toStr(_value.value<_namespace::_enum::Type>())); \
            if (_namespace::_enum::options().contains(_val.toString()) == false) \
                throw exHTTPBadRequest(QString("%1 is not a valid %2.") \
                    .arg(_val.toString()) \
                    .arg(/*QString(TARGOMAN_M2STR(_enum)).startsWith("enu") ? QString(TARGOMAN_M2STR(_enum)).mid(3) :*/ QString(TARGOMAN_M2STR(_enum))) \
                ); \
            return QString(_namespace::_enum::toEnum(_val.toString())); \
        }, \
        /* lambdaOptions      */ []() -> QStringList { return _namespace::_enum::options(); } \
    )

/************************************************************/
#define INTERNAL_C2DBL(v) static_cast<double>(v)

#define INTERNAL_V2DBL(v) v.toDouble()
#define INTERNAL_V2double(v) INTERNAL_V2DBL(v)

#define INTERNAL_V2U8(v) static_cast<quint8>(v.toDouble())
#define INTERNAL_V2uint8(v) INTERNAL_V2U8(v)
#define INTERNAL_V2U16(v) static_cast<quint16>(v.toDouble())
#define INTERNAL_V2uint16(v) INTERNAL_V2U16(v)
#define INTERNAL_V2U32(v) static_cast<quint32>(v.toDouble())
#define INTERNAL_V2uint32(v) INTERNAL_V2U32(v)
#define INTERNAL_V2U64(v) static_cast<quint64>(v.toDouble())
#define INTERNAL_V2uint64(v) INTERNAL_V2U64(v)

#define INTERNAL_V2S8(v) static_cast<qint8>(v.toDouble())
#define INTERNAL_V2int8(v) INTERNAL_V2S8(v)
#define INTERNAL_V2S16(v) static_cast<qint16>(v.toDouble())
#define INTERNAL_V2int16(v) INTERNAL_V2S16(v)
#define INTERNAL_V2S32(v) static_cast<qint32>(v.toDouble())
#define INTERNAL_V2int32(v) INTERNAL_V2S32(v)
#define INTERNAL_V2S64(v) static_cast<qint64>(v.toDouble())
#define INTERNAL_V2int64(v) INTERNAL_V2S64(v)

#define INTERNAL_N2J(_value)       [](auto v) { return TAPI::toJsonValue(v); } (_value)
#define INTERNAL_N2DBL(_value)     INTERNAL_NULLABLE_FROM_JSONVALUE_TO_TYPE(double,  _value)
#define INTERNAL_N2int8(_value)    INTERNAL_NULLABLE_FROM_JSONVALUE_TO_TYPE(qint8,   _value)
#define INTERNAL_N2int16(_value)   INTERNAL_NULLABLE_FROM_JSONVALUE_TO_TYPE(qint16,  _value)
#define INTERNAL_N2int32(_value)   INTERNAL_NULLABLE_FROM_JSONVALUE_TO_TYPE(qint32,  _value)
#define INTERNAL_N2int64(_value)   INTERNAL_NULLABLE_FROM_JSONVALUE_TO_TYPE(qint64,  _value)
#define INTERNAL_N2uint8(_value)   INTERNAL_NULLABLE_FROM_JSONVALUE_TO_TYPE(quint8,  _value)
#define INTERNAL_N2uint16(_value)  INTERNAL_NULLABLE_FROM_JSONVALUE_TO_TYPE(quint16, _value)
#define INTERNAL_N2uint32(_value)  INTERNAL_NULLABLE_FROM_JSONVALUE_TO_TYPE(quint32, _value)
#define INTERNAL_N2uint64(_value)  INTERNAL_NULLABLE_FROM_JSONVALUE_TO_TYPE(quint64, _value)

/************************************************************/
/************************************************************/
/************************************************************/
#define INTERNAL_SF_STRUCT_1(_validator) _validator
#define INTERNAL_SF_STRUCT_0()           INTERNAL_SF_STRUCT_1(v)

#define INTERNAL_SF_STRUCT(_type, _name, ...) \
    _type, \
    _name, \
    _type(), \
    TARGOMAN_M2CONCAT(INTERNAL_SF_STRUCT_, TARGOMAN_MACRO_ARG_COUNT(__VA_ARGS__))(__VA_ARGS__), \
    v.toJson(), \
    _type().fromJson(v.toObject())

/************************************************************/
#define INTERNAL_SF_STRING_2(_def, _validator) _def, _validator
#define INTERNAL_SF_STRING_1(_def)             INTERNAL_SF_STRING_2(_def, v.size())
#define INTERNAL_SF_STRING_0()                 INTERNAL_SF_STRING_1(QString())

/************************************************************/
#define INTERNAL_SF_INTEGRAL_2(_def, _validator) _def, _validator
#define INTERNAL_SF_INTEGRAL_1(_def)             INTERNAL_SF_INTEGRAL_2(_def, v==v)
#define INTERNAL_SF_INTEGRAL_0()                 INTERNAL_SF_INTEGRAL_1(0)

#define INTERNAL_SF_NULLABLE_INTEGRAL_2(_def, _validator) _def, _validator
#define INTERNAL_SF_NULLABLE_INTEGRAL_1(_def)             INTERNAL_SF_NULLABLE_INTEGRAL_2(_def, v==v)
#define INTERNAL_SF_NULLABLE_INTEGRAL_0()                 INTERNAL_SF_NULLABLE_INTEGRAL_1(NULLABLE_NULL_VALUE)

/************************************************************/
#define INTERNAL_SF(_type, _name, _typeGroup, _fromVariant, _toVariant, ...) \
    _type, \
    _name, \
    TARGOMAN_M2CONCAT(INTERNAL_SF_ ## _typeGroup ## _, TARGOMAN_MACRO_ARG_COUNT(__VA_ARGS__))(__VA_ARGS__), \
    _fromVariant, \
    _toVariant

/************************************************************/
/************************************************************/
/************************************************************/
#define INTERNAL_NULLABLE_FROM_JSONVALUE_TO_TYPE(_type, _value) \
    [](auto v)->NULLABLE_TYPE(_type) { \
        if (v.isNull()) \
            return NULLABLE_NULL_VALUE; \
        auto Storage = NULLABLE_TYPE(_type)(); \
        Storage = v.toDouble(); \
        return Storage; \
    } (_value)
//return v.toDouble();
//        qDebug() << "INTERNAL_NULLABLE_FROM_JSONVALUE_TO_TYPE" << #_type << v;
//        qDebug() << "INTERNAL_ (1)" << v.toDouble();

/************************************************************/
#define INTERNAL_TAPI_DECLARE_METATYPE_ENUM(_enum) \
    TAPI_DECLARE_METATYPE(_enum::Type); \
namespace TAPI { \
    inline void setFromVariant(_enum::Type& _storage, const QVariant& _val) { \
        _storage = _enum::toEnum(_val.toString()); \
    } \
    inline void setFromVariant(NULLABLE_TYPE(_enum::Type)& _storage, const QVariant& _val) { \
        if (_val.isValid() && _val.isNull() == false) \
            _storage = _enum::toEnum(_val.toString()); \
    } \
}

#endif // APIARGHELPERMACROSPRIVATE_H
