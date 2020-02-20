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

#ifndef TAPI_APIARGHELPERMACROS_HPP
#define TAPI_APIARGHELPERMACROS_HPP
#include <QString>

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

#define TAPI_HELEPER_VARIANTSTRUCT_CONS_INPUT1(t, n, d, i, to, fr)       const t& _a1=d
#define TAPI_HELEPER_VARIANTSTRUCT_CONS_INPUT2(t, n, d, i, to, fr, ...)  const t& _a2=d,  TAPI_HELEPER_VARIANTSTRUCT_CONS_INPUT1(__VA_ARGS__)
#define TAPI_HELEPER_VARIANTSTRUCT_CONS_INPUT3(t, n, d, i, to, fr, ...)  const t& _a3=d,  TAPI_HELEPER_VARIANTSTRUCT_CONS_INPUT2(__VA_ARGS__)
#define TAPI_HELEPER_VARIANTSTRUCT_CONS_INPUT4(t, n, d, i, to, fr, ...)  const t& _a4=d,  TAPI_HELEPER_VARIANTSTRUCT_CONS_INPUT3(__VA_ARGS__)
#define TAPI_HELEPER_VARIANTSTRUCT_CONS_INPUT5(t, n, d, i, to, fr, ...)  const t& _a5=d,  TAPI_HELEPER_VARIANTSTRUCT_CONS_INPUT4(__VA_ARGS__)
#define TAPI_HELEPER_VARIANTSTRUCT_CONS_INPUT6(t, n, d, i, to, fr, ...)  const t& _a6=d,  TAPI_HELEPER_VARIANTSTRUCT_CONS_INPUT5(__VA_ARGS__)
#define TAPI_HELEPER_VARIANTSTRUCT_CONS_INPUT8(t, n, d, i, to, fr, ...)  const t& _a8=d,  TAPI_HELEPER_VARIANTSTRUCT_CONS_INPUT7(__VA_ARGS__)
#define TAPI_HELEPER_VARIANTSTRUCT_CONS_INPUT9(t, n, d, i, to, fr, ...)  const t& _a9=d,  TAPI_HELEPER_VARIANTSTRUCT_CONS_INPUT8(__VA_ARGS__)
#define TAPI_HELEPER_VARIANTSTRUCT_CONS_INPUT10(t, n, d, i, to, fr, ...) const t& _a10=d, TAPI_HELEPER_VARIANTSTRUCT_CONS_INPUT9(__VA_ARGS__)
#define TAPI_HELEPER_VARIANTSTRUCT_CONS_INPUT11(t, n, d, i, to, fr, ...) const t& _a11=d, TAPI_HELEPER_VARIANTSTRUCT_CONS_INPUT10(__VA_ARGS__)
#define TAPI_HELEPER_VARIANTSTRUCT_CONS_INPUT12(t, n, d, i, to, fr, ...) const t& _a12=d, TAPI_HELEPER_VARIANTSTRUCT_CONS_INPUT11(__VA_ARGS__)
#define TAPI_HELEPER_VARIANTSTRUCT_CONS_INPUT13(t, n, d, i, to, fr, ...) const t& _a13=d, TAPI_HELEPER_VARIANTSTRUCT_CONS_INPUT12(__VA_ARGS__)
#define TAPI_HELEPER_VARIANTSTRUCT_CONS_INPUT14(t, n, d, i, to, fr, ...) const t& _a14=d, TAPI_HELEPER_VARIANTSTRUCT_CONS_INPUT13(__VA_ARGS__)
#define TAPI_HELEPER_VARIANTSTRUCT_CONS_INPUT15(t, n, d, i, to, fr, ...) const t& _a15=d, TAPI_HELEPER_VARIANTSTRUCT_CONS_INPUT14(__VA_ARGS__)
#define TAPI_HELEPER_VARIANTSTRUCT_CONS_INPUT16(t, n, d, i, to, fr, ...) const t& _a16=d, TAPI_HELEPER_VARIANTSTRUCT_CONS_INPUT15(__VA_ARGS__)
#define TAPI_HELEPER_VARIANTSTRUCT_CONS_INPUT17(t, n, d, i, to, fr, ...) const t& _a17=d, TAPI_HELEPER_VARIANTSTRUCT_CONS_INPUT16(__VA_ARGS__)
#define TAPI_HELEPER_VARIANTSTRUCT_CONS_INPUT18(t, n, d, i, to, fr, ...) const t& _a18=d, TAPI_HELEPER_VARIANTSTRUCT_CONS_INPUT17(__VA_ARGS__)
#define TAPI_HELEPER_VARIANTSTRUCT_CONS_INPUT19(t, n, d, i, to, fr, ...) const t& _a19=d, TAPI_HELEPER_VARIANTSTRUCT_CONS_INPUT18(__VA_ARGS__)
#define TAPI_HELEPER_VARIANTSTRUCT_CONS_INPUT20(t, n, d, i, to, fr, ...) const t& _a19=d, TAPI_HELEPER_VARIANTSTRUCT_CONS_INPUT19(__VA_ARGS__)

