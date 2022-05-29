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
    _301,_302,_303,_304,_305,_30,  \
    _291,_292,_293,_294,_295,_29,  \
    _281,_282,_283,_284,_285,_28,  \
    _271,_272,_273,_274,_275,_27,  \
    _261,_262,_263,_264,_265,_26,  \
    _251,_252,_253,_254,_255,_25,  \
    _241,_242,_243,_244,_245,_24,  \
    _231,_232,_233,_234,_235,_23,  \
    _221,_222,_223,_224,_225,_22,  \
    _211,_212,_213,_214,_215,_21,  \
    _201,_202,_203,_204,_205,_20,  \
    _191,_192,_193,_194,_195,_19,  \
    _181,_182,_183,_184,_185,_18,  \
    _171,_172,_173,_174,_175,_17,  \
    _161,_162,_163,_164,_165,_16,  \
    _151,_152,_153,_154,_155,_15,  \
    _141,_142,_143,_144,_145,_14,  \
    _131,_132,_133,_134,_135,_13,  \
    _121,_122,_123,_124,_125,_12,  \
    _111,_112,_113,_114,_115,_11,  \
    _101,_102,_103,_104,_105,_10,  \
    _091,_092,_093,_094,_095,_09,  \
    _081,_082,_083,_084,_085,_08,  \
    _071,_072,_073,_074,_075,_07,  \
    _061,_062,_063,_064,_065,_06,  \
    _051,_052,_053,_054,_055,_05,  \
    _041,_042,_043,_044,_045,_04,  \
    _031,_032,_033,_034,_035,_03,  \
    _021,_022,_023,_024,_025,_02,  \
    _011,_012,_013,_014,_015,_01,  \
    __N1,__N2,__N3,__N4,__N5,_N, ...) _N

#define TAPI_HELEPER_DEFINE_VARIANT_STRUCT_MACRO_SELECTOR(_fn,...) TAPI_HELEPER_DEFINE_VARIANT_STRUCT_MACRO_ARG_COUNT(__VA_ARGS__,\
    _fn##FAIL301,_fn##FAIL302,_fn##FAIL303,_fn##FAIL304,_fn##FAIL305,_fn##30,  \
    _fn##FAIL291,_fn##FAIL292,_fn##FAIL293,_fn##FAIL294,_fn##FAIL295,_fn##29,  \
    _fn##FAIL281,_fn##FAIL282,_fn##FAIL283,_fn##FAIL284,_fn##FAIL285,_fn##28,  \
    _fn##FAIL271,_fn##FAIL272,_fn##FAIL273,_fn##FAIL274,_fn##FAIL275,_fn##27,  \
    _fn##FAIL261,_fn##FAIL262,_fn##FAIL263,_fn##FAIL264,_fn##FAIL265,_fn##26,  \
    _fn##FAIL251,_fn##FAIL252,_fn##FAIL253,_fn##FAIL254,_fn##FAIL255,_fn##25,  \
    _fn##FAIL241,_fn##FAIL242,_fn##FAIL243,_fn##FAIL244,_fn##FAIL245,_fn##24,  \
    _fn##FAIL231,_fn##FAIL232,_fn##FAIL233,_fn##FAIL234,_fn##FAIL235,_fn##23,  \
    _fn##FAIL221,_fn##FAIL222,_fn##FAIL223,_fn##FAIL224,_fn##FAIL225,_fn##22,  \
    _fn##FAIL211,_fn##FAIL212,_fn##FAIL213,_fn##FAIL214,_fn##FAIL215,_fn##21,  \
    _fn##FAIL201,_fn##FAIL202,_fn##FAIL203,_fn##FAIL204,_fn##FAIL205,_fn##20,  \
    _fn##FAIL191,_fn##FAIL192,_fn##FAIL193,_fn##FAIL194,_fn##FAIL195,_fn##19,  \
    _fn##FAIL181,_fn##FAIL182,_fn##FAIL183,_fn##FAIL184,_fn##FAIL185,_fn##18,  \
    _fn##FAIL171,_fn##FAIL172,_fn##FAIL173,_fn##FAIL174,_fn##FAIL175,_fn##17,  \
    _fn##FAIL161,_fn##FAIL162,_fn##FAIL163,_fn##FAIL164,_fn##FAIL165,_fn##16,  \
    _fn##FAIL151,_fn##FAIL152,_fn##FAIL153,_fn##FAIL154,_fn##FAIL155,_fn##15,  \
    _fn##FAIL141,_fn##FAIL142,_fn##FAIL143,_fn##FAIL144,_fn##FAIL145,_fn##14,  \
    _fn##FAIL131,_fn##FAIL132,_fn##FAIL133,_fn##FAIL134,_fn##FAIL135,_fn##13,  \
    _fn##FAIL121,_fn##FAIL122,_fn##FAIL123,_fn##FAIL124,_fn##FAIL125,_fn##12,  \
    _fn##FAIL111,_fn##FAIL112,_fn##FAIL113,_fn##FAIL114,_fn##FAIL115,_fn##11,  \
    _fn##FAIL101,_fn##FAIL102,_fn##FAIL103,_fn##FAIL104,_fn##FAIL105,_fn##10,  \
    _fn##FAIL091,_fn##FAIL092,_fn##FAIL093,_fn##FAIL094,_fn##FAIL095,_fn##9,   \
    _fn##FAIL081,_fn##FAIL082,_fn##FAIL083,_fn##FAIL084,_fn##FAIL085,_fn##8,   \
    _fn##FAIL071,_fn##FAIL072,_fn##FAIL073,_fn##FAIL074,_fn##FAIL075,_fn##7,   \
    _fn##FAIL061,_fn##FAIL062,_fn##FAIL063,_fn##FAIL064,_fn##FAIL065,_fn##6,   \
    _fn##FAIL051,_fn##FAIL052,_fn##FAIL053,_fn##FAIL054,_fn##FAIL055,_fn##5,   \
    _fn##FAIL041,_fn##FAIL042,_fn##FAIL043,_fn##FAIL044,_fn##FAIL045,_fn##4,   \
    _fn##FAIL031,_fn##FAIL032,_fn##FAIL033,_fn##FAIL034,_fn##FAIL035,_fn##3,   \
    _fn##FAIL021,_fn##FAIL022,_fn##FAIL023,_fn##FAIL024,_fn##FAIL025,_fn##2,   \
    _fn##FAIL011,_fn##FAIL012,_fn##FAIL013,_fn##FAIL014,_fn##FAIL015,_fn##1,   \
    _fn##FAIL_N1,_fn##FAIL_N2,_fn##FAIL_N3,_fn##FAIL_N4,_fn##FAIL_N5, None)

