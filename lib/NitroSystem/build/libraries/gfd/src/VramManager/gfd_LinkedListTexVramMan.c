/*---------------------------------------------------------------------------*
  Project:  NITRO-System - libraries - gfd - src - VramManager
  File:     gfd_LinkedListTexVramMan.c

  Copyright 2004-2007 Nintendo.  All rights reserved.

  These coded instructions, statements, and computer programs contain
  proprietary information of Nintendo of America Inc. and/or Nintendo
  Company Ltd., and are protected by Federal copyright law.  They may
  not be disclosed to third parties or copied or duplicated in any form,
  in whole or in part, without the prior written consent of Nintendo.

  $Revision: 1.16 $
 *---------------------------------------------------------------------------*/
#include <nnsys/gfd/gfd_common.h>
#include <nnsys/gfd/VramManager/gfd_LinkedListTexVramMan.h>
#include "gfdi_LinkedListVramMan_Common.h"

#define GFD_SLOT_SIZE        0x20000
#define GFD_SLOT0_BASEADDR   0x00000
#define GFD_SLOT1_BASEADDR   0x20000
#define GFD_SLOT2_BASEADDR   0x40000 
#define GFD_SLOT3_BASEADDR   0x60000 

#define NNS_GFD_LNK_FREE_ERROR_INVALID_SIZE 2

//
// �}�l�[�W��
//
typedef struct NNS_GfdLnkTexVramManager
{
    NNSiGfdLnkVramMan       mgrNrm;
    NNSiGfdLnkVramMan       mgr4x4;
    
    NNSiGfdLnkVramBlock*    pBlockPoolList;
    
    //
    // ���Z�b�g���Ɏg�p���郁���o
    //
    u32                     szByte;
    u32                     szByteFor4x4;
    NNSiGfdLnkVramBlock*    pWorkHead;
    u32                     szByteManagementWork;

}NNS_GfdLnkTexVramManager;


typedef struct SlotData
{
    u32     szFree; // �󂫗̈�T�C�Y
    u32     szNrm;  // �ʏ�e�N�X�`���p�T�C�Y
    u32     sz4x4;  // 4x4���k�e�N�X�`���p�T�C�Y

}SlotData;


static NNS_GfdLnkTexVramManager         mgr_;

//------------------------------------------------------------------------------
// �f�o�b�N�p�֐�(�A�T�[�g���̒��ɂ̂݋L�q�����֐��ł��B�������J����֐��ł��B)
//------------------------------------------------------------------------------
static u32 Dbg_GetVramManTotalFreeBlockSize_( const NNSiGfdLnkVramMan* pMgr )
{
    u32   total = 0;
    const NNSiGfdLnkVramBlock* pBlk = pMgr->pFreeList;
    while( pBlk )
    {
        total   += pBlk->szByte;
        pBlk    = pBlk->pBlkNext;
    }
    return total;
}

//------------------------------------------------------------------------------
// �������T�C�Y�p�����[�^���L�����H
static BOOL Dbg_IsInitializeSizeParamsValid_( u32 szByte, u32 szByteFor4x4 )
{
    //
    // �T�C�Y���s���ł͂Ȃ����H
    //
    if( szByte > 0 && szByteFor4x4 <= GFD_SLOT_SIZE * 2 )
    {   
        //
        // 4x4�p�̃T�C�Y�w�肪����ꍇ
        //
        if( szByteFor4x4 > 0 )
        {   
            // �T�C�Y�� 0x20000 �ȉ��̏ꍇ
            if( szByteFor4x4 <= GFD_SLOT_SIZE )
            {
                // �C���f�b�N�X�e�[�u�������m�ۉ\�ȃT�C�Y���K�{�ƂȂ�
                return (BOOL)(szByte >= GFD_SLOT1_BASEADDR + szByteFor4x4 / 2);
            }else{
                // �C���f�b�N�X�e�[�u�������m�ۉ\�ȃT�C�Y���K�{�ƂȂ�
                // GFD_SLOT_SIZE �� �C���f�b�N�X�e�[�u�� �Ƃ��Ďg�p����� Slot 1 �̃T�C�Y
                return (BOOL)( szByte >= szByteFor4x4 + GFD_SLOT_SIZE );
            }
        }else{
            // �ő�T�C�Y�����������Ă��Ȃ����H
            return (BOOL)( szByte <= GFD_SLOT_SIZE * 4 );
        }
    }else{
        return FALSE;
    }
}

