/*---------------------------------------------------------------------------*
  Project:  NITRO-System - include - nnsys - gfd - VramManager
  File:     gfd_VramMan.h

  Copyright 2004-2007 Nintendo.  All rights reserved.

  These coded instructions, statements, and computer programs contain
  proprietary information of Nintendo of America Inc. and/or Nintendo
  Company Ltd., and are protected by Federal copyright law.  They may
  not be disclosed to third parties or copied or duplicated in any form,
  in whole or in part, without the prior written consent of Nintendo.

  $Revision: 1.12 $
 *---------------------------------------------------------------------------*/
#ifndef NNS_GFD_VRAMMAN_H_
#define NNS_GFD_VRAMMAN_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <nitro.h>

//
// �f�o�b�N�o�͂�L���ɂ���X�C�b�`
// ��`���Ƀf�o�b�N�o�͂��L���ɂȂ�܂�
//
//#define NNS_GFD_DEBUG


#ifdef SDK_DEBUG

    #define NNS_GFD_PRINTF( ... )   OS_Printf( __VA_ARGS__ )
    #define NNS_GFD_WARNING( ... )  OS_Warning( __VA_ARGS__ )

#else // SDK_DEBUG

    #define NNS_GFD_PRINTF( ... )   ((void) 0)
    #define NNS_GFD_WARNING( ... )  ((void) 0)

#endif // SDK_DEBUG
//------------------------------------------------------------------------------
#ifdef NNS_GFD_DEBUG
    #define NNS_GFD_DBGMSG( ... )   OS_Printf( __VA_ARGS__ )
#else // NNS_GFD_DEBUG
    #define NNS_GFD_DBGMSG( ... )   ((void) 0)
#endif // NNS_GFD_DEBUG


//
//	VRAM���烁�������m�ۂ�������������萔�B
// ----------------------------------------------------------------------------
enum
{
	NNS_GFD_ALLOC_FROM_HIGH		= 0,		// VRAM�̏�ʂ��烁�������m�ہB
	NNS_GFD_ALLOC_FROM_LOW		= 1			// VRAM�̉��ʂ��烁�������m�ہB
};


#ifdef __cplusplus
} /* extern "C" */
#endif

#endif // NNS_GFD_VRAMMAN_H_
