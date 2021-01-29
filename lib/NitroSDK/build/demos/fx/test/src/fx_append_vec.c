/*---------------------------------------------------------------------------*
  Project:  NitroSDK - FX - demos - test
  File:     fx_append_vec.c

  Copyright 2003-2007 Nintendo.  All rights reserved.

  These coded instructions, statements, and computer programs contain
  proprietary information of Nintendo of America Inc. and/or Nintendo
  Company Ltd., and are protected by Federal copyright law.  They may
  not be disclosed to third parties or copied or duplicated in any form,
  in whole or in part, without the prior written consent of Nintendo.
  
  $Log: fx_append_vec.c,v $
  Revision 1.9  2007/05/22 02:05:01  okubata_ryoma
  一部 warning が出る旨を追記

  Revision 1.8  2006/01/18 02:11:21  kitase_hirotake
  do-indent

  Revision 1.7  2005/02/28 05:26:13  yosizaki
  do-indent.

  Revision 1.6  2004/11/02 08:32:54  takano_makoto
  fix comment.

  Revision 1.5  2004/04/12 10:05:26  takano_makoto
  Fix a bug in BasicOperator_test_.

  Revision 1.4  2004/04/07 01:27:57  yada
  fix header comment

  Revision 1.3  2004/02/05 07:09:02  yasu
  change SDK prefix iris -> nitro

  Revision 1.2  2004/01/29 01:35:09  kitani_toshikazu
  Fix some bugs for debug build, restore appended test cases.
 
  $NoKeywords: $
    
 *---------------------------------------------------------------------------*/

#ifndef  FX_APPEND_VEC_H_
#include "fx_append_vec.h"
#endif //  FX_APPEND_VEC_H_



#define FX2D( fx ) (double)FX_FX32_TO_F32( fx )
#define FX64C2D( fx ) (double)( (double)( fx ) / (double)( 0x1LL << FX64C_SHIFT ) )
#define FX_FX32_TO_FX64C(x) ( (fx64)(x) << ( FX64C_SHIFT - FX32_SHIFT ))




#define fxIntMax        ( FX32_MAX & FX32_INT_MASK )
#define fxLSD           0x1


// 入力データ
static const VecFx32 vInputData[] = {
    {fxLSD, 0x0, 0x0},
    {fxLSD << 1, 0x0, 0x0},
    {FX32_ONE, 0x0, 0x0},
    {fxIntMax >> 6, 0x0, 0x0},
    {fxIntMax >> 5, 0x0, 0x0},
    {fxIntMax >> 4, 0x0, 0x0},
    {fxIntMax >> 3, 0x0, 0x0},
    {fxIntMax >> 2, 0x0, 0x0},
    {fxIntMax >> 1, 0x0, 0x0},
    {fxIntMax, 0x0, 0x0},

    // 内部で加算が発生するケース
    {fxLSD, fxLSD, 0x0},
    {fxLSD << 1, fxLSD << 1, 0x0},
    {fxIntMax >> 5, fxIntMax >> 5, 0x0},
    {fxIntMax >> 4, fxIntMax >> 4, 0x0},
    {fxIntMax >> 3, fxIntMax >> 3, 0x0},
    {fxIntMax >> 2, fxIntMax >> 2, 0x0},
    {fxIntMax >> 1, fxIntMax >> 1, 0x0},
    {fxIntMax, fxIntMax, 0x0}
};






// Util
static void VecFx32ToVecD_(const VecFx32 *src, VecD * dst);
static void VecSubVecD_(const VecD * a, const VecD * b, VecD * ab);
static void GetErrorVecD_(const VecD * a, const VecD * b, VecD * ab);
static void CalcErrorBtweenVecFxAndVecD_(const VecFx32 *a, const VecD * b, VecD * vError);
static double FindMaxElement_(const VecD * v);



