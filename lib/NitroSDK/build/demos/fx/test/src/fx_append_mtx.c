/*---------------------------------------------------------------------------*
  Project:  NitroSDK - FX - demos - test
  File:     fx_append_mtx.c

  Copyright 2003-2006 Nintendo.  All rights reserved.

  These coded instructions, statements, and computer programs contain
  proprietary information of Nintendo of America Inc. and/or Nintendo
  Company Ltd., and are protected by Federal copyright law.  They may
  not be disclosed to third parties or copied or duplicated in any form,
  in whole or in part, without the prior written consent of Nintendo.
  
  $Log: fx_append_mtx.c,v $
  Revision 1.8  2006/01/18 02:11:21  kitase_hirotake
  do-indent

  Revision 1.7  2005/02/28 05:26:13  yosizaki
  do-indent.

  Revision 1.6  2004/04/07 01:27:57  yada
  fix header comment

  Revision 1.5  2004/03/17 04:39:49  takano_makoto
  Bug fix in MTX_Concat44D_.

  Revision 1.4  2004/02/06 04:42:11  nishida_kenji
  Remove MTX_Trans43.

  Revision 1.3  2004/02/05 07:09:02  yasu
  change SDK prefix iris -> nitro

  Revision 1.2  2004/01/29 01:35:09  kitani_toshikazu
  Fix some bugs for debug build, restore appended test cases.
 
  $NoKeywords: $
    
 *---------------------------------------------------------------------------*/


#ifndef  FX_APPEND_MTX_H_
#include "fx_append_mtx.h"
#endif // FX_APPEND_MTX_H_


//----------------------------------------------------------------------------
//  double Mtx
//----------------------------------------------------------------------------


typedef union
{
    struct
    {
        double  _00, _01, _02, _03;
        double  _10, _11, _12, _13;
        double  _20, _21, _22, _23;
        double  _30, _31, _32, _33;
    };
    double  m[4][4];
    double  a[16];
}
MtxD44;


//----------------------------------------------------------------------------
//  MtxFx43: 4x3 Matrix of fx32
//----------------------------------------------------------------------------



typedef union
{
    struct
    {
        double  _00, _01, _02;
        double  _10, _11, _12;
        double  _20, _21, _22;
        double  _30, _31, _32;
    };
    double  m[4][3];
    double  a[12];
}
MtxD43;

const static MtxD43 s_mtxIdentity43D = {
    1.0, 0, 0,
    0, 1.0, 0,
    0, 0, 1.0,
    0, 0, 0
};

#define NUM_MTX43_ELEMENT 12
#define NUM_MTX44_ELEMENT 16


//----------------------------------------------------------------------------

static void MtxFxToD_(const fx32 *mf, double *md, int numElement);
static void MTX_Concat43D_(const MtxFx43 *afx, const MtxFx43 *bfx, MtxD43 * ab);
static void MTX_RotY43D_(MtxD43 * pDst, fx32 sinVal, fx32 cosVal);
static void MTX_RotZ43D_(MtxD43 * pDst, fx32 sinVal, fx32 cosVal);
static void MTX_Translate43D_(MtxD43 * pDst, fx32 x, fx32 y, fx32 z);
static void MTX_Concat44D_(const MtxFx44 *afx, const MtxFx44 *bfx, MtxD44 * ab);

static BOOL IsErrorMtxDValid_(const double *pM, int numElem);
static void GetDifferenceMtx_(const double *a, const double *b, double *diff, int numElement);
static BOOL IsNearlyIdentityMtx_(const MtxFx43 *mtxIn);
static BOOL CheckInvMtxValid_(const MtxFx43 *mtx);


//----------------------------------------------------------------------------
// test methods
static void MTX_concat_43_test_();
static void MTX_concat_44_test_();
static void MTX_inv_43_test_();

static void PrintMtxD_(const double *mtxArray, int numElement);

//----------------------------------------------------------------------------
// 追加テスト：エントリ
void MTXTest_appended()
{
    OS_Printf("**================== MTXTest_appended start ====================\n");
    MTX_concat_43_test_();
    MTX_concat_44_test_();
    MTX_inv_43_test_();
    OS_Printf("**================== MTXTest_appended done ====================\n");
}

//----------------------------------------------------------------------------
// デバックコンソールにMtxDの内容を出力します。
static void PrintMtxD_(const double *mtxArray, int numElement)
{
    int     i = 0;
    SDK_ASSERTMSG((numElement == NUM_MTX43_ELEMENT)
                  || (numElement == NUM_MTX44_ELEMENT), "Wrong Mtx Format");

    for (i = 0; i < numElement; i++)
    {
        if (i % 4 == 0)
            OutDetail("\n");

        OutDetail(" %f", mtxArray[i]);
    }
}

