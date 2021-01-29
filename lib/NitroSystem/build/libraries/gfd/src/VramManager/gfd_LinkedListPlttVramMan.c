/*---------------------------------------------------------------------------*
  Project:  NITRO-System - libraries - gfd - src - VramManager
  File:     gfd_LinkedListPlttVramMan.c

  Copyright 2004-2007 Nintendo.  All rights reserved.

  These coded instructions, statements, and computer programs contain
  proprietary information of Nintendo of America Inc. and/or Nintendo
  Company Ltd., and are protected by Federal copyright law.  They may
  not be disclosed to third parties or copied or duplicated in any form,
  in whole or in part, without the prior written consent of Nintendo.

  $Revision: 1.13 $
 *---------------------------------------------------------------------------*/
#include <nnsys/gfd/gfd_common.h>
#include <nnsys/gfd/VramManager/gfd_LinkedListPlttVramMan.h>
#include "gfdi_LinkedListVramMan_Common.h"

#define GFD_SLOT_SIZE        0x18000

#define NNS_GFD_BARPLTT_FREE_ERROR_INVALID_SIZE 1

//
// �}�l�[�W��
//
typedef struct NNS_GfdLnkPlttVramManager
{
    NNSiGfdLnkVramMan       mgr;
    NNSiGfdLnkVramBlock*    pBlockPoolList;
    
    //
    // ���Z�b�g���Ɏg�p���郁���o
    //
    u32                     szByte;
    NNSiGfdLnkVramBlock*    pWorkHead;
    u32                     szByteManagementWork;

}NNS_GfdLnkPlttVramManager;

static NNS_GfdLnkPlttVramManager         mgr_;


/*---------------------------------------------------------------------------*
  Name:         NNS_GfdDumpLnkPlttVramManager

  Description:  �}�l�[�W���̓�����Ԃ��f�o�b�N�o�͂��܂��B

  Arguments:    �Ȃ�            
  Returns:      �Ȃ�

 *---------------------------------------------------------------------------*/
void NNS_GfdDumpLnkPlttVramManager()
{
    OS_Printf("=== NNS_Gfd LnkPlttVramManager Dump ===\n");
    OS_Printf("   address:        size    \n");   // �w�b�_�[�s
    OS_Printf("=======================================\n");
    //
    // �ʏ�e�N�X�`���̃t���[���X�g�����ׂĕ\�����A�g�p�ʂ̑��a���v�Z���܂��B
    //
    OS_Printf("------ Free Blocks                -----\n");   
    
    NNSi_GfdDumpLnkVramManFreeListInfo( mgr_.mgr.pFreeList, mgr_.szByte );
    
    OS_Printf("=======================================\n");   
}

/*---------------------------------------------------------------------------*
  Name:         NNS_GfdDumpLnkPlttVramManagerEx

  Description:  �f�o�b�N�o�͏����֐����w�肵�āA
                �}�l�[�W���̓�����Ԃ��f�o�b�N�o�͂��܂��B

  Arguments:    pFunc               �f�o�b�N�o�͏����֐�
                pUserData           �f�o�b�N�o�͏����֐��Ɉ����Ƃ��ēn�����f�[�^
                
  Returns:      �Ȃ�

 *---------------------------------------------------------------------------*/

void NNS_GfdDumpLnkPlttVramManagerEx( 
    NNSGfdLnkDumpCallBack   pFunc, 
    void*                   pUserData )
{
    NNS_GFD_NULL_ASSERT( pFunc );
    NNSi_GfdDumpLnkVramManFreeListInfoEx( mgr_.mgr.pFreeList, pFunc, pUserData );
}

/*---------------------------------------------------------------------------*
  Name:         NNS_GfdGetLnkPlttVramManagerWorkSize

  Description:  VRAM�}�l�[�W�����Ǘ����̂��߂ɕK�v�Ƃ��郁�����̃o�C�g�T�C�Y���擾���܂��B
                �{�֐��̕Ԓl�𗘗p���āA�}�l�[�W���̏������p�����[�^�ł���A
                �Ǘ��̈�̏��������s���Ă��������B 


                
                
  Arguments:    numMemBlk   �m�ۂ����Ǘ��u���b�N���B�Ǘ�����󂫗̈�̍ő�ו������ƂȂ�B 

                
  Returns:      �Ǘ��̈�ɕK�v�ȃo�C�g�T�C�Y

 *---------------------------------------------------------------------------*/
