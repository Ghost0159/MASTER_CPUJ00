/*---------------------------------------------------------------------------*
  Project:  NITRO-System - libraries - g2d
  File:     g2d_OAMEx.c

  Copyright 2004-2007 Nintendo.  All rights reserved.

  These coded instructions, statements, and computer programs contain
  proprietary information of Nintendo of America Inc. and/or Nintendo
  Company Ltd., and are protected by Federal copyright law.  They may
  not be disclosed to third parties or copied or duplicated in any form,
  in whole or in part, without the prior written consent of Nintendo.

  $Revision: 1.31 $
 *---------------------------------------------------------------------------*/

#include <nitro.h>
#include <nnsys/g2d/g2d_OAMEx.h>
#include <nnsys/g2d/fmt/g2d_Cell_data.h>

#include "g2d_Internal.h"





static GXOamAttr       defaultOam_ = { 193, 193, 193 };







//------------------------------------------------------------------------------
// �e��o�^�֐� �� �Ó����������܂�
static NNS_G2D_INLINE BOOL IsOamEntryFuncsValid_
( 
    const NNSG2dOamManagerInstanceEx*  pMan,
    const NNSG2dOamExEntryFunctions*   pF 
)
{
    const BOOL bValid = (BOOL)(( pF                       != NULL ) &&
                               ( pF->getOamCapacity       != NULL ) &&    
                               ( pF->entryNewOam          != NULL ) );
                      
    // �A�t�B���p�����[�^���Ǘ�����ꍇ�ɂ�...         
    if( pMan->pAffineBuffer != NULL || pMan->lengthAffineBuffer != 0 )
    {
        // �֘A����R�[���o�b�N���������ݒ肳��Ă���K�v������܂��B
        return (BOOL)( bValid &&
                      ( pF->getAffineCapacity    != NULL ) &&
                      ( pF->entryNewAffine       != NULL ) );
    }else{
        return bValid;  
    }
    
}

//------------------------------------------------------------------------------
// ���L NNSG2dOamChunk ���� �V���� NNSG2dOamChunk �� �擾���܂��B
// ���L NNSG2dOamChunk �� ���������� ���[�U���ݒ肵�܂��B
//
static NNS_G2D_INLINE NNSG2dOamChunk* GetNewOamChunk_( NNSG2dOamManagerInstanceEx* pMan, const GXOamAttr* pOam )
{
    NNSG2dOamChunk*   pRet = NULL;
    NNS_G2D_NULL_ASSERT( pMan );
    NNS_G2D_NULL_ASSERT( pOam );
    
    if( pMan->numUsedOam < pMan->numPooledOam )
    {
        
        pRet = &pMan->pPoolOamChunks[pMan->numUsedOam];
        pMan->numUsedOam++;
        
        pRet->oam = *pOam;
        return pRet;
    }else{
        NNSI_G2D_DEBUGMSG0("We have no capacity for a new Oam.");
        return NULL;
    }
}

//------------------------------------------------------------------------------
// NNSG2dOamChunk �� NNSG2dOamChunkList �̐擪�ɉ����܂�
static NNS_G2D_INLINE void AddFront_( NNSG2dOamChunkList* pOamList, NNSG2dOamChunk* pChunk )
{
    pChunk->pNext       = pOamList->pChunks;
    pOamList->pChunks   = pChunk;
    
    pOamList->numChunks++;
}

//------------------------------------------------------------------------------
// NNSG2dOamChunk �� NNSG2dOamChunkList �̏I�[�ɉ����܂�
static NNS_G2D_INLINE void AddBack_( NNSG2dOamChunkList* pOamList, NNSG2dOamChunk* pChunk )
{
    pChunk->pNext               = NULL;
    
    if( pOamList->pLastChunk != NULL )
    {
        // ����ȍ~
        pOamList->pLastChunk->pNext = pChunk;
    }else{
        // ����
        pOamList->pChunks    = pChunk;
    }
    
    // ���X�g�I�[�̍X�V
    pOamList->pLastChunk = pChunk;
    
    pOamList->numChunks++;
}

//------------------------------------------------------------------------------
// ���ʃ��W���[����Oam��o�^���܂�
static NNS_G2D_INLINE void EntryOamToToBaseModule_( NNSG2dOamManagerInstanceEx* pMan, const GXOamAttr* pOam, u16 totalOam )
{
    (void)(*pMan->oamEntryFuncs.entryNewOam)( pOam, totalOam );
}


