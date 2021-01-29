/*---------------------------------------------------------------------------*
  Project:  NITRO-System - demos - g2d - samples - MultiCell_UILayout
  File:     main.c

  Copyright 2004-2007 Nintendo.  All rights reserved.

  These coded instructions, statements, and computer programs contain
  proprietary information of Nintendo of America Inc. and/or Nintendo
  Company Ltd., and are protected by Federal copyright law.  They may
  not be disclosed to third parties or copied or duplicated in any form,
  in whole or in part, without the prior written consent of Nintendo.

  $Revision: 1.3 $
 *---------------------------------------------------------------------------*/

// ============================================================================
//  �f���̓��e
//      �}���`�Z�������p���āA�Q�[���A�v���P�[�V�����ɂ�����UI(���[�U�C���^�t�F�[�X)
//      ����������T���v���ł��B
//
//  ������@
//      �\���L�[    �J�[�\���̈ړ�
//      
// ============================================================================

#include <nitro.h>
#include <nnsys/g2d.h>

// �o�C�i���R���o�[�^(-lbl�I�v�V����)�ɂ���ďo�͂��ꂽ�A
// �A�j���[�V�����V�[�P���X�ԍ��̕ʖ����`�����w�b�_�t�@�C��
#include "../data/win_pnl_NANR_LBLDEFS.h"
#include "g2d_demolib.h"


#define NUM_OF_OAM              128                 // OAM�}�l�[�W���Ǘ��Ώۂ�OAM��
#define NUM_OF_AFFINE           (NUM_OF_OAM/4)      // OAM�}�l�[�W���Ǘ��Ώۂ�Affine�p�����[�^��

// �}���`�Z���̈ʒu
#define MY_MC_BASE_POS_X        128
#define MY_MC_BASE_POS_Y        96
// MultiCell �\���ʒu
static const NNSG2dFVec2           MULTICELL_POSITION 
            = {MY_MC_BASE_POS_X << FX32_SHIFT, MY_MC_BASE_POS_Y << FX32_SHIFT};
            
            
// ���[�U����`�����A�Z���A�j�����̃A�j���t���[���ԍ��̈Ӗ�
#define APP_UI_ANIMFRAME_IDX_NORMAL 0   // �ʏ�̏��
#define APP_UI_ANIMFRAME_IDX_PUSHED 1   // �{�^�����������܂ꂽ���


//
// MultiCellAnimation ����
// �قȂ�2�̕����Ŏ��̂����������܂��B
// 
static NNSG2dMultiCellAnimation   mcAnim;


// OAM�A�g���r���[�g�ꎞ�o�b�t�@
static GXOamAttr                tempOamBuffer[NUM_OF_OAM];

//
// �}���`�Z���o���N
//
static NNSG2dMultiCellDataBank*        pMCBank = NULL;     // �}���`�Z���f�[�^
static NNSG2dCellDataBank*             pCellBank = NULL;   // �Z���f�[�^
static NNSG2dCellAnimBankData*         pAnimBank = NULL;   // �Z���A�j���[�V����
static NNSG2dMultiCellAnimBankData*    pMCABank = NULL;    // �}���`�Z���A�j���[�V����

// �|�C���^�̈ʒu
static NNSG2dSVec2          pointerPos_ = { 0, 0 };
static BOOL                 bHit_       = FALSE;


//------------------------------------------------------------------------------
// �v���g�^�C�v�錾

static void InitOamManager(NNSG2dOamManagerInstance* pOamManager);
static void LoadResources();
static void ProcessInput(fx32* pAnimSpeed, NNSG2dAnimController* pAnimCtrl);
void VBlankIntr(void);



/*---------------------------------------------------------------------------*
  Name:         InitOamManager

  Description:  OAM�}�l�[�W���V�X�e����OAM�}�l�[�W���̏��������s���܂��B

  Arguments:    pOamManager:    ����������OAM�}�l�[�W���ւ̃|�C���^�B

  Returns:      �Ȃ��B
 *---------------------------------------------------------------------------*/