//------------------------------------------------------------------------------
// 
static NNS_GFD_INLINE BOOL InitSlotFreeBlock_
( 
    NNSiGfdLnkVramMan*      pMgr, 
    NNSiGfdLnkVramBlock**   pPoolList, 
    u32                     baseAddr, 
    u32                     size 
)
{
    NNS_GFD_NULL_ASSERT( pMgr );
    NNS_GFD_NULL_ASSERT( pPoolList );

    NNS_GFD_ASSERT( GFD_SLOT_SIZE >= size );
    
    {
        BOOL result = TRUE;
        
        if( size > 0 )
        {
            result &= NNSi_GfdAddNewFreeBlock( pMgr, pPoolList, baseAddr, size );
        }
        return result;
    }
}

//------------------------------------------------------------------------------
// �}�l�[�W���̓�����Ԃ��f�o�b�N�o�͂��܂��B
void NNS_GfdDumpLnkTexVramManager()
{
    OS_Printf("=== NNS_Gfd LnkTexVramManager Dump ====\n");
    OS_Printf("   address:        size    \n");   // �w�b�_�[�s
    OS_Printf("=======================================\n");
    //
    // �ʏ�e�N�X�`���̃t���[���X�g�����ׂĕ\�����A�g�p�ʂ̑��a���v�Z���܂��B
    //
    OS_Printf("------ Normal Texture Free Blocks -----\n");   
    NNSi_GfdDumpLnkVramManFreeListInfo( mgr_.mgrNrm.pFreeList, mgr_.szByte );
        
        
    //
    // 4x4�e�N�X�`���̃t���[���X�g�����ׂĕ\�����A�g�p�ʂ̑��a���v�Z���܂��B
    //
    OS_Printf("------ 4x4    Texture Free Blocks -----\n");   
    if( mgr_.szByteFor4x4 != 0 )
    {
        NNSi_GfdDumpLnkVramManFreeListInfo( mgr_.mgr4x4.pFreeList, mgr_.szByteFor4x4 );
    }
    OS_Printf("=======================================\n");   
}

/*---------------------------------------------------------------------------*
  Name:         NNS_GfdDumpLnkTexVramManagerEx

  Description:  �f�o�b�N�o�͏����֐����w�肵�āA
                �t���[�u���b�N�����f�o�b�N�o�͂��܂��B
                
  Arguments:    pFuncForNrm            : �f�o�b�N�o�͏����֐�(�ʏ�e�N�X�`���p)
                pFuncFor4x4            : �f�o�b�N�o�͏����֐�(4��4���k�e�N�X�`���p)
                pUserData              : �f�o�b�N�o�͏����֐��Ɉ����Ƃ��ēn�����A
                                         �f�o�b�N�o�͏����p�f�[�^
               
  Returns:      �Ȃ�
  
 *---------------------------------------------------------------------------*/
void NNS_GfdDumpLnkTexVramManagerEx( 
    NNSGfdLnkDumpCallBack           pFuncForNrm, 
    NNSGfdLnkDumpCallBack           pFuncFor4x4, 
    void*                           pUserData )
{
    NNS_GFD_ASSERT( pFuncForNrm != NULL || pFuncFor4x4 != NULL );
    //
    // �ʏ�e�N�X�`���̃t���[���X�g
    //
    if( pFuncForNrm != NULL )
    {
        NNSi_GfdDumpLnkVramManFreeListInfoEx( mgr_.mgrNrm.pFreeList, pFuncForNrm, pUserData );   
    }
    
    //
    // 4x4�e�N�X�`���̃t���[���X�g
    //
    if( mgr_.szByteFor4x4 != 0 && pFuncFor4x4 != NULL )
    {
        NNSi_GfdDumpLnkVramManFreeListInfoEx( mgr_.mgr4x4.pFreeList, pFuncFor4x4, pUserData );
    }
}


//------------------------------------------------------------------------------
u32 NNS_GfdGetLnkTexVramManagerWorkSize( u32 numMemBlk )
{
    return numMemBlk * sizeof( NNSiGfdLnkVramBlock );
}