//------------------------------------------------------------------------------
// affineProxy �� �Ó������肵�܂�
static NNS_G2D_INLINE BOOL IsAffineProxyValid_( NNSG2dOamManagerInstanceEx* pMan )
{
    return ( pMan->pAffineBuffer != NULL && pMan->lengthAffineBuffer != 0 ) ? 
        TRUE : FALSE;
}
//------------------------------------------------------------------------------
// �V�K�o�^�ɏ\���ȗe�ʂ��c���Ă��邩�������܂��B
static NNS_G2D_INLINE BOOL HasEnoughCapacity_( NNSG2dOamManagerInstanceEx* pMan )
{
    NNS_G2D_NULL_ASSERT( pMan );
    return ( pMan->numAffineBufferUsed < pMan->lengthAffineBuffer ) ? TRUE : FALSE;
}
//------------------------------------------------------------------------------
// affineProxyIdx => affineHWIndex �ւ̕ϊ������܂�
static NNS_G2D_INLINE u16 ConvertAffineIndex_( NNSG2dOamManagerInstanceEx* pMan, u16 affineProxyIdx )
{
    NNS_G2D_ASSERT( IsAffineProxyValid_( pMan ) );
    NNS_G2D_ASSERT( affineProxyIdx < pMan->lengthAffineBuffer );
        
    return pMan->pAffineBuffer[affineProxyIdx].affineHWIndex;            
}

//------------------------------------------------------------------------------
// NNSG2dOamChunkList ���� ���ׂĂ� NNSG2dOamChunk �� �Q�� affine �p�����[�^ �f�[�^
// �� ���ۂ� HW�ɐݒ肳�ꂽ �f�[�^�ɕύX����
//
// �A�t�B���ϊ����ꂽOAM�̕`�揇�ƒʏ��OAM�̕`�揇�����ꂳ��܂����B
// �]���́A�A�t�B���ϊ����ꂽOAM�͓o�^���Ƃ͋t�̏��Ԃŕ`�悳��Ă��܂����B
// 
// �Ȃ��ANNS_G2D_OAMEX_USE_OLD_REINDEXOAMCHUNKLIST_ ���`���ă��C�u������
// �ăr���h���邱�ƂŁA�ߋ��̎����Ɠ����U�镑���ɖ߂����Ƃ��\�ł��B
//
//#define NNS_G2D_OAMEX_USE_OLD_REINDEXOAMCHUNKLIST_ 1

static NNS_G2D_INLINE void ReindexOamChunkList_
( 
    NNSG2dOamManagerInstanceEx*     pMan, 
    NNSG2dOamChunkList*             pChunkList
)
{
#ifdef NNS_G2D_OAMEX_USE_OLD_REINDEXOAMCHUNKLIST_
    //
    // �ߋ��̎���
    //
    NNS_G2D_NULL_ASSERT( pMan );
    NNS_G2D_NULL_ASSERT( pChunkList );
    {
        NNSG2dOamChunk*    pChunk     = pChunkList->pAffinedChunks;
        NNSG2dOamChunk*    pNextChunk = NULL;
        
        //
        // ���X�g�I�[�܂�...
        //
        while( pChunk != NULL )
        {
            //
            // �C���f�b�N�X�� AffineProxyIndex����A���ۂ�HW Index�� �ϊ�����
            //
            const u16 index = ConvertAffineIndex_( pMan, pChunk->affineProxyIdx );
            
            //OS_Printf("AffProxy_Idx = %d, => HW_Idx = %d \n", pChunk->affineProxyIdx, index );
            
            pNextChunk = pChunk->pNext;
            
            // �C���f�b�N�X���L���Ȓl�Ȃ��...
            if( index != NNS_G2D_OAMEX_HW_ID_NOT_INIT )
            {
                // OAM �� Index �� �t���ւ���
                // G2_SetOBJEffect( &pChunk->oam, NNS_G2dGetOamManExDoubleAffineFlag( pMan ), index );
                pChunk->oam.rsParam = index;
                
                //
                // �ʏ�� OamChunkList�Ɉړ�����
                // �ʏ�� OamChunkList�ɓo�^���Ƃ͋t���ɓo�^����Ă������ƂɂȂ�܂�
                //
                AddFront_( pChunkList, pChunk );
            }
            pChunk     = pNextChunk;
        }
    }
#else // NNS_G2D_OAMEX_USE_OLD_REINDEXOAMCHUNKLIST_
    //
    // ���݂̎���
    //
    NNS_G2D_NULL_ASSERT( pMan );
    NNS_G2D_NULL_ASSERT( pChunkList );
    {
        NNSG2dOamChunk*    pChunk     = pChunkList->pAffinedChunks;
        NNSG2dOamChunk*    pHeadChunk = NULL;
        NNSG2dOamChunk*    pPrevChunk = NULL;
        int                numAffinedOam = 0;
        
        
        //
        // �A�t�B���ϊ����ꂽOAM���X�g�I�[�܂�...
        //
        while( pChunk != NULL )
        {
            //
            // �C���f�b�N�X�� AffineProxyIndex����A���ۂ�HW Index�� �ϊ�����
            //
            const u16 index = ConvertAffineIndex_( pMan, pChunk->affineProxyIdx );
            
            // �C���f�b�N�X���L���Ȓl�Ȃ��...
            if( index != NNS_G2D_OAMEX_HW_ID_NOT_INIT )
            {
                // �i�L���ȁj�A�t�B���ϊ����ꂽOAM���X�g�̐擪���L�����Ă���
                if( pHeadChunk == NULL )
                {
                    pHeadChunk = pChunk;
                }
                
                //
                // OAM �� Index �� �t���ւ���
                //
                pChunk->oam.rsParam = index;
                
                
                // ���̃`�����N��
                pPrevChunk = pChunk;
                pChunk     = pChunk->pNext;
                numAffinedOam++;
                
            }else{
                // ���X�g�����菜��
                if( pPrevChunk != NULL )
                {
                    pPrevChunk->pNext = pChunk->pNext;
                }
                pMan->numUsedOam--;
                // ���̃`�����N��
                pChunk     = pChunk->pNext;
            }
        }
        
        //
        // pPrevChunk->pNext => �A�t�B���ϊ����ꂽOAM�̃��X�g�I�[
        // �A�t�B���ϊ����ꂽOAM�̃��X�g���܂Ƃ߂āA�ʏ�̃`�����N���X�g�̐擪�ɑ}�����܂�
        // �A�t�B���ϊ����ꂽOAM�̃��X�g���ł̏��Ԃ̕ύX�͂�����܂���
        // 
        if( numAffinedOam != 0 ) 
        {
           pPrevChunk->pNext     = pChunkList->pChunks;
           pChunkList->pChunks   = pHeadChunk;
           pChunkList->numChunks += numAffinedOam;
           pChunkList->pAffinedChunks = NULL;
        }
    }
#endif // NNS_G2D_OAMEX_USE_OLD_REINDEXOAMCHUNKLIST_
}