static void InitOamManager(NNSG2dOamManagerInstance* pOamManager)
{
    BOOL    result;

    // Oam Manager �V�X�e���̏�����
    NNS_G2dInitOamManagerModule();

    result = NNS_G2dGetNewManagerInstance(
                pOamManager,
                0,
                NUM_OF_OAM - 1,
                NNS_G2D_OAMTYPE_MAIN );
    SDK_ASSERT( result );

    result = NNS_G2dInitManagerInstanceAffine( pOamManager, 0, NUM_OF_AFFINE - 1 );
    SDK_ASSERT( result );
}



/*---------------------------------------------------------------------------*
  Name:         InitMultiCellInstance_

  Description:  �}���`�Z�����̂����������܂��B
  
  Returns:      �Ȃ��B
 *---------------------------------------------------------------------------*/
static void InitMultiCellInstance_
(
    NNSG2dMultiCellAnimation*       pMCAnim,
    NNSG2dCellAnimBankData*         pAnimBank,
    NNSG2dCellDataBank*             pCellBank,
    NNSG2dMultiCellDataBank*        pMCBank,
    NNSG2dMultiCellAnimBankData*    pMCABank,
    NNSG2dMCType                    mcType
)
{
    // �������ɕK�v�ȃ��[�N�������̃T�C�Y���擾���܂�
    const u32 szWork = NNS_G2dGetMCWorkAreaSize( pMCBank, mcType );
    void* pWorkMem   = G2DDemo_Alloc( szWork );
    
    // ������
    NNS_G2dInitMCAnimationInstance( pMCAnim, 
                                    pWorkMem, 
                                    pAnimBank, 
                                    pCellBank, 
                                    pMCBank, 
                                    mcType );
    
    SDK_NULL_ASSERT( pMCAnim );
    
    // �}���`�Z���A�j���[�V�����ɍĐ�����V�[�P���X���Z�b�g
    {
        const NNSG2dMultiCellAnimSequence* pSequence = NULL;    
        // �Đ�����V�[�P���X���擾
        pSequence = NNS_G2dGetAnimSequenceByIdx( pMCABank, 0 );
        SDK_ASSERT( pSequence );

        NNS_G2dSetAnimSequenceToMCAnimation( pMCAnim, pSequence);
    }
}

/*---------------------------------------------------------------------------*
  Name:         IsRectIntersect_

  Description:  ��`���_�ƏՓ˂��Ă��邩�ǂ������肵�܂��B

  Arguments:    pRect:          ����ΏۂƂȂ��`
                
                pPointerPos�F   ����ΏۂƂȂ�_
                
                pRectPos�F      ��`�̈ʒu

  Returns:      �Փ˂��Ă��邩�H�Փ˂��Ă���ꍇTRUE���Ԃ�܂��B
 *---------------------------------------------------------------------------*/
static BOOL IsRectIntersect_
( 
    const NNSG2dCellBoundingRectS16*    pRect,
    const NNSG2dSVec2*                  pPointerPos, 
    const NNSG2dSVec2*                  pRectPos 
)
{
    NNSG2dSVec2 posPtr = (*pPointerPos);
    
    posPtr.x -= pRectPos->x; 
    posPtr.y -= pRectPos->y; 
    
    if( pRect->maxX >= posPtr.x && 
        pRect->minX <= posPtr.x )
    {
        if( pRect->maxY >= posPtr.y && 
            pRect->minY <= posPtr.y )
        {
            // �Փ˂��Ă���
            return TRUE;        
        }
    }
    // �Փ˂��Ă��Ȃ�
    return FALSE;
}

/*---------------------------------------------------------------------------*
  Name:         OnButtonHitCallBack_

  Description:  UI�{�^���̏Փ˔��菈�����s���R�[���o�b�N�֐��ł��B
                NNS_G2dTraverseMCNodes()�ւ̈����Ƃ��Ďg�p����܂��B
                
                �{�֐��̓}���`�Z�����̂��ׂẴm�[�h�ɑ΂��ČĂяo�����
                �R�[���o�b�N�֐��Ƃ��Ďg�p����܂��B

  Arguments:    userParamater:   ���[�U�p�����[�^�A
                                 �{�T���v���ł͏Փ˂��N���������L������
                                 �t���O�ϐ��ւ̃|�C���^�l���i�[���Ă��܂��B
                
                pNodeData�F      �m�[�h���B�Z���A�j���[�V�������̃Z���A�j����
                                 �ʒu���Ȃǂ��i�[����Ă��܂��B
                
                pCellAnim�F      �Z���A�j���[�V����(�Z���A�j���[�V��������)
                
                nodeIdx�F        �m�[�h�ԍ�

  Returns:      �ȍ~�̃R�[���o�b�N�Ăяo���𒆎~���邩�ǂ����H
                �ȍ~�̃R�[���o�b�N�Ăяo���𒆎~�������ꍇ�́AFALSE��Ԃ��܂��B
 *---------------------------------------------------------------------------*/
