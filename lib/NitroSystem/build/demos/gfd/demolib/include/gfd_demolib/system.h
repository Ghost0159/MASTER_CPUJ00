/*---------------------------------------------------------------------------*
  Project:  NITRO-System - demos - gfd - demolib - include - gfd_demolib
  File:     system.h

  Copyright 2004-2007 Nintendo.  All rights reserved.

  These coded instructions, statements, and computer programs contain
  proprietary information of Nintendo of America Inc. and/or Nintendo
  Company Ltd., and are protected by Federal copyright law.  They may
  not be disclosed to third parties or copied or duplicated in any form,
  in whole or in part, without the prior written consent of Nintendo.

  $Revision: 1.5 $
 *---------------------------------------------------------------------------*/

#ifndef NNS_GFDDEMO_SYSTEM_H_
#define NNS_GFDDEMO_SYSTEM_H_

#include <nitro.h>
#include <nnsys/fnd.h>

#ifdef __cplusplus
extern "C" {
#endif

void    GFDDemo_CommonInit(void);
void*   GfDDemo_Allock( u32 size );
void    GfDDemo_Free( void* pBlk );

#ifdef __cplusplus
}/* extern "C" */
#endif


#endif // NNS_GFDDEMO_SYSTEM_H_