//------------------------------------------------------------------------------
// affine �ϊ����ꂽ NNSG2dOamChunk �� �`��\�Ȃ悤�ɏ������܂��B
//
// affine �ϊ����ꂽ NNSG2dOamChunk �� �ʏ�� NNSG2dOamChunk �� �ʂ̃��X�g�Ɋi�[����Ă��܂��B 
// NNSG2dOamChunk �� ����affineProxy�z�� �� Index��affine�p�����[�^���Q�Ƃ��Ă��邽�߁A
// �ʏ�� affine�p�����[�^Index �ɒl������������K�v������܂��B
// 
// 
static NNS_G2D_INLINE void ReindexAffinedOams_( NNSG2dOamManagerInstanceEx* pMan )
{
    NNS_G2D_NULL_ASSERT( pMan );
    NNS_G2D_ASSERT( pMan->numAffineBufferUsed != 0 );
    NNS_G2D_ASSERT( IsAffineProxyValid_( pMan ) );
    
    {
       u16 i;
       //
       // ���ׂĂ� NNSG2dOamChunkList �ɂ���...
       //
       for( i = 0; i < pMan->lengthOfOrderingTbl; i++ )
       {
           ReindexOamChunkList_( pMan, &pMan->pOamOrderingTbl[i] );
       }
    }
}




//------------------------------------------------------------------------------
// �w�肳�ꂽ �`��D��x�͓K�����������܂��B
// pMan->lengthOfOrderingTbl �� �}�l�[�W������������ ���[�U�����R�ɐݒ肷��l�ł��B
static NNS_G2D_INLINE BOOL IsPriorityValid_( NNSG2dOamManagerInstanceEx* pMan, u8 priority )
{
    return ( priority < pMan->lengthOfOrderingTbl ) ? TRUE : FALSE;
}

//------------------------------------------------------------------------------
// NNSG2dOamChunkList �� AffineIndex���w�肵�� OAMAttribute ��o�^���܂�
//
// �V���� NNSG2dOamChunk �� ��U Affine �ϊ����ꂽ NNSG2dOamChunk ��p�̃��X�g�Ɋi�[����܂��B
// NNSG2dOamChunk ��p�̃��X�g�́AReindexAffinedOams_() �ō�Index��������A
// �ʏ�� NNSG2dOamChunk ���X�g�ɑ}������܂�
//
static NNS_G2D_INLINE BOOL EntryNewOamWithAffine_
( 
    NNSG2dOamChunkList*       pOamList, 
    NNSG2dOamChunk*           pOamChunk, 
    u16                       affineIdx, 
    NNSG2dOamExDrawOrder      drawOrderType
)
{
    NNS_G2D_NULL_ASSERT( pOamList );
    
    if( pOamChunk != NULL )
    {
        //
        // Affine�𗘗p���Ȃ�
        //
        if( affineIdx == NNS_G2D_OAM_AFFINE_IDX_NONE )
        {
           if( drawOrderType == NNSG2D_OAMEX_DRAWORDER_BACKWARD )
           {
               //
               // �ʏ�� OamChunkList�Ɉړ�����
               //
               AddFront_( pOamList, pOamChunk );
           }else{
               AddBack_( pOamList, pOamChunk );
           }
        }else{
            
            pOamChunk->affineProxyIdx = affineIdx;
            if( drawOrderType == NNSG2D_OAMEX_DRAWORDER_BACKWARD )
            {
               //
               // ��U Affine ���X�g�ɂ��܂�
               // �擪�ɉ�����
               //
               pOamChunk->pNext            = pOamList->pAffinedChunks;
               pOamList->pAffinedChunks    = pOamChunk;
            }else{
               //
               // ��U Affine ���X�g�ɂ��܂�
               // �I�[�ɉ�����
               //
               pOamChunk->pNext = NULL;
               if( pOamList->pLastAffinedChunk != NULL )
               {
                  pOamList->pLastAffinedChunk->pNext = pOamChunk;
               }else{
                  pOamList->pAffinedChunks = pOamChunk;
               }
               pOamList->pLastAffinedChunk = pOamChunk;
            }
        }
        return TRUE;
    }else{
        return FALSE;
    }
}



