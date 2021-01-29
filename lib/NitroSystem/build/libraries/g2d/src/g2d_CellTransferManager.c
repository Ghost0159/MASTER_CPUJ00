/*---------------------------------------------------------------------------*
  Project:  NITRO-System - libraries - g2d
  File:     g2d_CellTransferManager.c

  Copyright 2004-2007 Nintendo.  All rights reserved.

  These coded instructions, statements, and computer programs contain
  proprietary information of Nintendo of America Inc. and/or Nintendo
  Company Ltd., and are protected by Federal copyright law.  They may
  not be disclosed to third parties or copied or duplicated in any form,
  in whole or in part, without the prior written consent of Nintendo.

  $Revision: 1.16 $
 *---------------------------------------------------------------------------*/

#include <nnsys/g2d/g2d_CellTransferManager.h>
#include <nnsys/gfd/VramTransferMan/gfd_VramTransferManager.h>

//------------------------------------------------------------------------------



//------------------------------------------------------------------------------
static NNSG2dCellTransferState*                s_pCellStateArray = NULL;
static u32                                     s_numCellState = 0;

VramTransferTaskRegisterFuncPtr                s_pTaskRegisterFunc = NULL;


//------------------------------------------------------------------------------
// �}�l�[�W���̏�Ԃ͕s���ł͂Ȃ���
static NNS_G2D_INLINE BOOL IsCellTransferStateManagerValid_()
{
    return (BOOL)( ( s_pCellStateArray != NULL) && 
                   ( s_numCellState != 0 ) && 
                   ( s_pTaskRegisterFunc != NULL ) ) ;
}

//------------------------------------------------------------------------------
// �n���h���͗L����
static NNS_G2D_INLINE BOOL IsValidHandle_( u32 handle )
{
    NNS_G2D_ASSERT( IsCellTransferStateManagerValid_() );
    
    if( handle != NNS_G2D_INVALID_CELL_TRANSFER_STATE_HANDLE )
    {    
        if( handle < s_numCellState )
        {
            if( s_pCellStateArray[handle].bActive )
            {
                return TRUE; 
            }
        }
    }
    return FALSE;
}

//------------------------------------------------------------------------------
static NNS_G2D_INLINE NNSG2dCellTransferState*
GetValidCellTransferState_( u32 validHandle )
{
    NNS_G2D_ASSERT( IsValidHandle_( validHandle ) );
    return &s_pCellStateArray[validHandle];
}



//------------------------------------------------------------------------------
// Vram�]���^�X�N�Ƃ��ēo�^����K�v������̂�?
static NNS_G2D_INLINE BOOL ShouldRegisterAsVramTransferTask_
( 
    const NNSG2dCellTransferState*    pState,
    NNS_G2D_VRAM_TYPE                   type
)
{
    NNS_G2D_NULL_ASSERT( pState );
    
    return (BOOL)(  NNSi_G2dGetCellTransferStateRequestFlag( pState, type ) &&
                    NNSi_G2dGetCellTransferStateCellDrawnFlag( pState, type ) );
}

//------------------------------------------------------------------------------
// �`�摮�������Z�b�g���܂�
static NNS_G2D_INLINE void ResetCellTransferStateDrawnFlag_
( 
    NNSG2dCellTransferState*  pState 
)
{
    pState->bDrawn = 0x0;
}



//------------------------------------------------------------------------------
// �]�����f�[�^���擾����
static NNS_G2D_INLINE const void* GetVramTransferSrc_
( 
    const NNSG2dCellTransferState*    pState,
    NNS_G2D_VRAM_TYPE                   type
)
{
    NNS_G2D_NULL_ASSERT( pState );
    
    if( type == NNS_G2D_VRAM_TYPE_3DMAIN )
    {
        return pState->pSrcNCBR;
    }else{
        return pState->pSrcNCGR;
    }
}

//------------------------------------------------------------------------------
// �]�����f�[�^�͗L�����H
static NNS_G2D_INLINE BOOL IsVramTransferSrcDataValid_
( 
    const NNSG2dCellTransferState*    pState,
    NNS_G2D_VRAM_TYPE                   type
)
{
    return (BOOL)( GetVramTransferSrc_( pState, type ) != NULL );
}



//------------------------------------------------------------------------------
// �]���̏������ƂƂ̂��Ă��邩 
static NNS_G2D_INLINE BOOL IsCellTransferStateValid_
( 
    const NNSG2dCellTransferState*  pState, 
    NNS_G2D_VRAM_TYPE               type
)
{
    // �]�����̃f�[�^���ݒ肳��Ă���
    // �]�����̗̈悪�ݒ肳��Ă���
    return (BOOL)( NNSi_G2dIsVramLocationReadyToUse( &pState->dstVramLocation, type ) && 
                   IsVramTransferSrcDataValid_( pState, type ) );
}