// double 版 実装
static double FX_Mul_d_(const fx32 a, const fx32 b);
static double VEC_DotProduct_d_(const VecFx32 *a, const VecFx32 *b);
static void VEC_CrossProduct_d_(const VecFx32 *a, const VecFx32 *b, VecD * axb);
static double VEC_Mag_d_(const VecFx32 *pSrc);
static void VEC_Normalize_d_(const VecFx32 *pSrc, VecD * pDst);

// Error 計算
static double GetDotProductError_(const VecFx32 *a, const VecFx32 *b);
static double GetCrossProductError_(const VecFx32 *a, const VecFx32 *b);
static double GetMagError_(const VecFx32 *a);
static void GetNormalizeError_(const VecFx32 *a, VecD * vError);

// Tests
static void VEC_Mag_test_();
static void VEC_Normalize_test_();
static void VEC_DotCrossProduct_test_();
static void BasicOperator_test_();
static void Unary_operator_test_(fx32 vf1);
static void Binary_operator_test_(fx32 vf1, fx32 vf2);


//------------------------------------------------------------------------------
// テスト追加項目エントリ
void VECTest_Appended()
{
    OS_Printf("**================== VECTest_Appended start ====================\n");
    BasicOperator_test_();
    VEC_DotCrossProduct_test_();
    VEC_Mag_test_();
    VEC_Normalize_test_();
    OS_Printf("**================== VECTest_Appended done ====================\n");
}

//==============================================================================
// 表示関連

//------------------------------------------------------------------------------
static void printVecFx32Msg(const char *msg, const VecFx32 *v)
{
    SDK_NULL_ASSERT(msg);
    SDK_NULL_ASSERT(v);

    OutDetail(msg);
    OutDetail("( %.5f, %.5f, %.5f )", FX_FX32_TO_F32(v->x), FX_FX32_TO_F32(v->y),
              FX_FX32_TO_F32(v->z));
    OutDetail("\n");
}

//------------------------------------------------------------------------------
static void printVecDMsg(const char *msg, const VecD * v)
{
    SDK_NULL_ASSERT(msg);
    SDK_NULL_ASSERT(v);

    OutDetail(msg);
    OutDetail("( %.5f, %.5f, %.5f )", v->x, v->y, v->z);
    OutDetail("\n");
}



//==============================================================================
// Misc.
//------------------------------------------------------------------------------
static void VecFx32ToVecD_(const VecFx32 *src, VecD * dst)
{
    SDK_NULL_ASSERT(src);
    SDK_NULL_ASSERT(dst);

    dst->x = (double)FX_FX32_TO_F32(src->x);
    dst->y = (double)FX_FX32_TO_F32(src->y);
    dst->z = (double)FX_FX32_TO_F32(src->z);
}

//------------------------------------------------------------------------------
static void VecSubVecD_(const VecD * a, const VecD * b, VecD * ab)
{
    SDK_NULL_ASSERT(a);
    SDK_NULL_ASSERT(b);
    SDK_NULL_ASSERT(ab);

    ab->x = a->x - b->x;
    ab->y = a->y - b->y;
    ab->z = a->z - b->z;
}

//------------------------------------------------------------------------------
static void GetErrorVecD_(const VecD * a, const VecD * b, VecD * ab)
{
    SDK_NULL_ASSERT(a);
    SDK_NULL_ASSERT(b);
    SDK_NULL_ASSERT(ab);

    VecSubVecD_(a, b, ab);

    ab->x = fabs(ab->x);
    ab->y = fabs(ab->y);
    ab->z = fabs(ab->z);
}

//------------------------------------------------------------------------------
static void CalcErrorBtweenVecFxAndVecD_(const VecFx32 *a, const VecD * b, VecD * vError)
{
    VecD    aD;

    SDK_NULL_ASSERT(a);
    SDK_NULL_ASSERT(b);
    SDK_NULL_ASSERT(vError);

    // fx => double
    VecFx32ToVecD_(a, &aD);

    // error = a - b
    GetErrorVecD_(&aD, b, vError);
}

