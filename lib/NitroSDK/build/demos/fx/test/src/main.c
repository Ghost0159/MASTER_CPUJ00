/*---------------------------------------------------------------------------*
  Project:  NitroSDK - FX - demos - test
  File:     main.c

  Copyright 2003-2006 Nintendo.  All rights reserved.

  These coded instructions, statements, and computer programs contain
  proprietary information of Nintendo of America Inc. and/or Nintendo
  Company Ltd., and are protected by Federal copyright law.  They may
  not be disclosed to third parties or copied or duplicated in any form,
  in whole or in part, without the prior written consent of Nintendo.

  $Log: main.c,v $
  Revision 1.14  2006/01/18 02:11:21  kitase_hirotake
  do-indent

  Revision 1.13  2005/02/28 05:26:13  yosizaki
  do-indent.

  Revision 1.12  2004/06/08 00:15:43  yada
  add '#pragma unused()' for FINALROM

  Revision 1.11  2004/04/07 01:27:57  yada
  fix header comment

  Revision 1.10  2004/02/10 09:49:57  nishida_kenji
  Improve some tests.

  Revision 1.9  2004/02/05 07:09:02  yasu
  change SDK prefix iris -> nitro

  Revision 1.8  2004/01/29 01:35:09  kitani_toshikazu
  Fix some bugs for debug build, restore appended test cases.

  Revision 1.7  2004/01/27 04:41:47  kitani_toshikazu
  Removed new tests for a while

  Revision 1.6  2004/01/27 01:35:40  kitani_toshikazu
  Appended some test cases

  Revision 1.4  2004/01/18 04:40:35  nishida_kenji
  revises the interfaces for asynchronous calculation

  Revision 1.3  2004/01/17 07:06:24  nishida_kenji
  add MTX_MultVec33

  Revision 1.2  2004/01/16 10:41:50  nishida_kenji
  tests the sin/cos table

  Revision 1.1  2003/12/12 08:11:15  nishida_kenji
  testing fixed point library

  $NoKeywords: $
 *---------------------------------------------------------------------------*/

#include <nitro.h>
#include <math.h>

#ifndef  FX_TEST_COMMON_H_
#include "fx_test_common.h"
#endif //  FX_TEST_COMMON_H_

#ifndef  FX_APPEND_VEC_H_
#include "fx_append_vec.h"
#endif //  FX_APPEND_VEC_H_

#ifndef  FX_APPEND_MTX_H_
#include "fx_append_mtx.h"
#endif //  FX_APPEND_MTX_H_


extern const fx16 testTable[];





static void test_FX_VEC()
{
    VecFx32 a = { FX32_ONE, FX32_ONE * 2, FX32_ONE * 3 };
    VecFx32 b = { FX32_ONE * 5, FX32_ONE * 7, FX32_ONE * 11 };
    VecFx32 c;
    fx32    tmp;

    OS_Printf("----- testing fx_vec.h -----\n");

    VEC_Add(&a, &b, &c);
    SDK_ASSERTMSG(c.x == 6 * FX32_ONE && c.y == 9 * FX32_ONE && c.z == 14 * FX32_ONE, "VEC_Add");

    VEC_Subtract(&a, &b, &c);
    SDK_ASSERTMSG(c.x == -4 * FX32_ONE && c.y == -5 * FX32_ONE
                  && c.z == -8 * FX32_ONE, "VEC_Subtract");

    tmp = VEC_DotProduct(&a, &b);
    SDK_ASSERTMSG(tmp == 52 * FX32_ONE, "VEC_DotProduct %d", tmp);

    VEC_CrossProduct(&a, &b, &c);
    SDK_ASSERTMSG(c.x == FX32_ONE && c.y == 4 * FX32_ONE
                  && c.z == -3 * FX32_ONE, "VEC_CrossProduct");

    tmp = VEC_Mag(&a);
    SDK_ASSERTMSG(tmp == 15326, "VEC_Mag");


    VEC_Normalize(&a, &c);
    SDK_ASSERTMSG(c.x == 1095 &&
                  c.y == 2189 && c.z == 3284, "VEC_Normalize: %d %d %d", c.x, c.y, c.z);

    OS_Printf(" done.\n");
}



