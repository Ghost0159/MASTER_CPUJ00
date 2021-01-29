/*---------------------------------------------------------------------------*
  Project:  NITRO-System - demos - g3d - demolib - include - g3d_demolib
  File:     system.h

  Copyright 2004-2007 Nintendo.  All rights reserved.

  These coded instructions, statements, and computer programs contain
  proprietary information of Nintendo of America Inc. and/or Nintendo
  Company Ltd., and are protected by Federal copyright law.  They may
  not be disclosed to third parties or copied or duplicated in any form,
  in whole or in part, without the prior written consent of Nintendo.

  $Revision: 1.4 $
 *---------------------------------------------------------------------------*/

#ifndef NNS_G3DDEMO_SYSTEM_H_
#define NNS_G3DDEMO_SYSTEM_H_

#include <nitro.h>
#include <nnsys/fnd.h>

#ifdef __cplusplus
extern "C" {
#endif


/*---------------------------------------------------------------------------*
	システム関係。

 *---------------------------------------------------------------------------*/

extern NNSFndHeapHandle G3DDemo_SysHeap;		// システムヒープ
extern NNSFndHeapHandle G3DDemo_AppHeap;		// アプリケーションヒープ

void G3DDemo_InitSystem(void);
void G3DDemo_InitVRAM(void);
void G3DDemo_InitMemory(void);
void G3DDemo_VBlankIntr(void);

void* G3DDemo_LoadFile(const char *path);


/*---------------------------------------------------------------------------*
	ゲームパッド関係。

 *---------------------------------------------------------------------------*/

// ゲームパッド構造体
typedef struct
{
	u16		trigger;
	u16		release;
	u16		button;
}
G3DDemoGamePad;

// ゲームパッドワーク。
extern G3DDemoGamePad	G3DDemo_GamePad;

#define G3DDEMO_IS_PAD_TRIGGER(key)		(G3DDemo_GamePad.trigger & key)
#define G3DDEMO_IS_PAD_RELEASE(key)		(G3DDemo_GamePad.release & key)
#define G3DDEMO_IS_PAD_PRESS(key)		(G3DDemo_GamePad.button  & key)

void G3DDemo_ReadGamePad(void);


/*---------------------------------------------------------------------------*
	ユーティリティ。

 *---------------------------------------------------------------------------*/
#define G3DDEMO_ROUND_UP(value, alignment) \
    (((u32)(value) + (alignment-1)) & ~(alignment-1))

#define G3DDEMO_ROUND_DOWN(value, alignment) \
    ((u32)(value) & ~(alignment-1))


#ifdef __cplusplus
}/* extern "C" */
#endif

// NNS_G3DDEMO_SYSTEM_H_
#endif