//------------------------------------------------------------------------------
// ���X�g�̃|�C���^�� num �������i�߂܂�
static NNS_G2D_INLINE NNSG2dOamChunk* AdvancePointer_( NNSG2dOamChunk* pChunk, u16 num )
{
    NNS_G2D_NULL_ASSERT( pChunk );
    while( num > 0 )
    {
        pChunk = pChunk->pNext;
        num--;
        NNS_G2D_NULL_ASSERT( pChunk );
    }
    
    return pChunk;
}

//------------------------------------------------------------------------------
// OamChunkList �� �`�悷��
// OamChunk �� ����̕`��D��x������ OamChunk ���Ƃɂ܂Ƃ܂��� OamChunkList ���`�����܂��B
//
static NNS_G2D_INLINE u16 DrawOamChunks_
( 
    NNSG2dOamManagerInstanceEx*    pMan, 
    NNSG2dOamChunkList*             pOamList, 
    NNSG2dOamChunk*                 pChunk, 
    u16                             numOam, 
    u16                             capacityOfHW, 
    u16                             numTotalOamDrawn 
)
{
    //
    // ���ׂĂ�OBJ��`�悵�I��邩�A�󂫗e�ʂ��Ȃ��Ȃ�܂�
    // 
    while( numOam > 0 && (capacityOfHW - numTotalOamDrawn) > 0 )
    {
        //
        // �l�̃R�s�[
        //
        EntryOamToToBaseModule_( pMan, &pChunk->oam, numTotalOamDrawn );
        
        
        pChunk = pChunk->pNext;
        // 
        // ���X�g�I�[�ɓ��B�����ꍇ�̓��X�g�擪����ăX�^�[�g������
        //
        if( pChunk == NULL )
        {
            pChunk = pOamList->pChunks;
        }
        
        
        numOam--;
        numTotalOamDrawn++;
    }
    return numTotalOamDrawn;
} 



//------------------------------------------------------------------------------
// AffineProxy �� ���ʃ��W���[���ɔ��f���܂��B
// �i�ʏ�A���ʃ��W���[�� �� ���f�� HW�ɓ`�d���܂��j
static NNS_G2D_INLINE void LoadAffineProxyToBaseModule_( NNSG2dOamManagerInstanceEx* pMan )
{
    NNS_G2D_NULL_ASSERT( pMan );
    NNS_G2D_ASSERT( pMan->numAffineBufferUsed != 0 );
    
    {    
       u16        count     = 0x0;
       u16        i         = pMan->lastFrameAffineIdx;
       const u16  numAffine = pMan->numAffineBufferUsed;
       const u16  capacity  = (*pMan->oamEntryFuncs.getAffineCapacity)();
           
       NNSG2dAffineParamProxy*   pAff = NULL;
       //
       // �O�t���[���̑�������o�^����悤�ȏ�����_���Ă��܂�
       //
       while( ( count < numAffine ) && 
              ( count < capacity ) )
       {
           if( i >= numAffine )
           {
              i = 0;
           }
           pAff = &pMan->pAffineBuffer[ i ];
           
           // store HW affine index.
           pAff->affineHWIndex 
              = (*pMan->oamEntryFuncs.entryNewAffine)( &pAff->mtxAffine, count );
           
           // pMan->lastFrameAffineIdx = i;
           
           i++;
           count++;    
       }
       
       pMan->lastFrameAffineIdx = i;
    }
}