//----------------------------------------------------------------------------
static void MtxFxToD_(const fx32 *mf, double *md, int numElement)
{
    int     i = 0;
    SDK_NULL_ASSERT(mf);
    SDK_NULL_ASSERT(md);
    SDK_ASSERTMSG((numElement == NUM_MTX43_ELEMENT)
                  || (numElement == NUM_MTX44_ELEMENT), "Wrong Mtx Format");

    for (i = 0; i < numElement; i++)
    {
        md[i] = (double)(FX_FX32_TO_F32(mf[i]));
    }
}

//----------------------------------------------------------------------------
static void MTX_Concat43D_(const MtxFx43 *afx, const MtxFx43 *bfx, MtxD43 * ab)
{
    MtxD43  a;
    MtxD43  b;

    register double x, y, z;
    register double xx, yy, zz;

    SDK_NULL_ASSERT(afx);
    SDK_NULL_ASSERT(bfx);
    SDK_NULL_ASSERT(ab);

    MtxFxToD_(afx->a, a.a, NUM_MTX43_ELEMENT);
    MtxFxToD_(bfx->a, b.a, NUM_MTX43_ELEMENT);
    // compute (a x b) -> p

    // row 0
    x = a._00;
    y = a._01;
    z = a._02;

    ab->_00 = x * b._00 + y * b._10 + z * b._20;
    ab->_01 = x * b._01 + y * b._11 + z * b._21;

    xx = b._02;
    yy = b._12;
    zz = b._22;

    ab->_02 = x * xx + y * yy + z * zz;

    // row 1
    x = a._10;
    y = a._11;
    z = a._12;

    ab->_12 = x * xx + y * yy + z * zz;
    ab->_11 = x * b._01 + y * b._11 + z * b._21;

    xx = b._00;
    yy = b._10;
    zz = b._20;

    ab->_10 = x * xx + y * yy + z * zz;

    // row 2
    x = a._20;
    y = a._21;
    z = a._22;

    ab->_20 = x * xx + y * yy + z * zz;
    ab->_21 = x * b._01 + y * b._11 + z * b._21;
    xx = b._02;
    yy = b._12;
    zz = b._22;

    ab->_22 = x * xx + y * yy + z * zz;

    // row 3
    x = a._30;
    y = a._31;
    z = a._32;

    ab->_32 = x * xx + y * yy + z * zz + b._32;
    ab->_31 = x * b._01 + y * b._11 + z * b._21 + b._31;
    ab->_30 = x * b._00 + y * b._10 + z * b._20 + b._30;

}

//----------------------------------------------------------------------------
static void MTX_RotY43D_(MtxD43 * pDst, fx32 sinVal, fx32 cosVal)
{
    SDK_NULL_ASSERT(pDst);

    pDst->_00 = (double)FX_FX32_TO_F32(cosVal);
    pDst->_01 = 0;
    pDst->_02 = (double)FX_FX32_TO_F32(-sinVal);
    pDst->_10 = 0;
    pDst->_11 = 1.0;
    pDst->_12 = 0;
    pDst->_20 = (double)FX_FX32_TO_F32(sinVal);
    pDst->_21 = 0;
    pDst->_22 = (double)FX_FX32_TO_F32(cosVal);
    pDst->_30 = 0;
    pDst->_31 = 0;
    pDst->_32 = 0;
}

//----------------------------------------------------------------------------
static void MTX_RotZ43D_(MtxD43 * pDst, fx32 sinVal, fx32 cosVal)
{
    SDK_NULL_ASSERT(pDst);

    pDst->_00 = (double)FX_FX32_TO_F32(cosVal);
    pDst->_01 = (double)FX_FX32_TO_F32(sinVal);
    pDst->_02 = 0;
    pDst->_10 = (double)FX_FX32_TO_F32(-sinVal);
    pDst->_11 = (double)FX_FX32_TO_F32(cosVal);
    pDst->_12 = 0;
    pDst->_20 = 0;
    pDst->_21 = 0;
    pDst->_22 = 1.0;
    pDst->_30 = 0;
    pDst->_31 = 0;
    pDst->_32 = 0;
}

//----------------------------------------------------------------------------
static void MTX_Translate43D_(MtxD43 * pDst, fx32 x, fx32 y, fx32 z)
{
    SDK_NULL_ASSERT(pDst);

    pDst->_30 = (double)FX_FX32_TO_F32(x);
    pDst->_31 = (double)FX_FX32_TO_F32(y);
    pDst->_32 = (double)FX_FX32_TO_F32(z);
}

