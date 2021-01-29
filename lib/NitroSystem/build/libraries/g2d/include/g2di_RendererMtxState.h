/*---------------------------------------------------------------------------*
  Project:  NITRO-System - libraries - g2d
  File:     g2di_RendererMtxState.h

  Copyright 2004-2007 Nintendo.  All rights reserved.

  These coded instructions, statements, and computer programs contain
  proprietary information of Nintendo of America Inc. and/or Nintendo
  Company Ltd., and are protected by Federal copyright law.  They may
  not be disclosed to third parties or copied or duplicated in any form,
  in whole or in part, without the prior written consent of Nintendo.

  $Revision: 1.10 $
 *---------------------------------------------------------------------------*/
 
#ifndef NNS_G2DI_RENDERERMTXSTATE_H_
#define NNS_G2DI_RENDERERMTXSTATE_H_

#include <nnsys/g2d/g2d_Renderer.h>

#include "g2d_Internal.h"
#include "g2di_RendererMtxStack.hpp"// For MatrixStack 
#include "g2di_RendererMtxCache.h"

//
// Matrix Cache state Manager
//
// �s��X�^�b�N�̏�Ԃ��Ǘ����A
// �s��X�^�b�N�̃}�g���N�X�L���b�V���ւ̃��[�h����������������W���[���B
//
// �s��X�^�b�N�ւ̑�������b�v���āA�s��X�^�b�N�̏�ԊǗ����s���܂��B
// 
// �����_�����W���[���ɂ���Ďg�p����܂��B
// �����_�����W���[���͒��ڍs��L���b�V���𑀍삹���ɁA
// ���ׂĂ̑����{���W���[���̃��\�b�h���o�R���čs���܂��B
//
// �s��X�^�b�N�ւ̑����񋓂��܂��B
//      A�F�J�����g�s���SR�l���ύX���ꂽ
//      B�F�J�����g�s���SR�l���s��L���b�V���ɓǂݍ��܂ꂽ
//      C�F�s��X�^�b�N��Push���ꂽ�B
//
// �Ή�����֐��Ăяo���́A
//
//      A�FNNSi_G2dMCMSetCurrentMtxSRChanged()
//      B�FNNSi_G2dMCMStoreCurrentMtxToMtxCache( )
//      C�FNNSi_G2dMCMSetMtxStackPushed( u16 newPos, u16 lastPos )
//
// �ƂȂ�܂��B
//
// �ΏƓI�Ƀ����_���R�A���W���[���͖{���W���[������؎Q�Ƃ��܂���B
// �����_�����R�A�W���[���͍s��L���b�V�����W���[���݂̂𗘗p���܂��B
//
// �֐����� NNSi_G2dMCM.....




//------------------------------------------------------------------------------
// ���W���[�������藘�p �^��`
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// �s��X�^�b�N���̍s��̏�Ԃ�����킷�B
typedef enum MCMRndMtxStateType
{
    MCM_MTX_NOT_SR = 0,                        // SR �ϊ�����Ă��Ȃ�
    MCM_MTX_SR_NOT_CACHELOADED,                // SR �ϊ�����Ă���A�}�g���N�X�L���b�V���ւ̃��[�h�����������Ă��Ȃ�
    MCM_MTX_SR_NOT_CACHELOADED_STACKCHANGED,   // SR �ϊ�����Ă���A�}�g���N�X�L���b�V���ւ̃��[�h�����������Ă��Ȃ�
                                               // �X�^�b�N�����삳��Ă���
    MCM_MTX_SR_CACHELOADED                     // SR �ϊ�����Ă���A�}�g���N�X�L���b�V���ւ̃��[�h���������Ă���

}MCMRndMtxStateType;

