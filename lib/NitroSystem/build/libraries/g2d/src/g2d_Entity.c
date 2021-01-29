/*---------------------------------------------------------------------------*
  Project:  NITRO-System - libraries - g2d
  File:     g2d_Entity.c

  Copyright 2004-2007 Nintendo.  All rights reserved.

  These coded instructions, statements, and computer programs contain
  proprietary information of Nintendo of America Inc. and/or Nintendo
  Company Ltd., and are protected by Federal copyright law.  They may
  not be disclosed to third parties or copied or duplicated in any form,
  in whole or in part, without the prior written consent of Nintendo.

  $Revision: 1.15 $
 *---------------------------------------------------------------------------*/

#include <nnsys/g2d/g2d_Entity.h>
#include <nnsys/g2d/g2d_CellAnimation.h>
#include <nnsys/g2d/g2d_MultiCellAnimation.h>
#include <nnsys/g2d/load/g2d_NAN_load.h>
#include "g2d_Internal.h"

//------------------------------------------------------------------------------
static NNS_G2D_INLINE void ResetPaletteTbl_( NNSG2dEntity* pEntity )
{
    
    NNS_G2D_ASSERT_ENTITY_VALID( pEntity );
    
    
    NNS_G2D_NULL_ASSERT( pEntity );
    
    // �J���[�p���b�g�ϊ��e�[�u���̃��Z�b�g
    NNS_G2dResetEntityPaletteTable( pEntity );
}

//------------------------------------------------------------------------------
static NNS_G2D_INLINE void SetCurrentAnimation_( NNSG2dEntity* pEntity )
{
    NNS_G2D_NULL_ASSERT( pEntity );
    NNS_G2D_ASSERT_ENTITY_VALID( pEntity );
    
    {
        const NNSG2dAnimSequence* pAnimSeq = NNS_G2dGetAnimSequenceByIdx( pEntity->pAnimDataBank,
                                                             pEntity->currentSequenceIdx );
        if( pAnimSeq )
        {
            switch( pEntity->pEntityData->type )
            {
            case NNS_G2D_ENTITYTYPE_CELL:
                NNS_G2dSetCellAnimationSequence( (NNSG2dCellAnimation*)pEntity->pDrawStuff, pAnimSeq );
                break;
            case NNS_G2D_ENTITYTYPE_MULTICELL:
                NNS_G2dSetAnimSequenceToMCAnimation( (NNSG2dMultiCellAnimation*)pEntity->pDrawStuff, pAnimSeq );
                break;
            default:
                NNS_G2D_ASSERT( FALSE );
            }
        }else{
            NNS_G2D_ASSERT( FALSE );
        }
    }   
}

/*---------------------------------------------------------------------------*
  Name:         NNS_G2dInitEntity

  Description:  Entity ���̂����������܂�
                
                
  Arguments:    pEntity         [OUT] �G���e�B�e�B 
                pDrawStuff      [IN]  �`��p�f�[�^( NNSG2dCellAnimation or NNSG2dMultiCellAnimation ) 
                pEntityData     [IN]  �G���e�B�e�B��`�f�[�^ 
                pAnimDataBank   [IN]  �A�j���[�V�����f�[�^�o���N 

  Returns:      �Ȃ�
  
 *---------------------------------------------------------------------------*/
void NNS_G2dInitEntity
( 
    NNSG2dEntity*               pEntity, 
    void*                       pDrawStuff, 
    const NNSG2dEntityData*     pEntityData, 
    const NNSG2dAnimBankData*   pAnimDataBank 
)
{
    NNS_G2D_NULL_ASSERT( pEntity );
    NNS_G2D_NULL_ASSERT( pDrawStuff );
    NNS_G2D_NULL_ASSERT( pEntityData );
    NNS_G2D_NULL_ASSERT( pAnimDataBank );
    
    
    // pEntityData->type
    pEntity->pDrawStuff             = pDrawStuff;
    pEntity->pAnimDataBank          = pAnimDataBank;
    pEntity->pEntityData            = pEntityData;
    pEntity->currentSequenceIdx     = 0;
     
    // �J���[�p���b�g�ϊ��e�[�u��������������
    ResetPaletteTbl_( pEntity );
}



/*---------------------------------------------------------------------------*
  Name:         NNS_G2dSetEntityCurrentAnimation

  Description:  NNSG2dEntity �̌��݂̍Đ��A�j���[�V������ݒ肷��
                
                
  Arguments:    pEntity:            [OUT] NNSG2dEntity ����
                idx:                [IN]  NNSG2dEntity ���ł� AnimationSequence Index
                
                �A�j���[�V�����̓��[�h�ς݂ł��邱�ƁB
                ���[�h����Ă��Ȃ���΂Ȃɂ����܂���B
                
  Returns:      �Ȃ�
  
 *---------------------------------------------------------------------------*/