//----------------------------------------------------------------------------
// 44
static void MTX_Concat44D_(const MtxFx44 *afx, const MtxFx44 *bfx, MtxD44 * ab)
{
    MtxD44  a;
    MtxD44  b;
    MtxD44 *m = ab;

    register double x, y, z, w;
    register double xx, yy, zz, ww;


    SDK_NULL_ASSERT(afx);
    SDK_NULL_ASSERT(bfx);
    SDK_NULL_ASSERT(ab);

    MtxFxToD_(afx->a, a.a, NUM_MTX44_ELEMENT);
    MtxFxToD_(bfx->a, b.a, NUM_MTX44_ELEMENT);



    // compute (a x b) -> p
    // row 0
    x = a._00;
    y = a._01;
    z = a._02;
    w = a._03;

    ab->_00 = x * b._00 + y * b._10 + z * b._20 + w * b._30;
    ab->_01 = x * b._01 + y * b._11 + z * b._21 + w * b._31;
    ab->_03 = x * b._03 + y * b._13 + z * b._23 + w * b._33;

    xx = b._02;
    yy = b._12;
    zz = b._22;
    ww = b._32;

    ab->_02 = x * xx + y * yy + z * zz + w * ww;

    // row 1
    x = a._10;
    y = a._11;
    z = a._12;
    w = a._13;

    ab->_12 = x * xx + y * yy + z * zz + w * ww;
    ab->_11 = x * b._01 + y * b._11 + z * b._21 + w * b._31;
    ab->_13 = x * b._03 + y * b._13 + z * b._23 + w * b._33;

    xx = b._00;
    yy = b._10;
    zz = b._20;
    ww = b._30;

    ab->_10 = x * xx + y * yy + z * zz + w * ww;

    // row 2
    x = a._20;
    y = a._21;
    z = a._22;
    w = a._23;

    ab->_20 = x * xx + y * yy + z * zz + w * ww;
    ab->_21 = x * b._01 + y * b._11 + z * b._21 + w * b._31;
    ab->_23 = x * b._03 + y * b._13 + z * b._23 + w * b._33;

    xx = b._02;
    yy = b._12;
    zz = b._22;
    ww = b._32;

    ab->_22 = x * xx + y * yy + z * zz + w * ww;

    // row 3
    x = a._30;
    y = a._31;
    z = a._32;
    w = a._33;

    ab->_32 = x * xx + y * yy + z * zz + w * ww;
    ab->_31 = x * b._01 + y * b._11 + z * b._21 + w * b._31;
    ab->_30 = x * b._00 + y * b._10 + z * b._20 + w * b._30;
    ab->_33 = x * b._03 + y * b._13 + z * b._23 + w * b._33;

}

//----------------------------------------------------------------------------
static BOOL IsErrorMtxDValid_(const double *pM, int numElem)
{
    int     i = 0;

    SDK_NULL_ASSERT(pM);
    SDK_ASSERTMSG((numElem == 12) || (numElem == 16), "Wrong Mtx Format");

    for (i = 0; i < numElem; i++)
    {

        if (fabs(pM[i]) > CRITICAL_ERROR_THRESHOLD)
        {
            OutDetail("Error Mtx \n");
            for (i = 0; i < numElem; i++)
            {
                OutDetail("  %f \n", pM[i]);
            }
            return FALSE;
        }
    }

    return TRUE;
}

//----------------------------------------------------------------------------
static void GetDifferenceMtx_(const double *a, const double *b, double *diff, int numElement)
{
    int     i = 0;
    SDK_NULL_ASSERT(a);
    SDK_NULL_ASSERT(b);
    SDK_NULL_ASSERT(diff);

    SDK_ASSERTMSG((numElement == 12) || (numElement == 16), "Wrong Mtx Format");

    for (i = 0; i < numElement; i++)
    {
        diff[i] = fabs(a[i] - b[i]);
    }
}

//----------------------------------------------------------------------------
// 正規行列に近いか（許容誤差以内か）
static BOOL IsNearlyIdentityMtx_(const MtxFx43 *mtxIn)
{
    int     i = 0;
    MtxD43  mtxInD;
    MtxD43  errorMtxD;

    MtxFxToD_(mtxIn->a, mtxInD.a, NUM_MTX43_ELEMENT);

    for (i = 0; i < NUM_MTX43_ELEMENT; i++)
    {
        errorMtxD.a[i] = fabs(s_mtxIdentity43D.a[i] - mtxInD.a[i]);
    }

    return IsErrorMtxDValid_(errorMtxD.a, NUM_MTX43_ELEMENT);
}