static void test_FX_TRIG()
{

    OS_Printf("----- testing fx_trig.h -----\n");

    {
        fx32    tmp = FX_DEG_TO_RAD(180 * FX32_ONE);
        SDK_ASSERTMSG(tmp == 12868, "FX_DEG_TO_RAD");
    }

    {
        int     tmp = FX_DEG_TO_IDX(180 * FX32_ONE);
        SDK_ASSERTMSG(tmp == 32768, "FX_DEG_TO_IDX");
    }

    {
        fx32    tmp = FX_RAD_TO_DEG((fx32)((2 * FX64C_PI + 0x80000LL) >> 20));
        SDK_ASSERTMSG(tmp == 1474564, "FX_RAD_TO_DEG");
    }

    {
        int     tmp = FX_RAD_TO_IDX((fx32)((FX64C_PI + 0x80000LL) >> 20));
        SDK_ASSERTMSG(tmp == 32768, "FX_RAD_TO_IDX");
    }

    // FX_SinIdx, FX_CosIdx, FX_Atan, FX_Atan2 not tested
    {
        double  max = 0.0;
        fx32    x;
        fx32    xmax = 0;

        for (x = 0; x < 25736; ++x)
        {                              // 0 ... 2pi
            double  a = sin((double)x / 4096.0);
            fx64c   b = FX_SinFx64c(x);
            double  miss = fabs(a - b / 4294967296.0);
            if (miss > max)
            {
                max = miss;
                xmax = x;
            }
        }
        OS_Printf("Maximum margin of error of FX_SinFx64c is %.10f(rad = %f)\n", max,
                  (double)xmax / 4096.0);
    }

    {
        double  max = 0.0;
        fx32    x;
        fx32    xmax = 0;

        for (x = 0; x < 25736; ++x)
        {                              // 0 ... 2pi
            double  a = cos((double)x / 4096.0);
            fx64c   b = FX_CosFx64c(x);
            double  miss = fabs(a - b / 4294967296.0);
            if (miss > max)
            {
                max = miss;
                xmax = x;
            }
        }
        OS_Printf("Maximum margin of error of FX_CosFx64c is %.10f(rad = %f)\n", max,
                  (double)xmax / 4096.0);
    }

    OS_Printf(" done.\n");
}

typedef double (*BINARY_FUNC_PTR) (f32 lhs, f32 rhs);


static double getErrorBtweenFx32AnbF32_Multiple(f32 lhs, f32 rhs)
{
    double  errorVal;
    fx32    fxl = FX_F32_TO_FX32(lhs);
    fx32    fxr = FX_F32_TO_FX32(rhs);
    fx32    fxResult;

    fxResult = FX_Mul(fxl, fxr);
    errorVal = fabs((double)lhs * (double)rhs - (FX_FX32_TO_F32(fxResult)));

    return errorVal;
}


static void test_FX_CP(void)
{
    OS_Printf("----- testing fx_cp.h -----\n");
    {
        fx32    tmp = FX_Div(FX32_ONE * 2, FX32_ONE * 3);
        SDK_ASSERTMSG(tmp == 2731, "FX_Div");
    }
    {
        fx32    tmp = FX_Inv(FX32_ONE + (FX32_ONE >> 1));
        SDK_ASSERTMSG(tmp == 2731, "FX_Inv");
    }
    {
        fx32    tmp = FX_Sqrt(FX32_ONE * 2);
        SDK_ASSERTMSG(tmp == 5793, "FX_Sqrt %d", tmp);
    }
    {
        fx32    tmp;
        FX_DivAsync(FX32_ONE * 2, FX32_ONE * 3);
        tmp = FX_GetDivResult();
        SDK_ASSERTMSG(tmp == 2731, "FX_DivAsync/FX_GetDivResultAsync");
    }
    {
        fx32    tmp;
        FX_InvAsync(FX32_ONE + (FX32_ONE >> 1));
        tmp = FX_GetInvResult();
        SDK_ASSERTMSG(tmp == 2731, "FX_InvAsync/FX_GetInvResultAsync");
    }
    {
        fx32    tmp;
        FX_SqrtAsync(FX32_ONE * 2);
        tmp = FX_GetSqrtResult();
        SDK_ASSERTMSG(tmp == 5793, "FX_SqrtAsync/FX_GetSqrtResultAsync");
    }
    {
        fx32    tmp;
        tmp = FX_InvSqrt(FX32_ONE * 4);
        SDK_ASSERTMSG(tmp == (FX32_ONE >> 1), "FX_InvSqrt");
    }

    OS_Printf(" done.\n");
}