static BOOL OnButtonHitCallBack_
( 
    u32                                 userParamater,
    const NNSG2dMultiCellHierarchyData* pNodeData,
    NNSG2dCellAnimation*                pCellAnim, 
    u16                                 nodeIdx 
)
{
#pragma unused( nodeIdx )

    NNSG2dSVec2       rectPos;
    
    const NNSG2dCellData*       pCD      = NNS_G2dGetCellAnimationCurrentCell( pCellAnim );
    const NNSG2dAnimController* pAnmCtrl = NNS_G2dGetCellAnimationAnimCtrl( pCellAnim );
    const NNSG2dAnimSequence*   pAnmSeq  = NNS_G2dGetAnimCtrlCurrentAnimSequence( pAnmCtrl );
    const u16                   seqIdx   = NNS_G2dGetAnimSequenceIndex( pAnimBank, pAnmSeq );
    
    const NNSG2dCellBoundingRectS16* pRect = NNS_G2dGetCellBoundingRect( pCD );
    
    rectPos.x = (s16)(pNodeData->posX + MY_MC_BASE_POS_X);
    rectPos.y = (s16)(pNodeData->posY + MY_MC_BASE_POS_Y);
    
    // �Փ˔��菈�����s���ׂ��Z���A�j�����H
    // 
    // ../data/win_pnl_NANR_LBLDEFS.h �ɋL�q����Ă���A�j���[�V�����V�[�P���X�ԍ�
    // �� define �𗘗p���Ă��܂��B
    // ���̃f�[�^���o�͂���ɂ́A�R���o�[�^�� -lbl �I�v�V�������w�肵�܂��B
    if( seqIdx != NANR_win_pnl_Win_bse )
    {
        if( IsRectIntersect_( pRect, &pointerPos_, &rectPos ) )
        {
            // ���[�U�p�����[�^�Ƃ��āA�t���O�ւ̃|�C���^���n����Ă��܂�
            // �����ł́A�t���O�̍X�V���s���܂��B
            BOOL* pHitFlag = (BOOL*)userParamater;
            OS_Printf("hit index = %d\n", seqIdx );
            *pHitFlag = TRUE;
            
            // �Z���A�j���̉摜�������ꂽ��Ԃɐݒ肵�܂�
            NNS_G2dSetCellAnimationCurrentFrame( pCellAnim, APP_UI_ANIMFRAME_IDX_PUSHED );            
            
            
            // �ȍ~�̃R�[���o�b�N�Ăяo���𒆎~����B
            return FALSE;            
        }else{
            // �Z���A�j���̉摜��ʏ�̏�Ԃɐݒ肵�܂�
            NNS_G2dSetCellAnimationCurrentFrame( pCellAnim, APP_UI_ANIMFRAME_IDX_NORMAL );
        }
    }
    
    return TRUE;
}

/*---------------------------------------------------------------------------*
  Name:         LoadResources

  Description:  �}���`�Z���A�j���[�V�����̍\�z�ƃL�����N�^�f�[�^�A
                �p���b�g�f�[�^�̓ǂݍ��݂��s���܂��B

  Arguments:    ppMCAnim:   �\�z�����}���`�Z���A�j���[�V�����ւ̃|�C���^��
                            �󂯎��|�C���^�B

  Returns:      �Ȃ��B
 *---------------------------------------------------------------------------*/