u32 NNS_GfdGetLnkPlttVramManagerWorkSize( u32 numMemBlk )
{
    return numMemBlk * sizeof( NNSiGfdLnkVramBlock );
}

/*---------------------------------------------------------------------------*
  Name:         NNS_GfdInitLnkPlttVramManager

  Description:  �t���[���p���b�gVRAM�}�l�[�W�������������܂��B
                �t���[���p���b�gVRAM�}�l�[�W���́A�p���b�gRAM�̐擪����szByte
                �Ŏw�肳�ꂽ�T�C�Y���̗̈���Ǘ�����悤�ɏ���������܂��B 
                
                
  Arguments:    szByte                  �Ǘ�����̈�̃o�C�g�T�C�Y�B(�ő� 0x18000)
                pManagementWork         �Ǘ����Ƃ��Ďg�p���郁�����̈�ւ̃|�C���^�B 
                szByteManagementWork    �Ǘ����̈�̃T�C�Y�B 
                useAsDefault            �����N�h���X�g�p���b�gVRAM�}�l�[�W�����J�����g�̃}�l�[�W���Ƃ��Ďg�p���邩�ǂ����B 

                
  Returns:      �Ȃ�

 *---------------------------------------------------------------------------*/
void NNS_GfdInitLnkPlttVramManager
( 
    u32     szByte,
    void*   pManagementWork, 
    u32     szByteManagementWork,
    BOOL    useAsDefault
)
{
    NNS_GFD_ASSERT( szByte <= GFD_SLOT_SIZE );
    NNS_GFD_NULL_ASSERT( pManagementWork );
    NNS_GFD_ASSERT( szByteManagementWork != 0 );
    
    {
        //
        mgr_.szByte                 = szByte;
        mgr_.pWorkHead              = pManagementWork;
        mgr_.szByteManagementWork   = szByteManagementWork;
        
        NNS_GfdResetLnkPlttVramState();
        
        //
        // �f�t�H���g�̃A���P�[�^�Ƃ��Ďg�p
        //
        if( useAsDefault )
        {
            NNS_GfdDefaultFuncAllocPlttVram = NNS_GfdAllocLnkPlttVram;
            NNS_GfdDefaultFuncFreePlttVram  = NNS_GfdFreeLnkPlttVram;
        }
    }
}

/*---------------------------------------------------------------------------*
  Name:         NNS_GfdAllocLnkPlttVram

  Description:  �����N�h���X�g�p���b�gVRAM�}�l�[�W������p���b�g�p���������m�ۂ��܂��B
                
  Arguments:    szByte�F     �m�ۂ������̈�̃o�C�g�T�C�Y 
                b4Pltt�F     4�F�p���b�g���B4�F�p���b�g�Ȃ�TRUE�B 
                opt �F       �I�v�V�����B���݂͎g�p����Ă��܂���B 
                
  Returns:      �m�ۂ���VRAM�̈��\���p���b�g�L�[��Ԃ��܂��B
                �m�ۂɎ��s�����ꍇ�́A�G���[�������L�[�ł���
                NNS_GFD_ALLOC_ERROR_PLTTKEY��Ԃ��܂��B

 *---------------------------------------------------------------------------*/