void NNS_G2dSetEntityCurrentAnimation( NNSG2dEntity* pEntity, u16 idx )
{
    NNS_G2D_NULL_ASSERT( pEntity );
    NNS_G2D_NULL_ASSERT( pEntity->pAnimDataBank );
    NNS_G2D_NULL_ASSERT( pEntity->pEntityData );
    
    if( pEntity->pEntityData->animData.numAnimSequence > idx )
    {
        pEntity->currentSequenceIdx = pEntity->pEntityData->animData.pAnimSequenceIdxArray[idx];
        // TODO:���̏����͏��������� �V�[�P���X�ԍ��z��̗v�f�S���ɑ΂��čs���ׂ��I
        NNS_G2D_ASSERT( pEntity->pAnimDataBank->numSequences > pEntity->currentSequenceIdx );
        //
        // �A�j���[�V�����؂�ւ�����
        //
        SetCurrentAnimation_( pEntity );
    }else{
        NNSI_G2D_DEBUGMSG0( FALSE, "Failure of finding animation data in NNS_G2dSetEntityCurrentAnimation()" );
    }
}

/*---------------------------------------------------------------------------*
  Name:         NNS_G2dSetColorPaletteTable

  Description:  NNSG2dEntity �� �J���[�p���b�g�ϊ��e�[�u����ݒ肷��
                
                
  Arguments:    pEntity:            [OUT] NNSG2dEntity ����
                pPlttTbl:           [IN]  �J���[�p���b�g�ϊ��e�[�u��
                
                
  Returns:      �Ȃ�
  
 *---------------------------------------------------------------------------*/
void NNS_G2dSetEntityPaletteTable
( 
    NNSG2dEntity*           pEntity, 
    NNSG2dPaletteSwapTable* pPlttTbl 
)
{
    NNS_G2D_NULL_ASSERT( pEntity );
    NNS_G2D_NULL_ASSERT( pPlttTbl );
    
    pEntity->pPaletteTbl = pPlttTbl;
}

/*---------------------------------------------------------------------------*
  Name:         NNS_G2dResetEntityPaletteTable

  Description:  NNSG2dEntity �� �J���[�p���b�g�ϊ��e�[�u���𖳌��ɐݒ肷��
                
                
  Arguments:    pEntity:            [OUT] NNSG2dEntity ����
                
                
  Returns:      �Ȃ�
  
 *---------------------------------------------------------------------------*/
void NNS_G2dResetEntityPaletteTable
( 
    NNSG2dEntity*           pEntity
)
{
    NNS_G2D_NULL_ASSERT( pEntity );
    pEntity->pPaletteTbl = NULL;
}

/*---------------------------------------------------------------------------*
  Name:         NNS_G2dIsEntityPaletteTblEnable

  Description:  NNSG2dEntity �� �J���[�p���b�g���L������������
                
                
  Arguments:    pEntity:            [IN]  NNSG2dEntity ����
                
                
                
  Returns:      �J���[�p���b�g���L���Ȃ�TRUE
  
 *---------------------------------------------------------------------------*/
BOOL NNS_G2dIsEntityPaletteTblEnable( const NNSG2dEntity* pEntity )
{
    NNS_G2D_NULL_ASSERT( pEntity );
    return ( pEntity->pPaletteTbl != NULL ) ? TRUE : FALSE;
}


/*---------------------------------------------------------------------------*
  Name:         NNS_G2dTickEntity

  Description:  NNSG2dEntity ���X�V����
                
                
  Arguments:    pEntity:            [OUT] NNSG2dEntity ����
                dt:                 [IN]  �i�߂鎞�ԁi�t���[���j
                
                
  Returns:      �Ȃ�
  
 *---------------------------------------------------------------------------*/
void  NNS_G2dTickEntity( NNSG2dEntity*    pEntity, fx32 dt )
{
    NNS_G2D_NULL_ASSERT( pEntity );
    NNS_G2D_ASSERT_ENTITY_VALID( pEntity );
    
    switch( pEntity->pEntityData->type )
    {
    case NNS_G2D_ENTITYTYPE_CELL:
        NNS_G2dTickCellAnimation( (NNSG2dCellAnimation*)pEntity->pDrawStuff, dt );
        break;
    case NNS_G2D_ENTITYTYPE_MULTICELL:
        NNS_G2dTickMCAnimation( (NNSG2dMultiCellAnimation*)pEntity->pDrawStuff, dt );
        break;
    default:
        NNS_G2D_ASSERT(FALSE);
    }
}

