/*---------------------------------------------------------------------------*
  Project:  NITRO-System - libraries - g2d
  File:     g2di_Dma.h

  Copyright 2004-2007 Nintendo.  All rights reserved.

  These coded instructions, statements, and computer programs contain
  proprietary information of Nintendo of America Inc. and/or Nintendo
  Company Ltd., and are protected by Federal copyright law.  They may
  not be disclosed to third parties or copied or duplicated in any form,
  in whole or in part, without the prior written consent of Nintendo.

  $Revision: 1.3 $
 *---------------------------------------------------------------------------*/
#ifndef G2DI_DMA_H_
#define G2DI_DMA_H_

#include <nitro.h>

#ifdef __cplusplus
extern "C" {
#endif

/*---------------------------------------------------------------------------*
  Name:         NNSi_G2dDmaCopy16

  Description:  Dma�R�s�[���s���܂��B

  Arguments:    dmaNo:  Dma�ԍ�
                        ( GX_DMA_NOT_USE���w�肳�ꂽ�ꍇ��
                          Dma�R�s�[�̑����Cpu�R�s�[���g�p����܂��B)
                src:    �]�����A�h���X
                dest:   �]����A�h���X
                size:   �]���T�C�Y

  Returns:      �Ȃ��B
 *---------------------------------------------------------------------------*/
NNS_G2D_INLINE void NNSi_G2dDmaCopy16( u32 dmaNo, const void* src, void* dest, u32 size )
{
    // dmaNo �� GX_DMA_NOT_USE ���w�肳�ꂽ�ꍇ�́ACpuCopy���g�p���܂��B
    if( dmaNo != GX_DMA_NOT_USE )
    {
        MI_DmaCopy16( dmaNo, src, dest, size );
    }else{
        MI_CpuCopy16( src, dest, size );
    }
}
                      

/*---------------------------------------------------------------------------*
  Name:         NNSi_G2dDmaFill32

  Description:  ���������w��̃f�[�^�Ŗ��߂܂��B

  Arguments:    dmaNo:  Dma�ԍ�
                        ( GX_DMA_NOT_USE���w�肳�ꂽ�ꍇ��
                          DmaFill�̑����CpuFill���g�p����܂��B)
                dest:   �]����A�h���X
                data:   �]���f�[�^
                size:   �]���T�C�Y

  Returns:      �Ȃ��B
 *---------------------------------------------------------------------------*/
NNS_G2D_INLINE void NNSi_G2dDmaFill32( u32 dmaNo, void* dest, u32 data, u32 size )
{
    // dmaNo �� GX_DMA_NOT_USE ���w�肳�ꂽ�ꍇ�́ACpuFill���g�p���܂��B
    if( dmaNo != GX_DMA_NOT_USE )
    {
        MI_DmaFill32( dmaNo, dest, data, size );
    }else{
        MI_CpuFill32( dest, data, size );
    }
}

#ifdef __cplusplus
}/* extern "C" */
#endif

#endif // G2DI_DMA_H_