#define TAPI_HELEPER_VARIANTSTRUCT_PARAMS1( n, t, d, i, to, fr)      t n;
#define TAPI_HELEPER_VARIANTSTRUCT_PARAMS2( n, t, d, i, to, fr, ...) t n; TAPI_HELEPER_VARIANTSTRUCT_PARAMS1(__VA_ARGS__)
#define TAPI_HELEPER_VARIANTSTRUCT_PARAMS3( n, t, d, i, to, fr, ...) t n; TAPI_HELEPER_VARIANTSTRUCT_PARAMS2(__VA_ARGS__)
#define TAPI_HELEPER_VARIANTSTRUCT_PARAMS4( n, t, d, i, to, fr, ...) t n; TAPI_HELEPER_VARIANTSTRUCT_PARAMS3(__VA_ARGS__)
#define TAPI_HELEPER_VARIANTSTRUCT_PARAMS5( n, t, d, i, to, fr, ...) t n; TAPI_HELEPER_VARIANTSTRUCT_PARAMS4(__VA_ARGS__)
#define TAPI_HELEPER_VARIANTSTRUCT_PARAMS6( n, t, d, i, to, fr, ...) t n; TAPI_HELEPER_VARIANTSTRUCT_PARAMS5(__VA_ARGS__)
#define TAPI_HELEPER_VARIANTSTRUCT_PARAMS7( n, t, d, i, to, fr, ...) t n; TAPI_HELEPER_VARIANTSTRUCT_PARAMS6(__VA_ARGS__)
#define TAPI_HELEPER_VARIANTSTRUCT_PARAMS8( n, t, d, i, to, fr, ...) t n; TAPI_HELEPER_VARIANTSTRUCT_PARAMS7(__VA_ARGS__)
#define TAPI_HELEPER_VARIANTSTRUCT_PARAMS9( n, t, d, i, to, fr, ...) t n; TAPI_HELEPER_VARIANTSTRUCT_PARAMS8(__VA_ARGS__)
#define TAPI_HELEPER_VARIANTSTRUCT_PARAMS10(n, t, d, i, to, fr, ...) t n; TAPI_HELEPER_VARIANTSTRUCT_PARAMS9(__VA_ARGS__)
#define TAPI_HELEPER_VARIANTSTRUCT_PARAMS11(n, t, d, i, to, fr, ...) t n; TAPI_HELEPER_VARIANTSTRUCT_PARAMS10(__VA_ARGS__)
#define TAPI_HELEPER_VARIANTSTRUCT_PARAMS12(n, t, d, i, to, fr, ...) t n; TAPI_HELEPER_VARIANTSTRUCT_PARAMS11(__VA_ARGS__)
#define TAPI_HELEPER_VARIANTSTRUCT_PARAMS13(n, t, d, i, to, fr, ...) t n; TAPI_HELEPER_VARIANTSTRUCT_PARAMS12(__VA_ARGS__)
#define TAPI_HELEPER_VARIANTSTRUCT_PARAMS14(n, t, d, i, to, fr, ...) t n; TAPI_HELEPER_VARIANTSTRUCT_PARAMS13(__VA_ARGS__)
#define TAPI_HELEPER_VARIANTSTRUCT_PARAMS15(n, t, d, i, to, fr, ...) t n; TAPI_HELEPER_VARIANTSTRUCT_PARAMS14(__VA_ARGS__)
#define TAPI_HELEPER_VARIANTSTRUCT_PARAMS16(n, t, d, i, to, fr, ...) t n; TAPI_HELEPER_VARIANTSTRUCT_PARAMS15(__VA_ARGS__)
#define TAPI_HELEPER_VARIANTSTRUCT_PARAMS17(n, t, d, i, to, fr, ...) t n; TAPI_HELEPER_VARIANTSTRUCT_PARAMS16(__VA_ARGS__)
#define TAPI_HELEPER_VARIANTSTRUCT_PARAMS18(n, t, d, i, to, fr, ...) t n; TAPI_HELEPER_VARIANTSTRUCT_PARAMS17(__VA_ARGS__)
#define TAPI_HELEPER_VARIANTSTRUCT_PARAMS19(n, t, d, i, to, fr, ...) t n; TAPI_HELEPER_VARIANTSTRUCT_PARAMS18(__VA_ARGS__)
#define TAPI_HELEPER_VARIANTSTRUCT_PARAMS20(n, t, d, i, to, fr, ...) t n; TAPI_HELEPER_VARIANTSTRUCT_PARAMS19(__VA_ARGS__)
#define TAPI_HELEPER_VARIANTSTRUCT_PARAMS21(n, t, d, i, to, fr, ...) t n; TAPI_HELEPER_VARIANTSTRUCT_PARAMS20(__VA_ARGS__)
#define TAPI_HELEPER_VARIANTSTRUCT_PARAMS22(n, t, d, i, to, fr, ...) t n; TAPI_HELEPER_VARIANTSTRUCT_PARAMS21(__VA_ARGS__)
#define TAPI_HELEPER_VARIANTSTRUCT_PARAMS23(n, t, d, i, to, fr, ...) t n; TAPI_HELEPER_VARIANTSTRUCT_PARAMS22(__VA_ARGS__)
#define TAPI_HELEPER_VARIANTSTRUCT_PARAMS24(n, t, d, i, to, fr, ...) t n; TAPI_HELEPER_VARIANTSTRUCT_PARAMS23(__VA_ARGS__)
#define TAPI_HELEPER_VARIANTSTRUCT_PARAMS25(n, t, d, i, to, fr, ...) t n; TAPI_HELEPER_VARIANTSTRUCT_PARAMS24(__VA_ARGS__)
#define TAPI_HELEPER_VARIANTSTRUCT_PARAMS26(n, t, d, i, to, fr, ...) t n; TAPI_HELEPER_VARIANTSTRUCT_PARAMS25(__VA_ARGS__)
#define TAPI_HELEPER_VARIANTSTRUCT_PARAMS27(n, t, d, i, to, fr, ...) t n; TAPI_HELEPER_VARIANTSTRUCT_PARAMS26(__VA_ARGS__)
#define TAPI_HELEPER_VARIANTSTRUCT_PARAMS28(n, t, d, i, to, fr, ...) t n; TAPI_HELEPER_VARIANTSTRUCT_PARAMS27(__VA_ARGS__)
#define TAPI_HELEPER_VARIANTSTRUCT_PARAMS29(n, t, d, i, to, fr, ...) t n; TAPI_HELEPER_VARIANTSTRUCT_PARAMS28(__VA_ARGS__)
#define TAPI_HELEPER_VARIANTSTRUCT_PARAMS30(n, t, d, i, to, fr, ...) t n; TAPI_HELEPER_VARIANTSTRUCT_PARAMS29(__VA_ARGS__)