#define TAPI_HELEPER_VARIANTSTRUCT_CONS_INIT1(t, n,d,i,to,fr)      n(_a1)
#define TAPI_HELEPER_VARIANTSTRUCT_CONS_INIT2(t, n,d,i,to,fr, ...) n(_a2),  TAPI_HELEPER_VARIANTSTRUCT_CONS_INIT1(__VA_ARGS__)
#define TAPI_HELEPER_VARIANTSTRUCT_CONS_INIT3(t, n,d,i,to,fr, ...) n(_a3),  TAPI_HELEPER_VARIANTSTRUCT_CONS_INIT2(__VA_ARGS__)
#define TAPI_HELEPER_VARIANTSTRUCT_CONS_INIT4(t, n,d,i,to,fr, ...) n(_a4),  TAPI_HELEPER_VARIANTSTRUCT_CONS_INIT3(__VA_ARGS__)
#define TAPI_HELEPER_VARIANTSTRUCT_CONS_INIT5(t, n,d,i,to,fr, ...) n(_a5),  TAPI_HELEPER_VARIANTSTRUCT_CONS_INIT4(__VA_ARGS__)
#define TAPI_HELEPER_VARIANTSTRUCT_CONS_INIT6(t, n,d,i,to,fr, ...) n(_a6),  TAPI_HELEPER_VARIANTSTRUCT_CONS_INIT5(__VA_ARGS__)
#define TAPI_HELEPER_VARIANTSTRUCT_CONS_INIT8(t, n,d,i,to,fr, ...) n(_a8),  TAPI_HELEPER_VARIANTSTRUCT_CONS_INIT7(__VA_ARGS__)
#define TAPI_HELEPER_VARIANTSTRUCT_CONS_INIT9(t, n,d,i,to,fr, ...) n(_a9),  TAPI_HELEPER_VARIANTSTRUCT_CONS_INIT8(__VA_ARGS__)
#define TAPI_HELEPER_VARIANTSTRUCT_CONS_INIT10(t,n,d,i,to,fr, ...) n(_a10), TAPI_HELEPER_VARIANTSTRUCT_CONS_INIT9(__VA_ARGS__)
#define TAPI_HELEPER_VARIANTSTRUCT_CONS_INIT11(t,n,d,i,to,fr, ...) n(_a11), TAPI_HELEPER_VARIANTSTRUCT_CONS_INIT10(__VA_ARGS__)
#define TAPI_HELEPER_VARIANTSTRUCT_CONS_INIT12(t,n,d,i,to,fr, ...) n(_a12), TAPI_HELEPER_VARIANTSTRUCT_CONS_INIT11(__VA_ARGS__)
#define TAPI_HELEPER_VARIANTSTRUCT_CONS_INIT13(t,n,d,i,to,fr, ...) n(_a13), TAPI_HELEPER_VARIANTSTRUCT_CONS_INIT12(__VA_ARGS__)
#define TAPI_HELEPER_VARIANTSTRUCT_CONS_INIT14(t,n,d,i,to,fr, ...) n(_a14), TAPI_HELEPER_VARIANTSTRUCT_CONS_INIT13(__VA_ARGS__)
#define TAPI_HELEPER_VARIANTSTRUCT_CONS_INIT15(t,n,d,i,to,fr, ...) n(_a15), TAPI_HELEPER_VARIANTSTRUCT_CONS_INIT14(__VA_ARGS__)
#define TAPI_HELEPER_VARIANTSTRUCT_CONS_INIT16(t,n,d,i,to,fr, ...) n(_a16), TAPI_HELEPER_VARIANTSTRUCT_CONS_INIT15(__VA_ARGS__)
#define TAPI_HELEPER_VARIANTSTRUCT_CONS_INIT17(t,n,d,i,to,fr, ...) n(_a17), TAPI_HELEPER_VARIANTSTRUCT_CONS_INIT16(__VA_ARGS__)
#define TAPI_HELEPER_VARIANTSTRUCT_CONS_INIT18(t,n,d,i,to,fr, ...) n(_a18), TAPI_HELEPER_VARIANTSTRUCT_CONS_INIT17(__VA_ARGS__)
#define TAPI_HELEPER_VARIANTSTRUCT_CONS_INIT19(t,n,d,i,to,fr, ...) n(_a19), TAPI_HELEPER_VARIANTSTRUCT_CONS_INIT18(__VA_ARGS__)
#define TAPI_HELEPER_VARIANTSTRUCT_CONS_INIT20(t,n,d,i,to,fr, ...) n(_a19), TAPI_HELEPER_VARIANTSTRUCT_CONS_INIT19(__VA_ARGS__)