//------------------------------------------------------------------------------
// �񋓎q�̕ϊ�
// ���݂̂Ƃ���͂����̃L���X�g
static NNS_G2D_INLINE NNS_GFD_DST_TYPE 
ConvertVramType_( NNS_G2D_VRAM_TYPE type )
{
    NNS_G2D_MINMAX_ASSERT( type, NNS_G2D_VRAM_TYPE_3DMAIN, NNS_G2D_VRAM_TYPE_2DSUB );
    {
        const static NNS_GFD_DST_TYPE cvtTbl []=
        {
            NNS_GFD_DST_3D_TEX_VRAM,      // NNS_G2D_VRAM_TYPE_3DMAIN
            NNS_GFD_DST_2D_OBJ_CHAR_MAIN, // NNS_G2D_VRAM_TYPE_2DMAIN
            NNS_GFD_DST_2D_OBJ_CHAR_SUB   // NNS_G2D_VRAM_TYPE_2DSUB
        };
        return cvtTbl[type];
    }
}

//------------------------------------------------------------------------------
// 
// �^�X�N�̐���
// NNSG2dCellTransferState ����͂Ƃ��� �}�l�[�W���ɓo�^����^�X�N�𐶐����܂�
//
static NNS_G2D_INLINE BOOL MakeVramTransferTask_
(
    const NNSG2dCellTransferState*    pState, 
    NNS_G2D_VRAM_TYPE                   type
)
{    
    NNS_G2D_NULL_ASSERT( pState );
    
    NNS_G2D_ASSERT( IsCellTransferStateValid_( pState, type ) );
    
    //
    // Vram�]���^�X�N�̓o�^���O�����W���[���ɈϏ����܂�
    //
    return (*s_pTaskRegisterFunc )( ConvertVramType_( type ),
                                    NNSi_G2dGetVramLocation( &pState->dstVramLocation, type ),
                                    (u8*)GetVramTransferSrc_( pState, type ) + pState->srcOffset,
                                    pState->szByte );
}
//------------------------------------------------------------------------------
// NNSG2dCellTransferState�̃��Z�b�g
static NNS_G2D_INLINE void ResetCellTransferState_( NNSG2dCellTransferState* pState )
{
    NNS_G2D_NULL_ASSERT( pState );
    {
        NNSi_G2dInitializeVRamLocation( &pState->dstVramLocation );
        pState->szDst = 0;
        pState->pSrcNCGR = NULL;
        pState->pSrcNCBR = NULL;
        pState->szSrcData = 0;
        pState->bActive = FALSE;
        
        
        pState->bDrawn              = 0x0;
        pState->bTransferRequested  = 0x0;
        pState->srcOffset           = 0x0;
        pState->szByte              = 0x0;
    }
}
//------------------------------------------------------------------------------
// ���C�u�����������J
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
NNSG2dCellTransferState* 
NNSi_G2dGetCellTransferState
( 
    u32 handle 
)
{
    return GetValidCellTransferState_( handle );
}

//------------------------------------------------------------------------------
// �w��n���h���̓]����ԊǗ��I�u�W�F�N�g�ɓ]��������ݒ肵�܂��B
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
)
{
    NNS_G2D_ASSERT( IsValidHandle_( handle ) );
    NNS_G2D_NON_ZERO_ASSERT( szDst );
    NNS_G2D_NON_ZERO_ASSERT( szSrcData );
    NNS_G2D_NULL_ASSERT( szDst );
    
    {
        NNSG2dCellTransferState* pState 
            = GetValidCellTransferState_( handle );
        
        NNS_G2D_NULL_ASSERT( pState );
        
        // �ǂꂩ���L���ł��邱��
        NNS_G2D_ASSERT( dstAddr3D       != NNS_G2D_VRAM_ADDR_NONE   ||
                        dstAddr2DMain   != NNS_G2D_VRAM_ADDR_NONE   || 
                        dstAddr2DSub    != NNS_G2D_VRAM_ADDR_NONE    );
        {
            NNSG2dVRamLocation* pImg = &pState->dstVramLocation;
            
            NNSi_G2dInitializeVRamLocation( pImg );
            
            if( dstAddr3D != NNS_G2D_VRAM_ADDR_NONE )
            {
                NNSi_G2dSetVramLocation( pImg, NNS_G2D_VRAM_TYPE_3DMAIN, dstAddr3D );
            }
            
            if( dstAddr2DMain != NNS_G2D_VRAM_ADDR_NONE )
            {
                NNSi_G2dSetVramLocation( pImg, NNS_G2D_VRAM_TYPE_2DMAIN, dstAddr2DMain );
            }
            
            if( dstAddr2DSub != NNS_G2D_VRAM_ADDR_NONE )
            {
                NNSi_G2dSetVramLocation( pImg, NNS_G2D_VRAM_TYPE_2DSUB , dstAddr2DSub );
            }
        }
        
        // ����NULL�͖��炩�ɗL���ł͂Ȃ�
        NNS_G2D_ASSERT( pSrcNCGR != NULL || pSrcNCBR != NULL );
        
        pState->szDst     = szDst;
        pState->pSrcNCGR  = pSrcNCGR;
        pState->pSrcNCBR  = pSrcNCBR;
        pState->szSrcData = szSrcData;
    }
}