#define TAPI_HELEPER_VARIANTSTRUCT_CONS_INPUT1( n, t, d, i, to, fr)      const t& _##n=d
#define TAPI_HELEPER_VARIANTSTRUCT_CONS_INPUT2( n, t, d, i, to, fr, ...) const t& _##n=d, TAPI_HELEPER_VARIANTSTRUCT_CONS_INPUT1(__VA_ARGS__)
#define TAPI_HELEPER_VARIANTSTRUCT_CONS_INPUT3( n, t, d, i, to, fr, ...) const t& _##n=d, TAPI_HELEPER_VARIANTSTRUCT_CONS_INPUT2(__VA_ARGS__)
#define TAPI_HELEPER_VARIANTSTRUCT_CONS_INPUT4( n, t, d, i, to, fr, ...) const t& _##n=d, TAPI_HELEPER_VARIANTSTRUCT_CONS_INPUT3(__VA_ARGS__)
#define TAPI_HELEPER_VARIANTSTRUCT_CONS_INPUT5( n, t, d, i, to, fr, ...) const t& _##n=d, TAPI_HELEPER_VARIANTSTRUCT_CONS_INPUT4(__VA_ARGS__)
#define TAPI_HELEPER_VARIANTSTRUCT_CONS_INPUT6( n, t, d, i, to, fr, ...) const t& _##n=d, TAPI_HELEPER_VARIANTSTRUCT_CONS_INPUT5(__VA_ARGS__)
#define TAPI_HELEPER_VARIANTSTRUCT_CONS_INPUT7( n, t, d, i, to, fr, ...) const t& _##n=d, TAPI_HELEPER_VARIANTSTRUCT_CONS_INPUT6(__VA_ARGS__)
#define TAPI_HELEPER_VARIANTSTRUCT_CONS_INPUT8( n, t, d, i, to, fr, ...) const t& _##n=d, TAPI_HELEPER_VARIANTSTRUCT_CONS_INPUT7(__VA_ARGS__)
#define TAPI_HELEPER_VARIANTSTRUCT_CONS_INPUT9( n, t, d, i, to, fr, ...) const t& _##n=d, TAPI_HELEPER_VARIANTSTRUCT_CONS_INPUT8(__VA_ARGS__)
#define TAPI_HELEPER_VARIANTSTRUCT_CONS_INPUT10(n, t, d, i, to, fr, ...) const t& _##n=d, TAPI_HELEPER_VARIANTSTRUCT_CONS_INPUT9(__VA_ARGS__)
#define TAPI_HELEPER_VARIANTSTRUCT_CONS_INPUT11(n, t, d, i, to, fr, ...) const t& _##n=d, TAPI_HELEPER_VARIANTSTRUCT_CONS_INPUT10(__VA_ARGS__)
#define TAPI_HELEPER_VARIANTSTRUCT_CONS_INPUT12(n, t, d, i, to, fr, ...) const t& _##n=d, TAPI_HELEPER_VARIANTSTRUCT_CONS_INPUT11(__VA_ARGS__)
#define TAPI_HELEPER_VARIANTSTRUCT_CONS_INPUT13(n, t, d, i, to, fr, ...) const t& _##n=d, TAPI_HELEPER_VARIANTSTRUCT_CONS_INPUT12(__VA_ARGS__)
#define TAPI_HELEPER_VARIANTSTRUCT_CONS_INPUT14(n, t, d, i, to, fr, ...) const t& _##n=d, TAPI_HELEPER_VARIANTSTRUCT_CONS_INPUT13(__VA_ARGS__)
#define TAPI_HELEPER_VARIANTSTRUCT_CONS_INPUT15(n, t, d, i, to, fr, ...) const t& _##n=d, TAPI_HELEPER_VARIANTSTRUCT_CONS_INPUT14(__VA_ARGS__)
#define TAPI_HELEPER_VARIANTSTRUCT_CONS_INPUT16(n, t, d, i, to, fr, ...) const t& _##n=d, TAPI_HELEPER_VARIANTSTRUCT_CONS_INPUT15(__VA_ARGS__)
#define TAPI_HELEPER_VARIANTSTRUCT_CONS_INPUT17(n, t, d, i, to, fr, ...) const t& _##n=d, TAPI_HELEPER_VARIANTSTRUCT_CONS_INPUT16(__VA_ARGS__)
#define TAPI_HELEPER_VARIANTSTRUCT_CONS_INPUT18(n, t, d, i, to, fr, ...) const t& _##n=d, TAPI_HELEPER_VARIANTSTRUCT_CONS_INPUT17(__VA_ARGS__)
#define TAPI_HELEPER_VARIANTSTRUCT_CONS_INPUT19(n, t, d, i, to, fr, ...) const t& _##n=d, TAPI_HELEPER_VARIANTSTRUCT_CONS_INPUT18(__VA_ARGS__)
#define TAPI_HELEPER_VARIANTSTRUCT_CONS_INPUT20(n, t, d, i, to, fr, ...) const t& _##n=d, TAPI_HELEPER_VARIANTSTRUCT_CONS_INPUT19(__VA_ARGS__)
#define TAPI_HELEPER_VARIANTSTRUCT_CONS_INPUT21(n, t, d, i, to, fr, ...) const t& _##n=d, TAPI_HELEPER_VARIANTSTRUCT_CONS_INPUT20(__VA_ARGS__)
#define TAPI_HELEPER_VARIANTSTRUCT_CONS_INPUT22(n, t, d, i, to, fr, ...) const t& _##n=d, TAPI_HELEPER_VARIANTSTRUCT_CONS_INPUT21(__VA_ARGS__)
#define TAPI_HELEPER_VARIANTSTRUCT_CONS_INPUT23(n, t, d, i, to, fr, ...) const t& _##n=d, TAPI_HELEPER_VARIANTSTRUCT_CONS_INPUT22(__VA_ARGS__)
#define TAPI_HELEPER_VARIANTSTRUCT_CONS_INPUT24(n, t, d, i, to, fr, ...) const t& _##n=d, TAPI_HELEPER_VARIANTSTRUCT_CONS_INPUT23(__VA_ARGS__)
#define TAPI_HELEPER_VARIANTSTRUCT_CONS_INPUT25(n, t, d, i, to, fr, ...) const t& _##n=d, TAPI_HELEPER_VARIANTSTRUCT_CONS_INPUT24(__VA_ARGS__)
#define TAPI_HELEPER_VARIANTSTRUCT_CONS_INPUT26(n, t, d, i, to, fr, ...) const t& _##n=d, TAPI_HELEPER_VARIANTSTRUCT_CONS_INPUT25(__VA_ARGS__)
#define TAPI_HELEPER_VARIANTSTRUCT_CONS_INPUT27(n, t, d, i, to, fr, ...) const t& _##n=d, TAPI_HELEPER_VARIANTSTRUCT_CONS_INPUT26(__VA_ARGS__)
#define TAPI_HELEPER_VARIANTSTRUCT_CONS_INPUT28(n, t, d, i, to, fr, ...) const t& _##n=d, TAPI_HELEPER_VARIANTSTRUCT_CONS_INPUT27(__VA_ARGS__)
#define TAPI_HELEPER_VARIANTSTRUCT_CONS_INPUT29(n, t, d, i, to, fr, ...) const t& _##n=d, TAPI_HELEPER_VARIANTSTRUCT_CONS_INPUT28(__VA_ARGS__)
#define TAPI_HELEPER_VARIANTSTRUCT_CONS_INPUT30(n, t, d, i, to, fr, ...) const t& _##n=d, TAPI_HELEPER_VARIANTSTRUCT_CONS_INPUT29(__VA_ARGS__)

