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

#define TAPI_HELPER_STRUCT_MACRO_ARG_COUNT( \
    _491,_492,_493,_494,_495,_49,  \
    _481,_482,_483,_484,_485,_48,  \
    _471,_472,_473,_474,_475,_47,  \
    _461,_462,_463,_464,_465,_46,  \
    _451,_452,_453,_454,_455,_45,  \
    _441,_442,_443,_444,_445,_44,  \
    _431,_432,_433,_434,_435,_43,  \
    _421,_422,_423,_424,_425,_42,  \
    _411,_412,_413,_414,_415,_41,  \
    _401,_402,_403,_404,_405,_40,  \
    _391,_392,_393,_394,_395,_39,  \
    _381,_382,_383,_384,_385,_38,  \
    _371,_372,_373,_374,_375,_37,  \
    _361,_362,_363,_364,_365,_36,  \
    _351,_352,_353,_354,_355,_35,  \
    _341,_342,_343,_344,_345,_34,  \
    _331,_332,_333,_334,_335,_33,  \
    _321,_322,_323,_324,_325,_32,  \
    _311,_312,_313,_314,_315,_31,  \
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
    _001,_002,_003,_004,_005,_00,  \
    __N1,__N2,__N3,__N4,__N5,_N, ...) _N

#define TAPI_HELPER_STRUCT_MACRO_SELECTOR(_fn,...) TAPI_HELPER_STRUCT_MACRO_ARG_COUNT(__VA_ARGS__, \
    _fn##FAIL_491,_fn##FAIL_492,_fn##FAIL_493,_fn##FAIL_494,_fn##FAIL_495,_fn##_49,  \
    _fn##FAIL_481,_fn##FAIL_482,_fn##FAIL_483,_fn##FAIL_484,_fn##FAIL_485,_fn##_48,  \
    _fn##FAIL_471,_fn##FAIL_472,_fn##FAIL_473,_fn##FAIL_474,_fn##FAIL_475,_fn##_47,  \
    _fn##FAIL_461,_fn##FAIL_462,_fn##FAIL_463,_fn##FAIL_464,_fn##FAIL_465,_fn##_46,  \
    _fn##FAIL_451,_fn##FAIL_452,_fn##FAIL_453,_fn##FAIL_454,_fn##FAIL_455,_fn##_45,  \
    _fn##FAIL_441,_fn##FAIL_442,_fn##FAIL_443,_fn##FAIL_444,_fn##FAIL_445,_fn##_44,  \
    _fn##FAIL_431,_fn##FAIL_432,_fn##FAIL_433,_fn##FAIL_434,_fn##FAIL_435,_fn##_43,  \
    _fn##FAIL_421,_fn##FAIL_422,_fn##FAIL_423,_fn##FAIL_424,_fn##FAIL_425,_fn##_42,  \
    _fn##FAIL_411,_fn##FAIL_412,_fn##FAIL_413,_fn##FAIL_414,_fn##FAIL_415,_fn##_41,  \
    _fn##FAIL_401,_fn##FAIL_402,_fn##FAIL_403,_fn##FAIL_404,_fn##FAIL_405,_fn##_40,  \
    _fn##FAIL_391,_fn##FAIL_392,_fn##FAIL_393,_fn##FAIL_394,_fn##FAIL_395,_fn##_39,  \
    _fn##FAIL_381,_fn##FAIL_382,_fn##FAIL_383,_fn##FAIL_384,_fn##FAIL_385,_fn##_38,  \
    _fn##FAIL_371,_fn##FAIL_372,_fn##FAIL_373,_fn##FAIL_374,_fn##FAIL_375,_fn##_37,  \
    _fn##FAIL_361,_fn##FAIL_362,_fn##FAIL_363,_fn##FAIL_364,_fn##FAIL_365,_fn##_36,  \
    _fn##FAIL_351,_fn##FAIL_352,_fn##FAIL_353,_fn##FAIL_354,_fn##FAIL_355,_fn##_35,  \
    _fn##FAIL_341,_fn##FAIL_342,_fn##FAIL_343,_fn##FAIL_344,_fn##FAIL_345,_fn##_34,  \
    _fn##FAIL_331,_fn##FAIL_332,_fn##FAIL_333,_fn##FAIL_334,_fn##FAIL_335,_fn##_33,  \
    _fn##FAIL_321,_fn##FAIL_322,_fn##FAIL_323,_fn##FAIL_324,_fn##FAIL_325,_fn##_32,  \
    _fn##FAIL_311,_fn##FAIL_312,_fn##FAIL_313,_fn##FAIL_314,_fn##FAIL_315,_fn##_31,  \
    _fn##FAIL_301,_fn##FAIL_302,_fn##FAIL_303,_fn##FAIL_304,_fn##FAIL_305,_fn##_30,  \
    _fn##FAIL_291,_fn##FAIL_292,_fn##FAIL_293,_fn##FAIL_294,_fn##FAIL_295,_fn##_29,  \
    _fn##FAIL_281,_fn##FAIL_282,_fn##FAIL_283,_fn##FAIL_284,_fn##FAIL_285,_fn##_28,  \
    _fn##FAIL_271,_fn##FAIL_272,_fn##FAIL_273,_fn##FAIL_274,_fn##FAIL_275,_fn##_27,  \
    _fn##FAIL_261,_fn##FAIL_262,_fn##FAIL_263,_fn##FAIL_264,_fn##FAIL_265,_fn##_26,  \
    _fn##FAIL_251,_fn##FAIL_252,_fn##FAIL_253,_fn##FAIL_254,_fn##FAIL_255,_fn##_25,  \
    _fn##FAIL_241,_fn##FAIL_242,_fn##FAIL_243,_fn##FAIL_244,_fn##FAIL_245,_fn##_24,  \
    _fn##FAIL_231,_fn##FAIL_232,_fn##FAIL_233,_fn##FAIL_234,_fn##FAIL_235,_fn##_23,  \
    _fn##FAIL_221,_fn##FAIL_222,_fn##FAIL_223,_fn##FAIL_224,_fn##FAIL_225,_fn##_22,  \
    _fn##FAIL_211,_fn##FAIL_212,_fn##FAIL_213,_fn##FAIL_214,_fn##FAIL_215,_fn##_21,  \
    _fn##FAIL_201,_fn##FAIL_202,_fn##FAIL_203,_fn##FAIL_204,_fn##FAIL_205,_fn##_20,  \
    _fn##FAIL_191,_fn##FAIL_192,_fn##FAIL_193,_fn##FAIL_194,_fn##FAIL_195,_fn##_19,  \
    _fn##FAIL_181,_fn##FAIL_182,_fn##FAIL_183,_fn##FAIL_184,_fn##FAIL_185,_fn##_18,  \
    _fn##FAIL_171,_fn##FAIL_172,_fn##FAIL_173,_fn##FAIL_174,_fn##FAIL_175,_fn##_17,  \
    _fn##FAIL_161,_fn##FAIL_162,_fn##FAIL_163,_fn##FAIL_164,_fn##FAIL_165,_fn##_16,  \
    _fn##FAIL_151,_fn##FAIL_152,_fn##FAIL_153,_fn##FAIL_154,_fn##FAIL_155,_fn##_15,  \
    _fn##FAIL_141,_fn##FAIL_142,_fn##FAIL_143,_fn##FAIL_144,_fn##FAIL_145,_fn##_14,  \
    _fn##FAIL_131,_fn##FAIL_132,_fn##FAIL_133,_fn##FAIL_134,_fn##FAIL_135,_fn##_13,  \
    _fn##FAIL_121,_fn##FAIL_122,_fn##FAIL_123,_fn##FAIL_124,_fn##FAIL_125,_fn##_12,  \
    _fn##FAIL_111,_fn##FAIL_112,_fn##FAIL_113,_fn##FAIL_114,_fn##FAIL_115,_fn##_11,  \
    _fn##FAIL_101,_fn##FAIL_102,_fn##FAIL_103,_fn##FAIL_104,_fn##FAIL_105,_fn##_10,  \
    _fn##FAIL_091,_fn##FAIL_092,_fn##FAIL_093,_fn##FAIL_094,_fn##FAIL_095,_fn##_09,  \
    _fn##FAIL_081,_fn##FAIL_082,_fn##FAIL_083,_fn##FAIL_084,_fn##FAIL_085,_fn##_08,  \
    _fn##FAIL_071,_fn##FAIL_072,_fn##FAIL_073,_fn##FAIL_074,_fn##FAIL_075,_fn##_07,  \
    _fn##FAIL_061,_fn##FAIL_062,_fn##FAIL_063,_fn##FAIL_064,_fn##FAIL_065,_fn##_06,  \
    _fn##FAIL_051,_fn##FAIL_052,_fn##FAIL_053,_fn##FAIL_054,_fn##FAIL_055,_fn##_05,  \
    _fn##FAIL_041,_fn##FAIL_042,_fn##FAIL_043,_fn##FAIL_044,_fn##FAIL_045,_fn##_04,  \
    _fn##FAIL_031,_fn##FAIL_032,_fn##FAIL_033,_fn##FAIL_034,_fn##FAIL_035,_fn##_03,  \
    _fn##FAIL_021,_fn##FAIL_022,_fn##FAIL_023,_fn##FAIL_024,_fn##FAIL_025,_fn##_02,  \
    _fn##FAIL_011,_fn##FAIL_012,_fn##FAIL_013,_fn##FAIL_014,_fn##FAIL_015,_fn##_01,  \
    _fn##FAIL_001,_fn##FAIL_002,_fn##FAIL_003,_fn##FAIL_004,_fn##FAIL_005,_fn##_00,  \
    _fn##FAIL_N1,_fn##FAIL_N2,_fn##FAIL_N3,_fn##FAIL_N4,_fn##FAIL_N5, None)

