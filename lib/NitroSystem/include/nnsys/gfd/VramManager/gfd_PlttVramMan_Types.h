/*---------------------------------------------------------------------------*
  Project:  NITRO-System - include - nnsys - gfd - VramManager
  File:     gfd_PlttVramMan_Types.h

  Copyright 2004-2007 Nintendo.  All rights reserved.

  These coded instructions, statements, and computer programs contain
  proprietary information of Nintendo of America Inc. and/or Nintendo
  Company Ltd., and are protected by Federal copyright law.  They may
  not be disclosed to third parties or copied or duplicated in any form,
  in whole or in part, without the prior written consent of Nintendo.

  $Revision: 1.15 $
 *---------------------------------------------------------------------------*/
#ifndef NNS_GFD_PLTTVRAMMAN_TYPES_H_
#define NNS_GFD_PLTTVRAMMAN_TYPES_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <nitro.h>
#include <nnsys/gfd/gfd_common.h>

#define NNS_GFD_PLTTMASK_16BIT              0xFFFF


#define NNS_GFD_PLTTKEY_SIZE_SHIFT 3
#define NNS_GFD_PLTTKEY_ADDR_SHIFT 3

#define NNS_GFD_PLTTSIZE_MIN ( 0x1 << NNS_GFD_PLTTKEY_SIZE_SHIFT )
#define NNS_GFD_PLTTSIZE_MAX ( NNS_GFD_PLTTMASK_16BIT << NNS_GFD_PLTTKEY_SIZE_SHIFT )


#define NNS_GFD_ALLOC_ERROR_PLTTKEY         (u32)0x0
#define NNS_GFD_4PLTT_MAX_ADDR              0x10000



//------------------------------------------------------------------------------
//
// NNSGfdPlttKey:
// 32bit�̒l�ŁA8�o�C�g�P�ʂŃe�N�X�`���p���b�g�X���b�g�̗̈���w��ł���B
// 0-0xffff�̒l�̓G���[�l�Ƃ��ė��p���邱�Ƃ��ł���i�T�C�Y��0�Ȃ��߁j
//
// 31                    16  15                         0
// 3bit�E�V�t�g���ꂽ�T�C�Y  3bit�E�V�t�g���ꂽ�I�t�Z�b�g
//------------------------------------------------------------------------------
typedef u32 NNSGfdPlttKey;


//------------------------------------------------------------------------------
// NNSGfdFuncAllocPlttVram
// szByte:  �m�ۂ���T�C�Y���o�C�g�P�ʂŎw��
// is4pltt: 4�F�p���b�g�̊i�[���\�łȂ���΂����Ȃ����ǂ���
// opt:     �����ˑ��̈���(ex �O����m�ۂ���Ƃ���납��m�ۂ���Ƃ�)
//------------------------------------------------------------------------------
typedef NNSGfdPlttKey (*NNSGfdFuncAllocPlttVram)(u32 szByte, BOOL is4pltt, u32 opt);




//------------------------------------------------------------------------------
// NNSGfdFuncFreePlttVram
// �L�[���w�肵�ăe�N�X�`���C���[�W�X���b�g�̈�̉�����s���B
// is4pltt�̎w������Ȃ��Ă��B���ɂȂ�Ȃ��悤�Ȏ����ł���K�v������B
// �Ԃ�l��0�Ő��탊�^�[���B���̑��͎����ˑ��̃G���[�B
//------------------------------------------------------------------------------
typedef int (*NNSGfdFuncFreePlttVram)(NNSGfdPlttKey plttKey);


//------------------------------------------------------------------------------
//
// ���[�U�[�ɂ���ĕύX����邱�Ƃ�����B
//
//------------------------------------------------------------------------------
extern NNSGfdFuncAllocPlttVram  NNS_GfdDefaultFuncAllocPlttVram; 
extern NNSGfdFuncFreePlttVram   NNS_GfdDefaultFuncFreePlttVram;  


//------------------------------------------------------------------------------
//
// ���C�u�����R�[�h�͂��̊֐��o�R�ł̂݃A�N�Z�X���邱�ƂɂȂ�܂��B
//
//------------------------------------------------------------------------------
NNS_GFD_INLINE NNSGfdPlttKey
NNS_GfdAllocPlttVram(u32 szByte, BOOL is4pltt, u32 opt)
{
    return (*NNS_GfdDefaultFuncAllocPlttVram)( szByte, is4pltt, opt );
}

NNS_GFD_INLINE int
NNS_GfdFreePlttVram(NNSGfdPlttKey plttKey)
{
    return (*NNS_GfdDefaultFuncFreePlttVram)(plttKey);
}


//------------------------------------------------------------------------------
//
// NNSGfdPlttKey ���� �֘A
//
//
//------------------------------------------------------------------------------


//------------------------------------------------------------------------------
// NNSGfdPlttKey �� �\���\�Ȃ悤�ɁA�؂�グ���T�C�Y���擾���܂��B
NNS_GFD_INLINE u32
NNSi_GfdGetPlttKeyRoundupSize( u32 size )
{
    if( size == 0 )
    {
        return NNS_GFD_PLTTSIZE_MIN;
    }else{
        return (((u32)(size) + ( NNS_GFD_PLTTSIZE_MIN-1 )) & ~( NNS_GFD_PLTTSIZE_MIN-1 ) );
    }
}

//------------------------------------------------------------------------------
NNS_GFD_INLINE NNSGfdPlttKey
NNS_GfdMakePlttKey( u32 addr, u32 size )
{
    // �؎̂Č덷���������Ă��Ȃ����H
    SDK_ASSERT( (addr & (u32)((0x1 << NNS_GFD_PLTTKEY_ADDR_SHIFT) - 1 )) == 0 );
    SDK_ASSERT( (size & (u32)((0x1 << NNS_GFD_PLTTKEY_SIZE_SHIFT) - 1 )) == 0 );
    
    // �I�[�o�[�t���[�͔������Ă��Ȃ����H
    SDK_ASSERT( ( (size >> NNS_GFD_PLTTKEY_SIZE_SHIFT) & ~NNS_GFD_PLTTMASK_16BIT ) == 0 );
    SDK_ASSERT( ( (addr >> NNS_GFD_PLTTKEY_ADDR_SHIFT) & ~NNS_GFD_PLTTMASK_16BIT ) == 0 );
    
    return    ( ( size >> NNS_GFD_PLTTKEY_SIZE_SHIFT ) << 16 ) 
            | ( 0xFFFF & ( addr >> NNS_GFD_PLTTKEY_ADDR_SHIFT ) );
}

//------------------------------------------------------------------------------
NNS_GFD_INLINE u32
NNS_GfdGetPlttKeyAddr( NNSGfdPlttKey plttKey )
{
    return (u32)( ( 0x0000FFFF & plttKey )  << NNS_GFD_PLTTKEY_ADDR_SHIFT );
}

//------------------------------------------------------------------------------
NNS_GFD_INLINE u32
NNS_GfdGetPlttKeySize( NNSGfdPlttKey plttKey )
{
    return (u32)( ( ( 0xFFFF0000 & plttKey ) >> 16 ) << NNS_GFD_PLTTKEY_SIZE_SHIFT );
}


#ifdef __cplusplus
} /* extern "C" */
#endif

#endif // NNS_GFD_PLTTVRAMMAN_TYPES_H_
