/*---------------------------------------------------------------------------*
  Project:  NITRO-System - demos - IDE - IDE_G2dDemoLib - include - g2d_demolib
  File:     system.h

  Copyright 2004-2007 Nintendo.  All rights reserved.

  These coded instructions, statements, and computer programs contain
  proprietary information of Nintendo of America Inc. and/or Nintendo
  Company Ltd., and are protected by Federal copyright law.  They may
  not be disclosed to third parties or copied or duplicated in any form,
  in whole or in part, without the prior written consent of Nintendo.

  $Revision: 1.3 $
 *---------------------------------------------------------------------------*/
#ifndef G2D_DEMO_SYSTEM_H_
#define G2D_DEMO_SYSTEM_H_

#ifdef __cplusplus
extern "C" {
#endif


/*---------------------------------------------------------------------------*
    初期化関係。
 *---------------------------------------------------------------------------*/

void G2DDemo_CommonInit(void);
void G2DDemo_CameraSetup(void);
void G2DDemo_MaterialSetup(void);


/*---------------------------------------------------------------------------*
    ゲームパッド関係。
 *---------------------------------------------------------------------------*/

// ゲームパッド構造体
typedef struct
{
    u16     trigger;
    u16     release;
    u16     button;
}
G2DDemoGamePad;

// ゲームパッドワーク。
extern G2DDemoGamePad   gGamePad;

#define G2DDEMO_IS_TRIGGER(key)     (gGamePad.trigger & key)
#define G2DDEMO_IS_RELEASE(key)     (gGamePad.release & key)
#define G2DDEMO_IS_PRESS(key)       (gGamePad.button  & key)

void G2DDemo_ReadGamePad(void);


/*---------------------------------------------------------------------------*
    その他
 *---------------------------------------------------------------------------*/

extern NNSFndHeapHandle       gHeapHandle;      // ヒープハンドル

NNSG2dCellAnimation*          G2DDemo_GetNewCellAnimation( u16 num );
NNSG2dNode*                   G2DDemo_GetNewNode( u16 num );

/*
NNSG2dMultiCellAnimation*     G2DDemo_GetNewMultiCellAnimation
(
    u16                                numNode,
    const NNSG2dCellAnimBankData*      pAnimBank,
    const NNSG2dCellDataBank*          pCellDataBank ,
    const NNSG2dMultiCellDataBank*     pMultiCellDataBank
);
*/

NNSG2dMultiCellAnimation*     G2DDemo_GetNewMultiCellAnimation
(
    const NNSG2dCellAnimBankData*      pAnimBank,
    const NNSG2dCellDataBank*          pCellDataBank ,
    const NNSG2dMultiCellDataBank*     pMultiCellDataBank,
    NNSG2dMCType                       mcType
);

static inline void G2DDemo_MTX_Rot22( MtxFx22* m, fx32 sinVal, fx32 cosVal )
{
    SDK_NULL_ASSERT( m );
    m->_00 = cosVal;
    m->_01 = sinVal;
    m->_10 = -sinVal;
    m->_11 = cosVal;
}

inline void* G2DDemo_Alloc(u32 size)
{
	SDK_ASSERT( size > 0 );
    return NNS_FndAllocFromExpHeap(gHeapHandle, size);
}

inline void G2DDemo_Free(void* memBlock)
{
	SDK_ASSERT( NNS_FndFindContainHeap(memBlock) == gHeapHandle );
    NNS_FndFreeToExpHeap(gHeapHandle, memBlock);
}

extern void VBlankIntr( void );


#ifdef __cplusplus
}/* extern "C" */
#endif

// G2D_DEMO_SYSTEM_H_
#endif