static void print_mtx33(const MtxFx33 *m)
{
#ifdef SDK_FINALROM
#pragma unused( m )
#endif

    OS_Printf("_\n");
    OS_Printf("%f %f %f\n", m->_00 / 4096.0, m->_01 / 4096.0, m->_02 / 4096.0);
    OS_Printf("%f %f %f\n", m->_10 / 4096.0, m->_11 / 4096.0, m->_12 / 4096.0);
    OS_Printf("%f %f %f\n", m->_20 / 4096.0, m->_21 / 4096.0, m->_22 / 4096.0);
    OS_Printf("_\n");
}

static BOOL equal_mtx33(const MtxFx33 *a, const MtxFx33 *b)
{
    return (a->_00 == b->_00 &&
            a->_01 == b->_01 &&
            a->_02 == b->_02 &&
            a->_10 == b->_10 &&
            a->_11 == b->_11 &&
            a->_12 == b->_12 && a->_20 == b->_20 && a->_21 == b->_21 && a->_22 == b->_22);
}

static BOOL equal_mtx43(const MtxFx43 *a, const MtxFx43 *b)
{
    return (a->_00 == b->_00 &&
            a->_01 == b->_01 &&
            a->_02 == b->_02 &&
            a->_10 == b->_10 &&
            a->_11 == b->_11 &&
            a->_12 == b->_12 &&
            a->_20 == b->_20 &&
            a->_21 == b->_21 &&
            a->_22 == b->_22 && a->_30 == b->_30 && a->_31 == b->_31 && a->_32 == b->_32);
}

static void print_mtx44(const MtxFx44 *m)
{
#ifdef SDK_FINALROM
#pragma unused( m )
#endif

    OS_Printf("_\n");
    OS_Printf("%f %f %f %f\n", m->_00 / 4096.0, m->_01 / 4096.0, m->_02 / 4096.0, m->_03 / 4096.0);
    OS_Printf("%f %f %f %f\n", m->_10 / 4096.0, m->_11 / 4096.0, m->_12 / 4096.0, m->_13 / 4096.0);
    OS_Printf("%f %f %f %f\n", m->_20 / 4096.0, m->_21 / 4096.0, m->_22 / 4096.0, m->_23 / 4096.0);
    OS_Printf("%f %f %f %f\n", m->_30 / 4096.0, m->_31 / 4096.0, m->_32 / 4096.0, m->_33 / 4096.0);
    OS_Printf("_\n");
}

static BOOL equal_mtx44(const MtxFx44 *a, const MtxFx44 *b)
{
    return (a->_00 == b->_00 &&
            a->_01 == b->_01 &&
            a->_02 == b->_02 &&
            a->_03 == b->_03 &&
            a->_10 == b->_10 &&
            a->_11 == b->_11 &&
            a->_12 == b->_12 &&
            a->_13 == b->_13 &&
            a->_20 == b->_20 &&
            a->_21 == b->_21 &&
            a->_22 == b->_22 &&
            a->_23 == b->_23 &&
            a->_30 == b->_30 && a->_31 == b->_31 && a->_32 == b->_32 && a->_33 == b->_33);
}