#define TAPI_HELEPER_VARIANTSTRUCT_CONS_INIT1( n, t, d, i, to, fr)      n(_##n)
#define TAPI_HELEPER_VARIANTSTRUCT_CONS_INIT2( n, t, d, i, to, fr, ...) n(_##n), TAPI_HELEPER_VARIANTSTRUCT_CONS_INIT1(__VA_ARGS__)
#define TAPI_HELEPER_VARIANTSTRUCT_CONS_INIT3( n, t, d, i, to, fr, ...) n(_##n), TAPI_HELEPER_VARIANTSTRUCT_CONS_INIT2(__VA_ARGS__)
#define TAPI_HELEPER_VARIANTSTRUCT_CONS_INIT4( n, t, d, i, to, fr, ...) n(_##n), TAPI_HELEPER_VARIANTSTRUCT_CONS_INIT3(__VA_ARGS__)
#define TAPI_HELEPER_VARIANTSTRUCT_CONS_INIT5( n, t, d, i, to, fr, ...) n(_##n), TAPI_HELEPER_VARIANTSTRUCT_CONS_INIT4(__VA_ARGS__)
#define TAPI_HELEPER_VARIANTSTRUCT_CONS_INIT6( n, t, d, i, to, fr, ...) n(_##n), TAPI_HELEPER_VARIANTSTRUCT_CONS_INIT5(__VA_ARGS__)
#define TAPI_HELEPER_VARIANTSTRUCT_CONS_INIT7( n, t, d, i, to, fr, ...) n(_##n), TAPI_HELEPER_VARIANTSTRUCT_CONS_INIT6(__VA_ARGS__)
#define TAPI_HELEPER_VARIANTSTRUCT_CONS_INIT8( n, t, d, i, to, fr, ...) n(_##n), TAPI_HELEPER_VARIANTSTRUCT_CONS_INIT7(__VA_ARGS__)
#define TAPI_HELEPER_VARIANTSTRUCT_CONS_INIT9( n, t, d, i, to, fr, ...) n(_##n), TAPI_HELEPER_VARIANTSTRUCT_CONS_INIT8(__VA_ARGS__)
#define TAPI_HELEPER_VARIANTSTRUCT_CONS_INIT10(n, t, d, i, to, fr, ...) n(_##n), TAPI_HELEPER_VARIANTSTRUCT_CONS_INIT9(__VA_ARGS__)
#define TAPI_HELEPER_VARIANTSTRUCT_CONS_INIT11(n, t, d, i, to, fr, ...) n(_##n), TAPI_HELEPER_VARIANTSTRUCT_CONS_INIT10(__VA_ARGS__)
#define TAPI_HELEPER_VARIANTSTRUCT_CONS_INIT12(n, t, d, i, to, fr, ...) n(_##n), TAPI_HELEPER_VARIANTSTRUCT_CONS_INIT11(__VA_ARGS__)
#define TAPI_HELEPER_VARIANTSTRUCT_CONS_INIT13(n, t, d, i, to, fr, ...) n(_##n), TAPI_HELEPER_VARIANTSTRUCT_CONS_INIT12(__VA_ARGS__)
#define TAPI_HELEPER_VARIANTSTRUCT_CONS_INIT14(n, t, d, i, to, fr, ...) n(_##n), TAPI_HELEPER_VARIANTSTRUCT_CONS_INIT13(__VA_ARGS__)
#define TAPI_HELEPER_VARIANTSTRUCT_CONS_INIT15(n, t, d, i, to, fr, ...) n(_##n), TAPI_HELEPER_VARIANTSTRUCT_CONS_INIT14(__VA_ARGS__)
#define TAPI_HELEPER_VARIANTSTRUCT_CONS_INIT16(n, t, d, i, to, fr, ...) n(_##n), TAPI_HELEPER_VARIANTSTRUCT_CONS_INIT15(__VA_ARGS__)
#define TAPI_HELEPER_VARIANTSTRUCT_CONS_INIT17(n, t, d, i, to, fr, ...) n(_##n), TAPI_HELEPER_VARIANTSTRUCT_CONS_INIT16(__VA_ARGS__)
#define TAPI_HELEPER_VARIANTSTRUCT_CONS_INIT18(n, t, d, i, to, fr, ...) n(_##n), TAPI_HELEPER_VARIANTSTRUCT_CONS_INIT17(__VA_ARGS__)
#define TAPI_HELEPER_VARIANTSTRUCT_CONS_INIT19(n, t, d, i, to, fr, ...) n(_##n), TAPI_HELEPER_VARIANTSTRUCT_CONS_INIT18(__VA_ARGS__)
#define TAPI_HELEPER_VARIANTSTRUCT_CONS_INIT20(n, t, d, i, to, fr, ...) n(_##n), TAPI_HELEPER_VARIANTSTRUCT_CONS_INIT19(__VA_ARGS__)
#define TAPI_HELEPER_VARIANTSTRUCT_CONS_INIT21(n, t, d, i, to, fr, ...) n(_##n), TAPI_HELEPER_VARIANTSTRUCT_CONS_INIT20(__VA_ARGS__)
#define TAPI_HELEPER_VARIANTSTRUCT_CONS_INIT22(n, t, d, i, to, fr, ...) n(_##n), TAPI_HELEPER_VARIANTSTRUCT_CONS_INIT21(__VA_ARGS__)
#define TAPI_HELEPER_VARIANTSTRUCT_CONS_INIT23(n, t, d, i, to, fr, ...) n(_##n), TAPI_HELEPER_VARIANTSTRUCT_CONS_INIT22(__VA_ARGS__)
#define TAPI_HELEPER_VARIANTSTRUCT_CONS_INIT24(n, t, d, i, to, fr, ...) n(_##n), TAPI_HELEPER_VARIANTSTRUCT_CONS_INIT23(__VA_ARGS__)
#define TAPI_HELEPER_VARIANTSTRUCT_CONS_INIT25(n, t, d, i, to, fr, ...) n(_##n), TAPI_HELEPER_VARIANTSTRUCT_CONS_INIT24(__VA_ARGS__)
#define TAPI_HELEPER_VARIANTSTRUCT_CONS_INIT26(n, t, d, i, to, fr, ...) n(_##n), TAPI_HELEPER_VARIANTSTRUCT_CONS_INIT25(__VA_ARGS__)
#define TAPI_HELEPER_VARIANTSTRUCT_CONS_INIT27(n, t, d, i, to, fr, ...) n(_##n), TAPI_HELEPER_VARIANTSTRUCT_CONS_INIT26(__VA_ARGS__)
#define TAPI_HELEPER_VARIANTSTRUCT_CONS_INIT28(n, t, d, i, to, fr, ...) n(_##n), TAPI_HELEPER_VARIANTSTRUCT_CONS_INIT27(__VA_ARGS__)
#define TAPI_HELEPER_VARIANTSTRUCT_CONS_INIT29(n, t, d, i, to, fr, ...) n(_##n), TAPI_HELEPER_VARIANTSTRUCT_CONS_INIT28(__VA_ARGS__)
#define TAPI_HELEPER_VARIANTSTRUCT_CONS_INIT30(n, t, d, i, to, fr, ...) n(_##n), TAPI_HELEPER_VARIANTSTRUCT_CONS_INIT29(__VA_ARGS__)

//extern QString toCammel(const QString& _name);
inline QString toCammel(const QString& _name) {
    return _name.mid(0,1).toLower() + _name.mid(1);
}

