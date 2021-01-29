/*---------------------------------------------------------------------------*
  Project:  NITRO-System - libraries - gfd - src - VramManager
  File:     gfdi_LinkedListVramMan_Common.c

  Copyright 2004-2007 Nintendo.  All rights reserved.

  These coded instructions, statements, and computer programs contain
  proprietary information of Nintendo of America Inc. and/or Nintendo
  Company Ltd., and are protected by Federal copyright law.  They may
  not be disclosed to third parties or copied or duplicated in any form,
  in whole or in part, without the prior written consent of Nintendo.

  $Revision: 1.14 $
 *---------------------------------------------------------------------------*/
#include <nnsys/gfd/gfd_common.h>
#include "gfdi_LinkedListVramMan_Common.h"



//------------------------------------------------------------------------------
// �u���b�N���烊�[�W�������擾���܂��B
static NNS_GFD_INLINE void 
GetRegionOfMemBlock_
(
    NNSiGfdLnkMemRegion*            pRegion,
    const NNSiGfdLnkVramBlock*      pBlk
)
{
    NNS_GFD_NULL_ASSERT( pBlk );
    NNS_GFD_NON_ZERO_ASSERT( pBlk->szByte );
    
    pRegion->start = pBlk->addr;
    pRegion->end   = pBlk->addr + pBlk->szByte;
    
    NNS_GFD_ASSERT( pRegion->end > pRegion->start );
}

//------------------------------------------------------------------------------
// ���[�W��������u���b�N�����������܂��B
static NNS_GFD_INLINE void InitBlockFromRegion_
( 
    NNSiGfdLnkVramBlock*        pBlk, 
    const NNSiGfdLnkMemRegion*  pRegion
)
{
    NNS_GFD_NULL_ASSERT( pBlk );
    NNS_GFD_ASSERT( pRegion->end > pRegion->start );
    
    pBlk->addr      = pRegion->start;
    pBlk->szByte    = (u32)(pRegion->end - pRegion->start);
    pBlk->pBlkPrev  = NULL;
    pBlk->pBlkNext  = NULL;
}

//------------------------------------------------------------------------------
// �p�����[�^�ނ���u���b�N�����������܂��B
static NNS_GFD_INLINE void InitBlockFromPrams_
( 
    NNSiGfdLnkVramBlock*    pBlk, 
    u32                     addr, 
    u32                     szByte 
)
{   
    NNS_GFD_NULL_ASSERT( pBlk );
    NNS_GFD_NON_ZERO_ASSERT( szByte );
    
    pBlk->addr      = addr;
    pBlk->szByte    = szByte;
    pBlk->pBlkPrev  = NULL;
    pBlk->pBlkNext  = NULL;
}

//------------------------------------------------------------------------------
// ���X�g�̐擪�֗v�f��}�����܂��B
static NNS_GFD_INLINE void InsertBlock_
(
    NNSiGfdLnkVramBlock**   pListHead,
    NNSiGfdLnkVramBlock*    prev
)
{
    NNS_GFD_NULL_ASSERT( prev );
    
    if( (*pListHead) != NULL )
    {
        (*pListHead)->pBlkPrev    = prev;
    }
    
    prev->pBlkNext          = *pListHead;
    prev->pBlkPrev          = NULL;
    *pListHead = prev;
    
}

//------------------------------------------------------------------------------
// ���X�g����v�f�����o���܂��B
static NNS_GFD_INLINE void RemoveBlock_
(
    NNSiGfdLnkVramBlock**  pListHead,
    NNSiGfdLnkVramBlock*   pBlk
)
{
    NNS_GFD_NULL_ASSERT( pBlk );
    {
    
        NNSiGfdLnkVramBlock *const pPrev = pBlk->pBlkPrev;
        NNSiGfdLnkVramBlock *const pNext = pBlk->pBlkNext;

        // �O�Q�ƃ����N
        if ( pPrev )
        {
            pPrev->pBlkNext = pNext;
        }else{
            *pListHead = pNext;
        }
        

        // ���Q�ƃ����N
        if ( pNext )
        {
            pNext->pBlkPrev = pPrev;
        }
    }
}

//------------------------------------------------------------------------------
// �V�����u���b�N���擾���܂��B
static NNS_GFD_INLINE NNSiGfdLnkVramBlock* 
GetNewBlock_( NNSiGfdLnkVramBlock**   ppBlockPoolList )
{
    NNS_GFD_NULL_ASSERT( ppBlockPoolList );
    {
        // ���X�g�擪������o��
        NNSiGfdLnkVramBlock*    pNew = *ppBlockPoolList;
        if( pNew )
        {
            *ppBlockPoolList = pNew->pBlkNext;
        }
        
        return pNew;
    }
}   