static void test_FX_MTX33(void)
{
    OS_Printf("----- testing fx_mtx33.h -----\n");

    {
        MtxFx33 m;
        MtxFx33 m33;
        MtxFx43 m43;
        MtxFx44 m44;

        const MtxFx33 ans = {
            FX32_ONE, 0, 0,
            0, FX32_ONE, 0,
            0, 0, FX32_ONE
        };
        const MtxFx43 ans43 = {
            FX32_ONE, 0, 0,
            0, FX32_ONE, 0,
            0, 0, FX32_ONE,
            0, 0, 0
        };
        const MtxFx44 ans44 = {
            FX32_ONE, 0, 0, 0,
            0, FX32_ONE, 0, 0,
            0, 0, FX32_ONE, 0,
            0, 0, 0, FX32_ONE
        };

        // they must be changed.
        m43._30 = m43._31 = m43._32 = 1;
        m44._30 = m44._31 = m44._32 = m44._33 = 1;
        m44._03 = m44._13 = m44._23 = 1;

        MTX_Identity33(&m);
        SDK_ASSERTMSG(equal_mtx33(&m, &ans), "MTX_Identity33");
        MTX_Copy33(&m, &m33);
        SDK_ASSERTMSG(equal_mtx33(&m, &m33), "MTX_Copy33");
        MTX_Copy33To43(&m, &m43);
        SDK_ASSERTMSG(equal_mtx43(&m43, &ans43), "MTX_Copy33To43");
        MTX_Copy33To44(&m, &m44);
        SDK_ASSERTMSG(equal_mtx44(&m44, &ans44), "MTX_Copy33To44");
    }

    {
        MtxFx33 m = {
            0, 3, 6,
            1, 4, 7,
            2, 5, 8
        };
        const MtxFx33 ans = {
            0, 1, 2,
            3, 4, 5,
            6, 7, 8
        };
        MTX_Transpose33(&m, &m);
        SDK_ASSERTMSG(equal_mtx33(&m, &ans), "MTX_Transpose33");
    }
    {
        MtxFx33 m;
        const MtxFx33 ans = {
            FX32_ONE, 0, 0,
            0, FX32_ONE * 2, 0,
            0, 0, FX32_ONE * 3
        };
        MTX_Scale33(&m, FX32_ONE, FX32_ONE * 2, FX32_ONE * 3);
        SDK_ASSERTMSG(equal_mtx33(&m, &ans), "MTX_Scale33");
    }
    {
        MtxFx33 m1;
        MtxFx33 m2;
        MtxFx33 m;
        MtxFx33 ans = {
            FX32_ONE, 0, 0,
            0, FX32_ONE, 0,
            0, 0, FX32_ONE
        };

        MTX_RotX33(&m1, FX32_ONE, 0);  // + 90
        MTX_RotX33(&m2, 0, -FX32_ONE); // + 180
        MTX_Concat33(&m1, &m2, &m);
        MTX_Concat33(&m, &m1, &m);
        SDK_ASSERTMSG(equal_mtx33(&m, &ans), "MTX_RotX33/MTX_Concat33");

        MTX_RotY33(&m1, FX32_ONE, 0);  // + 90
        MTX_RotY33(&m2, 0, -FX32_ONE); // + 180
        MTX_Concat33(&m1, &m2, &m);
        MTX_Concat33(&m, &m1, &m);
        SDK_ASSERTMSG(equal_mtx33(&m, &ans), "MTX_RotY33/MTX_Concat33");

        MTX_RotZ33(&m1, FX32_ONE, 0);  // + 90
        MTX_RotZ33(&m2, 0, -FX32_ONE); // + 180
        MTX_Concat33(&m1, &m2, &m);
        MTX_Concat33(&m, &m1, &m);
        SDK_ASSERTMSG(equal_mtx33(&m, &ans), "MTX_RotZ33/MTX_Concat33");
    }
    {

        MtxFx33 ans, m;
        const VecFx32 xAxis = { FX32_ONE, 0, 0 };
        const VecFx32 yAxis = { 0, FX32_ONE, 0 };
        const VecFx32 zAxis = { 0, 0, FX32_ONE };

        MTX_RotX33(&ans, FX32_ONE, 0); // + 90
        MTX_RotAxis33(&m, &xAxis, FX32_ONE, 0);
        SDK_ASSERTMSG(equal_mtx33(&ans, &m), "MTX_RotAxis33");

        MTX_RotY33(&ans, FX32_ONE, 0); // + 90
        MTX_RotAxis33(&m, &yAxis, FX32_ONE, 0);
        SDK_ASSERTMSG(equal_mtx33(&ans, &m), "MTX_RotAxis33");

        MTX_RotZ33(&ans, FX32_ONE, 0); // + 90
        MTX_RotAxis33(&m, &zAxis, FX32_ONE, 0);
        SDK_ASSERTMSG(equal_mtx33(&ans, &m), "MTX_RotAxis33");
    }
    {
        MtxFx33 ans, m;
        int     flag;

        MTX_RotX33(&ans, FX32_ONE, 0);
        flag = MTX_Inverse33(&ans, &m);
        SDK_ASSERTMSG(!flag, "MTX_Inverse33");
        MTX_RotX33(&ans, -FX32_ONE, 0);
        SDK_ASSERTMSG(equal_mtx33(&ans, &m), "MTX_Inverse33");

        MTX_RotY33(&ans, FX32_ONE, 0);
        flag = MTX_Inverse33(&ans, &m);
        SDK_ASSERTMSG(!flag, "MTX_Inverse33");
        MTX_RotY33(&ans, -FX32_ONE, 0);
        SDK_ASSERTMSG(equal_mtx33(&ans, &m), "MTX_Inverse33");

        MTX_RotZ33(&ans, FX32_ONE, 0);
        flag = MTX_Inverse33(&ans, &m);
        SDK_ASSERTMSG(!flag, "MTX_Inverse33");
        MTX_RotZ33(&ans, -FX32_ONE, 0);
        SDK_ASSERTMSG(equal_mtx33(&ans, &m), "MTX_Inverse33");
    }

    {
        int     flag;
        MtxFx33 m = {
            FX32_ONE, FX32_ONE, FX32_ONE,
            FX32_ONE, FX32_ONE, FX32_ONE,
            FX32_ONE, FX32_ONE, FX32_ONE
        };
        MtxFx33 ans = {
            FX32_ONE, FX32_ONE, FX32_ONE,
            FX32_ONE, FX32_ONE, FX32_ONE,
            FX32_ONE, FX32_ONE, FX32_ONE
        };
        flag = MTX_Inverse33(&m, &m);
        SDK_ASSERTMSG(flag, "MTX_Inverse33");
        SDK_ASSERTMSG(equal_mtx33(&ans, &m), "MTX_Inverse33");
    }

    {
        MtxFx33 m = {
            FX32_ONE, FX32_ONE * 2, FX32_ONE * 3,
            FX32_ONE * 4, FX32_ONE * 5, FX32_ONE * 6,
            FX32_ONE * 7, FX32_ONE * 8, FX32_ONE * 9
        };
        VecFx32 vec = { FX32_ONE, FX32_ONE, FX32_ONE };
        VecFx32 ans = { FX32_ONE * 12, FX32_ONE * 15, FX32_ONE * 18 };
        MTX_MultVec33(&vec, &m, &vec);
        SDK_ASSERTMSG(vec.x == ans.x && vec.y == ans.y && vec.z == ans.z, "MTX_MultVec33");
    }

    OS_Printf(" done.\n");
}