//------------------------------------------------------------------------------
// �s��X�^�b�N�̏�Ԃ�����킵�܂��B
// �}�g���N�X�X�^�b�N���̍s��Ɠ����T�C�Y�̃X�^�b�N�\�����Ƃ�܂��B
//
// �{�f�[�^�𗘗p���āA�}�g���N�X�L���b�V���ւ̃��[�h�����𐧌䂵�Ă��܂��B
//
// �s�񂻂̂��̈ȊO�̏����i�[���Ă��܂��B
typedef struct MCMMtxState
{
    u16                      mtxCacheIdx; // �s��L���b�V���ԍ�
    u16                      groupID;     // ����̃}�g���N�X�L���b�V�����Q�Ƃ���O���[�vID
    u16                      stateType;   // MCMRndMtxStateType
    u16                      pad16;
    
}MCMMtxState;

//------------------------------------------------------------------------------
NNS_G2D_INLINE u16 GetMtxStateMtxCacheIdx_( const MCMMtxState* pMtxState )
{
    return pMtxState->mtxCacheIdx;
}

NNS_G2D_INLINE void SetMtxStateMtxCacheIdx_( MCMMtxState* pMtxState, u16 cacheIdx )
{
    pMtxState->mtxCacheIdx = cacheIdx;
}

//------------------------------------------------------------------------------
NNS_G2D_INLINE u16 GetMtxStateGroupID_( const MCMMtxState* pMtxState )
{
    return pMtxState->groupID;
}

NNS_G2D_INLINE void SetMtxStateGroupID_( MCMMtxState* pMtxState, u16 groupID )
{
    pMtxState->groupID = groupID;
}



//------------------------------------------------------------------------------
// �s��X�^�b�N�̏�Ԃ�����킷�f�[�^
// �s��X�^�b�N�Ɠ����T�C�Y�̃X�^�b�N�ł��B
// �J�����g�ʒu���A�s��X�^�b�N�Ɠ������܂��B
static MCMMtxState           mtxStateStack_[G2Di_NUM_MTX_CACHE];
static u16                   groupID_ = 0;



//------------------------------------------------------------------------------
// ����������J�̊֐�
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
// �J�����g�s��̏�ԍ\���̂��擾���܂�
NNS_G2D_INLINE MCMMtxState* GetCuurentMtxState_()
{
    return &mtxStateStack_[NNSi_G2dGetMtxStackPos()];
}

//------------------------------------------------------------------------------
// �X�^�b�N�̐e�K�w�����ǂ�
// �e�K�w�� groupID ������Ȃ�΁A
// ������L���V�����[�h�ς݂ɃZ�b�g����
// (groupID ������Ȃ�΁A����̍s��L���b�V�����Q�Ƃ��܂��B)
NNS_G2D_INLINE void SetParentMtxStateLoaded_( u16 mtxCacheIdx, u16 groupID )
{
    int i;
    const u16 currntStackPos    = NNSi_G2dGetMtxStackPos();
    
    for( i = currntStackPos; i >= 0; i-- )
    {
        
        if( GetMtxStateGroupID_( &mtxStateStack_[i] ) != groupID )
        {
            break;
        }else{
            // �L���V�����[�h�ς� �� �Z�b�g
            mtxStateStack_[i].stateType   = MCM_MTX_SR_CACHELOADED;
            SetMtxStateMtxCacheIdx_( &mtxStateStack_[i], mtxCacheIdx );
        }
        
        NNSI_G2D_DEBUGMSG1( "Set Loaded => mtxStateStack_[%d].mtxCacheIdx = %d\n", 
                             i, 
                             GetMtxStateMtxCacheIdx_( &mtxStateStack_[i] ) );
    }
}

//------------------------------------------------------------------------------
// �V�����A�O���[�vID���擾���܂�
NNS_G2D_INLINE u16 GetNewGroupID_()
{
    return groupID_++;
}

//------------------------------------------------------------------------------
// �V�����A�O���[�vID���擾���܂�
NNS_G2D_INLINE void InitGroupID_()
{
    groupID_ = 0;
}