//------------------------------------------------------------------------------
// 最大の絶対値を持つ要素を得る
static double FindMaxElement_(const VecD * v)
{

    double  max = 0.0;

    SDK_NULL_ASSERT(v);

    if (fabs(v->x) > fabs(v->y))
    {
        max = fabs(v->x);
    }
    else
    {
        max = fabs(v->y);
    }

    if (fabs(v->z) > max)
        max = fabs(v->z);

    return max;
}





//------------------------------------------------------------------------------
// double 版
//------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------
static double FX_Mul_d_(const fx32 a, const fx32 b)
{
    return (double)(FX_FX32_TO_F32(a)) * (double)(FX_FX32_TO_F32(b));
}

//------------------------------------------------------------------------------
static double VEC_DotProduct_d_(const VecFx32 *a, const VecFx32 *b)
{
    VecD    da, db;

    SDK_NULL_ASSERT(a);
    SDK_NULL_ASSERT(b);


    VecFx32ToVecD_(a, &da);
    VecFx32ToVecD_(b, &db);

    return da.x * db.x + da.y * db.y + da.z * db.z;
}

//------------------------------------------------------------------------------
static void VEC_CrossProduct_d_(const VecFx32 *a, const VecFx32 *b, VecD * axb)
{
    VecD    da, db;

    SDK_NULL_ASSERT(a);
    SDK_NULL_ASSERT(b);
    SDK_NULL_ASSERT(axb);

    VecFx32ToVecD_(a, &da);
    VecFx32ToVecD_(b, &db);

    axb->x = da.y * db.z - da.z * db.y;
    axb->y = da.z * db.x - da.x * db.z;
    axb->z = da.x * db.y - da.y * db.x;
}

//------------------------------------------------------------------------------
static double VEC_Mag_d_(const VecFx32 *pSrc)
{
    VecD    pSrcD;

    SDK_NULL_ASSERT(pSrc);


    VecFx32ToVecD_(pSrc, &pSrcD);

    return sqrt(pSrcD.x * pSrcD.x + pSrcD.y * pSrcD.y + pSrcD.z * pSrcD.z);
}

//------------------------------------------------------------------------------
static void VEC_Normalize_d_(const VecFx32 *pSrc, VecD * pDst)
{
    double  t = 0.0;
    VecD    pSrcD;

    SDK_NULL_ASSERT(pSrc);
    SDK_NULL_ASSERT(pDst);


    VecFx32ToVecD_(pSrc, &pSrcD);

    t = VEC_Mag_d_(pSrc);
    SDK_ASSERTMSG(t > 0, "*pSrc is (0, 0, 0)");

    t = 1.0 / t;

    pDst->x = t * pSrcD.x;
    pDst->y = t * pSrcD.y;
    pDst->z = t * pSrcD.z;
}


//==============================================================================
// 誤差計算
//
//------------------------------------------------------------------------------
static double GetDotProductError_(const VecFx32 *a, const VecFx32 *b)
{
    double  resD = 0.0;
    double  resFX = 0.0;

    SDK_NULL_ASSERT(a);
    SDK_NULL_ASSERT(b);

    resD = VEC_DotProduct_d_(a, b);
    resFX = (double)(FX_FX32_TO_F32(VEC_DotProduct(a, b)));

    return fabs(resFX - resD);
}

//------------------------------------------------------------------------------
static double GetCrossProductError_(const VecFx32 *a, const VecFx32 *b)
{
    VecFx32 resFX;
    VecD    vError;

    SDK_NULL_ASSERT(a);
    SDK_NULL_ASSERT(b);


    VEC_CrossProduct_d_(a, b, &vError);
    VEC_CrossProduct(a, b, &resFX);

    CalcErrorBtweenVecFxAndVecD_(&resFX, &vError, &vError);

    return FindMaxElement_(&vError);
}

//------------------------------------------------------------------------------
static double GetMagError_(const VecFx32 *a)
{
    double  resD = 0.0;
    double  resFX = 0.0;

    SDK_NULL_ASSERT(a);

    resD = VEC_Mag_d_(a);
    resFX = (double)(FX_FX32_TO_F32(VEC_Mag(a)));


    OutDetail("**** VEC_Mag ::: FX     Result => %.10f\n");
    OutDetail("**** VEC_Mag ::: double Result => %.10f\n");


    return fabs(resFX - resD);
}