//------------------------------------------------------------------------------
//
// �`��o�^����͈͂��v�Z���܂�
//
// �菇�F
//
// 1�D�܂����ׂẴ��X�g��`�悳��Ȃ����X�g�Ƃ��Đݒ肵�܂�
// 2�D���X�g�𑖍����A�`��OAM�`�����N���J�E���g���Ȃ���A���X�g�ɕ`��̂��߂̏���ݒ肵�Ă����܂��B
// 3�D�󂫗e�ʂ������Ȃ����A�������͂��ׂẴ`�����N���̒������I������珈�����I�����܂��B
//
static NNS_G2D_INLINE void 
CalcDrawnListArea_( NNSG2dOamManagerInstanceEx* pMan )
{
    NNSG2dOamChunk*       pChunk    = NULL;
    NNSG2dOamChunkList*   pOamList  = NULL;
    u16       numTotalOamDrawn      = 0;
    const u16 capacityOfHW          = ( *(pMan->oamEntryFuncs.getOamCapacity) )();
    
    u16       i;
    
    
    //
    // ���ׂẴ`�����N���X�g�ɂ��āA�`��t���O�����Z�b�g���܂�
    //
    for( i = 0; i < pMan->lengthOfOrderingTbl; i++ )
    {
       pMan->pOamOrderingTbl[i].bDrawn = FALSE;
    }
    
    
    //
    // �f�[�^�̑����́A�O�t���[���ŏI���������̈ʒu����X�^�[�g����
    //
    i = (u16)(pMan->lastRenderedOrderingTblIdx);
    
    //
    // ���ׂẴ`�����N���X�g�̃`�����N�����������܂�...
    //
    while( numTotalOamDrawn < pMan->numUsedOam )
    {
       // �I�[�_�����O�e�[�u���I�[�܂ŒB������e�[�u���擪�ɖ߂�B
       if( i >= pMan->lengthOfOrderingTbl )
       {
           i = 0;
       }
       
       pOamList   = &pMan->pOamOrderingTbl[i];
       //
       // �`�����N���X�g�Ƀ`�����N���o�^����Ă����...
       //
       if( pOamList->numChunks != 0 )
       {
           const u16 currentCapacity = (u16)(capacityOfHW - numTotalOamDrawn);
           
           //
           // �`�悳���ׂ��`�����N���X�g�Ƃ��Đݒ肳��܂�
           // �܂��A�Ō�ɕ`�悳�ꂽ�`�����N���X�g�ԍ����X�V���܂�
           //
           pOamList->bDrawn                 = TRUE;
           pMan->lastRenderedOrderingTblIdx = i;
           
           //
           // ���X�g���̃`�����N�����ׂēo�^���\�Ȃ�
           //
           if( pOamList->numChunks <= currentCapacity )
           {
              pOamList->numLastFrameDrawn = 0;
              pOamList->numDrawn          = pOamList->numChunks;   
              
           }else{
              // �������X�g�I�[�܂ŕ`�悪���ꂽ�Ȃ�΁A���t���[���ł́A�{�`�����N���X�g�̎��̃��X�g����
              // �`����s���Ă����B
              if( (pOamList->numDrawn + pOamList->numLastFrameDrawn) / pOamList->numChunks > 0 )
              {
                  pMan->lastRenderedOrderingTblIdx = (u16)(i+1);
              }
              
              //
              // �O�t���[���ŕ`�悳�ꂽ�`�����N������A
              // ���t���[���ŕ`��O�ɗ^����I�t�Z�b�g�����v�Z���܂�
              //
              pOamList->numLastFrameDrawn = (u16)((pOamList->numDrawn + 
                                            pOamList->numLastFrameDrawn ) % pOamList->numChunks);
              
              pOamList->numDrawn          = currentCapacity;// �`���邾���`��    
              //
              // �I��
              //
              break;
           }
           numTotalOamDrawn += pOamList->numDrawn;
       }
       i++;
    }
}

/*---------------------------------------------------------------------------*
  Name:         NNS_G2dResetOamManExBuffer

  Description:  �}�l�[�W�������Z�b�g���܂��B
                                
  Arguments:    pMan        :   [OUT] �}�l�[�W������
                
  Returns:      �Ȃ�
  
 *---------------------------------------------------------------------------*/
void NNS_G2dResetOamManExBuffer( NNSG2dOamManagerInstanceEx* pOam )
{
    NNS_G2D_NULL_ASSERT( pOam );
    pOam->numUsedOam = 0;
    pOam->numAffineBufferUsed = 0;
    
    // reset the ordering table.
    {
        u16 i = 0;
        for( i = 0;i < pOam->lengthOfOrderingTbl; i++ )
        {
            pOam->pOamOrderingTbl[i].numChunks      = 0;
            pOam->pOamOrderingTbl[i].pChunks        = NULL;
            pOam->pOamOrderingTbl[i].pAffinedChunks = NULL;
            pOam->pOamOrderingTbl[i].pLastChunk        = NULL;
            pOam->pOamOrderingTbl[i].pLastAffinedChunk = NULL;
        }
    }
}