//------------------------------------------------------------------------------
// �u���b�N�̏I�[�A�h���X���擾���܂��B
static NNS_GFD_INLINE u32 GetBlockEndAddr_( NNSiGfdLnkVramBlock* pBlk )
{
    NNS_GFD_NULL_ASSERT( pBlk );
    
    return (u32)(pBlk->addr + pBlk->szByte);
}

//------------------------------------------------------------------------------
// NNSi_GfdDumpLnkVramManFreeListInfo() �Ŏg�p�����A�f�o�b�N�o�͏����֐��B
static void DefaultDumpCallBack_( 
    u32                             addr, 
    u32                             szByte, 
    void*                           pUserData )
{

#ifdef SDK_FINALROM
    #pragma unused(addr)
#endif // SDK_FINALROM

    if( szByte != 0 )
    {
        u32*                        pszFreeTotal    = (u32*)pUserData;
           
        OS_Printf("0x%08x:  0x%08x    \n", addr, szByte );   
        (*pszFreeTotal) += szByte;
    }
}

//------------------------------------------------------------------------------
// �t���[�u���b�N�𑖍����āA���[�W�������������܂��B
// �������N�������ǂ�����BOOL�ŕԂ��܂��B
static BOOL TryToMergeBlockRegion_( 
    NNSiGfdLnkVramMan*      pMan, 
    NNSiGfdLnkVramBlock**   ppBlockPoolList,
    NNSiGfdLnkMemRegion*    pRegion )
{
    NNS_GFD_NULL_ASSERT( pMan );
    NNS_GFD_NULL_ASSERT( ppBlockPoolList );
    NNS_GFD_NULL_ASSERT( pRegion );
            
    {
        // �w��G���A�ɗאڂ����t���[�G���A������
        NNSiGfdLnkVramBlock*        pCursor         = pMan->pFreeList;
        NNSiGfdLnkVramBlock*        pNext           = NULL;
        BOOL                        bMerged         = FALSE;

        // ���ׂẴt���[���X�g�v�f�ɂ���...
        while( pCursor )
        {
            pNext = pCursor->pBlkNext;
              
            // ����ɗאڂ���u���b�N��?
            if( pCursor->addr == pRegion->end )   
            {
                // �󂫃��[�W����������
                pRegion->end = GetBlockEndAddr_( pCursor );
                // ���X�g�����菜���APool �ɖ߂�
                RemoveBlock_( &pMan->pFreeList, pCursor );
                InsertBlock_( ppBlockPoolList, pCursor );
                bMerged |= TRUE;
            }
                            
            // �O���ɗאڂ���u���b�N��?
            if( GetBlockEndAddr_( pCursor ) == pRegion->start )
            {
                // �󂫃��[�W����������
                pRegion->start  = pCursor->addr;
                // ���X�g�����菜���APool �ɖ߂�
                RemoveBlock_( &pMan->pFreeList, pCursor );
                InsertBlock_( ppBlockPoolList, pCursor );
                bMerged |= TRUE;
            }
            
            pCursor = pNext;
        }

        return bMerged;
    }
}


/*---------------------------------------------------------------------------*
  Name:         NNSi_GfdDumpLnkVramManFreeListInfo

  Description:  �t���[�u���b�N�����f�o�b�N�o�͂��܂��B
                
  Arguments:    pFreeListHead          : �t���[�u���b�N��񃊃X�g�̐擪
                szReserved             : �m�ۂ���Ă���̈�̃T�C�Y
               
  Returns:      �Ȃ�
  
 *---------------------------------------------------------------------------*/
void NNSi_GfdDumpLnkVramManFreeListInfo
( 
    const NNSiGfdLnkVramBlock*      pFreeListHead,
    u32                             szReserved 
)
{
    
    u32                         szFreeTotal = 0; 
    const NNSiGfdLnkVramBlock*  pBlk        = pFreeListHead;
    
    // ���ׂẴt���[���X�g�̏���\��
    NNSi_GfdDumpLnkVramManFreeListInfoEx( pBlk, DefaultDumpCallBack_, &szFreeTotal );
    
    // �t���[���X�g�����݂��Ȃ��ꍇ�́A�_�~�[�s�̕\�����s��
    if( szFreeTotal == 0 )
    {
        OS_Printf("0x--------:  0x--------    \n");
    }
    
    // �g�p���̕\��
    {
        const u32 szUsedTotal = (szReserved - szFreeTotal);
        OS_Printf("    %08d / %08d bytes (%6.2f%%) used \n", 
            szUsedTotal, szReserved, 100.f *  szUsedTotal / szReserved );   
    }
}