//------------------------------------------------------------------------------
static void GetNormalizeError_(const VecFx32 *a, VecD * vError)
{
    VecFx32 resFX;

    SDK_NULL_ASSERT(a);
    SDK_NULL_ASSERT(vError);

    VEC_Normalize(a, &resFX);
    VEC_Normalize_d_(a, vError);


    OutDetail
        ("===================================================================================\n");
    printVecFx32Msg("**** VEC_Mag :::        Input  => ", a);
    printVecFx32Msg("                 FX     Result => ", &resFX);
    printVecDMsg("                 double Result => ", vError);


    CalcErrorBtweenVecFxAndVecD_(&resFX, vError, vError);


    OutDetail
        ("-----------------------------------------------------------------------------------\n");
    printVecDMsg("                 | FX-double | => ", vError);
    OutDetail("\n");

}

//------------------------------------------------------------------------------
static BOOL IsResultVecValid(const VecD * result)
{
    SDK_NULL_ASSERT(result);
    return (FindMaxElement_(result) < CRITICAL_ERROR_THRESHOLD) ? TRUE : FALSE;
}

//==============================================================================
// test Method
//==============================================================================
// 異なる大きさのベクトルに対して VEC_Mag_test_ を適用します
// 想定しない 誤差が検出された場合、メッセージを出力し アプリケーションを終了します。
static void VEC_Mag_test_()
{
    int     i = 0;
    double  err = 0.0;
    VecFx32 *v = NULL;

    OS_Printf("==================== VEC_Mag_test_ begin ====================\n");

    // OK Cases
    OS_Printf("------------ Must be OK case \n");
    for (i = 0; i < 9; i++)
        MUST_SUCCEED_ASSERT(GetMagError_(&vInputData[i]) < CRITICAL_ERROR_THRESHOLD, 1);

    // NG Case 
    OS_Printf("------------ Must be NG case \n");
    MUST_FAIL_ASSERT(GetMagError_(&vInputData[9]) < CRITICAL_ERROR_THRESHOLD, 2);

    // 内部で加算が発生するケース
    // OK Cases
    OS_Printf("------------ Must be OK case \n");
    for (i = 10; i < 16; i++)
        MUST_SUCCEED_ASSERT(GetMagError_(&vInputData[i]) < CRITICAL_ERROR_THRESHOLD, 3);


    // NG Case 
    OS_Printf("------------ Must be NG case \n");
    MUST_FAIL_ASSERT(GetMagError_(&vInputData[16]) < CRITICAL_ERROR_THRESHOLD, 4);
    MUST_FAIL_ASSERT(GetMagError_(&vInputData[17]) < CRITICAL_ERROR_THRESHOLD, 5);

    OS_Printf("==================== VEC_Mag_test_ done ====================\n");
}


//------------------------------------------------------------------------------
// ベクトルの正規化関数について double 版との出力誤差を比較します。
// 想定しない 誤差が検出された場合、メッセージを出力します
//
static void VEC_Normalize_test_()
{
    int     i = 0;
    VecD    testResult;

    OS_Printf("==================== VEC_Normalize_test_ begin ====================\n");
    OS_Printf("------------ Must SUCCEED case \n");
    for (i = 0; i < 7; i++)
    {
        GetNormalizeError_(&vInputData[i], &testResult);
        MUST_SUCCEED_ASSERT(IsResultVecValid(&testResult), 1);
    }


    for (i = 11; i < 14; i++)
    {
        GetNormalizeError_(&vInputData[i], &testResult);
        MUST_SUCCEED_ASSERT(IsResultVecValid(&testResult), 2);
    }

    OS_Printf("------------ Must FAIL case \n");
    // NG Case          very large 
    for (i = 7; i < 10; i++)
    {
        GetNormalizeError_(&vInputData[i], &testResult);
        MUST_FAIL_ASSERT(IsResultVecValid(&testResult), 3);
    }

    // NG Case          very small 
    GetNormalizeError_(&vInputData[10], &testResult);
    MUST_FAIL_ASSERT(IsResultVecValid(&testResult), 4);

    // NG Case          very large 
    for (i = 14; i < 18; i++)
    {
        GetNormalizeError_(&vInputData[i], &testResult);
        MUST_FAIL_ASSERT(IsResultVecValid(&testResult), 5);
    }

    OS_Printf("==================== VEC_Normalize_test_ done ====================\n");
}