/*---------------------------------------------------------------------------*
  Name:         NNS_G2dGetOamManExInstance

  Description:  �}�l�[�W�����̂𐶐����܂��B


                
                
                
  Arguments:    pOam                    [OUT] �g��OAM�}�l�[�W�� ���� 
                pOamOrderingTbl         [IN]  �I�[�_�����O�e�[�u���̐擪�A�h���X 
                lengthOfOrderingTbl     [IN]  �I�[�_�����O�e�[�u���̒��� 
                numPooledOam            [IN]  OBJChunk�̌� 
                pPooledOam              [IN]  OBJChunk�z��ւ̃|�C���^ 
                lengthAffineBuffer      [IN]  �A�t�B���p�����[�^�o�b�t�@�z��̒��� 
                pAffineBuffer           [IN]  �A�t�B���p�����[�^�o�b�t�@�z��ւ̃|�C���^ 

                
  Returns:      �������̐���(���݂̂Ƃ���A�K����������)
  
 *---------------------------------------------------------------------------*/
BOOL NNS_G2dGetOamManExInstance
( 
    NNSG2dOamManagerInstanceEx*     pOam, 
    NNSG2dOamChunkList*             pOamOrderingTbl, 
    u8                              lengthOfOrderingTbl,
    u16                             numPooledOam,
    NNSG2dOamChunk*                 pPooledOam,
    u16                             lengthAffineBuffer,
    NNSG2dAffineParamProxy*         pAffineBuffer 
)
{
    NNS_G2D_NULL_ASSERT( pOam );
    NNS_G2D_ASSERT( lengthOfOrderingTbl != 0 );
    NNS_G2D_NULL_ASSERT( pPooledOam );
    NNS_G2D_ASSERT( numPooledOam != 0 );
    
    // �`�揇�^�C�v
    // (�ߋ��̃o�[�W�����Ƃ̌݊����ێ��̂��߂ɁA�f�t�H���g�͋t��)
    pOam->drawOrderType = NNSG2D_OAMEX_DRAWORDER_BACKWARD;
    
    //
    // �`�����N���X�g�v�f�̃[���N���A
    //
    MI_CpuClear32( pOamOrderingTbl, lengthOfOrderingTbl * sizeof( NNSG2dOamChunkList ) );
    
    
    pOam->pOamOrderingTbl           = pOamOrderingTbl;
    pOam->lengthOfOrderingTbl       = lengthOfOrderingTbl;
    
    pOam->numPooledOam              = numPooledOam;
    pOam->pPoolOamChunks            = pPooledOam;
    
    pOam->lengthAffineBuffer    = lengthAffineBuffer;
    pOam->pAffineBuffer         = pAffineBuffer;
    
    
    
    //
    // �o�^�֐��֘A�̏�����
    //
    {
        NNSG2dOamExEntryFunctions*  pFuncs = &pOam->oamEntryFuncs;
        
        pFuncs->getOamCapacity       = NULL;
        pFuncs->getAffineCapacity    = NULL;
        pFuncs->entryNewOam          = NULL;
        pFuncs->entryNewAffine       = NULL;
    }
    
    
    NNS_G2dResetOamManExBuffer( pOam );
    
    return TRUE;
}

/*---------------------------------------------------------------------------*
  Name:         NNSG2d_SetOamManExDrawOrderType

  Description:  �}�l�[�W����OAM�`�揇�̎�ނ�ݒ肵�܂��B
                OAM�`�揇�̋K��l�� NNSG2D_OAMEX_DRAWORDER_BACKWARD �ɂȂ��Ă���
                �_�ɂ����ӂ��������B
                ����͈ȑO�̃o�[�W�����Ƃ̌݊������ێ����邽�߂̑΍�ł��B

  Arguments:    pOam                 :   [OUT] �}�l�[�W������
                drawOrderType        :   [IN]  �`�揇�̎��
                
  Returns:      �Ȃ�
  
 *---------------------------------------------------------------------------*/
void NNSG2d_SetOamManExDrawOrderType
( 
    NNSG2dOamManagerInstanceEx*    pOam, 
    NNSG2dOamExDrawOrder           drawOrderType
)
{
    NNS_G2D_NULL_ASSERT( pOam );
    
    pOam->drawOrderType = drawOrderType;
}

/*---------------------------------------------------------------------------*
  Name:         NNS_G2dSetOamManExEntryFunctions

  Description:  �}�l�[�W���� �e��o�^�֐���ݒ肵�܂��B
                �}�l�[�W�����p�O�ɕK���Ăяo���āA�o�^�֐���ݒ肷��K�v������܂��B

                
                
                
  Arguments:    pMan        :   [OUT] �}�l�[�W������
                pSrc        :   [IN]  �e��o�^�֐�
                
  Returns:      �Ȃ�
  
 *---------------------------------------------------------------------------*/