/*---------------------------------------------------------------------------*
  Name:         NNSi_GfdDumpLnkVramManFreeListInfoEx

  Description:  �f�o�b�N�o�͏����֐����w�肵�āA
                �t���[�u���b�N�����f�o�b�N�o�͂��܂��B
                
  Arguments:    pFreeListHead          : �t���[�u���b�N��񃊃X�g�̐擪
                pFunc                  : �f�o�b�N�o�͏����֐�
                pUserData              : �f�o�b�N�o�͏����֐��Ɉ����Ƃ��ēn�����A
                                         �f�o�b�N�o�͏����p�f�[�^
               
  Returns:      �Ȃ�
  
 *---------------------------------------------------------------------------*/
void NNSi_GfdDumpLnkVramManFreeListInfoEx( 
    const NNSiGfdLnkVramBlock*      pFreeListHead, 
    NNSGfdLnkDumpCallBack           pFunc, 
    void*                           pUserData )
{
    // ���ׂẴt���[���X�g�̏���\��
    const NNSiGfdLnkVramBlock*  pBlk        = pFreeListHead;
    
    NNS_GFD_NULL_ASSERT( pFunc );
    
    while( pBlk )
    {
        (*pFunc)( pBlk->addr, pBlk->szByte, pUserData );
        pBlk          = pBlk->pBlkNext;
    }
}

/*---------------------------------------------------------------------------*
  Name:         NNSi_GfdInitLnkVramMan

  Description:  NNSiGfdLnkVramMan�����������܂��B
                
  Arguments:    pMgr          : VRAM�}�l�[�W��
                
               
  Returns:      �Ȃ�
  
 *---------------------------------------------------------------------------*/
void 
NNSi_GfdInitLnkVramMan( NNSiGfdLnkVramMan* pMgr )
{
    NNS_GFD_NULL_ASSERT( pMgr );
    pMgr->pFreeList = NULL;
}


/*---------------------------------------------------------------------------*
  Name:         NNSi_GfdInitLnkVramBlockPool

  Description:  �Ǘ��u���b�N�v�[�������������܂��B
                �Ǘ����v�f�� == �Ǘ��\�ȃ������敪���ƂȂ�܂�
                
  Arguments:    pArrayHead          : �Ǘ����z��̐擪
                lengthOfArray       : �Ǘ����v�f��
               
  Returns:      ���X�g�̐擪
  
 *---------------------------------------------------------------------------*/
NNSiGfdLnkVramBlock* 
NNSi_GfdInitLnkVramBlockPool
( 
    NNSiGfdLnkVramBlock*    pArrayHead, 
    u32                     lengthOfArray 
)
{
    NNS_GFD_NULL_ASSERT( pArrayHead );
    NNS_GFD_NON_ZERO_ASSERT( lengthOfArray );
    
    //
    // ���X�g�łȂ��܂�
    //
    {
        int i;
        for( i = 0; i < lengthOfArray - 1; i++ )
        {
            pArrayHead[i].pBlkNext      = &pArrayHead[i+1];
            pArrayHead[i+1].pBlkPrev    = &pArrayHead[i];
        }
        
        pArrayHead[0].pBlkPrev                  = NULL;  
        pArrayHead[lengthOfArray - 1].pBlkNext  = NULL;    
    }
    
    // ���X�g�擪��Ԃ��܂�
    return &pArrayHead[0];
}

/*---------------------------------------------------------------------------*
  Name:         NNSi_GfdInitLnkVramMan

  Description:  �}�l�[�W���ɐV�����t���[�̈�̃u���b�N��ǉ����܂��B
                
                
  Arguments:    pMan                : �}�l�[�W��
                ppBlockPoolList     : ���ʃu���b�N�Ǘ����
                baseAddr            : �Ǘ��̈�̃x�[�X�A�h���X
                szByte              : �m�ۂ��郁�����T�C�Y
               
  Returns:      �Ȃ�
  
 *---------------------------------------------------------------------------*/
BOOL NNSi_GfdAddNewFreeBlock
(
    NNSiGfdLnkVramMan*      pMan, 
    NNSiGfdLnkVramBlock**   ppBlockPoolList,
    u32                     baseAddr,
    u32                     szByte
)
{
    NNS_GFD_NULL_ASSERT( pMan );
    NNS_GFD_NULL_ASSERT( ppBlockPoolList );
    NNS_GFD_NON_ZERO_ASSERT( szByte );
    
    // �t���[�u���b�N���쐬����
    {
        NNSiGfdLnkVramBlock*        pNew  = GetNewBlock_( ppBlockPoolList );
        if( pNew )
        {
            InitBlockFromPrams_( pNew, baseAddr, szByte );
            InsertBlock_( &pMan->pFreeList, pNew );
            
            return TRUE;
        }else{
            return FALSE;
        }
    }
}