//------------------------------------------------------------------------------
// 平行、直行するベクタに対する、内外積の値を計算し、double 計算との誤差を調査します。
// 想定しない 誤差が検出された場合、メッセージを出力し アプリケーションを終了します。
// FX_TEST__SHOW_DETAIL が定義された場合、詳細な情報をデバックコンソールに出力します
static void VEC_DotCrossProduct_test_()
{
    VecFx32 v1;
    VecFx32 v2;
    VecFx32 v3;

    int     i = 0, j = 0;
    const int angD = 182;              // 微小な角度変化( 約1°)
    const int ang45 = 8192;
    double  errDot = 0.0;
    double  errCross = 0.0;

    OS_Printf("==================== VEC_DotCrossProduct_test_ begin ====================\n");
    OS_Printf("-------- Some Warning is displayed \n");
    // 角度を微妙にずらしながら
    // （i = 0 では 理想的な直行、平行ベクタとなるため 誤差がまったく生じません）。
    for (i = 1; i < 5; i++)
    {
        v1.x = FX_CosIdx(ang45);
        v1.y = FX_SinIdx(ang45);
        v1.z = 0x0;
        v2.x = FX_CosIdx(ang45 + angD * i);
        v2.y = -FX_SinIdx(ang45 + angD * i);
        v2.z = 0x0;                    // 直行
        v3.x = FX_CosIdx(ang45 + angD * i);
        v3.y = FX_SinIdx(ang45 + angD * i);
        v3.z = 0x0;                    // 平行

        // だんだん長くする
        for (j = 0; j < 7; j++)
        {
            v1.x = FX_Mul(v1.x, FX32_ONE << j);
            v1.y = FX_Mul(v1.y, FX32_ONE << j);
            v2.x = FX_Mul(v2.x, FX32_ONE << j);
            v2.y = FX_Mul(v2.y, FX32_ONE << j);
            v3.x = FX_Mul(v3.x, FX32_ONE << j);
            v3.y = FX_Mul(v3.y, FX32_ONE << j);

            // double 実装との誤差の最大値を計算する
            errDot = GetDotProductError_(&v1, &v2);
            errCross = GetCrossProductError_(&v1, &v3);


            // 成否の確認
            if (j < 5)
            {
                MUST_SUCCEED_ASSERT((errDot < CRITICAL_ERROR_THRESHOLD) &&
                                    (errCross < CRITICAL_ERROR_THRESHOLD), 1);
            }
            else
            {
                // FX32_ONE << 5 以上の長さの場合、CrossProduct の結果に多量の誤差が生じます
                // したがってこの試行は失敗するはずです
                // なお、同じ長さの正反対を向く平行ベクタに対しては、より大きな値でも計算可能です。
                // CrossProduct 内部の更なる調査が期待されます。
                MUST_FAIL_ASSERT((errDot < CRITICAL_ERROR_THRESHOLD) &&
                                 (errCross < CRITICAL_ERROR_THRESHOLD), 2);
            }

            // 詳細な情報の出力
            {
                OutDetail("------------ input ---------------------\n");
                printVecFx32Msg("v1                      = ", &v1);
                printVecFx32Msg("v2(perpendicular to v1) = ", &v2);
                printVecFx32Msg("v3(parallel To v1)      = ", &v3);

                OutDetail("v1 . v2 Error   = %.10f \n", errDot);
                OutDetail("v1 X v3 Error   = %.10f \n", errCross);
            }
        }
    }

    OS_Printf("==================== VEC_DotCrossProduct_test_ done ====================\n");
}