#define TAPI_HELPER_STRUCT_PARAMS_00(n, t, d, i, to, fr)      t n;
#define TAPI_HELPER_STRUCT_PARAMS_01(n, t, d, i, to, fr, ...) t n; TAPI_HELPER_STRUCT_PARAMS_00(__VA_ARGS__)
#define TAPI_HELPER_STRUCT_PARAMS_02(n, t, d, i, to, fr, ...) t n; TAPI_HELPER_STRUCT_PARAMS_01(__VA_ARGS__)
#define TAPI_HELPER_STRUCT_PARAMS_03(n, t, d, i, to, fr, ...) t n; TAPI_HELPER_STRUCT_PARAMS_02(__VA_ARGS__)
#define TAPI_HELPER_STRUCT_PARAMS_04(n, t, d, i, to, fr, ...) t n; TAPI_HELPER_STRUCT_PARAMS_03(__VA_ARGS__)
#define TAPI_HELPER_STRUCT_PARAMS_05(n, t, d, i, to, fr, ...) t n; TAPI_HELPER_STRUCT_PARAMS_04(__VA_ARGS__)
#define TAPI_HELPER_STRUCT_PARAMS_06(n, t, d, i, to, fr, ...) t n; TAPI_HELPER_STRUCT_PARAMS_05(__VA_ARGS__)
#define TAPI_HELPER_STRUCT_PARAMS_07(n, t, d, i, to, fr, ...) t n; TAPI_HELPER_STRUCT_PARAMS_06(__VA_ARGS__)
#define TAPI_HELPER_STRUCT_PARAMS_08(n, t, d, i, to, fr, ...) t n; TAPI_HELPER_STRUCT_PARAMS_07(__VA_ARGS__)
#define TAPI_HELPER_STRUCT_PARAMS_09(n, t, d, i, to, fr, ...) t n; TAPI_HELPER_STRUCT_PARAMS_08(__VA_ARGS__)
#define TAPI_HELPER_STRUCT_PARAMS_10(n, t, d, i, to, fr, ...) t n; TAPI_HELPER_STRUCT_PARAMS_09(__VA_ARGS__)
#define TAPI_HELPER_STRUCT_PARAMS_11(n, t, d, i, to, fr, ...) t n; TAPI_HELPER_STRUCT_PARAMS_10(__VA_ARGS__)
#define TAPI_HELPER_STRUCT_PARAMS_12(n, t, d, i, to, fr, ...) t n; TAPI_HELPER_STRUCT_PARAMS_11(__VA_ARGS__)
#define TAPI_HELPER_STRUCT_PARAMS_13(n, t, d, i, to, fr, ...) t n; TAPI_HELPER_STRUCT_PARAMS_12(__VA_ARGS__)
#define TAPI_HELPER_STRUCT_PARAMS_14(n, t, d, i, to, fr, ...) t n; TAPI_HELPER_STRUCT_PARAMS_13(__VA_ARGS__)
#define TAPI_HELPER_STRUCT_PARAMS_15(n, t, d, i, to, fr, ...) t n; TAPI_HELPER_STRUCT_PARAMS_14(__VA_ARGS__)
#define TAPI_HELPER_STRUCT_PARAMS_16(n, t, d, i, to, fr, ...) t n; TAPI_HELPER_STRUCT_PARAMS_15(__VA_ARGS__)
#define TAPI_HELPER_STRUCT_PARAMS_17(n, t, d, i, to, fr, ...) t n; TAPI_HELPER_STRUCT_PARAMS_16(__VA_ARGS__)
#define TAPI_HELPER_STRUCT_PARAMS_18(n, t, d, i, to, fr, ...) t n; TAPI_HELPER_STRUCT_PARAMS_17(__VA_ARGS__)
#define TAPI_HELPER_STRUCT_PARAMS_19(n, t, d, i, to, fr, ...) t n; TAPI_HELPER_STRUCT_PARAMS_18(__VA_ARGS__)
#define TAPI_HELPER_STRUCT_PARAMS_20(n, t, d, i, to, fr, ...) t n; TAPI_HELPER_STRUCT_PARAMS_19(__VA_ARGS__)
#define TAPI_HELPER_STRUCT_PARAMS_21(n, t, d, i, to, fr, ...) t n; TAPI_HELPER_STRUCT_PARAMS_20(__VA_ARGS__)
#define TAPI_HELPER_STRUCT_PARAMS_22(n, t, d, i, to, fr, ...) t n; TAPI_HELPER_STRUCT_PARAMS_21(__VA_ARGS__)
#define TAPI_HELPER_STRUCT_PARAMS_23(n, t, d, i, to, fr, ...) t n; TAPI_HELPER_STRUCT_PARAMS_22(__VA_ARGS__)
#define TAPI_HELPER_STRUCT_PARAMS_24(n, t, d, i, to, fr, ...) t n; TAPI_HELPER_STRUCT_PARAMS_23(__VA_ARGS__)
#define TAPI_HELPER_STRUCT_PARAMS_25(n, t, d, i, to, fr, ...) t n; TAPI_HELPER_STRUCT_PARAMS_24(__VA_ARGS__)
#define TAPI_HELPER_STRUCT_PARAMS_26(n, t, d, i, to, fr, ...) t n; TAPI_HELPER_STRUCT_PARAMS_25(__VA_ARGS__)
#define TAPI_HELPER_STRUCT_PARAMS_27(n, t, d, i, to, fr, ...) t n; TAPI_HELPER_STRUCT_PARAMS_26(__VA_ARGS__)
#define TAPI_HELPER_STRUCT_PARAMS_28(n, t, d, i, to, fr, ...) t n; TAPI_HELPER_STRUCT_PARAMS_27(__VA_ARGS__)
#define TAPI_HELPER_STRUCT_PARAMS_29(n, t, d, i, to, fr, ...) t n; TAPI_HELPER_STRUCT_PARAMS_28(__VA_ARGS__)
#define TAPI_HELPER_STRUCT_PARAMS_30(n, t, d, i, to, fr, ...) t n; TAPI_HELPER_STRUCT_PARAMS_29(__VA_ARGS__)
#define TAPI_HELPER_STRUCT_PARAMS_31(n, t, d, i, to, fr, ...) t n; TAPI_HELPER_STRUCT_PARAMS_30(__VA_ARGS__)
#define TAPI_HELPER_STRUCT_PARAMS_32(n, t, d, i, to, fr, ...) t n; TAPI_HELPER_STRUCT_PARAMS_31(__VA_ARGS__)
#define TAPI_HELPER_STRUCT_PARAMS_33(n, t, d, i, to, fr, ...) t n; TAPI_HELPER_STRUCT_PARAMS_32(__VA_ARGS__)
#define TAPI_HELPER_STRUCT_PARAMS_34(n, t, d, i, to, fr, ...) t n; TAPI_HELPER_STRUCT_PARAMS_33(__VA_ARGS__)
#define TAPI_HELPER_STRUCT_PARAMS_35(n, t, d, i, to, fr, ...) t n; TAPI_HELPER_STRUCT_PARAMS_34(__VA_ARGS__)
#define TAPI_HELPER_STRUCT_PARAMS_36(n, t, d, i, to, fr, ...) t n; TAPI_HELPER_STRUCT_PARAMS_35(__VA_ARGS__)
#define TAPI_HELPER_STRUCT_PARAMS_37(n, t, d, i, to, fr, ...) t n; TAPI_HELPER_STRUCT_PARAMS_36(__VA_ARGS__)
#define TAPI_HELPER_STRUCT_PARAMS_38(n, t, d, i, to, fr, ...) t n; TAPI_HELPER_STRUCT_PARAMS_37(__VA_ARGS__)
#define TAPI_HELPER_STRUCT_PARAMS_39(n, t, d, i, to, fr, ...) t n; TAPI_HELPER_STRUCT_PARAMS_38(__VA_ARGS__)
#define TAPI_HELPER_STRUCT_PARAMS_40(n, t, d, i, to, fr, ...) t n; TAPI_HELPER_STRUCT_PARAMS_39(__VA_ARGS__)
#define TAPI_HELPER_STRUCT_PARAMS_41(n, t, d, i, to, fr, ...) t n; TAPI_HELPER_STRUCT_PARAMS_40(__VA_ARGS__)
#define TAPI_HELPER_STRUCT_PARAMS_42(n, t, d, i, to, fr, ...) t n; TAPI_HELPER_STRUCT_PARAMS_41(__VA_ARGS__)
#define TAPI_HELPER_STRUCT_PARAMS_43(n, t, d, i, to, fr, ...) t n; TAPI_HELPER_STRUCT_PARAMS_42(__VA_ARGS__)
#define TAPI_HELPER_STRUCT_PARAMS_44(n, t, d, i, to, fr, ...) t n; TAPI_HELPER_STRUCT_PARAMS_43(__VA_ARGS__)
#define TAPI_HELPER_STRUCT_PARAMS_45(n, t, d, i, to, fr, ...) t n; TAPI_HELPER_STRUCT_PARAMS_44(__VA_ARGS__)
#define TAPI_HELPER_STRUCT_PARAMS_46(n, t, d, i, to, fr, ...) t n; TAPI_HELPER_STRUCT_PARAMS_45(__VA_ARGS__)
#define TAPI_HELPER_STRUCT_PARAMS_47(n, t, d, i, to, fr, ...) t n; TAPI_HELPER_STRUCT_PARAMS_46(__VA_ARGS__)
#define TAPI_HELPER_STRUCT_PARAMS_48(n, t, d, i, to, fr, ...) t n; TAPI_HELPER_STRUCT_PARAMS_47(__VA_ARGS__)
#define TAPI_HELPER_STRUCT_PARAMS_49(n, t, d, i, to, fr, ...) t n; TAPI_HELPER_STRUCT_PARAMS_48(__VA_ARGS__)