/*---------------------------------------------------------------------------*
  Name:         NNSi_GfdAllocLnkVram

  Description:  �������m�ۂ��s���܂��B
                
                �c�胁�������\���ł��A
                �Ǘ��u���b�N���s�������ꍇ�Ɋm�ۂɎ��s����ꍇ������̂Œ��ӂ��K�v�ł��B 
                
  Arguments:    pMan                : �}�l�[�W��
                ppBlockPoolList     : ���ʃu���b�N�Ǘ����
                pRetAddr            : �m�ۂ���A�h���X�ւ̃|�C���^
                szByte              : �m�ۂ��郁�����T�C�Y
                
  Returns:      �������m�ۂ̐���
  
 *---------------------------------------------------------------------------*/
BOOL
NNSi_GfdAllocLnkVram
( 
    NNSiGfdLnkVramMan*      pMan, 
    NNSiGfdLnkVramBlock**   ppBlockPoolList,
    u32*                    pRetAddr,
    u32                     szByte
)
{
    return NNSi_GfdAllocLnkVramAligned( pMan, ppBlockPoolList, pRetAddr, szByte, 0 );
}

//------------------------------------------------------------------------------
// �A���C�������g���w��\�ȃ������m�ہB
// �A���C�������g�̍ۂɔ��������A�󂫗̈�̓t���[���X�g�ɐV���ɓo�^�����B
// �󂫗̈�̂��߂̊Ǘ����u���b�N�����݂��Ȃ��ꍇ�ɂ́A�m�ێ��s�ƂȂ�̂Œ��ӂ��K�v�ł���B
BOOL
NNSi_GfdAllocLnkVramAligned
( 
    NNSiGfdLnkVramMan*      pMan, 
    NNSiGfdLnkVramBlock**   ppBlockPoolList,
    u32*                    pRetAddr,
    u32                     szByte,
    u32                     alignment
)
{
    NNS_GFD_NULL_ASSERT( pMan );
    NNS_GFD_NULL_ASSERT( pRetAddr );
    
    NNS_GFD_NON_ZERO_ASSERT( szByte );
    {
        //
        // �t���[���X�g��������ɂ����u���b�N����������
        //
        u32     alignedAddr;
        u32     szNeeded;
        u32     difference;
        
        NNSiGfdLnkVramBlock* pBlkFound  = NULL;
        NNSiGfdLnkVramBlock* pBlk       = pMan->pFreeList;
        
        
        while( pBlk )
        {
            //
            // �K�v������΁A�A���C�����E�ɐ؂�グ���A�h���X���v�Z���܂�
            //
            if( alignment > 1 )
            {
                alignedAddr = (u32)(  (pBlk->addr + (alignment - 1)) & ~(alignment - 1) );
                // ���ۂɕK�v�ƂȂ�T�C�Y�͐؂�グ���������������܂�
                difference  = ( alignedAddr - pBlk->addr );
                szNeeded    = szByte + difference;
            }else{
                alignedAddr = pBlk->addr;
                difference  = 0;
                szNeeded    = szByte;
            }
            
            
            // �T�C�Y���v�����݂������H
            if( pBlk->szByte >= szNeeded )
            {
                pBlkFound = pBlk;
                break;
            }
            pBlk = pBlk->pBlkNext;
        }
        
        //
        // �����ɂ���u���b�N���������ꂽ�Ȃ��...
        //
        if ( pBlkFound ) 
        {
            // �A���C�����g�Ƃ��Ă��炵�������̓t���[�u���b�N�Ƃ��ēo�^���܂�
            if( difference > 0 )
            {    
                NNSiGfdLnkVramBlock*        pNewFreeBlk = GetNewBlock_( ppBlockPoolList );
                if( pNewFreeBlk )
                {
                    // �o�^
                    InitBlockFromPrams_( pNewFreeBlk, pBlkFound->addr, difference );
                    InsertBlock_( &pMan->pFreeList, pNewFreeBlk );
                }else{
                    // �m�ێ��s�Ƃ���
                    goto NG_CASE;
                }
            }
            
            // ���������t���[�u���b�N�̏����X�V
            {
                // �g�p�����������Z����
                pBlkFound->szByte   -= szNeeded;
                // �u���b�N�O������m�ۂ���B
                pBlkFound->addr     += szNeeded; 
                
                // ���傤�ǂ̑傫��������
                if( pBlkFound->szByte == 0 )
                {
                    // �t���[���X�g�����菜��
                    RemoveBlock_( &pMan->pFreeList, pBlkFound );
                    InsertBlock_( ppBlockPoolList, pBlkFound );
                }
            }
                    
            *pRetAddr = alignedAddr;
            return TRUE;
        }
        
NG_CASE:            
        //
        // �����ɍ����A�u���b�N�������ł��Ȃ������B
        //
        *pRetAddr = 0;
        return FALSE;
        
    }
}