//------------------------------------------------------------------------------
// �������֘A
//------------------------------------------------------------------------------
/*---------------------------------------------------------------------------*
  Name:         NNS_G2dInitCellTransferStateManager

  Description:  �Z��VRAM�]����ԃI�u�W�F�N�g�}�l�[�W�������������܂��B
                �Z��VRAM�]����ԃI�u�W�F�N�g�o�b�t�@�������Ƃ��ēn���܂��B
                pTaskRegisterFunc �� VRAM�]���^�X�N�o�^�𐿋�����֐��ւ̃|�C���^�ł��B
                
                
  Arguments:    pCellStateArray          [OUT] �Z��VRAM�]����ԃI�u�W�F�N�g�o�b�t�@ 
                numCellState             [IN]  �Z��VRAM�]����ԃI�u�W�F�N�g�o�b�t�@�� 
                pTaskRegisterFunc        [IN]  VRAM�]���o�^�֐��̃|�C���^ 
                    
  Returns:      �Ȃ�
  
 *---------------------------------------------------------------------------*/
void 
NNS_G2dInitCellTransferStateManager
( 
    NNSG2dCellTransferState*            pCellStateArray, 
    u32                                 numCellState,
    VramTransferTaskRegisterFuncPtr     pTaskRegisterFunc
)
{
    NNS_G2D_NULL_ASSERT( pCellStateArray );
    NNS_G2D_NON_ZERO_ASSERT( numCellState );
    NNS_G2D_NULL_ASSERT( pTaskRegisterFunc );
    
    
    s_pTaskRegisterFunc     = pTaskRegisterFunc;
    
    s_pCellStateArray       = pCellStateArray;
    s_numCellState          = numCellState;
    
    //
    // ���ׂĂ� NNSG2dCellTransferState �����Z�b�g����
    //
    {
        u32     i;
        for( i = 0; i < numCellState; i++ )
        {
            ResetCellTransferState_( &pCellStateArray[i] );
        }
    }
}


//------------------------------------------------------------------------------
// �]���֘A
//------------------------------------------------------------------------------

/*---------------------------------------------------------------------------*
  Name:         NNS_G2dUpdateCellTransferStateManager

  Description:  �]����ԊǗ��}�l�[�W�����X�V���܂��B
                
                ���t���[���A�j���X�V�A�`�搿�����I��������AVRAM�]���̑O�ɌĂ�ł��������B
                (�Ăяo���ʒu���d�v�ł��B)
                
                �����œo�^����Ă��邷�ׂĂ̓]����ԃI�u�W�F�N�g�ɂ��āA
                VRAM�]���̐����̕K�v�����邩�𔻒肵�A�K�v�Ȃ�΃^�X�N�̐������s���Ă��܂��B
                
                
                
  Arguments:    �Ȃ�
                    
  Returns:      �Ȃ�
  
 *---------------------------------------------------------------------------*/
void NNS_G2dUpdateCellTransferStateManager()
{
    u32 i;
    // �g�p����Ă���A���ׂĂ�Vram�]����ԃI�u�W�F�N�g�ɂ���...
    for( i = 0; i < s_numCellState; i++ )
    {
        NNS_G2D_VRAM_TYPE           type;
        NNSG2dCellTransferState*  pState                = &s_pCellStateArray[i];
        
        // �A�N�e�B�u���H
        if( pState->bActive )
        {
            //
            // ���ׂĂ̎�ނ̕`��HW�ɂ���...
            //
            for( type = NNS_G2D_VRAM_TYPE_3DMAIN; type < NNS_G2D_VRAM_TYPE_MAX; type++ )
            {
                //
                // �]������K�v������Ȃ�...
                //
                
                if( ShouldRegisterAsVramTransferTask_( pState, type ) )
                {
                    
                    // �}�l�[�W������擾                   
                    // ����                    
                    if( MakeVramTransferTask_( pState, type ) )
                    {
                        // OS_Printf( "VRAM transfer task is registered ! surface_Id = %d \n", type );
                        // �o�^������Ԃ����Z�b�g(�]�����s��ꂽVRAM��ނ���)
                        NNSi_G2dSetCellTransferStateRequestFlag( pState, type, FALSE );                    
                    }else{
                        // �o�^�Ɏ��s����
                        // TODO �F�x���H: �A�T�[�g
                    }
                }
            }
            
            //
            // �`���ԂɃ��Z�b�g(���ׂĂ�VRAM���)
            //
            ResetCellTransferStateDrawnFlag_( pState );
        }
    }
}