static void LoadResources()
{
    void* pBuf;

    //
    // MultiCell �֘A������
    //
    {
        
        // �Z���f�[�^�A�Z���A�j���[�V�����A�}���`�Z���f�[�^�A
        // �}���`�Z���A�j���[�V���������[�h�B
        // �����͍Ō�܂Ń��C����������Ŏg�p����̂� pBuf ��������܂���B
        pBuf = G2DDemo_LoadNCER( &pCellBank, "data/win_pnl.NCER" );
        SDK_NULL_ASSERT( pBuf );
        pBuf = G2DDemo_LoadNANR( &pAnimBank, "data/win_pnl.NANR" );
        SDK_NULL_ASSERT( pBuf );
        pBuf = G2DDemo_LoadNMCR( &pMCBank, "data/win_pnl.NMCR" );
        SDK_NULL_ASSERT( pBuf );
        pBuf = G2DDemo_LoadNMAR( &pMCABank, "data/win_pnl.NMAR" );
        SDK_NULL_ASSERT( pBuf );

        //
        // �}���`�Z���A�j���[�V�����̎��̂����������܂�
        // �قȂ������2�̎��̂����������܂��B
        //
        InitMultiCellInstance_( &mcAnim,
                                pAnimBank,
                                pCellBank,
                                pMCBank,
                                pMCABank,
                                NNS_G2D_MCTYPE_SHARE_CELLANIM  );
    }

    //
    // VRAM �֘A�̏�����
    //
    {
        // Loading NCG (Character Data)
        {
            NNSG2dCharacterData*        pCharData = NULL;

            pBuf = G2DDemo_LoadNCGR( &pCharData, "data/win_pnl.NCGR" );
            SDK_NULL_ASSERT( pBuf );

            // Loading For 2D Graphics Engine.
            DC_FlushRange( pCharData->pRawData, pCharData->szByte );
            GX_LoadOBJ( (void*)pCharData->pRawData, 0, pCharData->szByte );

            // �L�����N�^�f�[�^�� VRAM �ɃR�s�[�����̂�
            // ���� pBuf �͊J�����܂��B�ȉ������B
            G2DDemo_Free( pBuf );
        }

        // Loading NCL (Palette Data)
        {
            NNSG2dPaletteData*        pPltData = NULL;

            pBuf = G2DDemo_LoadNCLR( &pPltData, "data/win_pnl.NCLR" );
            SDK_NULL_ASSERT( pBuf );

            // Loading For 2D Graphics Engine.
            DC_FlushRange( pPltData->pRawData, pPltData->szByte );
            GX_LoadOBJPltt( (void*)pPltData->pRawData, 0, pPltData->szByte );

            G2DDemo_Free( pBuf );
        }
    }
}


//--------------------------------------------------------------------------------------------------------
// �A�v���P�[�V�����̕`��
static void AppDraw
( 
    const NNSG2dMultiCellAnimation* pMcAnim, 
    NNSG2dOamManagerInstance*       pOamMgr 
)
{
    u16                             numOamDrawn = 0;                  
    // �|�C���^�̕`��
    {
        const NNSG2dCellData* pCD = NNS_G2dGetCellDataByIdx( pCellBank, 0xd );
        NNSG2dFVec2           pointerPos; 
        
        pointerPos.x = pointerPos_.x << FX32_SHIFT;
        pointerPos.y = pointerPos_.y << FX32_SHIFT;
        
        //
        // MultiCell �Ɠ����� OBJ���X�g���e���|�����ɏ����o���܂�
        //
        numOamDrawn = NNS_G2dMakeCellToOams(
                            tempOamBuffer,              // �o�͐�Oam�o�b�t�@
                            (u16)(NUM_OF_OAM - numOamDrawn),   // �o�͐�o�b�t�@��
                            pCD,                        // �o�͑Ώ� Cell
                            NULL,                       // Affine �ϊ�
                            &pointerPos,                // �I�t�Z�b�g�ʒu
                            NULL,                       // Affine Index
                            FALSE );                    // �{�pAffine���H

        SDK_ASSERT( numOamDrawn < NUM_OF_OAM );

        // OAM�}�l�[�W����OBJ���X�g��o�^���܂�
        (void)NNS_G2dEntryOamManagerOam( pOamMgr, tempOamBuffer, numOamDrawn );
    }
    
    // �}���`�Z���`��
    //
    // �჌�x���ȕ`����@�� MultiCell ��`�悵�܂��B
    // ��ʃ��W���[���� G2dRenderer �� ���p����`����@������܂��B
    //
    //
    {
        
        
        const NNSG2dMultiCellInstance*  pMultiCell = &(pMcAnim->multiCellInstance); // �\������ MultiCell
        SDK_NULL_ASSERT( pMultiCell );

        //
        // MultiCell �Ɠ����� OBJ���X�g���e���|�����ɏ����o���܂�
        //
        numOamDrawn = NNS_G2dMakeSimpleMultiCellToOams(
                            tempOamBuffer,       // �o�͐�Oam�o�b�t�@
                            NUM_OF_OAM,          // �o�͐�o�b�t�@��
                            pMultiCell,          // �o�͑Ώ� MultiCell
                            NULL,                // Affine �ϊ�
                            &MULTICELL_POSITION, // �I�t�Z�b�g�ʒu
                            NULL,                // Affine Index
                            FALSE );             // �{�pAffine���H

        SDK_ASSERT( numOamDrawn < NUM_OF_OAM );

        // OAM�}�l�[�W����OBJ���X�g��o�^���܂�
        (void)NNS_G2dEntryOamManagerOam( pOamMgr, tempOamBuffer, numOamDrawn );
    }
    
    
}



   

