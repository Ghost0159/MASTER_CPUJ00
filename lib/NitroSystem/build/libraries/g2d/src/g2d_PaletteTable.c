/*---------------------------------------------------------------------------*
  Project:  NITRO-System - libraries - g2d
  File:     g2d_PaletteTable.c

  Copyright 2004-2007 Nintendo.  All rights reserved.

  These coded instructions, statements, and computer programs contain
  proprietary information of Nintendo of America Inc. and/or Nintendo
  Company Ltd., and are protected by Federal copyright law.  They may
  not be disclosed to third parties or copied or duplicated in any form,
  in whole or in part, without the prior written consent of Nintendo.

  $Revision: 1.5 $
 *---------------------------------------------------------------------------*/

#include <nnsys/g2d/g2d_PaletteTable.h>

/*---------------------------------------------------------------------------*
  Name:         NNS_G2dInitializePaletteTable

  Description:  �p���b�g�ϊ��e�[�u�������������܂�
                
  Arguments:    pPlttTbl:      [OUT] �p���b�g����
                
                
  Returns:      �Ȃ�
  
 *---------------------------------------------------------------------------*/
void NNS_G2dInitializePaletteTable( NNSG2dPaletteSwapTable* pPlttTbl )
{
    u16     i;
    NNS_G2D_NULL_ASSERT( pPlttTbl );
    
    
    // ����ւ� �Ȃ� �ɐݒ�
    for( i = 0; i < NNS_G2D_NUM_COLOR_PALETTE; i++ )
    {
        pPlttTbl->paletteIndex[i] = i;
    }    
}

/*---------------------------------------------------------------------------*
  Name:         NNS_G2dInitializePaletteTable

  Description:  �p���b�g�ϊ��e�[�u����ݒ肵�܂�
                
  Arguments:    pPlttTbl:       [OUT] �p���b�g����
                beforeIdx:      [IN]  �ϊ��O�̃p���b�g�ԍ�
                afterIdx:       [IN]  �ϊ���̃p���b�g�ԍ�
                
  Returns:      �Ȃ�
  
 *---------------------------------------------------------------------------*/
void NNS_G2dSetPaletteTableValue
( 
    NNSG2dPaletteSwapTable* pPlttTbl, 
    u16                     beforeIdx, 
    u16                     afterIdx 
)
{
    NNS_G2D_NULL_ASSERT( pPlttTbl );
    NNS_G2D_MINMAX_ASSERT( beforeIdx, 0, NNS_G2D_NUM_COLOR_PALETTE );
    NNS_G2D_MINMAX_ASSERT( afterIdx, 0, NNS_G2D_NUM_COLOR_PALETTE );
    
    pPlttTbl->paletteIndex[beforeIdx] = afterIdx;
}

/*---------------------------------------------------------------------------*
  Name:         NNS_G2dInitializePaletteTable

  Description:  �p���b�g�ϊ��e�[�u���̐ݒ���擾���܂�
                
  Arguments:    pPlttTbl:      [IN]  �p���b�g����
                beforeIdx:     [IN]  �ϊ��O�̃p���b�g�ԍ�
                
  Returns:      �ϊ���̃p���b�g�ԍ�
  
 *---------------------------------------------------------------------------*/
u16 NNS_G2dGetPaletteTableValue( const NNSG2dPaletteSwapTable* pPlttTbl, u16 beforeIdx )
{
    NNS_G2D_NULL_ASSERT( pPlttTbl );
    NNS_G2D_MINMAX_ASSERT( beforeIdx, 0, NNS_G2D_NUM_COLOR_PALETTE );
    
    {
        const u16       afterIdx = pPlttTbl->paletteIndex[beforeIdx];
        NNS_G2D_MINMAX_ASSERT( afterIdx, 0, NNS_G2D_NUM_COLOR_PALETTE );
        return afterIdx;
    }
}