#define TAPI_HELPER_STRUCT_CONST_INPUT_00(n, t, d, i, to, fr)      const t& _##n=d
#define TAPI_HELPER_STRUCT_CONST_INPUT_01(n, t, d, i, to, fr, ...) const t& _##n=d, TAPI_HELPER_STRUCT_CONST_INPUT_00(__VA_ARGS__)
#define TAPI_HELPER_STRUCT_CONST_INPUT_02(n, t, d, i, to, fr, ...) const t& _##n=d, TAPI_HELPER_STRUCT_CONST_INPUT_01(__VA_ARGS__)
#define TAPI_HELPER_STRUCT_CONST_INPUT_03(n, t, d, i, to, fr, ...) const t& _##n=d, TAPI_HELPER_STRUCT_CONST_INPUT_02(__VA_ARGS__)
#define TAPI_HELPER_STRUCT_CONST_INPUT_04(n, t, d, i, to, fr, ...) const t& _##n=d, TAPI_HELPER_STRUCT_CONST_INPUT_03(__VA_ARGS__)
#define TAPI_HELPER_STRUCT_CONST_INPUT_05(n, t, d, i, to, fr, ...) const t& _##n=d, TAPI_HELPER_STRUCT_CONST_INPUT_04(__VA_ARGS__)
#define TAPI_HELPER_STRUCT_CONST_INPUT_06(n, t, d, i, to, fr, ...) const t& _##n=d, TAPI_HELPER_STRUCT_CONST_INPUT_05(__VA_ARGS__)
#define TAPI_HELPER_STRUCT_CONST_INPUT_07(n, t, d, i, to, fr, ...) const t& _##n=d, TAPI_HELPER_STRUCT_CONST_INPUT_06(__VA_ARGS__)
#define TAPI_HELPER_STRUCT_CONST_INPUT_08(n, t, d, i, to, fr, ...) const t& _##n=d, TAPI_HELPER_STRUCT_CONST_INPUT_07(__VA_ARGS__)
#define TAPI_HELPER_STRUCT_CONST_INPUT_09(n, t, d, i, to, fr, ...) const t& _##n=d, TAPI_HELPER_STRUCT_CONST_INPUT_08(__VA_ARGS__)
#define TAPI_HELPER_STRUCT_CONST_INPUT_10(n, t, d, i, to, fr, ...) const t& _##n=d, TAPI_HELPER_STRUCT_CONST_INPUT_09(__VA_ARGS__)
#define TAPI_HELPER_STRUCT_CONST_INPUT_11(n, t, d, i, to, fr, ...) const t& _##n=d, TAPI_HELPER_STRUCT_CONST_INPUT_10(__VA_ARGS__)
#define TAPI_HELPER_STRUCT_CONST_INPUT_12(n, t, d, i, to, fr, ...) const t& _##n=d, TAPI_HELPER_STRUCT_CONST_INPUT_11(__VA_ARGS__)
#define TAPI_HELPER_STRUCT_CONST_INPUT_13(n, t, d, i, to, fr, ...) const t& _##n=d, TAPI_HELPER_STRUCT_CONST_INPUT_12(__VA_ARGS__)
#define TAPI_HELPER_STRUCT_CONST_INPUT_14(n, t, d, i, to, fr, ...) const t& _##n=d, TAPI_HELPER_STRUCT_CONST_INPUT_13(__VA_ARGS__)
#define TAPI_HELPER_STRUCT_CONST_INPUT_15(n, t, d, i, to, fr, ...) const t& _##n=d, TAPI_HELPER_STRUCT_CONST_INPUT_14(__VA_ARGS__)
#define TAPI_HELPER_STRUCT_CONST_INPUT_16(n, t, d, i, to, fr, ...) const t& _##n=d, TAPI_HELPER_STRUCT_CONST_INPUT_15(__VA_ARGS__)
#define TAPI_HELPER_STRUCT_CONST_INPUT_17(n, t, d, i, to, fr, ...) const t& _##n=d, TAPI_HELPER_STRUCT_CONST_INPUT_16(__VA_ARGS__)
#define TAPI_HELPER_STRUCT_CONST_INPUT_18(n, t, d, i, to, fr, ...) const t& _##n=d, TAPI_HELPER_STRUCT_CONST_INPUT_17(__VA_ARGS__)
#define TAPI_HELPER_STRUCT_CONST_INPUT_19(n, t, d, i, to, fr, ...) const t& _##n=d, TAPI_HELPER_STRUCT_CONST_INPUT_18(__VA_ARGS__)
#define TAPI_HELPER_STRUCT_CONST_INPUT_20(n, t, d, i, to, fr, ...) const t& _##n=d, TAPI_HELPER_STRUCT_CONST_INPUT_19(__VA_ARGS__)
#define TAPI_HELPER_STRUCT_CONST_INPUT_21(n, t, d, i, to, fr, ...) const t& _##n=d, TAPI_HELPER_STRUCT_CONST_INPUT_20(__VA_ARGS__)
#define TAPI_HELPER_STRUCT_CONST_INPUT_22(n, t, d, i, to, fr, ...) const t& _##n=d, TAPI_HELPER_STRUCT_CONST_INPUT_21(__VA_ARGS__)
#define TAPI_HELPER_STRUCT_CONST_INPUT_23(n, t, d, i, to, fr, ...) const t& _##n=d, TAPI_HELPER_STRUCT_CONST_INPUT_22(__VA_ARGS__)
#define TAPI_HELPER_STRUCT_CONST_INPUT_24(n, t, d, i, to, fr, ...) const t& _##n=d, TAPI_HELPER_STRUCT_CONST_INPUT_23(__VA_ARGS__)
#define TAPI_HELPER_STRUCT_CONST_INPUT_25(n, t, d, i, to, fr, ...) const t& _##n=d, TAPI_HELPER_STRUCT_CONST_INPUT_24(__VA_ARGS__)
#define TAPI_HELPER_STRUCT_CONST_INPUT_26(n, t, d, i, to, fr, ...) const t& _##n=d, TAPI_HELPER_STRUCT_CONST_INPUT_25(__VA_ARGS__)
#define TAPI_HELPER_STRUCT_CONST_INPUT_27(n, t, d, i, to, fr, ...) const t& _##n=d, TAPI_HELPER_STRUCT_CONST_INPUT_26(__VA_ARGS__)
#define TAPI_HELPER_STRUCT_CONST_INPUT_28(n, t, d, i, to, fr, ...) const t& _##n=d, TAPI_HELPER_STRUCT_CONST_INPUT_27(__VA_ARGS__)
#define TAPI_HELPER_STRUCT_CONST_INPUT_29(n, t, d, i, to, fr, ...) const t& _##n=d, TAPI_HELPER_STRUCT_CONST_INPUT_28(__VA_ARGS__)
#define TAPI_HELPER_STRUCT_CONST_INPUT_30(n, t, d, i, to, fr, ...) const t& _##n=d, TAPI_HELPER_STRUCT_CONST_INPUT_29(__VA_ARGS__)
#define TAPI_HELPER_STRUCT_CONST_INPUT_31(n, t, d, i, to, fr, ...) const t& _##n=d, TAPI_HELPER_STRUCT_CONST_INPUT_30(__VA_ARGS__)
#define TAPI_HELPER_STRUCT_CONST_INPUT_32(n, t, d, i, to, fr, ...) const t& _##n=d, TAPI_HELPER_STRUCT_CONST_INPUT_31(__VA_ARGS__)
#define TAPI_HELPER_STRUCT_CONST_INPUT_33(n, t, d, i, to, fr, ...) const t& _##n=d, TAPI_HELPER_STRUCT_CONST_INPUT_32(__VA_ARGS__)
#define TAPI_HELPER_STRUCT_CONST_INPUT_34(n, t, d, i, to, fr, ...) const t& _##n=d, TAPI_HELPER_STRUCT_CONST_INPUT_33(__VA_ARGS__)
#define TAPI_HELPER_STRUCT_CONST_INPUT_35(n, t, d, i, to, fr, ...) const t& _##n=d, TAPI_HELPER_STRUCT_CONST_INPUT_34(__VA_ARGS__)
#define TAPI_HELPER_STRUCT_CONST_INPUT_36(n, t, d, i, to, fr, ...) const t& _##n=d, TAPI_HELPER_STRUCT_CONST_INPUT_35(__VA_ARGS__)
#define TAPI_HELPER_STRUCT_CONST_INPUT_37(n, t, d, i, to, fr, ...) const t& _##n=d, TAPI_HELPER_STRUCT_CONST_INPUT_36(__VA_ARGS__)
#define TAPI_HELPER_STRUCT_CONST_INPUT_38(n, t, d, i, to, fr, ...) const t& _##n=d, TAPI_HELPER_STRUCT_CONST_INPUT_37(__VA_ARGS__)
#define TAPI_HELPER_STRUCT_CONST_INPUT_39(n, t, d, i, to, fr, ...) const t& _##n=d, TAPI_HELPER_STRUCT_CONST_INPUT_38(__VA_ARGS__)
#define TAPI_HELPER_STRUCT_CONST_INPUT_40(n, t, d, i, to, fr, ...) const t& _##n=d, TAPI_HELPER_STRUCT_CONST_INPUT_39(__VA_ARGS__)
#define TAPI_HELPER_STRUCT_CONST_INPUT_41(n, t, d, i, to, fr, ...) const t& _##n=d, TAPI_HELPER_STRUCT_CONST_INPUT_40(__VA_ARGS__)
#define TAPI_HELPER_STRUCT_CONST_INPUT_42(n, t, d, i, to, fr, ...) const t& _##n=d, TAPI_HELPER_STRUCT_CONST_INPUT_41(__VA_ARGS__)
#define TAPI_HELPER_STRUCT_CONST_INPUT_43(n, t, d, i, to, fr, ...) const t& _##n=d, TAPI_HELPER_STRUCT_CONST_INPUT_42(__VA_ARGS__)
#define TAPI_HELPER_STRUCT_CONST_INPUT_44(n, t, d, i, to, fr, ...) const t& _##n=d, TAPI_HELPER_STRUCT_CONST_INPUT_43(__VA_ARGS__)
#define TAPI_HELPER_STRUCT_CONST_INPUT_45(n, t, d, i, to, fr, ...) const t& _##n=d, TAPI_HELPER_STRUCT_CONST_INPUT_44(__VA_ARGS__)
#define TAPI_HELPER_STRUCT_CONST_INPUT_46(n, t, d, i, to, fr, ...) const t& _##n=d, TAPI_HELPER_STRUCT_CONST_INPUT_45(__VA_ARGS__)
#define TAPI_HELPER_STRUCT_CONST_INPUT_47(n, t, d, i, to, fr, ...) const t& _##n=d, TAPI_HELPER_STRUCT_CONST_INPUT_46(__VA_ARGS__)
#define TAPI_HELPER_STRUCT_CONST_INPUT_48(n, t, d, i, to, fr, ...) const t& _##n=d, TAPI_HELPER_STRUCT_CONST_INPUT_47(__VA_ARGS__)
#define TAPI_HELPER_STRUCT_CONST_INPUT_49(n, t, d, i, to, fr, ...) const t& _##n=d, TAPI_HELPER_STRUCT_CONST_INPUT_48(__VA_ARGS__)