/*---------------------------------------------------------------------------*
  Name:         NNS_GfdInitLnkTexVramManager

  Description:  �e�N�X�`���L�[����VRAM���̃e�N�X�`���̈���J�����܂��B
                
  Arguments:    szByte                  �Ǘ�����VRAM�̈�̃o�C�g�T�C�Y�B
                                        (1Slot = 0x20000,�ő�4Slot�Ƃ��Čv�Z����) 
                szByteFor4x4            �Ǘ��̈撆��4x4���k�e�N�X�`���Ɏg�p����̈�̃o�C�g�T�C�Y�B
                                        (1Slot = 0x20000,�ő�2Slot�Ƃ��Čv�Z����) 
                pManagementWork         �Ǘ����Ƃ��Ďg�p���郁�����̈�ւ̃|�C���^�B 
                szByteManagementWork    �Ǘ����̈�̃T�C�Y�B 
                useAsDefault            �����N�h���X�g�e�N�X�`��VRAM�}�l�[�W�����J�����g��
                                        �}�l�[�W���Ƃ��Ďg�p���邩�ǂ����B 

                
               
  Returns:      �Ȃ�
  
 *---------------------------------------------------------------------------*/
void NNS_GfdInitLnkTexVramManager
( 
    u32     szByte, 
    u32     szByteFor4x4,
    void*   pManagementWork, 
    u32     szByteManagementWork,
    BOOL    useAsDefault
)
{
    NNS_GFD_ASSERT( Dbg_IsInitializeSizeParamsValid_( szByte, szByteFor4x4 ) );
    
    NNS_GFD_NULL_ASSERT( pManagementWork );
    NNS_GFD_ASSERT( szByteManagementWork != 0 );
    
    {
        mgr_.szByte         = szByte;                                                         
        mgr_.szByteFor4x4   = szByteFor4x4;
        mgr_.pWorkHead      = pManagementWork;
        mgr_.szByteManagementWork = szByteManagementWork;
        
        
        NNS_GfdResetLnkTexVramState();
        
        //
        // �����������Ő������ꂽ�A�t���[�u���b�N�̑��e�ʂ����������̂��m�F���܂�
        //
        NNS_GFD_ASSERT( mgr_.szByte - ( mgr_.szByteFor4x4 + mgr_.szByteFor4x4 / 2 ) 
            == Dbg_GetVramManTotalFreeBlockSize_( &mgr_.mgrNrm ) );
        NNS_GFD_ASSERT( mgr_.szByteFor4x4 
            == Dbg_GetVramManTotalFreeBlockSize_( &mgr_.mgr4x4 ) );

        
        //
        // �f�t�H���g�̃A���P�[�^�Ƃ��Ďg�p
        //
        if( useAsDefault )
        {
            NNS_GfdDefaultFuncAllocTexVram = NNS_GfdAllocLnkTexVram;
            NNS_GfdDefaultFuncFreeTexVram  = NNS_GfdFreeLnkTexVram;
        }
    }
}

/*---------------------------------------------------------------------------*
  Name:         NNS_GfdAllocLnkTexVram

  Description:  �e�N�X�`���̈��VRAM����m�ۂ��܂��B
                
  Arguments:    szByte       : �m�ۂ���̈�o�C�g��
                is4x4comp    : 4x4���k�e�N�X�`�����H
                opt          : �I�v�V�����i�g�p����܂���j
                
               
  Returns:      �e�N�X�`���L�[
                �m�ۂɎ��s�����ꍇ�́A�G���[�������L�[�ł���NNS_GFD_ALLOC_ERROR_TEXKEY��Ԃ��܂��B

  
 *---------------------------------------------------------------------------*/