//------------------------------------------------------------------------------
// 基本演算関数 テスト
// 固定小数用演算関数と double で行った演算との誤差をデバックコンソールに表示します
// 
static void BasicOperator_test_()
{
    int     i = 0, j = 0;
    fx32    vf1 = 0x0, vf2 = 0x0;

    OS_Printf("==================== BasicOperator_test_ begin ====================\n");

    // unary_operators
    OS_Printf("------------ unary_operators begin ---------------------\n");
    for (i = 0; i < 32; i += 4)
    {
        vf1 += 0x1 << i;
        Unary_operator_test_(vf1);
    }
    OS_Printf("------------ unary_operators done ---------------------\n");
    // binary_operators
    OS_Printf("------------ binary_operators begin ---------------------\n");
    OS_Printf("-------- Some Warning is displayed \n");
    vf1 = 0x0;
    for (i = 0; i < 32; i += 4)
    {
        vf1 += 0x1 << i;
        vf2 = 0;
        for (j = 0; j < 32; j++)
        {
            vf2 += 0x1 << j;
            Binary_operator_test_(vf1, vf2);
        }
    }
    OS_Printf("------------ binary_operators done ---------------------\n");

    OS_Printf("==================== BasicOperator_test_ done ====================\n");
}



//------------------------------------------------------------------------------
// 固定小数 単項演算関数 テスト
// 入力値をパラメータとして、
// 演算を行い double で行った演算との誤差をデバックコンソールに表示します
// 
static void Unary_operator_test_(fx32 vf1)
{
    double  vd1 = (double)FX_FX32_TO_F32(vf1);

    double  diff = 0.0;

    // input fixed point value 
    OutDetail("                 v = %.10f \n", vd1);

    // FX_Sqrt
    diff = fabs(FX2D(FX_Sqrt(vf1)) - sqrt(vd1));
    OutDetail("sqrt( v1 )   Error = %.10f \n", diff);

    // FX_InvSqrt
    diff = fabs(FX2D(FX_InvSqrt(vf1)) - 1.0 / sqrt(vd1));
    OutDetail("InvSqrt( v1 )Error = %.10f \n", diff);

    // FX_Inv
    diff = fabs(FX2D(FX_Inv(vf1)) - 1.0 / vd1);
    OutDetail("1.0 / v1     Error = %.10f \n", diff);

    // FX_InvFx64c
    diff = fabs(FX64C2D(FX_InvFx64c(vf1)) - 1.0 / vd1);
    OutDetail("1.0 / v1 _64 Error = %.10f \n", diff);

}

//------------------------------------------------------------------------------
// 固定小数 2項演算関数 テスト
// 入力値をパラメータとして、
// 演算を行い double で行った演算との誤差をデバックコンソールに表示します
//
static void Binary_operator_test_(fx32 vf1, fx32 vf2)
{
    double  vd1 = (double)FX_FX32_TO_F32(vf1);
    double  vd2 = (double)FX_FX32_TO_F32(vf2);
    double  diff = 0.0;

    // input fixed point value 
    OutDetail("v1 = %.10f, v2 = %.10f\n", vd1, vd2);

    // FX_Mul
    diff = fabs(FX2D(FX_Mul(vf1, vf2)) - vd1 * vd2);
    OutDetail("v1 * v2     Error = %.10f \n", diff);

    // FX_Div
    diff = fabs(FX2D(FX_Div(vf1, vf2)) - vd1 / vd2);
    OutDetail("v1 / v2     Error = %.10f \n", diff);

    // FX_Mul32x64c
    diff = fabs(FX2D(FX_Mul32x64c(vf1, FX_FX32_TO_FX64C(vf2))) - vd1 * vd2);
    OutDetail("v1 * v2 _64 Error = %.10f \n", diff);

    // FX_DivFx64c
    diff = fabs(FX64C2D(FX_DivFx64c(vf1, vf2)) - vd1 / vd2);
    OutDetail("v1 / v2 _64 Error = %.10f \n", diff);

}