#define TAPI_HELPER_STRUCT_CONST_INIT_00(n, t, d, i, to, fr)      n(_##n)
#define TAPI_HELPER_STRUCT_CONST_INIT_01(n, t, d, i, to, fr, ...) n(_##n), TAPI_HELPER_STRUCT_CONST_INIT_00(__VA_ARGS__)
#define TAPI_HELPER_STRUCT_CONST_INIT_02(n, t, d, i, to, fr, ...) n(_##n), TAPI_HELPER_STRUCT_CONST_INIT_01(__VA_ARGS__)
#define TAPI_HELPER_STRUCT_CONST_INIT_03(n, t, d, i, to, fr, ...) n(_##n), TAPI_HELPER_STRUCT_CONST_INIT_02(__VA_ARGS__)
#define TAPI_HELPER_STRUCT_CONST_INIT_04(n, t, d, i, to, fr, ...) n(_##n), TAPI_HELPER_STRUCT_CONST_INIT_03(__VA_ARGS__)
#define TAPI_HELPER_STRUCT_CONST_INIT_05(n, t, d, i, to, fr, ...) n(_##n), TAPI_HELPER_STRUCT_CONST_INIT_04(__VA_ARGS__)
#define TAPI_HELPER_STRUCT_CONST_INIT_06(n, t, d, i, to, fr, ...) n(_##n), TAPI_HELPER_STRUCT_CONST_INIT_05(__VA_ARGS__)
#define TAPI_HELPER_STRUCT_CONST_INIT_07(n, t, d, i, to, fr, ...) n(_##n), TAPI_HELPER_STRUCT_CONST_INIT_06(__VA_ARGS__)
#define TAPI_HELPER_STRUCT_CONST_INIT_08(n, t, d, i, to, fr, ...) n(_##n), TAPI_HELPER_STRUCT_CONST_INIT_07(__VA_ARGS__)
#define TAPI_HELPER_STRUCT_CONST_INIT_09(n, t, d, i, to, fr, ...) n(_##n), TAPI_HELPER_STRUCT_CONST_INIT_08(__VA_ARGS__)
#define TAPI_HELPER_STRUCT_CONST_INIT_10(n, t, d, i, to, fr, ...) n(_##n), TAPI_HELPER_STRUCT_CONST_INIT_09(__VA_ARGS__)
#define TAPI_HELPER_STRUCT_CONST_INIT_11(n, t, d, i, to, fr, ...) n(_##n), TAPI_HELPER_STRUCT_CONST_INIT_10(__VA_ARGS__)
#define TAPI_HELPER_STRUCT_CONST_INIT_12(n, t, d, i, to, fr, ...) n(_##n), TAPI_HELPER_STRUCT_CONST_INIT_11(__VA_ARGS__)
#define TAPI_HELPER_STRUCT_CONST_INIT_13(n, t, d, i, to, fr, ...) n(_##n), TAPI_HELPER_STRUCT_CONST_INIT_12(__VA_ARGS__)
#define TAPI_HELPER_STRUCT_CONST_INIT_14(n, t, d, i, to, fr, ...) n(_##n), TAPI_HELPER_STRUCT_CONST_INIT_13(__VA_ARGS__)
#define TAPI_HELPER_STRUCT_CONST_INIT_15(n, t, d, i, to, fr, ...) n(_##n), TAPI_HELPER_STRUCT_CONST_INIT_14(__VA_ARGS__)
#define TAPI_HELPER_STRUCT_CONST_INIT_16(n, t, d, i, to, fr, ...) n(_##n), TAPI_HELPER_STRUCT_CONST_INIT_15(__VA_ARGS__)
#define TAPI_HELPER_STRUCT_CONST_INIT_17(n, t, d, i, to, fr, ...) n(_##n), TAPI_HELPER_STRUCT_CONST_INIT_16(__VA_ARGS__)
#define TAPI_HELPER_STRUCT_CONST_INIT_18(n, t, d, i, to, fr, ...) n(_##n), TAPI_HELPER_STRUCT_CONST_INIT_17(__VA_ARGS__)
#define TAPI_HELPER_STRUCT_CONST_INIT_19(n, t, d, i, to, fr, ...) n(_##n), TAPI_HELPER_STRUCT_CONST_INIT_18(__VA_ARGS__)
#define TAPI_HELPER_STRUCT_CONST_INIT_20(n, t, d, i, to, fr, ...) n(_##n), TAPI_HELPER_STRUCT_CONST_INIT_19(__VA_ARGS__)
#define TAPI_HELPER_STRUCT_CONST_INIT_21(n, t, d, i, to, fr, ...) n(_##n), TAPI_HELPER_STRUCT_CONST_INIT_20(__VA_ARGS__)
#define TAPI_HELPER_STRUCT_CONST_INIT_22(n, t, d, i, to, fr, ...) n(_##n), TAPI_HELPER_STRUCT_CONST_INIT_21(__VA_ARGS__)
#define TAPI_HELPER_STRUCT_CONST_INIT_23(n, t, d, i, to, fr, ...) n(_##n), TAPI_HELPER_STRUCT_CONST_INIT_22(__VA_ARGS__)
#define TAPI_HELPER_STRUCT_CONST_INIT_24(n, t, d, i, to, fr, ...) n(_##n), TAPI_HELPER_STRUCT_CONST_INIT_23(__VA_ARGS__)
#define TAPI_HELPER_STRUCT_CONST_INIT_25(n, t, d, i, to, fr, ...) n(_##n), TAPI_HELPER_STRUCT_CONST_INIT_24(__VA_ARGS__)
#define TAPI_HELPER_STRUCT_CONST_INIT_26(n, t, d, i, to, fr, ...) n(_##n), TAPI_HELPER_STRUCT_CONST_INIT_25(__VA_ARGS__)
#define TAPI_HELPER_STRUCT_CONST_INIT_27(n, t, d, i, to, fr, ...) n(_##n), TAPI_HELPER_STRUCT_CONST_INIT_26(__VA_ARGS__)
#define TAPI_HELPER_STRUCT_CONST_INIT_28(n, t, d, i, to, fr, ...) n(_##n), TAPI_HELPER_STRUCT_CONST_INIT_27(__VA_ARGS__)
#define TAPI_HELPER_STRUCT_CONST_INIT_29(n, t, d, i, to, fr, ...) n(_##n), TAPI_HELPER_STRUCT_CONST_INIT_28(__VA_ARGS__)
#define TAPI_HELPER_STRUCT_CONST_INIT_30(n, t, d, i, to, fr, ...) n(_##n), TAPI_HELPER_STRUCT_CONST_INIT_29(__VA_ARGS__)
#define TAPI_HELPER_STRUCT_CONST_INIT_31(n, t, d, i, to, fr, ...) n(_##n), TAPI_HELPER_STRUCT_CONST_INIT_30(__VA_ARGS__)
#define TAPI_HELPER_STRUCT_CONST_INIT_32(n, t, d, i, to, fr, ...) n(_##n), TAPI_HELPER_STRUCT_CONST_INIT_31(__VA_ARGS__)
#define TAPI_HELPER_STRUCT_CONST_INIT_33(n, t, d, i, to, fr, ...) n(_##n), TAPI_HELPER_STRUCT_CONST_INIT_32(__VA_ARGS__)
#define TAPI_HELPER_STRUCT_CONST_INIT_34(n, t, d, i, to, fr, ...) n(_##n), TAPI_HELPER_STRUCT_CONST_INIT_33(__VA_ARGS__)
#define TAPI_HELPER_STRUCT_CONST_INIT_35(n, t, d, i, to, fr, ...) n(_##n), TAPI_HELPER_STRUCT_CONST_INIT_34(__VA_ARGS__)
#define TAPI_HELPER_STRUCT_CONST_INIT_36(n, t, d, i, to, fr, ...) n(_##n), TAPI_HELPER_STRUCT_CONST_INIT_35(__VA_ARGS__)
#define TAPI_HELPER_STRUCT_CONST_INIT_37(n, t, d, i, to, fr, ...) n(_##n), TAPI_HELPER_STRUCT_CONST_INIT_36(__VA_ARGS__)
#define TAPI_HELPER_STRUCT_CONST_INIT_38(n, t, d, i, to, fr, ...) n(_##n), TAPI_HELPER_STRUCT_CONST_INIT_37(__VA_ARGS__)
#define TAPI_HELPER_STRUCT_CONST_INIT_39(n, t, d, i, to, fr, ...) n(_##n), TAPI_HELPER_STRUCT_CONST_INIT_38(__VA_ARGS__)
#define TAPI_HELPER_STRUCT_CONST_INIT_40(n, t, d, i, to, fr, ...) n(_##n), TAPI_HELPER_STRUCT_CONST_INIT_39(__VA_ARGS__)
#define TAPI_HELPER_STRUCT_CONST_INIT_41(n, t, d, i, to, fr, ...) n(_##n), TAPI_HELPER_STRUCT_CONST_INIT_40(__VA_ARGS__)
#define TAPI_HELPER_STRUCT_CONST_INIT_42(n, t, d, i, to, fr, ...) n(_##n), TAPI_HELPER_STRUCT_CONST_INIT_41(__VA_ARGS__)
#define TAPI_HELPER_STRUCT_CONST_INIT_43(n, t, d, i, to, fr, ...) n(_##n), TAPI_HELPER_STRUCT_CONST_INIT_42(__VA_ARGS__)
#define TAPI_HELPER_STRUCT_CONST_INIT_44(n, t, d, i, to, fr, ...) n(_##n), TAPI_HELPER_STRUCT_CONST_INIT_43(__VA_ARGS__)
#define TAPI_HELPER_STRUCT_CONST_INIT_45(n, t, d, i, to, fr, ...) n(_##n), TAPI_HELPER_STRUCT_CONST_INIT_44(__VA_ARGS__)
#define TAPI_HELPER_STRUCT_CONST_INIT_46(n, t, d, i, to, fr, ...) n(_##n), TAPI_HELPER_STRUCT_CONST_INIT_45(__VA_ARGS__)
#define TAPI_HELPER_STRUCT_CONST_INIT_47(n, t, d, i, to, fr, ...) n(_##n), TAPI_HELPER_STRUCT_CONST_INIT_46(__VA_ARGS__)
#define TAPI_HELPER_STRUCT_CONST_INIT_48(n, t, d, i, to, fr, ...) n(_##n), TAPI_HELPER_STRUCT_CONST_INIT_47(__VA_ARGS__)
#define TAPI_HELPER_STRUCT_CONST_INIT_49(n, t, d, i, to, fr, ...) n(_##n), TAPI_HELPER_STRUCT_CONST_INIT_48(__VA_ARGS__)

//extern QString toCammel(const QString& _name);
inline QString toCammel(const QString& _name) {
    return _name.mid(0,1).toLower() + _name.mid(1);
}