//------------------------------------------------------------------------------
// �O�����J�֐�
//------------------------------------------------------------------------------
NNS_G2D_INLINE void NNSi_G2dMCMInitMtxCache()
{
    NNSi_G2dInitRndMtxStack();
    NNSi_RMCInitMtxCache();
    
    InitGroupID_();
    
    //
    // �}�g���N�X�X�e�[�g�̏�����
    //
    MI_CpuClearFast( mtxStateStack_, sizeof( mtxStateStack_ ) );
}

//------------------------------------------------------------------------------
// �J�����g NNSG2dRndCore2DMtxCache �ւ̃|�C���^���擾���܂�
//
NNS_G2D_INLINE NNSG2dRndCore2DMtxCache*   NNSi_G2dMCMGetCurrentMtxCache()
{
    return NNSi_RMCGetMtxCacheByIdx( GetMtxStateMtxCacheIdx_( GetCuurentMtxState_() ) );
}

//------------------------------------------------------------------------------
// MtxChache �� ���e���������܂�
//
// 2D Graphics Engine �����^�����O��p�̏����ł��B
//
NNS_G2D_INLINE void NNSi_G2dMCMCleanupMtxCache()
{
    //
    // �}�g���N�X�X�^�b�N�̃��Z�b�g
    //
    NNSi_G2dInitRndMtxStack();
    //
    // �}�g���N�X�L���V���̃��Z�b�g
    //
    NNSi_RMCResetMtxCache();

    InitGroupID_();
    
    //
    // �}�g���N�X�X�e�[�g�̏�����
    //
    MI_CpuClearFast( mtxStateStack_, sizeof( mtxStateStack_ ) );
}

//------------------------------------------------------------------------------
// ���݂̍s�񂪃L���b�V���ɓǂݍ��ޕK�v�������Ԃ��擾���܂��B
NNS_G2D_INLINE BOOL NNSi_G2dMCMShouldCurrentMtxBeLoadedToMtxCache( )
{
    MCMMtxState*     pCurrMtxState = GetCuurentMtxState_();
    //
    // SR �ϊ�����Ă���
    // �L���b�V���ɓǂݍ��܂�Ă��Ȃ��Ȃ��...
    //       
    return (BOOL)( pCurrMtxState->stateType == MCM_MTX_SR_NOT_CACHELOADED ||
                   pCurrMtxState->stateType == MCM_MTX_SR_NOT_CACHELOADED_STACKCHANGED );
}

//------------------------------------------------------------------------------
// �J�����g�s��̏�Ԃ�SR�ύX���s��ꂽ��Ԃɐݒ肵�܂��B
NNS_G2D_INLINE void NNSi_G2dMCMSetCurrentMtxSRChanged()
{
    MCMMtxState*     pCurrMtxState = GetCuurentMtxState_();
    
    //
    // �J�����g�s��̏�Ԃɂ���āA�������ω����܂��B
    //
    switch( pCurrMtxState->stateType )
    {
    case MCM_MTX_SR_NOT_CACHELOADED:
        //
        // ���[�h�O�̏�ԂȂ�΁A�J�����g�s���Ԃ̍X�V�������͕K�v���Ȃ��B
        // (�܂�A�i�`�撼�O�̃^�C�~���O�Ŏ��s�����j���[�h�����O�ł���΁A
        // ���xSR�ϊ����s���Ă��}�g���N�X�L���b�V���͏���Ȃ��B)
        //
        return;
    case MCM_MTX_NOT_SR:
    case MCM_MTX_SR_NOT_CACHELOADED_STACKCHANGED:
    case MCM_MTX_SR_CACHELOADED:
        // �s��X�^�b�N�F�J�����g�s��̏�Ԃ��X�V����
        {
            // �V���� �s��L���b�V�� ���g�p����K�v������
            // �ʃO���[�v�Ƃ��ăO���[�vID��ݒ肵�܂��B
            SetMtxStateGroupID_( pCurrMtxState, GetNewGroupID_() );
            
            //
            // SR���ύX����Ă��邪�A�L���b�V���ւ̃��[�h������ł��Ȃ����
            //
            pCurrMtxState->stateType = MCM_MTX_SR_NOT_CACHELOADED;
            
            NNSI_G2D_DEBUGMSG1( "currentMtxCachePos change to %d at %d\n", 
                                 pCurrMtxState->mtxCacheIdx,
                                 NNSi_G2dGetMtxStackPos() );
        }
        break;
    }
}