/*---------------------------------------------------------------------------*
  Name:         NNSi_GfdMergeAllFreeBlocks

  Description:  �t���[�u���b�N�𑖍����āA�u���b�N�̌��������܂��B
  
 *---------------------------------------------------------------------------*/
void NNSi_GfdMergeAllFreeBlocks( 
    NNSiGfdLnkVramMan*      pMan, 
    NNSiGfdLnkVramBlock**   ppBlockPoolList
)
{
    NNSiGfdLnkMemRegion         region;
    
    // ���ׂẮA�t���[���X�g�ɂ���...
    NNSiGfdLnkVramBlock*        pCursor         = pMan->pFreeList;
    while( pCursor )
    {
        region.start    = pCursor->addr;
        region.end      = pCursor->addr + pCursor->szByte;
        
        // �u���b�N�������ł��Ȃ����m�F���܂��B
        // �����������...
        if( TryToMergeBlockRegion_( pMan, ppBlockPoolList, &region ) )
        {
            //
            // �������s��́A�V�����T�C�Y��ݒ肵�܂��B
            //
            pCursor->addr    = region.start;
            pCursor->szByte  = region.end - region.start;       
            
            // ���X�g�擪����A�ēx�������܂��B
            pCursor = pMan->pFreeList;
        }else{
            pCursor = pCursor->pBlkNext;
        }
    }
}

/*---------------------------------------------------------------------------*
  Name:         NNSi_GfdFreeLnkVram

  Description:  �������J�����s���܂��B
                �Ǘ��u���b�N���s�������ꍇ�ɊJ���Ɏ��s����ꍇ������̂Œ��ӂ��K�v�ł��B
                (�V���ȃt���[�u���b�N�̐����Ɏ��s���邽��)
                
                
  Arguments:    pMan                : �}�l�[�W��
                ppBlockPoolList     : ���ʃu���b�N�Ǘ����
                addr                : �J������A�h���X
                szByte              : �J�����郁�����T�C�Y
               
  Returns:      �������J���̐���
  
 *---------------------------------------------------------------------------*/
BOOL NNSi_GfdFreeLnkVram
( 
    NNSiGfdLnkVramMan*      pMan, 
    NNSiGfdLnkVramBlock**   ppBlockPoolList,
    u32                     addr,
    u32                     szByte
)
{
    NNS_GFD_NULL_ASSERT( pMan );
    NNS_GFD_NON_ZERO_ASSERT( szByte );
    
    {
        
        //------------------------------------------------------------------------------
        // �󂫃��[�W�������t���[�������u���b�N�֑g�ݓ���܂��B
        //      �t���[�u���b�N�Ɨאڂ��Ă���ꍇ�́A�t���[�u���b�N���g�����܂��B
        //      �t���[�u���b�N�Ɨאڂ��Ă��炸�A���t���[�u���b�N�Ƃ���قǂ�
        //      �T�C�Y�������ꍇ�́A����ɗאڂ���g�p�ς݃u���b�N�̃A���C�����g�l�Ƃ��܂��B
        //      ����ɗאڂ���g�p�ς݃u���b�N�������ꍇ�́A�֐��͎��s���܂��B
        {
            NNSiGfdLnkMemRegion     region;
        
            region.start    = addr;
            region.end      = addr + szByte;
        
            NNS_GFD_NULL_ASSERT( pMan );
            NNS_GFD_NULL_ASSERT( ppBlockPoolList );
            {
                (void)TryToMergeBlockRegion_( pMan, ppBlockPoolList, &region );
                                
                //
                // �V�����t���[�u���b�N��o�^����
                //
                {
                    NNSiGfdLnkVramBlock*        pNewFreeBlk = GetNewBlock_( ppBlockPoolList );
                    if( pNewFreeBlk == NULL )
                    {
                        // �Ǘ��̈�p�f�[�^������Ȃ�
                        // Free�Ɏ��s
                        return FALSE;
                    }else{
                    
                        InitBlockFromRegion_( pNewFreeBlk, &region );
                        InsertBlock_( &pMan->pFreeList, pNewFreeBlk );
                    }
                }
                
                return TRUE;
            }
        }
    }
}
