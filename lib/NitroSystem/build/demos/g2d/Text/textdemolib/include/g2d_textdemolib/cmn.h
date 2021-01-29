/*---------------------------------------------------------------------------*
  Project:  NITRO-System - demos - g2d - Text - textdemolib - include - g2d_textdemolib
  File:     cmn.h

  Copyright 2004-2007 Nintendo.  All rights reserved.

  These coded instructions, statements, and computer programs contain
  proprietary information of Nintendo of America Inc. and/or Nintendo
  Company Ltd., and are protected by Federal copyright law.  They may
  not be disclosed to third parties or copied or duplicated in any form,
  in whole or in part, without the prior written consent of Nintendo.

  $Revision: 1.2 $
 *---------------------------------------------------------------------------*/
#ifndef CMN_H_
#define CMN_H_


#include <nitro/types.h>
#include <nitro/os/common/interrupt.h>
#include <nitro/os/common/arena.h>
#include <nitro/os/common/alloc.h>
#include <nnsys/fnd/allocator.h>
#include <nnsys/fnd/archive.h>

#ifdef __cplusplus
extern "C" {
#endif



#define ARY_SIZEOF(ary)     ( sizeof(ary) / sizeof( (ary)[0] ) )
#define ROUNDUP_DIV(a, b)   (( (a) + ((b) - 1) ) / (b))

const static float ONE_FRAME_US = 16715.1;  // 1フレームは 16715.1 us
const static float ONE_VBLANK_US = 4512.4;  // Vブランク期間は 4512.4 us


typedef struct CMNGamePad
{
    u16     trigger;
    u16     release;
    u16     button;
}
CMNGamePad;



extern CMNGamePad CMNGamePadState;



void CMN_InitInterrupt( void );
void CMN_BeginVBlankIntr( OSIrqFunction vBlankFunc );
void CMN_InitAllocator( NNSFndAllocator* pAllocator );
void CMN_InitFileSystem( NNSFndAllocator* pAllocator );
void CMN_ClearVram( void );
void CMN_ReadGamePad(void);
u32 CMN_LoadFile(void** ppFile, const char* fpath, NNSFndAllocator* pAlloc);
void CMN_UnloadFile(void* pFile, NNSFndAllocator* pAlloc);
NNSFndArchive* CMN_LoadArchive(const char* name, const char* path, NNSFndAllocator* pAllocator);
void CMN_RemoveArchive(NNSFndArchive* archive, NNSFndAllocator* pAllocator);


static inline u16 CMN_IsTrigger(u16 key)
{
    return (u16)(CMNGamePadState.trigger & key);
}
static inline u16 CMN_IsRelease(u16 key)
{
    return (u16)(CMNGamePadState.release & key);
}
static inline u16 CMN_IsPress(u16 key)
{
    return (u16)(CMNGamePadState.button & key);
}

static inline void CMN_WaitVBlankIntr(void)
{
    OS_WaitIrq(TRUE, OS_IE_V_BLANK);
}

static inline void CMN_SetPlaneVisible(GXPlaneMask plane)
{
    GX_SetVisiblePlane(GX_GetVisiblePlane() | plane);
}

static inline void CMN_SetPlaneInvisible(GXPlaneMask plane)
{
    GX_SetVisiblePlane(GX_GetVisiblePlane() & ~plane);
}





#ifdef __cplusplus
}/* extern "C" */
#endif

#endif // CMN_H_