NNSGfdTexKey    NNS_GfdAllocLnkTexVram( u32 szByte, BOOL is4x4comp, u32 opt )
{
#pragma unused(opt)
    u32     addr;
    BOOL    result;
    
    {
        //
        // �e�N�X�`���L�[�ŕ\���ł��Ȃ������ȃT�C�Y�̊m�ۂ̏ꍇ�A�T�C�Y��؂�グ�Ċm�ۂ��܂��B
        //
        szByte = NNSi_GfdGetTexKeyRoundupSize( szByte );
        //
        // �e�N�X�`���L�[���\���ł��Ȃ��قǑ傫�ȃT�C�Y�̊m�ۂ̏ꍇ�A�G���[�L�[��Ԃ��܂��B
        //
        if( szByte >= NNS_GFD_TEXSIZE_MAX )
        {
            NNS_GFD_WARNING("Allocation size is too big. : NNS_GfdAllocLnkTexVram()");
            return NNS_GFD_ALLOC_ERROR_TEXKEY;
        }
        
        NNS_GFD_MINMAX_ASSERT( szByte, NNS_GFD_TEXSIZE_MIN, NNS_GFD_TEXSIZE_MAX );
    }
    
    
    if( is4x4comp )
    {
        result = NNSi_GfdAllocLnkVram( &mgr_.mgr4x4, &mgr_.pBlockPoolList, &addr, szByte );
    }else{
        result = NNSi_GfdAllocLnkVram( &mgr_.mgrNrm, &mgr_.pBlockPoolList, &addr, szByte );
    }
    
        
    if( result )
    {
        return NNS_GfdMakeTexKey( addr, szByte, is4x4comp );    
    }else{
        NNS_GFD_WARNING("failure in Vram Allocation. : NNS_GfdAllocLnkTexVram()");
        return NNS_GFD_ALLOC_ERROR_TEXKEY;
    }
}

/*---------------------------------------------------------------------------*
  Name:         NNS_GfdFreeLnkTexVram

  Description:  �e�N�X�`���̈��VRAM����J�����܂��B


                
                
  Arguments:    memKey : �e�N�X�`���L�[

                
  Returns:      �Ȃ�

 *---------------------------------------------------------------------------*/
int             NNS_GfdFreeLnkTexVram( NNSGfdTexKey memKey )
{
    BOOL        result;
    const u32   addr     = NNS_GfdGetTexKeyAddr( memKey );
    const u32   szByte   = NNS_GfdGetTexKeySize( memKey );
    const BOOL  b4x4     = NNS_GfdGetTexKey4x4Flag( memKey );
    
    if( szByte != 0 )
    { 
        if( b4x4 )
        {
            result = NNSi_GfdFreeLnkVram( &mgr_.mgr4x4, &mgr_.pBlockPoolList, addr, szByte );
        }else{
            result = NNSi_GfdFreeLnkVram( &mgr_.mgrNrm, &mgr_.pBlockPoolList, addr, szByte );
        }
        
        if( result ) 
        {
            return 0;
        }else{
            return 1;
        }
    }else{
        return NNS_GFD_LNK_FREE_ERROR_INVALID_SIZE;    
    }
}






/*---------------------------------------------------------------------------*
  Name:         NNS_GfdGetLnkPlttVramManagerWorkSize

  Description:  �����N�h���X�g�e�N�X�`��VRAM�}�l�[�W���̃e�N�X�`���p������
                �m�ۏ�Ԃ�������Ԃɖ߂��܂��B
      
  Arguments:    �Ȃ�

                
  Returns:      �Ȃ�

 *---------------------------------------------------------------------------*/