/*---------------------------------------------------------------------------*
  Name:         NNS_G2dSetEntityCurrentFrame

  Description:  NNSG2dEntity �̍Đ��A�j���[�V�����t���[����ݒ肵�܂�
                
                
  Arguments:    pEntity:            [OUT] NNSG2dEntity ����
                frameIndex:         [IN]  �A�j���[�V�����t���[���ԍ�
                
                
  Returns:      �Ȃ�
  
 *---------------------------------------------------------------------------*/
void  NNS_G2dSetEntityCurrentFrame
( 
    NNSG2dEntity*    pEntity,
    u16              frameIndex  
)
{
    NNS_G2D_NULL_ASSERT( pEntity );
    NNS_G2D_ASSERT_ENTITY_VALID( pEntity );
    
    switch( pEntity->pEntityData->type )
    {
    case NNS_G2D_ENTITYTYPE_CELL:
        NNS_G2dSetCellAnimationCurrentFrame( (NNSG2dCellAnimation*)pEntity->pDrawStuff, 
                                             frameIndex );
        break;
    case NNS_G2D_ENTITYTYPE_MULTICELL:
        NNS_G2dSetMCAnimationCurrentFrame( (NNSG2dMultiCellAnimation*)pEntity->pDrawStuff,
                                             frameIndex );
        break;
    default:
        NNS_G2D_ASSERT(FALSE);
    }
}

/*---------------------------------------------------------------------------*
  Name:         NNS_G2dSetEntitySpeed

  Description:  NNSG2dEntity �̃A�j���[�V�����X�s�[�h��ݒ肷��
                
                
  Arguments:    pEntity:            [OUT] NNSG2dEntity ����
                speed:              [IN]  �A�j���[�V�����X�s�[�h
                
                
  Returns:      �Ȃ�
  
 *---------------------------------------------------------------------------*/
void NNS_G2dSetEntitySpeed
(
    NNSG2dEntity*     pEntity,
    fx32              speed  
)
{
    NNS_G2D_NULL_ASSERT( pEntity );
    NNS_G2D_ASSERT_ENTITY_VALID( pEntity );
    
    switch( pEntity->pEntityData->type )
    {
    case NNS_G2D_ENTITYTYPE_CELL:
        NNS_G2dSetCellAnimationSpeed( (NNSG2dCellAnimation*)pEntity->pDrawStuff, 
                                             speed );
        break;
    case NNS_G2D_ENTITYTYPE_MULTICELL:
        NNS_G2dSetMCAnimationSpeed( (NNSG2dMultiCellAnimation*)pEntity->pDrawStuff,
                                             speed );
        break;
    default:
        NNS_G2D_ASSERT(FALSE);
    }
}

/*---------------------------------------------------------------------------*
  Name:         NNS_G2dIsEntityValid

  Description:  NNSG2dEntity �� �L���ȏ�Ԃ��������܂�
                
                
  Arguments:    pEntity:            [IN]  NNSG2dEntity ����
                
                
  Returns:      �L���ȏ�ԂȂ�TRUE��������
  
 *---------------------------------------------------------------------------*/
BOOL NNS_G2dIsEntityValid( NNSG2dEntity*    pEntity )
{
    NNS_G2D_NULL_ASSERT( pEntity );
    
    if( ( pEntity->pEntityData     != NULL ) &&
        ( pEntity->pDrawStuff      != NULL ) &&
        ( pEntity->pAnimDataBank   != NULL )  )
        {
            return TRUE;
        }else{
            return FALSE;
        }
}

/*---------------------------------------------------------------------------*
  Name:         NNS_G2dGetEntityAnimCtrl

  Description:  �G���e�B�e�B�̃A�j���[�V�����R���g���[�����擾���܂��B
                
                
  Arguments:    pEntity:            [IN]  NNSG2dEntity ����
                
                
  Returns:      �A�j���[�V�����R���g���[��
  
 *---------------------------------------------------------------------------*/
NNSG2dAnimController* NNS_G2dGetEntityAnimCtrl( NNSG2dEntity*    pEntity )
{
    NNS_G2D_NULL_ASSERT( pEntity );
    switch( pEntity->pEntityData->type )
    {
    case NNS_G2D_ENTITYTYPE_CELL:
        return NNS_G2dGetCellAnimationAnimCtrl( (NNSG2dCellAnimation*)pEntity->pDrawStuff );
    case NNS_G2D_ENTITYTYPE_MULTICELL:
        return NNS_G2dGetMCAnimAnimCtrl( (NNSG2dMultiCellAnimation*)pEntity->pDrawStuff );
    default:
        NNS_G2D_ASSERT(FALSE);
        return NULL;
    }
}