void NNS_G2dSetOamManExEntryFunctions
( 
    NNSG2dOamManagerInstanceEx*        pMan, 
    const NNSG2dOamExEntryFunctions*   pSrc 
)
{
    NNS_G2D_NULL_ASSERT( pMan );
    {
        NNSG2dOamExEntryFunctions*  pDst = &pMan->oamEntryFuncs;    
        
        NNS_G2D_NULL_ASSERT( pSrc );

        if( pSrc->getOamCapacity != NULL )
        {
            pDst->getOamCapacity = pSrc->getOamCapacity;
        }
        
        if( pSrc->getAffineCapacity != NULL )
        {
            pDst->getAffineCapacity = pSrc->getAffineCapacity;
        }
        
        if( pSrc->entryNewOam != NULL )
        {
            pDst->entryNewOam = pSrc->entryNewOam;
        }
        
        if( pSrc->entryNewAffine != NULL )
        {
            pDst->entryNewAffine = pSrc->entryNewAffine;
        }
        
        NNS_G2D_ASSERT( IsOamEntryFuncsValid_( pMan, pDst ) );
    }
}

/*---------------------------------------------------------------------------*
  Name:         NNS_G2dEntryOamManExOam

  Description:  �g��OAM�}�l�[�W����OBJ���A�`�掞�̓o�^�����w�肵�ēo�^���܂��B
                ���ۂɃn�[�h�E�G�A�ɓK�p�����Ƃ��ɁA�w�肵�����ԂœK�p���s���܂��B

                
                �\���ȗe�ʂ�����ꍇ�� �o�^���s�� TRUE��Ԃ��܂�
                
                
                
  Arguments:    pMan        :   [OUT] �}�l�[�W������
                pOam        :   [IN]  OAMAttribute 
                priority    :   [IN]  �`��D��x
                affineIdx   :   [IN]  �A�t�B���E�C���f�b�N�X
                
  Returns:      �o�^�̐���
  
 *---------------------------------------------------------------------------*/
BOOL NNS_G2dEntryOamManExOam
( 
    NNSG2dOamManagerInstanceEx*    pMan, 
    const GXOamAttr*               pOam, 
    u8                             priority 
)
{
    NNS_G2D_NULL_ASSERT( pMan );
    NNS_G2D_NULL_ASSERT( pOam );
    NNS_G2D_ASSERT( IsPriorityValid_( pMan, priority ) );
    
    {
        NNSG2dOamChunkList*   pOamList    = &pMan->pOamOrderingTbl[ priority ];
        NNSG2dOamChunk*       pOamChunk   = GetNewOamChunk_( pMan, pOam );
         
        return EntryNewOamWithAffine_( pOamList, 
                                       pOamChunk, 
                                       NNS_G2D_OAM_AFFINE_IDX_NONE, 
                                       pMan->drawOrderType );
    }
}

/*---------------------------------------------------------------------------*
  Name:         NNS_G2dEntryOamManExOamWithAffineIdx

  Description:  �}�l�[�W���� AffineIndex���w�肵�� OAMAttribute ��o�^���܂�
                NNS_G2dEntryOamManExAffine()�̕Ԃ�l���w�肵�Ă��������B
                
                �\���ȗe�ʂ�����ꍇ�� �o�^���s�� TRUE��Ԃ��܂�
                
                
                
  Arguments:    pMan        :   [OUT] �}�l�[�W������
                pOam        :   [IN]  OAMAttribute 
                priority    :   [IN]  �`��D��x
                affineIdx   :   [IN]  �A�t�B���E�C���f�b�N�X
                
  Returns:      �o�^�̐���
  
 *---------------------------------------------------------------------------*/
BOOL NNS_G2dEntryOamManExOamWithAffineIdx
( 
    NNSG2dOamManagerInstanceEx*    pMan, 
    const GXOamAttr*               pOam, 
    u8                             priority, 
    u16                            affineIdx 
)
{
    NNS_G2D_NULL_ASSERT( pMan );
    NNS_G2D_NULL_ASSERT( pOam );
    // �A�t�B���ϊ����L���Ǝw�肳��Ă���OAM�A�g���r���[�g���H
    NNS_G2D_ASSERT( pOam->rsMode & 0x1 );
    NNS_G2D_ASSERT( IsPriorityValid_( pMan, priority ) );
    
    {
        NNSG2dOamChunkList*   pOamList    = &pMan->pOamOrderingTbl[ priority ];
        NNSG2dOamChunk*       pOamChunk   = GetNewOamChunk_( pMan, pOam );
         
        return EntryNewOamWithAffine_( pOamList, 
                                       pOamChunk, 
                                       affineIdx, 
                                       pMan->drawOrderType );
    }
}