void            NNS_GfdResetLnkTexVramState( void )
{
    SlotData        sd[4] = 
    { 
        // �󂫗̈�T�C�Y, �ʏ�e�N�X�`���p�T�C�Y, 4x4���k�e�N�X�`���p�T�C�Y
        { 0x20000, 0, 0 },
        { 0x20000, 0, 0 },
        { 0x20000, 0, 0 },
        { 0x20000, 0, 0 }
    };
    
    // �X���b�g�P�Ɋm�ۂ����C���f�b�N�X�e�[�u���p�̈�̃T�C�Y
    // (�������A�}�l�[�W���̓t���[�u���b�N�Ƃ��ĊǗ��͂��Ȃ��j
    const u32   szIndexTbl  = mgr_.szByteFor4x4 / 2; 
    // �ʏ�e�N�X�`���T�C�Y                                                 
    u32         restNrm     = mgr_.szByte - ( mgr_.szByteFor4x4 + szIndexTbl );
    // 4x4���k�e�N�X�`���T�C�Y                                                 
    u32         rest4x4     = mgr_.szByteFor4x4;
    u32         slotNo;
    u32         val;
    
    
    //------------------------------------------------------------------------------
    // 4x4�p�̎g�p�e�ʂ��v�Z���A���ʂ� SlotData �Ɋi�[���܂�
    //
    for( slotNo = 0; slotNo < 4; slotNo++ )
    {
        // �X���b�g0��2�݂̂�4x4�p�̈�͊m�ۂ���܂�
        if( slotNo == 0 || slotNo == 2 )
        {
            if( sd[slotNo].szFree > 0 && rest4x4 > 0 )
            {
                if( sd[slotNo].szFree > rest4x4 )
                {
                    val = rest4x4;
                }else{
                    val = sd[slotNo].szFree;
                }
                
                sd[slotNo].sz4x4     += val;
                sd[slotNo].szFree    -= val;
                rest4x4              -= val;
            }
        }
    }
    
    //
    // �X���b�g�P�F�C���f�b�N�X�e�[�u���̈�̃T�C�Y���v�Z���A�t���[�̈悩�猸�Y���܂�
    //
    {
        sd[1].szFree    -= szIndexTbl;
        // �}�l�[�W���̓C���f�b�N�X�e�[�u���p�̗̈�͊Ǘ����܂���
    }
    
    //
    // �c�����̈悩��A�ʏ�e�N�X�`���̈�̃T�C�Y���v�Z���܂�
    //
    for( slotNo = 0; slotNo < 4; slotNo++ )
    {
        if( sd[slotNo].szFree > 0 && restNrm > 0 )
        {
            if( sd[slotNo].szFree > restNrm )
            {
                val = restNrm;
            }else{
                val = sd[slotNo].szFree;
            }
            
            sd[slotNo].szNrm     += val;
            sd[slotNo].szFree    -= val;
            restNrm              -= val;
        }
    }
    
    //------------------------------------------------------------------------------
    //
    // ����������(�Z�o���� SlotData �����Ƀt���[�u���b�N�����������܂��B)
    //
    {
        BOOL result = TRUE;    
        
        NNSi_GfdInitLnkVramMan( &mgr_.mgrNrm );
        NNSi_GfdInitLnkVramMan( &mgr_.mgr4x4 );
        
        
        //
        // ���L�Ǘ��u���b�N��������
        //
        mgr_.pBlockPoolList 
            = NNSi_GfdInitLnkVramBlockPool( (NNSiGfdLnkVramBlock*)mgr_.pWorkHead, 
                                            mgr_.szByteManagementWork / sizeof( NNSiGfdLnkVramBlock ) );
        
        {
            //
            // ���ʑ���4x4�A���̂�����ʑ��ɒʏ�̃t���[�u���b�N���쐬���}�l�[�W���� �o�^����
            //
            // slot 0 
            result &= 
            InitSlotFreeBlock_( &mgr_.mgr4x4, &mgr_.pBlockPoolList , GFD_SLOT0_BASEADDR              , sd[0].sz4x4 );
            result &= 
            InitSlotFreeBlock_( &mgr_.mgrNrm, &mgr_.pBlockPoolList , GFD_SLOT0_BASEADDR + sd[0].sz4x4, sd[0].szNrm );
            // slot 2
            result &= 
            InitSlotFreeBlock_( &mgr_.mgr4x4, &mgr_.pBlockPoolList , GFD_SLOT2_BASEADDR              , sd[2].sz4x4 );
            result &= 
            InitSlotFreeBlock_( &mgr_.mgrNrm, &mgr_.pBlockPoolList , GFD_SLOT2_BASEADDR + sd[2].sz4x4, sd[2].szNrm );
            
            // slot 3
            NNS_GFD_ASSERT( sd[3].sz4x4 == 0 ); 
            result &= 
            InitSlotFreeBlock_( &mgr_.mgrNrm, &mgr_.pBlockPoolList , GFD_SLOT3_BASEADDR              , sd[3].szNrm );
            
            // slot 1
            // �p���b�g�C���f�b�N�X�p�̗̈�̓}�l�[�W���ł͊Ǘ����Ȃ��B
            // �p���b�g�C���f�b�N�X�p�̗̈�ȊO�̗̈��ʏ�e�N�X�`���p�t���[�u���b�N�Ƃ��ď���������B
            result &= 
            InitSlotFreeBlock_( &mgr_.mgrNrm, &mgr_.pBlockPoolList , GFD_SLOT1_BASEADDR + szIndexTbl, sd[1].szNrm );
        }
        NNS_GFD_ASSERT( result );    
    }
        
    // �t���[���X�g�̌��������݂܂��B
    NNSi_GfdMergeAllFreeBlocks( &mgr_.mgrNrm, &mgr_.pBlockPoolList );
    NNSi_GfdMergeAllFreeBlocks( &mgr_.mgr4x4, &mgr_.pBlockPoolList );
}

