/*---------------------------------------------------------------------------*
  Project:  NITRO-System - demos - fnd - archive
  File:     nns_util.h

  Copyright 2004-2007 Nintendo.  All rights reserved.

  These coded instructions, statements, and computer programs contain
  proprietary information of Nintendo of America Inc. and/or Nintendo
  Company Ltd., and are protected by Federal copyright law.  They may
  not be disclosed to third parties or copied or duplicated in any form,
  in whole or in part, without the prior written consent of Nintendo.

  $Revision: 1.6 $
 *---------------------------------------------------------------------------*/

#ifndef NNS_UTIL_H_
#define NNS_UTIL_H_

#include <nitro.h>
#include <nnsys/fnd.h>


#ifdef __cplusplus
extern "C" {
#endif

typedef struct
{
	u16		trigger;
	u16		release;
	u16		button;
}
GamePad;

extern GamePad			gGamePad;
extern NNSFndHeapHandle gSysHeap;		// システムヒープ
extern NNSFndHeapHandle gAppHeap;		// アプリケーションヒープ


#define IS_TRIGGER(key)		(gGamePad.trigger & key)
#define IS_RELEASE(key)		(gGamePad.release & key)
#define IS_PRESS(key)		(gGamePad.button  & key)


#define ROUND_UP(value, alignment) \
    (((u32)(value) + (alignment-1)) & ~(alignment-1))

#define ROUND_DOWN(value, alignment) \
    ((u32)(value) & ~(alignment-1))


void			ReadGamePad(void);
void			InitMemory(void);
NNSFndArchive*	LoadArchive(const char* name, const char* path);
void			RemoveArchive(NNSFndArchive* archive);


#ifdef __cplusplus
}/* extern "C" */
#endif

// NNS_UTIL_H_
#endif