/*---------------------------------------------------------------------------*
  Name:         NNS_G2dEntryOamManExAffine

  Description:  �}�l�[�W����Affine�p�����[�^��o�^���܂�
                �}�l�[�W���ɏ\���ȗe�ʂ�����΁Aaffine�ϊ��s���
                ������ NNSG2dAffineParamProxy �� �i�[����AIndex ���Ԃ�܂��B
                �A�t�B���p�����[�^�p�o�b�t�@�z�񂪐ݒ肳��Ă��Ȃ��ꍇ�́A�A�T�[�g�Ɏ��s���܂��B
                
                OBJ �� �}�l�[�W�������ł� 
                NNSG2dAffineParamProxy �� Index�ɂ����Affine�p�����[�^���Q�Ƃ��܂��B
                
                
                
  Arguments:    pMan:      [OUT] �}�l�[�W������
                mtx :      [IN]  affine�ϊ��s��
  
  Returns:      ������ NNSG2dAffineParamProxy �� Index
  
 *---------------------------------------------------------------------------*/
u16 NNS_G2dEntryOamManExAffine
( 
    NNSG2dOamManagerInstanceEx*    pMan, 
    const MtxFx22*                 mtx 
)
{
    NNS_G2D_NULL_ASSERT( pMan ); 
    NNS_G2D_NULL_ASSERT( mtx );
    NNS_G2D_ASSERT( IsAffineProxyValid_( pMan ) );
    
    if( HasEnoughCapacity_( pMan ) )
    {
        NNSG2dAffineParamProxy* pAffineProxy 
           = &pMan->pAffineBuffer[pMan->numAffineBufferUsed];
           
        pAffineProxy->mtxAffine     = *mtx;
        pAffineProxy->affineHWIndex = NNS_G2D_OAMEX_HW_ID_NOT_INIT;
        
        return pMan->numAffineBufferUsed++;
    }   
    
    return NNS_G2D_OAM_AFFINE_IDX_NONE;
}



/*---------------------------------------------------------------------------*
  Name:         NNS_G2dApplyOamManExToBaseModule 

  Description:  �}�l�[�W���̓��e��HW�ɔ��f����B
                ���ۂɂ� HW�ɔ��f ����킯�ł͂Ȃ��A���ʃ��W���[���ɔ��f������B
                ���ʃ��W���[����HW�ւ��̓��e�̔��f���s���܂ł́A�`��ɔ��f����Ȃ��B
                
                
                
  Arguments:    pMan:      [OUT] �}�l�[�W������
  Returns:      �Ȃ�
  
 *---------------------------------------------------------------------------*/
void NNS_G2dApplyOamManExToBaseModule( NNSG2dOamManagerInstanceEx* pMan )
{
    NNS_G2D_NULL_ASSERT( pMan );
    NNS_G2D_NULL_ASSERT( pMan->oamEntryFuncs.getOamCapacity );
    {
        u16       numTotalOamDrawn = 0;
        const u16 capacityOfHW     = ( *(pMan->oamEntryFuncs.getOamCapacity) )();
        
        if( pMan->numUsedOam != 0 )
        {
            //
            // affine�ϊ����ꂽOam�̏���������
            // 
            if( pMan->numAffineBufferUsed != 0 )
            {
                // Affine Proxy �� ���e�� ���ʃ��W���[���ɓK�p���AAffine Index �����肷��
                LoadAffineProxyToBaseModule_( pMan );
                // ��Index
                ReindexAffinedOams_( pMan );
            }
            
            //
            // �`�����N���X�g�̓o�^�͈͂��v�Z���܂�
            //
            CalcDrawnListArea_( pMan );
            
            //
            // �O�����W���[���ɑ΂��Ď��ۂɕ`��o�^���s���܂�
            //
            {
                u16 i = 0;
                NNSG2dOamChunk*       pChunk    = NULL;
                NNSG2dOamChunkList*   pOamList  = NULL;
        
                for( i = 0; i < pMan->lengthOfOrderingTbl; i++ )
                {
                    pOamList  = &pMan->pOamOrderingTbl[i];
                    //
                    // �`�悷��K�v�̂���`�����N���X�g�Ȃ��...
                    //  
                    if( pOamList->bDrawn )
                    {
                        NNS_G2D_ASSERT( pOamList->numLastFrameDrawn < pOamList->numChunks );
                        
                        pChunk = AdvancePointer_( pOamList->pChunks, pOamList->numLastFrameDrawn );               
                         
                        numTotalOamDrawn = DrawOamChunks_( pMan, 
                                                 pOamList, 
                                                 pChunk, 
                                                 pOamList->numDrawn, 
                                                 capacityOfHW, 
                                                 numTotalOamDrawn );
                    }
                }
            }
        }
        
        //
        // �c��e�ʂ̓f�t�H���g�l�ŃN���A����
        //
        while( capacityOfHW > numTotalOamDrawn )
        {
            // �f�t�H���g�l�ŃN���A
            EntryOamToToBaseModule_( pMan, &defaultOam_, numTotalOamDrawn );
            numTotalOamDrawn++;
        }
    }
}




