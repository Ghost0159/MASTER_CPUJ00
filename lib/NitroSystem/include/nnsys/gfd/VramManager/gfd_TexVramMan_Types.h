/*---------------------------------------------------------------------------*
  Project:  NITRO-System - include - nnsys - gfd - VramManager
  File:     gfd_TexVramMan_Types.h

  Copyright 2004-2007 Nintendo.  All rights reserved.

  These coded instructions, statements, and computer programs contain
  proprietary information of Nintendo of America Inc. and/or Nintendo
  Company Ltd., and are protected by Federal copyright law.  They may
  not be disclosed to third parties or copied or duplicated in any form,
  in whole or in part, without the prior written consent of Nintendo.

  $Revision: 1.12 $
 *---------------------------------------------------------------------------*/
#ifndef NNS_GFD_TEXVRAMMAN_TYPES_H_
#define NNS_GFD_TEXVRAMMAN_TYPES_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <nitro.h>
#include <nnsys/gfd/gfd_common.h>

#define NNS_GFD_MASK_15BIT 0x7FFF
#define NNS_GFD_MASK_16BIT 0xFFFF

#define NNS_GFD_TEXKEY_SIZE_SHIFT 4
#define NNS_GFD_TEXKEY_ADDR_SHIFT 3


#define NNS_GFD_TEXSIZE_MIN ( 0x1 << NNS_GFD_TEXKEY_SIZE_SHIFT )
#define NNS_GFD_TEXSIZE_MAX ( 0x7FFF << NNS_GFD_TEXKEY_SIZE_SHIFT )




// �A���[�P�[�g�Ɏ��s�����s���ȃe�N�X�`���L�[
#define NNS_GFD_ALLOC_ERROR_TEXKEY          (u32)0x0


//------------------------------------------------------------------------------
//
// NNSGfdTexKey:
// 32bit�̒l�ŁA8�o�C�g�P�ʂŃe�N�X�`���C���[�W�X���b�g�̗̈���w��ł���B
// 0-0xffff�̒l�̓G���[�l�Ƃ��ė��p���邱�Ƃ��ł���i�T�C�Y��0�Ȃ��߁j
//
// 31      30                    17  16                         0    
// 4x4Comp 4bit�E�V�t�g���ꂽ�T�C�Y  3bit�E�V�t�g���ꂽ�I�t�Z�b�g    
//
//------------------------------------------------------------------------------
typedef u32 NNSGfdTexKey;

//------------------------------------------------------------------------------
// NNSGfdFuncAllocTexVram
// szByte:    �m�ۂ���T�C�Y���o�C�g�P�ʂŎw��
// is4x4comp: 4x4comp�t�H�[�}�b�g�p���ǂ���
// opt:       �����ˑ��̈���(ex �O����m�ۂ���Ƃ���납��m�ۂ���Ƃ�)
//
// is4x4comp��TRUE�̏ꍇ�A�Ԃ�l�̓e�N�X�`���C���[�W�̗̈�ɂȂ�B
// �e�N�X�`���p���b�g�C���f�b�N�X�̗̈���ʓr�m�ۂ���Ă��Ȃ���΂Ȃ�Ȃ��B
//------------------------------------------------------------------------------
typedef NNSGfdTexKey (*NNSGfdFuncAllocTexVram)(u32 szByte, BOOL is4x4comp, u32 opt);




//------------------------------------------------------------------------------
// NNSGfdFuncFreeTexVram
// �L�[���w�肵�ăe�N�X�`���C���[�W�X���b�g�̈�̉�����s���B
// is4x4comp�̎w������Ȃ��Ă��B���ɂȂ�Ȃ��悤�Ȏ����ł���K�v������B
// �Ԃ�l��0�Ő��탊�^�[���B���̑��͎����ˑ��̃G���[�B
//------------------------------------------------------------------------------
typedef int (*NNSGfdFuncFreeTexVram)(NNSGfdTexKey key);


//------------------------------------------------------------------------------
//
// ���[�U�[�ɂ���ĕύX����邱�Ƃ�����B
//
//------------------------------------------------------------------------------
extern NNSGfdFuncAllocTexVram   NNS_GfdDefaultFuncAllocTexVram; 
extern NNSGfdFuncFreeTexVram    NNS_GfdDefaultFuncFreeTexVram;  