#define TAPI_HELPER_STRUCT_TOJSON_METHOD(n, i, to) \
    if ([](Q_DECL_UNUSED auto v) -> bool { return i; }(n)) Obj[toCammel(#n)] = [](Q_DECL_UNUSED auto v) -> QJsonValue { return to; }(n);
#define TAPI_HELPER_STRUCT_TOJSON_00(n, t, d, i, to, fr)      TAPI_HELPER_STRUCT_TOJSON_METHOD(n, i, to);
#define TAPI_HELPER_STRUCT_TOJSON_01(n, t, d, i, to, fr, ...) TAPI_HELPER_STRUCT_TOJSON_METHOD(n, i, to); TAPI_HELPER_STRUCT_TOJSON_00(__VA_ARGS__)
#define TAPI_HELPER_STRUCT_TOJSON_02(n, t, d, i, to, fr, ...) TAPI_HELPER_STRUCT_TOJSON_METHOD(n, i, to); TAPI_HELPER_STRUCT_TOJSON_01(__VA_ARGS__)
#define TAPI_HELPER_STRUCT_TOJSON_03(n, t, d, i, to, fr, ...) TAPI_HELPER_STRUCT_TOJSON_METHOD(n, i, to); TAPI_HELPER_STRUCT_TOJSON_02(__VA_ARGS__)
#define TAPI_HELPER_STRUCT_TOJSON_04(n, t, d, i, to, fr, ...) TAPI_HELPER_STRUCT_TOJSON_METHOD(n, i, to); TAPI_HELPER_STRUCT_TOJSON_03(__VA_ARGS__)
#define TAPI_HELPER_STRUCT_TOJSON_05(n, t, d, i, to, fr, ...) TAPI_HELPER_STRUCT_TOJSON_METHOD(n, i, to); TAPI_HELPER_STRUCT_TOJSON_04(__VA_ARGS__)
#define TAPI_HELPER_STRUCT_TOJSON_06(n, t, d, i, to, fr, ...) TAPI_HELPER_STRUCT_TOJSON_METHOD(n, i, to); TAPI_HELPER_STRUCT_TOJSON_05(__VA_ARGS__)
#define TAPI_HELPER_STRUCT_TOJSON_07(n, t, d, i, to, fr, ...) TAPI_HELPER_STRUCT_TOJSON_METHOD(n, i, to); TAPI_HELPER_STRUCT_TOJSON_06(__VA_ARGS__)
#define TAPI_HELPER_STRUCT_TOJSON_08(n, t, d, i, to, fr, ...) TAPI_HELPER_STRUCT_TOJSON_METHOD(n, i, to); TAPI_HELPER_STRUCT_TOJSON_07(__VA_ARGS__)
#define TAPI_HELPER_STRUCT_TOJSON_09(n, t, d, i, to, fr, ...) TAPI_HELPER_STRUCT_TOJSON_METHOD(n, i, to); TAPI_HELPER_STRUCT_TOJSON_08(__VA_ARGS__)
#define TAPI_HELPER_STRUCT_TOJSON_10(n, t, d, i, to, fr, ...) TAPI_HELPER_STRUCT_TOJSON_METHOD(n, i, to); TAPI_HELPER_STRUCT_TOJSON_09(__VA_ARGS__)
#define TAPI_HELPER_STRUCT_TOJSON_11(n, t, d, i, to, fr, ...) TAPI_HELPER_STRUCT_TOJSON_METHOD(n, i, to); TAPI_HELPER_STRUCT_TOJSON_10(__VA_ARGS__)
#define TAPI_HELPER_STRUCT_TOJSON_12(n, t, d, i, to, fr, ...) TAPI_HELPER_STRUCT_TOJSON_METHOD(n, i, to); TAPI_HELPER_STRUCT_TOJSON_11(__VA_ARGS__)
#define TAPI_HELPER_STRUCT_TOJSON_13(n, t, d, i, to, fr, ...) TAPI_HELPER_STRUCT_TOJSON_METHOD(n, i, to); TAPI_HELPER_STRUCT_TOJSON_12(__VA_ARGS__)
#define TAPI_HELPER_STRUCT_TOJSON_14(n, t, d, i, to, fr, ...) TAPI_HELPER_STRUCT_TOJSON_METHOD(n, i, to); TAPI_HELPER_STRUCT_TOJSON_13(__VA_ARGS__)
#define TAPI_HELPER_STRUCT_TOJSON_15(n, t, d, i, to, fr, ...) TAPI_HELPER_STRUCT_TOJSON_METHOD(n, i, to); TAPI_HELPER_STRUCT_TOJSON_14(__VA_ARGS__)
#define TAPI_HELPER_STRUCT_TOJSON_16(n, t, d, i, to, fr, ...) TAPI_HELPER_STRUCT_TOJSON_METHOD(n, i, to); TAPI_HELPER_STRUCT_TOJSON_15(__VA_ARGS__)
#define TAPI_HELPER_STRUCT_TOJSON_17(n, t, d, i, to, fr, ...) TAPI_HELPER_STRUCT_TOJSON_METHOD(n, i, to); TAPI_HELPER_STRUCT_TOJSON_16(__VA_ARGS__)
#define TAPI_HELPER_STRUCT_TOJSON_18(n, t, d, i, to, fr, ...) TAPI_HELPER_STRUCT_TOJSON_METHOD(n, i, to); TAPI_HELPER_STRUCT_TOJSON_17(__VA_ARGS__)
#define TAPI_HELPER_STRUCT_TOJSON_19(n, t, d, i, to, fr, ...) TAPI_HELPER_STRUCT_TOJSON_METHOD(n, i, to); TAPI_HELPER_STRUCT_TOJSON_18(__VA_ARGS__)
#define TAPI_HELPER_STRUCT_TOJSON_20(n, t, d, i, to, fr, ...) TAPI_HELPER_STRUCT_TOJSON_METHOD(n, i, to); TAPI_HELPER_STRUCT_TOJSON_19(__VA_ARGS__)
#define TAPI_HELPER_STRUCT_TOJSON_21(n, t, d, i, to, fr, ...) TAPI_HELPER_STRUCT_TOJSON_METHOD(n, i, to); TAPI_HELPER_STRUCT_TOJSON_20(__VA_ARGS__)
#define TAPI_HELPER_STRUCT_TOJSON_22(n, t, d, i, to, fr, ...) TAPI_HELPER_STRUCT_TOJSON_METHOD(n, i, to); TAPI_HELPER_STRUCT_TOJSON_21(__VA_ARGS__)
#define TAPI_HELPER_STRUCT_TOJSON_23(n, t, d, i, to, fr, ...) TAPI_HELPER_STRUCT_TOJSON_METHOD(n, i, to); TAPI_HELPER_STRUCT_TOJSON_22(__VA_ARGS__)
#define TAPI_HELPER_STRUCT_TOJSON_24(n, t, d, i, to, fr, ...) TAPI_HELPER_STRUCT_TOJSON_METHOD(n, i, to); TAPI_HELPER_STRUCT_TOJSON_23(__VA_ARGS__)
#define TAPI_HELPER_STRUCT_TOJSON_25(n, t, d, i, to, fr, ...) TAPI_HELPER_STRUCT_TOJSON_METHOD(n, i, to); TAPI_HELPER_STRUCT_TOJSON_24(__VA_ARGS__)
#define TAPI_HELPER_STRUCT_TOJSON_26(n, t, d, i, to, fr, ...) TAPI_HELPER_STRUCT_TOJSON_METHOD(n, i, to); TAPI_HELPER_STRUCT_TOJSON_25(__VA_ARGS__)
#define TAPI_HELPER_STRUCT_TOJSON_27(n, t, d, i, to, fr, ...) TAPI_HELPER_STRUCT_TOJSON_METHOD(n, i, to); TAPI_HELPER_STRUCT_TOJSON_26(__VA_ARGS__)
#define TAPI_HELPER_STRUCT_TOJSON_28(n, t, d, i, to, fr, ...) TAPI_HELPER_STRUCT_TOJSON_METHOD(n, i, to); TAPI_HELPER_STRUCT_TOJSON_27(__VA_ARGS__)
#define TAPI_HELPER_STRUCT_TOJSON_29(n, t, d, i, to, fr, ...) TAPI_HELPER_STRUCT_TOJSON_METHOD(n, i, to); TAPI_HELPER_STRUCT_TOJSON_28(__VA_ARGS__)
#define TAPI_HELPER_STRUCT_TOJSON_30(n, t, d, i, to, fr, ...) TAPI_HELPER_STRUCT_TOJSON_METHOD(n, i, to); TAPI_HELPER_STRUCT_TOJSON_29(__VA_ARGS__)
#define TAPI_HELPER_STRUCT_TOJSON_31(n, t, d, i, to, fr, ...) TAPI_HELPER_STRUCT_TOJSON_METHOD(n, i, to); TAPI_HELPER_STRUCT_TOJSON_30(__VA_ARGS__)
#define TAPI_HELPER_STRUCT_TOJSON_32(n, t, d, i, to, fr, ...) TAPI_HELPER_STRUCT_TOJSON_METHOD(n, i, to); TAPI_HELPER_STRUCT_TOJSON_31(__VA_ARGS__)
#define TAPI_HELPER_STRUCT_TOJSON_33(n, t, d, i, to, fr, ...) TAPI_HELPER_STRUCT_TOJSON_METHOD(n, i, to); TAPI_HELPER_STRUCT_TOJSON_32(__VA_ARGS__)
#define TAPI_HELPER_STRUCT_TOJSON_34(n, t, d, i, to, fr, ...) TAPI_HELPER_STRUCT_TOJSON_METHOD(n, i, to); TAPI_HELPER_STRUCT_TOJSON_33(__VA_ARGS__)
#define TAPI_HELPER_STRUCT_TOJSON_35(n, t, d, i, to, fr, ...) TAPI_HELPER_STRUCT_TOJSON_METHOD(n, i, to); TAPI_HELPER_STRUCT_TOJSON_34(__VA_ARGS__)
#define TAPI_HELPER_STRUCT_TOJSON_36(n, t, d, i, to, fr, ...) TAPI_HELPER_STRUCT_TOJSON_METHOD(n, i, to); TAPI_HELPER_STRUCT_TOJSON_35(__VA_ARGS__)
#define TAPI_HELPER_STRUCT_TOJSON_37(n, t, d, i, to, fr, ...) TAPI_HELPER_STRUCT_TOJSON_METHOD(n, i, to); TAPI_HELPER_STRUCT_TOJSON_36(__VA_ARGS__)
#define TAPI_HELPER_STRUCT_TOJSON_38(n, t, d, i, to, fr, ...) TAPI_HELPER_STRUCT_TOJSON_METHOD(n, i, to); TAPI_HELPER_STRUCT_TOJSON_37(__VA_ARGS__)
#define TAPI_HELPER_STRUCT_TOJSON_39(n, t, d, i, to, fr, ...) TAPI_HELPER_STRUCT_TOJSON_METHOD(n, i, to); TAPI_HELPER_STRUCT_TOJSON_38(__VA_ARGS__)
#define TAPI_HELPER_STRUCT_TOJSON_40(n, t, d, i, to, fr, ...) TAPI_HELPER_STRUCT_TOJSON_METHOD(n, i, to); TAPI_HELPER_STRUCT_TOJSON_39(__VA_ARGS__)
#define TAPI_HELPER_STRUCT_TOJSON_41(n, t, d, i, to, fr, ...) TAPI_HELPER_STRUCT_TOJSON_METHOD(n, i, to); TAPI_HELPER_STRUCT_TOJSON_40(__VA_ARGS__)
#define TAPI_HELPER_STRUCT_TOJSON_42(n, t, d, i, to, fr, ...) TAPI_HELPER_STRUCT_TOJSON_METHOD(n, i, to); TAPI_HELPER_STRUCT_TOJSON_41(__VA_ARGS__)
#define TAPI_HELPER_STRUCT_TOJSON_43(n, t, d, i, to, fr, ...) TAPI_HELPER_STRUCT_TOJSON_METHOD(n, i, to); TAPI_HELPER_STRUCT_TOJSON_42(__VA_ARGS__)
#define TAPI_HELPER_STRUCT_TOJSON_44(n, t, d, i, to, fr, ...) TAPI_HELPER_STRUCT_TOJSON_METHOD(n, i, to); TAPI_HELPER_STRUCT_TOJSON_43(__VA_ARGS__)
#define TAPI_HELPER_STRUCT_TOJSON_45(n, t, d, i, to, fr, ...) TAPI_HELPER_STRUCT_TOJSON_METHOD(n, i, to); TAPI_HELPER_STRUCT_TOJSON_44(__VA_ARGS__)
#define TAPI_HELPER_STRUCT_TOJSON_46(n, t, d, i, to, fr, ...) TAPI_HELPER_STRUCT_TOJSON_METHOD(n, i, to); TAPI_HELPER_STRUCT_TOJSON_45(__VA_ARGS__)
#define TAPI_HELPER_STRUCT_TOJSON_47(n, t, d, i, to, fr, ...) TAPI_HELPER_STRUCT_TOJSON_METHOD(n, i, to); TAPI_HELPER_STRUCT_TOJSON_46(__VA_ARGS__)
#define TAPI_HELPER_STRUCT_TOJSON_48(n, t, d, i, to, fr, ...) TAPI_HELPER_STRUCT_TOJSON_METHOD(n, i, to); TAPI_HELPER_STRUCT_TOJSON_47(__VA_ARGS__)
#define TAPI_HELPER_STRUCT_TOJSON_49(n, t, d, i, to, fr, ...) TAPI_HELPER_STRUCT_TOJSON_METHOD(n, i, to); TAPI_HELPER_STRUCT_TOJSON_48(__VA_ARGS__)

//n = _obj.contains(toCammel(#n)) ? [](Q_DECL_UNUSED auto v) -> t { return fr; }(_obj.value(toCammel(#n))) : d
#define TAPI_HELPER_STRUCT_FROMJSON_METHOD(n, t, d, fr) \
    if (_obj.contains(toCammel(#n))) \
        n = [](Q_DECL_UNUSED auto v) -> t { return fr; }(_obj.value(toCammel(#n))); \
    else if (_applyDefaultValues) \
        n = d
#define TAPI_HELPER_STRUCT_FROMJSON_00(n, t, d, i, to, fr)      TAPI_HELPER_STRUCT_FROMJSON_METHOD(n, t, d, fr);
#define TAPI_HELPER_STRUCT_FROMJSON_01(n, t, d, i, to, fr, ...) TAPI_HELPER_STRUCT_FROMJSON_METHOD(n, t, d, fr); TAPI_HELPER_STRUCT_FROMJSON_00(__VA_ARGS__)
#define TAPI_HELPER_STRUCT_FROMJSON_02(n, t, d, i, to, fr, ...) TAPI_HELPER_STRUCT_FROMJSON_METHOD(n, t, d, fr); TAPI_HELPER_STRUCT_FROMJSON_01(__VA_ARGS__)
#define TAPI_HELPER_STRUCT_FROMJSON_03(n, t, d, i, to, fr, ...) TAPI_HELPER_STRUCT_FROMJSON_METHOD(n, t, d, fr); TAPI_HELPER_STRUCT_FROMJSON_02(__VA_ARGS__)
#define TAPI_HELPER_STRUCT_FROMJSON_04(n, t, d, i, to, fr, ...) TAPI_HELPER_STRUCT_FROMJSON_METHOD(n, t, d, fr); TAPI_HELPER_STRUCT_FROMJSON_03(__VA_ARGS__)
#define TAPI_HELPER_STRUCT_FROMJSON_05(n, t, d, i, to, fr, ...) TAPI_HELPER_STRUCT_FROMJSON_METHOD(n, t, d, fr); TAPI_HELPER_STRUCT_FROMJSON_04(__VA_ARGS__)
#define TAPI_HELPER_STRUCT_FROMJSON_06(n, t, d, i, to, fr, ...) TAPI_HELPER_STRUCT_FROMJSON_METHOD(n, t, d, fr); TAPI_HELPER_STRUCT_FROMJSON_05(__VA_ARGS__)
#define TAPI_HELPER_STRUCT_FROMJSON_07(n, t, d, i, to, fr, ...) TAPI_HELPER_STRUCT_FROMJSON_METHOD(n, t, d, fr); TAPI_HELPER_STRUCT_FROMJSON_06(__VA_ARGS__)
#define TAPI_HELPER_STRUCT_FROMJSON_08(n, t, d, i, to, fr, ...) TAPI_HELPER_STRUCT_FROMJSON_METHOD(n, t, d, fr); TAPI_HELPER_STRUCT_FROMJSON_07(__VA_ARGS__)
#define TAPI_HELPER_STRUCT_FROMJSON_09(n, t, d, i, to, fr, ...) TAPI_HELPER_STRUCT_FROMJSON_METHOD(n, t, d, fr); TAPI_HELPER_STRUCT_FROMJSON_08(__VA_ARGS__)
#define TAPI_HELPER_STRUCT_FROMJSON_10(n, t, d, i, to, fr, ...) TAPI_HELPER_STRUCT_FROMJSON_METHOD(n, t, d, fr); TAPI_HELPER_STRUCT_FROMJSON_09(__VA_ARGS__)
#define TAPI_HELPER_STRUCT_FROMJSON_11(n, t, d, i, to, fr, ...) TAPI_HELPER_STRUCT_FROMJSON_METHOD(n, t, d, fr); TAPI_HELPER_STRUCT_FROMJSON_10(__VA_ARGS__)
#define TAPI_HELPER_STRUCT_FROMJSON_12(n, t, d, i, to, fr, ...) TAPI_HELPER_STRUCT_FROMJSON_METHOD(n, t, d, fr); TAPI_HELPER_STRUCT_FROMJSON_11(__VA_ARGS__)
#define TAPI_HELPER_STRUCT_FROMJSON_13(n, t, d, i, to, fr, ...) TAPI_HELPER_STRUCT_FROMJSON_METHOD(n, t, d, fr); TAPI_HELPER_STRUCT_FROMJSON_12(__VA_ARGS__)
#define TAPI_HELPER_STRUCT_FROMJSON_14(n, t, d, i, to, fr, ...) TAPI_HELPER_STRUCT_FROMJSON_METHOD(n, t, d, fr); TAPI_HELPER_STRUCT_FROMJSON_13(__VA_ARGS__)
#define TAPI_HELPER_STRUCT_FROMJSON_15(n, t, d, i, to, fr, ...) TAPI_HELPER_STRUCT_FROMJSON_METHOD(n, t, d, fr); TAPI_HELPER_STRUCT_FROMJSON_14(__VA_ARGS__)
#define TAPI_HELPER_STRUCT_FROMJSON_16(n, t, d, i, to, fr, ...) TAPI_HELPER_STRUCT_FROMJSON_METHOD(n, t, d, fr); TAPI_HELPER_STRUCT_FROMJSON_15(__VA_ARGS__)
#define TAPI_HELPER_STRUCT_FROMJSON_17(n, t, d, i, to, fr, ...) TAPI_HELPER_STRUCT_FROMJSON_METHOD(n, t, d, fr); TAPI_HELPER_STRUCT_FROMJSON_16(__VA_ARGS__)
#define TAPI_HELPER_STRUCT_FROMJSON_18(n, t, d, i, to, fr, ...) TAPI_HELPER_STRUCT_FROMJSON_METHOD(n, t, d, fr); TAPI_HELPER_STRUCT_FROMJSON_17(__VA_ARGS__)
#define TAPI_HELPER_STRUCT_FROMJSON_19(n, t, d, i, to, fr, ...) TAPI_HELPER_STRUCT_FROMJSON_METHOD(n, t, d, fr); TAPI_HELPER_STRUCT_FROMJSON_18(__VA_ARGS__)
#define TAPI_HELPER_STRUCT_FROMJSON_20(n, t, d, i, to, fr, ...) TAPI_HELPER_STRUCT_FROMJSON_METHOD(n, t, d, fr); TAPI_HELPER_STRUCT_FROMJSON_19(__VA_ARGS__)
#define TAPI_HELPER_STRUCT_FROMJSON_21(n, t, d, i, to, fr, ...) TAPI_HELPER_STRUCT_FROMJSON_METHOD(n, t, d, fr); TAPI_HELPER_STRUCT_FROMJSON_20(__VA_ARGS__)
#define TAPI_HELPER_STRUCT_FROMJSON_22(n, t, d, i, to, fr, ...) TAPI_HELPER_STRUCT_FROMJSON_METHOD(n, t, d, fr); TAPI_HELPER_STRUCT_FROMJSON_21(__VA_ARGS__)
#define TAPI_HELPER_STRUCT_FROMJSON_23(n, t, d, i, to, fr, ...) TAPI_HELPER_STRUCT_FROMJSON_METHOD(n, t, d, fr); TAPI_HELPER_STRUCT_FROMJSON_22(__VA_ARGS__)
#define TAPI_HELPER_STRUCT_FROMJSON_24(n, t, d, i, to, fr, ...) TAPI_HELPER_STRUCT_FROMJSON_METHOD(n, t, d, fr); TAPI_HELPER_STRUCT_FROMJSON_23(__VA_ARGS__)
#define TAPI_HELPER_STRUCT_FROMJSON_25(n, t, d, i, to, fr, ...) TAPI_HELPER_STRUCT_FROMJSON_METHOD(n, t, d, fr); TAPI_HELPER_STRUCT_FROMJSON_24(__VA_ARGS__)
#define TAPI_HELPER_STRUCT_FROMJSON_26(n, t, d, i, to, fr, ...) TAPI_HELPER_STRUCT_FROMJSON_METHOD(n, t, d, fr); TAPI_HELPER_STRUCT_FROMJSON_25(__VA_ARGS__)
#define TAPI_HELPER_STRUCT_FROMJSON_27(n, t, d, i, to, fr, ...) TAPI_HELPER_STRUCT_FROMJSON_METHOD(n, t, d, fr); TAPI_HELPER_STRUCT_FROMJSON_26(__VA_ARGS__)
#define TAPI_HELPER_STRUCT_FROMJSON_28(n, t, d, i, to, fr, ...) TAPI_HELPER_STRUCT_FROMJSON_METHOD(n, t, d, fr); TAPI_HELPER_STRUCT_FROMJSON_27(__VA_ARGS__)
#define TAPI_HELPER_STRUCT_FROMJSON_29(n, t, d, i, to, fr, ...) TAPI_HELPER_STRUCT_FROMJSON_METHOD(n, t, d, fr); TAPI_HELPER_STRUCT_FROMJSON_28(__VA_ARGS__)
#define TAPI_HELPER_STRUCT_FROMJSON_30(n, t, d, i, to, fr, ...) TAPI_HELPER_STRUCT_FROMJSON_METHOD(n, t, d, fr); TAPI_HELPER_STRUCT_FROMJSON_29(__VA_ARGS__)
#define TAPI_HELPER_STRUCT_FROMJSON_31(n, t, d, i, to, fr, ...) TAPI_HELPER_STRUCT_FROMJSON_METHOD(n, t, d, fr); TAPI_HELPER_STRUCT_FROMJSON_30(__VA_ARGS__)
#define TAPI_HELPER_STRUCT_FROMJSON_32(n, t, d, i, to, fr, ...) TAPI_HELPER_STRUCT_FROMJSON_METHOD(n, t, d, fr); TAPI_HELPER_STRUCT_FROMJSON_31(__VA_ARGS__)
#define TAPI_HELPER_STRUCT_FROMJSON_33(n, t, d, i, to, fr, ...) TAPI_HELPER_STRUCT_FROMJSON_METHOD(n, t, d, fr); TAPI_HELPER_STRUCT_FROMJSON_32(__VA_ARGS__)
#define TAPI_HELPER_STRUCT_FROMJSON_34(n, t, d, i, to, fr, ...) TAPI_HELPER_STRUCT_FROMJSON_METHOD(n, t, d, fr); TAPI_HELPER_STRUCT_FROMJSON_33(__VA_ARGS__)
#define TAPI_HELPER_STRUCT_FROMJSON_35(n, t, d, i, to, fr, ...) TAPI_HELPER_STRUCT_FROMJSON_METHOD(n, t, d, fr); TAPI_HELPER_STRUCT_FROMJSON_34(__VA_ARGS__)
#define TAPI_HELPER_STRUCT_FROMJSON_36(n, t, d, i, to, fr, ...) TAPI_HELPER_STRUCT_FROMJSON_METHOD(n, t, d, fr); TAPI_HELPER_STRUCT_FROMJSON_35(__VA_ARGS__)
#define TAPI_HELPER_STRUCT_FROMJSON_37(n, t, d, i, to, fr, ...) TAPI_HELPER_STRUCT_FROMJSON_METHOD(n, t, d, fr); TAPI_HELPER_STRUCT_FROMJSON_36(__VA_ARGS__)
#define TAPI_HELPER_STRUCT_FROMJSON_38(n, t, d, i, to, fr, ...) TAPI_HELPER_STRUCT_FROMJSON_METHOD(n, t, d, fr); TAPI_HELPER_STRUCT_FROMJSON_37(__VA_ARGS__)
#define TAPI_HELPER_STRUCT_FROMJSON_39(n, t, d, i, to, fr, ...) TAPI_HELPER_STRUCT_FROMJSON_METHOD(n, t, d, fr); TAPI_HELPER_STRUCT_FROMJSON_38(__VA_ARGS__)
#define TAPI_HELPER_STRUCT_FROMJSON_40(n, t, d, i, to, fr, ...) TAPI_HELPER_STRUCT_FROMJSON_METHOD(n, t, d, fr); TAPI_HELPER_STRUCT_FROMJSON_39(__VA_ARGS__)
#define TAPI_HELPER_STRUCT_FROMJSON_41(n, t, d, i, to, fr, ...) TAPI_HELPER_STRUCT_FROMJSON_METHOD(n, t, d, fr); TAPI_HELPER_STRUCT_FROMJSON_40(__VA_ARGS__)
#define TAPI_HELPER_STRUCT_FROMJSON_42(n, t, d, i, to, fr, ...) TAPI_HELPER_STRUCT_FROMJSON_METHOD(n, t, d, fr); TAPI_HELPER_STRUCT_FROMJSON_41(__VA_ARGS__)
#define TAPI_HELPER_STRUCT_FROMJSON_43(n, t, d, i, to, fr, ...) TAPI_HELPER_STRUCT_FROMJSON_METHOD(n, t, d, fr); TAPI_HELPER_STRUCT_FROMJSON_42(__VA_ARGS__)
#define TAPI_HELPER_STRUCT_FROMJSON_44(n, t, d, i, to, fr, ...) TAPI_HELPER_STRUCT_FROMJSON_METHOD(n, t, d, fr); TAPI_HELPER_STRUCT_FROMJSON_43(__VA_ARGS__)
#define TAPI_HELPER_STRUCT_FROMJSON_45(n, t, d, i, to, fr, ...) TAPI_HELPER_STRUCT_FROMJSON_METHOD(n, t, d, fr); TAPI_HELPER_STRUCT_FROMJSON_44(__VA_ARGS__)
#define TAPI_HELPER_STRUCT_FROMJSON_46(n, t, d, i, to, fr, ...) TAPI_HELPER_STRUCT_FROMJSON_METHOD(n, t, d, fr); TAPI_HELPER_STRUCT_FROMJSON_45(__VA_ARGS__)
#define TAPI_HELPER_STRUCT_FROMJSON_47(n, t, d, i, to, fr, ...) TAPI_HELPER_STRUCT_FROMJSON_METHOD(n, t, d, fr); TAPI_HELPER_STRUCT_FROMJSON_46(__VA_ARGS__)
#define TAPI_HELPER_STRUCT_FROMJSON_48(n, t, d, i, to, fr, ...) TAPI_HELPER_STRUCT_FROMJSON_METHOD(n, t, d, fr); TAPI_HELPER_STRUCT_FROMJSON_47(__VA_ARGS__)
#define TAPI_HELPER_STRUCT_FROMJSON_49(n, t, d, i, to, fr, ...) TAPI_HELPER_STRUCT_FROMJSON_METHOD(n, t, d, fr); TAPI_HELPER_STRUCT_FROMJSON_48(__VA_ARGS__)

#define TAPI_HELPER_STRUCT_PARAMS(...) \
    TAPI_HELPER_STRUCT_MACRO_SELECTOR(TAPI_HELPER_STRUCT_PARAMS, __VA_ARGS__)(__VA_ARGS__)

#define TAPI_HELPER_STRUCT_CONST_INPUT(...) \
    TAPI_HELPER_STRUCT_MACRO_SELECTOR(TAPI_HELPER_STRUCT_CONST_INPUT, __VA_ARGS__)(__VA_ARGS__)

#define TAPI_HELPER_STRUCT_CONST_INIT(...) \
    TAPI_HELPER_STRUCT_MACRO_SELECTOR(TAPI_HELPER_STRUCT_CONST_INIT, __VA_ARGS__)(__VA_ARGS__)

#define TAPI_HELPER_STRUCT_TOJSON(...) \
    TAPI_HELPER_STRUCT_MACRO_SELECTOR(TAPI_HELPER_STRUCT_TOJSON, __VA_ARGS__)(__VA_ARGS__)

#define TAPI_HELPER_STRUCT_FROMJSON(...) \
    TAPI_HELPER_STRUCT_MACRO_SELECTOR( TAPI_HELPER_STRUCT_FROMJSON, __VA_ARGS__)(__VA_ARGS__)

/**********************************************************/
#define TAPI_REGISTER_METATYPE_FULL(_complexity, _namespace, _type, _fnToVariant, _fnFromVariant, _fnDescription, _fnToORMValue, _fnFromORMValue, _fnOptions) \
    namespace Targoman::API { \
        template <> std::function<QVariant(_namespace::_type _value)> tmplAPIArg<_namespace::_type, _complexity, false>::fnToVariant = _fnToVariant; \
        template <> std::function<_namespace::_type(QVariant _value, const QByteArray& _paramName)> tmplAPIArg<_namespace::_type, _complexity, false>::fnFromVariant = _fnFromVariant; \
        template <> std::function<QString(const QList<DBM::clsORMField>& _allFields)> tmplAPIArg<_namespace::_type, _complexity, false>::fnDescription = _fnDescription; \
        template <> std::function<QVariant(const QVariant&)> tmplAPIArg<_namespace::_type, _complexity, false>::fnToORMValue = _fnToORMValue; \
        template <> std::function<QVariant(const QVariant&)> tmplAPIArg<_namespace::_type, _complexity, false>::fnFromORMValue = _fnFromORMValue; \
        template <> std::function<QStringList()> tmplAPIArg<_namespace::_type, _complexity, false>::fnOptions = _fnOptions; \
        template <> std::function<QVariant(NULLABLE_TYPE(_namespace::_type) _value)> tmplAPIArg<NULLABLE_TYPE(_namespace::_type), _complexity, true>::fnToVariant = \
            [](NULLABLE_TYPE(_namespace::_type) _value) {return NULLABLE_IS_NULL(_value) ? QVariant() : tmplAPIArg<_namespace::_type, _complexity, false>::toVariant(*_value);}; \
        template <> std::function<NULLABLE_TYPE(_namespace::_type)(QVariant _value, const QByteArray& _paramName)> tmplAPIArg<NULLABLE_TYPE(_namespace::_type), _complexity, true>::fnFromVariant = \
            [](const QVariant& _value, const QByteArray& _paramName) -> NULLABLE_TYPE(_namespace::_type) { \
                if (!_value.isValid() || _value.isNull()) \
                    return NULLABLE_TYPE(_namespace::_type)(); \
                NULLABLE_VAR(_namespace::_type, Value); \
                Value = tmplAPIArg<_namespace::_type, _complexity, false>::fromVariant(_value, _paramName); \
                return Value; \
            }; \
        template <> std::function<QVariant(const QVariant&)> tmplAPIArg<NULLABLE_TYPE(_namespace::_type), _complexity, true>::fnToORMValue = _fnToORMValue; \
        template <> std::function<QVariant(const QVariant&)> tmplAPIArg<NULLABLE_TYPE(_namespace::_type), _complexity, true>::fnFromORMValue = _fnFromORMValue; \
        template <> std::function<QStringList()> tmplAPIArg<NULLABLE_TYPE(_namespace::_type), _complexity, true>::fnOptions = _fnOptions; \
        template <> std::function<QString(const QList<DBM::clsORMField>& _allFields)> tmplAPIArg<NULLABLE_TYPE(_namespace::_type), _complexity, true>::fnDescription = _fnDescription; \
    } \
    namespace _namespace { \
        using namespace Targoman::API; \
        static tmplAPIArg<_type, _complexity, false>* Dangling_##_type = tmplAPIArg<_type, _complexity, false>::instance(TARGOMAN_M2STR(_namespace::_type)); \
        static tmplAPIArg<NULLABLE_TYPE(_type), _complexity, true>* Dangling_QSP_##_type = tmplAPIArg<NULLABLE_TYPE(_type), _complexity, true>::instance(NULLABLE_M2STR(_namespace::_type)); \
    }
//            NULLABLE_TYPE(_namespace::_type) Value(new _namespace::_type);

#define TAPI_REGISTER_METATYPE_FromORM(_complexity, _namespace, _type, _fnToVariant, _fnFromVariant, _fnDescription, _fnToORMValue, _fnFromORMValue) \
    TAPI_REGISTER_METATYPE_FULL(_complexity, _namespace, _type, _fnToVariant, _fnFromVariant, _fnDescription, _fnToORMValue, _fnFromORMValue, nullptr)

#define TAPI_REGISTER_METATYPE_ToORM(_complexity, _namespace, _type, _fnToVariant, _fnFromVariant, _fnDescription, _fnToORMValue) \
    TAPI_REGISTER_METATYPE_FromORM(_complexity, _namespace, _type, _fnToVariant, _fnFromVariant, _fnDescription, _fnToORMValue, nullptr)

#define TAPI_REGISTER_METATYPE_Desc(_complexity, _namespace, _type, _fnToVariant, _fnFromVariant, _fnDescription) \
    TAPI_REGISTER_METATYPE_ToORM(_complexity, _namespace, _type, _fnToVariant, _fnFromVariant, _fnDescription, nullptr)

#define TAPI_REGISTER_METATYPE_FromVariant(_complexity, _namespace, _type, _fnToVariant, _fnFromVariant) \
    TAPI_REGISTER_METATYPE_Desc(_complexity, _namespace, _type, _fnToVariant, _fnFromVariant, nullptr)

#define TAPI_REGISTER_METATYPE_ToVariant(_complexity, _namespace, _type, _fnToVariant) \
    TAPI_REGISTER_METATYPE_FromVariant(_complexity, _namespace, _type, _fnToVariant, nullptr)

#define TAPI_REGISTER_METATYPE_MACRO_ARG_COUNT(_01,_02,_03,_04,_05,_06, N ,...) N

#define TAPI_REGISTER_METATYPE_MACRO_SELECTOR(_fn,...) \
    TAPI_REGISTER_METATYPE_MACRO_ARG_COUNT(__VA_ARGS__, _fn##FULL,_fn##FromORM,_fn##ToORM,_fn##Desc,_fn##FromVariant,_fn##ToVariant, None)

#define TAPI_REGISTER_TARGOMAN_ENUM_IMPL(_namespace, _enum, _fnToVariant, _fnFromVariant, _fnDescription, _fnToORMValue, _fnFromORMValue, _fnOptions) \
    namespace Targoman::API { \
        template <> std::function<QVariant(_namespace::_enum::Type _value)> tmplAPIArg<_namespace::_enum::Type, COMPLEXITY_Enum, false>::fnToVariant = _fnToVariant; \
        template <> std::function<_namespace::_enum::Type(QVariant _value, const QByteArray& _paramName)> tmplAPIArg<_namespace::_enum::Type, COMPLEXITY_Enum, false>::fnFromVariant = _fnFromVariant; \
        template <> std::function<QVariant(const QVariant&)> tmplAPIArg<_namespace::_enum::Type, COMPLEXITY_Enum, false>::fnToORMValue = _fnToORMValue; \
        template <> std::function<QVariant(const QVariant&)> tmplAPIArg<_namespace::_enum::Type, COMPLEXITY_Enum, false>::fnFromORMValue = _fnFromORMValue; \
        template <> std::function<QStringList()> tmplAPIArg<_namespace::_enum::Type, COMPLEXITY_Enum, false>::fnOptions = _fnOptions; \
        template <> std::function<QString(const QList<DBM::clsORMField>& _allFields)> tmplAPIArg<_namespace::_enum::Type, COMPLEXITY_Enum, false>::fnDescription = _fnDescription; \
        template <> std::function<QVariant(NULLABLE_TYPE(_namespace::_enum::Type) _value)> tmplAPIArg<NULLABLE_TYPE(_namespace::_enum::Type), COMPLEXITY_Enum, true>::fnToVariant = \
            [](NULLABLE_TYPE(_namespace::_enum::Type) _value) {return NULLABLE_IS_NULL(_value) ? QVariant() : tmplAPIArg<_namespace::_enum::Type, COMPLEXITY_Enum, false>::toVariant(*_value);}; \
        template <> std::function<NULLABLE_TYPE(_namespace::_enum::Type)(QVariant _value, const QByteArray& _paramName)> tmplAPIArg<NULLABLE_TYPE(_namespace::_enum::Type), COMPLEXITY_Enum, true>::fnFromVariant = \
            [](const QVariant& _value, const QByteArray& _paramName) -> NULLABLE_TYPE(_namespace::_enum::Type) { \
                if (!_value.isValid() || _value.isNull()) return NULLABLE_TYPE(_namespace::_enum::Type)(); \
                NULLABLE_VAR(_namespace::_enum::Type, Value); \
                Value = tmplAPIArg<_namespace::_enum::Type, COMPLEXITY_Enum, false>::fromVariant(_value, _paramName); \
                return Value; \
            }; \
        template <> std::function<QVariant(const QVariant&)> tmplAPIArg<NULLABLE_TYPE(_namespace::_enum::Type), COMPLEXITY_Enum, true>::fnToORMValue = _fnToORMValue; \
        template <> std::function<QVariant(const QVariant&)> tmplAPIArg<NULLABLE_TYPE(_namespace::_enum::Type), COMPLEXITY_Enum, true>::fnFromORMValue = _fnFromORMValue; \
        template <> std::function<QStringList()> tmplAPIArg<NULLABLE_TYPE(_namespace::_enum::Type), COMPLEXITY_Enum, true>::fnOptions = _fnOptions; \
        template <> std::function<QString(const QList<DBM::clsORMField>& _allFields)> tmplAPIArg<NULLABLE_TYPE(_namespace::_enum::Type), COMPLEXITY_Enum, true>::fnDescription = _fnDescription; \
    } \
    namespace _namespace { \
        using namespace Targoman::API; \
        static tmplAPIArg<_enum::Type, COMPLEXITY_Enum, false>* Dangling_##_enum = tmplAPIArg<_enum::Type, COMPLEXITY_Enum, false>::instance(TARGOMAN_M2STR(_namespace::_enum::Type)); \
        static tmplAPIArg<NULLABLE_TYPE(_enum::Type), COMPLEXITY_Enum, true>* Dangling_QSP_##_enum = tmplAPIArg<NULLABLE_TYPE(_enum::Type), COMPLEXITY_Enum, true>::instance(NULLABLE_M2STR(_namespace::_enum::Type)); \
    }
//NULLABLE_TYPE(_namespace::_enum::Type) Value(new _namespace::_enum::Type);

/************************************************************/
#define INTERNAL_TAPI_ADD_TYPE_SPECIALFROMVARIANT(_baseType, _typeName, _fnSetFromVariant) \
    class _typeName : public _baseType { \
    public: \
        _typeName() { ; } \
        _typeName(const _baseType& _other) :_baseType(_other) { ; } \
        void customFromVariant(const QVariant& _value, const QString &_paramName = {}) { _fnSetFromVariant(_value, _paramName); } \
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
        /* fnToVariant        */ [](_namespace::_enum::Type _value) -> QVariant { \
            return _namespace::_enum::toStr(_value); \
        }, \
        /* fnFromVariant      */ [](const QVariant& _value, Q_DECL_UNUSED const QString& _paramName = "") -> _namespace::_enum::Type { \
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
        /* fnDescription  */ [](const QList<Targoman::API::DBM::clsORMField>&) -> QString { \
            return QString("One of (%1)").arg(_namespace::_enum::options().join('|')); \
        }, \
        /* fnToORMValue   */ [](const QVariant& _value) -> QVariant { \
            return _namespace::_enum::toStr(static_cast<_namespace::_enum::Type>(_value.toString().toLatin1().at(0))); \
        }, \
        /* fnFromORMValue */ [](const QVariant& _value) -> QVariant { \
            QVariant _val = (_value.userType() == QMetaType::QString ? _value : _namespace::_enum::toStr(_value.value<_namespace::_enum::Type>())); \
            if (_namespace::_enum::options().contains(_val.toString()) == false) \
                throw exHTTPBadRequest(QString("%1 is not a valid %2.") \
                    .arg(_val.toString()) \
                    .arg(/*QString(TARGOMAN_M2STR(_enum)).startsWith("enu") ? QString(TARGOMAN_M2STR(_enum)).mid(3) :*/ QString(TARGOMAN_M2STR(_enum))) \
                ); \
            return QString(_namespace::_enum::toEnum(_val.toString())); \
        }, \
        /* fnOptions      */ []() -> QStringList { return _namespace::_enum::options(); } \
    )

/************************************************************/
#define INTERNAL_C2DBL(v) static_cast<double>(v)

#define INTERNAL_V2DBL(v) v.toDouble()
//#define INTERNAL_V2double(v) INTERNAL_V2DBL(v)

#define INTERNAL_C2bool(v)          static_cast<bool>(v)
//static_cast<bool>(v.toDouble())
#define INTERNAL_V2bool(v)          [](auto v) -> bool { return ((v == true) || (v == 1) || (v == "1")); }(v)
#define INTERNAL_N2bool(_value)     INTERNAL_NULLABLE_FROM_JSONVALUE_TO_BOOL(_value)

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
#define INTERNAL_SF_NULLABLE_INTEGRAL_1(_def)             INTERNAL_SF_NULLABLE_INTEGRAL_2(_def, v!=NULLABLE_NULL_VALUE)
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
#define INTERNAL_NULLABLE_FROM_JSONVALUE_TO_BOOL(_value) \
    [](auto v) -> NULLABLE_TYPE(bool) { \
        if (v.isNull()) \
            return NULLABLE_NULL_VALUE; \
        auto Storage = NULLABLE_TYPE(bool)(); \
        Storage = INTERNAL_V2bool(v); \
        return Storage; \
    } (_value)

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