static void test_FX_MTX43(void)
{
    OS_Printf("----- testing fx_mtx43.h -----\n");

    {
        MtxFx43 m;
        MtxFx33 m33;
        MtxFx43 m43;
        MtxFx44 m44;
        const MtxFx33 ans33 = {
            FX32_ONE, 0, 0,
            0, FX32_ONE, 0,
            0, 0, FX32_ONE
        };
        const MtxFx43 ans43 = {
            FX32_ONE, 0, 0,
            0, FX32_ONE, 0,
            0, 0, FX32_ONE,
            0, 0, 0
        };
        const MtxFx44 ans44 = {
            FX32_ONE, 0, 0, 0,
            0, FX32_ONE, 0, 0,
            0, 0, FX32_ONE, 0,
            0, 0, 0, FX32_ONE
        };
        m44._03 = m44._13 = m44._23 = m44._33 = 1;

        MTX_Identity43(&m);
        SDK_ASSERTMSG(equal_mtx43(&m, &ans43), "MTX_Identity43");
        MTX_Copy43(&m, &m43);
        SDK_ASSERTMSG(equal_mtx43(&m, &m43), "MTX_Copy43");
        MTX_Copy43To33(&m, &m33);
        SDK_ASSERTMSG(equal_mtx33(&m33, &ans33), "MTX_Copy43To33");
        MTX_Copy43To44(&m, &m44);
        SDK_ASSERTMSG(equal_mtx44(&m44, &ans44), "MTX_Copy43To44");
    }

    {
        MtxFx43 m = {
            0, 3, 6,
            1, 4, 7,
            2, 5, 8,
            9, 10, 11
        };
        const MtxFx43 ans = {
            0, 1, 2,
            3, 4, 5,
            6, 7, 8,
            0, 0, 0
        };
        MTX_Transpose43(&m, &m);
        SDK_ASSERTMSG(equal_mtx43(&m, &ans), "MTX_Transpose43");
    }
    {
        MtxFx43 m;
        const MtxFx43 ans = {
            FX32_ONE, 0, 0,
            0, FX32_ONE * 2, 0,
            0, 0, FX32_ONE * 3,
            0, 0, 0
        };
        MTX_Scale43(&m, FX32_ONE, FX32_ONE * 2, FX32_ONE * 3);
        SDK_ASSERTMSG(equal_mtx43(&m, &ans), "MTX_Scale43");
    }
    {
        MtxFx43 m1;
        MtxFx43 m2;
        MtxFx43 m;
        MtxFx43 ans = {
            FX32_ONE, 0, 0,
            0, FX32_ONE, 0,
            0, 0, FX32_ONE,
            0, 0, 0
        };

        MTX_RotX43(&m1, FX32_ONE, 0);  // + 90
        MTX_RotX43(&m2, 0, -FX32_ONE); // + 180
        MTX_Concat43(&m1, &m2, &m);
        MTX_Concat43(&m, &m1, &m);
        SDK_ASSERTMSG(equal_mtx43(&m, &ans), "MTX_RotX43/MTX_Concat43");

        MTX_RotY43(&m1, FX32_ONE, 0);  // + 90
        MTX_RotY43(&m2, 0, -FX32_ONE); // + 180
        MTX_Concat43(&m1, &m2, &m);
        MTX_Concat43(&m, &m1, &m);
        SDK_ASSERTMSG(equal_mtx43(&m, &ans), "MTX_RotY43/MTX_Concat43");

        MTX_RotZ43(&m1, FX32_ONE, 0);  // + 90
        MTX_RotZ43(&m2, 0, -FX32_ONE); // + 180
        MTX_Concat43(&m1, &m2, &m);
        MTX_Concat43(&m, &m1, &m);
        SDK_ASSERTMSG(equal_mtx43(&m, &ans), "MTX_RotZ43/MTX_Concat43");
    }
    {
        MtxFx43 ans, m;
        const VecFx32 xAxis = { FX32_ONE, 0, 0 };
        const VecFx32 yAxis = { 0, FX32_ONE, 0 };
        const VecFx32 zAxis = { 0, 0, FX32_ONE };

        MTX_RotX43(&ans, FX32_ONE, 0); // + 90
        MTX_RotAxis43(&m, &xAxis, FX32_ONE, 0);
        SDK_ASSERTMSG(equal_mtx43(&ans, &m), "MTX_RotAxis43");

        MTX_RotY43(&ans, FX32_ONE, 0); // + 90
        MTX_RotAxis43(&m, &yAxis, FX32_ONE, 0);
        SDK_ASSERTMSG(equal_mtx43(&ans, &m), "MTX_RotAxis43");

        MTX_RotZ43(&ans, FX32_ONE, 0); // + 90
        MTX_RotAxis43(&m, &zAxis, FX32_ONE, 0);
        SDK_ASSERTMSG(equal_mtx43(&ans, &m), "MTX_RotAxis43");

    }
    {
        MtxFx43 ans, m;
        int     flag;

        MTX_RotX43(&ans, FX32_ONE, 0);
        flag = MTX_Inverse43(&ans, &m);
        SDK_ASSERTMSG(!flag, "MTX_Inverse43");
        MTX_RotX43(&ans, -FX32_ONE, 0);
        SDK_ASSERTMSG(equal_mtx43(&ans, &m), "MTX_Inverse43");

        MTX_RotY43(&ans, FX32_ONE, 0);
        flag = MTX_Inverse43(&ans, &m);
        SDK_ASSERTMSG(!flag, "MTX_Inverse43");
        MTX_RotY43(&ans, -FX32_ONE, 0);
        SDK_ASSERTMSG(equal_mtx43(&ans, &m), "MTX_Inverse43");

        MTX_RotZ43(&ans, FX32_ONE, 0);
        flag = MTX_Inverse43(&ans, &m);
        SDK_ASSERTMSG(!flag, "MTX_Inverse43");
        MTX_RotZ43(&ans, -FX32_ONE, 0);
        SDK_ASSERTMSG(equal_mtx43(&ans, &m), "MTX_Inverse43");
    }

    {
        int     flag;
        MtxFx43 m = {
            FX32_ONE, FX32_ONE, FX32_ONE,
            FX32_ONE, FX32_ONE, FX32_ONE,
            FX32_ONE, FX32_ONE, FX32_ONE,
            FX32_ONE, FX32_ONE, FX32_ONE
        };
        MtxFx43 ans = {
            FX32_ONE, FX32_ONE, FX32_ONE,
            FX32_ONE, FX32_ONE, FX32_ONE,
            FX32_ONE, FX32_ONE, FX32_ONE,
            FX32_ONE, FX32_ONE, FX32_ONE
        };
        flag = MTX_Inverse43(&m, &m);
        SDK_ASSERTMSG(flag, "MTX_Inverse43");
        SDK_ASSERTMSG(equal_mtx43(&ans, &m), "MTX_Inverse43");
    }

    OS_Printf(" done.\n");
}