//------------------------------------------------------------------------------
//
// �}�N���݂����Ȃ���
// ���C�u�����R�[�h�͂��̊֐��o�R�ł̂݃A�N�Z�X���邱�ƂɂȂ�B
//
//------------------------------------------------------------------------------
NNS_GFD_INLINE NNSGfdTexKey
NNS_GfdAllocTexVram(u32 szByte, BOOL is4x4comp, u32 opt)
{
    return (*NNS_GfdDefaultFuncAllocTexVram)(szByte, is4x4comp, opt );
}

//------------------------------------------------------------------------------
//
// �}�N���݂����Ȃ���
// ���C�u�����R�[�h�͂��̊֐��o�R�ł̂݃A�N�Z�X���邱�ƂɂȂ�B
//
//------------------------------------------------------------------------------
NNS_GFD_INLINE int
NNS_GfdFreeTexVram(NNSGfdTexKey memKey)
{
    return (*NNS_GfdDefaultFuncFreeTexVram)(memKey);
}



//------------------------------------------------------------------------------
//
// NNSGfdTexKey ���� �֘A
//
//
//------------------------------------------------------------------------------




//------------------------------------------------------------------------------
// NNSGfdTexKey �� �\���\�Ȃ悤�ɁA�؂�グ���T�C�Y���擾���܂��B
NNS_GFD_INLINE u32
NNSi_GfdGetTexKeyRoundupSize( u32 size )
{
    if( size == 0 )
    {
        return NNS_GFD_TEXSIZE_MIN;
    }else{
        
        return (((u32)(size) + ( NNS_GFD_TEXSIZE_MIN-1 )) & ~( NNS_GFD_TEXSIZE_MIN-1 ) );
    }
}    

//------------------------------------------------------------------------------
NNS_GFD_INLINE NNSGfdTexKey
NNS_GfdMakeTexKey( u32 addr, u32 size, BOOL b4x4Comp )
{
    // �؎̂Č덷���������Ă��Ȃ����H
    SDK_ASSERT( (addr & (u32)((0x1 << NNS_GFD_TEXKEY_ADDR_SHIFT) - 1 )) == 0 );
    SDK_ASSERT( (size & (u32)((0x1 << NNS_GFD_TEXKEY_SIZE_SHIFT) - 1 )) == 0 );
    
    // �I�[�o�[�t���[�͔������Ă��Ȃ����H
    SDK_ASSERT( ( (size >> NNS_GFD_TEXKEY_SIZE_SHIFT) & ~NNS_GFD_MASK_15BIT ) == 0 );
    SDK_ASSERT( ( (addr >> NNS_GFD_TEXKEY_ADDR_SHIFT) & ~NNS_GFD_MASK_16BIT ) == 0 );
    
    return  ( ( size >> NNS_GFD_TEXKEY_SIZE_SHIFT ) << 16 ) 
            | ( ( NNS_GFD_MASK_16BIT & ( addr >> NNS_GFD_TEXKEY_ADDR_SHIFT ) ) ) 
            | b4x4Comp << 31;
}

//------------------------------------------------------------------------------
NNS_GFD_INLINE u32
NNS_GfdGetTexKeyAddr( NNSGfdTexKey memKey )
{
    return (u32)( ( ( 0x0000FFFF & memKey ) ) << NNS_GFD_TEXKEY_ADDR_SHIFT );
}

//------------------------------------------------------------------------------
NNS_GFD_INLINE u32
NNS_GfdGetTexKeySize( NNSGfdTexKey memKey )
{
    return (u32)( ( ( 0x7FFF0000 & memKey ) >> 16 ) << NNS_GFD_TEXKEY_SIZE_SHIFT );
}

//------------------------------------------------------------------------------
NNS_GFD_INLINE BOOL
NNS_GfdGetTexKey4x4Flag( NNSGfdTexKey memKey )
{
    return (BOOL)( (0x80000000 & memKey) >> 31 );
}

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif // NNS_GFD_TEXVRAMMAN_TYPES_H_