//----------------------------------------------------------------------------
static BOOL CheckInvMtxValid_(const MtxFx43 *mtx)
{
    MtxFx43 mtxInv, result;

    int     invResult = MTX_Inverse43(mtx, &mtxInv);


    OutDetail("---------------- mtx\n");
    print_mtx43(mtx);
    OutDetail("---------------- Invmtx\n");
    print_mtx43(&mtxInv);

    SDK_ASSERTMSG(invResult == 0, "Failure in MTX_Inverse43() that must be successful");
    MTX_Concat43(mtx, &mtxInv, &result);

    // 単位行列に近いか？
    // 差をとり絶対値をチェックする
    return IsNearlyIdentityMtx_(&result);
}

//----------------------------------------------------------------------------
static void MTX_concat_43_test_()
{
    const MtxFx43 src43 = {
        FX32_ONE * 1, FX32_ONE * 2, FX32_ONE * 3,
        FX32_ONE * 4, FX32_ONE * 5, FX32_ONE * 6,
        FX32_ONE * 7, FX32_ONE * 8, FX32_ONE * 9,
        FX32_ONE * 10, FX32_ONE * 11, FX32_ONE * 12
    };

    MtxD43  resD;
    MtxFx43 resFx;
    MtxD43  resFxD;
    MtxD43  resDiff;

    OS_Printf("==================== MTX_concat_43_test_ start ====================\n");

    MTX_Concat43D_(&src43, &src43, &resD);
    MTX_Concat43(&src43, &src43, &resFx);

    MtxFxToD_(resFx.a, resFxD.a, NUM_MTX43_ELEMENT);


    PrintMtxD_(resFxD.a, NUM_MTX43_ELEMENT);
    PrintMtxD_(resD.a, NUM_MTX43_ELEMENT);


    GetDifferenceMtx_(resD.a, resFxD.a, resDiff.a, NUM_MTX43_ELEMENT);

    MUST_SUCCEED_ASSERT(IsErrorMtxDValid_(resDiff.a, NUM_MTX43_ELEMENT), 1);
    OS_Printf("==================== MTX_concat_43_test_ done ====================\n");
}

//----------------------------------------------------------------------------
static void MTX_concat_44_test_()
{
    const MtxFx44 src44 = {
        FX32_ONE * 1, FX32_ONE * 2, FX32_ONE * 3, FX32_ONE * 4,
        FX32_ONE * 5, FX32_ONE * 6, FX32_ONE * 7, FX32_ONE * 8,
        FX32_ONE * 9, FX32_ONE * 10, FX32_ONE * 11, FX32_ONE * 12,
        FX32_ONE * 13, FX32_ONE * 14, FX32_ONE * 15, FX32_ONE * 16
    };

    MtxD44  resD;
    MtxFx44 resFx;
    MtxD44  resFxD;
    MtxD44  resDiff;

    OS_Printf("==================== MTX_concat_44_test_ start ====================\n");

    MTX_Concat44D_(&src44, &src44, &resD);
    MTX_Concat44(&src44, &src44, &resFx);

    MtxFxToD_(resFx.a, resFxD.a, NUM_MTX44_ELEMENT);

    GetDifferenceMtx_(resD.a, resFxD.a, resDiff.a, NUM_MTX44_ELEMENT);


    MUST_SUCCEED_ASSERT(IsErrorMtxDValid_(resDiff.a, NUM_MTX44_ELEMENT), 1);
    OS_Printf("==================== MTX_concat_44_test_ done ====================\n");
}

//----------------------------------------------------------------------------
static void MTX_inv_43_test_()
{
    int     radY;
    int     radZ;

    const int STEP_RAD = 256;
    const int MAX_RAD = 65535;

    MtxFx43 rotYFx, rotZFx, mtxFx, mtxFx2;

    OS_Printf("==================== MTX_inv_43_test_ start ====================\n");
    // さまざまな回転行列にたいして、逆行列の計算誤差を確認する
    for (radY = 0; radY < MAX_RAD; radY += STEP_RAD)
    {
        MTX_RotY43(&rotYFx, FX_SinIdx(radY), FX_CosIdx(radY));
        for (radZ = 0; radZ < MAX_RAD; radZ += STEP_RAD)
        {
            MTX_RotZ43(&rotZFx, FX_SinIdx(radZ), FX_CosIdx(radZ));
            MTX_Concat43(&rotYFx, &rotZFx, &mtxFx);
            MTX_TransApply43(&mtxFx, &mtxFx, FX32_ONE, FX32_ONE, FX32_ONE);
            MTX_TransApply43(&mtxFx, &mtxFx2, FX32_ONE, FX32_ONE, FX32_ONE);

            // 有効かチェックする
            // 必ず有効なはずである
            MUST_SUCCEED_ASSERT(CheckInvMtxValid_(&mtxFx), 1);
        }
    }

    // TODO:行列式がゼロに近いもの

    OS_Printf("==================== MTX_inv_43_test_ done ====================\n");
}