static void test_FX_MTX44(void)
{
    OS_Printf("----- testing fx_mtx44.h -----\n");

    {
        MtxFx44 m44;
        MtxFx43 m43;
        MtxFx33 m33;
        MtxFx44 m;
        const MtxFx33 ans33 = {
            FX32_ONE, 0, 0,
            0, FX32_ONE, 0,
            0, 0, FX32_ONE
        };
        const MtxFx43 ans43 = {
            FX32_ONE, 0, 0,
            0, FX32_ONE, 0,
            0, 0, FX32_ONE,
            0, 0, 0
        };
        const MtxFx44 ans44 = {
            FX32_ONE, 0, 0, 0,
            0, FX32_ONE, 0, 0,
            0, 0, FX32_ONE, 0,
            0, 0, 0, FX32_ONE
        };

        MTX_Identity44(&m);
        SDK_ASSERTMSG(equal_mtx44(&m, &ans44), "MTX_Identity44");
        MTX_Copy44(&m, &m44);
        SDK_ASSERTMSG(equal_mtx44(&m, &m44), "MTX_Copy44");
        MTX_Copy44To33(&m, &m33);
        SDK_ASSERTMSG(equal_mtx33(&m33, &ans33), "MTX_Copy44To33");
        MTX_Copy44To43(&m, &m43);
        SDK_ASSERTMSG(equal_mtx43(&m43, &ans43), "MTX_Copy44To43");
    }

    {
        MtxFx44 m = {
            0, 4, 8, 12,
            1, 5, 9, 13,
            2, 6, 10, 14,
            3, 7, 11, 15
        };
        const MtxFx44 ans = {
            0, 1, 2, 3,
            4, 5, 6, 7,
            8, 9, 10, 11,
            12, 13, 14, 15
        };
        MTX_Transpose44(&m, &m);
        SDK_ASSERTMSG(equal_mtx44(&m, &ans), "MTX_Transpose44");
    }
    {
        MtxFx44 m;
        const MtxFx44 ans = {
            FX32_ONE, 0, 0, 0,
            0, FX32_ONE * 2, 0, 0,
            0, 0, FX32_ONE * 3, 0,
            0, 0, 0, FX32_ONE
        };
        MTX_Scale44(&m, FX32_ONE, FX32_ONE * 2, FX32_ONE * 3);
        SDK_ASSERTMSG(equal_mtx44(&m, &ans), "MTX_Scale44");
    }
    {
        MtxFx44 m1;
        MtxFx44 m2;
        MtxFx44 m;
        MtxFx44 ans = {
            FX32_ONE, 0, 0, 0,
            0, FX32_ONE, 0, 0,
            0, 0, FX32_ONE, 0,
            0, 0, 0, FX32_ONE
        };
        MTX_RotX44(&m1, FX32_ONE, 0);  // + 90
        MTX_RotX44(&m2, 0, -FX32_ONE); // + 180
        MTX_Concat44(&m1, &m2, &m);
        MTX_Concat44(&m, &m1, &m);
        print_mtx44(&m);

        SDK_ASSERTMSG(equal_mtx44(&m, &ans), "MTX_RotX44/MTX_Concat44");

        MTX_RotY44(&m1, FX32_ONE, 0);  // + 90
        MTX_RotY44(&m2, 0, -FX32_ONE); // + 180
        MTX_Concat44(&m1, &m2, &m);
        MTX_Concat44(&m, &m1, &m);
        SDK_ASSERTMSG(equal_mtx44(&m, &ans), "MTX_RotY44/MTX_Concat44");

        MTX_RotZ44(&m1, FX32_ONE, 0);  // + 90
        MTX_RotZ44(&m2, 0, -FX32_ONE); // + 180
        MTX_Concat44(&m1, &m2, &m);
        MTX_Concat44(&m, &m1, &m);
        SDK_ASSERTMSG(equal_mtx44(&m, &ans), "MTX_RotZ44/MTX_Concat44");
    }
    {
        MtxFx44 ans, m;
        const VecFx32 xAxis = { FX32_ONE, 0, 0 };
        const VecFx32 yAxis = { 0, FX32_ONE, 0 };
        const VecFx32 zAxis = { 0, 0, FX32_ONE };

        MTX_RotX44(&ans, FX32_ONE, 0); // + 90
        MTX_RotAxis44(&m, &xAxis, FX32_ONE, 0);
        SDK_ASSERTMSG(equal_mtx44(&ans, &m), "MTX_RotAxis44");

        MTX_RotY44(&ans, FX32_ONE, 0); // + 90
        MTX_RotAxis44(&m, &yAxis, FX32_ONE, 0);
        SDK_ASSERTMSG(equal_mtx44(&ans, &m), "MTX_RotAxis44");

        MTX_RotZ44(&ans, FX32_ONE, 0); // + 90
        MTX_RotAxis44(&m, &zAxis, FX32_ONE, 0);
        SDK_ASSERTMSG(equal_mtx44(&ans, &m), "MTX_RotAxis44");
    }

    OS_Printf(" done.\n");
}

static void test_Table(void)
{
    u16     i;
    fx32    s, c;
    OS_Printf("----- testing FX_SinIdx, FX_CosIdx -----\n");

    for (i = 0; i < 4096; ++i)
    {
        s = FX_SinIdx((u16)(i * 16));
        c = FX_CosIdx((u16)(i * 16));
        SDK_ASSERTMSG(testTable[i * 2] == s || testTable[i * 2 + 1] == c, "FX_SinIdx, FX_CosIdx\n");
    }

    OS_Printf(" done.\n");
}

void NitroMain(void)
{
    OS_Init();
    OS_Printf("OS_Init() done.\n");
    FX_Init();
    OS_Printf("FX_Init() done.\n");



    test_FX_VEC();
    test_FX_TRIG();
    test_FX_CP();
    test_FX_MTX33();
    test_FX_MTX43();
    test_FX_MTX44();
    test_Table();

    // appended test cases
    VECTest_Appended();
    MTXTest_appended();


    OS_Printf("all tests passed\n");
    while (1)
        ;
}