extern QString toCammel(const QString& n);
#define TAPI_HELEPER_DEFINE_VARIANT_STRUCT_TOJSON_METHOD(n, i, to) \
    if([](auto v)->bool{return i;}(n)) Obj[toCammel(#n)]=[](auto v)->QJsonValue{return to;}(n);
#define TAPI_HELEPER_VARIANTSTRUCT_TOJSON1(t, n,d,i,to,fr) TAPI_HELEPER_DEFINE_VARIANT_STRUCT_TOJSON_METHOD(n, i, to);
#define TAPI_HELEPER_VARIANTSTRUCT_TOJSON2(t, n,d,i,to,fr,...) TAPI_HELEPER_DEFINE_VARIANT_STRUCT_TOJSON_METHOD(n, i, to); TAPI_HELEPER_VARIANTSTRUCT_TOJSON1(__VA_ARGS__)
#define TAPI_HELEPER_VARIANTSTRUCT_TOJSON3(t, n,d,i,to,fr,...) TAPI_HELEPER_DEFINE_VARIANT_STRUCT_TOJSON_METHOD(n, i, to); TAPI_HELEPER_VARIANTSTRUCT_TOJSON2(__VA_ARGS__)
#define TAPI_HELEPER_VARIANTSTRUCT_TOJSON4(t, n,d,i,to,fr,...) TAPI_HELEPER_DEFINE_VARIANT_STRUCT_TOJSON_METHOD(n, i, to); TAPI_HELEPER_VARIANTSTRUCT_TOJSON3(__VA_ARGS__)
#define TAPI_HELEPER_VARIANTSTRUCT_TOJSON5(t, n,d,i,to,fr,...) TAPI_HELEPER_DEFINE_VARIANT_STRUCT_TOJSON_METHOD(n, i, to); TAPI_HELEPER_VARIANTSTRUCT_TOJSON4(__VA_ARGS__)
#define TAPI_HELEPER_VARIANTSTRUCT_TOJSON6(t, n,d,i,to,fr,...) TAPI_HELEPER_DEFINE_VARIANT_STRUCT_TOJSON_METHOD(n, i, to); TAPI_HELEPER_VARIANTSTRUCT_TOJSON5(__VA_ARGS__)
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
#define TAPI_HELEPER_VARIANTSTRUCT_FROMJSON1(t, n,d,i,to,fr) TAPI_HELEPER_DEFINE_VARIANT_STRUCT_FROMJSON_METHOD(t, n, d, fr);
#define TAPI_HELEPER_VARIANTSTRUCT_FROMJSON2(t, n,d,i,to,fr,...) TAPI_HELEPER_DEFINE_VARIANT_STRUCT_FROMJSON_METHOD(t, n, d, fr); TAPI_HELEPER_VARIANTSTRUCT_FROMJSON1(__VA_ARGS__)
#define TAPI_HELEPER_VARIANTSTRUCT_FROMJSON3(t, n,d,i,to,fr,...) TAPI_HELEPER_DEFINE_VARIANT_STRUCT_FROMJSON_METHOD(t, n, d, fr); TAPI_HELEPER_VARIANTSTRUCT_FROMJSON2(__VA_ARGS__)
#define TAPI_HELEPER_VARIANTSTRUCT_FROMJSON4(t, n,d,i,to,fr,...) TAPI_HELEPER_DEFINE_VARIANT_STRUCT_FROMJSON_METHOD(t, n, d, fr); TAPI_HELEPER_VARIANTSTRUCT_FROMJSON3(__VA_ARGS__)
#define TAPI_HELEPER_VARIANTSTRUCT_FROMJSON5(t, n,d,i,to,fr,...) TAPI_HELEPER_DEFINE_VARIANT_STRUCT_FROMJSON_METHOD(t, n, d, fr); TAPI_HELEPER_VARIANTSTRUCT_FROMJSON4(__VA_ARGS__)
#define TAPI_HELEPER_VARIANTSTRUCT_FROMJSON6(t, n,d,i,to,fr,...) TAPI_HELEPER_DEFINE_VARIANT_STRUCT_FROMJSON_METHOD(t, n, d, fr); TAPI_HELEPER_VARIANTSTRUCT_FROMJSON5(__VA_ARGS__)
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

/**********************************************************/
#define TAPI_HELEPER_QSP_M2STR_POSTFIX ">"
#define TAPI_HELEPER_QSP_M2STR_PREFIX "QSharedPointer<"

#define TAPI_REGISTER_METATYPE_FULL(_complexity, _namespace, _type, _lambdaToVariant, _lambdaFromVariant, _lambdaDesc, _lambdaToORM, _lambdaFromORM, _lambdaOptions) \
namespace Targoman {namespace API { \
    template<> std::function<QVariant(_namespace::_type _value)> tmplAPIArg<_namespace::_type, _complexity, false>::toVariantLambda = _lambdaToVariant; \
    template<> std::function<_namespace::_type(QVariant _value, const QByteArray& _paramName)> tmplAPIArg<_namespace::_type, _complexity, false>::fromVariantLambda = _lambdaFromVariant; \
    template<> std::function<QString(const QList<ORM::clsORMField>& _allFields)> tmplAPIArg<_namespace::_type, _complexity, false>::descriptionLambda = _lambdaDesc; \
    template<> std::function<QVariant(QString _value)> tmplAPIArg<_namespace::_type, _complexity, false>::toORMValueLambda = _lambdaToORM; \
    template<> std::function<QVariant(const QVariant& _val)> tmplAPIArg<_namespace::_type, _complexity, false>::fromORMValueLambda = _lambdaFromORM; \
    template<> std::function<QStringList()> tmplAPIArg<_namespace::_type, _complexity, false>::optionsLambda = _lambdaOptions; \
    static tmplAPIArg<_namespace::_type, _complexity, false>* Dangling_##_namespace##_type = tmplAPIArg<_namespace::_type, _complexity, false>::instance(TARGOMAN_M2STR(_namespace::_type)); \
    \
    template<> std::function<QVariant(QSharedPointer<_namespace::_type> _value)> tmplAPIArg<QSharedPointer<_namespace::_type>, _complexity, true>::toVariantLambda = \
        [](QSharedPointer<_namespace::_type> _value){return _value.isNull() ? QVariant() : tmplAPIArg<_namespace::_type, _complexity, false>::toVariant(*_value);}; \
    template<> std::function<QSharedPointer<_namespace::_type>(QVariant _value, const QByteArray& _paramName)> tmplAPIArg<QSharedPointer<_namespace::_type>, _complexity, true>::fromVariantLambda = \
        [](const QVariant& _value, const QByteArray& _paramName) -> QSharedPointer<_namespace::_type> { \
            if(!_value.isValid() || _value.isNull()) return QSharedPointer<_namespace::_type>(); \
            QSharedPointer<_namespace::_type> Value(new _namespace::_type); *Value = tmplAPIArg<_namespace::_type, _complexity, false>::fromVariant(_value, _paramName); \
            return Value; \
        }; \
    template<> std::function<QVariant(QString _value)> tmplAPIArg<QSharedPointer<_namespace::_type>, _complexity, true>::toORMValueLambda = _lambdaToORM; \
    template<> std::function<QVariant(const QVariant& _val)> tmplAPIArg<QSharedPointer<_namespace::_type>, _complexity, true>::fromORMValueLambda = _lambdaFromORM; \
    template<> std::function<QStringList()> tmplAPIArg<QSharedPointer<_namespace::_type>, _complexity, true>::optionsLambda = _lambdaOptions; \
    template<> std::function<QString(const QList<ORM::clsORMField>& _allFields)> tmplAPIArg<QSharedPointer<_namespace::_type>, _complexity, true>::descriptionLambda = _lambdaDesc; \
    static tmplAPIArg<QSharedPointer<_namespace::_type>, _complexity, true>* Dangling_QSP_##_namespace##_type = tmplAPIArg<QSharedPointer<_namespace::_type>, _complexity, true>::instance(QSP_M2STR(_namespace::_type)); \
}}

#define TAPI_REGISTER_METATYPE_FromORM(_complexity, _namespace, _type, _lambdaToVariant, _lambdaFromVariant, _lambdaDesc, _lambdaToORM, _lambdaFromORM) \
    TAPI_REGISTER_METATYPE_FULL(_complexity, _namespace, _type, _lambdaToVariant, _lambdaFromVariant, _lambdaDesc, _lambdaToORM, _lambdaFromORM, nullptr)

#define TAPI_REGISTER_METATYPE_ToORM(_complexity, _namespace, _type, _lambdaToVariant, _lambdaFromVariant, _lambdaDesc, _lambdaToORM) \
    TAPI_REGISTER_METATYPE_FromORM(_complexity, _namespace, _type, _lambdaToVariant, _lambdaFromVariant, _lambdaDesc, _lambdaToORM, nullptr)

#define TAPI_REGISTER_METATYPE_Desc(_complexity, _namespace, _type, _lambdaToVariant, _lambdaFromVariant, _lambdaDesc) \
    TAPI_REGISTER_METATYPE_ToORM(_complexity, _namespace, _type, _lambdaToVariant, _lambdaFromVariant, _lambdaDesc, nullptr)

#define TAPI_REGISTER_METATYPE_FromVariant(_complexity, _namespace, _type, _lambdaToVariant, _lambdaFromVariant) \
    TAPI_REGISTER_METATYPE_Desc(_complexity, _namespace, _type, _lambdaToVariant, _lambdaFromVariant, nullptr)

#define TAPI_REGISTER_METATYPE_ToVariant(_complexity, _namespace, _type, _lambdaToVariant) \
    TAPI_REGISTER_METATYPE_FromVariant(_complexity, _namespace, _type, _lambdaToVariant, nullptr)

#define TAPI_REGISTER_METATYPE_MACRO_ARG_COUNT(_01,_02,_03,_04,_05,_06, N ,...) N

#define TAPI_REGISTER_METATYPE_MACRO_SELECTOR(_fn,...) TAPI_REGISTER_METATYPE_MACRO_ARG_COUNT(__VA_ARGS__,\
    _fn##FULL,_fn##FromORM,_fn##ToORM,_fn##Desc,_fn##FromVariant,_fn##ToVariant, None)

#define TAPI_REGISTER_TARGOMAN_ENUM_IMPL(_namespace, _enum, _lambdaToVariant, _lambdaFromVariant, _lambdaDesc, _lambdaToORM, _lambdaFromORM, _lambdaOptions) \
namespace Targoman {namespace API { \
    template<> std::function<QVariant(_namespace::_enum::Type _value)> tmplAPIArg<_namespace::_enum::Type, COMPLEXITY_Enum, false>::toVariantLambda = _lambdaToVariant; \
    template<> std::function<_namespace::_enum::Type(QVariant _value, const QByteArray& _paramName)> tmplAPIArg<_namespace::_enum::Type, COMPLEXITY_Enum, false>::fromVariantLambda = _lambdaFromVariant; \
    template<> std::function<QVariant(QString _value)> tmplAPIArg<_namespace::_enum::Type, COMPLEXITY_Enum, false>::toORMValueLambda = _lambdaToORM; \
    template<> std::function<QVariant(const QVariant& _val)> tmplAPIArg<_namespace::_enum::Type, COMPLEXITY_Enum, false>::fromORMValueLambda = _lambdaFromORM; \
    template<> std::function<QStringList()> tmplAPIArg<_namespace::_enum::Type, COMPLEXITY_Enum, false>::optionsLambda = _lambdaOptions; \
    template<> std::function<QString(const QList<ORM::clsORMField>& _allFields)> tmplAPIArg<_namespace::_enum::Type, COMPLEXITY_Enum, false>::descriptionLambda = _lambdaDesc; \
    static tmplAPIArg<_namespace::_enum::Type, COMPLEXITY_Enum, false>* Dangling_##_namespace##_enum = tmplAPIArg<_namespace::_enum::Type, COMPLEXITY_Enum, false>::instance(TARGOMAN_M2STR(_namespace::_enum::Type)); \
    \
    template<> std::function<QVariant(QSharedPointer<_namespace::_enum::Type> _value)> tmplAPIArg<QSharedPointer<_namespace::_enum::Type>, COMPLEXITY_Enum, true>::toVariantLambda = \
        [](QSharedPointer<_namespace::_enum::Type> _value){return _value.isNull() ? QVariant() : tmplAPIArg<_namespace::_enum::Type, COMPLEXITY_Enum, false>::toVariant(*_value);}; \
    template<> std::function<QSharedPointer<_namespace::_enum::Type>(QVariant _value, const QByteArray& _paramName)> tmplAPIArg<QSharedPointer<_namespace::_enum::Type>, COMPLEXITY_Enum, true>::fromVariantLambda = \
        [](const QVariant& _value, const QByteArray& _paramName) -> QSharedPointer<_namespace::_enum::Type> { \
            if(!_value.isValid() || _value.isNull()) return QSharedPointer<_namespace::_enum::Type>(); \
            QSharedPointer<_namespace::_enum::Type> Value(new _namespace::_enum::Type); *Value = tmplAPIArg<_namespace::_enum::Type, COMPLEXITY_Enum, false>::fromVariant(_value, _paramName); \
            return Value; \
        }; \
    template<> std::function<QVariant(QString _value)> tmplAPIArg<QSharedPointer<_namespace::_enum::Type>, COMPLEXITY_Enum, true>::toORMValueLambda = _lambdaToORM; \
    template<> std::function<QVariant(const QVariant& _val)> tmplAPIArg<QSharedPointer<_namespace::_enum::Type>, COMPLEXITY_Enum, true>::fromORMValueLambda = _lambdaFromORM; \
    template<> std::function<QStringList()> tmplAPIArg<QSharedPointer<_namespace::_enum::Type>, COMPLEXITY_Enum, true>::optionsLambda = _lambdaOptions; \
    template<> std::function<QString(const QList<ORM::clsORMField>& _allFields)> tmplAPIArg<QSharedPointer<_namespace::_enum::Type>, COMPLEXITY_Enum, true>::descriptionLambda = _lambdaDesc; \
    static tmplAPIArg<QSharedPointer<_namespace::_enum::Type>, COMPLEXITY_Enum, true>* Dangling_QSP_##_namespace##_enum = tmplAPIArg<QSharedPointer<_namespace::_enum::Type>, COMPLEXITY_Enum, true>::instance(QSP_M2STR(_namespace::_enum::Type)); \
}}

/************************************************************/
/************************************************************/
#define TAPI_ADD_SIMPLE_TYPE(_type, _name) \
    class _name:public _type{public:_name(){;}_name(const _type& _other):_type(_other){;}}\

/************************************************************/
#define TAPI_ADD_COMPLEX_TYPE(_type, _name) \
    class _name:public _type{ \
        public:_name(){;}_name(const _type& _other):_type(_other){;} \
        _name fromVariant (const QVariant& _value){_type Value = _type::fromVariant (_value); return *reinterpret_cast<_name*>(&Value);} \
    }
/************************************************************/
#define TAPI_DECLARE_METATYPE(_type) \
    Q_DECLARE_METATYPE(_type) \
    Q_DECLARE_METATYPE(QSharedPointer<_type>)
/************************************************************/
#define TAPI_REGISTER_METATYPE(_complexity, _namespace, _type, ...) \
    TAPI_REGISTER_METATYPE_MACRO_SELECTOR(TAPI_REGISTER_METATYPE_, __VA_ARGS__)(_complexity, _namespace, _type, __VA_ARGS__)
/************************************************************/
#define TAPI_VALIDATION_REQUIRED_TYPE_IMPL(_complexity, _namespace, _type, _validationRule, _toVariant, ...) \
    TAPI_REGISTER_METATYPE( \
        _complexity, _namespace, _type, \
        [](const _namespace::_type& _value) -> QVariant {return _toVariant;}, \
        [](const QVariant& _value, const QByteArray& _paramName) -> _namespace::_type { \
            static QFieldValidator Validator = QFieldValidator()._validationRule; \
            if(Validator.isValid(_value, _paramName) == false) throw exHTTPBadRequest(Validator.errorMessage()); \
            _namespace::_type Value; Value=_value.toString();  return  Value; \
        }, __VA_ARGS__ \
    )
/************************************************************/
#define TAPI_REGISTER_TARGOMAN_ENUM(_namespace, _enum) \
    TAPI_REGISTER_TARGOMAN_ENUM_IMPL( \
        _namespace, _enum, \
        [](_namespace::_enum::Type _value) -> QVariant{return _namespace::_enum::toStr(_value);}, \
        [](const QVariant& _value, const QByteArray& _paramName) -> _namespace::_enum::Type { \
            if(_namespace::_enum::options().contains(_value.toString())) return _namespace::_enum::toEnum(_value.toString()); \
            else try { return _namespace::_enum::toEnum(_value.toString(), true); } catch(...) { \
              throw exHTTPBadRequest(QString("%1(%2) is not a valid %3").arg( \
                      _paramName.size() ? _paramName.constData() : _value.toString()).arg(_value.toString()).arg( \
                      QString(TARGOMAN_M2STR(_enum)).startsWith("enu") ? QString(TARGOMAN_M2STR(_enum)).mid(3) : QString(TARGOMAN_M2STR(_enum)))); \
        }}, \
        [](const QList<ORM::clsORMField>&) -> QString {return QString("One of (%1)").arg(_namespace::_enum::options().join('|'));}, \
        [](const QString& _value) -> QVariant { \
          if(_namespace::_enum::options().contains(_value) == false) \
              throw exHTTPBadRequest(QString("%1(%2) is not a valid %3.").arg( \
                      _value).arg(_value).arg( \
                      QString(TARGOMAN_M2STR(_enum)).startsWith("enu") ? QString(TARGOMAN_M2STR(_enum)).mid(3) : QString(TARGOMAN_M2STR(_enum)))); \
          return QString(_namespace::_enum::toEnum(_value)); \
        },\
        [](const QVariant& _value) -> QVariant { \
            return _namespace::_enum::toStr(static_cast<_namespace::_enum::Type>(_value.toString().toLatin1().at(0))); \
        }, \
        []() -> QStringList { return _namespace::_enum::options();} \
    )

/************************************************************/
#define TAPI_DEFINE_VARIANT_ENABLED_STRUCT(_name, ...) struct _name{ \
      TAPI_HELEPER_DEFINE_VARIANT_STRUCT_PARAMS(__VA_ARGS__) \
      _name(TAPI_HELEPER_DEFINE_VARIANT_STRUCT_CONS_INPUT(__VA_ARGS__)) : TAPI_HELEPER_DEFINE_VARIANT_STRUCT_CONS_INIT(__VA_ARGS__) {} \
      QJsonObject toJson() const { QJsonObject Obj; TAPI_HELEPER_DEFINE_VARIANT_STRUCT_TOJSON(__VA_ARGS__); return Obj;} \
      _name& fromJson(const QJsonObject& _obj){ TAPI_HELEPER_DEFINE_VARIANT_STRUCT_FROMJSON(__VA_ARGS__) return *this;} \
};

/************************************************************/
#define NULLABLE(_type) QSharedPointer<_type>
#define NULLABLE_VALUE(_value) _value.isNull() ? QVariant() : *_value
#define QSP_M2STR(_type) TAPI_HELEPER_QSP_M2STR_PREFIX #_type TAPI_HELEPER_QSP_M2STR_POSTFIX

/*
#include <QString>
#include <QJsonObject>
#include <QDateTime>

struct stuDiscount{
    qint64 ID;
    QString Name;
    qint64 Amount;
    qint64 MaxAmount;
    stuDiscount(const qint64& _a4=0, const QString& _a3=QString(), const qint64& _a2=0, const qint64& _a1=0) : ID(_a4), Name(_a3), Amount(_a2), MaxAmount(_a1) {}

    QJsonObject toJson() {
        QJsonObject Obj;
        if([](auto v)->bool{return v > -1;}(ID)) Obj[toCammel("ID")]=[](auto v)->QJsonValue{return v;}(ID);;
        if([](auto v)->bool{return v.size();}(Name)) Obj[toCammel("Name")]=[](auto v)->QJsonValue{return v;}(Name);;
        if([](auto v)->bool{return true;}(Amount)) Obj[toCammel("Amount")]=[](auto v)->QJsonValue{return v;}(Amount);;
        if([](auto v)->bool{return true;}(MaxAmount)) Obj[toCammel("MaxAmount")]=[](auto v)->QJsonValue{return v;}(MaxAmount);;;
        return Obj;}
    stuDiscount& fromJson(const QJsonObject& _obj){ ID=_obj.contains(toCammel("ID"))?[](auto v)->qint64{return static_cast<qint64>(v.toDouble());}(_obj.value(toCammel("ID"))):0; Name=_obj.contains(toCammel("Name"))?[](auto v)->QString{return v.toString();}(_obj.value(toCammel("Name"))):QString(); Amount=_obj.contains(toCammel("Amount"))?[](auto v)->qint64{return static_cast<qint64>(v.toDouble());}(_obj.value(toCammel("Amount"))):0; MaxAmount=_obj.contains(toCammel("MaxAmount"))?[](auto v)->qint64{return static_cast<qint64>(v.toDouble());}(_obj.value(toCammel("MaxAmount"))):0; return *this;} };
*/


#endif // TAPI_APIARGHELPERMACROS_HPP
