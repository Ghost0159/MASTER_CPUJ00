/*---------------------------------------------------------------------------*
  Project:  NITRO-System - include - nnsys - g2d
  File:     g2d_CellTransferManager.h

  Copyright 2004-2007 Nintendo.  All rights reserved.

  These coded instructions, statements, and computer programs contain
  proprietary information of Nintendo of America Inc. and/or Nintendo
  Company Ltd., and are protected by Federal copyright law.  They may
  not be disclosed to third parties or copied or duplicated in any form,
  in whole or in part, without the prior written consent of Nintendo.

  $Revision: 1.10 $
 *---------------------------------------------------------------------------*/
 
#ifndef NNS_G2D_CELL_TRANSFER_MANAGER_H_
#define NNS_G2D_CELL_TRANSFER_MANAGER_H_

#include <nitro.h>
#include <nnsys/g2d/g2d_config.h>
#include <nnsys/g2d/g2d_Image.h>

#ifdef __cplusplus
extern "C" {
#endif

// ��ɕs�v�ƂȂ�H
#include <nnsys/gfd/VramTransferMan/gfd_VramTransferManager.h>

#define NNS_G2D_INVALID_CELL_TRANSFER_STATE_HANDLE    (u32)0xFFFFFFFF


/*---------------------------------------------------------------------------*
  Name:         VramTransferTaskRegisterFuncPtr

  Description:  Vram �]���^�X�N�̓o�^���s���֐��ւ̃|�C���^�ł��B
                
 *---------------------------------------------------------------------------*/
typedef BOOL (*VramTransferTaskRegisterFuncPtr)( NNS_GFD_DST_TYPE  type, 
                                              u32               dstAddr, 
                                              void*             pSrc, 
                                              u32               szByte );

//------------------------------------------------------------------------------
//
// NNSG2dCellTransferState 
//
//------------------------------------------------------------------------------
typedef struct NNSG2dCellTransferState
{
    //
    // �������t�F�[�Y�Őݒ肳��郁���o
    //
    NNSG2dVRamLocation    dstVramLocation;      // �]����̉摜�A�h���X(�e�̈�̃T�C�Y��szDst�ȏ�ł��邱��)
    u32                   szDst;                // �]����̗̈�̃T�C�Y
    
    const void*          pSrcNCGR;             // �]�����f�[�^�i�L�����N�^�����j
    const void*          pSrcNCBR;             // �]�����f�[�^�i�r�b�g�}�b�v�����j
    u32                   szSrcData;            // �]�����f�[�^�̑傫���i����ł��邱�Ɓj
    BOOL                  bActive;              // �A�N�e�B�u�ȏ�Ԃ��H
    
    //
    // ���t���[���X�V����郁���o
    //
    u32                   bDrawn;               // �`�悳�ꂽ���H
                                                // �O���t�B�b�N�X�G���W�����Ƃɕ`�悳�ꂽ���A�ǂ����̏�Ԃ��L������܂��B
                                                // �`�悳��Ȃ��Z����Vram�]�����������ړI�Ŏg�p����܂��B
                                                // �`�惂�W���[���ɂ���ăZ�b�g����
                                                // �Ǘ����W���[�� �ɂ���� ���t���[�����t���b�V������܂�   
                                                // ���[�U���Ǝ��̕`�惂�W���[�����\�z����ꍇ�́A�{�����o��K�؂ɐݒ肷��K�v������܂��B
                                            
    u32                   bTransferRequested;   // �]���v�������������H
                                                // �O���t�B�b�N�X�G���W�����Ƃɓ]���v���̏�Ԃ��L������܂��B
                                                // �Z���A�j���[�V�������䃂�W���[�����ݒ肵�܂�
                                                // �Ǘ����W���[�� �ɂ���� �]���^�X�N�o�^�����������ۂɃ��Z�b�g����܂��B
    
    u32                   srcOffset;            // �\�[�X�I�t�Z�b�g�i�]���v���̏ڍׁj
    u32                   szByte;               // �]���T�C�Y      �i�]���v���̏ڍׁj
    
}NNSG2dCellTransferState;



//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
void 
NNS_G2dInitCellTransferStateManager
( 
    NNSG2dCellTransferState*            pCellStateArray, 
    u32                                 numCellState,
    VramTransferTaskRegisterFuncPtr     pTaskRegisterFunc
);

//------------------------------------------------------------------------------
// �n���h���擾�A�ԋp
u32 
NNS_G2dGetNewCellTransferStateHandle();
void
NNS_G2dFreeCellTransferStateHandle( u32 handle );

//------------------------------------------------------------------------------
void NNS_G2dUpdateCellTransferStateManager();

//------------------------------------------------------------------------------
// �]�����N�G�X�g�֘A
// �Z���A�j���[�V�������� �����΂��
// 
void NNS_G2dSetCellTransferStateRequested
( 
    u32                                 handle, 
    u32                                 srcOffset,
    u32                                 szByte
);







//------------------------------------------------------------------------------
// internal functions 
//------------------------------------------------------------------------------
NNSG2dCellTransferState* 
NNSi_G2dGetCellTransferState
( 
    u32 handle 
);

//------------------------------------------------------------------------------
void NNSi_G2dInitCellTransferState
( 
    u32                   handle,
    
    u32                   dstAddr3D,
    u32                   dstAddr2DMain,
    u32                   dstAddr2DSub,
    u32                   szDst,
    
    const void*          pSrcNCGR,
    const void*          pSrcNCBR,
    u32                   szSrcData
);


//------------------------------------------------------------------------------
// inline functions 
//------------------------------------------------------------------------------
// �t���O���쓙�̃A�N�Z�T�F�������J�֐��ł��B
//------------------------------------------------------------------------------
NNS_G2D_INLINE void 
NNSi_G2dSetCellTransferStateRequestFlag( NNSG2dCellTransferState* pState, NNS_G2D_VRAM_TYPE type, BOOL flag )
{
    pState->bTransferRequested = ( pState->bTransferRequested & ~( 0x1 << type ) ) | ( flag << type );
}

NNS_G2D_INLINE void 
NNSi_G2dSetVramTransferRequestFlag( u32 handle, NNS_G2D_VRAM_TYPE type, BOOL flag )
{
    NNSG2dCellTransferState* pState = NNSi_G2dGetCellTransferState( handle );
    
    NNSi_G2dSetCellTransferStateRequestFlag( pState, type, flag );
}


//------------------------------------------------------------------------------
NNS_G2D_INLINE BOOL 
NNSi_G2dGetCellTransferStateRequestFlag( const NNSG2dCellTransferState* pState, NNS_G2D_VRAM_TYPE type )
{
    return (BOOL)( pState->bTransferRequested & ( 0x1 << type ) );
}

NNS_G2D_INLINE BOOL 
NNSi_G2dGetVramTransferRequestFlag( u32 handle, NNS_G2D_VRAM_TYPE type )
{
    const NNSG2dCellTransferState* pState = NNSi_G2dGetCellTransferState( handle );
    return NNSi_G2dGetCellTransferStateRequestFlag( pState, type );
}

//------------------------------------------------------------------------------
NNS_G2D_INLINE void 
NNSi_G2dSetCellTransferStateCellDrawnFlag( NNSG2dCellTransferState* pState, NNS_G2D_VRAM_TYPE type, BOOL flag )
{
    pState->bDrawn = ( pState->bDrawn & ~( 0x1 << type ) ) | ( flag << type );
}

NNS_G2D_INLINE void 
NNSi_G2dSetVramTransferCellDrawnFlag( u32 handle, NNS_G2D_VRAM_TYPE type, BOOL flag )
{
    NNSG2dCellTransferState* pState = NNSi_G2dGetCellTransferState( handle );
    NNSi_G2dSetCellTransferStateCellDrawnFlag( pState, type, flag );
}

//------------------------------------------------------------------------------
NNS_G2D_INLINE BOOL 
NNSi_G2dGetCellTransferStateCellDrawnFlag( const NNSG2dCellTransferState* pState, NNS_G2D_VRAM_TYPE type )
{
    return (BOOL)( pState->bDrawn & ( 0x1 << type ) );
}

NNS_G2D_INLINE BOOL 
NNSi_G2dGetVramTransferCellDrawnFlag( u32 handle, NNS_G2D_VRAM_TYPE type )
{
    const NNSG2dCellTransferState* pState = NNSi_G2dGetCellTransferState( handle );
    return NNSi_G2dGetCellTransferStateCellDrawnFlag( pState, type );
}


#ifdef __cplusplus
} /* extern "C" */
#endif

#endif // NNS_G2D_CELL_TRANSFER_MANAGER_H_