//------------------------------------------------------------------------------
// �J�����g�s��Push����ɑΉ����鏈�����s���܂��B
NNS_G2D_INLINE void NNSi_G2dMCMSetMtxStackPushed( u16 newPos, u16 lastPos )
{
    
    mtxStateStack_[newPos] = mtxStateStack_[lastPos];
    //
    // stateType �� MCM_MTX_SR_NOT_CACHELOADED �������ꍇ��
    // MCM_MTX_SR_NOT_CACHELOADED_STACKCHANGED �ւƏ�Ԃ�ύX���܂��B
    // 
    // �i ���̌��ʁA�V����SR�ϊ����N��NNSi_G2dMCMSetCurrentMtxSRChanged()
    //    �����s�����ۂ̐U�镑�����ω����܂��B)
    //    
    if( mtxStateStack_[lastPos].stateType == MCM_MTX_SR_NOT_CACHELOADED )
    {
        mtxStateStack_[newPos].stateType = MCM_MTX_SR_NOT_CACHELOADED_STACKCHANGED;
    }else{
        mtxStateStack_[newPos].stateType   = mtxStateStack_[lastPos].stateType;
    }
}

//------------------------------------------------------------------------------
// �J�����g�s����w�肵�� MtxChache�Ɋi�[���܂��B
// 
// flip �@�\ �� �g�p���� OBJ �� affine�ϊ�����K�v������ꍇ�� 
// ��p�� �s��� �������A�i�[���܂��B
//
// 2D Graphics Engine �����^�����O��p�̏����ł��B
//
static void NNSi_G2dMCMStoreCurrentMtxToMtxCache( )
{   
    // �K�v��������΁A�������Ȃ�  
    if( NNSi_G2dMCMShouldCurrentMtxBeLoadedToMtxCache() )
    {
        MCMMtxState* pCurrentState = GetCuurentMtxState_();            
        const u16 mtxCacheIdx = NNSi_RMCUseNewMtxCache();
        const u16 groupID     = GetMtxStateGroupID_( pCurrentState );
           
        NNS_G2D_MINMAX_ASSERT( mtxCacheIdx, 0, G2Di_NUM_MTX_CACHE - 1 );
           
        //
        // �L���V���C���f�b�N�X�̌���
        //
        SetMtxStateMtxCacheIdx_( pCurrentState, mtxCacheIdx );
           
        ////
        //// �K�v������΁c
        ////
        //if( mtxCacheIdx != NNS_G2D_OAM_AFFINE_IDX_NONE )
        {
           //
           // �L���b�V��������������
           //
           NNS_G2dInitRndCore2DMtxCache( NNSi_RMCGetMtxCacheByIdx( mtxCacheIdx ) );
                  
           //
           // �L���b�V���ɍs����R�s�[����
           //    
           NNSi_G2dGetMtxRS( NNSi_G2dGetCurrentMtxFor2DHW(), 
                                &NNSi_G2dMCMGetCurrentMtxCache()->m22 );
               
           NNSI_G2D_DEBUGMSG1( "Mtx No %d is cache loaded To %d = %d\n", 
                                        mtxStateStackPos_, 
                                        pCurrMtxState->mtxCacheIdx );
        }
        //
        // SR�ϊ������L���� �e�K�w�̏�Ԃ����[�h�ς݂ɍX�V����
        //
        SetParentMtxStateLoaded_( mtxCacheIdx, groupID );
    }
}


#endif // NNS_G2DI_RENDERERMTXSTATE_H_