/*---------------------------------------------------------------------------*
  Name:         NitroMain

  Description:  ���C���֐��ł��B

  Arguments:    �Ȃ��B

  Returns:      �Ȃ��B
 *---------------------------------------------------------------------------*/
void NitroMain()
{
    NNSG2dOamManagerInstance    oamManager;     // Oam �}�l�[�W������
    
        
    // Initialize App.
    {
        G2DDemo_CommonInit();
        G2DDemo_PrintInit();
        InitOamManager( &oamManager );
        LoadResources();
        
        // start display
        {
            SVC_WaitVBlankIntr();
            GX_DispOn();
            GXS_DispOn();
        }
    }

    //----------------------------------------------------
    // Main loop
    while( TRUE )
    {
        NNSG2dMultiCellAnimation*      pMcAnim      = &mcAnim;
        
        //
        //  ------ Pad handling.
        //
        {
            G2DDemo_ReadGamePad();

            // change animation speed.
            {
                if (G2DDEMO_IS_PRESS(PAD_KEY_UP))
                {
                    pointerPos_.y -= 1;
                }
                
                if (G2DDEMO_IS_PRESS(PAD_KEY_DOWN))
                {
                    pointerPos_.y += 1;
                }
                
                if (G2DDEMO_IS_PRESS(PAD_KEY_LEFT))
                {
                    pointerPos_.x -= 1;
                }
                
                if (G2DDEMO_IS_PRESS(PAD_KEY_RIGHT))
                {
                    pointerPos_.x += 1;
                }
            }
        }
        
        //  ------ �Փ˔���
        // �}���`�Z�����̊e�m�[�h�ɑ΂��āA�Փ˔���ƁA
        // �Փˎ��̃��A�N�V�����������s���R�[���o�b�N�Ăяo�����s���܂��B
        // ���[�U�p�����[�^�Ƃ��āA�t���O�ւ̃|�C���^��n���A�R�[���o�b�N�����ŁA�Փ˂��N�������ꍇ��
        // �t���O���X�V���܂��B
        bHit_       = FALSE;
        NNS_G2dTraverseMCNodes( &pMcAnim->multiCellInstance, OnButtonHitCallBack_, (u32)&bHit_ );

        //  ------ �`��
        AppDraw( pMcAnim, &oamManager );
        
        
        
        // ------ �f�o�b�N�������`�悵�܂�
        {
            if( bHit_ )
            {
                G2DDemo_PrintOutf(0, 0, "Hit");
            }else{
                G2DDemo_PrintOutf(0, 0, "   ");
            }
        }

        // ------ V Blank �҂�
        SVC_WaitVBlankIntr();
        G2DDemo_PrintApplyToHW();   // �f�o�b�N���������HW�ɏ����o���܂�

        // ------ �}�l�[�W���̓��e��HW�ɏ����o���܂�
        {
            NNS_G2dApplyOamManagerToHW          ( &oamManager );
            NNS_G2dResetOamManagerBuffer        ( &oamManager );
        }
    }
}


/*---------------------------------------------------------------------------*
  Name:         VBlankIntr

  Description:  V�u�����N���荞�݂��������܂��B

  Arguments:    �Ȃ��B

  Returns:      �Ȃ��B
 *---------------------------------------------------------------------------*/
void VBlankIntr(void)
{
    OS_SetIrqCheckFlag( OS_IE_V_BLANK );                   // checking VBlank interrupt
}