//------------------------------------------------------------------------------
// �o�^�֘A
//------------------------------------------------------------------------------
/*---------------------------------------------------------------------------*
  Name:         NNS_G2dSetCellTransferStateRequested

  Description:  �w��n���h���̓]����ԊǗ��I�u�W�F�N�g�ɓ]��������ݒ肵�܂��B
                
                
  Arguments:    handle:         [IN]  �]����ԊǗ��I�u�W�F�N�g�̃n���h��
             srcOffset:         [IN]  �]�����f�[�^�擪����� �I�t�Z�b�g�l
                szByte:         [IN]  �]���T�C�Y
                    
  Returns:      �Ȃ�
  
 *---------------------------------------------------------------------------*/
void NNS_G2dSetCellTransferStateRequested
( 
    u32                                 handle,
    u32                                 srcOffset,
    u32                                 szByte
)
{
    NNS_G2D_ASSERT( IsValidHandle_( handle ) );
    
    {
        NNSG2dCellTransferState* pState 
            = NNSi_G2dGetCellTransferState( handle );
        
        NNS_G2D_NULL_ASSERT( pState );
        NNS_G2D_ASSERT( szByte <= pState->szDst );
        //
        // �]��������Ԃɐ���
        //
        pState->bTransferRequested    = 0xFFFFFFFF;// memo:ON�ɂ���Ƃ��͈�Ă�ON�������肦�Ȃ�
        pState->srcOffset             = srcOffset;
        pState->szByte                = szByte;       
    }
}

/*---------------------------------------------------------------------------*
  Name:         NNS_G2dGetNewCellTransferStateHandle

  Description:  �Z��VRAM�]����ԃI�u�W�F�N�g�̃n���h�����擾���܂��B
                �Z��VRAM�]����ԃI�u�W�F�N�g�ւ̑���͂��̃n���h�����g�p���čs���܂��B
                �n���h���̓Z���A�j���[�V�������̂̃����o�Ƃ��ĕۑ�����܂��B
                �n���h���̎擾�Ɏ��s�����ꍇ�� NNS_G2D_INVALID_CELL_TRANSFER_STATE_HANDLE ��Ԃ��܂��B
                �����ŃZ��VRAM�]����ԃI�u�W�F�N�g�o�b�t�@����`�T�����Ă��܂��B
                �i�p�t�H�[�}���X�N���e�B�J���ȏꏊ�ł̌Ăт����͔����Ă��������B�j

                
                
  Arguments:    �Ȃ�
                    
  Returns:      �Z��VRAM�]����ԃI�u�W�F�N�g�̃n���h��
                �n���h���̎擾�Ɏ��s�����ꍇ�� NNS_G2D_INVALID_CELL_TRANSFER_STATE_HANDLE ��Ԃ��܂��B
  
 *---------------------------------------------------------------------------*/
u32 
NNS_G2dGetNewCellTransferStateHandle()
{
    NNS_G2D_ASSERT( IsCellTransferStateManagerValid_() );
    
    
    //
    // �g�p����Ă��Ȃ��]����ԃI�u�W�F�N�g��z��̐擪���猟�����܂�
    // 
    {
        u32 i = 0;
        for( i = 0;i < s_numCellState; i++ )
        {
            if( s_pCellStateArray[ i ].bActive != TRUE )
            {
                s_pCellStateArray[ i ].bActive = TRUE;
                return i;
            }
        }
    }    
    
    // �����ł��Ȃ�����
    return NNS_G2D_INVALID_CELL_TRANSFER_STATE_HANDLE;
}

/*---------------------------------------------------------------------------*
  Name:         NNS_G2dFreeCellTransferStateHandle

  Description:  �Z��VRAM�]����ԃI�u�W�F�N�g�̃n���h����ԋp���܂��B
                �g�p���Ȃ��Ȃ����n���h���ɑ΂��Ď��s���Ă��������B


                
                
  Arguments:    handle              [IN] �Z��VRAM�]����ԃI�u�W�F�N�g�̃n���h��
                    
  Returns:      �Ȃ�
  
 *---------------------------------------------------------------------------*/
void
NNS_G2dFreeCellTransferStateHandle( u32 handle )
{
    NNS_G2D_ASSERT( IsValidHandle_( handle ) );
    NNS_G2D_ASSERT( IsCellTransferStateManagerValid_() );
    
    ResetCellTransferState_( GetValidCellTransferState_( handle ) );    
}

   
    