NNSGfdPlttKey    NNS_GfdAllocLnkPlttVram( u32 szByte, BOOL b4Pltt, u32 opt )
{
#pragma unused(opt)
    u32     addr;
    BOOL    result;
    
    {
        //
        // �p���b�g�L�[�ŕ\���ł��Ȃ������ȃT�C�Y�̊m�ۂ̏ꍇ�A�T�C�Y��؂�グ�Ċm�ۂ��܂��B
        //
        szByte = NNSi_GfdGetPlttKeyRoundupSize( szByte );
        //
        // �p���b�g�L�[���\���ł��Ȃ��قǑ傫�ȃT�C�Y�̊m�ۂ̏ꍇ�A�G���[�L�[��Ԃ��܂��B
        //
        if( szByte >= NNS_GFD_PLTTSIZE_MAX )
        {
            NNS_GFD_WARNING("Allocation size is too big. : NNS_GfdAllocLnkPlttVram()");
            return NNS_GFD_ALLOC_ERROR_PLTTKEY;
        }
        
        NNS_GFD_MINMAX_ASSERT( szByte, NNS_GFD_PLTTSIZE_MIN, NNS_GFD_PLTTSIZE_MAX );
    }
    
    //
    // �p���b�g�t�H�[�}�b�g ���� �A���C�������g��ݒ肵�܂�
    //
    if( b4Pltt )
    {
        result = NNSi_GfdAllocLnkVramAligned( &mgr_.mgr, 
                                              &mgr_.pBlockPoolList, 
                                              &addr, 
                                              szByte, 
                                              0x08 );
        // �Q�ƕs�\�̈悩�`�F�b�N����
        if( addr + szByte > NNS_GFD_4PLTT_MAX_ADDR )
        {
            // NG
            if( !NNSi_GfdFreeLnkVram( &mgr_.mgr, 
                                      &mgr_.pBlockPoolList, 
                                      addr, 
                                      szByte ) )
            {
                // �x����\������
            }
                                 
            return NNS_GFD_ALLOC_ERROR_PLTTKEY;
        }
    }else{
        result = NNSi_GfdAllocLnkVramAligned( &mgr_.mgr, 
                                              &mgr_.pBlockPoolList, 
                                              &addr, 
                                              szByte, 
                                              0x10 );
    }
    
    if( result )
    {
        return NNS_GfdMakePlttKey( addr, szByte );    
    }else{
        NNS_GFD_WARNING("failure in Vram Allocation. : NNS_GfdAllocLnkPlttVram()");
        return NNS_GFD_ALLOC_ERROR_PLTTKEY;
    }
}

/*---------------------------------------------------------------------------*
  Name:         NNS_GfdFreeLnkPlttVram

  Description:  �����N�h���X�g�p���b�gVRAM�}�l�[�W������m�ۂ���
                �p���b�g�p��������������܂��B
                
  Arguments:    plttKey�F    �J������VRAM�̈��\���p���b�g�L�[
                
  Returns:      �J���̐��ۂ�Ԃ��܂��B
                �J���ɐ��������ꍇ��0��Ԃ��܂��B
                �T�C�Y�[���Ȃǂ̕s���ȃT�C�Y������킷�p���b�g�L�[�̊J��
                ���������ꍇ��1��Ԃ��܂��B

 *---------------------------------------------------------------------------*/
int             NNS_GfdFreeLnkPlttVram( NNSGfdPlttKey plttKey )
{
    
    const u32   addr     = NNS_GfdGetPlttKeyAddr( plttKey );
    const u32   szByte   = NNS_GfdGetPlttKeySize( plttKey );
    
    const BOOL  result   = NNSi_GfdFreeLnkVram( &mgr_.mgr, 
                                                &mgr_.pBlockPoolList, 
                                                addr, 
                                                szByte );
    
    if( result ) 
    {
        return 0;
    }else{
        return NNS_GFD_BARPLTT_FREE_ERROR_INVALID_SIZE;
    }
}

/*---------------------------------------------------------------------------*
  Name:         NNS_GfdResetLnkPlttVramState

  Description:  �}�l�[�W����������ԂɃ��Z�b�g���܂��B
                
  Arguments:    �Ȃ�
                
  Returns:      �Ȃ�
  
 *---------------------------------------------------------------------------*/
void            NNS_GfdResetLnkPlttVramState( void )
{
    
    //
    // ���L�Ǘ��u���b�N��������
    //
    mgr_.pBlockPoolList 
        = NNSi_GfdInitLnkVramBlockPool( 
            (NNSiGfdLnkVramBlock*)mgr_.pWorkHead, 
            mgr_.szByteManagementWork / sizeof( NNSiGfdLnkVramBlock ) );
    
    //
    // �t���[���X�g�̏�����
    //
    {
        BOOL    result;
        NNSi_GfdInitLnkVramMan( &mgr_.mgr );
        
        result = NNSi_GfdAddNewFreeBlock( &mgr_.mgr, 
                                          &mgr_.pBlockPoolList,
                                          0,
                                          mgr_.szByte );
        NNS_GFD_ASSERT( result );        
    }
    
    //
    // �t���[���X�g�̌���
    // 
    NNSi_GfdMergeAllFreeBlocks( &mgr_.mgr, &mgr_.pBlockPoolList );
    
}