#define TAPI_HELEPER_DEFINE_VARIANT_STRUCT_TOJSON_METHOD(n, i, to) \
    if ([](auto v)->bool{return i;}(n)) Obj[toCammel(#n)]=[](auto v)->QJsonValue{return to;}(n);
#define TAPI_HELEPER_VARIANTSTRUCT_TOJSON1( n, t, d, i, to, fr)     TAPI_HELEPER_DEFINE_VARIANT_STRUCT_TOJSON_METHOD(n, i, to);
#define TAPI_HELEPER_VARIANTSTRUCT_TOJSON2( n, t, d, i, to, fr,...) TAPI_HELEPER_DEFINE_VARIANT_STRUCT_TOJSON_METHOD(n, i, to); TAPI_HELEPER_VARIANTSTRUCT_TOJSON1(__VA_ARGS__)
#define TAPI_HELEPER_VARIANTSTRUCT_TOJSON3( n, t, d, i, to, fr,...) TAPI_HELEPER_DEFINE_VARIANT_STRUCT_TOJSON_METHOD(n, i, to); TAPI_HELEPER_VARIANTSTRUCT_TOJSON2(__VA_ARGS__)
#define TAPI_HELEPER_VARIANTSTRUCT_TOJSON4( n, t, d, i, to, fr,...) TAPI_HELEPER_DEFINE_VARIANT_STRUCT_TOJSON_METHOD(n, i, to); TAPI_HELEPER_VARIANTSTRUCT_TOJSON3(__VA_ARGS__)
#define TAPI_HELEPER_VARIANTSTRUCT_TOJSON5( n, t, d, i, to, fr,...) TAPI_HELEPER_DEFINE_VARIANT_STRUCT_TOJSON_METHOD(n, i, to); TAPI_HELEPER_VARIANTSTRUCT_TOJSON4(__VA_ARGS__)
#define TAPI_HELEPER_VARIANTSTRUCT_TOJSON6( n, t, d, i, to, fr,...) TAPI_HELEPER_DEFINE_VARIANT_STRUCT_TOJSON_METHOD(n, i, to); TAPI_HELEPER_VARIANTSTRUCT_TOJSON5(__VA_ARGS__)
#define TAPI_HELEPER_VARIANTSTRUCT_TOJSON7( n, t, d, i, to, fr,...) TAPI_HELEPER_DEFINE_VARIANT_STRUCT_TOJSON_METHOD(n, i, to); TAPI_HELEPER_VARIANTSTRUCT_TOJSON6(__VA_ARGS__)
#define TAPI_HELEPER_VARIANTSTRUCT_TOJSON8( n, t, d, i, to, fr,...) TAPI_HELEPER_DEFINE_VARIANT_STRUCT_TOJSON_METHOD(n, i, to); TAPI_HELEPER_VARIANTSTRUCT_TOJSON7(__VA_ARGS__)
#define TAPI_HELEPER_VARIANTSTRUCT_TOJSON9( n, t, d, i, to, fr,...) TAPI_HELEPER_DEFINE_VARIANT_STRUCT_TOJSON_METHOD(n, i, to); TAPI_HELEPER_VARIANTSTRUCT_TOJSON8(__VA_ARGS__)
#define TAPI_HELEPER_VARIANTSTRUCT_TOJSON10(n, t, d, i, to, fr,...) TAPI_HELEPER_DEFINE_VARIANT_STRUCT_TOJSON_METHOD(n, i, to); TAPI_HELEPER_VARIANTSTRUCT_TOJSON9(__VA_ARGS__)
#define TAPI_HELEPER_VARIANTSTRUCT_TOJSON11(n, t, d, i, to, fr,...) TAPI_HELEPER_DEFINE_VARIANT_STRUCT_TOJSON_METHOD(n, i, to); TAPI_HELEPER_VARIANTSTRUCT_TOJSON10(__VA_ARGS__)
#define TAPI_HELEPER_VARIANTSTRUCT_TOJSON12(n, t, d, i, to, fr,...) TAPI_HELEPER_DEFINE_VARIANT_STRUCT_TOJSON_METHOD(n, i, to); TAPI_HELEPER_VARIANTSTRUCT_TOJSON11(__VA_ARGS__)
#define TAPI_HELEPER_VARIANTSTRUCT_TOJSON13(n, t, d, i, to, fr,...) TAPI_HELEPER_DEFINE_VARIANT_STRUCT_TOJSON_METHOD(n, i, to); TAPI_HELEPER_VARIANTSTRUCT_TOJSON12(__VA_ARGS__)
#define TAPI_HELEPER_VARIANTSTRUCT_TOJSON14(n, t, d, i, to, fr,...) TAPI_HELEPER_DEFINE_VARIANT_STRUCT_TOJSON_METHOD(n, i, to); TAPI_HELEPER_VARIANTSTRUCT_TOJSON13(__VA_ARGS__)
#define TAPI_HELEPER_VARIANTSTRUCT_TOJSON15(n, t, d, i, to, fr,...) TAPI_HELEPER_DEFINE_VARIANT_STRUCT_TOJSON_METHOD(n, i, to); TAPI_HELEPER_VARIANTSTRUCT_TOJSON14(__VA_ARGS__)
#define TAPI_HELEPER_VARIANTSTRUCT_TOJSON16(n, t, d, i, to, fr,...) TAPI_HELEPER_DEFINE_VARIANT_STRUCT_TOJSON_METHOD(n, i, to); TAPI_HELEPER_VARIANTSTRUCT_TOJSON15(__VA_ARGS__)
#define TAPI_HELEPER_VARIANTSTRUCT_TOJSON17(n, t, d, i, to, fr,...) TAPI_HELEPER_DEFINE_VARIANT_STRUCT_TOJSON_METHOD(n, i, to); TAPI_HELEPER_VARIANTSTRUCT_TOJSON16(__VA_ARGS__)
#define TAPI_HELEPER_VARIANTSTRUCT_TOJSON18(n, t, d, i, to, fr,...) TAPI_HELEPER_DEFINE_VARIANT_STRUCT_TOJSON_METHOD(n, i, to); TAPI_HELEPER_VARIANTSTRUCT_TOJSON17(__VA_ARGS__)
#define TAPI_HELEPER_VARIANTSTRUCT_TOJSON19(n, t, d, i, to, fr,...) TAPI_HELEPER_DEFINE_VARIANT_STRUCT_TOJSON_METHOD(n, i, to); TAPI_HELEPER_VARIANTSTRUCT_TOJSON18(__VA_ARGS__)
#define TAPI_HELEPER_VARIANTSTRUCT_TOJSON20(n, t, d, i, to, fr,...) TAPI_HELEPER_DEFINE_VARIANT_STRUCT_TOJSON_METHOD(n, i, to); TAPI_HELEPER_VARIANTSTRUCT_TOJSON19(__VA_ARGS__)
#define TAPI_HELEPER_VARIANTSTRUCT_TOJSON21(n, t, d, i, to, fr,...) TAPI_HELEPER_DEFINE_VARIANT_STRUCT_TOJSON_METHOD(n, i, to); TAPI_HELEPER_VARIANTSTRUCT_TOJSON20(__VA_ARGS__)
#define TAPI_HELEPER_VARIANTSTRUCT_TOJSON22(n, t, d, i, to, fr,...) TAPI_HELEPER_DEFINE_VARIANT_STRUCT_TOJSON_METHOD(n, i, to); TAPI_HELEPER_VARIANTSTRUCT_TOJSON21(__VA_ARGS__)
#define TAPI_HELEPER_VARIANTSTRUCT_TOJSON23(n, t, d, i, to, fr,...) TAPI_HELEPER_DEFINE_VARIANT_STRUCT_TOJSON_METHOD(n, i, to); TAPI_HELEPER_VARIANTSTRUCT_TOJSON22(__VA_ARGS__)
#define TAPI_HELEPER_VARIANTSTRUCT_TOJSON24(n, t, d, i, to, fr,...) TAPI_HELEPER_DEFINE_VARIANT_STRUCT_TOJSON_METHOD(n, i, to); TAPI_HELEPER_VARIANTSTRUCT_TOJSON23(__VA_ARGS__)
#define TAPI_HELEPER_VARIANTSTRUCT_TOJSON25(n, t, d, i, to, fr,...) TAPI_HELEPER_DEFINE_VARIANT_STRUCT_TOJSON_METHOD(n, i, to); TAPI_HELEPER_VARIANTSTRUCT_TOJSON24(__VA_ARGS__)
#define TAPI_HELEPER_VARIANTSTRUCT_TOJSON26(n, t, d, i, to, fr,...) TAPI_HELEPER_DEFINE_VARIANT_STRUCT_TOJSON_METHOD(n, i, to); TAPI_HELEPER_VARIANTSTRUCT_TOJSON25(__VA_ARGS__)
#define TAPI_HELEPER_VARIANTSTRUCT_TOJSON27(n, t, d, i, to, fr,...) TAPI_HELEPER_DEFINE_VARIANT_STRUCT_TOJSON_METHOD(n, i, to); TAPI_HELEPER_VARIANTSTRUCT_TOJSON26(__VA_ARGS__)
#define TAPI_HELEPER_VARIANTSTRUCT_TOJSON28(n, t, d, i, to, fr,...) TAPI_HELEPER_DEFINE_VARIANT_STRUCT_TOJSON_METHOD(n, i, to); TAPI_HELEPER_VARIANTSTRUCT_TOJSON27(__VA_ARGS__)
#define TAPI_HELEPER_VARIANTSTRUCT_TOJSON29(n, t, d, i, to, fr,...) TAPI_HELEPER_DEFINE_VARIANT_STRUCT_TOJSON_METHOD(n, i, to); TAPI_HELEPER_VARIANTSTRUCT_TOJSON28(__VA_ARGS__)
#define TAPI_HELEPER_VARIANTSTRUCT_TOJSON30(n, t, d, i, to, fr,...) TAPI_HELEPER_DEFINE_VARIANT_STRUCT_TOJSON_METHOD(n, i, to); TAPI_HELEPER_VARIANTSTRUCT_TOJSON29(__VA_ARGS__)

#define TAPI_HELEPER_DEFINE_VARIANT_STRUCT_FROMJSON_METHOD(n, t, d, fr) \
    n=_obj.contains(toCammel(#n))?[](auto v)->t{return fr;}(_obj.value(toCammel(#n))):d
#define TAPI_HELEPER_VARIANTSTRUCT_FROMJSON1( n, t, d, i, to, fr)     TAPI_HELEPER_DEFINE_VARIANT_STRUCT_FROMJSON_METHOD(n, t, d, fr);
#define TAPI_HELEPER_VARIANTSTRUCT_FROMJSON2( n, t, d, i, to, fr,...) TAPI_HELEPER_DEFINE_VARIANT_STRUCT_FROMJSON_METHOD(n, t, d, fr); TAPI_HELEPER_VARIANTSTRUCT_FROMJSON1(__VA_ARGS__)
#define TAPI_HELEPER_VARIANTSTRUCT_FROMJSON3( n, t, d, i, to, fr,...) TAPI_HELEPER_DEFINE_VARIANT_STRUCT_FROMJSON_METHOD(n, t, d, fr); TAPI_HELEPER_VARIANTSTRUCT_FROMJSON2(__VA_ARGS__)
#define TAPI_HELEPER_VARIANTSTRUCT_FROMJSON4( n, t, d, i, to, fr,...) TAPI_HELEPER_DEFINE_VARIANT_STRUCT_FROMJSON_METHOD(n, t, d, fr); TAPI_HELEPER_VARIANTSTRUCT_FROMJSON3(__VA_ARGS__)
#define TAPI_HELEPER_VARIANTSTRUCT_FROMJSON5( n, t, d, i, to, fr,...) TAPI_HELEPER_DEFINE_VARIANT_STRUCT_FROMJSON_METHOD(n, t, d, fr); TAPI_HELEPER_VARIANTSTRUCT_FROMJSON4(__VA_ARGS__)
#define TAPI_HELEPER_VARIANTSTRUCT_FROMJSON6( n, t, d, i, to, fr,...) TAPI_HELEPER_DEFINE_VARIANT_STRUCT_FROMJSON_METHOD(n, t, d, fr); TAPI_HELEPER_VARIANTSTRUCT_FROMJSON5(__VA_ARGS__)
#define TAPI_HELEPER_VARIANTSTRUCT_FROMJSON7( n, t, d, i, to, fr,...) TAPI_HELEPER_DEFINE_VARIANT_STRUCT_FROMJSON_METHOD(n, t, d, fr); TAPI_HELEPER_VARIANTSTRUCT_FROMJSON6(__VA_ARGS__)
#define TAPI_HELEPER_VARIANTSTRUCT_FROMJSON8( n, t, d, i, to, fr,...) TAPI_HELEPER_DEFINE_VARIANT_STRUCT_FROMJSON_METHOD(n, t, d, fr); TAPI_HELEPER_VARIANTSTRUCT_FROMJSON7(__VA_ARGS__)
#define TAPI_HELEPER_VARIANTSTRUCT_FROMJSON9( n, t, d, i, to, fr,...) TAPI_HELEPER_DEFINE_VARIANT_STRUCT_FROMJSON_METHOD(n, t, d, fr); TAPI_HELEPER_VARIANTSTRUCT_FROMJSON8(__VA_ARGS__)
#define TAPI_HELEPER_VARIANTSTRUCT_FROMJSON10(n, t, d, i, to, fr,...) TAPI_HELEPER_DEFINE_VARIANT_STRUCT_FROMJSON_METHOD(n, t, d, fr); TAPI_HELEPER_VARIANTSTRUCT_FROMJSON9(__VA_ARGS__)
#define TAPI_HELEPER_VARIANTSTRUCT_FROMJSON11(n, t, d, i, to, fr,...) TAPI_HELEPER_DEFINE_VARIANT_STRUCT_FROMJSON_METHOD(n, t, d, fr); TAPI_HELEPER_VARIANTSTRUCT_FROMJSON10(__VA_ARGS__)
#define TAPI_HELEPER_VARIANTSTRUCT_FROMJSON12(n, t, d, i, to, fr,...) TAPI_HELEPER_DEFINE_VARIANT_STRUCT_FROMJSON_METHOD(n, t, d, fr); TAPI_HELEPER_VARIANTSTRUCT_FROMJSON11(__VA_ARGS__)
#define TAPI_HELEPER_VARIANTSTRUCT_FROMJSON13(n, t, d, i, to, fr,...) TAPI_HELEPER_DEFINE_VARIANT_STRUCT_FROMJSON_METHOD(n, t, d, fr); TAPI_HELEPER_VARIANTSTRUCT_FROMJSON12(__VA_ARGS__)
#define TAPI_HELEPER_VARIANTSTRUCT_FROMJSON14(n, t, d, i, to, fr,...) TAPI_HELEPER_DEFINE_VARIANT_STRUCT_FROMJSON_METHOD(n, t, d, fr); TAPI_HELEPER_VARIANTSTRUCT_FROMJSON13(__VA_ARGS__)
#define TAPI_HELEPER_VARIANTSTRUCT_FROMJSON15(n, t, d, i, to, fr,...) TAPI_HELEPER_DEFINE_VARIANT_STRUCT_FROMJSON_METHOD(n, t, d, fr); TAPI_HELEPER_VARIANTSTRUCT_FROMJSON14(__VA_ARGS__)
#define TAPI_HELEPER_VARIANTSTRUCT_FROMJSON16(n, t, d, i, to, fr,...) TAPI_HELEPER_DEFINE_VARIANT_STRUCT_FROMJSON_METHOD(n, t, d, fr); TAPI_HELEPER_VARIANTSTRUCT_FROMJSON15(__VA_ARGS__)
#define TAPI_HELEPER_VARIANTSTRUCT_FROMJSON17(n, t, d, i, to, fr,...) TAPI_HELEPER_DEFINE_VARIANT_STRUCT_FROMJSON_METHOD(n, t, d, fr); TAPI_HELEPER_VARIANTSTRUCT_FROMJSON16(__VA_ARGS__)
#define TAPI_HELEPER_VARIANTSTRUCT_FROMJSON18(n, t, d, i, to, fr,...) TAPI_HELEPER_DEFINE_VARIANT_STRUCT_FROMJSON_METHOD(n, t, d, fr); TAPI_HELEPER_VARIANTSTRUCT_FROMJSON17(__VA_ARGS__)
#define TAPI_HELEPER_VARIANTSTRUCT_FROMJSON19(n, t, d, i, to, fr,...) TAPI_HELEPER_DEFINE_VARIANT_STRUCT_FROMJSON_METHOD(n, t, d, fr); TAPI_HELEPER_VARIANTSTRUCT_FROMJSON18(__VA_ARGS__)
#define TAPI_HELEPER_VARIANTSTRUCT_FROMJSON20(n, t, d, i, to, fr,...) TAPI_HELEPER_DEFINE_VARIANT_STRUCT_FROMJSON_METHOD(n, t, d, fr); TAPI_HELEPER_VARIANTSTRUCT_FROMJSON19(__VA_ARGS__)
#define TAPI_HELEPER_VARIANTSTRUCT_FROMJSON21(n, t, d, i, to, fr,...) TAPI_HELEPER_DEFINE_VARIANT_STRUCT_FROMJSON_METHOD(n, t, d, fr); TAPI_HELEPER_VARIANTSTRUCT_FROMJSON20(__VA_ARGS__)
#define TAPI_HELEPER_VARIANTSTRUCT_FROMJSON22(n, t, d, i, to, fr,...) TAPI_HELEPER_DEFINE_VARIANT_STRUCT_FROMJSON_METHOD(n, t, d, fr); TAPI_HELEPER_VARIANTSTRUCT_FROMJSON21(__VA_ARGS__)
#define TAPI_HELEPER_VARIANTSTRUCT_FROMJSON23(n, t, d, i, to, fr,...) TAPI_HELEPER_DEFINE_VARIANT_STRUCT_FROMJSON_METHOD(n, t, d, fr); TAPI_HELEPER_VARIANTSTRUCT_FROMJSON22(__VA_ARGS__)
#define TAPI_HELEPER_VARIANTSTRUCT_FROMJSON24(n, t, d, i, to, fr,...) TAPI_HELEPER_DEFINE_VARIANT_STRUCT_FROMJSON_METHOD(n, t, d, fr); TAPI_HELEPER_VARIANTSTRUCT_FROMJSON23(__VA_ARGS__)
#define TAPI_HELEPER_VARIANTSTRUCT_FROMJSON25(n, t, d, i, to, fr,...) TAPI_HELEPER_DEFINE_VARIANT_STRUCT_FROMJSON_METHOD(n, t, d, fr); TAPI_HELEPER_VARIANTSTRUCT_FROMJSON24(__VA_ARGS__)
#define TAPI_HELEPER_VARIANTSTRUCT_FROMJSON26(n, t, d, i, to, fr,...) TAPI_HELEPER_DEFINE_VARIANT_STRUCT_FROMJSON_METHOD(n, t, d, fr); TAPI_HELEPER_VARIANTSTRUCT_FROMJSON25(__VA_ARGS__)
#define TAPI_HELEPER_VARIANTSTRUCT_FROMJSON27(n, t, d, i, to, fr,...) TAPI_HELEPER_DEFINE_VARIANT_STRUCT_FROMJSON_METHOD(n, t, d, fr); TAPI_HELEPER_VARIANTSTRUCT_FROMJSON26(__VA_ARGS__)
#define TAPI_HELEPER_VARIANTSTRUCT_FROMJSON28(n, t, d, i, to, fr,...) TAPI_HELEPER_DEFINE_VARIANT_STRUCT_FROMJSON_METHOD(n, t, d, fr); TAPI_HELEPER_VARIANTSTRUCT_FROMJSON27(__VA_ARGS__)
#define TAPI_HELEPER_VARIANTSTRUCT_FROMJSON29(n, t, d, i, to, fr,...) TAPI_HELEPER_DEFINE_VARIANT_STRUCT_FROMJSON_METHOD(n, t, d, fr); TAPI_HELEPER_VARIANTSTRUCT_FROMJSON28(__VA_ARGS__)
#define TAPI_HELEPER_VARIANTSTRUCT_FROMJSON30(n, t, d, i, to, fr,...) TAPI_HELEPER_DEFINE_VARIANT_STRUCT_FROMJSON_METHOD(n, t, d, fr); TAPI_HELEPER_VARIANTSTRUCT_FROMJSON29(__VA_ARGS__)

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
#define TAPI_REGISTER_METATYPE_FULL(_complexity, _namespace, _type, _toVariantLambda, _fromVariantLambda, _descriptionLambda, _toORMValueLambda, _fromORMValueLambda, _lambdaOptions) \
    namespace Targoman::API { \
        template <> std::function<QVariant(_namespace::_type _value)> tmplAPIArg<_namespace::_type, _complexity, false>::toVariantLambda = _toVariantLambda; \
        template <> std::function<_namespace::_type(QVariant _value, const QByteArray& _paramName)> tmplAPIArg<_namespace::_type, _complexity, false>::fromVariantLambda = _fromVariantLambda; \
        template <> std::function<QString(const QList<DBM::clsORMField>& _allFields)> tmplAPIArg<_namespace::_type, _complexity, false>::descriptionLambda = _descriptionLambda; \
        template <> std::function<QVariant(const QVariant&)> tmplAPIArg<_namespace::_type, _complexity, false>::toORMValueLambda = _toORMValueLambda; \
        template <> std::function<QVariant(const QVariant&)> tmplAPIArg<_namespace::_type, _complexity, false>::fromORMValueLambda = _fromORMValueLambda; \
        template <> std::function<QStringList()> tmplAPIArg<_namespace::_type, _complexity, false>::optionsLambda = _lambdaOptions; \
        template <> std::function<QVariant(NULLABLE_TYPE(_namespace::_type) _value)> tmplAPIArg<NULLABLE_TYPE(_namespace::_type), _complexity, true>::toVariantLambda = \
            [](NULLABLE_TYPE(_namespace::_type) _value) {return NULLABLE_IS_NULL(_value) ? QVariant() : tmplAPIArg<_namespace::_type, _complexity, false>::toVariant(*_value);}; \
        template <> std::function<NULLABLE_TYPE(_namespace::_type)(QVariant _value, const QByteArray& _paramName)> tmplAPIArg<NULLABLE_TYPE(_namespace::_type), _complexity, true>::fromVariantLambda = \
            [](const QVariant& _value, const QByteArray& _paramName) -> NULLABLE_TYPE(_namespace::_type) { \
                if (!_value.isValid() || _value.isNull()) \
                    return NULLABLE_TYPE(_namespace::_type)(); \
                NULLABLE_VAR(_namespace::_type, Value); \
                Value = tmplAPIArg<_namespace::_type, _complexity, false>::fromVariant(_value, _paramName); \
                return Value; \
            }; \
        template <> std::function<QVariant(const QVariant&)> tmplAPIArg<NULLABLE_TYPE(_namespace::_type), _complexity, true>::toORMValueLambda = _toORMValueLambda; \
        template <> std::function<QVariant(const QVariant&)> tmplAPIArg<NULLABLE_TYPE(_namespace::_type), _complexity, true>::fromORMValueLambda = _fromORMValueLambda; \
        template <> std::function<QStringList()> tmplAPIArg<NULLABLE_TYPE(_namespace::_type), _complexity, true>::optionsLambda = _lambdaOptions; \
        template <> std::function<QString(const QList<DBM::clsORMField>& _allFields)> tmplAPIArg<NULLABLE_TYPE(_namespace::_type), _complexity, true>::descriptionLambda = _descriptionLambda; \
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
        template <> std::function<QVariant(_namespace::_enum::Type _value)> tmplAPIArg<_namespace::_enum::Type, COMPLEXITY_Enum, false>::toVariantLambda = _toVariantLambda; \
        template <> std::function<_namespace::_enum::Type(QVariant _value, const QByteArray& _paramName)> tmplAPIArg<_namespace::_enum::Type, COMPLEXITY_Enum, false>::fromVariantLambda = _fromVariantLambda; \
        template <> std::function<QVariant(const QVariant&)> tmplAPIArg<_namespace::_enum::Type, COMPLEXITY_Enum, false>::toORMValueLambda = _toORMValueLambda; \
        template <> std::function<QVariant(const QVariant&)> tmplAPIArg<_namespace::_enum::Type, COMPLEXITY_Enum, false>::fromORMValueLambda = _fromORMValueLambda; \
        template <> std::function<QStringList()> tmplAPIArg<_namespace::_enum::Type, COMPLEXITY_Enum, false>::optionsLambda = _lambdaOptions; \
        template <> std::function<QString(const QList<DBM::clsORMField>& _allFields)> tmplAPIArg<_namespace::_enum::Type, COMPLEXITY_Enum, false>::descriptionLambda = _descriptionLambda; \
        template <> std::function<QVariant(NULLABLE_TYPE(_namespace::_enum::Type) _value)> tmplAPIArg<NULLABLE_TYPE(_namespace::_enum::Type), COMPLEXITY_Enum, true>::toVariantLambda = \
            [](NULLABLE_TYPE(_namespace::_enum::Type) _value) {return NULLABLE_IS_NULL(_value) ? QVariant() : tmplAPIArg<_namespace::_enum::Type, COMPLEXITY_Enum, false>::toVariant(*_value);}; \
        template <> std::function<NULLABLE_TYPE(_namespace::_enum::Type)(QVariant _value, const QByteArray& _paramName)> tmplAPIArg<NULLABLE_TYPE(_namespace::_enum::Type), COMPLEXITY_Enum, true>::fromVariantLambda = \
            [](const QVariant& _value, const QByteArray& _paramName) -> NULLABLE_TYPE(_namespace::_enum::Type) { \
                if (!_value.isValid() || _value.isNull()) return NULLABLE_TYPE(_namespace::_enum::Type)(); \
                NULLABLE_VAR(_namespace::_enum::Type, Value); \
                Value = tmplAPIArg<_namespace::_enum::Type, COMPLEXITY_Enum, false>::fromVariant(_value, _paramName); \
                return Value; \
            }; \
        template <> std::function<QVariant(const QVariant&)> tmplAPIArg<NULLABLE_TYPE(_namespace::_enum::Type), COMPLEXITY_Enum, true>::toORMValueLambda = _toORMValueLambda; \
        template <> std::function<QVariant(const QVariant&)> tmplAPIArg<NULLABLE_TYPE(_namespace::_enum::Type), COMPLEXITY_Enum, true>::fromORMValueLambda = _fromORMValueLambda; \
        template <> std::function<QStringList()> tmplAPIArg<NULLABLE_TYPE(_namespace::_enum::Type), COMPLEXITY_Enum, true>::optionsLambda = _lambdaOptions; \
        template <> std::function<QString(const QList<DBM::clsORMField>& _allFields)> tmplAPIArg<NULLABLE_TYPE(_namespace::_enum::Type), COMPLEXITY_Enum, true>::descriptionLambda = _descriptionLambda; \
    } \
    namespace _namespace { \
        using namespace Targoman::API; \
        static tmplAPIArg<_enum::Type, COMPLEXITY_Enum, false>* Dangling_##_enum = tmplAPIArg<_enum::Type, COMPLEXITY_Enum, false>::instance(TARGOMAN_M2STR(_namespace::_enum::Type)); \
        static tmplAPIArg<NULLABLE_TYPE(_enum::Type), COMPLEXITY_Enum, true>* Dangling_QSP_##_enum = tmplAPIArg<NULLABLE_TYPE(_enum::Type), COMPLEXITY_Enum, true>::instance(QSP_M2STR(_namespace::_enum::Type)); \
    }
//NULLABLE_TYPE(_namespace::_enum::Type) Value(new _namespace::_enum::Type);

/************************************************************/
#define INTERNAL_TAPI_ADD_TYPE_SPECIALFROMVARIANT(_baseType, _typeName, _setFromVariantLambda) \
    class _typeName : public _baseType { \
    public: \
        _typeName() { ; } \
        _typeName(const _baseType& _other) :_baseType(_other) { ; } \
        void customFromVariant(const QVariant& _value, const QString &_paramName = {}) { _setFromVariantLambda(_value, _paramName); } \
        static _typeName fromVariant(const QVariant& _value, const QString &_paramName = {}) { \
            _typeName _var; \
            _var.customFromVariant(_value, _paramName); \
            return _var; \
        } \
    }; \
    inline void setFromVariant(_typeName& _storage, const QVariant& _val, const QString &_paramName = "") { \
        _storage.customFromVariant(_val, _paramName); \
    } \
    inline void setFromVariant(NULLABLE_TYPE(_typeName)& _storage, const QVariant& _val, const QString &_paramName = "") { \
        if (_val.isValid() && _val.isNull() == false) { \
            _typeName var; \
            var.customFromVariant(_val, _paramName); \
            NULLABLE_SET(_storage, var); \
        } else \
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
        /* fromVariantLambda  */ [](const QVariant& _value, Q_DECL_UNUSED const QString& _paramName = "") -> _namespace::_enum::Type { \
            QVariant _val = (_value.userType() == QMetaType::QString ? _value : _namespace::_enum::toStr(_value.value<_namespace::_enum::Type>())); \
            if (_namespace::_enum::options().contains(_val.toString())) \
                return _namespace::_enum::toEnum(_val.toString()); \
            else \
                try { return _namespace::_enum::toEnum(_val.toString(), true); } \
                catch (...) { \
                    throw exHTTPBadRequest(QString("%1(%2) is not a valid %3") \
                        .arg(_paramName) \
                        .arg(_val.toString()) \
                        .arg(QString(TARGOMAN_M2STR(_enum)).startsWith("enu") ? QString(TARGOMAN_M2STR(_enum)).mid(3) : QString(TARGOMAN_M2STR(_enum))) \
                    ); \
                } \
        }, \
        /* descriptionLambda  */ [](const QList<Targoman::API::DBM::clsORMField>&) -> QString { \
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
//#define INTERNAL_V2double(v) INTERNAL_V2DBL(v)

#define INTERNAL_C2bool(v)      static_cast<bool>(v)
#define INTERNAL_V2bool(v)      static_cast<bool>(v.toDouble())
#define INTERNAL_N2bool(_value) INTERNAL_NULLABLE_FROM_JSONVALUE_TO_TYPE(bool, _value)

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

#define INTERNAL_SF_STRUCT(_name, _type, ...) \
    _name, \
    _type, \
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
#define INTERNAL_SF(_name, _type, _typeGroup, _fromVariant, _toVariant, ...) \
    _name, \
    _type, \
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
#define INTERNAL_TAPI_DECLARE_METATYPE_ENUM(_namespace, _enum) \
    TAPI_DECLARE_METATYPE(_namespace::_enum::Type); \
    namespace TAPI { \
        inline void setFromVariant(_namespace::_enum::Type& _storage, const QVariant& _val, Q_DECL_UNUSED const QString& _paramName = "") { \
            _storage = _namespace::_enum::toEnum(_val.toString()); \
        } \
        inline void setFromVariant(NULLABLE_TYPE(_namespace::_enum::Type)& _storage, const QVariant& _val, Q_DECL_UNUSED const QString& _paramName = "") { \
            _storage = NULLABLE_NULL_VALUE; \
            if (_val.isValid() && _val.isNull() == false) \
                _storage = _namespace::_enum::toEnum(_val.toString()); \
        } \
    }

#endif // APIARGHELPERMACROSPRIVATE_H
